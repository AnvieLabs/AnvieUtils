/**
 * @file StringBuffer.h
 * @date Mon, 6th October, 2023
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
 * @brief A @c StringBuffer is used to efficiently store strings
 * in a memory safe manner and quite efficient manner based on use case.
 * */

#include <Anvie/Types.h>
#include <Anvie/HelperDefines.h>

/**
 * A @c StringBuffer is used to store strings in a
 * vector of char like container. @c StringBuffer
 * takes care of all memory allocation and destruction
 * automatically.
 * One does not need to constantly keep track of length
 * of @c StringBuffer as it has a @c length member that
 * matches the @c NULL terminated string @c str.
 *
 * @c StringBuffer is used when the string is supposed
 * to change multiple times in it's size and capacity.
 * This is much like the use of vectors. If the length
 * of your data is static then better use vanilla @c String
 * but if length is variable, use a vector like container,
 * like this one.
 * */
typedef struct anvie_string_buffer_t {
    String str;                 /**< @c NULL terminated string. */
    Size   length;              /**< current length of @c str */
    Size   capacity;            /**< total memory allocated for string (excluding the NULL character). */
} StringBuffer;

#define strbuf_at(sb, idx) if(sb && sb->str) sb->str[idx]

StringBuffer* strbuf_create(String str);
void strbuf_destroy(StringBuffer* strbuf);

StringBuffer* strbuf_clone_buf(StringBuffer* sb);
String strbuf_clone_str(StringBuffer* sb);

void strbuf_set(StringBuffer* strbuf, String str);
void strbuf_setn(StringBuffer* sb, String str, Size n);
Int32 strbuf_cmpstr(StringBuffer* sb, String s);
Int32 strbuf_cmpstrn(StringBuffer* sb, String s, Size n);
Int32 strbuf_cmp(StringBuffer* sb1, StringBuffer* sb2);
Int32 strbuf_cmpn(StringBuffer* sb1, StringBuffer* sb2, Size n);

void strbuf_reserve(StringBuffer* buf, Size n);
void strbuf_clear(StringBuffer* buf);
void strbuf_clear_fast(StringBuffer* buf);
void strbuf_append_str(StringBuffer* buf, String s);
void strbuf_append_strn(StringBuffer* buf, String s, Size n);
void strbuf_append_char(StringBuffer* buf, Char c);
