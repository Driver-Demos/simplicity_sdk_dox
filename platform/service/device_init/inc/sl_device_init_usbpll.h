/***************************************************************************//**
 * @file
 * @brief Device initialization for USB PLL.
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
#ifndef SL_DEVICE_INIT_USBPLL_H
#define SL_DEVICE_INIT_USBPLL_H

#include "sl_status.h"

/**
 * @addtogroup device_init
 * @{
 * @addtogroup device_init_usbpll USB PLL Initialization
 * @brief Initialize the USB PLL
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function configures and initializes the USB PLL, ensuring it is
 * locked and ready for use. It should be called when setting up the USB
 * subsystem to ensure the PLL is correctly configured based on the
 * system's high-frequency crystal oscillator (HFXO) frequency. The
 * function checks if the HFXO frequency is one of the supported values
 * (38.0 MHz, 38.4 MHz, 39.0 MHz, or 40.0 MHz) and configures the PLL
 * accordingly. If the frequency is not supported, the function returns
 * an error status. This function must be called before any USB
 * operations that depend on the PLL.
 *
 * @return Returns SL_STATUS_OK if the PLL is successfully initialized, or
 * SL_STATUS_FAIL if the initialization fails due to an unsupported HFXO
 * frequency.
 ******************************************************************************/
sl_status_t sl_device_init_usbpll(void);

#ifdef __cplusplus
}
#endif

/**
 * @} device_init_usbpll
 * @} device_init
 */

#endif // SL_DEVICE_INIT_USBPLL_H
