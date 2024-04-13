#pragma once

#include "method_handler.hpp"

namespace sm {
    class Post final : public MethodHandler {
    public:
        Post() = delete;
        explicit Post(
            const http::verb requestMethod, std::unique_ptr<Response> response) noexcept;
        
        ~Post() override = default;

    protected:
        bool methodIsValid() const noexcept override;
    };
}