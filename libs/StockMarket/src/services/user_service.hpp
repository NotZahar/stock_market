#pragma once

#include <string>
#include <string_view>

namespace sm::service {
    class UserService {
    public:
        enum class errorCode {
            noError,
            alreadyExists,
            internalError,
            badData
        };

        struct User {
            std::string_view id;
            std::string_view email;
            int balance;
        };

        UserService() = default;

        ~UserService() = default;
        
        static std::string getBalance(std::string_view userId, errorCode& eCode) noexcept;
        static void decreaseBalance(
            std::string_view userId, 
            const int difference, 
            errorCode& eCode) noexcept;
        static void increaseBalance(
            std::string_view userId, 
            const int difference, 
            errorCode& eCode) noexcept;
        static void create(User user, errorCode& error) noexcept;
        static std::string getId(std::string_view email, errorCode& eCode) noexcept;
        static bool exists(std::string_view userId, errorCode& eCode) noexcept;

        inline static constexpr int INITIAL_BALANCE = 0;

    private:
        static const std::string _createUserQuery;
        static const std::string _getUserEmailQuery;
        static const std::string _getUserIdQuery;
        static const std::string _decreaseBalanceQuery;
        static const std::string _increaseBalanceQuery;
    };
}