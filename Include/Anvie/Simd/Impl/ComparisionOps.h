/**
 * @file ComparisionOps.h
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
 * @brief Implementation file for SIMD comparision operations.
 * Defined separately so, as to keep the definition and declarations separately.
 * */

#ifndef ANVIE_SIMD_IMPLEMENTATIONS_COMPARISION_OPERATIONS_H
#define ANVIE_SIMD_IMPLEMENTATIONS_COMPARISION_OPERATIONS_H

#include <Anvie/Types.h>
#include <Anvie/Simd/Types.h>
#include <Anvie/HelperDefines.h>

#if SIMD_ENABLED
#   if SIMD_LVL3
#   define SIMD_CMP_EPI(op, n)                                          \
    static inline MVec64 simd_cmp##op##_epi##n(MVec64 v1, MVec64 v2) {  \
        return _mm512_cmp##op##_epi8(v1, v2);                           \
    }                                                                   \
                                                                        \
    static inline MMask64 simd_cmp##op##_epi##n##_mask(MVec64 v1, MVec64 v2) { \
        return _mm512_cmp##op##_epi##n##_mask(v1, v2);                  \
    }                                                                   \
                                                                        \
    static inline MMask64 simd256_cmp##op##_epi##n##_mask(MVec32 v1, MVec32 v2) { \
        return _mm256_cmp##op##_epi##n##_mask(v1, v2);                  \
    }                                                                   \
                                                                        \
    static inline MMask64 simd128_cmp##op##_epi##n##_mask(MVec16 v1, MVec16 v2) { \
        return _mm_cmp##op##_epi##n##_mask(v1, v2);                     \
    }

#   elif SIMD_LVL2
#   define SIMD_CMP_EPI(op, n)                                          \
    static inline MVec32 simd_cmp##op##_epi##n(MVec32 v1, MVec32 v2) {  \
        return _mm256_cmp##op##_epi##n(v1, v2);                         \
    }
#   elif SIMD_LVL1
#   define SIMD_CMP_EPI(op, n)                                          \
    static inline MVec16 simd_cmp##op##_epi##n(MVec16 v1, MVec16 v2) {  \
        return _mm_cmp##op##_epi##n(v1, v2);                            \
    }
#   endif
#else
#   define SIMD_CMP_EPI(op, n)                                          \
    static inline Uint8 simd_cmp##op##_epi##n(Uint64 v1, Uint64 v2) {   \
        return 0;                                                       \
    }
#endif

/* define comparsion operations for 1, 2, 4, and 8 byte word sizes. */
SIMD_CMP_EPI(eq, 8);
SIMD_CMP_EPI(gt, 8);

SIMD_CMP_EPI(eq, 16);
SIMD_CMP_EPI(gt, 16);

SIMD_CMP_EPI(eq, 32);
SIMD_CMP_EPI(gt, 32);

SIMD_CMP_EPI(eq, 64);
SIMD_CMP_EPI(gt, 64);

#undef SIMD_CMP
#undef SIMD_CMP_EPI


#if SIMD_LVL2
#   define SIMD_CMP_EPI8_MASK(op)                                       \
    static inline MMask32 simd_cmp##op##_epi8_mask(MVec32 v1, MVec32 v2) { \
        /* movemask will convert given value to a uint32 mask */        \
        return _mm256_movemask_epi8(_mm256_cmp##op##_epi8(v1, v2));     \
    }
#elif SIMD_LVL1
#   define SIMD_CMP_EPI8_MASK(op)                                       \
    static inline MMask16 simd_cmp##op##_epi8_mask(MVec16 v1, MVec16 v2) { \
        /* movemask will convert given value to a uint32 mask */        \
        return _mm_movemask_epi8(_mm_cmp##op##_epi8(v1, v2));           \
    }
#endif

SIMD_CMP_EPI8_MASK(eq);
SIMD_CMP_EPI8_MASK(gt);

/**
 * Count trailing zeroes
 * */
static inline Uint8 simd_tzcnt(Uint64 v) {
#if SIMD_ENABLED
#   if SIMD_LVL3
        return (Uint8) _tzcnt_u64(v);
#   elif SIMD_LVL2
        return (Uint8) _tzcnt_u32(v);
#   elif SIMD_LVL1
        return (Uint8) _tzcnt_u16(v);
#   endif
#else
    return 0;
#endif
}

#endif // ANVIE_SIMD_IMPLEMENTATIONS_COMPARISION_OPERATIONS_H
