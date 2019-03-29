cmake_minimum_required(VERSION 3.1...3.14)

if (${CMAKE_VERSION} VERSION_LESS 3.14)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Shixxy_database)
set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wpedantic -O3)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp")

add_library(sxy_database SHARED ${SOURCES})
target_compile_features(sxy_database PRIVATE cxx_std_14)
target_link_libraries(sxy_database PRIVATE sqlite3)

set_target_properties(sxy_database PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS sxy_database
        LIBRARY DESTINATION ~/dev/lib
        PUBLIC_HEADER DESTINATION ~/dev/include/shixxy)

add_subdirectory(tests)
enable_testing()
