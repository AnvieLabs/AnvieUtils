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
 * Take a look at how existing tests are written for examples on how to use.
 * */

#ifndef ANVIE_UTILS_UNIT_TEST_H
#define ANVIE_UTILS_UNIT_TEST_H

#include <Anvie/Types.h>
#include <Anvie/HelperDefines.h>
#include <Anvie/Error.h>
#include <string.h>

/**
 * All unit tests look like this
 * */
typedef Bool (*UnitTestCallbackFn)();
#define TEST_FN static inline

/**
 * Test structure to contain test related data.
 * */
typedef struct anvie_unit_test_t {
    ZString name;
    UnitTestCallbackFn test_fn;
} UnitTest;

// begin tests recording
#define BEGIN_TESTS(name)                              \
    Size unit_##name##_tests() {                       \
    static ZString unit_tests_names = #name;           \
    static const UnitTest tests[] = {

// end tests recording
#define END_TESTS()                                                     \
    };                                                                  \
    static Size unit_tests_count = sizeof(tests)/sizeof(tests[0]);      \
    LINE80();                                                           \
    println(COLOR_BOLD_WHITE "UNIT : \"%s\"" COLOR_RESET ,unit_tests_names); \
                                                                        \
    Size iter = unit_tests_count;                                       \
    Size failed_count = 0;                                              \
    while(iter--) {                                                     \
        if(!tests[iter].test_fn()) {                                    \
            ERR("FAIL", "%s", tests[iter].name);                        \
            failed_count++;                                             \
        } else {                                                        \
            OK("PASS", "%s", tests[iter].name);                         \
        }                                                               \
        puts("\n---");                                                  \
    }                                                                   \
    return failed_count;                                                \
    }

// record a test function
#define TEST(test) {.name = #test, .test_fn = test}

#define BEGIN_UNIT_TESTS()                      \
    int main() {                                \
    Size failed = 0;                            \
    setbuf(stdout, NULL);

#define END_UNIT_TESTS()                        \
    }

#define UNIT_TEST(name)                                 \
    failed = unit_##name##_tests();                     \
    if(failed) {                                                        \
        println(COLOR_BOLD_RED "FAILED TESTS COUNT : \"%zu\"" COLOR_RESET, failed); \
    }                                                                   \
    else {                                                              \
        println(COLOR_BOLD_GREEN "ALL TESTS PASSED" COLOR_RESET);       \
    }                                                                   \

#define IMPORT_UNIT_TEST(name) Size unit_##name##_tests();

/**
 * This macro must be used at the end of each test (if used at all).
 * You define the code you want to run before returning. The code will be made
 * sure to run before returning from the test function.
 *
 * The macro will define two labels.
 * One label, named "SUCCEEDED" returns "True" and other named "FAILED"
 * returns "FAILED".
 * One can use the macro GOTO_LABEL_IF_FAIL to jump to any of the label,
 * based on the condition imposed.
 * One can also use one of the macros defined below to avoid writing
 * longer code.
 * */
#define DO_BEFORE_EXIT(code)                        \
    {                                               \
        code;                                       \
        return True;                                \
    FAILED:                                         \
        code;                                       \
        return False;                               \
    }

#define TEST_COND(cond, err) GOTO_LABEL_IF_FAIL(cond, FAILED, "LINE::%u %s\n", __LINE__, err_to_zstr(err))
#define TEST_LENGTH_EQ(len, val) TEST_COND(((len) == (val)), ERR_INVALID_LENGTH)
#define TEST_LENGTH_NE(len, val) TEST_COND(((len) != (val)), ERR_INVALID_LENGTH)
#define TEST_LENGTH_GE(len, val) TEST_COND(((len) >= (val)), ERR_INVALID_LENGTH)
#define TEST_LENGTH_GT(len, val) TEST_COND(((len) >  (val)), ERR_INVALID_LENGTH)
#define TEST_LENGTH_LE(len, val) TEST_COND(((len) <= (val)), ERR_INVALID_LENGTH)
#define TEST_LENGTH_LT(len, val) TEST_COND(((len) <  (val)), ERR_INVALID_LENGTH)
#define TEST_CAPACITY_EQ(len, val) TEST_COND(((len) == (val)), ERR_INVALID_CAPACITY)
#define TEST_CAPACITY_NE(len, val) TEST_COND(((len) != (val)), ERR_INVALID_CAPACITY)
#define TEST_CAPACITY_GE(len, val) TEST_COND(((len) >= (val)), ERR_INVALID_CAPACITY)
#define TEST_CAPACITY_GT(len, val) TEST_COND(((len) >  (val)), ERR_INVALID_CAPACITY)
#define TEST_CAPACITY_LE(len, val) TEST_COND(((len) <= (val)), ERR_INVALID_CAPACITY)
#define TEST_CAPACITY_LT(len, val) TEST_COND(((len) <  (val)), ERR_INVALID_CAPACITY)
#define TEST_OBJECT(obj)      RETURN_VALUE_IF_FAIL((obj), False, "%s", err_to_zstr(ERR_INVALID_OBJECT));
#define TEST_DATA_PTR(dp)     RETURN_VALUE_IF_FAIL((dp), False, "%s", err_to_zstr(ERR_INVALID_DATA_PTR));
#define TEST_UNEXPECTED(cond) TEST_COND((cond), ERR_UNEXPECTED)
#define TEST_CONTENTS(cond)   TEST_COND((cond), ERR_INVALID_CONTENTS)
#define TEST_EQUALITY(cond)   TEST_COND((cond), ERR_REQUIRED_EQUALITY_FAILED)

#endif // ANVIE_UTILS_UNIT_TEST_H
