#pragma once

#include <string_view>

#include "../response.hpp"

namespace sm {
    class SellResponse final : public Response {
    public:
        SellResponse() = delete;
        explicit SellResponse(
            RequestInfo request,
            std::string_view userId,
            std::string_view sellData);
        
        ~SellResponse() override = default;

        http::message_generator create() const override;

    private:
        std::string_view _userId;
        std::string_view _sellData;
    };
}