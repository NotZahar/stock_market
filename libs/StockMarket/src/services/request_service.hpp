#pragma once

#include <list>
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

        inline static const boost::bimap<requestSide, std::string> requestSides = 
            boost::assign::list_of<boost::bimap<requestSide, std::string>::relation>
            ( requestSide::purchase, "PURCHASE" )
            ( requestSide::sell, "SELL" );

        inline static const boost::bimap<requestStatus, std::string> requestStatuses = 
            boost::assign::list_of<boost::bimap<requestStatus, std::string>::relation>
            ( requestStatus::active, "ACTIVE" )
            ( requestStatus::closed, "CLOSED" );

        RequestService() = default;

        ~RequestService() = default;
        
        static std::string create(Request request, errorCode& eCode) noexcept;
        // static std::list<std::list<std::string>> getActivePurchases(
        //     const int price, 
        //     errorCode& eCode) noexcept;
        static std::list<std::list<std::string>> getActiveSells(
            const int price,
            std::string_view registrationTime, 
            errorCode& eCode) noexcept;
        static std::list<std::string> getRequestById(std::string_view id, errorCode& eCode) noexcept;
        static void decreaseVolume(std::string_view requestId, const int difference) noexcept;

    private:
        static const std::string _createRequestQuery;
        static const std::string _getActivePurchasesQuery;
        static const std::string _getActiveSellsQuery;
        static const std::string _getRequestByIdQuery;
        // static const std::string _finishRequestQuery;
        static const std::string _decreaseVolumeQuery;
    };
}