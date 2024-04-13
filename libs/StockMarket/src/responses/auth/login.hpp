#pragma once

#include "../response.hpp"
#include "services/auth_service.hpp"

namespace sm {
    class LoginResponse final : public Response {
    public:
        LoginResponse() = delete;
        explicit LoginResponse(
            RequestInfo request,
            service::AuthService::LoginUserData loginData);
        
        ~LoginResponse() override = default;

        http::message_generator create() const override;

    private:
        service::AuthService::LoginUserData _loginData;
    };
}