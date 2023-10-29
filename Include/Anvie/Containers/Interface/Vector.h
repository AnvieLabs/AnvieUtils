/**
 * @file VectorInterface.h
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

#define DEF_INTEGER_VECTOR_INTERFACE(prefix, type) DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(prefix, type, NULL, NULL)

#define DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(prefix, type, copy, destroy) \
    FORCE_INLINE Vector* prefix##_vector_create() {            \
        return vector_create(sizeof(type), copy, destroy);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_destroy(Vector* vec) {   \
        vector_destroy(vec);                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE Vector* prefix##_vector_clone(Vector* vec) { \
        return vector_clone(vec);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_resize(Vector* vec, Size sz) { \
        vector_resize(vec, sz);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_reserve(Vector* vec, Size sz) { \
        vector_reserve(vec, sz);                                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_clear(Vector* vec) {     \
        vector_clear(vec);                                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_get_subvector(Vector* vec, Size start, Size size) { \
        vector_get_subvector(vec, start, size);                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_copy(Vector* vec, Size to, Size from) { \
        vector_copy(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_move(Vector* vec, Size to, Size from) { \
        vector_move(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_overwrite(Vector* vec, Size to, type value) { \
        vector_overwrite(vec, to, (void*)(Uint64)value);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_insert(Vector* vec, type value, Size pos) { \
        vector_insert(vec, (void*)(Uint64)value, pos);              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_delete(Vector* vec, Size pos) { \
        vector_delete(vec, pos);                                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_remove(Vector* vec, Size pos) { \
        return (type)(Uint64)vector_remove(vec, pos);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_insert_fast(Vector* vec, type value, Size pos) { \
        vector_insert_fast(vec, (void*)(Uint64)value, pos);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_delete_fast(Vector* vec, Size pos) { \
        vector_delete_fast(vec, pos);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_remove_fast(Vector* vec, Size pos) { \
        return (type)(Uint64)vector_remove_fast(vec, pos);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_push_front(Vector* vec, type value) { \
        vector_push_front(vec, (void*)(Uint64)value);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_pofront(Vector* vec) { \
        return (type)(Uint64)vector_pofront(vec);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_push_front_fast(Vector* vec, type value) { \
        vector_push_front_fast(vec, (void*)(Uint64)value);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_pofront_fast(Vector* vec) { \
        return (type)(Uint64)vector_pofront_fast(vec);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_push_back(Vector* vec, type value) { \
        vector_push_back(vec, (void*)(Uint64)value);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_poback(Vector* vec) {  \
        return (type)(Uint64)vector_poback(vec);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_peek(Vector* vec, Size pos) { \
        return (type)(Uint64)vector_peek(vec, pos);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_front(Vector* vec) {     \
        return (type)(Uint64)vector_front(vec);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type prefix##_vector_back(Vector* vec) {      \
        return (type)(Uint64)vector_back(vec);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_print(Vector* vec, PrintElementCallback printer) { \
        vector_print(vec, printer);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_merge(Vector* vec, Vector* vec_other) { \
        vector_merge(vec, vec_other);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE Vector* prefix##_vector_filter(Vector* vec, FilterElementCallback filter, void* user_data) { \
        return vector_filter(vec, filter, user_data);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_swap(Vector* vec, Size p1, Size p2) { \
        vector_swap(vec, p1, p2);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_sort(Vector* vec, CompareElementCallback compare) { \
        vector_sort(vec, compare);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool prefix##_vector_check_sorted(Vector* vec, CompareElementCallback compare) { \
        return vector_check_sorted(vec, compare);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_insertion_sort(Vector* vec, CompareElementCallback compare) { \
        vector_insertion_sort(vec, compare);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_bubble_sort(Vector* vec, CompareElementCallback compare) { \
        vector_bubble_sort(vec, compare);                           \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_data(Vector* vec) {     \
        return (type*)vec->data;                                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_address_at(Vector* vec, Size idx) { \
        return (type*)vector_address_at(vec, idx);                  \
    }

#define DEF_STRUCT_VECTOR_INTERFACE(prefix, type, copy_create, copy_destroy) \
    typedef void(*prefix##CreateCopyCallback)(type* dst, type* src); \
    typedef void(*prefix##DestroyCopyCallback)(type* copy);        \
                                                                        \
    FORCE_INLINE Vector* prefix##_vector_create() {            \
        return vector_create(sizeof(type), (CreateElementCopyCallback)(copy_create), (DestroyElementCopyCallback)(copy_destroy)); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_destroy(Vector* vec) {   \
        vector_destroy(vec);                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE Vector* prefix##_vector_clone(Vector* vec) { \
        return vector_clone(vec);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_resize(Vector* vec, Size sz) { \
        vector_resize(vec, sz);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_reserve(Vector* vec, Size sz) { \
        vector_reserve(vec, sz);                                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_clear(Vector* vec) {     \
        vector_clear(vec);                                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_get_subvector(Vector* vec, Size start, Size size) { \
        vector_get_subvector(vec, start, size);                         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_copy(Vector* vec, Size to, Size from) { \
        vector_copy(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_move(Vector* vec, Size to, Size from) { \
        vector_move(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_overwrite(Vector* vec, Size to, type* data) { \
        vector_overwrite(vec, to, (void*)data);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_insert(Vector* vec, type* data, Size pos) { \
        vector_insert(vec, (void*)data, pos);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_delete(Vector* vec, Size pos) { \
        vector_delete(vec, pos);                                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_remove(Vector* vec, Size pos) { \
        return (type*)(Uint64)vector_remove(vec, pos);              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_insert_fast(Vector* vec, type* data, Size pos) { \
        vector_insert_fast(vec, (void*)data, pos);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_delete_fast(Vector* vec, Size pos) { \
        vector_delete_fast(vec, pos);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_remove_fast(Vector* vec, Size pos) { \
        return (type*)(Uint64)vector_remove_fast(vec, pos);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_push_front(Vector* vec, type* data) { \
        vector_push_front(vec, (void*)data);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_pofront(Vector* vec) { \
        return (type*)(Uint64)vector_pofront(vec);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_push_front_fast(Vector* vec, type* data) { \
        vector_push_front_fast(vec, (void*)data);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_pofront_fast(Vector* vec) { \
        return (type*)(Uint64)vector_pofront_fast(vec);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_push_back(Vector* vec, type* data) { \
        vector_push_back(vec, (void*)data);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_poback(Vector* vec) { \
        return (type*)(Uint64)vector_poback(vec);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_data(Vector* vec) {     \
        return (type*)vec->data;                                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_peek(Vector* vec, Size pos) { \
        return (type*)prefix##_vector_data(vec) + pos;            \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_front(Vector* vec) {    \
        return (type*)prefix##_vector_data(vec);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_back(Vector* vec) {     \
        return (type*)prefix##_vector_data(vec) + (vec->length - 1); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_print(Vector* vec, PrintElementCallback printer) { \
        vector_print(vec, printer);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_merge(Vector* vec, Vector* vec_other) { \
        vector_merge(vec, vec_other);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE Vector* prefix##_vector_filter(Vector* vec, FilterElementCallback filter, void* user_data) { \
        return vector_filter(vec, filter, user_data);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_swap(Vector* vec, Size p1, Size p2) { \
        vector_swap(vec, p1, p2);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_sort(Vector* vec, CompareElementCallback compare) { \
        vector_sort(vec, compare);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool prefix##_vector_check_sorted(Vector* vec, CompareElementCallback compare) { \
        return vector_check_sorted(vec, compare);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_insertion_sort(Vector* vec, CompareElementCallback compare) { \
        vector_insertion_sort(vec, compare);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void prefix##_vector_bubble_sort(Vector* vec, CompareElementCallback compare) { \
        vector_bubble_sort(vec, compare);                           \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* prefix##_vector_address_at(Vector* vec, Size idx) { \
        return (type*)vector_address_at(vec, idx);                  \
    }



#endif // UTILS_VECTOR_INTERFACE_H
