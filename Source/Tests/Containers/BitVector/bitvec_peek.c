/**
 * @file bitvec_peek.c
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
 * @brief Unit test for bitvec_peek in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>
#include <Anvie/Bit/Bit.h>

#include "drop_in_replacements.h"
#include "helpers.h"

TEST_FN Bool Peek_WHEN_LENGTH_IS_ZERO() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    TEST_CONTENTS(bitvec_peek(bv, 10) == False); /* bv is expected to return 0 outside it's length boundary */

    /* test bv */
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, 0x00, DIV8(bv->capacity)) == True);

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
    );
}

TEST_FN Bool Peek_AT_DIFFERENT_INDICES_IN_SAME_BYTE() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    Size oc = bv->capacity;
    bv->length = oc;

    /* fill 11th byte with some  */
    Uint8 bidx = DIV16(bv->capacity);
    Bool b0 = 1, b1 = 0, b2 = 1, b3 = 1, b4 = 0, b5 = 1, b6 = 0, b7 = 0;
    bv->data[bidx] = CREATE_MASK8(b0, b1, b2, b3, b4, b5, b6, b7);

    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 0) == b7);
    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 1) == b6);
    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 2) == b5);
    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 3) == b4);
    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 4) == b3);
    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 5) == b2);
    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 6) == b1);
    TEST_CONTENTS(bitvec_peek(bv, MUL8(bidx) + 7) == b0);

    /* test bv */
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, oc);
    TEST_CAPACITY_EQ(bv->capacity, oc);
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data, bidx, 0x00) == True);
    /* skip the byte at index `bidx` */
    /* continue from byte at index bidx+1 */
    TEST_CONTENTS(is_memory_filled_with_byte(bv->data + bidx + 1, DIV8(bv->capacity) - bidx - 1, 0x00) == True);

    DO_BEFORE_EXIT(
        if(bv) bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_peek)
    TEST(Peek_WHEN_LENGTH_IS_ZERO),
    TEST(Peek_AT_DIFFERENT_INDICES_IN_SAME_BYTE)
END_TESTS()
