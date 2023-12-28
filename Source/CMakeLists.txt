add_subdirectory(Containers)
add_subdirectory(Allocators)
add_subdirectory(Maths)
add_subdirectory(Tests)

FILE(GLOB ANVUTILS_COMMON_SRCS ${CMAKE_CURRENT_SOURCE_DIR} "*.c")
add_library(anvutils_common ${ANVUTILS_COMMON_SRCS})
target_link_libraries(anvutils_common anvutils_headers)
