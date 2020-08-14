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

        REQUIRE(vectorProd == 11);
    }

    SECTION("vector negative")
    {
        auto res = -(vector1);

        REQUIRE(res[0]==-1);
        REQUIRE(res[1]==-2);
    }

    SECTION("Add right vector to left constant")
    {
        double c = 2.0;
        auto res = c + vector1;

        REQUIRE(res[0]==3);
        REQUIRE(res[1]==4);
    }

    SECTION("Add left vector to right constant")
    {
        double c = 2.0;
        auto res = vector1 + c;

        REQUIRE(res[0]==3);
        REQUIRE(res[1]==4);
    }

    SECTION("Mul right vector to left constant")
    {
        double c = 2.0;
        auto res = c * vector1;

        REQUIRE(res[0]==2);
        REQUIRE(res[1]==4);
    }

    SECTION("Mul left vector to right constant")
    {
        double c = 2.0;
        auto res = vector1 * c;

        REQUIRE(res[0]==2);
        REQUIRE(res[1]==4);
    }

    SECTION("Multi operation")
    {
        auto x = pnc::Vector<2,int>(1,2);
        auto y = pnc::Vector<2,int>(3,4);
        auto c = 2;
        auto res = (x * c)+ y;

        REQUIRE(res[0]==5);
        REQUIRE(res[1]==8);
    }

    SECTION("Bug when combining stuff")
    {
        auto x = pnc::Vector<2,int>(1,2);
        auto y = pnc::Vector<2,int>(3,4);
        auto res = SUM(y+(-x)); // (3-1) + (4-2) = 2 + 2 = 4

        REQUIRE(res == 4);
    }
}

TEST_CASE("Helper methods")
{
    auto vec = pnc::Vector<2, double>( 1.0, 2.0 );
    auto vecWithsign = pnc::Vector<2, double>( -1.0, 2.0 );

    SECTION("SUM")
    {
        REQUIRE(SUM(vec) == 3);
    }

    SECTION("MAX")
    {
        REQUIRE(MAX(vec) == 2);
    }

    SECTION("L1 norm")
    {
        // l1-norm sum(|1|,|2|) = 3
        auto res = NormL1(vecWithsign);

        REQUIRE(res == 3);
    }
}
