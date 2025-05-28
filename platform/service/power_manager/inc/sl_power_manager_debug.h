/***************************************************************************//**
 * @file
 * @brief Power Manager API definition.
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

#ifndef SL_POWER_MANAGER_DEBUG_H
#define SL_POWER_MANAGER_DEBUG_H

#include "sl_power_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup power_manager
 * @{
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function outputs a formatted table to the standard output,
 * detailing the current energy mode (EM) requirements and the modules
 * that own these requirements. It is useful for debugging purposes to
 * understand which modules are influencing the power management system's
 * energy mode decisions. The function should be called when a detailed
 * view of energy mode dependencies is needed, especially during
 * development or troubleshooting. If the system has exhausted its debug
 * entry pool, a warning message is printed, indicating that the report
 * may be incomplete. This function does not modify any state or data.
 *
 * @return None
 ******************************************************************************/
void sl_power_manager_debug_print_em_requirements(void);

/** @} (end addtogroup power_manager) */

#ifdef __cplusplus
}
#endif

#endif
