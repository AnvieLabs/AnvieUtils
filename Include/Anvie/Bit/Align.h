/**
 * @file Align.h
 * @date Fri, 29th December, 2023
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
 * @brief Some helper macros defined for easy alignment of given
 * number to certain boundaries.
 * */

#ifndef ANVIE_UTILS_BIT_ALIGN_H
#define ANVIE_UTILS_BIT_ALIGN_H



/**
 * @brief Aligns a value to the next highest multiple of a given type's size.
 *
 * @param t The type to align to.
 * @param v The value to align.
 *
 * @return The aligned value.
 *
 * @note This macro aligns up to the nearest multiple of the type's size, even if the value is already aligned.
 * @see ALIGN_LO
 */
#define ALIGN_HI(t, v) (((v) + ((sizeof(t) << 3) - 1)) & ~((sizeof(t) << 3) - 1))

/**
 * @brief Aligns a value to the nearest lower multiple of a given type's size.
 *
 * @param t The type to align to.
 * @param v The value to align.
 *
 * @return The aligned value.
 *
 * @note This macro aligns down to the nearest multiple of the type's size, even if the value is already aligned.
 * @see ALIGN_HI
 */
#define ALIGN_LO(t, v) (v & ~( (sizeof(t) << 3) - 1) )

// Convenience macros for common alignment sizes:
#define ALIGN8_HI(v)  ALIGN_HI(Uint8, v)
#define ALIGN8_LO(v)  ALIGN_LO(Uint8, v)

#define ALIGN16_HI(v) ALIGN_HI(Uint16, v)
#define ALIGN16_LO(v) ALIGN_LO(Uint16, v)

#define ALIGN32_HI(v) ALIGN_HI(Uint32, v)
#define ALIGN32_LO(v) ALIGN_LO(Uint32, v)

#define ALIGN64_HI(v) ALIGN_HI(Uint64, v)
#define ALIGN64_LO(v) ALIGN_LO(Uint64, v)

#endif // ANVIE_UTILS_BIT_ALIGN_H
