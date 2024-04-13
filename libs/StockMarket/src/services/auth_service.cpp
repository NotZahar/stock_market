#include "auth_service.hpp"

#include "services/user_service.hpp"
#include "../utility/uuid.hpp"
#include "../utility/db.hpp"

namespace sm::service {
    void AuthService::registerUser(RegisterUserData registerData, errorCode& eCode) {
        const auto email = registerData.email;
        constexpr int INITIAL_BALANCE = 0;

        if (!db::sanitizer::check(email)) {
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

    bool AuthService::loginUser(LoginUserData loginData, errorCode& eCode) {
        const auto email = loginData.email;

        if (!db::sanitizer::check(email)) {
            eCode = errorCode::badData;
            return false;
        }

        UserService::errorCode loginError{};
        const bool userExists = UserService::exists(email, loginError);
        
        if (loginError != UserService::errorCode::noError) {
            eCode = errorCode::badData;
            return false;
        }

        return userExists;
    }
}