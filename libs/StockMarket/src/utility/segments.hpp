#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace sm::routes {
    enum class segment {
        auth_register,
        auth_login,
        market_purchase,
        market_sell
    };

    inline static const boost::bimap<segment, std::string> routeSegments = 
        boost::assign::list_of<boost::bimap<segment, std::string>::relation>
        ( segment::auth_register, "auth_register" )
        ( segment::auth_login, "auth_login" )
        ( segment::market_purchase, "market_purchase" )
        ( segment::market_sell, "market_sell" );
}