cmake_minimum_required(VERSION 3.16)

project(sxy-database VERSION 0.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)	
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -O3)
endif()

add_subdirectory(src)
add_subdirectory(tests)
