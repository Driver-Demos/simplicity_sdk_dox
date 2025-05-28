/***************************************************************************//**
 * @file
 * @brief Power Manager Internal API definition.
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

#include "sl_power_manager.h"
#include "sl_slist.h"
#include "sl_code_classification.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_EMLIB_CORE_DEBUG_CONFIG_PRESENT)
#include "emlib_core_debug_config.h"
#endif

#if !defined(SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING)
#define SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING   0
#endif

#if (SL_EMLIB_CORE_ENABLE_INTERRUPT_DISABLED_TIMING == 1)
#include "sl_cycle_counter.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

#define SLI_POWER_MANAGER_EM_TABLE_SIZE  2

#define SLI_POWER_MANAGER_EM4_ENTRY_WAIT_LOOPS 200
/*******************************************************************************
 *****************************   DATA TYPES   *********************************
 ******************************************************************************/

// Debug entry
/***************************************************************************//**
 * @brief The `sli_power_debug_requirement_entry_t` structure is used within the
 * power manager's internal API to represent a debug requirement entry.
 * It contains a linked list node, allowing it to be part of a singly
 * linked list, and a string pointer to the module name, which identifies
 * the module associated with this debug requirement. This structure is
 * likely used to manage and track power-related debug requirements for
 * different modules in a system.
 *
 * @param node A linked list node used to link this entry in a singly linked
 * list.
 * @param module_name A constant character pointer to the name of the module
 * associated with this entry.
 ******************************************************************************/
typedef struct {
  sl_slist_node_t node;
  const char *module_name;
} sli_power_debug_requirement_entry_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void sli_power_manager_init_hardware(void);

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
void sli_power_manager_apply_em(sl_power_manager_em_t em);

/***************************************************************************//**
 * @brief This function sets up the power manager's debug system by initializing
 * a pool of debug entries. It should be called during the initialization
 * phase of the power management system to ensure that the debug
 * infrastructure is ready for use. This function prepares a list of free
 * debug entries that can be used to track power management requirements.
 * It does not take any parameters and does not return any value, making
 * it straightforward to integrate into the initialization sequence.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_debug_init(void);

#if !defined(SL_CATALOG_POWER_MANAGER_NO_DEEPSLEEP_PRESENT)
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to save the current states of high-frequency
 * clock sources before entering a low-power mode. It should be called as
 * part of the power management process to ensure that the clock
 * configurations can be restored correctly after waking up from a low-
 * power state. This function does not take any parameters and does not
 * return any value. It is typically used internally within the power
 * management system and should be called before any deep sleep
 * operations that might alter the clock settings.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_save_states(void);

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function should be called before the system enters deep sleep
 * mode to perform necessary pre-deep sleep operations. It is intended
 * for internal use within the power management system to ensure that the
 * system state is correctly set before transitioning to a low-power
 * state. This function does not take any parameters and does not return
 * any value. It must be called in contexts where deep sleep is supported
 * and expected to be entered.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_handle_pre_deepsleep_operations(void);

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to restore the high-frequency accuracy clock
 * (HFXO) after the system wakes from a low-power state, ensuring that
 * the clock is running and accurate. It should be called when the system
 * transitions from a low-power mode where the high-frequency clock might
 * have been disabled or its accuracy compromised. The function checks if
 * the high-frequency oscillator is used and whether it needs to be
 * started or restored. It handles the clock restoration process,
 * including measuring the time taken for the oscillator to stabilize, if
 * necessary. This function must be called in contexts where the high-
 * frequency clock is required to be accurate after waking from sleep.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_restore_high_freq_accuracy_clk(void);

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function determines whether the high-frequency accuracy clock is
 * ready for use. It should be called when there is a need to verify the
 * readiness of the high-frequency clock, particularly in scenarios
 * involving power management and clock restoration. The function can
 * optionally wait for the clock to be ready based on the input
 * parameter. It is important to note that if the high-frequency
 * oscillator is not used, the function will immediately return true,
 * indicating readiness. This function is typically used in environments
 * where power management and clock accuracy are critical.
 *
 * @param wait A boolean value indicating whether the function should wait for
 * the clock to be ready. If true, the function may block until the
 * clock is ready; if false, it will return the current readiness
 * status immediately. The behavior of waiting is dependent on the
 * system configuration and may not be applicable in all cases.
 * @return Returns a boolean value: true if the high-frequency accuracy clock is
 * ready, false otherwise.
 ******************************************************************************/
bool sli_power_manager_is_high_freq_accuracy_clk_ready(bool wait);

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to restore various system states and
 * configurations that were saved before entering a deep sleep mode. It
 * should be called after the system wakes up from deep sleep to ensure
 * that the system clock, DPLL, and other critical settings are returned
 * to their pre-sleep states. This function is crucial for maintaining
 * system stability and ensuring that the system operates correctly after
 * waking up from a low-power state. It is typically used in power
 * management routines where deep sleep modes are employed to save
 * energy.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_restore_states(void);

/***************************************************************************//**
 * @brief This function is used to convert a given delay specified in
 * microseconds into the equivalent number of sleeptimer ticks. It is
 * useful when working with time-based operations that require conversion
 * from a standard time unit to the specific tick count used by the
 * sleeptimer. The function assumes that the sleeptimer frequency is
 * defined and used in the conversion process. It is important to ensure
 * that the input value is within a valid range to prevent overflow or
 * unexpected behavior.
 *
 * @param time_us The delay in microseconds to be converted. It must be a non-
 * negative integer and should be within a range that prevents
 * overflow when multiplied by the sleeptimer frequency.
 * @return Returns the equivalent number of sleeptimer ticks as a 32-bit
 * unsigned integer.
 ******************************************************************************/
uint32_t sli_power_manager_convert_delay_us_to_tick(uint32_t time_us);

/***************************************************************************//**
 * @brief This function provides the default minimum offtime for the high-
 * frequency crystal oscillator (HFXO) in terms of sleeptimer ticks. It
 * is useful in scenarios where power management requires knowledge of
 * the minimum time the oscillator should remain off to ensure proper
 * operation and power efficiency. This function can be called whenever
 * the default minimum offtime is needed for power management decisions.
 *
 * @return The function returns a uint32_t value representing the default
 * minimum offtime for the high-frequency oscillator in sleeptimer
 * ticks.
 ******************************************************************************/
uint32_t sli_power_manager_get_default_high_frequency_minimum_offtime(void);

/*******************************************************************************
 * Restores the Low Frequency clocks according to which LF oscillators are used.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is intended to restore the low frequency clocks used by
 * the system. It should be called when there is a need to ensure that
 * the low frequency oscillators are operational. However, due to the on-
 * demand feature of the system, the low frequency oscillators are
 * automatically enabled when needed, making this function effectively a
 * no-op in typical scenarios. It is safe to call this function at any
 * time, and it does not require any specific preconditions or result in
 * any side effects.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_low_frequency_restore(void);

/***************************************************************************//**
 * Informs the power manager if the high accuracy/high frequency clock
 * is used, prior to scheduling an early clock restore.
 *
 * @return true if HFXO is used, else false.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function determines whether the high frequency accuracy clock
 * (HFXO) is in use, which is relevant for managing power states and
 * scheduling clock restores. It should be called when there is a need to
 * verify the current clock usage status, particularly before performing
 * operations that depend on the clock's state. The function is useful in
 * scenarios where power management decisions are based on the clock's
 * usage.
 *
 * @return Returns true if the high frequency accuracy clock is used, otherwise
 * returns false.
 ******************************************************************************/
bool sli_power_manager_is_high_freq_accuracy_clk_used(void);
#endif

/*******************************************************************************
 * Gets the delay associated the wake-up process from EM23.
 *
 * @return Delay for the complete wake-up process with full restore.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t sli_power_manager_get_wakeup_process_time_overhead(void);

#if !defined(SL_CATALOG_POWER_MANAGER_NO_DEEPSLEEP_PRESENT)
/***************************************************************************//**
 * @brief This function is used to determine whether the power manager is
 * currently in a state where it is sleeping and waiting for the clock to
 * be restored. It is useful for internal diagnostics or control flow
 * decisions where the system needs to know if it is in a waiting state
 * for clock restoration. This function should be used in contexts where
 * deep sleep states are managed, and it is important to know the status
 * of clock restoration. It is intended for internal use only and should
 * not be used in application-level code.
 *
 * @return Returns true if the power manager is sleeping and waiting for clock
 * restoration, otherwise returns false.
 ******************************************************************************/
bool sli_power_manager_get_clock_restore_status(void);
#endif

#if defined(SL_CATALOG_POWER_MANAGER_NO_DEEPSLEEP_PRESENT)
/*******************************************************************************
 * HAL hook function for pre EM1HCLKDIV sleep.
 *
 * @note FOR INTERNAL USE ONLY.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
void sli_power_manager_em1hclkdiv_presleep_operations(void);

/*******************************************************************************
 * HAL hook function for post EM1HCLKDIV sleep.
 *
 * @note FOR INTERNAL USE ONLY.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
void sli_power_manager_em1hclkdiv_postsleep_operations(void);
#endif

#ifdef __cplusplus
}
#endif
