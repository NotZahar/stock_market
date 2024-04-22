#pragma once

#include <string>

#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace sm {
    struct time {
        static std::string now() {
            return boost::posix_time::to_iso_string(
                boost::posix_time::microsec_clock::universal_time());
        }
    };
}