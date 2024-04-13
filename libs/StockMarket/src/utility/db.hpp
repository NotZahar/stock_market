#pragma once

#include <regex>
#include <string_view>

namespace sm::db {
    struct sanitizer {
        static bool check(std::string_view value) {
            return std::regex_match(value.cbegin(), value.cend(), queryRegex);
        }

        inline static const std::regex queryRegex{ "^[a-zA-Z0-9.@ ]+$" };
    };
}