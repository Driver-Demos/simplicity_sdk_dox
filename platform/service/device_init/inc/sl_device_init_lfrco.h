/***************************************************************************//**
 * @file
 * @brief Device initialization for LFRCO.
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
#ifndef SL_DEVICE_INIT_LFRCO_H
#define SL_DEVICE_INIT_LFRCO_H

#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup device_init
 * @{
 * @addtogroup device_init_lfrco LFRCO Initialization
 * @brief Initialize the LFRCO oscillator.
 * @details
 * Initialize the low frequency RC oscillator. On Series 2 devices with
 * precision mode support, the precision mode is configured in the configuration
 * header `sl_device_init_lfrco_config.h`. On other devices, there is nothing
 * to configure.
 *
 * If using Simplicity Studio, this configuration header is also configurable
 * through the Project Configurator, by selecting the "Device Init: LFRCO"
 * configuration component.
 * @{
 */

/***************************************************************************//**
 * @brief This function initializes the low frequency RC oscillator (LFRCO) on
 * Silicon Labs devices. It is designed to configure the LFRCO based on
 * the device series and available features. For Series 2 devices with
 * precision mode support, the precision mode is set according to the
 * configuration specified in the `sl_device_init_lfrco_config.h` file.
 * For Series 1 devices, additional configurations such as ENVREF,
 * ENCHOP, and ENDEM are applied based on predefined macros. This
 * function should be called during the device initialization phase to
 * ensure the LFRCO is properly configured before use. It returns a
 * status code indicating the success of the initialization process.
 *
 * @return Returns SL_STATUS_OK if the LFRCO is initialized successfully.
 ******************************************************************************/
sl_status_t sl_device_init_lfrco(void);

/**
 * @} device_init_lfrco
 * @} device_init
 */

#ifdef __cplusplus
}
#endif

#endif // SL_DEVICE_INIT_LFRCO_H
