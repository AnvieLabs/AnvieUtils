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
} AnvStringBuffer;

#define anv_strbuf_at(sb, idx) if(sb && sb->str) sb->str[idx]

AnvStringBuffer* anv_strbuf_create(String str);
void anv_strbuf_destroy(AnvStringBuffer* strbuf);

AnvStringBuffer* anv_strbuf_clone_buf(AnvStringBuffer* sb);
String anv_strbuf_clone_str(AnvStringBuffer* sb);

void anv_strbuf_set(AnvStringBuffer* strbuf, String str);
void anv_strbuf_setn(AnvStringBuffer* sb, String str, Size n);
Int32 anv_strbuf_cmpstr(AnvStringBuffer* sb, String s);
Int32 anv_strbuf_cmpstrn(AnvStringBuffer* sb, String s, Size n);
Int32 anv_strbuf_cmp(AnvStringBuffer* sb1, AnvStringBuffer* sb2);
Int32 anv_strbuf_cmpn(AnvStringBuffer* sb1, AnvStringBuffer* sb2, Size n);

void anv_strbuf_reserve(AnvStringBuffer* buf, Size n);
void anv_strbuf_clear(AnvStringBuffer* buf);
void anv_strbuf_clear_fast(AnvStringBuffer* buf);
void anv_strbuf_append_str(AnvStringBuffer* buf, String s);
void anv_strbuf_append_strn(AnvStringBuffer* buf, String s, Size n);
void anv_strbuf_append_char(AnvStringBuffer* buf, Char c);
