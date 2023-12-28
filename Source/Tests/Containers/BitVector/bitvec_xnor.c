/**
 * @file bitvec_xnor.c
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
 * @brief Unit test for bitvec_xnor in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

#include "drop_in_replacements.h"

TEST_FN Bool Xnor_WHEN_LENGTH_IS_NOT_ALIGED_TO_8BIT() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    /* using masks here just to show off, can use any value in place of this */

    Uint8 m11 = CREATE_MASK8(1, 1, 0, 1, 0, 0, 1, 1);
    Uint8 m12 = CREATE_MASK8(1, 1, 0, 0, 0, 0, 0, 0);
    Uint8 m21 = CREATE_MASK8(0, 1, 0, 0, 1, 0, 0, 1);
    Uint8 m22 = CREATE_MASK8(0, 1, 0, 0, 0, 0, 0, 0);

    Size sz = 13;
    resizebv(bv1, sz);
    bv1->data[0] = m11;
    bv1->data[1] = m12;
    resizebv(bv2, sz);
    bv2->data[1] = m21;
    bv2->data[1] = m22;

    BitVector* bvres = bitvec_xnor(bv1, bv2);

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
    TEST_CONTENTS(bvres->data[0] = XNOR(m11, m21));
    TEST_CONTENTS(bvres->data[1] = XNOR(m12, m22));

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Xnor_WHEN_LENGTH_IS_ZERO() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    memset(bv1->data, 0xab, DIV8(bv1->capacity));
    memset(bv2->data, 0xcd, DIV8(bv2->capacity));

    BitVector* bvres = bitvec_xnor(bv1, bv2);
    TEST_OBJECT(bvres);

    TEST_DATA_PTR(bvres->data && bv1->data && bv2->data);
    TEST_LENGTH_EQ(bvres->length, bv1->length);
    TEST_LENGTH_EQ(bvres->capacity, bv1->capacity);

    /* test bv1 */
    for(Size s = 0; s < DIV8(bv1->capacity); s++) {
        TEST_CONTENTS(bv1->data[s] == 0xab);
    }
    /* test bv2 */
    for(Size s = 0; s < DIV8(bv2->capacity); s++) {
        TEST_CONTENTS(bv2->data[s] == 0xcd);
    }
    /* test bvres */
    for(Size s = 0; s < DIV8(bvres->capacity); s++) {
        TEST_CONTENTS(bvres->data[s] == 0x00);
    }

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Xnor_WHEN_LENGTH_IS_NON_ZERO_NOT_EQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    Size sz = 32;
    resizebv(bv1, sz); /* make unequal sizes */
    resizebv(bv2, DIV2(sz));
    memset(bv1->data, 0x88, DIV8(sz)); /* set some data to check later */
    memset(bv2->data, 0x11, DIV16(sz)); /* set some data to check later */

    /* and of 16 bit value with 32 bit value, where one is 0xaaaaaaaa and other is 0x0000 */
    BitVector* bvres = bitvec_xnor(bv1, bv2);

    TEST_OBJECT(bvres);
    TEST_DATA_PTR(bvres->data && bv1->data && bv2->data);
    TEST_LENGTH_EQ(bvres->length, MAX(bv1->length, bv2->length));
    TEST_LENGTH_EQ(bvres->capacity, MAX(bv1->capacity, bv2->capacity));


    /* check bv1 */
    for(Size s = 0; s < DIV8(sz); s++) {
        TEST_CONTENTS(bv1->data[s] == 0x88);
    }
    for(Size s = DIV8(sz); s < DIV8(bv1->capacity); s++) {
        TEST_CONTENTS(bv1->data[s] == 0x00);
    }
    /* check bv2 */
    for(Size s = 0; s < DIV16(sz); s++) {
        TEST_CONTENTS(bv2->data[s] == 0x11);
    }
    for(Size s = DIV16(sz); s < DIV8(bv2->capacity); s++) {
        TEST_CONTENTS(bv2->data[s] == 0x00);
    }
    /* check bvres */
    for(Size s = 0; s < DIV16(sz); s++) {
        TEST_CONTENTS(bvres->data[s] == 0x99);
    }
    for(Size s = DIV16(sz); s < DIV8(bvres->capacity); s++) {
        TEST_CONTENTS(bvres->data[s] == 0x00);
    }

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Xnor_WHEN_LENGTH_IS_EQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1 && bv2);

    Size sz = 32;
    resizebv(bv1, sz);
    resizebv(bv2, sz);
    memset(bv1->data, 0xf1, DIV8(sz));
    memset(bv2->data, 0x1f, DIV8(sz));

    /* and if 16 bit value with 32 bit value, both containing same data */
    BitVector* bvres = bitvec_xnor(bv1, bv2);

    TEST_OBJECT(bvres);
    TEST_DATA_PTR(bvres->data && bv1->data && bv2->data);
    TEST_LENGTH_EQ(bvres->length, MAX(bv1->length, bv2->length));
    TEST_LENGTH_EQ(bvres->capacity, MAX(bv1->capacity, bv2->capacity));

    /* check bv1 */
    for(Size s = 0; s < DIV8(sz); s++) {
        TEST_CONTENTS(bv1->data[s] == 0xf1);
    }
    for(Size s = sz; s < DIV8(bv1->capacity); s++) {
        TEST_CONTENTS(bv1->data[s] == 0x00);
    }
    /* check bv2 */
    for(Size s = 0; s < DIV8(sz); s++) {
        TEST_CONTENTS(bv2->data[s] == 0x1f);
    }
    for(Size s = sz; s < DIV8(bv1->capacity); s++) {
        TEST_CONTENTS(bv2->data[s] == 0x00);
    }
    /* check bvres */
    for(Size s = 0; s < DIV8(sz); s++) {
        TEST_CONTENTS(bvres->data[s] == 0xff);
    }
    for(Size s = sz; s < DIV8(bv1->capacity); s++) {
        TEST_CONTENTS(bvres->data[s] == 0x00);
    }

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
        if(bvres) bitvec_destroy(bvres);
    );
}

BEGIN_TESTS(bitvec_xnor)
    TEST(Xnor_WHEN_LENGTH_IS_NOT_ALIGED_TO_8BIT),
    TEST(Xnor_WHEN_LENGTH_IS_ZERO),
    TEST(Xnor_WHEN_LENGTH_IS_NON_ZERO_NOT_EQUAL),
    TEST(Xnor_WHEN_LENGTH_IS_EQUAL)
END_TESTS()
