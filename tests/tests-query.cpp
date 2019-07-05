#include <catch2/catch.hpp>

#include <iostream>
#include <vector>
#include <numeric>
#include <memory>

#include <Database.hpp>
#include <Query.hpp>

TEST_CASE("Query can be used to create a table", "[Query]") {
    Sxy::Database db;
    db.open(":memory:");

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
    db.open(":memory:");

    Sxy::Query query {db};
    bool res_prep = query.prepare("bad query syntax");
    REQUIRE(res_prep == false);
}

TEST_CASE("Query report returns accurate information", "[Query::last_*]") {
    Sxy::Database db;
    db.open(":memory:");

    Sxy::Query query1 {db};
    query1.exec("INSERT INTO missing_table (name) VALUES ('Error');");
    REQUIRE(query1.last_error() == "no such table: missing_table");

    Sxy::Query query2 {db};
    query2.exec("CREATE TABLE dogs (name TEXT);");
    REQUIRE(query2.last_query() == "CREATE TABLE dogs (name TEXT);");

    Sxy::Query q_insert1 {db};
    q_insert1.exec("INSERT INTO dogs (name) VALUES ('Fraser');");
    REQUIRE(q_insert1.last_insert_id() == 1);

    Sxy::Query q_insert2 {db};
    q_insert2.exec("INSERT INTO dogs (name) VALUES ('Flynn');");
    REQUIRE(q_insert2.last_insert_id() == 2);
}

// Bug recreation
TEST_CASE("Query::last_insert_id returns correct values", "[last_insert_id]") {

    struct Goalie {
        Goalie() = default;
        Goalie(const std::string& name, int wins, double gaa)
            : n{name}, w{wins}, g{gaa} { }

        int i = -1;
        std::string n;
        int w = 0;
        double g = 0.0;
    };

    Sxy::Database db;
    db.open(":memory:");

    Sxy::Query qc {db};
    qc.exec("CREATE TABLE goalies ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT, "
            "wins INTEGER, "
            "gaa REAL);");

    auto g1 = Goalie{"Ed Belfour", 484, 2.5};
    Sxy::Query qi1 {db};
    qi1.prepare("INSERT INTO goalies (name, wins, gaa) "
                "VALUES (:name, :wins, :gaa);");
    qi1.bind_value(":name", g1.n);
    qi1.bind_value(":wins", g1.w);
    qi1.bind_value(":gaa", g1.g);
    qi1.exec();
    g1.i = qi1.last_insert_id();

    REQUIRE(g1.i == 1);

    auto g2 = Goalie{"Martin Brodeur", 691, 2.24};
    Sxy::Query qi2 {db};
    qi2.prepare("INSERT INTO goalies (name, wins, gaa) "
                "VALUES (:name, :wins, :gaa);");
    qi2.bind_value(":name", g2.n);
    qi2.bind_value(":wins", g2.w);
    qi2.bind_value(":gaa", g2.g);
    qi2.exec();
    g2.i = qi2.last_insert_id();

    REQUIRE(g2.i == 2);

    g2.n = "Marty Brodeur";
    Sxy::Query qu1 {db};
    qu1.prepare("UPDATE goalies SET name = :name WHERE id = :id;");
    qu1.bind_value(":name", g2.n);
    qu1.bind_value(":id", g2.i);
    bool ok = qu1.exec();
    REQUIRE(ok == true);

    std::vector<Goalie> vgoalies;
    Sxy::Query qs1 {db};
    qs1.prepare("SELECT * FROM goalies;");
    while (qs1.step()) {
        Goalie g;
        g.i = qs1.value("id").to_int();
        g.n = qs1.value("name").to_string();
        g.w = qs1.value("wins").to_int();
        g.g = qs1.value("gaa").to_int();
        vgoalies.push_back(g);
    }

    REQUIRE(vgoalies.size() == 2);
    REQUIRE(vgoalies.back().n == "Marty Brodeur");
}

TEST_CASE("Query::step can be used to return data", "[Query::step]") {
    Sxy::Database db;
    db.open(":memory:");

    Sxy::Query q_create {db};
    q_create.exec("CREATE TABLE birds (name TEXT);");

    std::vector<std::string> vsb { "Cardinal", "Chickadee", "Robin" };
    for (const auto& s : vsb) {
        Sxy::Query query {db};
        query.prepare("INSERT INTO birds (name) VALUES (:name);");
        query.bind_value(":name", s);
        query.exec();
    }

    SECTION("Query::step iterates the correct number of times") {
        int count = 0;
        Sxy::Query query {db};
        query.prepare("SELECT name FROM birds;");
        while (query.step()) {
            ++count;
        }

        REQUIRE(count == vsb.size());
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
        bool res = std::equal(std::begin(vsb), std::end(vsb), std::begin(vs));
        REQUIRE(res == true);
    }
}

TEST_CASE("Query::step can be used to retrieve integer data",
        "[Query::step]") {
    Sxy::Database db;
    db.open(":memory:");

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
    db.open(":memory:");

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

TEST_CASE("Query::bind can be used to insert integers", "[Query::bind]") {
    Sxy::Database db;
    db.open(":memory:");

    Sxy::Query q_create {db};
    q_create.exec("CREATE TABLE fibonacci (num INTEGER);");

    std::vector<int> vi { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 };
    for (const auto i : vi) {
        Sxy::Query query {db};
        query.prepare("INSERT INTO fibonacci (num) VALUES (:num);");
        query.bind_value(":num", i);
        query.exec();
    }

    std::vector<int> viout;
    Sxy::Query q_select {db};
    q_select.prepare("SELECT num FROM fibonacci;");
    int count = 0;
    while (q_select.step()) {
        ++count;
        viout.push_back(q_select.value("num").to_int());
    }

    REQUIRE(count == vi.size());
    bool res = std::equal(std::begin(vi), std::end(vi), std::begin(viout));
    REQUIRE(res == true);
}

TEST_CASE("Query::bind can be used to insert doubles", "[Query::bind]") {
    Sxy::Database db;
    db.open(":memory:");

    Sxy::Query q_create {db};
    q_create.exec("CREATE TABLE goals_against_avg (gaa REAL);");

    std::vector<double> vd { 2.41, 2.89, 2.02, 2.37 };
    for (const auto d : vd) {
        Sxy::Query query {db};
        query.prepare("INSERT INTO goals_against_avg (gaa) VALUES (:gaa);");
        query.bind_value(":gaa", d);
        query.exec();
    }

    std::vector<double> vdout;
    Sxy::Query q_select {db};
    q_select.prepare("SELECT gaa FROM goals_against_avg;");
    int count = 0;
    while (q_select.step()) {
        ++count;
        vdout.push_back(q_select.value("gaa").to_double());
    }

    REQUIRE(count == vd.size());
    bool res = std::equal(std::begin(vd), std::end(vd), std::begin(vdout));
    REQUIRE(res == true);
}

TEST_CASE("Data can make the round trip", "[Query]") {

    // Data class
    class Game {
    public:
        Game() = default;

        int id() const { return i; }
        void id(int id) { i = id; }
        std::string title() const { return t; }
        void title(const std::string& title) { t = title; }
        std::string developer() const { return d; }
        void developer(const std::string& dev) { d = dev; }
        std::string system() const { return s; }
        void system(const std::string& system) { s = system; }
    private:
        int i = -1;
        std::string t;      // title
        std::string d;      // developer
        std::string s;      // system
    };

    Sxy::Database db;
    db.open(":memory:");

    Sxy::Query q_create {db};
    q_create.exec("CREATE TABLE games ("
                  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "title TEXT, "
                  "developer TEXT, "
                  "system TEXT);");

    // Data object
    Game g1 {};
    g1.title("Tomb Raider");
    g1.developer("Crystal Dynamics");
    g1.system("XBOX ONE");

    // Data goes into the database
    Sxy::Query q_insert {db};
    q_insert.prepare("INSERT INTO games (title, developer, system) "
                     "VALUES (:title, :developer, :system);");
    q_insert.bind_value(":title", g1.title());
    q_insert.bind_value(":developer", g1.developer());
    q_insert.bind_value(":system", g1.system());
    q_insert.exec();
    g1.id(q_insert.last_insert_id());

    // Data comes out of the database
    std::vector<std::unique_ptr<Game>> vgames;
    Sxy::Query q_select {db};
    q_select.prepare("SELECT * FROM games;");
    while (q_select.step()) {
        auto g = std::make_unique<Game>();
        g->id(q_select.value("id").to_int());
        g->title(q_select.value("title").to_string());
        g->developer(q_select.value("developer").to_string());
        g->system(q_select.value("system").to_string());
        vgames.push_back(std::move(g));
    }

    REQUIRE(vgames.size() == 1);
    Game g2 = *vgames.front();
    CHECK(g1.id() == g2.id());
    CHECK(g1.title() == g2.title());
    CHECK(g1.developer() == g2.developer());
    CHECK(g1.system() == g2.system());
}
