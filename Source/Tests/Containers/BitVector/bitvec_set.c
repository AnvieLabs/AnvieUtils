/**
 * @file bitvec_set.c
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
 * @brief Unit test for bitvec_set in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

#include "helpers.h"

TEST_FN Bool Set_ONE_BY_ONE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set very first bit, value = 1 */
    bitvec_set(bv, 0);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 1);
    TEST_CONTENTS(bv->data[1] == 0);

    /* set second bit, value = 3 */
    bitvec_set(bv, 1);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 2);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 3);
    TEST_CONTENTS(bv->data[1] == 0);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Set_CHECK_REDUNDANCY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set very first bit, value = 1 */
    bitvec_set(bv, 0);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 1);
    TEST_CONTENTS(bv->data[1] == 0);

    /* set the first bit again, value = 1 */
    bitvec_set(bv, 0);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 1);
    TEST_CONTENTS(bv->data[1] == 0);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Set_MULTIPLE_AND_CROSS_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    Size sz = oc + 5;
    for(Size s = 0; s < sz; s++) {
        bitvec_set(bv, s);
    }

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, sz);
    TEST_CAPACITY_GE(bv->capacity, sz);
    for(Size s = 0; s < DIV8(sz); s++) {
        TEST_CONTENTS(bv->data[s] == 0xff);
    }
    TEST_CONTENTS(bv->data[DIV8(sz)] == MASK_LO(MOD8(sz)));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Set_DIRECT_AND_AT_HIGH_OFFSET() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size pos = 100;
    bitvec_set(bv, pos);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, pos + 1); /* setting at index pos means length will be pos+1 */
    TEST_CAPACITY_GE(bv->capacity, pos + 1);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(pos), 0));
    TEST_CONTENTS(bv->data[DIV8(pos)] == 0x10); /* lower (100 % 8) = 4 bits are 0 */

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_set)
    TEST(Set_ONE_BY_ONE),
    TEST(Set_CHECK_REDUNDANCY),
    TEST(Set_MULTIPLE_AND_CROSS_CAPACITY),
    TEST(Set_DIRECT_AND_AT_HIGH_OFFSET),
END_TESTS()
