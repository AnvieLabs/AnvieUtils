/**
 * @file String.h
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
 * @brief A @c String is used to efficiently store strings
 * in a memory safe manner and quite efficient manner based on use case.
 * */

#include <Anvie/Types.h>
#include <Anvie/HelperDefines.h>

/**
 * @c String is a container to store non-NULL
 * terminated character arrays in a safe manner.
 *
 * This object will keep track of current length
 * of string to avoid call to @c strlen multiple times
 * in program.
 *
 * Total number of bytes of data a @c String object
 * can store is dependent on the capacity of that @c String.
 * The capacity of @c String grows dynamically as
 * more and more data is added or pushed to it.
 *
 * The implementation does not memset contents to 0 when length is
 * reduced in order to avoid data leaks. This is only done when
 * the string object is destroyed.
 *
 * Doing a memset everytime length is changed creates a far jump,
 * and may reduce performance. Besides, the application must
 * guarantee that it won't try to access contents of string beyond
 * it's length.
 * */
typedef struct String {
    char*   data;     /**< Non @c NULL terminated character array */
    Size    length;   /**< current length of @c data */
    Size    capacity; /**< total memory allocated for string @c data */
} String;

#define str_at(sb, idx) if(sb && sb->str) sb->str[idx]

String* str_create(ZString str);
void    str_destroy(String* strbuf);

String* str_clone(String* sb);
ZString str_clone_to_zstr(String* sb);

void    str_set_zstr(String* strbuf, ZString zstr);
void    str_setn_zstr(String* sb, ZString zstr, Size n);

Int32   str_cmp_zstr(String* sb, ZString zs);
Int32   str_cmpn_zstr(String* sb, ZString zs, Size n);
Int32   str_cmp(String* sb1, String* sb2);
Int32   str_cmpn(String* sb1, String* sb2, Size n);

void    str_reserve(String* buf, Size n);
void    str_clear(String* buf);
void    str_clear_fast(String* buf);

void    str_push_char(String* buf, Char c);
Char    str_pop_char(String* buf);
void    str_push_zstr(String* buf, ZString s);
void    str_pushn_zstr(String* buf, ZString s, Size n);
ZString str_popn_zstr(String* buf, Size n);
