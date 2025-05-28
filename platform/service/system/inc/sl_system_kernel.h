/***************************************************************************//**
 * @file
 * @brief System Kernel Initialization.
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
#ifndef SL_SYSTEM_KERNEL_H
#define SL_SYSTEM_KERNEL_H

/***************************************************************************//**
 * @addtogroup system
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function initiates the system kernel, transitioning the system
 * from an initialization state to a running state. It should be called
 * once all necessary system configurations and initializations are
 * complete, as it typically does not return control to the caller. This
 * function is essential for starting the main execution loop of the
 * system, and it is expected to be called in the main function or
 * equivalent entry point of the application.
 *
 * @return None
 ******************************************************************************/
void sl_system_kernel_start(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup system) */

#endif // SL_SYSTEM_KERNEL_H
