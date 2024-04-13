#include <gtest/gtest.h>

#include <string>

#include "../src/utility/db.hpp"
#include "../src/utility/net_helper.hpp"
#include "../src/utility/url.hpp"

// TEST(WebServer, db_utility) {
//     const std::string badQuery = "admin@example.com; DROP TABLE user";
//     const std::string goodQuery = "admin@example.com";

//     EXPECT_FALSE(ws::sanitizer::check(badQuery));
//     EXPECT_TRUE(ws::sanitizer::check(goodQuery));
// }

// TEST(WebServer, net_helper_utility) {
//     const std::string initStr = "some data";
//     const std::string processedStr = "some data; charset=utf-8;";

//     EXPECT_EQ(ws::netHelper::inUTF8(initStr), processedStr);
// }

// TEST(WebServer, url_helper_utility) {
//     const ws::url url{ "https://localhost:3000/path/to/file" };
//     const std::list<std::string> expectedSegments{
//         "path",
//         "to",
//         "file"
//     };

//     EXPECT_EQ(expectedSegments, url.getSegments());
// }

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}