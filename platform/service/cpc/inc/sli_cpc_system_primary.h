/***************************************************************************/ /**
 * @file
 * @brief Private CPC system endpoint primary. For internal use by other parts
 *        of CPC but not to be used by external users.
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

#ifndef SLI_CPC_SYSTEM_PRIMARY_H
#define SLI_CPC_SYSTEM_PRIMARY_H

#include "sl_common.h"
#include "sl_enum.h"
#include "sli_cpc_system_common.h"
#include "sli_cpc_timer.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SLI_CPC_SYSTEM_COMMAND_FLAG_IS_FIRST_TRY        (0x01 << 0)
#define SLI_CPC_SYSTEM_COMMAND_FLAG_BEING_TRANSMITTED   (0x01 << 1)
#define SLI_CPC_SYSTEM_COMMAND_FLAG_RECEIVED_REPLY      (0x01 << 2)

/***************************************************************************//**
 * @brief The `sli_cpc_system_command_flag_t` structure is a bit-field struct
 * used to manage and track the state of a command within the CPC system.
 * It contains three boolean flags: `is_first_try`, `being_transmitted`,
 * and `received_reply`, each occupying a single bit. These flags are
 * used to determine whether a command is on its first attempt, is
 * currently being transmitted, or has received a reply, respectively.
 * This structure is integral to the command handling process, providing
 * a compact and efficient way to manage command states.
 *
 * @param is_first_try Indicates if the command is being attempted for the first
 * time.
 * @param being_transmitted Indicates if the command is currently being
 * transmitted.
 * @param received_reply Indicates if a reply has been received for the command.
 ******************************************************************************/
typedef struct {
  bool is_first_try : 1;
  bool being_transmitted : 1;
  bool received_reply : 1;
} sli_cpc_system_command_flag_t;

/***************************************************************************//**
 * @brief The `sli_cpc_system_command_handle_t` structure is designed to manage
 * system commands within the CPC (Co-Processor Communication) framework.
 * It includes linked list nodes for organizing commands and retries, a
 * command object, and function pointers for handling command completion.
 * The structure also tracks retry attempts, frame type, and timeout
 * settings, while maintaining error status and timing information. Flags
 * are used to indicate the current state of the command, such as whether
 * it is the first attempt, being transmitted, or if a reply has been
 * received.
 *
 * @param node_commands A linked list node for managing command nodes.
 * @param node_retries A linked list node for managing retry nodes.
 * @param command Represents the system command to be handled.
 * @param on_final Pointer to a function to be called upon command completion.
 * @param on_final_arg Argument to be passed to the on_final function.
 * @param retry_count Number of retries attempted for the command.
 * @param frame_type Type of frame used for the command.
 * @param retry_timeout_tick Timeout value for retry attempts in ticks.
 * @param error_status Status indicating any error that occurred.
 * @param timer Timer handle for managing command timing.
 * @param flags Flags indicating the state of the command.
 ******************************************************************************/
typedef struct  {
  sl_slist_node_t node_commands;
  sl_slist_node_t node_retries;
  sli_cpc_system_cmd_t command;
  void *on_final;
  void *on_final_arg;
  uint8_t retry_count;
  sli_cpc_system_ep_frame_type_t frame_type;
  uint32_t retry_timeout_tick;
  sl_status_t error_status;
  sli_cpc_timer_handle_t timer;
  sli_cpc_system_command_flag_t flags;
} sli_cpc_system_command_handle_t;

/***************************************************************************//**
 * @brief The `sli_cpc_system_endpoint_t` is a placeholder structure used within
 * the CPC (Co-Processor Communication) system, specifically for the
 * primary endpoint. It currently contains a single dummy member to
 * satisfy compiler requirements for non-empty structures, with the
 * intention of being expanded with meaningful data fields in future
 * development. This struct is part of the internal implementation and is
 * not intended for external use.
 *
 * @param dummy A placeholder member to ensure the struct is not empty, intended
 * to be replaced by actual variables later.
 ******************************************************************************/
typedef struct sli_cpc_system_endpoint {
  // IAR is not happy when the struct is empty, so for now just add a dummy
  // element in the struct in order to compile. Later on, that will be replaced
  // by actual primary's variables.
  sl_cpc_endpoint_handle_t dummy;
} sli_cpc_system_endpoint_t;

/***************************************************************************//**
 * Callback for the no-op command.
 *
 * @brief
 *   This callback is called when the PRIMARY receives the reply from the SECONDARY.
 ******************************************************************************/
typedef void (*sli_cpc_system_noop_cmd_callback_t)(sl_status_t status);

/***************************************************************************//**
 * Callback for the reset command.
 *
 * @brief
 *   This callback is called when the PRIMARY receives the reply from the SECONDARY.
 *
 * @param
 *   [in] status
 *     The SECONDARY will return STATUS_OK if the reset will occur in the
 *     desired mode. STATUS_FAILURE will be returned otherwise.
 ******************************************************************************/
typedef void (*sli_cpc_system_reset_cmd_callback_t)(sl_status_t command_status,
                                                    sl_cpc_system_status_t reset_status);

/***************************************************************************//**
 * Callback for the property-get or set command.
 *
 * @param
 *   [in] property_id
 *     The ID of the property from the previously issued property-get/set.
 *
 *   [in] property_value
 *     A pointer to the value returned by the SECONDARY. Has to be casted to an
 *     appropriate value in function of the property ID.
 *
 *   [in] property_length
 *     The length of the property value in bytes.
 *
 *   [in] reply_arg
 *     Value passed as `on_property_set_reply_arg` or `on_property_get_reply_arg`
 *     in `sli_cpc_system_cmd_property_set` or `sli_cpc_system_cmd_property_get`
 *     respectively.
 ******************************************************************************/
typedef void (*sli_cpc_system_property_get_set_cmd_callback_t)(sli_cpc_property_id_t property_id,
                                                               void *property_value,
                                                               size_t property_length,
                                                               void *reply_arg,
                                                               sl_status_t status);

/***************************************************************************//**
 * @brief This function is used to send a no-operation (no-op) command, which
 * acts like a ping to ensure that the communication link between the
 * primary and secondary systems is operational. It is useful for
 * checking the connectivity status without performing any other
 * operations. The function requires a callback to handle the reply from
 * the secondary system, and it allows specifying the maximum number of
 * retry attempts and the timeout duration for each retry. The system
 * endpoint must be initialized before calling this function.
 *
 * @param on_noop_reply A callback function that is invoked when a reply to the
 * no-op command is received. This parameter must not be
 * null.
 * @param on_noop_reply_arg A user-defined argument that is passed to the
 * callback function. This can be null if no additional
 * data is needed.
 * @param retry_count_max The maximum number of times to retry sending the no-op
 * command if no reply is received. Must be a non-
 * negative integer.
 * @param retry_timeout_ms The timeout duration in milliseconds to wait for a
 * reply before retrying. Must be a positive integer.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * command initiation. Possible return values include SL_STATUS_OK for
 * success or an error code if the command could not be initiated.
 ******************************************************************************/
sl_status_t sli_cpc_system_cmd_noop(sli_cpc_system_noop_cmd_callback_t on_noop_reply,
                                    void *on_noop_reply_arg,
                                    uint8_t retry_count_max,
                                    uint32_t retry_timeout_ms);

/***************************************************************************//**
 * @brief This function sends a request to retrieve a property value from the
 * secondary system. It is used when you need to query a specific
 * property identified by its ID. The function requires a callback to
 * handle the response, which includes the property value and status. It
 * is important to note that only one property-get or set operation can
 * be active at a time; otherwise, the function will return
 * SL_STATUS_BUSY. Additionally, if the frame type is SYSTEM_EP_IFRAME,
 * the retry_count_max must be set to 0, or the function will return
 * SL_STATUS_INVALID_PARAMETER. Ensure the system endpoint is initialized
 * before calling this function.
 *
 * @param on_property_get_reply A callback function that is called when the
 * property-get reply is received. This callback
 * handles the property ID, value, length, and
 * status. Must not be null.
 * @param on_property_get_reply_arg A user-defined argument passed to the
 * callback function. Can be null if not
 * needed.
 * @param property_id The ID of the property to retrieve. Must be a valid
 * property ID recognized by the system.
 * @param retry_count_max The maximum number of retry attempts for the request.
 * Must be 0 if frame_type is SYSTEM_EP_IFRAME. Invalid
 * values will result in SL_STATUS_INVALID_PARAMETER.
 * @param retry_timeout_ms The timeout duration in milliseconds between retry
 * attempts. Must be a positive integer.
 * @param frame_type The type of frame to use for the request. If set to
 * SYSTEM_EP_IFRAME, retry_count_max must be 0.
 * @return Returns an sl_status_t indicating the success or failure of the
 * request. Possible return values include SL_STATUS_OK, SL_STATUS_BUSY,
 * and SL_STATUS_INVALID_PARAMETER.
 ******************************************************************************/
sl_status_t sli_cpc_system_cmd_property_get(sli_cpc_system_property_get_set_cmd_callback_t on_property_get_reply,
                                            void *on_property_get_reply_arg,
                                            sli_cpc_property_id_t property_id,
                                            uint8_t retry_count_max,
                                            uint32_t retry_timeout_ms,
                                            sli_cpc_system_ep_frame_type_t frame_type);

/***************************************************************************//**
 * @brief This function sends a request to set a property on the SECONDARY
 * device. It should be used when you need to update a property value on
 * the SECONDARY. The function requires a callback to handle the reply
 * from the SECONDARY. Only one property-get/set operation can be active
 * at a time; otherwise, the function will return SL_STATUS_BUSY. The
 * retry_count_max parameter must be set to 0 if the frame_type is
 * SYSTEM_EP_IFRAME. Ensure the system endpoint is initialized before
 * calling this function.
 *
 * @param on_property_set_reply A callback function that is called when a reply
 * is received from the SECONDARY. Must not be
 * null.
 * @param on_property_set_reply_arg A pointer to user-defined data that will be
 * passed to the callback function. Can be
 * null.
 * @param property_id The ID of the property to set. Must be a valid property ID
 * recognized by the SECONDARY.
 * @param retry_count_max The maximum number of retry attempts for the command.
 * Must be 0 if frame_type is SYSTEM_EP_IFRAME.
 * @param retry_timeout_ms The timeout in milliseconds between retry attempts.
 * Must be a positive integer.
 * @param value A pointer to the value to set for the property. Must not be null
 * and must point to a valid memory location containing the data to
 * be sent.
 * @param value_length The length in bytes of the value to be set. Must be
 * greater than 0.
 * @param frame_type The type of frame to use for the command. Must be a valid
 * sli_cpc_system_ep_frame_type_t value.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. Possible return values include SL_STATUS_OK for success
 * and SL_STATUS_INVALID_PARAMETER if retry_count_max is non-zero with
 * SYSTEM_EP_IFRAME.
 ******************************************************************************/
sl_status_t sli_cpc_system_cmd_property_set(sli_cpc_system_property_get_set_cmd_callback_t on_property_set_reply,
                                            void *on_property_set_reply_arg,
                                            sli_cpc_property_id_t property_id,
                                            uint8_t retry_count_max,
                                            uint32_t retry_timeout_ms,
                                            const void *value,
                                            size_t value_length,
                                            sli_cpc_system_ep_frame_type_t frame_type);

/***************************************************************************//**
 * @brief This function is used to send a reset command to the secondary system,
 * which is part of a communication protocol. It should be called when a
 * reset of the secondary system is required, and it is important to
 * ensure that the PROP_BOOTLOADER_REBOOT_MODE is set to the desired mode
 * before invoking this function. The function allows specifying a
 * callback to handle the response from the secondary system, and it
 * supports retry mechanisms with configurable retry count and timeout.
 * This function is intended for internal use within the CPC system and
 * should not be used by external users.
 *
 * @param on_reset_reply A callback function that is called when a reply to the
 * reset command is received from the secondary system.
 * This parameter must not be null.
 * @param on_reset_reply_arg A pointer to user-defined data that will be passed
 * to the callback function. The caller retains
 * ownership of this data.
 * @param retry_count_max The maximum number of times the reset command should
 * be retried if no response is received. Must be a non-
 * negative integer.
 * @param retry_timeout_ms The timeout in milliseconds between retries of the
 * reset command. Must be a non-negative integer.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * command initiation. Possible return values include SL_STATUS_OK for
 * success or an error code for failure.
 ******************************************************************************/
sl_status_t sli_cpc_system_cmd_reboot(sli_cpc_system_reset_cmd_callback_t on_reset_reply,
                                      void *on_reset_reply_arg,
                                      uint8_t retry_count_max,
                                      uint32_t retry_timeout_ms);

/***************************************************************************//**
 * @brief This function is used to inform the primary system endpoint that a
 * reset has occurred on the secondary system. It should be called
 * whenever a reset event is detected, passing the reason for the reset.
 * The function will notify all registered event listeners about the
 * reset event, allowing them to handle it appropriately. This function
 * must be used in contexts where the system is expected to handle reset
 * events, and it assumes that event listeners are properly registered
 * and capable of handling the reset reason provided.
 *
 * @param reset_reason The reason for the reset, as provided by the secondary
 * system. This parameter must be a valid
 * `sl_cpc_system_status_t` value, and it is used to inform
 * event listeners about the nature of the reset.
 * @return None
 ******************************************************************************/
void sli_cpc_system_notify_reset(sl_cpc_system_status_t reset_reason);

/***************************************************************************//**
 * @brief This function determines whether the system endpoint is prepared for
 * normal operations by verifying if it is connected to the secondary and
 * if the reboot sequence has completed. It should be used to ensure that
 * the system is in a state where it can perform its intended functions
 * without issues. This function is useful in scenarios where operations
 * depend on the system being fully initialized and ready. It does not
 * require any parameters and can be called at any time to check the
 * readiness status.
 *
 * @return Returns true if the system endpoint is ready, otherwise returns
 * false.
 ******************************************************************************/
bool sli_cpc_system_is_ready(void);

#ifdef __cplusplus
}
#endif

#endif /* SLI_CPC_SYSTEM_PRIMARY_H */
