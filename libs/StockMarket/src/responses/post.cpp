#include "post.hpp"

namespace sm {
    Post::Post(
        const http::verb requestMethod, std::unique_ptr<Response> response) noexcept
        : MethodHandler(requestMethod, std::move(response))
    {}

    bool Post::methodIsValid() const noexcept {
        return _requestMethod == http::verb::post;
    }
}