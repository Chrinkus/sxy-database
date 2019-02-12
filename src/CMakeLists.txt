add_library(sqlite3 STATIC ${SQLITE_DIR}/sqlite3.c ${SQLITE_DIR}/sqlite3.h)
target_link_libraries(sqlite3 PRIVATE Threads::Threads)
target_link_libraries(sqlite3 PRIVATE -ldl)

add_library(sxy_db STATIC Database.cpp
    ${PROJECT_SOURCE_DIR}/include/Database.hpp)
target_compile_features(sxy_db PRIVATE cxx_std_14)
target_link_libraries(sxy_db PUBLIC sqlite3)

add_executable(main main.cpp)
target_compile_features(main PRIVATE cxx_std_14)
target_link_libraries(main PUBLIC sxy_db)
