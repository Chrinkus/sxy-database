#include "Database.hpp"

#include <iostream>

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
    }
}

}
