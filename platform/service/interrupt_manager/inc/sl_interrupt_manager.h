/***************************************************************************//**
 * @file
 * @brief Interrupt Management API to enable and configure interrupts.
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

#ifndef SL_INTERRUPT_MANAGER_H
#define SL_INTERRUPT_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include "sl_core.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup interrupt_manager Interrupt Manager
 * @brief Interrupt management service can be used for general interrupt management.
 * The source files for Interrupt Manager platform software module are present under
 * platform/services/interrupt_manager.
 * @details
 * ## Overview
 * The Interrupt Manager is a service that offers interupt management functions and configurations
 * for setting the interrupt vector in RAM, managing the core reset initiation function and
 * doing general interrupt management operations.
 *
 * ## Configuration Options
 *
 * Some properties of the Interrupt Manager are compile-time configurable. These
 * properties are set in the sl_interrupt_manager_s2_config.h file.
 * These are the available configuration parameters with default values defined.
 * @code
 *
 * // <q SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM> Put the interrupt vector table in RAM.
 * // <i> Set to 1 to put the vector table in RAM.
 * // <i> Default: 0
 * #define SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM  0
 * @endcode
 *
 * @note The SL_INTERRUPT_MANAGER_S2_INTERRUPTS_IN_RAM configuration is only available
 * on series 2. Enabling the S2_INTERRUPTS_IN_RAM configuration will tell the Interrupt Manager
 * to copy the interrupt vector table from ROM to RAM and select it as the interrupt table.
 * On newer series this feature is always enabled.
 *
 * ## The API
 *
 * This section contains brief descriptions of the functions in the API. For more
 * information on input and output parameters and return values,
 * click on the hyperlinked function names.
 *
 * @ref sl_interrupt_manager_disable_interrupts and @ref sl_interrupt_manager_enable_interrupts()
 * are used to prevent interrupts from executing during a certain timelapse.
 *
 * @ref sl_interrupt_manager_is_irq_disabled, @ref sl_interrupt_manager_is_irq_blocked
 * are used to know the status of an interrupt, either if it's disabled or blocked by one of the
 * following reasons: priority masking, disabling or an active interrupt of higher priority
 * is executing.
 *
 * @ref sl_interrupt_manager_is_irq_pending, @ref sl_interrupt_manager_set_irq_pending and
 * @ref sl_interrupt_manager_clear_irq_pending
 * are used for control and query of external interrupt source pending status.
 *
 * @ref sl_interrupt_manager_get_irq_priority and @ref sl_interrupt_manager_set_irq_priority
 * are used to get or set the priority for a specific interrupt.
 *
 * ## Priority Stratification
 * With the Interrupt Manager service and more generally in the Simplicity SDK, there are multiple distinct
 * levels of priority stratification.
 *
 * Each of these has their own characteristics and purposes.
 * For example, the higher priority group is considered to not be able to call kernel, power manager
 * or protocol stacks functions. They will only be impacted by critical sections (general interrupt
 * disable) but will be above atomic base interrupt priority level for execution. The higher level
 * is considered to be between 0 and 2 and the base interrupt priority level is 3.
 *
 * In the normal priority group you will find most application interrupts and such interrupts will be
 * the ones that will make calls to more features such as kernel, power manager and protocol stacks API.
 * It is this way because they are less deterministic than the "higher priority interrupts".
 *
 * <table>
 * <caption id="interrupt_priority_strat">Priority stratification inside SDK</caption>
 * <tr><th>Priority<th>Purpose
 * <tr><td>0 - 2 (Highest)<td>
 * <ul>
 *     <li>No Kernel calls
 *     <li>No Power Manager calls
 *      <li>Not maskable by atomic sections
 * </ul>
 * <tr><td>3 - 7 (Normal)<td>
 * <ul>
 *     <li>kernel calls
 *     <li>power manager
 *      <li>protocol stacks API
 * </ul>
 * <tr><td>7 (Lowest)<td>
 * <ul>
 *     <li>PendSV level of priority
 * </ul>
 * </table>
 * @{
 ******************************************************************************/

/// @brief sl_interrupt_manager interrupt handler function.
typedef void(*sl_interrupt_manager_irq_handler_t)(void);

/***************************************************************************//**
 * @brief This function initializes the interrupt manager, ensuring that the
 * vector table is set up correctly in RAM if the configuration allows.
 * It should be called once at the start of the program to prepare the
 * interrupt system for use. The function is idempotent, meaning
 * subsequent calls after the first initialization will have no effect.
 * It sets default priorities for interrupts and, if configured, copies
 * the vector table from ROM to RAM. This function must be called before
 * any other interrupt management functions to ensure proper operation.
 *
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_init(void);

/***************************************************************************//**
 * @brief This function is used to reset the CPU core, effectively restarting
 * the system. It should be called when a system reset is required, such
 * as in response to a critical error or to apply certain configuration
 * changes that necessitate a restart. This function does not take any
 * parameters and does not return a value. It is important to ensure that
 * any necessary data is saved or state is preserved before calling this
 * function, as it will not return and the system will restart
 * immediately.
 *
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_reset_system(void);

/***************************************************************************//**
 * @brief Use this function to globally disable all interrupts, ensuring that no
 * interrupt service routines are executed until interrupts are re-
 * enabled. This is typically used in critical sections of code where
 * atomic operations are required, and interrupt interference must be
 * prevented. It is important to re-enable interrupts after the critical
 * section to maintain system responsiveness. This function should be
 * used with caution, as disabling interrupts for extended periods can
 * lead to missed interrupts and degraded system performance.
 *
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_disable_interrupts(void);

/***************************************************************************//**
 * @brief This function is used to enable global interrupts, allowing the system
 * to respond to interrupt requests. It should be called when the system
 * is ready to handle interrupts, typically after initialization or when
 * resuming from a state where interrupts were intentionally disabled.
 * There are no parameters to configure, and it does not return a value.
 * This function is essential for systems that rely on interrupt-driven
 * processing.
 *
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_enable_interrupts(void);

/***************************************************************************//**
 * @brief This function is used to disable a specific interrupt source
 * identified by its interrupt number. It is typically called when an
 * interrupt should be temporarily or permanently disabled to prevent it
 * from being triggered. The function must be called with a valid
 * interrupt number within the range of available external interrupts. If
 * the interrupt number is outside the valid range, the behavior is
 * undefined. This function does not return a value and does not provide
 * feedback on the success of the operation.
 *
 * @param irqn The interrupt number of the interrupt source to be disabled. It
 * must be a non-negative integer and within the range of available
 * external interrupts. Passing a value outside this range results
 * in undefined behavior.
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_disable_irq(int32_t irqn);

/***************************************************************************//**
 * @brief This function is used to enable an interrupt for a given interrupt
 * source identified by its interrupt number. It is essential to ensure
 * that the interrupt number provided is within the valid range, which is
 * from 0 to EXT_IRQ_COUNT inclusive. This function should be called when
 * you want to allow the specified interrupt to be triggered and handled
 * by the system. It is important to note that the function does not
 * return any value, and it assumes that the interrupt number is valid as
 * per the precondition.
 *
 * @param irqn The interrupt number of the interrupt source to be enabled. It
 * must be a non-negative integer and should not exceed
 * EXT_IRQ_COUNT. If the value is outside this range, the behavior
 * is undefined.
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_enable_irq(int32_t irqn);

/***************************************************************************//**
 * @brief This function determines whether a given interrupt, identified by its
 * interrupt number, is currently disabled. It is useful for checking the
 * status of an interrupt before attempting operations that require the
 * interrupt to be disabled. The function should be called with a valid
 * interrupt number within the range of available external interrupts. If
 * the provided interrupt number is outside the valid range, the behavior
 * is undefined. This function does not modify any state or have side
 * effects.
 *
 * @param irqn The interrupt number of the interrupt source. It must be a non-
 * negative integer and less than the total number of external
 * interrupts (EXT_IRQ_COUNT). Providing an invalid interrupt number
 * results in undefined behavior.
 * @return Returns true if the specified interrupt is disabled, otherwise
 * returns false.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_disabled(int32_t irqn);

/***************************************************************************//**
 * @brief This function determines whether a given interrupt is currently
 * blocked, which can occur if the interrupt is disabled, masked by
 * priority, or if a higher priority interrupt is active. It is useful
 * for checking the status of an interrupt before attempting operations
 * that require it to be unblocked. The function should be called with a
 * valid interrupt number, and it assumes that the interrupt manager has
 * been properly initialized.
 *
 * @param irqn The interrupt number of the interrupt source. It must be within
 * the valid range for the platform, specifically between
 * MemoryManagement_IRQn and EXT_IRQ_COUNT for Cortex-M3 and above,
 * or between SVCall_IRQn and EXT_IRQ_COUNT for other platforms.
 * Invalid values will trigger an assertion failure.
 * @return Returns true if the interrupt is blocked due to being disabled,
 * masked by priority, or if a higher priority interrupt is active;
 * otherwise, returns false.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_blocked(int32_t irqn);

/***************************************************************************//**
 * @brief Use this function to determine if a given interrupt, identified by its
 * interrupt number, is currently pending. This is useful for checking
 * the status of an interrupt before taking action based on its state.
 * The function should be called with a non-negative interrupt number, as
 * negative values will result in a return value of false, indicating
 * that the interrupt is not pending. This function does not modify any
 * state or have side effects.
 *
 * @param irqn The interrupt number to check. Must be a non-negative integer. If
 * a negative value is provided, the function will return false,
 * indicating the interrupt is not pending.
 * @return Returns a boolean value: true if the specified interrupt is pending,
 * false otherwise.
 ******************************************************************************/
bool sl_interrupt_manager_is_irq_pending(int32_t irqn);

/***************************************************************************//**
 * @brief This function is used to set the pending status of a specified
 * interrupt, identified by its interrupt number. It is typically called
 * when an interrupt needs to be marked as pending, which means it is
 * ready to be serviced by the processor. The function should only be
 * called with a non-negative interrupt number, as negative values are
 * not valid and will result in no action being taken. This function does
 * not return a value and does not provide feedback if the operation was
 * successful or not.
 *
 * @param irqn The interrupt number of the interrupt source. Must be a non-
 * negative integer. If a negative value is provided, the function
 * will not perform any action.
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_set_irq_pending(int32_t irqn);

/***************************************************************************//**
 * @brief Use this function to clear the pending status of an interrupt,
 * effectively resetting its pending state. This is useful when you want
 * to ensure that an interrupt is not erroneously triggered due to a
 * previously set pending status. The function should be called with a
 * valid interrupt number, which must be non-negative. If the interrupt
 * number is negative, the function will not perform any action. This
 * function is typically used in interrupt management scenarios where
 * precise control over interrupt states is required.
 *
 * @param irqn The interrupt number of the interrupt source. It must be a non-
 * negative integer. If a negative value is provided, the function
 * will not clear any pending status.
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_clear_irq_pending(int32_t irqn);

/***************************************************************************//**
 * @brief This function assigns a new interrupt handler to a specified interrupt
 * source, provided that the interrupt vector table is located in RAM. It
 * is essential to ensure that the interrupt manager is properly
 * initialized and configured to use a RAM-based vector table before
 * calling this function. The function will return an error if the
 * interrupt number is invalid or if the vector table is not in RAM. It
 * is typically used when you need to change the behavior of an interrupt
 * dynamically during runtime.
 *
 * @param irqn The interrupt number of the source for which the handler is being
 * set. It must be a non-negative integer and less than the total
 * number of external interrupts (EXT_IRQ_COUNT). If the value is
 * out of range, the function returns SL_STATUS_INVALID_PARAMETER.
 * @param handler A function pointer to the new interrupt handler. This handler
 * will be called when the specified interrupt occurs. The caller
 * retains ownership of the function pointer, and it must not be
 * null.
 * @return Returns SL_STATUS_OK on success, SL_STATUS_INVALID_PARAMETER if the
 * interrupt number is invalid, SL_STATUS_NOT_INITIALIZED if the vector
 * table is not in RAM, or SL_STATUS_INVALID_CONFIGURATION if the
 * configuration does not support RAM-based vector tables.
 ******************************************************************************/
sl_status_t sl_interrupt_manager_set_irq_handler(int32_t irqn,
                                                 sl_interrupt_manager_irq_handler_t handler);

/***************************************************************************//**
 * @brief Use this function to obtain the preemption priority of a specific
 * interrupt source identified by its interrupt number. This function is
 * useful when you need to check the current priority level of an
 * interrupt to make decisions based on its urgency or to compare it with
 * other interrupts. Ensure that the interrupt number provided is within
 * the valid range for your platform, as the function expects the
 * interrupt number to be between -CORTEX_INTERRUPTS and EXT_IRQ_COUNT.
 * The function will assert if the interrupt number is out of this range.
 *
 * @param irqn The interrupt number of the interrupt source. It must be within
 * the range of -CORTEX_INTERRUPTS to EXT_IRQ_COUNT. If the value is
 * outside this range, the function will assert.
 * @return Returns the priority of the specified interrupt as a uint32_t value,
 * where a lower number indicates a higher priority.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_irq_priority(int32_t irqn);

/***************************************************************************//**
 * @brief This function is used to assign a new preemption priority to a
 * specific interrupt source. It is essential to ensure that the
 * interrupt number and priority values are within valid ranges before
 * calling this function. The function should be used when there is a
 * need to change the priority level of an interrupt to manage its
 * execution order relative to other interrupts. It is important to note
 * that the priority level is platform-dependent, and the function
 * assumes that the interrupt manager has been properly initialized.
 *
 * @param irqn The interrupt number of the interrupt source. It must be within
 * the range of valid interrupt numbers, specifically between
 * -CORTEX_INTERRUPTS and EXT_IRQ_COUNT inclusive. Invalid values
 * will trigger an assertion failure.
 * @param priority The new priority level for the interrupt. It must be a non-
 * negative value not exceeding LOWEST_NVIC_PRIORITY. Invalid
 * values will trigger an assertion failure.
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_set_irq_priority(int32_t irqn, uint32_t priority);

/***************************************************************************//**
 * @brief This function adjusts the priority of a specified interrupt by
 * decreasing its priority value relative to the default priority,
 * effectively increasing its preemption priority. It is useful for
 * making architecture-agnostic priority adjustments. The function should
 * be called when you need to elevate the priority of an interrupt beyond
 * its default setting. Ensure that the interrupt number is valid and
 * that the resulting priority does not exceed platform-specific limits.
 *
 * @param irqn The interrupt number of the source whose priority is to be
 * increased. It must be a valid interrupt number for the platform.
 * @param diff The amount by which to increase the priority, specified as a
 * positive integer. The function will subtract this value from the
 * default priority to determine the new priority.
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_increase_irq_priority_from_default(int32_t irqn, uint32_t diff);

/***************************************************************************//**
 * @brief This function is used to decrease the preemption priority of a
 * specified interrupt source by a given amount relative to its default
 * priority. It is useful for adjusting interrupt priorities in a
 * platform-independent manner. The function should be called when you
 * need to lower the priority of an interrupt, making it less urgent
 * compared to others. Ensure that the interrupt number is valid and that
 * the priority adjustment does not exceed platform-specific limits.
 *
 * @param irqn The interrupt number of the source whose priority is to be
 * changed. It must be a valid interrupt number for the platform.
 * @param diff The amount by which to decrease the interrupt's priority. It
 * should be a non-negative value, and care should be taken to
 * ensure the resulting priority remains within valid bounds.
 * @return None
 ******************************************************************************/
void sl_interrupt_manager_decrease_irq_priority_from_default(int32_t irqn, uint32_t diff);

/***************************************************************************//**
 * @brief Use this function to obtain the default priority level for interrupt
 * preemption, which is useful for configuring or comparing interrupt
 * priorities within the system. This function can be called at any time
 * to retrieve the default priority value, which is a constant defined in
 * the system. It is particularly useful when setting or adjusting
 * interrupt priorities relative to the default.
 *
 * @return Returns the default interrupt preemption priority as a 32-bit
 * unsigned integer.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_default_priority(void);

/***************************************************************************//**
 * @brief This function returns the highest possible interrupt preemption
 * priority value that can be set within the system. It is useful for
 * determining the upper bound of priority levels when configuring
 * interrupts. This function does not require any initialization or
 * configuration to be called and has no side effects.
 *
 * @return The function returns a uint32_t representing the highest interrupt
 * preemption priority value.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_highest_priority(void);

/***************************************************************************//**
 * @brief This function is used to obtain the lowest possible interrupt
 * preemption priority value within the system. It is useful when
 * configuring or comparing interrupt priorities to ensure that a given
 * interrupt is set to the lowest priority level available. This function
 * can be called at any time and does not require any prior
 * initialization of the interrupt manager. It is a read-only operation
 * and does not modify any system state.
 *
 * @return Returns the lowest interrupt preemption priority value as a uint32_t.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_lowest_priority(void);

/***************************************************************************//**
 * @brief This function checks whether a specific interrupt, identified by its
 * interrupt number, is currently active. It is useful for determining if
 * an interrupt is being serviced at the moment. The function should be
 * called with a valid interrupt number, and it will return a non-zero
 * value if the interrupt is active, or zero if it is not. It is
 * important to ensure that the interrupt number provided is non-
 * negative, as negative values will result in a return value of zero,
 * indicating the interrupt is not active.
 *
 * @param irqn The interrupt number of the interrupt source. It must be a non-
 * negative integer. If a negative value is provided, the function
 * will return zero, indicating the interrupt is not active.
 * @return Returns a non-zero value if the specified interrupt is active,
 * otherwise returns zero.
 ******************************************************************************/
uint32_t sl_interrupt_manager_get_active_irq(int32_t irqn);

/***************************************************************************//**
 * @brief This function returns a pointer to the current Interrupt Service
 * Routine (ISR) table, which may reside in RAM or FLASH depending on the
 * configuration of the Interrupt Manager. The table may also include
 * ISRs wrapped with enter/exit hooks if such hooks are enabled. This
 * function is useful for accessing the current configuration of
 * interrupt handlers, especially in systems where the ISR table location
 * or wrapping behavior can change based on compile-time settings.
 *
 * @return A pointer to the current ISR table, which is an array of interrupt
 * handler functions.
 ******************************************************************************/
sl_interrupt_manager_irq_handler_t* sl_interrupt_manager_get_isr_table(void);

/** @} (end addtogroup interrupt_manager) */

#ifdef __cplusplus
}
#endif

#endif /* SL_INTERRUPT_MANAGER_H */
