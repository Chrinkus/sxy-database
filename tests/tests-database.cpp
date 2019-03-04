#include <catch2/catch.hpp>

#include <Sxy_Database.hpp>
#include <Sxy_Query.hpp>

TEST_CASE("Database successfully opens", "[Database::open]") {

    Sxy::Database db;

    bool res = db.open(":memory:");

    REQUIRE(res == true);
}

TEST_CASE("Database can be queried for tables", "[Database::tables]") {

    Sxy::Database db;
    db.open(":memory:");

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
