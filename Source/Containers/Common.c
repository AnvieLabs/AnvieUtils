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
 * @param p_data String to be copied
 * */
void anv_string_create_copy(void* to, void* p_data) {
    if(p_data) *(String*)to = strdup((String)p_data);
    else *(String*)to = NULL;
}

/**
 * Destroy given string
 * */
void anv_string_destroy_copy(void* p_data){
    if(p_data) FREE(*(String*)p_data);
}

void anv_print_i8(void* x, Size idx) { UNUSED(idx); printf("%d, ", (Int32)(Int64)x); }
void anv_print_i16(void* x, Size idx) { UNUSED(idx); printf("%d, ", (Int32)(Int64)x); }
void anv_print_i32(void* x, Size idx) { UNUSED(idx); printf("%d, ", (Int32)(Int64)x); }
void anv_print_i64(void* x, Size idx) { UNUSED(idx); printf("%ld, ", (Int64)x); }

void anv_print_u8(void* x, Size idx) { UNUSED(idx); printf("%u, ", (Uint32)(Uint64)x); }
void anv_print_u16(void* x, Size idx) { UNUSED(idx); printf("%u, ", (Uint32)(Uint64)x); }
void anv_print_u32(void* x, Size idx) { UNUSED(idx); printf("%u, ", (Uint32)(Uint64)x); }
void anv_print_u64(void* x, Size idx) { UNUSED(idx); printf("%lu, ", (Uint64)x); }

void anv_print_string(void* x, Size idx) { UNUSED(idx); printf("%s ,", (String)x); }
