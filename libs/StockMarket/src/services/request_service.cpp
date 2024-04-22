#include "request_service.hpp"

#include <cassert>
#include <string>

#include "db_service.hpp"
#include "services/user_service.hpp"
#include "utility/time.hpp"

namespace sm::service {
    const std::string RequestService::_createRequestQuery = 
        R"(
            INSERT INTO 
                request(id, volume, price, deal_time, registration_time, side, status, client)
            VALUES(?, ?, ?, ?, ?, ?, ?, ?);
        )";

    const std::string RequestService::_getActiveSellsQuery = 
        R"(
            SELECT
                id, 
                volume, 
                price,
                client
            FROM 
                request
            WHERE
                side IS ?
            AND
                status IS ?
            AND
                price <= ?
            AND 
                registration_time <= ?
            ORDER BY
                price ASC,
                registration_time ASC
        )";

    const std::string RequestService::_getRequestByIdQuery = 
        R"(
            SELECT
                volume, 
                price,
                deal_time,
                registration_time,
                side,
                status,
                client
            FROM 
                request
            WHERE
                id IS ?
        )";

    // const std::string RequestService::_finishRequestQuery = 
    //     R"(
    //         UPDATE 
    //             request
    //         SET
    //             volume = 0,
    //             deal_time = ?, 
    //             status = ?
    //         WHERE
    //             id IS ?
    //     )";

    const std::string RequestService::_decreaseVolumeQuery = 
        R"(
            UPDATE 
                request
            SET
                volume = volume - ?
            WHERE
                id IS ?
        )";

    std::string RequestService::create(Request request, errorCode& eCode) noexcept {
        DBService::errorCode insertError{};
        std::vector<std::string> values;
        const std::string createTime = time::now();

        values.emplace_back(request.id);
        values.emplace_back(std::to_string(request.volume));
        values.emplace_back(std::to_string(request.price));
        values.emplace_back(DBService::NULL_VALUE);
        values.emplace_back(createTime);
        values.emplace_back(requestSides.left.find(request.side)->second);
        values.emplace_back(requestStatuses.left.find(request.status)->second);
        values.emplace_back(request.client_id);
        
        const int result = DBService::insert(
            _createRequestQuery,
            std::move(values),
            insertError
        );
        
        if (insertError != DBService::errorCode::noError) {
            eCode = errorCode::badData;
            return {};
        }

        if (result == DBService::ZERO_CHANGES) {
            eCode = errorCode::internalError;
            return {};
        }

        return createTime;
    }

    std::list<std::list<std::string>> RequestService::getActiveSells(
            const int price,
            std::string_view registrationTime,
            errorCode& eCode) noexcept {
        DBService::errorCode selectError{};
        std::vector<std::string> values;
        constexpr int SCHEMA_SIZE = 4;
        
        values.emplace_back(requestSides.left.find(requestSide::sell)->second);
        values.emplace_back(requestStatuses.left.find(requestStatus::active)->second);
        values.emplace_back(std::to_string(price));
        values.emplace_back(registrationTime);
        
        const auto sells = DBService::select(
            _getActiveSellsQuery, 
            std::move(values),
            SCHEMA_SIZE,
            selectError
        );
        
        if (selectError != DBService::errorCode::noError) {
            eCode = errorCode::internalError;
            return {};
        }

        return sells;
    }

    std::list<std::string> RequestService::getRequestById(
            std::string_view id, 
            errorCode& eCode) noexcept {
        DBService::errorCode selectError{};
        std::vector<std::string> values;
        constexpr int SCHEMA_SIZE = 7;
        
        values.emplace_back(id);
        
        const auto requests = DBService::select(
            _getRequestByIdQuery, 
            std::move(values),
            SCHEMA_SIZE,
            selectError
        );
        
        if (selectError != DBService::errorCode::noError) {
            eCode = errorCode::internalError;
            return {};
        }

        assert(requests.size() == 1);
        return requests.front();
    }

    // void RequestService::finishSell(
    //         std::string_view sellId,
    //         std::string_view sellerId, 
    //         const int increasingBalance) noexcept {
    //     DBService::errorCode finishSellError{};
    //     std::vector<std::string> values;
        
    //     values.emplace_back(time::now());
    //     values.emplace_back(requestStatuses.left.find(requestStatus::closed)->second);
    //     values.emplace_back(sellId);
        
    //     const int changes = DBService::update(
    //         _finishRequestQuery,
    //         std::move(values),
    //         finishSellError
    //     );
        
    //     assert(finishSellError == DBService::errorCode::noError);
    //     assert(changes != DBService::ZERO_CHANGES);

    //     UserService::errorCode updateBalanceError{};
    //     UserService::increaseBalance(sellerId, increasingBalance, updateBalanceError);
        
    //     assert(updateBalanceError == UserService::errorCode::noError);
    // }

    void RequestService::decreaseVolume(std::string_view requestId, const int difference) noexcept {
        DBService::errorCode updateError{};
        std::vector<std::string> values;
        
        values.emplace_back(std::to_string(difference));
        values.emplace_back(requestId);
        
        const int changes = DBService::update(
            _decreaseVolumeQuery,
            std::move(values),
            updateError
        );
        
        assert(updateError == DBService::errorCode::noError);
        assert(changes != DBService::ZERO_CHANGES);
    }
}