# Anvie Utils

<div style="text-align:center;">
    <img src="https://raw.githubusercontent.com/AnvieLabs/.github/master/profile/logo.png" alt="AnvieLabs" style="width: 100px; height: 100px;">
</div>

AnvieUtils is in early stages of development. The end goal is to have a good level of replacement library for those having an STL (C++) background, as well as new users. AnvieUtils aims to be platform independent library in the long run, so that it can even be used when starting new Operating System projects. The goal is to have a wide range of containers, allocators, RNGs, etc... (anything that feels like a utility).

## Add To Your Project
Anvie Utils can be quickly added as a sub-module in your project.  

``` bash
mkdir Modules
git submodule add git@github.com:AnvieLabs/AnvieUtils.git Modules/Utils
```  

If you're using `CMake` for your build system, then just include this as a sub-directory in your root `CMakeLists.txt` file.   

``` cmake
add_subdirectory(Modules/Utils)
```

Now, to include headers from Anvie Utils, u just need to link the interface library `anvutils_headers` in your make targets :

``` cmake
add_subdirectory(Modules/Utils)

# create your target, and then link using following command
target_link_libraries(<target_name> anvutils_headers ... other libs )

# or you can do a global include 
link_libraries(anvutils_headers)
```

This will make all headers from Anvie Utils available to the target the headers interface is linked to.

## Linking & Compiling
For linking different parts of Anvie Utils, you can either perform a global include :  

``` cmake
link_libraries(anvutils_containers anvutils_allocators ...)
```  

or you can do a target specific include :

``` cmake
target_link_libraries(<target_name> anvutils_containers anvutils_maths ...)
```

## List of Libraries In Anvie Utils

Instead of linking to Anvie Utils as a single monolithic library, you can link to it in parts :
- `anvutils_containers` : Link to all containers
- `anvutils_allocators` : Link to allocators
- `anvutils_maths` : Link to maths containers and functions.

This model makes sure you only pay for what you use (kind-of). It might be possible that linking to one of these libraries, may actually link to more than one of Anvie Utils libraries. One such example is linking to `anvutils_allocators`. This will also create a link for `anvutils_containers`.

## Documentation Index

Following is a list of places inside the source code where you'll find more documentation of different utilities/libraries inside Anvie Utils.

- [`Anvie/Allocators`](Include/Anvie/Allocators) : Dedicated allocators for specific use cases.
- [`Anvie/Bit`](Include/Anvie/Bit) : Bit manipulation utilities.
- [`Anvie/Chrono`](Include/Anvie/Chrono) : Time computation utilities.
- [`Anvie/Containers`](Include/Anvie/Containers) : Containers like vectors, hash maps, trees, lists, etc...
- [`Anvie/Maths`](Include/Anvie/Maths) : Maths utility libraries.
-  `Anvie/Simd` : Not ready for use at the moment.
- [`Anvie/Test`](Include/Anvie/Test) : Test creation helpers.

## Current Support

| Utility Name        | Has Test | Actively in use in AnvieLabs | Has been used |
|---------------------|----------|------------------------------|---------------|
| Container/Vector    | Yes      | Yes                          | Yes           |
| Container/Tree      | No       | No                           | No            |
| Container/String    | No       | No                           | No            |
| Maths/Vector2f      | No       | Yes                          | Yes           |
| Maths/Vector3f      | No       | Yes                          | Yes           |
| Maths/Vector4f      | No       | Yes                          | Yes           |
| Maths/Matrix4f      | No       | Yes                          | Yes           |
| Container/DenseMap  | No       | No                           | No            |
| Container/SparseMap | No       | No                           | No            |
| Container/BitVector | Yes      | No                           | No            |

## TODO

- [ ] Stack
- [ ] Linked List
- [ ] Graphs
- [ ] Set
- [ ] RangeMap
- [ ] Allocators(Heap, BitMap, etc...)

## License

Copyright 2023 AnvieLabs

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
