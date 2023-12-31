
# [`Anvie/Containers/SparseMap`](../SparseMap.h)

## Purpose & Overview

`SparseMap` is a special implementation of [Hash Map](https://en.wikipedia.org/wiki/Hash_table) data structure where-in data (key-value pairs) are stored in sparse manner and uses [separate-chaining](https://en.wikipedia.org/wiki/Hash_table#Separate_chaining) for collision resolution. Opposite of this would be a `DenseMap` which stores data in a contiguous array and uses [open-addressing](https://en.wikipedia.org/wiki/Hash_table#Separate_chaining) for collision resolution. 

Both implementations have different use cases and must be selected judiciously. The API of both of these behave in a very similar manner, inner workings however is completely different.

## Key-Value Pairs
The key-value pairs are stored in a special struct named `SparseMapItem`. The generic definition of `SparseMapItem` looks like this :
``` c
struct SparseMapItem {
    void*          key;
    void*          data;
    SparseMapItem* next; /* point to next item in bucket */
};
```
When using interface-builders however, the `InType` of `key` and `data` members will be changed to whatever is provided in the interface builder. So, for eg : if user is using this to map a `Uint32` value to a `ZString`, then the sparse map will be defined for that like following :   

``` c
struct U32_ZStr_SparseMapItem {
    Uint32                  key;
    ZString                 data;
    U32_ZStr_SparseMapItem* next;
};
```

This item will be returned when you do search operations on the `SparseMap`.

## Available Interface Builders
There are total of four combinations of Key-Value pair `InType` classes because there are two options for each. Due to this, there are four interface-builders defined :  
| Key `InType` | Value `InType` | Interface Builder Name                                           |
|--------------|----------------|------------------------------------------------------------------|
| Integer      | Integer        | `DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY` |
| Integer      | Struct         | `DEF_INTEGER_STRUCT_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY`  |
| Struct       | Integer        | `DEF_STRUCT_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY`  |
| Struct       | Struct         | `DEF_STRUCT_STRUCT_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY`   |

And, if you don't want to use copy-constructor and destructor with these then there are four simpler defines wrapping these builders  :  

| Key `InType` | Value `InType` | Interface Builder Name                     |
|--------------|----------------|--------------------------------------------|
| Integer      | Integer        | `DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE` |
| Integer      | Struct         | `DEF_INTEGER_STRUCT_SPARSE_MAP_INTERFACE`  |
| Struct       | Integer        | `DEF_STRUCT_INTEGER_SPARSE_MAP_INTERFACE`  |
| Struct       | Struct         | `DEF_STRUCT_STRUCT_SPARSE_MAP_INTERFACE`   |

## Using Interface Builders
Before using interface builders, you need to decide the following things :
- What `api_prefix` to add to all base API functions? It's recommended to follow `<key>_<value>` prefix name style, because this is what is used for defining hash tables for commonly used `InType`s. Some examples are :
	- `u8_u8` for mapping `Uint8` to `Uint8`
	- `u64_zstr` for mapping `Uint64` to `ZString`
	- `zstr_zst` for mapping `ZString` to `ZString`
- What `typename` to use for the `key`-`value`  pair. You must decide these `typename`s  separately for both `InType`s.
- Whether or not your key and value `InType`s require a copy-constructor and destructor. This is very important because `SparseMap` will always try to make it's own copy of data.
- Whether your map will handle collisions (`is_multimap`)
- Maximum load factor (a floating point value)
- A hash function, that always returns `Size` as it's hashed value.
- A key compare function. Unlike C++'s partial ordering requirement, this one requires you to compare for equality as well. The comparison function returns negative if first key is less than second key, return 0 if they're equal and positive otherwise.

Below are a few examples of how this was used :  

``` c
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u8,  ZStr, U8,  hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint8 , NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u16, ZStr, U16, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint16, NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u32, ZStr, U32, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint32, NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_u64, ZStr, U64, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, Uint64, NULL,    NULL,     True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);

/*                                                             prefix    prefix     hash      ktype   kcreate  kdestroy  kcompare     dtype   dcreate             ddestroy             is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u8_zstr,  U8, ZStr,  hash_u8 , Uint8 , NULL,    NULL,     compare_u8 , ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u16_zstr, U16, ZStr, hash_u16, Uint16, NULL,    NULL,     compare_u16, ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u32_zstr, U32, ZStr, hash_u32, Uint32, NULL,    NULL,     compare_u32, ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(u64_zstr, U64, ZStr, hash_u64, Uint64, NULL,    NULL,     compare_u64, ZString, zstr_create_copy, zstr_destroy_copy, True,        SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);

/*                                                             prefix     prefix     hash       ktype    kcreate           kdestroy           kcompare      dtype    dcreate           ddestroy           is_multimap  max_load_factor */
DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE_WITH_COPY_AND_DESTROY(zstr_zstr, ZStr, ZStr, hash_zstr, ZString, zstr_create_copy, zstr_destroy_copy, compare_zstr, ZString, zstr_create_copy, zstr_destroy_copy, True, SPARSE_MAP_DEFAULT_LOAD_FACTOR_TOLERANCE);
```

## Typedefs

Each interface builder will create completely new and different `typedef` of `SparseMapItem` (sometimes used as `Smi` or `SMI`). This will separate this new `typedef` from other versions of it and from the generic version at least, hence adding to it's [type-safety](https://en.wikipedia.org/wiki/Type_safety).

So, if you're creating a `SparseMap` to store `ZString` and `Uint64` key-value pairs, and using interface builder `INTEGER_INTEGER`, then following things will be `typedef`ed :  

``` c
typedef struct ZStr_U64_SparseMapItem {
    ZString                 key;
    Uint64                  data;
    ZStr_U64_SparseMapItem* next;
} ZStr_U64_SparseMapItem;

typedef struct ZStr_U64_Smi_Vector {
    .  
    . /* vector things : callback, element size, capacity, etc... */
    .
    Size                    length;
    ZStr_U64_SparseMapItem* data; /* data array storing type-specific contents */
} ZStr_U64_Smi_Vector;

typedef struct ZStr_U64_SparseMap {
		.
		. /* sparse map things : callbacks, element sizes, etc... */
		.
		ZStr_U64_Smi_Vector map; /* map containing key-value pairs */
} ZStr_U64_SparseMap;
```

Besides these, type-specific API wrapper functions will also be defined. These wrappers will take the prefix and a base name for sparse map, but for the type-specific `SparseMapItem` `Vector`, they'll have slightly different names, as they would add a `_smi` at the end of this prefix and then use it as prefix to issue the interface builder for `Vector`.  So, vector API names would look like `zstr_u64_smi_vector_create(...)` and so on... and for `SparseMap`, it'll look like `zstr_u64_sparse_map_create(...)`.

The function names might be getting long, but I like them this way because they keep reminding me about the type of hash table I'm working with. Whenever I see an object being used with one of these functions, I'll instantly know it's type and I won't have to search it's definition.

## Callbacks

Callbacks behave much similar to that in [`Anvie/Container/Vector`](https://github.com/AnvieLabs/AnvieUtils/blob/master/Include/Anvie/Containers/Docs/Vector.md#callbacks). I don't like repeating myself, so just read it from there.

## Full List of Base API Function Names
- `sparse_map_create(hash, key_size, create_key_copy, destroy_key_copy, compare_key, data_size, create_data_copy, destroy_data_copy, is_multimap, max_load_factor)`
- `sparse_map_destroy(map, udata)`
- `sparse_map_resize(map, size, udata)`
- `sparse_map_insert(map, key, value, udata)`
- `sparse_map_search(map, key, udata)`
- `sparse_map_delete(map, key, udata)`

<p align="center" style="font-size: small; line-height: 1.2;">
    Author: Siddharth Mishra<br>
    Writing Date: 1st January, 2024<br>
    Last Modified: 1st January, 2024<br>
    License: Apache 2.0 License<br> <br>
    Copyright (c) 2023 AnvieLabs, Siddharth Mishra
</p>
