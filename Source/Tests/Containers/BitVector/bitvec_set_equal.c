/**
 * @file bitvec_set_equal.c
 * @date Sun, 24th December, 2023
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
 * @brief Unit test for bitvec_set_equal in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>

#include "drop_in_replacements.h"

TEST_FN Bool SetEqual_WITH_DST_BITVEC_LEN_LT_SRC_BITVEC_LEN() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* fill with some data, length is 8 bit aligned */
    bv->data[DIV8(bv->capacity) - 1] = 0xca;
    bv->data[DIV16(bv->capacity)]    = 0xfe;
    bv->data[DIV32(bv->capacity)]    = 0xba;
    bv->data[DIV64(bv->capacity)]    = 0xbe;
    bv->length = bv->capacity;

    /* create bitvec to equate with original vec */
    BitVector* bveq = bitvec_create();
    TEST_OBJECT(bveq);
    /* destination bitvector's length is zero here */

    /* equate */
    bitvec_set_equal(bveq, bv);

    TEST_DATA_PTR(bv->data);
    TEST_UNEXPECTED(bv->data != bveq->data);
    TEST_LENGTH_EQ(bv->length, bveq->length);
    TEST_CAPACITY_EQ(bv->capacity, bveq->capacity);

    /* VALIDATE CONTENTS */
    TEST_COND(!memcmp(bv->data, bveq->data, bitvec_get_length_in_bytes(bv)), ERR_INVALID_CONTENTS);

    DO_BEFORE_EXIT(
        bitvec_destroy(bveq);
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetEqual_WITH_DST_BITVEC_LEN_EQ_SRC_BITVEC_LEN() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* fill with some data, length is 8 bit aligned */
    bv->data[DIV8(bv->capacity) - 1] = 0xca;
    bv->data[DIV16(bv->capacity)]    = 0xfe;
    bv->data[DIV32(bv->capacity)]    = 0xba;
    bv->data[DIV64(bv->capacity)]    = 0xbe;
    bv->length = bv->capacity;
    Size oc = bv->capacity;

    /* create bitvec to equate with original vec */
    BitVector* bveq = bitvec_create();
    TEST_OBJECT(bveq);
    resizebv(bveq, bv->length);

    /* equate */
    bitvec_set_equal(bveq, bv);

    TEST_DATA_PTR(bv->data);
    TEST_DATA_PTR(bveq->data);
    TEST_UNEXPECTED(bv->data != bveq->data);
    TEST_LENGTH_EQ(bv->length, bveq->length);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CAPACITY_GE(bveq->capacity, bv->length);

    /* VALIDATE CONTENTS */
    TEST_COND(!memcmp(bv->data, bveq->data, bitvec_get_length_in_bytes(bv)), ERR_INVALID_CONTENTS);

    DO_BEFORE_EXIT(
        bitvec_destroy(bveq);
        bitvec_destroy(bv);
    );
}

TEST_FN Bool SetEqual_WITH_DST_BITVEC_LEN_GT_SRC_BITVEC_LEN() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* fill with some data, length is 8 bit aligned */
    bv->data[DIV8(bv->capacity) - 1] = 0xca;
    bv->data[DIV16(bv->capacity)]    = 0xfe;
    bv->data[DIV32(bv->capacity)]    = 0xba;
    bv->data[DIV64(bv->capacity)]    = 0xbe;
    bv->length = bv->capacity;

    /* create bitvec to equate with original vec */
    BitVector* bveq = bitvec_create();
    TEST_OBJECT(bveq);
    /* resize to value greater than capacity of source bitvector itself */
    resizebv(bveq, bv->capacity + 10);

    /* equate */
    bitvec_set_equal(bveq, bv);

    TEST_DATA_PTR(bv->data);
    TEST_UNEXPECTED(bv->data != bveq->data);
    TEST_LENGTH_EQ(bv->length, bveq->length);
    TEST_CAPACITY_GE(bveq->capacity, bv->capacity); /* note the difference, this time we expect this to be greater or equal */

    /* VALIDATE CONTENTS */
    TEST_COND(!memcmp(bv->data, bveq->data, bitvec_get_length_in_bytes(bv)), ERR_INVALID_CONTENTS);

    DO_BEFORE_EXIT(
        if(bveq) bitvec_destroy(bveq);
        if(bv) bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_set_equal)
    TEST(SetEqual_WITH_DST_BITVEC_LEN_LT_SRC_BITVEC_LEN),
    TEST(SetEqual_WITH_DST_BITVEC_LEN_EQ_SRC_BITVEC_LEN),
    TEST(SetEqual_WITH_DST_BITVEC_LEN_GT_SRC_BITVEC_LEN),
END_TESTS()
