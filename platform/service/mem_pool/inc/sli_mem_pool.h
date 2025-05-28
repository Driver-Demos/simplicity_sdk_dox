/***************************************************************************/ /**
 * @file
 * @brief Memory Pool API.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#ifndef SL_MEM_POOL_H
#define SL_MEM_POOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef DOXYGEN
/***************************************************************************/ /**
 * @addtogroup mempool Memory Pool
 * @brief Memory Pool
 * @{
 ******************************************************************************/

/// @brief Memory Pool Handle
#endif // DOXYGEN
/***************************************************************************//**
 * @brief The `sli_mem_pool_handle_t` is a structure used to manage a memory
 * pool, which is a collection of fixed-size memory blocks. It contains
 * pointers to the head of the free block list and the buffer, as well as
 * information about the size and total number of blocks in the pool.
 * This structure is essential for efficiently allocating and freeing
 * memory blocks within the pool, facilitating memory management in
 * embedded systems or applications where dynamic memory allocation is
 * constrained.
 *
 * @param free_block_addr Pointer to the head of the free block list.
 * @param data Pointer to the buffer.
 * @param block_size Size of the blocks.
 * @param block_count Total number of blocks in the pool.
 ******************************************************************************/
typedef struct sli_mem_pool_handle{
  void* free_block_addr;   ///< Pointer to head of free block list.
  void* data;              ///< Pointer to buffer.
  uint16_t block_size;     ///< Size of the blocks.
  uint16_t block_count;    ///< Total number of blocks in pool.
} sli_mem_pool_handle_t;

#define SLI_MEM_POOL_PAD_OBJECT_SIZE(block_size) (block_size + sizeof(uint32_t) - 1)

#define SLI_MEM_POOL_BUFFER_SIZE(block_size, block_count) (block_count                                \
                                                           * SLI_MEM_POOL_PAD_OBJECT_SIZE(block_size) \
                                                           / sizeof(uint32_t))

#ifndef DOXYGEN
/***************************************************************************/ /**
 * Helper macro to create a memory pool buffer.
 *
 * @param[in] mempool_name  Name of memory pool.
 * @param[in] block_size  Size of the blocks.
 * @param[in] block_count  Number of blocks in pool.
 ******************************************************************************/
#endif // DOXYGEN
#define SLI_MEM_POOL_DECLARE_BUFFER(mempool_name, block_size, block_count) \
  static uint32_t mempool_name##_buffer[SLI_MEM_POOL_BUFFER_SIZE(block_size, block_count)]

#ifndef DOXYGEN
/***************************************************************************/ /**
 * Helper macro to create a memory pool buffer by providing the block type.
 *
 * @param[in] mempool_name  Name of memory pool.
 * @param[in] block_type  Type of the blocks.
 * @param[in] block_count  Number of blocks in pool.
 ******************************************************************************/
#endif // DOXYGEN
#define SLI_MEM_POOL_DECLARE_BUFFER_WITH_TYPE(mempool_name, block_type, block_count) \
  static struct {                                                                    \
    union {                                                                          \
      void *ptr;                                                                     \
      block_type entry;                                                              \
    };                                                                               \
  } mempool_name##_buffer[block_count]

#ifndef DOXYGEN
// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************/ /**
 * Creates a memory pool.
 *
 * @param[in] mem_pool  Pointer to memory pool handle.
 * @param[in] block_size  Size of the blocks.
 * @param[in] block_count  Number of blocks in pool.
 * @param[in] buffer  Pointer to buffer to use as pool storage.
 * @param[in] buffer_size  Size of the buffer to use as pool storage.
 ******************************************************************************/
#endif // DOXYGEN
/***************************************************************************//**
 * @brief This function initializes a memory pool using a provided buffer,
 * setting up the pool to manage a specified number of blocks of a given
 * size. It must be called before any allocation or deallocation
 * operations are performed on the memory pool. The buffer must be large
 * enough to accommodate the total number of blocks, each with the
 * specified size plus any required padding. The function requires valid
 * pointers for both the memory pool handle and the buffer, and it
 * asserts if these are null or if the buffer size is insufficient.
 *
 * @param mem_pool Pointer to a memory pool handle that will be initialized.
 * Must not be null.
 * @param block_size Size of each block in the pool. Must be greater than zero.
 * @param block_count Number of blocks in the pool. Must be greater than zero.
 * @param buffer Pointer to the buffer used for pool storage. Must not be null.
 * @param buffer_size Size of the buffer in bytes. Must be at least block_count
 * * (block_size + required padding).
 * @return None
 ******************************************************************************/
void sli_mem_pool_create(sli_mem_pool_handle_t *mem_pool,
                         uint16_t block_size,
                         uint16_t block_count,
                         void* buffer,
                         uint32_t buffer_size);

#ifndef DOXYGEN
/***************************************************************************/ /**
 * Allocates an object from a memory pool.
 *
 * @param[in] mem_pool  Pointer to memory pool handle.
 *
 * @return Pointer to allocated block. Null if pool is empty.
 ******************************************************************************/
#endif // DOXYGEN
/***************************************************************************//**
 * @brief This function is used to allocate a block of memory from a specified
 * memory pool. It should be called when a block of memory is needed from
 * the pool, and the pool must have been previously initialized using
 * `sli_mem_pool_create`. If the memory pool is empty, the function
 * returns NULL, indicating that no blocks are available. The function is
 * thread-safe and can be used in interrupt contexts, as it manages
 * critical sections internally.
 *
 * @param mem_pool Pointer to a memory pool handle from which to allocate a
 * block. Must not be null. If null, the function will assert
 * and return NULL.
 * @return Returns a pointer to the allocated block if successful, or NULL if
 * the memory pool is empty or if the input is invalid.
 ******************************************************************************/
void* sli_mem_pool_alloc(sli_mem_pool_handle_t *mem_pool);

#ifndef DOXYGEN
/***************************************************************************/ /**
 * Frees an object previously allocated to a memory pool.
 *
 * @param[in] mem_pool  Pointer to memory pool handle.
 * @param[in] block  Pointer to block to free.
 ******************************************************************************/
#endif // DOXYGEN
/***************************************************************************//**
 * @brief This function is used to return a block of memory, previously
 * allocated from a memory pool, back to the pool for future reuse. It
 * must be called with a valid memory pool handle and a block pointer
 * that was obtained from the same pool. The function ensures that the
 * block being freed is within the valid range of the memory pool's
 * buffer. It is important to ensure that the memory pool handle and
 * block pointer are not null before calling this function, as invalid
 * inputs will lead to assertions. This function is typically called when
 * the allocated memory is no longer needed, allowing efficient reuse of
 * memory resources.
 *
 * @param mem_pool Pointer to a memory pool handle. Must not be null. The memory
 * pool must have been previously initialized and the block must
 * have been allocated from this pool.
 * @param block Pointer to the block to be freed. Must not be null and must
 * point to a valid block within the memory pool's buffer range.
 * The block should have been previously allocated from the same
 * memory pool.
 * @return None
 ******************************************************************************/
void sli_mem_pool_free(sli_mem_pool_handle_t *mem_pool, void *block);

#ifndef DOXYGEN
/** @} (end addtogroup mempool) */
#endif // DOXYGEN

#ifdef __cplusplus
}
#endif

#endif // SL_MEM_POOL_H
