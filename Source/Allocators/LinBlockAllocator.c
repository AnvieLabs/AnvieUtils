/**
 * @file LinBlockAllocator.c
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
 * @brief Implementation of @c LinBlockAllocator in Allocators/BlockAllocator.h
 * */

#include <Anvie/Allocators/BlockAllocator.h>
#include <Anvie/HelperDefines.h>
#include <Anvie/Error.h>
#include <string.h>

/**
 * The implementation works by maintaining a matrix of memory blocks of fixed size.
 * Each column contains an array of memory blocks. Columns are allocated in fixed size
 * but dynamically. An array stores pointer to each column. The growth of this array
 * is linear and is governed by the size defined below. But, in all this column size
 * remains same, meaning number of rows are also same. But, row size increases with use,
 * hence, so does number of columns.
 *
 * In easy words, the implementation of matrix here is column major.
 *
 * Each time the linear allocator runs out of memory, It will allocate a whole column.
 * This makes sure that the growth of linear allocator is linear as well as when
 * used wisely can be cache friendly. The user needs to make sure that a single
 * array of memory blocks should be able to fit inside the cache level they want
 * the arrays to fit into.
 *
 * This might also be useful for data oriented programs.
 *
 * A diagram to show how the memory block matrix looks
 *
 *        | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | a | b | c | d | e | f |
 *        |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *        |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *        |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *        |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *        |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 *        .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .
 *        .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .
 *        .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .   .
 *
 *  In the above diagram, each column represents a memory block array. The indices
 *  are just to display the initial layout (16 columns).
 * */

/* this value should always be a power of 2*/
/* influences growth step size of array that stores pointers to each row (or page)*/
#define COL_STEP 16

/* power of two by which we grow the linear allocator's memory */
#define FACTOR LIN_BLOCK_ALLOCATOR_RESIZE_FACTOR

/* analogous to number of columns in matrix */
#define NUM_ROWS (1 << FACTOR)

/* finds next multiple of COL_STEP from given rowcnt */
#define NEXT_INCREMENTED_COL_COUNT(rowcnt) (((rowcnt) & (~(COL_STEP - 1))) + COL_STEP)

/* convert given index to column index */
#define COL(i) ((i) >> FACTOR)          /* quotient when divided by (1 << FACTOR) or two raised to power of FACTOR */
#define ROW(i) ((i) & ~(NUM_ROWS - 1))  /* remainder when divided by (1 << FACTOR) */

/**
 * Create a new linear block allocator to allocate
 * fixed memory blocks of given size.
 *
 * @param block_size Size of each block to be allocated.
 * @return LinBlockAllocator* A valid object on success.
 * @return NULL on failure.
 * */
LinBlockAllocator* lballoc_create(Size block_size) {
    ERR_RETURN_VALUE_IF_FAIL(block_size, NULL, ERR_INVALID_ARGUMENTS);

    /* create new lba */
    LinBlockAllocator* lba = NEW(LinBlockAllocator);
    ERR_RETURN_VALUE_IF_FAIL(lba, NULL, ERR_INVALID_ARGUMENTS);

    /* bitvector to store allocation status */
    lba->occupancy = bitvec_create();
    ERR_RETURN_VALUE_IF_FAIL(lba->occupancy, NULL, ERR_INVALID_OBJECT);

    /* Create a matrix of memory blocks. Not all rows are allocated at once. */
    lba->block_matrix = ALLOCATE(MemBlockArray, COL_STEP);
    if(!lba->block_matrix) {
        goto HELL;
    }

    /* allocate first array of memory block */
    MemBlockArray mmblk = ALLOCATE(Uint8, NUM_ROWS * block_size);
    if(!mmblk) {
        goto HELL;
    }

    /* set first row of memory block matrix. */
    *lba->block_matrix    = mmblk;
    lba->block_size       = block_size;
    lba->total_capacity   = NUM_ROWS;

    return lba;

HELL:
    lballoc_destroy(lba);
    ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
    return NULL;
}

/**
 * Destroy given linear block allocator.
 *
 * @param lba
 * */
inline void lballoc_destroy(LinBlockAllocator* lba) {
    ERR_RETURN_IF_FAIL(lba, ERR_INVALID_ARGUMENTS);

    if(lba->occupancy) {
        bitvec_destroy(lba->occupancy);
        lba->occupancy = NULL;
    }

    if(lba->block_matrix) {
        MemBlockArray* m = lba->block_matrix;
        while(*m) {
            FREE(*m);
            *m++ = NULL;
        }
        FREE(lba->block_matrix);
        lba->block_matrix = NULL;
    }

    FREE(lba);
}

/**
 * Reserve given number of blocks. A call this this function
 * is recommented if approximate number of allocations is known
 * beforehand.
 *
 * @param lba
 * @param num_blocks Number of blocks to reserve memory for.
 * */
void lballoc_reserve(LinBlockAllocator* lba, Size num_blocks) {
    ERR_RETURN_IF_FAIL(lba && num_blocks, ERR_INVALID_ARGUMENTS);

    /* if total memory to reserve is less than what is required at the moment then exit. */
    if(num_blocks < lba->total_capacity) {
        return;
    }

    /* get total number of current cols (mem block array) */
    Size col_cnt = COL(lba->total_capacity);

    /* if number of blocks to allocate exceeds the number of blocks that can actually fit
     * in all the preallocated cols, reallocate */
    if(num_blocks > (col_cnt * NUM_ROWS)) {
        /* number of cols for which memory block array will be allocated */
        Size new_col_cnt = COL(num_blocks);

        /* resize array containing pointer to each row in memory block matrix */
        /* number of rows actually in new matrix. This is always integral multiple of COL_STEP */
        Size new_matrix_cols = NEXT_INCREMENTED_COL_COUNT(new_col_cnt);

        /* Create a matrix of memory blocks. Not all rows are allocated at once. */
        MemBlockArray* tmp = realloc(lba->block_matrix, new_matrix_cols * sizeof(MemBlockArray*));
        ERR_RETURN_IF_FAIL(tmp, ERR_OUT_OF_MEMORY);
        memset(tmp, 0, (new_matrix_cols - new_col_cnt) * sizeof(MemBlockArray*));
        lba->block_matrix = tmp;
        if(!lba->block_matrix) {
            goto HELL;
        }

        /* allocate rows */
        while(new_col_cnt - col_cnt) {
            /* allocate first array of memory block */
            MemBlockArray mmblk = ALLOCATE(Uint8, NUM_ROWS * lba->block_size);
            if(!mmblk) {
                goto HELL;
            }
            lba->block_matrix[++col_cnt] = mmblk;
        }

        lba->total_capacity = col_cnt * NUM_ROWS;
    }

HELL:
    ERR(__FUNCTION__, ERRFMT, ERRMSG(ERR_OUT_OF_MEMORY));
}

/**
 * Allocate a new block.
 * The allocation implementation aims to provide constant
 * time allocation but the worst case may be linear time.
 *
 * @param lba
 * @return MemBlock A pointer/reference to allocated memory on success.
 * @return INVALID_MEM_BLOCK otherwise.
 * */
MemBlock lballoc_allocate(LinBlockAllocator* lba) {
    ERR_RETURN_VALUE_IF_FAIL(lba, INVALID_MEM_BLOCK, ERR_INVALID_ARGUMENTS);

#define RETURN_BLOCK_AT(i)                                          \
    bitvec_set(lba->occupancy, i);                                  \
    return (lba->block_matrix[COL(i)] + (ROW(i) * lba->block_size))

    /* if we've recently free'd a block then allocate that one first. */
    if(lba->last_freed_block != SIZE_MAX) {
        RETURN_BLOCK_AT(lba->last_freed_block);

        /* if the block just after this is already allocated then next allocation
         * needs to search the whole matrix for a free block. */
        if(bitvec_peek(lba->occupancy, ++lba->last_freed_block)) {
            lba->last_freed_block = SIZE_MAX;
        }

        /* this part of code tends to search and allocate block in a linear fashion at the very beginning of creation */
    }

    /* else linearly search through */
    Size r = 0;
    Size c = 0;
    Size col_cnt = COL(lba->total_capacity);
    while((c < col_cnt) && (r < NUM_ROWS)) {
        Size pos = c*NUM_ROWS + r;
        if(bitvec_peek(lba->occupancy, pos)) {
            RETURN_BLOCK_AT(pos); /* this part of code skips COL_STEP number of elemnets at once to search (linear, but with strides, possibly faster) */
        }
        c++;
        if(c == col_cnt) {
            c = 0;
            r++;
        }
    }

    lballoc_reserve(lba, lba->total_capacity+1);
    lba->last_freed_block = lba->total_capacity + 2;
    RETURN_BLOCK_AT(lba->total_capacity + 1);

#undef RETURN_BLOCK_AT
}

/**
 * Free provided @c MemBlock. User must set all previous
 * references to this @c MemBlock to @c INVALID_MEM_BLOCK
 * after calling this free. This is just like what you do
 * when using @c FREE.
 *
 * @param lba
 * @param blk
 * */
void lballoc_free(LinBlockAllocator* lba, MemBlock blk) {
    ERR_RETURN_IF_FAIL(lba && blk, ERR_INVALID_ARGUMENTS);

    Size cols = COL(lba->total_capacity);
    for(Size c = 0; c < cols; c++) {
        /* if given block falls in the boundary of any range then we're sure this was allocated from this allocator */
        if(blk >= lba->block_matrix[c] && blk <= lba->block_matrix[NUM_ROWS-1]) {
            Size index = c * NUM_ROWS + ROW(blk - lba->block_matrix[c]);

            /* no need to abort the program and be dramatic about it, since a debug warning should suffice */
            RETURN_IF_FAIL(!bitvec_peek(lba->occupancy, index), COLOR_RED "DOUBLE FREE" COLOR_RESET " : from LinBlockAllocator\n");
            bitvec_clear(lba->occupancy, index);

            lba->last_freed_block = index;
            return;
        }
    }

    ERR(__FUNCTION__, COLOR_RED "INVALID FREE" COLOR_RESET " : Provided MemBlock not allocated from provided LinBlockAllocator\n");
}

/**
 * Get a value between 0 and 1 representing load on the
 * linear block allocator. The value is total number of allocated
 * blocks divided by total number of blocks that has been or will
 * be allocated.
 *
 * @param lba
 * */
Float32  lballoc_get_load(LinBlockAllocator* lba) {
    return (Float32)lba->allocation_count/(Float32)lba->total_capacity;
}
