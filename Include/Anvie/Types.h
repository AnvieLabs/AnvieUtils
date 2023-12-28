/**
 * @file AvTypes.h
 * @date Wed, 5th July, 2023
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
 * @brief Common typedefs for AnvieUtils
 * */

#ifndef AVTYPES_H_
#define AVTYPES_H_

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char Uint8;
typedef unsigned short Uint16;
typedef unsigned int Uint32;
typedef unsigned long Uint64;

#define UINT8_MAX (0xFF)
#define UINT16_MAX (0xFFFF)
#define UINT32_MAX (0xFFFFFFFF)
#define UINT64_MAX (0xFFFFFFFFFFFFFFFF)

typedef char Int8;
typedef short Int16;
typedef int Int32;
typedef long Int64;

#define INT8_MIN (-128)
#define INT8_MAX (127)
#define INT16_MIN (-32768)
#define INT16_MAX (32767)
#define INT32_MIN (-2147483648)
#define INT32_MAX (2147483647)
#define INT64_MIN (-9223372036854775808LL)
#define INT64_MAX (9223372036854775807LL)

typedef Int64 PtrDiff;
typedef Uint64 UintPtr;
typedef Uint64 Size;
typedef Uint64 Offset;
typedef Uint64 Address;

#define ADDRESS_MAX (~(UintPtr)0)
#define OFFSET_MAX (~(UintPtr)0)
#define PTR_DIFF_MAX ((PtrDiff)(((size_t)1 << (sizeof(PtrDiff) * 8 - 1)) - 1))
#define PTR_DIFF_MIN ((PtrDiff)(-(ptrdiff_t)(((size_t)1 << (sizeof(PtrDiff) * 8 - 1)) - 1) - 1))
#define SIZE_MAX (~(Size)0)

typedef float Float32;
typedef double Float64;

#define FLOAT32_MAX   3.40282347e+38f
#define FLOAT32_MIN   1.17549435e-38f
#define FLOAT64_MAX   1.7976931348623157e+308
#define FLOAT64_MIN   2.2250738585072014e-308

typedef char Char;
typedef Uint8 Byte;
typedef const char* ZString; /**< Zero terminated string */
typedef char* ByteArray;
typedef Uint8* UByteArray;

typedef void*   VoidPtr;
typedef Uint8*  Uint8Ptr;
typedef Uint16* Uint16Ptr;
typedef Uint32* Uint32Ptr;
typedef Uint64* Uint64Ptr;
typedef Int8*   Int8Ptr;
typedef Int16*  Int16Ptr;
typedef Int32*  Int32Ptr;
typedef Int64*  Int64Ptr;
typedef Float32* Float32Ptr;
typedef Float64* Float64Ptr;
typedef Size*   SizePtr;

#define INVALID_ZSTRING     ((ZString)0)
#define INVALID_BYTE_ARRAY  ((ByteArray)0)
#define INVALID_UBYTE_ARRAY ((UByteArray)0)
#define INVALID_VOID_PTR    ((VoidPtr)0)
#define INVALID_UINT8_PTR   ((Uint8Ptr)0)
#define INVALID_UINT16_PTR  ((Uint16Ptr)0)
#define INVALID_UINT32_PTR  ((Uint32Ptr)0)
#define INVALID_UINT64_PTR  ((Uint64Ptr)0)
#define INVALID_INT8_PTR    ((Int8Ptr)0)
#define INVALID_INT16_PTR   ((Int16Ptr)0)
#define INVALID_INT32_PTR   ((Int32Ptr)0)
#define INVALID_INT64_PTR   ((Int64Ptr)0)
#define INVALID_FLOAT32_PTR ((Float32Ptr)0)
#define INVALID_FLOAT64_PTR ((Float64Ptr)0)
#define INVALID_SIZE_PTR    ((SizePtr)0)

typedef Int8 Bool;
#define False 0
#define True 1

#define TO_UINT8(v)  ((Uint8)TO_UINT64(v))
#define TO_UINT16(v) ((Uint16)TO_UINT64(v))
#define TO_UINT32(v) ((Uint32)TO_UINT64(v))
#define TO_UINT64(v) ((Uint64)(v))
#define TO_INT8(v)   ((Int8)TO_INT64(v))
#define TO_INT16(v)  ((Int16)TO_INT64(v))
#define TO_INT32(v)  ((Int32)TO_INT64(v))
#define TO_INT64(v)  ((Int64)(v))

#define TO_SIZE(v)    ((Size)TO_UINT64(v))
#define TO_ADDRESS(v) ((Address)(TO_UINT64(v) & 0x7fffffffffff))
#define TO_VOIDPTR(v) ((void*)TO_UINT64(v))
#define TO_ZSTRING(v) ((ZString)(uintptr_t)(v))

#define TO_UINT8_PTR(v)  ((Uint8*)TO_ADDRESS(v))
#define TO_UINT16_PTR(v) ((Uint16*)TO_ADDRESS(v))
#define TO_UINT32_PTR(v) ((Uint32*)TO_ADDRESS(v))
#define TO_UINT64_PTR(v) ((Uint64*)TO_ADDRESS(v))
#define TO_INT8_PTR(v)   ((Int8*)TO_ADDRESS(v))
#define TO_INT16_PTR(v)  ((Int16*)TO_ADDRESS(v))
#define TO_INT32_PTR(v)  ((Int32*)TO_ADDRESS(v))
#define TO_INT64_PTR(v)  ((Int64*)TO_ADDRESS(v))
#define TO_SIZE_PTR(v)   ((Size*)TO_ADDRESS(v))

#endif // AVTYPES_H_
