/***************************************************************************//**
 * @file sl_cli_storage_nvm3.h
 * @brief Implement storing of CLI command lines in NVM3.
 * @version x.x.x
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Labs, Inc. http://www.silabs.com</b>
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

#ifndef SL_CLI_STORAGE_NVM3_H
#define SL_CLI_STORAGE_NVM3_H

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "nvm3.h"
#include "sl_cli_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cli
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup cli_storage_nvm3 CLI Commands in NVM3
 * @brief Support for CLI commands stored in and executed from NVM3
 * @{
 ******************************************************************************/

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
/// Defined in the "NVM3 Default Instance Key Space Regions"
/// Note: These definitions cannot be changed!
#define SL_CLI_STORAGE_NVM3_KEY_COUNT    SL_CLI_NVM3_KEY_COUNT
#define SL_CLI_STORAGE_NVM3_KEY_BEGIN    SL_CLI_NVM3_KEY_BEGIN
#define SL_CLI_STORAGE_NVM3_KEY_END      (SL_CLI_STORAGE_NVM3_KEY_BEGIN + SL_CLI_STORAGE_NVM3_KEY_COUNT)
/// @endcond

/// @brief Struct representing an instance of the CLI storage NVM3.
/***************************************************************************//**
 * @brief The `cli_storage_nvm3_t` structure is designed to manage the storage
 * and execution of CLI command lines within NVM3, a non-volatile memory
 * management system. It holds various fields to track the state and
 * configuration of command line storage, such as the CLI instance
 * handle, command prompt, and end string. It also manages key offsets
 * and counts for NVM3, and flags to indicate the progress of command
 * definition and execution. The structure supports both bare-metal and
 * kernel configurations, with an additional execution offset field
 * available when the kernel is not present.
 *
 * @param cli_handle The CLI instance associated with this instance.
 * @param prompt The command prompt.
 * @param end_string The string that define the end of definition.
 * @param key_offset The NVM3 key offset for this instance.
 * @param key_count The NVM3 key count for this instance.
 * @param key_next The next key value that will be used.
 * @param execute_while_define True if the command shell execute when defined.
 * @param define_in_progress True when definition is in progress.
 * @param execute_in_progress True when execute is in progress.
 * @param init_tick True after the initial (first) tick has been executed.
 * @param exec_offset The script execution offset, only defined if
 * SL_CATALOG_KERNEL_PRESENT is not defined.
 ******************************************************************************/
typedef struct {
  sl_cli_handle_t cli_handle;   ///< The CLI instance associated with this instance.
  char *prompt;                 ///< The command prompt.
  char *end_string;             ///< The string that define the end of definition.
  size_t key_offset;            ///< The NVM3 key offset for this instance.
  size_t key_count;             ///< The NVM3 key count for this instance.
  size_t key_next;              ///< The next key value that will be used.
  bool execute_while_define;    ///< True if the command shell execute when defined.
  bool define_in_progress;      ///< True when definition is in progress.
  bool execute_in_progress;     ///< True when execute is in progress.
  bool init_tick;               ///< True after the initial (first) tick has been executed.
#if !defined(SL_CATALOG_KERNEL_PRESENT)
  size_t exec_offset;           ///< The script execution offset.
#endif
} cli_storage_nvm3_t;

/// @brief Definition of the CLI storage NVM3 handle.
typedef cli_storage_nvm3_t *cli_storage_nvm3_handle_t;

/***************************************************************************//**
 * @brief This function is used to remove all command lines that have been
 * stored in the NVM3 storage associated with a CLI instance. It is
 * typically used when there is a need to reset or clear the stored
 * command history or configuration. The function should be called with a
 * valid set of command arguments, and it will iterate through the keys
 * associated with the CLI storage instance, deleting each one. This
 * operation effectively resets the storage to its initial state.
 *
 * @param arguments A pointer to an sl_cli_command_arg_t structure containing
 * the command arguments. This parameter must not be null and
 * should contain a valid handle to the CLI storage instance.
 * If the handle is invalid, the behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_cli_storage_nvm3_clear(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function is used to display the command lines that have been
 * stored in the NVM3 storage. It is typically used as a CLI command
 * handler to allow users to view the stored commands. The function must
 * be called with a valid pointer to a command argument structure, which
 * includes the handle to the CLI storage instance. It is important to
 * ensure that the arguments pointer is not null before calling this
 * function to avoid undefined behavior.
 *
 * @param arguments A pointer to an sl_cli_command_arg_t structure containing
 * the command arguments. This must include a valid handle to
 * the CLI storage instance. The pointer must not be null, as
 * this will lead to undefined behavior.
 * @return None
 ******************************************************************************/
void sl_cli_storage_nvm3_list(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function initiates the process of defining new command lines that
 * will be stored in NVM3. It should be used when you want to begin
 * capturing command lines for later execution or storage. The function
 * checks if a command execution is currently in progress and, if not,
 * redirects the command input to the NVM3 storage system. This function
 * is typically used as a CLI command handler and should be called when
 * no other command execution is ongoing.
 *
 * @param arguments A pointer to an sl_cli_command_arg_t structure containing
 * the command arguments. This parameter must not be null, and
 * it is expected to contain a valid handle for the CLI storage
 * NVM3 instance.
 * @return None
 ******************************************************************************/
void sl_cli_storage_nvm3_define(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function executes command lines that have been previously stored
 * in NVM3. It can be used directly as a CLI command handler. In a kernel
 * configuration, all stored commands are executed before the function
 * returns. In a bare-metal configuration, the function signals that
 * stored commands should be executed by the `sl_cli_storage_nvm3_tick`
 * function, with one command executed per tick. The function ensures
 * that execution is not already in progress before proceeding.
 *
 * @param arguments A pointer to an `sl_cli_command_arg_t` structure containing
 * the command arguments. This parameter must not be null, and
 * the handle within must be valid. If the handle indicates
 * that execution is already in progress, the function will
 * return immediately without executing any commands.
 * @return None
 ******************************************************************************/
void sl_cli_storage_nvm3_execute(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function retrieves the count of command lines currently stored in
 * the NVM3 storage associated with a given CLI handle. It is useful for
 * determining how many command lines have been saved and are available
 * for execution or management. The function should be called with a
 * valid CLI handle that is connected to the CLI storage RAM component.
 * The count returned may be limited by the configuration of the
 * instance.
 *
 * @param cli_handle Handle to the CLI where the CLI storage RAM component is
 * connected. It must be a valid handle, and passing an
 * invalid handle may result in undefined behavior.
 * @return The function returns the number of command lines stored in NVM3 as a
 * size_t value.
 ******************************************************************************/
size_t sl_cli_storage_nvm3_count(sl_cli_handle_t cli_handle);

/***************************************************************************//**
 * @brief This function prepares a CLI storage NVM3 instance for use by setting
 * its internal state to default values. It should be called before any
 * other operations are performed on the instance to ensure it is in a
 * known state. This function does not allocate memory or perform any I/O
 * operations, and it is safe to call multiple times if reinitialization
 * is needed.
 *
 * @param handle A pointer to a cli_storage_nvm3_t structure representing the
 * CLI storage NVM3 instance. The handle must not be null, and the
 * caller retains ownership of the memory. The function assumes
 * the structure is properly allocated and accessible.
 * @return None
 ******************************************************************************/
void sl_cli_storage_nvm3_init(cli_storage_nvm3_handle_t handle);

/***************************************************************************//**
 * @brief This function is used to execute command lines stored in NVM3,
 * particularly in a bare-metal configuration where it processes one
 * command per call. It should be called after initiating command
 * execution with sl_cli_storage_nvm3_execute. The function manages the
 * execution state and returns whether there are more commands to
 * execute. It is essential in environments without a kernel, as it
 * handles command execution incrementally.
 *
 * @param cli_handle A handle to the CLI instance associated with the NVM3
 * storage. It must be a valid handle, and the caller retains
 * ownership. Invalid handles may lead to undefined behavior.
 * @return Returns a boolean indicating if there are more commands to execute
 * (true) or if all commands have been executed (false).
 ******************************************************************************/
bool sl_cli_storage_nvm3_tick(sl_cli_handle_t cli_handle);

/** @} (end addtogroup cli_storage_nvm3) */
/** @} (end addtogroup cli) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLI_STORAGE_NVM3_H
