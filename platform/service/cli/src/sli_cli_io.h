/***************************************************************************//**
 * @file
 * @brief CLI io functions.
 *    All cli input and output will go through these functions.
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

#ifndef SLI_CLI_IO_H
#define SLI_CLI_IO_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function is used to read a single character from the standard
 * input stream. It is typically called when there is a need to process
 * user input or data from the standard input. The function will return
 * the character read if successful, or EOF if no character is available
 * or an error occurs. It is important to check the return value to
 * handle cases where EOF is returned, indicating that no character could
 * be read.
 *
 * @return Returns the input character as an integer, or EOF if no character is
 * available or an error occurs.
 ******************************************************************************/
int sli_cli_io_getchar(void);

/***************************************************************************//**
 * @brief This function is used to send a single character to the standard
 * output stream. It is typically called when there is a need to output
 * characters individually, such as in low-level I/O operations or when
 * implementing custom output functions. The function should be used when
 * the standard output stream is properly initialized and available. It
 * returns the character on success, allowing the caller to verify the
 * output operation, or EOF if an error occurs, indicating that the
 * character could not be output.
 *
 * @param ch The character to be output. It is an integer value representing the
 * character to be sent to the standard output. The function expects a
 * valid character value and does not perform any validation on the
 * input.
 * @return Returns the character if successful, or EOF if an error occurs during
 * the output operation.
 ******************************************************************************/
int sli_cli_io_putchar(int ch);

/***************************************************************************//**
 * @brief This function is used to output a formatted string to the standard
 * input/output stream, similar to the standard printf function. It
 * should be used when you need to display formatted text to the console
 * or a similar output device. The function takes a format string
 * followed by a variable number of arguments, which are used to format
 * the output. It is important to ensure that the format string and the
 * arguments match in type and number to avoid undefined behavior. The
 * function always returns 0, indicating successful execution.
 *
 * @param format A pointer to a null-terminated string that specifies how
 * subsequent arguments are converted for output. The format
 * string must not be null, and it should follow the same rules as
 * the standard printf format strings.
 * @return Returns 0, indicating successful execution.
 ******************************************************************************/
int sli_cli_io_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif // SLI_CLI_IO_H
