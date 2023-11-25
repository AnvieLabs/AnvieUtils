/**
 * @file HashMap.h
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
 * @brief HashMap data structure for constant time
 * search and insertions.
 * */

#include "Vector.h"

/**
 * A hash function will take data and convert it to a
 * 64 bit hash value that'll be used to place elements
 * into the array.
 * @param key Key of hash function. This key can be an integer value,
 * a pointer to some memory or some struct, anything at all! It can be
 * a function as well! All depends on the @c HashCallback.
 * @param elem_count Number of elements in @c HashMap.
 * @param udata User data provided when doing insertion or any other
 * related operation. This can be used to pass in extra things that
 * keep changing through the program.
 * @return A unique hash value associated with given data.
 * */
typedef Size (*HashCallback)(void* key, Size elem_count, void* udata);

/**
 * Analogous to unordered map in CPP.
 * */
typedef struct anvie_hash_map_t {
    HashCallback hash;  /**< Hash function */
    Vector*              elements; /**< A vector to store all elements in the map */
} HashMap;

HashMap* hash_map_create(
    HashCallback              hash,
    Size                      element_size,
    CreateElementCopyCallback create_copy,
    DestroyElementCopyCallback destroy_copy
);
void hash_map_destroy(HashMap* map, void* udata);
void hash_map_insert(HashMap* map, void* key, void* value, void* udata);
