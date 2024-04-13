#pragma once

#include "utility/types.hpp"

namespace sm {
    /*!
        \brief Главный маршрутизатор http-запросов

        Все http-запросы проходят через данный класс, генерирует http-ответы
    */
    class Router {
    public:
        Router() = default;

        ~Router() = default;

        static http::message_generator makeResponse(http::request<http::string_body> request);
    };
}