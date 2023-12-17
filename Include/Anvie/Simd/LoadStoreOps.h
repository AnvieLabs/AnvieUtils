/**
 * @file LoadStoreOps.h
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
 * @brief Declares and Defines wrappers around Load/Store intrinsics.
 * */

#ifndef ANVIE_SIMD_LOAD_STORE_OPERATIONS_H
#define ANVIE_SIMD_LOAD_STORE_OPERATIONS_H

#include <Anvie/Types.h>
#include <Anvie/Simd/Types.h>

#if SIMD_ENABLED

    static inline MVec simd_set1_epi8(Int8 a);
    static inline MVec simd_set1_epi16(Int16 a);
    static inline MVec simd_set1_epi32(Int32 a);
    static inline MVec simd_set1_epi64(Int64 a);

#   if SIMD_LVL1
        static inline MVec16 simd_loadu_si16(const void* m);
        static inline MVec16 simd_loadu_si32(const void* m);
        static inline MVec16 simd_loadu_si64(const void* m);
        static inline MVec16 simd_loadu_si128(const void* m);
#   endif // SIMD_LVL1

#   if SIMD_LVL2
        static inline MVec32 simd_loadu_si256(const void* m);
#   endif // SIMD_LVL2

#   if SIMD_LVL3
        static inline MVec simd_loadu_si512(const void* m);

        static inline MVec16 simd_load128_epi8(const void* m);
        static inline MVec16 simd_load128_epi16(const void* m);
        static inline MVec16 simd_load128_epi32(const void* m);
        static inline MVec16 simd_load128_epi64(const void* m);

        static inline MVec32 simd_load256_epi8(const void* m);
        static inline MVec32 simd_load256_epi16(const void* m);
        static inline MVec32 simd_load256_epi32(const void* m);
        static inline MVec32 simd_load256_epi64(const void* m);

        // when SIMD_LVL3 is defined, MVec = MVec64
        static inline MVec simd_load512_epi8(const void* m);
        static inline MVec simd_load512_epi16(const void* m);
        static inline MVec simd_load512_epi32(const void* m);
        static inline MVec simd_load512_epi64(const void* m);

        static inline MVec simd_loadu_epi8(const void* m);
        static inline MVec simd_loadu_epi16(const void* m);
        static inline MVec simd_loadu_epi32(const void* m);
        static inline MVec simd_loadu_epi64(const void* m);

        static inline MVec simd256_loadu_epi8(const void* m);
        static inline MVec simd256_loadu_epi16(const void* m);
        static inline MVec simd256_loadu_epi32(const void* m);
        static inline MVec simd256_loadu_epi64(const void* m);

        static inline MVec simd128_loadu_epi8(const void* m);
        static inline MVec simd128_loadu_epi16(const void* m);
        static inline MVec simd128_loadu_epi32(const void* m);
        static inline MVec simd128_loadu_epi64(const void* m);
#   endif // SIMD_LVL3

#if SIMD_LVL3
#   define simd_loadu simd_loadu_si512
#elif SIMD_LVL2
#   define simd_loadu simd_loadu_si256
#elif SIMD_LVL1
#   define simd_loadu simd_loadu_si128
#endif

#include <Anvie/Simd/Impl/LoadStoreOps.h>

#endif // SIMD_ENABLED

#endif // ANVIE_SIMD_LOAD_STORE_OPERATIONS_H
