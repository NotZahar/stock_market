#pragma once

#include <string_view>

#include "../response.hpp"

namespace sm {
    class BalanceResponse final : public Response {
    public:
        BalanceResponse() = delete;
        explicit BalanceResponse(
            RequestInfo request,
            std::string_view userId);
        
        ~BalanceResponse() override = default;

        http::message_generator create() const override;

    private:
        std::string_view _userId;
    };
}