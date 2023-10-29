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

#include <Anvie/Types.h>

/**
 * Compute shannon entropy for given data.
 *
 * @param data Pointer to data to compute entropy for.
 * @param sz Size of data in bytes.
 * */
Float32 compute_shannon_entropy(void* data, Size sz);
