# [Anvie/Containers/BitVector](../BitVector.h)

## Purpose & Overview
`BitVector` is container to store multiple boolean values in a vector like container. The other alternative is to store `Bool` values in a `Vector`, but the main drawback of this is, you store each `Bool` in an 8-bit value, instead of using a single bit for this. `BitVector` solves this problem by storing 8 boolean values in a single `Byte`.

## Interface Builders
There  are no interface builders defined for this container because it's not a generic container. You can only store boolean values here.

## Callbacks? Copy Constructors? Destructors?
Yep, you don't require those as well, since boolean values don't require copy constructor and destructor. In fact, for interacting with non-generic containers, you'll almost never require to create any type of callback functions.

## Full List of API Functions

- `bitvec_length(bv)`: Returns the length of the bit vector.
- `bitvec_capacity(bv)`: Returns the capacity of the bit vector.
- `bitvec_data(bv)`: Returns the data of the bit vector.
- `bitvec_get_length_in_bytes(bv)`: Returns the length in bytes of the bit vector.
- `bitvec_get_capacity_in_bytes(bv)`: Returns the capacity in bytes of the bit vector.
- `bitvec_create()`: Creates a new BitVector.
- `bitvec_destroy(BitVector* bv)`: Destroys a BitVector.
- `bitvec_clone(BitVector* bv)`: Clones a BitVector.
- `bitvec_set_equal(BitVector* dstbv, BitVector* srcbv)`: Sets one BitVector equal to another.
- `bitvec_reserve(BitVector* bv, Size numbools)`: Reserves space for the BitVector.
- `bitvec_resize(BitVector* bv, Size numbools)`: Resizes the BitVector.
- `bitvec_push(BitVector* bv, Bool val)`: Pushes a value to the BitVector.
- `bitvec_pop(BitVector* bv)`: Pops a value from the BitVector.
- `bitvec_set(BitVector* bv, Size index)`: Sets a bit at a given index.
- `bitvec_clear(BitVector* bv, Size index)`: Clears a bit at a given index.
- `bitvec_set_all(BitVector* bv)`: Sets all bits in the BitVector.
- `bitvec_clear_all(BitVector* bv)`: Clears all bits in the BitVector.
- `bitvec_set_range(BitVector* bv, Size range_begin, Size range_size)`: Sets a range of bits in the BitVector.
- `bitvec_clear_range(BitVector* bv, Size range_begin, Size range_size)`: Clears a range of bits in the BitVector.
- `bitvec_peek(BitVector* bv, Size index)`: Peeks at a bit in the BitVector.
- `bitvec_xor(BitVector* bv1, BitVector* bv2)`: Performs XOR operation between two BitVectors.
- `bitvec_and(BitVector* bv1, BitVector* bv2)`: Performs AND operation between two BitVectors.
- `bitvec_or(BitVector* bv1, BitVector* bv2)`: Performs OR operation between two BitVectors.
- `bitvec_xnor(BitVector* bv1, BitVector* bv2)`: Performs XNOR operation between two BitVectors.
- `bitvec_nand(BitVector* bv1, BitVector* bv2)`: Performs NAND operation between two BitVectors.
- `bitvec_nor(BitVector* bv1, BitVector* bv2)`: Performs NOR operation between two BitVectors.
- `bitvec_shl(BitVector* bv, Size index)`: Shifts left the BitVector by a specified index.
- `bitvec_shr(BitVector* bv, Size index)`: Shifts right the BitVector by a specified index.
- `bitvec_not(BitVector* bv)`: Performs a NOT operation on the BitVector.
- `bitvec_cmpeq(BitVector* bv1, BitVector* bv2)`: Compares two BitVectors for equality.

<p align="center" style="font-size: small; line-height: 1.2;">
    Author: Siddharth Mishra<br>
    Writing Date: 31st December, 2023<br>
    Last Modified: 1st January, 2024<br>
    License: Apache 2.0 License<br> <br>
    Copyright (c) 2023 AnvieLabs, Siddharth Mishra
</p>
