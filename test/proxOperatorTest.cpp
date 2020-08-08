#include"proxOperator.hpp"
#define CATCH_CONFIG_MAIN
#include"catch.hpp"

TEST_CASE("Box test")
{
    SECTION("inside the box, should stay inside the box")
    {
        auto vec = pnc::Vector<1,int>(2);
        REQUIRE(pnc::Box<int,1,3>(vec)[0] == 2);
    }

    SECTION("below the box, should return low border")
    {
        auto vec = pnc::Vector<1,int>(0);
        REQUIRE(pnc::Box<int,1,3>(vec)[0] == 1);
    }

    SECTION("behigh the box, should return high border")
    {
        auto vec = pnc::Vector<1,int>(4);
        REQUIRE(pnc::Box<int,1,3>(vec)[0] == 3);
    }
}
