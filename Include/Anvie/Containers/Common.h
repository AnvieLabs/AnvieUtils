/**
 * @file Common.h
 * @date Mon, 13th July, 2023
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
 * @brief Defines common types/defines for different container types.
 * */

#ifndef ANVIE_UTILS_COMMON_H
#define ANVIE_UTILS_COMMON_H

#include <Anvie/Types.h>

/**
 * Will create a copy of data structure given in src to dst.
 *
 * Consider this struct for eg:
 * @code{c}
 * typedef struct anvie_my_struct_t {
 *     void* memptr;
 *     Size size;
 * } AnvMyStruct;
 * @endcode
 * To copy this, if we directly do a @c memcpy, the data pointed by @c memptr
 * is not copied and just pointer value is copied. This might lead to
 * memory bugs.
 *
 * It's now responsibility of the copy callback to create
 * a new copy of data into dst. Necessary memory is already allocated
 * for the top level struct and just memory of internal pointers needs
 * to be allocated.
 *
 * @param dst Make a copy of @c src in @c dst.
 * @param src Element to make copy for.
 * */
typedef void (*AnvCreateElementCopyCallback)(void* dst, void* src);

/**
 * Similar to @c AnvElementCopyCallback, this will be used to free/destroy
 * the copied element.
 * If copy callback is set then free must also be set.
 * @param copy Copy of element to be destroyed.
 * */
typedef void (*AnvDestroyElementCopyCallback)(void* copy);

/**
 * Typedef for callback functions that take an element print it.
 * @param p_element Element to be printed.
 * @param pos Positon of element in the array.
 * */
typedef void (*AnvPrintElementCallback)(void* p_element, Size pos);

/**
 * To filter elements in a container.
 * @param p_element, Element passed to callback from container.
 * @param p_user_data User data passed to filter function.
 * */
typedef Bool (*AnvFilterElementCallback)(void* p_element, void* p_user_data);

/**
 * Compare two provided elements.
 * If return > 1 then first element is greater than second element.
 * If return < 1 then first element is less than second element.
 * If return == 0 then first element and second element are equivalent
 * */
typedef Int32 (*AnvCompareElementCallback)(void* p1, void* p2);

void anv_print_i8(void* x, Size idx);
void anv_print_i16(void* x, Size idx);
void anv_print_i32(void* x, Size idx);
void anv_print_i64(void* x, Size idx);

void anv_print_u8(void* x, Size idx);
void anv_print_u16(void* x, Size idx);
void anv_print_u32(void* x, Size idx);
void anv_print_u64(void* x, Size idx);

void anv_print_string(void* x, Size idx);
void anv_string_create_copy(void* to, void* p_data);
void anv_string_destroy_copy(void* p_data);

#endif // ANVIE_UTILS_COMMON_H
