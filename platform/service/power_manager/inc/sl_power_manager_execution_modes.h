/***************************************************************************//**
 * @file
 * @brief Power Manager execution modes API definition.
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

#ifndef SL_POWER_MANAGER_EXECUTION_MODES_H
#define SL_POWER_MANAGER_EXECUTION_MODES_H

#include "cmsis_compiler.h"
#include "sl_code_classification.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * Updates execution mode requirements.
 *
 * @param add boolean value indicating whether to add or remove requirement.
 *
 * @note FOR INTERNAL USE ONLY.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function adjusts the requirement for the performance execution
 * mode by either incrementing or decrementing a counter based on the
 * provided boolean parameter. It is intended for internal use only and
 * should be used when there is a need to modify the execution mode
 * requirements, such as when entering or exiting a performance-critical
 * section. The function ensures that the counter does not exceed its
 * bounds, preventing wraparound errors. It should be called only when
 * the execution modes feature is enabled.
 *
 * @param add A boolean value indicating whether to add (true) or remove (false)
 * a requirement for the performance execution mode. The parameter
 * must be used carefully to avoid exceeding the counter's limits.
 * @return None
 ******************************************************************************/
void sli_power_manager_update_execution_mode_requirement(bool  add);

/***************************************************************************//**
 * Adds requirement on performance execution mode.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The function `sl_power_manager_add_performance_mode_requirement` adds
 * a requirement for the system to operate in performance execution mode.
 *
 * @return There is no direct output from this function as it is a void
 * function, but it modifies the system's execution mode requirements by
 * adding a performance mode requirement.
 ******************************************************************************/
__INLINE void sl_power_manager_add_performance_mode_requirement(void)
{
  sli_power_manager_update_execution_mode_requirement(true);
}

/***************************************************************************//**
 * Removes requirement on performance execution mode.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The function `sl_power_manager_remove_performance_mode_requirement`
 * removes the requirement for the performance execution mode by updating
 * the execution mode requirement to false.
 *
 * @return The function does not return any value; it performs an action by
 * calling another function to update the execution mode requirement.
 ******************************************************************************/
__INLINE void sl_power_manager_remove_performance_mode_requirement(void)
{
  sli_power_manager_update_execution_mode_requirement(false);
}

/***************************************************************************//**
 * @brief This function sets up the execution mode feature within the power
 * manager system. It is intended for internal use only and should be
 * called to ensure that the execution modes are properly initialized
 * before any execution mode requirements are added or removed. This
 * function does not take any parameters and does not return any values.
 * It must be called before any other execution mode management functions
 * are used to ensure proper operation.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_executions_modes_init(void);

/***************************************************************************//**
 * Implement execution mode if not already implemented during a wakeup event.
 *
 * @note FOR INTERNAL USE ONLY.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to ensure that the appropriate execution mode is
 * applied when the system wakes up from a low-power state. It checks the
 * current system clock source and the performance mode requirements, and
 * applies the necessary changes to the execution mode if they have not
 * already been implemented. This function is intended for internal use
 * only and should be called during the wakeup process to ensure that the
 * system operates in the correct mode based on the current requirements.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_implement_execution_mode_on_wakeup(void);

#ifdef __cplusplus
}
#endif
#endif // SL_POWER_MANAGER_EXECUTION_MODES_H
