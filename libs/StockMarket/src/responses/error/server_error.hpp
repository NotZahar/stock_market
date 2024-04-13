#pragma once

#include "../response.hpp"

namespace sm {
    class ServerErrorResponse final : public Response {
    public:
        ServerErrorResponse() = delete;
        ServerErrorResponse(RequestInfo request, std::string_view what) noexcept;
        
        ~ServerErrorResponse() override = default;

        http::message_generator create() const override;
    
    private:
        std::string _what;
    };
}