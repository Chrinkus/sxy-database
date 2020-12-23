find_package(Catch2 REQUIRED)

# Access project headers
include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(tests_all EXCLUDE_FROM_ALL
	tests-main.cpp
	tests-database.cpp
	tests-query.cpp
)

target_link_libraries(tests_all
	PRIVATE sxy_database
		Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(tests_all)

