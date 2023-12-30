/**
 * @file Operators.h
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
 * @brief Defines macros for different bitwise operators.
 * These macros are defined this way to be used as arguments
 * to some other macros, when doing some macro magic.
 * */

#ifndef ANVIE_UTILS_BIT_OPERATORS_H
#define ANVIE_UTILS_BIT_OPERATORS_H

/**
 * Performs a bitwise AND operation between two values.
 *
 * @param a First value
 * @param b Second value
 * @return Result of the bitwise AND operation
 */
#define AND(a, b) ((a) & (b))

/**
 * Performs a bitwise OR operation between two values.
 *
 * @param a First value
 * @param b Second value
 * @return Result of the bitwise OR operation
 */
#define OR(a, b) ((a) | (b))

/**
 * Performs a bitwise XOR operation between two values.
 *
 * @param a First value
 * @param b Second value
 * @return Result of the bitwise XOR operation
 */
#define XOR(a, b) ((a) ^ (b))

/**
 * Performs a bitwise NOT operation on a value.
 *
 * @param a Value to be negated
 * @return Result of the bitwise NOT operation
 */
#define NOT(a) (~(a))

/**
 * Performs a bitwise NAND operation between two values.
 *
 * @param a First value
 * @param b Second value
 * @return Result of the bitwise NAND operation
 */
#define NAND(a, b) NOT(AND(a, b))

/**
 * Performs a bitwise NOR operation between two values.
 *
 * @param a First value
 * @param b Second value
 * @return Result of the bitwise NOR operation
 */
#define NOR(a, b) NOT(OR(a, b))

/**
 * Performs a bitwise XNOR operation between two values.
 *
 * @param a First value
 * @param b Second value
 * @return Result of the bitwise XNOR operation
 */
#define XNOR(a, b) NOT(XOR(a, b))

/**
 * Performs a bitwise shift right operation on a value.
 *
 * @param a Value to be shifted
 * @param n Number of positions to shift by
 * @return Result of the bitwise shift right operation
 */
#define SHR(a, n) (TO_UINT64(a) >> (n))

/**
 * Performs a bitwise shift left operation on a value.
 *
 * @param a Value to be shifted
 * @param n Number of positions to shift by
 * @return Result of the bitwise shift left operation
 */
#define SHL(a, n) (TO_UINT64(a) << (n))

#endif // ANVIE_UTILS_BIT_OPERATORS_H
