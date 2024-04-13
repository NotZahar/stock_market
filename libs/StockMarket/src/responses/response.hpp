#pragma once

#include "../utility/types.hpp"

namespace sm {
    /*!
        \brief Родитель для всех http-ответов
    */
    class Response {
    public:
        struct RequestInfo {
            unsigned httpVersion;
            bool keepAlive;
        };

        Response() = delete;
        explicit Response(RequestInfo request) noexcept;

        virtual ~Response() = default;

        virtual http::message_generator create() const = 0;
        RequestInfo getRequestInfo() const noexcept;

    protected:
        RequestInfo _request;
    };
}