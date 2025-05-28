/***************************************************************************/ /**
 * @file
 * @brief CPC Types
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

#ifndef SLI_CPC_TYPE_H
#define SLI_CPC_TYPE_H

#include "sl_cpc.h"
#include "sl_slist.h"
#include "sl_cpc_config.h"
#include "sli_cpc_timer.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "cmsis_os2.h"
#include "sl_cmsis_os2_common.h"
#endif

#if defined(SL_CATALOG_CPC_SECONDARY_PRESENT)
#define SL_CPC_ON_POLL_PRESENT
#endif

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
#define SL_CPC_ON_FINAL_PRESENT
#endif

#if (!defined(SL_CATALOG_CPC_SECURITY_PRESENT))
#define SECURITY_ENABLED    (0)
#else
#if (SL_CPC_SECURITY_ENABLED >= 1)
#define SECURITY_ENABLED    (1)
#else
#define SECURITY_ENABLED    (0)
#endif
#endif

#if (!defined(SL_CATALOG_CPC_PRIMARY_PRESENT) && !defined(SL_CATALOG_CPC_SECONDARY_PRESENT))
// This is required for unit testing (assumed that we are unit testing on a secondary)
#define SL_CPC_ON_POLL_PRESENT
#endif

// Forward declaration of sli_cpc_instance_t. Including
// the header directly results in a circular dependency.
struct sli_cpc_instance;
/***************************************************************************//**
 * @brief The `sli_cpc_instance_t` is a forward declaration of a structure named
 * `sli_cpc_instance`. This type is used as a placeholder for a more
 * complex structure that is defined elsewhere, allowing for the creation
 * of pointers to this type without needing the full definition. This is
 * often used to manage dependencies and avoid circular includes in C
 * programming.
 *
 ******************************************************************************/
typedef struct sli_cpc_instance sli_cpc_instance_t;

typedef void (*sl_cpc_on_final_t)(uint8_t endpoint_id, void *arg, void *answer, uint32_t answer_lenght);

/***************************************************************************//**
 * @brief The `sl_cpc_poll_final_t` structure is designed to handle the final
 * callback mechanism in the CPC (Communication Protocol Controller)
 * framework. It contains a generic pointer `on_fnct_arg` for passing
 * arguments to the callback function, and conditionally includes a
 * function pointer `on_final` for the callback itself, which is only
 * present if the `SL_CPC_ON_FINAL_PRESENT` macro is defined. This
 * structure is used to manage and execute final operations in the CPC
 * communication process, ensuring that necessary actions are taken at
 * the end of a communication sequence.
 *
 * @param on_fnct_arg A pointer to an argument for the on function.
 * @param on_final A function pointer for the final callback, included only if
 * SL_CPC_ON_FINAL_PRESENT is defined.
 ******************************************************************************/
typedef struct {
  void *on_fnct_arg;                               ///< on fnct arg
#ifdef SL_CPC_ON_FINAL_PRESENT
  sl_cpc_on_final_t on_final;
#endif
} sl_cpc_poll_final_t;

/***************************************************************************//**
 * @brief The `sl_cpc_endpoint_t` structure is a comprehensive data structure
 * used in the Silicon Labs' CPC (Co-Processor Communication) framework
 * to manage communication endpoints. It encapsulates various fields
 * necessary for handling data transmission and reception, including
 * sequence and acknowledgment numbers, transmission window sizes, and
 * retransmission mechanisms. The structure also includes callback
 * functions for handling events such as data reception, write
 * completion, errors, and connection events. Additionally, it supports
 * features like security, with fields for encryption and frame counters,
 * and is designed to work with operating system features like mutexes
 * and semaphores for synchronization. This makes it a critical component
 * for managing reliable and efficient communication between processors.
 *
 * @param node A node in a singly linked list.
 * @param node_closing A node representing the closing state in a singly linked
 * list.
 * @param id Unique identifier for the endpoint.
 * @param flags Flags associated with the endpoint.
 * @param seq Sequence number for the endpoint.
 * @param ack Acknowledgment number for the endpoint.
 * @param configured_tx_window_size Configured transmission window size.
 * @param current_tx_window_space Current available space in the transmission
 * window.
 * @param uframe_tx_complete_pending Indicates if a uframe transmission
 * completion is pending.
 * @param frames_count_re_transmit_queue Count of frames in the retransmission
 * queue.
 * @param packet_re_transmit_count Count of packets retransmitted.
 * @param re_transmit_timeout Timeout value for retransmissions.
 * @param last_iframe_sent_timestamp Timestamp of the last iframe sent.
 * @param smoothed_rtt Smoothed round-trip time.
 * @param rtt_variation Variation in round-trip time.
 * @param re_transmit_timer Timer handle for retransmissions.
 * @param state Current state of the endpoint.
 * @param poll_final Structure for CPC poll final operations.
 * @param on_iframe_write_completed Callback for when an iframe write is
 * completed.
 * @param on_iframe_data_reception Callback for when iframe data is received.
 * @param on_iframe_data_reception_arg Argument for the iframe data reception
 * callback.
 * @param on_uframe_write_completed Callback for when a uframe write is
 * completed.
 * @param on_write_complete_pending_count Count of pending write complete
 * callbacks.
 * @param on_uframe_data_reception Callback for when uframe data is received.
 * @param on_uframe_data_reception_arg Argument for the uframe data reception
 * callback.
 * @param on_error Callback for error handling.
 * @param on_error_arg Argument for the error callback.
 * @param on_connect Callback for connection events.
 * @param on_connect_arg Argument for the connect callback.
 * @param iframe_receive_queue Queue for receiving iframes.
 * @param uframe_receive_queue Queue for receiving uframes.
 * @param re_transmit_queue Queue for retransmitting frames.
 * @param iframe_holding_list List holding iframes.
 * @param uframe_holding_list List holding uframes.
 * @param shutdown_timeout Timeout for shutdown in ticks.
 * @param write_timeout Timeout for write operations.
 * @param lock_cb Control block for mutex lock.
 * @param lock Mutex lock for synchronization.
 * @param receive_signal Semaphore for receive signal.
 * @param state_event_signal Semaphore indicating state change.
 * @param frame_counter_rx Counter for received frames.
 * @param frame_counter_tx Counter for transmitted frames.
 * @param encrypted Indicates if encryption is enabled.
 * @param packets_held_for_security Indicates if packets are held for security
 * reasons.
 * @param instance Pointer to the CPC instance.
 ******************************************************************************/
typedef struct  {
  sl_slist_node_t node;                                  ///< node
  sl_slist_node_t node_closing;                          ///< node closing
  uint8_t id;                                            ///< id
  uint8_t flags;                                         ///< flags
  uint8_t seq;                                           ///< seq
  uint8_t ack;                                           ///< ack
  uint8_t configured_tx_window_size;                     ///< configured tx window size
  uint8_t current_tx_window_space;                       ///< current tx window space
  bool uframe_tx_complete_pending;                       ///< uframe tx complete pending
  uint8_t frames_count_re_transmit_queue;                ///< frames count re transmit queue
  uint8_t packet_re_transmit_count;                      ///< packet re transmit count
  uint32_t re_transmit_timeout;                          ///< re transmit timeout
  uint64_t last_iframe_sent_timestamp;                   ///< last iframe sent timestamp
  uint64_t smoothed_rtt;                                 ///< smoothed rtt
  uint64_t rtt_variation;                                ///< rtt variation
  sli_cpc_timer_handle_t re_transmit_timer;              ///< re transmit timer
  sl_cpc_endpoint_state_t state;                         ///< state
  sl_cpc_poll_final_t poll_final;                        ///< poll final
  sl_cpc_on_write_completed_t on_iframe_write_completed; ///< on iframe write completed
  sl_cpc_on_data_reception_t on_iframe_data_reception;   ///< on iframe data reception
  void *on_iframe_data_reception_arg;                    ///< on iframe data reception arg
  sl_cpc_on_write_completed_t on_uframe_write_completed; ///< on uframe write completed
  uint16_t on_write_complete_pending_count;              ///< number of pending on_write_complete callbacks
  sl_cpc_on_data_reception_t on_uframe_data_reception;   ///< on uframe data reception
  void *on_uframe_data_reception_arg;                    ///< on uframe data reception arg
  sl_cpc_on_error_callback_t on_error;                   ///< on error
  void *on_error_arg;                                    ///< on error arg
  sl_cpc_on_connect_callback_t on_connect;               ///< on connect
  void *on_connect_arg;                                  ///< on connect arg
  sl_slist_node_t *iframe_receive_queue;                 ///< iframe receive queue
  sl_slist_node_t *uframe_receive_queue;                 ///< uframe receive queue
  sl_slist_node_t *re_transmit_queue;                    ///< re transmit queue
  sl_slist_node_t *iframe_holding_list;                  ///< iframe holding list
  sl_slist_node_t *uframe_holding_list;                  ///< uframe holding list
#if (SL_CPC_DEBUG_ENDPOINT_EVENT_COUNTERS == 1)
  sl_cpc_endpoint_debug_counters_t debug_counters;
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
  uint32_t shutdown_timeout;                             ///< shutdown timeout in ticks
  uint32_t write_timeout;                                ///< write timeout
  __ALIGNED(4) uint8_t lock_cb[osMutexCbSize];           ///< lock cb
  osMutexId_t lock;                                      ///< lock
  osSemaphoreId_t receive_signal;                        ///< receive signal
  osSemaphoreId_t state_event_signal;                    ///< endpoint state has changed
#endif
#if (SECURITY_ENABLED >= 1)
  uint32_t frame_counter_rx;
  uint32_t frame_counter_tx;
  bool encrypted;
  bool packets_held_for_security;
#endif

  sli_cpc_instance_t *instance;
} sl_cpc_endpoint_t;

#endif
