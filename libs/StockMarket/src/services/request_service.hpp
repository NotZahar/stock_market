#pragma once

#include <string>
#include <string_view>

#include <boost/bimap.hpp>
#include <boost/assign.hpp>

namespace sm::service {
    /*!
        \brief Сервис для работы с торговой заявкой
    */
    class RequestService {
    public:
        enum class errorCode {
            noError,
            internalError,
            badData
        };

        enum class requestSide {
            purchase,
            sell
        };

        enum class requestStatus {
            active,
            closed
        };

        struct Request {
            std::string_view id;
            int volume;
            int price;
            requestSide side;
            requestStatus status;
            std::string_view client_id;
        };

        RequestService() = default;

        ~RequestService() = default;
        
        static void create(Request request, errorCode& error) noexcept;

    private:
        static const std::string _createRequestQuery;
        
        inline static const boost::bimap<requestSide, std::string> requestSides = 
            boost::assign::list_of<boost::bimap<requestSide, std::string>::relation>
            ( requestSide::purchase, "PURCHASE" )
            ( requestSide::sell, "SELL" );

        inline static const boost::bimap<requestStatus, std::string> requestStatuses = 
            boost::assign::list_of<boost::bimap<requestStatus, std::string>::relation>
            ( requestStatus::active, "ACTIVE" )
            ( requestStatus::closed, "CLOSED" );
    };
}