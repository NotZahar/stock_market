#include "balance.hpp"

#include "../error/bad_request.hpp"
#include "services/user_service.hpp"
#include "utility/messages.hpp"
#include "utility/net_helper.hpp"
#include "utility/config.hpp"

namespace sm {
    BalanceResponse::BalanceResponse(
        RequestInfo request, 
        std::string_view userId)
        : Response(request),
          _userId(userId)
    {}

    http::message_generator BalanceResponse::create() const {
        using namespace netHelper;
        using namespace service;

        UserService::errorCode userError{};
        const std::string balance = UserService::getBalance(_userId, userError);
        if (userError != UserService::errorCode::noError)
            return BadRequestResponse{ _request, messages::errors::INTERNAL_ERROR_GENERAL }.create();

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
        response.body() = balance;
        response.prepare_payload();

        return response;
    }
}