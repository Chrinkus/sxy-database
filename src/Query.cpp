#include "Query.hpp"
#include "Database.hpp"
#include <iostream>

namespace Sxy {

Query::Query(const Database& rdb)
    : db{rdb}
{
}

Query::~Query()
{
    sqlite3_finalize(pq);
}

bool Query::prepare(const std::string& stmt)
{
    int rc = sqlite3_prepare_v2(db.data(), stmt.data(), -1, &pq, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Could not prepare query [" << rc << "]:\n"
                  << stmt << '\n'
                  << sqlite3_errmsg(db.data()) << '\n';
        return false;
    } else {
        std::cerr << "Statement prepared successfully\n";
        return true;
    }
}

bool Query::exec()
{
    int rc = sqlite3_step(pq);

    if (rc != SQLITE_DONE) {
        std::cerr << "Could not execute prepared statement\n";
        return false;
    } else {
        std::cerr << "Statement executed successfully\n";
        return true;
    }
}

}
