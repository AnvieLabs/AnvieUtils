/**
 * @file UnitTest.h
 * @date Wed, 13th July, 2023
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
 * @brief Helper functions and defines for easy creation of unit tests.
 * */

#ifndef ANVIE_UTILS_UNIT_TEST_H
#define ANVIE_UTILS_UNIT_TEST_H

#include <Anvie/Types.h>
#include <Anvie/HelperDefines.h>
#include <string.h>

/**
 * All unit tests look like this
 * */
typedef Bool (*AnvUnitTestCallbackFn)();
#define TEST_FN static inline

/**
 * Test structure to contain test related data.
 * */
typedef struct anvie_unit_test_t {
    String s_name;
    AnvUnitTestCallbackFn pfn_test;
} AnvUnitTest;

// begin tests recording
#define BEGIN_TESTS(name)                              \
    Size unit_##name##_tests() {                       \
    static String unit_tests_names = #name;            \
    static const AnvUnitTest tests[] = {

// end tests recording
#define END_TESTS()                                                     \
    };                                                                  \
    static Size unit_tests_count = sizeof(tests)/sizeof(tests[0]);      \
    LINE80();                                                           \
    println(COLOR_BOLD_WHITE "UNIT : \"%s\"" COLOR_RESET ,unit_tests_names); \
    LINE80();                                                           \
                                                                        \
    Size iter = unit_tests_count;                                       \
    Size failed_count = 0;                                              \
    while(iter--) {                                                     \
        if(!tests[iter].pfn_test()) {                                   \
            ERR("FAIL", "%s", tests[iter].s_name);                      \
            failed_count++;                                             \
        } else {                                                        \
            OK("PASS", "%s", tests[iter].s_name);                       \
        }                                                               \
        puts("\n---");                                                  \
    }                                                                   \
    return failed_count;                                                \
    }

// record a test function
#define TEST(test) {.s_name = #test, .pfn_test = test}

#define BEGIN_UNIT_TESTS()                      \
    int main() {                                \
    Size failed = 0;                            \
    setbuf(stdout, NULL);

#define END_UNIT_TESTS()                        \
    }

#define UNIT_TEST(name)                                 \
    failed = unit_##name##_tests();                     \
    LINE80();                                           \
    if(failed) {                                                        \
        println(COLOR_BOLD_RED "FAILED TESTS COUNT : \"%zu\"" COLOR_RESET, failed); \
    }                                                                   \
    else {                                                              \
        println(COLOR_BOLD_GREEN "FAILED TESTS COUNT : \"%zu\"" COLOR_RESET, failed); \
    }                                                                   \
    LINE80();

#define IMPORT_UNIT_TEST(name) Size unit_##name##_tests();

#endif // ANVIE_UTILS_UNIT_TEST_H
