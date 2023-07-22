/**
 * @file StructVector.c
 * @date Wed, 15th July, 2023
 * @author Siddharth Mishra (admin@brightprogrammer.in)
 * @copyright Copyright (c) 2023 Anvie Labs
 *
 * Anvie Struct Vector Container Tests
 * */

#include <Anvie/Containers/Vector.h>
#include <Anvie/Test/UnitTest.h>

#define TEST_DATA_SIZE ((Size)91)

/**
 * Declare a test struct
 * */
typedef struct string_entry_t {
    String s_name;
    Size len;
} StringEntry;

static String allocated_strings[TEST_DATA_SIZE] = {0};
static Size strid = 0;

void CreateCopy(void* dst, void* src) {
    StringEntry* to = (StringEntry*)dst;
    StringEntry* from = (StringEntry*)src;
    RETURN_IF_FAIL(to && from && from->s_name && from->len , ERR_INVALID_ARGUMENTS);

    to->s_name = strdup(from->s_name);
    FATAL_IF(!to->s_name, ERR_OUT_OF_MEMORY);
    to->len = from->len;

//    DBG(__FUNCTION__, "STRID = \"%zu\"\n", strid);
    allocated_strings[strid++] = to->s_name;
}

void DestroyCopy(void* copy) {
    StringEntry* s = (StringEntry*)copy;
    RETURN_IF_FAIL(s && s->s_name && s->len , ERR_INVALID_ARGUMENTS);

    Bool b_found = False;
    for(Size i = 0; i < TEST_DATA_SIZE; i++) {
        if(allocated_strings[i] == s->s_name) {
            allocated_strings[i] = NULL;
            b_found = True;
            break;
        }
    }

    if(!b_found) {
        DBG(__FUNCTION__, "PROVIDED STRING ENTRY WAS NEVER ALLOCATED!\n");
        return;
    }

    FREE(s->s_name);
    s->len = 0;
    s->s_name = NULL;
}

Bool CompareString(StringEntry* s1, StringEntry* s2) {
    RETURN_VALUE_IF_FAIL(s1 && s2, False, ERR_INVALID_ARGUMENTS);
    return (s1->len == s2->len) && (strcmp(s1->s_name, s1->s_name) == 0);
}

DEF_ANV_STRUCT_VECTOR_INTERFACE(String, StringEntry, CreateCopy, DestroyCopy);

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create1() {
    AnvVector* vec = AnvVector_Create(sizeof(StringEntry), NULL, NULL);
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
    AnvVector* vec = AnvVector_Create(sizeof(StringEntry), CreateCopy, DestroyCopy);
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
    AnvVector* vec = AnvVector_Create(sizeof(StringEntry), NULL, DestroyCopy);
    RETURN_VALUE_IF_FAIL(!vec, False, "AnvVector CREATION SHOULD HAVE FAILED\n");
    if(vec) AnvVector_Destroy(vec); // we don't actually need to destroy, but still...
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create4() {
    AnvVector* vec = AnvVector_Create(sizeof(StringEntry), CreateCopy, NULL);
    RETURN_VALUE_IF_FAIL(!vec, False, "AnvVector CREATION SHOULD HAVE FAILED\n");
    if(vec) AnvVector_Destroy(vec); // we don't actually need to destroy, but still...
    return True;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Insert() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_Insert(vec, &entry, iter);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH\n");
        res = False;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
        res = False;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = AnvStringVector_Peek(vec, iter);

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(!res) break;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete operation of vector
 * */
TEST_FN Bool Delete() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_Insert(vec, &entry, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_Delete(vec, rand() % vec->length);

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

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Remove() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_Insert(vec, &entry, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = AnvStringVector_Remove(vec, 0);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        DestroyCopy(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert fast operation of vector
 * */
TEST_FN Bool InsertFast() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_InsertFast(vec, &entry, iter);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH\n");
        res = False;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
        res = False;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = AnvStringVector_Peek(vec, iter);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!res) break;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool DeleteFast() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_InsertFast(vec, &entry, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_DeleteFast(vec, rand() % vec->length);

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

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool RemoveFast() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_InsertFast(vec, &entry, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = AnvStringVector_RemoveFast(vec, 0);
        if(!CompareString(ref, &entry)) {

            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        DestroyCopy(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushBack() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_PushBack(vec, &entry);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH | EXPECTED : \"%zu\" | GOT : \"%zu\"\n", TEST_DATA_SIZE, vec->length);
        res = False;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
        res = False;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = AnvStringVector_Peek(vec, iter);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!res) break;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopBack() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_PushBack(vec, &entry);
    }

    // delete all elements and check size
    for(Size iter = TEST_DATA_SIZE; iter; iter--) {
        StringEntry* ref = AnvStringVector_PopBack(vec);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        DestroyCopy(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushFront() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // insert array backwards
        AnvStringVector_PushFront(vec, &entry);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, "VECTOR LENGTH MISMATCH\n");
        res = False;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, "VECTOR CAPACITY CANNOT BE LESS THAN VECTOR LENGTH\n");
        res = False;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = AnvStringVector_Peek(vec, iter);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            DBG(__FUNCTION__, "REF = \"%p\" | REF.STRING = \"%s\" | REF.LEN = \"%zu\"\n", ref, ref->s_name, ref->len);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!res) break;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopFront() {
    strid = 0;

    StringEntry entry = {
        .s_name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    AnvStringVector* vec = AnvStringVector_Create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        AnvStringVector_PushBack(vec, &entry);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = AnvStringVector_PopFront(vec);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        DestroyCopy(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    AnvStringVector_Destroy(vec);
    return res;
}

BEGIN_TESTS(AnvStructVector)
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
