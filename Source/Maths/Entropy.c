/**
 * @file Entropy.h
 * @date Sat, 29th September, 2023
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
 * @brief Different functions for computing entropy of given data.
 * */

#include <Anvie/Maths/Entropy.h>
#include <Anvie/HelperDefines.h>

#include <math.h>

/**
 * Compute shannon entropy for given data.
 *
 * @param data Pointer to data to compute entropy for.
 * @param sz Size of data in bytes.
 * */
Float32 compute_shannon_entropy(void* data, Size sz) {
    RETURN_VALUE_IF_FAIL(data, 0.f, ERR_INVALID_ARGUMENTS);

    if(sz < 2) return 0;

    // shannon entropy and frequency table.
    Float32 se = 0.f;
    Size ftab[0x100] = {0};

    // align the array size to be divisible by 4
    Uint8* arr = (Uint8*)data;
    while(sz % 8){
        ftab[*arr++]++;
        sz--;
    }

    // get frequency count for each byte
    for(Size s = 0; s < sz;) {
        ftab[arr[s++]]++;
        ftab[arr[s++]]++;
        ftab[arr[s++]]++;
        ftab[arr[s++]]++;

        ftab[arr[s++]]++;
        ftab[arr[s++]]++;
        ftab[arr[s++]]++;
        ftab[arr[s++]]++;
    }

    // compute overall entropy
    for(Size s = 0; s < 255; s++) {
        if(ftab[s]) {
            Float32 p = ftab[s]/(Float32)sz;
            se -= p*logf(p);
        }
    }

    return se * logf(2) / logf(sz);
}
