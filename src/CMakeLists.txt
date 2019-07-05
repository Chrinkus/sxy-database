file(GLOB SOURCES "*.cpp")

# Intended to be a shared tool on development machine
add_library(sxy_database SHARED "${SOURCES}")
target_compile_features(sxy_database PRIVATE cxx_std_14)
target_link_libraries(sxy_database PRIVATE sqlite3)

file(GLOB HEADERS "../include/*.hpp")
set_target_properties(sxy_database PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS sxy_database
        LIBRARY DESTINATION ~/libs/lib64
        PUBLIC_HEADER DESTINATION ~/libs/include/shixxy)
