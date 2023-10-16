/**
 * @file StringBuffer.c
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

#include <Anvie/Containers/StringBuffer.h>
#include <string.h>

/**
 * Create a new string buffer object.
 *
 * @param str String to be set initially. This can be @c NULL.
 *
 * @return AnvStringBuffer on success, NULL otherwise.
 * */
AnvStringBuffer* anv_strbuf_create(String str) {
    AnvStringBuffer* sb = NEW(AnvStringBuffer);
    RETURN_VALUE_IF_FAIL(sb, NULL, ERR_OUT_OF_MEMORY);

    if(str) {
        sb->length = strlen(str);
        sb->capacity = sb->length;
        sb->str = strdup(str);
        if(!sb->str) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            FREE(sb);
            return NULL;
        }
    }

    return sb;
}

/**
 * Destroy given string buffer object.
 *
 * @param strbuf String buffer object to be destroyed.
 * */
void anv_strbuf_destroy(AnvStringBuffer* strbuf) {
    RETURN_IF_FAIL(strbuf, ERR_INVALID_ARGUMENTS);

    if(strbuf->str) {
        FREE(strbuf->str);
        strbuf->str = NULL;
    }

    FREE(strbuf);
}

/**
 * Clone the given @c StringBuffer.
 * This will clone every aspect of given @c StringBuffer.
 * The two buffers will be identical in their content,
 * capacity and length.
 *
 * @param sb @c StringBuffer to be cloned.
 *
 * @return AnvStringBuffer* on success, @c NULL otherwise.
 * */
AnvStringBuffer* anv_strbuf_clone_buf(AnvStringBuffer* sb) {
    RETURN_VALUE_IF_FAIL(sb, NULL, ERR_INVALID_ARGUMENTS);

    AnvStringBuffer* sbclone = NEW(AnvStringBuffer);

    sbclone->str = ALLOCATE(Char, sb->capacity);
    if(!sbclone->str) {
        ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
        FREE(sbclone);
        return NULL;
    }

    memcpy((void*)sbclone->str, sb->str, sb->length);
    sbclone->capacity = sb->capacity;
    sbclone->length = sb->length;

    return sbclone;
}

/**
 * Clone only the @c String part of @c StringBuffer.
 *
 * @param sb @c StringBuffer object.
 *
 * @return Duplicated @c String on success, @c NULL otherwise.
 * This does not check for @c NULL returned by strdup.
 * */
String anv_strbuf_clone_str(AnvStringBuffer* sb) {
    RETURN_VALUE_IF_FAIL(sb, NULL, ERR_INVALID_ARGUMENTS);
    return strdup(sb->str);
}

/**
 * Set the given string to given string buffer
 *
 * @param strbuf StringBuffer object
 * @param str String to be set in the given @c StringBuffer.
 * Passing NULL is equivalent to clearing the string.
 * */
void anv_strbuf_set(AnvStringBuffer* strbuf, String str) {
    RETURN_IF_FAIL(strbuf, ERR_INVALID_ARGUMENTS);

    if(str) {
        // create duplicate
        String tmpstr = strdup(str);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }

        // set duplicate string
        if(strbuf->str) {
            FREE(strbuf->str);
        }
        strbuf->str = tmpstr;
        strbuf->length = strlen(str);
        strbuf->capacity = strbuf->length;
    } else {
        strbuf->length = 0;
        *(Char*)strbuf->str = 0;
    }
}

/**
 * Set only first n bytes of given string to the given @c StringBuffer.
 *
 * @param strbuf @c StringBuffer object.
 * @param str String to be duplicated.
 * @param n Number of characters to be duplicated from the beginning of
 * given string. Behaviour is undefined if @p n is greater than size
 * of @p str.
 * */
void anv_strbuf_setn(AnvStringBuffer* strbuf, String str, Size n) {
    RETURN_IF_FAIL(strbuf, ERR_INVALID_ARGUMENTS);

    if(str) {
        String tmpstr = strndup(str, n);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }

        // set duplicate string
        if(strbuf->str) {
            FREE(strbuf->str);
        }
        strbuf->str = tmpstr;
        strbuf->length = n;
        strbuf->capacity = n;
    } else {
        strbuf->length = 0;
        *(Char*)strbuf->str = 0;
    }
}

/**
 * Compare a @c StringBuffer and a @c NULL terminated
 * string.
 *
 * @param sb @c StringBuffer
 * @param s C style @c NULL terminated string.
 *
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 anv_strbuf_cmpstr(AnvStringBuffer* sb, String s) {
    RETURN_VALUE_IF_FAIL(sb && s, 1, ERR_INVALID_ARGUMENTS);
    return strcmp(sb->str, s);
}

/**
 * Compare first n bytes of a @c StringBuffer and a @c NULL terminated
 * string.
 *
 * @param sb @c StringBuffer
 * @param s C style @c NULL terminated string.
 * @param n Number of bytes to compare from beginning of given string.
 * Behaviour is undefined if @p n is greater than size of @p str or @p sb
 *
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 anv_strbuf_cmpstrn(AnvStringBuffer* sb, String s, Size n) {
    RETURN_VALUE_IF_FAIL(sb && s && n, 1, ERR_INVALID_ARGUMENTS);
    return strncmp(sb->str, s, n);
}

/**
 * @c Compare two @c StringBuffers.
 *
 * @param sb1 @c StringBuffer
 * @param sb2 @c StringBuffer
 *
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 anv_strbuf_cmp(AnvStringBuffer* sb1, AnvStringBuffer* sb2) {
    RETURN_VALUE_IF_FAIL(sb1 && sb2, 1, ERR_INVALID_ARGUMENTS);
    if(sb1->length != sb2->length) return 1;
    return strncmp(sb1->str, sb2->str, sb1->length);
}

/**
 * @c Compare first n bytes of two @c StringBuffers.
 *
 * @param sb1 @c StringBuffer
 * @param sb2 @c StringBuffer
 * @param n Number of bytes to compare from beginning of given string.
 * Behaviour is undefined if @p n is greater than size of @p sb1 or @p sb2.
 *
 * @return 0 if strings are equal, non zero otherwise.
 * */
Int32 anv_strbuf_cmpn(AnvStringBuffer* sb1, AnvStringBuffer* sb2, Size n) {
    RETURN_VALUE_IF_FAIL(sb1 && sb2 && n, 1, ERR_INVALID_ARGUMENTS);
    return strncmp(sb1->str, sb2->str, n);
}

/**
 * Reserve space in @c StringBuffer. This only changes
 * the capacity if @p n is greater than current capacity.
 * Reserving space is a good step if you know an upper bound
 * on the length of string.
 *
 * @param buf @c StringBuffer to reserve space for.
 * @param n Number of bytes to reserve.
 * */
void anv_strbuf_reserve(AnvStringBuffer* buf, Size n) {
    RETURN_IF_FAIL(buf && n, ERR_INVALID_ARGUMENTS);
    if(n > buf->capacity) {
        String tmpstr = realloc((void*)buf->str, n + 1);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }
        buf->str = tmpstr;
        buf->capacity = n;
    }
}

/**
 * Clear contents of given string in O(n) time.
 * This is slower but safer than it's faster alternative.
 *
 * @param buf
 * */
void anv_strbuf_clear(AnvStringBuffer* buf) {
    RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);
    *(char*)buf->str = 0;
    buf->length = 0;
}

/**
 * Clear contents of given string in constant time.
 * This is way faster but not safe to data leaks
 * than it's slower alternative.
 *
 * @param buf
 * */
void anv_strbuf_clear_fast(AnvStringBuffer* buf) {
    RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);
    *(char*)buf->str = 0;
    buf->length = 0;
}

/**
 * Append a string to this @c StringBuffer.
 * Nothing is appended if @p s is @c NULL.
 *
 * @param buf @c StringBuffer to append given string to.
 * @param s @c NULL terminated string
 * */
void anv_strbuf_append_str(AnvStringBuffer* buf, String s) {
    if(!s) return;
    RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);

    Size ssz = strlen(s);
    Size newlen = buf->length + ssz;

    // allocate new space if needed
    if(newlen > buf->capacity) {
        Size newcap = buf->capacity;
        while(newlen >= newcap) newcap *= 2;
        String tmpstr = realloc((void*)buf->str, newcap + 1);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }
        buf->str = tmpstr;
        buf->capacity = newcap;
    }

    memcpy((void*)(buf->str + buf->length), s, ssz);
    buf->length = newlen;
}

/**
 * Append first @p n bytes of given string @p s to given
 * @c StringBuffer @p buf. No action is performed if
 * either of @p s or @p n are zero. Behaviour is undefined if
 * n is greater than size of given string @p s.
 *
 * @param buf @c StringBuffer to be appended to.
 * @param s String to be appended.
 * @param n Nunber of bytes to append to @c StringBuffer. Must be
 * less than or equal to size of given string.
 * */
void anv_strbuf_append_strn(AnvStringBuffer* buf, String s, Size n) {
    if(!s || !n) return;
    RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);

    Size ssz = strlen(s);
    Size newlen = buf->length + ssz;

    // allocate new space if needed
    if(newlen >= buf->capacity) {
        Size newcap = buf->capacity;
        while(newlen >= newcap) newcap *= 2;
        String tmpstr = realloc((void*)buf->str, newcap + 1);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }
        buf->str = tmpstr;
        buf->capacity = newcap;
    }

    memcpy((void*)(buf->str + buf->length), s, ssz);
    buf->length = newlen;

}

/**
 * Append a character to end of given @c StringBuffer.
 *
 * @param buf @c StringBuffer
 * @param c Character to be appended.
 * */
void anv_strbuf_append_char(AnvStringBuffer* buf, Char c) {
    RETURN_IF_FAIL(buf, ERR_INVALID_ARGUMENTS);

    Size newlen = buf->length++;

    // allocate new space if needed
    if(newlen >= buf->capacity) {
        Size newcap = buf->capacity;
        while(newlen >= newcap) newcap *= 2;
        String tmpstr = realloc((void*)buf->str, newcap + 1);
        if(!tmpstr) {
            ERR(__FUNCTION__, ERR_OUT_OF_MEMORY);
            return;
        }
        buf->str = tmpstr;
        buf->capacity = newcap;
    }

    Char* str = (Char*)buf->str;
    str[buf->length] = c;
    str[newlen] = 0;
    buf->length = newlen;
}
