/**
 * @file bitvec_cmpeq.c
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
 * @brief Unit test for bitvec_cmpeq in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

#include "drop_in_replacements.h"
#include "helpers.h"

TEST_FN Bool CmpEq_WHEN_LENGTH_IS_ZERO() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1);
    TEST_OBJECT(bv2);

    Size oc = bv1->capacity;

    TEST_EQUALITY(bitvec_cmpeq(bv1, bv2));

    /* test bv1 */
    TEST_DATA_PTR(bv1->data);
    TEST_LENGTH_EQ(bv1->length, 0);
    TEST_CAPACITY_EQ(bv1->capacity, oc);
    /* test bv2 */
    TEST_DATA_PTR(bv2->data);
    TEST_LENGTH_EQ(bv2->length, 0);
    TEST_CAPACITY_EQ(bv2->capacity, oc);

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
    )
}

TEST_FN Bool CmpEq_WHEN_LENGTH_UNEQUAL_CONTENTS_EQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1);
    TEST_OBJECT(bv2);

    Size oc = bv1->capacity;

    /* length unequal */
    bv1->length = 13;
    bv2->length = 17;

    /* contents equal */
    bv1->data[0] = 0xca; bv1->data[1] = 0xfe;
    bv2->data[0] = 0xca; bv2->data[1] = 0xfe;

    TEST_EQUALITY(bitvec_cmpeq(bv1, bv2) == True);

    /* test bv1 */
    TEST_DATA_PTR(bv1->data);
    TEST_LENGTH_EQ(bv1->length, 13);
    TEST_CAPACITY_EQ(bv1->capacity, oc);
    /* test bv2 */
    TEST_DATA_PTR(bv2->data);
    TEST_LENGTH_EQ(bv2->length, 17);
    TEST_CAPACITY_EQ(bv2->capacity, oc);

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
    )
}

TEST_FN Bool CmpEq_WHEN_LENGTH_UNEQUAL_CONTENTS_UNEQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1);
    TEST_OBJECT(bv2);

    Size oc = bv1->capacity;

    /* length unequal */
    bv1->length = 16;
    bv2->length = 32;

    /* contents equal */
    bv1->data[0] = 0xca; bv1->data[1] = 0xfe;
    bv2->data[0] = 0xca; bv2->data[1] = 0xfd;
    bv2->data[2] = 0xb0; bv2->data[3] = 0x00;

    TEST_EQUALITY(bitvec_cmpeq(bv1, bv2) == False);

    /* test bv1 */
    TEST_DATA_PTR(bv1->data);
    TEST_LENGTH_EQ(bv1->length, 16);
    TEST_CAPACITY_EQ(bv1->capacity, oc);
    /* test bv2 */
    TEST_DATA_PTR(bv2->data);
    TEST_LENGTH_EQ(bv2->length, 32);
    TEST_CAPACITY_EQ(bv2->capacity, oc);

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
    )
}


TEST_FN Bool CmpEq_WHEN_LENGTH_EQUAL_CONTENTS_UNEQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1);
    TEST_OBJECT(bv2);

    Size oc = bv1->capacity;

    /* length equal */
    bv1->length = 32;
    bv2->length = 32;

    /* contents unequal */
    bv1->data[0] = 0xc1; bv1->data[1] = 0x56;
    bv1->data[2] = 0xc0; bv1->data[3] = 0x3d;
    bv2->data[0] = 0xca; bv2->data[1] = 0xf7;
    bv2->data[2] = 0xb0; bv2->data[3] = 0x1d;

    TEST_EQUALITY(bitvec_cmpeq(bv1, bv2) == False);

    /* test bv1 */
    TEST_DATA_PTR(bv1->data);
    TEST_LENGTH_EQ(bv1->length, 32);
    TEST_CAPACITY_EQ(bv1->capacity, oc);
    /* test bv2 */
    TEST_DATA_PTR(bv2->data);
    TEST_LENGTH_EQ(bv2->length, 32);
    TEST_CAPACITY_EQ(bv2->capacity, oc);

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
    )
}

TEST_FN Bool CmpEq_WHEN_LENGTH_EQUAL_CONTENTS_EQUAL() {
    BitVector* bv1 = bitvec_create();
    BitVector* bv2 = bitvec_create();
    TEST_OBJECT(bv1);
    TEST_OBJECT(bv2);

    bv1->length = bv1->capacity;
    bv2->length = bv2->capacity;
    Size oc = bv1->capacity;

    memset(bv1->data, 0xff, DIV8(oc));
    memset(bv2->data, 0xff, DIV8(oc));

    TEST_EQUALITY(bitvec_cmpeq(bv1, bv2));

    /* test bv1 */
    TEST_DATA_PTR(bv1->data);
    TEST_LENGTH_EQ(bv1->length, oc);
    TEST_CAPACITY_EQ(bv1->capacity, oc);
    /* test bv2 */
    TEST_DATA_PTR(bv2->data);
    TEST_LENGTH_EQ(bv2->length, oc);
    TEST_CAPACITY_EQ(bv2->capacity, oc);

    DO_BEFORE_EXIT(
        if(bv1) bitvec_destroy(bv1);
        if(bv2) bitvec_destroy(bv2);
    )
}

BEGIN_TESTS(bitvec_cmpeq)
    TEST(CmpEq_WHEN_LENGTH_IS_ZERO),
    TEST(CmpEq_WHEN_LENGTH_UNEQUAL_CONTENTS_EQUAL),
    TEST(CmpEq_WHEN_LENGTH_UNEQUAL_CONTENTS_UNEQUAL),
    TEST(CmpEq_WHEN_LENGTH_EQUAL_CONTENTS_UNEQUAL),
    TEST(CmpEq_WHEN_LENGTH_EQUAL_CONTENTS_EQUAL)
END_TESTS()
