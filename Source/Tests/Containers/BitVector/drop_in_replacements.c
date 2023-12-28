/**
 * @file drop_in_replacements.c
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
 * @brief Tests for functions in drop_in_replacements.h
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>

#include "drop_in_replacements.h"

TEST_FN Bool ResizeBV_WITH_SUBSEQUENT_INCREASING_SIZES() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* create space to store 100 boolean values, and set length to 100 bits */
    resizebv(bv, 100);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 100);
    TEST_CAPACITY_GE(bv->capacity, 100);

    /* resize to allocate for 1024 booleans. */
    resizebv(bv, 1024);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1024);
    TEST_CAPACITY_GE(bv->capacity, 1024);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool ResizeBV_WITH_SUBSEQUENT_DECREASING_SIZES() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* create space to store 1024 boolean values */
    resizebv(bv, 1024);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1024);
    TEST_CAPACITY_GE(bv->capacity, 1024);

    /* resize to allocate for 100 booleans. this must not change capacity */
    resizebv(bv, 1024);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 100);
    TEST_CAPACITY_GE(bv->capacity, 1024);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(drop_in_replacements)
    TEST(ResizeBV_WITH_SUBSEQUENT_INCREASING_SIZES),
    TEST(ResizeBV_WITH_SUBSEQUENT_DECREASING_SIZES)
END_TESTS()
