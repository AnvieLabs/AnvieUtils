# [`Anvie/Containers/Vector`](../Vector.h)

## Purpose & Overview

A `Vector` is a generic container in AnvieUtils to store elements of same `InType` in a contiguous manner. There are two types of `Vector` container depending on the class of `InType` :
- Integer Vectors (`Int8`, `Int16`, `Int32`, `Int64`, `Uint8`, `Uint16`, `Uint32`, `Uint64`, `ZString`, `Float32`, `Float64`, `void*`, or any pointer type)
- Struct Vectors (all user defined `struct`s)

## Available Interface Builders
[`Anvie/Containers/Interface/Vector`](../Interface/Vector.h) defines three interface builders for three different use cases :
- `DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY` : to build interface for integer types that require copy-constructor and destructor.
- `DEF_INTEGER_VECTOR_INTERFACE` : to build interface for integer types that don't require a pair of copy-constructor and destructor.
- `DEF_STRUCT_VECTOR_INTERFACE` : to build interface for user-defined structure types.

Using these three interface builders you can theoretically build all type specific vector containers you can imagine about (at least almost all ~ 99.9999...%).

## Using Interface Builders
To use these interface builders, you need to decide the following things beforehand :
- The `InType`.
- What `api_prefix` will be appended to each of the generic API functions? eg : `u64` is used for vector of `Uint64`.
- What `typename` will be used to append to `Vector` object type? eg : `Str` is used for storing `String` objects. The resulting type name is `Str_Vector`.
- Whether you need copy-constructors and copy-destructors for your `InType`?

After deciding all these, you can directly pass them as arguments to the interface builders. Below are some examples of how they're used to define some commonly used `Vector` containers.  

```c
DEF_INTEGER_VECTOR_INTERFACE(u8,  U8,  Uint8);
... and other types

DEF_INTEGER_VECTOR_INTERFACE(i8,  I8, Int8);
... and other types

DEF_INTEGER_VECTOR_INTERFACE(f32, F32, Float32);
DEF_INTEGER_VECTOR_INTERFACE(f64, F64, Float64);

DEF_INTEGER_VECTOR_INTERFACE_WITH_COPY_AND_DESTROY(zstr, ZStr_, ZString, zstr_create_copy, zstr_destroy_copy);
DEF_INTEGER_VECTOR_INTERFACE(vptr, VPtr, void*);

// define interface to contain vector of vectors
void vector_create_copy(Vector* dst, Vector* src, void* udata);
void vector_destroy_copy(Vector* copy, void* udata);
DEF_STRUCT_VECTOR_INTERFACE(vector, Vec, Vector, vector_create_copy, vector_destroy_copy);
DEF_INTEGER_VECTOR_INTERFACE(pvec, PVec, Vector*);
```
## Typedefs

The interface builders will create a set of new `typedef`s every time they're used. This will include create a new `Vector` type, and creating associated callback function types. This makes sure that you cannot use two vectors with different `InType`s with same wrapper function. The size of every new `typedef` however remains exactly same. This allows certain level of polymorphism to a vector type.

Now, it's very important that when defining callback functions, the argument sizes are exactly same as that of base type. Almost every time, each argument in a callback function will be 64-bits wide. Not following this convention might work sometimes and might fail sometimes. At the time of writing this, I've tested with smaller sized argument and it worked perfectly, but this is not a tested result.

## Callbacks
### Copy Constructor & Destructor
Generic `typedef` for copy-constructor is 
``` c
typedef void (*CreateElementCopyCallback)(void* dst, void* src, void* udata);
```
The first argument `dst` will be pointer to memory where data is to be stored. This will be pre-allocated by the `Vector` container and then passed down to this copy-constructor. This means the copy-constructor must not attempt to allocate memory for `InType` and that it must just assume the memory is allocated if `dst != NULL`.  

The second argument `src` is the data that needs to be copied. This is the copy provided to the `Vector` and now it will attempt to create a copy of this `src` for itself. This is very important when a struct stores pointers to memory areas like strings, that if not free-d after use then might cause problem in longer run.  

The third argument `udata` is self-explanatory. All callbacks take a `udata`  

Similarly, generic `typedef` for copy-destructor is  :  

``` c
typedef void (*DestroyElementCopyCallback)(void* copy, void* udata);
```

Again, the first argument `copy` is pointer to memory where data is stored, and is not the data itself. Second argument again is self-explanatory.

### All Other Callbacks

All other callbacks differ from the above two in the following ways :
- Even though the first argument in their `typedef`s are `void*`, the value in this `void*` depends on which type of vector is calling it. 
	- If an integer vector is issuing this callback then the `void*` is not a pointer, but the value/data itself.
	- If a struct vector is issuing this callback, then the `void*` is pointer to where the data exists.

The programmer however mustn't worry about these things. You can always use your own type instead of `void*`, just make sure that you try to take 64-bit wide arguments and don't run too much after [code sugar](https://en.wikipedia.org/wiki/Syntactic_sugar). The type of callback can be any compatible function, because when using interface-builders, all your type-specific callbacks will be automatically casted to these generic ones. Just take a reference from above usage examples of interface-builders.

## Full List of Base API Function Names
Following are base function names of generic `Vector` container. You add a prefix to these base names and they become type specific.

- `vector_at(vec, type, pos)`: Access element at a specific position.
- `vector_address_at(vec, pos)`: Get the memory address of an element at a specific position.
- `vector_length(vec)`: Get the length of the vector.
- `vector_element_size(vec)`: Get the size of each element in the vector.
- `vector_create(element_size, create_copy, destroy_copy)`: Create a new vector.
- `vector_destroy(vec, udata)`: Destroy the vector and its elements.
- `vector_clone(vec, udata)`: Create a copy of the vector.
- `vector_resize(vec, new_size)`: Resize the vector to a new size.
- `vector_reserve(vec, capacity)`: Reserve memory for a certain capacity.
- `vector_clear(vec, udata)`: Clear the vector.
- `vector_get_subvector(vec, start, size, udata)`: Get a subvector from the vector.
- `vector_copy(vec, to, from, udata)`: Copy elements from one position to another.
- `vector_move(vec, to, from, udata)`: Move elements from one position to another.
- `vector_overwrite(vec, pos, data, udata)`: Overwrite an element at a specific position.
- `vector_insert(vec, data, pos, udata)`: Insert an element at a specific position. Element ordering preserved.
- `vector_insert_fast(vec, data, pos, udata)`: Fast insertion at a specific position. Element ordering not preserved.
- `vector_delete(vec, pos, udata)`: Delete an element at a specific position. Element ordering preserved.
- `vector_delete_fast(vec, pos, udata)`: Fast deletion at a specific position. Element ordering not preserved.
- `vector_remove(vec, pos)`: Remove and return an element at a specific position. Element ordering preserved.
- `vector_remove_fast(vec, pos)`: Fast removal of an element at a specific position. Element ordering not preserved.
- `vector_push_front(vec, data, udata)`: Push an element to the front of the vector. Element ordering preserved.
- `vector_push_front_fast(vec, data, udata)`: Fast push to the front of the vector. Element ordering not preserved.
- `vector_pop_front(vec)`: Pop an element from the front of the vector. Element ordering preserved.
- `vector_pop_front_fast(vec)`: Fast pop from the front of the vector. Element ordering not preserved
- `vector_push_back(vec, data, udata)`: Push an element to the back of the vector.
- `vector_pop_back(vec)`: Pop an element from the back of the vector.
- `vector_peek(vec, pos)`: Peek at an element at a specific position.
- `vector_front(vec)`: Get the element at the front of the vector.
- `vector_back(vec)`: Get the element at the back of the vector.
- `vector_print(vec, printer, udata)`: Print the elements of the vector.
- `vector_merge(vec, other, udata)`: Merge two vectors.
- `vector_filter(vec, filter, udata)`: Filter elements based on a condition.
- `vector_swap(vec, p1, p2)`: Swap elements at two positions.
- `vector_sort(vec, compare, udata)`: Sort the vector.
- `vector_check_sorted(vec, compare, udata)`: Check if the vector is sorted.

<p align="center" style="font-size: small; line-height: 1.2;">
    Author: Siddharth Mishra<br>
    Writing Date: 31st December, 2023<br>
    Last Modified: 1st January, 2024<br>
    License: Apache 2.0 License<br> <br>
    Copyright (c) 2023 AnvieLabs, Siddharth Mishra
</p>
