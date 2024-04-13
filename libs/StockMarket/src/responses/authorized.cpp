#include "authorized.hpp"

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
        service::AuthService::errorCode authErrorCode{};
        const bool authenticated = service::AuthService::authenticateUser(
            _requestParams, 
            authErrorCode);

        if (!authenticated)
            return BadRequestResponse{ 
                _handler->getRequestInfo(), 
                messages::errors::INVALID_AUTH }.create();

        return _handler->response();
    }
}