#include "Database.hpp"

#include <algorithm>

namespace Sxy {

Database::~Database()
{
    if (db) {
        sqlite3_close(db);
    }
}

bool Database::open(const std::string& db_name)
{
    int rc = sqlite3_open(db_name.data(), &db);

    return rc == SQLITE_OK ? true : false;
}

bool Database::close()
{
    int rc = sqlite3_close(db);

    if (rc == SQLITE_OK) {
        db = nullptr;
        return true;
    } // no consideration for SQLITE_BUSY

    return false;
}

std::vector<std::string> Database::tables() const
{
    std::vector<std::string> vt;
    sqlite3_stmt* pstmt = nullptr;
    std::string sql {"SELECT name FROM sqlite_master "
                     "WHERE type='table' "
                     "AND name NOT LIKE 'sqlite_%';"};

    sqlite3_prepare_v2(db, sql.data(), sql.size() + 1, &pstmt, nullptr);

    while (sqlite3_step(pstmt) != SQLITE_DONE) {
        vt.emplace_back(std::string{
                // sqlite3_column_text() returns `const unsigned char*`
                reinterpret_cast<const char*>(sqlite3_column_text(pstmt, 0))
            }); 
    }

    return vt;
}

bool Database::has_table(const std::string& tname) const
{
    auto vt = tables();
    auto it = std::find(std::begin(vt), std::end(vt), tname);
    return it == std::end(vt) ? false : true;
}

}
