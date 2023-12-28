/**
 * @file bitvec_clear.c
 * @date Mon, 25th December, 2023
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
 * @brief Unit test for bitvec_clear in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

#include "helpers.h"

TEST_FN Bool Clear_ONE_BY_ONE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    bv->data[0] = 0xff;

    /* clear very first bit */
    bitvec_clear(bv, 0);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0xfe);
    TEST_CONTENTS(bv->data[1] == 0);

    /* clear second bit */
    bitvec_clear(bv, 1);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 2);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0xfc);
    TEST_CONTENTS(bv->data[1] == 0);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Clear_CHECK_REDUNDANCY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    bv->data[0] = 0xff;

    /* clear very first bit, value = 0xfe */
    bitvec_clear(bv, 0);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0xfe);
    TEST_CONTENTS(bv->data[1] == 0);

    /* clear the first bit again, value = 0xfe */
    bitvec_clear(bv, 0);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0xfe);
    TEST_CONTENTS(bv->data[1] == 0);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Clear_MULTIPLE_AND_CROSS_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    memset(bv->data, 0xff, DIV8(bv->capacity));

    Size oc = bv->capacity;

    Size sz = oc + 5;
    for(Size s = 0; s < sz; s++) {
        bitvec_clear(bv, s);
    }

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, sz);
    TEST_CAPACITY_GE(bv->capacity, sz);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(sz), 0x00));
    TEST_CONTENTS(bv->data[DIV8(sz)] == 0);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Clear_DIRECT_AND_AT_HIGH_OFFSET() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    memset(bv->data, 0xff, DIV8(bv->capacity));

    Size pos = 100;
    bitvec_clear(bv, pos);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, pos + 1); /* if bit at index=pos is cleared then length of bv must be pos+1 */
    TEST_CAPACITY_GE(bv->capacity, pos + 1);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(pos), 0xff))
    TEST_CONTENTS(bv->data[DIV8(pos)] == 0xef) /* bit at index 4 is cleared */;

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_clear)
    TEST(Clear_ONE_BY_ONE),
    TEST(Clear_CHECK_REDUNDANCY),
    TEST(Clear_MULTIPLE_AND_CROSS_CAPACITY),
    TEST(Clear_DIRECT_AND_AT_HIGH_OFFSET),
END_TESTS()
