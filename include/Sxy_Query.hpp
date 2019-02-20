#ifndef SXY_QUERY_HPP
#define SXY_QUERY_HPP

#include <sqlite3.h>
#include <string>

namespace Sxy {

class Database;

class Query {
private:
    class Value;
public:
    Query(const Database& rdb);

    ~Query();

    std::string last_query() const;
    std::string last_error() const;
    int64_t last_insert_id() const;

    bool prepare(const std::string& sql);
    bool exec() const;
    bool exec(const std::string& sql);
    bool step() const;

    Value value(const std::string& col_name) const;

private:
    const Database& db;
    sqlite3_stmt* pstmt = nullptr;
};

class Query::Value {
public:
    Value() = default;
    Value(sqlite3_value* p);

    int to_int() const;
    double to_double() const;
    std::string to_string() const;

    explicit operator bool() const { return false; }

private:
    sqlite3_value* pval = nullptr;
};

}

#endif  // SXY_QUERY_HPP
