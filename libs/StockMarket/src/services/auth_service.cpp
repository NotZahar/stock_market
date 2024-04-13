#include "auth_service.hpp"

#include "services/user_service.hpp"
#include "../utility/uuid.hpp"
#include "../utility/db.hpp"
#include "../utility/url.hpp"

namespace sm::service {
    void AuthService::registerUser(RegisterUserData registerData, errorCode& eCode) noexcept {
        const auto email = registerData.email;
        constexpr int INITIAL_BALANCE = 0;

        if (!db::sanitizer::isValid(email)) {
            eCode = errorCode::badData;
            return;
        }

        UserService::errorCode createError{};
        UserService::create(
            UserService::User{ 
                uuid::generateUUID(),
                email,
                INITIAL_BALANCE 
            }, 
            createError);
        
        if (createError != UserService::errorCode::noError) {
            eCode = errorCode::badData;
            return;
        }
    }

    std::string AuthService::loginUser(LoginUserData loginData, errorCode& eCode) noexcept {
        const auto email = loginData.email;

        if (!db::sanitizer::isValid(email)) {
            eCode = errorCode::badData;
            return {};
        }

        UserService::errorCode loginError{};
        const auto userId = UserService::getId(email, loginError);

        if (loginError != UserService::errorCode::noError) {
            eCode = errorCode::badData;
            return {};
        }

        return userId;
    }

    bool AuthService::authenticateUser(
            const std::unordered_map<std::string, std::string>& params, 
            errorCode& eCode) noexcept {
        if (!url::paramsExists(params, { urlParam::user })) {
            eCode = errorCode::noData;
            return false;
        }

        const auto userId = params.at(urlParams.left.find(urlParam::user)->second);
        if (!db::sanitizer::isValid(userId)) {
            eCode = errorCode::badData;
            return false;
        }

        UserService::errorCode authError{};
        const bool userExists = UserService::exists(userId, authError);

        if (authError != UserService::errorCode::noError) {
            eCode = errorCode::badData;
            return false;
        }

        return userExists;
    }
}