#include "market_service.hpp"

#include <exception>

#include <json.hpp>

#include "logger.hpp"
#include "services/request_service.hpp"
#include "../utility/uuid.hpp"

namespace sm::service {
    const std::string MarketService::_VOLUME_KEY = "volume";
    const std::string MarketService::_PRICE_KEY = "price";

    void MarketService::purchase(
            std::string_view userId, 
            std::string_view rawPurchaseData, 
            errorCode& eCode) noexcept {
        constexpr int WRONG_VALUE = -1;
        int volume = WRONG_VALUE;
        int price = WRONG_VALUE;
        
        nlohmann::json purchaseData;
        try {
            purchaseData = nlohmann::json::parse(rawPurchaseData);
            volume = purchaseData[_VOLUME_KEY].get<int>();
            price = purchaseData[_PRICE_KEY].get<int>();
        } catch (const std::exception& error) {
            Logger::instance().err(error.what());
            eCode = errorCode::badData;
            return;
        }

        if (volume < 0 || price < 0) {
            eCode = errorCode::badData;
            return;
        }

        RequestService::errorCode createError{};
        RequestService::create(
            RequestService::Request{ 
                uuid::generateUUID(),
                volume,
                price,
                RequestService::requestSide::purchase,
                RequestService::requestStatus::active,
                userId
            }, 
            createError);
        
        if (createError != RequestService::errorCode::noError) {
            eCode = errorCode::badData;
            return;
        }
    }

    void MarketService::sell(
            std::string_view userId, 
            std::string_view rawSellData, 
            errorCode& eCode) noexcept {
        constexpr int WRONG_VALUE = -1;
        int volume = WRONG_VALUE;
        int price = WRONG_VALUE;
        
        nlohmann::json sellData;
        try {
            sellData = nlohmann::json::parse(rawSellData);
            volume = sellData[_VOLUME_KEY].get<int>();
            price = sellData[_PRICE_KEY].get<int>();
        } catch (const std::exception& error) {
            Logger::instance().err(error.what());
            eCode = errorCode::badData;
            return;
        }

        if (volume < 0 || price < 0) {
            eCode = errorCode::badData;
            return;
        }

        RequestService::errorCode createError{};
        RequestService::create(
            RequestService::Request{ 
                uuid::generateUUID(),
                volume,
                price,
                RequestService::requestSide::sell,
                RequestService::requestStatus::active,
                userId
            }, 
            createError);
        
        if (createError != RequestService::errorCode::noError) {
            eCode = errorCode::badData;
            return;
        }
    }
}