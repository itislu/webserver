#include "GetHandler.hpp"
#include "HttpException.hpp"
#include "Logger.hpp"
#include "ResponseBuilder.hpp"
#include "defines.hpp"
#include "utils.hpp"
#include <iosfwd>
#include <string>

namespace webkernel
{

webshell::Response GetHandler::handle(int fd, EventProcessingState& state,
                                      const webconfig::RequestConfig& config,
                                      const webshell::Request& request)
{
    if (state == INITIAL)
    {
        _target_path = _preProcess(config, request);
        state = PROCESSING;
    }
    weblog::Logger::log(weblog::DEBUG, "GetHandler handle: " + _target_path);
    std::string content;

    if (access(_target_path.c_str(), R_OK) == -1)
        throw utils::HttpException(webshell::FORBIDDEN,
                                   "Forbidden no read permission on file: " +
                                       _target_path);
    if (!config.redirect.empty())
        throw utils::HttpException(webshell::MOVED_PERMANENTLY,
                                   "Redirect to " + config.redirect);
    if (utils::isDirectory(_target_path) && !config.autoindex)
        throw utils::HttpException(webshell::FORBIDDEN,
                                   "Forbidden autoindex is disabled");
    if (_get_respones_encoding(config, request) & webkernel::CHUNKED)
        _handle_chunked(fd, state, _target_path, content);
    else
        _handle_standard(state, _target_path, content);

    // prepare headers in post process and encode the content if needed e.g.
    // gzip
    _postProcess(config, request, _target_path, content);
    return (webshell::ResponseBuilder::buildResponse(
        webshell::OK, _response_headers, content,
        state & WRITE_OTHERS_CHUNKED));
}

void GetHandler::_handle_standard(EventProcessingState& state,
                                  const std::string& target_path,
                                  std::string& content) const
{

    std::ifstream file(target_path.c_str(), std::ios::binary);

    if (!file.is_open())
        throw utils::HttpException(webshell::INTERNAL_SERVER_ERROR,
                                   "Open file failed");
    content = std::string((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    file.close();
    state = COMPELETED;
}

void GetHandler::_handle_chunked(int fd, EventProcessingState& state,
                                 const std::string& target_path,
                                 std::string& content)
{
    std::ifstream file(target_path.c_str(), std::ios::binary);

    if (!file.is_open())
        throw utils::HttpException(webshell::INTERNAL_SERVER_ERROR,
                                   "Open file failed");
    state = WRITE_CHUNKED;
    std::streampos& file_offset = _chunked_file_records[fd];
    if (file_offset == 0)
        state = static_cast<EventProcessingState>(state | WRITE_FIRST_CHUNKED);
    else
        state = static_cast<EventProcessingState>(state | WRITE_OTHERS_CHUNKED);
    file.seekg(file_offset);
    content.resize(CHUNKED_SIZE);
    file.read(&content[0], CHUNKED_SIZE);
    std::streamsize bytes_read = file.gcount();
    content.resize(bytes_read);
    content = _chunked_codec.encode(content);
    if (file.eof())
    {
        _chunked_file_records.erase(fd);
        content += "0\r\n\r\n";
        state = static_cast<EventProcessingState>(state | COMPELETED);
    }
    else if (file.tellg() == -1)
    {
        _chunked_file_records.erase(fd);
        throw utils::HttpException(webshell::INTERNAL_SERVER_ERROR,
                                   "file.tellg() failed");
    }
    else
        file_offset = file.tellg();
    file.close();
}

} // namespace webkernel
