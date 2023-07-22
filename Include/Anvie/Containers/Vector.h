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
 * Initially the size of AnvVector is some base value which is not 0.
 * This can be set in AnvVector.c and experimented upon.
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
 * This means, no matter what, @c AnvVector will always create it's own
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

#define AnvVector_At(p_vec, type, pos) ((type*)((p_vec)->p_data))[pos]
#define AnvVector_AddressAt(p_vec, pos) ((p_vec)->p_data + (pos) * (p_vec)->element_size)
#define AnvVector_Length(p_vec) ((p_vec)->length)
#define AnvVector_ElementSize(p_vec) ((p_vec)->element_size)

AnvVector*  AnvVector_Create(Size element_size,
                             AnvCreateElementCopyCallback pfn_create_copy,
                             AnvDestroyElementCopyCallback pfn_destroy_copy);
void  AnvVector_Destroy(AnvVector* p_vec);
void  AnvVector_Resize(AnvVector* p_vec, Size new_size);
void  AnvVector_Clear(AnvVector* p_vec);

void  AnvVector_Copy(AnvVector* p_vec, Size to, Size from);
void  AnvVector_Move(AnvVector* p_vec, Size to, Size from);
void  AnvVector_Overwrite(AnvVector* p_vec, Size pos, void* p_data);

void  AnvVector_Insert(AnvVector* p_vec, void* p_data, Size pos);
void  AnvVector_Delete(AnvVector* p_vec, Size pos);
void* AnvVector_Remove(AnvVector* p_vec, Size pos);

void  AnvVector_InsertFast(AnvVector* p_vec, void* p_data, Size pos);
void  AnvVector_DeleteFast(AnvVector* p_vec, Size pos);
void* AnvVector_RemoveFast(AnvVector* p_vec, Size pos);

void  AnvVector_PushFront(AnvVector* p_vec, void* p_data);
void* AnvVector_PopFront(AnvVector* p_vec);

void  AnvVector_PushFrontFast(AnvVector* p_vec, void* p_data);
void* AnvVector_PopFrontFast(AnvVector* p_vec);

void  AnvVector_PushBack(AnvVector* p_vec, void* p_data);
void* AnvVector_PopBack(AnvVector* p_vec);

void* AnvVector_Peek(AnvVector* p_vec, Size pos);
void* AnvVector_Front(AnvVector* p_vec);
void* AnvVector_Back(AnvVector* p_vec);

void  AnvVector_Print(AnvVector* p_vec, AnvPrintElementCallback pfn_printer);

/*---------------- DEFINE COMMON INTERFACES FOR TYPE-SAFETY-----------------*/

#include <Anvie/Containers/VectorInterface.h>

DEF_ANV_INTEGER_VECTOR_INTERFACE(U8,  Uint8);
DEF_ANV_INTEGER_VECTOR_INTERFACE(U16, Uint16);
DEF_ANV_INTEGER_VECTOR_INTERFACE(U32, Uint32);
DEF_ANV_INTEGER_VECTOR_INTERFACE(U64, Uint64);

DEF_ANV_INTEGER_VECTOR_INTERFACE(I8,  Int8);
DEF_ANV_INTEGER_VECTOR_INTERFACE(I16, Int16);
DEF_ANV_INTEGER_VECTOR_INTERFACE(I32, Int32);
DEF_ANV_INTEGER_VECTOR_INTERFACE(I64, Int64);

#endif // ANV_VECTOR_H
