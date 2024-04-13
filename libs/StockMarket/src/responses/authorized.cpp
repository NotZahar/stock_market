#include "authorized.hpp"

#include "login.hpp"
#include "error/bad_request.hpp"
#include "../services/auth_service.hpp"
#include "../utility/messages.hpp"

namespace sm {
    Authorized::Authorized(
        const std::unordered_map<std::string, std::string>& requestParams, 
        std::unique_ptr<MethodHandler> handler) noexcept 
        : _requestParams(requestParams),
          _handler(std::move(handler))
    {}

    http::message_generator Authorized::response() const {
        AuthService::errorCode authErrorCode{};
        AuthService::authenticate(_requestParams, authErrorCode);
        switch (authErrorCode) {
            case AuthService::errorCode::noData:
                return LoginResponse{ _handler->getRequestInfo() }.create();
            case AuthService::errorCode::badData:
                return BadRequestResponse{ 
                    _handler->getRequestInfo(), 
                    messages::errors::INVALID_AUTH 
                }.create();
            default:
                return _handler->response();        
        }
    }
}