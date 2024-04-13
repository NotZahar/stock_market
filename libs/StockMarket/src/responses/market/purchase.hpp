#pragma once

#include <string_view>

#include "../response.hpp"

namespace sm {
    class PurchaseResponse final : public Response {
    public:
        PurchaseResponse() = delete;
        explicit PurchaseResponse(
            RequestInfo request,
            std::string_view userId,
            std::string_view purchaseData);
        
        ~PurchaseResponse() override = default;

        http::message_generator create() const override;

    private:
        std::string_view _userId;
        std::string_view _purchaseData;
    };
}