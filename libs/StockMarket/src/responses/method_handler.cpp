#include "method_handler.hpp"

#include "error/bad_request.hpp"
#include "utility/messages.hpp"

namespace sm {
    MethodHandler::MethodHandler(
        const http::verb requestMethod, std::unique_ptr<Response> response) noexcept 
        : _requestMethod(requestMethod),
          _response(std::move(response))
    {}

    http::message_generator MethodHandler::response() const {
        if (!methodIsValid()) {
            return BadRequestResponse{ 
                _response->getRequestInfo(), 
                messages::errors::INVALID_METHOD 
            }.create();
        }

        return _response->create();
    }

    Response::RequestInfo MethodHandler::getRequestInfo() const noexcept {
        return _response->getRequestInfo();
    }  
}