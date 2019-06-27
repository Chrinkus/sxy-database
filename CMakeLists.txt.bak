cmake_minimum_required(VERSION 3.1...3.14)

if (${CMAKE_VERSION} VERSION_LESS 3.14)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(sxy_database VERSION 0.1 LANGUAGES CXX)
set(CMAKE_BUILD_TYPE Release)

add_compile_options(-Wall -Wextra -Wpedantic -O3)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.hpp")

add_library(${CMAKE_PROJECT_NAME} SHARED ${SOURCES})
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_14)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sqlite3)

set_target_properties(${CMAKE_PROJECT_NAME}
        PROPERTIES PUBLIC_HEADER "${HEADERS}")

install(TARGETS ${CMAKE_PROJECT_NAME}
        LIBRARY DESTINATION /usr/local/lib
        PUBLIC_HEADER DESTINATION /usr/local/include/shixxy)

add_subdirectory(tests)
enable_testing()
