/***************************************************************************/ /**
 * @file
 * @brief CPC
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

#ifndef SL_CPC_H
#define SL_CPC_H

#if defined(__linux__)
#error Wrong platform - this header file is intended for the secondary application
#endif

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#include "sl_enum.h"
#include "sl_status.h"
#include "sl_slist.h"
#include "sl_common.h"
#include "sl_cpc_config.h"

#include <stddef.h>
#include <stdarg.h>

#if (defined(SL_CATALOG_CPC_SECURITY_PRESENT))
#include "sl_cpc_security.h"
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "cmsis_os2.h"
#include "sl_cmsis_os2_common.h"
#endif

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************/ /**
 * @addtogroup cpc CPC
 * @{
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Data Types

/// @brief Enumeration representing user endpoint.
SL_ENUM(sl_cpc_user_endpoint_id_t){
  SL_CPC_ENDPOINT_USER_ID_0 = 90, ///< User endpoint ID 0
  SL_CPC_ENDPOINT_USER_ID_1 = 91, ///< User endpoint ID 1
  SL_CPC_ENDPOINT_USER_ID_2 = 92, ///< User endpoint ID 2
  SL_CPC_ENDPOINT_USER_ID_3 = 93, ///< User endpoint ID 3
  SL_CPC_ENDPOINT_USER_ID_4 = 94, ///< User endpoint ID 4
  SL_CPC_ENDPOINT_USER_ID_5 = 95, ///< User endpoint ID 5
  SL_CPC_ENDPOINT_USER_ID_6 = 96, ///< User endpoint ID 6
  SL_CPC_ENDPOINT_USER_ID_7 = 97, ///< User endpoint ID 7
  SL_CPC_ENDPOINT_USER_ID_8 = 98, ///< User endpoint ID 8
  SL_CPC_ENDPOINT_USER_ID_9 = 99, ///< User endpoint ID 9
};

/// @brief Enumeration representing the possible endpoint state.
SL_ENUM(sl_cpc_endpoint_state_t){
  SL_CPC_STATE_FREED = 0,                     ///< State freed
  SL_CPC_STATE_OPEN,                          ///< State open
  SL_CPC_STATE_CLOSED,                        ///< State close
  SL_CPC_STATE_CLOSING,                       ///< State closing
  SL_CPC_STATE_CONNECTING,                    ///< Connecting to remote's endpoint
  SL_CPC_STATE_CONNECTED,                     ///< Connected to remote's endpoint
  SL_CPC_STATE_SHUTTING_DOWN,                 ///< Transmissions shutting down
  SL_CPC_STATE_SHUTDOWN,                      ///< Transmissions shutdown
  SL_CPC_STATE_REMOTE_SHUTDOWN,               ///< Remote transmissions shutdown
  SL_CPC_STATE_DISCONNECTED,                  ///< Connection terminated
  SL_CPC_STATE_ERROR_DESTINATION_UNREACHABLE, ///< Error state, destination unreachable
  SL_CPC_STATE_ERROR_SECURITY_INCIDENT,       ///< Error state, security incident
  SL_CPC_STATE_ERROR_FAULT,                   ///< Error state, fault
};

/// @brief Enumeration representing the possible configurable options for an endpoint.
SL_ENUM(sl_cpc_endpoint_option_t){
  SL_CPC_ENDPOINT_ON_IFRAME_RECEIVE = 0,     ///< Set callback for iframe received notification.
  SL_CPC_ENDPOINT_ON_IFRAME_RECEIVE_ARG,     ///< Set callback argument for iframe received notification.
  SL_CPC_ENDPOINT_ON_IFRAME_WRITE_COMPLETED, ///< Set callback for write complete notification.
  SL_CPC_ENDPOINT_ON_ERROR,                  ///< Set callback for error notification.
  SL_CPC_ENDPOINT_ON_ERROR_ARG,              ///< Set callback argument for error notification.
  SL_CPC_ENDPOINT_ON_CONNECT,                ///< Set callback when host/primary connects to endpoint.
  SL_CPC_ENDPOINT_ON_CONNECT_ARG,            ///< Set callback argument for connect notification.
#if defined(SL_CATALOG_KERNEL_PRESENT) || defined(DOXYGEN)
  SL_CPC_ENDPOINT_SHUTDOWN_TIMEOUT,          ///< Set shutdown timeout, in ticks.
  SL_CPC_ENDPOINT_WRITE_TIMEOUT,             ///< Set the timeout time for blocking write in ticks.
#endif
  // Private options
  SL_CPC_ENDPOINT_ON_UFRAME_RECEIVE,         ///< Set callback for uframe received notification.
  SL_CPC_ENDPOINT_ON_UFRAME_RECEIVE_ARG,     ///< Set callback argument for uframe received notification.
  SL_CPC_ENDPOINT_ON_UFRAME_WRITE_COMPLETED, ///< Set callback for write complete notification.
  SL_CPC_ENDPOINT_ON_FINAL,                  ///< Set callback for on final notification.
  SL_CPC_ENDPOINT_ON_FINAL_ARG,              ///< Set callback argument for on final notification.
};

#define SL_CPC_ENDPOINT_FLAG_NONE                      0           ///< sl cpc open endpoint flag none
#define SL_CPC_ENDPOINT_FLAG_DISABLE_ENCRYPTION        (0x01 << 3) ///< sl cpc open endpoint flag disable encryption

#define SL_CPC_OPEN_ENDPOINT_FLAG_NONE                      SL_CPC_ENDPOINT_FLAG_NONE
#define SL_CPC_OPEN_ENDPOINT_FLAG_DISABLE_ENCRYPTION        SL_CPC_ENDPOINT_FLAG_DISABLE_ENCRYPTION

#define SL_CPC_MAJOR_VERSION     4
#define SL_CPC_MINOR_VERSION     6
#define SL_CPC_PATCH_VERSION     0

/***************************************************************************//**
 * Typedef for the user - supplied callback function which is called when
 * CPC is done with the provided buffer.
 *
 * @param endpoint_id   Endpoint ID
 *
 * @param buffer  Pointer to data buffer.
 *
 * @param arg   Transaction user's argument.
 *
 * @param status  Status indicating the transaction result:
 *                  SL_STATUS_OK  Remote should have receive the frame
 *                  SL_STATUS_TRANSMIT_INCOMPLETE Remote has not received the frame.
 ******************************************************************************/
typedef void (*sl_cpc_on_write_completed_t)(sl_cpc_user_endpoint_id_t endpoint_id,
                                            void *buffer,
                                            void *arg,
                                            sl_status_t status);

/***************************************************************************//**
 * Typedef for the user - supplied callback function which is called when
 * CPC receive data on an endpoint.
 *
 * @param endpoint_id   Endpoint ID
 *
 * @param arg   User-specific argument .
 ******************************************************************************/
typedef void (*sl_cpc_on_data_reception_t)(uint8_t endpoint_id, void *arg);

/***************************************************************************//**
 * Typedef for the user-supplied callback function which is called when
 * CPC detects a fatal error on an endpoint.
 *
 * @param endpoint_id   Endpoint ID
 *
 * @param arg   User-specific argument .
 ******************************************************************************/
typedef void (*sl_cpc_on_error_callback_t)(uint8_t endpoint_id, void *arg);

/***************************************************************************//**
 * Typedef for the user-supplied callback function which is called when
 * CPC connection to a secondary completes, successfully or not.
 *
 * @note  If several users connect to the same endpoint on the host side, this
 *        callback will only be called only when the first connection occurs.
 *
 * @param endpoint_id   Endpoint ID
 *
 * @param arg   User-specific argument.
 *
 * @param status        Indicate if the connection was successful or not:
 *                        SL_STATUS_OK        successfully connected
 *                        SL_STATUS_TIMEOUT   operation timed out
 *                        SL_STATUS_NOT_READY secondary's endpoint is not ready
 ******************************************************************************/
typedef void (*sl_cpc_on_connect_callback_t)(uint8_t endpoint_id, void *arg, sl_status_t status);

/***************************************************************************//**
 * @brief The `sl_cpc_endpoint_handle_t` is a structure used to represent a
 * handle to a CPC (Communication Protocol Controller) endpoint. It
 * contains a pointer to the endpoint object, an identifier for the
 * endpoint, and a reference counter to manage the endpoint's usage. This
 * structure is crucial for managing endpoint connections and operations
 * within the CPC framework, although its members are not intended to be
 * directly manipulated by users.
 *
 * @param ep Endpoint object; Do not touch.
 * @param id Endpoint ID; Do not touch.
 * @param ref_count Endpoint reference counter; Do not touch.
 ******************************************************************************/
typedef struct {
  void *ep;           ///< Endpoint object; Do not touch
  uint8_t id;         ///< Endpoint ID; Do not touch
  uint32_t ref_count; ///< Endpoint reference counter; Do not touch
} sl_cpc_endpoint_handle_t;

/***************************************************************************//**
 * @brief The `sl_cpc_endpoint_debug_counters_t` structure is designed to track
 * various statistics related to the transmission and reception of frames
 * in a CPC (Communication Protocol Controller) endpoint. It includes
 * counters for different types of frames such as data frames,
 * supervisory frames, and unnumbered frames, as well as specific events
 * like frames being queued, dropped, or retransmitted. This structure is
 * useful for debugging and monitoring the performance and reliability of
 * the communication protocol by providing detailed insights into the
 * number of frames processed, transmitted, and any errors encountered
 * during these operations.
 *
 * @param rxd_packet Number of packets received.
 * @param rxd_data_frame Number of frames with payload (dataframes) received.
 * @param rxd_data_frame_queued Number of dataframes with data queued.
 * @param rxd_data_frame_dropped Number of dataframes with data dropped.
 * @param rxd_supervisory_frame Number of supervisory frames received.
 * @param rxd_supervisory_frame_processed Number of supervisory frames
 * processed.
 * @param rxd_supervisory_frame_dropped Number of supervisory frames dropped.
 * @param rxd_unnumbered_frame Number of unnumbered frames received.
 * @param rxd_unnumbered_frame_processed Number of unnumbered frames processed.
 * @param rxd_unnumbered_frame_dropped Number of unnumbered frames dropped.
 * @param rxd_duplicate_data_frame Number of duplicate dataframes received.
 * @param rxd_ack Number of ACK supervisory-frames received.
 * @param rxd_reject_destination_unreachable Number of destination unreachable
 * supervisory-frames received.
 * @param rxd_reject_seq_mismatch Number of out of order supervisory-frames
 * received.
 * @param rxd_reject_checksum_mismatch Number of checksum error supervisory-
 * frames received.
 * @param rxd_reject_security_issue Number of security issue supervisory-frames
 * received.
 * @param rxd_reject_out_of_memory Number of out of memory supervisory-frames
 * received.
 * @param rxd_reject_fault Number of fault supervisory-frames received.
 * @param txd_data_frame Number of dataframes transmitted.
 * @param txd_ack Number of ACK supervisory-frames transmitted.
 * @param txd_reject_destination_unreachable Number of destination unreachable
 * supervisory-frames transmitted.
 * @param txd_reject_seq_mismatch Number of out of order supervisory-frames
 * transmitted.
 * @param txd_reject_checksum_mismatch Number of checksum error supervisory-
 * frames transmitted.
 * @param txd_reject_security_issue Number of security issue supervisory-frames
 * transmitted.
 * @param txd_reject_out_of_memory Number of out of memory supervisory-frames
 * transmitted.
 * @param txd_reject_fault Number of fault supervisory-frames transmitted.
 * @param retxd_data_frame Number of dataframes retransmitted.
 * @param frame_transmit_submitted Number of frames submitted to the driver.
 * @param frame_transmit_completed Number of frames confirmed sent by the
 * driver.
 * @param data_frame_transmit_completed Number of dataframes confirmed sent by
 * the driver.
 * @param supervisory_frame_transmit_completed Number of supervisory-frames
 * confirmed sent by the driver.
 ******************************************************************************/
typedef struct {
  uint32_t rxd_packet;                            ///< Number of packet received
  uint32_t rxd_data_frame;                        ///< Number of frame with payload (dataframe);
  uint32_t rxd_data_frame_queued;                 ///< Number of dataframe with data queued
  uint32_t rxd_data_frame_dropped;                ///< Number of dataframe with data dropped

  uint32_t rxd_supervisory_frame;                 ///< Number of supervisory frame received
  uint32_t rxd_supervisory_frame_processed;       ///< Number of supervisory frame processed
  uint32_t rxd_supervisory_frame_dropped;         ///< Number of supervisory frame dropped

  uint32_t rxd_unnumbered_frame;                  ///< Number of unnumbered frame received
  uint32_t rxd_unnumbered_frame_processed;        ///< Number of unnumbered frame processed
  uint32_t rxd_unnumbered_frame_dropped;          ///< Number of unnumbered frame dropped

  uint32_t rxd_duplicate_data_frame;              ///< Number of duplicate dataframe received
  uint32_t rxd_ack;                               ///< Number of ACK supervisory-frame received
  uint32_t rxd_reject_destination_unreachable;    ///< Number of destination unreachable supervisory-frame received
  uint32_t rxd_reject_seq_mismatch;               ///< Number of out of order supervisory-frame received
  uint32_t rxd_reject_checksum_mismatch;          ///< Number of checksum error supervisory-frame received
  uint32_t rxd_reject_security_issue;             ///< Number of security issue supervisory-frame received
  uint32_t rxd_reject_out_of_memory;              ///< Number of out of memorry supervisory-frame received
  uint32_t rxd_reject_fault;                      ///< Number of fault supervisory-frame received

  uint32_t txd_data_frame;                        ///< Number of dataframe transmitted
  uint32_t txd_ack;                               ///< Number of ACK supervisory-frame transmitted
  uint32_t txd_reject_destination_unreachable;    ///< Number of destination unreachable supervisory-frame transmitted
  uint32_t txd_reject_seq_mismatch;               ///< Number of out of order supervisory-frame transmitted
  uint32_t txd_reject_checksum_mismatch;          ///< Number of checksum error supervisory-frame transmitted
  uint32_t txd_reject_security_issue;             ///< Number of security issue supervisory-frame transmitted
  uint32_t txd_reject_out_of_memory;              ///< Number of out of memorry supervisory-frame transmitted
  uint32_t txd_reject_fault;                      ///< Number of fault supervisory-frame transmitted

  uint32_t retxd_data_frame;                      ///< Number of dataframe retransmitted

  uint32_t frame_transmit_submitted;              ///< Number of frame submitted to the driver
  uint32_t frame_transmit_completed;              ///< Number of frame confirmed sent by the driver
  uint32_t data_frame_transmit_completed;         ///< Number of dataframe-frame confirmed sent by the driver
  uint32_t supervisory_frame_transmit_completed;  ///< Number of supervisory-frame confirmed sent by the driver
} sl_cpc_endpoint_debug_counters_t;

/// @brief Struct representing CPC Core debug counters.
/***************************************************************************//**
 * @brief The `sl_cpc_core_debug_counters_t` structure is designed to keep track
 * of various debugging metrics related to the operation of CPC
 * (Communication Protocol Controller) endpoints. It includes counters
 * for the number of endpoints opened and closed, frames received and
 * transmitted, as well as specific error conditions such as invalid
 * checksums and driver-reported errors. This structure is essential for
 * monitoring and diagnosing the performance and reliability of CPC
 * communications.
 *
 * @param endpoint_opened Number of endpoint opened.
 * @param endpoint_closed Number of endpoint closed.
 * @param rxd_frame Total number of frames received.
 * @param rxd_valid_iframe Total number of i-frames received.
 * @param rxd_valid_uframe Total number of u-frames received.
 * @param rxd_valid_sframe Total number of s-frames received.
 * @param rxd_data_frame_dropped Total number of frames dropped.
 * @param txd_reject_destination_unreachable Total number of unreachable
 * destination transmissions.
 * @param txd_reject_error_fault Total number of fault transmissions.
 * @param txd_completed Total number of frames confirmed sent by the driver.
 * @param retxd_data_frame Total number of data frame retransmissions.
 * @param driver_error Total number of errors reported by the driver.
 * @param driver_packet_dropped Total number of frames dropped by the driver.
 * @param invalid_header_checksum Total number of frames received with invalid
 * header checksum.
 * @param invalid_payload_checksum Total number of frames received with invalid
 * payload checksum.
 ******************************************************************************/
typedef struct {
  uint32_t endpoint_opened;                     ///< Number of endpoint opened
  uint32_t endpoint_closed;                     ///< Number of endpoint closed
  uint32_t rxd_frame;                           ///< Total number of frame received
  uint32_t rxd_valid_iframe;                    ///< Total number of i-frame received
  uint32_t rxd_valid_uframe;                    ///< Total number of u-frame received
  uint32_t rxd_valid_sframe;                    ///< Total number of s-frame received
  uint32_t rxd_data_frame_dropped;              ///< Total number of frame dropped
  uint32_t txd_reject_destination_unreachable;  ///< Total number of unreachable destination transmisted
  uint32_t txd_reject_error_fault;              ///< Total number of fault transmisted
  uint32_t txd_completed;                       ///< Total number of frame confirmed sent by the driver
  uint32_t retxd_data_frame;                    ///< Total number of dataframe retransmission
  uint32_t driver_error;                        ///< Total number of error reported by the driver
  uint32_t driver_packet_dropped;               ///< Total number of frame dropped by the driver
  uint32_t invalid_header_checksum;             ///< Total number of frame received with invalid header checksum
  uint32_t invalid_payload_checksum;            ///< Total number of frame received with invalid frame checksum
} sl_cpc_core_debug_counters_t;

/** @brief Struct representing a core debug. */
#if ((SL_CPC_DEBUG_CORE_EVENT_COUNTERS == 1) \
  || (SL_CPC_DEBUG_MEMORY_ALLOCATOR_COUNTERS == 1))
/***************************************************************************//**
 * @brief The `sl_cpc_core_debug_t` structure is a conditional data structure
 * used for debugging purposes within the CPC (Communication Protocol
 * Controller) framework. It contains a single member, `core_counters`,
 * which is of type `sl_cpc_core_debug_counters_t`. This member is
 * included only when the `SL_CPC_DEBUG_CORE_EVENT_COUNTERS` preprocessor
 * directive is set to 1, allowing for the collection and tracking of
 * various core event counters related to CPC operations. This structure
 * is primarily used for monitoring and debugging the internal workings
 * of the CPC system.
 *
 * @param core_counters Core debug counters, included only if
 * SL_CPC_DEBUG_CORE_EVENT_COUNTERS is set to 1.
 ******************************************************************************/
typedef struct {
#if (SL_CPC_DEBUG_CORE_EVENT_COUNTERS == 1)
  sl_cpc_core_debug_counters_t core_counters;   ///< Core debug counters
#endif
} sl_cpc_core_debug_t;
#endif

#define SL_CPC_USER_ENDPOINT_ID_START     ((uint8_t)SL_CPC_ENDPOINT_USER_ID_0)                                        ///< sl cpc endpoint id start.
#define SL_CPC_USER_ENDPOINT_ID_END       ((uint8_t)SL_CPC_ENDPOINT_USER_ID_9)                                        ///< sl cpc user endpoint id end.
#define SL_CPC_USER_ENDPOINT_MAX_COUNT    (SL_CPC_USER_ENDPOINT_ID_END - SL_CPC_USER_ENDPOINT_ID_START + 1)           ///< maximum simultaneous user endpoint.

#define SL_CPC_FLAG_NO_BLOCK    0x01                                                                                  ///< sl cpc flag no block.

#define SLI_CPC_SECURITY_TAG_LENGTH 8                                                                                 ///< sli cpc security tag length.
#define SLI_CPC_LDMA_DESCRIPTOR_MAX_SIZE 2048                                                                         ///< sli cpc ldma desciptor max size.
#define SLI_CPC_HEADER_SIZE 7                                                                                         ///< sli cpc header size.
#define SLI_CPC_PAYLOAD_CRC_SIZE 2                                                                                    ///< sli cpc payload crc size.

#define SL_CPC_TX_PAYLOAD_MAX_LENGTH  ((2 * SLI_CPC_LDMA_DESCRIPTOR_MAX_SIZE) - SLI_CPC_HEADER_SIZE - SLI_CPC_PAYLOAD_CRC_SIZE) ///< sl cpc tx payload max length.
#define SL_CPC_TX_PAYLOAD_MAX_LENGTH_WITH_SECURITY (SL_CPC_TX_PAYLOAD_MAX_LENGTH - SLI_CPC_SECURITY_TAG_LENGTH)                 ///< sl cpc tx payload max lenght with security.

#define SL_CPC_TRANSMIT_WINDOW_MIN_SIZE  1         ///< sl cpc transmit window min size.
#define SL_CPC_TRANSMIT_WINDOW_MAX_SIZE  1         ///< sl cpc transmit window max size.

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function initializes the CPC (Communication Protocol Controller)
 * module, preparing it for operation. It should be called before any
 * other CPC-related functions to ensure that the module is properly set
 * up. The function iterates over all configured CPC instances,
 * initializing memory and setting up each instance. It returns a status
 * indicating the success or failure of the initialization process. This
 * function must be called once during the system startup to ensure the
 * CPC stack is ready for use.
 *
 * @return Returns an sl_status_t value indicating the result of the
 * initialization process, where SL_STATUS_OK signifies success.
 ******************************************************************************/
sl_status_t sl_cpc_init(void);

/***************************************************************************/ /**
 *  The bare metal process action function.
 ******************************************************************************/
#if !defined(SL_CATALOG_KERNEL_PRESENT) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function iterates over all available CPC instances and processes
 * actions for each one. It is typically used in a bare-metal environment
 * to handle CPC-related tasks that need to be executed regularly. This
 * function should be called in the main loop of the application to
 * ensure that all CPC instances are serviced appropriately. It assumes
 * that the CPC module has been initialized and that the number of
 * instances is correctly set. There are no side effects or return
 * values, and it does not require any parameters.
 *
 * @return None
 ******************************************************************************/
void sl_cpc_process_action(void);
#endif

/***************************************************************************//**
 * @brief This function is used to initialize a user endpoint with a specified
 * ID and configuration flags. It must be called before any operations on
 * the endpoint can be performed. The function checks the validity of the
 * input parameters, including the endpoint handle, endpoint ID, and
 * flags. If the parameters are valid, the endpoint is initialized;
 * otherwise, an error status is returned. This function should be used
 * when setting up communication channels in a CPC (Co-Processor
 * Communication) system.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * that will be initialized. Must not be null. The caller
 * retains ownership.
 * @param id An sl_cpc_user_endpoint_id_t value representing the endpoint ID.
 * Must be between SL_CPC_USER_ENDPOINT_ID_START and
 * SL_CPC_USER_ENDPOINT_ID_END inclusive. Invalid values result in
 * SL_STATUS_INVALID_PARAMETER.
 * @param flags A uint8_t representing initialization flags. Only specific flags
 * are allowed, and invalid flags result in
 * SL_STATUS_INVALID_PARAMETER.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. Possible return values include SL_STATUS_OK for success,
 * SL_STATUS_INVALID_HANDLE if the endpoint_handle is null, and
 * SL_STATUS_INVALID_PARAMETER for invalid ID or flags.
 ******************************************************************************/
sl_status_t sl_cpc_init_user_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle,
                                      sl_cpc_user_endpoint_id_t id,
                                      uint8_t flags);

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function attempts to establish a connection between a local CPC
 * endpoint and a remote endpoint. It should be used when a connection is
 * required for communication between endpoints. The function can operate
 * in blocking or non-blocking mode, depending on the flags provided. In
 * blocking mode, it waits until the connection is established or fails.
 * In non-blocking mode, it returns immediately with a status indicating
 * the connection is in progress. The endpoint must be in a valid state
 * to initiate a connection, and the function handles various endpoint
 * states appropriately. It is important to ensure that the CPC core is
 * ready before calling this function.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * representing the endpoint to connect. Must not be
 * null. If null, the function returns
 * SL_STATUS_INVALID_HANDLE.
 * @param flags A set of flags that modify the function's behavior.
 * SL_CPC_FLAG_NO_BLOCK can be used to make the function non-
 * blocking. Other values are reserved for future use.
 * @return Returns an sl_status_t value indicating the result of the connection
 * attempt. Possible return values include SL_STATUS_OK for a successful
 * connection, SL_STATUS_IN_PROGRESS if the connection is ongoing, and
 * other error codes for various failure conditions.
 ******************************************************************************/
sl_status_t sl_cpc_connect_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle, uint8_t flags);
#endif

#if defined(SL_CATALOG_CPC_SECONDARY_PRESENT) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function is used to open a user endpoint for communication,
 * allowing data to be sent and received through the specified endpoint.
 * It should be called when a new communication channel is needed, and it
 * requires a valid endpoint handle and endpoint ID. The function
 * supports optional flags for endpoint configuration, such as disabling
 * encryption. The transmission window size must be specified, although
 * currently only a size of 1 is supported. The function returns a status
 * indicating success or the type of error encountered.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * that will be initialized to represent the opened
 * endpoint. Must not be null, and the caller retains
 * ownership.
 * @param id An sl_cpc_user_endpoint_id_t value representing the endpoint ID,
 * which must be between SL_CPC_USER_ENDPOINT_ID_START (90) and
 * SL_CPC_USER_ENDPOINT_ID_END (99). Invalid IDs will result in an
 * assertion failure.
 * @param flags A uint8_t value representing endpoint configuration flags. Valid
 * flags include SL_CPC_OPEN_ENDPOINT_FLAG_NONE for default
 * behavior and SL_CPC_OPEN_ENDPOINT_FLAG_DISABLE_ENCRYPTION to
 * disable encryption.
 * @param tx_window_size A uint8_t value specifying the maximum number of
 * packets that can be sent before waiting for an
 * acknowledgment. Currently, only a value of 1 is
 * supported.
 * @return Returns an sl_status_t indicating whether the endpoint was opened
 * successfully or if an error occurred.
 ******************************************************************************/
sl_status_t sl_cpc_open_user_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle,
                                      sl_cpc_user_endpoint_id_t id,
                                      uint8_t flags,
                                      uint8_t tx_window_size);

/***************************************************************************//**
 * @brief This function is used to set an endpoint into a listening state,
 * allowing it to wait for a connection from a remote endpoint. It is
 * typically called after initializing an endpoint and is essential for
 * establishing communication in a CPC (Co-Processor Communication)
 * setup. The function can operate in blocking or non-blocking mode,
 * depending on the flags provided. In a real-time operating system
 * (RTOS) environment, the function will block until a connection is
 * established unless the SL_CPC_FLAG_NO_BLOCK flag is set. It is
 * important to ensure that the endpoint is in a closed state before
 * calling this function, as it will return an error if the endpoint is
 * already open or in an invalid state.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * representing the endpoint to be put in listening mode.
 * Must not be null and should point to a valid,
 * initialized endpoint handle.
 * @param flags A set of flags that modify the behavior of the function. The
 * SL_CPC_FLAG_NO_BLOCK flag can be used to make the function non-
 * blocking in an RTOS environment. Other values are reserved for
 * future use.
 * @return Returns an sl_status_t value indicating the result of the operation.
 * Possible return values include SL_STATUS_OK if the remote
 * successfully connects, SL_STATUS_IN_PROGRESS if waiting for a
 * connection, and other error codes if an error occurs.
 ******************************************************************************/
sl_status_t sl_cpc_listen_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle, uint8_t flags);

/***************************************************************************//**
 * @brief This function is used to close a CPC endpoint that was previously
 * opened using the legacy API. It should be called when the endpoint is
 * no longer needed, to release resources and prevent further data
 * transmission or reception. The function checks if the endpoint is
 * eligible for closure and handles cases where the endpoint is busy or
 * already closed. It is important to ensure that the endpoint handle is
 * valid and that the endpoint was initialized with the legacy API before
 * calling this function.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * representing the endpoint to be closed. The endpoint
 * must have been initialized with the legacy API. The
 * pointer must not be null, and the function will return
 * an error if the endpoint is busy or not supported for
 * closure.
 * @return Returns an sl_status_t value indicating the result of the operation:
 * SL_STATUS_OK if successful, SL_STATUS_NOT_SUPPORTED if the endpoint
 * was not initialized with the legacy API, SL_STATUS_BUSY if the
 * endpoint is currently in use, or SL_STATUS_FAIL if the endpoint is
 * already closed or closing.
 ******************************************************************************/
sl_status_t sl_cpc_close_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle);
#endif

/***************************************************************************//**
 * @brief This function allows you to configure various options for a given CPC
 * endpoint by specifying the option type and its corresponding value. It
 * is essential to ensure that the endpoint handle is valid and not null
 * before calling this function. The function can be used to set
 * callbacks for data reception, write completion, error handling, and
 * connection events, among others. It is important to note that some
 * options are only available when certain features are enabled, such as
 * RTOS-specific options. The function returns a status code indicating
 * the success or failure of the operation, and it handles invalid
 * parameters by returning an appropriate error status.
 *
 * @param endpoint_handle A pointer to the CPC endpoint handle. Must not be
 * null. The caller retains ownership.
 * @param option An enumeration value of type sl_cpc_endpoint_option_t
 * specifying the option to set. Must be a valid option.
 * @param value A pointer to the value to set for the specified option. The type
 * and validity of the value depend on the option being set.
 * @return Returns an sl_status_t code indicating the result of the operation,
 * such as SL_STATUS_OK for success or an error code for failure.
 ******************************************************************************/
sl_status_t sl_cpc_set_endpoint_option(sl_cpc_endpoint_handle_t *endpoint_handle,
                                       sl_cpc_endpoint_option_t option,
                                       void *value);

/***************************************************************************//**
 * @brief This function is used to initiate the shutdown of an endpoint
 * connection, ensuring that any pending transmission frames are handled
 * appropriately. It should be called when the endpoint is actively
 * connected and needs to be disconnected. In an RTOS environment, the
 * function is blocking by default, but can be made non-blocking by using
 * the SL_CPC_FLAG_NO_BLOCK flag. The function returns various status
 * codes to indicate the success or failure of the shutdown process,
 * including whether the operation is still in progress or if it has been
 * aborted due to an error.
 *
 * @param endpoint_handle A pointer to the endpoint handle that identifies the
 * endpoint to be shut down. Must not be null and should
 * point to a valid, initialized endpoint handle.
 * @param flags Optional flags to modify the function's behavior. The
 * SL_CPC_FLAG_NO_BLOCK flag can be used to make the function non-
 * blocking in an RTOS environment. Other values are reserved for
 * future use.
 * @return Returns an sl_status_t value indicating the result of the shutdown
 * operation, such as SL_STATUS_OK for success, SL_STATUS_IN_PROGRESS if
 * the operation is ongoing, or other error codes if a failure occurs.
 ******************************************************************************/
sl_status_t sl_cpc_shutdown_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle,
                                     uint8_t flags);

/***************************************************************************//**
 * @brief This function is used to terminate a specified endpoint, effectively
 * dropping any pending transmission and reception frames. It should be
 * called when an endpoint needs to be disconnected and before attempting
 * to reconnect it to a remote. The function may block until all frames
 * have been dropped, especially in an RTOS environment. It is important
 * to ensure that the endpoint is not in a legacy API mode, as this is
 * not supported. The function returns different status codes to indicate
 * the progress or completion of the termination process.
 *
 * @param endpoint_handle A pointer to the endpoint handle that identifies the
 * endpoint to be terminated. Must not be null, and the
 * endpoint must be valid and initialized.
 * @param flags Termination flags reserved for future use. Currently, no
 * specific flags are required or supported.
 * @return Returns an sl_status_t value indicating the result of the operation:
 * SL_STATUS_OK if the endpoint was successfully terminated,
 * SL_STATUS_IN_PROGRESS if termination is ongoing, or other error codes
 * if an issue occurred.
 ******************************************************************************/
sl_status_t sl_cpc_terminate_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle,
                                      uint8_t flags);

/***************************************************************************//**
 * @brief This function is used to free the resources associated with a CPC
 * endpoint, allowing the endpoint to be reused. It should be called only
 * after the endpoint has been terminated. The function checks if the
 * endpoint is in a closed state and if the reference count is
 * appropriate before proceeding. If the endpoint is not in a valid state
 * or if the handle is invalid, the function returns an error status.
 * This function must be called in a context where it is safe to modify
 * the endpoint's state.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * representing the endpoint to be freed. The endpoint
 * must be in a closed state, and the handle must not be
 * null. If the handle is invalid or the endpoint is not
 * in the correct state, the function returns an error.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. Possible return values include SL_STATUS_OK for success,
 * SL_STATUS_INVALID_HANDLE if the handle is invalid,
 * SL_STATUS_INVALID_STATE if the endpoint is not in a closed state, and
 * SL_STATUS_BUSY if the endpoint is currently in use.
 ******************************************************************************/
sl_status_t sl_cpc_free_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle);

/***************************************************************************/ /**
 * Abort read; Allow unblocking task in blocked by a read.
 *
 * @note This function can be called from an ISR.
 *
 * @param[in] endpoint_handle   Endpoint handle.
 *
 * @retval  SL_STATUS_OK    Successfully aborted read operation.
 * @retval  Other sl_status_t if error occurred.
 ******************************************************************************/
#if defined(SL_CATALOG_KERNEL_PRESENT) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function is used to abort an ongoing read operation on a
 * specified CPC endpoint, allowing tasks that are blocked by the read to
 * be unblocked. It is particularly useful in scenarios where a read
 * operation needs to be terminated prematurely, such as when the data is
 * no longer needed or when the system is shutting down. The function
 * must be called when the operating system kernel is running, as it will
 * return an error if the kernel is not in the running state. It can be
 * safely called from an interrupt service routine (ISR).
 *
 * @param endpoint_handle A pointer to the CPC endpoint handle on which the read
 * operation is to be aborted. The pointer must not be
 * null, and the endpoint must be valid and initialized.
 * If the handle is invalid, the function will return an
 * error status.
 * @return Returns SL_STATUS_OK if the read operation was successfully aborted,
 * SL_STATUS_NONE_WAITING if there was no read operation to abort, or an
 * error status if the operation could not be completed.
 ******************************************************************************/
sl_status_t sl_cpc_abort_read(sl_cpc_endpoint_handle_t *endpoint_handle);
#endif

/***************************************************************************//**
 * @brief This function attempts to read data from a specified CPC endpoint,
 * potentially blocking until data is available or a timeout occurs. It
 * is used when data needs to be retrieved from an endpoint that has been
 * previously initialized and opened. The function can operate in
 * blocking or non-blocking mode, depending on the flags provided. It
 * requires a valid endpoint handle and will return an error if the
 * endpoint is not in a readable state or if other conditions prevent
 * reading. The function is designed to be used in environments with an
 * RTOS, where it can block and wait for data, or return immediately if
 * no data is available when non-blocking mode is specified.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * representing the endpoint from which to read. Must not
 * be null and should point to a valid, initialized
 * endpoint.
 * @param data A pointer to a void pointer where the address of the received
 * data will be stored. Must not be null.
 * @param data_length A pointer to a uint16_t where the length of the received
 * data will be stored. Must not be null.
 * @param timeout The maximum time to wait for data in ticks. If zero, the
 * function will wait indefinitely. Only applicable in blocking
 * mode.
 * @param flags Optional flags to modify the behavior of the function.
 * SL_CPC_FLAG_NO_BLOCK can be used to make the function non-
 * blocking.
 * @return Returns an sl_status_t indicating the result of the read operation,
 * such as SL_STATUS_OK for success or an error code for failure.
 ******************************************************************************/
sl_status_t sl_cpc_read(sl_cpc_endpoint_handle_t *endpoint_handle,
                        void **data,
                        uint16_t *data_length,
                        uint32_t timeout,
                        uint8_t flags);

/***************************************************************************//**
 * @brief This function is used to send data through a specified CPC endpoint.
 * It requires a valid endpoint handle and a non-null data buffer with a
 * positive length. The function supports optional flags to modify its
 * behavior, such as non-blocking operation. It is important to ensure
 * that the endpoint is properly initialized and connected before calling
 * this function. The function cannot be called from an interrupt service
 * routine (ISR). If the data buffer is encrypted, its original content
 * will be replaced by the encrypted data. The function returns a status
 * code indicating the success or failure of the operation.
 *
 * @param endpoint_handle A pointer to an initialized sl_cpc_endpoint_handle_t
 * structure representing the endpoint to which data will
 * be written. Must not be null.
 * @param data A pointer to the data buffer to be sent. Must not be null.
 * @param data_length The length of the data in the buffer. Must be greater than
 * zero.
 * @param flags Optional flags to modify the function's behavior, such as
 * SL_CPC_FLAG_NO_BLOCK for non-blocking operation.
 * @param on_write_completed_arg A user-defined argument that will be passed to
 * the on_write_completed callback function.
 * @return Returns an sl_status_t value indicating the result of the write
 * operation, such as SL_STATUS_OK for success or an error code for
 * failure.
 ******************************************************************************/
sl_status_t sl_cpc_write(sl_cpc_endpoint_handle_t *endpoint_handle,
                         void* data,
                         uint16_t data_length,
                         uint8_t flags,
                         void *on_write_completed_arg);

/***************************************************************************//**
 * @brief This function is used to obtain the current state of a CPC endpoint
 * identified by the provided endpoint handle. It is useful for checking
 * the status of an endpoint, such as whether it is open, closed, or in
 * an error state. The function must be called with a valid endpoint
 * handle, and it will return the state of the endpoint. If the endpoint
 * is not found or has been freed, the function will return
 * `SL_CPC_STATE_FREED`. This function is typically used in scenarios
 * where the application needs to make decisions based on the endpoint's
 * state.
 *
 * @param endpoint_handle A pointer to an `sl_cpc_endpoint_handle_t` structure
 * representing the endpoint. This parameter must not be
 * null, and the handle should be valid and initialized.
 * If the handle is invalid or the endpoint is not found,
 * the function will return `SL_CPC_STATE_FREED`.
 * @return The function returns an `sl_cpc_endpoint_state_t` value representing
 * the current state of the endpoint. Possible states include
 * `SL_CPC_STATE_OPEN`, `SL_CPC_STATE_CLOSED`, `SL_CPC_STATE_FREED`,
 * among others.
 ******************************************************************************/
sl_cpc_endpoint_state_t sl_cpc_get_endpoint_state(sl_cpc_endpoint_handle_t *endpoint_handle);

/***************************************************************************//**
 * @brief This function checks whether encryption is enabled for a given CPC
 * endpoint. It is useful for determining the security status of
 * communications over the endpoint. The function should be called with a
 * valid endpoint handle, which must have been previously initialized. If
 * the endpoint handle is invalid or the endpoint cannot be found, the
 * function will return false, indicating that encryption is not enabled.
 * This function is only effective if endpoint security is enabled in the
 * build configuration.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * representing the endpoint. The handle must be valid
 * and initialized. If the handle is null or the endpoint
 * cannot be found, the function will return false.
 * @return A boolean value indicating the encryption state of the endpoint: true
 * if encryption is enabled, false otherwise.
 ******************************************************************************/
bool sl_cpc_get_endpoint_encryption(sl_cpc_endpoint_handle_t *endpoint_handle);

/***************************************************************************//**
 * @brief Use this function to release a receive buffer that was previously
 * allocated by the CPC system. It is essential to call this function
 * after processing the data to prevent memory leaks. Ensure that the
 * data pointer is not null before calling this function, as passing a
 * null pointer will result in an error status.
 *
 * @param data Pointer to the data buffer to be freed. Must not be null. If
 * null, the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on successful buffer release, or
 * SL_STATUS_NULL_POINTER if the data pointer is null.
 ******************************************************************************/
sl_status_t sl_cpc_free_rx_buffer(void *data);

/***************************************************************************//**
 * @brief This function is used to determine the maximum payload size that can
 * be transmitted to a remote endpoint, taking into account whether
 * encryption is enabled. It should be called when you need to know the
 * maximum data size that can be sent in a single transmission to ensure
 * compatibility with the remote endpoint's capabilities. The function
 * requires a valid endpoint handle and will return a default maximum
 * length if the endpoint is not properly initialized or if the remote
 * endpoint's maximum payload length is not set.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * representing the endpoint. This parameter must not be
 * null, and the endpoint should be properly initialized.
 * If the endpoint is not initialized, the function will
 * return a default maximum payload length.
 * @return The function returns a uint16_t value representing the maximum
 * payload length that can be transmitted to the remote endpoint. This
 * value accounts for encryption overhead if encryption is enabled on
 * the endpoint.
 ******************************************************************************/
uint16_t sl_cpc_get_tx_max_payload_length(sl_cpc_endpoint_handle_t *endpoint_handle);

// -----------------------------------------------------------------------------
// Internal Prototypes only to be used by Power Manager module.
#if (!defined(SL_CATALOG_KERNEL_PRESENT) && defined(SL_CATALOG_POWER_MANAGER_PRESENT)) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function checks whether the system is in a state that allows it
 * to enter a low-power sleep mode. It should be called when the system
 * is considering entering sleep mode, typically as part of a power
 * management routine. The function returns true if there are no pending
 * receive operations and if the transmit queue is either empty or not
 * ready to transmit. This ensures that no critical communication tasks
 * are pending before the system goes to sleep.
 *
 * @return Returns a boolean value: true if it is safe to enter sleep mode,
 * false otherwise.
 ******************************************************************************/
bool sl_cpc_is_ok_to_sleep(void);
#endif

#if (!defined(SL_CATALOG_KERNEL_PRESENT) && defined(SL_CATALOG_POWER_MANAGER_PRESENT)) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function is used to decide whether the system should remain awake
 * or can ignore power management actions upon exiting an interrupt
 * service routine (ISR). It evaluates the current state to determine if
 * it is safe to enter a low-power mode. This function should be called
 * in contexts where power management decisions are needed after an ISR,
 * particularly in systems where power efficiency is critical. It assumes
 * that the necessary power management and CPC configurations are in
 * place.
 *
 * @return Returns SL_POWER_MANAGER_IGNORE if it is safe to sleep, otherwise
 * returns SL_POWER_MANAGER_WAKEUP.
 ******************************************************************************/
sl_power_manager_on_isr_exit_t sl_cpc_sleep_on_isr_exit(void);
#endif

/** @} (end addtogroup cpc) */

#ifdef __cplusplus
}
#endif

#endif // SL_CPC_H
