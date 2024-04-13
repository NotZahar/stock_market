#include "bad_request.hpp"

#include "../../utility/net_helper.hpp"
#include "../../utility/config.hpp"

namespace sm {
    BadRequestResponse::BadRequestResponse(
        RequestInfo request,
        std::string_view reason) noexcept
        : Response(request),
          _reason(reason)
    {}

    http::message_generator BadRequestResponse::create() const {
        using namespace netHelper;
        
        http::response<http::string_body> response{ 
            http::status::bad_request, 
            _request.httpVersion 
        };

        response.set(http::field::server, config::fieldServer);
        response.set(
            http::field::content_type, 
            inUTF8(MIMEType.left.find(MIME::text_html)->second)
        );

        response.keep_alive(_request.keepAlive);
        response.body() = _reason;
        response.prepare_payload();

        return response;
    }
}