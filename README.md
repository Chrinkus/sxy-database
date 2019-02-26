# Shixxy Database
### A C++ wrapper for SQLite3

## Dependencies
To use this library wrapper you will need:
- CMake
    - v3.10 or later
- Sqlite3
    - installed according to [this gist](https://gist.github.com/Chrinkus/85a1db14b74460699dae60dfb541e50c)
- Catch2 (optional)
    - locally installed w/CMake hooks
    - skip this by commenting out the bottom two lines in the root CMakeLists.txt file.

## Installation
To install this library locally, clone the repository and from the root of the project enter:
```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ sudo make install
```
This will install the library to `~/dev/lib/` and the necessary headers to `~/dev/include/shixxy/`. 

## Linking
To link this library to a project using CMake add the following to your root `CMakeLists.txt`:
```
set(PROJECT_LINK_DIRECTORIES libsxy_database.so)
link_directories(~/dev/lib)
include_directories(~/dev/include)
```

## Usage
First make the symbols available:
```
#include <shixxy/Sxy_Database.hpp>
#include <shixxy/Sxy_Query.hpp>
```
Then create an `Sxy::Database` wrapper and connect it to the desired database file:
```
Sxy::Database db;
db.connect("test.db");
```
This connection object is then passed to `Sxy::Query` objects to execute SQL queries on the database.
```
Sxy::Query query {db};
query.exec("CREATE TABLE cats (name TEXT, age INTEGER);");
```

to be continued...
