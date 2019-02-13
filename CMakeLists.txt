cmake_minimum_required(VERSION 3.1...3.12)

if (${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Shixxy_db)

#set(THREADS_PREFER_PTHREAD_FLAG ON)
#find_package(Threads REQUIRED)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(include)

add_subdirectory(src)
#add_subdirectory(test)

#enable_testing()
