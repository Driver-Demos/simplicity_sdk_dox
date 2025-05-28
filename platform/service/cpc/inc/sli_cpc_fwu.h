/***************************************************************************/ /**
 * @file
 * @brief CPC Firmware Upgrade Driver Interface
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_CPC_DRV_FWU_H
#define SLI_CPC_DRV_FWU_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "sl_status.h"
#include "sl_common.h"

#include "sl_cpc_primary_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************//**
 * @brief This function is part of the main CPC process action and is
 * responsible for managing the firmware upgrade process. It should be
 * called regularly to ensure the firmware upgrade progresses correctly.
 * The function checks if a firmware upgrade is in progress and whether
 * the bootloader is running. If the bootloader is not running and
 * recovery pins support is not enabled, it attempts to reboot the
 * secondary into bootloader mode. Once the bootloader is running, it
 * initializes the firmware upgrade driver and executes the firmware
 * upgrade state machine. The function stops processing once the firmware
 * upgrade is complete or fails. It is essential to ensure that the
 * firmware upgrade process has been started before calling this
 * function.
 *
 * @return None
 ******************************************************************************/
void sli_cpc_fwu_process_action(void);

/***************************************************************************//**
 * @brief This function checks the status of the firmware upgrade process and
 * returns a boolean indicating whether the upgrade is currently in
 * progress. It is useful for monitoring the state of the firmware
 * upgrade and ensuring that other operations do not interfere with the
 * upgrade process. This function can be called at any time to verify the
 * upgrade status, and it does not require any prior initialization or
 * setup.
 *
 * @return A boolean value: 'true' if a firmware upgrade is in progress, 'false'
 * otherwise.
 ******************************************************************************/
bool sli_cpc_is_fwu_in_progress(void);

/***************************************************************************//**
 * @brief Use this function to determine if the bootloader is active by checking
 * for a response to a specific probe. It should be called when you need
 * to verify the bootloader's status, typically before initiating
 * firmware upgrade processes. The function caches the result after the
 * first call, so subsequent calls will return the cached status without
 * re-probing. This function is useful in scenarios where knowing the
 * bootloader state is critical for further operations.
 *
 * @return Returns true if the bootloader is running and responded to the probe,
 * otherwise returns false.
 ******************************************************************************/
bool sli_cpc_is_bootloader_running(void);

/***************************************************************************//**
 * @brief This function is part of a finite state machine that manages the
 * firmware upgrade process for a device. It should be called repeatedly
 * by the main process to progress through the various stages of the
 * firmware upgrade. The function handles different states such as
 * waiting for prompts, sending data frames, and acknowledging responses.
 * It is essential to call this function until it returns a status
 * indicating that the firmware upgrade is complete. The function assumes
 * that the firmware upgrade process has been properly initialized and
 * started.
 *
 * @return Returns an sl_status_t indicating the current status of the firmware
 * upgrade process: SL_STATUS_IN_PROGRESS if the upgrade is ongoing and
 * SL_STATUS_OK if the upgrade is complete.
 ******************************************************************************/
sl_status_t sli_cpc_drv_fwu_step(void);

/***************************************************************************//**
 * @brief This function prepares the firmware update driver for operation by
 * enabling the UART peripheral. It should be called before any firmware
 * update operations are performed to ensure that the necessary hardware
 * interfaces are correctly configured and ready for use. This function
 * does not take any parameters and does not return any value, indicating
 * that it is intended to be a straightforward initialization step in the
 * firmware update process.
 *
 * @return None
 ******************************************************************************/
void sli_cpc_drv_fwu_init();

/***************************************************************************//**
 * @brief This function is used to start the firmware upgrade process for a
 * secondary device in a CPC (Co-Processor Communication) system. It
 * should be called when the user is ready to begin the firmware upgrade.
 * Before calling this function, ensure that the system is in a state
 * where a firmware upgrade can be initiated, as determined by the
 * `sl_cpc_is_fwu_startable` function. If the upgrade process can be
 * started, the function will activate the firmware upgrade process,
 * blocking the CPC API and continuously calling the user-implemented
 * `sl_cpc_get_fwu_chunk` callback until the firmware image is fully
 * transferred. If the function returns a status other than
 * `SL_STATUS_OK`, it indicates that the upgrade process could not be
 * started, and the return value will provide the reason.
 *
 * @return Returns `SL_STATUS_OK` if the firmware upgrade process starts
 * successfully. If not, it returns the status from
 * `sl_cpc_is_fwu_startable`, indicating why the process could not be
 * started.
 ******************************************************************************/
sl_status_t sl_cpc_fwu_start(void);

/***************************************************************************//**
 * @brief This function determines whether the conditions are met to initiate a
 * firmware upgrade process. It should be called before attempting to
 * start a firmware upgrade to ensure that the system is ready and
 * capable of proceeding. The function checks for ongoing firmware
 * upgrades, the presence of necessary user-implemented callbacks, and
 * the initialization state of the system. It also considers the status
 * of the bootloader and any required reset sequences. This function is
 * essential for preventing premature or unsupported upgrade attempts.
 *
 * @return Returns an sl_status_t indicating the readiness for a firmware
 * upgrade: SL_STATUS_OK if startable, SL_STATUS_IN_PROGRESS if an
 * upgrade is ongoing, SL_STATUS_NOT_READY if prerequisites are not met,
 * SL_STATUS_NOT_AVAILABLE if a required callback is missing, and
 * SL_STATUS_NOT_SUPPORTED if the bootloader is unsuitable.
 ******************************************************************************/
sl_status_t sl_cpc_is_fwu_startable(void);

#if (SL_CPC_PRIMARY_FIRMWARE_UPGRADE_RECOVERY_PINS_SUPPORT_ENABLED >= 1)
/***************************************************************************//**
 * @brief This function is used to reset the secondary device and force it into
 * bootloader mode by manipulating the bootloader recovery pins. It is
 * typically called when a firmware upgrade is required, and the device
 * needs to be in bootloader mode to accept the new firmware. This
 * function should be used when the system supports bootloader recovery
 * pins and the firmware upgrade process is initiated. It is important to
 * ensure that the system is configured to support this operation, as
 * indicated by the
 * SL_CPC_PRIMARY_FIRMWARE_UPGRADE_RECOVERY_PINS_SUPPORT_ENABLED macro.
 *
 * @return None
 ******************************************************************************/
void sli_cpc_drv_fwu_enter_bootloader_via_recovery_pins(void);
#endif

/***************************************************************************//**
 * Probe for the presence of a bootloader prompt at the remote device.
 ******************************************************************************/
sl_status_t sli_cpc_fwu_probe_bootloader(void);

/** @} (end addtogroup cpc) */

#ifdef __cplusplus
}
#endif

#endif // SLI_CPC_DRV_FWU_H
