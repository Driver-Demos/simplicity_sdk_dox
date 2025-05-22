/***************************************************************************//**
 * @file
 * @brief Functions to find and execute cli commands.
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

#include "sl_cli.h"
#include "sl_cli_command.h"
#include "sl_cli_tokenize.h"
#include "sli_cli_io.h"
#include "sli_cli_arguments.h"
#include "sl_string.h"
#include "sl_common.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>

#define SL_CLI_TERMINAL_LINE_LENGTH  (80)

#ifndef SL_CLI_EOL_STRING
#define SL_CLI_EOL_STRING "\n"
#endif

/**
 * @brief The `sli_cli_pre_cmd_hook` function is a weakly defined hook that is executed before a CLI command, but does nothing unless specifically redefined.
 *
 * @param arguments A pointer to an `sl_cli_command_arg_t` structure containing the arguments passed to the command handler.
 * @return The function does not produce any output or return any value.
 */
SL_WEAK void sli_cli_pre_cmd_hook(sl_cli_command_arg_t* arguments)
{
  (void) arguments;
}

/**
 * @brief The `sli_cli_post_cmd_hook` function is a weakly defined hook that executes after a command is processed, but does nothing unless overridden.
 *
 * @param arguments A pointer to an `sl_cli_command_arg_t` structure containing the arguments passed to the command handler.
 * @return The function does not produce any output or return a value.
 */
SL_WEAK void sli_cli_post_cmd_hook(sl_cli_command_arg_t* arguments)
{
  (void) arguments;
}

/**
 * @brief The `cmd_strcmp` function compares two command strings, either case-sensitively or case-insensitively, based on a preprocessor directive.
 *
 * @param a The first string to compare.
 * @param b The second string to compare.
 * @return The function returns an integer greater than, less than, or equal to 0, indicating whether the first string is greater than, less than, or equal to the second string.
 */
static int cmd_strcmp(const char *a, const char *b)
{
#if SL_CLI_IGNORE_COMMAND_CASE
  return sl_strcasecmp(a, b);
#else
  return strcmp(a, b);
#endif
}

#if SL_CLI_HELP_DESCRIPTION_ENABLED
/**
 * @brief The `print_and_pad` function prints a given string and appends spaces until a specified total length is reached.
 *
 * @param string A pointer to the string that needs to be printed.
 * @param total_length The total length that the printed string should occupy, including the original string and any added spaces.
 * @return The function does not return a value; it outputs the string followed by spaces directly to the standard output.
 */
static void print_and_pad(const char *string,
                          size_t total_length)
{
  size_t string_length = strlen(string);
  sli_cli_io_printf(string);
  for (; string_length < total_length; string_length++) {
    sli_cli_io_printf(" ");
  }
}

/**
 * @brief The `get_arg_type_string` function returns a string representation of a given CLI argument type.
 *
 * @param type An enumeration value of type `sl_cli_argument_type_t` representing the CLI argument type to be translated into a string.
 * @return A constant character pointer to a string that represents the input argument type.
 */
static const char *get_arg_type_string(sl_cli_argument_type_t type)
{
  const char *str;

  switch (type) {
    case SL_CLI_ARG_UINT8:
      str = "uint8";
      break;
    case SL_CLI_ARG_UINT16:
      str = "uint16";
      break;
    case SL_CLI_ARG_UINT32:
      str = "uint32";
      break;
    case SL_CLI_ARG_INT8:
      str = "int8";
      break;
    case SL_CLI_ARG_INT16:
      str = "int16";
      break;
    case SL_CLI_ARG_INT32:
      str = "int32";
      break;
    case SL_CLI_ARG_STRING:
      str = "string";
      break;
    case SL_CLI_ARG_HEX:
      str = "hex";
      break;
    case SL_CLI_ARG_UINT8OPT:
      str = "uint8opt";
      break;
    case SL_CLI_ARG_UINT16OPT:
      str = "uint16opt";
      break;
    case SL_CLI_ARG_UINT32OPT:
      str = "uint32opt";
      break;
    case SL_CLI_ARG_INT8OPT:
      str = "int8opt";
      break;
    case SL_CLI_ARG_INT16OPT:
      str = "int16opt";
      break;
    case SL_CLI_ARG_INT32OPT:
      str = "int32opt";
      break;
    case SL_CLI_ARG_STRINGOPT:
      str = "stringopt";
      break;
    case SL_CLI_ARG_HEXOPT:
      str = "hexopt";
      break;
    case SL_CLI_ARG_ADDITIONAL:
      str = "additional";
      break;
    case SL_CLI_ARG_WILDCARD:
      str = "wildcard";
      break;
    case SL_CLI_ARG_GROUP:
      str = "group";
      break;
    case SL_CLI_ARG_END:
      str = "end";
      break;
    default:
      str = "unknown";
      break;
  }

  return str;
}

/***************************************************************************//**
 * @brief
 *   Print out the available commands in the given command table and their
 *   argument list and help description. Line length on terminal is assumed to
 *   be given by the SL_CLI_TERMINAL_LINE_LENGTH #define or larger.
 *
 * @param[in] command_table
 *   sl_cli_command_entry_t array that is {NULL, NULL}-terminated. Can be both
 *   root command table and group command table
 ******************************************************************************/
static void cmd_help_command(const sl_cli_command_entry_t *cmd_entry)
{
  print_and_pad("", SL_CLI_HELP_CMD_PRE);
  print_and_pad(cmd_entry->name, SL_CLI_HELP_CMD_SIZE);
  sli_cli_io_printf(cmd_entry->command->help);
  sli_cli_io_printf(SL_CLI_EOL_STRING);

  // Print argument list
  const sl_cli_argument_type_t *arg_type_list;
  arg_type_list = cmd_entry->command->arg_type_list;
  if (arg_type_list[0] == SL_CLI_ARG_GROUP ||  arg_type_list[0] == SL_CLI_ARG_END) {
    // Do not do anything. No arguments to print
  } else {
    char *arg_help = cmd_entry->command->arg_help;
    int arg_no = 0;
    int arg_help_ofs = -1;
    int arg_help_length = strlen(arg_help);
    while (arg_type_list[arg_no] != SL_CLI_ARG_END) {
      const char *arg_str;
      arg_str = get_arg_type_string(arg_type_list[arg_no]);
      if (arg_help_length > 0) {
        arg_help_ofs++;
      }
      if (arg_type_list[arg_no] != SL_CLI_ARG_ADDITIONAL) {
        print_and_pad("", SL_CLI_HELP_CMD_PRE);
        print_and_pad("", SL_CLI_HELP_CMD_SIZE);
        sli_cli_io_printf("[");
        sli_cli_io_printf("%s", (strcmp(arg_str, "wildcard") == 0) ? "*" : arg_str);
        if (arg_type_list[arg_no + 1] == SL_CLI_ARG_ADDITIONAL) {
          sli_cli_io_putchar('+');
        }
        sli_cli_io_printf("] ");
        if ((arg_help_length > 0) && (arg_help[arg_help_ofs] != SL_CLI_UNIT_SEPARATOR_CHAR)) {
          while ((arg_help[arg_help_ofs] != SL_CLI_UNIT_SEPARATOR_CHAR) &&  (arg_help_ofs < arg_help_length)) {
            sli_cli_io_putchar(arg_help[arg_help_ofs]);
            arg_help_ofs++;
          }
        }
        sli_cli_io_printf(SL_CLI_EOL_STRING);
      }
      arg_no++;
    }
  }
}

#else
/**
 * @brief The `cmd_help_command` function prints the name of a CLI command entry followed by a newline.
 *
 * @param cmd_entry A pointer to an `sl_cli_command_entry_t` structure representing a command entry, which contains the name of the command to be printed.
 * @return The function does not return any value; it performs an output operation to print the command name.
 */
static void cmd_help_command(const sl_cli_command_entry_t *cmd_entry)
{
  sli_cli_io_printf("  %s" SL_CLI_EOL_STRING, cmd_entry->name);
}

#endif // SL_CLI_HELP_DESCRIPTION_ENABLED

/**
 * @brief The `cmd_help_group` function iterates over a command table and prints help information for each command that is not a shortcut.
 *
 * @param command_table An array of `sl_cli_command_entry_t` structures representing the command table, which is terminated by an entry with a NULL name.
 * @return The function does not return a value; it outputs help information for each non-shortcut command in the command table.
 */
static void cmd_help_group(const sl_cli_command_entry_t command_table[])
{
  int i = 0;

  //printf("---------------\n");
  while (command_table[i].name) {
    if (command_table[i].is_shortcut) {
      i++;
      continue;
    }
    cmd_help_command(&command_table[i]);
    i++;
  }
}

/**
 * @brief The `cmd_help` function provides help information for CLI commands by printing details of a specific command or a group of commands based on the input parameters.
 *
 * @param handle A handle to the CLI instance, which contains the command groups.
 * @param cmd_table A pointer to a command table (array of command entries) that is either a specific command or a group of commands.
 * @param single A boolean flag indicating whether to display help for a single command (true) or a group of commands (false).
 * @return The function does not return a value; it outputs help information to the CLI interface.
 */
static void cmd_help(sl_cli_handle_t handle, const sl_cli_command_entry_t *cmd_table, bool single)
{
  if (cmd_table != NULL) {
    if (single) {
      cmd_help_command(cmd_table);
    } else {
      cmd_help_group(cmd_table);
    }
  } else {
    sl_cli_command_group_t *cmd_group;
    const sl_cli_command_entry_t *table;

    SL_SLIST_FOR_EACH_ENTRY(handle->command_group, cmd_group, sl_cli_command_group_t, node) {
      table = cmd_group->command_table;
      if (table != NULL) {
        cmd_help_group(table);
      }
    }
  }
}

/**
 * @brief The function `sl_cli_command_add_command_group` adds a command group to a CLI handle if it is not already in use.
 *
 * @param handle A pointer to an `sl_cli_handle_t` structure representing the CLI handle to which the command group will be added.
 * @param command_group A pointer to an `sl_cli_command_group_t` structure representing the command group to be added.
 * @return Returns `true` if the command group was successfully added to the CLI handle, otherwise returns `false`.
 */
bool sl_cli_command_add_command_group(sl_cli_handle_t handle, sl_cli_command_group_t *command_group)
{
  bool status = false;

  if (command_group != NULL) {
    if (!command_group->in_use) {
      command_group->in_use = true;
      sl_slist_push(&handle->command_group, &command_group->node);
      status = true;
    }
  }
  return status;
}

/**
 * @brief The function `sl_cli_command_remove_command_group` removes a command group from a CLI handle if it is currently in use.
 *
 * @param handle A pointer to the CLI handle (`sl_cli_handle_t`) from which the command group is to be removed.
 * @param command_group A pointer to the command group (`sl_cli_command_group_t`) that is to be removed from the CLI handle.
 * @return Returns a boolean value `true` if the command group was successfully removed, otherwise `false`.
 */
bool sl_cli_command_remove_command_group(sl_cli_handle_t handle, sl_cli_command_group_t *command_group)
{
  bool status = false;

  if (command_group != NULL) {
    if (command_group->in_use) {
      command_group->in_use = false;
      sl_slist_remove(&handle->command_group, &command_group->node);
      command_group->node.node = NULL;
      status = true;
    }
  }
  return status;
}

// The following function is only needed if advanced input handling is enabled
#if !defined(SL_CLI_DUT) && (SL_CLI_ADVANCED_INPUT_HANDLING == 1)
/**
 * @brief The `sl_cli_command_find_matches` function searches for command matches in a CLI input buffer and populates a string with possible matches.
 *
 * @param handle A pointer to the CLI handle containing the input buffer and command group information.
 * @param possible_matches A character array to store the possible command matches found.
 * @param possible_matches_size The size of the `possible_matches` array.
 * @param input_length A pointer to an integer where the length of the input command will be stored.
 * @param input_position A pointer to an integer where the position in the input buffer will be stored.
 * @return The function returns the number of possible command matches found.
 */
int sl_cli_command_find_matches(sl_cli_handle_t handle,
                                char *possible_matches,
                                size_t possible_matches_size,
                                int *input_length,
                                int *input_position)
{
  int len;
  int original_input_buffer_len = strlen(handle->input_buffer);
  char *token_v[SL_CLI_MAX_INPUT_ARGUMENTS];
  char *buf;
  int token_c;

  *input_length = 0;
  *input_position = 0;
  sl_status_t status = sl_cli_tokenize(handle->input_buffer, &token_c, token_v);
  if (status != SL_STATUS_OK) {
    return 0;
  }

  int i = 0;
  int arg_ofs = 0;
  int number_of_matches = 0;
  *input_length = strlen(token_v[0]);

  sl_cli_command_group_t *cmd_group;
  const sl_cli_command_entry_t *table;
  SL_SLIST_FOR_EACH_ENTRY(handle->command_group, cmd_group, sl_cli_command_group_t, node) {
    table = cmd_group->command_table;
    if (table == NULL) {
      continue;
    }

    // If input buffer begins with spaces, increment input position to match
    while (handle->input_buffer[i] == ' ') {
      *input_position = *input_position + 1;
      i++;
    }

    // Look for possible command matches in command table and within groups.
    // Add all possible matches to possible_matches string
    i = 0;
    while ((table[i].name != NULL) && (arg_ofs <= token_c)) {
      if (table[i].is_shortcut) {
        // Ignore shortcuts
        i++;
        continue;
      }
      *input_length = handle->input_len - *input_position;
      if (strncmp(token_v[arg_ofs], table[i].name, *input_length) == 0) {
        int table_entry_length = strlen(table[i].name);
        if ((table[i].command->arg_type_list[0] == SL_CLI_ARG_GROUP) && (arg_ofs != (token_c - 1))) {
          table = (sl_cli_command_entry_t *)(table[i].command->function);
          i = 0;
          *input_position += strlen(token_v[arg_ofs]) + 1;
          while (handle->input_buffer[*input_position] == '\0') {
            *input_position = *input_position + 1;
          }
          arg_ofs++;
          continue;
        } else if (handle->input_len - *input_position <= table_entry_length) {
          // Will only show the total possible matches up to length of the
          // input buffer, and add "..." if there are more possible matches
          if (strlen(possible_matches) + strlen(table[i].name)
              >= SL_CLI_INPUT_BUFFER_SIZE - 4) {
            sl_strcat_s(possible_matches, possible_matches_size, "... ");
            break;
          }
          number_of_matches++;
          sl_strcat_s(possible_matches, possible_matches_size, table[i].name);
          sl_strcat_s(possible_matches, possible_matches_size, " ");
        } else if ((table[i].command->arg_type_list[0] == SL_CLI_ARG_GROUP) && (arg_ofs == (token_c - 1))) {
          int k = 0;
          table = (sl_cli_command_entry_t *)(table[i].command->function);
          while (table[k].name) {
            sl_strcat_s(possible_matches, possible_matches_size, table[k].name);
            sl_strcat_s(possible_matches, possible_matches_size, " ");
            number_of_matches++;
            k++;
          }
          *input_position = original_input_buffer_len;
          *input_length = 0;
          break;
        }
      }
      i++;
    }

    // The tokenizer will replace all spaces with '\0' in the input buffer,
    // these need to be converted back to spaces
    buf = handle->input_buffer;
    len = original_input_buffer_len;
    while (len > 0) {
      if (*buf == '\0') {
        *buf = ' ';
      }
      buf++;
      len--;
    }
  }

  return number_of_matches;
}
#endif // SL_CLI_ADVANCED_INPUT_HANDLING

/**
 * @brief The `scan_entry` function searches for a command or command group in a command entry list based on tokenized input and updates flags and offsets accordingly.
 *
 * @param cmd_entry_in A pointer to the initial command entry to start the search from.
 * @param group A boolean indicating if the current search context is within a command group.
 * @param found A pointer to a boolean that will be set to true if a matching command or group is found.
 * @param token_c A pointer to an integer representing the count of tokens in the input.
 * @param token_v An array of strings representing the tokenized input arguments.
 * @param arg_ofs A pointer to an integer offset indicating the current position in the token array.
 * @param single_flag A pointer to a boolean that will be set to true if a single command is found.
 * @param help_flag A pointer to a boolean that will be set to true if help information is requested or needed.
 * @return Returns a pointer to the command entry that matches the input tokens, or NULL if no match is found.
 */
static const sl_cli_command_entry_t *scan_entry(const sl_cli_command_entry_t *cmd_entry_in,
                                                bool group,
                                                bool *found,
                                                int *token_c,
                                                char *token_v[],
                                                int *arg_ofs,
                                                bool *single_flag,
                                                bool *help_flag)
{
  const sl_cli_command_entry_t *cmd_entry = cmd_entry_in;

  while ((cmd_entry->name != NULL) && (*arg_ofs < *token_c)) {
    if (cmd_strcmp(cmd_entry->name, token_v[*arg_ofs]) == 0) {
      // Command or group found
      (*arg_ofs)++;
      if (cmd_entry->command->arg_type_list[0] == SL_CLI_ARG_GROUP) {
        // Group found, continue search
        cmd_entry = (sl_cli_command_entry_t *)(cmd_entry->command->function);
        cmd_entry = scan_entry(cmd_entry, true, found, token_c, token_v, arg_ofs, single_flag, help_flag);
        break;
      } else {
        // Command found, stop search
        *single_flag = true;
        *found = true;
        break;
      }
    }
    cmd_entry++;
  }

  if (!(*found) && (!(*help_flag))) {
    if (group) {
      *help_flag = true;
      cmd_entry = cmd_entry_in;
      *found = true;
    } else {
      cmd_entry = NULL;
    }
  }

  return cmd_entry;
}

/**
 * @brief The `sl_cli_command_find` function searches for a command entry in a CLI command group based on input tokens and sets flags for help or single command execution.
 *
 * @param handle A handle to the CLI instance containing command groups.
 * @param token_c A pointer to an integer representing the count of tokens in the input.
 * @param token_v An array of strings representing the tokens from the input command line.
 * @param arg_ofs A pointer to an integer that will be updated to indicate the offset of the current argument being processed.
 * @param single_flag A pointer to a boolean that will be set to true if a single command is found.
 * @param help_flag A pointer to a boolean that will be set to true if the help command is detected.
 * @return Returns a pointer to the found `sl_cli_command_entry_t` if a command is found, otherwise returns NULL.
 */
const sl_cli_command_entry_t *sl_cli_command_find(sl_cli_handle_t handle,
                                                  int *token_c,
                                                  char *token_v[],
                                                  int *arg_ofs,
                                                  bool *single_flag,
                                                  bool *help_flag)
{
  const sl_cli_command_entry_t *cmd_entry = NULL;
  bool help = false;
  bool found = false;

  *arg_ofs = 0;
  *single_flag = false;
  *help_flag = false;

  if (cmd_strcmp("help", token_v[*arg_ofs]) == 0) {
    help = true;
    (*arg_ofs)++;
  }

  sl_cli_command_group_t *cmd_group;
  SL_SLIST_FOR_EACH_ENTRY(handle->command_group, cmd_group, sl_cli_command_group_t, node) {
    cmd_entry = cmd_group->command_table;
    if (cmd_entry == NULL) {
      continue;
    }
    cmd_entry = scan_entry(cmd_entry, false, &found, token_c, token_v, arg_ofs, single_flag, help_flag);
    if (found) {
      break;
    }
  }
  if (help) {
    *help_flag = true;
  }

  return cmd_entry;
}

/**
 * @brief The `sl_cli_command_execute` function processes a command input string, finds the corresponding command, converts its arguments, and executes the command function.
 *
 * @param handle A handle to the CLI context, which contains command groups and other CLI-related data.
 * @param input A string containing the command and its arguments to be executed.
 * @return The function returns an `sl_status_t` status code indicating the success or failure of the command execution process.
 */
sl_status_t sl_cli_command_execute(sl_cli_handle_t handle,
                                   char *input)
{
  char *token_v[SL_CLI_MAX_INPUT_ARGUMENTS];
  int token_c;
  uint32_t memory_array[SL_CLI_MAX_INPUT_ARGUMENTS];
  void *argv[SL_CLI_MAX_INPUT_ARGUMENTS];
  sl_cli_command_arg_t arguments;
  int arg_ofs;

  // Split input string
  sl_status_t status = sl_cli_tokenize(input, &token_c, token_v);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Find command
  bool help_flag = false;
  bool single_flag;
  const sl_cli_command_entry_t *cmd_table = sl_cli_command_find(handle,
                                                                &token_c,
                                                                token_v,
                                                                &arg_ofs,
                                                                &single_flag,
                                                                &help_flag);
  if (help_flag) {
    cmd_help(handle, cmd_table, single_flag);
    return SL_STATUS_OK;
  } else if (cmd_table == NULL) {
    return SL_STATUS_NOT_FOUND;
  }

  // Convert arguments to correct type
  const sl_cli_command_info_t *cmd_info = cmd_table->command;
  status = sli_cli_arguments_convert_multiple(cmd_info->arg_type_list,
                                              token_c,
                                              token_v,
                                              arg_ofs,
                                              argv,
                                              memory_array);
  if (status != SL_STATUS_OK) {
    return status;
  }

  // Call function
  arguments.handle = handle;
  arguments.argc = token_c;
  arguments.argv = argv;
  arguments.arg_ofs = arg_ofs;
  arguments.arg_type_list = cmd_info->arg_type_list;

  sli_cli_pre_cmd_hook(&arguments);
  cmd_info->function(&arguments);
  sli_cli_post_cmd_hook(&arguments);

  // Command executed, return status that in this case is success
  return status;
}
