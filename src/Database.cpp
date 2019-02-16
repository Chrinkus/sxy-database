#include "Database.hpp"

#include <iostream>
#include <algorithm>

namespace Sxy {

Database::~Database()
{
    if (db) {
        sqlite3_close(db);
        std::cout << "Database closed\n";
    }
}

bool Database::connect(const std::string& db_name)
{
    int rc = sqlite3_open(db_name.data(), &db);

    if (rc) {
        std::cerr << "Could not open database: " << db_name << '\n';
        return false;
    } else {
        std::cout << "Database " << db_name << " opened successfully\n";
        return true;
    }
}

std::vector<std::string> Database::tables() const
{
    std::vector<std::string> vt;
    sqlite3_stmt* pstmt = nullptr;

    sqlite3_prepare_v2(db,
            "SELECT name FROM sqlite_master " \
            "WHERE type='table' " \
            "AND name NOT LIKE 'sqlite_%';",
            -1,
            &pstmt, nullptr);

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
