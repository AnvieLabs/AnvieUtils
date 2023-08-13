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
    String s_name;
    Size len;
} StringEntry;

// NOTE: Size of this array might be cause of your segfault if everything seems fine
static String allocated_strings[TEST_DATA_SIZE*2] = {0};
static Size strid = 0;

static inline void string_entry_init(StringEntry* p_se, String s) {
    RETURN_IF_FAIL(p_se && s, ERR_INVALID_ARGUMENTS);
    p_se->s_name = s;
    p_se->len = strlen(p_se->s_name);
}

void __StringEntry_CreateCopy___(void* dst, void* src) {
    StringEntry* to = (StringEntry*)dst;
    StringEntry* from = (StringEntry*)src;
    RETURN_IF_FAIL(to && from && from->s_name && from->len , ERR_INVALID_ARGUMENTS);

    to->s_name = strdup(from->s_name);
    FATAL_IF(!to->s_name, ERR_OUT_OF_MEMORY);
    to->len = from->len;

//    DBG(__FUNCTION__, "STRID = \"%zu\"\n", strid);
    allocated_strings[strid++] = to->s_name;
}

void __StringEntry_DestroyCopy___(void* copy) {
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

static inline Bool CompareString(StringEntry* s1, StringEntry* s2) {
    RETURN_VALUE_IF_FAIL(s1 && s2, False, ERR_INVALID_ARGUMENTS);
    return (s1->len == s2->len) && (strcmp(s1->s_name, s1->s_name) == 0);
}

DEF_ANV_STRUCT_VECTOR_INTERFACE(string_entry, StringEntry, __StringEntry_CreateCopy___, __StringEntry_DestroyCopy___);

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create1() {
    AnvVector* vec = anv_vector_create(sizeof(StringEntry), NULL, NULL);
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
    AnvVector* vec = anv_vector_create(sizeof(StringEntry), __StringEntry_CreateCopy___, __StringEntry_DestroyCopy___);
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
    AnvVector* vec = anv_vector_create(sizeof(StringEntry), NULL, __StringEntry_DestroyCopy___);
    RETURN_VALUE_IF_FAIL(!vec, False, "anv_vector CREATION SHOULD HAVE FAILED\n");
    if(vec) anv_vector_destroy(vec); // we don't actually need to destroy, but still...
    return True;
}

/**
 * @TEST
 * Vector creation must fail when both create_copy()
 * and destory_copy() are not null or nonnull at the same time.
 * */
TEST_FN Bool Create4() {
    AnvVector* vec = anv_vector_create(sizeof(StringEntry), __StringEntry_CreateCopy___, NULL);
    RETURN_VALUE_IF_FAIL(!vec, False, "anv_vector CREATION SHOULD HAVE FAILED\n");
    if(vec) anv_vector_destroy(vec); // we don't actually need to destroy, but still...
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_insert(vec, &entry, iter);
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
        StringEntry* ref = anv_string_entry_vector_peek(vec, iter);

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

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_insert(vec, &entry, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_delete(vec, rand() % vec->length);

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

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_insert(vec, &entry, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = anv_string_entry_vector_remove(vec, 0);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        __StringEntry_DestroyCopy___(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_insert_fast(vec, &entry, iter);
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
        StringEntry* ref = anv_string_entry_vector_peek(vec, iter);

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

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_insert_fast(vec, &entry, iter);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_delete_fast(vec, rand() % vec->length);

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

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_insert_fast(vec, &entry, iter);
    }

    // check whether data is correct or incorrect
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = anv_string_entry_vector_remove_fast(vec, 0);
        if(!CompareString(ref, &entry)) {

            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
            DBG(__FUNCTION__, "BOTH STRINGS MUST NOT POINT TO SAME ADDRESS\n");
            res = False;
        }

        __StringEntry_DestroyCopy___(ref);
        FREE(ref);
        ref = NULL;

        if(!res) break;
    }

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_push_back(vec, &entry);
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
        StringEntry* ref = anv_string_entry_vector_peek(vec, iter);

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

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_push_back(vec, &entry);
    }

    // delete all elements and check size
    for(Size iter = TEST_DATA_SIZE; iter; iter--) {
        StringEntry* ref = anv_string_entry_vector_pop_back(vec);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
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

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        // insert array backwards
        anv_string_entry_vector_push_front(vec, &entry);
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
        StringEntry* ref = anv_string_entry_vector_peek(vec, iter);

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

    anv_string_entry_vector_destroy(vec);
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
    AnvVector* vec = anv_string_entry_vector_create();
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        anv_string_entry_vector_push_back(vec, &entry);
    }

    // delete all elements and check size
    for(Size iter = 0; iter < TEST_DATA_SIZE; iter++) {
        StringEntry* ref = anv_string_entry_vector_pop_front(vec);

        if(!CompareString(ref, &entry)) {
            DBG(__FUNCTION__, "INVALID COPY FOUND IN STRUCT ARRAY AT INDEX \"%zu\"\n", iter);
            res = False;
        }

        if(ref->s_name == entry.s_name) {
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

    anv_string_entry_vector_destroy(vec);
    return res;
}

TEST_FN Bool Merge() {
    strid = 0;

    AnvVector* /* String */ vec1 = anv_string_entry_vector_create();
    AnvVector* /* String */ vec2 = anv_string_entry_vector_create();

    StringEntry se;

    // prepare data
    // vec1
    string_entry_init(&se, "Siddharth");
    anv_string_entry_vector_push_back(vec1, &se);

    string_entry_init(&se, "Mishra");
    anv_string_entry_vector_push_back(vec1, &se);

    // vec2
    string_entry_init(&se, "is");
    anv_string_entry_vector_push_back(vec2, &se);

    string_entry_init(&se, "@brightprogrammer");
    anv_string_entry_vector_push_back(vec2, &se);

    // merge
    anv_string_entry_vector_merge(vec1, vec2);

    // check merge
    RETURN_VALUE_IF_FAIL(!strcmp(anv_string_entry_vector_peek(vec1, 2)->s_name, anv_string_entry_vector_peek(vec2, 0)->s_name), False, "MERGE OPERATION IS INVALID! ELEMENTS DON'T MATCH!\n");
    RETURN_VALUE_IF_FAIL(!strcmp(anv_string_entry_vector_peek(vec1, 3)->s_name, anv_string_entry_vector_peek(vec2, 1)->s_name), False, "MERGE OPERATION IS INVALID! ELEMENTS DON'T MATCH!\n");

    anv_string_entry_vector_destroy(vec1);
    anv_string_entry_vector_destroy(vec2);

    return True;
}

static inline Bool element_filter(void* x, void* udata) {
    StringEntry* v = (StringEntry*)x;
    Size sz_limit = (Size)udata;

    return strlen(v->s_name) > sz_limit;
}

TEST_FN Bool Filter() {
    strid = 0;

    AnvVector* /* StringEntry */ vec = anv_string_entry_vector_create();
    char str[11] = {0};
    StringEntry se;

    // prepare data
    for(int i = 0; i < 10; i++) {
        str[i] = 'a';
        string_entry_init(&se, str);
        anv_string_entry_vector_push_back(vec, &se);
    }

    // all elements greater than 0 and less than equal to zero are filtered
    AnvVector* /* StringEntry */ vec_g5 = anv_string_entry_vector_filter(vec, element_filter, (void*)5);
    if(!vec_g5){
        anv_vector_destroy(vec);
        DBG(__FUNCTION__, "FAILED TO FILTER ELEMENTS (vec_g0)\n");
        return False;
    }

    // check length of all elements in vec_g5 are greater than 5
    for(Size s = 0; s < vec_g5->length; s++) {
        RETURN_VALUE_IF_FAIL(strlen(anv_string_entry_vector_peek(vec_g5, s)->s_name) > 5, False, "FILTERED VECTOR CONTAINS WRONG CONTENT\n");
    }

    anv_string_entry_vector_destroy(vec);
    anv_string_entry_vector_destroy(vec_g5);

    return True;
}

BEGIN_TESTS(AnvStructVector)
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
