/**
 * @file bitvec_create.c
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
 * @brief BitVector container unit test for the function
 * @c bitvec_create.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>

TEST_FN Bool Create() {
    BitVector* bv = bitvec_create();

    TEST_OBJECT(bv);
    TEST_DATA_PTR(bv->data);
    TEST_LENGTH_EQ(bv->length, 0);
    TEST_CAPACITY_GE(bv->capacity, 64);

    DO_BEFORE_EXIT(
        bitvec_destroy(bv);
    );
}

BEGIN_TESTS(bitvec_create)
TEST(Create)
END_TESTS()
