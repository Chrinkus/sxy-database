#pragma once

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

    std::vector<std::string> tables() const;
    bool has_table(const std::string& tname) const;

    bool open(const std::string& db_file);
    bool open_at(const std::string& db_path);
    bool close();

    sqlite3* data() const { return db; }

private:
    sqlite3* db = nullptr;
};

}   // Sxy

