file(GLOB_RECURSE UTILS_TESTS_SRCS ${CMAKE_CURRENT_SOURCE_DIR} "*.c")
add_executable(anvutils_tests ${UTILS_TESTS_SRCS})
target_link_libraries(anvutils_tests anvutils_containers anvutils_headers anvutils_common)
