#pragma once

#include "method_handler.hpp"

namespace sm {
    class Get final : public MethodHandler {
    public:
        Get() = delete;
        explicit Get(
            const http::verb requestMethod, std::unique_ptr<Response> response) noexcept;
        
        ~Get() override = default;

    protected:
        bool methodIsValid() const noexcept override;
    };
}