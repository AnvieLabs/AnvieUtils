cmake_minimum_required(VERSION 3.5)

project(AnvieUtils VERSION 0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

 set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -ggdb -fsanitize=address")
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -ggdb")

# For GCC or Clang
# if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
#     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native")
# endif()

include_directories("Include")
add_subdirectory("Source")
#add_subdirectory("Tests")

add_library(anvutils_headers INTERFACE)
target_include_directories(anvutils_headers INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Set the input header file and output file
file(GLOB_RECURSE INPUT_HEADER_FILE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

# Loop through each header file
foreach(INPUT_HEADER_FILE IN LISTS INPUT_HEADER_FILES)
    # Set the output file for each header
    set(OUTPUT_PREPROCESSED_FILE "${INPUT_HEADER_FILE}_preprocessed.h")

    # Create a custom command to preprocess the file
    add_custom_command(
        OUTPUT ${OUTPUT_PREPROCESSED_FILE}
        COMMAND ${CMAKE_C_COMPILER} -E ${INPUT_HEADER_FILE} > ${OUTPUT_PREPROCESSED_FILE}
        DEPENDS ${INPUT_HEADER_FILE}
        COMMENT "Generating preprocessed file: ${OUTPUT_PREPROCESSED_FILE}"
        VERBATIM
    )

    # Add a custom target for each header
    add_custom_target(preprocess_target_${INPUT_HEADER_FILE} DEPENDS ${OUTPUT_PREPROCESSED_FILE})

    # Add dependencies to preprocess_target
    add_dependencies(preprocess_target preprocess_target_${INPUT_HEADER_FILE})

    # Include preprocessed files in the compilation of the main target
    # Replace "your_target_name" with the name of your actual target
    target_compile_options(your_target_name PRIVATE "-include ${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PREPROCESSED_FILE}")
endforeach()
