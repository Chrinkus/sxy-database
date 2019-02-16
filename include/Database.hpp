#ifndef SXY_DATABASE_HPP
#define SXY_DATABASE_HPP

#include <sqlite3.h>
#include <string>
#include <vector>

namespace Sxy {

class Database {
public:
    Database() = default;

    Database(const Database&) = delete;
    Database(Database&&) = delete;

    ~Database();

    sqlite3* data() const { return db; }
    std::vector<std::string> tables() const;
    bool has_table(const std::string& tname) const;

    bool connect(const std::string& db_name);


private:
    sqlite3* db;
};

}   // Sxy

#endif  // SXY_DATABASE_HPP
