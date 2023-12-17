/**
 * @file DenseMap.h
 * @date Mon, 17th December, 2023
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
 * @brief DenseMap Defines generic interface for interacting
 * with dense map api
 * */

#ifndef ANVIE_UTILS_CONTAINERS_DENSE_MAP_INTERFACE_H
#define ANVIE_UTILS_CONTAINERS_DENSE_MAP_INTERFACE_H

#include <Anvie/Containers/DenseMap.h>

#define DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE 0.875f

/**
 * Define a new @c DenseMap interface for integer key and integer value.
 * @param api_prefix What prefix to add before each @c DenseMap api call?
 * @param type_prefix What prefix to add before @c DenseMap type?
 * This is new typedef for @c DenseMap only. eg: for StrStr -> StrStrDenseMap.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param kcompare Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * */
#define DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(api_prefix, type_prefix, hash, ktype, kcompare, dtype) \
    DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, NULL, NULL, kcompare, dtype, NULL, NULL, True, DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

/**
 * Define an new @c DenseMap interface for struct key and integer data.
 * @param api_prefix What prefix to add before each @c DenseMap api call?
 * @param type_prefix What prefix to add before @c DenseMap type?
 * This is new typedef for @c DenseMap only.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param kcreate Copy constructor callback for key.
 * @param kdestroy Copy destructor callback for key.
 * @param kcompare Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * */
#define DEF_STRUCT_INTEGER_DENSE_MAP_INTERFACE(api_prefix, type_prefix, hash, ktype, kcreate, kdestroy, kcompare, dtype) \
    DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, kcreate, kdestroy, kcompare, dtype, NULL, NULL, True, DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

/**
 * Define an new @c DenseMap interface for integer key and struct data.
 * @param api_prefix What prefix to add before each @c DenseMap api call?
 * @param type_prefix What prefix to add before @c DenseMap type?
 * This is new typedef for @c DenseMap only.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param kcompare Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * @param dcreate Copy constructor callback for data.
 * @param ddestroy Copy destructor callback for data.
 * */
#define DEF_INTEGER_STRUCT_DENSE_MAP_INTERFACE(api_prefix, type_prefix, hash, ktype, kcompare, dtype, dcreate, ddestroy) \
    DEF_INTEGER_STRUCT_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, NULL, NULL, kcompare, dtype, dcreate, ddestroy, True, DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

/**
 * Define an new @c DenseMap interface for struct key and struct data.
 * @param api_prefix What prefix to add before each @c DenseMap api call?
 * @param type_prefix What prefix to add before @c DenseMap type?
 * This is new typedef for @c DenseMap only.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param kcreate Copy constructor callback for key.
 * @param kdestroy Copy destructor callback for key.
 * @param kcompare Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * @param dcreate Copy constructor callback for data.
 * @param ddestroy Copy destructor callback for data.
 * */
#define DEF_STRUCT_STRUCT_DENSE_MAP_INTERFACE(api_prefix, type_prefix, hash, ktype, kcreate, kdestroy, kcompare, dtype, dcreate, ddestroy) \
    DEF_STRUCT_STRUCT_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, kcreate, kdestroy, kcompare, dtype, dcreate, ddestroy, True, DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

#define DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, create_key_copy, destroy_key_copy, compare_key, dtype, create_data_copy, destroy_data_copy, is_multimap, max_load_factor) \
    typedef DenseMap type_prefix##DenseMap;                             \
    typedef struct type_prefix##DenseMapItem {                          \
        ktype key;                                                      \
        dtype data;                                                     \
    } type_prefix##DenseMapItem;                                        \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMap* api_prefix##_dense_map_create() { \
        return dense_map_create((HashCallback)(void*)hash,                     \
                                sizeof(ktype),                          \
                                (CreateElementCopyCallback)(void*)create_key_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_key_copy, \
                                (CompareElementCallback)(void*)compare_key,    \
                                sizeof(dtype),                          \
                                (CreateElementCopyCallback)(void*)create_data_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_data_copy, \
                                is_multimap, max_load_factor);          \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_destroy(type_prefix##DenseMap* map, void* udata) { \
        dense_map_destroy(map, udata);                                  \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_resize(type_prefix##DenseMap* map, Size size, void* udata) { \
        dense_map_resize(map, size, udata);                             \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_insert(type_prefix##DenseMap* map, ktype key, dtype data, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_insert(map, (void*)(Uint64)key, (void*)(Uint64)data, udata);   \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_search(type_prefix##DenseMap* map, ktype key, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_search(map, (void*)(Uint64)key, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_delete(type_prefix##DenseMap* map, ktype key, void* udata) { \
        dense_map_delete(map, (void*)(Uint64)key, udata);                              \
    }

#define DEF_STRUCT_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, create_key_copy, destroy_key_copy, compare_key, dtype, create_data_copy, destroy_data_copy, is_multimap, max_load_factor) \
    typedef DenseMap type_prefix##DenseMap;                             \
    typedef struct type_prefix##DenseMapItem {                          \
        ktype* key;                                                     \
        dtype data;                                                     \
    } type_prefix##DenseMapItem;                                        \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMap* api_prefix##_dense_map_create() { \
        return dense_map_create((HashCallback)(void*)hash,                     \
                                sizeof(ktype),                          \
                                (CreateElementCopyCallback)(void*)create_key_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_key_copy, \
                                (CompareElementCallback)(void*)compare_key,    \
                                sizeof(dtype),                          \
                                (CreateElementCopyCallback)(void*)create_data_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_data_copy, \
                                is_multimap, max_load_factor);          \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_destroy(type_prefix##DenseMap* map, void* udata) { \
        dense_map_destroy(map, udata);                                  \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_resize(type_prefix##DenseMap* map, Size size, void* udata) { \
        dense_map_resize(map, size, udata);                             \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_insert(type_prefix##DenseMap* map, ktype* key, dtype data, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_insert(map, (void*)(Uint64)key, (void*)(Uint64)data, udata);   \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_search(type_prefix##DenseMap* map, ktype* key, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_search(map, (void*)(Uint64)key, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_delete(type_prefix##DenseMap* map, ktype* key, void* udata) { \
        dense_map_delete(map, (void*)(Uint64)key, udata);                              \
    }

#define DEF_INTEGER_STRUCT_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, create_key_copy, destroy_key_copy, compare_key, dtype, create_data_copy, destroy_data_copy, is_multimap, max_load_factor) \
    typedef DenseMap type_prefix##DenseMap;                             \
    typedef struct type_prefix##DenseMapItem {                          \
        ktype key;                                                      \
        dtype* data;                                                    \
    } type_prefix##DenseMapItem;                                        \
                                                                    \
    static FORCE_INLINE type_prefix##DenseMap* api_prefix##_dense_map_create() { \
        return dense_map_create((HashCallback)(void*)hash,                     \
                                sizeof(ktype),                          \
                                (CreateElementCopyCallback)(void*)create_key_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_key_copy, \
                                (CompareElementCallback)(void*)compare_key,    \
                                sizeof(dtype),                          \
                                (CreateElementCopyCallback)(void*)create_data_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_data_copy, \
                                is_multimap, max_load_factor);          \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_destroy(type_prefix##DenseMap* map, void* udata) { \
        dense_map_destroy(map, udata);                                  \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_resize(type_prefix##DenseMap* map, Size size, void* udata) { \
        dense_map_resize(map, size, udata);                             \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_insert(type_prefix##DenseMap* map, ktype key, dtype* data, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_insert(map, (void*)(Uint64)key, (void*)(Uint64)data, udata);   \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_search(type_prefix##DenseMap* map, ktype key, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_search(map, (void*)(Uint64)key, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_delete(type_prefix##DenseMap* map, ktype key, void* udata) { \
        dense_map_delete(map, (void*)(Uint64)key, udata);                              \
    }

#define DEF_STRUCT_STRUCT_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(api_prefix, type_prefix, hash, ktype, create_key_copy, destroy_key_copy, compare_key, dtype, create_data_copy, destroy_data_copy, is_multimap, max_load_factor) \
    typedef DenseMap type_prefix##DenseMap;                             \
    typedef struct type_prefix##DenseMapItem {                          \
        ktype* key;                                                     \
        dtype* data;                                                    \
    } type_prefix##DenseMapItem;                                        \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMap* api_prefix##_dense_map_create() { \
        return dense_map_create((HashCallback)(void*)hash,                     \
                                sizeof(ktype),                          \
                                (CreateElementCopyCallback)(void*)create_key_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_key_copy, \
                                (CompareElementCallback)(void*)compare_key,    \
                                sizeof(dtype),                          \
                                (CreateElementCopyCallback)(void*)create_data_copy, \
                                (DestroyElementCopyCallback)(void*)destroy_data_copy, \
                                is_multimap, max_load_factor);          \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_destroy(type_prefix##DenseMap* map, void* udata) { \
        dense_map_destroy(map, udata);                                  \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_resize(type_prefix##DenseMap* map, Size size, void* udata) { \
        dense_map_resize(map, size, udata);                             \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_insert(type_prefix##DenseMap* map, ktype* key, dtype* data, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_insert(map, (void*)(Uint64)key, (void*)(Uint64)data, udata);   \
    }                                                                   \
                                                                        \
    static FORCE_INLINE type_prefix##DenseMapItem* api_prefix##_dense_map_search(type_prefix##DenseMap* map, ktype* key, void* udata) { \
        return (type_prefix##DenseMapItem*)dense_map_search(map, (void*)(Uint64)key, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void api_prefix##_dense_map_delete(type_prefix##DenseMap* map, ktype* key, void* udata) { \
        dense_map_delete(map, (void*)(Uint64)key, udata);                              \
    }

#endif // ANVIE_UTILS_CONTAINERS_DENSE_MAP_INTERFACE_H
