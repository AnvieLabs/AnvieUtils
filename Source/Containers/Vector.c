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
 * If any one of @c create_copy or @c destroy_copy is non null,
 * then both must be non null!
 *
 * @param element_size Size of each element in this dynamic array.
 * @param create_copy Copy constructor for elements. Can be NULL.
 * @param destroy_copy Copy destructor for elements. Can be NULL.
 * @return Vector* or NULL if allocation failed
 * */
Vector* vector_create(Size element_size,
                            CreateElementCopyCallback create_copy,
                            DestroyElementCopyCallback destroy_copy)
{
    RETURN_VALUE_IF_FAIL(element_size, NULL, ERR_INVALID_ARGUMENTS);

    // both must be null or non null at the same time
    Bool b1 = create_copy != NULL;
    Bool b2 = destroy_copy != NULL;
    RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL,
                         "Either both copy constructor and destructor should be NULL, "
                         "or both should be non NULL at the same time!");

    // create a new dyn array object
    Vector* vec = NEW(Vector);
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_OUT_OF_MEMORY);

    // allocate initial memory
    vec->data = ALLOCATE(Uint8, AV_VECTOR_INIT_ELEMENT_COUNT * element_size);
    if(!vec->data) {
        WARN_IF(True, ERR_OUT_OF_MEMORY);
        FREE(vec);
        return NULL;
    }
    vec->capacity = AV_VECTOR_INIT_ELEMENT_COUNT;
    vec->element_size = element_size;
    vec->resize_factor = AV_VECTOR_DEFAULT_RESIZE_FACTOR;
    vec->create_copy = create_copy;
    vec->destroy_copy = destroy_copy;
    return vec;
}

/**
 * Destroy given dynamic array.
 * @param vec Vector to be destroyed
 * @param udata User data to be passed to callback functions.
 * */
void vector_destroy(Vector* vec, void* udata) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // destroy all object copies if possible and memset whole array!
    if(vec->length != 0) vector_clear(vec, udata);

    FREE(vec->data);
    vec->data = NULL;
    FREE(vec);
}

/**
 * Create a clone of this vector.
 * This will not only create clone of this vector but also all the
 * data that it contains. Meaning these two vectors might look like
 * clones but they're internally completely different in memory!
 * @param vec Vector to be cloned.
 * @param udata User data to be passed to callback functions.
 * */
Vector* vector_clone(Vector* vec, void* udata) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);

    Vector* vec_clone = NEW(Vector);
    RETURN_VALUE_IF_FAIL(vec_clone, NULL, ERR_OUT_OF_MEMORY);

    // create copy of given vector
    // this is the copy constructor used in vector of vectors
    vector_create_copy(vec_clone, vec, udata);

    return vec_clone;
}

/**
 * Resize the array to contain given number of elements
 * @param vec
 * @param new_size New capacity of this dyn array.
 * */
inline void vector_resize(Vector* vec, Size new_size) {
    RETURN_IF_FAIL(vec && new_size, ERR_INVALID_ARGUMENTS);

    void* temp = realloc(vec->data, new_size * vec->element_size);
    RETURN_IF_FAIL(temp, ERR_OUT_OF_MEMORY);

    // memset to make make new area nullified
    // ref : https://stackoverflow.com/a/32732502
    Size old_size_in_bytes = vec->length * vec->element_size;
    Size new_size_in_bytes = new_size * vec->element_size;
    memset((UByteArray)temp + old_size_in_bytes, 0, new_size_in_bytes - old_size_in_bytes);

    vec->data = temp;
    vec->capacity = new_size;
    vec->length = new_size;
}

/**
 * Reserve space for new array. This does not increase length of array.
 * @param vec
 * @param capacity
 * */
inline void vector_reserve(Vector* vec, Size capacity) {
    RETURN_IF_FAIL(vec && capacity, ERR_INVALID_ARGUMENTS);
    vector_resize(vec, capacity);
    vec->length = 0;
}

/**
 * Clear vector.
 * Destroy each element if copy destructor is provided,
 * otherwise, just length is set to 0 in order to save time.
 * @param vec Vector to be destroyed
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_clear(Vector* vec, void* udata) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // destroy element copies if we need to
    if(vec->destroy_copy) {
        while(vec->length--) {
            vec->destroy_copy(vector_address_at(vec, vec->length), udata);
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
Vector* vector_get_subvector(Vector* vec, Size start, Size size, void* udata) {
    RETURN_VALUE_IF_FAIL(vec && size, NULL, ERR_INVALID_ARGUMENTS);

    Vector* new_vec = vector_create(vec->element_size, vec->create_copy, vec->destroy_copy);
    for(Size s = start; s < size; s++) {
        vector_push_back(new_vec, vector_peek(vec, s), udata);
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
 * @param vec
 * @param to Final move position.
 * @param from Position of element to be moved.
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_move(Vector* vec, Size to, Size from, void* udata) {
    if(to == from) return;
    RETURN_IF_FAIL(vec && (to < vec->capacity) && (from < vec->length), ERR_INVALID_ARGUMENTS);

    // we need to destroy element if it's being overwritten to
    if((to < vec->length) && vec->destroy_copy) {
        vec->destroy_copy(vector_address_at(vec, to), udata);
    }

    memcpy(vector_address_at(vec, to), vector_address_at(vec, from), vec->element_size);
    memset(vector_address_at(vec, from), 0, vector_element_size(vec));
}

/**
 * Creates a copy of element by destroying whatever is in target position.
 * This results in two different copies of same elment. If a copy constructor
 * is assigned to vector then it's used to create copy of element.
 * @p from must be less than @c length of vector.
 * @p to must be less than @c capacity of vector.
 * @p to must be different from @p from
 * @param vec
 * @param to Target position of element.
 * @param from Position of element to be copied.
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_copy(Vector* vec, Size to, Size from, void* udata) {
    RETURN_IF_FAIL(vec && (to < vec->capacity) && (from < vec->length) && (to != from), ERR_INVALID_ARGUMENTS);

    // use copy constructor whenever possible
    if(vec->create_copy) {
        void* elem_to = vector_address_at(vec, to);
        void* elem_from = vector_address_at(vec, from);
        if(to < vec->length) vec->destroy_copy(elem_to, udata);
        vec->create_copy(elem_to, elem_from, udata);
    } else switch(vector_element_size(vec)) {
        case 8 : vector_at(vec, Uint64, to) = (Uint64)vector_at(vec, Uint64, from); break;
        case 4 : vector_at(vec, Uint32, to) = (Uint64)vector_at(vec, Uint32, from); break;
        case 2 : vector_at(vec, Uint16, to) = (Uint64)vector_at(vec, Uint16, from); break;
        case 1 : vector_at(vec, Uint8, to) = (Uint64)vector_at(vec, Uint8, from); break;
        default : {
            void* elem_to = vector_address_at(vec, to);
            void* elem_from = vector_address_at(vec, from);
            memcpy(elem_to, elem_from, vector_element_size(vec));
        }
    }
}

/**
 * Move an element between two positions.
 * This will automatically destroy element where data is to be
 * overwritten before writing anything.
 * @param vec
 * @param pos Position where this new data
 * @param data Data to be moved
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_overwrite(Vector* vec, Size pos, void* data, void* udata) {
    RETURN_IF_FAIL(vec && (pos < vec->capacity), ERR_INVALID_ARGUMENTS);

    if(vec->create_copy) {
        void* elem = vector_address_at(vec, pos);
        if(pos < vec->length) vec->destroy_copy(elem, udata);
        if(data) vec->create_copy(elem, data, udata);
        else memset(elem, 0, vec->element_size);
    } else {
        Uint64 value = (Uint64)data;
        switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, pos) = (Uint64)value; break;
            case 4 : vector_at(vec, Uint32, pos) = (Uint32)value; break;
            case 2 : vector_at(vec, Uint16, pos) = (Uint16)value; break;
            case 1 : vector_at(vec, Uint8, pos) = (Uint8)value; break;
            default : {
                void* elem = vector_address_at(vec, pos);
                if(data) memcpy(elem, data, vector_element_size(vec));
                else memset(elem, 0, vector_element_size(vec));
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
 * @param vec
 * @param data Pointer to memory or just the value to be inserted into
 * contiguous array maintained in @c vector.
 * @param pos Position where insertion will take place.
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_insert(Vector* vec, void *data, Size pos, void* udata) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = vec->capacity * (1 + vec->resize_factor);
        }

        // reallocate if we need to
        void* temp = realloc(vec->data, new_capacity * vector_element_size(vec));
        RETURN_IF_FAIL(temp, ERR_OUT_OF_MEMORY);
        vec->data = temp;
        vec->capacity = new_capacity;
        vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(vec->length >= vec->capacity) {
        Size new_size = vec->capacity * (vec->resize_factor + 1);
        void* temp = realloc(vec->data, new_size * vector_element_size(vec));
        RETURN_IF_FAIL(temp, ERR_OUT_OF_MEMORY);
        vec->data = temp;
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
                void* elem_to = vector_address_at(vec, to);
                void* elem_from = vector_address_at(vec, from);
                memcpy(elem_to, elem_from, vector_element_size(vec));
            }
        }
    }

    Uint64 value = (Uint64)data;

    // use copy constructor to create a copy for vector
    // this is really useful in case of structures that contain other pointers
    // or strings for eg
    if(vec->create_copy) {
        void* elem = vector_address_at(vec, pos);
        if(data) vec->create_copy(elem, data, udata);
        else memset(elem, 0, vec->element_size);
    } else switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, pos) = (Uint64)value; break;
            case 4 : vector_at(vec, Uint32, pos) = (Uint32)value; break;
            case 2 : vector_at(vec, Uint16, pos) = (Uint16)value; break;
            case 1 : vector_at(vec, Uint8,  pos) = (Uint8) value; break;
            default: {
                void* elem = vector_address_at(vec, pos);
                if(data) memcpy(elem, data, vector_element_size(vec));
                else memset(elem, 0, vector_element_size(vec));
            }
        }

    vec->length++;
}

/**
 * Delete an element from given position.
 * Order will be preserved.
 * @param vec
 * @param pos Positon of element to be deleted.
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_delete(Vector* vec, Size pos, void* udata) {
    RETURN_IF_FAIL(vec && (pos < vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(vec->length, ERR_CONTAINER_UNDERFLOW);

    // destroy element at position if possible
    if(vec->destroy_copy) {
        vec->destroy_copy(vector_address_at(vec, pos), udata);
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
inline void* vector_remove(Vector* vec, Size pos) {
    RETURN_VALUE_IF_FAIL(vec && (pos < vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    // create a copy of element out of vector
    // here we can just return the copy that vector has,
    // instead of creating another copy and destroying old one
    void* elem = NULL;
    switch(vector_element_size(vec)) {
    case 8 : elem = (void*)(Uint64) vector_at(vec, Uint64, pos); break;
    case 4 : elem = (void*)(Uint64) vector_at(vec, Uint32, pos); break;
    case 2 : elem = (void*)(Uint64) vector_at(vec, Uint16, pos); break;
    case 1 : elem = (void*)(Uint64) vector_at(vec, Uint8, pos); break;
    default :
        elem = ALLOCATE(Uint8, vec->element_size);
        RETURN_VALUE_IF_FAIL(elem, NULL, ERR_OUT_OF_MEMORY);
        memcpy(elem, vector_address_at(vec, pos), vec->element_size);
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
    return elem;
}

/**
 * Insert at element without preserving order of array.
 * Use this when you don't care about order being preserved or
 * if you can later sort array.
 * Insert will always create a copy of provided data for itself.
 * This ensures that vector will never free or misuse given pointer.
 * This also allows user to have it's own copy of data, by either
 * creating it on stack and passing pointer or just a heap allocated data.
 * @param vec
 * @param data Data to be inserted.
 * @param pos Insert position.
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_insert_fast(Vector* vec, void* data, Size pos, void* udata) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);

    // resize array if insert position is more than capacity
    if(pos >= vec->capacity) {
        // calculate new allocation capacity if required
        Size new_capacity = vec->capacity;
        while(pos >= new_capacity) {
            new_capacity = vec->capacity * (1 + vec->resize_factor);
        }

        // reallocate if we need to
        void* temp = realloc(vec->data, new_capacity * vector_element_size(vec));
        RETURN_IF_FAIL(temp, ERR_OUT_OF_MEMORY);
        vec->data = temp;
        vec->capacity = new_capacity;
        vec->length = pos;
    }

    // resize array if insert position is in between but array is at capacity
    if(vec->length >= vec->capacity) {
        Size new_size = vec->capacity * (vec->resize_factor + 1);
        void* temp = realloc(vec->data, new_size * vector_element_size(vec));
        RETURN_IF_FAIL(temp, ERR_OUT_OF_MEMORY);
        vec->data = temp;
        vec->capacity = new_size;
    }

    // NOTE: No shift operation required here! compared to vanilla insert op

    if(pos < vec->length) {
        memcpy(vector_address_at(vec, vec->length), vector_address_at(vec, pos), vec->element_size);
    }

    Uint64 value = (Uint64)data;
    if(vec->create_copy) {
        void* elem = vector_address_at(vec, pos);
        if(data) vec->create_copy(elem, data, udata);
        else memset(elem, 0, vec->element_size);
    } else switch(vector_element_size(vec)) {
            case 8 : vector_at(vec, Uint64, pos) = (Uint64)value; break;
            case 4 : vector_at(vec, Uint32, pos) = (Uint32)value; break;
            case 2 : vector_at(vec, Uint16, pos) = (Uint16)value; break;
            case 1 : vector_at(vec, Uint8, pos) = (Uint8)value; break;
            default: {
                void* elem = vector_address_at(vec, pos);
                if(data) memcpy(elem, data, vector_element_size(vec));
                else memset(elem, 0, vector_element_size(vec));
            }
        }

    vec->length++;
}

/**
 * Delete an element without preserving order.
 * Data at delete position will be destroyed if possible.
 * @param vec
 * @param pos Position of element where delete will happen.
 * @param udata User data to be passed to callback functions.
 * */
inline void vector_delete_fast(Vector* vec, Size pos, void* udata) {
    RETURN_IF_FAIL(vec && (pos < vec->length), ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(vec->length, ERR_CONTAINER_UNDERFLOW);

    // if position is not the last element then we just move last value to
    // position that needs to be deleted
    if(pos != vec->length - 1) {
        vector_move(vec, pos, vec->length - 1, udata);
    } else { // else we destroy copy
        if(vec->destroy_copy)
            vec->destroy_copy(vector_address_at(vec, pos), udata);
        else memset(vector_address_at(vec, pos), 0, vec->element_size);
    }

    vec->length--;
}

/**
 * Remove an element fast without preserving element
 * @param vec
 * @param pos Position of element to be removed
 * */
inline void* vector_remove_fast(Vector* vec, Size pos) {
    RETURN_VALUE_IF_FAIL(vec && (pos < vec->length), NULL, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(vec->length, NULL, ERR_CONTAINER_UNDERFLOW);

    // create element copy without copy constructor
    void* elem = NULL;
    switch(vector_element_size(vec)) {
        case 8 : elem = (void*)(Uint64) vector_at(vec, Uint64, pos); break;
        case 4 : elem = (void*)(Uint64) vector_at(vec, Uint32, pos); break;
        case 2 : elem = (void*)(Uint64) vector_at(vec, Uint16, pos); break;
        case 1 : elem = (void*)(Uint64) vector_at(vec, Uint8, pos); break;
        default :
            elem = ALLOCATE(Uint8, vec->element_size);
            RETURN_VALUE_IF_FAIL(elem, NULL, ERR_OUT_OF_MEMORY);
            memcpy(elem, vector_address_at(vec, pos), vec->element_size);
    }

    // then just move last element to position that needs to be removed
    memcpy(vector_address_at(vec, pos), vector_address_at(vec, --vec->length), vec->element_size);

    return elem;
}

/**
 * Push an element to front by preserving order.
 * @param vec
 * @param data
 * @param udata User data to be passed to callback functions.
 * */
void vector_push_front(Vector* vec, void* data, void* udata) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    vector_insert(vec, data, 0, udata);
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
 * Push an element to front without preserving order.
 * @param vec
 * @param data
 * @param udata User data to be passed to callback functions.
 * */
void vector_push_front_fast(Vector* vec, void* data, void* udata) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    vector_insert_fast(vec, data, 0, udata);
}

/**
 * Remove an element from front without preserving order.
 * This doesn't result in a new copy of element and just
 * copy inside of Array is returned.
 * @param vec
 * @return Removed element.
 * */
void* vector_pop_front_fast(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);
    return vector_remove_fast(vec, 0);
}

/**
 * Push an element to back. This will internally call
 * the fast insert operation.
 * @param vec
 * @param data
 * @param udata User data to be passed to callback functions.
 * */
void vector_push_back(Vector* vec, void* data, void* udata) {
    RETURN_IF_FAIL(vec, ERR_INVALID_ARGUMENTS);
    vector_insert_fast(vec, data, vec->length, udata);
}

/**
 * Remove an element from back. This will internally
 * call lthe fast remove operation.
 * @param vec
 * @return Removed element.
 * */
inline void* vector_pop_back(Vector* vec) {
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
inline void* vector_peek(Vector* vec, Size pos) {
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
inline void* vector_back(Vector* vec) {
    RETURN_VALUE_IF_FAIL(vec, NULL, ERR_INVALID_ARGUMENTS);
    return vector_peek(vec, vec->length-1);
}

/**
 * Print all elements of this @c vector using given printer.
 * @param vec
 * @param printer Function to print each element.
 * */
void vector_print(Vector* vec, PrintElementCallback printer, void* udata) {
    RETURN_IF_FAIL(vec && printer, ERR_INVALID_ARGUMENTS);

    switch(vec->element_size) {
        case 8 :
            for(Size iter = 0; iter < vec->length; iter++)
                printer((void*)vector_at(vec, Uint64, iter), iter, udata);
            break;
        case 4 :
            for(Size iter = 0; iter < vec->length; iter++)
                printer((void*)(Uint64)vector_at(vec, Uint32, iter), iter, udata);
            break;
        case 2 :
            for(Size iter = 0; iter < vec->length; iter++)
                printer((void*)(Uint64)vector_at(vec, Uint16, iter), iter, udata);
            break;
        case 1 :
            for(Size iter = 0; iter < vec->length; iter++)
                printer((void*)(Uint64)vector_at(vec, Uint8, iter), iter, udata);
            break;
        default:
            for(Size iter = 0; iter < vec->length; iter++)
                printer(vector_address_at(vec, iter), iter, udata);
            break;
    }
}

/**
 * Merge two vectors into the first vector.
 * @param vec Vector to merge other vector into
 * @param vec_other Vector to be merged
 * @param udata User data to be passed to callback functions.
 * */
void vector_merge(Vector* vec, Vector* vec_other, void* udata) {
    RETURN_IF_FAIL(vec && vec_other, ERR_INVALID_ARGUMENTS);
    RETURN_IF_FAIL(vec->element_size == vec_other->element_size, "Arrays of different element sizes cannot be merged\n");

    for(Size s = 0; s < vec_other->length; s++) {
        vector_push_back(vec, vector_peek(vec_other, s), udata);
    }
}

/**
 * Filter elements from this vector and return a new vector with filtered elements.
 * @param vec Vector to filter elements from.
 * @param filter Filter function to filter elements
 * @param udata User data to be passed to callback functions.
 * @return New Vector object containing filtered elements on success, NULL otherwise.
 * */
Vector* vector_filter(Vector* vec, FilterElementCallback filter, void* udata) {
    RETURN_VALUE_IF_FAIL(vec && filter, NULL, ERR_INVALID_ARGUMENTS);

    // create new vector for containing filtered vectors
    Vector* filtered_vec = vector_create(vec->element_size, vec->create_copy, vec->destroy_copy);
    RETURN_VALUE_IF_FAIL(filtered_vec, NULL, "Failed to create new filtered vector\n");

    // filter elements
    for(Size i = 0; i < vec->length; i++) {
        if(filter(vector_peek(vec, i), udata)) {
            vector_push_back(filtered_vec, vector_peek(vec, i), udata);
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
inline void vector_swap(Vector* vec, Size p1, Size p2) {
    RETURN_IF_FAIL(vec && (p1 < vec->length || p2 < vec->length), ERR_INVALID_ARGUMENTS);

    // define to keep less repetitions
#define SWAINT(type, i1, i2) {                                        \
        type t = vector_at(vec, type, p1);                        \
        vector_at(vec, type, p1) = vector_at(vec, type, p2); \
        vector_at(vec, type, p2) = t;                             \
        return;                                                         \
    }

    switch(vec->element_size) {
        case 8: SWAINT(Uint64, p1, p2);
        case 4: SWAINT(Uint32, p1, p2);
        case 2: SWAINT(Uint16, p1, p2);
        case 1: SWAINT(Uint8,  p1, p2);
        default: {
            Byte temp[vec->element_size];
            memcpy(temp, vector_address_at(vec, p1), vec->element_size);
            memcpy(vector_address_at(vec, p1), vector_address_at(vec, p2), vec->element_size);
            memcpy(vector_address_at(vec, p2), temp, vec->element_size);
            return;
        }
    }

#undef SWAINT
}

/**
 * Apply the fastest sort algorithm for given array possible.
 * @param vec Vector to be sorted
 * @param cmp Compare function
 * @param udata User data to be passed to callback functions.
 * */
void vector_sort(Vector* vec, CompareElementCallback cmp, void* udata) {
    RETURN_IF_FAIL(vec && cmp, ERR_INVALID_ARGUMENTS);
    vector_insertion_sort(vec, cmp, udata);
}

/**
 * Check whether the array is sorted in any manner
 * By default the algorithm is written for checking array in descending order,
 * but this can be checked for any array type by modifying the compare method.
 *
 * This means if compare returns > 0 for any comparision then array is not sorted
 *
 * @param vec
 * @param compare
 * @param udata User data to be passed to callback functions.
 * */
Bool vector_check_sorted(Vector* vec, CompareElementCallback compare, void* udata) {
    RETURN_VALUE_IF_FAIL(vec && compare, False, ERR_INVALID_ARGUMENTS);
    for(Size s = 1; s < vec->length; s++)
        if(compare(vector_peek(vec, s), vector_peek(vec, s-1), udata) > 0)
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
 * @param compare Compare function
 * @param udata User data to be passed to callback functions.
 * */
void vector_insertion_sort(Vector* vec, CompareElementCallback compare, void* udata) {
    RETURN_IF_FAIL(vec && compare, ERR_INVALID_ARGUMENTS);

    for(Size s = 1; s < vec->length; s++) {
        Size m = s;
        while((m > 0) && (compare(vector_peek(vec, m-1), vector_peek(vec, m), udata) > 0)) {
            vector_swap(vec, m, m-1); m--;
        }
    }
}

/**
 * Bubble sort algorithm.
 * @param vec
 * @param compare
 * @param udata User data to be passed to callback functions.
 * */
void vector_bubble_sort(Vector* vec, CompareElementCallback compare, void* udata) {
    RETURN_IF_FAIL(vec && compare, ERR_INVALID_ARGUMENTS);

    Bool b_swapped;
    for (Size i = 0; i < vec->length; i++) {
        b_swapped = False;
        for (Size j = 0; j < vec->length; j++) {
            if (compare(vector_peek(vec, j+1), vector_peek(vec, j), udata) > 0) {
                vector_swap(vec, j, j+1);
                b_swapped = True;
            }
        }
        if (!b_swapped)
            break;
    }
}

static inline void merge_sort(Vector* vec, CompareElementCallback compare, Size start, Size size, void* udata) {
    if(size < 2) return;

    RETURN_IF_FAIL(vec && compare && (start + size) <= vec->length, ERR_INVALID_ARGUMENTS);

    if((size == 2) && (compare(vector_peek(vec, start), vector_peek(vec, start + 1), udata) > 0)) {
        vector_swap(vec, start, start+1);
        return;
    }

    Size mid = start + size/2;
    merge_sort(vec, compare, start, size/2, udata);
    merge_sort(vec, compare, mid, size - size/2, udata);

    Size s = start;
    Size m = mid;

    Vector* subvec = vector_get_subvector(vec, start, size, udata);
    subvec->create_copy = NULL; // reduce mempy calls
    subvec->destroy_copy = NULL;

    vector_reserve(subvec, size);

    // merge
    while((s < mid) && (m < start + size - 1)) {
        Int32 res = compare(vector_peek(vec, s), vector_peek(vec, m), udata);
        if(res > 0) {
            vector_push_back(subvec, vector_peek(vec, s++), NULL);
        } else if (res < 0) {
            vector_push_back(subvec, vector_peek(vec, m++), NULL);
        } else {
            vector_push_back(subvec, vector_peek(vec, s++), NULL);
            vector_push_back(subvec, vector_peek(vec, m++), NULL);
        }
    }

    // copy back sorted array
    memcpy(vector_address_at(vec, start), subvec->data, subvec->element_size * subvec->length);
    vector_destroy(subvec, NULL);
}


/**
 * Merge sort algorithm
 * @param vec
 * @param compare
 * @param udata User data to be passed to callback functions.
 * */
void vector_merge_sort(Vector* vec, CompareElementCallback compare, void* udata) {
    RETURN_IF_FAIL(vec && compare, ERR_INVALID_ARGUMENTS);
    merge_sort(vec, compare, 0, vec->length, udata);
}

/**
 * Create a copy of given vector, to a place already
 * allocated for destination (copy) vector.
 *
 * @param dst Vector* casted to void* where new vector will be copied.
 * Or more precisely, pointer to an already allocated place for new Vector.
 * @param src Vector* containing vector data to be copied.
 * */
void vector_create_copy(void* dst, void* src, void* udata) {
    RETURN_IF_FAIL(dst && src, ERR_INVALID_ARGUMENTS);

    Vector* vec_dst = (Vector*)(dst);
    Vector* vec_src = (Vector*)(src);

    // allocate space for storing array
    vec_dst->data = ALLOCATE(Uint8, vec_src->element_size * vec_src->length);
    RETURN_IF_FAIL(vec_dst->data, ERR_OUT_OF_MEMORY);

    // initialize vector with basic data
    // note how length and capacity are initialized
    vec_dst->length        = 0;
    vec_dst->capacity      = vec_src->length;
    vec_dst->element_size  = vec_src->element_size;
    vec_dst->create_copy   = vec_src->create_copy;
    vec_dst->destroy_copy  = vec_src->destroy_copy;
    vec_dst->resize_factor = vec_src->resize_factor;

    // insert each element one by one
    // essentially using the copy constructors in src vector
    for(Size s = 0; s < vec_dst->length; s++) {
        vector_push_back(vec_dst, vector_peek(vec_src, s), udata);
    }
}

/**
 * Destroy given copy of Vector.
 *
 * @param copy Copy of Vector* created using vector_create_copy().
 * */
void vector_destroy_copy(void* copy, void* udata) {
    RETURN_IF_FAIL(copy, ERR_INVALID_ARGUMENTS);

    Vector* vec_copy = (Vector*)copy;

    if(vec_copy->length) vector_clear(vec_copy, udata);
    FREE(vec_copy->data);
    vec_copy->data = NULL;
}
