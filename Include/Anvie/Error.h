/**
 * @file Error.h
 * @date Sun, 24th December, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2023 Siddharth Mishra
 * @copyright Copyright 2023 Anvie Labs
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @brief Standard error messages used across the program
 * */

#ifndef ANVIE_ERROR_MESSAGES_H
#define ANVIE_ERROR_MESSAGES_H


#include <Anvie/HelperDefines.h>
#include <Anvie/Types.h>

/**
 * Standard error defines
 * */
typedef enum Error {
    ERR_OUT_OF_MEMORY,
    ERR_INVALID_ARGUMENTS,
    ERR_FUNCTION_INTERNAL_ERROR,
    ERR_CONTAINER_UNDERFLOW,
    ERR_INVALID_CONTENTS,
    ERR_INVALID_LENGTH,
    ERR_INVALID_CAPACITY,
    ERR_INVALID_DATA_PTR,
    ERR_INVALID_OBJECT,
    ERR_INVALID_INDEX,
    ERR_INVALID_USER_DATA,
    ERR_REQUIRED_EQUALITY_FAILED,
    ERR_TYPE_MISMATCH,
    ERR_OPERATION_FAILED,
    ERR_UNEXPECTED,
    ERR_UNKNOWN,
} Error;

#define ERRMSG_OUT_OF_MEMORY            "Out of memory (allocation failed)"
#define ERRMSG_INVALID_ARGUMENTS        "Invalid argument(s)"
#define ERRMSG_FUNCTION_INTERNAL_ERROR  "Internal function error"
#define ERRMSG_CONTAINER_UNDERFLOW      "Container underflow (access to memory in unxpected manner)"
#define ERRMSG_INVALID_LENGTH           "Length is invalid (not what was expected)"
#define ERRMSG_INVALID_CONTENTS         "Contents are invalid (not what was expected)"
#define ERRMSG_INVALID_CAPACITY         "Capacity is invalid (not what was expected)"
#define ERRMSG_INVALID_DATA_PTR         "Data pointer is invalid (NULL)"
#define ERRMSG_INVALID_OBJECT           "Invalid object (NULL)"
#define ERRMSG_INVALID_INDEX            "Invalid index (outside possible range)"
#define ERRMSG_INVALID_USER_DATA        "Invalid user data (passed to callback)"
#define ERRMSG_REQUIRED_EQUALITY_FAILED "Required equality failed"
#define ERRMSG_TYPE_MISMATCH            "Type mismatch"
#define ERRMSG_OPERATION_FAILED         "Operation failed"
#define ERRMSG_UNEXPECTED               "Something unexpected happened"
#define ERRMSG_UNKNOWN                  "Undefined/Unknown error"

ZString err_to_zstr(Error err);
#define ERRMSG(err) err_to_zstr(err)

#define ERRFMT "%s\n"
#define ERR_RETURN_IF_FAIL(cond, err) RETURN_IF_FAIL((cond), ERRFMT, ERRMSG(err))
#define ERR_RETURN_VALUE_IF_FAIL(cond, value, err) RETURN_VALUE_IF_FAIL((cond), (value), ERRFMT, ERRMSG(err))

// print an error message
#define ERR(tag, ...) do {                                              \
    fprintf(stderr, COLOR_BOLD_RED"[-]"COLOR_RESET " [" COLOR_BOLD_BLUE MODULE_NAME COLOR_RESET "] ["COLOR_BOLD_RED" %s "COLOR_RESET"] : ", tag); \
    fprintf(stderr, __VA_ARGS__);                                        \
        } while(0)

// print a debug message
#define DBG(tag, ...) do {                                              \
    fprintf(stderr, COLOR_BOLD_YELLOW"[!]"COLOR_RESET " [" COLOR_BOLD_BLUE MODULE_NAME COLOR_RESET "] ["COLOR_BOLD_YELLOW" %s "COLOR_RESET"] : ", tag); \
    fprintf(stderr, __VA_ARGS__);                                       \
        } while(0)

// print a success message
#define OK(tag, ...) do {                                                   \
    fprintf(stderr, COLOR_BOLD_GREEN"[+]"COLOR_RESET " [" COLOR_BOLD_BLUE MODULE_NAME COLOR_RESET "] ["COLOR_BOLD_GREEN" %s "COLOR_RESET"] : ", tag); \
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
        if(________res____) return;                    \
    }

// return value if condition fails to be true
#define RETURN_VALUE_IF_FAIL(cond, value, ...)                       \
    {                                                                \
        Bool _________res_____ = !(cond);                            \
        FATAL_IF(_________res_____, __VA_ARGS__);                    \
        if(_________res_____) return value;                          \
    }

#define GOTO_LABEL_IF_FAIL(cond, label, ...)                         \
    {                                                                \
        Bool _________res_____ = !(cond);                            \
        FATAL_IF(_________res_____, __VA_ARGS__);                    \
        if(_________res_____) goto label;                            \
    }

#endif // ANVIE_ERROR_MESSAGES_H
