#define CATCH_CONFIG_MAIN
#include <VectorAlgebra.hpp>
#include <catch.hpp>

using namespace pnc;

TEST_CASE("vector algebra test")
{
    auto vector1 = pnc::Vector<2, double>(new double[2] { 1, 2 });
    auto vector2 = pnc::Vector<2, double>(new double[2] { 3, 4 });
    SECTION("vector sum")
    {
        auto vectorSum = vector1 + vector2;

        REQUIRE(vectorSum[0] == 4);
        REQUIRE(vectorSum[1] == 6);
    }

    SECTION("vector product")
    {
        auto vectorProd = vector1 * vector2;
        unsigned int test = vectorProd.size;

        REQUIRE(vectorProd[0] == 3);
        REQUIRE(vectorProd[1] == 8);
        REQUIRE(SUM(vectorProd) == 11);
    }

    SECTION("vector negative")
    {
        auto res = -(vector1);

        REQUIRE(res[0]==-1);
        REQUIRE(res[1]==-2);
    }
}

TEST_CASE("Helper methods")
{
    auto vec = pnc::Vector<2, double>(new double[2] { 1, 2 });
    auto constantVec = pnc::VectorUnit<2,double>(2.0);
    SECTION("Add constant to vector")
    {
        auto res = vec + constantVec;

        REQUIRE(res[0]==3);
        REQUIRE(res[1]==4);
    }

    SECTION("To Vec Assigment")
    {
        auto buffer = Vector<2,double>();
        buffer = constantVec;

        REQUIRE(buffer[0]==2);
        REQUIRE(buffer[1]==2);
    }

    SECTION("SUM")
    {
        REQUIRE(SUM(vec) == 3);
    }

    SECTION("MAX")
    {
        REQUIRE(MAX(vec) == 2);
    }

    SECTION("ToVector")
    {
        //auto blah = vec | ToVector();
    }
}
