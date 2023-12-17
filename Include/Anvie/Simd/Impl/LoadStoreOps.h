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
 * @brief Implements wrappers around Load/Store intrinsics.
 * */

#ifndef ANVIE_SIMD_IMPLEMENTATION_LOAD_STORE_OPERATIONS_H
#define ANVIE_SIMD_IMPLEMENTATION_LOAD_STORE_OPERATIONS_H

#include <Anvie/Types.h>
#include <Anvie/Simd/Types.h>

/* intel really needs to be so dumb in naming somethings, why 64x only for 64? not others? idiot. */
#define _mm256_set_epi64 _mm256_set_epi64x
#define _mm256_set1_epi64 _mm256_set1_epi64x

#if SIMD_LVL3
#   define SIMD_SET1_EPI(n)                            \
    static inline MVec simd_set1_epi##n(Int##n a) {    \
        return _mm512_set1_epi##n(a);                  \
    }

#   define SIMD_LOADU_EPI(n)                                    \
    static inline MVec simd_loadu_epi##n(const void* m) {       \
        return _mm512_loadu_epi8(m);                            \
    }                                                           \
                                                                \
    static inline MVec simd256_loadu_epi##n(const void* m) {    \
        return _mm256_loadu_epi8(m);                            \
    }                                                           \
                                                                \
    }                                                           \
    static inline MVec simd128_loadu_epi##n(const void* m) {    \
        return _mm_loadu_epi8(m);                               \
    }
#elif SIMD_LVL2
#   define SIMD_SET1_EPI(n)                         \
    static inline MVec simd_set1_epi##n(Int##n a) { \
        return _mm256_set1_epi##n(a);               \
    }
#elif SIMD_LVL1
#   define SIMD_SET1_EPI(n)                         \
    static inline MVec simd_set1_epi##n(Int##n a) { \
        return _mm_set1_epi##n(a);                  \
    }
#endif // SIMD_LVL3

SIMD_SET1_EPI(8);
SIMD_SET1_EPI(16);
SIMD_SET1_EPI(32);
SIMD_SET1_EPI(64);

#if SIMD_LVL3
SIMD_LOADU_EPI(8);
SIMD_LOADU_EPI(16);
SIMD_LOADU_EPI(32);
SIMD_LOADU_EPI(64);
#endif // SIMD_LVL3

#undef SIMD_LOADU_EPI
#undef SIMD_SET1_EPI

#if SIMD_LVL1
#   define SIMD_LOADU_SI(n)                                   \
    static inline MVec16 simd_loadu_si##n(const void* m) {    \
        return _mm_loadu_si##n(m);                            \
    }

SIMD_LOADU_SI(16);
SIMD_LOADU_SI(32);
SIMD_LOADU_SI(64);
SIMD_LOADU_SI(128);

#undef SIMD_LOADU_SI
#endif // SIMD_LVL1

#if SIMD_LVL2
static inline MVec32 simd_loadu_si256(const void* m) {
    return _mm256_loadu_si256(m);
}
#endif // SIMD_LVL2

#if SIMD_LVL3
static inline MVec simd_loadu_si512(const void* m) {
    return _mm512_loadu_si512(m);
}

#   define SIMD_LOADU_EPI(n)                                      \
    static inline MVec16 simd_loadu128_epi##n(const void* m) {    \
        return _mm_loadu_epi##n(m);                               \
    }                                                             \
    static inline MVec32 simd_loadu256_epi##n(const void* m) {    \
        return _mm256_loadu_epi##n(m);                            \
    }                                                             \
    static inline MVec64 simd_loadu512_epi##n(const void* m) {    \
        return _mm512_loadu_epi##n(m);                            \
    }

SIMD_LOADU_EPI(8);
SIMD_LOADU_EPI(16);
SIMD_LOADU_EPI(32);
SIMD_LOADU_EPI(64);

#undef SIMD_LOADU_EPI
#endif // SIMD_LVL3

#endif // ANVIE_SIMD_IMPLEMENTATION_LOAD_STORE_OPERATIONS_H
