#include <catch2/catch.hpp>

#include <shixxy/Database.hpp>
#include <shixxy/Query.hpp>

TEST_CASE("Database successfully connects", "[Database::connect]") {

    Sxy::Database db;

    bool res = db.connect(":memory:");

    REQUIRE(res == true);
}

TEST_CASE("Database can be queried for tables", "[Database::tables]") {

    Sxy::Database db;
    db.connect(":memory:");

    auto vtables = db.tables();
    REQUIRE(vtables.size() == 0);

    Sxy::Query query {db};
    query.prepare("CREATE TABLE dragons ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "age INTEGER, "
            "heads INTEGER);");
    query.exec();

    vtables = db.tables();
    REQUIRE(vtables.size() == 1);

    REQUIRE(db.has_table("dragons") == true);
    REQUIRE(db.has_table("unicorns") == false);
}