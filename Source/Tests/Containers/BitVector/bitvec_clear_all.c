/**
 * @file bitvec_clear_all.c
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
 * @brief Unit test for bitvec_clear_all in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

#include "helpers.h"

TEST_FN Bool ClearAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    memset(bv->data, 0xff, DIV8(oc));

    /* clear all bits */
    bitvec_clear_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0xff);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool ClearAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_NON_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    memset(bv->data, 0xff, DIV8(oc));

    /* clear all bits */
    bv->length = oc;
    bitvec_clear_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(oc), 0x00));

    /* clear all bits again, the data must remain same, this is redundancy check */
    bitvec_clear_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(oc), 0x00));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool ClearAll_CLEAR_SINGLE_BIT_WHEN_LENGTH_EQUAL_ONE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    memset(bv->data, 0xff, DIV8(oc));

    /* clear all bits when length is 1 */
    bv->length = 1;
    bitvec_clear_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_GE(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0xfe);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool ClearAll_CLEAR_LARGE_WHEN_LENGTH_IS_NOT_8BIT_ALIGNED() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    memset(bv->data, 0xff, DIV8(oc));

    /* clear all bits when length is not 8 bit aligned */
    Size len = DIV4(bv->capacity) + 11;
    bv->length = len;
    bitvec_clear_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, len);
    TEST_CAPACITY_GE(bv->capacity, len);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(len), 0x00));
    TEST_CONTENTS(bv->data[DIV8(len) + 1] = NOT(MASK_LO(MOD8(len))));
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data + DIV8(len) + 2, DIV8(oc-len) - 2, 0xff));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool ClearAll_WHEN_LENGTH_GREATER_THAN_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    memset(bv->data, 0xff, DIV8(oc));

    /* the test might fail here, this is completely implementation dependent
     * because user is not supposed to alter the length and capacity on their
     * own without using the interface provided for interaction witht he bv */
    Size sz = bv->capacity + 11;
    bv->length = sz;
    bitvec_clear_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, sz);
    TEST_CAPACITY_GE(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(oc), 0x00));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_clear_all)
    TEST(ClearAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_ZERO),
    TEST(ClearAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_NON_ZERO),
    TEST(ClearAll_CLEAR_SINGLE_BIT_WHEN_LENGTH_EQUAL_ONE),
    TEST(ClearAll_CLEAR_LARGE_WHEN_LENGTH_IS_NOT_8BIT_ALIGNED),
    TEST(ClearAll_WHEN_LENGTH_GREATER_THAN_CAPACITY),
END_TESTS()
