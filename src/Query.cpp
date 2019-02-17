#include "Query.hpp"
#include "Database.hpp"

namespace Sxy {

Query::Query(const Database& rdb)
    : db{rdb}
{
}

Query::~Query()
{
    sqlite3_finalize(pstmt);
}

std::string Query::last_query() const
{
    return std::string{sqlite3_sql(pstmt)};
}

std::string Query::last_error() const
{
    return std::string{sqlite3_errmsg(db.data())};
}

bool Query::prepare(const std::string& sql)
{
    int rc = sqlite3_prepare_v2(db.data(), sql.data(), -1, &pstmt, nullptr);

    return rc == SQLITE_OK ? true : false;
}

bool Query::exec()
{
    int rc = sqlite3_step(pstmt);

    return rc == SQLITE_DONE ? true : false;
}

}
