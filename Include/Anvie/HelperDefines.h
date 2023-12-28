/**
 * @file HelperDefines.h
 * @date Wed, 5th July, 2023
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
 * @brief Defines some helper macros for making tasks
 * like debug printing, etc... easy.
 * */

#ifndef AVHELPERDEFINE_H_
#define AVHELPERDEFINE_H_

#include <stdlib.h>
#include <Anvie/TerminalColors.h>

#ifndef MODULE_NAME
#define MODULE_NAME "Utils"
#endif//MODULE_NAME

// basic min/max
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define MIN3(a, b, c) MIN(a, MIN(b, c))
#define MAX3(a, b, c) MAX(a, MAX(b, c))

#define LINE80() println("================================================================================")
#define newline() putchar(0xa)

// allocation heloers
#define ALLOCATE(type, count) (type*)calloc(count, sizeof(type))
#define NEW(type) ALLOCATE(type, 1)
#define FREE(x) free((void*)x)

// get array size
#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
// ceate packed enums, unions, structs etc...
#define PACKED __attribute__((__packed__))

// print helpers
#define print(...) printf(__VA_ARGS__)
#define println(...) printf(__VA_ARGS__); putchar('\n')

// use when a variable is left unused intentionally and you
// don't want compiler to complain about it
#define UNUSED(x) (void)(x)

// use when a function is to be forced to be inlined
#define FORCE_INLINE inline __attribute__((always_inline))

#endif // AVHELPERDEFINE_H_
