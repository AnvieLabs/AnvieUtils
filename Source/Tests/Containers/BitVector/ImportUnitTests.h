/**
 * @file ImportUnitTests.h
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
 * @brief BitVector container unit tests import header.
 * Include this header exactly once throughout the whole program.
 * This will import these unit tests to wherever this file is included.
 * */

#ifndef ANVIE_UTILS_TESTS_CONTAINERS_BITVECTOR_IMPORT_UNIT_TESTS_H
#define ANVIE_UTILS_TESTS_CONTAINERS_BITVECTOR_IMPORT_UNIT_TESTS_H

#include <Anvie/Test/UnitTest.h>

IMPORT_UNIT_TEST(bitvec_create)
IMPORT_UNIT_TEST(bitvec_clone)
IMPORT_UNIT_TEST(bitvec_set_equal)
IMPORT_UNIT_TEST(bitvec_reserve)
IMPORT_UNIT_TEST(bitvec_resize)
IMPORT_UNIT_TEST(bitvec_push)
IMPORT_UNIT_TEST(bitvec_pop)
IMPORT_UNIT_TEST(bitvec_set)
IMPORT_UNIT_TEST(bitvec_clear)
IMPORT_UNIT_TEST(bitvec_set_all)
IMPORT_UNIT_TEST(bitvec_clear_all)
IMPORT_UNIT_TEST(bitvec_set_range)
IMPORT_UNIT_TEST(bitvec_clear_range)
IMPORT_UNIT_TEST(bitvec_peek)
IMPORT_UNIT_TEST(bitvec_xor)
IMPORT_UNIT_TEST(bitvec_and)
IMPORT_UNIT_TEST(bitvec_or)
IMPORT_UNIT_TEST(bitvec_xnor)
IMPORT_UNIT_TEST(bitvec_nand)
IMPORT_UNIT_TEST(bitvec_nor)
IMPORT_UNIT_TEST(bitvec_shl)
IMPORT_UNIT_TEST(bitvec_shr)
IMPORT_UNIT_TEST(bitvec_not)
IMPORT_UNIT_TEST(bitvec_cmpeq)

IMPORT_UNIT_TEST(drop_in_replacements)

#endif // ANVIE_UTILS_TESTS_CONTAINERS_BITVECTOR_IMPORT_UNIT_TESTS_H
