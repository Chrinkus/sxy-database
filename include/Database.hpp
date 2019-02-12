#ifndef SXY_DATABASE_HPP
#define SXY_DATABASE_HPP

#include <sqlite3.h>
#include <string>

namespace Sxy {

class Database {
public:
    Database() = default;

    Database(const Database&) = delete;
    Database(Database&&) = delete;

    ~Database();

    bool connect(const std::string& db_name);

private:
    sqlite3* db;
};

}   // Sxy

#endif  // SXY_DATABASE_HPP
