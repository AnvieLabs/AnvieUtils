/**
 * @file Arithmetic.h
 * @date Fri, 29th December, 2023
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
 * @brief Defines some hacky defines that can be used to do
 * faster arithmetic sometimes
 * */

#ifndef ANVIE_UTILS_BIT_ARITHMETIC_H
#define ANVIE_UTILS_BIT_ARITHMETIC_H

#include <Anvie/Types.h>

#define DIV_BY_POW2(v, n) ( (v) >> (n) )
#define MOD_BY_POW2(v, n) ( (v) & ((1 << (n)) - 1) )
#define MUL_BY_POW2(v, n) ( (v) << (n) )

#define DIV2(v)   DIV_BY_POW2(v, 1)
#define DIV4(v)   DIV_BY_POW2(v, 2)
#define DIV8(v)   DIV_BY_POW2(v, 3)
#define DIV16(v)  DIV_BY_POW2(v, 4)
#define DIV32(v)  DIV_BY_POW2(v, 5)
#define DIV64(v)  DIV_BY_POW2(v, 6)
#define DIV128(v) DIV_BY_POW2(v, 7)
#define DIV256(v) DIV_BY_POW2(v, 8)
#define DIV512(v) DIV_BY_POW2(v, 9)

#define MOD2(v)   MOD_BY_POW2(v, 1)
#define MOD4(v)   MOD_BY_POW2(v, 2)
#define MOD8(v)   MOD_BY_POW2(v, 3)
#define MOD16(v)  MOD_BY_POW2(v, 4)
#define MOD32(v)  MOD_BY_POW2(v, 5)
#define MOD64(v)  MOD_BY_POW2(v, 6)
#define MOD128(v) MOD_BY_POW2(v, 7)
#define MOD256(v) MOD_BY_POW2(v, 8)
#define MOD512(v) MOD_BY_POW2(v, 9)

#define MUL2(v)   MUL_BY_POW2(v, 1)
#define MUL4(v)   MUL_BY_POW2(v, 2)
#define MUL8(v)   MUL_BY_POW2(v, 3)
#define MUL16(v)  MUL_BY_POW2(v, 4)
#define MUL32(v)  MUL_BY_POW2(v, 5)
#define MUL64(v)  MUL_BY_POW2(v, 6)
#define MUL128(v) MUL_BY_POW2(v, 7)
#define MUL256(v) MUL_BY_POW2(v, 8)
#define MUL512(v) MUL_BY_POW2(v, 9)

static inline Uint64 NEXT_POW2(Uint64 n) {
    n--; // Decrease n by 1 to handle cases where n is already a power of 2
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1; // Increment the result by 1 to get the next power of 2
}

#endif // ANVIE_UTILS_BIT_ARITHMETIC_H
