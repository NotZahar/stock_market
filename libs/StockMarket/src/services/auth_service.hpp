#pragma once

#include <string_view>
#include <unordered_map>

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

        static void registerUser(RegisterUserData registerData, errorCode& eCode) noexcept;
        static std::string loginUser(LoginUserData loginData, errorCode& eCode) noexcept;
        static bool authenticateUser(
            const std::unordered_map<std::string, std::string>& params, 
            errorCode& eCode) noexcept;
    };
}