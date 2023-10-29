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
#define AV_VECTOR_INIT_ELEMENT_COUNT 4
#define AV_VECTOR_DEFAULT_RESIZE_FACTOR 1
#define AV_VECTOR_DEFAULT_FREE_WHEN_POSSIBLE True

/**
 * Create a new dynamic array.
 * If any one of @c pfn_create_copy or @c pfn_destroy_copy is non null,
 * then both must be non null!
 *
 * @param element_size Size of each element in this dynamic array.
 * @param pfn_create_copy Copy constructor for elements. Can be NULL.
 * @param pfn_destroy_copy Copy destructor for elements. Can be NULL.
 * @return Vector* or NULL if allocation failed
 * */
Vector* vector_create(Size element_size,
                            CreateElementCopyCallback pfn_create_copy,
                            DestroyElementCopyCallback pfn_destroy_copy)
{
    RETURN_VALUE_IF_FAIL(element_size, NULL, ERR_INVALID_ARGUMENTS);

    // both must be null or non null at the same time
    Bool b1 = pfn_create_copy != NULL;
    Bool b2 = pfn_destroy_copy != NULL;
    RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL,
                         "Either both copy constructor and destructor should be NULL, "
                         "or both should be non NULL at the same time!");

    // create a new dyn array object
    Vector* vec = NEW(Vector);
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_OUT_OF_MEMORY);

    // allocate initial memory
    vec->p_data = ALLOCATE(Uint8, AV_VECTOR_INIT_ELEMENT_COUNT * element_size);
    if(!vec->p_data) {
        WARN_IF(True, ERR_OUT_OF_MEMORY);
        FREE(vec);
        return NULL;
    }
    vec->capacity = AV_VECTOR_INIT_ELEMENT_COUNT;
    vec->element_size = element_size;
    vec->resize_factor = AV_VECTOR_DEFAULT_RESIZE_FACTOR;
    vec->pfn_create_copy = pfn_create_copy;
    vec->pfn_destroy_copy = pfn_destroy_copy;
    return vec;
}

/**
 * Destroy given dynamic array.
 * @param vec
 * */
void vector_destroy(Vector* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // destroy all object copies if possible and memset whole array!
    if(vec->length != 0) vector_clear(vec);

    FREE(vec->p_data);
    vec->p_data = NULL;
    FREE(vec);
}

/**
 * Create a clone of this vector.
 * This will not only create clone of this vector but also all the
 * data that it contains. Meaning these two vectors might look like
 * clones but they're internally completely different in memory!
 *
 * @param vec Vector to be cloned.
 * */
Vector* vector_clone(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);

    Vector* vec_clone = NEW(Vector);
    RETURN_VALUE_IF_FAIL(vec_clone, NULL, ERR_OUT_OF_MEMORY);

    // create copy of given vector
    vector_create_copy(vec_clone, vec);

    return vec_clone;
}

/**
 * Resize the array to contain given number of elements
 * @param vec
 * @param new_size New capacity of this dyn array.
 * */
FORCE_INLINE void vector_resize(Vector* vec, Size new_size) {
    RETURN_IF_FAIL(vec && new_size, ERR_INVALID_ARGUMENTS);

    void* p_temp = realloc(vec->p_data, new_size * vec->element_size);
    RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);

    // memset to make make new area nullified
    // ref : https://stackoverflow.com/a/32732502
    Size old_size_in_bytes = vec->length * vec->element_size;
    Size new_size_in_bytes = new_size * vec->element_size;
    memset((UByteArray)p_temp + old_size_in_bytes, 0, new_size_in_bytes - old_size_in_bytes);

    vec->p_data = p_temp;
    vec->capacity = new_size;
    vec->length = new_size;
}

/**
 * Reserve space for new array. This does not increase length of array.
 * @param vec
 * @param capacity
 * */
FORCE_INLINE void vector_reserve(Vector* vec, Size capacity) {
    RETURN_IF_FAIL(vec && capacity, ERR_INVALID_ARGUMENTS);
    vector_resize(vec, capacity);
    vec->length = 0;
}

/**
 * Clear vector.
 * Destroy each element if copy destructor is provided,
 * otherwise, just length is set to 0 in order to save time.
 *
 * @param vec
 * */
FORCE_INLINE void vector_clear(Vector* vec) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // destroy element copies if we need to
    if(vec->pfn_destroy_copy) {
        while(vec->length--) {
            vec->pfn_destroy_copy(vector_address_at(vec, vec->length));
        }
    }

    vec->length = 0;
}

/**
 * Get subvector of given vector by creating another vector containing copy
 * of data.
 * @param vec
 * @param start
 * @param size
 * */
Vector* vector_get_subvector(Vector* vec, Size start, Size size) {
    RETURN_VALUE_IF_FAIL(vec && size, NULL, ERR_INVALID_ARGUMENTS);

    Vector* new_vec = vector_create(vec->element_size, vec->pfn_create_copy, vec->pfn_destroy_copy);
    for(Size s = start; s < size; s++) {
        vector_push_back(new_vec, vector_peek(vec, s));
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
 * @param vec
 * @param to Final move position.
 * @param from Position of element to be moved.
 * */
FORCE_INLINE void vector_move(Vector* vec, Size to, Size from) {
    if(to == from) return;
    RETURN_IF_FAIL(vec && (to < vec->capacity) && (from < vec->length), ERR_INVALID_ARGUMENTS);

    // we need to destroy element if it's being overwritten to
    if((to < vec->length) && vec->pfn_destroy_copy) {
        vec->pfn_destroy_copy(vector_address_at(vec, to));
    }

    memcpy(vector_address_at(vec, to), vector_address_at(vec, from), vec->element_size);
    memset(vector_address_at(vec, from), 0, vector_element_size(vec));
}

/**
 * Creates a copy of element by destroying whatever is in target position.
 * This results in two different copies of same elment. If a copy constructor
 * is assigned to vector then it's used to create copy of element.
 *
 * @p from must be less than @c length of vector.
 * @p to must be less than @c capacity of vector.
 * @p to must be different from @p from
 *
 * @param vec
 * @param to Target position of element.
 * @param from Position of element to be copied.
 * */
FORCE_INLINE void vector_copy(Vector* vec, Size to, Size from) {
    RETURN_IF_FAIL(vec && (to < vec->capacity) && (from < vec->length) && (to != from), ERR_INVALID_ARGUMENTS);

    // use copy constructor whenever possible
    if(vec->pfn_create_copy) {
        void* p_elem_to = vector_address_at(vec, to);
        void* p_elem_from = vector_address_at(vec, from);
        if(to < vec->length) vec->pfn_destroy_copy(p_elem_to);
        vec->pfn_create_copy(p_elem_to, p_elem_from);
    } else switch(vector_element_size(vec)) {
        case 8 : vector_at(vec, Uint64, to) = (Uint64)vector_at(vec, Uint64, from); break;
        case 4 : vector_at(vec, Uint32, to) = (Uint64)vector_at(vec, Uint32, from); break;
        case 2 : vector_at(vec, Uint16, to) = (Uint64)vector_at(vec, Uint16, from); break;
        case 1 : vector_at(vec, Uint8, to) = (Uint64)vector_at(vec, Uint8, from); break;
        default : {
            void* p_elem_to = vector_address_at(vec, to);
            void* p_elem_from = vector_address_at(vec, from);
            memcpy(p_elem_to, p_elem_from, vector_element_size(vec));
        }
    }
}

/**
 * Move an element between two positions.
 * This will automatically destroy element where data is to be
 * overwritten before writing anything.
 *
 * @param vec
 * @param pos Position where this new data
 * @param p_data Data to be moved
 * */
FORCE_INLINE void vector_overwrite(Vector* vec, Size pos, void* p_data) {
    RETURN_IF_FAIL(vec && (pos < vec->capacity), ERR_INVALID_ARGUMENTS);

    if(vec->pfn_create_copy) {
        void* p_elem = vector_address_at(vec, pos);
        if(pos < vec->length) vec->pfn_destroy_copy(p_elem);
        if(p_data) vec->pfn_create_copy(p_elem, p_data);
        else memset(p_elem, 0, vec->element_size);
    } else {
        Uint64 value = (Uint64)p_data;
        switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, pos) = (Uint64)value; break;
            case 4 : vector_at(vec, Uint32, pos) = (Uint32)value; break;
            case 2 : vector_at(vec, Uint16, pos) = (Uint16)value; break;
            case 1 : vector_at(vec, Uint8, pos) = (Uint8)value; break;
            default : {
                void* p_elem = vector_address_at(vec, pos);
                if(p_data) memcpy(p_elem, p_data, vector_element_size(vec));
                else memset(p_elem, 0, vector_element_size(vec));
            }
        }
    }

    if(pos >= vec->length) vec->length = pos + 1;
}

/**
 * Insert an element into @c vector.
 * This will insert element at position by creating space.
 * This means order of array will be preserved before and after
 * insertion.
 *
 * Insert will always create a copy of provided data for itself.
 * This ensures that vector will never free or misuse given pointer.
 * This also allows user to have it's own copy of data, by either
 * creating it on stack and passing pointer or just a heap allocated data.
 *
 * @param vec
 * @param p_data Pointer to memory or just the value to be inserted into
 * contiguous array maintained in @c vector.
 * @param pos Position where insertion will take place.
 * */
FORCE_INLINE void vector_insert(Vector* vec, void *p_data, Size pos) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = vec->capacity * (1 + vec->resize_factor);
        }

        // reallocate if we need to
        void* p_temp = realloc(vec->p_data, new_capacity * vector_element_size(vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        vec->p_data = p_temp;
        vec->capacity = new_capacity;
        vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(vec->length >= vec->capacity) {
        Size new_size = vec->capacity * (vec->resize_factor + 1);
        void* p_temp = realloc(vec->p_data, new_size * vector_element_size(vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        vec->p_data = p_temp;
        vec->capacity = new_size;
    }

    // shift elements to create space
    Size shift_index = vec->length;
    while(shift_index > pos) {
        Size to = shift_index; // at the starting of loop, this is index of position just after last element
        Size from = --shift_index; // at the starting of loop, this is index of last element

        // NOTE: we don't need to use create copy here because we already have a copy
        // we just need to shift those copies, memcpy is enough for that
        switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, to) = vector_at(vec, Uint64, from); break;
            case 4 : vector_at(vec, Uint32, to) = vector_at(vec, Uint32, from); break;
            case 2 : vector_at(vec, Uint16, to) = vector_at(vec, Uint16, from); break;
            case 1 : vector_at(vec, Uint8, to) = vector_at(vec, Uint8, from); break;
            default: {
                void* p_elem_to = vector_address_at(vec, to);
                void* p_elem_from = vector_address_at(vec, from);
                memcpy(p_elem_to, p_elem_from, vector_element_size(vec));
            }
        }
    }

    Uint64 value = (Uint64)p_data;

    // use copy constructor to create a copy for vector
    // this is really useful in case of structures that contain other pointers
    // or strings for eg
    if(vec->pfn_create_copy) {
        void* p_elem = vector_address_at(vec, pos);
        if(p_data) vec->pfn_create_copy(p_elem, p_data);
        else memset(p_elem, 0, vec->element_size);
    } else switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, pos) = (Uint64)value; break;
            case 4 : vector_at(vec, Uint32, pos) = (Uint32)value; break;
            case 2 : vector_at(vec, Uint16, pos) = (Uint16)value; break;
            case 1 : vector_at(vec, Uint8,  pos) = (Uint8) value; break;
            default: {
                void* p_elem = vector_address_at(vec, pos);
                if(p_data) memcpy(p_elem, p_data, vector_element_size(vec));
                else memset(p_elem, 0, vector_element_size(vec));
            }
        }

    vec->length++;
}

/**
 * Delete an element from given position.
 * Order will be preserved.
 * @param vec
 * @param pos Positon of element to be deleted.
 * */
FORCE_INLINE void vector_delete(Vector* vec, Size pos) {
    RETURN_IF_FAIL(vec && (pos < vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(vec->length, ERR_CONTAINER_UNDERFLOW);

    // destroy element at position if possible
    if(vec->pfn_destroy_copy) {
        vec->pfn_destroy_copy(vector_address_at(vec, pos));
    }

    // shift array by one place at pos
    while(++pos < vec->length) {
        Size prev = pos-1;
        Size next = pos;
        memcpy(vector_address_at(vec, prev), vector_address_at(vec, next), vec->element_size);
    }

    // memset to avoid data copies
    memset(vector_address_at(vec, --vec->length), 0, vec->element_size);
}

/**
 * Remove an element from given position and return it's data.
 * Order will be preserved.
 * This will not result in a new copy of removed element and just
 * copy inside the vector is returned and removed from vector.
 * @param vec
 * @param pos Position of element to be removed.
 * */
FORCE_INLINE void* vector_remove(Vector* vec, Size pos) {
    RETURN_VALUE_IF_FAIL(vec && (pos < vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    // create a copy of element out of vector
    // here we can just return the copy that vector has,
    // instead of creating another copy and destroying old one
    void* p_elem = NULL;
    switch(vector_element_size(vec)) {
    case 8 : p_elem = (void*)(Uint64) vector_at(vec, Uint64, pos); break;
    case 4 : p_elem = (void*)(Uint64) vector_at(vec, Uint32, pos); break;
    case 2 : p_elem = (void*)(Uint64) vector_at(vec, Uint16, pos); break;
    case 1 : p_elem = (void*)(Uint64) vector_at(vec, Uint8, pos); break;
    default :
        p_elem = ALLOCATE(Uint8, vec->element_size);
        RETURN_VALUE_IF_FAIL(p_elem, NULL, ERR_OUT_OF_MEMORY);
        memcpy(p_elem, vector_address_at(vec, pos), vec->element_size);
    }

    // shift array by one place at pos
    while(++pos < vec->length) {
        Size prev = pos-1;
        Size next = pos;
        switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, prev) = vector_at(vec, Uint64, next); break;
            case 4 : vector_at(vec, Uint32, prev) = vector_at(vec, Uint32, next); break;
            case 2 : vector_at(vec, Uint16, prev) = vector_at(vec, Uint16, next); break;
            case 1 : vector_at(vec, Uint8,  prev) = vector_at(vec, Uint8,  next); break;
            default :
                memcpy(vector_address_at(vec, prev), vector_address_at(vec, next), vec->element_size);
        }
    }

    vec->length--;
    return p_elem;
}

/**
 * Insert at element without preserving order of array.
 * Use this when you don't care about order being preserved or
 * if you can later sort array.
 * Data at insert position will be destroyed if possible.
 *
 * Insert will always create a copy of provided data for itself.
 * This ensures that vector will never free or misuse given pointer.
 * This also allows user to have it's own copy of data, by either
 * creating it on stack and passing pointer or just a heap allocated data.
 *
 * @param vec
 * @param p_data Data to be inserted.
 * @param pos Insert position.
 * */
FORCE_INLINE void vector_insert_fast(Vector* vec, void* p_data, Size pos) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = vec->capacity * (1 + vec->resize_factor);
        }

        // reallocate if we need to
        void* p_temp = realloc(vec->p_data, new_capacity * vector_element_size(vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        vec->p_data = p_temp;
        vec->capacity = new_capacity;
        vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(vec->length >= vec->capacity) {
        Size new_size = vec->capacity * (vec->resize_factor + 1);
        void* p_temp = realloc(vec->p_data, new_size * vector_element_size(vec));
        RETURN_IF_FAIL(p_temp, ERR_OUT_OF_MEMORY);
        vec->p_data = p_temp;
        vec->capacity = new_size;
    }

    // NOTE: No shift operation required here!

    if(pos < vec->length) {
        memcpy(vector_address_at(vec, vec->length), vector_address_at(vec, pos), vec->element_size);
    }

    Uint64 value = (Uint64)p_data;
    if(vec->pfn_create_copy) {
        void* p_elem = vector_address_at(vec, pos);
        if(p_data) vec->pfn_create_copy(p_elem, p_data);
        else memset(p_elem, 0, vec->element_size);
    } else switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, pos) = (Uint64)value; break;
            case 4 : vector_at(vec, Uint32, pos) = (Uint32)value; break;
            case 2 : vector_at(vec, Uint16, pos) = (Uint16)value; break;
            case 1 : vector_at(vec, Uint8, pos) = (Uint8)value; break;
            default: {
                void* p_elem = vector_address_at(vec, pos);
                if(p_data) memcpy(p_elem, p_data, vector_element_size(vec));
                else memset(p_elem, 0, vector_element_size(vec));
            }
        }

    vec->length++;
}

/**
 * Delete an element without preserving order.
 * Data at delete position will be destroyed if possible.
 * @param vec
 * @param pos Position of element where delete will happen
 * */
FORCE_INLINE void vector_delete_fast(Vector* vec, Size pos) {
    RETURN_IF_FAIL(vec && (pos < vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(vec->length, ERR_CONTAINER_UNDERFLOW);

    // if position is not the last element then we just move last value to
    // position that needs to be deleted
    if(pos != vec->length - 1) {
        vector_move(vec, pos, vec->length - 1);
    } else { // else we destroy copy
        if(vec->pfn_destroy_copy)
            vec->pfn_destroy_copy(vector_address_at(vec, pos));
        else memset(vector_address_at(vec, pos), 0, vec->element_size);
    }

    vec->length--;
}

/**
 * Remove an element fast without preserving element
 * @param vec
 * @param pos Position of element to be removed
 * */
FORCE_INLINE void* vector_remove_fast(Vector* vec, Size pos) {
    RETURN_VALUE_IF_FAIL(vec && (pos < vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    // create element copy without copy constructor
    void* p_elem = NULL;
    switch(vector_element_size(vec)) {
        case 8 : p_elem = (void*)(Uint64) vector_at(vec, Uint64, pos); break;
        case 4 : p_elem = (void*)(Uint64) vector_at(vec, Uint32, pos); break;
        case 2 : p_elem = (void*)(Uint64) vector_at(vec, Uint16, pos); break;
        case 1 : p_elem = (void*)(Uint64) vector_at(vec, Uint8, pos); break;
        default :
            p_elem = ALLOCATE(Uint8, vec->element_size);
            RETURN_VALUE_IF_FAIL(p_elem, NULL, ERR_OUT_OF_MEMORY);
            memcpy(p_elem, vector_address_at(vec, pos), vec->element_size);
    }

    // then just move last element to position that needs to be removed
    memcpy(vector_address_at(vec, pos), vector_address_at(vec, --vec->length), vec->element_size);

    return p_elem;
}

/**
 * Push an element to front by preserving order.
 * @param vec
 * @param p_data
 * */
void vector_push_front(Vector* vec, void* p_data) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    vector_insert(vec, p_data, 0);
}

/**
 * Remove an element from front by preserving order.
 * This doesn't result in a new copy of element and just
 * copy inside of Array is returned.
 * @param vec
 * @return Removed element.
 * */
void* vector_pop_front(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec && vec->length, NULL, ERR_INVALID_ARGUMENTS);
    return vector_remove(vec, 0);
}

/**
 * Push an element to front by preserving order.
 * @param vec
 * @param p_data
 * */
void vector_push_front_fast(Vector* vec, void* p_data) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    vector_insert(vec, p_data, 0);
}

/**
 * Remove an element from front by preserving order.
 * This doesn't result in a new copy of element and just
 * copy inside of Array is returned.
 * @param vec
 * @return Removed element.
 * */
void* vector_pop_front_fast(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);
    return vector_remove(vec, 0);
}

/**
 * Push an element to back. This will internally call
 * the fast insert operation.
 * @param vec
 * @param p_data
 * */
void vector_push_back(Vector* vec, void* p_data) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    vector_insert_fast(vec, p_data, vec->length);
}

/**
 * Remove an element from back. This will internally
 * call lthe fast remove operation.
 * @param vec
 * @return Removed element.
 * */
FORCE_INLINE void* vector_pop_back(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec && vec->length, NULL, ERR_INVALID_ARGUMENTS);
    return vector_remove_fast(vec, vec->length - 1);
}

/**
 * Get mutable reference of element at position inside
 * array.
 * @param vec
 * @param pos
 * @return Mutable reference to element at given position.
 * */
FORCE_INLINE void* vector_peek(Vector* vec, Size pos) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);
    switch(vec->element_size) {
        case 8: return (void*)vector_at(vec, Uint64, pos); break;
        case 4: return (void*)(Uint64)vector_at(vec, Uint32, pos); break;
        case 2: return (void*)(Uint64)vector_at(vec, Uint16, pos); break;
        case 1: return (void*)(Uint64)vector_at(vec, Uint8, pos); break;
        default: return vector_address_at(vec, pos);
    }
}

/**
 * Get mutable reference to data at front.
 * @param vec
 * @return Reference of element at front
 * */
void* vector_front(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);
    return vector_peek(vec, 0);
}

/**
 * Get mutable reference to data at the back of array.
 * @param vec
 * @return Reference to element at back
 * */
FORCE_INLINE void* vector_back(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);
    return vector_peek(vec, vec->length-1);
}

/**
 * Print all elements of this @c vector using given printer.
 * @param vec
 * @param pfn_printer Function to print each element.
 * */
void vector_print(Vector* vec, PrintElementCallback pfn_printer) {
    RETURN_IF_FAIL(vec && pfn_printer, ERR_INVALID_ARGUMENTS);

    switch(vec->element_size) {
        case 8 :
            for(Size iter = 0; iter < vec->length; iter++)
                pfn_printer((void*)vector_at(vec, Uint64, iter), iter);
            break;
        case 4 :
            for(Size iter = 0; iter < vec->length; iter++)
                pfn_printer((void*)(Uint64)vector_at(vec, Uint32, iter), iter);
            break;
        case 2 :
            for(Size iter = 0; iter < vec->length; iter++)
                pfn_printer((void*)(Uint64)vector_at(vec, Uint16, iter), iter);
            break;
        case 1 :
            for(Size iter = 0; iter < vec->length; iter++)
                pfn_printer((void*)(Uint64)vector_at(vec, Uint8, iter), iter);
            break;
        default:
            for(Size iter = 0; iter < vec->length; iter++)
                pfn_printer(vector_address_at(vec, iter), iter);
            break;
    }
}

/**
 * Merge two vectors into the first vector.
 * @param vec Vector to merge other vector into
 * @param vec_other Vector to be merged
 * */
void vector_merge(Vector* vec, Vector* vec_other) {
    RETURN_IF_FAIL(vec && vec_other, ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(vec->element_size == vec_other->element_size, "Arrays of different element sizes cannot be merged\n");

    for(Size s = 0; s < vec_other->length; s++) {
        vector_push_back(vec, vector_peek(vec_other, s));
    }
}

/**
 * Filter elements from this vector and return a new vector with filtered elements.
 * @param vec Vector to filter elements from.
 * @param pfn_filter Filter function to filter elements
 * @return New Vector object containing filtered elements on success, NULL otherwise.
 * */
Vector* vector_filter(Vector* vec, FilterElementCallback pfn_filter, void* p_user_data) {
    RETURN_VALUE_IF_FAIL(vec && pfn_filter, NULL, ERR_INVALID_ARGUMENTS);

    // create new vector for containing filtered vectors
    Vector* filtered_vec = vector_create(vec->element_size, vec->pfn_create_copy, vec->pfn_destroy_copy);
    RETURN_VALUE_IF_FAIL(filtered_vec, NULL, "Failed to create new filtered vector\n");

    // filter elements
    for(Size i = 0; i < vec->length; i++) {
        if(pfn_filter(vector_peek(vec, i), p_user_data)) {
            vector_push_back(filtered_vec, vector_peek(vec, i));
        }
    }

    return filtered_vec;
}

/**
 * Swap two elements in same vector.
 * @param vec Vector in which arrays will be sorted
 * @param p1 Position of first element
 * @param p2 Position of second element
 * */
FORCE_INLINE void vector_swap(Vector* vec, Size p1, Size p2) {
    RETURN_IF_FAIL(vec && (p1 < vec->length || p2 < vec->length), ERR_INVALID_ARGUMENTS);

    // define to keep less repetitions
#define SWAP_INT(type, i1, i2) {                                        \
        type t = vector_at(vec, type, p1);                        \
        vector_at(vec, type, p1) = vector_at(vec, type, p2); \
        vector_at(vec, type, p2) = t;                             \
        return;                                                         \
    }

    switch(vec->element_size) {
        case 8: SWAP_INT(Uint64, p1, p2);
        case 4: SWAP_INT(Uint32, p1, p2);
        case 2: SWAP_INT(Uint16, p1, p2);
        case 1: SWAP_INT(Uint8,  p1, p2);
        default: {
            Byte temp[vec->element_size];
            memcpy(temp, vector_address_at(vec, p1), vec->element_size);
            memcpy(vector_address_at(vec, p1), vector_address_at(vec, p2), vec->element_size);
            memcpy(vector_address_at(vec, p2), temp, vec->element_size);
            return;
        }
    }

#undef SWAP_INT
}

/**
 * Apply the fastest sort algorithm for given array possible.
 * @param vec Vector to be sorted
 * @param pfn_cmp Compare function
 * */
void vector_sort(Vector* vec, CompareElementCallback pfn_cmp) {
    RETURN_IF_FAIL(vec && pfn_cmp, ERR_INVALID_ARGUMENTS);
    vector_insertion_sort(vec, pfn_cmp);
}

/**
 * Check whether the array is sorted in any manner
 * By default the algorithm is written for checking array in descending order,
 * but this can be checked for any array type by modifying the compare method.
 *
 * This means if pfn_compare returns > 0 for any comparision then array is not sorted
 *
 * @param vec
 * @param pfn_compare
 * */
Bool vector_check_sorted(Vector* vec, CompareElementCallback pfn_compare) {
    RETURN_VALUE_IF_FAIL(vec && pfn_compare, False, ERR_INVALID_ARGUMENTS);
    for(Size s = 1; s < vec->length; s++)
        if(pfn_compare(vector_peek(vec, s), vector_peek(vec, s-1)) > 0)
            return False;
    return True;
}

/**
 * Use insertion sort mechanism to sort this array.
 * Time complexity:
 * BEST : O(n)
 * AVERAGE : O(n^2)
 * WORST : O(n^2)
 * @param vec
 * @param pfn_compare Compare function
 * */
void vector_insertion_sort(Vector* vec, CompareElementCallback pfn_compare) {
    RETURN_IF_FAIL(vec && pfn_compare, ERR_INVALID_ARGUMENTS);

    for(Size s = 1; s < vec->length; s++) {
        Size m = s;
        while((m > 0) && (pfn_compare(vector_peek(vec, m-1), vector_peek(vec, m)) > 0)) {
            vector_swap(vec, m, m-1); m--;
        }
    }
}

/**
 * Bubble sort algorithm.
 * @param vec
 * @param pfn_compare
 * */
void vector_bubble_sort(Vector* vec, CompareElementCallback pfn_compare) {
    RETURN_IF_FAIL(vec && pfn_compare, ERR_INVALID_ARGUMENTS);

    Bool b_swapped;
    for (Size i = 0; i < vec->length; i++) {
        b_swapped = False;
        for (Size j = 0; j < vec->length; j++) {
            if (pfn_compare(vector_peek(vec, j+1), vector_peek(vec, j)) > 0) {
                vector_swap(vec, j, j+1);
                b_swapped = True;
            }
        }
        if (!b_swapped)
            break;
    }
}

static inline void print_subarray(Vector* vec, Size start, Size size) {
    for(Size s = start; s < size; s++) {
        printf("%d, ", vector_at(vec, Uint32, s));
    }
}

static inline void merge_sort(Vector* vec, CompareElementCallback pfn_compare, Size start, Size size) {
    if(size == 1) return;

    RETURN_IF_FAIL(vec && pfn_compare && (start + size) <= vec->length, ERR_INVALID_ARGUMENTS);

    if((size == 2) && (pfn_compare(vector_peek(vec, start), vector_peek(vec, start + 1)) > 0)) {
        vector_swap(vec, start, start+1);
        return;
    }

    Size mid = start + size/2;
    merge_sort(vec, pfn_compare, start, size/2);
    merge_sort(vec, pfn_compare, mid, size - size/2);

    Size s = start;
    Size m = mid;

    Vector* subvec = vector_get_subvector(vec, start, size);
    subvec->pfn_create_copy = NULL; // reduce mempy calls
    subvec->pfn_destroy_copy = NULL;

    vector_reserve(subvec, size);

    // merge
    while((s < mid) && (m < start + size - 1)) {
        Int32 res = pfn_compare(vector_peek(vec, s), vector_peek(vec, m));
        if(res > 0) {
            vector_push_back(subvec, vector_peek(vec, s++));
        } else if (res < 0) {
            vector_push_back(subvec, vector_peek(vec, m++));
        } else {
            vector_push_back(subvec, vector_peek(vec, s++));
            vector_push_back(subvec, vector_peek(vec, m++));
        }
    }

    // copy back sorted array
    memcpy(vector_address_at(vec, start), subvec->p_data, subvec->element_size * subvec->length);
    vector_destroy(subvec);
}


/**
 * Merge sort algorithm
 * @param vec
 * @param pfn_compare
 * */
void vector_merge_sort(Vector* vec, CompareElementCallback pfn_compare) {
    RETURN_IF_FAIL(vec && pfn_compare, ERR_INVALID_ARGUMENTS);
    merge_sort(vec, pfn_compare, 0, vec->length);
}

/**
 * Create a copy of given vector, to a place already
 * allocated for destination (copy) vector.
 *
 * @param p_dst Vector* casted to void* where new vector will be copied.
 * Or more precisely, pointer to an already allocated place for new Vector.
 * @param p_src Vector* containing vector data to be copied.
 * */
void vector_create_copy(void* p_dst, void* p_src) {
    RETURN_IF_FAIL(p_dst && p_src, ERR_INVALID_ARGUMENTS);

    Vector* vec_dst = (Vector*)(p_dst);
    Vector* vec_src = (Vector*)(p_src);

    // allocate space for storing array
    vec_dst->p_data = ALLOCATE(Uint8, vec_src->element_size * vec_src->length);
    RETURN_IF_FAIL(vec_dst->p_data, ERR_OUT_OF_MEMORY);

    // initialize vector with basic data
    // note how length and capacity are initialized
    vec_dst->length           = 0;
    vec_dst->capacity         = vec_src->length;
    vec_dst->element_size     = vec_src->element_size;
    vec_dst->pfn_create_copy  = vec_src->pfn_create_copy;
    vec_dst->pfn_destroy_copy = vec_src->pfn_destroy_copy;
    vec_dst->resize_factor    = vec_src->resize_factor;

    // insert each element one by one
    // essentially using the copy constructors in src vector
    for(Size s = 0; s < vec_dst->length; s++) {
        vector_push_back(vec_dst, vector_peek(vec_src, s));
    }
}

/**
 * Destroy given copy of Vector.
 *
 * @param p_copy Copy of Vector* created using vector_create_copy().
 * */
void vector_destroy_copy(void* p_copy) {
    RETURN_IF_FAIL(p_copy, ERR_INVALID_ARGUMENTS);

    Vector* vec_copy = (Vector*)p_copy;

    if(vec_copy->length) vector_clear(vec_copy);
    FREE(vec_copy->p_data);
    vec_copy->p_data = NULL;
}
