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
 * @brief Defines generic SIMD (Single Instruction Multiple Data) wrappers over
 * comparision operations. The only condition is that SIMD at atleast one of the
 * all three levels must be enabled.
 *
 * This file only contains definions. The actual implementation of each function
 * is in the Impl file.
 * */

#ifndef ANVIE_SIMD_COMPARISION_OPERATIONS_H
#define ANVIE_SIMD_COMPARISION_OPERATIONS_H

#include <Anvie/Simd/Types.h>

/**
 * @note
 * The comparision operation functions are a wrapper around the intrinsic SIMD functions.
 * This is to avoid use of if-elif-else macros everywhere.
 *
 * The following functions return the result of comparision operation in a `MVec` type variable.
 * This is either 1, 2, 4, or 8 byte variable, which can be used interchangeably with corresponding,
 * `Uint` counterparts. Each bit in the mask specifies whether or not the word at corresponding index
 * in `v1` and `v2` satisfied the comparision check. A word in the above statement can be of
 * 1, 2, 4, or 8 byte long.
 *
 * In the end, the working of these functions is exactly same as the intrinsic function counterparts
 * of highest size. Meaning if AVX512 extensions are available then the wrapper will behave as corresponding
 * intrinsic functions of AVX512 extensions, leaving all other lower sized extensions.
 *
 * If AVX extensions are not abled at all, then each function will return 0 irrespective of the values
 * passed as parameters.
 * */

#if SIMD_ENABLED

    static inline MVec simd_cmpeq_epi8(MVec v1, MVec v2);
    static inline MVec simd_cmpgt_epi8(MVec v1, MVec v2);

    static inline MVec simd_cmpeq_epi16(MVec v1, MVec v2);
    static inline MVec simd_cmpgt_epi16(MVec v1, MVec v2);

    static inline MVec simd_cmpeq_epi32(MVec v1, MVec v2);
    static inline MVec simd_cmpgt_epi32(MVec v1, MVec v2);

    static inline MVec simd_cmpeq_epi64(MVec v1, MVec v2);
    static inline MVec simd_cmpgt_epi64(MVec v1, MVec v2);

    static inline MMask simd_cmpeq_epi8_mask(MVec v1, MVec v2);
    static inline MMask simd_cmpgt_epi8_mask(MVec v1, MVec v2);

    static inline Uint8 simd_tzcnt(Uint64 v);

#include <Anvie/Simd/Impl/ComparisionOps.h>

#endif // SIMD_ENABLED

#endif // ANVIE_SIMD_COMPARISION_OPERATIONS_H
