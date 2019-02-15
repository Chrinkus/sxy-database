#include <catch2/catch.hpp>

#include <shixxy/Database.hpp>

TEST_CASE("Always passes to test build", "[always_passes]") {

    Sxy::Database db;

    bool res = db.connect("test");

    REQUIRE(res == true);
}
