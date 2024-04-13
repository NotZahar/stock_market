#include "router.hpp"

#include "responses/auth/register.hpp"
#include "responses/auth/login.hpp"
// #include "responses/authorized.hpp"
// #include "responses/get.hpp"
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
        const Response::RequestInfo requestInfo = { request.version(), request.keep_alive() };
        const std::unordered_map<std::string, std::string> requestParams = requestURL.getParams();

        const auto requestSegments = requestURL.getSegments();
        if (requestSegments.empty() || routeSegments.right.count(requestSegments.front()) == 0)
            return BadRequestResponse{ requestInfo }.create();

        const routeSegment route = routeSegments.right.find(requestSegments.front())->second;
        switch (route) {
            case routeSegment::auth_register: {
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
            } case routeSegment::auth_login: {
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
            } default: { 
                return BadRequestResponse{ requestInfo }.create();
            }
        }
    }
}