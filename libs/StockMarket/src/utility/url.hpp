#pragma once

#include <list>
#include <unordered_set>
#include <algorithm>

#include "types.hpp"
#include "params.hpp"

namespace sm {
    struct url {
        std::list<std::string> getSegments() const noexcept {
            auto urlView = urls::url_view{ relativeUrl };
            std::list<std::string> segments;
            for (auto segment : urlView.segments())
                segments.emplace_back(segment);
            return segments;
        }

        std::unordered_map<std::string, std::string> getParams() const noexcept {
            auto urlView = urls::url_view{ relativeUrl };
            std::unordered_map<std::string, std::string> params;
            for (auto param : urlView.params())
                params.insert({ param.key, param.value });
            return params;
        }

        bool isEmpty() const noexcept {
            return urls::url_view{ relativeUrl }.empty();
        }

        static bool paramsExists(
            const std::unordered_map<std::string, std::string>& params, 
            std::unordered_set<urlParam> paramsToCheck) noexcept {
                return std::ranges::all_of(paramsToCheck, [&params](urlParam param) {
                    return params.contains(urlParams.left.find(param)->second);
                });
        }

        std::string relativeUrl;
    };
}