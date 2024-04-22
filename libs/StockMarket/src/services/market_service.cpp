#include "market_service.hpp"

#include <cassert>
#include <exception>
#include <iterator>
#include <string_view>

#include <json.hpp>

#include "logger.hpp"
#include "../tools/request_queue.hpp"
#include "../tools/send_queue.hpp"
#include "services/request_service.hpp"
#include "services/user_service.hpp"
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

        if (volume <= 0 || price <= 0) {
            eCode = errorCode::badData;
            return;
        }

        const std::string newPurchaseId = uuid::generateUUID();
        RequestService::errorCode createError{};
        const std::string registrationTime = RequestService::create(
            RequestService::Request{ 
                newPurchaseId,
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

        tools::RequestQueue::instance().appendTask([
            volume, 
            price, 
            purchaseId = std::move(newPurchaseId), 
            clientId = std::string{ userId },
            creationTime = std::move(registrationTime)]() {
            makePurchase(volume, price, purchaseId, clientId, creationTime);
        });
    }

    void MarketService::sell(
            std::string_view userId, 
            std::string_view rawSellData, 
            errorCode& eCode) noexcept {
        // TODO: [here] change all logic below
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

        const std::string newSellId = uuid::generateUUID();
        RequestService::errorCode createError{};
        const std::string registrationTime = RequestService::create(
            RequestService::Request{ 
                newSellId,
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

    bool MarketService::requestDone(std::string_view requestId, errorCode& eCode) noexcept {
        RequestService::errorCode getTargetPurchaseError{};
        const auto purchaseData = RequestService::getRequestById(requestId, getTargetPurchaseError);

        if (getTargetPurchaseError != RequestService::errorCode::noError) {
            eCode = errorCode::internalError;
            return false;
        }

        auto purchaseDataIt = purchaseData.cbegin();
        
        assert(purchaseData.size() == 7);
        std::advance(purchaseDataIt, 5);
        const std::string rawStatus = *purchaseDataIt;
        
        assert(RequestService::requestStatuses.right.count(rawStatus) > 0);
        const RequestService::requestStatus status = 
            RequestService::requestStatuses.right.find(rawStatus)->second;
        
        return status == RequestService::requestStatus::closed;
    }

    void MarketService::makeDeal(
            std::string_view sellId, 
            std::string_view purchaseId,
            std::string_view sellerId,
            std::string_view purchaserId,
            const int dealVolume,
            const int dealPrice) noexcept {
        const int dealSum = dealVolume * dealPrice; 
        UserService::errorCode decreaseBalanceError{};
        UserService::errorCode increaseBalanceError{};

        RequestService::decreaseVolume(sellId, dealVolume);
        RequestService::decreaseVolume(purchaseId, dealVolume);
        UserService::decreaseBalance(purchaserId, dealSum, decreaseBalanceError);
        UserService::increaseBalance(sellerId, dealSum, increaseBalanceError);

        assert(decreaseBalanceError == UserService::errorCode::noError);
        assert(increaseBalanceError == UserService::errorCode::noError);        
    }

    void MarketService::makePurchase(
            const int targetVolume, 
            const int targetPrice, 
            std::string targetRequestId,
            std::string targetClientId,
            std::string targetRegistrationTime) noexcept {
        errorCode checkError{};
        const bool isDone = requestDone(targetRequestId, checkError);
        if (checkError != errorCode::noError)
            return;
        if (isDone)
            return;

        RequestService::errorCode getSellsError{};
        const auto rawSells = RequestService::getActiveSells(
            targetPrice, 
            targetRegistrationTime, 
            getSellsError);
        if (getSellsError != RequestService::errorCode::noError)
            return;

        int remainingVolume = targetVolume;
        for (auto&& rawSell : rawSells) {
            assert(rawSell.size() == 4);
            auto rawSellIt = rawSell.cbegin();
            
            const std::string sellId = *rawSellIt;

            std::advance(rawSellIt, 1);
            const int volume = std::stoi(*rawSellIt);
            
            std::advance(rawSellIt, 2);
            const std::string sellerId = *rawSellIt;

            if (remainingVolume >= volume) {
                remainingVolume -= volume;
                makeDeal(sellId, targetRequestId, sellerId, targetClientId, volume, targetPrice);
                RequestService::closeRequest(sellId);
                tools::SendQueue::instance().sendRequestDone(sellId);
                if (remainingVolume == 0)
                    break;
                continue;
            }

            if (remainingVolume < volume) {
                makeDeal(sellId, targetRequestId, sellerId, targetClientId, remainingVolume, targetPrice);
                remainingVolume = 0;
                break;
            }
        }

        if (remainingVolume > 0) {
            // TODO: [here]
            // TODO: change balance
            RequestService::setVolume(targetRequestId, remainingVolume);
            return;
        }

        assert(remainingVolume == 0);
        RequestService::finishPurchase(targetRequestId, targetClientId, targetPrice);
        tools::SendQueue::instance().sendRequestDone(targetRequestId);
    }
}