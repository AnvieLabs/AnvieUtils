file(GLOB_RECURSE ANV_UTILS_TESTS_SRCS ${CMAKE_CURRENT_SOURCE_DIR} "*.c")
add_executable(anvutils_tests ${ANV_UTILS_TESTS_SRCS})
target_link_libraries(anvutils_tests anvutils_containers)
