#include <Anvie/Containers/HashMap.h>
#include <Anvie/HelperDefines.h>

/**
 * Create a new hash map.
 * @param hash Hash function to be used.
 * @param element_size Size of each element in hash map.
 * @param create_copy Copy constructor for creating hash map of of given values.
 * @param destroy_copy Copy destructor for creating hash map of of given values.
 * @return HashMap object on success, NULL otherwise.
 * */
HashMap* hash_map_create(
    HashCallback hash,
    Size element_size,
    CreateElementCopyCallback create_copy,
    DestroyElementCopyCallback destroy_copy
) {
    RETURN_VALUE_IF_FAIL(hash && element_size, NULL, ERR_INVALID_ARGUMENTS);

    Vector* vec = vector_create(element_size, create_copy, destroy_copy);
    RETURN_VALUE_IF_FAIL(vec, NULL, "Failed to create vector to store elements of hash map");

    HashMap* map = NEW(HashMap);
    map->elements = vec;
    map->hash     = hash;

    return map;
}

/**
 * Destroy hash map.
 * @param map HashMap object to be destroyed.
 * @param udata User data passed to callback functions.
 * This will be passed to copy destructor for each data copy
 * of this @c HashMap.
 * */
void hash_map_destroy(HashMap* map, void* udata) {
    RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);

    if(map->elements) {
        vector_destroy(map->elements, udata);
        map->elements = NULL;
    }
    FREE(map);
}

/**
 * Insert given key-value pair into given @c HashMap.
 * @param map
 * @param key Can be an integer value or pointer to some struct or memory.
 * @param value Value to be inserted into @c HashMap.
 * This too can be an integer value or a pointer to some memory.
 * @param udata Pointer to user data provied to callbacks.
 * This can be used to provide extra information to stages such as
 * @c HashCallback or @c Create/DestroyElementCopyCallback etc...
 * */
void hash_map_insert(HashMap* map, void* key, void* value, void* udata) {
    RETURN_IF_FAIL(map, ERR_INVALID_ARGUMENTS);

    Size pos = map->hash(key, map->elements->length, udata);
    vector_insert(map->elements, value, pos, udata);
}
