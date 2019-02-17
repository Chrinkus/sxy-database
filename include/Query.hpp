#ifndef SXY_QUERY_HPP
#define SXY_QUERY_HPP

#include <sqlite3.h>
#include <string>

namespace Sxy {

class Database;

class Query {
public:
    Query(const Database& rdb);

    ~Query();

    std::string last_query() const;
    std::string last_error() const;

    bool prepare(const std::string& sql);
    bool exec();

private:
    const Database& db;
    sqlite3_stmt* pstmt = nullptr;
};

}

#endif  // SXY_QUERY_HPP
