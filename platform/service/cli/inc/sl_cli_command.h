/***************************************************************************//**
 * @file
 * @brief The command interpreter has functions to interpret a command string
 *   and execute the correct command function with decoded parameters.
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

#ifndef SL_CLI_COMMAND_H
#define SL_CLI_COMMAND_H

#include <stdint.h>
#include <stdbool.h>

#include "sl_cli_config.h"
#include "sl_cli_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cli
 * @{
 ******************************************************************************/

#if SL_CLI_HELP_DESCRIPTION_ENABLED
// Macros requiring help text

/***************************************************************************//**
 *  @brief A macro, which is used to create commands.
 ******************************************************************************/
  #define SL_CLI_COMMAND(function, help_text, arg_help, ...) \
  {                                                          \
    (function),     /* Pointer to command function */        \
    (help_text),    /* Help text */                          \
    (arg_help),     /* Argument help text */                 \
    __VA_ARGS__     /* Argument list*/                       \
  }

/***************************************************************************//**
 *  @brief A macro, which is used to create command groups.
 ******************************************************************************/
  #define SL_CLI_COMMAND_GROUP(group_table, help_text)                       \
  {                                                                          \
    ((sl_cli_command_func_t)(group_table)),   /* Group pointer */            \
    (help_text),                              /* Help text */                \
    (""),                                     /* Empty argument help text */ \
    { SL_CLI_ARG_GROUP, }                     /* Group indicator */          \
  }
#else
// Macros, which do not allow help text.

/***************************************************************************//**
 *  @brief A macro, which is used to create commands.
 ******************************************************************************/
  #define SL_CLI_COMMAND(function, help_text, arg_help, ...) \
  {                                                          \
    (function),    /* Pointer to command function */         \
    __VA_ARGS__    /* Argument list*/                        \
  }

/***************************************************************************//**
 *  @brief A macro, which is used to create command groups.
 ******************************************************************************/
  #define SL_CLI_COMMAND_GROUP(group_table, help_text)              \
  {                                                                 \
    ((sl_cli_command_func_t)(group_table)),   /* Group pointer */   \
    { SL_CLI_ARG_GROUP, }                     /* Group indicator */ \
  }
#endif // SL_CLI_HELP_DESCRIPTION_ENABLED

/***************************************************************************//**
 * @brief This function is used to add a new command group to a specified CLI
 * instance. It should be called when you want to extend the command set
 * available in a CLI by adding a new group of commands. The command
 * group structure must not be in use and should be properly initialized
 * with NULL in all elements except the command table before calling this
 * function. If the command group is successfully added, the function
 * returns true; otherwise, it returns false. This function does not
 * modify the CLI instance or command group in any other way.
 *
 * @param handle A handle to a CLI instance. This parameter must be a valid,
 * initialized CLI handle, and the caller retains ownership.
 * @param command_group A pointer to a command group structure. The structure
 * must not be null, must not be in use, and should have
 * NULL in all elements except the command table. The
 * caller retains ownership, and the function will set the
 * 'in_use' flag to true if the group is added
 * successfully.
 * @return Returns true if the command group is successfully added; otherwise,
 * returns false.
 ******************************************************************************/
bool sl_cli_command_add_command_group(sl_cli_handle_t handle, sl_cli_command_group_t *command_group);

/***************************************************************************//**
 * @brief Use this function to remove a command group from a CLI instance when
 * it is no longer needed or should be disabled. The function requires a
 * valid CLI handle and a pointer to the command group to be removed. The
 * command group must be currently in use for the removal to succeed.
 * This function will return false if the command group is null or not in
 * use, indicating that the removal was unsuccessful.
 *
 * @param handle A handle to a CLI instance. It must be a valid, initialized
 * handle that the command group is associated with.
 * @param command_group A pointer to the command group structure to be removed.
 * The command group must not be null and should be
 * currently marked as in use for the removal to succeed.
 * @return Returns true if the command group was successfully removed, false
 * otherwise.
 ******************************************************************************/
bool sl_cli_command_remove_command_group(sl_cli_handle_t handle, sl_cli_command_group_t *command_group);

/***************************************************************************//**
 * @brief This function searches for a command entry within the command groups
 * associated with a given CLI handle, using the provided tokenized
 * input. It is typically used to identify and retrieve the command entry
 * that matches the user's input, which can then be executed or further
 * processed. The function updates several output parameters to indicate
 * the position of the first command argument, whether a specific command
 * was detected, and if a help command was issued. It is important to
 * ensure that the CLI handle and tokenized input are valid before
 * calling this function.
 *
 * @param handle A handle to a CLI instance. Must be a valid, initialized CLI
 * handle.
 * @param token_c A pointer to an integer representing the number of tokens in
 * the token_v array. Must not be null.
 * @param token_v An array of strings representing the tokenized input. Each
 * string must be null-terminated.
 * @param arg_ofs A pointer to an integer that will be set to the index of the
 * first command argument. Must not be null.
 * @param single_flag A pointer to a boolean that will be set to true if a
 * specific command is detected. Must not be null.
 * @param help_flag A pointer to a boolean that will be set to true if a 'help'
 * command is detected. Must not be null.
 * @return A pointer to the command entry that matches the input tokens, or NULL
 * if no match is found. If a help command is issued, the return value
 * may point to a command entry table.
 ******************************************************************************/
const sl_cli_command_entry_t *sl_cli_command_find(sl_cli_handle_t handle,
                                                  int *token_c,
                                                  char *token_v[],
                                                  int *arg_ofs,
                                                  bool *single_flag,
                                                  bool *help_flag);

/***************************************************************************//**
 * @brief This function is used to interpret a command string and execute the
 * corresponding command function with the decoded parameters. It should
 * be called when a command needs to be executed from a given input
 * string. The input string is modified in-place during processing, so it
 * must be a mutable, null-terminated C-string. The function returns a
 * status indicating the success or failure of the command execution,
 * which can be used to handle errors or confirm successful execution.
 *
 * @param handle A handle to a CLI instance. The caller must ensure that this
 * handle is valid and properly initialized before calling the
 * function.
 * @param input A mutable, null-terminated C-string containing the user input.
 * The string will be modified in-place, so it must not be a string
 * literal or constant.
 * @return Returns a status code of type sl_status_t indicating the result of
 * the command execution. Possible return values include success,
 * command not found, or errors during tokenization or argument
 * conversion.
 ******************************************************************************/
sl_status_t sl_cli_command_execute(sl_cli_handle_t handle,
                                   char *input);

/***************************************************************************//**
 * @brief
 *   Find a number of matches and command matches from CLI's input buffer.
 *
 * @param[in, out] handle
 *   A handle to a CLI instance.
 *
 * @param[out] possible_matches
 *   A string containing all possible command matches for what is currently in
 *   the input buffer.
 *
 * @param[in] possible_matches_size
 *   The size of the possible_matches string buffer.
 *
 * @param[out] input_length
 *   An integer value indicating the length of the last word in the input buffer.
 *
 * @param[out] input_position
 *   An integer value indicating where the last word in the input buffer begins.
 *
 * @return
 *   A number of possible matches found in the command table.
 ******************************************************************************/
#if SL_CLI_ADVANCED_INPUT_HANDLING
/***************************************************************************//**
 * @brief This function is used to identify and list potential command matches
 * based on the current input buffer of a CLI instance. It is
 * particularly useful for implementing command auto-completion or
 * suggestions. The function requires a valid CLI handle and outputs a
 * string of possible matches, along with the length and position of the
 * last word in the input buffer. It should be called when the input
 * buffer is populated with a command string, and the caller must ensure
 * that the possible_matches buffer is large enough to hold the results.
 * The function returns the number of matches found.
 *
 * @param handle A handle to a CLI instance. Must be a valid, initialized
 * handle.
 * @param possible_matches A buffer to store the possible command matches. The
 * caller must ensure it is large enough to hold the
 * results.
 * @param possible_matches_size The size of the possible_matches buffer. Must be
 * sufficient to store the expected matches.
 * @param input_length A pointer to an integer where the function will store the
 * length of the last word in the input buffer. Must not be
 * null.
 * @param input_position A pointer to an integer where the function will store
 * the starting position of the last word in the input
 * buffer. Must not be null.
 * @return Returns the number of possible command matches found in the command
 * table.
 ******************************************************************************/
int sl_cli_command_find_matches(sl_cli_handle_t handle,
                                char *possible_matches,
                                size_t possible_matches_size,
                                int *input_length,
                                int *input_position);
#endif

/** @} (end addtogroup cli) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLI_COMMAND_H
