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

#include "sl_power_manager_config.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

#if defined(SL_POWER_MANAGER_EXECUTION_MODES_FEATURE_EN) && (SL_POWER_MANAGER_EXECUTION_MODES_FEATURE_EN == 1)

/*******************************************************************************
 *****************************   HAL PROTOTYPES   ******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function sets up the power manager to operate in its standard
 * execution mode. It is intended for internal use only and should be
 * called to ensure that the power manager is correctly initialized to
 * handle execution modes. This function must be used in environments
 * where the execution modes feature is enabled, as indicated by the
 * relevant configuration macros.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_executions_modes_hal_init(void);

/***************************************************************************//**
 * Applies performance mode execution.
 *
 * @note FOR INTERNAL USE ONLY.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function configures the system to operate in performance mode,
 * which is intended for scenarios requiring maximum processing power. It
 * should be used when the system needs to switch to a high-performance
 * state, typically after ensuring that the necessary hardware
 * components, such as oscillators, are ready. The function is designed
 * for internal use only and should be called in contexts where
 * performance mode is required and supported by the hardware
 * configuration.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_hal_apply_performance_mode(void);

/***************************************************************************//**
 * Applies standard mode execution.
 *
 * @note FOR INTERNAL USE ONLY.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function sets the system clock source to the standard mode, which
 * is intended for internal use only. It should be called when the system
 * needs to switch to a standard execution mode, typically to balance
 * performance and power consumption. The function assumes that the
 * execution modes feature is enabled and that the system is in a state
 * where changing the clock source is safe. It asserts that the operation
 * is successful, which implies that it should be used in environments
 * where assertions are enabled and failure to set the clock source is
 * considered critical.
 *
 * @return None
 ******************************************************************************/
void sli_power_manager_hal_apply_standard_mode(void);
#endif // (SL_POWER_MANAGER_EXECUTION_MODES_FEATURE_EN)

#ifdef __cplusplus
}
#endif
