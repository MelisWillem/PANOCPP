#include <panoc/proxOperators.hpp>
#define CATCH_CONFIG_MAIN
#include"catch.hpp"

TEST_CASE("Box test")
{
    SECTION("inside the box, should stay inside the box")
    {
        pnc::Vector<int> vec = {2};
        REQUIRE(pnc::Box(vec, 1, 3)[0] == 2);
    }

    SECTION("below the box, should return low border")
    {
        pnc::Vector<int> vec = {0};
        REQUIRE(pnc::Box(vec, 1, 3)[0] == 1);
    }

    SECTION("behigh the box, should return high border")
    {
        pnc::Vector<int> vec = {4};
        REQUIRE(pnc::Box(vec, 1, 3)[0] == 3);
    }
}

TEST_CASE("Norm box test")
{
    using namespace pnc;
    using Vec = Vector<int>;
    // Test all 3 cases of the normbox
    Vec v{1,2};
    // cost = max{0,l1norm-offset}

    SECTION("low") // l1norm<offset
    {
        constexpr int offset = 4;
        auto op = NormBox<int>(offset);
        auto res = op(v); 
        REQUIRE(res.cost()== 0); // 3-4 = -1 -> 0
        REQUIRE(res[0] == v[0]); // stuff should just remain the same
        REQUIRE(res[1] == v[1]);
    }

    SECTION("mid") // offset<l1norm<2*offset
    {
        constexpr int offset = 2;
        auto res = NormBox<int>(offset)(v); 
        REQUIRE(res.cost()== 1); // 3-2 = 1 -> 1
        REQUIRE(res[0] == offset); // sign*offset
        REQUIRE(res[1] == offset);
    }

    SECTION("high") // offset*2< l1norm
    { 
        constexpr int offset = 1;
        auto res = NormBox<int>(offset)(v);
        REQUIRE(res.cost()== 2); // 3 - 1 = 2
        REQUIRE(res[0] == (v[0]-offset)); // sign*(x-offset)
        REQUIRE(res[1] == (v[1]-offset));
    }
}
