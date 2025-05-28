/***************************************************************************/ /**
 * @file
 * @brief CPC Internal Definitions
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

#ifndef SLI_CPC_H
#define SLI_CPC_H

#include <stddef.h>
#include <stdarg.h>

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#include "sl_status.h"
#include "sl_enum.h"
#include "sl_slist.h"
#include "sli_cpc_types.h"
#include "sli_cpc_timer.h"
#include "sl_cpc.h"
#include "sl_cpc_config.h"
#include "sli_cpc_system_common.h"
#if defined(SL_CATALOG_CPC_SECURITY_PRESENT)
#include "sl_cpc_security.h"
#include "sl_cpc_security_config.h"
#include "sli_cpc_security.h"
#endif

#if defined(SL_CATALOG_CPC_JOURNAL_PRESENT)
#include "sl_cpc_journal.h"
#else
#define SL_CPC_JOURNAL_RECORD_ERROR(string, value) ((void)(string), (void)(value))
#define SL_CPC_JOURNAL_RECORD_WARNING(string, value) ((void)(string), (void)(value))
#define SL_CPC_JOURNAL_RECORD_INFO(string, value) ((void)(string), (void)(value))
#define SL_CPC_JOURNAL_RECORD_DEBUG(string, value) ((void)(string), (void)(value))
#endif

#if !defined(SL_CATALOG_EMLIB_CORE_PRESENT) && !defined(SL_CATALOG_SL_CORE_PRESENT)
#include "mcu.h"
#else
#if defined(SL_CATALOG_EMLIB_CORE_PRESENT)
#include "em_core.h"
#elif defined(SL_CATALOG_SL_CORE_PRESENT)
#include "sl_core.h"
#endif
#include "sl_atomic.h"

#define MCU_DECLARE_IRQ_STATE   CORE_DECLARE_IRQ_STATE
#define MCU_ENTER_CRITICAL      CORE_ENTER_CRITICAL
#define MCU_EXIT_CRITICAL       CORE_EXIT_CRITICAL
#define MCU_ATOMIC_SECTION      CORE_ATOMIC_SECTION
#define MCU_ENTER_ATOMIC        CORE_ENTER_ATOMIC
#define MCU_EXIT_ATOMIC         CORE_EXIT_ATOMIC
#define MCU_ATOMIC_LOAD         sl_atomic_load
#define MCU_ATOMIC_STORE        sl_atomic_store
#endif

#define SLI_CPC_PROTOCOL_VERSION            (5)

#define SLI_CPC_ENDPOINT_SYSTEM             (1)

#if (!defined(SL_CATALOG_CPC_SECURITY_PRESENT))
#define SL_CPC_ENDPOINT_SECURITY_ENABLED    (0)
#else
#if (SL_CPC_SECURITY_ENABLED >= 1)
#define SL_CPC_ENDPOINT_SECURITY_ENABLED    (1)
#else
#define SL_CPC_ENDPOINT_SECURITY_ENABLED    (0)
#endif
#endif

#if defined(SL_CATALOG_BLUETOOTH_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_BLUETOOTH_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_BLUETOOTH_ENABLED  (0)
#endif

#if defined(SL_CATALOG_RAIL_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_RAIL_ENABLED (2)
#else
#define SLI_CPC_ENDPOINT_RAIL_ENABLED (0)
#endif

#if defined(SL_CATALOG_ZIGBEE_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_ZIGBEE_ENABLED (1)
#else
#define SLI_CPC_ENDPOINT_ZIGBEE_ENABLED (0)
#endif

#if defined(SL_CATALOG_ZWAVE_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_ZWAVE_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_ZWAVE_ENABLED  (0)
#endif

#if defined(SL_CATALOG_CONNECT_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_CONNECT_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_CONNECT_ENABLED  (0)
#endif

#if defined(SL_CATALOG_GPIO_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_GPIO_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_GPIO_ENABLED  (0)
#endif

#if defined(SL_CATALOG_OPENTHREAD_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_OPENTHREAD_ENABLED (1)
#else
#define SLI_CPC_ENDPOINT_OPENTHREAD_ENABLED (0)
#endif

#if defined(SL_CATALOG_WISUN_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_WISUN_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_WISUN_ENABLED  (0)
#endif

#if defined(SL_CATALOG_WIFI_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_WIFI_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_WIFI_ENABLED  (0)
#endif

#if defined(SL_CATALOG_15_4_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_15_4_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_15_4_ENABLED  (0)
#endif

#if defined(SL_CATALOG_CLI_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_CLI_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_CLI_ENABLED  (0)
#endif

#if defined(SL_CATALOG_CLI_NCP_PRESENT)
#define SLI_CPC_ENDPOINT_CLI_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_CLI_ENABLED  (0)
#endif

#if defined(SL_CATALOG_BLUETOOTH_RCP_PRESENT)
#define SLI_CPC_ENDPOINT_BLUETOOTH_RCP_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_BLUETOOTH_RCP_ENABLED  (0)
#endif

#if defined(SL_CATALOG_ACP_PRESENT)
#define SLI_CPC_ENDPOINT_ACP_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_ACP_ENABLED  (0)
#endif

#if defined(SL_CATALOG_SE_CPC_PRESENT)
#define SLI_CPC_ENDPOINT_SE_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_SE_ENABLED  (0)
#endif

#if defined(SL_CATALOG_CPC_NVM3_PRESENT)
#define SLI_CPC_ENDPOINT_NVM3_ENABLED  (1)
#else
#define SLI_CPC_ENDPOINT_NVM3_ENABLED  (0)
#endif

// Frame Flags
// I-Frame is enabled by default; this flag MUST be set to disable the I-Frame support by the endpoint
#define SL_CPC_ENDPOINT_FLAG_IFRAME_DISABLE             (0x01 << 0)
// U-Frame is disabled by default; this flag MUST be set to enable U-Frame support by the endpoint
#define SL_CPC_ENDPOINT_FLAG_UFRAME_ENABLE              (0x01 << 1)
// Disable non-poll/final U-Frame
#define SL_CPC_ENDPOINT_FLAG_UFRAME_INFORMATION_DISABLE (0x01 << 2)
// Disable encryption
// SL_CPC_ENDPOINT_FLAG_DISABLE_ENCRYPTION              (0x01 << 3) is defined in public header
// Endpoint was opened using legacy API
#define SL_CPC_ENDPOINT_FLAG_LEGACY_API                 (0x01 << 4)

// Legacy Flags
#define SL_CPC_OPEN_ENDPOINT_FLAG_IFRAME_DISABLE        SL_CPC_ENDPOINT_FLAG_IFRAME_DISABLE
#define SL_CPC_OPEN_ENDPOINT_FLAG_UFRAME_ENABLE         SL_CPC_ENDPOINT_FLAG_UFRAME_ENABLE
#define SL_CPC_OPEN_ENDPOINT_FLAG_UFRAME_INFORMATION_DISABLE SL_CPC_ENDPOINT_FLAG_UFRAME_INFORMATION_DISABLE
// SL_CPC_OPEN_ENDPOINT_FLAG_DISABLE_ENCRYPTION         is defined in public header

// SL_CPC_FLAG_NO_BLOCK = 0x01 << 0
#define SL_CPC_FLAG_UNNUMBERED_INFORMATION      (0x01 << 1)
#define SL_CPC_FLAG_UNNUMBERED_POLL_FINAL       (0x01 << 2)
#define SL_CPC_FLAG_UNNUMBERED_ACKNOWLEDGE      (0x01 << 3)
#define SL_CPC_FLAG_INFORMATION_POLL_FINAL      (0x01 << 4)
#define SL_CPC_FLAG_BLOCKING_WRITE              (0x01 << 5)

/// @brief Enumeration representing the possible id
SL_ENUM(sl_cpc_service_endpoint_id_t){
  SL_CPC_ENDPOINT_SYSTEM = 0,          ///< System control
#if (SL_CPC_ENDPOINT_SECURITY_ENABLED >= 1)
  SL_CPC_ENDPOINT_SECURITY = 1,        ///< Security - related functionality
#endif
#if (SLI_CPC_ENDPOINT_BLUETOOTH_ENABLED >= 1)
  SL_CPC_ENDPOINT_BLUETOOTH = 2,       ///< Bluetooth endpoint
#endif
#if (SLI_CPC_ENDPOINT_RAIL_ENABLED >= 1)
  SL_CPC_SLI_CPC_ENDPOINT_RAIL_DOWNSTREAM = 3, ///< RAIL downstream endpoint
  SL_CPC_SLI_CPC_ENDPOINT_RAIL_UPSTREAM = 4,   ///< RAIL upstream endpoint
#endif
#if (SLI_CPC_ENDPOINT_ZIGBEE_ENABLED >= 1)
  SL_CPC_ENDPOINT_ZIGBEE = 5,          ///< ZigBee EZSP endpoint
#endif
#if (SLI_CPC_ENDPOINT_ZWAVE_ENABLED >= 1)
  SL_CPC_ENDPOINT_ZWAVE = 6,           ///< Z-Wave endpoint
#endif
#if (SLI_CPC_ENDPOINT_CONNECT_ENABLED >= 1)
  SL_CPC_ENDPOINT_CONNECT = 7,         ///< Connect endpoint
  #endif
#if (SLI_CPC_ENDPOINT_GPIO_ENABLED >= 1)
  SL_CPC_ENDPOINT_GPIO = 8,            ///< GPIO endpoint for controlling GPIOs on SECONDARYs
#endif
#if (SLI_CPC_ENDPOINT_OPENTHREAD_ENABLED >= 1)
  SL_CPC_ENDPOINT_OPENTHREAD = 9,      ///< Openthread Spinel endpoint
#endif
#if (SLI_CPC_ENDPOINT_WISUN_ENABLED >= 1)
  SL_CPC_ENDPOINT_WISUN = 10,           ///< WiSun endpoint
#endif
#if (SLI_CPC_ENDPOINT_WIFI_ENABLED >= 1)
  SL_CPC_ENDPOINT_WIFI = 11,           ///< WiFi endpoint(main control)
#endif
#if (SLI_CPC_ENDPOINT_15_4_ENABLED >= 1)
  SL_CPC_ENDPOINT_15_4 = 12,           ///< 802.15.4 endpoint
#endif
#if (SLI_CPC_ENDPOINT_CLI_ENABLED >= 1)
  SL_CPC_ENDPOINT_CLI = 13,            ///< Ascii based CLI for stacks / applications
#endif
#if (SLI_CPC_ENDPOINT_BLUETOOTH_RCP_ENABLED >= 1)
  SL_CPC_ENDPOINT_BLUETOOTH_RCP = 14,  ///< Bluetooth RCP endpoint
#endif
#if (SLI_CPC_ENDPOINT_ACP_ENABLED >= 1)
  SL_CPC_ENDPOINT_ACP = 15,            ///< ACP endpoint
#endif
#if (SLI_CPC_ENDPOINT_SE_ENABLED >= 1)
  SL_CPC_ENDPOINT_SE = 16,             ///< Secure Engine endpoint
#endif
#if (SLI_CPC_ENDPOINT_NVM3_ENABLED >= 1)
  SL_CPC_ENDPOINT_NVM3 = 17,           ///< NVM3 endpoint
#endif
  SL_CPC_ENDPOINT_LAST_ID_MARKER,      // DO NOT USE THIS ENDPOINT ID
};

#define SLI_CPC_SERVICE_ENDPOINT_MAX_COUNT    (SLI_CPC_ENDPOINT_SYSTEM + SL_CPC_ENDPOINT_SECURITY_ENABLED + SLI_CPC_ENDPOINT_BLUETOOTH_ENABLED          \
                                               + SLI_CPC_ENDPOINT_RAIL_ENABLED + SLI_CPC_ENDPOINT_ZIGBEE_ENABLED + SLI_CPC_ENDPOINT_ZWAVE_ENABLED       \
                                               + SLI_CPC_ENDPOINT_CONNECT_ENABLED + SLI_CPC_ENDPOINT_GPIO_ENABLED + SLI_CPC_ENDPOINT_OPENTHREAD_ENABLED \
                                               + SLI_CPC_ENDPOINT_WISUN_ENABLED + SLI_CPC_ENDPOINT_WIFI_ENABLED + SLI_CPC_ENDPOINT_15_4_ENABLED         \
                                               + SLI_CPC_ENDPOINT_CLI_ENABLED + SLI_CPC_ENDPOINT_BLUETOOTH_RCP_ENABLED + SLI_CPC_ENDPOINT_ACP_ENABLED   \
                                               + SLI_CPC_ENDPOINT_SE_ENABLED + SLI_CPC_ENDPOINT_NVM3_ENABLED)

#define SLI_CPC_SERVICE_ENDPOINT_ID_START     ((uint8_t)SL_CPC_ENDPOINT_SYSTEM)
#define SLI_CPC_SERVICE_ENDPOINT_ID_END       ((uint8_t)SL_CPC_ENDPOINT_LAST_ID_MARKER - 1)

/***************************************************************************//**
 * The maximum size of a system endpoint command buffer.
 *
 * @note
 *   For the moment, this value must be manually set.
 *
 * @note : it is set to the size of the payload of a
 * CMD_PROPERTY_GET::PROP_ENDPOINT_STATES.
 *
 ******************************************************************************/
#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define SL_CPC_ENDPOINT_MAX_COUNT  256

#ifndef SLI_CPC_SYSTEM_COMMAND_BUFFER_COUNT
#define SLI_CPC_SYSTEM_COMMAND_BUFFER_COUNT 5
#endif

#ifndef SLI_CPC_SYSTEM_COMMAND_HANDLE_COUNT
#define SLI_CPC_SYSTEM_COMMAND_HANDLE_COUNT 2
#endif

// Length of the maximum CPC payload that can be received. This take into
// account: user payload + CRC (2B) [ + Security Tag (8B) ]
#if (SL_CPC_ENDPOINT_SECURITY_ENABLED >= 1)
  #define SLI_CPC_RX_DATA_MAX_LENGTH_CFG(_user_payload_max) (_user_payload_max \
                                                             + 2               \
                                                             + SLI_SECURITY_TAG_LENGTH_BYTES)
#else
  #define SLI_CPC_RX_DATA_MAX_LENGTH_CFG(_user_payload_max) (_user_payload_max \
                                                             + 2)
#endif

// Number of payload to allocate to send HDLC reject reasons
#define SLI_CPC_HDLC_REJECT_MAX_COUNT_CFG(_user_rx_buffer_max)         ((_user_rx_buffer_max) / 2 + 1)

// Maximum number of RX queue item. It is _user_rx_buffer_max - 1 to make sure
// there is one RX buffer available to receive an acknowledgement from the
// remote, otherwise the bus could end up deadlock
#define SLI_CPC_RX_QUEUE_ITEM_MAX_COUNT_CFG(_user_rx_buffer_max)       ((_user_rx_buffer_max) - 1)

// Maximum number of S-Frame, it matches the number of RX queue item to make
// sure they can be all acked
#define SLI_CPC_TX_QUEUE_ITEM_SFRAME_MAX_COUNT_CFG(_rx_queue_item_max) (_rx_queue_item_max)

// Maximum number of buffer handles. There must be enough for all TX queue
// item (user data), RX buffer and TX S-Frames, so it's just the sum of that.
#define SLI_CPC_BUFFER_HANDLE_MAX_COUNT_CFG(_user_tx_queue_item_max, _user_rx_buffer_max, _tx_queue_item_sframe_max) \
  (_user_tx_queue_item_max + _user_rx_buffer_max + _tx_queue_item_sframe_max)

// Maximum number of HDLC header. There is one header per buffer handle.
#define SLI_CPC_HDLC_HEADER_MAX_COUNT_CFG(_buffer_handle_max_cnt)      (_buffer_handle_max_cnt)

// Maximum frame size, ie. the maximum size for a full CPC frame on the bus
// (header + user payload)
#define SLI_CPC_RX_FRAME_MAX_LENGTH_CFG(_rx_data_max_length)           ((uint16_t)((_rx_data_max_length) + SLI_CPC_HDLC_HEADER_RAW_SIZE))

// define for the size that must be allocated to have an sli_cpc_rx_buffer_t
// with a data array of size "_rx_frame_max_length"
#define SLI_CPC_RX_BUFFER_MAX_LENGTH_CFG(_rx_frame_max_length)         ((_rx_frame_max_length) + sizeof(sli_cpc_rx_buffer_t))

// Signal count max = closing signal per endpoint + tx queue items + rx queue items
#define SLI_CPC_EVENT_SIGNAL_MAX_COUNT_CFG(_user_tx_queue_item_max, _rx_queue_item_max) \
  (SL_CPC_ENDPOINT_MAX_COUNT + _user_tx_queue_item_max + _rx_queue_item_max)

#define SLI_CPC_RE_TRANSMIT                    (10)
#define SLI_CPC_INIT_RE_TRANSMIT_TIMEOUT_MS    (100)
#define SLI_CPC_MAX_RE_TRANSMIT_TIMEOUT_MS     (5000)
#define SLI_CPC_MIN_RE_TRANSMIT_TIMEOUT_MS     (5)
#define SLI_CPC_DISCONNECTION_NOTIFICATION_TIMEOUT_MS  (1000)
#define SLI_CPC_MIN_RE_TRANSMIT_TIMEOUT_MINIMUM_VARIATION_MS (5)
#define SLI_CPC_SYSTEM_CMD_TIMEOUT_MS           (10000)

#define SLI_CPC_TEMPORARY_ENDPOINT_ID_START   100
#define SLI_CPC_TEMPORARY_ENDPOINT_ID_END     255
#define SLI_CPC_ENDPOINT_TEMPORARY_MAX_COUNT  (SLI_CPC_TEMPORARY_ENDPOINT_ID_END - SLI_CPC_TEMPORARY_ENDPOINT_ID_START + 1)

#define SLI_CPC_ENDPOINT_MIN_COUNT  (SLI_CPC_ENDPOINT_SYSTEM + SL_CPC_ENDPOINT_SECURITY_ENABLED)

SL_ENUM(sl_cpc_signal_type_t) {
  SL_CPC_SIGNAL_RX,
  SL_CPC_SIGNAL_TX,
  SL_CPC_SIGNAL_CLOSED,
  SL_CPC_SIGNAL_SYSTEM,
};

SL_ENUM(sl_cpc_reject_reason_t){
  SL_CPC_REJECT_NO_ERROR = 0,
  SL_CPC_REJECT_CHECKSUM_MISMATCH,
  SL_CPC_REJECT_SEQUENCE_MISMATCH,
  SL_CPC_REJECT_OUT_OF_MEMORY,
  SL_CPC_REJECT_SECURITY_ISSUE,
  SL_CPC_REJECT_UNREACHABLE_ENDPOINT,
  SL_CPC_REJECT_ERROR
};

SL_ENUM(sl_cpc_buffer_handle_type_t) {
  SL_CPC_UNKNOWN_BUFFER_HANDLE,
  SL_CPC_RX_INTERNAL_BUFFER_HANDLE,
  SL_CPC_RX_USER_BUFFER_HANDLE,
  SL_CPC_TX_REJECT_BUFFER_HANDLE,
  SL_CPC_TX_DATA_BUFFER_HANDLE,
  SL_CPC_TX_SFRAME_BUFFER_HANDLE,
};

typedef void (*sl_cpc_dispatcher_fnct_t)(void *data);

/***************************************************************************//**
 * @brief The `sl_cpc_dispatcher_handle_t` structure is used within the CPC (Co-
 * Processor Communication) framework to manage dispatching functions
 * along with their associated data. It contains a linked list node for
 * queue management, a function pointer for the dispatcher function, a
 * pointer to the CPC instance, a generic data pointer, and a boolean
 * flag to indicate if the handle has been submitted for processing. This
 * structure facilitates the organization and execution of tasks within
 * the CPC system.
 *
 * @param node A node in a singly linked list.
 * @param fnct A function pointer for the dispatcher function.
 * @param instance A pointer to the CPC instance associated with this handle.
 * @param data A generic pointer to data associated with the dispatcher handle.
 * @param submitted A boolean indicating if the handle has been submitted for
 * processing.
 ******************************************************************************/
typedef struct {
  sl_slist_node_t node;                            ///< node
  sl_cpc_dispatcher_fnct_t fnct;                   ///< fnct
  sli_cpc_instance_t *instance;                    ///< instance
  void *data;                                      ///< data
  bool submitted;                                  ///< submitted
} sl_cpc_dispatcher_handle_t;

/***************************************************************************//**
 * @brief The `sl_cpc_buffer_handle_t` structure is a comprehensive data
 * structure used in the Silicon Labs CPC (Co-Processor Communication)
 * framework to manage buffer handles. It encapsulates various fields
 * necessary for handling data transmission and reception, including
 * pointers to data and headers, control and address bytes, and status
 * indicators. The structure also supports security features, reference
 * counting, and is linked to specific endpoints and instances, making it
 * integral to the CPC's data handling and communication processes.
 *
 * @param hdlc_header Pointer to the HDLC header.
 * @param data Pointer to the data buffer.
 * @param security_tag Pointer to the security tag, included if security is
 * enabled.
 * @param data_length Length of the data buffer.
 * @param fcs Frame Check Sequence for error checking.
 * @param control Control byte for the buffer.
 * @param address Address byte for the buffer.
 * @param ref_count Reference count for the buffer.
 * @param type Type of the buffer handle.
 * @param endpoint Pointer to the associated endpoint.
 * @param instance Pointer to the CPC instance.
 * @param reason Reason for rejection, if applicable.
 * @param arg Generic argument pointer.
 * @param on_write_complete_pending Flag indicating if write completion is
 * pending.
 * @param write_status Status of the write operation.
 * @param driver_node Node for the driver list.
 * @param core_node Node for the core list.
 ******************************************************************************/
typedef struct {
  uint8_t *hdlc_header;                            ///< hdlc header
  uint8_t *data;                                   ///< data
#if (SL_CPC_ENDPOINT_SECURITY_ENABLED >= 1)
  void *security_tag;
#endif
  uint16_t data_length;                            ///< data_length
  uint16_t fcs;                                    ///< fcs
  uint8_t control;                                 ///< control
  uint8_t address;                                 ///< address
  uint8_t ref_count;                               ///< ref count
  sl_cpc_buffer_handle_type_t type;                ///< buffer handle type
  sl_cpc_endpoint_t *endpoint;                     ///< endpoint
  sli_cpc_instance_t *instance;                    ///< instance
  sl_cpc_reject_reason_t reason;                   ///< reason
  void *arg;                                       ///< arg
  bool on_write_complete_pending;                  ///< on write complete pending
  sl_status_t write_status;                        ///< write status
  sl_slist_node_t driver_node;                     ///< driver node
  sl_slist_node_t core_node;                       ///< core node
} sl_cpc_buffer_handle_t;

/***************************************************************************//**
 * @brief The `sl_cpc_receive_queue_item_t` structure is used to represent an
 * item in a receive queue within the CPC (Co-Processor Communication)
 * framework. It contains a linked list node for queue management, a
 * pointer to the data being received, the length of this data, and a
 * type indicating the buffer handle associated with the data. This
 * structure is essential for managing data reception in a CPC system,
 * allowing for efficient queuing and processing of incoming data
 * packets.
 *
 * @param node A linked list node used to link items in the queue.
 * @param data A pointer to the data associated with this queue item.
 * @param data_length The length of the data in bytes.
 * @param buffer_type The type of buffer handle associated with this queue item.
 ******************************************************************************/
typedef struct {
  sl_slist_node_t node;                            ///< node
  void *data;                                      ///< data
  uint16_t data_length;                            ///< data length
  sl_cpc_buffer_handle_type_t buffer_type;         ///< buffer handle type
} sl_cpc_receive_queue_item_t;

#ifdef __cplusplus
extern "C"
{
#endif
// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief Use this function to retrieve the instance associated with a specific
 * endpoint ID. It is essential for managing and interacting with
 * different communication endpoints in the system. The function should
 * be called with a valid endpoint ID, and it will return a pointer to
 * the corresponding instance. If the endpoint ID is invalid or not
 * supported, the function may return NULL. This function is particularly
 * useful in systems where multiple endpoints are managed, and specific
 * instances need to be accessed based on their IDs.
 *
 * @param ep_id The endpoint ID for which the instance is requested. It must be
 * a valid ID corresponding to a supported endpoint. If the ID is
 * 0, the function will assert and return NULL. For other invalid
 * IDs, the function will return a default instance pointer.
 * @return A pointer to the instance associated with the given endpoint ID, or
 * NULL if the ID is invalid or not supported.
 ******************************************************************************/
sli_cpc_instance_t* sli_cpc_get_instance(uint8_t ep_id);

/***************************************************************************//**
 * @brief This function is used to initialize a service endpoint for
 * communication within the Silicon Labs CPC framework. It should be
 * called when setting up a new endpoint to ensure it is properly
 * configured with the specified ID and flags. The function requires a
 * valid endpoint handle and a service endpoint ID within the allowed
 * range. Flags can be used to modify the endpoint's behavior, such as
 * disabling encryption or enabling U-frame support. The function returns
 * a status code indicating success or the type of error encountered,
 * such as invalid parameters or handle.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * where the initialized endpoint will be stored. Must
 * not be null.
 * @param id A sl_cpc_service_endpoint_id_t value representing the endpoint ID.
 * Must be within the range defined by
 * SLI_CPC_SERVICE_ENDPOINT_ID_START and
 * SLI_CPC_SERVICE_ENDPOINT_ID_END.
 * @param flags A uint8_t value representing initialization flags. Valid flags
 * include SL_CPC_ENDPOINT_FLAG_DISABLE_ENCRYPTION,
 * SL_CPC_ENDPOINT_FLAG_IFRAME_DISABLE,
 * SL_CPC_ENDPOINT_FLAG_UFRAME_ENABLE, and
 * SL_CPC_ENDPOINT_FLAG_UFRAME_INFORMATION_DISABLE. Invalid flags
 * result in an error.
 * @return Returns an sl_status_t code indicating success or the type of error
 * encountered, such as SL_STATUS_INVALID_HANDLE or
 * SL_STATUS_INVALID_PARAMETER.
 ******************************************************************************/
sl_status_t sli_cpc_init_service_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle,
                                          sl_cpc_service_endpoint_id_t id,
                                          uint8_t flags);

/***************************************************************************//**
 * @brief This function is used to allocate and initialize a temporary endpoint
 * within a specified range of IDs. It should be called when a temporary
 * communication channel is needed. The function assigns an available
 * endpoint ID within the predefined range and initializes the endpoint
 * with the provided flags. It is important to ensure that the
 * `endpoint_handle` and `id` pointers are not null before calling this
 * function. The function returns a status code indicating success or the
 * type of error encountered, such as invalid parameters or resource
 * exhaustion.
 *
 * @param endpoint_handle A pointer to an `sl_cpc_endpoint_handle_t` structure
 * where the endpoint handle will be stored. Must not be
 * null. The caller retains ownership.
 * @param id A pointer to a `uint8_t` where the assigned endpoint ID will be
 * stored. Must not be null. The caller retains ownership.
 * @param flags A `uint8_t` representing initialization flags for the endpoint.
 * The flags are reserved for future use and should be set
 * according to the API's requirements.
 * @return Returns an `sl_status_t` status code indicating the result of the
 * operation. Possible return values include success, invalid handle,
 * invalid parameter, or no more resources available.
 ******************************************************************************/
sl_status_t sli_cpc_init_temporary_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle,
                                            uint8_t *id,
                                            uint8_t flags);

/***************************************************************************//**
 * @brief This function is used to open a Silicon Labs Internal Service
 * endpoint, which is necessary for communication over the CPC (Co-
 * Processor Communication) framework. It requires a valid endpoint
 * handle and a service endpoint ID, which must be within the defined
 * range of service endpoint IDs. The function also accepts flags to
 * modify the behavior of the endpoint and a transmission window size to
 * control flow. It returns a status code indicating the success or
 * failure of the operation. This function should be called when setting
 * up communication channels between the host and the co-processor.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * that will be used to manage the endpoint. Must not be
 * null.
 * @param id An sl_cpc_service_endpoint_id_t value representing the endpoint ID.
 * Must be within the range of defined service endpoint IDs.
 * @param flags A uint8_t value representing endpoint flags that modify the
 * behavior of the endpoint. These flags can enable or disable
 * certain features like I-Frame or U-Frame support.
 * @param tx_window_size A uint8_t value specifying the transmission window size
 * for the endpoint, which controls the flow of data.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation.
 ******************************************************************************/
sl_status_t sli_cpc_open_service_endpoint (sl_cpc_endpoint_handle_t *endpoint_handle,
                                           sl_cpc_service_endpoint_id_t id,
                                           uint8_t flags,
                                           uint8_t tx_window_size);

/***************************************************************************//**
 * @brief This function is used to open a service endpoint specifically within a
 * given CPC instance, which is reserved for system endpoints and should
 * not be used outside of CPC. It requires a valid endpoint handle and
 * instance pointer, and the endpoint ID must be set to
 * SL_CPC_ENDPOINT_SYSTEM. The function also accepts flags for endpoint
 * configuration and a transmission window size. It is crucial to ensure
 * that the endpoint ID is correctly set to avoid assertion failures.
 *
 * @param endpoint_handle A pointer to an sl_cpc_endpoint_handle_t structure
 * that will be used to manage the endpoint. Must not be
 * null.
 * @param inst A pointer to an sli_cpc_instance_t structure representing the CPC
 * instance in which the endpoint will be opened. Must not be null.
 * @param id The endpoint ID, which must be SL_CPC_ENDPOINT_SYSTEM. Any other
 * value will result in an assertion failure.
 * @param flags A set of configuration flags for the endpoint. These flags
 * control various endpoint behaviors, such as enabling or
 * disabling certain frame types.
 * @param tx_window_size The size of the transmission window for the endpoint,
 * which determines how many frames can be sent before
 * requiring an acknowledgment.
 * @return Returns an sl_status_t code indicating the success or failure of the
 * operation.
 ******************************************************************************/
sl_status_t sli_cpc_open_service_endpoint_in_instance(sl_cpc_endpoint_handle_t *endpoint_handle,
                                                      sli_cpc_instance_t *inst,
                                                      sl_cpc_service_endpoint_id_t id,
                                                      uint8_t flags,
                                                      uint8_t tx_window_size);

/***************************************************************************//**
 * @brief This function is used to open a temporary endpoint for communication
 * within the CPC framework. It assigns a unique endpoint ID within a
 * predefined range and initializes the endpoint with specified flags and
 * transmission window size. This function should be called when a
 * temporary communication channel is needed, and it must be ensured that
 * the endpoint handle is valid and the ID pointer is not null. The
 * function handles resource allocation and returns appropriate status
 * codes if resources are unavailable or parameters are invalid.
 *
 * @param endpoint_handle A pointer to an endpoint handle structure where the
 * endpoint information will be stored. Must not be null.
 * @param id A pointer to a uint8_t where the assigned endpoint ID will be
 * stored. Must not be null.
 * @param flags A uint8_t representing initialization flags for the endpoint.
 * These flags control endpoint behavior, such as enabling or
 * disabling certain frame types.
 * @param tx_window_size A uint8_t specifying the transmission window size for
 * the endpoint. Determines how many frames can be sent
 * before requiring an acknowledgment.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. Possible return values include SL_STATUS_OK,
 * SL_STATUS_INVALID_PARAMETER, and SL_STATUS_NO_MORE_RESOURCE.
 ******************************************************************************/
sl_status_t sli_cpc_open_temporary_endpoint(sl_cpc_endpoint_handle_t *endpoint_handle,
                                            uint8_t *id,
                                            uint8_t flags,
                                            uint8_t tx_window_size);

/***************************************************************************//**
 * @brief This function is used to signal an event to a given CPC instance,
 * which may involve different actions depending on the presence of a
 * kernel. When a kernel is present, the function releases a semaphore
 * associated with the instance. Without a kernel, it increments a
 * process flag if the signal type is for receiving data. This function
 * should be called whenever an event needs to be signaled to ensure
 * proper handling by the CPC system.
 *
 * @param inst A pointer to the CPC instance to which the event is being
 * signaled. Must not be null.
 * @param signal_type The type of signal being sent, represented by the
 * enumeration sl_cpc_signal_type_t. Valid values are defined
 * by this enumeration.
 * @return None
 ******************************************************************************/
void sli_cpc_signal_event(sli_cpc_instance_t *inst, sl_cpc_signal_type_t signal_type);

/***************************************************************************//**
 * @brief This function sets up a CPC dispatcher handle by initializing its
 * fields and associating it with a specified CPC instance. It is
 * essential to call this function before using the handle in any
 * dispatcher operations. The function ensures that the handle is not
 * null and initializes its fields to default values, preparing it for
 * subsequent use in the CPC dispatching process.
 *
 * @param handle A pointer to a sl_cpc_dispatcher_handle_t structure that will
 * be initialized. Must not be null.
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance to associate with the handle. The caller retains
 * ownership.
 * @return None
 ******************************************************************************/
void sli_cpc_dispatcher_init_handle(sl_cpc_dispatcher_handle_t *handle,
                                    sli_cpc_instance_t *inst);

/***************************************************************************//**
 * @brief This function is used to enqueue a function along with its associated
 * data into the dispatch queue for later execution. It should be called
 * when you want to schedule a function to be executed asynchronously.
 * The function ensures that the handle is not already submitted before
 * enqueuing. If the handle is already submitted, the function returns a
 * busy status. This function is typically used in systems where tasks
 * need to be deferred and executed in a controlled manner.
 *
 * @param handle A pointer to a sl_cpc_dispatcher_handle_t structure that
 * represents the dispatch queue node. The handle must not be null
 * and should not be already submitted.
 * @param fnct A function pointer of type sl_cpc_dispatcher_fnct_t that points
 * to the function to be dispatched. This function will be called
 * with the provided data when dispatched.
 * @param data A pointer to the data that will be passed to the function when it
 * is dispatched. The caller retains ownership of the data.
 * @return Returns SL_STATUS_OK if the function and data are successfully pushed
 * onto the queue, or SL_STATUS_BUSY if the handle is already submitted.
 ******************************************************************************/
sl_status_t sli_cpc_dispatcher_push(sl_cpc_dispatcher_handle_t *handle,
                                    sl_cpc_dispatcher_fnct_t fnct,
                                    void *data);

/***************************************************************************//**
 * @brief This function is used to cancel a previously submitted dispatcher
 * handle, effectively removing it from the dispatch queue if it has been
 * submitted. It should be called when a dispatch operation is no longer
 * needed or should be aborted. The function must be called with a valid
 * handle that has been initialized and possibly submitted. It operates
 * atomically to ensure thread safety, and it will only perform the
 * cancellation if the handle is currently marked as submitted.
 *
 * @param handle A pointer to a sl_cpc_dispatcher_handle_t structure. The handle
 * must be valid and initialized. If the handle is not submitted,
 * the function will have no effect. The caller retains ownership
 * of the handle.
 * @return None
 ******************************************************************************/
void sli_cpc_dispatcher_cancel(sl_cpc_dispatcher_handle_t *handle);

/***************************************************************************//**
 * @brief This function processes all the functions registered in the dispatch
 * queue of the specified CPC instance. It should be called to handle and
 * execute the queued functions, ensuring that each function is called
 * with its associated data. The function must be called in a context
 * where the CPC instance is valid and properly initialized. It is
 * important to ensure that the dispatch queue is not modified
 * concurrently while this function is executing.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance whose dispatch queue is to be processed. Must not be
 * null. The caller retains ownership of the instance.
 * @return None
 ******************************************************************************/
void sli_cpc_dispatcher_process(sli_cpc_instance_t* inst);

/***************************************************************************//**
 * @brief This function is used to handle a connection attempt from a remote
 * endpoint to a specified endpoint within a CPC instance. It should be
 * called when a remote endpoint is attempting to connect, and it will
 * determine if the connection can be accepted based on the current state
 * of the endpoint. The function updates the reply state to indicate
 * whether the connection is accepted or not. It is important to ensure
 * that the instance and endpoint are properly initialized before calling
 * this function.
 *
 * @param inst A pointer to the CPC instance structure. Must not be null, and
 * the instance should be properly initialized before use.
 * @param endpoint_id The identifier of the endpoint that the remote is
 * attempting to connect to. It should be a valid endpoint ID
 * within the instance.
 * @param reply_state A pointer to a variable where the function will store the
 * state of the connection attempt. Must not be null.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * connection attempt. SL_STATUS_OK if the connection is accepted, or
 * SL_STATUS_INVALID_STATE if the connection cannot be accepted.
 ******************************************************************************/
sl_status_t sli_cpc_instance_on_remote_ep_connecting(sli_cpc_instance_t *inst,
                                                     uint8_t endpoint_id,
                                                     sl_cpc_endpoint_state_t *reply_state);

/***************************************************************************//**
 * @brief This function is used to notify the system that a remote endpoint has
 * successfully connected. It should be called when a connection
 * handshake with a remote endpoint is completed. The function requires a
 * valid CPC instance and an endpoint ID to identify the specific
 * endpoint that has connected. It is important to ensure that the
 * endpoint has not been terminated or freed before calling this
 * function, as it will simply ignore the command in such cases. This
 * function does not return any value and does not modify the input
 * parameters.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance. Must not be null.
 * @param endpoint_id An unsigned 8-bit integer representing the ID of the
 * endpoint that has connected. Must correspond to a valid
 * endpoint within the instance.
 * @param status An sl_status_t value indicating the status of the connection.
 * This status is used to inform the system of the connection
 * result.
 * @return None
 ******************************************************************************/
void sli_cpc_instance_on_remote_ep_connected(sli_cpc_instance_t *inst,
                                             uint8_t endpoint_id,
                                             sl_status_t status);

/***************************************************************************//**
 * @brief This function processes a shutdown request from a remote endpoint for
 * a specified instance and endpoint ID. It should be called when a
 * remote endpoint requests a shutdown, and it updates the endpoint's
 * state accordingly. The function handles various endpoint states, such
 * as already shutdown, shutting down, or connected, and sets the
 * appropriate reply state. It returns a status indicating the result of
 * the shutdown request handling. The function must be called with a
 * valid instance and endpoint ID, and the reply state pointer must not
 * be null.
 *
 * @param inst A pointer to the CPC instance associated with the endpoint. Must
 * not be null.
 * @param endpoint_id The ID of the endpoint for which the shutdown request is
 * being processed. Must be a valid endpoint ID.
 * @param reply_state A pointer to a variable where the function will store the
 * resulting state of the endpoint after processing the
 * shutdown request. Must not be null.
 * @return Returns an sl_status_t value indicating the result of the shutdown
 * request handling, such as SL_STATUS_OK, SL_STATUS_NOT_FOUND, or
 * SL_STATUS_IN_PROGRESS.
 ******************************************************************************/
sl_status_t sli_cpc_instance_on_remote_ep_shutdown(sli_cpc_instance_t *inst,
                                                   uint8_t endpoint_id,
                                                   sl_cpc_endpoint_state_t *reply_state);

/***************************************************************************//**
 * @brief This function is used to manage the disconnection process of a remote
 * endpoint within a CPC instance. It should be called when a remote
 * endpoint is known to have disconnected, either cleanly or due to an
 * error. The function ensures that the endpoint's state is updated
 * appropriately, transitioning to a disconnected state if the shutdown
 * handshake was completed successfully. It is important to call this
 * function only when the endpoint is expected to be in a shutdown or
 * error state, as it does not provide a callback for disconnection
 * events.
 *
 * @param inst A pointer to the CPC instance structure. Must not be null, as it
 * represents the instance containing the endpoint to be
 * disconnected.
 * @param endpoint_id The identifier of the endpoint to be disconnected. It
 * should be a valid endpoint ID within the instance.
 * @return None
 ******************************************************************************/
void sli_cpc_instance_on_remote_ep_disconnected(sli_cpc_instance_t *inst, uint8_t endpoint_id);

/***************************************************************************//**
 * @brief This function is used to manage the termination of a connection to a
 * remote endpoint within a CPC instance. It should be called when a
 * remote endpoint has been terminated, and it ensures that the local
 * endpoint state is updated accordingly. The function sets the reply
 * state to CLOSED, indicating that the endpoint is no longer active. It
 * handles different scenarios based on the current state of the
 * endpoint, such as notifying errors if the connection was active or
 * handling specific cases when the endpoint was in the CONNECTING state.
 * This function must be called with a valid CPC instance and is expected
 * to be used in environments where endpoint connections are managed
 * dynamically.
 *
 * @param inst A pointer to the CPC instance structure. Must not be null, as it
 * represents the context in which the endpoint operates.
 * @param endpoint_id The identifier of the endpoint that has been terminated.
 * It should be a valid endpoint ID within the range
 * supported by the CPC instance.
 * @param reply_state A pointer to a variable where the function will store the
 * state of the endpoint after processing the termination.
 * The caller must provide a valid pointer, and the function
 * will set it to SL_CPC_STATE_CLOSED.
 * @return None
 ******************************************************************************/
void sli_cpc_instance_on_remote_ep_terminated(sli_cpc_instance_t *inst,
                                              uint8_t endpoint_id,
                                              sl_cpc_endpoint_state_t *reply_state);

/***************************************************************************//**
 * @brief This function is intended to be called when a frame needs to be
 * retransmitted. It is a placeholder for handling retransmission logic,
 * which may involve updating internal states or notifying other
 * components of the retransmission event. This function does not perform
 * any operations on the provided frame and is safe to call with any
 * valid or invalid frame pointer. It is typically used in systems where
 * frame retransmission is a part of the communication protocol.
 *
 * @param frame A pointer to an sl_cpc_buffer_handle_t structure representing
 * the frame to be retransmitted. The pointer can be null or
 * invalid, as the function does not perform any operations on it.
 * The caller retains ownership of the frame.
 * @return None
 ******************************************************************************/
void sli_cpc_on_frame_retransmit(sl_cpc_buffer_handle_t* frame);

/***************************************************************************//**
 * @brief This function is intended to be called when a retransmit timer expires
 * for a CPC frame. It is typically used in communication protocols where
 * frames need to be retransmitted if an acknowledgment is not received
 * within a certain time frame. The function takes a pointer to a frame
 * handle, which represents the frame that was being retransmitted. This
 * function does not perform any operations on the frame, as indicated by
 * the void cast, and serves as a placeholder for potential future
 * functionality. It should be used in contexts where retransmission
 * logic is implemented and requires a callback for timer expiration
 * events.
 *
 * @param frame A pointer to an sl_cpc_buffer_handle_t structure representing
 * the frame associated with the expired retransmit timer. The
 * pointer must not be null, but the function does not use or
 * modify the frame.
 * @return None
 ******************************************************************************/
void sli_cpc_on_retransmit_timer_expiration(sl_cpc_buffer_handle_t *frame);

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
/***************************************************************************//**
 * @brief This function is used to handle unsolicited open requests from a
 * remote device for a specified endpoint identified by the given ID. It
 * should be called when such a request is detected. The function checks
 * if the endpoint is valid and, if so, attempts to transition the
 * endpoint to a connecting state by sending a connection request. If the
 * endpoint is already open, it will attempt to reconnect. If the
 * connection request fails, the endpoint is closed, and the failure is
 * notified. This function must be called in a context where endpoint
 * list access is properly synchronized.
 *
 * @param id The ID of the endpoint to be opened. Must be a valid endpoint ID.
 * If the endpoint is not found, the function will return immediately
 * without making any changes.
 * @return None
 ******************************************************************************/
void sli_cpc_remote_open_unsolicited(uint8_t id);

/***************************************************************************//**
 * @brief This function is used to process unsolicited shutdown requests from a
 * remote source for a specific endpoint identified by its ID. It should
 * be called when such a request is received to ensure the endpoint is
 * properly handled, either by transitioning it to a disconnected state
 * or by ignoring the request if the endpoint is not allocated. The
 * function ensures that any pending transmissions are completed before
 * the endpoint is shut down, and it manages the endpoint's state
 * accordingly. It is important to ensure that the endpoint ID provided
 * is valid and that the function is called in a context where endpoint
 * state changes are permissible.
 *
 * @param id The ID of the endpoint to be shut down. It must be a valid endpoint
 * ID. If the endpoint is not allocated, the request is ignored.
 * @return None
 ******************************************************************************/
void sli_cpc_remote_shutdown_unsolicited(uint8_t id);

/***************************************************************************//**
 * @brief This function is used to reset the CPC core, which involves setting
 * the core into a reset state and updating the states of all endpoints
 * managed by the core. It should be called when a reset of the CPC
 * system is required, such as during error recovery or reinitialization.
 * The function iterates over all endpoints, changing their states based
 * on their current status, and notifies them of connection failures or
 * errors as appropriate. It is important to ensure that the CPC system
 * is in a state where a reset is safe to perform, as this function will
 * alter the state of all endpoints.
 *
 * @return None
 ******************************************************************************/
void sli_cpc_reset(void);
#endif

#if (SL_CPC_ENDPOINT_SECURITY_ENABLED >= 1)
/***************************************************************************//**
 * @brief This function is used to set the security counters for a specified
 * endpoint, which is necessary for maintaining synchronization and
 * security in communication. It should be called when the endpoint is in
 * an open state to ensure the counters are correctly mapped. The
 * function returns a status indicating success or failure, which can
 * occur if the endpoint is not ready or in an invalid state.
 *
 * @param endpoint_id The identifier of the endpoint for which the security
 * counters are being set. It must correspond to an existing
 * endpoint; otherwise, the function will return an error.
 * @param primary_rx_counter The receive counter value to be set for the primary
 * endpoint. This value is mapped to the secondary's
 * transmit counter.
 * @param primary_tx_counter The transmit counter value to be set for the
 * primary endpoint. This value is mapped to the
 * secondary's receive counter.
 * @return Returns an sl_status_t indicating the result of the operation:
 * SL_STATUS_OK on success, SL_STATUS_NOT_READY if the endpoint is not
 * found, or SL_STATUS_INVALID_STATE if the endpoint is not open.
 ******************************************************************************/
sl_status_t sli_cpc_set_security_counters(uint8_t endpoint_id, uint32_t primary_rx_counter, uint32_t primary_tx_counter);
#endif

/***************************************************************************//**
 * @brief This function is used to configure the maximum payload length that the
 * remote endpoint can handle for transmissions. It should be called when
 * there is a need to update the remote's transmission capabilities,
 * typically during initialization or when the communication parameters
 * change. The function directly modifies the instance's configuration,
 * so it must be ensured that the instance is valid and properly
 * initialized before calling this function.
 *
 * @param inst A pointer to a `sli_cpc_instance_t` structure representing the
 * CPC instance. This pointer must not be null and should point to a
 * valid, initialized instance.
 * @param remote_tx_max_payload_length A 16-bit unsigned integer specifying the
 * maximum payload length that the remote
 * can receive. The value should be within
 * the valid range for payload sizes
 * supported by the system.
 * @return None
 ******************************************************************************/
void sli_cpc_set_remote_tx_max_payload_length(sli_cpc_instance_t *inst, uint16_t remote_tx_max_payload_length);
// -----------------------------------------------------------------------------
// Driver to core notifications

/***************************************************************************//**
 * @brief This function is used to signal the core that new data has been
 * received from the driver. It should be called whenever the driver
 * completes a data reception, allowing the core to process the received
 * data. This function is typically used in environments where the driver
 * operates asynchronously and needs to notify the core of completed
 * operations. It is important to ensure that the instance pointer
 * provided is valid and properly initialized before calling this
 * function.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance. This pointer must not be null and should be properly
 * initialized before use. The function does not take ownership of
 * this pointer.
 * @return None
 ******************************************************************************/
void sli_cpc_notify_rx_data_from_drv(sli_cpc_instance_t *inst);

/***************************************************************************//**
 * @brief This function should be called to inform the core that a data
 * transmission has been completed by the driver. It is typically used in
 * scenarios where the transmission status needs to be updated or further
 * actions are required based on the completion of the transmission. The
 * function must be called with a valid buffer handle that was used for
 * the transmission. It is important to ensure that the buffer handle is
 * not null and is correctly initialized before calling this function.
 *
 * @param buffer_handle A pointer to an sl_cpc_buffer_handle_t structure
 * representing the buffer that was transmitted. Must not
 * be null and should be properly initialized before use.
 * The function does not take ownership of the buffer
 * handle.
 * @return None
 ******************************************************************************/
void sli_cpc_notify_tx_data_by_drv(sl_cpc_buffer_handle_t *buffer_handle);

#if defined(SL_CATALOG_KERNEL_PRESENT)
/***************************************************************************//**
 * Notifies core of a freed transmit item
 ******************************************************************************/
void sli_cpc_notify_tx_item_freed(void);

/***************************************************************************//**
 * Notifies core of a freed buffer handle
 ******************************************************************************/
void sli_cpc_notify_buffer_handle_freed(void);

/***************************************************************************//**
 * @brief This function is responsible for managing the main task loop of a CPC
 * (Communication Protocol Controller) instance. It should be called
 * within a thread context, as it enters an infinite loop to handle
 * various CPC operations such as receiving and transmitting frames,
 * processing system commands, and managing endpoint states. The function
 * expects a valid CPC instance pointer as its argument, and it will
 * terminate the thread if the instance cannot be started successfully.
 * It is crucial to ensure that the CPC instance is properly initialized
 * before invoking this function.
 *
 * @param arg A pointer to a CPC instance (sli_cpc_instance_t*). Must not be
 * null. The function will terminate the thread if the instance
 * cannot be started.
 * @return None
 ******************************************************************************/
void sl_cpc_task(void *arg);
#endif

/** @} (end addtogroup cpc) */

#ifdef __cplusplus
}
#endif

#endif // SLI_CPC_H
