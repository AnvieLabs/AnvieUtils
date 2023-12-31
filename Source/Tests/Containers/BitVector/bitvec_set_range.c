/**
 * @file bitvec_set_range.c
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
 * @brief Unit test for bitvec_set_range in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>

#include "helpers.h"

TEST_FN Bool SetRange_CHECK_REDUNDANCY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set bits */
    Size setlen = DIV2(bv->capacity);
    bitvec_set_range(bv, 0, setlen);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, setlen);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(setlen), 0xff));
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data + DIV8(setlen), DIV8(oc) - DIV8(setlen), 0x00));

    /* set bits again, and check for values to be same */
    bitvec_set_range(bv, 0, DIV2(bv->capacity));

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, DIV2(bv->capacity));
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV16(oc), 0xff));
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data + DIV16(oc), DIV16(oc), 0x00));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetRange_WHEN_RANGE_SIZE_EQUAL_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set bits with range size equal 0, this should print an error message */
    bitvec_set_range(bv, 10, 0);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0x00);
    TEST_CONTENTS(bv->data[1] == 0x00);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetRange_WHEN_RANGE_SIZE_EQUAL_ONE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set bits */
    bitvec_set_range(bv, 10, 1);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 11);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0x00);
    TEST_CONTENTS(bv->data[1] == 0x04);
    TEST_CONTENTS(bv->data[2] == 0x00);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetRange_WHEN_RANGE_SIZE_IS_AT_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set bits */
    bitvec_set_range(bv, 0, oc);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(bv->capacity), 0xff));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetRange_WHEN_RANGE_SIZE_IS_GREATER_THAN_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set bits */
    Size start = DIV2(oc);
    Size range = MUL2(oc);
    bitvec_set_range(bv, start, range);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, range + start);
    TEST_CAPACITY_GE(bv->capacity, range + start);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(start), 0x00));
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data + DIV8(start), DIV8(range), 0xff));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_set_range)
    TEST(SetRange_CHECK_REDUNDANCY),
    TEST(SetRange_WHEN_RANGE_SIZE_EQUAL_ZERO),
    TEST(SetRange_WHEN_RANGE_SIZE_EQUAL_ONE),
    TEST(SetRange_WHEN_RANGE_SIZE_IS_AT_CAPACITY),
    TEST(SetRange_WHEN_RANGE_SIZE_IS_GREATER_THAN_CAPACITY)
END_TESTS()
