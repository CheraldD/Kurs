#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "main.cpp"
TEST_CASE("Addition", "[add]") {
    REQUIRE(add(2, 3) == 5);
    REQUIRE(add(-1, 1) == 0);
    REQUIRE(add(0, 0) == 0);
}