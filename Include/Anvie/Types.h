/**
 * @file AvTypes.h
 * @date Wed, 5th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Common typedefs for AnvieUtils
 * */


#ifndef AVTYPES_H_
#define AVTYPES_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef uint8_t Uint8;
typedef uint16_t Uint16;
typedef uint32_t Uint32;
typedef uint64_t Uint64;

typedef int8_t Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef size_t Size;

typedef float Float32;
typedef double Float64;

typedef char Char;
typedef Uint8 Byte;
typedef const char* String;
typedef char* ByteArray;
typedef Uint8* UByteArray;

typedef Int8 Bool;
#define False 0
#define True 1

#endif // AVTYPES_H_
