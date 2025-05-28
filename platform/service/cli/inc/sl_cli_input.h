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

#ifndef SL_CLI_INPUT_H
#define SL_CLI_INPUT_H

#include "sl_cli_config.h"
#include "sl_cli_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cli
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   MACROS   ************************************
 ******************************************************************************/

/// @brief Define the UP-direction.
#define SL_CLI_INPUT_DIRECTION_UP    (1)
/// @brief Define the DOWN-direction.
#define SL_CLI_INPUT_DIRECTION_DOWN  (-1)

/***************************************************************************//**
 * @brief This function handles character input from a terminal, updating the
 * CLI instance's input buffer and echoing characters back to the
 * terminal as needed. It should be called whenever a new character is
 * detected. The function is designed to handle various control
 * characters, such as backspace and delete, and can process line endings
 * to signal command completion. The behavior of this function can be
 * customized through configuration settings. It is important to ensure
 * that the CLI instance handle is valid and properly initialized before
 * calling this function.
 *
 * @param handle A handle to a CLI instance, which must be valid and properly
 * initialized. The function updates the input buffer and state
 * within this handle.
 * @param c The character input to be processed. It can be any valid character,
 * including control characters like newline, backspace, and delete.
 * @return Returns true if a newline or return character is detected, indicating
 * command completion; otherwise, returns false.
 ******************************************************************************/
bool sl_cli_input_char(sl_cli_handle_t handle,
                       char c);

/***************************************************************************//**
 * @brief Use this function to reset the input buffer of a CLI instance after
 * processing the current command. It should be called once a newline has
 * been detected and the command in the buffer has been executed or
 * otherwise handled. This ensures that the buffer is empty and ready for
 * new input, preventing any residual data from affecting subsequent
 * commands.
 *
 * @param handle A handle to a CLI instance. This parameter must not be null and
 * should point to a valid CLI instance whose input buffer is to
 * be cleared. The function will not perform any action if the
 * handle is invalid.
 * @return None
 ******************************************************************************/
void sl_cli_input_clear(sl_cli_handle_t handle);

#if SL_CLI_ADVANCED_INPUT_HANDLING
/***************************************************************************//**
 * @brief This function attempts to auto-complete the current command in the
 * input buffer of a CLI instance. It should be used when the user
 * requests auto-completion, typically by pressing a specific key. If a
 * single match is found, the command is completed in the buffer and
 * displayed on the terminal. If multiple matches are found, a list of
 * possible matches is printed, and the longest common prefix is used to
 * update the buffer and terminal. If no matches are found, the function
 * does nothing. The function assumes the input buffer is properly
 * initialized and that the handle is valid.
 *
 * @param handle A handle to a CLI instance. It must be a valid, initialized
 * handle, and the function will modify the input buffer
 * associated with this handle.
 * @return None
 ******************************************************************************/
void sl_cli_input_autocomplete(sl_cli_handle_t handle);
#endif // SL_CLI_ADVANCED_INPUT_HANDLING

#if SL_CLI_NUM_HISTORY_BYTES
/***************************************************************************//**
 * @brief This function is used to navigate through the command history in a
 * command-line interface (CLI) environment. It should be called when the
 * user presses the up or down arrow keys, which are typically used to
 * access previous or next commands stored in the history buffer. The
 * function updates the input buffer with the command retrieved from the
 * history, allowing the user to edit or re-execute it. It is important
 * to ensure that the CLI instance handle is valid and that the history
 * feature is enabled before calling this function. The function does not
 * handle invalid direction values, which will result in no action being
 * taken.
 *
 * @param handle A handle to a CLI instance. This parameter must be a valid,
 * initialized CLI handle, and the caller retains ownership. It
 * must not be null.
 * @param direction An integer indicating the direction to navigate in the
 * history. Use 1 for up (previous command) and -1 for down
 * (next command). Invalid values will result in no action.
 * @return None
 ******************************************************************************/
void sl_cli_input_get_history(sl_cli_handle_t handle,
                              int direction);
#endif // SL_CLI_NUM_HISTORY_BYTES

/** @} (end addtogroup cli) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLI_INPUT_H
