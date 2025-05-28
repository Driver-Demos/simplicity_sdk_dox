/***************************************************************************//**
 * @file
 * @brief The cli input functions support character input from a user and
 *   creates a command string that can be passed on for execution.
 *   The user may input some control characters like BS, DEL and arrow keys
 *   according to ANSII escape sequences used by VT100 terminals to control
 *   editing and access previous commands stored in the history buffer.
 * @version x.y.z
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_CLI_INPUT_H
#define SLI_CLI_INPUT_H

#include "sl_cli_config.h"
#include "sl_cli_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *   Update the history buffer and position.
 *
 * @details
 *   This function should be called after newline has been detected, before
 *   the relevant command has been called, but only if the input buffer is not empty.
 *   If the input buffer contains new input, the history is shifted upwards,
 *   and the buffer is copied into the bottom of history.
 *
 * @param[in, out] handle
 *   A handle to a CLI instance.
 ******************************************************************************/
#if SL_CLI_NUM_HISTORY_BYTES
/***************************************************************************//**
 * @brief This function updates the command history buffer with the current
 * input from the CLI instance. It should be called after a newline
 * character is detected in the input buffer, but before executing the
 * command, and only if the input buffer is not empty. The function
 * ensures that the history buffer has enough space by removing the
 * oldest entries if necessary, then appends the current input to the
 * history. This allows users to access previous commands using control
 * characters or arrow keys.
 *
 * @param handle A handle to a CLI instance, which must be valid and
 * initialized. The function modifies the history buffer and
 * position within this handle. The handle must not be null.
 * @return None
 ******************************************************************************/
void sli_cli_input_update_history(sl_cli_handle_t handle);
#else
  #define sli_cli_input_update_history(s)  ((void)0)
#endif

#ifdef __cplusplus
}
#endif

#endif // SLI_CLI_INPUT_H
