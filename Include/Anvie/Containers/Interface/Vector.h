/**
 * @file Vector.h
 * @date Mon, 14th July, 2023
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
 * @brief Defines macros that'll help in quick creation of vectors for any type.
 * Take a look at IntegerVectorTests.c and StructVectorTests.c for more insight on how to use these.
 *
 * This file defines three macros :
 * - DEF_INTEGER_VECTOR_INTERFACE (integers)
 * - DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY (mostly pointers)
 * - DEF_STRUCT_VECTOR_INTERFACE (structs)
 * */

#ifndef UTILS_VECTOR_INTERFACE_H
#define UTILS_VECTOR_INTERFACE_H

#include <Anvie/HelperDefines.h>
#include <Anvie/Containers/Interface/Common.h>

/**
 * @def DEF_INTEGER_VECTOR_INTERFACE
 * @brief Define the Integer Vector Container Interface
 *
 * This macro defines the interface for an integer vector container
 * with default copy and destroy functions.
 *
 * @param api_prefix The API prefix for functions (e.g., `MYLIB_`).
 * @param typename The typename for the integer vector container.
 * @param type The type of integer elements stored in the vector.
 */
#define DEF_INTEGER_VECTOR_INTERFACE(api_prefix, typename, type)        \
    DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, typename, type, NULL, NULL)

/**
 * @def DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY
 * @brief Define the Integer Vector Container Interface with Copy and Destroy Functions
 *
 * This macro defines the interface for an integer vector container with copy and destroy functions.
 *
 * @param api_prefix The API prefix for functions (e.g., `MYLIB_`).
 * @param typename The typename for the integer vector container.
 * @param type The type of integer elements stored in the vector.
 * @param copy Callback function for copying integer elements.
 * @param destroy Callback function for destroying integer elements.
 */
#define DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, typename, type, copy, destroy) \
    /**
     * Define the same Vector but with type specific members.
     * Note that size of this vector and the generic vector is
     * exactly same, so they can be used interchangeably by type
     * casting using pointer magic.
     * Meaning more type-safety when calling the type-specific
     * api wrappers.
     * */                                                               \
    DEF_INTEGER_TYPE_SPECIFIC_CALLBACKS(type, typename);                \
    typedef struct typename##_Vector {                                  \
        Size                            element_size;                   \
        Size                            length;                         \
        Size                            capacity;                       \
        type*                           data;                           \
        Create##typename##CopyCallback  create_copy;                    \
        Destroy##typename##CopyCallback destroy_copy;                   \
        Float32                         resize_factor;                  \
    } typename##_Vector;                                                \
                                                                        \
    /**
     * Now each api wrapper is completely different from other api wrappers.
     * Because each vector type is different from other, as long
     * as typename is different.
     * */                                                               \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_create() {      \
        return (typename##_Vector*)vector_create(sizeof(type),          \
                             (CreateElementCopyCallback)(void*)(copy),  \
                             (DestroyElementCopyCallback)(void*)destroy); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_destroy(typename##_Vector* vec, void* udata) { \
        vector_destroy((Vector*)vec, udata);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_clone(typename##_Vector* vec, void* udata) { \
        return (typename##_Vector*)vector_clone((Vector*)vec, udata);   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_resize(typename##_Vector* vec, Size sz) { \
        vector_resize((Vector*)vec, sz);                                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_reserve(typename##_Vector* vec, Size sz) { \
        vector_reserve((Vector*)vec, sz);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_clear(typename##_Vector* vec, void* udata) { \
        vector_clear((Vector*)vec, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_get_subvector(typename##_Vector* vec, Size start, Size size, void* udata) { \
        return (typename##_Vector*)vector_get_subvector((Vector*)vec, start, size, udata);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_copy(typename##_Vector* vec, Size to, Size from, void* udata) { \
        vector_copy((Vector*)vec, to, from, udata);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_move(typename##_Vector* vec, Size to, Size from, void* udata) { \
        vector_move((Vector*)vec, to, from, udata);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_overwrite(typename##_Vector* vec, Size to, type value, void* udata) { \
        vector_overwrite((Vector*)vec, to, (void*)(Uint64)value, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert(typename##_Vector* vec, type value, Size pos, void* udata) { \
        vector_insert((Vector*)vec, (void*)(Uint64)value, pos, udata);  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete(typename##_Vector* vec, Size pos, void* udata) { \
        vector_delete((Vector*)vec, pos, udata);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_remove(typename##_Vector* vec, Size pos) { \
        return (type)(Uint64)vector_remove((Vector*)vec, pos);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert_fast(typename##_Vector* vec, type value, Size pos, void* udata) { \
        vector_insert_fast((Vector*)vec, (void*)(Uint64)value, pos, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete_fast(typename##_Vector* vec, Size pos, void* udata) { \
        vector_delete_fast((Vector*)vec, pos, udata);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_remove_fast(typename##_Vector* vec, Size pos) { \
        return (type)(Uint64)vector_remove_fast((Vector*)vec, pos);     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front(typename##_Vector* vec, type value, void* udata) { \
        vector_push_front((Vector*)vec, (void*)(Uint64)value, udata);   \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_pop_front(typename##_Vector* vec) { \
        return (type)(Uint64)vector_pop_front((Vector*)vec);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front_fast(typename##_Vector* vec, type value, void* udata) { \
        vector_push_front_fast((Vector*)vec, (void*)(Uint64)value, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_pop_front_fast(typename##_Vector* vec) { \
        return (type)(Uint64)vector_pop_front_fast((Vector*)vec);       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_back(typename##_Vector* vec, type value, void* udata) { \
        vector_push_back((Vector*)vec, (void*)(Uint64)value, udata);    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_pop_back(typename##_Vector* vec) { \
        return (type)(Uint64)vector_pop_back((Vector*)vec);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_peek(typename##_Vector* vec, Size pos) { \
        return (type)(Uint64)vector_peek((Vector*)vec, pos);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_front(typename##_Vector* vec) { \
        return (type)(Uint64)vector_front((Vector*)vec);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_back(typename##_Vector* vec) { \
        return (type)(Uint64)vector_back((Vector*)vec);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_print(typename##_Vector* vec, PrintElementCallback printer, void* udata) { \
        vector_print((Vector*)vec, (PrintElementCallback)(void*)printer, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_merge(typename##_Vector* vec, typename##_Vector* vec_other, void* udata) { \
        vector_merge((Vector*)vec, (Vector*)vec_other, udata);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_filter(typename##_Vector* vec, Filter##typename##Callback filter, void* udata) { \
        return (typename##_Vector*)vector_filter((Vector*)vec, (FilterElementCallback)(void*)filter, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_swap(typename##_Vector* vec, Size p1, Size p2) { \
        vector_swap((Vector*)vec, p1, p2);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_sort(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        vector_sort((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool api_prefix##_vector_check_sorted(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        return vector_check_sorted((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insertion_sort(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        vector_insertion_sort((Vector*)vec, (CompareElementCallback)(void*)compare, udata);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_bubble_sort(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        vector_bubble_sort((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_merge_sort(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        vector_merge_sort((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_data(typename##_Vector* vec) { \
        return vec ? vec->data : NULL;                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_address_at(typename##_Vector* vec, Size idx) { \
        return (type*)vector_address_at((Vector*)vec, idx);             \
    }

/**
 * @def DEF_STRUCT_VECTOR_INTERFACE
 * @brief Define the Vector Container Interface
 *
 * This macro defines the interface for a vector container.
 *
 * @param api_prefix The API prefix for functions (e.g., `MYLIB_`).
 * @param typename The typename for the vector container.
 * @param type The type of elements stored in the vector.
 * @param copy_create Callback function for creating a copy of elements.
 * @param copy_destroy Callback function for destroying a copy of elements.
 */
#define DEF_STRUCT_VECTOR_INTERFACE(api_prefix, typename, type, copy_create, copy_destroy) \
    /**
     * Define the same vector but with type specific members.
     * This will allow easy interpretation of members, and will
     * separate each typedef with each other.
     * Meaning more type-safety when calling
     * */                                                               \
    DEF_STRUCT_TYPE_SPECIFIC_CALLBACKS(type, typename);                 \
    typedef struct typename##_Vector {                                  \
        Size                            element_size;                   \
        Size                            length;                         \
        Size                            capacity;                       \
        type*                           data;                           \
        Create##typename##CopyCallback  create_copy;                    \
        Destroy##typename##CopyCallback destroy_copy;                   \
        Float32                         resize_factor;                  \
    } typename##_Vector;                                                \
                                                                        \
    /**
     * Now each api wrapper is completely different from other api wrappers.
     * */                                                               \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_create() {      \
        return (typename##_Vector*)vector_create(sizeof(type),          \
                                                 (CreateElementCopyCallback)(copy_create), \
                                                 (DestroyElementCopyCallback)(copy_destroy)); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_destroy(typename##_Vector* vec, void* udata) { \
        vector_destroy((Vector*)vec, udata);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_clone(typename##_Vector* vec, void* udata) { \
        return (typename##_Vector*)vector_clone((Vector*)vec, udata);   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_resize(typename##_Vector* vec, Size sz) { \
        vector_resize((Vector*)vec, sz);                                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_reserve(typename##_Vector* vec, Size sz) { \
        vector_reserve((Vector*)vec, sz);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_clear(typename##_Vector* vec, void* udata) { \
        vector_clear((Vector*)vec, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_get_subvector(typename##_Vector* vec, Size start, Size size, void* udata) { \
        return (typename##_Vector*)vector_get_subvector((Vector*)vec, start, size, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_copy(typename##_Vector* vec, Size to, Size from, void* udata) { \
        vector_copy((Vector*)vec, to, from, udata);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_move(typename##_Vector* vec, Size to, Size from, void* udata) { \
        vector_move((Vector*)vec, to, from, udata);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_overwrite(typename##_Vector* vec, Size to, type* data, void* udata) { \
        vector_overwrite((Vector*)vec, to, (void*)data, udata);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert(typename##_Vector* vec, type* data, Size pos, void* udata) { \
        vector_insert((Vector*)vec, (void*)data, pos, udata);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete(typename##_Vector* vec, Size pos, void* udata) { \
        vector_delete((Vector*)vec, pos, udata);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_remove(typename##_Vector* vec, Size pos) { \
        return (type*)(Uint64)vector_remove((Vector*)vec, pos);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert_fast(typename##_Vector* vec, type* data, Size pos, void* udata) { \
        vector_insert_fast((Vector*)vec, (void*)data, pos, udata);      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete_fast(typename##_Vector* vec, Size pos, void* udata) { \
        vector_delete_fast((Vector*)vec, pos, udata);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_remove_fast(typename##_Vector* vec, Size pos) { \
        return (type*)(Uint64)vector_remove_fast((Vector*)vec, pos);    \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front(typename##_Vector* vec, type* data, void* udata) { \
        vector_push_front((Vector*)vec, (void*)data, udata);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_pop_front(typename##_Vector* vec) { \
        return (type*)(Uint64)vector_pop_front((Vector*)vec);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front_fast(typename##_Vector* vec, type* data, void* udata) { \
        vector_push_front_fast((Vector*)vec, (void*)data, udata);       \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_pop_front_fast(typename##_Vector* vec) { \
        return (type*)(Uint64)vector_pop_front_fast((Vector*)vec);      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_back(typename##_Vector* vec, type* data, void* udata) { \
        vector_push_back((Vector*)vec, (void*)data, udata);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_pop_back(typename##_Vector* vec) { \
        return (type*)(Uint64)vector_pop_back((Vector*)vec);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_data(typename##_Vector* vec) { \
        return vec ? vec->data : NULL;                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_peek(typename##_Vector* vec, Size pos) { \
        return vec ? vec->data + pos : NULL;                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_front(typename##_Vector* vec) { \
        return (vec) ? vec->data : NULL;                                \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_back(typename##_Vector* vec) { \
        return vec ? vec->data + (vec->length - 1) : NULL; \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_print(typename##_Vector* vec, Print##typename##Callback printer, void* udata) { \
        vector_print((Vector*)vec, (PrintElementCallback)(void*)printer, udata);        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_merge(typename##_Vector* vec, typename##_Vector* vec_other, void* udata) { \
        vector_merge((Vector*)vec, (Vector*)vec_other, udata);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE typename##_Vector* api_prefix##_vector_filter(typename##_Vector* vec, Filter##typename##Callback filter, void* udata) { \
        return (typename##_Vector*)vector_filter((Vector*)vec, (FilterElementCallback)(void*)filter, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_swap(typename##_Vector* vec, Size p1, Size p2) { \
        vector_swap((Vector*)vec, p1, p2);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_sort(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        vector_sort((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool api_prefix##_vector_check_sorted(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        return vector_check_sorted((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insertion_sort(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        vector_insertion_sort((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_bubble_sort(typename##_Vector* vec, Compare##typename##Callback compare, void* udata) { \
        vector_bubble_sort((Vector*)vec, (CompareElementCallback)(void*)compare, udata); \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_address_at(typename##_Vector* vec, Size idx) { \
        return (type*)vector_address_at((Vector*)vec, idx);             \
    }



#endif // UTILS_VECTOR_INTERFACE_H
