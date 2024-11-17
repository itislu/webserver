#include "RequestHandlerManager.hpp"
#include "GetHandler.hpp"
#include "HttpException.hpp"

namespace webkernel
{

RequestHandlerManager::RequestHandlerManager()
{
    _handlers[webshell::GET] = new GetHandler();
}

RequestHandlerManager::~RequestHandlerManager()
{
    for (std::map<webshell::RequestMethod, ARequestHandler*>::iterator it =
             _handlers.begin();
         it != _handlers.end(); ++it)
    {
        delete it->second;
    }
}

RequestHandlerManager& RequestHandlerManager::getInstance()
{
    static RequestHandlerManager instance;
    return instance;
}

webshell::Response
RequestHandlerManager::handleRequest(webshell::RequestMethod method,
                                     const webconfig::RequestConfig& config,
                                     const webshell::Request& request)
{
    std::map<webshell::RequestMethod, ARequestHandler*>::iterator it =
        _handlers.find(method);
    if (it != _handlers.end())
    {
        return it->second->handle(config, request);
    }
    throw utils::HttpException(webshell::NOT_IMPLEMENTED,
                               "Method not implemented");
}

} // namespace webkernel
