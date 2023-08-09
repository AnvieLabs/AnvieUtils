/**
 * @file AnvHelperDefines.h
 * @date Wed, 5th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Defines some helper macros for making tasks
 * like debug printing, etc... easy.
 * */

#ifndef AVHELPERDEFINE_H_
#define AVHELPERDEFINE_H_

#include <stdlib.h>
#include <Anvie/TerminalColors.h>

#ifndef ANV_MODULE_NAME
#define ANV_MODULE_NAME "AnvUtils"
#endif//ANV_MODULE_NAME

#define LINE80() println("================================================================================")
#define newline() putchar(0xa)

// strings for some error types
#define ERR_OUT_OF_MEMORY "out of memory (allocation failed)\n"
#define ERR_INVALID_ARGUMENTS "invalid arguments\n"
#define ERR_FUNCTION_INTERNAL_ERROR "internal function error\n"
#define ERR_CONTAINER_UNDERFLOW "container underflow\n"

// allocation heloers
#define ALLOCATE(type, count) (type*)calloc(count, sizeof(type))
#define NEW(type) ALLOCATE(type, 1)
#define FREE(x) free((void*)x)

// print helpers
#define print(...) printf(__VA_ARGS__)
#define println(...) printf(__VA_ARGS__); putchar('\n')

// use when a variable is left unused intentionally and you
// don't want compiler to complain about it
#define UNUSED(x) (void)(x)

// use when a function is to be forced to be inlined
#define FORCE_INLINE inline __attribute__((always_inline))

// print an error message
#define ERR(tag, ...) do {                                              \
    fprintf(stderr, COLOR_BOLD_RED"[-]"COLOR_RESET " [" COLOR_BOLD_BLUE ANV_MODULE_NAME COLOR_RESET "] ["COLOR_BOLD_RED" %s "COLOR_RESET"] : ", tag); \
    fprintf(stderr, __VA_ARGS__);                                        \
        } while(0)

// print a debug message
#define DBG(tag, ...) do {                                              \
    fprintf(stderr, COLOR_BOLD_YELLOW"[!]"COLOR_RESET " [" COLOR_BOLD_BLUE ANV_MODULE_NAME COLOR_RESET "] ["COLOR_BOLD_YELLOW" %s "COLOR_RESET"] : ", tag); \
    fprintf(stderr, __VA_ARGS__);                                       \
        } while(0)

// print a success message
#define OK(tag, ...) do {                                                   \
    fprintf(stderr, COLOR_BOLD_GREEN"[+]"COLOR_RESET " [" COLOR_BOLD_BLUE ANV_MODULE_NAME COLOR_RESET "] ["COLOR_BOLD_GREEN" %s "COLOR_RESET"] : ", tag); \
    fprintf(stderr, __VA_ARGS__);                                       \
        } while(0)

// make an assertion
#define ASSERT(cond, ...)                           \
    if(!(cond)) {                                   \
        ERR(__FUNCTION__, __VA_ARGS__);             \
        exit(1);                                    \
    }

// print success if condition is met
#define SUCCESS_IF(cond, ...)                          \
    if(cond) {                                         \
        OK(__FUNCTION__, __VA_ARGS__);                 \
    }

// print debug if condition is met
#define WARN_IF(cond, ...)                             \
    if(cond) {                                         \
        DBG(__FUNCTION__, __VA_ARGS__);                \
    }

// print error if condition is met
#define FATAL_IF(cond, ...)                         \
    if(cond) {                                      \
        ERR(__FUNCTION__, __VA_ARGS__);             \
    }


// return if condition fails to be true
#define RETURN_IF_FAIL(cond, ...)                      \
    {                                                  \
        Bool ________res____ = !(cond);                \
        FATAL_IF(________res____, __VA_ARGS__);        \
    }

// return value if condition fails to be true
#define RETURN_VALUE_IF_FAIL(cond, value, ...)                       \
    {                                                                \
        Bool _________res_____ = !(cond);                            \
        FATAL_IF(_________res_____, __VA_ARGS__);                    \
        if(_________res_____) return value;                          \
    }

// get array size
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
// ceate packed enums, unions, structs etc...
#define PACKED __attribute__((__packed__))

#endif // AVHELPERDEFINE_H_
