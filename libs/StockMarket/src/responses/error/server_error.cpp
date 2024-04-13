#include "server_error.hpp"

#include "../../utility/net_helper.hpp"
#include "../../utility/config.hpp"
#include "../../utility/messages.hpp"

namespace sm {
    ServerErrorResponse::ServerErrorResponse(
        RequestInfo request,
        std::string_view what) noexcept
        : Response(request),
          _what(what)
    {}

    http::message_generator ServerErrorResponse::create() const {
        using namespace netHelper;

        http::response<http::string_body> response{
            http::status::internal_server_error, 
            _request.httpVersion
        };
        
        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            inUTF8(MIMEType.left.find(MIME::text_html)->second)
        );
        
        response.keep_alive(_request.keepAlive);
        response.body() = messages::errors::INTERNAL_ERROR + _what;
        response.prepare_payload();
        
        return response;
    }
}