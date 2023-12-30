/**
 * @file bitvec_not.c
 * @date Wed, 27th December, 2023
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
 * @brief Unit test for bitvec_not in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>

#include "drop_in_replacements.h"
#include "helpers.h"

TEST_FN Bool Not_CHECK_REDUNDANCY_WHEN_LENGTH_IS_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    memset(bv->data, 0xff, DIV8(bv->capacity));

    /* length of bv is still zero when calling this */
    BitVector* bvres = bitvec_not(bv);
    TEST_OBJECT(bvres);

    /* test bvres */
    TEST_DATA_PTR(bvres->data);
    TEST_LENGTH_EQ(bvres->length, bv->length);
    TEST_CAPACITY_EQ(bvres->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data, DIV8(bvres->capacity), 0) == True);
    /* test bv */
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(bv->capacity), 0xff) == True);

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Not_LENGTH_IS_8BIT_ALIGNED() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    bv->length = bv->capacity;
    memset(bv->data, 0x88, DIV8(bv->capacity));

    BitVector* bvres = bitvec_not(bv);
    TEST_OBJECT(bvres);

    /* test bvres */
    TEST_DATA_PTR(bvres->data);
    TEST_LENGTH_EQ(bvres->length, bv->length);
    TEST_CAPACITY_EQ(bvres->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data, DIV8(bvres->length), TO_UINT8(NOT(0x88))) == True);
    /* test bv */
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, DIV8(bv->capacity), 0x88) == True);

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
        if(bvres) bitvec_destroy(bvres);
    );
}

TEST_FN Bool Not_LENGTH_IS_NOT_8BIT_ALIGNED() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    Size len = DIV2(bv->capacity) + 3;
    Size sz = DIV8(len);

    /* intitialize with some data */
    bv->length = len;
    memset(bv->data, 0xaa, sz);
    SET8_LO(bv->data[sz], 3);

    BitVector* bvres = bitvec_not(bv);
    TEST_OBJECT(bvres);

    /* test bvres */
    TEST_DATA_PTR(bvres->data);
    TEST_LENGTH_EQ(bvres->length, len);
    TEST_CAPACITY_EQ(bvres->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data, sz, TO_UINT8(NOT(0xaa))) == True);
    TEST_CONTENTS(bvres->data[sz] == GET8_LO(~MASK8_LO(3), 3)); /* last byte has lower 3 bits set and higher 5 bits 0 */
    TEST_CONTENTS(is_memory_filled_with_byte(bvres->data + sz + 1, DIV8(bvres->capacity) - sz - 1, 0) == True);
    /* test bv */
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, len);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, sz, 0xaa) == True);
    TEST_CONTENTS(bv->data[sz] == TO_UINT8(MASK8_LO(3))); /* last byte has lower 3 bits set and higher 5 bits 0 */
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data + sz + 1, DIV8(bv->capacity) - sz - 1, 0) == True);

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
        if(bvres) bitvec_destroy(bvres);
    );
}

BEGIN_TESTS(bitvec_not)
    TEST(Not_CHECK_REDUNDANCY_WHEN_LENGTH_IS_ZERO),
    TEST(Not_LENGTH_IS_8BIT_ALIGNED),
    TEST(Not_LENGTH_IS_NOT_8BIT_ALIGNED)
END_TESTS()
