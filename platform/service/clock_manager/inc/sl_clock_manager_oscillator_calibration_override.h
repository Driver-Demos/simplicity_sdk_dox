/***************************************************************************//**
 * @file
 * @brief Clock Manager Oscillator Calibration Override APIs.
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_CLOCK_MANAGER_OSCILLATOR_CALIBRATION_OVERRIDE_H
#define SL_CLOCK_MANAGER_OSCILLATOR_CALIBRATION_OVERRIDE_H

#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup clock_manager Clock Manager
 * @{
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function initializes the oscillator calibration by configuring
 * the CTune settings using override key values stored in NVM3. It should
 * be called when the system requires the oscillator to be calibrated
 * with specific override values, typically during system initialization
 * or when recalibration is necessary. The function reads the calibration
 * value from NVM3 and applies it to the oscillator. It returns a status
 * code indicating success or failure, which should be checked by the
 * caller to ensure the calibration was applied correctly.
 *
 * @return Returns a status code of type sl_status_t, which is SL_STATUS_OK if
 * the operation is successful, or an error code if it fails.
 ******************************************************************************/
sl_status_t sl_clock_manager_init_oscillator_calibration_override(void);

/***************************************************************************//**
 * @brief This function is used to store a High-Frequency Crystal Oscillator
 * (HFXO) calibration value, specified by the ctune parameter, into non-
 * volatile memory (NVM3). It is typically called when there is a need to
 * override the default calibration settings with a specific value. The
 * function ensures that the provided ctune value is within the allowable
 * range before attempting to write it to NVM3. If the ctune value is out
 * of range, the function returns an error status. This function should
 * be used when precise control over the oscillator calibration is
 * required, and the user must ensure that the ctune value is valid
 * before calling this function.
 *
 * @param ctune The HFXO override CTune value to write to NVM3. It must be
 * within the allowable range defined by the hardware. If the value
 * exceeds this range, the function returns
 * SL_STATUS_INVALID_PARAMETER. The caller retains ownership of
 * this value.
 * @return Returns a status code indicating the success or failure of the
 * operation. SL_STATUS_OK is returned if the operation is successful,
 * otherwise an error code is returned.
 ******************************************************************************/
sl_status_t sl_clock_manager_write_hfxo_calibration_override(uint32_t ctune);

/***************************************************************************//**
 * @brief This function retrieves the HFXO override calibration value stored in
 * NVM3 and writes it to the provided output parameter. It should be used
 * when you need to access the current calibration setting for the HFXO
 * oscillator. The function requires a valid pointer to a uint32_t
 * variable where the calibration value will be stored. If the
 * calibration value is not found in NVM3, the function sets the output
 * to 0xFFFF. It is important to ensure that the pointer provided is not
 * null, as passing a null pointer will result in an
 * SL_STATUS_NULL_POINTER error.
 *
 * @param ctune A pointer to a uint32_t variable where the HFXO override CTune
 * value will be stored. Must not be null. If null, the function
 * returns SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t status code. SL_STATUS_OK if the read
 * operation is successful, or an error code if it fails. If the value
 * is not found, ctune is set to 0xFFFF.
 ******************************************************************************/
sl_status_t sl_clock_manager_read_hfxo_calibration_override(uint32_t *ctune);

/***************************************************************************//**
 * @brief This function removes the High-Frequency Crystal Oscillator (HFXO)
 * calibration override value stored in NVM3. It should be used when the
 * override calibration value is no longer needed or needs to be reset.
 * The function returns a status code indicating the success or failure
 * of the operation. It is important to check the return value to ensure
 * that the deletion was successful.
 *
 * @return Returns a status code of type sl_status_t, which is SL_STATUS_OK if
 * the deletion was successful, or an error code if it failed.
 ******************************************************************************/
sl_status_t sl_clock_manager_delete_hfxo_calibration_override(void);

/** @} (end addtogroup clock_manager) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLOCK_MANAGER_OSCILLATOR_CALIBRATION_OVERRIDE_H
