#pragma once

#include "../response.hpp"
#include "services/auth_service.hpp"

namespace sm {
    class RegisterResponse final : public Response {
    public:
        RegisterResponse() = delete;
        explicit RegisterResponse(
            RequestInfo request,
            service::AuthService::RegisterUserData registerData);
        
        ~RegisterResponse() override = default;

        http::message_generator create() const override;

    private:
        service::AuthService::RegisterUserData _registerData;
    };
}