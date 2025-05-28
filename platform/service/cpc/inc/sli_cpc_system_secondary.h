/***************************************************************************/ /**
 * @file
 * @brief CPC system endpoint secondary part
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

#ifndef SLI_CPC_SYSTEM_SECONDARY_H
#define SLI_CPC_SYSTEM_SECONDARY_H

#include "sl_common.h"
#include "sl_compiler.h"
#include "sli_cpc_system_common.h"
#include "sli_cpc_timer.h"

/***************************************************************************//**
 * Each command buffer must be big enough to hold the metadata around the command
 * and the command itself (command_id, seq, length, and payload).  Considering
 * PAYLOAD_MAX_LENGTH is already accounted for in sizeof(system_cmd_context),
 * it must be substracted to avoid allocating extra space.
 ******************************************************************************/
#define SLI_CPC_SYSTEM_COMMAND_BUFFER_SIZE \
  (sizeof(sli_cpc_system_cmd_context_t)    \
   - PAYLOAD_LENGTH_MAX                    \
   + SLI_CPC_SYSTEM_COMMAND_PAYLOAD_LENGTH_MAX)

/***************************************************************************//**
 * @brief The `sli_cpc_system_endpoint_t` structure is a compound data type used
 * in the CPC (Co-Processor Communication) system to manage and store
 * state information related to a system endpoint. It includes various
 * fields such as a handle to the endpoint, version information, flags
 * for restart and reset sequence status, timing information for IRQ
 * handling, and a flow control validation value. This structure is
 * essential for maintaining the operational state and control flow of a
 * CPC system endpoint.
 *
 * @param handle A handle to the CPC endpoint.
 * @param primary_version Stores the primary version information of the CPC
 * system.
 * @param restart_flag Indicates whether a restart is required.
 * @param reset_sequence_done Indicates if the reset sequence has been
 * completed.
 * @param enter_irq_end_ms Stores the end time in milliseconds for entering an
 * IRQ.
 * @param enter_irq_timer A timer handle for managing IRQ entry timing.
 * @param process_uframes_flag Flag to indicate if unframed data should be
 * processed.
 * @param fc_validation_value Stores a flow control validation value.
 ******************************************************************************/
typedef struct sli_cpc_system_endpoint {
  sl_cpc_endpoint_handle_t handle;

  sli_cpc_system_primary_version_t primary_version;

  bool restart_flag;
  bool reset_sequence_done;

  uint32_t enter_irq_end_ms;
  sli_cpc_timer_handle_t enter_irq_timer;

  bool process_uframes_flag;

  uint32_t fc_validation_value;
} sli_cpc_system_endpoint_t;

/***************************************************************************//**
 * @brief The sli_cpc_system_cmd_context_t structure is used to encapsulate a
 * command and its associated context within the CPC system endpoint. It
 * includes a pointer to the system endpoint, an argument for completion
 * callbacks, an endpoint ID, and the command itself, which is aligned
 * for compatibility with certain system endpoint operations. This
 * structure is essential for managing command execution and completion
 * within the CPC system.
 *
 * @param ep A pointer to a sli_cpc_system_endpoint_t structure, representing
 * the system endpoint associated with the command.
 * @param on_complete_arg A void pointer to an argument that can be used when
 * the command completes.
 * @param ep_id An 8-bit unsigned integer representing the endpoint ID.
 * @param command A sli_cpc_system_cmd_t structure, aligned to 4 bytes,
 * representing the command to be sent.
 ******************************************************************************/
typedef struct {
  sli_cpc_system_endpoint_t         *ep;
  void                              *on_complete_arg;
  uint8_t                           ep_id;

  // due to its definition, sli_cpc_system_cmd_t alignment is two bytes,
  // but some casts in the system endpoint expects it to be word-aligned.
  __ALIGNED(4) sli_cpc_system_cmd_t command;
} sli_cpc_system_cmd_context_t;

SL_NORETURN void cpc_system_reset(sli_cpc_system_reboot_mode_t reboot_mode);

sl_cpc_system_status_t cpc_get_reset_reason(void);

void cpc_get_bootloader_info(sli_cpc_system_bootloader_info_t *infos);

/***************************************************************************//**
 * @brief This function processes a poll request received on a CPC system
 * endpoint, determining the appropriate response based on the command
 * type and frame type. It should be called when a frame with the
 * Poll/Final flag is set. The function validates the input parameters
 * and prepares a reply if the command is supported. It requires a valid
 * system endpoint and frame type, and the poll data must be correctly
 * formatted. The function may return an error status if the input
 * parameters are invalid or if memory allocation for the command context
 * fails.
 *
 * @param system_ep A pointer to a sli_cpc_system_endpoint_t structure
 * representing the system endpoint. Must not be null.
 * @param frame_type The type of frame received, which must be either
 * SYSTEM_EP_UFRAME or SYSTEM_EP_IFRAME. Invalid values result
 * in an error.
 * @param poll_data A pointer to the data received in the poll request. Must not
 * be null and must contain at least a command header.
 * @param poll_data_length The length of the poll data in bytes. Must be at
 * least the size of a command header.
 * @param reply_data A pointer to a location where the function will store the
 * address of the reply data. The caller must provide a valid
 * pointer.
 * @param reply_data_length A pointer to a location where the function will
 * store the length of the reply data. The caller must
 * provide a valid pointer.
 * @param context A pointer to a location where the function will store the
 * command context. The caller must provide a valid pointer.
 * @return Returns a status code indicating success or the type of error
 * encountered. If successful, reply_data and reply_data_length are
 * populated with the response information.
 ******************************************************************************/
sl_status_t sli_cpc_system_secondary_on_poll(sli_cpc_system_endpoint_t *system_ep,
                                             sli_cpc_system_ep_frame_type_t frame_type,
                                             void *poll_data,
                                             uint32_t poll_data_length, // Rx buffer is freed once this on_poll function return
                                             void **reply_data,
                                             uint32_t *reply_data_length,
                                             sli_cpc_system_cmd_context_t **context);

#endif
