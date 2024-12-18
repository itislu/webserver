#pragma once
#include "Uri.hpp"
#include "defines.hpp"
#include <map>
#include <string>

namespace webshell
{

class Request
{
  public:
    Request();
    Request(const Request&);
    Request& operator=(const Request&);
    ~Request();

    const RequestMethod& method() const;
    Uri uri() const;
    float version() const;
    const std::map<std::string, std::string>& headers() const;
    const std::string& header(const std::string& name) const;
    const std::string& body() const;
    const std::string serialize() const;

    void setMethod(RequestMethod method);
    void setUri(Uri uri);
    void setVersion(float version);
    void setHeaders(std::map<std::string, std::string> headers);
    void setBody(std::string& body);
    void addHeader(std::string& name, std::string& value);

  private:
    RequestMethod _method;
    Uri _uri;
    float _version;
    std::map<std::string, std::string> _headers;
    std::string _body;
};

} // namespace webshell
