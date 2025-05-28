/***************************************************************************//**
 * @file
 * @brief SLEEPTIMER SDK internal APIs.
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

#ifndef SLI_SLEEPTIMER_H
#define SLI_SLEEPTIMER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "em_device.h"
#include "sl_sleeptimer_config.h"
#include "sl_code_classification.h"

#define SLEEPTIMER_EVENT_OF (0x01)
#define SLEEPTIMER_EVENT_COMP (0x02)

#define SLI_SLEEPTIMER_POWER_MANAGER_EARLY_WAKEUP_TIMER_FLAG 0x02
#define SLI_SLEEPTIMER_POWER_MANAGER_HF_ACCURACY_CLK_FLAG 0x04

#if SL_SLEEPTIMER_PERIPHERAL == SL_SLEEPTIMER_PERIPHERAL_DEFAULT
#if defined(RTCC_PRESENT) && RTCC_COUNT >= 1
#undef SL_SLEEPTIMER_PERIPHERAL
#define SL_SLEEPTIMER_PERIPHERAL SL_SLEEPTIMER_PERIPHERAL_RTCC
#elif defined(RTC_PRESENT) && RTC_COUNT >= 1
#undef SL_SLEEPTIMER_PERIPHERAL
#define SL_SLEEPTIMER_PERIPHERAL SL_SLEEPTIMER_PERIPHERAL_RTC
#elif defined(SYSRTC_PRESENT) && SYSRTC_COUNT >= 1
#undef SL_SLEEPTIMER_PERIPHERAL
#define SL_SLEEPTIMER_PERIPHERAL SL_SLEEPTIMER_PERIPHERAL_SYSRTC
#elif defined(BURTC_PRESENT) && BURTC_COUNT >= 1
#undef SL_SLEEPTIMER_PERIPHERAL
#define SL_SLEEPTIMER_PERIPHERAL SL_SLEEPTIMER_PERIPHERAL_BURTC
#elif defined(WTIMER_PRESENT) && WTIMER_COUNT >= 1
#undef SL_SLEEPTIMER_PERIPHERAL
#define SL_SLEEPTIMER_PERIPHERAL SL_SLEEPTIMER_PERIPHERAL_WTIMER
#elif defined(TIMER_PRESENT) && TIMER_COUNT >= 1
#undef SL_SLEEPTIMER_PERIPHERAL
#define SL_SLEEPTIMER_PERIPHERAL SL_SLEEPTIMER_PERIPHERAL_TIMER
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function sets up the necessary hardware connections to integrate
 * the power manager with the sleeptimer, enabling early wakeup
 * functionality through the HFXO. It should be called during the
 * initialization phase of the system to ensure that the power management
 * features are correctly configured. This function does not take any
 * parameters and does not return any values. It is designed to be used
 * in systems where power management and precise timing are critical, and
 * it must be called before any power management operations that depend
 * on the sleeptimer.
 *
 * @return None
 ******************************************************************************/
__WEAK void sli_sleeptimer_hal_power_manager_integration_init(void);

/***************************************************************************//**
 * @brief This function sets up the necessary hardware connections and
 * configurations for integrating the High-Frequency Crystal Oscillator
 * (HFXO) with the sleeptimer. It should be called during the
 * initialization phase of the system to ensure that the HFXO is properly
 * connected to the system real-time clock (SYSRTC) via the Peripheral
 * Reflex System (PRS). This setup is crucial for systems that rely on
 * precise timing and clock management. The function does not take any
 * parameters and does not return a value, indicating that it performs
 * its operations internally without requiring external input or
 * providing output.
 *
 * @return None
 ******************************************************************************/
__WEAK void sli_sleeptimer_hal_hfxo_manager_integration_init(void);

/*******************************************************************************
 * Hardware Abstraction Layer to get interrupt status.
 *
 * @param local_flag Internal interrupt flag.
 *
 * @return Boolean indicating if specified interrupt is set.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
bool sli_sleeptimer_hal_is_int_status_set(uint8_t local_flag);

/**************************************************************************//**
 * Determines if next timer to expire has the option flag
 * "SLI_SLEEPTIMER_POWER_MANAGER_EARLY_WAKEUP_TIMER_FLAG".
 *
 * @return true if power manager will expire at next compare match,
 *         false otherwise.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function determines whether the next scheduled timer to expire is
 * associated with the power manager, specifically if it has the option
 * flag `SLI_SLEEPTIMER_POWER_MANAGER_EARLY_WAKEUP_TIMER_FLAG`. It should
 * be used when there is a need to verify if the power manager's timer is
 * the next one to trigger, which can be crucial for managing power
 * states effectively. The function returns a boolean indicating the
 * status, and it ensures that the timer is not only next to expire but
 * also actually expired.
 *
 * @return Returns `true` if the power manager timer is the next to expire and
 * is actually expired; otherwise, returns `false`.
 ******************************************************************************/
bool sli_sleeptimer_is_power_manager_timer_next_to_expire(void);

/***************************************************************************//**
 * Set lowest energy mode based on a project's configurations and clock source
 *
 * @note If power_manager_no_deepsleep component is included in a project, the
 *       lowest possible energy mode is EM1, else lowest energy mode is
 *       determined by clock source.
 ******************************************************************************/
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
void sli_sleeptimer_set_pm_em_requirement(void);
#endif

/***************************************************************************//**
 * @brief
 *   Update sleep_on_isr_exit flag.
 *
 * @param flag Boolean value update_sleep_on_isr_exit will be set to.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function sets the behavior of the system regarding whether it
 * should enter sleep mode upon exiting an interrupt service routine
 * (ISR). It is typically used in power management scenarios where the
 * system's energy mode needs to be controlled based on interrupt
 * activity. The function should be called whenever there is a need to
 * change the sleep behavior after ISR execution, ensuring that the
 * system's power management aligns with the application's requirements.
 *
 * @param flag A boolean value indicating the desired behavior. If true, the
 * system will enter sleep mode upon ISR exit; if false, it will
 * not. The caller retains ownership of this parameter, and it must
 * be a valid boolean value.
 * @return None
 ******************************************************************************/
void sli_sleeptimer_update_sleep_on_isr_exit(bool flag);

/*******************************************************************************
 * Gets the associated peripheral capture channel current value.
 *
 * @return Capture value
 *         0 if capture channel is not valid
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the current capture value from the
 * associated peripheral's capture channel. It is typically called when
 * there is a need to read the current state or value captured by the
 * peripheral. The function should be used in contexts where the capture
 * channel is valid and properly configured. If the capture channel is
 * not valid, the function will return 0, indicating an invalid or
 * uninitialized state. This function is useful in time-critical
 * applications where precise timing or state capture is required.
 *
 * @return Returns the current capture value as a 32-bit unsigned integer, or 0
 * if the capture channel is not valid.
 ******************************************************************************/
uint32_t sli_sleeptimer_get_capture(void);

/*******************************************************************************
 * Resets the PRS signal triggered by the associated peripheral.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to reset the PRS (Peripheral Reflex System)
 * signal that is triggered by the associated peripheral. It should be
 * called when there is a need to clear or reset the PRS signal state,
 * typically after handling the event that caused the signal. This
 * function does not require any parameters and does not return any
 * value. It is expected to be used in contexts where the PRS signal
 * needs to be managed or controlled as part of the system's operation.
 *
 * @return None
 ******************************************************************************/
void sli_sleeptimer_reset_prs_signal(void);

#ifdef __cplusplus
}
#endif

#endif /* SLI_SLEEPTIMER_H */
