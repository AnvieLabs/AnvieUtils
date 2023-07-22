/**
 * @file IntegerVector.c
 * @date Wed, 13th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Anvie Integer Vector Container Tests
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
    AnvVector* vec = AnvVector_Create(sizeof(Uint8), NULL, NULL);
    RETURN_VALUE_IF_FAIL(vec, False, "FAILED TO CREATE AnvVector\n");
    AnvVector_Destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create2() {
    AnvVector* vec = AnvVector_Create(sizeof(Uint16), (AnvCreateElementCopyCallback)1, (AnvDestroyElementCopyCallback)1);
    RETURN_VALUE_IF_FAIL(vec, False, "FAILED TO CREATE AnvVector\n");
    AnvVector_Destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create3() {
    AnvVector* vec = AnvVector_Create(sizeof(Uint32), NULL, (AnvDestroyElementCopyCallback)1);
    RETURN_VALUE_IF_FAIL(!vec, False, "AnvVector CREATION SHOULD HAVE FAILED\n");
    if(vec) AnvVector_Destroy(vec);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create4() {
    AnvVector* vec = AnvVector_Create(sizeof(Uint64), (AnvCreateElementCopyCallback)1, NULL);
    RETURN_VALUE_IF_FAIL(!vec, False, "AnvVector CREATION SHOULD HAVE FAILED\n");
    if(vec) AnvVector_Destroy(vec);
    return True;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Insert() {
    Bool res = True;
    AnvU64Vector* vec = AnvU64Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU32Vector_Insert(vec, iter, iter);

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
        if(AnvU64Vector_Peek(vec, iter) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    AnvU64Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete operation of vector
 * */
TEST_FN Bool Delete() {
    Bool res = True;

    // first create vector with some valid data
    AnvU16Vector* vec = AnvU16Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU16Vector_Insert(vec, iter, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU16Vector_Delete(vec, rand() % vec->length);

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

    AnvU16Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Remove() {
    Bool res = True;
    AnvU32Vector* vec = AnvU32Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU32Vector_Insert(vec, iter, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(AnvU32Vector_Remove(vec, 0) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; break;
        }
    }

    AnvU32Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert fast operation of vector
 * */
TEST_FN Bool InsertFast() {
    Bool res = True;
    AnvU32Vector* vec = AnvU32Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU32Vector_InsertFast(vec, iter, iter);
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
            if(AnvU32Vector_Peek(vec, k) == iter) {
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
    AnvU32Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool DeleteFast() {
    Bool res = True;

    // first create vector with some valid data
    AnvU64Vector* vec = AnvU64Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU64Vector_InsertFast(vec, iter, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU64Vector_DeleteFast(vec, rand() % vec->length);

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

    AnvU64Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool RemoveFast() {
    Bool res = True;
    AnvU32Vector* vec = AnvU32Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU32Vector_InsertFast(vec, iter, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // since order is not maintained, we just need to check for presence of each element
        Bool b_found = False;
        Uint32 val = AnvU32Vector_RemoveFast(vec, 0); // always remove from first position
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

    AnvU32Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushBack() {
    Bool res = True;
    AnvU32Vector* vec = AnvU32Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU32Vector_PushBack(vec, iter);
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
        if(AnvU32Vector_Peek(vec, iter) != iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    AnvU32Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopBack() {
    Bool res = True;
    // first create vector with some valid data
    AnvU32Vector* vec = AnvU32Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU64Vector_PushBack(vec, iter);
    }

    // delete all elements and check size
    for(Size iter = TEST_DATA_SIZE; iter; iter--) {
        if(AnvU32Vector_PopBack(vec) != iter-1) {
            DBG(__FUNCTION__, "PUSHED DATA MISMATCH AT ENTRY INDEX \"%zu\"\n", iter-1);
            res = False; break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    AnvU32Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushFront() {
    Bool res = True;
    AnvU32Vector* vec = AnvU32Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // insert array backwards
        AnvU32Vector_PushFront(vec, iter);
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
        if(AnvU32Vector_Peek(vec, iter) != TEST_DATA_SIZE - 1 - iter) {
            DBG(__FUNCTION__, "VECTOR INSERTED ELEMENTS MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; goto Exit;
        }
    }

Exit:
    AnvU32Vector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopFront() {
    Bool res = True;
    // first create vector with some valid data
    AnvU32Vector* vec = AnvU32Vector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvU64Vector_PushFront(vec, TEST_DATA_SIZE - 1 - iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        if(AnvU32Vector_PopFront(vec) != iter) {
            DBG(__FUNCTION__, "PUSHED DATA MISMATCH AT ENTRY INDEX \"%zu\"\n", iter);
            res = False; break;
        }
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    AnvU32Vector_Destroy(vec);
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
