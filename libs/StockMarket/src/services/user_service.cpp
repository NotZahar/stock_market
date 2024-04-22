#include "user_service.hpp"

#include <cassert>
#include <iterator>

#include "db_service.hpp"
#include "../utility/db.hpp"

namespace sm::service {
    const std::string UserService::_createUserQuery = 
        R"(
            INSERT INTO 
                user(id, email, balance)
            VALUES(?, ?, ?);
        )";

    const std::string UserService::_getUserEmailQuery = 
        R"(
            SELECT 
                id, email, balance
            FROM 
                user
            WHERE
                email IS ?
        )";
    
    const std::string UserService::_getUserIdQuery = 
        R"(
            SELECT 
                id, email, balance
            FROM 
                user
            WHERE
                id IS ?
        )";

    const std::string UserService::_decreaseBalanceQuery = 
        R"(
            UPDATE 
                user
            SET
                balance = balance - ?
            WHERE
                id IS ?
        )";

    const std::string UserService::_increaseBalanceQuery = 
        R"(
            UPDATE 
                user
            SET
                balance = balance + ?
            WHERE
                id IS ?
        )";

    std::string UserService::getBalance(std::string_view userId, errorCode& eCode) noexcept {
        if (!db::sanitizer::isValid(userId)) {
            eCode = errorCode::badData;
            return {};
        }
        
        DBService::errorCode selectError{};
        std::vector<std::string> values;
        constexpr int SCHEMA_SIZE = 3;
        
        values.emplace_back(userId);
        
        const auto users = DBService::select(
            _getUserIdQuery, 
            std::move(values),
            SCHEMA_SIZE,
            selectError
        );
        
        if (selectError != DBService::errorCode::noError) {
            eCode = errorCode::badData;
            return {};
        }

        if (users.empty())
            return {};

        assert(users.size() == 1);
        auto rawUser = users.front();
        auto rawUserIt = rawUser.cbegin();
        std::advance(rawUserIt, 2);
        
        const std::string dbBalance = *rawUserIt;
        return dbBalance;
    }

    void UserService::decreaseBalance(
            std::string_view userId, 
            const int difference, 
            errorCode& eCode) noexcept {
        DBService::errorCode updateError{};
        std::vector<std::string> values;
        
        values.emplace_back(std::to_string(difference));
        values.emplace_back(userId);
        
        const int changes = DBService::update(
            _decreaseBalanceQuery,
            std::move(values),
            updateError
        );
        
        if (updateError != DBService::errorCode::noError) {
            eCode = errorCode::internalError;
            return;
        }

        if (changes == DBService::ZERO_CHANGES) {
            eCode = errorCode::internalError;
            return;
        }
    }

    void UserService::increaseBalance(
            std::string_view userId, 
            const int difference, 
            errorCode& eCode) noexcept {
        DBService::errorCode updateError{};
        std::vector<std::string> values;
     
        values.emplace_back(std::to_string(difference));
        values.emplace_back(userId);
        
        const int changes = DBService::update(
            _increaseBalanceQuery,
            std::move(values),
            updateError
        );
        
        if (updateError != DBService::errorCode::noError) {
            eCode = errorCode::internalError;
            return;
        }

        if (changes == DBService::ZERO_CHANGES) {
            eCode = errorCode::internalError;
            return;
        }
    }

    void UserService::create(User user, errorCode& error) noexcept {
        DBService::errorCode insertError{};
        std::vector<std::string> values;
        
        values.emplace_back(user.id);
        values.emplace_back(user.email);
        values.emplace_back(std::to_string(user.balance));
        
        const int result = DBService::insert(
            _createUserQuery,
            std::move(values),
            insertError
        );
        
        if (insertError != DBService::errorCode::noError) {
            error = errorCode::badData;
            return;
        }

        if (result == DBService::ZERO_CHANGES) {
            error = errorCode::alreadyExists;
            return;
        }
    }

    std::string UserService::getId(std::string_view email, errorCode& eCode) noexcept {
        DBService::errorCode selectError{};
        std::vector<std::string> values;
        constexpr int SCHEMA_SIZE = 3;
        
        values.emplace_back(email);
        
        const auto users = DBService::select(
            _getUserEmailQuery, 
            std::move(values),
            SCHEMA_SIZE,
            selectError
        );
        
        if (selectError != DBService::errorCode::noError) {
            eCode = errorCode::badData;
            return {};
        }

        if (users.empty())
            return {};

        assert(users.size() == 1);
        auto rawUser = users.front();
        auto rawUserIt = rawUser.cbegin();
        
        const std::string dbId = *rawUserIt;
        return dbId;
    }

    bool UserService::exists(std::string_view userId, errorCode& eCode) noexcept {
        DBService::errorCode selectError{};
        std::vector<std::string> values;
        constexpr int SCHEMA_SIZE = 3;
        
        values.emplace_back(userId);
        
        const auto users = DBService::select(
            _getUserIdQuery, 
            std::move(values),
            SCHEMA_SIZE,
            selectError
        );
        
        if (selectError != DBService::errorCode::noError) {
            eCode = errorCode::badData;
            return false;
        }

        if (users.empty())
            return false;

        return true;
    }
}