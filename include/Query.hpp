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

    bool prepare(const std::string& stmt);

    bool exec();

private:
    const Database& db;
    sqlite3_stmt* pq = nullptr;
};

}

#endif  // SXY_QUERY_HPP
