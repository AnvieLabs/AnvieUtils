/**
 * @file Simd.h
 * @date Fri, 8th December, 2023
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
 * @brief One single header to include all SIMD headers.
 * This utility module provides access to wrappers around various SIMD intrinsic
 * functions. Every functions uses if-elif-else cases to decide best AVX extension
 * to use inside the wrapper implementation.
 *
 * Outside this module, user only needs to use `MVec` and `MMask` variable types
 * as a generic vector register, meaning irrespective of the latest AVX extension
 * version, always use the above varible types in order to reduce confusion in user code.
 * */

#ifndef ANVIE_SIMD_SIMD_H
#define ANVIE_SIMD_SIMD_H

#include <Anvie/Simd/Types.h>
#include <Anvie/Simd/ComparisionOps.h>
#include <Anvie/Simd/LoadStoreOps.h>

#endif // ANVIE_SIMD_SIMD_H
