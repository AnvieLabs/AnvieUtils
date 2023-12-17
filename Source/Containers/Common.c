/**
 * @file Common.c
 * @date Mon, 25th September, 2023
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
 * @brief Implementation of common functions required by all containers.
 * */

#include <Anvie/Containers/Common.h>
#include <Anvie/HelperDefines.h>

#include <string.h>

/**
 * Copy constructor for strings
 * @param to Pointer where new created string will be stored
 * @param data String to be copied
 * @param udata Unused here
 * */
void string_create_copy(void* dst, void* src, void* udata) {
    UNUSED(udata);
    if(dst && src) *(String*)dst = strdup((String)src);
}

/**
 * Destroy given string
 * @param copy Pointer to copy of string to be destroyed.
 * @param udata Unused here
 * */
void string_destroy_copy(void* copy, void* udata){
    UNUSED(udata);
    if(copy) FREE(*(String*)copy);
}

/**
 * @brief Prints a signed 8-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_i8(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%d, ", (Int32)(Int64)x);
}

/**
 * @brief Prints a signed 16-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_i16(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%d, ", (Int32)(Int64)x);
}

/**
 * @brief Prints a signed 32-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_i32(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%d, ", (Int32)(Int64)x);
}

/**
 * @brief Prints a signed 64-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_i64(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%ld, ", (Int64)x);
}

/**
 * @brief Prints an unsigned 8-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_u8(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%u, ", (Uint32)(Uint64)x);
}

/**
 * @brief Prints an unsigned 16-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_u16(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%u, ", (Uint32)(Uint64)x);
}

/**
 * @brief Prints an unsigned 32-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_u32(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%u, ", (Uint32)(Uint64)x);
}

/**
 * @brief Prints an unsigned 64-bit integer value.
 * @param x Pointer to the value to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_u64(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%lu, ", (Uint64)x);
}

/**
 * @brief Prints a string value.
 * @param x Pointer to the string to be printed.
 * @param idx Index (unused in this implementation).
 * @param udata User data (unused in this implementation).
 */
void print_string(void* x, Size idx, void* udata) {
    UNUSED(idx && udata);
    printf("%s ,", (String)x);
}

/**
 * Hashes an 8-bit unsigned integer value.
 * @param val The value to be hashed.
 * @param udata User data (unused in this implementation).
 * @return The hashed value.
 */
Uint64 hash_u8(Uint64 val, void* udata) {
    UNUSED(udata);
    return val % UINT64_MAX;
}

/**
 * Hashes a 16-bit unsigned integer value.
 * @param val The value to be hashed.
 * @param udata User data (unused in this implementation).
 * @return The hashed value.
 */
Uint64 hash_u16(Uint64 val, void* udata) {
    UNUSED(udata);
    return val % UINT64_MAX;
}

/**
 * Hashes a 32-bit unsigned integer value.
 * @param val The value to be hashed.
 * @param udata User data (unused in this implementation).
 * @return The hashed value.
 */
Uint64 hash_u32(Uint64 val, void* udata) {
    UNUSED(udata);
    return val % UINT64_MAX;
}

/**
 * Hashes a 64-bit unsigned integer value.
 * @param val The value to be hashed.
 * @param udata User data (unused in this implementation).
 * @return The hashed value.
 */
Uint64 hash_u64(Uint64 val, void* udata) {
    UNUSED(udata);
    return val % UINT64_MAX;
}

/**
 * Hashes a string.
 * @param val The string to be hashed.
 * @param udata User data (unused in this implementation).
 * @return The hashed value.
 */
Uint64 hash_string(String val, void* udata) {
    UNUSED(udata);
    Uint64 hash = 0;
    int i = 0;
    while (val[i] != '\0') {
        hash = hash * 31 + val[i];
        i++;
    }
    return hash;
}

/**
 * Compares two 8-bit unsigned integer values.
 * @param v1 First value to compare.
 * @param v2 Second value to compare.
 * @param udata User data (unused in this implementation).
 * @return 0 if values are equal, negative if v1 < v2, positive if v1 > v2.
 */
Int32 compare_u8(Uint64 v1, Uint64 v2, void* udata) {
    UNUSED(udata);
    return (Int32)(v1 - v2);
}

/**
 * Compares two 16-bit unsigned integer values.
 * @param v1 First value to compare.
 * @param v2 Second value to compare.
 * @param udata User data (unused in this implementation).
 * @return 0 if values are equal, negative if v1 < v2, positive if v1 > v2.
 */
Int32 compare_u16(Uint64 v1, Uint64 v2, void* udata) {
    UNUSED(udata);
    return (Int32)(v1 - v2);
}

/**
 * Compares two 32-bit unsigned integer values.
 * @param v1 First value to compare.
 * @param v2 Second value to compare.
 * @param udata User data (unused in this implementation).
 * @return 0 if values are equal, negative if v1 < v2, positive if v1 > v2.
 */
Int32 compare_u32(Uint64 v1, Uint64 v2, void* udata) {
    UNUSED(udata);
    return (Int32)(v1 - v2);
}

/**
 * Compares two 64-bit unsigned integer values.
 * @param v1 First value to compare.
 * @param v2 Second value to compare.
 * @param udata User data (unused in this implementation).
 * @return 0 if values are equal, negative if v1 < v2, positive if v1 > v2.
 */
Int32 compare_u64(Uint64 v1, Uint64 v2, void* udata) {
    UNUSED(udata);
    if(v1 == v2) {
        return 0;
    } else if(v1 > v2) {
        return 1;
    } else {
        return -1;
    }
}
