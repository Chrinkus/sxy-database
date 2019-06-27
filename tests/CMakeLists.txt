file(GLOB TEST_SOURCES "*.cpp")

add_executable(tests_sxy_database "${TEST_SOURCES}")
target_compile_features(tests_sxy_database PRIVATE cxx_std_14)
target_link_libraries(tests_sxy_database PRIVATE sxy_database)
