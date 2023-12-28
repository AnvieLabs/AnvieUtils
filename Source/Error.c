/**
 * @file Error.c
 * @date Wed, 27th December, 2023
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

#include <Anvie/Error.h>

static ZString errmsgs[] = {
    [ERR_OUT_OF_MEMORY]            = ERRMSG_OUT_OF_MEMORY,
    [ERR_INVALID_ARGUMENTS]        = ERRMSG_INVALID_ARGUMENTS,
    [ERR_FUNCTION_INTERNAL_ERROR]  = ERRMSG_FUNCTION_INTERNAL_ERROR,
    [ERR_CONTAINER_UNDERFLOW]      = ERRMSG_CONTAINER_UNDERFLOW,
    [ERR_INVALID_CONTENTS]         = ERRMSG_INVALID_CONTENTS,
    [ERR_INVALID_LENGTH]           = ERRMSG_INVALID_LENGTH,
    [ERR_INVALID_CAPACITY]         = ERRMSG_INVALID_CAPACITY,
    [ERR_INVALID_DATA_PTR]         = ERRMSG_INVALID_DATA_PTR,
    [ERR_INVALID_OBJECT]           = ERRMSG_INVALID_OBJECT,
    [ERR_INVALID_INDEX]            = ERRMSG_INVALID_INDEX,
    [ERR_INVALID_USER_DATA]        = ERRMSG_INVALID_USER_DATA,
    [ERR_REQUIRED_EQUALITY_FAILED] = ERRMSG_REQUIRED_EQUALITY_FAILED,
    [ERR_TYPE_MISMATCH]            = ERRMSG_TYPE_MISMATCH,
    [ERR_OPERATION_FAILED]         = ERRMSG_OPERATION_FAILED,
    [ERR_UNEXPECTED]               = ERRMSG_UNEXPECTED,
    [ERR_UNKNOWN]                  = ERRMSG_UNKNOWN
};

/**
 * Convert error enum to error message.
 * @param err
 * @return ZString
 * */
ZString err_to_zstr(Error err) {
    if (err >= 0 && err <= ERR_UNKNOWN) {
        return errmsgs[err];
    } else {
        return ERRMSG_UNKNOWN;
    }
}
