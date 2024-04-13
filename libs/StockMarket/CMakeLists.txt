cmake_minimum_required(VERSION 3.28)

project("StockMarket" VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SM_TESTS_TARGET "sm_tests")

find_package(Boost 1.82 REQUIRED 
    COMPONENTS 
        system
        program_options
        url)
find_package(SQLiteCpp REQUIRED)
find_package(GTest REQUIRED)

add_library(${PROJECT_NAME} STATIC)

add_executable(${SM_TESTS_TARGET}
    tests/main.cpp
)

target_sources(${PROJECT_NAME}
    PRIVATE
        src/stock_market.cpp
        src/options_parser.cpp
        src/logger.cpp
        src/router.cpp

        # response
        src/responses/response.cpp
        src/responses/get.cpp
        src/responses/post.cpp
        src/responses/method_handler.cpp
        src/responses/error/bad_request.cpp
        src/responses/error/server_error.cpp
        src/responses/authorized.cpp
        src/responses/auth/login.cpp
        src/responses/auth/register.cpp
        src/responses/market/purchase.cpp
        src/responses/market/sell.cpp
        src/responses/user/balance.cpp

        # services
        src/services/db_service.cpp
        src/services/auth_service.cpp
        src/services/user_service.cpp
        src/services/market_service.cpp
        src/services/request_service.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        # where the library itself will look for its internal headers
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/json/include
    PUBLIC
        # where top-level project will look for the library's public headers
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # where external projects will look for the library's public headers
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_include_directories(${SM_TESTS_TARGET} 
    PRIVATE 
        ${GTEST_INCLUDE_DIRS})

# without it public headers won't get installed
set(PUBLIC_HEADERS
    include/stock_market.hpp
    include/options_parser.hpp
    include/logger.hpp
)

set_target_properties(${PROJECT_NAME} 
    PROPERTIES PUBLIC_HEADER "${PUBLIC_HEADERS}")

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
        ${Boost_LIBRARIES} 
        SQLiteCpp)
target_link_libraries(${SM_TESTS_TARGET}
    PRIVATE 
        ${GTEST_BOTH_LIBRARIES} 
        ${PROJECT_NAME})

if(CMAKE_COMPILER_IS_GNUCXX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -pedantic)
    target_compile_options(${SM_TESTS_TARGET} PRIVATE -Wall -Wextra -Werror -pedantic)
endif()

# where to find our CMake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Installing)

# tests
enable_testing()
add_test(NAME ${SM_TESTS_TARGET} COMMAND ${SM_TESTS_TARGET})

# test files after deploy
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})