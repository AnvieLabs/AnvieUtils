/**
 * @file Types.h
 * @date Fri, 8th December, 2023
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
 * @brief Defines SIMD (Single Instruction Multiple Data) types for easy
 * and direct use in SIMD wrapper functions.
 * */

#ifndef ANVIE_SIMD_TYPES_H
#define ANVIE_SIMD_TYPES_H

#include <emmintrin.h>
#include <immintrin.h>

/*
** Define basic type names first. Writing __mmXXX or __mmaskXX
** is too much of work.
 */

#ifdef __AVX__
    typedef __m128i MVec16;   /**< SIMD Vector register to store 16 bytes of data. */
    typedef __mmask16 MMask16; /**< Equivalent to Uint16. Returned as result or passed as argument to mask operations. */
#   define SIMD_LVL1 1
#   pragma message "AVX EXTENSIONS ENABLED"
#else
#   define SIMD_LVL1 0
#endif // __AVX__

#ifdef __AVX2__
    typedef __m256i MVec32;   /**< SIMD Vector register to store 32 bytes of data. */
    typedef __mmask32 MMask32; /**< Equivalent to Uint32. Returned as result or passed as argument to mask operations. */
#   define SIMD_LVL2 1
#   pragma message "AVX2 EXTENSIONS ENABLED"
#else
#   define SIMD_LVL2 0
#endif // __AVX2__

#if defined(__AVX512BW__) && defined(__AVX512DQ__) && defined(__AVX512VL__) && defined(__AVX512F__)
    typedef __m512i MVec64;   /**< SIMD Vector register to store 64 bytes of data. */
    typedef __mmask64 MMask64; /**< Equivalent to Uint64. Returned as result or passed as argument to mask operations. */
#   define SIMD_LVL3 1
#   pragma message "AVX512 EXTENSIONS ENABLED"
#else
#   define SIMD_LVL3 0
#endif // __AVX512__

/*
** Define standard name of vector registers. Below code will
** select the maximum possible size of register.
*/
#if SIMD_LVL3
    typedef MVec64 MVec;
    typedef MMask64 MMask;
#elif SIMD_LVL2
    typedef MVec32 MVec;
    typedef MMask32 MMask;
#elif SIMD_LVL1
    typedef MVec16 MVec;
    typedef MMask16 MMask;
#endif // SIMD_LVL1

/*
** Define a single macro to inform whether or not SIMD is enabled at all!
** This single macro is to replace checking for each level one by one again and again.
 */
#if SIMD_LVL1 || SIMD_LVL2 || SIMD_LVL3
#   define SIMD_ENABLED 1
#   define SIMD_VECTOR_REGISTER_SIZE sizeof(MVec)
#   pragma message "SIMD IS ENABLED"
#else
#   define SIMD_ENABLED 0
#   define SIMD_VECTOR_REGISTER_SIZE 0
#   pragma message "SIMD IS NOT ENABLED"
#endif // SIMD LEVELS


#endif // ANVIE_SIMD_TYPES_H
