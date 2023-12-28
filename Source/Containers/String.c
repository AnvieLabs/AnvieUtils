/**
 * @file String.c
 * @date Mon, 7th October, 2023
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
 * @brief A string buffer is used to efficiently store strings
 * in a memory safe manner and quite efficient manner based on use case.
 * */

#include <Anvie/Containers/String.h>
#include <Anvie/Error.h>
#include <string.h>

#ifndef STR_INITIAL_CAPACITY
#define STR_INITIAL_CAPACITY 32
#endif// STR_INITIAL_CAPACITY

/**
 * Create a new string buffer object. If @p zstr is @c NULL
 * then empty string will be created with some default initial
 * capacity to store new strings.
 *
 * @param zstr String to be set initially. This can be @c NULL.
 * @return String on success, NULL otherwise.
 * */
String* str_create(ZString zstr) {
    String* sb = NEW(String);
    ERR_RETURN_VALUE_IF_FAIL(sb, NULL, ERR_OUT_OF_MEMORY);

    /* get size to allocate for storing string */
    Size zstrlen = zstr ? strlen(zstr) : 0;
    sb->capacity = MAX(zstrlen, STR_INITIAL_CAPACITY);

    /* string objects are not null terminated,
     * so no need to allocate an extra byte for that */
    sb->data = ALLOCATE(Char, sb->capacity);
    if(!sb->data) {
        ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
        FREE(sb);
        return NULL;
    }

    /* if string is not null then create copy and adjust length */
    if(zstr) {
        memcpy(sb->data, zstr, sb->capacity);
        sb->length = zstrlen;
    }

    return sb;
}

/**
 * Destroy given string buffer object.
 *
 * @param string String buffer object to be destroyed.
 * */
void str_destroy(String* string) {
    ERR_RETURN_IF_FAIL(string, ERR_INVALID_ARGUMENTS);

    if(string->data) {
        memset(string->data, 0, string->capacity);
        FREE(string->data);
        string->data = NULL;
    }

    FREE(string);
}

/**
 * Clone the given @c String.
 * This will clone every aspect of given @c String.
 * The two buffers will be identical in their content,
 * capacity and length.
 *
 * @param sb @c String to be cloned.
 *
 * @return String* on success, @c NULL otherwise.
 * */
String* str_clone(String* sb) {
    ERR_RETURN_VALUE_IF_FAIL(sb, NULL, ERR_INVALID_ARGUMENTS);

    String* sbclone = NEW(String);
    ERR_RETURN_VALUE_IF_FAIL(sbclone, NULL, ERR_OUT_OF_MEMORY);

    sbclone->data = ALLOCATE(Char, sb->capacity);
    if(!sbclone->data) {
        ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
        FREE(sbclone);
        return NULL;
    }

    memcpy((void*)sbclone->data, sb->data, sb->length);
    sbclone->capacity = sb->capacity;
    sbclone->length   = sb->length;

    return sbclone;
}

/**
 * Create a NULL terminated data copy of @c String object.
 *
 * @param sb @c String object.
 * @return Duplicated @c ZString on success, @c NULL otherwise.
 * This does not check for @c NULL returned by strdup.
 * */
ZString str_clone_zstr(String* sb) {
    ERR_RETURN_VALUE_IF_FAIL(sb, NULL, ERR_INVALID_ARGUMENTS);

    Char* zstrclone = (Char*)malloc(sb->length + 1);
    ERR_RETURN_VALUE_IF_FAIL(zstrclone, INVALID_ZSTRING, ERR_OUT_OF_MEMORY);
    memcpy(TO_VOIDPTR(zstrclone), sb->data, sb->length);
    zstrclone[sb->length] = 0;

    return (ZString)(zstrclone);
}

/**
 * Set the given null terminated string to given string.
 *
 * @param string String object
 * @param str String to be set in the given @c String.
 * Passing NULL is equivalent to clearing the string.
 * */
void str_set_zstr(String* str, ZString zstr) {
    ERR_RETURN_IF_FAIL(str, ERR_INVALID_ARGUMENTS);

    /* no need to go further if zstr is NULL */
    if(!zstr) {
        str->length = 0;
        return;
    }

    Size zstrlen = strlen(zstr);

    /* realloc to store string */
    if(zstrlen > str->capacity) {
        Char* tmp = realloc(str->data, zstrlen);
        if(!tmp) {
            ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
            return;
        }
        str->capacity = zstrlen;
    }

    /* create copy of given null terminated string */
    memcpy(str->data, zstr, zstrlen);
    str->length = zstrlen;
}

/**
 * Set only first n bytes of given string to the given @c String.
 * If value of n is such that it escapes the NULL termination character,
 * then the value is clamped to size of string.
 *
 * @param string @c String object.
 * @param str String to be duplicated.
 * @param n Number of characters to be duplicated from the beginning of
 * given string. Behaviour is undefined if @p n is greater than size
 * of @p str.
 * */
void str_setn_zstr(String* str, ZString zstr, Size n) {
    ERR_RETURN_IF_FAIL(str, ERR_INVALID_ARGUMENTS);

    /* check if we need to proceed further */
    if(!(zstr && n)) {
        str->length = 0;
        return;
    }

    /* clamp value */
    Size zstrlen = strlen(zstr);
    n = MIN(zstrlen, n);

    /* null terminate string at index */
    Char* _zstr = (Char*)zstr;
    Char tmpchar = _zstr[n];
    _zstr[n] = 0;

    /* set string */
    str_set_zstr(str, zstr);

    /* revert to original string */
    _zstr[n] = tmpchar;
}

/**
 * Compare a @c String and a @c NULL terminated
 * string.
 *
 * @param sb @c String.
 * @param s C style @c NULL terminated string.
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 str_cmp_zstr(String* sb, ZString s) {
    ERR_RETURN_VALUE_IF_FAIL(sb && s, 1, ERR_INVALID_ARGUMENTS);

    Size zstrlen = strlen(s);
    if(zstrlen != sb->length) return 1;
    return memcmp(sb->data, s, sb->length);
}

/**
 * Compare first n bytes of a @c String and a @c NULL terminated
 * string.
 *
 * @param sb @c String.
 * @param s C style @c NULL terminated string.
 * @param n Number of bytes to compare from beginning of given string.
 * Behaviour is undefined if @p n is greater than size of @p str or @p sb.
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 str_cmpn_zstr(String* sb, ZString s, Size n) {
    ERR_RETURN_VALUE_IF_FAIL(sb && s && n, 1, ERR_INVALID_ARGUMENTS);

    Size zstrlen = MIN(strlen(s), n);
    return memcmp(sb->data, s, MIN(sb->length, zstrlen));
}

/**
 * @c Compare two @c Strings.
 *
 * @param sb1 @c String.
 * @param sb2 @c String.
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 str_cmp(String* sb1, String* sb2) {
    ERR_RETURN_VALUE_IF_FAIL(sb1 && sb2, 1, ERR_INVALID_ARGUMENTS);

    if(sb1->length != sb2->length) return 1;
    return memcmp(sb1->data, sb2->data, sb1->length);
}

/**
 * @c Compare first n bytes of two @c Strings.
 *
 * @param sb1 @c String
 * @param sb2 @c String
 * @param n Number of bytes to compare from beginning of given string.
 * Behaviour is undefined if @p n is greater than size of @p sb1 or @p sb2.
 *
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 str_cmpn(String* sb1, String* sb2, Size n) {
    ERR_RETURN_VALUE_IF_FAIL(sb1 && sb2 && n, 1, ERR_INVALID_ARGUMENTS);

    return memcmp(sb1->data, sb2->data, MIN3(sb1->length, sb2->length, n));
}

/**
 * Reserve space in @c String. This only changes
 * the capacity if @p n is greater than current capacity.
 * Reserving space is a good step if you know an upper bound
 * on the length of string.
 *
 * @param buf @c String to reserve space for.
 * @param n Number of bytes to reserve.
 * */
void str_reserve(String* buf, Size n) {
    ERR_RETURN_IF_FAIL(buf && n, ERR_INVALID_ARGUMENTS);

    /* resize only if reserve size is greater than capacity */
    if(n > buf->capacity) {
        Char* tmpstr = realloc((void*)buf->data, n + 1);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
            return;
        }
        buf->data = tmpstr;
        buf->capacity = n;
    }
}

/**
 * Clear contents of given string in O(n) time.
 * This is slower but safer than it's faster alternative.
 * This is suggested when using with unsafe code,
 * that might try to read more than length of string.
 *
 * @param buf
 * */
void str_clear(String* buf) {
    ERR_RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);
    memset(buf->data, 0, buf->length);
    buf->length = 0;
}

/**
 * Clear contents of given string in constant time.
 * This is way faster but not safe to data leaks
 * than it's slower alternative.
 *
 * This is however recommended if it's sure that
 * no part of code will ever try to read more
 * than length of string.
 *
 * @param buf
 * */
void str_clear_fast(String* buf) {
    ERR_RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);
    buf->length = 0;
}

/**
 * Append a character to end of given @c String.
 *
 * @param buf @c String
 * @param c Character to be appended.
 * */
void str_push_char(String* buf, Char c) {
    ERR_RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);

    Size newlen = buf->length++;

    // allocate new space if needed
    if(newlen > buf->capacity) {
        Size newcap = buf->capacity;
        while(newlen >= newcap) newcap *= 2;
        Char* tmpstr = realloc((void*)buf->data, newcap);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
            return;
        }
        buf->data = tmpstr;
        buf->capacity = newcap;
    }

    buf->data[buf->length - 1] = c;
    buf->length = newlen;
}

/**
 * Pop last character from string.
 *
 * @param str String to pop out character from
 * @return Char last character popped out from string.
 * */
Char str_pop_char(String* str) {
    ERR_RETURN_VALUE_IF_FAIL(str, 0, ERR_INVALID_ARGUMENTS);
    return str->data[str->length--];
}

/**
 * Append a string to this @c String.
 * Nothing is appended if @p s is @c NULL.
 *
 * @param buf @c String to append given string to.
 * @param s @c NULL terminated string
 * */
void str_push_zstr(String* buf, ZString s) {
    if(!s) return;
    ERR_RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);

    Size ssz = strlen(s);
    Size newlen = buf->length + ssz;

    // allocate new space if needed
    if(newlen > buf->capacity) {
        Size newcap = buf->capacity;
        while(newlen >= newcap) newcap *= 2;
        Char* tmpstr = realloc((void*)buf->data, newcap);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
            return;
        }
        buf->data = tmpstr;
        buf->capacity = newcap;
    }

    memcpy((void*)(buf->data + buf->length), s, ssz);
    buf->length = newlen;
}

/**
 * Append first @p n bytes of given string @p s to given
 * @c String @p buf. No action is performed if
 * either of @p s or @p n are zero.
 * If n is greater than length of string then it's clamped
 * to copy only string, not outside it's boundary.
 *
 * @param buf @c String to be appended to.
 * @param s String to be appended.
 * @param n Nunber of bytes to append to @c String. Must be
 * less than or equal to size of given string.
 * */
void str_pushn_zstr(String* buf, ZString s, Size n) {
    if(!s || !n) return;
    ERR_RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);

    Size ssz = MIN(n, strlen(s));
    Size newlen = buf->length + ssz;

    // allocate new space if needed
    if(newlen >= buf->capacity) {
        Size newcap = buf->capacity;
        while(newlen >= newcap) newcap *= 2;
        Char* tmpstr = realloc((void*)buf->data, newcap + 1);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
            return;
        }
        buf->data = tmpstr;
        buf->capacity = newcap;
    }

    memcpy((void*)(buf->data + buf->length), s, ssz);
    buf->length = newlen;
}

/**
 * Remove and create a NULL terminated string of size
 * n from last of given @c String object.
 *
 * @param str
 * @param n
 * @return ZString
 * */
ZString str_popn_zstr(String* str, Size n) {
    ERR_RETURN_VALUE_IF_FAIL(str && n, NULL, ERR_INVALID_ARGUMENTS);

    /* create space for null terminated string */
    Char* zstr = malloc(n+1);
    zstr[n] = 0;

    str->length -= n;
    memcpy(zstr, str->data + str->length, n);

    return zstr;
}
