/***************************************************************************//**
 * @file
 * @brief Clock Manager HAL APIs.
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

#ifndef SL_CLOCK_MANAGER_HAL_H
#define SL_CLOCK_MANAGER_HAL_H

#include "sl_clock_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function is used to initialize the runtime environment for the
 * Clock Manager. It should be called before any other Clock Manager
 * operations are performed to ensure that the system is properly set up.
 * This function does not require any parameters and will always return a
 * success status, indicating that the initialization has been completed
 * successfully. It is a simple setup function with no side effects or
 * dependencies on other system components.
 *
 * @return Returns SL_STATUS_OK to indicate successful initialization.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_runtime_init(void);

/***************************************************************************//**
 * Gets frequency of given oscillator.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the frequency of a specified
 * oscillator within the system. It requires a valid oscillator
 * identifier and a pointer to a variable where the frequency will be
 * stored. The function must be called with a valid oscillator type;
 * otherwise, it will return an error status. It is important to ensure
 * that the pointer provided for the frequency is not null, as the
 * function will attempt to write the frequency value to this location.
 * The function returns a status code indicating success or failure,
 * allowing the caller to handle any errors appropriately.
 *
 * @param oscillator An identifier for the oscillator whose frequency is to be
 * retrieved. Must be a valid sl_oscillator_t value. If an
 * invalid oscillator is provided, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @param frequency A pointer to a uint32_t variable where the frequency of the
 * specified oscillator will be stored. Must not be null, as
 * the function writes the frequency value to this location.
 * @return Returns SL_STATUS_OK on success, or SL_STATUS_INVALID_PARAMETER if
 * the oscillator is invalid. The frequency is written to the provided
 * pointer if successful.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_oscillator_frequency(sl_oscillator_t oscillator,
                                                           uint32_t        *frequency);

/***************************************************************************//**
 * @brief This function is used to obtain the precision of a specified
 * oscillator, which is represented as a 16-bit unsigned integer. It is
 * essential to call this function when you need to determine the
 * precision of an oscillator for configuration or diagnostic purposes.
 * The function requires a valid oscillator identifier and a pointer to
 * store the precision value. If the oscillator is not supported or the
 * precision is not available, the function returns an appropriate status
 * code and sets the precision to a default value. Ensure that the
 * pointer provided is valid and that the function is called in a context
 * where the oscillator's precision is relevant.
 *
 * @param oscillator An identifier of type sl_oscillator_t representing the
 * oscillator whose precision is to be retrieved. Must be a
 * valid oscillator type; otherwise, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @param precision A pointer to a uint16_t where the precision of the
 * oscillator will be stored. Must not be null. The caller must
 * ensure that the pointer is valid and points to a writable
 * memory location.
 * @return Returns sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success,
 * SL_STATUS_NOT_AVAILABLE if the precision is not available for the
 * specified oscillator, and SL_STATUS_INVALID_PARAMETER if an invalid
 * oscillator is specified.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_oscillator_precision(sl_oscillator_t oscillator,
                                                           uint16_t        *precision);

/***************************************************************************//**
 * Gets frequency of given clock branch.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the frequency of a specified clock
 * branch within the system. It is essential to call this function when
 * you need to know the operating frequency of a particular clock branch
 * for configuration or monitoring purposes. The function requires a
 * valid clock branch identifier and a pointer to store the frequency
 * result. If the specified clock branch is not supported or invalid, the
 * function will return an error status and set the frequency to zero. It
 * is important to ensure that the frequency pointer is not null to avoid
 * undefined behavior.
 *
 * @param clock_branch An identifier for the clock branch whose frequency is to
 * be retrieved. It must be a valid value from the
 * sl_clock_branch_t enumeration. If an invalid clock branch
 * is provided, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @param frequency A pointer to a uint32_t where the frequency of the specified
 * clock branch will be stored. This pointer must not be null,
 * and the caller retains ownership of the memory. If the clock
 * branch is invalid, the frequency will be set to zero.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while
 * SL_STATUS_INVALID_PARAMETER is returned if the clock branch is
 * invalid.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_clock_branch_frequency(sl_clock_branch_t clock_branch,
                                                             uint32_t          *frequency);

/***************************************************************************//**
 * @brief This function is used to obtain the precision of a specified clock
 * branch, which is identified by the `clock_branch` parameter. It is
 * essential to call this function when you need to determine the
 * precision of a clock branch for configuration or diagnostic purposes.
 * The function writes the precision value to the location pointed to by
 * the `precision` parameter. It handles various clock branches,
 * returning a status code that indicates success, unsupported branches,
 * or invalid parameters. Ensure that the `precision` pointer is valid
 * and that the function is called in a context where atomic operations
 * are safe.
 *
 * @param clock_branch An identifier for the clock branch whose precision is to
 * be retrieved. It must be a valid `sl_clock_branch_t`
 * value. If the clock branch is not supported or invalid,
 * the function returns an appropriate status code.
 * @param precision A pointer to a `uint16_t` where the precision of the
 * specified clock branch will be stored. This pointer must not
 * be null, and the caller retains ownership. If the clock
 * branch is unsupported or invalid, the value pointed to may
 * be set to 0.
 * @return Returns an `sl_status_t` indicating the result of the operation:
 * `SL_STATUS_OK` for success, `SL_STATUS_NOT_SUPPORTED` for unsupported
 * branches, `SL_STATUS_NOT_AVAILABLE` if precision cannot be
 * determined, or `SL_STATUS_INVALID_PARAMETER` for invalid input.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_clock_branch_precision(sl_clock_branch_t clock_branch,
                                                             uint16_t          *precision);

/***************************************************************************//**
 * @brief This function is used to control the bus clock associated with a
 * specific module, allowing it to be enabled or disabled as needed. It
 * is essential to ensure that the module_bus_clock parameter is valid
 * before calling this function, as an invalid value will result in the
 * function returning an error status. This function is typically used in
 * scenarios where power management or module-specific clock control is
 * required. It is important to note that the function will return a
 * status indicating success or failure, which should be checked to
 * ensure the operation was performed as expected.
 *
 * @param module Specifies the bus clock associated with the module to be
 * controlled. Must be a valid sl_bus_clock_t value. If set to
 * SL_BUS_CLOCK_INVALID, the function will return
 * SL_STATUS_NOT_AVAILABLE.
 * @param enable A boolean value indicating whether to enable (true) or disable
 * (false) the bus clock for the specified module. The caller
 * retains ownership of this parameter.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while
 * SL_STATUS_NOT_AVAILABLE is returned if the module parameter is
 * invalid.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_enable_bus_clock(sl_bus_clock_t module,
                                                   bool enable);

/***************************************************************************//**
 * @brief This function configures a specified GPIO pin to output a clock signal
 * from a user-selected clock source. It is used when there is a need to
 * export a clock signal to an external device or component. The function
 * must be called with a valid clock source and output selection. It is
 * important to ensure that the selected clock source is supported by the
 * hardware. If an unsupported clock source is specified, the function
 * will return an error status. The function operates atomically to
 * prevent race conditions during configuration.
 *
 * @param export_clock_source Specifies the clock source to be exported. Must be
 * a valid value from the
 * sl_clock_manager_export_clock_source_t
 * enumeration. If an unsupported value is provided,
 * the function returns SL_STATUS_NOT_SUPPORTED.
 * @param output_select Determines which output pin configuration to use. Must
 * be a valid value from the
 * sl_clock_manager_export_clock_output_select_t
 * enumeration.
 * @param divider Specifies the clock divider to apply to the clock source. Must
 * be a non-negative integer.
 * @param port Specifies the GPIO port number where the clock signal will be
 * output. Must be a valid port number.
 * @param pin Specifies the GPIO pin number on the specified port where the
 * clock signal will be output. Must be a valid pin number.
 * @return Returns SL_STATUS_OK on success or SL_STATUS_NOT_SUPPORTED if the
 * clock source is not supported.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_gpio_clock_output(sl_clock_manager_export_clock_source_t export_clock_source,
                                                        sl_clock_manager_export_clock_output_select_t output_select,
                                                        uint16_t divider,
                                                        uint32_t port,
                                                        uint32_t pin);

/***************************************************************************//**
 * @brief This function is used to adjust the frequency tuning of a specified RC
 * oscillator. It should be called when there is a need to calibrate the
 * oscillator to achieve a desired frequency. The function supports
 * specific oscillators, and if an unsupported oscillator is specified,
 * it returns an error status. It is important to ensure that the
 * oscillator parameter corresponds to a supported oscillator type before
 * calling this function.
 *
 * @param oscillator Specifies the RC oscillator to be calibrated. Must be one
 * of the supported oscillator types, such as
 * SL_OSCILLATOR_HFRCODPLL, SL_OSCILLATOR_HFRCOEM23 (if
 * available), or SL_OSCILLATOR_LFRCO. If an unsupported
 * oscillator is provided, the function returns
 * SL_STATUS_NOT_SUPPORTED.
 * @param val The calibration value to set for the specified oscillator. This
 * value is used to adjust the frequency tuning of the oscillator.
 * @return Returns SL_STATUS_OK if the calibration is successfully set, or
 * SL_STATUS_NOT_SUPPORTED if the specified oscillator is not supported.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                                uint32_t val);

/***************************************************************************//**
 * @brief This function is used to obtain the current frequency tuning setting
 * of a specified RC oscillator. It is useful for applications that need
 * to read the calibration value for monitoring or adjustment purposes.
 * The function requires a valid oscillator type to be specified, and it
 * writes the calibration value to the provided pointer. If the specified
 * oscillator is not supported, the function returns an error status.
 * Ensure that the pointer provided for the calibration value is not null
 * and that the oscillator type is supported by the hardware.
 *
 * @param oscillator Specifies the RC oscillator for which the calibration value
 * is to be retrieved. Must be a valid sl_oscillator_t value.
 * If the oscillator is not supported, the function returns
 * SL_STATUS_NOT_SUPPORTED.
 * @param val A pointer to a uint32_t where the calibration value will be
 * stored. Must not be null. The function writes the calibration
 * value to this location if the oscillator is supported.
 * @return Returns SL_STATUS_OK if the calibration value is successfully
 * retrieved, or SL_STATUS_NOT_SUPPORTED if the oscillator is not
 * supported.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                                uint32_t *val);

/***************************************************************************//**
 * @brief This function is used to set the calibration value for the High-
 * Frequency Crystal Oscillator (HFXO). It should be called when there is
 * a need to adjust the HFXO's frequency tuning to ensure accurate clock
 * operation. The function operates atomically to prevent interference
 * from interrupts during the calibration process. It is important to
 * ensure that the provided calibration value is within the valid range
 * for the specific hardware being used.
 *
 * @param val The calibration value to set for the HFXO. The valid range of this
 * value depends on the specific hardware and oscillator
 * characteristics. The caller must ensure that the value is
 * appropriate for the intended tuning.
 * @return Returns SL_STATUS_OK to indicate successful setting of the
 * calibration value.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_hfxo_calibration(uint32_t val);

/***************************************************************************//**
 * @brief This function is used to obtain the current calibration value of the
 * High-Frequency Crystal Oscillator (HFXO). It is typically called when
 * there is a need to verify or log the current calibration setting of
 * the HFXO. The function must be called with a valid pointer to a
 * uint32_t variable where the calibration value will be stored. It is
 * important to ensure that the pointer is not null to avoid undefined
 * behavior.
 *
 * @param val A pointer to a uint32_t variable where the HFXO calibration value
 * will be stored. Must not be null. The caller retains ownership of
 * the memory pointed to by this parameter.
 * @return Returns SL_STATUS_OK to indicate successful retrieval of the
 * calibration value.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_hfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * @brief This function sets the CTUNE value for the High-Frequency Crystal
 * Oscillator (HFXO), which is used to adjust the oscillator's tuning
 * capacitance. It should be called when there is a need to modify the
 * HFXO's tuning parameters, typically during system initialization or
 * when recalibrating the oscillator. The function operates atomically to
 * ensure thread safety. It returns a status code indicating the success
 * or failure of the operation.
 *
 * @param ctune The CTUNE value to set for the HFXO. It must be a valid 32-bit
 * unsigned integer representing the desired tuning capacitance.
 * The function will return an error status if the value is not
 * acceptable.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while other status
 * codes indicate specific errors.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_hfxo_set_ctune(uint32_t ctune);

/***************************************************************************//**
 * @brief Use this function to obtain the current CTUNE setting for the High-
 * Frequency Crystal Oscillator (HFXO). This function is typically called
 * when you need to read the current calibration value of the HFXO to
 * ensure it is set correctly or to log its value for diagnostic
 * purposes. It is important to ensure that the pointer provided is valid
 * and writable, as the function will store the CTUNE value at the
 * location pointed to by the parameter.
 *
 * @param ctune A pointer to a uint32_t where the current HFXO CTUNE setting
 * will be stored. Must not be null, and the caller must ensure the
 * pointer is valid and points to a writable memory location.
 * @return Returns SL_STATUS_OK to indicate successful retrieval of the CTUNE
 * value.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_hfxo_get_ctune(uint32_t *ctune);

/***************************************************************************//**
 * @brief This function is used to set the HFXO CTUNE value and perform
 * calibration of the Core Bias Current. It should be called when there
 * is a need to adjust the tuning capacitance of the HFXO to ensure
 * optimal performance. The function waits for the HFXO to be ready
 * before applying the CTUNE value and performing the calibration. It is
 * important to ensure that the HFXO is properly initialized and ready
 * before calling this function to avoid unexpected behavior.
 *
 * @param ctune The CTUNE value to be set for the HFXO. It must be a valid
 * 32-bit unsigned integer representing the desired tuning
 * capacitance. The caller is responsible for ensuring the value is
 * within the acceptable range for the hardware.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while other status
 * codes indicate specific errors.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_hfxo_calibrate_ctune(uint32_t ctune);

/***************************************************************************//**
 * @brief This function is used to set the calibration value for the Low-
 * Frequency Crystal Oscillator (LFXO). It should be called when there is
 * a need to adjust the LFXO frequency tuning to achieve the desired
 * oscillator performance. The function operates atomically to ensure
 * that the calibration setting is applied without interruption. It is
 * important to provide a valid calibration value to avoid incorrect
 * oscillator behavior.
 *
 * @param val The calibration value to set for the LFXO. It must be a valid
 * 32-bit unsigned integer representing the desired tuning setting.
 * The function does not validate the range of this value, so it is
 * the caller's responsibility to ensure it is appropriate for the
 * hardware configuration.
 * @return Returns SL_STATUS_OK to indicate successful setting of the
 * calibration value.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_lfxo_calibration(uint32_t val);

/***************************************************************************//**
 * @brief This function is used to obtain the current calibration value for the
 * Low-Frequency Crystal Oscillator (LFXO). It is typically called when
 * there is a need to read the current tuning setting of the LFXO, which
 * may be necessary for diagnostics or configuration purposes. The
 * function requires a valid pointer to a uint32_t variable where the
 * calibration value will be stored. It is important to ensure that the
 * pointer is not null before calling this function to avoid undefined
 * behavior.
 *
 * @param val A pointer to a uint32_t variable where the LFXO calibration value
 * will be stored. Must not be null. The caller retains ownership of
 * the memory pointed to by this parameter.
 * @return Returns an sl_status_t value indicating the success of the operation.
 * SL_STATUS_OK is returned if the operation is successful.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_lfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * @brief This function sets up the calibration parameters for the RC
 * oscillator, allowing the user to specify the number of cycles and the
 * clock sources for both the down and up counters. It also allows
 * enabling or disabling continuous calibration. This function should be
 * used when precise calibration of the RC oscillator is required, and it
 * must be called before starting the calibration process. The function
 * returns a status code indicating success or the type of error
 * encountered, such as unsupported or unavailable selections.
 *
 * @param cycles Specifies the number of cycles for the calibration process.
 * Must be a valid non-zero integer.
 * @param down_counter_selection Specifies the clock source for the down
 * counter. Must be a valid value from the
 * sl_clock_manager_clock_calibration_t
 * enumeration. If an unsupported value is
 * provided, the function returns
 * SL_STATUS_NOT_AVAILABLE.
 * @param up_counter_selection Specifies the clock source for the up counter.
 * Must be a valid value from the
 * sl_clock_manager_clock_calibration_t enumeration.
 * If an unsupported value is provided, the function
 * returns SL_STATUS_NOT_AVAILABLE, and if HCLK is
 * selected, it returns SL_STATUS_NOT_SUPPORTED.
 * @param continuous_calibration A boolean value indicating whether continuous
 * calibration should be enabled (true) or
 * disabled (false).
 * @return Returns sl_status_t indicating success (SL_STATUS_OK) or an error
 * code such as SL_STATUS_NOT_AVAILABLE or SL_STATUS_NOT_SUPPORTED.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_configure_rco_calibration(uint32_t cycles,
                                                            sl_clock_manager_clock_calibration_t down_counter_selection,
                                                            sl_clock_manager_clock_calibration_t up_counter_selection,
                                                            bool continuous_calibration);

/***************************************************************************//**
 * @brief This function is used to start the calibration process for the RC
 * oscillator (RCO) within the clock management system. It should be
 * called when the system requires precise calibration of the RCO to
 * ensure accurate clock frequencies. This function does not take any
 * parameters and does not return a value. It is typically used in
 * conjunction with other RCO calibration functions to configure,
 * monitor, and complete the calibration process. Ensure that any
 * necessary configuration has been performed prior to calling this
 * function.
 *
 * @return None
 ******************************************************************************/
void sli_clock_manager_hal_start_rco_calibration (void);

/***************************************************************************//**
 * @brief Use this function to halt the ongoing RC oscillator (RCO) calibration
 * process. It is typically called when the calibration is no longer
 * needed or if the process needs to be interrupted for any reason. This
 * function should be used in conjunction with other RCO calibration
 * functions to manage the calibration lifecycle effectively. Ensure that
 * the calibration process is active before calling this function to
 * avoid unnecessary operations.
 *
 * @return None
 ******************************************************************************/
void sli_clock_manager_hal_stop_rco_calibration(void);

/***************************************************************************//**
 * @brief This function is used to wait for the completion of the RC oscillator
 * (RCO) calibration process. It should be called when a calibration is
 * initiated and the system needs to ensure that the calibration has
 * finished before proceeding. However, if the RCO is set to continuous
 * calibration mode, this function will return immediately without
 * waiting. This function is typically used in scenarios where precise
 * timing and calibration are critical, and it must be ensured that the
 * calibration process is complete before any dependent operations are
 * performed.
 *
 * @return None
 ******************************************************************************/
void sli_clock_manager_hal_wait_rco_calibration(void);

/***************************************************************************//**
 * @brief This function is used to obtain the current calibration count of the
 * RC oscillator (RCO). It is typically called after an RCO calibration
 * process to determine the number of calibration cycles that have been
 * completed. The function requires a valid pointer to a uint32_t
 * variable where the calibration count will be stored. It is important
 * to ensure that the pointer is not null before calling this function to
 * avoid undefined behavior.
 *
 * @param count A pointer to a uint32_t variable where the calibration count
 * will be stored. Must not be null. The caller retains ownership
 * of the memory.
 * @return Returns SL_STATUS_OK to indicate successful retrieval of the
 * calibration count.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_rco_calibration_count(uint32_t *count);

/***************************************************************************//**
 * Sets SYSCLK clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function configures the system clock (SYSCLK) to use the
 * specified oscillator as its source. It should be called when there is
 * a need to change the clock source for the system, such as during
 * initialization or when switching to a different power mode. The
 * function operates atomically to ensure thread safety. It is important
 * to pass a valid oscillator type; otherwise, the function will return
 * an error status. The function does not perform any validation on the
 * current state of the system clock, so it should be used with caution
 * to avoid unintended system behavior.
 *
 * @param source Specifies the oscillator to be used as the system clock source.
 * Valid values are defined by the `sl_oscillator_t` enumeration,
 * such as `SL_OSCILLATOR_FSRCO`, `SL_OSCILLATOR_HFXO`,
 * `SL_OSCILLATOR_HFRCODPLL`, `SL_OSCILLATOR_RFFPLL`, and
 * `SL_OSCILLATOR_CLKIN0`. Passing an invalid value will result in
 * the function returning `SL_STATUS_INVALID_PARAMETER`.
 * @return Returns `SL_STATUS_OK` if the clock source is set successfully, or
 * `SL_STATUS_INVALID_PARAMETER` if the provided oscillator type is not
 * valid.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_sysclk_source(sl_oscillator_t source);

/***************************************************************************//**
 * Gets SYSCLK clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to determine the current oscillator source for the
 * system clock (SYSCLK). It is essential for applications that need to
 * adapt their behavior based on the clock source. This function must be
 * called with a valid pointer to an `sl_oscillator_t` variable where the
 * result will be stored. The function assumes that the system clock
 * source is correctly configured and will return the corresponding
 * oscillator type. It is important to ensure that the pointer provided
 * is not null to avoid undefined behavior.
 *
 * @param source A pointer to an `sl_oscillator_t` variable where the current
 * SYSCLK source will be stored. Must not be null. The caller
 * retains ownership of the memory pointed to by this parameter.
 * @return Returns `SL_STATUS_OK` to indicate successful retrieval of the SYSCLK
 * source.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_sysclk_source(sl_oscillator_t *source);

/***************************************************************************//**
 * @brief This function is used to ensure that the USB Phase-Locked Loop (PLL)
 * is both ready and locked before proceeding with operations that depend
 * on it. It should be called when the USB PLL is required to be stable
 * and operational. The function will block until the USB PLL achieves
 * the ready and locked state. If the USB PLL is not present in the
 * hardware configuration, the function will return immediately with a
 * status indicating that the feature is not available. This function is
 * typically used in initialization routines or before enabling USB-
 * related functionalities.
 *
 * @return Returns SL_STATUS_OK if the USB PLL is ready and locked, or
 * SL_STATUS_NOT_AVAILABLE if the USB PLL is not present.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_wait_usbpll(void);

/***************************************************************************//**
 * Sets the external FLASH reference clock.
 *
 * @note This API is not thread-safe and should therefore not be called
 *       across multiple tasks.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to set the reference clock for external FLASH
 * memory. It is important to note that this API is not thread-safe,
 * meaning it should not be called concurrently from multiple tasks. The
 * function currently does not perform any operation and always returns a
 * status indicating that the feature is not available. This implies that
 * the functionality may be intended for future implementation or is
 * conditionally compiled out.
 *
 * @param oscillator Specifies the oscillator to be used as the reference clock
 * for the external FLASH. The parameter is accepted but not
 * used in the current implementation. The function does not
 * validate this parameter, and it has no effect on the
 * function's behavior.
 * @return Returns SL_STATUS_NOT_AVAILABLE, indicating that the feature is not
 * currently available.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_set_ext_flash_clk(sl_oscillator_t oscillator);

/***************************************************************************//**
 * Gets the external FLASH clock source.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the current oscillator source for the
 * external FLASH clock. It requires a valid pointer to an
 * `sl_oscillator_t` variable where the oscillator information will be
 * stored. If the provided pointer is null, the function returns an error
 * status indicating a null pointer. This function is not thread-safe and
 * should be used with caution in multi-threaded environments. It is
 * important to note that the function currently returns a status
 * indicating that the feature is not available.
 *
 * @param oscillator A pointer to an `sl_oscillator_t` variable where the
 * external FLASH clock source will be stored. Must not be
 * null. If null, the function returns
 * `SL_STATUS_NULL_POINTER`.
 * @return Returns `SL_STATUS_NOT_AVAILABLE` indicating the feature is not
 * currently available, or `SL_STATUS_NULL_POINTER` if the input pointer
 * is null.
 ******************************************************************************/
sl_status_t sli_clock_manager_hal_get_ext_flash_clk(sl_oscillator_t *oscillator);

#ifdef __cplusplus
}
#endif

#endif // SL_CLOCK_MANAGER_HAL_H
