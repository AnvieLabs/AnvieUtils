file(GLOB_RECURSE ANV_UTILS_MATHS_SRCS ${CMAKE_CURRENT_SOURCE_DIR} "*.c")
add_library(anvutils_maths ${ANV_UTILS_MATHS_SRCS})
target_link_libraries(anvutils_maths m)
