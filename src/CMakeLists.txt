add_library(sxy_database Database.cpp Query.cpp)
target_link_libraries(sxy_database PRIVATE sqlite3)

file(GLOB HEADERS "./*.hpp")
set_target_properties(sxy_database PROPERTIES PUBLIC_HEADER "${HEADERS}")

# Linux specific
install(TARGETS sxy_database
        LIBRARY DESTINATION ~/libs/lib64
        PUBLIC_HEADER DESTINATION ~/libs/include/shixxy)
