/**
 * @file bitvec_shl.c
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
 * @brief Unit test for bitvec_shl in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>

#include "drop_in_replacements.h"
#include "helpers.h"

TEST_FN Bool Shl_CHECK_REDUNDANCY_WHEN_SIZE_IS_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* shift right 13 bits */
    Size shllen = 13;
    BitVector* bvres = bitvec_shl(bv, shllen);
    TEST_OBJECT(bvres);

    TEST_DATA_PTR(bvres->data);
    TEST_LENGTH_EQ(bvres->length, 0);
    TEST_CAPACITY_GE(bvres->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data, DIV8(bvres->capacity), 0));

    TEST_DATA_PTR(bv->data)
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(bv->capacity), 0));

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Shl_WHEN_LENGTH_IS_NOT_8BIT_ALIGNED() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    bv->length = 13;
    Uint8 m1 = CREATE_MASK8(1, 1, 1, 0, 0, 0, 0, 0);
    Uint8 m2 = CREATE_MASK8(0, 0, 0, 1, 1, 1, 1, 1); /* total 13 bits */
    bv->data[0] = m1;
    bv->data[1] = m2;

    /* shift left 13 bits */
    Size shlsz = 5;
    BitVector* bvres = bitvec_shl(bv, shlsz);
    TEST_OBJECT(bvres);

    /* test bvres */
    TEST_DATA_PTR(bvres->data);
    TEST_LENGTH_EQ(bvres->length, bv->length - shlsz);
    TEST_CAPACITY_GE(bvres->capacity, oc);
    TEST_CONTENTS(bvres->data[0] == 0xff); /* after shifting all bits are flagged in first byte */
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data + 1, DIV8(bvres->capacity) - 1, 0));

    /* test bv */
    TEST_DATA_PTR(bv->data)
    TEST_LENGTH_EQ(bv->length, 13);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS((bv->data[0] == m1) && (bv->data[1] == m2));
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data + 2, DIV8(bv->capacity) - 2, 0));

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Shl_WHEN_LENGTH_IS_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    bv->length = oc;
    memset(bv->data, 0xff, DIV8(oc));

    /* shift left 24 bits */
    Size shllen = 24;
    BitVector* bvres = bitvec_shl(bv, shllen);
    TEST_OBJECT(bvres);

    Size shlsz = DIV8(shllen);
    Size xffsz = DIV8(bv->capacity - shllen);

    /* test bvres */
    TEST_DATA_PTR(bvres->data);
    TEST_LENGTH_EQ(bvres->length, bv->length - shllen);
    TEST_CAPACITY_GE(bvres->capacity, bv->length - shllen);
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data, xffsz, 0xff));
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data + xffsz, shlsz, 0));

    /* test bv */
    TEST_DATA_PTR(bv->data)
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(bv->capacity), 0xff));

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
        if(bvres) bitvec_destroy(bvres);
    );
}

BEGIN_TESTS(bitvec_shl)
    TEST(Shl_CHECK_REDUNDANCY_WHEN_SIZE_IS_ZERO),
    TEST(Shl_WHEN_LENGTH_IS_NOT_8BIT_ALIGNED),
    TEST(Shl_WHEN_LENGTH_IS_CAPACITY)
END_TESTS()
