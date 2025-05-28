/***************************************************************************//**
 * @file
 * @brief API defining access to SYSCFG registers
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

#ifndef SL_HAL_SYSCFG_H
#define SL_HAL_SYSCFG_H

#include <stdint.h>

#if defined(SL_TRUSTZONE_NONSECURE)
#include "sli_tz_service_syscfg.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup syscfg SYSTEM CONFIGURATION - System Configurations
 * @brief Syscfg API
 * @details
 *
 * @{
 ******************************************************************************/

/*******************************************************************************
 *********************************   DEFINES   *********************************
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/*******************************************************************************
 ******************************** TZ SERVICES **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function retrieves the current chip revision identifier by
 * reading the CHIPREV register. It is useful for applications that need
 * to adapt behavior based on the specific chip revision. The function
 * can be used in both secure and non-secure TrustZone environments,
 * automatically selecting the appropriate method to access the register.
 * It is important to ensure that the system configuration is properly
 * initialized before calling this function to avoid undefined behavior.
 *
 * @return Returns a 32-bit unsigned integer representing the chip revision
 * identifier.
 ******************************************************************************/
uint32_t sl_hal_syscfg_read_chip_rev(void);

/***************************************************************************//**
 * @brief This function is used to enable the external clock for the system tick
 * by setting the SYSTICEXTCLKEN bit in the CFGSYSTIC register. It should
 * be called when the system requires the external clock for the system
 * tick functionality. The function handles both secure and non-secure
 * environments, making it suitable for use in TrustZone-enabled systems.
 * Ensure that the system configuration is properly initialized before
 * calling this function to avoid undefined behavior.
 *
 * @return None
 ******************************************************************************/
void sl_hal_syscfg_set_systicextclken_cfgsystic(void);

/***************************************************************************//**
 * @brief This function is used to clear the SYSTICEXTCLKEN bit in the CFGSYSTIC
 * register, effectively disabling the external clock for the system tick
 * timer. It should be called when you need to ensure that the system
 * tick timer does not use an external clock source. This function is
 * applicable in both secure and non-secure TrustZone configurations,
 * automatically handling the appropriate context. It is important to
 * ensure that any necessary configurations or dependencies are managed
 * before calling this function to avoid unintended behavior.
 *
 * @return None
 ******************************************************************************/
void sl_hal_syscfg_clear_systicextclken_cfgsystic(void);

#ifdef __cplusplus
}
#endif
#endif // SL_HAL_SYSCFG_H
