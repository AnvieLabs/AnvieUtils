/**
 * @file Vector.h
 * @date Mon, 10th July, 2023
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
 * @brief Dynamic array style generic container.
 * To define vectors of different types, use the macros defined in
 * `VectorInterface.h` and use the corresponding functions only.
 * There are no runtime costs for using the designated functions as they are
 * always inlined.
 * */

#ifndef UTILS_VECTOR_H
#define UTILS_VECTOR_H

#include <Anvie/Types.h>
#include <Anvie/Containers/Common.h>

/**
 * Represents a Dynamic Array.
 *
 * CREATION SEMANTICS
 * Initially the size of vector is some base value which is not 0.
 * This can be set in vector.c and experimented upon.
 *
 * INSERTION SEMANTICS
 * - if @c element_size is less than or equal to 8, then it's treated
 *   as a value array and data is just set as if it were a value array.
 *   This means all void* will be treated as Uint8, Uint16, Uint32 or Uint64
 *   depending on size of element.
 * - if @c element_size is more than 8 then it's treated as a structure
 *   array and data is copied to internal memory using @c memcpy.
 * - if @c copy is set, it will ALWAYS be used to create copy of data,
 *   else it's one of the methods described above.
 * This means, no matter what, @c vector will always create it's own
 * copy of data in order to avoid memory errors.
 *
 * RESIZE SEMANTICS
 * - @c resize_factor decides what the new size of new allocation will be.
 * - new_size = old_size * (1 + resize_factor)
 * - this means a @c resize_factor of 1 means 2x resize and 2 means 3x resize.
 * - if @c free_when_possible is set to @c True, then array will autmatically be
 *   freed if new size is less than capcity/2. It is set to True by default.
 * */
typedef struct anvie_dynamic_array_t {
    Size                          element_size; /**< size of each element in array */
    Size                          length; /**< number of active insertions, counted in number of elements */
    Size                          capacity; /**< total number of elements array can hold */
    UByteArray                    data; /**< data of array in form of Uint8 array */
    CreateElementCopyCallback  create_copy; /**< copy constructor functor */
    DestroyElementCopyCallback destroy_copy; /**< copy destructor functor */
    Float32                       resize_factor; /**< percent factor for resizing arrays, by defualt it's 1, this means 2x resize */
} Vector;

#define vector_at(vec, type, pos) ((type*)((vec)->data))[pos]
#define vector_address_at(vec, pos) ((vec)->data + (pos) * (vec)->element_size)
#define vector_length(vec) ((vec)->length)
#define vector_element_size(vec) ((vec)->element_size)

Vector* vector_create (
    Size element_size,
    CreateElementCopyCallback create_copy,
    DestroyElementCopyCallback destroy_copy
);
void vector_destroy(Vector* vec);

Vector* vector_clone(Vector* vec);

void vector_resize(Vector* vec, Size new_size);
void vector_reserve(Vector* vec, Size capacity);
void vector_clear(Vector* vec);

Vector* vector_get_subvector(Vector* vec, Size start, Size size);

void vector_copy(Vector* vec, Size to, Size from);
void vector_move(Vector* vec, Size to, Size from);
void vector_overwrite(Vector* vec, Size pos, void* data);

void  vector_insert(Vector* vec, void* data, Size pos);
void  vector_delete(Vector* vec, Size pos);
void* vector_remove(Vector* vec, Size pos);

void  vector_insert_fast(Vector* vec, void* data, Size pos);
void  vector_delete_fast(Vector* vec, Size pos);
void* vector_remove_fast(Vector* vec, Size pos);

void  vector_push_front(Vector* vec, void* data);
void* vector_pofront(Vector* vec);

void  vector_push_front_fast(Vector* vec, void* data);
void* vector_pofront_fast(Vector* vec);

void  vector_push_back(Vector* vec, void* data);
void* vector_poback(Vector* vec);

void* vector_peek(Vector* vec, Size pos);
void* vector_front(Vector* vec);
void* vector_back(Vector* vec);

void vector_print(Vector* vec, PrintElementCallback printer);

void vector_merge(Vector* vec, Vector* vec_other);
Vector* vector_filter(Vector* vec, FilterElementCallback filter, void* user_data);
// TODO: accumulate, sort, iterators, intersection, duplicate, slice, insert_range

void vector_swap(Vector* vec, Size p1, Size p2);
void vector_sort(Vector* vec, CompareElementCallback compare);
Bool vector_check_sorted(Vector* vec, CompareElementCallback compare);

// sorting algorithms
void vector_insertion_sort(Vector* vec, CompareElementCallback compare);
void vector_bubble_sort(Vector* vec, CompareElementCallback compare);
void vector_merge_sort(Vector* vec, CompareElementCallback compare);

/*---------------- DEFINE COMMON INTERFACES FOR TYPE-SAFETY-----------------*/

#include <Anvie/Containers/Interface/Vector.h>

DEF_INTEGER_VECTOR_INTERFACE(u8,  Uint8);
DEF_INTEGER_VECTOR_INTERFACE(u16, Uint16);
DEF_INTEGER_VECTOR_INTERFACE(u32, Uint32);
DEF_INTEGER_VECTOR_INTERFACE(u64, Uint64);

DEF_INTEGER_VECTOR_INTERFACE(i8,  Int8);
DEF_INTEGER_VECTOR_INTERFACE(i16, Int16);
DEF_INTEGER_VECTOR_INTERFACE(i32, Int32);
DEF_INTEGER_VECTOR_INTERFACE(i64, Int64);

DEF_INTEGER_VECTOR_INTERFACE(f32, Float32);
DEF_INTEGER_VECTOR_INTERFACE(f64, Float64);

DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(string, String, string_create_copy, string_destroy_copy);
DEF_INTEGER_VECTOR_INTERFACE(voidptr, void*);

// define interface to contain vector of vectors
void vector_create_copy(void* dst, void* src);
void vector_destroy_copy(void* copy);
DEF_STRUCT_VECTOR_INTERFACE(vector, Vector, vector_create_copy, vector_destroy_copy);
DEF_INTEGER_VECTOR_INTERFACE(pvector, Vector*);

#endif // UTILS_VECTOR_H
