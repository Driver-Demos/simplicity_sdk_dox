/***************************************************************************//**
 * @file
 * @brief CLI simple password protection.
 * @version x.y.z
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

#include "sl_cli_types.h"

#ifndef SL_CLI_SIMPLE_PASSWORD_H
#define SL_CLI_SIMPLE_PASSWORD_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cli
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function initializes the CLI simple password protection module,
 * setting up necessary cryptographic keys and state. It should be called
 * before any password-related operations to ensure the module is
 * properly configured. The function handles the presence of existing
 * keys and initializes new ones if necessary. It also sets initial
 * values for security-related flags and counters. This function must be
 * called once during the application startup to ensure the password
 * protection mechanism is ready for use.
 *
 * @return Returns an sl_status_t indicating the success or failure of the
 * initialization process.
 ******************************************************************************/
sl_status_t sl_cli_simple_password_init(void);

/***************************************************************************//**
 * @brief This function is used to log out a user from a command line interface
 * (CLI) session associated with the provided handle. It should be called
 * when a user needs to be logged out, either due to user action or
 * session timeout. The function updates the session state based on the
 * current module configuration and ensures that the CLI does not prompt
 * for input or block sleep if the kernel is not present. It is important
 * to ensure that the handle provided is valid and corresponds to an
 * active session.
 *
 * @param handle A handle to the CLI session. It must be a valid handle
 * associated with an active session. The function does not
 * perform any action if the handle is invalid or null.
 * @return Returns SL_STATUS_OK to indicate the logout operation was processed.
 ******************************************************************************/
sl_status_t sl_cli_simple_password_logout(sl_cli_handle_t handle);

/***************************************************************************//**
 * @brief This function sets a new password for CLI access by hashing the
 * provided password and storing the encrypted hash securely. It should
 * be used when a new password needs to be configured for CLI protection.
 * The function requires the password to meet a minimum length
 * requirement, and it will return an error status if this condition is
 * not met. The function ensures that the password is securely processed
 * and stored, and it updates the module state to reflect the new
 * configuration.
 *
 * @param new_password A string representing the new password to be set. It must
 * be at least SL_CLI_SIMPLE_PASSWORD_LENGTH_MIN characters
 * long. The caller retains ownership of the string, and it
 * must not be null. If the password is shorter than the
 * minimum length, the function returns an error status.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while
 * SL_STATUS_INVALID_SIGNATURE is returned if the password is too short.
 ******************************************************************************/
sl_status_t sl_cli_set_simple_password(char *new_password);

/***************************************************************************//**
 * @brief This function is used to destroy the existing password key,
 * effectively invalidating the current password and requiring the user
 * to set a new one. It should be called when the password needs to be
 * reset or when security policies dictate that a new password must be
 * configured. The function ensures that the key is securely destroyed
 * and the system is set to a state where a new password configuration is
 * necessary. It is important to ensure that any necessary preparations
 * for password reconfiguration are in place before calling this
 * function.
 *
 * @return Returns an operation status code indicating success or failure of the
 * key destruction process.
 ******************************************************************************/
sl_status_t sl_cli_simple_password_destroy_key(void);

/***************************************************************************//**
 * @brief This function is used to reset the security warning flag within the
 * CLI simple password protection module. It should be called when the
 * security warning condition has been acknowledged and cleared, ensuring
 * that the system no longer indicates a security warning state. This
 * function does not require any parameters and is expected to be called
 * in contexts where the security warning needs to be reset to false. It
 * returns a status indicating the success of the operation.
 *
 * @return Returns an sl_status_t value indicating the success of the operation,
 * typically SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t sl_cli_reset_security_warning_flag(void);

/** @} (end addtogroup cli) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLI_SIMPLE_PASSWORD_H
