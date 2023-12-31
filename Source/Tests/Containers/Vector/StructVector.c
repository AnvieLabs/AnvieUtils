/**
 * @file StructVector.c
 * @date Wed, 15th July, 2023
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
 * @brief Struct vector container tests
 * */

#include <Anvie/Containers/Vector.h>
#include <Anvie/Test/UnitTest.h>
#include <Anvie/Error.h>

#define TEST_DATA_SIZE ((Size)91)

/**
 * Declare a test struct
 * */
typedef struct ZStringEntry {
    ZString name;
    Size len;
} ZStringEntry;

#define CREATE_DATA 69
#define DESTROY_DATA -69
#define COMPARE_DATA 0

typedef struct ZStringData {
    Uint8 create; /**< should be 69 */
    Uint8 compare; /**< should be 0 */
    Int32 destroy; /**< should be -69 */
} ZStringData;

// NOTE: Size of this array might be cause of your segfault if everything seems fine
static ZString allocated_strings[TEST_DATA_SIZE*2] = {0};
static Size strid = 0;

static inline void se_init(ZStringEntry* se, ZString s) {
    ERR_RETURN_IF_FAIL(se && s, ERR_INVALID_ARGUMENTS);
    se->name = s;
    se->len = strlen(se->name);
}

void __ZStringEntry_CreateCopy___(ZStringEntry* dst, ZStringEntry* src, ZStringData* udata) {
    ERR_RETURN_IF_FAIL(dst && src && udata, ERR_INVALID_ARGUMENTS);

    ERR_RETURN_IF_FAIL(udata->create == CREATE_DATA, ERR_INVALID_USER_DATA);

    if(src->name && src->len) {
        dst->name = strdup(src->name);
        FATAL_IF(!dst->name, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
        dst->len = src->len;
    }

    allocated_strings[strid++] = dst->name;
}

void __ZStringEntry_DestroyCopy___(ZStringEntry* copy, ZStringData* udata) {
    ERR_RETURN_IF_FAIL(copy && udata, ERR_INVALID_ARGUMENTS);
    ERR_RETURN_IF_FAIL(udata->destroy == DESTROY_DATA, ERR_INVALID_USER_DATA);

    Bool b_found = False;
    for(Size i = 0; i < TEST_DATA_SIZE; i++) {
        if(copy->name && allocated_strings[i] == copy->name) {
            allocated_strings[i] = NULL;
            b_found = True;
            break;
        }
    }

    if(!b_found) {
        DBG(__FUNCTION__, "PROVIDED STRING ENTRY WAS NEVER ALLOCATED!\n");
        return;
    }

    FREE(copy->name);
    copy->len = 0;
    copy->name = NULL;
}

DEF_STRUCT_VECTOR_INTERFACE(zse, Zse, ZStringEntry, __ZStringEntry_CreateCopy___, __ZStringEntry_DestroyCopy___);

static inline Bool CompareZString(ZStringEntry* s1, ZStringEntry* s2) {
    ERR_RETURN_VALUE_IF_FAIL(s1 && s2, False, ERR_INVALID_ARGUMENTS);
    return (s1->name && s2->name) && (s1->len && s2->len) && (s1->len == s2->len) && (strcmp(s1->name, s1->name) == 0);
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create1() {
    Vector* vec = vector_create(sizeof(ZStringEntry), NULL, NULL);
    ERR_RETURN_VALUE_IF_FAIL(vec, False, ERR_INVALID_OBJECT);
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};
    vector_destroy(vec, &sd);
    return True;

}
/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create2() {
    Vector* vec = vector_create(
        sizeof(ZStringEntry),
        (CreateElementCopyCallback)(__ZStringEntry_CreateCopy___),
        (DestroyElementCopyCallback)(__ZStringEntry_DestroyCopy___)
    );
    ERR_RETURN_VALUE_IF_FAIL(vec, False, ERR_INVALID_OBJECT);
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};
    vector_destroy(vec, &sd);
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create3() {
    Vector* vec = vector_create(sizeof(ZStringEntry), NULL, (DestroyElementCopyCallback)(__ZStringEntry_DestroyCopy___));
    ERR_RETURN_VALUE_IF_FAIL(!vec, False, ERR_UNEXPECTED);
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};
    if(vec) vector_destroy(vec, &sd); // we don't actually need to destroy, but still...
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create4() {
    Vector* vec = vector_create(sizeof(ZStringEntry), (CreateElementCopyCallback)(__ZStringEntry_CreateCopy___), NULL);
    ERR_RETURN_VALUE_IF_FAIL(!vec, False, ERR_UNEXPECTED);
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};
    if(vec) vector_destroy(vec, &sd); // we don't actually need to destroy, but still...
    return True;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Insert() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_insert(vec, &entry, iter, &sd);
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
        ZStringEntry* ref = zse_vector_peek(vec, iter);

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!CompareZString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(!res) break;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Check delete operation of vector
 * */
TEST_FN Bool Delete() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    // first create vector with some valid data
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_insert(vec, &entry, iter, &sd);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_delete(vec, rand() % vec->length, &sd);

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

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Remove() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_insert(vec, &entry, iter, &sd);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        ZStringEntry* ref = zse_vector_remove(vec, 0);

        if(!CompareZString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        __ZStringEntry_DestroyCopy___(ref, &sd);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Check insert fast operation of vector
 * */
TEST_FN Bool InsertFast() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_insert_fast(vec, &entry, iter, &sd);
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
        ZStringEntry* ref = zse_vector_peek(vec, iter);

        if(!CompareZString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!res) break;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool DeleteFast() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    // first create vector with some valid data
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_insert_fast(vec, &entry, iter, &sd);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_delete_fast(vec, rand() % vec->length, &sd);

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

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool RemoveFast() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_insert_fast(vec, &entry, iter, &sd);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        ZStringEntry* ref = zse_vector_remove_fast(vec, 0);
        if(!CompareZString(ref, &entry) || (ref->name == entry.name)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        __ZStringEntry_DestroyCopy___(ref, &sd);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushBack() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_push_back(vec, &entry, &sd);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_LENGTH));
        res = False;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_CAPACITY));
        res = False;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        ZStringEntry* ref = zse_vector_peek(vec, iter);

        if(!CompareZString(ref, &entry) || (ref->name == entry.name)) {
            DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_CONTENTS));
            res = False;
        }

        if(!res) break;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopBack() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    // first create vector with some valid data
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_push_back(vec, &entry, &sd);
    }

    // delete all elements and check size
    for(Size iter = TEST_DATA_SIZE; iter; iter--) {
        ZStringEntry* ref = zse_vector_pop_back(vec);

        if(!CompareZString(ref, &entry) || (ref->name == entry.name)) {
            DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_CONTENTS));
            res = False;
        }

        __ZStringEntry_DestroyCopy___(ref, &sd);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_LENGTH));
        res = False;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Even though PushBack internally calls InsertFast internally
 * we still want to verify whether the API is working correctly
 * */
TEST_FN Bool PushFront() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // insert array backwards
        zse_vector_push_front(vec, &entry, &sd);
    }

    if(vec->length != TEST_DATA_SIZE) {
        DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_LENGTH));
        res = False;
    }

    if(vec->capacity < vec->length) {
        DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_CAPACITY));
        res = False;
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        ZStringEntry* ref = zse_vector_peek(vec, iter);

        if(!CompareZString(ref, &entry) || (ref->name == entry.name)) {
            DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_CONTENTS));
            res = False;
        }

        if(!res) break;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopFront() {
    strid = 0;

    ZStringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    Bool res = True;
    // first create vector with some valid data
    Zse_Vector* vec = zse_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        zse_vector_push_back(vec, &entry, &sd);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        ZStringEntry* ref = zse_vector_pop_front(vec);

        if(!CompareZString(ref, &entry) || (ref->name == entry.name)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        __ZStringEntry_DestroyCopy___(ref, &sd);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_INVALID_LENGTH));
        res = False;
    }

    zse_vector_destroy(vec, &sd);
    return res;
}

TEST_FN Bool Merge() {
    strid = 0;

    Zse_Vector* vec1 = zse_vector_create();
    Zse_Vector* vec2 = zse_vector_create();

    ZStringEntry se;
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    // prepare data
    // vec1
    se_init(&se, "Siddharth");
    zse_vector_push_back(vec1, &se, &sd);

    se_init(&se, "Mishra");
    zse_vector_push_back(vec1, &se, &sd);

    // vec2
    se_init(&se, "is");
    zse_vector_push_back(vec2, &se, &sd);

    se_init(&se, "@brightprogrammer");
    zse_vector_push_back(vec2, &se, &sd);

    // merge
    zse_vector_merge(vec1, vec2, &sd);

    // check merge
    ERR_RETURN_VALUE_IF_FAIL(!strcmp(zse_vector_peek(vec1, 2)->name, zse_vector_peek(vec2, 0)->name), False, ERR_OPERATION_FAILED);
    ERR_RETURN_VALUE_IF_FAIL(!strcmp(zse_vector_peek(vec1, 3)->name, zse_vector_peek(vec2, 1)->name), False, ERR_OPERATION_FAILED);

    zse_vector_destroy(vec1, &sd);
    zse_vector_destroy(vec2, &sd);

    return True;
}

static inline Bool element_filter(ZStringEntry* x, void* udata) {
    Size sz_limit = (Size)udata;

    return strlen(x->name) > sz_limit;
}

TEST_FN Bool Filter() {
    strid = 0;

    Zse_Vector* vec = zse_vector_create();
    char str[11] = {0};
    ZStringEntry se;
    ZStringData sd = {.create = CREATE_DATA, .compare = COMPARE_DATA, .destroy = DESTROY_DATA};

    // prepare data
    for(int i = 0; i < 10; i++) {
        str[i] = 'a';
        se_init(&se, str);
        zse_vector_push_back(vec, &se, &sd);
    }

    // all elements greater than 0 and less than equal to zero are filtered
    Zse_Vector* vec_g5 = zse_vector_filter(vec, element_filter, &sd);
    if(!vec_g5){
        zse_vector_destroy(vec, &sd);
        DBG(__FUNCTION__, ERRFMT, ERRMSG(ERR_OPERATION_FAILED));
        return False;
    }

    // check length of all elements in vec_g5 are greater than 5
    for(Size s = 0; s < vec_g5->length; s++) {
        ERR_RETURN_VALUE_IF_FAIL(strlen(zse_vector_peek(vec_g5, s)->name) > 5, False, ERR_INVALID_CONTENTS);
    }

    zse_vector_destroy(vec, &sd);
    zse_vector_destroy(vec_g5, &sd);

    return True;
}

BEGIN_TESTS(StructVector)
    // MISC
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
