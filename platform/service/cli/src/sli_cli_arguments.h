/***************************************************************************//**
 * @file
 * @brief Functions for argument parsing in the CLI framework.
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

#ifndef SLI_CLI_ARGUMENTS_H
#define SLI_CLI_ARGUMENTS_H

#include <stdint.h>
#include <stdbool.h>

#include "sl_cli_config.h"
#include "sl_cli_types.h"
#include "sl_cli_arguments.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function checks whether a given string argument represents a
 * valid unsigned integer within the constraints of a specified bit size.
 * It supports both decimal and hexadecimal formats. The size parameter
 * must be a multiple of four or one less than a multiple of four, and
 * the function ensures that the argument fits within the specified bit
 * size. The neg parameter should be true if the original number was
 * negative, allowing for validation of negative values by adjusting the
 * magnitude check. This function is useful in command-line interfaces
 * where input validation is necessary to ensure correct data types and
 * sizes.
 *
 * @param size Size of the unsigned integer in bits; must be a multiple of four
 * or one less than a multiple of four.
 * @param argument String representing the number to validate; can be in decimal
 * or hexadecimal format. Must not be null.
 * @param neg Boolean indicating if the original number was negative; affects
 * validation of magnitude for negative values.
 * @return Returns a boolean indicating whether the argument is a valid unsigned
 * integer of the specified size.
 ******************************************************************************/
bool sli_cli_arguments_validate_uint(int size,
                                     const char *argument,
                                     bool neg);

/***************************************************************************//**
 * @brief This function checks if a given string represents a valid signed
 * integer of a specified bit size. It is useful when parsing command-
 * line arguments to ensure they conform to expected integer formats. The
 * function should be called with the size of the integer in bits, which
 * must be a multiple of four. The argument string can be in decimal or
 * hexadecimal format. The function handles both positive and negative
 * numbers, using a helper function to validate the unsigned portion of
 * the number. It returns a boolean indicating the validity of the
 * argument.
 *
 * @param size Size of the integer variable in bits, which must be a multiple of
 * four. This parameter determines the range of valid integer
 * values.
 * @param argument A string representing the integer to be validated. It can be
 * in decimal or hexadecimal format. The string must not be
 * null.
 * @return A boolean value indicating whether the argument is a valid signed
 * integer of the specified size.
 ******************************************************************************/
bool sli_cli_arguments_validate_int(int size,
                                    const char *argument);

/***************************************************************************//**
 * @brief This function checks if a given string is a valid hex string wrapped
 * in curly braces and containing an even number of hex digits. It is
 * useful for validating input strings that are expected to represent
 * hexadecimal values. The string must start with '{' and end with '}',
 * and it can contain spaces between hex digits. The function returns
 * false if the string does not meet these criteria or if it contains
 * invalid characters.
 *
 * @param argument A pointer to a null-terminated string that represents the
 * argument to be validated. The string must start with '{' and
 * end with '}', and it can contain spaces and valid hex digits
 * (0-9, a-f, A-F) only. The caller retains ownership of the
 * string, and it must not be null.
 * @return A boolean value indicating whether the argument is a valid hex
 * string. Returns true if valid, false otherwise.
 ******************************************************************************/
bool sli_cli_arguments_validate_hex_string(const char *argument);

/***************************************************************************//**
 * @brief This function checks if a given argument string is valid according to
 * the specified argument type. It is useful in command-line interface
 * (CLI) applications where arguments need to be validated before
 * processing. The function supports various types including unsigned and
 * signed integers of different sizes, strings, and hexadecimal values.
 * It returns a boolean indicating the validity of the argument. The
 * function should be called with a valid argument type and a non-null
 * argument string. If an unsupported type is provided, the function will
 * return false.
 *
 * @param type The type of the argument to validate, specified as
 * sl_cli_argument_type_t. It determines which validation function
 * to use. Must be a valid type defined in the CLI framework.
 * @param argument A pointer to the argument string to be validated. Must not be
 * null. The string is checked against the specified type for
 * validity.
 * @return A boolean value indicating whether the argument is valid for the
 * specified type. Returns true if valid, false otherwise.
 ******************************************************************************/
bool sli_cli_validate_argument(sl_cli_argument_type_t type,
                               const char *argument);

/***************************************************************************//**
 * @brief This function processes a single command-line interface (CLI) argument
 * by converting it according to its specified type and storing it in a
 * provided memory array. It then updates an array of argument pointers
 * to reference the converted value. This function is useful when parsing
 * and handling CLI inputs that may be of various types, such as
 * integers, strings, or hexadecimal values. It requires the caller to
 * manage indices for both the argument pointer array and the memory
 * array, ensuring that they are correctly incremented after storing each
 * argument. The function assumes a little-endian byte format and handles
 * different argument types, including unsigned and signed integers,
 * strings, and hex values. It does not perform any conversion for
 * wildcard arguments and will not modify indices if the argument type is
 * unknown.
 *
 * @param type Specifies the type of the argument to be converted, using the
 * sl_cli_argument_type_t enumeration. It determines how the
 * argument string will be processed.
 * @param argument A string representing the argument to be converted. It must
 * be a valid representation of the specified type. The caller
 * retains ownership, and the function may modify it for certain
 * types like hex.
 * @param argv An array of pointers where the function will store a pointer to
 * the converted argument. The caller must ensure this array is
 * large enough to hold the pointer.
 * @param memory_array An array where the function will store the converted
 * argument's value, if applicable. The caller must ensure
 * this array is large enough to hold the converted data.
 * @param arg_index A pointer to an integer that tracks the current index in the
 * argv array. The function increments this index after storing
 * the pointer to the converted argument.
 * @param mem_index A pointer to an integer that tracks the current index in the
 * memory_array. The function increments this index after
 * storing the converted argument, if applicable.
 * @return None
 ******************************************************************************/
void sli_cli_arguments_convert_single(sl_cli_argument_type_t type,
                                      char *argument,
                                      void *argv[],
                                      uint32_t *memory_array,
                                      int *arg_index,
                                      int *mem_index);

/***************************************************************************//**
 * @brief This function processes a list of command-line interface (CLI)
 * arguments, converting them according to a specified list of argument
 * types. It is used when parsing and validating multiple arguments
 * passed to a CLI command. The function expects a list of argument
 * types, the number of arguments, and the arguments themselves as input.
 * It then converts these arguments, storing pointers to them in the
 * provided `argv` array and storing any numerical arguments byte-for-
 * byte in the `memory_array`. The function handles optional, additional,
 * and wildcard argument types, ensuring that the number of arguments
 * matches expectations. It returns a status code indicating success or
 * the type of error encountered, such as invalid argument type or count.
 *
 * @param arg_type_list An array of `sl_cli_argument_type_t` values specifying
 * the expected types for each argument. The list must end
 * with `SL_CLI_ARG_END`.
 * @param token_c The number of arguments in the `token_v` array. Must be
 * greater than or equal to `arg_ofs`.
 * @param token_v An array of strings representing the arguments to be
 * converted. The array must contain at least `token_c` elements.
 * @param arg_ofs The offset in the `token_v` array where the command arguments
 * start. Must be less than or equal to `token_c`.
 * @param argv An output array where pointers to the converted arguments will be
 * stored. The caller must ensure it is large enough to hold all
 * pointers.
 * @param memory_array An output array where numerical arguments will be stored
 * byte-for-byte. The caller must ensure it is large enough
 * to hold all converted data.
 * @return Returns `SL_STATUS_OK` on success, `SL_STATUS_INVALID_TYPE` if an
 * argument is invalid, `SL_STATUS_INVALID_COUNT` if the number of
 * arguments is incorrect, and `SL_STATUS_INVALID_PARAMETER` for
 * internal errors.
 ******************************************************************************/
sl_status_t sli_cli_arguments_convert_multiple(const sl_cli_argument_type_t *argument_type_list,
                                               const int token_c,
                                               char *token_v[],
                                               int arg_ofs,
                                               void *argv[],
                                               uint32_t *memory_array);

#ifdef __cplusplus
}
#endif

#endif // SLI_CLI_ARGUMENTS_H
