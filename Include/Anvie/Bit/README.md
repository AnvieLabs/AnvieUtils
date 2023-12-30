## Anvie Utility for Bitwise Operations & Bit Manipulation

This utility defines some macros for direct and easy bit manipulation. To use all these components at once, directly include [`Anvie/Bit/Bit.h`](Bit.h) in your code. All these macros are extensively used throughout the AnvieUtils source code. Most easy way to find their actual usage is in [`Tests`](../../../Source/Tests).

## Tutorials

### [`Anvie/Bit/Align.h`](Align.h)

Defines some macros to align given numbers to upper and lower boundaries of powers of two (8, 16, 32 and 64).

``` c
/* number to be aligned */
Uint32 v = 51;

Uint32 a8  = ALIGN8(v);  /* 56 */
Uint32 a16 = ALIGN16(v); /* 64 */
Uint32 a32 = ALIGN32(v); /* 64 */
Uint32 a64 = ALIGN64(v); /* 64 */
```

### [`Anvie/Bit/Mask.h`](Mask.h)

This one provides lots and lots of macros. At the time of writing, these are used extensively in implementation of [`Anvie/Containers/BitVector`](../../../Source/Containers/BitVector.c) and it's tests
in [`Tests/Containers/BitVector`](../../../Source/Tests/Containers/BitVector). All these can be used as real use cases of these macros.  

There are macros to create mask :  

``` c
/* MASK<N>_LO */
Uint8 m8  = MASK8_LO(3);        /* 0x07 */
Uint8 m16 = MASK16_LO(11);      /* 0x07ff */
Uint8 m16 = MASK_LO(Uint16, 5); /* 0x001f */
```  

Similarly, there exists macros for creating masks for flagging upper bits and a range of bits named `MASK8_HI`, `MASK32_HI`, etc... and `MASK8_RANGE`, `MASK16_RANGE` etc...   

There are more macros for :
- Selecting a range of bits (without shifting them, meaning you get bits as they are, just masked with selection with mask) : `SEL<N>_RANGE`, `SEL<N>_ LO`, `SEL<N>_HI`.
- Getting lower, or higher bits or a range of bits. These differ from selection macros in way that they shift and adjust the select bits such that the selected range always start from bit index 0 : `GET<N>_RANGE`, `GET<N>_LO`, `GET<N>_HI`.
- Setting a range of bits in a given variable and a specified range : `SET<N>_RANGE`, `SET<N>_LO`, `SET<N>_HI`.
- Clearing a range of bits in a given variable and a specified range : `CLR<N>_RANGE`, `CLR<N>_LO`, `CLR<N>_HI`.  

Many more can be added in future. It's possible that this documentation may not be updated, because these macros are not much significant features. Better to take look inside the header that defines them : [`Anvie/Bit/Mask.h`](Mask.h)

### [`Anvie/Bit/Arithmetic.h`](Arithmetic.h)

Computing modulo, or quotient of a division operation is slow (that's what I've read and seen in talks). Devs who focus on optimizing code, often state this fact. Even though your compiler will often take care of this when optimizing your code with compiler `-O3` or some other flags, This header defines some macros to do almost the same the the compiler will do. You can use these macros as a substitute to division and modulo in very selected situations.

``` c
Uint32 m = MUL32(7);      /* 224 */ 
Uint32 d = DIV16(31);     /* 1 */
Uint32 r = MOD128(129);   /* 1 */

Uint32 v1 = NEXT_POW2(5);  /* 8 */
Uint32 v2 = NEXT_POW2(64); /* 64 */
```
These macros are also used throughout the code in AnvieUtils.

### [`Anvie/Bit/Operators.h`](Operators.h)

This header defines macros for basic logical operators like `and`, `or`, `xor`, etc... and some more operators. I'm assuming the reader will take a look inside this header, so I won't be writing about these operators, because they are quite trivial.

---

<p align="center" style="font-size: small; line-height: 1.2;">
    Author: Siddharth Mishra<br>
    Writing Date: December 30th, 2023<br>
    Last Modified: December 30th, 2023<br>
    License: Apache 2.0 License<br> <br>
    Copyright (c) 2023 AnvieLabs, Siddharth Mishra
</p>
