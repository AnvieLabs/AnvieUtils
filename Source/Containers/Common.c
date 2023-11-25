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

void print_i8(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%d, ", (Int32)(Int64)x); }
void print_i16(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%d, ", (Int32)(Int64)x); }
void print_i32(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%d, ", (Int32)(Int64)x); }
void print_i64(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%ld, ", (Int64)x); }

void print_u8(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%u, ", (Uint32)(Uint64)x); }
void print_u16(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%u, ", (Uint32)(Uint64)x); }
void print_u32(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%u, ", (Uint32)(Uint64)x); }
void print_u64(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%lu, ", (Uint64)x); }

void print_string(void* x, Size idx, void* udata) { UNUSED(idx && udata); printf("%s ,", (String)x); }
