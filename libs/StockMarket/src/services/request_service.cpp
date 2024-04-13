#include "request_service.hpp"

#include "db_service.hpp"

namespace sm::service {
    const std::string RequestService::_createRequestQuery = 
        R"(
            INSERT INTO 
                request(id, volume, price, deal_time, side, status, client)
            VALUES(?, ?, ?, ?, ?, ?, ?);
        )";

    void RequestService::create(Request request, errorCode& error) noexcept {
        DBService::errorCode insertError{};
        std::vector<std::string> values;
        
        values.emplace_back(request.id);
        values.emplace_back(std::to_string(request.volume));
        values.emplace_back(std::to_string(request.price));
        values.emplace_back(DBService::NULL_VALUE);
        values.emplace_back(requestSides.left.find(request.side)->second);
        values.emplace_back(requestStatuses.left.find(request.status)->second);
        values.emplace_back(request.client_id);
        
        const int result = DBService::insert(
            _createRequestQuery,
            std::move(values),
            insertError
        );
        
        if (insertError != DBService::errorCode::noError) {
            error = errorCode::badData;
            return;
        }

        if (result == DBService::ZERO_CHANGES) {
            error = errorCode::internalError;
            return;
        }
    }
}