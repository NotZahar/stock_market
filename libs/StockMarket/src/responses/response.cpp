#include "response.hpp"

namespace sm {
    Response::Response(RequestInfo request) noexcept
        : _request(request)
    {}

    Response::RequestInfo Response::getRequestInfo() const noexcept {
        return _request;
    }
}