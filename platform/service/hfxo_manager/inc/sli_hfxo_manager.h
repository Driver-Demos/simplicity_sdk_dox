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

#ifndef SLI_HFXO_MANAGER_H
#define SLI_HFXO_MANAGER_H

#include <stdbool.h>
#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_SLEEPTIMER_PRESENT) && defined(SYSRTC_PRESENT)
#include "sli_sleeptimer.h"
#if (SL_SLEEPTIMER_PERIPHERAL == SL_SLEEPTIMER_PERIPHERAL_SYSRTC)
#define HFXO_MANAGER_SLEEPTIMER_SYSRTC_INTEGRATION_ON
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function sets up the necessary hardware configurations for the
 * HFXO Manager module, ensuring that the HFXO interrupt has a high
 * priority and is enabled. It should be called before any operations
 * that depend on the HFXO being properly configured. The function
 * handles interrupt priority settings and enables necessary interrupts,
 * preparing the system for HFXO operations. It is crucial to call this
 * function during the initialization phase of the application to ensure
 * that the HFXO is correctly set up and ready for use.
 *
 * @return None
 ******************************************************************************/
void sli_hfxo_manager_init_hardware(void);

/***************************************************************************//**
 * @brief This function should be called immediately before starting the High-
 * Frequency Crystal Oscillator (HFXO) to begin measuring its startup
 * time. It records the current tick count, which is used as a reference
 * point for calculating the startup duration. This function is part of
 * the HFXO Manager module and is essential for accurate measurement of
 * the oscillator's startup time. Ensure that the HFXO Manager hardware
 * has been initialized before calling this function.
 *
 * @return None
 ******************************************************************************/
void sli_hfxo_manager_begin_startup_measurement(void);

#if defined(HFXO_MANAGER_SLEEPTIMER_SYSRTC_INTEGRATION_ON)
/***************************************************************************//**
 * @brief This function should be called when a SYSRTC compare match event
 * triggers a PRS signal to start the HFXO. It checks if the current
 * timer frequency is greater than the ULFRCO clock frequency, as the
 * ULFRCO lacks the precision needed for measuring HFXO startup time. If
 * the condition is met, it captures the current tick count to begin
 * measuring the HFXO startup time. This function is only relevant when
 * the integration with the SYSRTC is enabled, as indicated by the
 * preprocessor directive.
 *
 * @return None
 ******************************************************************************/
void sli_hfxo_manager_retrieve_begining_startup_measurement(void);
#endif

/***************************************************************************//**
 * @brief This function should be called immediately after the High-Frequency
 * Crystal Oscillator (HFXO) becomes ready. It calculates the startup
 * time by comparing the current tick count with the previously saved
 * tick count from when the startup measurement began. The function
 * ensures that the measured startup time is at least one tick and checks
 * if the measured time is within a valid range. If the measurement is
 * valid, it updates the average startup time and the startup time table.
 * This function must be called only if a startup measurement was
 * initiated.
 *
 * @return None
 ******************************************************************************/
void sli_hfxo_manager_end_startup_measurement(void);

/***************************************************************************//**
 * @brief Use this function to obtain the average startup time of the High-
 * Frequency Crystal Oscillator (HFXO) in ticks. This function is
 * typically called after the HFXO has been initialized and startup
 * measurements have been conducted. It provides a measure of the time
 * taken for the HFXO to become ready, which can be useful for
 * performance monitoring and optimization. Ensure that the startup
 * measurement process has been completed before calling this function to
 * get an accurate average value.
 *
 * @return Returns the average startup time of the HFXO in ticks as a 32-bit
 * unsigned integer.
 ******************************************************************************/
uint32_t sli_hfxo_manager_get_startup_time(void);

/***************************************************************************//**
 * @brief Use this function to obtain the most recent startup time of the High-
 * Frequency Crystal Oscillator (HFXO). This function is useful for
 * monitoring and diagnostics purposes, allowing you to track the time
 * taken for the HFXO to become ready after being started. It should be
 * called after the HFXO has been started and the startup time has been
 * measured using the appropriate functions in the HFXO Manager API.
 *
 * @return Returns the latest HFXO startup time as a 32-bit unsigned integer,
 * representing the time in ticks.
 ******************************************************************************/
uint32_t sli_hfxo_manager_get_latest_startup_time(void);

/***************************************************************************//**
 * @brief This function determines whether the High-Frequency Crystal Oscillator
 * (HFXO) is ready for use. It can be used in scenarios where the
 * readiness of the HFXO is critical before proceeding with operations
 * that depend on it. The function provides an option to wait until the
 * HFXO is ready, which is useful in synchronous operations where
 * subsequent actions depend on the HFXO's state. It should be noted that
 * the function ensures that the HFXO is not in the process of being
 * restarted with different settings, which could affect its readiness.
 *
 * @param wait A boolean value indicating whether the function should wait for
 * the HFXO to become ready. If set to true, the function will block
 * until the HFXO is ready. If false, the function will return
 * immediately with the current readiness status. The caller retains
 * ownership of this parameter.
 * @return Returns true if the HFXO is ready; otherwise, returns false.
 ******************************************************************************/
bool sli_hfxo_manager_is_hfxo_ready(bool wait);

#ifdef __cplusplus
}
#endif

#endif /* SLI_HFXO_MANAGER_H */
