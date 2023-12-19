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
 * search and insertions.
 * */

#include <Anvie/Containers/SparseMap.h>
#include <Anvie/HelperDefines.h>
#include <string.h>

#define SPARSE_MAP_INITIAL_SIZE 64

/**
 * Callback data to be passed to copy constructor and copy destructor of
 * @c SparseMapItem.
 * */
typedef struct CallbackData {
    void*      udata;           /**< User data passed to callbacks. */
    SparseMap* map;             /**< @c SparseMap to which @c SparseMapItem will be inserted*/
} CallbackData;
void create_smi_copy(SparseMapItem* dst, SparseMapItem* src,  CallbackData* clbk_data);
void destroy_smi_copy(SparseMapItem* copy, CallbackData* clbk_data);
DEF_STRUCT_VECTOR_INTERFACE(smi, SparseMapItem, create_smi_copy, destroy_smi_copy);

static FORCE_INLINE Size compute_next_power_of_two(Size n);
static FORCE_INLINE SparseMapItem* insert_into_sparse_map_directly(SparseMap* map, SparseMapItem* item, void* udata);

/**
 * Create a new hash map.
 * @param hash Hash function to be used.
 * @param element_size Size of each element in hash map.
 * @param create_copy Copy constructor for creating hash map of of given values.
 * @param destroy_copy Copy destructor for creating hash map of of given values.
 * @return SparseMap object on success, NULL otherwise.
 * */

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
) {
    RETURN_VALUE_IF_FAIL(hash && data_size && key_size, NULL, ERR_INVALID_ARGUMENTS);

    // both must be null or non null at the same time
    Bool b1 = create_data_copy != NULL;
    Bool b2 = destroy_data_copy != NULL;
    RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL,
                         "Either both data copy constructor and destructor should be NULL, "
                         "or both should be non NULL at the same time!");

    // both must be null or non null at the same time
    b1 = create_key_copy != NULL;
    b2 = destroy_key_copy != NULL;
    RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL,
                         "Either both key copy constructor and destructor should be NULL, "
                         "or both should be non NULL at the same time!");

    // create bitvector to keep track of occupied buckets
    BitVector* bv = bitvector_create();
    RETURN_VALUE_IF_FAIL(bv, NULL, "Failed to create a BitVector\n");
    bitvector_resize(bv, SPARSE_MAP_INITIAL_SIZE);

    // create vector to store SparseMapItem entries for the SparseMap.
    Vector* smi_vec = smi_vector_create();
    if(!smi_vec) {
        bitvector_destroy(bv);
        ERR(__FUNCTION__, "Failed to create Vector<SparseMapItem>\n");
        return NULL;
    }
    smi_vector_resize(smi_vec, SPARSE_MAP_INITIAL_SIZE);

    // finally create dense map
    SparseMap* map = NEW(SparseMap);
    if(!map) {
        bitvector_destroy(bv);
        smi_vector_destroy(smi_vec, NULL);
        ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
        return NULL;
    }

    map->occupancy         = bv;
    map->map               = smi_vec;
    map->hash              = hash;
    map->create_data_copy  = create_data_copy;
    map->destroy_data_copy = destroy_data_copy;
    map->data_size         = data_size;
    map->create_key_copy   = create_key_copy;
    map->destroy_key_copy  = destroy_key_copy;
    map->compare_key       = compare_key;
    map->key_size          = key_size;
    map->is_multimap       = is_multimap;
    map->max_load_factor   = max_load_factor;
    map->item_count        = 0;

    return map;
}

/**
 * Destroy hash map.
 * @param map SparseMap object to be destroyed.
 * @param udata User data passed to callback functions.
 * This will be passed to copy destructor for each data copy
 * of this @c SparseMap.
 * */
void sparse_map_destroy(SparseMap* map, void* udata) {
    RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);

    if(map->occupancy) {
        bitvector_destroy(map->occupancy);
        map->occupancy = NULL;
    }

    if(map->map) {
        CallbackData clbk_data = {
            .udata = udata,
            .map   = map
        };
        smi_vector_destroy(map->map, &clbk_data);
        map->map = NULL;
    }

    FREE(map);
}

/**
 * Resize hash map to contain the given number of items.
 * The implementation will in the end force the hash map to keep
 * the size in power of 2. So, for example if size is 200, the actual
 * size of hash map will be 256. If new size is 1024, then it stays 1024.
 *
 * It's recommended to keepp
 * @param map SparseMap to be resized.
 * @param size New size. The actual size of hash map will be next power of 2 from given size.
 * @param udata User data passed to callback functions.
 * Since the whole @c SparseMap will be rehashed, we need this user data as well.
 * */
void sparse_map_resize(SparseMap* map, Size size, void* udata) {
    RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);

    // when we reach a size greater than or equal to given size and  that's also a power of 2, then we break.
    Size sz = compute_next_power_of_two(size);

    /* Create a ne bitvector to store data about occupancy of each slot. */
    BitVector* new_occupancy = bitvector_create();
    RETURN_IF_FAIL(new_occupancy, "Failed to create a new BitVector to store occupancy data\n");
    bitvector_resize(new_occupancy, size);

    // create vector to store SparseMapItem entries for the SparseMap.
    Vector* smi_vec = smi_vector_create();
    if(!smi_vec) {
        bitvector_destroy(new_occupancy);
        ERR(__FUNCTION__, "Failed to create Vector<SparseMapItem>\n");
        return;
    }
    smi_vector_resize(smi_vec, sz);

    // store old map vector with different name
    Vector*    old_smi_vec   = map->map;
    BitVector* old_occupancy = map->occupancy;

    // assign new, unhashed maps to this SparseMap
    map->map        = smi_vec;
    map->occupancy  = new_occupancy;
    map->item_count = 0;

    // go through each item in old map and insert if the slot is occupied.
    Size old_map_len = old_smi_vec->length;
    for(Size s = 0; s < old_map_len; s++) {
        if(bitvector_peek(old_occupancy, s)) {
            /* go through each item in a bucket and keep inserting while we not reach the end of bucket */
            SparseMapItem* iter = smi_vector_peek(old_smi_vec, s);
            insert_into_sparse_map_directly(map, iter, udata);

            /* items coming after the first one in a bucket are allocated separately. After inserting, free them. */
            iter = iter->next;
            while(iter) {
                insert_into_sparse_map_directly(map, iter, udata);
                SparseMapItem* next = iter->next;
                memset(iter, 0, sizeof(SparseMapItem));
                FREE(iter);
                iter = next;
            }
        }
    }

    // Cannot destroy map directly as this will destroy all previously created copies
    // All this trickery is to void calling copy constructor twice for same key-value pair.
    FREE(old_smi_vec->data);
    old_smi_vec->data = NULL;
    FREE(old_smi_vec);

    bitvector_destroy(old_occupancy);
}

/**
 * Insert given key-value pair into given @c SparseMap.
 * @param map
 * @param key Can be an integer value or pointer to some struct or memory.
 * @param value Value to be inserted into @c SparseMap.
 * This too can be an integer value or a pointer to some memory.
 * @param udata Pointer to user data provied to callbacks :
 * `hash`, `compare_key`, `create_data_copy`, `create_key_copy`, `destroy_data_copy`, `destroy_key_copy`
 * @return Pointer to new inserted @c SparseMapItem in @c SparseMap.
 * */
SparseMapItem* sparse_map_insert(SparseMap* map, void* key, void* value, void* udata) {
    RETURN_VALUE_IF_FAIL(map, NULL, ERR_INVALID_ARGUMENTS);

    Float32 load_factor = ((Float32)map->item_count + 1)/((Float32)map->map->length);
    if(load_factor > map->max_load_factor) {
        sparse_map_resize(map, map->map->length * 2, udata);
    }

    CallbackData clbk_data = {
        .udata = udata,
        .map   = map
    };
    SparseMapItem tmp_smi = { .key = key, .data = value };

    if(!map->is_multimap) {
        SparseMapItem* searched_smi = sparse_map_search(map, key, udata);
        if(searched_smi) {
            destroy_smi_copy(searched_smi, &clbk_data);
            create_smi_copy(searched_smi, &tmp_smi, &clbk_data);
            return searched_smi;
        }
    }

    SparseMapItem this_smi;
    create_smi_copy(&this_smi, &tmp_smi, &clbk_data);

    return insert_into_sparse_map_directly(map, &this_smi, udata);
}

/**
 * Search for element in @c SparseMap.
 * @param map
 * @param key
 * @param udata User data passed to callbacks : `hash`, `compare_key`
 * @return Pointer to first @c SparseMapItem* with equivalent or matching key, else @c NULL.
 * */
SparseMapItem* sparse_map_search(SparseMap* map, void* key, void* udata) {
    RETURN_VALUE_IF_FAIL(map, NULL, ERR_INVALID_ARGUMENTS);

    Size len_wrap_mask = map->map->length - 1;
    Size hash = map->hash(key, udata);
    Size pos = hash & len_wrap_mask;

    /* if the bucket is empty, then no value is there, return NULL */
    if(!bitvector_peek(map->occupancy, pos)) {
        return NULL;
    }

    /* if however bucket is not empty, search for matching key in bucket at position */
    SparseMapItem* iter = smi_vector_peek(map->map, pos);
    while(iter) {
        if(map->compare_key(iter->key, key, udata) == 0) {
            return iter;
        }
        iter = iter->next;
    }

    return NULL;
}

/**
 * Delete all items with given key in @c SparseMap.
 * When behaviour is of multimap then all items with same key
 * are destroyed. When behaviour is otherwise then only the entry
 * with given key is destroyed, because there exists only one such
 * entry.
 * @param map
 * @param key
 * @param udata User data passed to callbacks : `hash`, `compare_key`,
 * `destroy_smi_copy`.
 * */
void sparse_map_delete(SparseMap* map, void* key, void* udata) {
    RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);

    Size len_wrap_mask = map->map->length - 1;
    Size hash = map->hash(key, udata);
    Size pos = hash & len_wrap_mask;

    /* if the bucket is empty, then no value is there, return NULL */
    if(!bitvector_peek(map->occupancy, pos)) {
        return;
    }

    CallbackData clbk_data = {
        .udata = udata,
        .map   = map
    };

    // search for item with given key
    SparseMapItem* prev = NULL;
    SparseMapItem* iter = sparse_map_search(map, key, udata);
    while(iter) {
        /* destroy only if keys are exactly same */
        SparseMapItem* next = iter->next;
        if(map->compare_key(iter->key, key, udata) == 0) {
            destroy_smi_copy(iter, &clbk_data);
            if(prev) {
                prev->next = next;
                memset(iter, 0, sizeof(SparseMapItem));
                FREE(iter);
            } else if (next) {
                memcpy(iter, next, sizeof(SparseMapItem));
            }
        }
        iter = next;
    }
}

/************************************ PRIVATE FUNCTIONS ***************************************/

/**
 * Macro to create code for wrapper over copy constructors
 * @param d Data variable name.
 * @param s Src variable name.
 * @param n Name of copy constructor (key/data).
 * */
#define CREATE_COPY(d, s, n)                                            \
    do {                                                                \
        SparseMap* hmap = clbk_data->map;                               \
        if(hmap->create_##n##_copy) {                                   \
            (d)->n = ALLOCATE(Uint8, hmap->n##_size);                   \
            RETURN_IF_FAIL((d)->n, ERR_OUT_OF_MEMORY);                  \
            /* same callback data is passed to all callbacks. */        \
            hmap->create_##n##_copy((d)->n, (s)->n, clbk_data->udata);  \
        } else if(hmap->n##_size <= 8) {                                \
            (d)->n = (s)->n;                                            \
        } else  {                                                       \
            (d)->n = ALLOCATE(Uint8, hmap->n##_size);                   \
            RETURN_IF_FAIL((d)->n, ERR_OUT_OF_MEMORY);                  \
            if((s)->n) memcpy((d)->n, (s)->n, hmap->n##_size);          \
        }                                                               \
    } while(0)

/**
 * Macro to create code for wrapper over copy destructors
 * @param c Copy variable name.
 * @param n Name of copy destructor (key/data).
 * */
#define DESTROY_COPY(c, n)                                      \
    do {                                                        \
        SparseMap* hmap = clbk_data->map;                       \
        if(hmap->destroy_##n##_copy) {                          \
            /* same callback data is passed to all callbacks. */\
            hmap->destroy_##n##_copy(c->n, clbk_data->udata);   \
            FREE(c->n);                                         \
        } else if(hmap->n##_size > 8) {                         \
            FREE(c->n);                                         \
        }                                                       \
        c->n = 0;                                               \
    } while(0)

/**
 * Copy constructor for @c SparseMapItem array storing <key, value> pair of hash map.
 * @param dst Destination where @c SparseMapItem will be copied to
 * @param src Pointer where original copy of @c SparseMapItem relies.
 * @param clbk_data Callback data passed to copy constructor
 * */
void create_smi_copy(SparseMapItem* dst, SparseMapItem* src, CallbackData* clbk_data){
    RETURN_IF_FAIL(dst && clbk_data, ERR_INVALID_ARGUMENTS);

    CREATE_COPY(dst, src, data);
    CREATE_COPY(dst, src, key);
}

/**
 * Destroy element copy from the void*
 * @param copy Pointer to SparseMapItem copy to be destroyed
 * @param clbk_data Callback data
 * */
void destroy_smi_copy(SparseMapItem* copy, CallbackData* clbk_data) {
    RETURN_IF_FAIL(copy && clbk_data, ERR_INVALID_ARGUMENTS);

    DESTROY_COPY(copy, data);
    DESTROY_COPY(copy, key);
}

/**
 * Compute next power of two compared to given number n in constant time.
 * @param n
 * @return 2^(floor(log(n)) + 1),
 * */
static FORCE_INLINE Size compute_next_power_of_two(Size n) {
    n--; // Decrease n by 1 to handle cases where n is already a power of 2
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1; // Increment the result by 1 to get the next power of 2
}

/**
 * Insert a value into hash map without creating any copy of it.
 * @param map
 * @param item
 * @param udata
 * */
static inline SparseMapItem* insert_into_sparse_map_directly(SparseMap* map, SparseMapItem* item, void* udata) {
    RETURN_VALUE_IF_FAIL(map && item, NULL, ERR_INVALID_ARGUMENTS);

    Size hash = map->hash(item->key, udata);
    Size len_wrap_mask = map->map->length - 1;
    Size pos = hash & len_wrap_mask; /* it's guaranteed that size of sparsemap will always be in powers of two */

    SparseMapItem* iter = smi_vector_address_at(map->map, pos);
    if(bitvector_peek(map->occupancy, pos)) {
        /* if bucket has is occupied : separate-chaining */
        while(iter->next) {
            iter = iter->next;
        }

        /* create new item for chain and add to chain. */
        SparseMapItem* newitem = NEW(SparseMapItem);
        RETURN_VALUE_IF_FAIL(newitem, NULL, ERR_OUT_OF_MEMORY);
        memcpy(newitem, item, sizeof(SparseMapItem));
        iter->next = newitem;

        map->item_count++;
        return newitem;
    } else {
        /* if bucket is empty */
        memcpy(iter, item, sizeof(SparseMapItem));
        iter->next = NULL;

        map->item_count++;
        return iter;
    }
}
