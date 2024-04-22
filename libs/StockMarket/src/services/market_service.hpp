#pragma once

#include <string>
#include <string_view>

namespace sm::service {
    class MarketService {
    public:
        enum class errorCode {
            noError,
            internalError,
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
        static bool requestDone(std::string_view requestId, errorCode& eCode) noexcept;
        static void makeDeal(
            std::string_view sellId, 
            std::string_view purchaseId,
            std::string_view sellerId,
            std::string_view purchaserId,
            const int dealVolume,
            const int dealPrice) noexcept;
        static void makePurchase(
            const int targetVolume, 
            const int targetPrice,
            std::string targetRequestId,
            std::string clientId,
            std::string registrationTime) noexcept;
        static void makeSell(
            const int targetVolume, 
            const int targetPrice, 
            std::string targetRequestId) noexcept;

        static const std::string _VOLUME_KEY;
        static const std::string _PRICE_KEY;
    };
}