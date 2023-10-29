/**
 * @file IntegerVector.c
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
 * @brief Integer/Pointer vector container tests
 * */

#include <Anvie/Containers/Vector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Chrono/Time.h>

#define TEST_DATA_SIZE ((Size)49)

void PrintU32(void* value, Size pos) {
    UNUSED(pos);
    printf(" %x", (Uint32)(Uint64)value);
}

void PrintU64(void* value, Size pos) {
    UNUSED(pos);
    printf("%lx ", (Uint64)value);
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create1() {
    Vector* vec = vector_create(sizeof(Uint8), NULL, NULL);
    RETURN_VALUE_IF_FAIL(vec, False, "FAILED TO CREATE vector\n");
    vector_destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create2() {
    Vector* vec = vector_create(sizeof(Uint16), (CreateElementCopyCallback)1, (DestroyElementCopyCallback)1);
    RETURN_VALUE_IF_FAIL(vec, False, "FAILED TO CREATE vector\n");
    vector_destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create3() {
    Vector* vec = vector_create(sizeof(Uint32), NULL, (DestroyElementCopyCallback)1);
    RETURN_VALUE_IF_FAIL(!vec, False, "vector CREATION SHOULD HAVE FAILED\n");
    if(vec) vector_destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create4() {
    Vector* vec = vector_create(sizeof(Uint64), (CreateElementCopyCallback)1, NULL);
    RETURN_VALUE_IF_FAIL(!vec, False, "vector CREATION SHOULD HAVE FAILED\n");
    if(vec) vector_destroy(vec);
    return True;
}

// TODO: add rest for overwrite, copy, swap, move

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Insert() {
    Bool res = True;
    Vector* vec = u64_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_insert(vec, iter, iter);

        if(vec->capacity < vec->length) {
            DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
            res = False; goto Exit;
        }
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH\n");
        res = False; goto Exit;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(u64_vector_peek(vec, iter) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    u64_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete operation of vector
 * */
TEST_FN Bool Delete() {
    Bool res = True;

    // first create vector with some valid data
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_insert(vec, iter, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_delete(vec, rand() % vec->length);

        if(vec->length != (TEST_DATA_SIZE - iter - 1)) {
            DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING EXPECTED TO BE \"%zu\", FOUND \"%zu\"\n", TEST_DATA_SIZE - iter - 1, vec->length);
            res = False;
            break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Remove() {
    Bool res = True;
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_insert(vec, iter, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(u32_vector_remove(vec, 0) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; break;
        }
    }

    u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert fast operation of vector
 * */
TEST_FN Bool InsertFast() {
    Bool res = True;
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_insert_fast(vec, iter, iter);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH\n");
        res = False; goto Exit;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
        res = False; goto Exit;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // since order is not maintained, we just need to check for presence of each element
        Bool b_found = False;
        for(Size k = 0; k < TEST_DATA_SIZE; k++) {
            if(u32_vector_peek(vec, k) == iter) {
                b_found = True;
            }
        }

        // if element is not found then there will be problem! XD
        if(!b_found) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool DeleteFast() {
    Bool res = True;

    // first create vector with some valid data
    Vector* vec = u64_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u64_vector_insert_fast(vec, iter, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u64_vector_delete_fast(vec, rand() % vec->length);

        if(vec->length != (TEST_DATA_SIZE - iter - 1)) {
            DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING EXPECTED TO BE \"%zu\", FOUND \"%zu\"\n", TEST_DATA_SIZE - iter - 1, vec->length);
            res = False;
            break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    u64_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool RemoveFast() {
    Bool res = True;
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_insert_fast(vec, iter, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // since order is not maintained, we just need to check for presence of each element
        Bool b_found = False;
        Uint32 val = u32_vector_remove_fast(vec, 0); // always remove from first position
        for(Size k = 0; k < TEST_DATA_SIZE; k++) {
            if(val == k) {
                b_found = True;
            }
        }

        // if element is not found then there will be problem! XD
        if(!b_found) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; break;
        }
    }

    u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushBack() {
    Bool res = True;
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_push_back(vec, iter);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH | EXPECTED : \"%zu\" | GOT : \"%zu\"\n", TEST_DATA_SIZE, vec->length);
        res = False; goto Exit;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
        res = False; goto Exit;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(u32_vector_peek(vec, iter) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopBack() {
    Bool res = True;
    // first create vector with some valid data
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u64_vector_push_back(vec, iter);
    }

    // delete all elements and check size
    for(Size iter = TEST_DATA_SIZE; iter; iter--) {
        if(u32_vector_poback(vec) != iter-1) {
            DBG(__FUNCTION__, "PUSHED DATA MISMATCH AT ENTRY INDEX \"%zu\"\n", iter-1);
            res = False; break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushFront() {
    Bool res = True;
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // insert array backwards
        u32_vector_push_front(vec, iter);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH\n");
        res = False; goto Exit;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
        res = False; goto Exit;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(u32_vector_peek(vec, iter) != TEST_DATA_SIZE - 1 - iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopFront() {
    Bool res = True;
    // first create vector with some valid data
    Vector* vec = u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        u32_vector_push_front(vec, TEST_DATA_SIZE - 1 - iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(u32_vector_pofront(vec) != iter) {
            DBG(__FUNCTION__, "PUSHED DATA MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    u32_vector_destroy(vec);
    return res;
}

TEST_FN Bool Merge() {
    Vector* /* String */ vec1 = string_vector_create();
    Vector* /* String */ vec2 = string_vector_create();

    // prepare data
    // vec1
    String s = strdup("Siddharth");
    string_vector_push_back(vec1, s);
    FREE(s);

    s = strdup("Mishra");
    string_vector_push_back(vec1, s);
    FREE(s);
    s = strdup("is");

    // vec2
    string_vector_push_back(vec2, "is");
    FREE(s);

    s = strdup("@brightprogrammer");
    string_vector_push_back(vec2, s);
    FREE(s);

    // merge
    vector_merge(vec1, vec2);

    Bool res = True;
    if(strcmp(vector_peek(vec1, 2), vector_peek(vec2, 0)) != 0) {
        ERR(__FUNCTION__, "MERGE OPERATION IS INVALID! ELEMENTS DON'T MATCH!\n");
        res = False;
    }

    if(strcmp(vector_peek(vec1, 2), vector_peek(vec2, 0)) != 0) {
        ERR(__FUNCTION__, "MERGE OPERATION IS INVALID! ELEMENTS DON'T MATCH!\n");
        res = False;
    }

    string_vector_destroy(vec1);
    string_vector_destroy(vec2);

    // check merge
    return res;
}

static inline Bool element_filter(void* x, void* udata) {
    Int32 v = (Int32)(Int64)x;
    Bool g0 = (Bool)(Int64)udata;

    return g0 && v > 0;
}

TEST_FN Bool Filter() {
    Vector* /* Int32 */ vec = i32_vector_create();

    // prepare data
    for(int i = -10; i < 10; i++) {
        i32_vector_push_back(vec, i);
    }

    // all elements greater than 0 and less than equal to zero are filtered
    Vector* /* Int32 */ vec_g0 = i32_vector_filter(vec, element_filter, (void*)True);
    if(!vec_g0){
        vector_destroy(vec);
        DBG(__FUNCTION__, "FAILED TO FILTER ELEMENTS (vec_g0)\n");
        return False;
    }

    Vector* /* Int32 */ vec_le0 = i32_vector_filter(vec, element_filter, (void*)False);
    if(!vec_le0){
        vector_destroy(vec);
        vector_destroy(vec_g0);
        DBG(__FUNCTION__, "FAILED TO FILTER ELEMENTS (vec_le0)\n");
        return False;
    }

    // check all elements in vec_g0 are greater than 0
    Bool res = True;

    for(Size s = 0; s < vec_g0->length; s++) {
        if(i32_vector_peek(vec_g0, s) <= 0) {
            ERR(__FUNCTION__, "FILTERED VECTOR (g0) CONTAINS WRONG CONTENT\n");
            res = False;
            break;
        }
    }

    // check all elements in vec_le0 are less than or equal to 0
    for(Size s = 0; s < vec_le0->length; s++) {
        if(i32_vector_peek(vec_le0, s) > 0) {
            ERR(__FUNCTION__, "FILTERED VECTOR (le0) CONTAINS WRONG CONTENT\n");
            res = False;
            break;
        }
    }

    i32_vector_destroy(vec);
    i32_vector_destroy(vec_g0);
    i32_vector_destroy(vec_le0);

    return res;
}

TEST_FN Bool Swap() {
    Vector* vec = i32_vector_create();

    Int32 v0 = 32, v1 = 64;
    i32_vector_push_back(vec, v0);
    i32_vector_push_back(vec, v1);
    i32_vector_swap(vec, 0, 1);

    if(i32_vector_peek(vec, 0) != v1 || i32_vector_peek(vec, 1) != v0) {
        ERR(__FUNCTION__, "SWAP NOT CORRECT!\n");
    }

    i32_vector_destroy(vec);
    return True;
}

static Int32 compare_i32(void* x, void* y) {
    Int32 vx = (Int32)(Int64)x;
    Int32 vy = (Int32)(Int64)y;
    return vx - vy;
}

static void my_print_i32(void* x, Size s) {
    UNUSED(s);
    Int32 v = (Int32)(Int64)x;
    printf("%d, ", v);
}

#define TEST_VECTOR_SORT_FN(TestName, sort_type)                \
    TEST_FN Bool TestName(void) {                               \
        Vector* /* Int32 */ vec = i32_vector_create();   \
                                                                \
        Float32 avg_time = 0;                                           \
        Size iter_count = 10;                                           \
        Size arr_size = 10;                                             \
        for(Size k = 0; k < iter_count; k++) {                          \
            for(Size s = 0; s < arr_size; s++) {                        \
                i32_vector_push_back(vec, rand()%arr_size);         \
            }                                                           \
                                                                        \
            Size start = chrono_get_time_as_microseconds();         \
            vector_##sort_type##_sort(vec, compare_i32);            \
            Size stop = chrono_get_time_as_microseconds();          \
            avg_time += stop - start;                                   \
            vector_print(vec, my_print_i32); newline();                \
            vector_clear(vec);                                      \
        }                                                               \
        OK(#TestName, "Average test time for %zu iterations of %zu array size is %f ms\n", iter_count, arr_size, avg_time/iter_count/1000); \
        RETURN_VALUE_IF_FAIL(vector_check_sorted(vec, compare_i32), False, "ARRAY NOT SORTED!\n"); \
                                                                        \
        i32_vector_destroy(vec);                                    \
        return True;                                                    \
    }

TEST_VECTOR_SORT_FN(InsertionSort, insertion);
TEST_VECTOR_SORT_FN(BubbleSort, bubble);
TEST_VECTOR_SORT_FN(MergeSort, merge);

BEGIN_TESTS(IntegerVector)
    // SORTING
    TEST(MergeSort),
    TEST(BubbleSort),
    TEST(InsertionSort),

    // MISC
    TEST(Swap),
    TEST(Filter),
    TEST(Merge),

    // push/pop APIs
    TEST(PopFront),
    TEST(PushFront),
    TEST(PopBack),
    TEST(PushBack),

    // insert delete tests
    TEST(Remove),
    TEST(RemoveFast),
    TEST(DeleteFast),
    TEST(InsertFast),
    TEST(Delete),
    TEST(Insert),

    // constructor tests
    TEST(Create1),
    TEST(Create2),
    TEST(Create3),
    TEST(Create4),
END_TESTS()
