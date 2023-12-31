# [`Anvie/Containers`](.)

Containers provide generic implementations for some commonly used data structures. These include, but are not limited to, `Vector`, `HashMap`, `Tree`, `BitVector`, `List`, etc...

All the generic containers are generic inherently. They all use `void*` throughout. Due to this, direct usage of these containers through the raw API provided for interacting with them is not easy, and is error prone. To solve this, each generic container has a way to define a custom type-specific interface for interacting with it. It's highly recommended to use the containers through these interfaces.

## Interfaces

All interfaces are defined in [`Anvie/Containers/Interface`](Interface). Each interface header will define some interface-builder macros, that when used, will instantly define a set of wrapper functions that are instructed to be forcefully inline throughout the program. The general form for these defines are `DEF_<CONTAINER_TYPE>_INTERFACE`, eg : `DEF_INTEGER_VECTOR_INTERFACE`, `DEF_INTEGER_STRUCT_DENSE_MAP_INTERFACE`, `DEF_INTEGER_INTEGER_SPARSE_MAP_INTERFACE`, etc...

This is very close to using templates in C++, just that templates are not polluted everywhere with this implementation. This is not a replacement for STL in any way and just a way to have ready-to-use generic containers in your C projects.

<div style="background-color: #ffcc66; padding: 10px; border-radius: 5px; box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);">
    <h3 style="font-weight: bold;">Note</h3>
    <p>
    Each container will store data in one or more than one data-types. Throughout the document, I'll use the term <code>InType</code> to refer to this internal data representation. If multiple data-types are used in the container, then this <code>InType</code> will be indexed like <code>InType1</code>, <code>InType2</code>, and so on...
    </p>

</div>

## Using Interface Builders

The usage of these interface builder macros is almost always in the following way :
- If `InType` is a user defined structure (`struct`) then use `DEF_STRUCT_XYZ_INTERFACE`. Exact definition must be present, just `typedef` won't work because the interface-builder needs to know the exact size of this structure. If this is not possible, then you can define your own interface by modifying the interface builder to ask for data type size explicitly. This is not recommended however because it may cause unmanageable code in the long run.
- If the `InType` is a pointer type (`String*`, `ZString`, `const char*`, etc...), or an integer type (`Int32`, `Uint8`, etc...) then use `DEF_INTEGER_XYZ_INTERFACE`
- If the `InType` requires a `copy-constructor` or a `copy-destructor` to create or destroy copies of the structures or pointers, like in case of C-style null-terminated strings, or structures that contain pointers that behave like c-style strings, and other similar cases, then you need to provide the `copy-constructor` and `copy-destructor` when issuing the interface-builder macro.
- Either you provide both `copy-constructor` and `copy-destructor`, at the same time, or you don't provide any at all. Providing just one will cause an error and you won't be able  to create that container.
- When `copy-constructor` or `copy-destructor` is issued, you'll get a pointer to memory where the object/data is stored. You must **NOT** attempt to destroy this memory. It's not the job of these callbacks to attempt to allocate memory for `InType`. If `InType` is a pointer, then you only need to allocate space for the object/array/string, **NOT** the pointer itself. If `InType` is an object, then you only need to store data and construct other members in the data, **DON'T** allocate memory for the `InType` itself!

## Callbacks

Every container uses callbacks to implement some features. There are callback `typedef`s for printing elements, filtering them, sorting them, constructing and destructing elements, etc...
  
All of these callback types are defined in [`Anvie/Containers/Common.h`](Common.h). Each of these callback take a user-data of type `void*`. The actual callback implementations don't exactly need to fall these types as long as the size and number of arguments remain same. The callback function provided by user to interface-builders will be type-casted to the type of callback it was passed for. Note that this may cause problem if not used correctly and carefully.   

The passed user-data can be used for constructing/destructing/filtering/etc... elements inside the container. A real life example of this is when using containers for storing `VkDeviceBuffer` objects when working with a [Vulkan](https://vulkan.org/) application. `VkDevice` handle is required for destruction of these device handles. It is this single requirement that forced me to add this user-data argument to all callback functions.

## Understanding API Naming Conventions

Each container has a set of functions to interact with the container. It's important to understand how these functions are named in order to be able to guess names easily on the spot.  
- To create and destroy container objects, functions are named `<base_name>_create`, and `<base_name>_destroy`, where `base_name` is replaced with `vector`, `densemap`, `sparsemap`, `bitvec`, `str`, etc... One may imagine this to be similar to `Vector::create` or `SparseMap.create` styled namings.
- To perform an operation like `insert`, `delete`, `remove`, `sort`, `filter`, `accumulate`, etc..., you just append these names to the base names like : `vector_insert`, `vector_insert_fast`, `vector_push_back`, `vector_push_front_fast`, and so on...
- There are two types of operations : `fast` and normal ones. Take example of `insert` operation above. The fast operations differ from their vanilla versions by not preserving the order of elements inside container. If user needs the elements to stay in a specific order, it's highly recommended to use the non-fast versions. If user want's a quick way to perform the operation, and doesn't care about the order of elements, then it's highlt recommended to fast versions.

That's it! Function naming is not rocket science here, just like any other easy to use language/library.

## Memory Management & Ownership
Each container will try as much as possible to maintain their own copy of inserted data. Once data is deleted, or cleared, the vector will attempt to destroy the copy of data it has. In this attempt, if user is not careful with copy construction and destruction, the container is very likely to attempt to destroy something that will be used in future in program. For this it's very important to use the interface-builders and provide copy-constructors and destructors whenever using structures of any size.

## Documentation Index

- [Vector](Docs/Vector.md)
- [DenseMap](Docs/DenseMap.md)
- [SparseMap](Docs/SparseMap.md)
- [String](Docs/String.md)
- [Tree](Docs/Tree.md)
- [BitVector](Docs/BitVector.md)

---

<p align="center" style="font-size: small; line-height: 1.2;">
    Author: Siddharth Mishra<br>
    Writing Date: December 30th, 2023<br>
    Last Modified: December 30th, 2023<br>
    License: Apache 2.0 License<br> <br>
    Copyright (c) 2023 AnvieLabs, Siddharth Mishra
</p>
