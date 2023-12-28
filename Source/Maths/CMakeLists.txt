file(GLOB_RECURSE UTILS_MATHS_SRCS ${CMAKE_CURRENT_SOURCE_DIR} "*.c")
add_library(anvutils_maths ${UTILS_MATHS_SRCS})
target_link_libraries(anvutils_maths anvutils_headers anvutils_common m)
