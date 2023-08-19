/**
 * @file Vector.c
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
 * @brief Implementation of underlying generic vector.
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
AnvVector* anv_vector_create(Size element_size,
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
void anv_vector_destroy(AnvVector* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // destroy all object copies if possible and memset whole array!
    if(p_vec->length != 0) anv_vector_clear(p_vec);

    FREE(p_vec->p_data);
    p_vec->p_data = NULL;
    FREE(p_vec);
}

/**
 * Resize the array to contain given number of elements
 * @param p_vec
 * @param new_size New capacity of this dyn array.
 * */
FORCE_INLINE void anv_vector_resize(AnvVector* p_vec, Size new_size) {
    RETURN_IF_FAIL(p_vec && new_size, ERR_INVALID_ARGUMENTS);

    void* p_temp = realloc(p_vec->p_data, new_size * p_vec->element_size);
    RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);

    // memset to make make new area nullified
    // ref : https://stackoverflow.com/a/32732502
    Size old_size_in_bytes = p_vec->length * p_vec->element_size;
    Size new_size_in_bytes = new_size * p_vec->element_size;
    memset((UByteArray)p_temp + old_size_in_bytes, 0, new_size_in_bytes - old_size_in_bytes);

    p_vec->p_data = p_temp;
    p_vec->capacity = new_size;
    p_vec->length = new_size;
}

/**
 * Reserve space for new array. This does not increase length of array.
 * @param p_vec
 * @param capacity
 * */
FORCE_INLINE void anv_vector_reserve(AnvVector* p_vec, Size capacity) {
    RETURN_IF_FAIL(p_vec && capacity, ERR_INVALID_ARGUMENTS);
    anv_vector_resize(p_vec, capacity);
    p_vec->length = 0;
}

/**
 * Clear vector.
 * This will destroy all elements and memset whole memory to 0.
 * Capacity is still not 0 after Clear.
 *
 * @param p_vec
 * */
FORCE_INLINE void anv_vector_clear(AnvVector* p_vec) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // destroy element copies if we need to
    if(p_vec->pfn_destroy_copy) {
        while(p_vec->length--) {
            p_vec->pfn_destroy_copy(anv_vector_address_at(p_vec, p_vec->length));
        }
    }

    memset(p_vec->p_data, 0, p_vec->capacity * anv_vector_element_size(p_vec));
    p_vec->length = 0;
}

/**
 * Get subvector of given vector by creating another vector containing copy
 * of data.
 * @param p_vec
 * @param start
 * @param size
 * */
AnvVector* anv_vector_get_subvector(AnvVector* p_vec, Size start, Size size) {
    RETURN_VALUE_IF_FAIL(p_vec && size, NULL, ERR_INVALID_ARGUMENTS);

    AnvVector* new_vec = anv_vector_create(p_vec->element_size, p_vec->pfn_create_copy, p_vec->pfn_destroy_copy);
    for(Size s = start; s < size; s++) {
        anv_vector_push_back(new_vec, anv_vector_peek(p_vec, s));
    }

    return new_vec;
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
FORCE_INLINE void anv_vector_move(AnvVector* p_vec, Size to, Size from) {
    if(to == from) return;
    RETURN_IF_FAIL(p_vec && (to < p_vec->capacity) && (from < p_vec->length), ERR_INVALID_ARGUMENTS);

    // we need to destroy element if it's being overwritten to
    if((to < p_vec->length) && p_vec->pfn_destroy_copy) {
        p_vec->pfn_destroy_copy(anv_vector_address_at(p_vec, to));
    }

    memcpy(anv_vector_address_at(p_vec, to), anv_vector_address_at(p_vec, from), p_vec->element_size);
    memset(anv_vector_address_at(p_vec, from), 0, anv_vector_element_size(p_vec));
}

/**
 * Creates a copy of element by destroying whatever is in target position.
 * This results in two different copies of same elment. If a copy constructor
 * is assigned to anv_vector then it's used to create copy of element.
 *
 * @p from must be less than @c length of vector.
 * @p to must be less than @c capacity of vector.
 * @p to must be different from @p from
 *
 * @param p_vec
 * @param to Target position of element.
 * @param from Position of element to be copied.
 * */
FORCE_INLINE void anv_vector_copy(AnvVector* p_vec, Size to, Size from) {
    RETURN_IF_FAIL(p_vec && (to < p_vec->capacity) && (from < p_vec->length) && (to != from), ERR_INVALID_ARGUMENTS);

    // use copy constructor whenever possible
    if(p_vec->pfn_create_copy) {
        void* p_elem_to = anv_vector_address_at(p_vec, to);
        void* p_elem_from = anv_vector_address_at(p_vec, from);
        if(to < p_vec->length) p_vec->pfn_destroy_copy(p_elem_to);
        p_vec->pfn_create_copy(p_elem_to, p_elem_from);
    } else switch(anv_vector_element_size(p_vec)) {
        case 8 : anv_vector_at(p_vec, Uint64, to) = (Uint64)anv_vector_at(p_vec, Uint64, from); break;
        case 4 : anv_vector_at(p_vec, Uint32, to) = (Uint64)anv_vector_at(p_vec, Uint32, from); break;
        case 2 : anv_vector_at(p_vec, Uint16, to) = (Uint64)anv_vector_at(p_vec, Uint16, from); break;
        case 1 : anv_vector_at(p_vec, Uint8, to) = (Uint64)anv_vector_at(p_vec, Uint8, from); break;
        default : {
            void* p_elem_to = anv_vector_address_at(p_vec, to);
            void* p_elem_from = anv_vector_address_at(p_vec, from);
            memcpy(p_elem_to, p_elem_from, anv_vector_element_size(p_vec));
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
FORCE_INLINE void anv_vector_overwrite(AnvVector* p_vec, Size pos, void* p_data) {
    RETURN_IF_FAIL(p_vec && (pos < p_vec->capacity), ERR_INVALID_ARGUMENTS);

    if(p_vec->pfn_create_copy) {
        void* p_elem = anv_vector_address_at(p_vec, pos);
        if(pos < p_vec->length) p_vec->pfn_destroy_copy(p_elem);
        if(p_data) p_vec->pfn_create_copy(p_elem, p_data);
        else memset(p_elem, 0, p_vec->element_size);
    } else {
        Uint64 value = (Uint64)p_data;
        switch(anv_vector_element_size(p_vec)) {
            case 8 : anv_vector_at(p_vec, Uint64, pos) = (Uint64)value; break;
            case 4 : anv_vector_at(p_vec, Uint32, pos) = (Uint32)value; break;
            case 2 : anv_vector_at(p_vec, Uint16, pos) = (Uint16)value; break;
            case 1 : anv_vector_at(p_vec, Uint8, pos) = (Uint8)value; break;
            default : {
                void* p_elem = anv_vector_address_at(p_vec, pos);
                if(p_data) memcpy(p_elem, p_data, anv_vector_element_size(p_vec));
                else memset(p_elem, 0, anv_vector_element_size(p_vec));
            }
        }
    }

    if(pos >= p_vec->length) p_vec->length = pos + 1;
}

/**
 * Insert an element into @c anv_vector.
 * This will insert element at position by creating space.
 * This means order of array will be preserved before and after
 * insertion.
 *
 * Insert will always create a copy of provided data for itself.
 * This ensures that anv_vector will never free or misuse given pointer.
 * This also allows user to have it's own copy of data, by either
 * creating it on stack and passing pointer or just a heap allocated data.
 *
 * @param p_vec
 * @param p_data Pointer to memory or just the value to be inserted into
 * contiguous array maintained in @c anv_vector.
 * @param pos Position where insertion will take place.
 * */
FORCE_INLINE void anv_vector_insert(AnvVector* p_vec, void *p_data, Size pos) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= p_vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = p_vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = p_vec->capacity * (1 + p_vec->resize_factor);
        }

        // reallocate if we need to
        void* p_temp = realloc(p_vec->p_data, new_capacity * anv_vector_element_size(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_capacity;
        p_vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(p_vec->length >= p_vec->capacity) {
        Size new_size = p_vec->capacity * (p_vec->resize_factor + 1);
        void* p_temp = realloc(p_vec->p_data, new_size * anv_vector_element_size(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_size;
    }

    // shift elements to create space
    Size shift_index = p_vec->length;
    while(shift_index > pos) {
        Size to = shift_index; // at the starting of loop, this is index of position just after last element
        Size from = --shift_index; // at the starting of loop, this is index of last element

        // NOTE: we don't need to use create copy here because we already have a copy
        // we just need to shift those copies, memcpy is enough for that
        switch(anv_vector_element_size(p_vec)) {
            case 8 : anv_vector_at(p_vec, Uint64, to) = anv_vector_at(p_vec, Uint64, from); break;
            case 4 : anv_vector_at(p_vec, Uint32, to) = anv_vector_at(p_vec, Uint32, from); break;
            case 2 : anv_vector_at(p_vec, Uint16, to) = anv_vector_at(p_vec, Uint16, from); break;
            case 1 : anv_vector_at(p_vec, Uint8, to) = anv_vector_at(p_vec, Uint8, from); break;
            default: {
                void* p_elem_to = anv_vector_address_at(p_vec, to);
                void* p_elem_from = anv_vector_address_at(p_vec, from);
                memcpy(p_elem_to, p_elem_from, anv_vector_element_size(p_vec));
            }
        }
    }

    Uint64 value = (Uint64)p_data;

    // use copy constructor to create a copy for vector
    // this is really useful in case of structures that contain other pointers
    // or strings for eg
    if(p_vec->pfn_create_copy) {
        void* p_elem = anv_vector_address_at(p_vec, pos);
        if(p_data) p_vec->pfn_create_copy(p_elem, p_data);
        else memset(p_elem, 0, p_vec->element_size);
    } else switch(anv_vector_element_size(p_vec)) {
            case 8 : anv_vector_at(p_vec, Uint64, pos) = (Uint64)value; break;
            case 4 : anv_vector_at(p_vec, Uint32, pos) = (Uint32)value; break;
            case 2 : anv_vector_at(p_vec, Uint16, pos) = (Uint16)value; break;
            case 1 : anv_vector_at(p_vec, Uint8,  pos) = (Uint8) value; break;
            default: {
                void* p_elem = anv_vector_address_at(p_vec, pos);
                if(p_data) memcpy(p_elem, p_data, anv_vector_element_size(p_vec));
                else memset(p_elem, 0, anv_vector_element_size(p_vec));
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
FORCE_INLINE void anv_vector_delete(AnvVector* p_vec, Size pos) {
    RETURN_IF_FAIL(p_vec && (pos < p_vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(p_vec->length, ERR_CONTAINER_UNDERFLOW);

    // destroy element at position if possible
    if(p_vec->pfn_destroy_copy) {
        p_vec->pfn_destroy_copy(anv_vector_address_at(p_vec, pos));
    }

    // shift array by one place at pos
    while(++pos < p_vec->length) {
        Size prev = pos-1;
        Size next = pos;
        memcpy(anv_vector_address_at(p_vec, prev), anv_vector_address_at(p_vec, next), p_vec->element_size);
    }

    // memset to avoid data copies
    memset(anv_vector_address_at(p_vec, --p_vec->length), 0, p_vec->element_size);
}

/**
 * Remove an element from given position and return it's data.
 * Order will be preserved.
 * This will not result in a new copy of removed element and just
 * copy inside the anv_vector is returned and removed from anv_vector.
 * @param p_vec
 * @param pos Position of element to be removed.
 * */
FORCE_INLINE void* anv_vector_remove(AnvVector* p_vec, Size pos) {
    RETURN_VALUE_IF_FAIL(p_vec && (pos < p_vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(p_vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    // create a copy of element out of vector
    // here we can just return the copy that vector has,
    // instead of creating another copy and destroying old one
    void* p_elem = NULL;
    switch(anv_vector_element_size(p_vec)) {
    case 8 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint64, pos); break;
    case 4 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint32, pos); break;
    case 2 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint16, pos); break;
    case 1 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint8, pos); break;
    default :
        p_elem = ALLOCATE(Uint8, p_vec->element_size);
        RETURN_VALUE_IF_FAIL(p_elem, NULL, ERR_OUT_OF_MEMORY);
        memcpy(p_elem, anv_vector_address_at(p_vec, pos), p_vec->element_size);
    }

    // shift array by one place at pos
    while(++pos < p_vec->length) {
        Size prev = pos-1;
        Size next = pos;
        switch(anv_vector_element_size(p_vec)) {
            case 8 : anv_vector_at(p_vec, Uint64, prev) = anv_vector_at(p_vec, Uint64, next); break;
            case 4 : anv_vector_at(p_vec, Uint32, prev) = anv_vector_at(p_vec, Uint32, next); break;
            case 2 : anv_vector_at(p_vec, Uint16, prev) = anv_vector_at(p_vec, Uint16, next); break;
            case 1 : anv_vector_at(p_vec, Uint8,  prev) = anv_vector_at(p_vec, Uint8,  next); break;
            default :
                memcpy(anv_vector_address_at(p_vec, prev), anv_vector_address_at(p_vec, next), p_vec->element_size);
        }
    }

    p_vec->length--;
    return p_elem;
}

/**
 * Insert at element without preserving order of array.
 * Use this when you don't care about order being preserved or
 * if you can later sort array.
 * Data at insert position will be destroyed if possible.
 *
 * Insert will always create a copy of provided data for itself.
 * This ensures that anv_vector will never free or misuse given pointer.
 * This also allows user to have it's own copy of data, by either
 * creating it on stack and passing pointer or just a heap allocated data.
 *
 * @param p_vec
 * @param p_data Data to be inserted.
 * @param pos Insert position.
 * */
FORCE_INLINE void anv_vector_insert_fast(AnvVector* p_vec, void* p_data, Size pos) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= p_vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = p_vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = p_vec->capacity * (1 + p_vec->resize_factor);
        }

        // reallocate if we need to
        void* p_temp = realloc(p_vec->p_data, new_capacity * anv_vector_element_size(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_capacity;
        p_vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(p_vec->length >= p_vec->capacity) {
        Size new_size = p_vec->capacity * (p_vec->resize_factor + 1);
        void* p_temp = realloc(p_vec->p_data, new_size * anv_vector_element_size(p_vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        p_vec->p_data = p_temp;
        p_vec->capacity = new_size;
    }

    // NOTE: No shift operation required here!

    if(pos < p_vec->length) {
        memcpy(anv_vector_address_at(p_vec, p_vec->length), anv_vector_address_at(p_vec, pos), p_vec->element_size);
    }

    Uint64 value = (Uint64)p_data;
    if(p_vec->pfn_create_copy) {
        void* p_elem = anv_vector_address_at(p_vec, pos);
        if(p_data) p_vec->pfn_create_copy(p_elem, p_data);
        else memset(p_elem, 0, p_vec->element_size);
    } else switch(anv_vector_element_size(p_vec)) {
            case 8 : anv_vector_at(p_vec, Uint64, pos) = (Uint64)value; break;
            case 4 : anv_vector_at(p_vec, Uint32, pos) = (Uint32)value; break;
            case 2 : anv_vector_at(p_vec, Uint16, pos) = (Uint16)value; break;
            case 1 : anv_vector_at(p_vec, Uint8, pos) = (Uint8)value; break;
            default: {
                void* p_elem = anv_vector_address_at(p_vec, pos);
                if(p_data) memcpy(p_elem, p_data, anv_vector_element_size(p_vec));
                else memset(p_elem, 0, anv_vector_element_size(p_vec));
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
FORCE_INLINE void anv_vector_delete_fast(AnvVector* p_vec, Size pos) {
    RETURN_IF_FAIL(p_vec && (pos < p_vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(p_vec->length, ERR_CONTAINER_UNDERFLOW);

    // if position is not the last element then we just move last value to
    // position that needs to be deleted
    if(pos != p_vec->length - 1) {
        anv_vector_move(p_vec, pos, p_vec->length - 1);
    } else { // else we destroy copy
        if(p_vec->pfn_destroy_copy)
            p_vec->pfn_destroy_copy(anv_vector_address_at(p_vec, pos));
        else memset(anv_vector_address_at(p_vec, pos), 0, p_vec->element_size);
    }

    p_vec->length--;
}

/**
 * Remove an element fast without preserving element
 * @param p_vec
 * @param pos Position of element to be removed
 * */
FORCE_INLINE void* anv_vector_remove_fast(AnvVector* p_vec, Size pos) {
    RETURN_VALUE_IF_FAIL(p_vec && (pos < p_vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(p_vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    // create element copy without copy constructor
    void* p_elem = NULL;
    switch(anv_vector_element_size(p_vec)) {
        case 8 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint64, pos); break;
        case 4 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint32, pos); break;
        case 2 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint16, pos); break;
        case 1 : p_elem = (void*)(Uint64) anv_vector_at(p_vec, Uint8, pos); break;
        default :
            p_elem = ALLOCATE(Uint8, p_vec->element_size);
            RETURN_VALUE_IF_FAIL(p_elem, NULL, ERR_OUT_OF_MEMORY);
            memcpy(p_elem, anv_vector_address_at(p_vec, pos), p_vec->element_size);
    }

    // then just move last element to position that needs to be removed
    memcpy(anv_vector_address_at(p_vec, pos), anv_vector_address_at(p_vec, --p_vec->length), p_vec->element_size);

    return p_elem;
}

/**
 * Push an element to front by preserving order.
 * @param p_vec
 * @param p_data
 * */
void anv_vector_push_front(AnvVector* p_vec, void* p_data) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    anv_vector_insert(p_vec, p_data, 0);
}

/**
 * Remove an element from front by preserving order.
 * This doesn't result in a new copy of element and just
 * copy inside of AnvArray is returned.
 * @param p_vec
 * @return Removed element.
 * */
void* anv_vector_pop_front(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec && p_vec->length, NULL, ERR_INVALID_ARGUMENTS);
    return anv_vector_remove(p_vec, 0);
}

/**
 * Push an element to front by preserving order.
 * @param p_vec
 * @param p_data
 * */
void anv_vector_push_front_fast(AnvVector* p_vec, void* p_data) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    anv_vector_insert(p_vec, p_data, 0);
}

/**
 * Remove an element from front by preserving order.
 * This doesn't result in a new copy of element and just
 * copy inside of AnvArray is returned.
 * @param p_vec
 * @return Removed element.
 * */
void* anv_vector_pop_front_fast(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    return anv_vector_remove(p_vec, 0);
}

/**
 * Push an element to back. This will internally call
 * the fast insert operation.
 * @param p_vec
 * @param p_data
 * */
void anv_vector_push_back(AnvVector* p_vec, void* p_data) {
    RETURN_IF_FAIL(p_vec, ERR_INVALID_ARGUMENTS);
    anv_vector_insert_fast(p_vec, p_data, p_vec->length);
}

/**
 * Remove an element from back. This will internally
 * call lthe fast remove operation.
 * @param p_vec
 * @return Removed element.
 * */
FORCE_INLINE void* anv_vector_pop_back(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec && p_vec->length, NULL, ERR_INVALID_ARGUMENTS);
    return anv_vector_remove_fast(p_vec, p_vec->length - 1);
}

/**
 * Get mutable reference of element at position inside
 * array.
 * @param p_vec
 * @param pos
 * @return Mutable reference to element at given position.
 * */
FORCE_INLINE void* anv_vector_peek(AnvVector* p_vec, Size pos) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    switch(p_vec->element_size) {
        case 8: return (void*)anv_vector_at(p_vec, Uint64, pos); break;
        case 4: return (void*)(Uint64)anv_vector_at(p_vec, Uint32, pos); break;
        case 2: return (void*)(Uint64)anv_vector_at(p_vec, Uint16, pos); break;
        case 1: return (void*)(Uint64)anv_vector_at(p_vec, Uint8, pos); break;
        default: return anv_vector_address_at(p_vec, pos);
    }
}

/**
 * Get mutable reference to data at front.
 * @param p_vec
 * @return Reference of element at front
 * */
void* anv_vector_front(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    return anv_vector_peek(p_vec, 0);
}

/**
 * Get mutable reference to data at the back of array.
 * @param p_vec
 * @return Reference to element at back
 * */
FORCE_INLINE void* anv_vector_back(AnvVector* p_vec) {
    RETURN_VALUE_IF_FAIL(p_vec, NULL, ERR_INVALID_ARGUMENTS);
    return anv_vector_peek(p_vec, p_vec->length-1);
}

/**
 * Print all elements of this @c anv_vector using given printer.
 * @param p_vec
 * @param pfn_printer Function to print each element.
 * */
void anv_vector_print(AnvVector* p_vec, AnvPrintElementCallback pfn_printer) {
    RETURN_IF_FAIL(p_vec && pfn_printer, ERR_INVALID_ARGUMENTS);

    switch(p_vec->element_size) {
        case 8 :
            for(Size iter = 0; iter < p_vec->length; iter++)
                pfn_printer((void*)anv_vector_at(p_vec, Uint64, iter), iter);
            break;
        case 4 :
            for(Size iter = 0; iter < p_vec->length; iter++)
                pfn_printer((void*)(Uint64)anv_vector_at(p_vec, Uint32, iter), iter);
            break;
        case 2 :
            for(Size iter = 0; iter < p_vec->length; iter++)
                pfn_printer((void*)(Uint64)anv_vector_at(p_vec, Uint16, iter), iter);
            break;
        case 1 :
            for(Size iter = 0; iter < p_vec->length; iter++)
                pfn_printer((void*)(Uint64)anv_vector_at(p_vec, Uint8, iter), iter);
            break;
        default:
            for(Size iter = 0; iter < p_vec->length; iter++)
                pfn_printer(anv_vector_address_at(p_vec, iter), iter);
            break;
    }
}

/**
 * Merge two vectors into the first vector.
 * @param p_vec Vector to merge other vector into
 * @param p_vec_other Vector to be merged
 * */
void anv_vector_merge(AnvVector* p_vec, AnvVector* p_vec_other) {
    RETURN_IF_FAIL(p_vec && p_vec_other, ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(p_vec->element_size == p_vec_other->element_size, "Arrays of different element sizes cannot be merged\n");

    for(Size s = 0; s < p_vec_other->length; s++) {
        anv_vector_push_back(p_vec, anv_vector_peek(p_vec_other, s));
    }
}

/**
 * Filter elements from this vector and return a new vector with filtered elements.
 * @param p_vec Vector to filter elements from.
 * @param pfn_filter Filter function to filter elements
 * @return New AnvVector object containing filtered elements on success, NULL otherwise.
 * */
AnvVector* anv_vector_filter(AnvVector* p_vec, AnvFilterElementCallback pfn_filter, void* p_user_data) {
    RETURN_VALUE_IF_FAIL(p_vec && pfn_filter, NULL, ERR_INVALID_ARGUMENTS);

    // create new vector for containing filtered vectors
    AnvVector* filtered_vec = anv_vector_create(p_vec->element_size, p_vec->pfn_create_copy, p_vec->pfn_destroy_copy);
    RETURN_VALUE_IF_FAIL(filtered_vec, NULL, "Failed to create new filtered vector\n");

    // filter elements
    for(Size i = 0; i < p_vec->length; i++) {
        if(pfn_filter(anv_vector_peek(p_vec, i), p_user_data)) {
            anv_vector_push_back(filtered_vec, anv_vector_peek(p_vec, i));
        }
    }

    return filtered_vec;
}

/**
 * Swap two elements in same vector.
 * @param p_vec Vector in which arrays will be sorted
 * @param p1 Position of first element
 * @param p2 Position of second element
 * */
FORCE_INLINE void anv_vector_swap(AnvVector* p_vec, Size p1, Size p2) {
    RETURN_IF_FAIL(p_vec && (p1 < p_vec->length || p2 < p_vec->length), ERR_INVALID_ARGUMENTS);

    // define to keep less repetitions
#define SWAP_INT(type, i1, i2) {                                        \
        type t = anv_vector_at(p_vec, type, p1);                        \
        anv_vector_at(p_vec, type, p1) = anv_vector_at(p_vec, type, p2); \
        anv_vector_at(p_vec, type, p2) = t;                             \
        return;                                                         \
    }

    switch(p_vec->element_size) {
        case 8: SWAP_INT(Uint64, p1, p2);
        case 4: SWAP_INT(Uint32, p1, p2);
        case 2: SWAP_INT(Uint16, p1, p2);
        case 1: SWAP_INT(Uint8,  p1, p2);
        default: {
            Byte temp[p_vec->element_size];
            memcpy(temp, anv_vector_address_at(p_vec, p1), p_vec->element_size);
            memcpy(anv_vector_address_at(p_vec, p1), anv_vector_address_at(p_vec, p2), p_vec->element_size);
            memcpy(anv_vector_address_at(p_vec, p2), temp, p_vec->element_size);
            return;
        }
    }

#undef SWAP_INT
}

/**
 * Apply the fastest sort algorithm for given array possible.
 * @param p_vec Vector to be sorted
 * @param pfn_cmp Compare function
 * */
void anv_vector_sort(AnvVector* p_vec, AnvCompareElementCallback pfn_cmp) {
    RETURN_IF_FAIL(p_vec && pfn_cmp, ERR_INVALID_ARGUMENTS);
    anv_vector_insertion_sort(p_vec, pfn_cmp);
}

/**
 * Check whether the array is sorted in any manner
 * By default the algorithm is written for checking array in descending order,
 * but this can be checked for any array type by modifying the compare method.
 *
 * This means if pfn_compare returns > 0 for any comparision then array is not sorted
 *
 * @param p_vec
 * @param pfn_compare
 * */
Bool anv_vector_check_sorted(AnvVector* p_vec, AnvCompareElementCallback pfn_compare) {
    RETURN_VALUE_IF_FAIL(p_vec && pfn_compare, False, ERR_INVALID_ARGUMENTS);
    for(Size s = 1; s < p_vec->length; s++)
        if(pfn_compare(anv_vector_peek(p_vec, s), anv_vector_peek(p_vec, s-1)) > 0)
            return False;
    return True;
}

/**
 * Use insertion sort mechanism to sort this array.
 * Time complexity:
 * BEST : O(n)
 * AVERAGE : O(n^2)
 * WORST : O(n^2)
 * @param p_vec
 * @param pfn_compare Compare function
 * */
void anv_vector_insertion_sort(AnvVector* p_vec, AnvCompareElementCallback pfn_compare) {
    RETURN_IF_FAIL(p_vec && pfn_compare, ERR_INVALID_ARGUMENTS);

    for(Size s = 1; s < p_vec->length; s++) {
        Size m = s;
        while((m > 0) && (pfn_compare(anv_vector_peek(p_vec, m-1), anv_vector_peek(p_vec, m)) > 0)) {
            anv_vector_swap(p_vec, m, m-1); m--;
        }
    }
}

/**
 * Bubble sort algorithm.
 * @param p_vec
 * @param pfn_compare
 * */
void anv_vector_bubble_sort(AnvVector* p_vec, AnvCompareElementCallback pfn_compare) {
    RETURN_IF_FAIL(p_vec && pfn_compare, ERR_INVALID_ARGUMENTS);

    Bool b_swapped;
    for (Size i = 0; i < p_vec->length; i++) {
        b_swapped = False;
        for (Size j = 0; j < p_vec->length; j++) {
            if (pfn_compare(anv_vector_peek(p_vec, j+1), anv_vector_peek(p_vec, j)) > 0) {
                anv_vector_swap(p_vec, j, j+1);
                b_swapped = True;
            }
        }
        if (!b_swapped)
            break;
    }
}

static inline void print_subarray(AnvVector* p_vec, Size start, Size size) {
    for(Size s = start; s < size; s++) {
        printf("%d, ", anv_vector_at(p_vec, Uint32, s));
    }
}

static inline void merge_sort(AnvVector* p_vec, AnvCompareElementCallback pfn_compare, Size start, Size size) {
    if(size == 1) return;

    RETURN_IF_FAIL(p_vec && pfn_compare && (start + size) <= p_vec->length, ERR_INVALID_ARGUMENTS);

    if((size == 2) && (pfn_compare(anv_vector_peek(p_vec, start), anv_vector_peek(p_vec, start + 1)) > 0)) {
        anv_vector_swap(p_vec, start, start+1);
        return;
    }

    Size mid = start + size/2;
    merge_sort(p_vec, pfn_compare, start, size/2);
    merge_sort(p_vec, pfn_compare, mid, size - size/2);

    Size s = start;
    Size m = mid;

    AnvVector* subvec = anv_vector_get_subvector(p_vec, start, size);
    subvec->pfn_create_copy = NULL; // reduce mempy calls
    subvec->pfn_destroy_copy = NULL;

    anv_vector_reserve(subvec, size);

    // merge
    while((s < mid) && (m < start + size - 1)) {
        Int32 res = pfn_compare(anv_vector_peek(p_vec, s), anv_vector_peek(p_vec, m));
        if(res > 0) {
            anv_vector_push_back(subvec, anv_vector_peek(p_vec, s++));
        } else if (res < 0) {
            anv_vector_push_back(subvec, anv_vector_peek(p_vec, m++));
        } else {
            anv_vector_push_back(subvec, anv_vector_peek(p_vec, s++));
            anv_vector_push_back(subvec, anv_vector_peek(p_vec, m++));
        }
    }

    // copy back sorted array
    memcpy(anv_vector_address_at(p_vec, start), subvec->p_data, subvec->element_size * subvec->length);
    anv_vector_destroy(subvec);
}


/**
 * Merge sort algorithm
 * @param p_vec
 * @param pfn_compare
 * */
void anv_vector_merge_sort(AnvVector* p_vec, AnvCompareElementCallback pfn_compare) {
    RETURN_IF_FAIL(p_vec && pfn_compare, ERR_INVALID_ARGUMENTS);
    merge_sort(p_vec, pfn_compare, 0, p_vec->length);
}
