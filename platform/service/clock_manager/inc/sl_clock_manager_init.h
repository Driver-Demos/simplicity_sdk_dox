/***************************************************************************//**
 * @file
 * @brief Clock Manager Init APIs.
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

#ifndef SL_CLOCK_MANAGER_INIT_H
#define SL_CLOCK_MANAGER_INIT_H

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
 * @brief This function is used to initialize the oscillators and clock branches
 * within the system. It should be called before any clock-dependent
 * operations are performed to ensure that the clock system is properly
 * configured and operational. The function returns a status code
 * indicating the success or failure of the initialization process. It is
 * important to check the return value to handle any potential errors
 * that may occur during initialization.
 *
 * @return Returns a status code of type sl_status_t, which is SL_STATUS_OK if
 * the initialization is successful, or an error code if it fails.
 ******************************************************************************/
sl_status_t sl_clock_manager_init(void);

/** @} (end addtogroup clock_manager) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLOCK_MANAGER_INIT_H
