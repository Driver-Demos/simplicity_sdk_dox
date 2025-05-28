/***************************************************************************//**
 * @file
 * @brief Memory Manager Driver API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_MEMORY_MANAGER_H_
#define SLI_MEMORY_MANAGER_H_

#include "sl_memory_manager.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *********************************   DEFINES   *********************************
 ******************************************************************************/

// Memory Manager integration to SystemView is enabled on GCC builds of
// applications that include the SystemView component
#if defined(SL_CATALOG_SYSTEMVIEW_TRACE_PRESENT) && defined(__GNUC__)
#define SLI_MEMORY_MANAGER_ENABLE_SYSTEMVIEW
#endif

// Minimum block alignment in bytes. 8 bytes is the minimum alignment to account for largest CPU data type
// that can be used in some block allocation scenarios. 64-bit data type may be used to manipulate the
// allocated block. The ARM processor ABI defines data types and byte alignment, and 8-byte alignment
// can be seen for the largest data object type.
#define SLI_BLOCK_ALLOC_MIN_ALIGN   SL_MEMORY_BLOCK_ALIGN_8_BYTES

// Minimum block allocation size to avoid creating a block too small while splitting up an allocated block.
// Minimum size is formed from (metadata + payload) size. Size expressed in bytes.
#define SLI_BLOCK_ALLOCATION_MIN_SIZE   (SLI_BLOCK_METADATA_SIZE_BYTE + SL_MEMORY_MANAGER_BLOCK_ALLOCATION_MIN_SIZE)

// Minimum block reservation size to avoid creating a block too small while splitting up a reserved block.
// Contrary to block allocations, reservations don't have metadata.
#define SLI_BLOCK_RESERVATION_MIN_SIZE_BYTE   SL_MEMORY_MANAGER_BLOCK_ALLOCATION_MIN_SIZE
#define SLI_BLOCK_RESERVATION_MIN_SIZE_DWORD  SLI_BLOCK_LEN_BYTE_TO_DWORD(SL_MEMORY_MANAGER_BLOCK_ALLOCATION_MIN_SIZE)

// 64-bit word size (in octets).
#define SLI_WORD_SIZE_64                8u
// 32-bit word size (in octets).
#define SLI_WORD_SIZE_32                4u
#define SLI_DEF_INT_32_NBR_BITS         32u

// 1-byte size (in bits).
#define SLI_DEF_INT_08_NBR_BITS         8u

// Size of block metadata area in different units.
#define SLI_BLOCK_METADATA_SIZE_BYTE    sizeof(sli_block_metadata_t)
#define SLI_BLOCK_METADATA_SIZE_DWORD   SLI_BLOCK_LEN_BYTE_TO_DWORD(SLI_BLOCK_METADATA_SIZE_BYTE)

// Size of reservation handle area in different units.
#define SLI_RESERVATION_HANDLE_SIZE_BYTE    sizeof(sl_memory_reservation_t)
#define SLI_RESERVATION_HANDLE_SIZE_DWORD   SLI_BLOCK_LEN_BYTE_TO_DWORD(SLI_RESERVATION_HANDLE_SIZE_BYTE)

// Size of pool handle area in different units.
#define SLI_POOL_HANDLE_SIZE_BYTE    sizeof(sl_memory_pool_t)
#define SLI_POOL_HANDLE_SIZE_DWORD   SLI_BLOCK_LEN_BYTE_TO_DWORD(SLI_POOL_HANDLE_SIZE_BYTE)

#ifdef SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES
#define SLI_MAX_RESERVATION_COUNT 32
#endif

/*******************************************************************************
 **********************************   MACROS   *********************************
 ******************************************************************************/

// Macros to align a value to the nearest value multiple of the specified alignment
// (rounded up or down). These macros are used for memory addresses requiring an alignment.
#define SLI_ALIGN_ROUND_UP(num, align) (((num) + ((align) - 1)) & ~((align) - 1))
#define SLI_ALIGN_ROUND_DOWN(num, align) ((num) & ~((align) - 1))

// Macros to convert block length in different units (bytes, double words).
// Byte to word will round up to account for extra bytes.
#define SLI_BLOCK_LEN_DWORD_TO_BYTE(len) ((len) * SLI_WORD_SIZE_64)
#define SLI_BLOCK_LEN_BYTE_TO_DWORD(len) ((len + SLI_WORD_SIZE_64 - 1) / SLI_WORD_SIZE_64)

// Macro to test address given a specified data alignment.
#define SLI_ADDR_IS_ALIGNED(ptr, align_byte)   (((uintptr_t)(const void *)(ptr)) % (align_byte) == 0)

// Macro to convert from bits to byte.
#define SLI_POOL_BITS_TO_BYTE(bits) (((bits) + 7u) / SLI_DEF_INT_08_NBR_BITS)

/*******************************************************************************
 *********************************   TYPEDEF   *********************************
 ******************************************************************************/

// Block metadata containing information about allocated block.
// This metadata allows to implement explicit free blocks list.
// NOTE: The metadata size should ideally be a multiple of 8 bytes (see description of
// SLI_BLOCK_ALLOC_MIN_ALIGN for other details) or at least multiple of CPU data size
// (e.g. 4 bytes for 32-bit CPU).
// 'length' is expressed in double words unit. It can described a block up to 512 KB (65535 * 8 bytes).
/***************************************************************************//**
 * @brief The `sli_block_metadata_t` structure is used to store metadata about
 * memory blocks managed by the memory manager. It includes flags for
 * block usage and alignment, a block type indicator (when SystemView is
 * enabled), and reserved bits for future use. The structure also
 * contains fields for the block's size and offsets to neighboring
 * blocks, facilitating the management of a linked list of memory blocks.
 * This metadata is crucial for implementing an explicit free blocks list
 * and ensuring efficient memory allocation and deallocation.
 *
 * @param block_in_use Flag indicating if the block is allocated or not.
 * @param heap_start_align Flag indicating if the first block at heap start has
 * undergone a data payload adjustment.
 * @param block_type Block type (LT or ST), only defined if
 * SLI_MEMORY_MANAGER_ENABLE_SYSTEMVIEW is enabled.
 * @param reserved Unallocated bits reserved for future usage.
 * @param length Block size in double words (64-bit), excluding metadata.
 * @param offset_neighbour_prev Offset to the previous neighbor block in double
 * words, including metadata and payload sizes.
 * @param offset_neighbour_next Offset to the next neighbor block in double
 * words.
 ******************************************************************************/
typedef struct {
  uint16_t block_in_use : 1;        // Flag indicating if block allocated or not.
  uint16_t heap_start_align : 1;    // Flag indicating if first block at heap start undergone a data payload adjustment.
#if defined(SLI_MEMORY_MANAGER_ENABLE_SYSTEMVIEW)
  uint16_t block_type : 1;          // Block type (LT or ST).
  uint16_t reserved : 13;           // Unallocated for future usage.
#else
  uint16_t reserved : 14;           // Unallocated for future usage.
#endif
  uint16_t length;                  // Block size (metadata not included just data payload), in double words (64 bit).
  uint16_t offset_neighbour_prev;   // Offset to previous neighbor, in double words. It includes metadata/payload sizes.
  uint16_t offset_neighbour_next;   // Offset to next neighbor, in double words.
} sli_block_metadata_t;

/*******************************************************************************
 ****************************   GLOBAL VARIABLES   *****************************
 ******************************************************************************/

extern sli_block_metadata_t *sli_free_st_list_head;
extern sli_block_metadata_t *sli_free_lt_list_head;
extern uint32_t sli_free_blocks_number;
#if defined(DEBUG_EFM) || defined(DEBUG_EFM_USER)
extern bool reserve_no_retention_first;
#endif

#ifdef SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES
extern sl_memory_reservation_t* sli_reservation_handle_ptr_table[];
extern uint32_t sli_reservation_alignment_table[];

extern sl_memory_reservation_t sli_reservation_no_retention_table[];
extern uint32_t sli_reservation_no_retention_alignment_table[];
#endif

// The heap name is also used as the Memory Profiler tracker handle for the heap
// pool managed by the Memory Manager
extern const char sli_mm_heap_name[];
extern const char sli_mm_heap_reservation_name[];

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function is used to reset a memory block metadata structure to
 * its default state, setting all fields to zero. It is typically called
 * before using a new or existing `sli_block_metadata_t` structure to
 * ensure it starts with a known state. This function must be called
 * before the metadata is used in any memory management operations to
 * prevent undefined behavior. The caller must ensure that the
 * `block_metadata` pointer is valid and not null before calling this
 * function.
 *
 * @param block_metadata Pointer to a `sli_block_metadata_t` structure that will
 * be initialized. Must not be null. The caller retains
 * ownership of the memory.
 * @return None
 ******************************************************************************/
void sli_memory_metadata_init(sli_block_metadata_t *block_metadata);

/***************************************************************************//**
 * @brief This function searches for a free memory block that meets the
 * specified size and alignment requirements, and is suitable for either
 * long-term or short-term use. It is useful when allocating memory
 * dynamically, ensuring that the block is properly aligned and of
 * adequate size. The function must be called with a valid block type and
 * alignment, and it will return the size of the block adjusted for
 * alignment. If no suitable block is found, the function returns 0 and
 * sets the block pointer to NULL.
 *
 * @param size The size of the block in bytes. Must be a positive value.
 * @param align The required alignment for the block in bytes. Must be a power
 * of two and at least the minimum alignment defined by the system.
 * @param type The type of block, either long-term or short-term. Must be a
 * valid enumeration value of sl_memory_block_type_t.
 * @param block_reservation A boolean indicating if the block is for a dynamic
 * reservation. True if reservation is needed, false
 * otherwise.
 * @param block A pointer to a pointer that will receive the address of the free
 * block. Must not be null.
 * @return Returns the size of the block adjusted for alignment, or 0 if no
 * suitable block is found.
 ******************************************************************************/
size_t sli_memory_find_free_block(size_t size,
                                  size_t align,
                                  sl_memory_block_type_t type,
                                  bool block_reservation,
                                  sli_block_metadata_t **block);

/***************************************************************************//**
 * @brief This function searches for the next available free memory block of the
 * specified type, either long-term or short-term, starting from a given
 * block or from a default location based on the block type. It is useful
 * for memory management tasks where identifying free blocks is necessary
 * for allocation or reallocation purposes. The function should be called
 * when there are free blocks available, as indicated by a non-zero free
 * block count. If no starting block is provided, the search begins from
 * the heap start for long-term blocks or near the heap end for short-
 * term blocks. The function returns a pointer to the free block or NULL
 * if no suitable block is found.
 *
 * @param type Specifies the type of memory block to search for, either
 * BLOCK_TYPE_LONG_TERM or BLOCK_TYPE_SHORT_TERM. This determines
 * the direction of the search within the memory heap.
 * @param block_start_from Pointer to the block from which to start the search.
 * If NULL, the search begins from a default location
 * based on the block type. The caller retains ownership
 * and must ensure the pointer is valid if not NULL.
 * @return Returns a pointer to the next free block of the specified type, or
 * NULL if no free block is found.
 ******************************************************************************/
sli_block_metadata_t *sli_memory_find_head_free_block(sl_memory_block_type_t type,
                                                      sli_block_metadata_t *block_start_from);

/***************************************************************************//**
 * @brief Use this function to obtain a pointer to the first available block in
 * the long-term memory free list. This is useful when managing memory
 * allocations that are intended to persist for a longer duration. The
 * function does not require any parameters and can be called at any time
 * to check the current head of the long-term free list. It is important
 * to ensure that the memory manager is properly initialized before
 * calling this function to avoid undefined behavior.
 *
 * @return Returns a pointer to the first free long-term memory block, or NULL
 * if no such block is available.
 ******************************************************************************/
void *sli_memory_get_longterm_head_ptr(void);

/***************************************************************************//**
 * @brief This function provides access to the head of the list of free short-
 * term memory blocks managed by the memory manager. It is useful for
 * applications that need to inspect or manipulate the list of available
 * short-term memory blocks. The function does not modify any state or
 * perform any allocation; it simply returns the current head pointer. It
 * should be called when the caller needs to know the starting point of
 * the free short-term memory blocks.
 *
 * @return Returns a pointer to the first free short-term memory block, or NULL
 * if no such block exists.
 ******************************************************************************/
void *sli_memory_get_shortterm_head_ptr(void);

/***************************************************************************//**
 * @brief This function updates the head pointers of the free block lists for
 * both long-term and short-term memory blocks. It should be used when
 * there is a need to adjust the starting points of these lists, either
 * by searching for a new head or by directly setting a new head based on
 * a condition. The function can either search for a new head if the
 * current head matches a specified condition or directly update the head
 * pointers based on the provided free block. It is important to ensure
 * that the `free_head` and `condition_block` parameters are valid
 * pointers to block metadata, and the function should be called in
 * contexts where the memory manager's state is consistent and not
 * concurrently modified.
 *
 * @param free_head Pointer to the block metadata that serves as the new head or
 * starting point for searching. Must not be null.
 * @param condition_block Pointer to the block metadata used as a condition to
 * determine if the head should be updated. Can be null,
 * in which case the function will always search for a
 * new head.
 * @param search Boolean flag indicating whether to search for a new head (true)
 * or to update the head directly based on the condition (false).
 * @return None
 ******************************************************************************/
void sli_update_free_list_heads(sli_block_metadata_t *free_head,
                                const sli_block_metadata_t *condition_block,
                                bool search);

#ifdef SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES
/***************************************************************************//**
 * @brief This function is used to obtain a pointer to a memory reservation
 * handle associated with a given block address. It is useful when you
 * need to manage or query the reservation details of a specific memory
 * block. The function should be called with a valid address that
 * corresponds to a reserved memory block. If the address does not match
 * any known reservation, the function returns NULL, indicating that no
 * reservation handle is associated with the provided address.
 *
 * @param addr A pointer to the block reservation. It must be a valid address of
 * a reserved memory block. If the address is invalid or does not
 * correspond to any reservation, the function returns NULL.
 * @return A pointer to the sl_memory_reservation_t structure associated with
 * the given address, or NULL if no reservation is found for the
 * address.
 ******************************************************************************/
sl_memory_reservation_t *sli_memory_get_reservation_handle_by_addr(void *addr);

/***************************************************************************//**
 * @brief This function retrieves the size of a memory reservation associated
 * with a given address. It is useful for determining the amount of
 * memory reserved at a specific location. The function should be called
 * with a valid address that is expected to be part of a memory
 * reservation. If the address does not correspond to a valid
 * reservation, the function returns 0, indicating no reservation is
 * associated with the address. This function is typically used in memory
 * management scenarios where understanding the size of reserved blocks
 * is necessary.
 *
 * @param addr A pointer to the address of the memory block whose reservation
 * size is to be retrieved. The address must be part of a valid
 * memory reservation. If the address is not associated with a
 * reservation, the function returns 0.
 * @return The function returns the size of the memory reservation in bytes if
 * the address is valid and part of a reservation. If the address is not
 * associated with a reservation, it returns 0.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_size_by_addr(void *addr);

/***************************************************************************//**
 * @brief This function is used to obtain the alignment in bytes of a memory
 * reservation given its address. It is useful when you need to verify or
 * utilize the alignment property of a reserved memory block. The
 * function should be called with a valid address that corresponds to a
 * memory reservation. If the address does not correspond to a valid
 * reservation, the function returns 0, indicating that no alignment
 * information is available for the given address.
 *
 * @param addr A pointer to the block reservation. It must be a valid address
 * corresponding to a memory reservation. If the address is invalid
 * or does not correspond to a reservation, the function returns 0.
 * @return Returns the alignment of the reservation in bytes if the address is
 * valid; otherwise, returns 0.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_align_by_addr(void *addr);

/***************************************************************************//**
 * @brief This function is used to store a memory reservation handle along with
 * its alignment for profiling purposes. It should be called when a new
 * reservation is made, and the handle needs to be tracked. The function
 * attempts to find an available slot to store the reservation handle and
 * its alignment. If successful, it returns a success status; otherwise,
 * it indicates that the storage is full. This function is typically used
 * in environments where memory reservations need to be monitored or
 * logged.
 *
 * @param reservation_handle_ptr A pointer to the memory reservation handle to
 * be stored. Must not be null, and the caller
 * retains ownership of the memory.
 * @param align The alignment of the reservation in bytes. It should be a valid
 * alignment value, typically a power of two.
 * @return Returns SL_STATUS_OK if the reservation handle is successfully
 * stored, or SL_STATUS_FULL if there is no available space to store the
 * handle.
 ******************************************************************************/
sl_status_t sli_memory_save_reservation_handle(sl_memory_reservation_t *reservation_handle_ptr,
                                               uint32_t align);

/***************************************************************************//**
 * @brief This function is used to remove a memory reservation handle from the
 * internal records. It should be called when a reservation is no longer
 * needed and must be removed from the system's tracking. The function
 * expects a valid pointer to a reservation handle. If the handle is
 * found in the records, it is removed, and the function returns a
 * success status. If the handle is not found, the function returns a not
 * found status. This function does not modify the memory associated with
 * the reservation handle, only the internal tracking records.
 *
 * @param reservation_handle_ptr A pointer to the memory reservation handle to
 * be removed. Must not be null. If the handle is
 * not found in the records, the function returns
 * SL_STATUS_NOT_FOUND.
 * @return Returns SL_STATUS_OK if the reservation handle is successfully
 * removed, or SL_STATUS_NOT_FOUND if the handle does not exist in the
 * records.
 ******************************************************************************/
sl_status_t sli_memory_remove_reservation_handle(sl_memory_reservation_t *reservation_handle_ptr);

/***************************************************************************//**
 * @brief This function is used to register a memory block reservation that does
 * not require retention, meaning the reservation is temporary and not
 * preserved across system resets or power cycles. It should be called
 * when a temporary reservation is needed for a block of memory with a
 * specified size and alignment. The function attempts to find an
 * available reservation slot and, if successful, records the reservation
 * details. If no slots are available, it returns an error status. This
 * function is typically used in scenarios where memory management needs
 * to track temporary allocations without long-term retention.
 *
 * @param block_address Pointer to the start address of the memory block to be
 * reserved. Must not be null, and the caller retains
 * ownership of the memory.
 * @param block_size Size of the memory block to be reserved, in bytes. Must be
 * a positive integer.
 * @param align Alignment requirement for the memory block, in bytes. Must be a
 * power of two and at least the minimum alignment defined by the
 * system.
 * @return Returns SL_STATUS_OK if the reservation is successfully recorded, or
 * SL_STATUS_FULL if no reservation slots are available.
 ******************************************************************************/
sl_status_t sli_memory_save_reservation_no_retention(void * block_address, uint32_t block_size, uint32_t align);

/***************************************************************************//**
 * @brief This function is used to obtain the size of a memory reservation that
 * does not retain data across power cycles, based on the provided memory
 * address. It is useful when managing memory reservations that are
 * temporary and do not require data retention. The function should be
 * called with a valid memory address that corresponds to a non-retention
 * reservation. If the address does not correspond to such a reservation,
 * the function returns a size of 0, indicating no reservation is
 * associated with the address.
 *
 * @param addr A pointer to the memory address for which the reservation size is
 * being queried. The address must correspond to a valid non-
 * retention reservation. If the address is invalid or does not
 * correspond to a reservation, the function returns 0.
 * @return The function returns the size of the non-retention reservation in
 * bytes. If the address does not correspond to a reservation, it
 * returns 0.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_no_retention_size(void * addr);

/***************************************************************************//**
 * @brief This function is used to obtain the alignment of a memory reservation
 * that does not retain its state across power cycles, identified by a
 * given address. It is useful in scenarios where alignment information
 * is needed for memory management or debugging purposes. The function
 * should be called with a valid address that corresponds to a non-
 * retention reservation. If the address does not correspond to a valid
 * reservation, the function returns 0, indicating no alignment
 * information is available.
 *
 * @param addr A pointer to the memory reservation whose alignment is to be
 * retrieved. The address must correspond to a valid non-retention
 * reservation. If the address is invalid or does not correspond to
 * a reservation, the function returns 0.
 * @return Returns the alignment of the specified non-retention reservation in
 * bytes, or 0 if the address is invalid or not associated with a
 * reservation.
 ******************************************************************************/
uint32_t sli_memory_get_reservation_no_retention_align(void * addr);

/***************************************************************************//**
 * @brief This function performs a forward integrity check of the heap starting
 * from the head of the free list. It is used to detect any corruption in
 * the heap structure by verifying the consistency of block metadata and
 * ensuring that the end of the heap matches the expected address. The
 * function should be called when there is a need to validate the heap's
 * integrity, such as during debugging or before performing operations
 * that depend on a consistent heap state. If corruption is detected, the
 * function returns a pointer to the corrupted block's metadata.
 *
 * @return Returns a pointer to the corrupted block's metadata if corruption is
 * detected, otherwise returns NULL.
 ******************************************************************************/
sli_block_metadata_t * sli_memory_check_heap_integrity_forwards(void);

/***************************************************************************//**
 * @brief This function performs a backward integrity check of the heap starting
 * from the head of the short-term free list. It is used to detect any
 * corruption in the heap structure by verifying the consistency of block
 * metadata. The function should be called when there is a need to ensure
 * the heap's integrity, especially after operations that modify the
 * heap. It returns a pointer to the first corrupted block metadata if
 * any corruption is detected, or NULL if the heap is intact.
 *
 * @return Returns a pointer to the corrupted sli_block_metadata_t if corruption
 * is detected, otherwise returns NULL.
 ******************************************************************************/
sli_block_metadata_t *sli_memory_check_heap_integrity_backwards(void);
#endif /* SLI_MEMORY_MANAGER_ENABLE_TEST_UTILITIES */

#ifdef __cplusplus
}
#endif

#endif /* SLI_MEMORY_MANAGER_H_ */
