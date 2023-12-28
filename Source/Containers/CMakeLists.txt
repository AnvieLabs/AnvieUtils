file(GLOB_RECURSE UTILS_CONTAINERS_SRCS ${CMAKE_CURRENT_SOURCE_DIR} "*.c")
add_library(anvutils_containers ${UTILS_CONTAINERS_SRCS})
target_link_libraries(anvutils_containers anvutils_headers anvutils_common)
