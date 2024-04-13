#include "get.hpp"

namespace sm {
    Get::Get(
        const http::verb requestMethod, std::unique_ptr<Response> response) noexcept
        : MethodHandler(requestMethod, std::move(response))
    {}

    bool Get::methodIsValid() const noexcept {
        return _requestMethod == http::verb::get;
    }
}