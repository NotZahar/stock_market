#pragma once

#include <filesystem>
#include <unordered_map>
#include <string_view>
#include <chrono>

#include <boost/program_options.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/url.hpp>

namespace sm {
    namespace core = boost::core;
    namespace asio = boost::asio;
    namespace ip = asio::ip;
    namespace fs = std::filesystem;
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace urls = boost::urls;

    using options_description = boost::program_options::options_description;
    using tcp_stream = typename beast::tcp_stream::rebind_executor<
        asio::use_awaitable_t<>::executor_with_default<asio::any_io_executor>>::other;

    /*!
        \brief Non-copyable and non-movable
    */
    #define SM_NCM(T) T(const T&) = delete; \
        T& operator=(const T&) = delete;    \
        T(T&&) = delete;                    \
        T& operator=(T&&) = delete;
}