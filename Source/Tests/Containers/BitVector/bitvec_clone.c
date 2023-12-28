/**
 * @file bitvec_clone.c
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
 * @brief Unit test for bitvec_clone in BitVector container.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/BitManipulation.h>

TEST_FN Bool Clone() {
    BitVector* bv = bitvec_create();
    TEST_OBJECT(bv);

    /* fill with some data */
    bv->data[DIV8(bv->capacity) - 1]  = 0xca;
    bv->data[DIV16(bv->capacity)]     = 0xfe;
    bv->data[DIV32(bv->capacity)]     = 0xba;
    bv->data[DIV64(bv->capacity)]     = 0xbe;
    bv->length = bv->capacity;

    /* create clone */
    BitVector* bvclone = bitvec_clone(bv);
    TEST_OBJECT(bvclone);

    TEST_UNEXPECTED(bv != bvclone);
    TEST_DATA_PTR(bvclone->data);
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, bvclone->length);
    TEST_CAPACITY_EQ(bv->capacity, bvclone->capacity);
    TEST_CONTENTS(memcmp(bv->data, bvclone->data, bitvec_get_capacity_in_bytes(bv)) == 0);

    DO_BEFORE_EXIT(
        bitvec_destroy(bvclone);
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_clone)
    TEST(Clone)
END_TESTS()
