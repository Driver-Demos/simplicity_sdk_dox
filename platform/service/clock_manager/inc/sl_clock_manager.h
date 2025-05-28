/***************************************************************************//**
 * @file
 * @brief Clock Manager APIs.
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

#ifndef SL_CLOCK_MANAGER_H
#define SL_CLOCK_MANAGER_H

#include <stdbool.h>
#include <stdlib.h>
#include "sl_status.h"
#include "sl_enum.h"
#include "sl_device_clock.h"
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup clock_manager Clock Manager
 *
 * @details
 * ## Overview
 *
 *  Clock Manager is a platform-level software module that manages
 *  the device's oscillators and clock tree.
 *  The Clock Manager module is split into two main parts: The Initialization part
 *  and the Runtime part. The runtime part has its component
 *  \a clock_manager_runtime and can be used independently from the initialization
 *  part. The \a clock_manager component includes both the initialization part and the
 *  runtime part and it should be the component added to your project slcp file.
 *
 *  ## Initialization
 *  The initialization part includes the configuration files
 *  \a sl_clock_manager_oscillator_config.h and \a sl_clock_manager_tree_config.h.
 *  As their name indicates, those C header files are used to configure the different
 *  device oscillators and the device clock tree. Those header files use the CMSIS
 *  Configuration Wizard Annotations and are specific to each device.
 *  The API function sl_clock_manager_init() is used to initialize the Clock Manager
 *  module based on the configuration values specified in the two configuration files.
 *  This function must be called early during your initialization sequence.
 *  If the SL System component (@ref system) is used by your application, the
 *  sl_clock_manager_init() call will be added automatically to your initialization
 *  sequence.
 *
 *  ### Oscillators Configuration
 *  Oscillators' configurations are all grouped in the \a sl_clock_manager_oscillator_config.h
 *  file. Crystal-based oscillators, HFXO and LFXO, have an enable/disable configuration to
 *  indicate if the required crystal is present or not. In the absence of the required
 *  crystal, the configuration must be disabled and the associated oscillator will
 *  not be initialized.
 *
 *  The HFXO configuration also provides the configuration for the Crystal Sharing
 *  feature when supported by the device. This feature allows to use the dedicated
 *  HFCLKOUT pin to output a sinusoidal clock that can be used as the HFXO
 *  input for another EFR device. In the configuration, you need to specify if your
 *  device is the leader or the follower. The leader will be the one outputting the
 *  clock and the follower, the one receiving the clock signal. In the leader configuration,
 *  the GPIO pin is used to receive the request from the follower. You can refer to your
 *  device datasheet to know the available location for the HFXO BUFOUT_REQ pin.
 *  In the follower mode, the pin configuration can be used to send an HFXO request
 *  signal to the leader. The "High Frequency Clock Ouput" section of your device
 *  reference manual also provides more details about this feature.
 *
 *  The first HFRCO module, whose output clock is called HFRCODPLL, can be connected to
 *  the DPLL module to have a better precision clock. When the DPLL is enabled through
 *  the configuration define \a SL_CLOCK_MANAGER_HFRCO_DPLL_EN, the DPLL settings
 *  take precedence over the HFRCO band configuration.
 *
 *  ### Clock Tree Configuration
 *  The device clock tree configurations are all grouped in the
 *  \a sl_clock_manager_tree_config.h file. Refer to your device's reference manual for
 *  the clock tree diagram and see which peripherals are connected to which clock branches.
 *  In the configuration file, each clock branch can be independently configured.
 *  However, to facilitate the clock setup for users, two additional configurations
 *  were added: \a SL_CLOCK_MANAGER_DEFAULT_HF_CLOCK_SOURCE and
 *  \a SL_CLOCK_MANAGER_DEFAULT_LF_CLOCK_SOURCE. Those configurations allow the selection
 *  of the default high-frequency and low-frequency oscillators to be used inside the clock
 *  tree. Every clock branch that can benefit from those default selections will use them
 *  by default.
 *  On certain devices, the @ref power_manager module offers an Execution Modes feature
 *  with the \a SL_POWER_MANAGER_EXECUTION_MODES_FEATURE_EN configuration. When this feature
 *  is enabled, the \a SL_CLOCK_MANAGER_SYSCLK_SOURCE configuration could be overriden by the
 *  Execution Modes feature. Refer to the description of \a SL_CLOCK_MANAGER_SYSCLK_SOURCE in
 *  \a sl_clock_manager_tree_config.h file to know if this is the case.
 *
 *  Some peripherals have an internal clock divider. Those are not handled by the
 *  Clock Manager configuration. The peripheral driver will usually expose the divider
 *  configuration when such a divider is present inside the peripheral module.
 *
 *  The Clock tree configuration is available at compile-time only. The Clock Manager
 *  module does not offer API functions to manipulate the clock tree at runtime.
 *
 *  The more oscillators are used by different clock branches the more power
 *  consumption you will have. To limit your power consumption, you can try to limit the
 *  number of oscillators used. So for example, only use one High-frequency oscillator
 *  and one Low-frequency oscillator across the clock tree. However, if the
 *  application is radio-based, the HFXO oscillator is mandatory for the Radio
 *  clock branch and if the Radio clock branch is connected to the SYSCLK branch,
 *  this will limit you to use HFXO for SYSCLK as well. In this specific case, SYSCLK
 *  could also be configured to use HFRCO with DPLL, but the chosen frequency must be two
 *  times the HFXO frequency so that the Radio module can retrieve the HF crystal frequency
 *  with its divider. This will also come with an increase in power consumption since both
 *  HFXO and HFRCO oscillators will be used. Refer to your device reference manual to know
 *  if your Radio clock is connected to the SYSCLK clock branch or not.
 *
 *  @note The Clock Manager Initialization is incompatible with the \a device_init_clocks
 *        (@ref device_init_clocks), \a device_init_hfxo (@ref device_init_hfxo),
 *        \a device_init_hfrco (@ref device_init_hfrco) \a device_init_dpll (@ref device_init_dpll),
 *        \a device_init_rffpll (@ref device_init_rffpll), \a device_init_usbpll (@ref device_init_usbpll),
 *        \a device_init_lfxo (@ref device_init_lfxo) and \a device_init_lfrco (@ref device_init_lfrco)
 *        components.
 *        This does not mean that the \a device_init component (@ref device_init) is incompatible with the
 *        \a clock_manager component. The \a device_init component can pull other initialization
 *        modules like EMU and DCDC that are not related to clocks. Therefore, both
 *        \a device_init and \a clock_manager should be present in your project file. SLC will
 *        take care of pulling only the sub \a device_init_xxx components that are needed.
 *
 *  The runtime part, which is associated with the \a clock_manager_runtime component,
 *  has also an initialization function of its own, sl_clock_manager_runtime_init().
 *  This function must also be part of the initialization sequence. If the SL System
 *  component (@ref system) is used by your application, the
 *  sl_clock_manager_runtime_init() call will be added automatically to your
 *  initialization sequence.
 *
 *  ## Functionalities
 *  The Runtime part includes functionalities related to oscillators, clock tree
 *  and the CMU hardware module features. The main functionalities are:
 *    - Retrieving the frequency or precision of an oscillator or clock branch
 *    - Enabling/Disabling modules' bus clock
 *    - Retrieving or setting calibration values for oscillators
 *    - Exporting clocks to GPIO
 *    - Starting an RCO Calibration process based on a reference clock source
 *
 *  ### Retrieve the frequency or precision of an oscillator or clock branch
 *  API functions sl_clock_manager_get_oscillator_frequency() and
 *  sl_clock_manager_get_oscillator_precision() allow retrieving respectively
 *  the frequency and precision of a given oscillator. Similar functions
 *  exist for clock branches: sl_clock_manager_get_clock_branch_frequency() and
 *  sl_clock_manager_get_clock_branch_precision().
 *
 *  To retrieve the frequency or precision of a specific peripheral, you will
 *  first need to retrieve to which clock branch this peripheral is connected.
 *  To do so, the Device Manager and its @ref device_peripheral
 *  can be used. The below code example shows how to retrieve the clock branch
 *  of the TIMER0 peripheral.
 *
 *  @code{.c}
 *  #include "sl_clock_manager.h"
 *  #include "sl_device_peripheral.h"
 *
 *  sl_status_t status;
 *  uint32_t freq;
 *  sl_clock_branch_t clock_branch;
 *
 *  clock_branch = sl_device_peripheral_get_clock_branch(SL_PERIPHERAL_TIMER0);
 *  status = sl_clock_manager_get_clock_branch_frequency(clock_branch, &freq);
 *  @endcode
 *
 * ### Enable/Disable modules' bus clock
 * Before accessing a peripheral's register interface, its bus clock must be enabled,
 * or else a bus fault exception will be triggered. API functions
 * sl_clock_manager_enable_bus_clock() and sl_clock_manager_disable_bus_clock()
 * allow to perform such operations.
 *
 * Note that the peripheral clock will automatically be enabled when a peripheral
 * is enabled with the clock on-demand feature.
 *
 * ### Oscillator Calibration
 * The Clock Manager initialization, if present, will calibrate the different
 * oscillators during the initialization sequence, but sometimes calibration
 * values must be updated during runtime in certain conditions, for example, if
 * the device temperature changes too much. This is considered an advanced
 * functionality and users must be careful as to when to use this functionality.
 *
 * API functions sl_clock_manager_set_rc_oscillator_calibration() and
 * sl_clock_manager_get_rc_oscillator_calibration() allow to set or get the
 * CAL register of HFRCO and LFRCO oscillators. Not all devices have an LFRCO
 * module with a CAL register. Some LFRCO modules will have a high-precision
 * configuration allowing to use the HFXO to auto-calibrate the LFRCO. Refer
 * to your device reference manual to retrieve oscillator specifications.
 *
 * API functions sl_clock_manager_set_hfxo_calibration() and
 * sl_clock_manager_get_hfxo_calibration() allow to set or get the \a COREBIASANA
 * inside the HFXO \a XTALCTRL register. The HFXO module has a Core Bias Optimization
 * stage at the end of the oscillator startup sequence that allows to further
 * optimize current consumption. This optimization will automatically set the
 * \a COREBIASANA bitfield when finished. Upon reset, this optimization will run
 * the first time HFXO is started and afterwards, the \a XTALCTRL->SKIPCOREBIASOPT
 * bit will automatically be set so that next time HFXO is started during the
 * application lifetime, the optimization stage will be skipped. This optimization
 * stage takes a while to run, in the order of hundreds of milliseconds, therefore
 * we don't want it to run each time HFXO is started.
 * With the function sl_clock_manager_set_hfxo_calibration() it is possible to
 * manually set the \a COREBIASANA bitfield and set the \a SKIPCOREBIASOPT bit.
 * This function will usually be used in the context of an EM4 wake-up where to
 * save on the initialization sequence time, we want to skip the Core Bias Optimization
 * stage and manually set the value that would have previously been retrieved with
 * sl_clock_manager_get_hfxo_calibration() and saved in an EM4 retained memory.
 * In this context, sl_clock_manager_set_hfxo_calibration() will need to be called
 * early in the initialization sequence, before the usual clock initialization
 * function.
 *
 * slx_clock_manager_hfxo_set_ctune(), slx_clock_manager_hfxo_get_ctune() and
 * slx_clock_manager_hfxo_calibrate_ctune() functions allow to manipulate the
 * HFXO tuning capacitances. Changing the CTUNE value while HFXO is running
 * can result in significant clock glitches for one clock period. Therefore,
 * those functions should be used with caution. The difference between the
 * slx_clock_manager_hfxo_set_ctune() and slx_clock_manager_hfxo_calibrate_ctune()
 * functions is that the calibration one will also start and wait for the HFXO
 * Core Bias Optimization stage to complete.
 *
 * API functions sl_clock_manager_set_lfxo_calibration() and
 * sl_clock_manager_get_lfxo_calibration() allow to set and get the LFXO CTUNE
 * value.
 *
 * ### Export clocks to GPIO
 * The CMU module offers the functionality to export a given clock source to a
 * GPIO pin. Refer to function sl_clock_manager_set_gpio_clock_output() for more
 * details and the #sl_clock_manager_export_clock_source_t enum for a list of
 * acceptable clock sources. Note that there is a specific clock branch named
 * EXPCLK that is usually connected to the SYSCLK and offers an additional divider.
 *
 * ### RCO Calibration
 * The CMU module also offers RCO Calibration hardware support. This can be
 * used to calibrate at runtime HFRCO and LFRCO oscillators using a high-precision
 * reference clock. Refer to your device reference manual for more
 * details about this functionality. API function
 * sl_clock_manager_configure_rco_calibration() can be used to configure the
 * calibration process. Then sl_clock_manager_start_rco_calibration() and
 * sl_clock_manager_stop_rco_calibration() can be called to start/stop the
 * process. sl_clock_manager_wait_rco_calibration() function can be called to
 * actively wait for the process to finish. And finally,
 * sl_clock_manager_get_rco_calibration_count() can be called to retrieve the
 * calibration process result.
 *
 * @{
 ******************************************************************************/

/// Export clock source.
/// This is to be used with the sl_clock_manager_set_gpio_clock_output() API function.
SL_ENUM(sl_clock_manager_export_clock_source_t) {
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_DISABLED,   ///< Export Clock Source Disabled
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_FSRCO,      ///< Export Clock Source FSRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFXO,       ///< Export Clock Source HFXO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFRCODPLL,  ///< Export Clock Source HFRCODPLL
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFRCOEM23,  ///< Export Clock Source HFRCOEM23
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HFEXPCLK,   ///< Export Clock Source HFEXPCLK
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_LFXO,       ///< Export Clock Source LFXO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_PLFRCO,     ///< Export Clock Source PLFRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_LFRCO,      ///< Export Clock Source LFRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_ULFRCO,     ///< Export Clock Source ULFRCO
  SL_CLOCK_MANAGER_EXPORT_CLOCK_SOURCE_HCLK,       ///< Export Clock Source HCLK
};

/// Export clock output selection.
/// This is to be used with the sl_clock_manager_set_gpio_clock_output() API function.
SL_ENUM(sl_clock_manager_export_clock_output_select_t) {
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_0 = 0,  ///< Export Clock Output #0
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_1,      ///< Export Clock Output #1
  SL_CLOCK_MANAGER_EXPORT_CLOCK_OUTPUT_SELECT_2,      ///< Export Clock Output #2
};

/// Clocks available for Calibration.
/// This is to be used with the sl_clock_manager_configure_rco_calibration() API function.
SL_ENUM(sl_clock_manager_clock_calibration_t) {
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HCLK,       ///< Clock Calibration HCLK
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_PRS,        ///< Clock Calibration PRS
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFXO,       ///< Clock Calibration HFXO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_LFXO,       ///< Clock Calibration LFXO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFRCODPLL,  ///< Clock Calibration HFRCODPLL
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_HFRCOEM23,  ///< Clock Calibration HFRCOEM23
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_FSRCO,      ///< Clock Calibration FSRCO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_LFRCO,      ///< Clock Calibration LFRCO
  SL_CLOCK_MANAGER_CLOCK_CALIBRATION_ULFRCO      ///< Clock Calibration ULFRCO
};

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function is used to initialize the runtime components of the
 * Clock Manager, which is essential for managing the device's
 * oscillators and clock tree during operation. It should be called as
 * part of the system's initialization sequence to ensure that the
 * runtime functionalities of the Clock Manager are properly set up. If
 * the SL System component is used, this function will be automatically
 * included in the initialization sequence. The function returns a status
 * code indicating success or failure, which should be checked to ensure
 * that the initialization was successful.
 *
 * @return Returns a status code of type sl_status_t, indicating SL_STATUS_OK if
 * the initialization is successful, or an error code otherwise.
 ******************************************************************************/
sl_status_t sl_clock_manager_runtime_init(void);

/***************************************************************************//**
 * Gets frequency of given oscillator.
 *
 * @param[in] oscillator  Oscillator
 *
 * @param[out] frequency  Oscillator's frequency in Hertz
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the frequency of a given oscillator in
 * Hertz. It is essential to ensure that the `frequency` parameter is not
 * null before calling this function, as a null pointer will result in an
 * error. This function is typically used in scenarios where precise
 * frequency information of an oscillator is required for further
 * processing or configuration. It is important to handle the return
 * status to check for successful execution or to identify any errors
 * that may occur.
 *
 * @param oscillator Specifies the oscillator whose frequency is to be
 * retrieved. It must be a valid oscillator identifier as
 * defined in the system.
 * @param frequency A pointer to a uint32_t where the oscillator's frequency
 * will be stored. Must not be null, as passing a null pointer
 * will result in an SL_STATUS_NULL_POINTER error.
 * @return Returns an sl_status_t code indicating success (SL_STATUS_OK) or an
 * error code if the operation fails.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_oscillator_frequency(sl_oscillator_t oscillator,
                                                      uint32_t *frequency);

/***************************************************************************//**
 * @brief This function is used to obtain the precision of a given oscillator in
 * parts per million (PPM). It is essential to ensure that the
 * `precision` parameter is not null before calling this function, as a
 * null pointer will result in an error. This function is typically used
 * in scenarios where precise oscillator characteristics are required for
 * system operations or diagnostics. It is important to handle the return
 * status to check for successful execution or to identify any errors.
 *
 * @param oscillator Specifies the oscillator for which precision is to be
 * retrieved. It must be a valid oscillator identifier as
 * defined in the system.
 * @param precision A pointer to a uint16_t where the precision of the
 * oscillator will be stored. Must not be null, as passing a
 * null pointer will result in an SL_STATUS_NULL_POINTER error.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while
 * SL_STATUS_NULL_POINTER is returned if the precision pointer is null.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_oscillator_precision(sl_oscillator_t oscillator,
                                                      uint16_t *precision);

/***************************************************************************//**
 * Gets frequency of given clock branch.
 *
 * @param[in] clock_branch  Clock Branch
 *
 * @param[out] frequency    Clock Branch's frequency in Hertz
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the frequency of a specified clock
 * branch in Hertz. It is essential for applications that need to know
 * the operating frequency of different clock branches for configuration
 * or monitoring purposes. The function requires a valid clock branch
 * identifier and a non-null pointer to store the frequency result. If
 * the frequency pointer is null, the function returns an error status
 * indicating a null pointer.
 *
 * @param clock_branch Specifies the clock branch whose frequency is to be
 * retrieved. It must be a valid clock branch identifier as
 * defined in the system.
 * @param frequency A pointer to a uint32_t where the frequency of the specified
 * clock branch will be stored. Must not be null. If null, the
 * function returns SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t indicating success or the type of error
 * encountered, such as SL_STATUS_NULL_POINTER if the frequency pointer
 * is null.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_clock_branch_frequency(sl_clock_branch_t clock_branch,
                                                        uint32_t *frequency);

/***************************************************************************//**
 * @brief This function is used to obtain the precision, in parts per million
 * (PPM), of a specified clock branch. It is essential to ensure that the
 * `precision` pointer is not null before calling this function, as a
 * null pointer will result in an error. This function is typically used
 * in scenarios where precise clock management is required, such as in
 * timing-sensitive applications. It is important to handle the return
 * status to ensure that the operation was successful.
 *
 * @param clock_branch Specifies the clock branch whose precision is to be
 * retrieved. It must be a valid clock branch identifier.
 * @param precision A pointer to a uint16_t where the precision of the clock
 * branch will be stored. Must not be null, as passing a null
 * pointer will result in an SL_STATUS_NULL_POINTER error.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while
 * SL_STATUS_NULL_POINTER is returned if the precision pointer is null.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_clock_branch_precision(sl_clock_branch_t clock_branch,
                                                        uint16_t *precision);

/***************************************************************************//**
 * @brief This function is used to enable the bus clock for a specific module,
 * which is necessary before accessing the module's register interface to
 * avoid triggering a bus fault exception. It should be called whenever a
 * module's bus clock needs to be activated, typically during the
 * initialization or configuration phase of a peripheral. The function
 * returns a status code indicating success or failure, allowing the
 * caller to handle any errors appropriately.
 *
 * @param module_bus_clock Specifies the bus clock of the module to be enabled.
 * It must be a valid value defined in the device's Bus
 * Clock Defines section. Invalid values may result in
 * an error status being returned.
 * @return Returns a status code of type sl_status_t, which is SL_STATUS_OK if
 * the operation is successful, or an error code if it fails.
 ******************************************************************************/
sl_status_t sl_clock_manager_enable_bus_clock(sl_bus_clock_t module_bus_clock);

/***************************************************************************//**
 * @brief Use this function to disable the bus clock for a specific module when
 * it is not needed, which can help reduce power consumption. This
 * function should be called when the module is not in use to prevent
 * unnecessary power usage. Ensure that the module's bus clock is not
 * required for any ongoing operations before disabling it, as this may
 * lead to a bus fault exception if the module is accessed without an
 * enabled clock.
 *
 * @param module_bus_clock Specifies the bus clock of the module to be disabled.
 * It must be a valid sl_bus_clock_t value as defined in
 * the Bus Clock Defines section of the device_clock
 * header. Invalid values may result in an error status.
 * @return Returns an sl_status_t code indicating success (SL_STATUS_OK) or an
 * error code if the operation fails.
 ******************************************************************************/
sl_status_t sl_clock_manager_disable_bus_clock(sl_bus_clock_t module_bus_clock);

/***************************************************************************//**
 * @brief This function is used to export a clock source to a specified GPIO
 * pin, allowing external devices to utilize the clock signal. It
 * requires specifying the clock source, the output channel, and the GPIO
 * port and pin where the clock will be output. The function also allows
 * setting a divider for the HFEXP clock source, which affects only the
 * EXPCLK branch frequency. It is important to ensure that the GPIO port
 * and pin are correctly configured and available for clock output. The
 * function returns a status code indicating success or the type of error
 * encountered.
 *
 * @param export_clock_source Specifies the clock source to be exported. Must be
 * a valid value from the
 * sl_clock_manager_export_clock_source_t
 * enumeration.
 * @param output_select Specifies the output channel for the clock. Must be a
 * valid value from the
 * sl_clock_manager_export_clock_output_select_t
 * enumeration.
 * @param hfexp_divider Specifies the divider for the HFEXP clock source. Valid
 * range is 1 to 32. This parameter only affects the EXPCLK
 * branch frequency.
 * @param port Specifies the GPIO port number where the clock will be output.
 * Must be a valid GPIO port number.
 * @param pin Specifies the GPIO pin number where the clock will be output. Must
 * be a valid GPIO pin number.
 * @return Returns an sl_status_t status code indicating success or the type of
 * error encountered.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_gpio_clock_output(sl_clock_manager_export_clock_source_t export_clock_source,
                                                   sl_clock_manager_export_clock_output_select_t output_select,
                                                   uint16_t hfexp_divider,
                                                   uint32_t port,
                                                   uint32_t pin);

/***************************************************************************//**
 * @brief Use this function to adjust the frequency tuning of a specified RC
 * oscillator during runtime. This is an advanced operation typically
 * used when environmental conditions, such as temperature, change
 * significantly and affect oscillator performance. The function should
 * be used with caution, as it modifies the calibration set during
 * production. Ensure that the Clock Manager module is initialized before
 * calling this function.
 *
 * @param oscillator Specifies the RC oscillator to calibrate. Must be a valid
 * sl_oscillator_t value representing an RC oscillator, such
 * as SL_OSCILLATOR_HFRCODPLL, SL_OSCILLATOR_HFRCOEM23, or
 * SL_OSCILLATOR_LFRCO. Invalid values may result in undefined
 * behavior.
 * @param val The calibration value to set for the oscillator. This is a 32-bit
 * unsigned integer representing the desired tuning setting. The
 * value should be chosen based on the specific requirements and
 * characteristics of the oscillator.
 * @return Returns an sl_status_t code indicating success or failure of the
 * operation. SL_STATUS_OK is returned if the operation is successful;
 * otherwise, an error code is returned.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                           uint32_t val);

/***************************************************************************//**
 * @brief This function is used to obtain the current frequency tuning setting
 * of a specified RC oscillator. It is essential for applications that
 * need to monitor or adjust the oscillator's calibration during runtime.
 * The function requires a valid oscillator identifier and a non-null
 * pointer to store the retrieved calibration value. If the pointer is
 * null, the function returns an error status. This function is
 * particularly useful in scenarios where precise oscillator calibration
 * is necessary, such as in temperature-sensitive applications.
 *
 * @param oscillator An identifier for the RC oscillator whose calibration
 * setting is to be retrieved. Must be a valid RC oscillator
 * type supported by the system.
 * @param val A pointer to a uint32_t variable where the current calibration
 * setting will be stored. Must not be null, or the function will
 * return SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t code indicating success or the type of error
 * encountered, such as SL_STATUS_NULL_POINTER if the output pointer is
 * null.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_rc_oscillator_calibration(sl_oscillator_t oscillator,
                                                           uint32_t *val);

/***************************************************************************//**
 * @brief Use this function to manually set the calibration value for the High-
 * Frequency Crystal Oscillator (HFXO). This is typically necessary in
 * advanced scenarios, such as after an EM4 wake-up, to skip the Core
 * Bias Optimization stage and reduce initialization time. It should be
 * called early in the initialization sequence, before the usual clock
 * initialization function.
 *
 * @param val The calibration setting to be applied to the HFXO. The function
 * expects a valid calibration value, and improper values may lead to
 * undefined behavior.
 * @return Returns a status code indicating success or failure of the operation.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_hfxo_calibration(uint32_t val);

/***************************************************************************//**
 * @brief This function is used to obtain the current calibration value of the
 * High-Frequency Crystal Oscillator (HFXO). It is typically called when
 * there is a need to save the calibration state, such as before a low-
 * power mode transition, to restore it later. The function must be
 * called with a valid pointer to store the retrieved calibration value.
 * If the provided pointer is null, the function will return an error
 * status indicating a null pointer.
 *
 * @param val A pointer to a uint32_t where the HFXO calibration value will be
 * stored. Must not be null. If null, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t indicating success or the type of error
 * encountered, such as SL_STATUS_NULL_POINTER if the input pointer is
 * null.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_hfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * @brief This function is used to set the CTUNE value for the High-Frequency
 * Crystal Oscillator (HFXO), which adjusts the tuning capacitance. It
 * should be used with caution as changing the CTUNE value while the HFXO
 * is running can cause significant clock glitches for one clock period.
 * This function is typically used in scenarios where precise tuning of
 * the HFXO is required, such as during calibration or when environmental
 * conditions change. It is important to ensure that the HFXO is not in
 * use or that the system can tolerate potential glitches when calling
 * this function.
 *
 * @param ctune The CTUNE value to set for the HFXO. It must be a valid tuning
 * capacitance value as per the hardware specifications. Invalid
 * values may result in undefined behavior.
 * @return Returns an sl_status_t status code indicating success or failure of
 * the operation.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_set_ctune(uint32_t ctune);

/***************************************************************************//**
 * @brief This function is used to obtain the current tuning capacitance (CTUNE)
 * value for the High-Frequency Crystal Oscillator (HFXO). It is
 * essential for applications that need to adjust or verify the
 * oscillator's tuning settings. The function requires a valid pointer to
 * store the retrieved CTUNE value. It must be called with a non-null
 * pointer, as passing a null pointer will result in an error status.
 * This function is typically used in scenarios where precise oscillator
 * tuning is necessary for optimal performance.
 *
 * @param ctune A pointer to a uint32_t where the CTUNE value will be stored.
 * Must not be null. If null, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t indicating success or failure. SL_STATUS_OK if
 * successful, or an error code if the input is invalid.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_get_ctune(uint32_t *ctune);

/***************************************************************************//**
 * @brief This function is used to adjust the tuning capacitances of the High-
 * Frequency Crystal Oscillator (HFXO) by setting the CTUNE value, which
 * is crucial for maintaining the oscillator's frequency accuracy. It
 * also calibrates the Core Bias Current, which can optimize the
 * oscillator's performance. This function should be used with caution as
 * it can cause clock glitches during the calibration process. It is
 * typically used in scenarios where precise frequency tuning is
 * required, such as after significant temperature changes or during
 * specific operational modes.
 *
 * @param ctune The CTUNE value for the HFXO. This parameter is used to set the
 * tuning capacitance of the oscillator. The value must be chosen
 * carefully to ensure proper oscillator operation. Invalid values
 * may lead to incorrect frequency tuning or operational
 * instability.
 * @return Returns an sl_status_t status code indicating success or failure of
 * the calibration process.
 ******************************************************************************/
sl_status_t slx_clock_manager_hfxo_calibrate_ctune(uint32_t ctune);

/***************************************************************************//**
 * @brief This function is used to set the calibration value for the Low-
 * Frequency Crystal Oscillator (LFXO). It is typically called when there
 * is a need to adjust the LFXO calibration during runtime, such as in
 * response to environmental changes like temperature fluctuations. This
 * function should be used with caution, as incorrect calibration values
 * can affect the oscillator's performance. It is expected to be called
 * after the Clock Manager has been initialized.
 *
 * @param val The calibration setting for the LFXO. It must be a valid 32-bit
 * unsigned integer representing the desired calibration value. The
 * function does not perform validation on this value, so it is the
 * caller's responsibility to ensure it is appropriate for the
 * specific hardware and application context.
 * @return Returns an sl_status_t status code indicating success or failure of
 * the operation. SL_STATUS_OK is returned if the operation is
 * successful; otherwise, an error code is returned.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_lfxo_calibration(uint32_t val);

/***************************************************************************//**
 * @brief This function is used to obtain the current calibration setting for
 * the Low-Frequency Crystal Oscillator (LFXO). It is typically called
 * when there is a need to verify or store the calibration value for
 * later use, such as during system initialization or before entering a
 * low-power state. The function requires a valid pointer to a uint32_t
 * variable where the calibration value will be stored. It is important
 * to ensure that the pointer is not null before calling this function,
 * as passing a null pointer will result in an error status being
 * returned.
 *
 * @param val A pointer to a uint32_t variable where the LFXO calibration value
 * will be stored. Must not be null. If null, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t indicating success or failure. SL_STATUS_OK is
 * returned on success, while SL_STATUS_NULL_POINTER is returned if the
 * input pointer is null.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_lfxo_calibration(uint32_t *val);

/***************************************************************************//**
 * @brief This function sets up the parameters for the RCO (Resistor-Capacitor
 * Oscillator) calibration process, which is used to calibrate
 * oscillators using a high-precision reference clock. It should be
 * called when you need to adjust the calibration settings, such as the
 * number of cycles for calibration and the selection of clocks for
 * counting. The function allows for continuous calibration if specified.
 * It is important to note that RCO calibration functions are not thread-
 * safe and should not be used across multiple tasks.
 *
 * @param cycles Specifies the number of cycles to run the calibration. A higher
 * number increases precision but also the time taken for
 * calibration. Must be a positive integer.
 * @param down_counter_selection Specifies the clock to be used for counting
 * down cycles. Must be a valid value from the
 * sl_clock_manager_clock_calibration_t
 * enumeration.
 * @param up_counter_selection Specifies the clock whose generated cycles will
 * be counted and added up. Must be a valid value
 * from the sl_clock_manager_clock_calibration_t
 * enumeration.
 * @param continuous_calibration A boolean flag indicating whether the
 * calibration should be continuous. If true,
 * continuous calibration is configured.
 * @return Returns an sl_status_t status code indicating success or the type of
 * error encountered.
 ******************************************************************************/
sl_status_t sl_clock_manager_configure_rco_calibration(uint32_t cycles,
                                                       sl_clock_manager_clock_calibration_t down_counter_selection,
                                                       sl_clock_manager_clock_calibration_t up_counter_selection,
                                                       bool continuous_calibration);

/***************************************************************************//**
 * @brief This function is used to start the RCO (Resistor-Capacitor Oscillator)
 * calibration process, which is part of the Clock Manager's runtime
 * functionalities. It should be called when you need to begin
 * calibrating the RCO using a high-precision reference clock. This
 * function is not thread-safe, so it should not be invoked from multiple
 * tasks concurrently. Ensure that the RCO calibration has been properly
 * configured before calling this function.
 *
 * @return None
 ******************************************************************************/
void sl_clock_manager_start_rco_calibration(void);

/***************************************************************************//**
 * @brief Use this function to halt the ongoing RCO calibration process when it
 * is no longer needed or if you need to reconfigure the calibration
 * settings. This function should be called in a single-threaded context
 * or with appropriate synchronization, as RCO calibration functions are
 * not thread-safe. Ensure that the calibration process is actively
 * running before calling this function to avoid unnecessary calls.
 *
 * @return None
 ******************************************************************************/
void sl_clock_manager_stop_rco_calibration(void);

/***************************************************************************//**
 * @brief This function is used to block the calling thread until the RCO
 * (Resistor-Capacitor Oscillator) calibration process is finished. It
 * should be called after initiating the RCO calibration to ensure that
 * the calibration has completed before proceeding with operations that
 * depend on the calibrated oscillator. This function is not thread-safe,
 * so it should not be called from multiple tasks concurrently.
 *
 * @return None
 ******************************************************************************/
void sl_clock_manager_wait_rco_calibration(void);

/***************************************************************************//**
 * @brief This function is used to obtain the current calibration count value
 * from the RCO calibration process. It should be called after the RCO
 * calibration has been configured and executed to retrieve the result.
 * The function requires a valid pointer to store the count value, and it
 * will return an error if the pointer is null. This function is not
 * thread-safe, so it should not be called concurrently from multiple
 * tasks.
 *
 * @param count A pointer to a uint32_t where the calibration count value will
 * be stored. Must not be null. If null, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t indicating success or the type of error
 * encountered, such as SL_STATUS_NULL_POINTER if the input pointer is
 * null.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_rco_calibration_count(uint32_t *count);

/***************************************************************************//**
 * @brief This function is used to ensure that the USB Phase-Locked Loop
 * (USBPLL) clock is ready before proceeding with operations that depend
 * on it. It should be called when the USBPLL clock is required to be
 * stable and operational, typically during initialization or before
 * enabling USB-related functionalities. The function returns a status
 * code indicating whether the USBPLL clock is ready or if an error
 * occurred during the wait process.
 *
 * @return Returns a status code of type sl_status_t, indicating SL_STATUS_OK if
 * the USBPLL clock is ready, or an error code if it is not.
 ******************************************************************************/
sl_status_t sl_clock_manager_wait_usbpll(void);

/***************************************************************************//**
 * @brief This function is a callback that is invoked when the High-Frequency
 * Crystal Oscillator (HFXO) fails to start twice consecutively with
 * normal configurations. It indicates a potential issue with the
 * crystal, such as a bad crystal, and should be treated as an error
 * condition. The function is called only if the
 * SL_CLOCK_MANAGER_HFXO_SLEEPY_CRYSTAL_SUPPORT configuration is enabled.
 * When this callback is triggered, the HFXO is running, but its
 * properties like frequency and precision are not guaranteed.
 *
 * @return None
 ******************************************************************************/
void sl_clock_manager_hfxo_notify_consecutive_failed_startups(void);

/***************************************************************************//**
 * Sets the external FLASH reference clock.
 *
 * @param[in] oscillator  Oscillator used to clock the external FLASH.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note This API is not thread-safe and should therefore not be called
 *       across multiple tasks.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function configures the oscillator used to clock the external
 * FLASH memory. It should be used when there is a need to change the
 * clock source for the external FLASH, typically during system
 * configuration or when adjusting for different operational conditions.
 * The function is not thread-safe, so it should not be called
 * concurrently from multiple tasks. Ensure that the Clock Manager is
 * properly initialized before calling this function to avoid undefined
 * behavior.
 *
 * @param oscillator Specifies the oscillator to be used for the external FLASH
 * clock. It must be a valid `sl_oscillator_t` value. Invalid
 * values may result in an error status being returned.
 * @return Returns an `sl_status_t` status code, which is `SL_STATUS_OK` if the
 * operation is successful, or an error code if it fails.
 ******************************************************************************/
sl_status_t sl_clock_manager_set_ext_flash_clk(sl_oscillator_t oscillator);

/***************************************************************************//**
 * Gets the external FLASH clock source.
 *
 * @param[out] oscillator Oscillator used to clock the external FLASH.
 *
 * @return  Status code.
 *          SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_CLOCK_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to obtain the current oscillator that is being used
 * to clock the external FLASH. This function is useful when you need to
 * verify or log the current clock source for the external FLASH. It is
 * important to ensure that the pointer provided is valid and that the
 * function is called in a context where it is safe to access the clock
 * manager, as the function is not thread-safe.
 *
 * @param oscillator A pointer to an sl_oscillator_t variable where the current
 * oscillator used for the external FLASH will be stored. Must
 * not be null, and the caller retains ownership of the
 * memory.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned if successful, otherwise an error
 * code is returned.
 ******************************************************************************/
sl_status_t sl_clock_manager_get_ext_flash_clk(sl_oscillator_t *oscillator);

/** @} (end addtogroup clock_manager) */

#ifdef __cplusplus
}
#endif

#endif // SL_CLOCK_MANAGER_H
