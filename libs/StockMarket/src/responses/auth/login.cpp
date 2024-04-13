#include "login.hpp"

#include "../error/bad_request.hpp"
#include "utility/messages.hpp"
#include "utility/net_helper.hpp"
#include "utility/config.hpp"

namespace sm {
    LoginResponse::LoginResponse(
        RequestInfo request, 
        service::AuthService::LoginUserData loginData)
        : Response(request),
          _loginData(loginData)
    {}

    http::message_generator LoginResponse::create() const {
        using namespace netHelper;
        using namespace service;

        AuthService::errorCode authError{};
        AuthService::loginUser(_loginData, authError);
        if (authError != AuthService::errorCode::noError)
            return BadRequestResponse{ _request, messages::errors::INVALID_AUTH_LOGIN }.create();

        http::response<http::string_body> response{ 
            http::status::ok,
            _request.httpVersion 
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            inUTF8(MIMEType.left.find(MIME::text_plain)->second)
        );

        response.keep_alive(_request.keepAlive);
        response.body() = messages::success::USER_LOGGED;
        response.prepare_payload();

        return response;
    }
}