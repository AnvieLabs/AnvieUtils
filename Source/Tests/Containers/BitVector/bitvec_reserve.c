/**
 * @file bitvec_reserve.c
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
 * @brief Unit test for bitvec_reserve in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>

TEST_FN Bool Reserve_WITH_SUBSEQUENT_INCREASING_SIZES() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* create space to store 100 boolean values */
    bitvec_reserve(bv, 100);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_GE(bv->capacity, 100);

    /* this time resize to actually size, allocate for 1024 booleans. */
    bitvec_reserve(bv, 1024);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_GE(bv->capacity, 1024); /* capacity must increase */

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Reserve_WITH_SUBSEQUENT_DECREASING_SIZES() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* create space to store 1024 boolean values */
    bitvec_reserve(bv, 1024);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_GE(bv->capacity, 1024);

    /* reserve a size lower than initial capacity */
    bitvec_reserve(bv, 100);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_GE(bv->capacity, 1024); /* capacity must not change */

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}


BEGIN_TESTS(bitvec_reserve)
    TEST(Reserve_WITH_SUBSEQUENT_INCREASING_SIZES),
    TEST(Reserve_WITH_SUBSEQUENT_DECREASING_SIZES)
END_TESTS()
