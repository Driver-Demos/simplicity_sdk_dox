/***************************************************************************//**
 * @file
 * @brief Header file for hooks required by Legacy HAL integration with other
 * components.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories, Inc, www.silabs.com</b>
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

/***************************************************************************//**
 * @brief This function is used to specify the action that should be taken by
 * the power manager when an interrupt service routine (ISR) exits. It is
 * typically called within the context of an ISR to indicate whether the
 * system should remain awake or can return to a low-power state. This
 * function is part of the legacy HAL integration and is crucial for
 * managing power efficiently in systems that require precise control
 * over power states during and after interrupt handling.
 *
 * @return Returns a value of type `sl_power_manager_on_isr_exit_t` indicating
 * the power management action to be taken, specifically
 * `SL_POWER_MANAGER_WAKEUP`, which suggests that the system should
 * remain awake after ISR exit.
 ******************************************************************************/
sl_power_manager_on_isr_exit_t sl_legacy_hal_sleep_on_isr_exit(void);
