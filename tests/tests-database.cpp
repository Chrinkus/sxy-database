#include <catch2/catch.hpp>

#include <Database.hpp>
#include <Query.hpp>

#include <cstdlib>
#include <iostream>

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

TEST_CASE("Database can be opened in current directory", "[open_at]") {
    Sxy::Database db;

    bool res = db.open_at("data.db");
    REQUIRE(res == true);
}

TEST_CASE("Database can be opened using environment variables", "[open_at]") {
    Sxy::Database db;
    const std::string home = std::getenv("HOME");
    std::cout << home << '\n';

    bool res = db.open(home + std::string{"/db_test.db"});
    REQUIRE(res == true);
}
