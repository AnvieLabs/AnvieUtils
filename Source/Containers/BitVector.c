/**
 * @file BitVector.c
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
 * @brief BitVector implementation.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/HelperDefines.h>
#include <string.h>

/**
 * Always keep this in power of 2, also greater than equal to 8,
 * like 8, 16, 32, 64, 128 and so on...
 * Each time bitvector is resized to store more booleans, the size is
 * incremented by this value. Meaning, the capacity of bitvector is always
 * in multiples of this value. */
#define BITVECTOR_DEFAULT_INCREMENT_SIZE 32 /* 256 bits at once */

/**
 * Macro to generate next multiple of increment size.
 * This is where we need the @c BITVECTOR_DEFAULT_INCREMENT_SIZE
 * to be a power of two. This will help us quickly compute next
 * multiple values.
 * @param bytelen Length of BitVector in terms of bytes
 * */
#define BITVECTOR_NEXT_INCREMENTED_LENGTH(bytelen)                      \
    (((bytelen) & (~(BITVECTOR_DEFAULT_INCREMENT_SIZE - 1))) + BITVECTOR_DEFAULT_INCREMENT_SIZE)


/**
 * Create a Uint8 flag with n-th bit set
 * */
#define U8_SET_NTH_BIT(n) (Uint8)(1 << (n))

/**
 * Create a new @c BitVector.
 * @return BitVector* A valid pointer on success.
 * @return BitVector* @c NULL on failure.
 * */
BitVector* bitvector_create() {
    BitVector* bv = NEW(BitVector);
    RETURN_VALUE_IF_FAIL(bv, NULL, ERR_OUT_OF_MEMORY);

    bv->data = ALLOCATE(Uint8, BITVECTOR_DEFAULT_INCREMENT_SIZE);
    if(!bv->data) {
        FREE(bv);
        ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
    }

    bv->capacity = BITVECTOR_DEFAULT_INCREMENT_SIZE*8;

    return bv;
}

/**
 * Destroy given @c BitVector.
 * @param bv @c BitVector.
 * */
void bitvector_destroy(BitVector* bv) {
    RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    if(bv->data) {
        FREE(bv->data);
        bv->data = NULL;
    }
    FREE(bv);
}

/**
 * Create a clone of the given @c BitVector.
 * @param bv
 * @return BitVector* valid pointer on success.
 * @return BitVector* NULL otherwise.
 * */
BitVector* bitvector_clone(BitVector* bv) {
    RETURN_VALUE_IF_FAIL(bv, NULL, ERR_INVALID_ARGUMENTS);

    BitVector* bvclone = bitvector_create();
    RETURN_VALUE_IF_FAIL(bvclone, NULL, ERR_INVALID_ARGUMENTS);

    bitvector_resize(bvclone, bv->length);
    memcpy(bvclone->data, bv->data, (bv->length >> 3) + 1);

    return bvclone;
}

/**
 * Make @p dstbv and @p srcbv equal, i.e., @p dstbv = @p srcbv.
 * The implementation does not care about the length of
 * both @c BitVector values. The contents of @p dstbv in
 * the end will match @p srcbv. The capacity is not guaranteed
 * to change in same manner, just the content is guaranteed to
 * be exactly same.
 * @param dstbv
 * @param srcbv
 * */
void bitvector_set_equal(BitVector* dstbv, BitVector* srcbv) {
    RETURN_IF_FAIL(dstbv && srcbv, ERR_INVALID_ARGUMENTS);

    bitvector_resize(dstbv, srcbv->length);
    memcpy(dstbv->data, srcbv->data, 1 + (srcbv->length >> 3));
}

/**
 * Reserve space to store @p numbools number of booleans.
 * This results in capacity increase of @c BitVector but not
 * increase in length.
 * @param bv
 * @param numbools Number of booleans.
 * */
void bitvector_reserve(BitVector* bv, Size numbools) {
    RETURN_IF_FAIL(bv && numbools, ERR_INVALID_ARGUMENTS);

    /* no change in size of new reserved size is less than current capacity */
    if(numbools < bv->capacity) {
        return;
    }

    /* recompute new size and resize */
    Size newsz = BITVECTOR_NEXT_INCREMENTED_LENGTH(numbools >> 3); /* next multiple */
    bv->data = realloc(bv->data, newsz);

    /* clear all bits in allocated new memory */
    Size cap = bv->capacity >> 3; /* divide by 8 */
    memset(bv->data + cap, 0, newsz - cap);

    /* since capacity indicates total number of booleans, we multiply by 8 */
    bv->capacity = newsz << 3; /* multiply with 8 to count number of bits */
}

/**
 * Resize this @c BitVector to store @p numbools number of booleans.
 * This will result in capacity as well as length increase.
 * Current implementation of @c BitVector does not decrease
 * capacity, even if length goes to 0.
 * @param bv
 * @param numbools Number of booleans.
 * */
void bitvector_resize(BitVector* bv, Size numbools) {
    RETURN_IF_FAIL(bv && numbools, ERR_INVALID_ARGUMENTS);

    if(numbools < bv->length) {
        bitvector_clear_range(bv, numbools, bv->length);
    } else if(numbools > bv->capacity) {
        bitvector_reserve(bv, numbools);
    }

    bv->length = numbools;
}

/**
 * Push back a boolean value to the back of @c BitVector.
 * @param bv @c BitVector.
 * @param val @c Bool value to be pushed.
 * */
void bitvector_push_back(BitVector* bv, Bool val) {
    RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    if(bv->length == bv->capacity) {
        bitvector_reserve(bv, bv->length+1);
    }

    Size len = bv->length >> 3; /* divide by 8 */
    Size rem = bv->length & 7; /* take modulo with 8, but faster */
    if(val) {
        bv->data[len] |= U8_SET_NTH_BIT(rem);
    } else {
        bv->data[len] &= ~(U8_SET_NTH_BIT(rem));
    }

}

/**
 * Remove last value from @c BitVector.
 * @param bv
 * @return True if last boolean is True.
 * @return False if last boolean is False.
 * */
Bool bitvector_pop_back(BitVector* bv) {
    RETURN_VALUE_IF_FAIL(bv, False, ERR_INVALID_ARGUMENTS);
    RETURN_VALUE_IF_FAIL(bv->length, False, "Cannot remove from BitVector. No booleans previously stored.\n");

    Size len = bv->length >> 3;
    Size rem = bv->length & 7; /* remainder when divided by 8 */
    Bool val = bv->data[len] & U8_SET_NTH_BIT(rem); /* retrieve the bit */
    bv->data[len] &= ~(U8_SET_NTH_BIT(rem)); /* unflag the bit */
    bv->length--; /*  decrement size */
    return val;
}

/**
 * Get boolean value at given index.
 * @param bv
 * @param index Index of boolean to retrieve.
 * @return True if boolean at given index is True.
 * @return False if boolean at given index is False.
 * */
Bool bitvector_peek(BitVector* bv, Size index) {
    RETURN_VALUE_IF_FAIL(bv, False, ERR_INVALID_ARGUMENTS);

    Size len = index >> 3;
    Size rem = index & 7;
    return (bv->data[len] & (U8_SET_NTH_BIT(rem))) != 0;
}

/**
 * Set bit/boolean at given index.
 * If bit at given index is already set, then no change
 * will take place (as should be expected).
 * This will automatically resize the @c BitVector
 * to store at least @p index number of booleans if @p index
 * exceeds total length of @c BitVector.
 * @param bv
 * @param index Index of bit to be set/flagged.
 * */
inline void bitvector_set(BitVector* bv, Size index) {
    RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    /* if write position exceeds the capacity, then reserve more space */
    if(index > bv->capacity) {
        bitvector_reserve(bv, index);
    }

    /* if we wrote directly after the length of bitvector,
     * then length automatically increases */
    if(index >= bv->length) {
        bv->length = index;
    }

    /* set the bit */
    Size pos = index >> 3; /* divide by 8 */
    Size rem = index & 7; /* take modulo with 8, but faster */
    bv->data[pos] |= U8_SET_NTH_BIT(rem); /* set bit */
}

/**
 * Clear bit/boolean at given index.
 * If bit at given index is already cleared, then no change
 * will take place (as should be expected).
 * This will automatically resize the @c BitVector
 * to store at least @p index number of booleans if @p index
 * exceeds total length of @c BitVector.
 * @param bv
 * @param index Index of bit to be set/flagged.
 * */
inline void bitvector_clear(BitVector* bv, Size index) {
    RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    /* if write position exceeds the capacity, then reserve more space */
    if(index > bv->capacity) {
        bitvector_reserve(bv, index);
    }

    /* if we wrote directly after the length of bitvector,
     * then length automatically increases */
    if(index >= bv->length) {
        bv->length = index;
    }

    /* clear the bit */
    Size pos = index >> 3; /* divide by 8 */
    Size rem = index & 7; /* take modulo with 8, but faster */
    bv->data[pos] &= ~(U8_SET_NTH_BIT(rem)); /* unset bit */
}

/**
 * This will set all boolean(s)/bit(s) in
 * the whole @c BitVector. Not just length,
 * but whole capacity.
 * @param bv
 * */
void bitvector_set_all(BitVector* bv) {
    RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);
    memset(bv->data, 0xff, bv->capacity << 3);
}

/**
 * This will clear/unset all boolean(s)/bit(s) in
 * the whole @c BitVector. Not just length,
 * but whole capacity.
 * @param bv
 * */
inline void bitvector_clear_all(BitVector* bv) {
    RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);
    memset(bv->data, 0, bv->capacity << 3);
}

/**
 * Set all bits in a given range.
 * @param bv
 * @param range_begin
 * @param range_size
 * */
inline void bitvector_set_range(BitVector* bv, Size range_begin, Size range_size) {
    RETURN_IF_FAIL(bv && range_size, ERR_INVALID_ARGUMENTS);

    Size rem = (range_begin & 7); /* remainder after dividing by 8 */

    /* align beginning of clear range to 8 bit boundary*/
    /* if range begins from middle of a byte, then clear from the beginning, for that byte only */
    if(rem) {
        /* clear bits very selectively */
        Uint8 clearbitsz = MIN(range_size, 8-rem);
        for(Uint8 s = 0; s < clearbitsz; s++) {
            bitvector_set(bv, range_begin + s);
        }

        /* if range size fits between starting of clear region and ending of the byte, then make an early exit */
        if(range_size <= 8-rem) {
            return;
        }

        /* if not then adjust size marking that we cleared and we're not starting from a new boundary, 8 bit aligned */
        range_size -= clearbitsz;
        range_begin += clearbitsz;
    }

    /* align ending of clear range to 8 bit boundary */
    rem = ((range_begin + range_size) & 7); /* now checking whether range ends between a byte */
    if(rem) {
        Size pos = ((range_begin + range_size) >> 3);
        bv->data[pos] |= (-1U << (8-rem));

        /* if we've clear all bits then exit */
        if(range_size <= rem) {
            return;
        }

        /* now the range end is aligned to 8 bit boundary as well */
        range_size -= rem;
    }

    /* if more bits need to be cleared, we can directly perform a memset, since everything is 8 bit aligned. */
    if(range_size) {
        memset(bv->data + (range_begin >> 3), 1, range_size >> 3); /* do not adjust size by 1 in this case, because now size is 8 bit aligned */
    }
}

/**
 * Clear all bits in a given range.
 * @param bv
 * @param range_begin
 * @param range_size
 * */
void bitvector_clear_range(BitVector* bv, Size range_begin, Size range_size) {
    RETURN_IF_FAIL(bv && range_size, ERR_INVALID_ARGUMENTS);

    Size rem = (range_begin & 7); /* remainder after dividing by 8 */

    /* align beginning of clear range to 8 bit boundary*/
    /* if range begins from middle of a byte, then clear from the beginning, for that byte only */
    if(rem) {
        /* clear bits very selectively */
        Uint8 clearbitsz = MIN(range_size, 8-rem);
        for(Uint8 s = 0; s < clearbitsz; s++) {
            bitvector_clear(bv, range_begin + s);
        }

        /* if range size fits between starting of clear region and ending of the byte, then make an early exit */
        if(range_size <= 8-rem) {
            return;
        }

        /* if not then adjust size marking that we cleared and we're not starting from a new boundary, 8 bit aligned */
        range_size -= clearbitsz;
        range_begin += clearbitsz;
    }

    /* align ending of clear range to 8 bit boundary */
    rem = ((range_begin + range_size) & 7); /* now checking whether range ends between a byte */
    if(rem) {
        Size pos = ((range_begin + range_size) >> 3);
        bv->data[pos] &= ~(-1U << (8-rem));

        /* if we've clear all bits then exit */
        if(range_size <= rem) {
            return;
        }

        /* now the range end is aligned to 8 bit boundary as well */
        range_size -= rem;
    }

    /* if more bits need to be cleared, we can directly perform a memset, since everything is 8 bit aligned. */
    if(range_size) {
        memset(bv->data + (range_begin >> 3), 0, range_size >> 3); /* do not adjust size by 1 in this case, because now size is 8 bit aligned */
    }
}

#define LOGXOR(v1, v2) ((v1) ^ (v2))
#define LOGAND(v1, v2) ((v1) & (v2))
#define LOGOR(v1, v2) ((v1) | (v2))
#define LOGXNOR(v1, v2) (~((v1) ^ (v2)))
#define LOGNAND(v1, v2) (~((v1) & (v2)))
#define LOGNOR(v1, v2) (~((v1) | (v2)))

#define OPERATE_ON_BIT_VECTORS(bv1, bv2, OP)                            \
    do {                                                                \
        /* Check for valid arguments */                                 \
        RETURN_VALUE_IF_FAIL(bv1 && bv2, NULL, ERR_INVALID_ARGUMENTS);  \
                                                                        \
        /* Calculate minimum and maximum lengths */                     \
        Size minlen = MIN(bv1->length, bv2->length);                    \
        Size maxlen = MAX(bv1->length, bv2->length);                    \
                                                                        \
        /* Create a BitVector to store the result of the operation */   \
        BitVector* bvres = bitvector_create();                          \
        bitvector_resize(bvres, maxlen);                                \
        RETURN_VALUE_IF_FAIL(bvres, NULL, "Failed to create a BitVector to store result of operation.\n"); \
                                                                        \
        Uint64* dres = (Uint64*)bvres->data;                            \
        Uint64* d1 = (Uint64*)bv1->data;                                \
        Uint64* d2 = (Uint64*)bv2->data;                                \
                                                                        \
        /* Perform operation on 64-bit blocks */                        \
        /* we can compute common 64 bit blocks this way because
         * it's guaranteed that capacity of vector is always in
         * multiples of a power of 2, defined already, and the bits
         * after length and before capacity are always 0, which makes
         * sure that taking xor with a little extra won't change
         * the desired result */                                        \
        Size num64bitblocks = BITVECTOR_NEXT_INCREMENTED_LENGTH(minlen >> 3) >> 3; \
        for (Size s = 0; s < num64bitblocks; s++) {                     \
            *dres++ = OP(*d1++, *d2++);                                 \
        }                                                               \
                                                                        \
        /* Handle remaining data, copy from the BitVector with the
         * maximum length */                                            \
        Uint64* d = (maxlen == bv1->length) ? d1 : d2;                  \
        for (Size s = num64bitblocks; s < (maxlen >> 6); s++) {         \
            *dres++ = *d++;                                             \
        }                                                               \
        return bvres;                                                   \
    } while (0)

/**
 * Take logical XOR of two @c BitVector values and
 * return the resulting bitvector.
 *
 * The provided @c BitVector values need not be of same length.
 * If the lengths are unequal then the remaining bits
 * of smaller @c BitVector would be assumed to be 0 (cleared/unset).
 * This allows @c BitVector to work with values of unequal
 * lengths.
 *
 * The new, resultant @c BitVector's length will be the maximum
 * of length of both the @c BitVectors
 *
 * @param bv1
 * @param bv2
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* NULL on failure.
 * */
BitVector* bitvector_xor(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, LOGXOR);
}

/**
 * Take logical AND of two @c BitVector values and
 * return the resulting bitvector.
 *
 * The provided @c BitVector values need not be of same length.
 * If the lengths are unequal then the remaining bits
 * of smaller @c BitVector would be assumed to be 0 (cleared/unset).
 * This allows @c BitVector to work with values of unequal
 * lengths.
 *
 * The new, resultant @c BitVector's length will be the maximum
 * of length of both the @c BitVectors
 *
 * @param bv1
 * @param bv2
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* NULL on failure.
 * */
BitVector* bitvector_and(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, LOGAND);
}

/**
 * Take logical OR of two @c BitVector values and
 * return the resulting bitvector.
 *
 * The provided @c BitVector values need not be of same length.
 * If the lengths are unequal then the remaining bits
 * of smaller @c BitVector would be assumed to be 0 (cleared/unset).
 * This allows @c BitVector to work with values of unequal
 * lengths.
 *
 * The new, resultant @c BitVector's length will be the maximum
 * of length of both the @c BitVectors
 *
 * @param bv1
 * @param bv2
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* NULL on failure.
 * */
BitVector* bitvector_or(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, LOGOR);
}

/**
 * Take logical XNOR of two @c BitVector values and
 * return the resulting bitvector.
 *
 * The provided @c BitVector values need not be of same length.
 * If the lengths are unequal then the remaining bits
 * of smaller @c BitVector would be assumed to be 0 (cleared/unset).
 * This allows @c BitVector to work with values of unequal
 * lengths.
 *
 * The new, resultant @c BitVector's length will be the maximum
 * of length of both the @c BitVectors
 *
 * @param bv1
 * @param bv2
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* NULL on failure.
 * */
BitVector* bitvector_xnor(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, LOGXNOR);
}

/**
 * Take logical NAND of two @c BitVector values and
 * return the resulting bitvector.
 *
 * The provided @c BitVector values need not be of same length.
 * If the lengths are unequal then the remaining bits
 * of smaller @c BitVector would be assumed to be 0 (cleared/unset).
 * This allows @c BitVector to work with values of unequal
 * lengths.
 *
 * The new, resultant @c BitVector's length will be the maximum
 * of length of both the @c BitVectors
 *
 * @param bv1
 * @param bv2
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* NULL on failure.
 * */
BitVector* bitvector_nand(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, LOGNAND);
}

/**
 * Take logical NOR of two @c BitVector values and
 * return the resulting bitvector.
 *
 * The provided @c BitVector values need not be of same length.
 * If the lengths are unequal then the remaining bits
 * of smaller @c BitVector would be assumed to be 0 (cleared/unset).
 * This allows @c BitVector to work with values of unequal
 * lengths.
 *
 * The new, resultant @c BitVector's length will be the maximum
 * of length of both the @c BitVectors
 *
 * @param bv1
 * @param bv2
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* NULL on failure.
 * */
BitVector* bitvector_nor(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, LOGNOR);
}

#undef OPERATE_ON_BIT_VECTORS
#undef LOGXNOR
#undef LOGNAND
#undef LOGNOR
#undef LOGOR
#undef LOGAND
#undef LOGXOR

/**
 * Compute logical NOT of two @c BitVector values and
 * return the resulting @c BitVector.
 * This logical not is only for the length of @c BitVector,
 * not for the whole capacity. Capacity is always greater
 * than or equal to @c BitVector's length. If length of
 * @c BitVector is 128, it's capcity is 256, then not,
 * of only first 128 bits are taken. The remaining 128
 * bits remain unset in both given and resultant @c BitVector.
 *
 * @param bv
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* NULL on failure.
 * */
BitVector* bitvector_not(BitVector* bv) {
    RETURN_VALUE_IF_FAIL(bv, NULL, ERR_INVALID_ARGUMENTS);

    /* create new bitvector to store result */
    BitVector* notbv = bitvector_create();
    RETURN_VALUE_IF_FAIL(notbv, NULL, ERR_INVALID_ARGUMENTS);
    bitvector_reserve(notbv, bv->length);

    Uint64* dres = (Uint64*)notbv->data;
    Uint64* d    = (Uint64*)bv->data;

    Size num64bitblocks = bv->length >> 6;
    for(Size s = 0; s < num64bitblocks; s++) {
        *dres++ = ~*d++;
    }

    return notbv;
}

/**
 * Perform shift-left (<<) operation on given @c BitVector.
 * Length of the resulting @c BitVector will exactly same as
 * that of given @c BitVector @p bv.
 * @param bv
 * @param index
 */
BitVector* bitvector_shl(BitVector* bv, Size index) {
    RETURN_VALUE_IF_FAIL(bv, NULL, ERR_INVALID_ARGUMENTS);

    BitVector* newbv = bitvector_create();
    RETURN_VALUE_IF_FAIL(newbv, NULL, "Failed to create BitVector to store result of operation\n");
    bitvector_reserve(newbv, bv->length - index + 8);

    Uint8 rem = (index & 7); /* remainder when index is divided by 8 */

    if(rem) {
        Uint8 mask1 = (-1U) << (8 - rem); /* to select lower (rem) bits at once */
        Uint8 mask2 = (-1U) >> rem; /* to select higher (8-rem) bits at once */

        Size pos = (index >> 3);
        Size end = (bv->length - index) >> 3;
        for(Size s = 0; s < end; s++) {
            newbv->data[s] |= (bv->data[pos] & mask2) << rem; /* select high 8-rem bits and shift them down */
            newbv->data[s] |= (bv->data[pos+1] & mask1) >> (8-rem); /* select low rem bits from next byte and shift them up */
            pos++;
        }
    } else {
        /* if bit shift is aligned to 8 bit boundary, then direct memcpy is fast */
        memcpy(newbv->data, bv->data + (index >> 8), 1 + ((bv->length - index) >> 3));
    }

    /* keep the length same as that of given bitvector. */
    newbv->length = bv->length;

    return newbv;
}

/**
 * Perform shift-right (>>) operation on given @c BitVector.
 * @param bv
 * @param index
 * */
BitVector* bitvector_shr(BitVector* bv, Size index) {
    RETURN_VALUE_IF_FAIL(bv, NULL, ERR_INVALID_ARGUMENTS);

    BitVector* newbv = bitvector_create();
    RETURN_VALUE_IF_FAIL(newbv, NULL, "Failed to create BitVector to store result of operation\n");
    bitvector_reserve(newbv, bv->length + index + 8); /* need space for extra 8 bits for cases when new length escapes boundary */

    Uint8 rem = (index & 7); /* remainder when index is divided by 8 */

    if(rem) {
        Uint8 mask1 = (-1U) >> (8 - rem); /* to select higher (rem) bits at once */
        Uint8 mask2 = (-1U) << rem; /* to select lower (8-rem) bits at once */

        Size pos = (index >> 3);
        Size end = bv->length >> 3;
        for(Size s = 0; s < end; s++) {
            newbv->data[pos]   |= (bv->data[s] & mask2) >> rem; /* select lower 8-rem bits and place them first */
            newbv->data[pos+1] |= (bv->data[s] & mask1) << (8-rem); /* select remaining higher rem bits and place them after */
            pos++;
        }
    } else {
        /* if bit shift is aligned to 8 bit boundary, then direct memcpy is fast */
        memcpy(newbv->data + (index >> 3), bv->data, 1 + (bv->length >> 3));
    }

    newbv->length = bv->length + index;

    return newbv;
}

/**
 * Compare whether or not given two @c BitVector values
 * are equal.
 *
 * If length of both @c BitVector values are not equal,
 * then the implementation assumes remaining
 * @param bv1
 * @param bv2
 * @return True if bv1 and bv2 are equal.
 * @return False otherwise.
 * */
Bool bitvector_cmpeq(BitVector* bv1, BitVector* bv2) {
    RETURN_VALUE_IF_FAIL(bv1 && bv2, False, ERR_INVALID_ARGUMENTS);

    Uint64* d1 = (Uint64*)bv1->data;
    Uint64* d2 = (Uint64*)bv2->data;

    /* compare 8n blocks first */
    Size minlen = MIN(bv1->length, bv2->length);
    Size commonlen = 1 + (minlen >> 3); /* number of bytes common between the two, always atleat 1 byte is common */
    if(memcmp(d1, d2, commonlen) != 0) {
        return False;
    }

    /* no need to compare further of both bitvectors are of same length */
    if(bv1->length == bv2->length) {
        return True;
    }

    /* check if remaining bits in bitvector of maxm length are 0 or not */
    Uint8* d = (Uint8*)((minlen == bv1->length) ? d2 : d1); /* bv data of maxm length */
    Size max8bitblocks = MAX(bv1->length, bv2->length) >> 3;
    for(Size s = commonlen; s < max8bitblocks; s++) {
        if(d[s]) return False;
    }

    return True;
}
