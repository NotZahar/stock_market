#pragma once

#include <string>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace sm {
    enum class urlParam {
        email,
        user
    };

    inline static const boost::bimap<urlParam, std::string> urlParams = 
        boost::assign::list_of<boost::bimap<urlParam, std::string>::relation>
        ( urlParam::email, "email" )
        ( urlParam::user, "user" );
}