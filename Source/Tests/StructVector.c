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

#define TEST_DATA_SIZE ((Size)91)

/**
 * Declare a test struct
 * */
typedef struct string_entry_t {
    String name;
    Size len;
} StringEntry;

// NOTE: Size of this array might be cause of your segfault if everything seems fine
static String allocated_strings[TEST_DATA_SIZE*2] = {0};
static Size strid = 0;

static inline void string_entry_init(StringEntry* se, String s) {
    RETURN_IF_FAIL(se && s, ERR_INVALID_ARGUMENTS);
    se->name = s;
    se->len = strlen(se->name);
}

void __StringEntry_CreateCopy___(void* dst, void* src) {
    StringEntry* to = (StringEntry*)dst;
    StringEntry* from = (StringEntry*)src;
    RETURN_IF_FAIL(to && from && from->name && from->len , ERR_INVALID_ARGUMENTS);

    to->name = strdup(from->name);
    FATAL_IF(!to->name, ERR_OUT_OF_MEMORY);
    to->len = from->len;

//    DBG(__FUNCTION__, "STRID = \"%zu\"\n", strid);
    allocated_strings[strid++] = to->name;
}

void __StringEntry_DestroyCopy___(void* copy) {
    StringEntry* s = (StringEntry*)copy;
    RETURN_IF_FAIL(s && s->name && s->len , ERR_INVALID_ARGUMENTS);

    Bool b_found = False;
    for(Size i = 0; i < TEST_DATA_SIZE; i++) {
        if(allocated_strings[i] == s->name) {
            allocated_strings[i] = NULL;
            b_found = True;
            break;
        }
    }

    if(!b_found) {
        DBG(__FUNCTION__, "PROVIDED STRING ENTRY WAS NEVER ALLOCATED!\n");
        return;
    }

    FREE(s->name);
    s->len = 0;
    s->name = NULL;
}

static inline Bool CompareString(StringEntry* s1, StringEntry* s2) {
    RETURN_VALUE_IF_FAIL(s1 && s2, False, ERR_INVALID_ARGUMENTS);
    return (s1->len == s2->len) && (strcmp(s1->name, s1->name) == 0);
}

DEF_STRUCT_VECTOR_INTERFACE(string_entry, StringEntry, __StringEntry_CreateCopy___, __StringEntry_DestroyCopy___);

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create1() {
    Vector* vec = vector_create(sizeof(StringEntry), NULL, NULL);
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
    Vector* vec = vector_create(sizeof(StringEntry), __StringEntry_CreateCopy___, __StringEntry_DestroyCopy___);
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
    Vector* vec = vector_create(sizeof(StringEntry), NULL, __StringEntry_DestroyCopy___);
    RETURN_VALUE_IF_FAIL(!vec, False, "vector CREATION SHOULD HAVE FAILED\n");
    if(vec) vector_destroy(vec); // we don't actually need to destroy, but still...
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create4() {
    Vector* vec = vector_create(sizeof(StringEntry), __StringEntry_CreateCopy___, NULL);
    RETURN_VALUE_IF_FAIL(!vec, False, "vector CREATION SHOULD HAVE FAILED\n");
    if(vec) vector_destroy(vec); // we don't actually need to destroy, but still...
    return True;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Insert() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_insert(vec, &entry, iter);
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
        StringEntry* ref = string_entry_vector_peek(vec, iter);

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(!res) break;
    }

    string_entry_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete operation of vector
 * */
TEST_FN Bool Delete() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_insert(vec, &entry, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_delete(vec, rand() % vec->length);

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

    string_entry_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool Remove() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_insert(vec, &entry, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = string_entry_vector_remove(vec, 0);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        __StringEntry_DestroyCopy___(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    string_entry_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert fast operation of vector
 * */
TEST_FN Bool InsertFast() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_insert_fast(vec, &entry, iter);
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
        StringEntry* ref = string_entry_vector_peek(vec, iter);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!res) break;
    }

    string_entry_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool DeleteFast() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_insert_fast(vec, &entry, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_delete_fast(vec, rand() % vec->length);

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

    string_entry_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check insert operation of vector
 * */
TEST_FN Bool RemoveFast() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_insert_fast(vec, &entry, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = string_entry_vector_remove_fast(vec, 0);
        if(!CompareString(ref, &entry)) {

            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        __StringEntry_DestroyCopy___(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    string_entry_vector_destroy(vec);
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
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_push_back(vec, &entry);
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
        StringEntry* ref = string_entry_vector_peek(vec, iter);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!res) break;
    }

    string_entry_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopBack() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_push_back(vec, &entry);
    }

    // delete all elements and check size
    for(Size iter = TEST_DATA_SIZE; iter; iter--) {
        StringEntry* ref = string_entry_vector_poback(vec);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        __StringEntry_DestroyCopy___(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    string_entry_vector_destroy(vec);
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
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // insert array backwards
        string_entry_vector_push_front(vec, &entry);
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
        StringEntry* ref = string_entry_vector_peek(vec, iter);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            DBG(__FUNCTION__, "REF = \"%p\" | REF.STRING = \"%s\" | REF.LEN = \"%zu\"\n", ref, ref->name, ref->len);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        if(!res) break;
    }

    string_entry_vector_destroy(vec);
    return res;
}

/**
 * @TEST
 * Check delete fast operation of vector
 * */
TEST_FN Bool PopFront() {
    strid = 0;

    StringEntry entry = {
        .name = "AnvieUtils",
        .len = strlen("AnvieUtils")
    };

    Bool res = True;
    // first create vector with some valid data
    Vector* vec = string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        string_entry_vector_push_back(vec, &entry);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = string_entry_vector_pofront(vec);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->name == entry.name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        __StringEntry_DestroyCopy___(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    if(vec->length != 0) {
        DBG(__FUNCTION__, "VECTOR LENGTH AFTER DELETING ALL ENTRIES MUST BE ZERO, FOUND \"%zu\"\n", vec->length);
        res = False;
    }

    string_entry_vector_destroy(vec);
    return res;
}

TEST_FN Bool Merge() {
    strid = 0;

    Vector* /* String */ vec1 = string_entry_vector_create();
    Vector* /* String */ vec2 = string_entry_vector_create();

    StringEntry se;

    // prepare data
    // vec1
    string_entry_init(&se, "Siddharth");
    string_entry_vector_push_back(vec1, &se);

    string_entry_init(&se, "Mishra");
    string_entry_vector_push_back(vec1, &se);

    // vec2
    string_entry_init(&se, "is");
    string_entry_vector_push_back(vec2, &se);

    string_entry_init(&se, "@brightprogrammer");
    string_entry_vector_push_back(vec2, &se);

    // merge
    string_entry_vector_merge(vec1, vec2);

    // check merge
    RETURN_VALUE_IF_FAIL(!strcmp(string_entry_vector_peek(vec1, 2)->name, string_entry_vector_peek(vec2, 0)->name), False, "MERGE OPERATION IS INVALID! ELEMENTS DON'T MATCH!\n");
    RETURN_VALUE_IF_FAIL(!strcmp(string_entry_vector_peek(vec1, 3)->name, string_entry_vector_peek(vec2, 1)->name), False, "MERGE OPERATION IS INVALID! ELEMENTS DON'T MATCH!\n");

    string_entry_vector_destroy(vec1);
    string_entry_vector_destroy(vec2);

    return True;
}

static inline Bool element_filter(void* x, void* udata) {
    StringEntry* v = (StringEntry*)x;
    Size sz_limit = (Size)udata;

    return strlen(v->name) > sz_limit;
}

TEST_FN Bool Filter() {
    strid = 0;

    Vector* /* StringEntry */ vec = string_entry_vector_create();
    char str[11] = {0};
    StringEntry se;

    // prepare data
    for(int i = 0; i < 10; i++) {
        str[i] = 'a';
        string_entry_init(&se, str);
        string_entry_vector_push_back(vec, &se);
    }

    // all elements greater than 0 and less than equal to zero are filtered
    Vector* /* StringEntry */ vec_g5 = string_entry_vector_filter(vec, element_filter, (void*)5);
    if(!vec_g5){
        vector_destroy(vec);
        DBG(__FUNCTION__, "FAILED TO FILTER ELEMENTS (vec_g0)\n");
        return False;
    }

    // check length of all elements in vec_g5 are greater than 5
    for(Size s = 0; s < vec_g5->length; s++) {
        RETURN_VALUE_IF_FAIL(strlen(string_entry_vector_peek(vec_g5, s)->name) > 5, False, "FILTERED VECTOR CONTAINS WRONG CONTENT\n");
    }

    string_entry_vector_destroy(vec);
    string_entry_vector_destroy(vec_g5);

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
