/**
 * @file bitvec_xor.c
 * @date Tue, 26th December, 2023
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
 * @brief Unit test for bitvec_xor in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

#include "drop_in_replacements.h"
#include "helpers.h"

TEST_FN Bool Xor_WHEN_LENGTH_IS_NOT_ALIGED_TO_8BIT() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    /* the masks are created such that xor of values create 0xff */

    Uint8 m11 = CREATE_MASK8(1, 0, 1, 1, 0, 1, 1, 0);
    Uint8 m12 = CREATE_MASK8(1, 0, 1, 0, 0, 0, 0, 0);
    Uint8 m21 = CREATE_MASK8(0, 1, 0, 0, 1, 0, 0, 1);
    Uint8 m22 = CREATE_MASK8(0, 1, 0, 0, 0, 0, 0, 0);

    Size sz = 13;
    resizebv(bv1, sz);
    bv1->data[0] = m11;
    bv1->data[1] = m12;
    resizebv(bv2, sz);
    bv2->data[1] = m21;
    bv2->data[1] = m22;

    BitVector* bvres = bitvec_xor(bv1, bv2);

    TEST_OBJECT(bvres);
    TEST_DATA_PTR(bvres->data && bv1->data && bv2->data);
    TEST_LENGTH_EQ(bvres->length, bv1->length);
    TEST_LENGTH_EQ(bvres->capacity, bv1->capacity);

    /* test bv1 */
    TEST_CONTENTS(bv1->data[0] == m11);
    TEST_CONTENTS(bv1->data[1] == m12);
    /* test bv2 */
    TEST_CONTENTS(bv2->data[0] == m21);
    TEST_CONTENTS(bv2->data[1] == m22);
    /* test bvres */
    TEST_CONTENTS(bvres->data[0] = XOR(m11, m21));
    TEST_CONTENTS(bvres->data[1] = XOR(m12, m22));

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Xor_WHEN_LENGTH_IS_ZERO() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    memset(bv1->data, 0xab, DIV8(bv1->capacity));
    memset(bv2->data, 0xcd, DIV8(bv2->capacity));

    BitVector* bvres = bitvec_xor(bv1, bv2);
    TEST_OBJECT(bvres);

    TEST_DATA_PTR(bvres->data && bv1->data && bv2->data);
    TEST_LENGTH_EQ(bvres->length, bv1->length);
    TEST_LENGTH_EQ(bvres->capacity, bv1->capacity);

    for(Size s = 0; s < DIV8(bv1->capacity); s++) {
        TEST_CONTENTS(bv1->data[s] == 0xab);
    }
    for(Size s = 0; s < DIV8(bv2->capacity); s++) {
        TEST_CONTENTS(bv2->data[s] == 0xcd);
    }
    for(Size s = 0; s < DIV8(bvres->capacity); s++) {
        TEST_CONTENTS(bvres->data[s] == 0x00);
    }

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Xor_WHEN_LENGTH_IS_NON_ZERO_NOT_EQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    Size sz1 = 32, sz2 = DIV2(sz1);
    bv1->length = sz1; /* make unequal sizes */
    bv2->length = sz2;
    memset(bv1->data, 0xaa, DIV8(sz1)); /* set some data to check later */

    /* xor if 16 bit value with 32 bit value, where one is 0xaaaaaaaa and other is 0x0000 */
    BitVector* bvres = bitvec_xor(bv1, bv2);

    TEST_OBJECT(bvres);
    TEST_DATA_PTR(bvres->data && bv1->data && bv2->data);
    TEST_LENGTH_EQ(bv1->length, sz1);
    TEST_LENGTH_EQ(bv2->length, sz2);
    TEST_LENGTH_EQ(bvres->length, MAX(bv1->length, bv2->length));
    TEST_LENGTH_EQ(bvres->capacity, MAX(bv1->capacity, bv2->capacity));

    /* check bv1 */
    TEST_CONTENTS(is_memory_filled_with_byte(bv1->data, DIV8(sz1), 0xaa));
    TEST_CONTENTS(is_memory_filled_with_byte(bv1->data + DIV8(sz1), DIV8(bv1->capacity - sz1), 0x00));
    /* check bv2 */
    TEST_CONTENTS(is_memory_filled_with_byte(bv2->data, DIV8(bv2->capacity), 0x00));
    /* test bvres */
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data, DIV8(sz2), XOR(0xaa, 0x00))); /* 0xaa */
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data + DIV8(sz2), DIV8(bvres->capacity - sz2), 0x00));

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Xor_WHEN_LENGTH_IS_EQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    Size sz = 32;
    bv1->length = sz;
    bv2->length = sz;
    memset(bv1->data, 0xa0, DIV8(sz));
    memset(bv2->data, 0x0a, DIV8(sz));

    /* xor if 16 bit value with 32 bit value, both containing same data */
    BitVector* bvres = bitvec_xor(bv1, bv2);

    TEST_OBJECT(bvres);
    TEST_DATA_PTR(bvres->data && bv1->data && bv2->data);
    TEST_LENGTH_EQ(bvres->length, MAX(bv1->length, bv2->length));
    TEST_LENGTH_EQ(bvres->capacity, MAX(bv1->capacity, bv2->capacity));

    /* check bv1 */
    TEST_CONTENTS(is_memory_filled_with_byte(bv1->data, DIV8(sz), 0xa0));
    TEST_CONTENTS(is_memory_filled_with_byte(bv1->data + DIV8(sz), DIV8(bv1->capacity - sz), 0x00));
    /* check bv2 */
    TEST_CONTENTS(is_memory_filled_with_byte(bv2->data, DIV8(sz), 0x0a));
    TEST_CONTENTS(is_memory_filled_with_byte(bv2->data + DIV8(sz), DIV8(bv2->capacity - sz), 0x00));
    /* test bvres */
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data, DIV8(sz), XOR(0xa0, 0x0a))); /* 0xaa */
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data + DIV8(sz), DIV8(bvres->capacity - sz), 0x00));

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

BEGIN_TESTS(bitvec_xor)
    TEST(Xor_WHEN_LENGTH_IS_NOT_ALIGED_TO_8BIT),
    TEST(Xor_WHEN_LENGTH_IS_ZERO),
    TEST(Xor_WHEN_LENGTH_IS_NON_ZERO_NOT_EQUAL),
    TEST(Xor_WHEN_LENGTH_IS_EQUAL)
END_TESTS()
