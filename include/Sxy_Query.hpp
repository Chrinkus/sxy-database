#ifndef SXY_QUERY_HPP
#define SXY_QUERY_HPP

#include <sqlite3.h>
#include <string>

namespace Sxy {

class Database;

class Value {
public:
    Value() = default;
    Value(sqlite3_value* p);

    /*
    int to_int();
    double to_double();
    std::string to_string();
    */

    explicit operator bool() const { return false; }

private:
    sqlite3_value* pval = nullptr;
};

class Query {
public:
    Query(const Database& rdb);

    ~Query();

    std::string last_query() const;
    std::string last_error() const;

    bool prepare(const std::string& sql);
    bool exec();
    bool step();

    Value value(const std::string& col_name);

private:
    const Database& db;
    sqlite3_stmt* pstmt = nullptr;
};

}

#endif  // SXY_QUERY_HPP
