/***************************************************************************//**
 * @file
 * @brief Interrupt Manager API internal utility functions.
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

#ifndef SLI_INTERRUPT_MANAGER_H
#define SLI_INTERRUPT_MANAGER_H

#include "sl_interrupt_manager.h"
#include "em_device.h"

#if defined(_SILICON_LABS_32B_SERIES_2)
#include "sl_interrupt_manager_s2_config.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief
 *   Set a new RAM based interrupt vector table.
 *
 * @details
 *   This function will copy the existing vector table to a RAM area supplied
 *   by the application and set the interrupt controller to use that.
 *
 * @note
 *   The table RAM area must be aligned to a TBD byte boundary.
 *
 * @param[in] table
 *   Base address of new table.
 *
 * @param[in] handler_count
 *   The size of the table, unit is number of interrupt handlers.
 *
 * @return
 *   The prior interrupt vector table address.
 ******************************************************************************/
#if defined(_SILICON_LABS_32B_SERIES_3)                  \
  || (defined(SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM) \
  && (SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM == 1))

/***************************************************************************//**
 * @brief This function sets a new interrupt vector table located in RAM,
 * allowing the application to specify a custom table for handling
 * interrupts. It is typically used when the default vector table needs
 * to be replaced with a custom one stored in RAM. The function must be
 * called with a valid table address and a handler count that specifies
 * the number of interrupt handlers. The table must be aligned to a
 * specific byte boundary, and the function will return the address of
 * the previous interrupt vector table. It is important to ensure that
 * the table address and size are correctly aligned and within valid
 * memory ranges to avoid assertion failures.
 *
 * @param table Base address of the new interrupt vector table. Must be a valid
 * RAM address, aligned to a specific byte boundary, and within the
 * RAM size limits. The caller retains ownership.
 * @param handler_count Number of interrupt handlers in the table. Must be a
 * positive integer, and the table size must be aligned to
 * its size rounded up to the nearest power of two.
 * @return Returns the address of the prior interrupt vector table.
 ******************************************************************************/
sl_interrupt_manager_irq_handler_t *sli_interrupt_manager_set_irq_table(sl_interrupt_manager_irq_handler_t *table,
                                                                        uint32_t handler_count);

#endif

/***************************************************************************//**
 * @brief This function serves as a pre-interrupt hook that is called before
 * each interrupt service routine when the interrupt manager hooks
 * feature is enabled. It is weakly defined, allowing users to override
 * it with their own implementation to execute custom code prior to
 * handling an interrupt. By default, the function does nothing, so it is
 * safe to leave it unimplemented if no pre-interrupt processing is
 * needed. This function is useful for tasks such as logging, performance
 * monitoring, or preparing system state before an interrupt is
 * processed.
 *
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_irq_enter_hook(void);

/***************************************************************************//**
 * @brief This function is intended to be called after each interrupt service
 * routine when the interrupt manager hooks feature is enabled. It is
 * weakly defined, allowing users to provide their own implementation if
 * specific post-interrupt processing is required. By default, the
 * function does nothing, serving as a placeholder for user-defined
 * behavior. This function should be used when there is a need to execute
 * custom code immediately after an interrupt is handled.
 *
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_irq_exit_hook(void);

#ifdef __cplusplus
}
#endif

#endif /* SL_INTERRUPT_MANAGER_H */
