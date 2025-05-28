/***************************************************************************//**
 * @file
 * @brief HFXO Manager API definition.
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

/***************************************************************************//**
 * @addtogroup hfxo_manager HFXO Manager
 * @brief HFXO Manager
 * @details
 * ## Overview
 *
 * HFXO Manager is a platform service module intended to manage the High
 * Frequency Crystal Oscillator (HFXO) module and offer related functionalities
 * and services.
 * For the moment, this module is only supported on Silicon Labs series 2
 * devices.
 * Among others, it handles the HFXO startup failures. This is to support
 * sleepy crystals (crystals where the ESR value could change unexpectedly up
 * to 5 times its value during the startup). In case of a failure during the
 * HFXO startup, the HFXO Manager will retry the startup process with more
 * aggressive settings (sleepy crystal settings) to try waking up the crystal
 * from its sleepy state so that the ESR value can fall back to normal values.
 * Once the crystal is out of its sleepy state, the module will put back the
 * normal settings ensuring the right oscillation frequency. This feature can
 * be enabled/disabled via the configuration define
 * SL_HFXO_MANAGER_SLEEPY_CRYSTAL_SUPPORT.
 * The module catches startup failures through interrupts using the HFXO
 * interrupt handler. If your application also needs the HFXO interrupt
 * handler, the configuration define SL_HFXO_MANAGER_CUSTOM_HFXO_IRQ_HANDLER
 * can be used to remove the HFXO interrupt handler definition from the HFXO
 * Manager so that it can be defined in your application. In that case, your
 * definition of the HFXO Interrupt Handler will need to call the
 * sl_hfxo_manager_irq_handler() function so that HFXO Manager can continue
 * to work properly.
 * The HFXO Manager is also required by the Power Manager module for some
 * internal features and therefore becomes mandatory every time the Power
 * Manager is present.
 *
 *
 * ## Initialization
 *
 * Two functions are required to initialize the module.
 * sl_hfxo_manager_init_hardware() is to initialize the HFXO interrupts and
 * must therefore be called before any other HFXO initialization functions like
 * the emlib CMU_HFXOInit() or device_init function sl_device_init_hfxo().
 * The second initialization function sl_hfxo_manager_init() is required for
 * internal use and needs to be called before going to sleep.
 *
 * @{
 ******************************************************************************/

#ifndef SL_HFXO_MANAGER_H
#define SL_HFXO_MANAGER_H

#include "sl_status.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Sleepy Crystal settings
/***************************************************************************//**
 * @brief The `sl_hfxo_manager_sleepy_xtal_settings_t` structure is used to
 * define settings for managing sleepy crystals in the High Frequency
 * Crystal Oscillator (HFXO) module. It contains parameters for tuning
 * capacitance and core bias current, which are crucial for ensuring
 * proper oscillation during different stages of the HFXO operation,
 * especially when dealing with crystals that may exhibit unexpected
 * changes in their equivalent series resistance (ESR) during startup.
 * This structure is part of the HFXO Manager API, which is designed to
 * handle startup failures and optimize settings to wake up crystals from
 * a sleepy state.
 *
 * @param ana_ctune Tuning Capacitance values for XI and XO during startup
 * intermediate and steady stages.
 * @param core_bias_current Core Bias current value during all stages.
 ******************************************************************************/
typedef struct sl_hfxo_manager_sleepy_xtal_settings {
  uint32_t ana_ctune;         ///< Tuning Capacitance values for XI and XO during startup intermediate and steady stages
  uint32_t core_bias_current; ///< Core Bias current value during all stages
} sl_hfxo_manager_sleepy_xtal_settings_t;

/***************************************************************************//**
 * @brief This function is used to initialize the hardware-specific components
 * necessary for the HFXO Manager module to function correctly. It must
 * be called before any other HFXO initialization functions, such as
 * emlib CMU_HFXOInit() or sl_device_init_hfxo(), to ensure that the HFXO
 * interrupts are properly set up. This is a crucial step in the
 * initialization process of the HFXO Manager, which is responsible for
 * managing the High Frequency Crystal Oscillator (HFXO) and handling
 * startup failures, especially in devices with sleepy crystals. Proper
 * initialization is necessary for the module to manage HFXO startup
 * failures and to support the Power Manager module when present.
 *
 * @return None
 ******************************************************************************/
void sl_hfxo_manager_init_hardware(void);

/***************************************************************************//**
 * @brief This function initializes the HFXO Manager module, which is essential
 * for managing the High Frequency Crystal Oscillator (HFXO) on Silicon
 * Labs series 2 devices. It must be called before the system enters
 * sleep mode to ensure proper HFXO management, including handling
 * startup failures and integrating with the Sleeptimer module. The
 * function should be used after initializing the hardware-specific
 * components with sl_hfxo_manager_init_hardware(). It returns a status
 * code indicating success or failure, which should be checked to ensure
 * the initialization was successful.
 *
 * @return Returns a status code of type sl_status_t indicating the success or
 * failure of the initialization process.
 ******************************************************************************/
sl_status_t sl_hfxo_manager_init(void);

/***************************************************************************//**
 * @brief This function is used to update the settings for a sleepy crystal
 * oscillator, which may be necessary if the default settings are
 * insufficient to wake up the crystal. It should be called when there is
 * a need to adjust the tuning capacitance and core bias current values
 * to ensure proper oscillation. The function requires a valid settings
 * structure and returns a status code indicating the success or failure
 * of the operation.
 *
 * @param settings A pointer to a sl_hfxo_manager_sleepy_xtal_settings_t
 * structure containing the new settings for the sleepy crystal.
 * The structure must not be null, and it should be properly
 * initialized with the desired tuning capacitance and core bias
 * current values.
 * @return A status code of type sl_status_t indicating the success or failure
 * of the operation.
 ******************************************************************************/
sl_status_t sl_hfxo_manager_update_sleepy_xtal_settings(const sl_hfxo_manager_sleepy_xtal_settings_t *settings);

/***************************************************************************//**
 * @brief This function is called when the High Frequency Crystal Oscillator
 * (HFXO) fails to start twice consecutively with normal configurations,
 * indicating a potential issue with the crystal. It serves as a callback
 * to notify the system of this error condition, where the HFXO is
 * operational but its frequency and precision are not guaranteed. This
 * function should be used to handle error situations related to HFXO
 * startup failures, and it is expected to be part of the error
 * management strategy for systems relying on HFXO.
 *
 * @return None
 ******************************************************************************/
void sl_hfxo_manager_notify_consecutive_failed_startups(void);

/***************************************************************************//**
 * @brief This function processes interrupts related to the High Frequency
 * Crystal Oscillator (HFXO) and is essential for the HFXO Manager module
 * to function correctly. It must be invoked by the HFXO interrupt
 * handler to manage various interrupt flags such as HFXO_IF_RDY,
 * HFXO_IF_DNSERR, and HFXO_XTALCTRL_SKIPCOREBIASOPT. This function is
 * crucial for handling startup failures and ensuring the HFXO operates
 * correctly, especially in systems with sleepy crystals. It is typically
 * used in conjunction with the Power Manager module and should be
 * integrated into applications that require HFXO management.
 *
 * @return None
 ******************************************************************************/
void sl_hfxo_manager_irq_handler(void);

/** @} (end addtogroup hfxo_manager) */

#ifdef __cplusplus
}
#endif

#endif // SL_HFXO_MANAGER_H
