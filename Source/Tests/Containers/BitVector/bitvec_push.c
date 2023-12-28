/**
 * @file bitvec_push.c
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
 * @brief Unit test for bitvec_push in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

TEST_FN Bool Push_ONE_BY_ONE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* push true to bit position 0 */
    bitvec_push(bv, True);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_GE(bv->capacity, 1);
    TEST_COND(bv->data[0] == 0x01, ERR_INVALID_CONTENTS);
    TEST_COND(bv->data[1] == 0x00, ERR_INVALID_CONTENTS);

    /* reset length, and push False so False is now at bit position 0 */
    bv->length = 0;
    bitvec_push(bv, False);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 1);
    TEST_CAPACITY_GE(bv->capacity, 1);
    TEST_COND(bv->data[0] == 0x00, ERR_INVALID_CONTENTS);
    TEST_COND(bv->data[1] == 0x00, ERR_INVALID_CONTENTS);

    /* push True, pos = 1 */
    bitvec_push(bv, True);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 2);
    TEST_CAPACITY_GE(bv->capacity, 2);
    TEST_COND(bv->data[0] == 0x02, ERR_INVALID_CONTENTS);
    TEST_COND(bv->data[1] == 0x00, ERR_INVALID_CONTENTS);

    /* push true, pos = 2 */
    bitvec_push(bv, True);

    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 3);
    TEST_CAPACITY_GE(bv->capacity, 3);
    TEST_COND(bv->data[0] == 0x06, ERR_INVALID_CONTENTS);
    TEST_COND(bv->data[1] == 0x00, ERR_INVALID_CONTENTS);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Push_MANY_AT_ONCE_BUT_DONT_CROSS_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity ;

    /* push many, but keep in capacity boundary */
    for(Size rem = 0; rem < 8; rem++) {
        /* reset length */
        bv->length = 0;

        Size len = oc - 16;
        Size psz = len + rem;
        for(Size s = 0; s < psz; s++) {
            bitvec_push(bv, (s & 1));
        }

        TEST_DATA_PTR(bv->data);
        TEST_LENGTH_EQ(bv->length, psz);
        TEST_CAPACITY_EQ(bv->capacity, oc); /* capacity must be same, no resize expeced */
        for(Size s = 0; s < DIV8(psz); s++) {
            TEST_CONTENTS(bv->data[s] == 0xaa);
        }
        TEST_CONTENTS(bv->data[DIV8(psz)] == GET8_LO(0xaa, rem));
        TEST_CONTENTS(bv->data[DIV8(psz) + 1] == 0);

        /* reset contents */
        memset(bv->data, 0, DIV8(bv->capacity));
    }

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

TEST_FN Bool Push_MANY_AT_ONCE_AND_CROSS_CAPACITY() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* push many and cross the capacity boundary */
    for(Size rem = 0; rem < 8; rem++) {
        /* reset length before use */
        bv->length = 0;

        /* push many */
        Size oc = bv->capacity;
        Size psz = oc + DIV2(oc) + rem;
        for(Size s = 0; s < psz; s++) {
            bitvec_push(bv, True);
        }

        TEST_DATA_PTR(bv->data);
        TEST_LENGTH_EQ(bv->length, psz);
        TEST_CAPACITY_GE(bv->capacity, psz);
        for(Size s = 0; s < DIV8(psz); s++) {
            TEST_CONTENTS(bv->data[DIV8(s)] == 0xff);
        }
        TEST_CONTENTS(bv->data[DIV8(psz)] == MASK_LO(rem));
        TEST_CONTENTS(bv->data[DIV8(psz) + 1] == 0x00);

        /* reset contents */
        memset(bv->data, 0, DIV8(bv->capacity));
    }

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_push)
    TEST(Push_ONE_BY_ONE),
    TEST(Push_MANY_AT_ONCE_BUT_DONT_CROSS_CAPACITY),
    TEST(Push_MANY_AT_ONCE_AND_CROSS_CAPACITY)
END_TESTS()
