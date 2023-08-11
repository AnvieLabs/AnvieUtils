/**
 * @file Vector.h
 * @date Mon, 10th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Dynamic Array Container.
 * */

#ifndef ANV_VECTOR_H
#define ANV_VECTOR_H

#include <Anvie/Types.h>
#include <Anvie/Containers/Common.h>
#include <string.h>

/**
 * Represents a Dynamic Array.
 *
 * CREATION SEMANTICS
 * Initially the size of anv_vector is some base value which is not 0.
 * This can be set in anv_vector.c and experimented upon.
 *
 * INSERTION SEMANTICS
 * - if @c element_size is less than or equal to 8, then it's treated
 *   as a value array and data is just set as if it were a value array.
 *   This means all void* will be treated as Uint8, Uint16, Uint32 or Uint64
 *   depending on size of element.
 * - if @c element_size is more than 8 then it's treated as a structure
 *   array and data is copied to internal memory using @c memcpy.
 * - if @c pfn_copy is set, it will ALWAYS be used to create copy of data,
 *   else it's one of the methods described above.
 * This means, no matter what, @c anv_vector will always create it's own
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
    UByteArray                    p_data; /**< data of array in form of Uint8 array */
    AnvCreateElementCopyCallback  pfn_create_copy; /**< copy constructor functor */
    AnvDestroyElementCopyCallback pfn_destroy_copy; /**< copy destructor functor */
    Float32                       resize_factor; /**< percent factor for resizing arrays, by defualt it's 1, this means 2x resize */
} AnvVector;

#define anv_vector_at(p_vec, type, pos) ((type*)((p_vec)->p_data))[pos]
#define anv_vector_address_at(p_vec, pos) ((p_vec)->p_data + (pos) * (p_vec)->element_size)
#define anv_vector_length(p_vec) ((p_vec)->length)
#define anv_vector_element_size(p_vec) ((p_vec)->element_size)

AnvVector* anv_vector_create(Size element_size,
                             AnvCreateElementCopyCallback pfn_create_copy,
                             AnvDestroyElementCopyCallback pfn_destroy_copy);
void anv_vector_destroy(AnvVector* p_vec);
void anv_vector_resize(AnvVector* p_vec, Size new_size);
void anv_vector_clear(AnvVector* p_vec);

void anv_vector_copy(AnvVector* p_vec, Size to, Size from);
void anv_vector_move(AnvVector* p_vec, Size to, Size from);
void anv_vector_overwrite(AnvVector* p_vec, Size pos, void* p_data);

void  anv_vector_insert(AnvVector* p_vec, void* p_data, Size pos);
void  anv_vector_delete(AnvVector* p_vec, Size pos);
void* anv_vector_remove(AnvVector* p_vec, Size pos);

void  anv_vector_insert_fast(AnvVector* p_vec, void* p_data, Size pos);
void  anv_vector_delete_fast(AnvVector* p_vec, Size pos);
void* anv_vector_remove_fast(AnvVector* p_vec, Size pos);

void  anv_vector_push_front(AnvVector* p_vec, void* p_data);
void* anv_vector_pop_front(AnvVector* p_vec);

void  anv_vector_push_front_fast(AnvVector* p_vec, void* p_data);
void* anv_vector_pop_front_fast(AnvVector* p_vec);

void  anv_vector_push_back(AnvVector* p_vec, void* p_data);
void* anv_vector_pop_back(AnvVector* p_vec);

void* anv_vector_peek(AnvVector* p_vec, Size pos);
void* anv_vector_front(AnvVector* p_vec);
void* anv_vector_back(AnvVector* p_vec);

void anv_vector_print(AnvVector* p_vec, AnvPrintElementCallback pfn_printer);

void anv_vector_merge(AnvVector* p_vec, AnvVector* p_vec_other);
AnvVector* anv_vector_filter(AnvVector* p_vec, AnvFilterElementCallback pfn_filter, void* p_user_data);
// TODO: accumulate, sort, iterators, intersection, duplicate, slice, insert_range

void anv_vector_swap(AnvVector* p_vec, Size p1, Size p2);
void anv_vector_sort(AnvVector* p_vec, AnvCompareElementCallback p_compare);
Bool anv_vector_check_sorted(AnvVector* p_vec, AnvCompareElementCallback p_compare);

// sorting algorithms
void anv_vector_insertion_sort(AnvVector* p_vec, AnvCompareElementCallback p_compare);
void anv_vector_bubble_sort(AnvVector* p_vec, AnvCompareElementCallback p_compare);

/*---------------- DEFINE COMMON INTERFACES FOR TYPE-SAFETY-----------------*/

#include <Anvie/Containers/VectorInterface.h>

inline void anv_print_i8(void* x, Size idx) { UNUSED(idx); printf("%d, ", (Int32)(Int64)x); }
inline void anv_print_i16(void* x, Size idx) { UNUSED(idx); printf("%d, ", (Int32)(Int64)x); }
inline void anv_print_i32(void* x, Size idx) { UNUSED(idx); printf("%d, ", (Int32)(Int64)x); }
inline void anv_print_i64(void* x, Size idx) { UNUSED(idx); printf("%ld, ", (Int64)x); }

inline void anv_print_u8(void* x, Size idx) { UNUSED(idx); printf("%u, ", (Uint32)(Uint64)x); }
inline void anv_print_u16(void* x, Size idx) { UNUSED(idx); printf("%u, ", (Uint32)(Uint64)x); }
inline void anv_print_u32(void* x, Size idx) { UNUSED(idx); printf("%u, ", (Uint32)(Uint64)x); }
inline void anv_print_u64(void* x, Size idx) { UNUSED(idx); printf("%lu, ", (Uint64)x); }

inline void anv_print_string(void* x, Size idx) { UNUSED(idx); printf("%s ,", (String)x); }

DEF_ANV_INTEGER_VECTOR_INTERFACE(u8,  Uint8);
DEF_ANV_INTEGER_VECTOR_INTERFACE(u16, Uint16);
DEF_ANV_INTEGER_VECTOR_INTERFACE(u32, Uint32);
DEF_ANV_INTEGER_VECTOR_INTERFACE(u64, Uint64);

DEF_ANV_INTEGER_VECTOR_INTERFACE(i8,  Int8);
DEF_ANV_INTEGER_VECTOR_INTERFACE(i16, Int16);
DEF_ANV_INTEGER_VECTOR_INTERFACE(i32, Int32);
DEF_ANV_INTEGER_VECTOR_INTERFACE(i64, Int64);

void anv_string_create_copy(void* to, void* p_data);
void anv_string_destroy_copy(void* p_data);

DEF_ANV_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(string, String, anv_string_create_copy, anv_string_destroy_copy);
DEF_ANV_INTEGER_VECTOR_INTERFACE(voidptr, void*);

#endif // ANV_VECTOR_H
