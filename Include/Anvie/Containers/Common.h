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
 * } MyStruct;
 * @endcode
 * To copy this, if we directly do a @c memcpy, the data pointed by @c memptr
 * is not copied and just pointer value is copied. This might lead to
 * memory bugs.
 *
 * It's now responsibility of the copy callback to create
 * a new copy of data into dst. Necessary memory is already allocated
 * for the top level struct and just memory of internal pointers needs
 * to be allocated.
 * @param dst Make a copy of @c src in @c dst. A @c type* pointer.
 * @param src Element to make copy for. A @c type* pointer or @c type value.
 * @param udata User data passed to this copy constructor.
 * */
typedef void (*CreateElementCopyCallback)(void* dst, void* src, void* udata);

/**
 * Similar to @c ElementCopyCallback, this will be used to free/destroy
 * the copied element.
 * If copy callback is set then free must also be set.
 * @param copy Copy of element to be destroyed. A @c type* pointer.
 * @param udata User data passed to this copy destructor.
 * */
typedef void (*DestroyElementCopyCallback)(void* copy, void* udata);

/**
 * Typedef for callback functions that take an element print it.
 * @param element Element to be printed. A @c type* pointer or @c type value.
 * @param pos Positon of element in the array.
 * @param udata User data passed to this print callback.
 * */
typedef void (*PrintElementCallback)(void* element, Size pos, void* udata);

/**
 * To filter elements in a container.
 * @param element, Element passed to callback from container.
 * A @c type* pointer or @c type value.
 * @param udata User data passed to filter function.
 * */
typedef Bool (*FilterElementCallback)(void* element, void* udata);

/**
 * Compare two provided elements.
 * @param p1 A @c type* pointer or @c type value.
 * @param p2 A @c type* pointer or @c type value.
 * @param udata User data passed to this copy constructor.
 * @return If return > 1 then @p p1 > @p p2.
 * If return < 1  then @p p1 is less than @p p2.
 * If return == 0 then @p p1 and @p p2 are equivalent
 * */
typedef Int32 (*CompareElementCallback)(void* p1, void* p2, void* udata);

void print_i8(void* x, Size idx, void* udata);
void print_i16(void* x, Size idx, void* udata);
void print_i32(void* x, Size idx, void* udata);
void print_i64(void* x, Size idx, void* udata);

void print_u8(void* x, Size idx, void* udata);
void print_u16(void* x, Size idx, void* udata);
void print_u32(void* x, Size idx, void* udata);
void print_u64(void* x, Size idx, void* udata);

void print_string(void* x, Size idx, void* udata);
void string_create_copy(void* to, void* data, void* udata);
void string_destroy_copy(void* data, void* udata);

#endif // ANVIE_UTILS_COMMON_H
