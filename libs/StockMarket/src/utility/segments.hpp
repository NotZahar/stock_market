#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace sm {
    enum class routeSegment {
        auth_register,
        auth_login
    };

    inline static const boost::bimap<routeSegment, std::string> routeSegments = 
        boost::assign::list_of<boost::bimap<routeSegment, std::string>::relation>
        ( routeSegment::auth_register, "auth_register" )
        ( routeSegment::auth_login, "auth_login" );
}