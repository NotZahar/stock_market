#pragma once

#include "types.hpp"

namespace sm {
    struct config {
        // program options
        inline static const std::string defaultAddress = "127.0.0.1";
        inline static const unsigned short defaultPort = 3000;
        inline static const int defaultThreads = 1;
        inline static const std::string defaultAuthSecret = "SECRET_JWT";

        inline static ip::address address = ip::make_address(defaultAddress);
        inline static unsigned short port = defaultPort;
        inline static int threads = defaultThreads;
        inline static std::string authSecret = defaultAuthSecret;

        // http options
        inline static const std::chrono::milliseconds defaultSessionTimeout{ 10'000 };
        inline static std::chrono::milliseconds sessionTimeout = defaultSessionTimeout;

        inline static const std::string fieldServer = "SM (Linux)";
        inline static const std::string authAlgorithm = "HS256";
    };
}