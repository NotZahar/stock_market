#pragma once

#include <string_view>

namespace sm::service {
    /*!
        \brief Модуль аутентификации
    */
    class AuthService {
    public:
        enum class errorCode {
            noError,
            noData,
            badData
        };

        struct RegisterUserData {
            std::string_view email;
        };

        struct LoginUserData {
            std::string_view email;
        };

        AuthService() = default;

        ~AuthService() = default;

        static void registerUser(RegisterUserData registerData, errorCode& eCode);
        static bool loginUser(LoginUserData loginData, errorCode& eCode);
    };
}