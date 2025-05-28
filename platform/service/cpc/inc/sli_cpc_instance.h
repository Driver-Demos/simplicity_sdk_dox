/***************************************************************************/ /**
 * @file
 * @brief CPC Instance Internal Definitions
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_CPC_INSTANCE_H
#define SLI_CPC_INSTANCE_H

#include "sl_slist.h"

#include "sli_cpc.h"
#include "sli_cpc_drv.h"
#include "sli_cpc_memory.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "cmsis_os2.h"
#if defined(SL_CATALOG_CMSIS_OS_COMMON_PRESENT)
#include "sl_cmsis_os2_common.h"
#endif
#include "sl_cpc_kernel_config.h"
#endif

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
#include "sli_cpc_system_primary.h"
#endif

#if defined(SL_CATALOG_CPC_SECONDARY_PRESENT)
#include "sli_cpc_system_secondary.h"
#endif

#if defined(SL_CATALOG_MEMORY_MANAGER_PRESENT)
#include "sl_memory_manager.h"
#else
#include "sli_mem_pool.h"
#endif

// Derive name of sli_cpc_instance_t based on the peripheral name
#define SL_CPC_INSTANCE_NAME(_peripheral)       SL_CONCAT_PASTER_2(sli_cpc_instance_, _peripheral)

#define SL_CPC_INSTANCE_PTR(_peripheral)        SL_CONCAT_PASTER_2(&sli_cpc_instance_, _peripheral)

// This set of macro allows to compute different values
// based on the user configuration stored in the instance
#define SLI_CPC_RX_DATA_MAX_LENGTH_INST(_inst)             (SLI_CPC_RX_DATA_MAX_LENGTH_CFG((_inst)->rx_user_payload_max_length))
#define SLI_CPC_HDLC_REJECT_MAX_COUNT_INST(_inst)          (SLI_CPC_HDLC_REJECT_MAX_COUNT_CFG((_inst)->rx_buffer_max_count))
#define SLI_CPC_RX_QUEUE_ITEM_MAX_COUNT_INST(_inst)        (SLI_CPC_RX_QUEUE_ITEM_MAX_COUNT_CFG((_inst)->rx_buffer_max_count))
#define SLI_CPC_TX_QUEUE_ITEM_SFRAME_MAX_COUNT_INST(_inst) (SLI_CPC_TX_QUEUE_ITEM_SFRAME_MAX_COUNT_CFG(SLI_CPC_RX_QUEUE_ITEM_MAX_COUNT_INST(_inst)))
#define SLI_CPC_BUFFER_HANDLE_MAX_COUNT_INST(_inst)        (SLI_CPC_BUFFER_HANDLE_MAX_COUNT_CFG( \
                                                              (_inst)->tx_queue_item_max_count,  \
                                                              (_inst)->rx_buffer_max_count,      \
                                                              SLI_CPC_TX_QUEUE_ITEM_SFRAME_MAX_COUNT_INST(_inst)))
#define SLI_CPC_HDLC_HEADER_MAX_COUNT_INST(_inst)          (SLI_CPC_HDLC_HEADER_MAX_COUNT_CFG(SLI_CPC_BUFFER_HANDLE_MAX_COUNT_INST(_inst)))
#define SLI_CPC_RX_FRAME_MAX_LENGTH_INST(_inst)            (SLI_CPC_RX_FRAME_MAX_LENGTH_CFG(SLI_CPC_RX_DATA_MAX_LENGTH_INST(_inst)))
#define SLI_CPC_RX_BUFFER_MAX_LENGTH_INST(_inst)           (SLI_CPC_RX_BUFFER_MAX_LENGTH_CFG(SLI_CPC_RX_FRAME_MAX_LENGTH_INST(_inst)))
#define SLI_CPC_EVENT_SIGNAL_MAX_COUNT_INST(_inst)         (SLI_CPC_EVENT_SIGNAL_MAX_COUNT_CFG( \
                                                              (_inst)->tx_queue_item_max_count, \
                                                              SLI_CPC_RX_QUEUE_ITEM_MAX_COUNT_INST(_inst)))

/***************************************************************************//**
 * @brief The `sl_cpc_mem_pool_handle_t` structure is designed to manage memory
 * pools within the CPC (Communication Protocol Controller) system. It
 * conditionally includes a handle to a memory pool, which can be either
 * a pointer to `sl_memory_pool_t` or a `sli_mem_pool_handle_t`, based on
 * whether the memory manager is present. Additionally, if memory
 * allocator debugging is enabled, it includes a counter to track the
 * number of allocated blocks in the pool, aiding in memory management
 * and debugging.
 *
 * @param handle A pointer to a memory pool handle, which can be either
 * `sl_memory_pool_t` or `sli_mem_pool_handle_t`, depending on the
 * presence of the memory manager.
 * @param used_block_cnt A counter for the number of blocks currently in use,
 * included only if debug memory allocator counters are
 * enabled.
 ******************************************************************************/
typedef struct sl_cpc_mem_pool_handle_t {
  // support having the memory manager or not
#if defined(SL_CATALOG_MEMORY_MANAGER_PRESENT)
  sl_memory_pool_t      *handle;
#else
  sli_mem_pool_handle_t handle;
#endif

#if (SL_CPC_DEBUG_MEMORY_ALLOCATOR_COUNTERS == 1)
  // counter of allocated blocks in the pool
  uint32_t used_block_cnt;      ///< Number of block in use
#endif
} sl_cpc_mem_pool_handle_t;

/***************************************************************************//**
 * @brief The `sli_cpc_instance` structure is a comprehensive data structure
 * used to manage and maintain the state and operations of a CPC
 * (Communication Protocol Controller) instance. It includes various
 * linked lists for managing endpoints, queues for packet transmission
 * and security readiness, and callback lists for handling write
 * completions. The structure also contains dispatcher handles for
 * managing callbacks and retransmissions, as well as memory pool handles
 * for efficient memory management. Additionally, it includes
 * configuration parameters such as maximum payload lengths and buffer
 * counts, and it supports both primary and secondary CPC configurations
 * with flags for version and capability mismatches. The structure is
 * designed to be flexible and adaptable to different operating
 * environments, including those with or without kernel support.
 *
 * @param endpoints Pointer to the list of endpoints.
 * @param defer_endpoint_free_list Pointer to the list of deferred endpoint free
 * operations.
 * @param transmit_queue Pointer to the queue of packets to be transmitted.
 * @param pending_on_security_ready_queue Pointer to the queue of packets
 * pending security readiness.
 * @param write_complete_callback_list Pointer to the list of write complete
 * callbacks.
 * @param expired_retransmit_list Pointer to the list of expired
 * retransmissions.
 * @param callback_dispatcher_handle Handle for the callback dispatcher.
 * @param retransmit_dispatcher_handle Handle for the retransmit dispatcher.
 * @param process_queue Pointer to the queue of processes.
 * @param event_counter Counter for events.
 * @param thread_id ID of the thread.
 * @param event_signal Signal for events.
 * @param endpoints_list_lock Lock for the endpoints list.
 * @param tx_item_availability_signal Signal for transmit queue item
 * availability.
 * @param rx_process_flag Flag for RX process.
 * @param driver Pointer to the CPC driver.
 * @param sli_cpc_driver_capabilities Capabilities of the CPC driver.
 * @param rx_user_payload_max_length Maximum length of a user payload.
 * @param rx_buffer_max_count Maximum count of RX buffers.
 * @param tx_queue_item_max_count Maximum count of TX queue items.
 * @param remote_tx_payload_max_length Maximum length of remote TX payload.
 * @param postponed_free_rx_queue_item Pointer to the postponed free RX queue
 * item.
 * @param tx_item_used_count Count of used TX items.
 * @param sframe_tx_item_used_count Count of used S-frame TX items.
 * @param mempool_buffer_handle Handle for the buffer memory pool.
 * @param mempool_hdlc_header Handle for the HDLC header memory pool.
 * @param mempool_hdlc_reject Handle for the HDLC reject memory pool.
 * @param mempool_rx_buffer Handle for the RX buffer memory pool.
 * @param mempool_rx_queue_item Handle for the RX queue item memory pool.
 * @param mempool_command_handle Handle for the command memory pool.
 * @param mempool_system_command Handle for the system command memory pool.
 * @param mempool_tx_security_tag Handle for the TX security tag memory pool.
 * @param endpoints_used_block_cnt Count of used blocks in endpoints.
 * @param core_debug Core debug information.
 * @param system_ep System endpoint information.
 * @param cpc_core_in_reset Flag indicating if CPC core is in reset.
 * @param cpc_initialized Flag indicating if CPC is initialized.
 * @param secondary_protocol_version_mismatch Flag for secondary protocol
 * version mismatch.
 * @param secondary_capabilities_mismatch Flag for secondary capabilities
 * mismatch.
 * @param secondary_application_version_mismatch Flag for secondary application
 * version mismatch.
 ******************************************************************************/
struct sli_cpc_instance {
  sl_slist_node_t *endpoints;
  sl_slist_node_t *defer_endpoint_free_list;
  sl_slist_node_t *transmit_queue;
  sl_slist_node_t *pending_on_security_ready_queue;
  sl_slist_node_t *write_complete_callback_list;
  sl_slist_node_t *expired_retransmit_list;

  sl_cpc_dispatcher_handle_t callback_dispatcher_handle;
  sl_cpc_dispatcher_handle_t retransmit_dispatcher_handle;

  // dispatcher holding variables
  sl_slist_node_t *process_queue;
  uint8_t event_counter;

#if defined(SL_CATALOG_KERNEL_PRESENT)
#if defined(SL_CATALOG_CMSIS_OS_COMMON_PRESENT)
  /*
   * __ALIGNED is part of CMSIS-Core, but it's possible that an CMSIS-RTOS2 OS
   * is used without CMSIS-Core. In that case, instead of providing statically
   * allocated memory, let the OS dynamically allocate it.
   */
#define THREAD_STACK_SIZE   (SL_CPC_TASK_STACK_SIZE * sizeof(void *)) & 0xFFFFFFF8u

  __ALIGNED(4) uint8_t endpoints_list_mutex_cb[osMutexCbSize];
  __ALIGNED(4) uint8_t event_semaphore_cb[osSemaphoreCbSize];
  __ALIGNED(4) uint8_t thread_cb[osThreadCbSize];
  __ALIGNED(8) uint8_t thread_stack[THREAD_STACK_SIZE];
  __ALIGNED(4) uint8_t tx_item_availability_semaphore_cb[osSemaphoreCbSize];
#endif

  osThreadId_t thread_id;                       ///< thread ID
  osSemaphoreId_t event_signal;                 ///< event signal
  osMutexId_t endpoints_list_lock;              ///< endpoints list lock
  osSemaphoreId_t tx_item_availability_signal;  ///< transmit queue item availability signal
#else
  volatile uint32_t rx_process_flag;
#endif

  sli_cpc_drv_t *driver;
  sli_cpc_drv_capabilities_t sli_cpc_driver_capabilities;

  uint32_t rx_user_payload_max_length;          ///< Maximum length of a user payload (USER CONFIG)
  uint16_t rx_buffer_max_count;                 ///< Maximum count of RX buffer (USER CONFIG)
  uint16_t tx_queue_item_max_count;             ///< Maximum TX queue item (USER CONFIG)

  // The default value of 0 means that there is no maximum length.
  // This will stay at 0 with a Linux host and will be set to a
  // certain value with a MCU Primary.
  uint16_t remote_tx_payload_max_length;

  sl_slist_node_t *postponed_free_rx_queue_item;
#if !defined(SL_CATALOG_KERNEL_PRESENT)
  uint16_t tx_item_used_count;
#endif
  uint16_t sframe_tx_item_used_count;

  sl_cpc_mem_pool_handle_t mempool_buffer_handle;
  sl_cpc_mem_pool_handle_t mempool_hdlc_header;
  sl_cpc_mem_pool_handle_t mempool_hdlc_reject;
  sl_cpc_mem_pool_handle_t mempool_rx_buffer;
  sl_cpc_mem_pool_handle_t mempool_rx_queue_item;

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
  sl_cpc_mem_pool_handle_t mempool_command_handle;
#endif

#if defined(SL_CATALOG_CPC_SECONDARY_PRESENT)
  sl_cpc_mem_pool_handle_t mempool_system_command;
#endif

#if (SL_CPC_ENDPOINT_SECURITY_ENABLED >= 1)
  sl_cpc_mem_pool_handle_t mempool_tx_security_tag;
#endif

#if (SL_CPC_DEBUG_MEMORY_ALLOCATOR_COUNTERS == 1)
  uint32_t endpoints_used_block_cnt;
#endif

#if ((SL_CPC_DEBUG_CORE_EVENT_COUNTERS == 1) \
  || (SL_CPC_DEBUG_MEMORY_ALLOCATOR_COUNTERS == 1))
  sl_cpc_core_debug_t core_debug;
#endif

  sli_cpc_system_endpoint_t system_ep;

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
  bool cpc_core_in_reset;
  bool cpc_initialized;
  bool secondary_protocol_version_mismatch;
  bool secondary_capabilities_mismatch;
  bool secondary_application_version_mismatch;
#endif // SL_CATALOG_CPC_PRIMARY_PRESENT
};

/***************************************************************************//**
 * @brief The `sli_cpc_instance_t` structure is a comprehensive data structure
 * used to manage and operate a CPC (Co-Processor Communication)
 * instance. It contains various members that handle different aspects of
 * the CPC instance, such as endpoint management, transmission queues,
 * dispatcher handles, and memory pool handles. The structure is designed
 * to support both primary and secondary CPC configurations, with
 * specific fields for handling kernel operations, memory management, and
 * security features. It also includes configuration parameters for user
 * payloads and buffer counts, as well as flags for tracking the
 * initialization and compatibility status of the CPC instance.
 *
 * @param endpoints Pointer to a list of endpoints.
 * @param defer_endpoint_free_list Pointer to a list of deferred endpoints to be
 * freed.
 * @param transmit_queue Pointer to the queue of items to be transmitted.
 * @param pending_on_security_ready_queue Pointer to the queue of items pending
 * security readiness.
 * @param write_complete_callback_list Pointer to the list of write complete
 * callbacks.
 * @param expired_retransmit_list Pointer to the list of expired
 * retransmissions.
 * @param callback_dispatcher_handle Handle for the callback dispatcher.
 * @param retransmit_dispatcher_handle Handle for the retransmit dispatcher.
 * @param process_queue Pointer to the queue of processes.
 * @param event_counter Counter for events.
 * @param thread_id ID of the thread.
 * @param event_signal Signal for events.
 * @param endpoints_list_lock Lock for the endpoints list.
 * @param tx_item_availability_signal Signal for transmit queue item
 * availability.
 * @param rx_process_flag Flag for RX process.
 * @param driver Pointer to the CPC driver.
 * @param sli_cpc_driver_capabilities Capabilities of the CPC driver.
 * @param rx_user_payload_max_length Maximum length of a user payload.
 * @param rx_buffer_max_count Maximum count of RX buffers.
 * @param tx_queue_item_max_count Maximum count of TX queue items.
 * @param remote_tx_payload_max_length Maximum length of remote TX payload.
 * @param postponed_free_rx_queue_item Pointer to postponed free RX queue items.
 * @param tx_item_used_count Count of used TX items.
 * @param sframe_tx_item_used_count Count of used S-frame TX items.
 * @param mempool_buffer_handle Handle for the memory pool buffer.
 * @param mempool_hdlc_header Handle for the memory pool HDLC header.
 * @param mempool_hdlc_reject Handle for the memory pool HDLC reject.
 * @param mempool_rx_buffer Handle for the memory pool RX buffer.
 * @param mempool_rx_queue_item Handle for the memory pool RX queue item.
 * @param mempool_command_handle Handle for the memory pool command.
 * @param mempool_system_command Handle for the memory pool system command.
 * @param mempool_tx_security_tag Handle for the memory pool TX security tag.
 * @param endpoints_used_block_cnt Count of used blocks in endpoints.
 * @param core_debug Core debug information.
 * @param system_ep System endpoint.
 * @param cpc_core_in_reset Flag indicating if CPC core is in reset.
 * @param cpc_initialized Flag indicating if CPC is initialized.
 * @param secondary_protocol_version_mismatch Flag for secondary protocol
 * version mismatch.
 * @param secondary_capabilities_mismatch Flag for secondary capabilities
 * mismatch.
 * @param secondary_application_version_mismatch Flag for secondary application
 * version mismatch.
 ******************************************************************************/
typedef struct sli_cpc_instance sli_cpc_instance_t;

/***************************************************************************//**
 * @brief The function `sli_cpc_instance_from_system_ep` retrieves a
 * `sli_cpc_instance_t` structure from a given
 * `sli_cpc_system_endpoint_t` pointer using the `container_of` macro.
 *
 * @param system_ep A pointer to a `sli_cpc_system_endpoint_t` structure,
 * representing a system endpoint within a CPC instance.
 * @return The function returns a pointer to the `sli_cpc_instance_t` structure
 * that contains the provided `system_ep`.
 ******************************************************************************/
static inline sli_cpc_instance_t* sli_cpc_instance_from_system_ep(sli_cpc_system_endpoint_t *system_ep)
{
  return container_of(system_ep, sli_cpc_instance_t, system_ep);
}

/***************************************************************************//**
 * @brief This function initializes a CPC (Co-Processor Communication) instance,
 * preparing it for use. It must be called before any other operations on
 * the instance are performed. The function handles both early and late
 * initialization stages, and if any step fails, it ensures proper
 * cleanup before returning an error status. This function is essential
 * for setting up the communication infrastructure and should be called
 * once per instance before starting any communication tasks.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance to be initialized. Must not be null. The caller retains
 * ownership of the instance structure.
 * @return Returns SL_STATUS_OK if the initialization is successful. If an error
 * occurs during initialization, a corresponding sl_status_t error code
 * is returned.
 ******************************************************************************/
sl_status_t sli_cpc_instance_init(sli_cpc_instance_t *inst);

/***************************************************************************//**
 * @brief Use this function to deinitialize a CPC instance when it is no longer
 * needed. This function should be called to release resources associated
 * with the instance, ensuring that any ongoing operations are properly
 * terminated. It is important to ensure that the instance is not in use
 * by any other operations before calling this function to avoid
 * undefined behavior.
 *
 * @param inst A pointer to the CPC instance to be deinitialized. Must not be
 * null. The caller retains ownership of the memory.
 * @return None
 ******************************************************************************/
void sli_cpc_instance_deinit(sli_cpc_instance_t *inst);

/***************************************************************************//**
 * @brief This function is used to start a CPC instance, enabling it to process
 * incoming and outgoing packets. It should be called after the instance
 * has been properly initialized. The function attempts to start the
 * receive operations and the system endpoint for the given instance. If
 * any of these operations fail, the function will return an error
 * status. It is important to handle these error statuses appropriately
 * to ensure the instance is correctly started.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance to be started. This pointer must not be null, and the
 * instance should be properly initialized before calling this
 * function. If the pointer is invalid or the instance is not
 * initialized, the behavior is undefined.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while other status
 * codes indicate specific errors.
 ******************************************************************************/
sl_status_t sli_cpc_instance_start(sli_cpc_instance_t *inst);

/***************************************************************************//**
 * @brief This function retrieves the current bitrate of the communication bus
 * associated with a given CPC instance. It is useful for applications
 * that need to monitor or adjust their behavior based on the bus speed.
 * The function must be called with a valid CPC instance that has been
 * properly initialized. If the instance or its driver is null, the
 * function will assert, indicating a critical error in usage.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance. Must not be null and must have a valid driver
 * associated with it. The caller retains ownership of the instance.
 * @return The function returns a uint32_t value representing the current
 * bitrate of the bus.
 ******************************************************************************/
uint32_t sli_cpc_instance_bus_get_bitrate(sli_cpc_instance_t *inst);

/***************************************************************************//**
 * @brief This function retrieves the maximum bitrate of the communication bus
 * for a given CPC instance. It is useful for determining the upper limit
 * of data transfer rates that can be achieved with the specified
 * instance. The function must be called with a valid CPC instance that
 * has been properly initialized and configured. It is important to
 * ensure that the instance and its associated driver are not null before
 * calling this function, as invalid inputs will result in undefined
 * behavior.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance. Must not be null, and the instance's driver must also
 * be valid. The caller retains ownership of the instance.
 * @return Returns the maximum bitrate of the bus as a uint32_t value.
 ******************************************************************************/
uint32_t sli_cpc_instance_bus_get_max_bitrate(sli_cpc_instance_t *inst);

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
/***************************************************************************//**
 * @brief Use this function to determine whether a given CPC instance has been
 * successfully initialized. This is useful for ensuring that the
 * instance is ready for operations that require initialization. It
 * should be called before performing any actions that depend on the
 * instance being initialized.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance. Must not be null. If the pointer is invalid, the
 * behavior is undefined.
 * @return Returns true if the instance is initialized, false otherwise.
 ******************************************************************************/
bool sli_cpc_instance_is_initialized(sli_cpc_instance_t *inst);
#endif
#endif /* SLI_CPC_INSTANCE_H */
