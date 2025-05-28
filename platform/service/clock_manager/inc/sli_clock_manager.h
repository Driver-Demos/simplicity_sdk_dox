/***************************************************************************//**
 * @file
 * @brief Clock Manager Private API definition.
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

#ifndef SLI_CLOCK_MANAGER_H
#define SLI_CLOCK_MANAGER_H

#include "sl_clock_manager.h"
#include "sl_status.h"
#include "sl_compiler.h"
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * Set SYSCLK clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function sets the system clock (SYSCLK) source to the specified
 * oscillator. It is used to configure the clock source for the system,
 * which is critical for ensuring the correct operation of the device.
 * The function should be called when a change in the clock source is
 * required, such as during initialization or when switching between
 * different power modes. It is important to ensure that the specified
 * oscillator is available and stable before calling this function to
 * avoid system instability.
 *
 * @param source The oscillator to be used as the SYSCLK source. It must be a
 * valid `sl_oscillator_t` value representing an available and
 * stable oscillator. Invalid values may result in undefined
 * behavior.
 * @return Returns an `sl_status_t` indicating the success or failure of the
 * operation. Possible return values include success or error codes
 * related to invalid parameters or hardware issues.
 ******************************************************************************/
sl_status_t sli_clock_manager_set_sysclk_source(sl_oscillator_t source);

/***************************************************************************//**
 * Get SYSCLK clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the current system clock (SYSCLK)
 * source. It is essential to call this function when you need to
 * determine which oscillator is currently being used as the SYSCLK
 * source. The function requires a valid pointer to a sl_oscillator_t
 * variable where the result will be stored. It is important to ensure
 * that the pointer is not null before calling this function, as passing
 * a null pointer will result in an error status being returned.
 *
 * @param source A pointer to a sl_oscillator_t variable where the current
 * SYSCLK source will be stored. Must not be null. If null, the
 * function returns SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. SL_STATUS_NULL_POINTER is returned if the source pointer
 * is null.
 ******************************************************************************/
sl_status_t sli_clock_manager_get_sysclk_source(sl_oscillator_t *source);

/***************************************************************************//**
 * @brief This function should be called when the High-Frequency Crystal
 * Oscillator (HFXO) is ready to be used. It ensures that if the system
 * is in performance mode and the current system clock source is not the
 * SOCPLL, the system clock source is switched to SOCPLL. Additionally,
 * it clears the FORCEEN bit in the HFXO control register to allow normal
 * operation. This function must be called in a context where interrupts
 * are disabled to prevent race conditions.
 *
 * @return None
 ******************************************************************************/
__WEAK void sli_clock_manager_notify_hfxo_ready(void);

#ifdef __cplusplus
}
#endif

#endif // SLI_CLOCK_MANAGER_H
