/**
 * @file Vector.c
 * @date Mon, 10th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Dynamic Array Container.
 * */

#include <Anvie/Containers/Vector.h>
#include <Anvie/HelperDefines.h>
#include <string.h>

/**
 * To escape the check for whether array is allocated or not
 * everytime an element is inserted, we preallocate some memory.
 * This will reduce that one instruction from high frequency
 * array insertion.
 *
 * We can experiment with this value based on application usage
 * requirements.
 * */
#define AV_DYN_ARRAY_INIT_ELEMENT_COUNT 4
#define AV_DYN_ARRAY_DEFAULT_RESIZE_FACTOR 1
#define AV_DYN_ARRAY_DEFAULT_FREE_WHEN_POSSIBLE True

/**
 * Create a new dynamic array.
 * If any one of @c pfn_create_copy or @c pfn_destroy_copy is non null,
 * then both must be non null!
 *
 * @param element_size Size of each element in this dynamic array.
 * @param pfn_create_copy Copy constructor for elements. Can be NULL.
 * @param pfn_destroy_copy Copy destructor for elements. Can be NULL.
 * @return AnvVector* or NULL if allocation failed
 * */
AnvVector* AnvVector_Create(Size element_size,
                            AnvCreateElementCopyCallback pfn_create_copy,
                            AnvDestroyElementCopyCallback pfn_destroy_copy)
{
    RETURN_VALUE_IF_FAIL(element_size, NULL, ERR_INVALID_ARGUMENTS);

    // both must be null or non null at the same time
    Bool b1 = pfn_create_copy != NULL;
    Bool b2 = pfn_destroy_copy != NULL;
    RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL, ERR_INVALID_ARGUMENTS);

    // create a new dyn array object
    AnvVector* p_vec = NEW(AnvVector);
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_OUT_OF_MEMORY);

    // allocate initial memory
    p_vec->p_data = ALLOCATE(Uint8, AV_DYN_ARRAY_INIT_ELEMENT_COUNT * element_size);
    if(!p_vec->p_data) {
        WARN_IF(True, ERR_OUT_OF_MEMORY);
        FREE(p_vec);
        return NULL;
    }
    p_vec->capacity = AV_DYN_ARRAY_INIT_ELEMENT_COUNT;
    p_vec->element_size = element_size;
    p_vec->resize_factor = AV_DYN_ARRAY_DEFAULT_RESIZE_FACTOR;
    p_vec->pfn_create_copy = pfn_create_copy;
    p_vec->pfn_destroy_copy = pfn_destroy_copy;
    return p_vec;
}

/**
 * Destroy given dynamic array.
 * @param p_vec
 * */
void AnvVector_Destroy(AnvVector* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // destroy all object copies if possible and memset whole array!
    if(p_vec->length != 0) AnvVector_Clear(p_vec);

    FREE(p_vec->p_data);
    p_vec->p_data = NULL;
    FREE(p_vec);
}

/**
 * Resize the array to contain given number of elements
 * @param p_vec
 * @param new_size New capacity of this dyn array.
 * */
FORCE_INLINE void AnvVector_Resize(AnvVector* p_vec, Size new_size) {
    RETURN_IF_FAIL(p_vec && new_size, ERR_INVALID_ARGUMENTS);

    void* p_temp = realloc(p_vec->p_data, new_size * AnvVector_ElementSize(p_vec));
    RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
    p_vec->p_data = p_temp;
    p_vec->capacity = new_size;
    p_vec->length = new_size;
}

/**
 * Clear vector.
 * This will destroy all elements and memset whole memory to 0.
 * Capacity is still not 0 after Clear.
 *
 * @param p_vec
 * */
FORCE_INLINE void AnvVector_Clear(AnvVector* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // destroy element copies if we need to
    if(p_vec->pfn_destroy_copy) {
        while(p_vec->length--) {
            p_vec->pfn_destroy_copy(AnvVector_AddressAt(p_vec, p_vec->length));
        }
    }

    memset(p_vec->p_data, 0, p_vec->capacity * AnvVector_ElementSize(p_vec));
}

/**
 * Move an element between two positions.
 * Data at @c to position is first destroyed and then any copy/move is performed.
 * Data at @c from position is memset to 0 after move in order to avoid copies.
 *
 * @p from must be less than @c length of vector.
 * @p to must be less than @c capacity of vector.
 * @p to must be different from @p from
 * If @p to and @p from are same, then move is not performed.
 *
 * @param p_vec
 * @param to Final move position.
 * @param from Position of element to be moved.
 * */
FORCE_INLINE void AnvVector_Move(AnvVector* p_vec, Size to, Size from) {
    if(to == from) return;
    RETURN_IF_FAIL(p_vec && (to < p_vec->capacity) && (from < p_vec->length), ERR_INVALID_ARGUMENTS);

    // we need to destroy element if it's being overwritten to
    if((to < p_vec->length) && p_vec->pfn_destroy_copy) {
        p_vec->pfn_destroy_copy(AnvVector_AddressAt(p_vec, to));
    }

    memcpy(AnvVector_AddressAt(p_vec, to), AnvVector_AddressAt(p_vec, from), p_vec->element_size);
    memset(AnvVector_AddressAt(p_vec, from), 0, AnvVector_ElementSize(p_vec));
}

/**
 * Creates a copy of element by destroying whatever is in target position.
 * This results in two different copies of same elment. If a copy constructor
 * is assigned to AnvVector then it's used to create copy of element.
 *
 * @p from must be less than @c length of vector.
 * @p to must be less than @c capacity of vector.
 * @p to must be different from @p from
 *
 * @param p_vec
 * @param to Target position of element.
 * @param from Position of element to be copied.
 * */
FORCE_INLINE void AnvVector_Copy(AnvVector* p_vec, Size to, Size from) {
    RETURN_IF_FAIL(p_vec && (to < p_vec->capacity) && (from < p_vec->length) && (to != from), ERR_INVALID_ARGUMENTS);
    switch(AnvVector_ElementSize(p_vec)) {
        case 8 : AnvVector_At(p_vec, Uint64, to) = (Uint64)AnvVector_At(p_vec, Uint64, from); break;
        case 4 : AnvVector_At(p_vec, Uint32, to) = (Uint64)AnvVector_At(p_vec, Uint32, from); break;
        case 2 : AnvVector_At(p_vec, Uint16, to) = (Uint64)AnvVector_At(p_vec, Uint16, from); break;
        case 1 : AnvVector_At(p_vec, Uint8, to) = (Uint64)AnvVector_At(p_vec, Uint8, from); break;
        default : {
            void* p_elem_to = AnvVector_AddressAt(p_vec, to);
            void* p_elem_from = AnvVector_AddressAt(p_vec, from);
            if(p_vec->pfn_create_copy) {
                if(to < p_vec->length) p_vec->pfn_destroy_copy(p_elem_to);
                p_vec->pfn_create_copy(p_elem_to, p_elem_from);
            } else {
                memcpy(p_elem_to, p_elem_from, AnvVector_ElementSize(p_vec));
            }
        }
    }
}

/**
 * Move an element between two positions.
 * This will automatically destroy element where data is to be
 * overwritten before writing anything.
 *
 * @param p_vec
 * @param pos Position where this new data
 * @param p_data Data to be moved
 * */
FORCE_INLINE void AnvVector_Overwrite(AnvVector* p_vec, Size pos, void* p_data) {
    RETURN_IF_FAIL(p_vec && (pos < p_vec->capacity), ERR_INVALID_ARGUMENTS);

    Uint64 value = (Uint64)p_data;
    switch(AnvVector_ElementSize(p_vec)) {
        case 8 : AnvVector_At(p_vec, Uint64, pos) = (Uint64)value; break;
        case 4 : AnvVector_At(p_vec, Uint32, pos) = (Uint32)value; break;
        case 2 : AnvVector_At(p_vec, Uint16, pos) = (Uint16)value; break;
        case 1 : AnvVector_At(p_vec, Uint8, pos) = (Uint8)value; break;
        default : {
            if(p_vec->pfn_create_copy) {
                void* p_elem = AnvVector_AddressAt(p_vec, pos);
                if(pos < p_vec->length) p_vec->pfn_destroy_copy(p_elem);
                if(p_data) p_vec->pfn_create_copy(p_elem, p_data);
                else memset(p_elem, 0, p_vec->element_size);
            } else {
                void* p_elem = AnvVector_AddressAt(p_vec, pos);
                if(p_data) memcpy(p_elem, p_data, AnvVector_ElementSize(p_vec));
                else memset(p_elem, 0, AnvVector_ElementSize(p_vec));
            }
        }
    }
}

/**
 * Insert an element into @c AnvVector.
 * This will insert element at position by creating space.
 * This means order of array will be preserved before and after
 * insertion.
 *
 * @param p_vec
 * @param p_data Pointer to memory or just the value to be inserted into
 * contiguous array maintained in @c AnvVector.
 * @param pos Position where insertion will take place.
 * */
FORCE_INLINE void AnvVector_Insert(AnvVector* p_vec, void *p_data, Size pos) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= p_vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = p_vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = p_vec->capacity * (1 + p_vec->resize_factor);
        }

        // reallocate if we need to
        void* p_temp = realloc(p_vec->p_data, new_capacity * AnvVector_ElementSize(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_capacity;
        p_vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(p_vec->length >= p_vec->capacity) {
        Size new_size = p_vec->capacity * (p_vec->resize_factor + 1);
        void* p_temp = realloc(p_vec->p_data, new_size * AnvVector_ElementSize(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_size;
    }

    // shift elements to create space
    Size shift_index = p_vec->length;
    while(shift_index > pos) {
        Size to = shift_index; // at the starting of loop, this is index of position just after last element
        Size from = --shift_index; // at the starting of loop, this is index of last element

        switch(AnvVector_ElementSize(p_vec)) {
        case 8 : AnvVector_At(p_vec, Uint64, to) = AnvVector_At(p_vec, Uint64, from); break;
        case 4 : AnvVector_At(p_vec, Uint32, to) = AnvVector_At(p_vec, Uint32, from); break;
        case 2 : AnvVector_At(p_vec, Uint16, to) = AnvVector_At(p_vec, Uint16, from); break;
        case 1 : AnvVector_At(p_vec, Uint8, to)  = AnvVector_At(p_vec, Uint8, from); break;
        default :
            void* p_elem_to = AnvVector_AddressAt(p_vec, to);
            void* p_elem_from = AnvVector_AddressAt(p_vec, from);
            memcpy(p_elem_to, p_elem_from, AnvVector_ElementSize(p_vec));
        }
    }

    Uint64 value = (Uint64)p_data;
    switch(AnvVector_ElementSize(p_vec)) {
        case 8 : AnvVector_At(p_vec, Uint64, pos) = (Uint64)value; break;
        case 4 : AnvVector_At(p_vec, Uint32, pos) = (Uint32)value; break;
        case 2 : AnvVector_At(p_vec, Uint16, pos) = (Uint16)value; break;
        case 1 : AnvVector_At(p_vec, Uint8, pos)  = (Uint8)value; break;
        default :
            void* p_elem = AnvVector_AddressAt(p_vec, pos);
            if(p_vec->pfn_create_copy) {
                if(p_data) p_vec->pfn_create_copy(p_elem, p_data);
                else memset(p_elem, 0, p_vec->element_size);
            } else {
                if(p_data) memcpy(p_elem, p_data, AnvVector_ElementSize(p_vec));
                else memset(p_elem, 0, AnvVector_ElementSize(p_vec));
            }
    }

    p_vec->length++;
}

/**
 * Delete an element from given position.
 * Order will be preserved.
 * @param p_vec
 * @param pos Positon of element to be deleted.
 * */
FORCE_INLINE void AnvVector_Delete(AnvVector* p_vec, Size pos) {
    RETURN_IF_FAIL(p_vec && (pos < p_vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(p_vec->length, ERR_CONTAINER_UNDERFLOW);

    // destroy element at position if possible
    if(p_vec->pfn_destroy_copy) {
        p_vec->pfn_destroy_copy(AnvVector_AddressAt(p_vec, pos));
    }

    // shift array by one place at pos
    while(++pos < p_vec->length) {
        Size prev = pos-1;
        Size next = pos;
        memcpy(AnvVector_AddressAt(p_vec, prev), AnvVector_AddressAt(p_vec, next), p_vec->element_size);
    }

    // memset to avoid data copies
    memset(AnvVector_AddressAt(p_vec, --p_vec->length), 0, p_vec->element_size);
}

/**
 * Remove an element from given position and return it's data.
 * Order will be preserved.
 * This will not result in a new copy of removed element and just
 * copy inside the AnvVector is returned and removed from AnvVector.
 * @param p_vec
 * @param pos Position of element to be removed.
 * */
FORCE_INLINE void* AnvVector_Remove(AnvVector* p_vec, Size pos) {
    RETURN_VALUE_IF_FAIL(p_vec && (pos < p_vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(p_vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    void* p_elem = NULL;
    switch(AnvVector_ElementSize(p_vec)) {
    case 8 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint64, pos); break;
    case 4 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint32, pos); break;
    case 2 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint16, pos); break;
    case 1 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint8, pos); break;
    default :
        p_elem = ALLOCATE(Uint8, p_vec->element_size);
        RETURN_VALUE_IF_FAIL(p_elem, NULL, ERR_OUT_OF_MEMORY);
        memcpy(p_elem, AnvVector_AddressAt(p_vec, pos), p_vec->element_size);
    }

    // shift array by one place at pos
    while(++pos < p_vec->length) {
        Size prev = pos-1;
        Size next = pos;
        memcpy(AnvVector_AddressAt(p_vec, prev), AnvVector_AddressAt(p_vec, next), p_vec->element_size);
    }

    p_vec->length--;
    return p_elem;
}

/**
 * Insert at element without preserving order of array.
 * Use this when you don't care about order being preserved or
 * if you can later sort array.
 * Data at insert position will be destroyed if possible.
 * @param p_vec
 * @param p_data Data to be inserted.
 * @param pos Insert position.
 * */
FORCE_INLINE void AnvVector_InsertFast(AnvVector* p_vec, void* p_data, Size pos) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= p_vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = p_vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = p_vec->capacity * (1 + p_vec->resize_factor);
        }

        // reallocate if we need to
        void* p_temp = realloc(p_vec->p_data, new_capacity * AnvVector_ElementSize(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_capacity;
        p_vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(p_vec->length >= p_vec->capacity) {
        Size new_size = p_vec->capacity * (p_vec->resize_factor + 1);
        void* p_temp = realloc(p_vec->p_data, new_size * AnvVector_ElementSize(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_size;
    }

    // NOTE: No shift operation required here!

    if(pos < p_vec->length) {
        memcpy(AnvVector_AddressAt(p_vec, p_vec->length), AnvVector_AddressAt(p_vec, pos), p_vec->element_size);
    }

    Uint64 value = (Uint64)p_data;
    switch(AnvVector_ElementSize(p_vec)) {
    case 8 : AnvVector_At(p_vec, Uint64, pos) = (Uint64)value; break;
    case 4 : AnvVector_At(p_vec, Uint32, pos) = (Uint32)value; break;
    case 2 : AnvVector_At(p_vec, Uint16, pos) = (Uint16)value; break;
    case 1 : AnvVector_At(p_vec, Uint8, pos) = (Uint8)value; break;
    default :
        void* p_elem = AnvVector_AddressAt(p_vec, pos);
        if(p_vec->pfn_create_copy) {
            if(p_data) p_vec->pfn_create_copy(p_elem, p_data);
            else memset(p_elem, 0, p_vec->element_size);
        } else {
            if(p_data) memcpy(p_elem, p_data, AnvVector_ElementSize(p_vec));
            else memset(p_elem, 0, AnvVector_ElementSize(p_vec));
        }
    }

    p_vec->length++;
}

/**
 * Delete an element without preserving order.
 * Data at delete position will be destroyed if possible.
 * @param p_vec
 * @param pos Position of element where delete will happen
 * */
FORCE_INLINE void AnvVector_DeleteFast(AnvVector* p_vec, Size pos) {
    RETURN_IF_FAIL(p_vec && (pos < p_vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(p_vec->length, ERR_CONTAINER_UNDERFLOW);
    if(pos != p_vec->length - 1) {
        AnvVector_Move(p_vec, pos, p_vec->length - 1);
    } else {
        if(p_vec->pfn_destroy_copy) {
            p_vec->pfn_destroy_copy(AnvVector_AddressAt(p_vec, pos));
        } else memset(AnvVector_AddressAt(p_vec, pos), 0, p_vec->element_size);
    }

    p_vec->length--;
}

/**
 * Remove an element fast without preserving element
 * @param p_vec
 * @param pos Position of element to be removed
 * */
FORCE_INLINE void* AnvVector_RemoveFast(AnvVector* p_vec, Size pos) {
    RETURN_VALUE_IF_FAIL(p_vec && (pos < p_vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(p_vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    // create element copy without copy constructor
    void* p_elem = NULL;
    switch(AnvVector_ElementSize(p_vec)) {
    case 8 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint64, pos); break;
    case 4 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint32, pos); break;
    case 2 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint16, pos); break;
    case 1 : p_elem = (void*)(Uint64) AnvVector_At(p_vec, Uint8, pos); break;
    default :
        p_elem = ALLOCATE(Uint8, p_vec->element_size);
        RETURN_VALUE_IF_FAIL(p_elem, NULL, ERR_OUT_OF_MEMORY);
        memcpy(p_elem, AnvVector_AddressAt(p_vec, pos), p_vec->element_size);
    }

    memcpy(AnvVector_AddressAt(p_vec, pos), AnvVector_AddressAt(p_vec, --p_vec->length), p_vec->element_size);

    return p_elem;
}

/**
 * Push an element to front by preserving order.
 * @param p_vec
 * @param p_data
 * */
void AnvVector_PushFront(AnvVector* p_vec, void* p_data) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    AnvVector_Insert(p_vec, p_data, 0);
}

/**
 * Remove an element from front by preserving order.
 * This doesn't result in a new copy of element and just
 * copy inside of AnvArray is returned.
 * @param p_vec
 * @return Removed element.
 * */
void* AnvVector_PopFront(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec && p_vec->length, NULL, ERR_INVALID_ARGUMENTS);
    return AnvVector_Remove(p_vec, 0);
}

/**
 * Push an element to front by preserving order.
 * @param p_vec
 * @param p_data
 * */
void AnvVector_PushFrontFast(AnvVector* p_vec, void* p_data) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    AnvVector_Insert(p_vec, p_data, 0);
}

/**
 * Remove an element from front by preserving order.
 * This doesn't result in a new copy of element and just
 * copy inside of AnvArray is returned.
 * @param p_vec
 * @return Removed element.
 * */
void* AnvVector_PopFrontFast(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    return AnvVector_Remove(p_vec, 0);
}

/**
 * Push an element to back. This will internally call
 * the fast insert operation.
 * @param p_vec
 * @param p_data
 * */
void AnvVector_PushBack(AnvVector* p_vec, void* p_data) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    AnvVector_InsertFast(p_vec, p_data, p_vec->length);
}

/**
 * Remove an element from back. This will internally
 * call lthe fast remove operation.
 * @param p_vec
 * @return Removed element.
 * */
FORCE_INLINE void* AnvVector_PopBack(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec && p_vec->length, NULL, ERR_INVALID_ARGUMENTS);
    return AnvVector_RemoveFast(p_vec, p_vec->length - 1);
}

/**
 * Get mutable reference of element at position inside
 * array.
 * @param p_vec
 * @param pos
 * @return Mutable reference to element at given position.
 * */
FORCE_INLINE void* AnvVector_Peek(AnvVector* p_vec, Size pos) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    switch(p_vec->element_size) {
        case 8: return (void*)AnvVector_At(p_vec, Uint64, pos); break;
        case 4: return (void*)(Uint64)AnvVector_At(p_vec, Uint32, pos); break;
        case 2: return (void*)(Uint64)AnvVector_At(p_vec, Uint16, pos); break;
        case 1: return (void*)(Uint64)AnvVector_At(p_vec, Uint8, pos); break;
        default: return AnvVector_AddressAt(p_vec, pos);
    }
}

/**
 * Get mutable reference to data at front.
 * @param p_vec
 * @return Reference of element at front
 * */
void* AnvVector_Front(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    return AnvVector_Peek(p_vec, 0);
}

/**
 * Get mutable reference to data at the back of array.
 * @param p_vec
 * @return Reference to element at back
 * */
FORCE_INLINE void* AnvVector_Back(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    return AnvVector_Peek(p_vec, p_vec->length-1);
}

/**
 * Print all elements of this @c AnvVector using given printer.
 * @param p_vec
 * @param pfn_printer Function to print each element.
 * */
void AnvVector_Print(AnvVector* p_vec, AnvPrintElementCallback pfn_printer) {
    RETURN_IF_FAIL(p_vec && pfn_printer, ERR_INVALID_ARGUMENTS);
    for(Size iter = 0; iter < p_vec->length; iter++) {
        switch(p_vec->element_size) {
            case 8 : pfn_printer((void*)AnvVector_At(p_vec, Uint64, iter), iter); break;
            case 4 : pfn_printer((void*)(Uint64)AnvVector_At(p_vec, Uint32, iter), iter); break;
            case 2 : pfn_printer((void*)(Uint64)AnvVector_At(p_vec, Uint16, iter), iter); break;
            case 1 : pfn_printer((void*)(Uint64)AnvVector_At(p_vec, Uint8, iter), iter); break;
            default: pfn_printer(AnvVector_AddressAt(p_vec, iter), iter);
        }
    }
}
