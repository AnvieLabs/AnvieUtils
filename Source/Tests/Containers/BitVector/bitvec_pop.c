/**
 * @file bitvec_pop.c
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
 * @brief Unit test for bitvec_pop in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>

TEST_FN Bool Pop_CHECK_REDUNDANCY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;

    /* bitvector length is 0 initially, it must return false always */
    TEST_CONTENTS(bitvec_pop(bv) == False);
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Pop_ONE_BY_ONE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* fill with some data that we'll pop and verify */
    Uint8 v[] = {0x15, 0xa6, 0xd2};
    memcpy(bv->data, v, sizeof(v));
    bv->length =  MUL8(sizeof(v));
    Size oc = bv->capacity;

    Size len = bv->length;
    for(Size s = 0; s < MUL8(sizeof(v)); s++) {
        len--;
        TEST_CONTENTS(bitvec_pop(bv) == GET8_BIT(v[DIV8(len)], MOD8(len)));
    }

    /* bitvector length is 0 initially, it must return false always */
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );

}

BEGIN_TESTS(bitvec_pop)
    TEST(Pop_CHECK_REDUNDANCY),
    TEST(Pop_ONE_BY_ONE),
END_TESTS()
