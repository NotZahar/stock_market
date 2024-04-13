#pragma once

#include <string>
#include <string_view>

namespace sm::service {
    class UserService {
    public:
        enum class errorCode {
            noError,
            alreadyExists,
            badData
        };

        struct User {
            std::string_view id;
            std::string_view email;
            int balance;
        };

        UserService() = default;

        ~UserService() = default;
        
        static void create(User user, errorCode& error) noexcept;
        static bool exists(std::string_view email, errorCode& error) noexcept;

    private:
        static const std::string _createUserQuery;
        static const std::string _getUserQuery;
    };
}