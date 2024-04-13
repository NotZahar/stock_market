#include "purchase.hpp"

#include "../error/bad_request.hpp"
#include "services/market_service.hpp"
#include "utility/messages.hpp"
#include "utility/net_helper.hpp"
#include "utility/config.hpp"

namespace sm {
    PurchaseResponse::PurchaseResponse(
        RequestInfo request, 
        std::string_view userId,
        std::string_view purchaseData)
        : Response(request),
          _userId(userId),
          _purchaseData(purchaseData)
    {}

    http::message_generator PurchaseResponse::create() const {
        using namespace netHelper;
        using namespace service;

        MarketService::errorCode marketError{};
        MarketService::purchase(_userId, _purchaseData, marketError);
        if (marketError != MarketService::errorCode::noError)
            return BadRequestResponse{ _request, messages::errors::INTERNAL_ERROR_GENERAL }.create();

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
        response.body() = messages::success::PURCHASED;
        response.prepare_payload();

        return response;
    }
}