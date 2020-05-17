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
        unsigned int test = vectorProd.get_size();

        REQUIRE(vectorProd[0] == 3);
        REQUIRE(vectorProd[1] == 8);
        REQUIRE(SUM(vectorProd) == 11);
    }
}

TEST_CASE("Helper methods")
{
    auto vec = pnc::Vector<2, double>(new double[2] { 1, 2 });
    SECTION("SUM")
    {
        REQUIRE(SUM(vec) == 3);
    }
    SECTION("MAX")
    {
        REQUIRE(MAX(vec) == 2);
    }
}
