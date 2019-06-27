file(GLOB SOURCES "*.cpp")

add_library(sxy_database "${SOURCES}")
target_compile_features(sxy_database PRIVATE cxx_std_14)
target_link_libraries(sxy_database PRIVATE sqlite3)
