#include "register.hpp"

#include "../error/bad_request.hpp"
#include "utility/messages.hpp"
#include "utility/net_helper.hpp"
#include "utility/config.hpp"

namespace sm {
    RegisterResponse::RegisterResponse(
        RequestInfo request, 
        service::AuthService::RegisterUserData registerData)
        : Response(request),
          _registerData(registerData)
    {}

    http::message_generator RegisterResponse::create() const {
        using namespace netHelper;
        using namespace service;

        AuthService::errorCode authError{};
        AuthService::registerUser(_registerData, authError);
        if (authError != AuthService::errorCode::noError)
            return BadRequestResponse{ _request, messages::errors::INVALID_AUTH_CREATE }.create();

        http::response<http::string_body> response{ 
            http::status::created, 
            _request.httpVersion 
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            inUTF8(MIMEType.left.find(MIME::text_plain)->second)
        );

        response.keep_alive(_request.keepAlive);
        response.body() = messages::success::USER_CREATED;
        response.prepare_payload();

        return response;
    }
}