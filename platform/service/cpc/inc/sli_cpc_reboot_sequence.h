/***************************************************************************/ /**
 * @file
 * @brief CPC Reboot sequence
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

#include <stdbool.h>

/***************************************************************************//**
 * @brief Use this function to determine if the reset bootloader sequence has
 * been completed successfully. This is useful in scenarios where you
 * need to verify that the system has rebooted into bootloader mode,
 * which may be necessary before performing certain operations like
 * firmware updates. The function returns a boolean value indicating the
 * completion status of the bootloader sequence.
 *
 * @return A boolean value: `true` if the bootloader sequence is done, `false`
 * otherwise.
 ******************************************************************************/
bool sli_cpc_reboot_is_bootloader_sequence_done(void);

/***************************************************************************//**
 * @brief This function manages the state transitions required to reboot a
 * secondary device in a CPC (Co-Processor Communication) system. It
 * should be called periodically to ensure the reboot sequence progresses
 * through its various states, handling communication and acknowledgments
 * with the secondary device. The function is designed to be used in a
 * bare-metal environment and does not return until the sequence is
 * complete or an error state is reached. It is essential to ensure that
 * the system is in a state ready to handle the reboot sequence before
 * calling this function.
 *
 * @param None This function does not take any parameters.
 * @return None
 ******************************************************************************/
void sli_cpc_reboot_sequence_process(void);

/***************************************************************************//**
 * @brief This function manages the state machine for rebooting a device into
 * bootloader mode. It should be called periodically to progress through
 * the sequence of setting the reboot mode, waiting for acknowledgments,
 * and confirming the reset. The function does not take any parameters
 * and does not return a value. It is essential to ensure that the system
 * is in a state where a reboot into bootloader mode is appropriate
 * before calling this function. The function handles different states
 * internally and will not perform any action if the sequence is already
 * completed.
 *
 * @return None
 ******************************************************************************/
void sli_cpc_reboot_bootloader_sequence_process(void);

/***************************************************************************//**
 * @brief Use this function to determine if a Gecko bootloader, which is
 * suitable for performing firmware upgrades, is present in the system.
 * This function is useful in scenarios where the application needs to
 * verify the availability of the bootloader before attempting a firmware
 * update. It returns a boolean value indicating the presence or absence
 * of the bootloader, allowing the application to make decisions based on
 * this information.
 *
 * @return A boolean value: `true` if a Gecko bootloader is present, `false`
 * otherwise.
 ******************************************************************************/
bool sli_cpc_is_gecko_bootloader_present(void);

/***************************************************************************//**
 * @brief This function is used to reset the state machine associated with the
 * CPC reboot sequence, allowing the sequence to be re-executed. It is
 * typically called when a fresh start of the reboot process is required,
 * such as after a previous sequence has completed or failed. The
 * function initializes various internal flags and state variables to
 * their default values, ensuring that the reboot sequence can proceed
 * without residual state from prior executions. This function should be
 * used in contexts where a controlled reboot sequence is necessary, and
 * it assumes that any necessary cleanup or preparation has been
 * performed prior to invocation.
 *
 * @return None
 ******************************************************************************/
void sli_cpc_reboot_sequence_start(void);
