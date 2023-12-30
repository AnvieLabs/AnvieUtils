/**
 * @file drop_in_replacements.h
 * @date Mon, 26th December, 2023
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
 * @brief Contains definititon of functions that are drop-in-replacement
 * for some selected API functions in BitVector that cannot be used in
 * individual unit tests, because they'll violate the isolation policy of
 * unit tests.
 * */

#ifndef ANVIE_UTILS_TESTS_CONTAINERS_BITVECTOR_DROP_IN_REPLACEMENTS_H
#define ANVIE_UTILS_TESTS_CONTAINERS_BITVECTOR_DROP_IN_REPLACEMENTS_H

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Bit/Bit.h>
#include <Anvie/Error.h>

/**
 * A drop-in replacement for resize method in bitvector.
 * Not a complete implementation of bitvector's resize
 * but should be enough to resize the memory.
 * One could obviously raise the question, where's the test for this? XD
 *
 * This function is not intended to act in the same way bitvec_resize
 * works!
 *
 * @param bv
 * @param len Number of bits
 * */
static FORCE_INLINE void resizebv(BitVector* bv, Size len) {
    if(len + 8 - MOD8(len) > bv->capacity) {
        len += 8 - MOD8(len);
        Uint8* tmp = realloc(bv->data, DIV8(len));
        ERR_RETURN_IF_FAIL(tmp, ERR_OUT_OF_MEMORY);
        bv->data = tmp;
        bv->length = len;
        bv->capacity = len;
    } else {
        bv->length = len;
    }
}

#endif // ANVIE_UTILS_TESTS_CONTAINERS_BITVECTOR_DROP_IN_REPLACEMENTS_H
