/***************************************************************************//**
 * @file
 * @brief Power Manager Private API definition.
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

#ifndef SLI_POWER_MANAGER_H
#define SLI_POWER_MANAGER_H

#include "sl_power_manager.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ******************************   HOOK REFERENCES   ****************************
 ******************************************************************************/

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function checks whether the system should enter a sleep state
 * upon exiting an interrupt service routine (ISR). It is typically used
 * in power management scenarios where conserving energy is crucial. The
 * function is expected to be called within the context of an ISR to
 * decide if the system should transition to a low-power state
 * immediately after the ISR completes. This function always returns
 * false, indicating that the system should not sleep on ISR exit by
 * default.
 *
 * @return A boolean value indicating whether the system should sleep on ISR
 * exit, always false in this implementation.
 ******************************************************************************/
bool sl_power_manager_sleep_on_isr_exit(void);

// Callback to application after wakeup but before restoring interrupts.
// For internal Silicon Labs use only
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_on_wakeup(void);

// Hook that can be used by the log outputer to suspend transmission of logs
// in case it would require energy mode changes while in the sleep loop.
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_suspend_log_transmission(void);

// Hook that can be used by the log outputer to resume transmission of logs.
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_resume_log_transmission(void);

// Callback to notify possible transition from EM1P to EM2.
// For internal Silicon Labs use only
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
__WEAK void sli_power_manager_em1p_to_em2_notification(void);

/***************************************************************************//**
 * Mandatory callback that allows to cancel sleeping action.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function checks whether the system is in a state that allows it
 * to enter sleep mode. It should be called before attempting to
 * transition the system into a low-power sleep state to ensure that no
 * critical operations are pending that would prevent sleeping. This
 * function is part of the power management system and is crucial for
 * maintaining system stability and performance by preventing sleep when
 * it is not safe to do so.
 *
 * @return Returns a boolean value indicating whether it is safe to enter sleep
 * mode. A return value of 'true' means it is safe to sleep, while
 * 'false' would indicate otherwise.
 ******************************************************************************/
bool sl_power_manager_is_ok_to_sleep(void);

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function is used to manage the requirement for preserving high-
 * frequency clock settings, which is necessary when transitioning
 * between certain energy modes. It should be called when there is a need
 * to add or remove this requirement, typically in conjunction with
 * energy mode transitions. The function ensures that the requirement
 * counter does not exceed its bounds and that the operation is valid
 * given the current energy mode. It is important to use this function in
 * contexts where the preservation of high-frequency clock settings is
 * critical, such as when maintaining specific energy modes.
 *
 * @param add A boolean value indicating whether to add (true) or remove (false)
 * the requirement for preserving high-frequency clock settings. The
 * function will assert and return without making changes if adding
 * would cause the counter to exceed its maximum value or if removing
 * would cause it to drop below zero.
 * @return None
 ******************************************************************************/
void sli_power_manager_update_hf_clock_settings_preservation_requirement(bool add);

/***************************************************************************//**
 * @brief This function is used to ensure that the high frequency clock settings
 * are preserved, which is necessary when the system is operating in
 * certain energy modes. It should be used in conjunction with adding an
 * EM2 requirement to maintain proper power management. This function is
 * intended for internal use only and should be called when there is a
 * need to maintain high frequency clock settings across different power
 * states.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_add_hf_clock_settings_preservation_requirement(void);

/***************************************************************************//**
 * @brief This function is used to remove the requirement for preserving the
 * settings of high frequency clocks in the power management system. It
 * should be called when the preservation of these settings is no longer
 * necessary, typically in conjunction with removing an EM2 requirement.
 * This function is intended for internal use only and should be used
 * carefully to ensure that the system's power management requirements
 * are correctly maintained.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_remove_hf_clock_settings_preservation_requirement(void);

/***************************************************************************//**
 * @brief This function should be used to inform the power management module
 * that the high accuracy/high frequency clock is currently being
 * utilized. It is particularly important to call this function during
 * the initialization of the Radio Abstraction Interface Layer (RAIL) if
 * the radio clock settings are not configured before the Power Manager
 * is initialized. This function is intended for internal use only and
 * should be used in scenarios where the clock settings need to be
 * preserved to ensure proper operation of the system.
 *
 * @return None
 ******************************************************************************/
__WEAK void sli_power_manager_set_high_accuracy_hf_clock_as_used(void);

/***************************************************************************//**
 * Gets the wake-up restore process time.
 * If we are not in the context of a deepsleep and therefore don't need to
 * do a restore, the return value is 0.
 *
 *
 * @return   Wake-up restore process time.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function retrieves the time required to restore the system after
 * waking up from a deep sleep state. It should be used when determining
 * the delay needed for system restoration upon wake-up. If the system is
 * not in a deep sleep state, the function returns 0, indicating no
 * restoration delay is necessary. This function is particularly useful
 * in power management scenarios where precise timing for wake-up
 * processes is critical.
 *
 * @return Returns the wake-up restore process time in ticks as a 32-bit
 * unsigned integer. If the system is not in a deep sleep state, it
 * returns 0.
 ******************************************************************************/
uint32_t sli_power_manager_get_restore_delay(void);

/***************************************************************************//**
 * @brief This function is used to start the process of restoring system
 * settings after waking up from a deep sleep state. It should be called
 * when the system needs to transition back to an active state and
 * restore any necessary configurations that were altered during sleep.
 * This function is only applicable if deep sleep is supported and
 * enabled in the system configuration. It is important to ensure that
 * this function is called in a context where it is safe to modify system
 * settings, typically after a wake-up event.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_initiate_restore(void);

/***************************************************************************//**
 * Performs pre sleep operations.
 *
 * @note Must only be called by the RTOS integration code.
 ******************************************************************************/
void sli_power_manager_pre_sleep(void);

/***************************************************************************//**
 * @brief This function retrieves the current energy mode of the system, which
 * is useful for determining the current power state. It can be called at
 * any time to check the energy mode, and it does not require any prior
 * setup or initialization. This function is particularly useful in power
 * management scenarios where the energy mode needs to be monitored or
 * logged.
 *
 * @return Returns the current energy mode as a value of type
 * `sl_power_manager_em_t`.
 ******************************************************************************/
sl_power_manager_em_t sli_power_manager_get_current_em(void);

/***************************************************************************//**
 * @brief This function is used to configure the settings for Energy Mode 4
 * (EM4) in the power management module. It should be called when the
 * system needs to update or initialize the EM4 settings, typically
 * during the system's power management setup phase. This function does
 * not take any parameters and does not return any values. It is designed
 * to be used internally within the power management system to ensure
 * that the EM4 settings are correctly applied based on the current
 * hardware configuration.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_init_em4(void);

/***************************************************************************//**
 * Enable or disable fast wake-up in EM2 and EM3
 *
 * @note Will also update the wake up time from EM2 to EM0.
 ******************************************************************************/
void sli_power_manager_em23_voltage_scaling_enable_fast_wakeup(bool enable);

/***************************************************************************//**
 * @brief This function prepares the energy mode transition event list for use,
 * ensuring it is in a clean state before any operations are performed on
 * it. It should be called before any energy mode transition events are
 * added or processed, typically during the initialization phase of the
 * power management system. This function does not take any parameters
 * and does not return any values, making it straightforward to use as
 * part of the setup process.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_em_transition_event_list_init(void);

/***************************************************************************//**
 * Notify subscribers about energy mode transition.
 *
 * @param  from  Energy mode from which CPU comes from.
 *
 * @param  to    Energy mode to which CPU is going to.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to inform registered subscribers about a
 * transition between two energy modes. It should be called whenever the
 * system transitions from one energy mode to another, allowing
 * subscribers to react accordingly. The function requires valid energy
 * mode identifiers for both the current and target modes. It is crucial
 * for maintaining system behavior consistency across energy mode
 * changes.
 *
 * @param from The energy mode from which the CPU is transitioning. Must be a
 * valid `sl_power_manager_em_t` value representing a defined energy
 * mode.
 * @param to The energy mode to which the CPU is transitioning. Must be a valid
 * `sl_power_manager_em_t` value representing a defined energy mode.
 * @return None
 ******************************************************************************/
void sli_power_manager_notify_em_transition(sl_power_manager_em_t from,
                                            sl_power_manager_em_t to);

#ifdef __cplusplus
}
#endif

#endif /* SLI_POWER_MANAGER_H */
