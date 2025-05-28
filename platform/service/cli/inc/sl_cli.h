/***************************************************************************//**
 * @file
 * @brief Backbone of CLI framework
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

#ifndef SL_CLI_H
#define SL_CLI_H

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "sl_cli_types.h"
#include "sl_cli_command.h"
#include "sl_cli_arguments.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cli
 * @{
 ******************************************************************************/
/***************************************************************************//**
 * @brief
 *  Enable the CLI tick
 *
 * @details
 *  Trigger the creation of a tick task or the implentation of the tick handling
 *  function.
 *
 ******************************************************************************/
#ifndef SL_CLI_TICK_ENABLE
#define SL_CLI_TICK_ENABLE 1
#endif

/***************************************************************************//**
 * @brief
 *  Obtain the number of command arguments.
 *
 * @details
 *  Users obtain the number of command arguments with this macro.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @return
 *   The number of command arguments.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the number of arguments (after the command strings).
 *     int arg_count = sl_cli_get_argument_count(args); // arg_count = 4
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_count(a)      ((int)((a)->argc - (a)->arg_ofs))

/***************************************************************************//**
 * @brief
 *  Obtain the type of argument.
 *
 * @details
 *  Users obtain the type of the argument with this macro.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   enum of the argument type.
 *
 * @details
 *   For auto-generated commands, it could be useful in the handler to know
 *   the type of the argument.
 *
 ******************************************************************************/
#define sl_cli_get_argument_type(a, n)      ((sl_cli_arg_t)((a)->arg_type_list[(n)]))

/***************************************************************************//**
 * @brief
 *  Obtain int8 arguments.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   The argument value.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the arguments (after the command strings).
 *     char *text_ptr = sl_cli_get_argument_string(args, 0);  // char ptr to
 *     // "hello"
 *     uint8_t num1_u8 = sl_cli_get_argument_uint8(args, 1);  // num1_u8 = 255
 *     int16_t num2_i16 = sl_cli_get_argument_int16(args, 2); // num2_i16 = -100
 *     size_t hex_array_size;
 *     uint8_t *hex_array_ptr = sl_cli_get_argument_hex(args, 3, &hex_array_size);
 *     // uint8_t ptr to {0xAA, 0xBB, 0xCC}, hex_array_size = 3
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_int8(a, n)    (*(int8_t *)((a)->argv[(a)->arg_ofs + (n)]))

/***************************************************************************//**
 * @brief
 *  Obtain int16 arguments.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   The argument value.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the arguments (after the command strings).
 *     char *text_ptr = sl_cli_get_argument_string(args, 0);  // char ptr to
 *     // "hello"
 *     uint8_t num1_u8 = sl_cli_get_argument_uint8(args, 1);  // num1_u8 = 255
 *     int16_t num2_i16 = sl_cli_get_argument_int16(args, 2); // num2_i16 = -100
 *     size_t hex_array_size;
 *     uint8_t *hex_array_ptr = sl_cli_get_argument_hex(args, 3, &hex_array_size);
 *     // uint8_t ptr to {0xAA, 0xBB, 0xCC}, hex_array_size = 3
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_int16(a, n)   (*(int16_t *)((a)->argv[(a)->arg_ofs + (n)]))

/***************************************************************************//**
 * @brief
 *  Obtain int32 arguments.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   The argument value.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the arguments (after the command strings).
 *     char *text_ptr = sl_cli_get_argument_string(args, 0);  // char ptr to
 *     // "hello"
 *     uint8_t num1_u8 = sl_cli_get_argument_uint8(args, 1);  // num1_u8 = 255
 *     int16_t num2_i16 = sl_cli_get_argument_int16(args, 2); // num2_i16 = -100
 *     size_t hex_array_size;
 *     uint8_t *hex_array_ptr = sl_cli_get_argument_hex(args, 3, &hex_array_size);
 *     // uint8_t ptr to {0xAA, 0xBB, 0xCC}, hex_array_size = 3
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_int32(a, n)   (*(int32_t *)((a)->argv[(a)->arg_ofs + (n)]))

/***************************************************************************//**
 * @brief
 *  Obtain uint8 arguments.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   The argument value.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the arguments (after the command strings).
 *     char *text_ptr = sl_cli_get_argument_string(args, 0);  // char ptr to
 *     // "hello"
 *     uint8_t num1_u8 = sl_cli_get_argument_uint8(args, 1);  // num1_u8 = 255
 *     int16_t num2_i16 = sl_cli_get_argument_int16(args, 2); // num2_i16 = -100
 *     size_t hex_array_size;
 *     uint8_t *hex_array_ptr = sl_cli_get_argument_hex(args, 3, &hex_array_size);
 *     // uint8_t ptr to {0xAA, 0xBB, 0xCC}, hex_array_size = 3
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_uint8(a, n)   (*(uint8_t *)((a)->argv[(a)->arg_ofs + (n)]))

/***************************************************************************//**
 * @brief
 *  Obtain uint16 arguments.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   The argument value.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the arguments (after the command strings).
 *     char *text_ptr = sl_cli_get_argument_string(args, 0);  // char ptr to
 *     // "hello"
 *     uint8_t num1_u8 = sl_cli_get_argument_uint8(args, 1);  // num1_u8 = 255
 *     int16_t num2_i16 = sl_cli_get_argument_int16(args, 2); // num2_i16 = -100
 *     size_t hex_array_size;
 *     uint8_t *hex_array_ptr = sl_cli_get_argument_hex(args, 3, &hex_array_size);
 *     // uint8_t ptr to {0xAA, 0xBB, 0xCC}, hex_array_size = 3
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_uint16(a, n)  (*(uint16_t *)((a)->argv[(a)->arg_ofs + (n)]))

/***************************************************************************//**
 * @brief
 *  Obtain uint32 arguments.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   The argument value.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the arguments (after the command strings).
 *     char *text_ptr = sl_cli_get_argument_string(args, 0);  // char ptr to
 *     // "hello"
 *     uint8_t num1_u8 = sl_cli_get_argument_uint8(args, 1);  // num1_u8 = 255
 *     int16_t num2_i16 = sl_cli_get_argument_int16(args, 2); // num2_i16 = -100
 *     size_t hex_array_size;
 *     uint8_t *hex_array_ptr = sl_cli_get_argument_hex(args, 3, &hex_array_size);
 *     // uint8_t ptr to {0xAA, 0xBB, 0xCC}, hex_array_size = 3
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_uint32(a, n)  (*(uint32_t *)((a)->argv[(a)->arg_ofs + (n)]))

/***************************************************************************//**
 * @brief
 *  Obtain string arguments.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The argument number. The first argument is the number 0, the next 1, etc.
 *
 * @return
 *   The argument value.
 *   For hex arguments, the return value is a pointer to a buffer containing
 *   decoded values. For string arguments, the return value is a pointer to the
 *   string.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the arguments (after the command strings).
 *     char *text_ptr = sl_cli_get_argument_string(args, 0);  // char ptr to
 *     // "hello"
 *     uint8_t num1_u8 = sl_cli_get_argument_uint8(args, 1);  // num1_u8 = 255
 *     int16_t num2_i16 = sl_cli_get_argument_int16(args, 2); // num2_i16 = -100
 *     size_t hex_array_size;
 *     uint8_t *hex_array_ptr = sl_cli_get_argument_hex(args, 3, &hex_array_size);
 *     // uint8_t ptr to {0xAA, 0xBB, 0xCC}, hex_array_size = 3
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_argument_string(a, n)  ((char *)((a)->argv[(a)->arg_ofs + (n)]))

/***************************************************************************//**
 * @brief
 *  Obtain the number of command strings.
 *
 * @details
 *  Users obtain the number of command strings with this macro.
 *
 * @param[in] a
 *   The number of command strings.
 *
 * @return
 *   The number of command strings.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the number of command strings (before the arguments).
 *     int cmd_count = sl_cli_get_command_count(args); // cmd_count = 2
 *     // (first string being "example_sub_menu" and second "example_command")
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_command_count(a)       ((int)((a)->arg_ofs))

/***************************************************************************//**
 * @brief
 *  Obtain the command string(s).
 *
 * @details
 *  Users get the command string with this macro. If the command is
 *  used in a group, both the group string(s) and command string can be obtained.
 *  The total number of command strings is available in the command argument
 *  arg_ofs variable.
 *
 * @param[in] a
 *   A pointer to the command arguments.
 *
 * @param[in] n
 *   The command string number. The first string is number 0, the next 1, etc.
 *
 * @details
 *   An example could be the CLI input of
 *   `example_sub_menu example_command hello 255 -100 {AA BB CC}`
 *   with corresponding C code of:
 *   ```c
 *   void example_command(sl_cli_command_arg_t *args)
 *   {
 *     // Get the command strings (before the arguments).
 *     char *cmd1_ptr = sl_cli_get_command_string(args, 0); // char ptr to
 *     // "example_sub_menu"
 *     char *cmd2_ptr = sl_cli_get_command_string(args, 1); // char ptr to
 *     // "example_command"
 *   }
 *   ```
 ******************************************************************************/
#define sl_cli_get_command_string(a, n)   ((char *)((a)->argv[(n)]))

/***************************************************************************//**
 *  @brief The default CLI handle.
 ******************************************************************************/
extern sl_cli_handle_t sl_cli_default_handle;

/***************************************************************************//**
 *  @brief The default CLI command group.
 ******************************************************************************/
extern sl_cli_command_group_t *sl_cli_default_command_group;

/***************************************************************************//**
 * @brief This function is used to obtain a hex argument from a command's
 * argument list, along with its length. It is particularly useful when
 * dealing with command-line interfaces where arguments are passed in a
 * structured format. The function expects a valid command argument
 * structure and an index specifying which argument to retrieve. The
 * length of the hex data is stored in a two-byte format, and the
 * function will populate the provided length pointer with this value.
 * This function should be called only when the argument at the specified
 * index is known to be a hex type.
 *
 * @param a A pointer to the sl_cli_command_arg_t structure containing the
 * command arguments. Must not be null.
 * @param n The index of the argument to retrieve, starting from 0. Must be
 * within the range of available arguments.
 * @param l A pointer to a size_t variable where the length of the hex data will
 * be stored. Must not be null.
 * @return Returns a pointer to the buffer containing the hex data, starting
 * after the length bytes.
 ******************************************************************************/
uint8_t *sl_cli_get_argument_hex(sl_cli_command_arg_t *a, size_t n, size_t *l);

/***************************************************************************//**
 * @brief Use this function to reset a CLI handle to its default state,
 * preparing it for fresh input and command processing. This is typically
 * necessary when reinitializing the CLI or after a significant change in
 * the CLI's configuration. The function sets up the input buffer,
 * initializes the command group list, and configures default prompt
 * settings. It must be called with a valid CLI handle, and the handle
 * should not be null. This function does not return a value and does not
 * handle invalid handles.
 *
 * @param handle A valid handle to the CLI that will be cleared. Must not be
 * null, and the caller retains ownership.
 * @return None
 ******************************************************************************/
void sl_cli_clear(sl_cli_handle_t handle);

/***************************************************************************//**
 * @brief This function allows the redirection of user input from the default
 * CLI command handler to an alternative command function. It is useful
 * when you need to temporarily change the command processing behavior,
 * such as for custom command handling or interactive sessions. The
 * function should be called with a valid CLI handle and a non-null
 * command function to activate redirection. To stop redirection, pass
 * null for the command function, prompt, and user parameters. The prompt
 * parameter can be used to specify a custom command prompt during
 * redirection, and the user parameter can pass additional context to the
 * command function.
 *
 * @param handle A handle to the CLI instance. Must not be null.
 * @param command_function A pointer to the function that will receive user
 * input during redirection. Pass null to stop
 * redirection.
 * @param prompt A pointer to a string used as the command prompt during
 * redirection. Pass null to stop redirection.
 * @param user A pointer to user-defined data passed to the command function.
 * Pass null to stop redirection.
 * @return None
 ******************************************************************************/
void sl_cli_redirect_command(sl_cli_handle_t handle, sl_cli_command_function_t command_function, const char *prompt, void *aux);

/***************************************************************************//**
 * @brief This function processes a complete command line input for a CLI
 * instance, executing the command and its arguments as specified in the
 * input string. It should be used when a command line input needs to be
 * processed by the CLI framework. If a command function is set for
 * redirection, the input is passed to that function instead. The
 * function must be called with a valid CLI handle and a non-null command
 * string. It returns an execution status indicating success or failure
 * of the command execution.
 *
 * @param handle A handle to the CLI instance. It must be a valid, initialized
 * handle, and must not be null.
 * @param string A pointer to the string containing the command line to be
 * executed. It must not be null, and should contain a valid
 * command line input.
 * @return Returns an sl_status_t value indicating the execution status. If the
 * command is redirected, it always returns SL_STATUS_OK.
 ******************************************************************************/
sl_status_t sl_cli_handle_input(sl_cli_handle_t handle, char *string);

/***************************************************************************//**
 * @brief This function sets up a Command Line Interface (CLI) instance using
 * the provided parameters. It must be called to initialize a CLI
 * instance before any CLI operations can be performed. The function
 * configures the CLI with the specified I/O stream handle and default
 * command group. If the system supports a kernel and CLI tick is
 * enabled, it also sets up timing parameters for task scheduling. The
 * function returns a status indicating success or failure of the
 * initialization process.
 *
 * @param handle A handle to the CLI instance that will be initialized. Must not
 * be null, and the caller retains ownership.
 * @param parameters A pointer to a structure containing the parameters for the
 * CLI instance. Must not be null, and the caller retains
 * ownership. The structure should include valid I/O stream
 * handle and default command group.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * initialization process. Possible return values include SL_STATUS_OK
 * for success or an error code for failure.
 ******************************************************************************/
sl_status_t sl_cli_instance_init(sl_cli_handle_t handle,
                                 sl_cli_instance_parameters_t *parameters);

#if !defined(SL_CATALOG_KERNEL_PRESENT) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function checks whether the CLI instance is in a state that
 * permits the system to enter a sleep mode. It should be used in bare
 * metal configurations to decide if the system can safely sleep without
 * missing any CLI input or activity. The function evaluates the current
 * input state and any flags that might block sleep, returning a boolean
 * indicating the result. It is important to ensure that the CLI instance
 * is properly initialized before calling this function.
 *
 * @param handle A handle to the CLI instance. Must not be null, and should
 * point to a valid, initialized CLI instance. If the handle is
 * invalid, the behavior is undefined.
 * @return Returns a boolean value: true if the CLI allows the system to sleep,
 * false otherwise.
 ******************************************************************************/
bool sl_cli_is_ok_to_sleep(sl_cli_handle_t handle);

/***************************************************************************//**
 * @brief This function should be called periodically to handle time-dependent
 * operations for a CLI instance. It processes various tick-related tasks
 * depending on the configuration of the CLI, such as delay handling or
 * storage management. This function must be called in environments where
 * the CLI is used, especially in bare metal configurations, to ensure
 * proper operation of time-based features.
 *
 * @param handle A handle to the CLI instance. Must be a valid handle
 * initialized prior to calling this function. The function does
 * not check for null or invalid handles, so the caller must
 * ensure the handle is valid.
 * @return None
 ******************************************************************************/
void sl_cli_tick_instance(sl_cli_handle_t handle);
#endif

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup cli) */

/* *INDENT-OFF* */
/* THE REST OF THE FILE IS DOCUMENTATION ONLY! */
/***************************************************************************//**
 * @addtogroup cli Command Line Interface
 * @details The Command Line Interface (CLI) software component simplifies
 * creating command line user interfaces for programs. It handles tasks such as
 * receiving keyboard input, parsing the input strings for commands and
 * arguments, and finally calling the proper C function with decoded and
 * validated arguments. Functions can use standard output functions such as
 * printf to write information to the console. Many modules of the SDK have
 * built-in commands that are provided via CLI, users can even add the
 * custom commands if required.
 *
 * The Command Line Interface (CLI) depends on @ref iostream
 * for reading and writing bytes. You can set the I/O stream instance in the CLI
 * instance configuration.
 *
 ******************************************************************************/
#endif // SL_CLI_H
