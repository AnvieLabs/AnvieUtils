/**
 * @file DenseMap.h
 * @date Wed, 29th October, 2023
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

#include <Anvie/Containers/DenseMap.h>
#include <Anvie/HelperDefines.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>
#include <string.h>

#define MDATA_OCCUPANCY_MASK (1 << 7)
#define MDATA_HASH_MASK (0x7f)
#define DENSE_MAP_INITIAL_SIZE 64

/**
 * Callback data to be passed to copy constructor and copy destructor of
 * @c DenseMapItem.
 * */
typedef struct CallbackData {
    void*                     udata; /**< User data passed to callbacks. */
    DenseMap*                 map; /**< @c DenseMap to which @c DenseMapItem will be inserted*/
} CallbackData;
void create_dmi_copy(DenseMapItem* dst, DenseMapItem* src,  CallbackData* clbk_data);
void destroy_dmi_copy(DenseMapItem* copy, CallbackData* clbk_data);
DEF_STRUCT_VECTOR_INTERFACE(dmi, Dmi_, DenseMapItem, create_dmi_copy, destroy_dmi_copy);

static inline DenseMapItem* insert_into_dense_map_directly(DenseMap* map, DenseMapItem* dmi, void* udata);

/**
 * Create a new hash map.
 * @param hash Hash function to be used.
 * @param element_size Size of each element in hash map.
 * @param create_copy Copy constructor for creating hash map of of given values.
 * @param destroy_copy Copy destructor for creating hash map of of given values.
 * @return DenseMap object on success, NULL otherwise.
 * */

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
) {
    ERR_RETURN_VALUE_IF_FAIL(hash && data_size && key_size, NULL, ERR_INVALID_ARGUMENTS);

    // both must be null or non null at the same time
    Bool b1 = create_data_copy != NULL;
    Bool b2 = destroy_data_copy != NULL;
    ERR_RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL, ERR_INVALID_ARGUMENTS);

    // both must be null or non null at the same time
    b1 = create_key_copy != NULL;
    b2 = destroy_key_copy != NULL;
    ERR_RETURN_VALUE_IF_FAIL(!(b1 ^ b2), NULL, ERR_INVALID_ARGUMENTS);

    // create vector to store DenseMapItem entries for the DenseMap.
    Vector* dmi_vec = dmi_vector_create();
    ERR_RETURN_VALUE_IF_FAIL(dmi_vec, NULL, ERR_INVALID_OBJECT);
    dmi_vector_resize(dmi_vec, DENSE_MAP_INITIAL_SIZE);

    // create vector to store metadata about each entry in the DenseMap.
    Vector* mdata_vec = u8_vector_create();
    if(!mdata_vec) {
        dmi_vector_destroy(dmi_vec, NULL);
        ERR(__FUNCTION__, ERRMSG(ERR_INVALID_OBJECT));
        return NULL;
    }
    u8_vector_resize(mdata_vec, DENSE_MAP_INITIAL_SIZE);

    Vector* pl_vec = u8_vector_create();
    if(!pl_vec) {
        dmi_vector_destroy(dmi_vec, NULL);
        u8_vector_destroy(mdata_vec, NULL);
        ERR(__FUNCTION__, ERRMSG(ERR_INVALID_OBJECT));
        return NULL;
    }
    u8_vector_resize(pl_vec, DENSE_MAP_INITIAL_SIZE);

    // finally create dense map
    DenseMap* map = NEW(DenseMap);
    if(!map) {
        dmi_vector_destroy(dmi_vec, NULL);
        u8_vector_destroy(mdata_vec, NULL);
        u8_vector_destroy(pl_vec, NULL);
        ERR(__FUNCTION__, ERRMSG(ERR_OUT_OF_MEMORY));
        return NULL;
    }

    map->probe_len         = pl_vec;
    map->metadata          = mdata_vec;
    map->map               = dmi_vec;
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
    map->item_count = 0;

    return map;
}

/**
 * Destroy hash map.
 * @param map DenseMap object to be destroyed.
 * @param udata User data passed to callback functions.
 * This will be passed to copy destructor for each data copy
 * of this @c DenseMap.
 * */
void dense_map_destroy(DenseMap* map, void* udata) {
    ERR_RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);

    if(map->map) {
        CallbackData clbk_data = {
            .udata = udata,
            .map   = map
        };
        dmi_vector_destroy(map->map, &clbk_data);
        map->map = NULL;
    }

    if(map->metadata) {
        u8_vector_destroy(map->metadata, NULL);
        map->metadata = NULL;
    }

    if(map->probe_len) {
        u8_vector_destroy(map->probe_len, NULL);
        map->probe_len = NULL;
    }

    FREE(map);
}

/**
 * Resize hash map to contain the given number of items.
 * It's recommented to keep hash sizes in power of 2. Keeping
 * the size in power of two allows the implementation to replace
 * modulo operation with logical and which is many many many times
 * faster!
 *
 * The implementation will in the end force the hash map to keep
 * the size in power of 2. So, for example if size is 200, the actual
 * size of hash map will be 256. If new size is 1024, then it stays 1024.
 *
 * If ater resize, there are collisions, then it'll be resolved,
 * as if this were a multimap. But when using the insert operation
 * after resize, it'll fallback to it's old behaviour.
 *
 * Using resize, one cannot reduce the size of hash map. This implementation
 * does not reduce size, even when the whole hash map becomes empty at once
 * stage.
 *
 * @param map DenseMap to be resized.
 * @param size New size. The actual size of hash map will be next power of 2 from given size.
 * @param udata User data passed to callback functions.
 * Since the whole @c DenseMap will be rehashed, we need this user data as well.
 * */
void dense_map_resize(DenseMap* map, Size size, void* udata) {
    ERR_RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);
    if(size <= map->item_count) {
        return;
    }

    // when we reach a size greater than or equal to given size and  that's also a power of 2, then we break.
    Size sz = NEXT_POW2(size);

    // create vector to store DenseMapItem entries for the DenseMap.
    Vector* dmi_vec = dmi_vector_create();
    ERR_RETURN_IF_FAIL(dmi_vec,  ERR_INVALID_OBJECT);
    dmi_vector_resize(dmi_vec, sz);

    // create vector to store metadata about each entry in the DenseMap.
    Vector* mdata_vec = u8_vector_create();
    if(!mdata_vec) {
        dmi_vector_destroy(dmi_vec, NULL);
        ERR(__FUNCTION__, ERRMSG(ERR_INVALID_OBJECT));
        return;
    }
    u8_vector_resize(mdata_vec, sz);

    // create vector to store probe sequence length
    Vector* psl_vec = u8_vector_create();
    if(!psl_vec) {
        dmi_vector_destroy(dmi_vec, NULL);
        u8_vector_destroy(mdata_vec, NULL);
        ERR(__FUNCTION__, ERRMSG(ERR_INVALID_OBJECT));
        return;
    }
    u8_vector_resize(psl_vec, sz);

    // store old map vectors with different names
    Vector* old_dmi_vec = map->map;

    Vector* old_mdata_vec = map->metadata;
    u8_vector_destroy(map->probe_len, NULL);

    // assign new, unhashed maps to this DenseMap
    map->map       = dmi_vec;
    map->metadata  = mdata_vec;
    map->probe_len = psl_vec;
    map->item_count = 0;

    // rehash and insert
    Size old_map_len = old_dmi_vec->length;
    for(Size s = 0; s < old_map_len; s++) {
        const Uint8 mdata = u8_vector_peek(old_mdata_vec, s);
        if(mdata & MDATA_OCCUPANCY_MASK) {
            /* insert directly without creating any more copies of data */
            /* this function will automatically create both metadata and probe sequence lengths */
            insert_into_dense_map_directly(map, dmi_vector_peek(old_dmi_vec, s), udata);
        }
    }

    // Cannot destroy map directly as this will destroy all previously created copies
    // All this trickery is to void calling copy constructor twice for same key-value pair.
    FREE(old_dmi_vec->data);
    old_dmi_vec->data = NULL;
    FREE(old_dmi_vec);
    u8_vector_destroy(old_mdata_vec, NULL);
}

/**
 * Insert given key-value pair into given @c DenseMap.
 * @param map
 * @param key Can be an integer value or pointer to some struct or memory.
 * @param value Value to be inserted into @c DenseMap.
 * This too can be an integer value or a pointer to some memory.
 * @param udata Pointer to user data provied to callbacks :
 * `hash`, `compare_key`, `create_data_copy`, `create_key_copy`, `destroy_data_copy`, `destroy_key_copy`
 * @return Pointer to new inserted @c DenseMapItem in @c DenseMap.
 * */
DenseMapItem* dense_map_insert(DenseMap* map, void* key, void* value, void* udata) {
    ERR_RETURN_VALUE_IF_FAIL(map, NULL, ERR_INVALID_ARGUMENTS);

    Float32 load_factor = ((Float32)map->item_count + 1)/((Float32)map->map->length);
    if(load_factor > map->max_load_factor) {
        dense_map_resize(map, map->map->length * 2, udata);
    }

    // set callback data and a temporary instance of DenseMapItem that we'll create copy of
    CallbackData clbk_data = {
        .udata = udata,
        .map   = map
    };
    DenseMapItem tmp_dmi = { .key = key, .data = value };

    // if not multimap, then first find any entry with same key
    if(!map->is_multimap) {
        // if an element with same key exists then insert this new data there by deleting old one.
        DenseMapItem* searched_dmi = dense_map_search(map, key, udata);
        if(searched_dmi) {
            // destroy whatever's present at the given place
            destroy_dmi_copy(searched_dmi, &clbk_data);
            // create a new copy at the searched item's slot
            create_dmi_copy(searched_dmi, &tmp_dmi, &clbk_data);
            // return the new created copy
            return searched_dmi;
        }
    }

    // If no key-value pair exists with same key ten
    // Create DenseMap's own copy of key-value pair
    DenseMapItem this_dmi;
    create_dmi_copy(&this_dmi, &tmp_dmi, &clbk_data);

    // next insert in such a way that no more copy constructors will be called.
    DenseMapItem* inserted_item = insert_into_dense_map_directly(map, &this_dmi, udata);
    if(!inserted_item) {
        destroy_dmi_copy(&this_dmi, &clbk_data);
        ERR(__FUNCTION__, ERRMSG(ERR_OPERATION_FAILED));
        return NULL;
    }

    return inserted_item;
}

/**
 * Search for element in @c DenseMap.
 * @param map
 * @param key
 * @param udata User data passed to callbacks : `hash`, `compare_key`
 * @return Pointer to first @c DenseMapItem* with equivalent or matching key, else @c NULL.
 * */
DenseMapItem* dense_map_search(DenseMap* map, void* key, void* udata) {
    ERR_RETURN_VALUE_IF_FAIL(map, NULL, ERR_INVALID_ARGUMENTS);

    Size len_wrap_mask = map->map->length - 1;
    Size hash = map->hash(key, udata);
    Size pos = hash & len_wrap_mask;
    Size iter = pos;
    Size iter_mdata = u8_vector_peek(map->metadata, iter);
    Uint8 this_mdata = MDATA_OCCUPANCY_MASK | (hash & MDATA_HASH_MASK);

    while((iter_mdata & MDATA_OCCUPANCY_MASK) && (((iter + 1) & len_wrap_mask) != pos)) {
        if(iter_mdata == this_mdata) {
            DenseMapItem* item = dmi_vector_peek(map->map, iter);
            if(map->compare_key(item->key, key, udata) == 0) {
                return item;
            }
        }

        iter = (iter + 1) & len_wrap_mask;
        iter_mdata = u8_vector_peek(map->metadata, iter);
    }

    return NULL;
}

/**
 * Delete all items with given key in @c DenseMap.
 * When behaviour is of multimap then all items with same key
 * are destroyed. When behaviour is otherwise then only the entry
 * with given key is destroyed, because there exists only one such
 * entry.
 * @param map
 * @param key
 * @param udata User data passed to callbacks : `hash`, `compare_key`,
 * `destroy_dmi_copy`.
 * */
void dense_map_delete(DenseMap* map, void* key, void* udata) {
    ERR_RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);

    CallbackData clbk_data = {
        .udata = udata,
        .map   = map
    };

    // search for item with given key
    DenseMapItem* item = dense_map_search(map, key, udata);
    destroy_dmi_copy(item, &clbk_data);
    map->item_count--;
    while(map->compare_key(++item->key, key, udata) == 0) {
        destroy_dmi_copy(item, &clbk_data);
        map->item_count--;
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
        DenseMap* hmap = clbk_data->map;                                \
        if(hmap->create_##n##_copy) {                                   \
            (d)->n = ALLOCATE(Uint8, hmap->n##_size);                   \
            ERR_RETURN_IF_FAIL((d)->n, ERR_OUT_OF_MEMORY);                  \
            hmap->create_##n##_copy((d)->n, (s)->n, clbk_data->udata);  \
        } else if(hmap->n##_size <= 8) {                                \
            (d)->n = (s)->n;                                            \
        } else  {                                                       \
            (d)->n = ALLOCATE(Uint8, hmap->n##_size);                   \
            ERR_RETURN_IF_FAIL((d)->n, ERR_OUT_OF_MEMORY);                  \
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
        DenseMap* hmap = clbk_data->map;                        \
        if(hmap->destroy_##n##_copy) {                          \
            hmap->destroy_##n##_copy(c->n, clbk_data->udata);   \
            FREE(c->n);                                         \
        } else if(hmap->n##_size > 8) {                         \
            FREE(c->n);                                         \
        }                                                       \
        c->n = 0;                                               \
    } while(0)

/**
 * Copy constructor for @c DenseMapItem array storing <key, value> pair of hash map.
 * @param dst Destination where @c DenseMapItem will be copied to
 * @param src Pointer where original copy of @c DenseMapItem relies.
 * @param clbk_data Callback data passed to copy constructor
 * */
void create_dmi_copy(DenseMapItem* dst, DenseMapItem* src, CallbackData* clbk_data){
    ERR_RETURN_IF_FAIL(dst && clbk_data, ERR_INVALID_ARGUMENTS);

    CREATE_COPY(dst, src, data);
    CREATE_COPY(dst, src, key);
}

/**
 * Destroy element copy from the void*
 * @param copy Pointer to DenseMapItem copy to be destroyed
 * @param clbk_data Callback data
 * */
void destroy_dmi_copy(DenseMapItem* copy, CallbackData* clbk_data) {
    ERR_RETURN_IF_FAIL(copy && clbk_data, ERR_INVALID_ARGUMENTS);

    DESTROY_COPY(copy, data);
    DESTROY_COPY(copy, key);
}

/**
 *                      GENNERAL IMPLEMENTATION INFO
 * The following implementation uses a modified version of Robin Hood hashing.
 * Robin Hood hashing works by maintaining an attribute for each item in the
 * @c DenseMap. This attribute measures how far the item is from it's desired
 * position in the map. This is known as probe length. In @c DenseMap, this is
 * stored in a vector named @c probe_len.
 *
 * Robin Hood hashing is a collision resolution technique within hash tables that aims
 * to minimize probe length, reducing clustering of elements with the same hash values.
 * While it attempts to optimize by favorably positioning items with identical hash values,
 * it does not ensure the optimal layout for all elements in the table. This
 * introduces another property of Robin Hood hashing, clustering. Due to clustering,
 * all items having same hash value will be found clustered in the whole map. All
 * such items will be found in vicinity of each other and there won't be any item
 * with a different hash value in bettween any two items with same hash value.
 * Here clusters are analogous to buckets in separate chaining.
 *
 *                                SPECIFICS
 * The modification in Robin Hood hashing is that now we maintain a new vector, containing
 * 8 bit values, with highest bit indicitating whether or not the entry is pre-occupied.
 * The lower 7 bits contains, lower 7 bits of hash value of corresponding item in the map.
 * By corresponding item, I mean, item at same index. This 8 bit value is called metadata
 * and is stored with same name in @c DenseMap.
 *
 * Now, the un-optimized implementation of insert operation does not use this whole metadata
 * value. It needs only the highest btt of metadata to know occupancy status at given posn.
 * This un-optimized version hence is the vanilla implementation of Robin Hood hashing.
 *
 * Some references
 * [0] : https://programming.guide/robin-hood-hashing.html (explains with animations)
 * [1] : https://jasonlue.github.io/algo/2019/08/20/clustered-hashing.html (lots of content)
 * [2] : https://youtu.be/M2fKMP47slQ?si=rTfRyId52pXqMfEQ (from where I got the idea to implement this)
 * */

/**< Defined this way to standardize the names of variables in macros that need same set of variables */

/**
 * Insert into the given @c DenseMap directly without triggering copy constructors.
 * @param map @c DenseMap to insert into.
 * @param dmi @c DenseMapItem to be directly inserted.
 * @param udata User data passed to callbacks : `hash`
 * @return Pointer to inserted @c DenseMapItem into the map. Note that inserted
 * @c DenseMapItem is a direct @c memcpy of provided @c DenseMapItem. The
 * returned pointer will be an address in the internal mapping vector maintained
 * by the given @c DenseMap.
 * */
static inline DenseMapItem* insert_into_dense_map_directly(DenseMap* map, DenseMapItem* dmi, void* udata) {
    ERR_RETURN_VALUE_IF_FAIL(map && dmi, NULL, ERR_INVALID_ARGUMENTS);

    DenseMapItem this_dmi = *dmi;
    Size hash = map->hash(dmi->key, udata);
    Size len_wrap_mask = map->map->length - 1;
    Size pos = hash & len_wrap_mask; /* it's guaranteed that size of densemap will always be in powers of two */
    Size iter = pos;
    Uint8 iter_psl = u8_vector_peek(map->probe_len, iter);
    Uint8 this_psl = 0;
    Uint8 this_mdata = MDATA_OCCUPANCY_MASK | (hash & MDATA_HASH_MASK);

    /* if total size of psl vector is less than maximum size of vector, use unoptimized search */
    while(True) {
        /* when this becomes poor as compared to iterated position, swap values */
        if(this_psl > iter_psl) {
            /* swap metadata before, to know the occupancy status. */
            Uint8 tmp_mdata = u8_vector_peek(map->metadata, iter);
            u8_vector_overwrite(map->metadata, iter, this_mdata, NULL);
                this_mdata = tmp_mdata;

                /* swap items */
                DenseMapItem* iter_dmi = dmi_vector_peek(map->map, iter);
                /* need to swap only if this position is already occupied */
                if(this_mdata & MDATA_OCCUPANCY_MASK) {
                    DenseMapItem tmp_dmi = *iter_dmi;
                    memcpy(iter_dmi, &this_dmi, sizeof(DenseMapItem));
                    this_dmi = tmp_dmi;
                } else {
                    /* if the position is not already occupied then we just do an overwrite. */
                    memcpy(iter_dmi, &this_dmi, sizeof(DenseMapItem));
                }

                /* swap psls' as well */
                Uint8 tmp_psl = u8_vector_peek(map->probe_len, iter);
                u8_vector_overwrite(map->probe_len, iter, this_psl, NULL);
                this_psl = tmp_psl;

                /* stop processing if the value we just swapped with was in an empty slot. */
                if(!(this_mdata & MDATA_OCCUPANCY_MASK)) {
                    break;
                }
        }

        /* psl of element not yet inserted in the map will keep increasing */
        this_psl++;

        /* increase iterator and fecth metadata as well as probe sequence length */
        iter = (iter + 1) & len_wrap_mask;
        iter_psl = u8_vector_peek(map->probe_len, iter);
    }

    // finally return the stored item
    map->item_count++;
    return dmi_vector_peek(map->map, iter);
}
