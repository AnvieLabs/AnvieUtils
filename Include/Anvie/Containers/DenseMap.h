/**
 * @file DenseMap.h
 * @date Mon, 6th October, 2023
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
 * @brief DenseMap data structure for constant time
 * search and insertions.
 * */

#ifndef ANVIE_UTILS_CONTAINERS_DENSE_MAP_H
#define ANVIE_UTILS_CONTAINERS_DENSE_MAP_H

#include "Vector.h"
#include "Common.h"

/**
 * A hash function will take data and convert it to a
 * 64 bit hash value that'll be used to place elements
 * into the array.
 * @param key Key of hash function. This key can be an integer value,
 * a pointer to some memory or some struct, anything at all! It can be
 * a function as well! All depends on the @c HashCallback.
 * @param udata User data provided when doing insertion or any other
 * related operation. This can be used to pass in extra things that
 * keep changing through the program.
 * @return A unique hash value associated with given data.
 * */
typedef Size (*HashCallback)(void* key, void* udata);

/**
 * Represents a single item in the hash table.
 *
 * This is a complex structure that has two fields that might need
 * to call their own copy constructor and destructors if provided.
 *
 * Since this will be stored in a vector by itself, and from above
 * point, we know that it's construction can be quite complex, we
 * provide a copy constructor for this structure itself to make the
 * insertion to @c DenseMap::map vector easy.
 * */
typedef struct DenseMapItem {
    void* key; /**< To find position in sparse_map : `pos = hash(key) % length`*/
    void* data; /**< Data stored in this hash item. This is always a separate copy from what's provided by user. */
} DenseMapItem;

/**
 * Analogous to @c std::unordered_map or @c std::unordered_multimap in CPP.
 *
 * The @c DenseMap stores the `<key, value>` pair in a @c DenseMapItem struct.
 * This struct keeps it's own copy of both key and data whenever possible.
 * For eg: when both type of key and data is string : @c DenseMap<String, String>
 * In such cases, one might need to store the actual string instead of just
 * their pointers to avoid memory bugs (`use-after-free`).
 *
 * To achieve this, we ask the user to provide copy constructor and destructor
 * for both key and data if required. The user is also supposed to give us the
 * key compare method. This will be used to resolve searches when there are
 * collisions.
 *
 * Notes :
 * - If the data size or key size is not 1, 2, 4 or 8, then it's highly recommended
 *   to provide a copy constructor and destructor. The @c DenseMap implementation will
 *   by default choose to do a memcpy treating the value as a pointer.
 * - If the data size or key size is 1, 2, 4, or 8 and it's a struct data, then it's
 *   strictly required to provide a copy constructor and destructor as in otherwise case
 *   the implementation will be copying the pointer and not the data pointed by the
 *   pointer.
 *   In case of hash map of struct key or struct value pairs, it's strictly required
 *   to provide copy constructors and destructors!
 * - The implementation uses Robin Hood collision resolution strategy to solve collisions.
 * - Due to the above point, the whole hash map is like a flat array and no use of separate
 *   chaining is done. This is what gives the name @c DenseMap, because all items are in
 *   a sparse vector of key-value pairs and not sparse vector of pointer to the pairs.
 *
 * The implementation provides two use cases :
 * - Not multimap : There will exist items with each having a unique key. The hash of the
 *   keys might be same, but no two items in the map will exist with same key.
 * - Multimap : Multiple items with same key may coexist in the same hashmap. When search
 *   is performed, the first entry with given matching key is returned and it is guaranteed
 *   that all the following entries will either have same key or have same hash value up until
 *   certain number of entries. To get this size, one may call the @c dense_map_count_same_hash()
 *   method to get exact number of entries with same hash value or @c dense_map_count_same_key()
 *   method to get exact number of entries with same key.
 *
 * The above feature is dependent on the `is_multimap` member of @c DenseMap object. This can
 * be changed anytime but is also required as an argument to the constructor of @c DenseMap.
 * */
typedef struct DenseMap {
    HashCallback               hash; /**< Hash function */
    CreateElementCopyCallback  create_data_copy; /**< Copy constructor of key */
    DestroyElementCopyCallback destroy_data_copy; /**< Copy destructor of key */
    Size                       data_size; /**< Size of data in bytes. */
    CreateElementCopyCallback  create_key_copy; /**< Copy constructor of key */
    DestroyElementCopyCallback destroy_key_copy; /**< Copy destructor of key */
    CompareElementCallback     compare_key; /**< Comparision function to compare whether or not two keys are same. */
    Size                       key_size; /**< Size of key in bytes. */
    Bool                       is_multimap; /**< True when contains multiple items with same key. False otherwise. */
    Float32                    max_load_factor; /**< Maximum load factor tolerance before we resize the hash table. */
    Size                       filled_slot_count; /**< Total number of slots filled in the hash table. */
    Vector*                    probe_len; /**< Vector<Uint8> to store probing length for each corresponding item in the map. */
    Vector*                    metadata; /**< Vector<Uint8> to store metadata about each corresponding element in map. */
    Vector*                    map; /**< Vector<DenseMapItem> A vector to store all elements in the map. */
} DenseMap;

DenseMap* dense_map_create(
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
void          dense_map_destroy(DenseMap* map, void* udata);
void          dense_map_resize(DenseMap* map, Size size, void* udata);
DenseMapItem* dense_map_insert(DenseMap* map, void* key, void* value, void* udata);
DenseMapItem* dense_map_search(DenseMap* map, void* key, void* udata);
void          dense_map_delete(DenseMap* map, void* key, void* udata);

#include "Interface/DenseMap.h"

/*                                      prefix  prefix   hash     ktype  kcompare    dtype */
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u8_u8,  U8_U8_,  hash_u8, Uint8, compare_u8, Uint8);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u8_u16, U8_U16_, hash_u8, Uint8, compare_u8, Uint16);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u8_u32, U8_U32_, hash_u8, Uint8, compare_u8, Uint32);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u8_u64, U8_U64_, hash_u8, Uint8, compare_u8, Uint64);

/*                                      prefix   prefix    hash      ktype   kcompare     dtype */
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u16_u8,  U16_U8_,  hash_u16, Uint16, compare_u16, Uint8);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u16_u16, U16_U16_, hash_u16, Uint16, compare_u16, Uint16);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u16_u32, U16_U32_, hash_u16, Uint16, compare_u16, Uint32);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u16_u64, U16_U64_, hash_u16, Uint16, compare_u16, Uint64);

/*                                      prefix   prefix    hash      ktype   kcompare     dtype */
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u32_u8,  U32_U8_,  hash_u32, Uint32, compare_u32, Uint8);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u32_u16, U32_U16_, hash_u32, Uint32, compare_u32, Uint16);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u32_u32, U32_U32_, hash_u32, Uint32, compare_u32, Uint32);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u32_u64, U32_U64_, hash_u32, Uint32, compare_u32, Uint64);

/*                                      prefix   prefix    hash      ktype   kcompare     dtype */
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u64_u8,  U64_U8_,  hash_u64, Uint64, compare_u64, Uint8);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u64_u16, U64_U16_, hash_u64, Uint64, compare_u64, Uint16);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u64_u32, U64_U32_, hash_u64, Uint64, compare_u64, Uint32);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE(u64_u64, U64_U64_, hash_u64, Uint64, compare_u64, Uint64);

/*                                                            prefix   prefix    hash         ktype   kcreate             kdestroy             kcompare        dtype   dcreate  ddestroy  is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(str_u8,  Str_U8_,  hash_string, String, string_create_copy, string_destroy_copy, compare_string, Uint8 , NULL,    NULL,     True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(str_u16, Str_U16_, hash_string, String, string_create_copy, string_destroy_copy, compare_string, Uint16, NULL,    NULL,     True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(str_u32, Str_U32_, hash_string, String, string_create_copy, string_destroy_copy, compare_string, Uint32, NULL,    NULL,     True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(str_u64, Str_U64_, hash_string, String, string_create_copy, string_destroy_copy, compare_string, Uint64, NULL,    NULL,     True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);

/*                                                            prefix   prefix    hash      ktype   kcreate  kdestroy  kcompare     dtype   dcreate             ddestroy             is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u8_str,  U8_Str_,  hash_u8 , Uint8 , NULL,    NULL,     compare_u8 , String, string_create_copy, string_destroy_copy, True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u16_str, U16_Str_, hash_u16, Uint16, NULL,    NULL,     compare_u16, String, string_create_copy, string_destroy_copy, True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u32_str, U32_Str_, hash_u32, Uint32, NULL,    NULL,     compare_u32, String, string_create_copy, string_destroy_copy, True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u64_str, U64_Str_, hash_u64, Uint64, NULL,    NULL,     compare_u64, String, string_create_copy, string_destroy_copy, True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);

/*                                                            prefix   prefix   hash         ktype   kcreate             kdestroy             kcompare        dtype   dcreate             ddestroy             is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_DENSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(str_str, Str_Str, hash_string, String, string_create_copy, string_destroy_copy, compare_string, String, string_create_copy, string_destroy_copy, True,        DENSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);


#endif // ANVIE_UTILS_CONTAINERS_DENSE_MAP_H
