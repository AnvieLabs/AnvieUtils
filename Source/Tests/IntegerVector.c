/**
 * @file IntegerVector.c
 * @date Wed, 13th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * AnvVector Container Tests
 * */

#include <Anvie/Containers/Vector.h>
#include <Anvie/Test/UnitTest.h>

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
    AnvVector* vec = anv_vector_create(sizeof(Uint8), NULL, NULL);
    RETURN_VALUE_IF_FAIL(vec, False, "FAILED TO CREATE anv_vector\n");
    anv_vector_destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create2() {
    AnvVector* vec = anv_vector_create(sizeof(Uint16), (AnvCreateElementCopyCallback)1, (AnvDestroyElementCopyCallback)1);
    RETURN_VALUE_IF_FAIL(vec, False, "FAILED TO CREATE anv_vector\n");
    anv_vector_destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create3() {
    AnvVector* vec = anv_vector_create(sizeof(Uint32), NULL, (AnvDestroyElementCopyCallback)1);
    RETURN_VALUE_IF_FAIL(!vec, False, "anv_vector CREATION SHOULD HAVE FAILED\n");
    if(vec) anv_vector_destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create4() {
    AnvVector* vec = anv_vector_create(sizeof(Uint64), (AnvCreateElementCopyCallback)1, NULL);
    RETURN_VALUE_IF_FAIL(!vec, False, "anv_vector CREATION SHOULD HAVE FAILED\n");
    if(vec) anv_vector_destroy(vec);
    return True;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Insert() {
    Bool res = True;
    AnvVector* vec = anv_u64_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_insert(vec, iter, iter);

        if(vec->length != iter + 1) {
            DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH\n");
            res = False; goto Exit;
        }

        if(vec->capacity < vec->length) {
            DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
            res = False; goto Exit;
        }
    }



    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(anv_u64_vector_peek(vec, iter) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    anv_u64_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete operation of vector
 * */
TEST_FN Bool Delete() {
    Bool res = True;

    // first create vector with some valid data
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_insert(vec, iter, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_delete(vec, rand() % vec->length);

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

    anv_u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Remove() {
    Bool res = True;
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_insert(vec, iter, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(anv_u32_vector_remove(vec, 0) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; break;
        }
    }

    anv_u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert fast operation of vector
 * */
TEST_FN Bool InsertFast() {
    Bool res = True;
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_insert_fast(vec, iter, iter);
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
            if(anv_u32_vector_peek(vec, k) == iter) {
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
    anv_u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool DeleteFast() {
    Bool res = True;

    // first create vector with some valid data
    AnvVector* vec = anv_u64_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u64_vector_insert_fast(vec, iter, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u64_vector_delete_fast(vec, rand() % vec->length);

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

    anv_u64_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool RemoveFast() {
    Bool res = True;
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_insert_fast(vec, iter, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // since order is not maintained, we just need to check for presence of each element
        Bool b_found = False;
        Uint32 val = anv_u32_vector_remove_fast(vec, 0); // always remove from first position
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

    anv_u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushBack() {
    Bool res = True;
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_push_back(vec, iter);
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
        if(anv_u32_vector_peek(vec, iter) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    anv_u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopBack() {
    Bool res = True;
    // first create vector with some valid data
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u64_vector_push_back(vec, iter);
    }

    // delete all elements and check size
    for(Size iter = TEST_DATA_SIZE; iter; iter--) {
        if(anv_u32_vector_pop_back(vec) != iter-1) {
            DBG(__FUNCTION__, "PUSHED DATA MISMATCH AT ENTRY INDEX \"%zu\"\n", iter-1);
            res = False; break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    anv_u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushFront() {
    Bool res = True;
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // insert array backwards
        anv_u32_vector_push_front(vec, iter);
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
        if(anv_u32_vector_peek(vec, iter) != TEST_DATA_SIZE - 1 - iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    anv_u32_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopFront() {
    Bool res = True;
    // first create vector with some valid data
    AnvVector* vec = anv_u32_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_u32_vector_push_front(vec, TEST_DATA_SIZE - 1 - iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(anv_u32_vector_pop_front(vec) != iter) {
            DBG(__FUNCTION__, "PUSHED DATA MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    anv_u32_vector_destroy(vec);
    return res;
}

BEGIN_TESTS(AnvIntegerVector)
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
