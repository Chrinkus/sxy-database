cmake_minimum_required(VERSION 3.13)

project(sxy_database VERSION 0.2 LANGUAGES CXX)

add_compile_options(-Wall -Wextra -Wpedantic -O3)

include_directories(include)

add_subdirectory(src)
add_subdirectory(tests)
