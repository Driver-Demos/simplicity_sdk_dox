/***************************************************************************//**
 * @file sl_cli_storage_ram.h
 * @brief Implement storing of CLI command lines in RAM.
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

#ifndef SL_CLI_STORAGE_RAM_H
#define SL_CLI_STORAGE_RAM_H

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "sl_cli_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cli
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup cli_storage_ram CLI Commands in RAM
 * @brief Support for CLI commands stored in and executed from RAM
 * @{
 ******************************************************************************/

/// @brief Struct representing an instance of the CLI storage RAM.
/***************************************************************************//**
 * @brief The `cli_storage_ram_t` structure is designed to manage the storage
 * and execution of command lines in RAM for a Command Line Interface
 * (CLI) system. It includes fields for associating a CLI instance,
 * managing command prompts, and defining the end of command definitions.
 * The structure also tracks the size and current position within the RAM
 * buffer, and flags to indicate the status of command definition and
 * execution processes. Additionally, it conditionally includes an
 * execution offset for script execution when the kernel is not present.
 *
 * @param cli_handle The CLI instance associated with this instance.
 * @param prompt The command prompt.
 * @param end_string The string that define the end of definition.
 * @param ram_size The number of command lines that can be stored in RAM.
 * @param ram_next The next free line in RAM.
 * @param ram_buffer Pointer to the RAM buffer.
 * @param execute_while_define True if the command shell execute when defined.
 * @param define_in_progress True when definition is in progress.
 * @param execute_in_progress True when execute is in progress.
 * @param exec_offset The script execution offset, only defined if
 * SL_CATALOG_KERNEL_PRESENT is not defined.
 ******************************************************************************/
typedef struct {
  sl_cli_handle_t cli_handle;   ///< The CLI instance associated with this instance.
  char *prompt;                 ///< The command prompt.
  char *end_string;             ///< The string that define the end of definition.
  size_t ram_size;              ///< The number of command lines that can be stored in RAM.
  size_t ram_next;              ///< The next free line in RAM.
  char   *ram_buffer;           ///< Pointer to the RAM buffer.
  bool execute_while_define;    ///< True if the command shell execute when defined.
  bool define_in_progress;      ///< True when definition is in progress.
  bool execute_in_progress;     ///< True when execute is in progress.
#if !defined(SL_CATALOG_KERNEL_PRESENT)
  size_t exec_offset;           ///< The script execution offset.
#endif
} cli_storage_ram_t;

/// @brief Definition of the CLI storage RAM handle.
typedef cli_storage_ram_t *cli_storage_ram_handle_t;

/***************************************************************************//**
 * @brief This function is used to clear all command lines that have been stored
 * in RAM for a specific CLI instance. It is typically used when you want
 * to reset the command storage, removing all previously stored commands.
 * This function can be directly used as a CLI command handler, making it
 * convenient for integration into command line interfaces. It is
 * important to ensure that the `arguments` parameter is correctly
 * initialized and points to a valid `sl_cli_command_arg_t` structure
 * before calling this function.
 *
 * @param arguments A pointer to an `sl_cli_command_arg_t` structure containing
 * the command arguments. This parameter must not be null and
 * should be properly initialized before use. The function
 * expects this structure to contain a valid handle to the CLI
 * storage RAM instance.
 * @return None
 ******************************************************************************/
void sl_cli_storage_ram_clear(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function is used to display the command lines that have been
 * stored in RAM. It is intended to be used as a CLI command handler,
 * allowing users to view the stored commands directly from the command
 * line interface. This function should be called when there is a need to
 * review or verify the commands currently held in RAM. It requires a
 * valid pointer to a command argument structure, which provides the
 * necessary context for the operation.
 *
 * @param arguments A pointer to an sl_cli_command_arg_t structure, which
 * contains the command arguments. This parameter must not be
 * null, as it is used to determine the context for listing the
 * stored commands.
 * @return None
 ******************************************************************************/
void sl_cli_storage_ram_list(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function initiates the process of defining new command lines that
 * will be stored in RAM for later execution. It should be used when you
 * want to begin entering a series of commands that need to be stored and
 * potentially executed later. The function must be called with a valid
 * command argument structure, and it will only proceed if no execution
 * is currently in progress. Once invoked, it sets the system into a
 * state where command definition is in progress, allowing subsequent
 * commands to be captured and stored.
 *
 * @param arguments A pointer to an sl_cli_command_arg_t structure containing
 * the command arguments. This parameter must not be null and
 * should be properly initialized before calling the function.
 * The function will not proceed if the current execution is in
 * progress.
 * @return None
 ******************************************************************************/
void sl_cli_storage_ram_define(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function executes command lines that have been previously stored
 * in RAM. It should be used when you want to run these stored commands.
 * In a kernel configuration, all commands are executed before the
 * function returns, while in a bare-metal configuration, execution is
 * signaled and must be completed by calling `sl_cli_storage_ram_tick`.
 * The function ensures that execution is not already in progress before
 * starting. It is important to ensure that the `arguments` parameter is
 * valid and correctly initialized before calling this function.
 *
 * @param arguments A pointer to an `sl_cli_command_arg_t` structure containing
 * the command arguments. This parameter must not be null and
 * should be properly initialized before calling the function.
 * Invalid or null pointers may lead to undefined behavior.
 * @return None
 ******************************************************************************/
void sl_cli_storage_ram_execute(sl_cli_command_arg_t *arguments);

/***************************************************************************//**
 * @brief This function retrieves the count of command lines currently stored in
 * RAM for a given CLI instance. It is useful for monitoring or managing
 * the storage of command lines in RAM. The function requires a valid CLI
 * handle that is associated with a CLI storage RAM instance. It should
 * be called when you need to know how many command lines are stored, for
 * example, before adding more lines or clearing the storage.
 *
 * @param cli_handle Handle to the CLI where the cli_storage_ram component is
 * connected. Must be a valid handle associated with a CLI
 * storage RAM instance. If the handle is invalid, the
 * behavior is undefined.
 * @return The number of command lines stored in RAM.
 ******************************************************************************/
size_t sl_cli_storage_ram_count(sl_cli_handle_t cli_handle);

/***************************************************************************//**
 * @brief This function sets up a CLI storage RAM instance for use, preparing it
 * to store and manage command lines in RAM. It should be called before
 * any operations are performed on the CLI storage RAM instance to ensure
 * it is in a known, default state. This function is essential for
 * initializing the internal state of the storage RAM, ensuring that it
 * is ready to handle command definitions and executions. It is important
 * to call this function before using the instance in any CLI operations.
 *
 * @param handle A handle to the CLI storage RAM instance. This must not be
 * null, and the caller retains ownership. The function
 * initializes the internal state of the instance pointed to by
 * this handle.
 * @return None
 ******************************************************************************/
void sl_cli_storage_ram_init(cli_storage_ram_handle_t handle);

#if !defined(SL_CATALOG_KERNEL_PRESENT)
/***************************************************************************//**
 * @brief This function is used to continue executing command lines stored in
 * RAM for a CLI instance in a bare-metal configuration. It should be
 * called after initiating command execution with
 * `sl_cli_storage_ram_execute`. The function processes one command per
 * call and should be repeatedly invoked until it returns false,
 * indicating that all commands have been executed. This function is only
 * applicable in environments where the kernel is not present.
 *
 * @param cli_handle A handle to the CLI instance associated with the RAM
 * storage. It must be a valid handle that has been previously
 * initialized. The function will not execute properly if the
 * handle is invalid or uninitialized.
 * @return Returns a boolean value: true if there are more commands to execute,
 * false if all commands have been executed.
 ******************************************************************************/
bool sl_cli_storage_ram_tick(sl_cli_handle_t cli_handle);
#endif

/** @} (end addtogroup cli_storage_ram) */
/** @} (end addtogroup cli) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLI_STORAGE_RAM_H
