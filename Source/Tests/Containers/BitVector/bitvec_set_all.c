/**
 * @file bitvec_set_all.c
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
 * @brief Unit test for bitvec_set_all in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>

#include "helpers.h"

TEST_FN Bool SetAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* flag all bits */
    bitvec_set_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 0);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_NON_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* flag all bits */
    bv->length = oc;
    bitvec_set_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(oc), 0xff));

    /* flag all bits again, the data must remain same, this is redundancy check */
    bitvec_set_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(oc), 0xff));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetAll_SET_SINGLE_BIT_WHEN_LENGTH_EQUAL_ONE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* set all bits when length is 1 */
    bv->length = 1;
    bitvec_set_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_GE(bv->capacity, oc);
    TEST_CONTENTS(bv->data[0] == 1);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetAll_SET_LARGE_WHEN_LENGTH_IS_NOT_8BIT_ALIGNED() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* set all bits when length is not 8 bit aligned */
    Size len = DIV4(bv->capacity) + 11;
    bv->length = len;
    bitvec_set_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, len);
    TEST_CAPACITY_GE(bv->capacity, len);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(len), 0xff))
    TEST_CONTENTS(bv->data[DIV8(len)] = MASK8_LO(MOD8(len)));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetAll_WHEN_LENGTH_GREATER_THAN_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* the test might fail here, this is completely implementation dependent
     * because user is not supposed to alter the length and capacity on their
     * own without using the interface provided for interaction witht he bv */
    Size len = bv->capacity + 11;
    Size sz = DIV8(len);
    bv->length = len;
    bitvec_set_all(bv);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, len);
    TEST_CAPACITY_GE(bv->capacity, len);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, sz, 0xff))
    TEST_CONTENTS(bv->data[sz] == MASK8_LO(MOD8(len)));

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_set_all)
    TEST(SetAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_ZERO),
    TEST(SetAll_CHECK_REDUNDANCY_WHEN_LENGTH_IS_NON_ZERO),
    TEST(SetAll_SET_SINGLE_BIT_WHEN_LENGTH_EQUAL_ONE),
    TEST(SetAll_SET_LARGE_WHEN_LENGTH_IS_NOT_8BIT_ALIGNED),
    TEST(SetAll_WHEN_LENGTH_GREATER_THAN_CAPACITY),
END_TESTS()
