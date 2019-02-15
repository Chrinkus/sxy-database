cmake_minimum_required(VERSION 3.1...3.12)

if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Shixxy_db)
set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp")

add_library(sxy_db SHARED ${SOURCES})
target_compile_features(sxy_db PRIVATE cxx_std_14)
target_link_libraries(sxy_db PRIVATE sqlite3)

set_target_properties(sxy_db PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS sxy_db
        LIBRARY DESTINATION ~/dev/lib
        PUBLIC_HEADER DESTINATION ~/dev/include/shixxy)
