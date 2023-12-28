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
 * */

#ifndef UTILS_VECTOR_INTERFACE_H
#define UTILS_VECTOR_INTERFACE_H

#include <Anvie/HelperDefines.h>

#define DEF_INTEGER_VECTOR_INTERFACE(api_prefix, type_prefix, type) DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, type, NULL, NULL)

#define DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, type, copy, destroy) \
    typedef Vector type_prefix##Vector;                                 \
    FORCE_INLINE type_prefix##Vector* api_prefix##_vector_create() {    \
        return vector_create(sizeof(type),                              \
                             (CreateElementCopyCallback)(void*)(copy),  \
                             (DestroyElementCopyCallback)(void*)destroy); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_destroy(type_prefix##Vector* vec, void* udata) { \
        vector_destroy(vec, udata);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type_prefix##Vector* api_prefix##_vector_clone(type_prefix##Vector* vec, void* udata) { \
        return vector_clone(vec, udata);                                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_resize(type_prefix##Vector* vec, Size sz) { \
        vector_resize(vec, sz);                                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_reserve(type_prefix##Vector* vec, Size sz) { \
        vector_reserve(vec, sz);                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_clear(type_prefix##Vector* vec, void* udata) { \
        vector_clear(vec, udata);                                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_get_subvector(type_prefix##Vector* vec, Size start, Size size, void* udata) { \
        vector_get_subvector(vec, start, size, udata);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_copy(type_prefix##Vector* vec, Size to, Size from, void* udata) { \
        vector_copy(vec, to, from, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_move(type_prefix##Vector* vec, Size to, Size from, void* udata) { \
        vector_move(vec, to, from, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_overwrite(type_prefix##Vector* vec, Size to, type value, void* udata) { \
        vector_overwrite(vec, to, (void*)(Uint64)value, udata);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert(type_prefix##Vector* vec, type value, Size pos, void* udata) { \
        vector_insert(vec, (void*)(Uint64)value, pos, udata);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete(type_prefix##Vector* vec, Size pos, void* udata) { \
        vector_delete(vec, pos, udata);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_remove(type_prefix##Vector* vec, Size pos) { \
        return (type)(Uint64)vector_remove(vec, pos);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert_fast(type_prefix##Vector* vec, type value, Size pos, void* udata) { \
        vector_insert_fast(vec, (void*)(Uint64)value, pos, udata);      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete_fast(type_prefix##Vector* vec, Size pos, void* udata) { \
        vector_delete_fast(vec, pos, udata);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_remove_fast(type_prefix##Vector* vec, Size pos) { \
        return (type)(Uint64)vector_remove_fast(vec, pos);              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front(type_prefix##Vector* vec, type value, void* udata) { \
        vector_push_front(vec, (void*)(Uint64)value, udata);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_pop_front(type_prefix##Vector* vec) { \
        return (type)(Uint64)vector_pop_front(vec);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front_fast(type_prefix##Vector* vec, type value, void* udata) { \
        vector_push_front_fast(vec, (void*)(Uint64)value, udata);       \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_pop_front_fast(type_prefix##Vector* vec) { \
        return (type)(Uint64)vector_pop_front_fast(vec);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_back(type_prefix##Vector* vec, type value, void* udata) { \
        vector_push_back(vec, (void*)(Uint64)value, udata);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_pop_back(type_prefix##Vector* vec) { \
        return (type)(Uint64)vector_pop_back(vec);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_peek(type_prefix##Vector* vec, Size pos) { \
        return (type)(Uint64)vector_peek(vec, pos);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_front(type_prefix##Vector* vec) { \
        return (type)(Uint64)vector_front(vec);                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE type api_prefix##_vector_back(type_prefix##Vector* vec) { \
        return (type)(Uint64)vector_back(vec);                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_print(type_prefix##Vector* vec, PrintElementCallback printer, void* udata) { \
        vector_print(vec, printer, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_merge(type_prefix##Vector* vec, type_prefix##Vector* vec_other, void* udata) { \
        vector_merge(vec, vec_other, udata);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type_prefix##Vector* api_prefix##_vector_filter(type_prefix##Vector* vec, FilterElementCallback filter, void* udata) { \
        return vector_filter(vec, filter, udata);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_swap(type_prefix##Vector* vec, Size p1, Size p2) { \
        vector_swap(vec, p1, p2);                                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_sort(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        vector_sort(vec, compare, udata);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool api_prefix##_vector_check_sorted(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        return vector_check_sorted(vec, compare, udata);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insertion_sort(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        vector_insertion_sort(vec, compare, udata);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_bubble_sort(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        vector_bubble_sort(vec, compare, udata);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_merge_sort(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        vector_merge_sort(vec, compare, udata);                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_data(type_prefix##Vector* vec) { \
        return (type*)vec->data;                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_address_at(type_prefix##Vector* vec, Size idx) { \
        return (type*)vector_address_at(vec, idx);                      \
    }

#define DEF_STRUCT_VECTOR_INTERFACE(api_prefix, type_prefix, type, copy_create, copy_destroy) \
    typedef Vector type_prefix##Vector;                                 \
                                                                        \
    FORCE_INLINE type_prefix##Vector* api_prefix##_vector_create() {    \
        return vector_create(sizeof(type), (CreateElementCopyCallback)(copy_create), (DestroyElementCopyCallback)(copy_destroy)); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_destroy(type_prefix##Vector* vec, void* udata) { \
        vector_destroy(vec, udata);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type_prefix##Vector* api_prefix##_vector_clone(type_prefix##Vector* vec, void* udata) { \
        return vector_clone(vec, udata);                                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_resize(type_prefix##Vector* vec, Size sz) { \
        vector_resize(vec, sz);                                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_reserve(type_prefix##Vector* vec, Size sz) { \
        vector_reserve(vec, sz);                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_clear(type_prefix##Vector* vec, void* udata) { \
        vector_clear(vec, udata);                                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_get_subvector(type_prefix##Vector* vec, Size start, Size size, void* udata) { \
        vector_get_subvector(vec, start, size, udata);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_copy(type_prefix##Vector* vec, Size to, Size from, void* udata) { \
        vector_copy(vec, to, from, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_move(type_prefix##Vector* vec, Size to, Size from, void* udata) { \
        vector_move(vec, to, from, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_overwrite(type_prefix##Vector* vec, Size to, type* data, void* udata) { \
        vector_overwrite(vec, to, (void*)data, udata);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert(type_prefix##Vector* vec, type* data, Size pos, void* udata) { \
        vector_insert(vec, (void*)data, pos, udata);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete(type_prefix##Vector* vec, Size pos, void* udata) { \
        vector_delete(vec, pos, udata);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_remove(type_prefix##Vector* vec, Size pos) { \
        return (type*)(Uint64)vector_remove(vec, pos);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insert_fast(type_prefix##Vector* vec, type* data, Size pos, void* udata) { \
        vector_insert_fast(vec, (void*)data, pos, udata);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_delete_fast(type_prefix##Vector* vec, Size pos, void* udata) { \
        vector_delete_fast(vec, pos, udata);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_remove_fast(type_prefix##Vector* vec, Size pos) { \
        return (type*)(Uint64)vector_remove_fast(vec, pos);             \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front(type_prefix##Vector* vec, type* data, void* udata) { \
        vector_push_front(vec, (void*)data, udata);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_pop_front(type_prefix##Vector* vec) { \
        return (type*)(Uint64)vector_pop_front(vec);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_front_fast(type_prefix##Vector* vec, type* data, void* udata) { \
        vector_push_front_fast(vec, (void*)data, udata);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_pop_front_fast(type_prefix##Vector* vec) { \
        return (type*)(Uint64)vector_pop_front_fast(vec);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_push_back(type_prefix##Vector* vec, type* data, void* udata) { \
        vector_push_back(vec, (void*)data, udata);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_pop_back(type_prefix##Vector* vec) { \
        return (type*)(Uint64)vector_pop_back(vec);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_data(type_prefix##Vector* vec) { \
        return (type*)vec->data;                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_peek(type_prefix##Vector* vec, Size pos) { \
        return (type*)api_prefix##_vector_data(vec) + pos;              \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_front(type_prefix##Vector* vec) { \
        return (type*)api_prefix##_vector_data(vec);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_back(type_prefix##Vector* vec) { \
        return (type*)api_prefix##_vector_data(vec) + (vec->length - 1); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_print(type_prefix##Vector* vec, PrintElementCallback printer, void* udata) { \
        vector_print(vec, printer, udata);                              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_merge(type_prefix##Vector* vec, type_prefix##Vector* vec_other, void* udata) { \
        vector_merge(vec, vec_other, udata);                            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type_prefix##Vector* api_prefix##_vector_filter(type_prefix##Vector* vec, FilterElementCallback filter, void* udata) { \
        return vector_filter(vec, filter, udata);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_swap(type_prefix##Vector* vec, Size p1, Size p2) { \
        vector_swap(vec, p1, p2);                                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_sort(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        vector_sort(vec, compare, udata);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool api_prefix##_vector_check_sorted(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        return vector_check_sorted(vec, compare, udata);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_insertion_sort(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        vector_insertion_sort(vec, compare, udata);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void api_prefix##_vector_bubble_sort(type_prefix##Vector* vec, CompareElementCallback compare, void* udata) { \
        vector_bubble_sort(vec, compare, udata);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* api_prefix##_vector_address_at(type_prefix##Vector* vec, Size idx) { \
        return (type*)vector_address_at(vec, idx);                      \
    }



#endif // UTILS_VECTOR_INTERFACE_H
