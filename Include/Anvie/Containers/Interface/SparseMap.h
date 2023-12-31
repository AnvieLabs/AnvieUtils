/**
 * @file SparseMap.h
 * @date Mon, 17th December, 2023
 * @author Sid_cpy_dtrharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright 2023 Sid_cpy_dtrharth Mishra
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
 * @brief SparseMap Defines generic interface for interacting
 * with dense map api
 * */

#ifndef ANVIE_UTILS_CONTAINERS_SPARSE_MAP_INTERFACE_H
#define ANVIE_UTILS_CONTAINERS_SPARSE_MAP_INTERFACE_H

#define SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE 0.75f

/**
 * Define a new @c SparseMap interface for integer key and integer value.
 * @param pfx What prefix to ad_cpy_dtr before each @c SparseMap api call?
 * @param ktname Typename to use for key
 * @param dtname Typename to use for data
 * This is new typedef for @c SparseMap only. eg: for StrStr -> StrStrSparseMap.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param k_cmp Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * */
#define DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(pfx, ktname, dtname, hash, ktype, k_cmp, dtype) \
    DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, ktname, dtname, hash, ktype, NULL, NULL, k_cmp, dtype, NULL, NULL, True, SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

/**
 * Define an new @c SparseMap interface for struct key and integer data.
 * @param pfx What prefix to ad_cpy_dtr before each @c SparseMap api call?
 * @param type_prefix What prefix to ad_cpy_dtr before @c SparseMap type?
 * This is new typedef for @c SparseMap only.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param k_cpy_ctr Copy constructor callback for key.
 * @param k_cpy_dtr Copy destructor callback for key.
 * @param k_cmp Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * */
#define DEF_STRUCT_INTEGER_SPARSE_MAP_INTERFACE(pfx, ktname, dtname, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype) \
    DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, ktname, dtname, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype, NULL, NULL, True, SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

/**
 * Define an new @c SparseMap interface for integer key and struct data.
 * @param pfx What prefix to ad_cpy_dtr before each @c SparseMap api call?
 * @param type_prefix What prefix to ad_cpy_dtr before @c SparseMap type?
 * This is new typedef for @c SparseMap only.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param k_cmp Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * @param d_cpy_ctr Copy constructor callback for data.
 * @param d_cpy_dtr Copy destructor callback for data.
 * */
#define DEF_INTEGER_STRUCT_SPARSE_MAP_INTERFACE(pfx, ktname, dtname, hash, ktype, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr) \
    DEF_INTEGER_STRUCT_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, ktname, dtname, hash, ktype, NULL, NULL, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr, True, SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

/**
 * Define an new @c SparseMap interface for struct key and struct data.
 * @param pfx What prefix to ad_cpy_dtr before each @c SparseMap api call?
 * @param type_prefix What prefix to ad_cpy_dtr before @c SparseMap type?
 * This is new typedef for @c SparseMap only.
 * @param hash Hash function. Cannot be NULL.
 * @param ktype Type of key.
 * @param k_cpy_ctr Copy constructor callback for key.
 * @param k_cpy_dtr Copy destructor callback for key.
 * @param k_cmp Key compare callback. Cannot be NULL.
 * @param dtype Type of data.
 * @param d_cpy_ctr Copy constructor callback for data.
 * @param d_cpy_dtr Copy destructor callback for data.
 * */
#define DEF_STRUCT_STRUCT_SPARSE_MAP_INTERFACE(pfx, ktname, dtname, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr) \
    DEF_STRUCT_STRUCT_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, ktname, dtname, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr, True, SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE)

#define DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, ktname, dtname, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr, is_mm, max_lf) \
    /* define type specific callback types */                           \
    DEF_INTEGER_TYPE_SPECIFIC_CALLBACKS(ktype, ktname);                 \
                                                                        \
    /* forward declaration so that we can use it easily below */        \
    typedef struct ktname##_##dtname##_SparseMap ktname##_##dtname##_SparseMap; \
    typedef struct ktname##_##dtname##_SparseMapItem ktname##_##dtname##_SparseMapItem; \
                                                                        \
    /**
     * The aligned attribute makes sure that even if ktype and dtype are of
     * size less than 8 bytes, they'll still behave as 8 byte values.
     * This is necessary for it's compatibility with generic API.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMapItem {                  \
        ktype key __attribute__((aligned(8)));                          \
        dtype data __attribute__((aligned(8)));                         \
        ktname##_##dtname##_SparseMapItem* next __attribute__((aligned(8))); \
    } ktname##_##dtname##_SparseMapItem;                                \
    DEF_STRUCT_VECTOR_INTERFACE(pfx##_smi, ktname##_##dtname##_Smi, ktname##_##dtname##_SparseMapItem, create_smi_copy, destroy_smi_copy); \
                                                                        \
    /**
     * Create type specific definition of SparseMap.
     * This definition will make the SparseMap different from other
     * type specific SparseMap definitions, separating the different
     * wrapper APIs from each other automatically.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMap {                      \
        Hash##ktname##Callback        hash;                             \
        Create##ktname##CopyCallback  create_data_copy;                 \
        Destroy##ktname##CopyCallback destroy_data_copy;                \
        Size                          data_size;                        \
        Create##ktname##CopyCallback  create_key_copy;                  \
        Destroy##ktname##CopyCallback destroy_key_copy;                 \
        Compare##ktname##Callback     compare_key;                      \
        Size                          key_size;                         \
        Bool                          is_multimap;                      \
        Size                          max_item_count;                   \
        Size                          item_count;                       \
        BitVector*                    occupancy;                        \
        ktname##_##dtname##_Smi_Vector*   map;                          \
    } ktname##_##dtname##_SparseMap;                                            \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMap* pfx##_sparse_map_create() { \
        return (ktname##_##dtname##_SparseMap*)sparse_map_create((HashCallback)(void*)hash, \
                                                                 sizeof(ktype), \
                                                                 (CreateElementCopyCallback)(void*)k_cpy_ctr, \
                                                                 (DestroyElementCopyCallback)(void*)k_cpy_dtr, \
                                                                 (CompareElementCallback)(void*)k_cmp, \
                                                                 sizeof(dtype), \
                                                                 (CreateElementCopyCallback)(void*)d_cpy_ctr, \
                                                                 (DestroyElementCopyCallback)(void*)d_cpy_dtr, \
                                                                 is_mm, max_lf); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_destroy(ktname##_##dtname##_SparseMap* map, void* udata) { \
        sparse_map_destroy((SparseMap*)map, udata);                     \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_resize(ktname##_##dtname##_SparseMap* map, Size size, void* udata) { \
        sparse_map_resize((SparseMap*)map, size, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_insert(ktname##_##dtname##_SparseMap* map, ktype key, dtype data, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_insert((SparseMap*)map, (void*)(Uint64)key, (void*)(Uint64)data, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_search(ktname##_##dtname##_SparseMap* map, ktype key, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_search((SparseMap*)map, (void*)(Uint64)key, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_delete(ktname##_##dtname##_SparseMap* map, ktype key, void* udata) { \
        sparse_map_delete((SparseMap*)map, (void*)(Uint64)key, udata);  \
    }

#define DEF_STRUCT_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, type_prefix, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr, is_mm, max_lf) \
    /* define type specific callback types */                           \
    DEF_STRUCT_TYPE_SPECIFIC_CALLBACKS(ktype, ktname);                  \
                                                                        \
    /* forward declaration so that we can use it easily below */        \
    typedef struct ktname##_##dtname##_SparseMap ktname##_##dtname##_SparseMap; \
    typedef struct ktname##_##dtname##_SparseMapItem ktname##_##dtname##_SparseMapItem; \
                                                                        \
    /**
     * The aligned attribute makes sure that even if ktype and dtype are of
     * size less than 8 bytes, they'll still behave as 8 byte values.
     * This is necessary for it's compatibility with generic API.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMapItem {                  \
        ktype* key __attribute__((aligned(8)));                         \
        dtype  data __attribute__((aligned(8)));                        \
        ktname##_##dtname##_SparseMapItem* next __attribute__((aligned(8))); \
    } ktname##_##dtname##_SparseMapItem;                                \
    DEF_STRUCT_VECTOR_INTERFACE(pfx##_smi, ktname##_##dtname##_Smi, ktname##_##dtname##_SparseMapItem, create_smi_copy, destroy_smi_copy); \
                                                                        \
    /**
     * Create type specific definition of SparseMap.
     * This definition will make the SparseMap different from other
     * type specific SparseMap definitions, separating the different
     * wrapper APIs from each other automatically.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMap {                      \
        Hash##ktname##Callback          hash;                           \
        Create##ktname##CopyCallback    create_data_copy;               \
        Destroy##ktname##CopyCallback   destroy_data_copy;              \
        Size                            data_size;                      \
        Create##ktname##CopyCallback    create_key_copy;                \
        Destroy##ktname##CopyCallback   destroy_key_copy;               \
        Compare##ktname##Callback       compare_key;                    \
        Size                            key_size;                       \
        Bool                            is_multimap;                    \
        Size                            max_item_count;                 \
        Size                            item_count;                     \
        BitVector*                      occupancy;                      \
        ktname##_##dtname##_Smi_Vector* map;                            \
    } ktname##_##dtname##_SparseMap;                                    \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMap* pfx##_sparse_map_create() { \
        return (ktname##_##dtname##_SparseMap*)sparse_map_create((HashCallback)(void*)hash, \
                                                                 sizeof(ktype), \
                                                                 (CreateElementCopyCallback)(void*)k_cpy_ctr, \
                                                                 (DestroyElementCopyCallback)(void*)k_cpy_dtr, \
                                                                 (CompareElementCallback)(void*)k_cmp, \
                                                                 sizeof(dtype), \
                                                                 (CreateElementCopyCallback)(void*)d_cpy_ctr, \
                                                                 (DestroyElementCopyCallback)(void*)d_cpy_dtr, \
                                                                 is_mm, max_lf); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_destroy(ktname##_##dtname##_SparseMap* map, void* udata) { \
        sparse_map_destroy((SparseMap*)map, udata);                     \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_resize(ktname##_##dtname##_SparseMap* map, Size size, void* udata) { \
        sparse_map_resize((SparseMap*)map, size, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_insert(ktname##_##dtname##_SparseMap* map, ktype* key, dtype data, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_insert((SparseMap*)map, (void*)(Uint64)key, (void*)(Uint64)data, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_search(ktname##_##dtname##_SparseMap* map, ktype* key, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_search((SparseMap*)map, (void*)(Uint64)key, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_delete(ktname##_##dtname##_SparseMap* map, ktype* key, void* udata) { \
        sparse_map_delete((SparseMap*)map, (void*)(Uint64)key, udata);  \
    }

#define DEF_INTEGER_STRUCT_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, type_prefix, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr, is_mm, max_lf) \
    /* define type specific callback types */                           \
    DEF_INTEGER_TYPE_SPECIFIC_CALLBACKS(ktype, ktname);                 \
                                                                        \
    /* forward declaration so that we can use it easily below */        \
    typedef struct ktname##_##dtname##_SparseMap ktname##_##dtname##_SparseMap; \
    typedef struct ktname##_##dtname##_SparseMapItem ktname##_##dtname##_SparseMapItem; \
                                                                        \
    /**
     * The aligned attribute makes sure that even if ktype and dtype are of
     * size less than 8 bytes, they'll still behave as 8 byte values.
     * This is necessary for it's compatibility with generic API.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMapItem {                  \
        ktype  key __attribute__((aligned(8)));                         \
        dtype* data __attribute__((aligned(8)));                        \
        ktname##_##dtname##_SparseMapItem* next __attribute__((aligned(8))); \
    } ktname##_##dtname##_SparseMapItem;                                \
    DEF_STRUCT_VECTOR_INTERFACE(pfx##_smi, ktname##_##dtname##_Smi, ktname##_##dtname##_SparseMapItem, create_smi_copy, destroy_smi_copy); \
                                                                        \
    /**
     * Create type specific definition of SparseMap.
     * This definition will make the SparseMap different from other
     * type specific SparseMap definitions, separating the different
     * wrapper APIs from each other automatically.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMap {                      \
        Hash##ktname##Callback          hash;                           \
        Create##ktname##CopyCallback    create_data_copy;               \
        Destroy##ktname##CopyCallback   destroy_data_copy;              \
        Size                            data_size;                      \
        Create##ktname##CopyCallback    create_key_copy;                \
        Destroy##ktname##CopyCallback   destroy_key_copy;               \
        Compare##ktname##Callback       compare_key;                    \
        Size                            key_size;                       \
        Bool                            is_multimap;                    \
        Size                            max_item_count;                 \
        Size                            item_count;                     \
        BitVector*                      occupancy;                      \
        ktname##_##dtname##_Smi_Vector* map;                            \
    } ktname##_##dtname##_SparseMap;                                    \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMap* pfx##_sparse_map_create() { \
        return (ktname##_##dtname##_SparseMap*)sparse_map_create((HashCallback)(void*)hash, \
                                                         sizeof(ktype), \
                                                         (CreateElementCopyCallback)(void*)k_cpy_ctr, \
                                                         (DestroyElementCopyCallback)(void*)k_cpy_dtr, \
                                                         (CompareElementCallback)(void*)k_cmp, \
                                                         sizeof(dtype), \
                                                         (CreateElementCopyCallback)(void*)d_cpy_ctr, \
                                                         (DestroyElementCopyCallback)(void*)d_cpy_dtr, \
                                                         is_mm, max_lf); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_destroy(ktname##_##dtname##_SparseMap* map, void* udata) { \
        sparse_map_destroy((SparseMap*)map, udata);                     \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_resize(ktname##_##dtname##_SparseMap* map, Size size, void* udata) { \
        sparse_map_resize((SparseMap*)map, size, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_insert(ktname##_##dtname##_SparseMap* map, ktype key, dtype* data, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_insert((SparseMap*)map, (void*)(Uint64)key, (void*)(Uint64)data, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_search(ktname##_##dtname##_SparseMap* map, ktype key, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_search((SparseMap*)map, (void*)(Uint64)key, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_delete(ktname##_##dtname##_SparseMap* map, ktype key, void* udata) { \
        sparse_map_delete((SparseMap*)map, (void*)(Uint64)key, udata);  \
    }

#define DEF_STRUCT_STRUCT_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(pfx, type_prefix, hash, ktype, k_cpy_ctr, k_cpy_dtr, k_cmp, dtype, d_cpy_ctr, d_cpy_dtr, is_mm, max_lf) \
    /* define type specific callback types */                           \
    DEF_STRUCT_TYPE_SPECIFIC_CALLBACKS(ktype, ktname);                  \
                                                                        \
    /* forward declaration so that we can use it easily below */        \
    typedef struct ktname##_##dtname##_SparseMap ktname##_##dtname##_SparseMap; \
    typedef struct ktname##_##dtname##_SparseMapItem ktname##_##dtname##_SparseMapItem; \
                                                                        \
    /**
     * The aligned attribute makes sure that even if ktype and dtype are of
     * size less than 8 bytes, they'll still behave as 8 byte values.
     * This is necessary for it's compatibility with generic API.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMapItem {                  \
        ktype* key __attribute__((aligned(8)));                         \
        dtype* data __attribute__((aligned(8)));                        \
        ktname##_##dtname##_SparseMapItem* next __attribute__((aligned(8))); \
    } ktname##_##dtname##_SparseMapItem;                                \
    DEF_STRUCT_VECTOR_INTERFACE(pfx##_smi, ktname##_##dtname##_Smi, ktname##_##dtname##_SparseMapItem, create_smi_copy, destroy_smi_copy); \
                                                                        \
    /**
     * Create type specific definition of SparseMap.
     * This definition will make the SparseMap different from other
     * type specific SparseMap definitions, separating the different
     * wrapper APIs from each other automatically.
     * */                                                               \
    typedef struct ktname##_##dtname##_SparseMap {                      \
        Hash##ktname##Callback          hash;                           \
        Create##ktname##CopyCallback    create_data_copy;               \
        Destroy##ktname##CopyCallback   destroy_data_copy;              \
        Size                            data_size;                      \
        Create##ktname##CopyCallback    create_key_copy;                \
        Destroy##ktname##CopyCallback   destroy_key_copy;               \
        Compare##ktname##Callback       compare_key;                    \
        Size                            key_size;                       \
        Bool                            is_multimap;                    \
        Size                            max_item_count;                 \
        Size                            item_count;                     \
        BitVector*                      occupancy;                      \
        ktname##_##dtname##_Smi_Vector* map;                            \
    } ktname##_##dtname##_SparseMap;                                    \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMap* pfx##_sparse_map_create() { \
        return (ktype##_##dtname##_SparseMap*)sparse_map_create((HashCallback)(void*)hash, \
                                                                sizeof(ktype),    \
                                                                (CreateElementCopyCallback)(void*)k_cpy_ctr, \
                                                                (DestroyElementCopyCallback)(void*)k_cpy_dtr, \
                                                                (CompareElementCallback)(void*)k_cmp, \
                                                                sizeof(dtype),    \
                                                                (CreateElementCopyCallback)(void*)d_cpy_ctr, \
                                                                (DestroyElementCopyCallback)(void*)d_cpy_dtr, \
                                                                is_mm, max_lf); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_destroy(ktname##_##dtname##_SparseMap* map, void* udata) { \
        sparse_map_destroy((SparseMap*)map, udata);                     \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_resize(ktname##_##dtname##_SparseMap* map, Size size, void* udata) { \
        sparse_map_resize((SparseMap*)map, size, udata);                \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_insert(ktname##_##dtname##_SparseMap* map, ktype* key, dtype* data, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_insert((SparseMap*)map, (void*)(Uint64)key, (void*)(Uint64)data, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE ktname##_##dtname##_SparseMapItem* pfx##_sparse_map_search(ktname##_##dtname##_SparseMap* map, ktype* key, void* udata) { \
        return (ktname##_##dtname##_SparseMapItem*)sparse_map_search((SparseMap*)map, (void*)(Uint64)key, udata); \
    }                                                                   \
                                                                        \
    static FORCE_INLINE void pfx##_sparse_map_delete(ktname##_##dtname##_SparseMap* map, ktype* key, void* udata) { \
        sparse_map_delete((SparseMap*)map, (void*)(Uint64)key, udata);  \
    }

#endif // ANVIE_UTILS_CONTAINERS_SPARSE_MAP_INTERFACE_H
