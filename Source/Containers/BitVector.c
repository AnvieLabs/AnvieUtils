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
#include <Anvie/Bit/Bit.h>
#include <Anvie/Error.h>
#include <string.h>

/**
 * Always keep this in power of 2, also greater than equal to 8,
 * like 8, 16, 32, 64, 128 and so on...
 * Each time bitvector is resized to store more booleans, the size is
 * incremented by this value. Meaning, the capacity of bitvector is always
 * in multiples of this value. */
#define BITVEC_DEFAULT_INCREMENT_SIZE 32 /* 256 bits at once */

/**
 * Macro to generate next multiple of increment size.
 * This is where we need the @c BITVEC_DEFAULT_INCREMENT_SIZE
 * to be a power of two. This will help us quickly compute next
 * multiple values.
 * @param bytelen Length of BitVector in terms of bytes
 * */
#define BITVEC_NEXT_INCREMENTED_LENGTH(bytelen)                      \
    (((bytelen) & (~(BITVEC_DEFAULT_INCREMENT_SIZE - 1))) + BITVEC_DEFAULT_INCREMENT_SIZE)

/**
 * Create a new @c BitVector.
 * @return BitVector* A valid pointer on success.
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_create() {
    BitVector* bv = NEW(BitVector);
    ERR_RETURN_VALUE_IF_FAIL(bv, INVALID_BITVECTOR, ERR_OUT_OF_MEMORY);

    bv->data = ALLOCATE(Uint8, BITVEC_DEFAULT_INCREMENT_SIZE);
    if(!bv->data) {
        FREE(bv);
        ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
    }

    bv->capacity = MUL8(BITVEC_DEFAULT_INCREMENT_SIZE);

    return bv;
}

/**
 * Destroy given @c BitVector.
 * @param bv @c BitVector.
 * */
void bitvec_destroy(BitVector* bv) {
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

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
 * @return BitVector* @c INVALID_BITVECTOR otherwise.
 * */
BitVector* bitvec_clone(BitVector* bv) {
    ERR_RETURN_VALUE_IF_FAIL(bv, INVALID_BITVECTOR, ERR_INVALID_ARGUMENTS);

    BitVector* bvclone = bitvec_create();
    ERR_RETURN_VALUE_IF_FAIL(bvclone, INVALID_BITVECTOR, ERR_INVALID_OBJECT);

    bitvec_resize(bvclone, bv->length);
    memcpy(bvclone->data, bv->data, DIV8(bv->capacity));

    return bvclone;
}

/**
 * Make @p dstbv and @p srcbv equal, i.e., @p dstbv = @p srcbv.
 * The implementation does not care about the length of
 * both @c BitVector values. The contents of @p dstbv in
 * the end will match @p srcbv. The capacity is not guaranteed
 * to change in same manner, just the content is guaranteed to
 * be exactly same.
 *
 * @param dstbv
 * @param srcbv
 * */
void bitvec_set_equal(BitVector* dstbv, BitVector* srcbv) {
    ERR_RETURN_IF_FAIL(dstbv && srcbv, ERR_INVALID_ARGUMENTS);

    /* resize as well as clear contents after length boundary in dstbv */
    bitvec_resize(dstbv, srcbv->length);

    /* if length is not 8bit aligned, then set last byte carefully */
    Size sz = DIV8(srcbv->length);
    Uint8 rem = MOD8(srcbv->length);
    if(rem) {
        dstbv->data[sz + 1] = srcbv->data[sz + 1];
    }
    memcpy(dstbv->data, srcbv->data, sz);
}

/**
 * Reserve space to store @p numbools number of booleans.
 * This results in capacity increase of @c BitVector but not
 * increase in length.
 *
 * @param bv
 * @param numbools Number of booleans.
 * */
void bitvec_reserve(BitVector* bv, Size numbools) {
    ERR_RETURN_IF_FAIL(bv && numbools, ERR_INVALID_ARGUMENTS);

    /* no change in size of new reserved size is less than current capacity */
    if(numbools < bv->capacity) {
        return;
    }

    /* recompute new size and resize */
    Size newsz = BITVEC_NEXT_INCREMENTED_LENGTH(DIV8(numbools)); /* next multiple */
    Uint8* tmp = realloc(bv->data, newsz);
    ERR_RETURN_IF_FAIL(tmp, ERR_OUT_OF_MEMORY);
    bv->data = tmp;

    /* clear all bits in allocated new memory */
    Size ol = bv->length;
    bitvec_clear_range(bv, bv->capacity, MUL8(newsz) - bv->capacity);
    bv->length = ol;

    /* since capacity indicates total number of booleans, we multiply by 8 */
    bv->capacity = MUL8(newsz);
}

/**
 * Resize this @c BitVector to store @p numbools number of booleans.
 * This will result in capacity as well as length increase.
 * Current implementation of @c BitVector does not decrease
 * capacity, even if length goes to 0.
 *
 * @p numbools must be strictly less than (1 << 63).
 *
 * @param bv
 * @param numbools Number of booleans.
 * */
void bitvec_resize(BitVector* bv, Size numbools) {
    ERR_RETURN_IF_FAIL(bv && numbools, ERR_INVALID_ARGUMENTS);

    if(numbools < bv->length) {
        bitvec_clear_range(bv, numbools, bv->length - numbools);
    } else if(numbools > bv->capacity) {
        bitvec_reserve(bv, numbools);
    }

    bv->length = numbools;
}

/**
 * Push back a boolean value to the back of @c BitVector.
 * @param bv @c BitVector.
 * @param val @c Bool value to be pushed.
 * */
void bitvec_push(BitVector* bv, Bool val) {
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    if(bv->length >= bv->capacity) {
        bitvec_reserve(bv, bv->length + 1); /* create space to store just one more bool */
    }

    Size pos = DIV8(bv->length);
    Size rem = MOD8(bv->length);
    if(val) {
        SET8_BIT(bv->data[pos], rem);
    } else {
        CLR8_BIT(bv->data[pos], rem);
    }
    bv->length++;
}

/**
 * Remove last value from @c BitVector.
 * @param bv
 * @return True if last boolean is True.
 * @return False if last boolean is False.
 * */
Bool bitvec_pop(BitVector* bv) {
    ERR_RETURN_VALUE_IF_FAIL(bv, False, ERR_INVALID_ARGUMENTS);

    if(!bv->length) {
        return False;
    }

    bv->length--;
    Size pos = DIV8(bv->length);
    Size rem = MOD8(bv->length);
    Bool v = GET8_BIT(bv->data[pos], rem);
    CLR8_BIT(bv->data[pos], rem);
    return v;
}

/**
 * Get boolean value at given index.
 *
 * @p index must be strictly less than (1 << 63).
 *
 * @param bv
 * @param index Index of boolean to retrieve.
 * @return True if boolean at given index is True.
 * @return False if boolean at given index is False.
 * */
Bool bitvec_peek(BitVector* bv, Size index) {
    ERR_RETURN_VALUE_IF_FAIL(bv, False, ERR_INVALID_ARGUMENTS);

    Size len = DIV8(index);
    Size rem = MOD8(index);
    return GET8_BIT(bv->data[len], rem);
}

/**
 * Set bit/boolean at given index.
 * If bit at given index is already set, then no change
 * will take place (as should be expected).
 * This will automatically resize the @c BitVector
 * to store at least @p index number of booleans if @p index
 * exceeds total length of @c BitVector.
 *
 * @p index must be strictly less than (1 << 63).
 *
 * @param bv
 * @param index Index of bit to be set/flagged.
 * */
inline void bitvec_set(BitVector* bv, Size index) {
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    /* if write position exceeds the capacity, then reserve more space */
    if(index >= bv->capacity) {
        bitvec_reserve(bv, index);
    }

    /* if we wrote directly after the length of bitvector,
     * then length automatically increases */
    if(index >= bv->length) {
        bv->length = index + 1;
    }

    /* set the bit */
    Size pos = DIV8(index);
    Size rem = MOD8(index);
    SET8_BIT(bv->data[pos], rem);
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
inline void bitvec_clear(BitVector* bv, Size index) {
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    /* if write position exceeds the capacity, then reserve more space */
    if(index >= bv->capacity) {
        bitvec_reserve(bv, index);
    }

    /* if we wrote directly after the length of bitvector,
     * then length automatically increases */
    if(index >= bv->length) {
        bv->length = index + 1;
    }

    /* clear the bit */
    Size pos = DIV8(index);
    Size rem = MOD8(index);
    CLR8_BIT(bv->data[pos], rem);
}

/**
 * This will set all boolean(s)/bit(s) in
 * the whole @c BitVector.
 * If length is 0, then call is redundant.
 *
 * @param bv
 * */
void bitvec_set_all(BitVector* bv) {
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);
    if(bv->length) bitvec_set_range(bv, 0, bv->length);
}

/**
 * This will clear/unset all boolean(s)/bit(s) in
 * the whole @c BitVector.
 * If length is 0, then call is redundant.
 *
 * @param bv
 * */
inline void bitvec_clear_all(BitVector* bv) {
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);
    if(bv->length) bitvec_clear_range(bv, 0, bv->length);
}

/**
 * Set all bits in a given range.
 *
 * @p range_begin must be strictly less than (1 << 63).
 * @p range_size must be strictly less than (1 << 63).
 *
 * @param bv
 * @param range_begin
 * @param range_size
 * */
inline void bitvec_set_range(BitVector* bv, Size range_begin, Size range_size) {
    if(!range_size) return;
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    /* reserve more space if needed */
    if(range_begin + range_size > bv->capacity) {
        bitvec_reserve(bv, range_begin + range_size);
    }
    bv->length = MAX(range_begin + range_size, bv->length);

    Size rem = MOD8(range_begin);

    /* align beginning of clear range to 8 bit boundary*/
    /* if range begins from middle of a byte, then clear from the beginning, for that byte only */
    if(rem) {
        /* clear bits very selectively */
        Uint8 setbitsz = MIN(range_size, (8 - rem));
        SET8_RANGE(bv->data[DIV8(range_begin)], rem, setbitsz);

        /* if range size fits between starting of clear region and ending of the byte, then make an early exit */
        if(range_size <= (8 - rem)) {
            return;
        }

        /* if not then adjust size marking that we cleared and we're not starting from a new boundary, 8 bit aligned */
        range_size = MAX(TO_INT64(range_size) - setbitsz, 0);
        range_begin += setbitsz;
    }

    /* align ending of clear range to 8 bit boundary */
    rem = MOD8(range_begin + range_size); /* now checking whether range ends between a byte */
    if(rem) {
        Size pos = DIV8(range_begin + range_size);
        SET8_LO(bv->data[pos], rem);

        /* if we've clear all bits then exit */
        if(range_size <= rem) {
            return;
        }

        /* now the range end is aligned to 8 bit boundary as well */
        range_size = MAX(TO_INT64(range_size) - rem, 0);
    }

    /* if more bits need to be cleared, we can directly perform a memset, since everything is 8 bit aligned. */
    if(range_size) {
        /* do not adjust size by 1 in this case, because now size is 8 bit aligned */
        memset(bv->data + DIV8(range_begin), 0xff, DIV8(range_size));
    }
}

/**
 * Clear all bits in a given range.
 *
 * @p range_begin must be strictly less than (1 << 63).
 * @p range_size must be strictly less than (1 << 63).
 *
 * @param bv
 * @param range_begin
 * @param range_size
 * */
void bitvec_clear_range(BitVector* bv, Size range_begin, Size range_size) {
    if(!range_size) return;
    ERR_RETURN_IF_FAIL(bv, ERR_INVALID_ARGUMENTS);

    /* reserve more space if needed */
    if(range_begin + range_size > bv->capacity) {
        /* calling resize here causes infnite recursion */
        /* bitvec_reserve(bv, range_begin + range_size); */

        /* resize manually here */
        Size newsz = BITVEC_NEXT_INCREMENTED_LENGTH(DIV8(range_begin + range_size)); /* next multiple */
        Uint8* tmp = realloc(bv->data, newsz);
        ERR_RETURN_IF_FAIL(tmp, ERR_OUT_OF_MEMORY);
        bv->data = tmp;
        bv->capacity = MUL8(newsz);
    }
    bv->length = MAX(range_begin + range_size, bv->length);

    Size rem = MOD8(range_begin);

    /* align beginning of clear range to 8 bit boundary */
    /* if range begins from middle of a byte, then clear from the beginning, for that byte only */
    if(rem) {
        /* clear bits very selectively */
        Uint8 clearbitsz = MIN(range_size, (8 - rem));
        CLR8_RANGE(bv->data[DIV8(range_begin)], rem, clearbitsz);

        /* if range size fits between starting of clear region and ending of the byte, then make an early exit */
        if(range_size <= (8 - rem)) {
            return;
        }

        /* if not then adjust size marking that we cleared and we're not starting from a new boundary, 8 bit aligned */
        range_size = MAX(TO_INT64(range_size) - clearbitsz, 0);
        range_begin += clearbitsz;
    }

    /* align ending of clear range to 8 bit boundary */
    rem = MOD8(range_begin + range_size); /* now checking whether range ends between a byte */
    if(rem) {
        Size pos = DIV8(range_begin + range_size);
        CLR8_LO(bv->data[pos], MIN(rem, range_size));

        /* if we've clear all bits then exit */
        if(range_size <= rem) {
            return;
        }

        /* now the range end is aligned to 8 bit boundary as well */
        range_size = MAX(TO_INT64(range_size) - rem, 0);
    }

    /* if more bits need to be cleared, we can directly perform a memset, since everything is 8 bit aligned. */
    if(range_size) {
        memset(bv->data + DIV8(range_begin), 0, DIV8(range_size)); /* do not adjust size by 1 in this case, because now size is 8 bit aligned */
    }
}

#define OPERATE_ON_BIT_VECTORS(bv1, bv2, OP)                            \
    do {                                                                \
        /* Check for valid arguments */                                 \
        ERR_RETURN_VALUE_IF_FAIL((bv1) && (bv2), INVALID_BITVECTOR, ERR_INVALID_ARGUMENTS); \
                                                                        \
        /* Calculate minimum and maximum lengths */                     \
        Size minlen = MIN((bv1)->length, (bv2)->length);                \
        Size maxlen = MAX((bv1)->length, (bv2)->length);                \
                                                                        \
        /* Create a BitVector to store the result of the operation */   \
        BitVector* bvres = bitvec_create();                             \
        bitvec_resize(bvres, maxlen);                                   \
        ERR_RETURN_VALUE_IF_FAIL(bvres, INVALID_BITVECTOR, ERR_INVALID_OBJECT);      \
                                                                        \
        /* if maxlen is 0 then no need to process */                    \
        if(maxlen == 0) {                                               \
            return bvres;                                               \
        }                                                               \
                                                                        \
        /* align minlen and maxlen to upper bound 8 bit boundaries */   \
        if(MOD8(maxlen)) maxlen = ALIGN8_HI(maxlen);                    \
        if(MOD8(maxlen)) minlen = ALIGN8_HI(minlen);                    \
                                                                        \
        Uint8* dres = (bvres)->data;                                    \
        Uint8* d1 = (bv1)->data;                                        \
        Uint8* d2 = (bv2)->data;                                        \
                                                                        \
        for (Size s = 0; s < DIV8(minlen); s++) {                       \
            *dres++ = OP(*d1++, *d2++);                                 \
        }                                                               \
                                                                        \
        /* make early exit if possible */                               \
        if(minlen == maxlen) {                                          \
            return bvres;                                               \
        }                                                               \
                                                                        \
        Uint8* d = ((bv1)->length == maxlen ? d1 : d2);                 \
        for (Size s = DIV8(minlen); s < DIV8(maxlen); s++) {            \
            *dres++ = OP(*d++, 0);                                      \
        }                                                               \
                                                                        \
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
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_xor(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, XOR);
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
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_and(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, AND);
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
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_or(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, OR);
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
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_xnor(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, XNOR);
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
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_nand(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, NAND);
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
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_nor(BitVector* bv1, BitVector* bv2) {
    OPERATE_ON_BIT_VECTORS(bv1, bv2, NOR);
}

#undef OPERATE_ON_BIT_VECTORS

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
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_not(BitVector* bv) {
    ERR_RETURN_VALUE_IF_FAIL(bv, INVALID_BITVECTOR, ERR_INVALID_ARGUMENTS);

    /* create new bitvector to store result */
    BitVector* notbv = bitvec_create();
    ERR_RETURN_VALUE_IF_FAIL(notbv, INVALID_BITVECTOR, ERR_INVALID_ARGUMENTS);

    if(!bv->length) {
        return notbv;
    }

    /* create space to store result*/
    bitvec_resize(notbv, bv->length);

    Uint8* dres = notbv->data;
    Uint8* d    = bv->data;

    /* handle aligned bits */
    for(Size s = 0; s < DIV8(bv->length); s++) {
        *dres++ = ~(*d++);
    }

    /* handle unaligned bits */
    if(MOD8(bv->length)) {
        *dres = GET8_LO(~(*d), MOD8(bv->length));
    }

    return notbv;
}

/**
 * Perform shift-left (<<) operation on given @c BitVector.
 * Length of the resulting @c BitVector will exactly same as
 * that of given @c BitVector @p bv.
 * @p index must be strictly less than (1 << 63).
 *
 * `|  0  |  1  |  2  | . . . | n-1 |  n  | <<- bits will be introduced from this side`
 * New length will be original length + shift index.
 *
 * @param bv.
 * @param index.
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 */
BitVector* bitvec_shl(BitVector* bv, Size index) {
    ERR_RETURN_VALUE_IF_FAIL(bv, INVALID_BITVECTOR, ERR_INVALID_ARGUMENTS);

    /* create bitvec to store result */
    BitVector* newbv = bitvec_create();
    ERR_RETURN_VALUE_IF_FAIL(newbv, INVALID_BITVECTOR, ERR_INVALID_OBJECT);

    /* reserve space to store in new bitvector */
    Int64 newbvlen = TO_INT64(bv->length) - TO_INT64(index);
    if(newbvlen <= 0) {
        return newbv;
    } else {
        bitvec_resize(newbv, newbvlen);
    }

    /* process differently if not 8bit aligned and if 8bit aligned */
    Uint8 rem = MOD8(index);
    if(rem) {
        Size pos = DIV8(index);
        Size sz = DIV8(newbvlen);
        for(Size s = 0; s < sz; s++) {
            newbv->data[s] |= GET8_HI(bv->data[pos], 8-rem); /* rem */ /* get lower 8-rem bits, can use select here also, because it's already adjusted */
            newbv->data[s] |= SHL(GET8_LO(bv->data[pos+1], rem), 8-rem); /* select remaining higher rem bits and place them after */
            pos++;
        }
    } else {
        /* if bit shift is aligned to 8 bit boundary, then direct memcpy is fast */
        memcpy(newbv->data, bv->data + DIV8(index), DIV8(newbvlen));
    }

    return newbv;
}

/**
 * Perform shift-left (<<) operation on given @c BitVector.
 * @p index must be strictly less than (1 << 63).
 *
 * `bits will be introduced from this side -->> |  0  |  1  |  2  | . . . | n-1 |  n  |`
 * New length will be original length + shift index.
 *
 * @param bv
 * @param index
 * @return BitVector* A valid @c BitVector on success.
 * @return BitVector* @c INVALID_BITVECTOR on failure.
 * */
BitVector* bitvec_shr(BitVector* bv, Size index) {
    ERR_RETURN_VALUE_IF_FAIL(bv, INVALID_BITVECTOR, ERR_INVALID_ARGUMENTS);

    BitVector* newbv = bitvec_create();
    ERR_RETURN_VALUE_IF_FAIL(newbv, INVALID_BITVECTOR, ERR_INVALID_OBJECT);
    bitvec_reserve(newbv, bv->length + index + 8); /* need space for extra 8 bits for cases when new length escapes boundary */

    Uint8 rem = MOD8(index); /* remainder when index is divided by 8 */

    if(rem) {
        Size pos = DIV8(index);
        /* adjust size depending on whether or not some extra bits need to be covered */
        Size sz = DIV8(MOD8(bv->length) ? ALIGN8_HI(bv->length) : bv->length);
        for(Size s = 0; s < sz; s++) {
            newbv->data[pos]   |= SHL(GET8_LO(bv->data[s], 8-rem), rem); /* get lower 8-rem bits, (already adjusted) */
            newbv->data[pos+1] |= GET8_HI(bv->data[s], rem); /* 8-rem */ /* (already adjusted), get higher rem bits */
            pos++;
        }
    } else {
        /* if bit shift is aligned to 8 bit boundary, then direct memcpy is fast */
        memcpy(newbv->data + DIV8(index), bv->data, DIV8(bv->length));
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
Bool bitvec_cmpeq(BitVector* bv1, BitVector* bv2) {
    ERR_RETURN_VALUE_IF_FAIL(bv1 && bv2, False, ERR_INVALID_ARGUMENTS);

    Uint64* d1 = (Uint64*)bv1->data;
    Uint64* d2 = (Uint64*)bv2->data;

    /* compare 8bit aligned blocks first */
    Size minlen = MIN(bv1->capacity, bv2->capacity);
    Size commonlen = DIV8(minlen); /* number of bytes common between the two, always atleat 1 byte is common */
    if(memcmp(d1, d2, commonlen) != 0) {
        return False;
    }

    /* no need to compare further of both bitvectors are of same length */
    if(bv1->length == bv2->length) {
        return True;
    }

    /* check if remaining bits in bitvector of maxm length are 0 or not */
    Uint8* d = (Uint8*)((minlen == bv1->length) ? d2 : d1); /* bv data of maxm length */
    Size max8bitblocks = DIV8(MAX(bv1->length, bv2->length));
    for(Size s = commonlen; s < max8bitblocks; s++) {
        if(d[s]) return False;
    }

    return True;
}
