#pragma once

#include <string>
#include <string_view>

namespace sm::service {
    class MarketService {
    public:
        enum class errorCode {
            noError,
            badData
        };

        MarketService() = default;

        ~MarketService() = default;
        
        static void purchase(
            std::string_view userId, 
            std::string_view rawPurchaseData, 
            errorCode& eCode) noexcept;
        static void sell(
            std::string_view userId, 
            std::string_view rawSellData, 
            errorCode& eCode) noexcept;

    private:
        static const std::string _VOLUME_KEY;
        static const std::string _PRICE_KEY;
    };
}