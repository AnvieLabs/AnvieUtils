file(GLOB_RECURSE UTILS_CONTAINERS_SRCS ${CMAKE_CURRENT_SOURCE_DIR} "*.c")
add_library(anvutils_containers ${UTILS_CONTAINERS_SRCS})
target_link_libraries(anvutils_containers anvutils_headers anvutils_common)

# Add a custom target to generate preprocessed output
add_custom_target(generate_pheaders
    COMMAND ${CMAKE_C_COMPILER} -E -P ${CMAKE_SOURCE_DIR}/Modules/Utils/Source/Tests/UnitTest.c -o ${CMAKE_BINARY_DIR}/pheaders_output.i -I${CMAKE_SOURCE_DIR}/Modules/Utils/Include
    COMMENT "Generating pheaders file"
    VERBATIM
)
