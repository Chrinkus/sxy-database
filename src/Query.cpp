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

int Query::last_insert_id() const
{
    return sqlite3_last_insert_rowid(db.data());
}

bool Query::prepare(const std::string& sql)
{
    int rc = sqlite3_prepare_v2(db.data(), sql.data(), sql.size() + 1, &pstmt,
            nullptr);

    return rc == SQLITE_OK ? true : false;
}

bool Query::exec() const
{
    int rc = sqlite3_step(pstmt);

    return rc == SQLITE_DONE ? true : false;
}

bool Query::exec(const std::string& sql)
{
    prepare(sql);
    return exec();
}

bool Query::step() const
{
    int rc = sqlite3_step(pstmt);

    return rc == SQLITE_ROW ? true : false;
}

bool Query::bind_value(const std::string& param, int value)
{
    int index = sqlite3_bind_parameter_index(pstmt, param.data());
    int rc = sqlite3_bind_int(pstmt, index, value);

    return rc == SQLITE_OK ? true : false;
}

bool Query::bind_value(const std::string& param, double value)
{
    int index = sqlite3_bind_parameter_index(pstmt, param.data());
    int rc = sqlite3_bind_double(pstmt, index, value);

    return rc == SQLITE_OK ? true : false;
}

bool Query::bind_value(const std::string& param, const std::string& value)
{
    int index = sqlite3_bind_parameter_index(pstmt, param.data());
    int rc = sqlite3_bind_text(pstmt, index, value.data(), value.size(),
            SQLITE_TRANSIENT);

    return rc == SQLITE_OK ? true : false;
}

Query::Value Query::value(const std::string& col_name) const
{
    if (!pstmt || sqlite3_column_count(pstmt) == 0) {
        return Value{};
    }

    for (int i = 0; i < sqlite3_column_count(pstmt); ++i) {
        if (col_name == sqlite3_column_name(pstmt, i)) {
            return Value{sqlite3_column_value(pstmt, i)};
        }
    }
    return Value{};
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 

Query::Value::Value(sqlite3_value* p)
    : pval{p}
{
}

int Query::Value::to_int() const
{
    return sqlite3_value_int(pval);
}

double Query::Value::to_double() const
{
    return sqlite3_value_double(pval);
}

std::string Query::Value::to_string() const
{
    return std::string{
        reinterpret_cast<const char*>(sqlite3_value_text(pval))};
}

}
