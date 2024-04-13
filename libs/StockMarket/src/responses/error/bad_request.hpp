#pragma once

#include "../response.hpp"
#include "utility/messages.hpp"

namespace sm {
    class BadRequestResponse final : public Response {
    public:
        BadRequestResponse() = delete;
        BadRequestResponse(
            RequestInfo request, 
            std::string_view reason = messages::errors::INVALID_REQUEST) noexcept;
        
        ~BadRequestResponse() override = default;

        http::message_generator create() const override;
    
    private:
        std::string _reason;
    };
}