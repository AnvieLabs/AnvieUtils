/**
 * @file VectorInterface.h
 * @date Mon, 14th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Vector Interface Creator.
 * */


#ifndef ANV_UTILS_VECTOR_INTERFACE_H
#define ANV_UTILS_VECTOR_INTERFACE_H

#include <Anvie/HelperDefines.h>

#define DEF_ANV_INTEGER_VECTOR_INTERFACE(prefix, type)                \
    FORCE_INLINE AnvVector* anv_##prefix##_vector_create() {          \
        return anv_vector_create(sizeof(type), NULL, NULL);           \
    }                                                                 \
                                                                      \
    FORCE_INLINE void anv_##prefix##_vector_destroy(AnvVector* vec) {   \
        anv_vector_destroy(vec);                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_resize(AnvVector* vec, Size sz) { \
        anv_vector_resize(vec, sz);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_clear(AnvVector* vec) {     \
        anv_vector_clear(vec);                                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_copy(AnvVector* vec, Size to, Size from) { \
        anv_vector_copy(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_move(AnvVector* vec, Size to, Size from) { \
        anv_vector_move(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_overwrite(AnvVector* vec, Size to, type value) { \
        anv_vector_overwrite(vec, to, (void*)(Uint64)value);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_insert(AnvVector* vec, type value, Size pos) { \
        anv_vector_insert(vec, (void*)(Uint64)value, pos);              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_delete(AnvVector* vec, Size pos) { \
        anv_vector_delete(vec, pos);                                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_remove(AnvVector* vec, Size pos) { \
        return (type)(Uint64)anv_vector_remove(vec, pos);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_insert_fast(AnvVector* vec, type value, Size pos) { \
        anv_vector_insert_fast(vec, (void*)(Uint64)value, pos);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_delete_fast(AnvVector* vec, Size pos) { \
        anv_vector_delete_fast(vec, pos);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_remove_fast(AnvVector* vec, Size pos) { \
        return (type)(Uint64)anv_vector_remove_fast(vec, pos);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_push_front(AnvVector* vec, type value) { \
        anv_vector_push_front(vec, (void*)(Uint64)value);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_pop_front(AnvVector* vec) { \
        return (type)(Uint64)anv_vector_pop_front(vec);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_push_front_fast(AnvVector* vec, type value) { \
        anv_vector_push_front_fast(vec, (void*)(Uint64)value);          \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_pop_front_fast(AnvVector* vec) { \
        return (type)(Uint64)anv_vector_pop_front_fast(vec);            \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_push_back(AnvVector* vec, type value) { \
        anv_vector_push_back(vec, (void*)(Uint64)value);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_pop_back(AnvVector* vec) {  \
        return (type)(Uint64)anv_vector_pop_back(vec);                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_peek(AnvVector* vec, Size pos) { \
        return (type)(Uint64)anv_vector_peek(vec, pos);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_front(AnvVector* vec) {     \
        return (type)(Uint64)anv_vector_front(vec);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE type anv_##prefix##_vector_back(AnvVector* vec) {      \
        return (type)(Uint64)anv_vector_back(vec);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_print(AnvVector* vec, AnvPrintElementCallback printer) { \
        anv_vector_print(vec, printer);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_merge(AnvVector* vec, AnvVector* vec_other) { \
        anv_vector_merge(vec, vec_other);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvVector* anv_##prefix##_vector_filter(AnvVector* vec, AnvFilterElementCallback filter, void* user_data) { \
        return anv_vector_filter(vec, filter, user_data);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_swap(AnvVector* vec, Size p1, Size p2) { \
        anv_vector_swap(vec, p1, p2);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_sort(AnvVector* vec, AnvCompareElementCallback compare) { \
        anv_vector_sort(vec, compare);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool anv_##prefix##_vector_check_sorted(AnvVector* vec, AnvCompareElementCallback compare) { \
        return anv_vector_check_sorted(vec, compare);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_insertion_sort(AnvVector* vec, AnvCompareElementCallback compare) { \
        anv_vector_insertion_sort(vec, compare);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_bubble_sort(AnvVector* vec, AnvCompareElementCallback compare) { \
        anv_vector_bubble_sort(vec, compare);                           \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_data(AnvVector* vec) {     \
        return (type*)vec->p_data;                                      \
    }

#define DEF_ANV_STRUCT_VECTOR_INTERFACE(prefix, type, copy_create, copy_destroy) \
    typedef void(*Anv##prefix##CreateCopyCallback)(type* dst, type* src); \
    typedef void(*Anv##prefix##DestroyCopyCallback)(type* copy);        \
                                                                        \
    FORCE_INLINE AnvVector* anv_##prefix##_vector_create() {            \
        return anv_vector_create(sizeof(type), (AnvCreateElementCopyCallback)(copy_create), (AnvDestroyElementCopyCallback)(copy_destroy)); \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_destroy(AnvVector* vec) {   \
        anv_vector_destroy(vec);                                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_resize(AnvVector* vec, Size sz) { \
        anv_vector_resize(vec, sz);                                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_clear(AnvVector* vec) {     \
        anv_vector_clear(vec);                                          \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_copy(AnvVector* vec, Size to, Size from) { \
        anv_vector_copy(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_move(AnvVector* vec, Size to, Size from) { \
        anv_vector_move(vec, to, from);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_overwrite(AnvVector* vec, Size to, type* p_data) { \
        anv_vector_overwrite(vec, to, (void*)p_data);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_insert(AnvVector* vec, type* p_data, Size pos) { \
        anv_vector_insert(vec, (void*)p_data, pos);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_delete(AnvVector* vec, Size pos) { \
        anv_vector_delete(vec, pos);                                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_remove(AnvVector* vec, Size pos) { \
        return (type*)(Uint64)anv_vector_remove(vec, pos);              \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_insert_fast(AnvVector* vec, type* p_data, Size pos) { \
        anv_vector_insert_fast(vec, (void*)p_data, pos);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_delete_fast(AnvVector* vec, Size pos) { \
        anv_vector_delete_fast(vec, pos);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_remove_fast(AnvVector* vec, Size pos) { \
        return (type*)(Uint64)anv_vector_remove_fast(vec, pos);         \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_push_front(AnvVector* vec, type* p_data) { \
        anv_vector_push_front(vec, (void*)p_data);                      \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_pop_front(AnvVector* vec) { \
        return (type*)(Uint64)anv_vector_pop_front(vec);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_push_front_fast(AnvVector* vec, type* p_data) { \
        anv_vector_push_front_fast(vec, (void*)p_data);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_pop_front_fast(AnvVector* vec) { \
        return (type*)(Uint64)anv_vector_pop_front_fast(vec);           \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_push_back(AnvVector* vec, type* p_data) { \
        anv_vector_push_back(vec, (void*)p_data);                       \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_pop_back(AnvVector* vec) { \
        return (type*)(Uint64)anv_vector_pop_back(vec);                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_peek(AnvVector* vec, Size pos) { \
        return (type*)(Uint64)anv_vector_peek(vec, pos);                \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_front(AnvVector* vec) {    \
        return (type*)(Uint64)anv_vector_front(vec);                    \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_back(AnvVector* vec) {     \
        return (type*)(Uint64)anv_vector_back(vec);                     \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_print(AnvVector* vec, AnvPrintElementCallback printer) { \
        anv_vector_print(vec, printer);                                 \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_merge(AnvVector* vec, AnvVector* vec_other) { \
        anv_vector_merge(vec, vec_other);                               \
    }                                                                   \
                                                                        \
    FORCE_INLINE AnvVector* anv_##prefix##_vector_filter(AnvVector* vec, AnvFilterElementCallback filter, void* user_data) { \
        return anv_vector_filter(vec, filter, user_data);               \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_swap(AnvVector* vec, Size p1, Size p2) { \
        anv_vector_swap(vec, p1, p2);                                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_sort(AnvVector* vec, AnvCompareElementCallback compare) { \
        anv_vector_sort(vec, compare);                                  \
    }                                                                   \
                                                                        \
    FORCE_INLINE Bool anv_##prefix##_vector_check_sorted(AnvVector* vec, AnvCompareElementCallback compare) { \
        return anv_vector_check_sorted(vec, compare);                   \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_insertion_sort(AnvVector* vec, AnvCompareElementCallback compare) { \
        anv_vector_insertion_sort(vec, compare);                        \
    }                                                                   \
                                                                        \
    FORCE_INLINE void anv_##prefix##_vector_bubble_sort(AnvVector* vec, AnvCompareElementCallback compare) { \
        anv_vector_bubble_sort(vec, compare);                           \
    }                                                                   \
                                                                        \
    FORCE_INLINE type* anv_##prefix##_vector_data(AnvVector* vec) {     \
        return (type*)vec->p_data;                                      \
    }

#endif // ANV_UTILS_VECTOR_INTERFACE_H
