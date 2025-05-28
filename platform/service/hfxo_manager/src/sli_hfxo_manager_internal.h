/***************************************************************************//**
 * @file
 * @brief HFXO Manager Internal API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_HFXO_MANAGER_INTERNAL_H
#define SLI_HFXO_MANAGER_INTERNAL_H

#include "sl_hfxo_manager.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function is used to update the hardware registers with new
 * settings for the sleepy crystal oscillator. It should be called when
 * there is a need to change the configuration of the sleepy crystal
 * oscillator, typically to optimize performance or power consumption.
 * The function requires that the system supports sleepy crystal
 * functionality, as indicated by the compile-time configuration. If the
 * support is not available, the function will return a status indicating
 * that the operation is not available. It is important to ensure that
 * the settings provided are within the valid range, as the function will
 * assert if they are not.
 *
 * @param settings A pointer to a structure containing the new settings for the
 * sleepy crystal oscillator. The structure must not be null,
 * and the values within must be within the valid range for the
 * hardware registers. The caller retains ownership of the
 * memory.
 * @return Returns SL_STATUS_OK if the settings are successfully updated. If
 * sleepy crystal support is not available, returns
 * SL_STATUS_NOT_AVAILABLE.
 ******************************************************************************/
sl_status_t sli_hfxo_manager_update_sleepy_xtal_settings_hardware(const sl_hfxo_manager_sleepy_xtal_settings_t *settings);

#ifdef __cplusplus
}
#endif

#endif /* SLI_HFXO_MANAGER_INTERNAL_H */
