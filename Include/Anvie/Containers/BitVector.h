/**
 * @file BitVector.h
 * @date Mon, 18th December, 2023
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
 * @brief Need only one bit to store boolean values instead of 8 bits?
 * I got you bro! @c BitVector is a container to store a vector of bits.
 * Each bit is a boolean value and can be accessed by using an index.
 * BitVector must be used in cases, where you need to store a large number
 * of boolean values together in an array and using 1 byte boolean values
 * is just too much for that.
 * */

#ifndef ANVIE_UTILS_CONTAINER_BIT_VECTOR_H
#define ANVIE_UTILS_CONTAINER_BIT_VECTOR_H

#include <Anvie/Types.h>
#include <Anvie/Containers/Common.h>
#include <Anvie/BitManipulation.h>

/**
 * BitVector stores multiple boolean values in a single byte.
 * Instead of using 1 byte to store boolean, you can now store
 * 8 boolean values in a single byte.
 * */
typedef struct BitVector {
    Size   length;              /**< Number of booleans used. */
    Size   capacity;            /**< Total number of booleans that can be stored. */
    Uint8* data;                /**< Array to store boolean values. */
} BitVector;

/* the returned value depends on what is expected when length is not 8bit aligned */
#define    bitvec_get_length_in_bytes(bv) (bv ? DIV8(bv->length) : 0)
/* the returned value depends on what is expected when capacity is not 8bit aligned */
#define    bitvec_get_capacity_in_bytes(bv) (bv ? DIV8(bv->capacity) : 0)

BitVector* bitvec_create();
void       bitvec_destroy(BitVector* bv);
BitVector* bitvec_clone(BitVector* bv);
void       bitvec_set_equal(BitVector* dstbv, BitVector* srcbv);

/* resize/reserve operation */
void       bitvec_reserve(BitVector* bv, Size numbools);
void       bitvec_resize(BitVector* bv, Size numbools);

/* set/remove operation */
void       bitvec_push(BitVector* bv, Bool val);
Bool       bitvec_pop(BitVector* bv);
void       bitvec_set(BitVector* bv, Size index);
void       bitvec_clear(BitVector* bv, Size index);
void       bitvec_set_all(BitVector* bv);
void       bitvec_clear_all(BitVector* bv);
void       bitvec_set_range(BitVector* bv, Size range_begin, Size range_size);
void       bitvec_clear_range(BitVector* bv, Size range_begin, Size range_size);

/* get operation */
Bool       bitvec_peek(BitVector* bv, Size index);

/* logical operations */
/* binary operations */
BitVector* bitvec_xor(BitVector* bv1, BitVector* bv2);
BitVector* bitvec_and(BitVector* bv1, BitVector* bv2);
BitVector* bitvec_or(BitVector* bv1, BitVector* bv2);
BitVector* bitvec_xnor(BitVector* bv1, BitVector* bv2);
BitVector* bitvec_nand(BitVector* bv1, BitVector* bv2);
BitVector* bitvec_nor(BitVector* bv1, BitVector* bv2);
BitVector* bitvec_shl(BitVector* bv, Size index);
BitVector* bitvec_shr(BitVector* bv, Size index);
/* unary operation */
BitVector* bitvec_not(BitVector* bv);

/* TODO: Will implement these later, writing tests take too much time,
 * I also need to improve and implement tests for other containers.
 * Will implemenent these when I feel the need for these or when I want to.
 * For, now it's just a reminder.
 * */
/* binary inline operation */
/* void       bitvec_xor_inline(BitVector* bv1, BitVector* bv2); */
/* void       bitvec_and_inline(BitVector* bv1, BitVector* bv2); */
/* void       bitvec_or_inline(BitVector* bv1, BitVector* bv2); */
/* void       bitvec_xnor_inline(BitVector* bv1, BitVector* bv2); */
/* void       bitvec_nand_inline(BitVector* bv1, BitVector* bv2); */
/* void       bitvec_nor_inline(BitVector* bv1, BitVector* bv2); */
/* void       bitvec_shl_inline(BitVector* bv, Size index); */
/* void       bitvec_shr_inline(BitVector* bv, Size index); */
/* unary operation */
/* void       bitvec_not_inline(BitVector* bv); */

/* comparision operation */
Bool       bitvec_cmpeq(BitVector* bv1, BitVector* bv2);

/* misc operations */
/* TODO: BitVector* bitvec_reverse(BitVector* bv); */

/**
 * @c BitVector does not need le, lt, ge, gt comparisions,
 * because this is not a number, for that concept we need @c Number,
 * which will treat BitVector as a number.
 * Comparision operation will depend on whether a number is
 * negative or positive and some other factors as well.
 * BitVector is just supposed to store a vector of bits,
 * not make a full blown number.
 * */


#endif // ANVIE_UTILS_CONTAINER_BIT_VECTOR_H
