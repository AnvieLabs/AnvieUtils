/**
 * @file helpers.h
 * @date Tue, 26th December, 2023
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
 * @brief Helper functions for easy testing of bitvector.
 * */

#include <Anvie/Containers/BitVector.h>
#include <Anvie/Error.h>
#include <Anvie/BitManipulation.h>

/**
 * Compare whether memory of bitvec is completely filled with
 * given value.
 * @param data
 * @param sz
 * @param v Value to compare with
 * */
static FORCE_INLINE Bool is_memory_filled_with_byte(Uint8* data, Size sz, Uint8 v) {
    ERR_RETURN_VALUE_IF_FAIL(data, False, ERR_INVALID_ARGUMENTS);

    for(Size s = 0; s < sz; s++) {
        if(data[s] != v) {
            return False;
        }
    }

    return True;
}
