# Utility Modules

## Anvie [`Type`](Types.h) System :
All size types are defined with mindset of being used on 64-bit systems only. One may still use this to program 32-bit system software but I don't take guarantee of whether it will run as expected or not. The `typedef`s are platform independent. Most commonly used and fundamental types are `typedef`ed to give special names :  
- For unsigned integers, the name is of the form : `UintN`, where N is the number of bits. If number is 64 bits wide, then name of type is `Uint64`.  
- For signed integers, name is : `IntN`, again N is number of bits. eg : `Int8`, `Int16`, and so on...  
- For storing `Size`, there's `Size` which is 64-bit on all systems. Therefore when working with 32-bit systems, it's recommended to use `Size32` or `Size64` defines to make it more clearer.  
- Similarly, there are `typedef`s for `UintPtr`, `Address`, `Offset` etc... All these are `typedef`s of `Uint64` or `Uint32` or signed versions of these. One can use the length-suffixed versions of these `typedef`s as well : `Address32`, `Offset64` and so on... This way, there's no ambiguity like _"`size_t` is 32-bit on some platforms, and 64-bit on some platforms..."_  
- For zero-terminated strings, the type is named `ZString`, short for _"Zero String"_. For `String` objects, one can take a look inside [`Anvie/Containers/String`](Containers/String.h). So, wherever you see `String` don't confuse it with a null-terminated string, and don't take `ZString` as a completely new type, it's your friendly neighborhood `const char*`.  
- For floating point numbers, `float` is `typedef`ed to `Float32` and `double` is `typedef`ed to `Float64`, representing their respective sizes.  

## Type Specific `NULL` Defines

There are special defines of specific pointer type to denote `NULL` value for that pointer type. While you can use `NULL` everywhere, because it's shorter in writing, but to make code clear at some places, I've used `INVALID_<TYPE>` defines. For eg :  
- `INVALID_ZSTRING` is same as `((ZString)0)`  
- `INVALID_UINT32_PTR` is same as `((Uint32*)0)`  
  
At the time of writing, I don't know where exactly this will profit in the long run, but I think, at least, it'll be a more type safe way and readable way of `NULL` pointer comparisons.  

## [`Error`](Error.h) Handling
There's no single and definite way to handle errors in your program. Some use complex stack traces, some use exceptions and exception handlers.   

In Anvie, we use simple check and early exit. Whenever an argument is wrong, or anything inside a function goes wrong, the function will print a standard error message to indicate something wrong happened in that function and return something invalid or just issue a return (if function does not return anything).  

This is quite easy to implement, doesn't need extra setup and you just need to include [`Anvie/Error`](Error.h) to handle use this feature. One advantage is that if a stack of functions fail in succession, then you'll see a nice stack trace like thing-y.  

Multiple macro defines are present for error handling, but most used and basic ones are :  
``` c
RETURN_IF_FAIL(cond, ...); /* in the ..., you pass your format string and arguments to be printed as error message) */
RETURN_VALUE_IF_FAIL(cond, retval, ...); /* same as above */

ERR_RETURN_IF_FAIL(cond, errcode); /* this will print a standard error message, recommended over above alternatives */
ERR_RETURN_VALUE_IF_FAIL(cond, value, errcode); /* same as above */
```  

There are also other more generic defines to print debug and error messages based on severity :   
``` c
ERR(tag, ...);
DBG(tag, ...);
OK(tag, ...);

ASSERT(cond, ...); /* calls exit(1) if fails and issues an ERR(__FUNCTION__, ...) before that */

SUCCESS_IF(cond, ...); /* prints OK(__FUNCTION__, ...) if condition is true */
WARN_IF(cond, ...); /* same as above, but issues DBG(__FUNCTION__, ...) */
FATAL_IF(cond, ...); /* same as above, but issues ERR(__FUNCTION__, ...) */
```  

There's also one very special define that is not recommended to be used other than in test programs :  
``` c
GOTO_LABEL_IF_FAIL(cond, label, ...)
```  

This is used extensively in [Anvie/Test/UnitTest](Test/UnitTest.h) by wrapping this inside some other macros to ease up test case writing. There are usually only two cases in a test : _PASS_ or _FAIL_. This macro is used to handle these two cases by use of `DO_BEFORE_EXIT` macro define.  

## [`Helper Defines`](HelperDefines.h)
This header will define some commonly used helper macros like `MIN`, `MAX`, wrapper of `calloc` as `ALLOCATE`, `free` as `FREE` and for allocating a specific type : `NEW`, wrappers over `printf` as `println` and `print`. Better to just look inside it. It'll probably later be split and moved to some other header or module.  

**TODO** : I'll later write my own format printer, as a safer alternative to `printf` (probably).  

## [`Terminal Colors`](TerminalColors.)
Defines some commonly used terminal color macros on unix based terminals. These terminal colors are used when printing error messages to make them more readable. To use it in your program, just include it and use it the following way :   
``` c
println(COLOR_RED "Anvie Labs" COLOR_RESET); /* Anvie Labs will be printed in red color */
println(COLOR_GREEN "Version 0" COLOR_BLUE " Build 16" COLOR_RESET);
```
It's important to reset color, otherwise, it'll keep using last set color.  

## Documentation Index

**WIP** means Work In Progress and new code is being constantly pushed into these modules. **HALT** means the work inside these is almost negligible or completely halted, due to any reason you can think of.

- [Anvie/Allocators](Allocators) : A set of allocators for very selected use cases in Anvie Labs. (WIP)  
- [Anvie/Bit](Bit) : For easy bit manipulation and other hacks.  
- [Anvie/Chrono](Chrono) : Set of functions defined for time handling. Aims to be platform independent, but for the moment, works only on unix based systems (or only linux). Will extend it's features when I move to windows builds. (HALT)  
- [Anvie/Containers](Containers) : Collections, containers, data structures, whatever you wanna call them... (WIP)  
- [Anvie/Maths](Maths) : Set of maths containers (matrices, vectors) and other functions for easy maths computation. (HALT)  
- [Anvie/Simd](Simd) : Single Instruction Multiple Data. A library aimed for easy use of SIMD instructions inside Anvie Labs. Broken and not used at the moment, but kept just because I spent a lot of time experimenting with it and `Containers/DenseMap`. (HALT)  
- [Anvie/Test](Test) : Defines set macros for easy test creation.  
