/**
 * @file SparseMap.h
 * @date Mon, 18th December, 2023
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
 * @brief SparseMap data structure for constant time
 * search and insertions. Unlike dense map, this creates a linked
 * list of items that hash to same entry in the hash map. These linked
 * chains are called buckets and the whole idea is called separate chaining.
 * */

#ifndef ANVIE_UTILS_CONTAINERS_SPARSE_MAP_H
#define ANVIE_UTILS_CONTAINERS_SPARSE_MAP_H

#include "BitVector.h"
#include "Vector.h"
#include "Common.h"

/**
 * Represents a single item in the hash table.
 *
 * This is a complex structure that has two fields that might need
 * to call their own copy constructor and destructors if provided.
 *
 * Since this will be stored in a vector by itself, and from above
 * point, we know that it's construction can be quite complex, we
 * provide a copy constructor for this structure itself to make the
 * insertion to @c SparseMap::map vector easy.
 * */
typedef struct SparseMapItem SparseMapItem;
struct SparseMapItem {
    void* key; /**< To find position in sparse_map : `pos = hash(key) % length`*/
    void* data; /**< Data stored in this hash item. This is always a separate copy from what's provided by user. */
    SparseMapItem* next; /*< Point to next item in this bucket. If this is the last entry, then this ponts to NULL. */
};

/**
 * @brief Analogous to @c std::unordered_map or @c std::unordered_multimap in CPP.
 *
 * The @c SparseMap stores the `<key, value>` pair in a @c SparseMapItem struct
 * using separate chaining for collision resolution. This struct maintains its
 * own copy of both the key and data whenever possible.
 *
 * For example, in the case of both the key and data being strings: @c SparseMap<String, String>,
 * it's advisable to store the actual string instead of just their pointers to avoid
 * memory bugs (`use-after-free`).
 *
 * To ensure proper functionality, the user is required to provide copy constructors
 * and destructors for both the key and data if necessary. Additionally, the user must
 * provide the key comparison method, which will be used to resolve searches in cases of
 * collisions.
 *
 * Notes:
 * - If the data size or key size is not 1, 2, 4, or 8, it's highly recommended to provide
 *   copy constructors and destructors. Otherwise, the default behavior of @c SparseMap
 *   will be to use memcpy, treating the value as a pointer.
 * - For data sizes of 1, 2, 4, or 8 and if it's a structured data, it's mandatory to provide
 *   copy constructors and destructors. Otherwise, the implementation will copy the pointer
 *   rather than the data pointed to by the pointer.
 * - The implementation uses separate chaining for collision resolution, distinguishing it from
 *   @c DenseMap, which stores all elements in a linear array using linear probing (Robin Hood).
 *
 * The implementation of @c SparseMap provides two primary use cases:
 * - Not multimap: Each item has a unique key, allowing items to share the same hash while ensuring
 *   no two items in the map exist with the same key.
 * - Multimap: Multiple items with the same key can coexist in the same hashmap. When a search is performed,
 *   the first entry with the matching key is returned, guaranteeing that subsequent entries will either
 *   have the same key or the same hash value up to a certain number of entries.
 *
 * The behavior is influenced by the `is_multimap` member of the @c SparseMap object, which can be altered
 * at any time. However, it's also a required argument in the constructor of @c SparseMap.
 */
typedef struct SparseMap {
    HashCallback               hash; /**< Hash function */
    CreateElementCopyCallback  create_data_copy; /**< Copy constructor of key */
    DestroyElementCopyCallback destroy_data_copy; /**< Copy destructor of key */
    Size                       data_size; /**< Size of data in bytes. */
    CreateElementCopyCallback  create_key_copy; /**< Copy constructor of key */
    DestroyElementCopyCallback destroy_key_copy; /**< Copy destructor of key */
    CompareElementCallback     compare_key; /**< Comparision function to compare whether or not two keys are same. */
    Size                       key_size; /**< Size of key in bytes. */
    Bool                       is_multimap; /**< True when contains multiple items with same key. False otherwise. */
    Size                       max_item_count; /**< Maximum load factor tolerance before we resize the hash table. */
    Size                       item_count; /**< Total number of elements in the hash table. */
    BitVector*                 occupancy; /**< BitVector to store whether a particular bucket is empty or occupied. */
    Vector*                    map; /**< Vector<SparseMapItem> A vector to store all elements in the map. */
} SparseMap;

SparseMap* sparse_map_create(
    HashCallback               hash,
    Size                       key_size,
    CreateElementCopyCallback  create_key_copy,
    DestroyElementCopyCallback destroy_key_copy,
    CompareElementCallback     compare_key,
    Size                       data_size,
    CreateElementCopyCallback  create_data_copy,
    DestroyElementCopyCallback destroy_data_copy,
    Bool                       is_multimap,
    Float32                    max_load_factor
);
void           sparse_map_destroy(SparseMap* map, void* udata);
void           sparse_map_resize(SparseMap* map, Size size, void* udata);
SparseMapItem* sparse_map_insert(SparseMap* map, void* key, void* value, void* udata);
SparseMapItem* sparse_map_search(SparseMap* map, void* key, void* udata);
void           sparse_map_delete(SparseMap* map, void* key, void* udata);

#include "Interface/SparseMap.h"

/*                                      prefix  prefix   hash     ktype  kcompare    dtype */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u8_u8,  U8_U8_,  hash_u8, Uint8, compare_u8, Uint8);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u8_u16, U8_U16_, hash_u8, Uint8, compare_u8, Uint16);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u8_u32, U8_U32_, hash_u8, Uint8, compare_u8, Uint32);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u8_u64, U8_U64_, hash_u8, Uint8, compare_u8, Uint64);

/*                                      prefix   prefix    hash      ktype   kcompare     dtype */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u16_u8,  U16_U8_,  hash_u16, Uint16, compare_u16, Uint8);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u16_u16, U16_U16_, hash_u16, Uint16, compare_u16, Uint16);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u16_u32, U16_U32_, hash_u16, Uint16, compare_u16, Uint32);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u16_u64, U16_U64_, hash_u16, Uint16, compare_u16, Uint64);

/*                                      prefix   prefix    hash      ktype   kcompare     dtype */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u32_u8,  U32_U8_,  hash_u32, Uint32, compare_u32, Uint8);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u32_u16, U32_U16_, hash_u32, Uint32, compare_u32, Uint16);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u32_u32, U32_U32_, hash_u32, Uint32, compare_u32, Uint32);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u32_u64, U32_U64_, hash_u32, Uint32, compare_u32, Uint64);

/*                                      prefix   prefix    hash      ktype   kcompare     dtype */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u64_u8,  U64_U8_,  hash_u64, Uint64, compare_u64, Uint8);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u64_u16, U64_U16_, hash_u64, Uint64, compare_u64, Uint16);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u64_u32, U64_U32_, hash_u64, Uint64, compare_u64, Uint32);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE(u64_u64, U64_U64_, hash_u64, Uint64, compare_u64, Uint64);

/*                                                             prefix    prefix     hash          ktype    kcreate              kdestroy              kcompare         dtype   dcreate  ddestroy  is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u8,  ZStr_U8_,  hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint8 , NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u16, ZStr_U16_, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint16, NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u32, ZStr_U32_, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint32, NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u64, ZStr_U64_, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint64, NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);

/*                                                             prefix    prefix     hash      ktype   kcreate  kdestroy  kcompare     dtype   dcreate             ddestroy             is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u8_zstr,  U8_ZStr_,  hash_u8 , Uint8 , NULL,    NULL,     compare_u8 , ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u16_zstr, U16_ZStr_, hash_u16, Uint16, NULL,    NULL,     compare_u16, ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u32_zstr, U32_ZStr_, hash_u32, Uint32, NULL,    NULL,     compare_u32, ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u64_zstr, U64_ZStr_, hash_u64, Uint64, NULL,    NULL,     compare_u64, ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);

/*                                                             prefix     prefix     hash       ktype    kcreate           kdestroy           kcompare      dtype    dcreate           ddestroy           is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_zstr, ZStr_ZStr, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);


#endif // ANVIE_UTILS_CONTAINERS_SPARSE_MAP_H
