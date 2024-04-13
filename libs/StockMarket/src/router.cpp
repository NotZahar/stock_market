#include "router.hpp"

#include "responses/auth/register.hpp"
#include "responses/auth/login.hpp"
#include "responses/authorized.hpp"
#include "responses/market/purchase.hpp"
#include "responses/market/sell.hpp"
#include "responses/user/balance.hpp"
#include "responses/get.hpp"
#include "responses/post.hpp"
#include "responses/error/bad_request.hpp"
#include "services/auth_service.hpp"
#include "utility/net_helper.hpp"
#include "utility/url.hpp"
#include "utility/params.hpp"
#include "utility/segments.hpp"

namespace sm {
    http::message_generator Router::makeResponse(http::request<http::string_body> request) {
        const http::verb requestMethod = request.method();
        const url requestURL{ request.target() };
        const std::string requestBody = request.body();
        const Response::RequestInfo requestInfo = { request.version(), request.keep_alive() };
        const std::unordered_map<std::string, std::string> requestParams = requestURL.getParams();

        const auto requestSegments = requestURL.getSegments();
        if (requestSegments.empty() || routes::routeSegments.right.count(requestSegments.front()) == 0)
            return BadRequestResponse{ requestInfo }.create();

        const routes::segment route =
            routes::routeSegments.right.find(requestSegments.front())->second;
        switch (route) {
            case routes::segment::auth_register: {
                if (!url::paramsExists(requestParams, { urlParam::email }))
                    return BadRequestResponse{ requestInfo }.create();
                
                return Post{
                    requestMethod, 
                    std::make_unique<RegisterResponse>(
                        requestInfo,
                        service::AuthService::RegisterUserData{
                            requestParams.at(urlParams.left.find(urlParam::email)->second)
                        }
                    )
                }.response();
            } case routes::segment::auth_login: {
                if (!url::paramsExists(requestParams, { urlParam::email }))
                    return BadRequestResponse{ requestInfo }.create();
                
                return Post{
                    requestMethod, 
                    std::make_unique<LoginResponse>(
                        requestInfo,
                        service::AuthService::LoginUserData{
                            requestParams.at(urlParams.left.find(urlParam::email)->second)
                        }
                    )
                }.response();
            } case routes::segment::market_purchase: {
                if (!url::paramsExists(requestParams, { urlParam::user }))
                    return BadRequestResponse{ requestInfo }.create();

                return Authorized{
                    requestParams,
                    std::make_unique<Post>(
                        requestMethod, 
                        std::make_unique<PurchaseResponse>(
                            requestInfo,
                            requestParams.at(urlParams.left.find(urlParam::user)->second),
                            requestBody)
                    )
                }.response();
            } case routes::segment::market_sell: {
                if (!url::paramsExists(requestParams, { urlParam::user }))
                    return BadRequestResponse{ requestInfo }.create();

                return Authorized{
                    requestParams,
                    std::make_unique<Post>(
                        requestMethod, 
                        std::make_unique<SellResponse>(
                            requestInfo,
                            requestParams.at(urlParams.left.find(urlParam::user)->second),
                            requestBody)
                    )
                }.response();
            } case routes::segment::user_balance: {
                if (!url::paramsExists(requestParams, { urlParam::user }))
                    return BadRequestResponse{ requestInfo }.create();

                return Authorized{
                    requestParams,
                    std::make_unique<Get>(
                        requestMethod, 
                        std::make_unique<BalanceResponse>(
                            requestInfo,
                            requestParams.at(urlParams.left.find(urlParam::user)->second))
                    )
                }.response();
            } default: { 
                return BadRequestResponse{ requestInfo }.create();
            }
        }
    }
}