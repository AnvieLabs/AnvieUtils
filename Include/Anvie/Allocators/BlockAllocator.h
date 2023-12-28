/**
 * @file BlockAllocator.h
 * @date Wed, 20th December, 2023
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
 * @brief A block allocator is like a vector of fixed sized blocks.
 * This vector's size can be increased and decreased but the block size
 * remains fixed.
 *
 * Block allocators can be used in cases where there will be multiple calls
 * to @c malloc() or @c calloc() for fixed size. If a new allocation is performed,
 * and if there's space to keep that block in the block allocator, then pointer
 * to that memory will be returned, but if there's no space for allocating
 * a new block from the block allocator, then it'll resize itself to store
 * more blocks.
 *
 * Block allocator is also useful when the allocation size is too small,
 * say less than 32 bytes, because most heap allocators keep a minimum heap
 * size to 32 bytes. So, even if you want to allocate just one byte, you'll
 * end up allocating 32 bytes.
 *
 * The @c BlockAllocator optimizes memory allocation by dividing memory
 * into fixed-size blocks, suitable for scenarios where objects of the
 * same size are frequently allocated and deallocated. It minimizes fragmentation
 * and improves memory usage efficiency compared to general-purpose allocators.
 *
 * Even though blocks are allocated in a contiguous memory, don't assume that
 * next block after the block you currently have will point to another valid block.
 * This is completely implementation dependent, and assuming so will result in
 * unwanted software and security bugs.
 *
 * PROS:
 * - Can be cache friendly if used wisely.
 * - Fast allocation, since technically, memory is already allocated on average.
 * - No segmentation issues, since allocations are performed in fixed sizes.
 * CONS:
 * - Not asynchronous.
 * - Cannot allocate an array of contiguous blocks. This is because, allocating
 *   an array can cause segmentation and not be in constant time.
 * */

#ifndef ANVIE_UTILS_ALLOCATORS_BLOCK_ALLLOCATOR_H
#define ANVIE_UTILS_ALLOCATORS_BLOCK_ALLLOCATOR_H

#include <Anvie/Types.h>
#include <Anvie/Containers/BitVector.h>

/**
 * @c BlockAllocators allocate memory blocks. All
 * memory blocks allocated from a @c BlockAllocator will
 * be of same size. A @c MemBlock is analogous to a @c void*
 * that you get when using @c calloc, @c malloc or @c realloc.
 *
 * Don't confuse however between a @c MemBlock and a @c MemBlock*.
 * A @c MemBlock is the pointer to memory itself, and a @c MemBlock*
 * is a pointer to the pointer to that memory.
 * */
typedef Uint8* MemBlock;

/**
 * @c INVALID_MEM_BLOCK is to @c MemBlock what @c NULL is to @c void*
 * */
#define INVALID_MEM_BLOCK ((MemBlock)0)

/**
 * A @c MemBlockArray, is an array of @c MemBlocks.
 * This will contain multiple number of @c MemBlocks, in
 * a contiguous memory region.
 *
 * This means size of all @c MemBlockArrays are integral
 * multiple of the size of @c MemBlocks they store.
 * */
typedef Uint8* MemBlockArray;

#ifndef LIN_BLOCK_ALLOCATOR_RESIZE_FACTOR
/**
 * The @c BlockAllocator will maintain a matrix of @c MemBlocks.
 * There are two possible ways in which we can increase the memory
 * Either linear or quadratic. This value governs the intial size and
 * growth size of a linear allocator. eg : A value of 5 means (1 << 5) or
 * 32 step size. A value of 6 would mean 64 step size.
 *
 * If user wants to change this value, they need to define this value
 * before including the allocator header. It's encouranged for user
 * to this this value based on how much allocations they require at max.
 * This value will also govern the cache friendliness of linear allocator.
 * A very large value and a very low value might result in frequent
 * cache misses, however a wisely selected value might result in very
 * low cache miss when accessing blocks allocated from linear allocator.
 * */
#define LIN_BLOCK_ALLOCATOR_RESIZE_FACTOR 5
#endif

/**
 * A @c LinBlockAllocator has a linear growth policy, meaning everytime,
 * the @c LinrBlockAllocator is at it's capacity and cannot allocate more
 * blocks, it'll increase it's size by a pre-defined step value.
 * This is for use cases when there will be allocations, but the number
 * of allocations grows in an almost linear fashion.
 * */
typedef struct LinBlockAllocator {
    Size           allocation_count; /**< Total number of allocated blocks. */
    Size           total_capacity; /**< Total number of blocks that can and has been allocated. */
    Size           block_size;  /**< Size of each block. */
    Size           last_freed_block; /**< Index of last free'd block. */
    BitVector*     occupancy; /**< To store whether or not a block is allocated. */
    MemBlockArray* block_matrix; /**< A 2D Matrix of memory blocks. */
} LinBlockAllocator;

LinBlockAllocator* lballoc_create(Size block_size);
void               lballoc_destroy(LinBlockAllocator* lba);
void               lballoc_reserve(LinBlockAllocator* lba, Size num_blocks);
MemBlock           lballoc_allocate(LinBlockAllocator* lba);
void               lballoc_free(LinBlockAllocator* lba, MemBlock blk);
Float32            lballoc_get_load(LinBlockAllocator* lba);

/**
 * The @c ExpBlockAllocator has an exponential growth policy, much like
 * any other continuous container out there. The size will increase to
 * twice it's current value, everytime this block allocator feels like
 * resizing.
 * */
typedef struct ExpBlockAllocator {
    Size          allocation_count; /**< Total number of allocated blocks. */
    Size          total_capacity; /**< Total number of blocks that can and has been allocated. */
    Size          block_size;  /**< Size of each block. */
    Size          last_freed_block; /**< Index of last free'd block. */
    BitVector*    occupancy; /**< To store whether or not a bit is allocated. */
    MemBlockArray block_array;  /**< A linear resizable array of Memory */
} ExpBlockAllocator;

/* ExpBlockAllocator* eballoc_create(Size block_size); */
/* void               eballoc_destroy(ExpBlockAllocator* lba); */
/* MemBlock           eballoc_allocate(ExpBlockAllocator* lba); */
/* void               eballoc_free(ExpBlockAllocator* lba, MemBlock blk); */
/* Float32            eballoc_get_load(ExpBlockAllocator* lba); */

#endif // ANVIE_UTILS_ALLOCATORS_BLOCK_ALLLOCATOR_H
