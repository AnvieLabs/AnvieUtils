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
#define anv_vector_AddressAt(p_vec, pos) ((p_vec)->p_data + (pos) * (p_vec)->element_size)
#define anv_vector_Length(p_vec) ((p_vec)->length)
#define anv_vector_ElementSize(p_vec) ((p_vec)->element_size)

AnvVector*  anv_vector_create(Size element_size,
                             AnvCreateElementCopyCallback pfn_create_copy,
                             AnvDestroyElementCopyCallback pfn_destroy_copy);
void  anv_vector_destroy(AnvVector* p_vec);
void  anv_vector_resize(AnvVector* p_vec, Size new_size);
void  anv_vector_clear(AnvVector* p_vec);

void  anv_vector_copy(AnvVector* p_vec, Size to, Size from);
void  anv_vector_move(AnvVector* p_vec, Size to, Size from);
void  anv_vector_overwrite(AnvVector* p_vec, Size pos, void* p_data);

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

void  anv_vector_print(AnvVector* p_vec, AnvPrintElementCallback pfn_printer);

/*---------------- DEFINE COMMON INTERFACES FOR TYPE-SAFETY-----------------*/

#include <Anvie/Containers/VectorInterface.h>

DEF_ANV_INTEGER_VECTOR_INTERFACE(u8,  Uint8);
DEF_ANV_INTEGER_VECTOR_INTERFACE(u16, Uint16);
DEF_ANV_INTEGER_VECTOR_INTERFACE(u32, Uint32);
DEF_ANV_INTEGER_VECTOR_INTERFACE(u64, Uint64);

DEF_ANV_INTEGER_VECTOR_INTERFACE(i8,  Int8);
DEF_ANV_INTEGER_VECTOR_INTERFACE(i16, Int16);
DEF_ANV_INTEGER_VECTOR_INTERFACE(i32, Int32);
DEF_ANV_INTEGER_VECTOR_INTERFACE(i64, Int64);

#endif // ANV_VECTOR_H
