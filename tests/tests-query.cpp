#include <catch2/catch.hpp>

#include <iostream>
#include <vector>
#include <numeric>

#include <Sxy_Database.hpp>
#include <Sxy_Query.hpp>

TEST_CASE("Query can be used to create a table", "[Query]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query query {db};
    bool res_prep = query.prepare("CREATE TABLE cats ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT);");
    REQUIRE(res_prep == true);

    bool res_exec = query.exec();
    REQUIRE(res_exec == true);

    SECTION("Query can be used to INSERT") {
        Sxy::Query query {db};
        auto res_prep = query.prepare(
                "INSERT INTO cats (name) VALUES ('Chloe');");
        auto res_exec = query.exec();

        REQUIRE(res_prep == true);
        REQUIRE(res_exec == true);
    }
}

TEST_CASE("Query methods return false with bad statements", "[Query]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query query {db};
    bool res_prep = query.prepare("bad query syntax");
    REQUIRE(res_prep == false);
}

TEST_CASE("Query report returns accurate information", "[Query::last_*]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query query1 {db};
    query1.exec("INSERT INTO missing_table (name) VALUES ('Error');");
    REQUIRE(query1.last_error() == "no such table: missing_table");

    Sxy::Query query2 {db};
    query2.exec("CREATE TABLE dogs (name TEXT);");
    REQUIRE(query2.last_query() == "CREATE TABLE dogs (name TEXT);");

    Sxy::Query q_insert {db};
    q_insert.exec("INSERT INTO dogs (name) VALUES ('Fraser');");
    REQUIRE(q_insert.last_insert_id() == 1);
}

TEST_CASE("Query::step can be used to return data", "[Query::step]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query q_create {db};
    q_create.exec("CREATE TABLE birds (name TEXT);");

    Sxy::Query q_insert1 {db};
    q_insert1.exec("INSERT INTO birds (name) VALUES ('Cardinal');");
    Sxy::Query q_insert2 {db};
    q_insert2.exec("INSERT INTO birds (name) VALUES ('Chickadee');");
    Sxy::Query q_insert3 {db};
    q_insert3.exec("INSERT INTO birds (name) VALUES ('Robin');");

    SECTION("Query::step iterates the correct number of times") {
        int count = 0;
        Sxy::Query query {db};
        query.prepare("SELECT name FROM birds;");
        while (query.step()) {
            ++count;
        }

        REQUIRE(count == 3);
    }

    SECTION("Query::step can retrieve strings") {
        Sxy::Query query {db};
        query.prepare("SELECT name FROM birds;");
        std::vector<std::string> vs;
        while (query.step()) {
            vs.push_back(query.value("name").to_string());
        }
        REQUIRE(vs.size() == 3);
        REQUIRE(vs.front() == "Cardinal");
    }
}

TEST_CASE("Query::step can be used to retrieve integer data",
        "[Query::step]") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query q_create {db};
    q_create.exec("CREATE TABLE fav_nums (num INTEGER);");

    Sxy::Query q_insert1 {db};
    q_insert1.exec("INSERT INTO fav_nums (num) VALUES (37);");

    SECTION("Query::step can retrieve integers") {
        Sxy::Query query {db};
        query.prepare("SELECT num FROM fav_nums;");
        query.step();
        int val = query.value("num").to_int();

        REQUIRE(val == 37);
    }

    Sxy::Query q_insert2 {db};
    q_insert2.exec("INSERT INTO fav_nums (num) VALUES (10);");
    Sxy::Query q_insert3 {db};
    q_insert3.exec("INSERT INTO fav_nums (num) VALUES (2);");
    Sxy::Query q_insert4 {db};
    q_insert4.exec("INSERT INTO fav_nums (num) VALUES (13);");

    SECTION("Query::step can be used to fill a vector") {
        std::vector<int> vi;

        Sxy::Query query {db};
        query.prepare("SELECT num FROM fav_nums;");
        while (query.step()) {
            vi.push_back(query.value("num").to_int());
        }
        REQUIRE(vi.size() == 4);

        int sum = std::accumulate(std::begin(vi), std::end(vi), 0);
        REQUIRE(sum == 62);
    }
}

TEST_CASE("Query::step can be used to retrieve floating-point data") {
    Sxy::Database db;
    db.connect(":memory:");

    Sxy::Query q_create {db};
    q_create.exec("CREATE TABLE scores (score REAL);");

    Sxy::Query q_insert1 {db};
    q_insert1.exec("INSERT INTO scores (score) VALUES (0.123);");

    SECTION("Query::step can retrieve doubles") {
        Sxy::Query query {db};
        query.prepare("SELECT score FROM scores;");
        query.step();
        double val = query.value("score").to_double();

        REQUIRE(val == Approx(0.123));
    }

    Sxy::Query q_insert2 {db};
    q_insert2.exec("INSERT INTO scores (score) VALUES (0.456);");
    Sxy::Query q_insert3 {db};
    q_insert3.exec("INSERT INTO scores (score) VALUES (0.789);");

    SECTION("Query::step can fill a vector") {
        std::vector<double> vd;
        Sxy::Query query {db};
        query.prepare("SELECT score FROM scores;");
        while (query.step()) {
            vd.push_back(query.value("score").to_double());
        }
        REQUIRE(vd.size() == 3);

        double sum = std::accumulate(std::begin(vd), std::end(vd), 0.0);
        REQUIRE(sum == Approx(1.368));
    }
}
