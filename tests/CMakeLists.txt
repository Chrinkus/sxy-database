find_package(Catch2 REQUIRED)

file(GLOB TEST_SOURCES "./*.cpp")

set(PROJECT_LINK_DIRECTORIES libsxy_db.so)
link_directories(~/dev/lib)
include_directories(~/dev/include)
#link_directories(${PROJECT_SOURCE_DIR}/build)
#include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(tests "${TEST_SOURCES}")
target_compile_features(tests PRIVATE cxx_std_14)
target_link_libraries(tests PUBLIC Catch2::Catch2)
target_link_libraries(tests PRIVATE sxy_db)

include(CTest)
include(Catch)
catch_discover_tests(tests)
