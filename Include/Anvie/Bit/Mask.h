/**
 * @file Mask.h
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
 * @brief Defines macros for performing masking operations on
 * provided values.
 * */

#ifndef ANVIE_UTILS_BIT_MASK_H
#define ANVIE_UTILS_BIT_MASK_H

#include <Anvie/Types.h>

/**
 * Generates a mask with lower @p n bits flagged as 1.
 *
 * @param n Number of bits
 * @return Mask with lower @p n bits set to 1
 */
#define MASK_LO(t, n) (t)( ~(((t)(-1)) << (n)) )

/**
 * Generates a mask with higher @p n bits flagged as 1.
 *
 * @param n Number of bits
 * @return Mask with higher @p n bits set to 1
 */
#define MASK_HI(t, n) (t)( ~(((t)(-1)) >> (n)) )

/**
 * Generates a mask with bits flagged as 1 from index @p s to @p s + @p n.
 *
 * @param s Starting index
 * @param n Number of bits
 * @return Mask with bits flagged as 1 from index @p s to @p s + @p n
 */
#define MASK_RANGE(t, s, n) (t)( (((t)-1) << (s)) & ~(((t)-1) << ((s) + (n))) )

/**
 * Selects lower @p n bits from value @p a.
 *
 * @param a Value to select bits from
 * @param n Number of bits to select
 * @return Selected lower @p n bits from value @p a
 */
#define SEL_LO(t, a, n) ((a) & MASK_LO(t, n))

/**
 * Selects higher @p n bits from value @p a.
 *
 * @param a Value to select bits from
 * @param n Number of bits to select
 * @return Selected higher @p n bits from value @p a
 */
#define SEL_HI(t, a, n) ((a) & MASK_HI(t, n))

/**
 * Selects a range of bits starting from index @p s to @p s + @p n from value @p a.
 *
 * @param a Value to select bits from
 * @param s Starting index
 * @param n Number of bits to select
 * @return Selected range of bits from value @p a
 */
#define SEL_RANGE(t, a, s, n) ((a) & MASK_RANGE(t, s, n))

/**
 * Set bit at index @p n of given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to set the bit in
 * @param n Bit position/index to set
 */
#define SET_BIT(t, v, n) ( (v) |= ((t)1 << (n)) )

/**
 * Set all bits from 0 to @p n (inclusive) in the given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to set the bits in
 * @param n Bit position/index to set (low to n)
 */
#define SET_LO(t, v, n)  ((v) |= MASK_LO(t, n))

/**
 * Set all bits from @p n to the highest bit in the given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to set the bits in
 * @param n Bit position/index to set (high to n)
 */
#define SET_HI(t, v, n)  ((v) |= MASK_HI(t, n))

/**
 * Set all bits within a given range [@p s, @p s + @p n] in the variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to set the bits in
 * @param s Starting bit position/index of the range
 * @param n Number of bits in the range
 */
#define SET_RANGE(t, v, s, n) ( (v) |= MASK_RANGE(t, s, n) )

/**
 * Clear bit at index @p n of given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to clear the bit from
 * @param n Bit position/index to clear
 */
#define CLR_BIT(t, v, n) ( (v) &= (~((t)1 << (n))) )

/**
 * Clear all bits from 0 to @p n (inclusive) in the given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to clear the bits from
 * @param n Bit position/index to clear (low to n)
 */
#define CLR_LO(t, v, n)  ( (v) &= ~(t)MASK_LO(t, n) )

/**
 * Clear all bits from @p n to the highest bit in the given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to clear the bits from
 * @param n Bit position/index to clear (high to n)
 */
#define CLR_HI(t, v, n)  ( (v) &= ~(t)MASK_HI(t, n) )
/**
 * Clear all bits within a given range [@p s, @p s + @p n] in the variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to clear the bits from
 * @param s Starting bit position/index of the range
 * @param n Number of bits in the range
 */
#define CLR_RANGE(t, v, s, n) ( (v) &= ~(t)MASK_RANGE(t, s, n) )

/**
 * Get the value of bit at index @p n of given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to get the bit from
 * @param n Bit position/index to retrieve
 * @return Value of the bit at the specified index (0 or 1)
 */
#define GET_BIT(t, v, n) ( ( (v) & ((t)1 << (n)) ) != 0 )

/**
 * Get the value of first @p in bits from bottom in the given variable @p v.
 *
 * @param t Type of the variable
 * @param v Variable to get the bits from
 * @param n Bit position/index to retrieve (low to n)
 * @return Value of the bits from 0 to @p n in the variable @p v
 */
#define GET_LO(t, v, n) (v & (MASK_LO(t, n)))

/**
 * Get the value of last @p n bits from the top in the given variable @p v.
 * The returned value will be shifted to left.
 *
 * @param t Type of the variable
 * @param v Variable to get the bits from
 * @param n Bit position/index to retrieve (high to n)
 * @return Value of the bits from @p n to the highest bit in the variable @p v
 */
#define GET_HI(t, v, n) (  ( (v) & (MASK_HI(t, n)) ) >> (MUL8(sizeof(t)) - (n))  )

/**
 * Get the value of bits within a given range [@p s, @p s + @p n] in the variable @p v.
 * The returned value will be shifted to left.
 *
 * @param t Type of the variable
 * @param v Variable to get the bits from
 * @param s Starting bit position/index of the range
 * @param n Number of bits in the range
 * @return Value of the bits within the specified range in the variable @p v
 */
#define GET_RANGE(t, v, s, n) ( ((v) & (t)MASK_RANGE(t, s, n)) >> s )

#define MASK8_LO(n)             MASK_LO  (Uint8, n)
#define MASK8_HI(n)             MASK_HI  (Uint8, n)
#define MASK8_RANGE(n)          MASK_RANGE(Uint8, n)
#define SET8_BIT(v, n)          SET_BIT  (Uint8, v, n)
#define SET8_LO(v, n)           SET_LO   (Uint8, v, n)
#define SET8_HI(v, n)           SET_HI   (Uint8, v, n)
#define SET8_RANGE(v, s, n)     SET_RANGE(Uint8, v, s, n)
#define CLR8_BIT(v, n)          CLR_BIT  (Uint8, v, n)
#define CLR8_LO(v, n)           CLR_LO   (Uint8, v, n)
#define CLR8_HI(v, n)           CLR_HI   (Uint8, v, n)
#define CLR8_RANGE(v, s, n)     CLR_RANGE(Uint8, v, s, n)
#define GET8_BIT(v, n)          GET_BIT  (Uint8, v, n)
#define GET8_LO(v, n)           GET_LO   (Uint8, v, n)
#define GET8_HI(v, n)           GET_HI   (Uint8, v, n)
#define GET8_RANGE(v, s, n)     GET_RANGE(Uint8, v, s, n)

#define SET16_BIT(v, n)         SET_BIT  (Uint16, v, n)
#define SET16_LO(v, n)          SET_LO   (Uint16, v, n)
#define SET16_HI(v, n)          SET_HI   (Uint16, v, n)
#define SET16_RANGE(v, s, n)    SET_RANGE(Uint16, v, s, n)
#define CLR16_BIT(v, n)         CLR_BIT  (Uint16, v, n)
#define CLR16_LO(v, n)          CLR_LO   (Uint16, v, n)
#define CLR16_HI(v, n)          CLR_HI   (Uint16, v, n)
#define CLR16_RANGE(v, s, n)    CLR_RANGE(Uint16, v, s, n)
#define GET16_BIT(v, n)         GET_BIT  (Uint16, v, n)
#define GET16_LO(v, n)          GET_LO   (Uint16, v, n);
#define GET16_HI(v, n)          GET_HI   (Uint16, v, n);
#define GET16_RANGE(v, s, n)    GET_RANGE(Uint16, v, s, n)

#define SET32_BIT(v, n)         SET_BIT  (Uint32, v, n)
#define SET32_LO(v, n)          SET_LO   (Uint32, v, n)
#define SET32_HI(v, n)          SET_HI   (Uint32, v, n)
#define SET32_RANGE(v, s, n)    SET_RANGE(Uint32, v, s, n)
#define CLR32_BIT(v, n)         CLR_BIT  (Uint32, v, n)
#define CLR32_LO(v, n)          CLR_LO   (Uint32, v, n)
#define CLR32_HI(v, n)          CLR_HI   (Uint32, v, n)
#define CLR32_RANGE(v, s, n)    CLR_RANGE(Uint32, v, s, n)
#define GET32_BIT(v, n)         GET_BIT  (Uint32, v, n)
#define GET32_LO(v, n)          GET_LO   (Uint32, v, n)
#define GET32_HI(v, n)          GET_HI   (Uint32, v, n);
#define GET32_RANGE(v, s, n)    GET_RANGE(Uint32, v, s, n)

#define SET64_BIT(v, n)         SET_BIT  (Uint64, v, n)
#define SET64_LO(v, n)          SET_LO   (Uint64, v, n)
#define SET64_HI(v, n)          SET_HI   (Uint64, v, n)
#define SET64_RANGE(v, s, n)    SET_RANGE(Uint64, v, s, n)
#define CLR64_BIT(v, n)         CLR_BIT  (Uint64, v, n)
#define CLR64_LO(v, n)          CLR_LO   (Uint64, v, n)
#define CLR64_HI(v, n)          CLR_HI   (Uint64, v, n)
#define CLR64_RANGE(v, s, n)    CLR_RANGE(Uint64, v, s, n)
#define GET64_BIT(v, n)         GET_BIT  (Uint64, v, n)
#define GET64_LO(v, n)          GET_LO   (Uint64, v, n)
#define GET64_HI(v, n)          GET_HI   (Uint64, v, n)
#define GET64_RANGE(v, s, n)    GET_RANGE(Uint64, v, s, n)

#define CREATE_MASK8(a, b, c, d, e, f, g, h)                            \
    (( ((a) & 1) << 7 ) | \
     ( ((b) & 1) << 6 ) | \
     ( ((c) & 1) << 5 ) | \
     ( ((d) & 1) << 4 ) | \
     ( ((e) & 1) << 3 ) | \
     ( ((f) & 1) << 2 ) | \
     ( ((g) & 1) << 1 ) | \
        (h  & 1))

#define BCAST_8_TO_16(v) OR((Uint16)(v) << 8, (Uint16)v)
#define BCAST_8_TO_32(v) OR((Uint32)BCAST_8_TO_16(v) << 16, (Uint32)BCAST_8_TO_16(v))
#define BCAST_8_TO_64(v) OR((Uint64)BCAST_8_TO_32(v) << 32, (Uint64)BCAST_8_TO_32(v))

#define BCAST_16_TO_32(v) OR((Uint32)(v) << 16, (Uint32)v)
#define BCAST_16_TO_64(v) OR((Uint64)BCAST_16_TO_32(v) << 32, (Uint64)BCAST_16_TO_32(v))

#define BCAST_32_TO_64(v) OR((Uint64)(v) << 32, (Uint64)v)

#define GET_U8_AT(v, idx)  ( ( (v) & (0xff   << MUL8(idx) ) ) >> MUL8(idx)  )
#define GET_U16_AT(v, idx) ( ( (v) & (0xffff << MUL16(idx)) ) >> MUL16(idx) )
#define GET_U32_AT(v, idx) ( ( (v) & (0xffff << MUL32(idx)) ) >> MUL32(idx) )

#endif // ANVIE_UTILS_BIT_MASK_H
