/***************************************************************************//**
 * @file
 * @brief Coulomb Counter Driver API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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

/**************************************************************************//**
 * @addtogroup coulomb_counter Coulomb Counter API
 * @brief Coulomb Counter Driver API.
 * @{
 *****************************************************************************/

#ifndef SL_COULOMB_COUNTER_H
#define SL_COULOMB_COUNTER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "em_device.h"
#include "sl_slist.h"
#include "sl_status.h"

#include "sl_coulomb_counter_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *******************************  DEFINES   ************************************
 ******************************************************************************/

#if (SL_COULOMB_COUNTER_DRIVER_PERIPHERAL == SL_COULOMB_COUNTER_DRIVER_PERIPHERAL_EFP)
/// EFP's Output VOA.
#define SL_COULOMB_COUNTER_OUTPUT_EFP_VOA         ((sl_coulomb_counter_output_mask_t)0x01)
/// EFP's Output VOB in EM0 mode.
#define SL_COULOMB_COUNTER_OUTPUT_EFP_VOB_EM0     ((sl_coulomb_counter_output_mask_t)0x02)
/// EFP's Output VOB in EM2 mode.
#define SL_COULOMB_COUNTER_OUTPUT_EFP_VOB_EM2     ((sl_coulomb_counter_output_mask_t)0x04)
/// EFP's Output VOC.
#define SL_COULOMB_COUNTER_OUTPUT_EFP_VOC         ((sl_coulomb_counter_output_mask_t)0x08)
/// All Outputs.
#define SL_COULOMB_COUNTER_OUTPUT_ALL             ((sl_coulomb_counter_output_mask_t)0x0F)
#endif

#if (SL_COULOMB_COUNTER_DRIVER_PERIPHERAL == SL_COULOMB_COUNTER_DRIVER_PERIPHERAL_DCDC)
/// DCDC's Output VOB in EM0 mode.
#define SL_COULOMB_COUNTER_OUTPUT_DCDC_EM0        ((sl_coulomb_counter_output_mask_t)0x01)
/// DCDC's Output VOB in EM2 mode.
#define SL_COULOMB_COUNTER_OUTPUT_DCDC_EM2        ((sl_coulomb_counter_output_mask_t)0x02)
/// All Outputs.
#define SL_COULOMB_COUNTER_OUTPUT_ALL             ((sl_coulomb_counter_output_mask_t)0x03)
#endif

/// Calibration done interrupt flag.
#define SL_COULOMB_COUNTER_INT_CALIBRATION_DONE   0x1
/// Counter full interrupt flag.
#define SL_COULOMB_COUNTER_INT_COUNTER_FULL       0x2

/// Bitmask of outputs to operate on.
typedef uint32_t sl_coulomb_counter_output_mask_t;

/***************************************************************************//**
 * @brief The `sl_coulomb_counter_prescaler_t` is an enumeration that defines
 * different prescaler settings for a Coulomb counter. Each enumerator
 * represents a specific prescaler value, which affects the resolution of
 * the counter by determining how many pulse-frequency modulation (PFM)
 * pulses are counted per unit of charge. The prescaler setting is
 * crucial for calibrating the counter to accurately measure the charge
 * passing through a system, as it influences the calculation of charge-
 * per-pulse (CPP) values.
 *
 * @param SL_COULOMB_COUNTER_PRESCALER_ZERO Set prescaler to 0.
 * @param SL_COULOMB_COUNTER_PRESCALER_ONE Set prescaler to 1.
 * @param SL_COULOMB_COUNTER_PRESCALER_TWO Set prescaler to 2.
 * @param SL_COULOMB_COUNTER_PRESCALER_THREE Set prescaler to 3.
 ******************************************************************************/
typedef enum {
  SL_COULOMB_COUNTER_PRESCALER_ZERO,  ///< Set prescaler to 0.
  SL_COULOMB_COUNTER_PRESCALER_ONE,   ///< Set prescaler to 1.
  SL_COULOMB_COUNTER_PRESCALER_TWO,   ///< Set prescaler to 2.
  SL_COULOMB_COUNTER_PRESCALER_THREE, ///< Set prescaler to 3.
} sl_coulomb_counter_prescaler_t;

/***************************************************************************//**
 * @brief The `sl_coulomb_counter_threshold_t` is an enumeration that defines
 * various threshold levels for the Coulomb counter's full interrupt,
 * expressed as percentages. These thresholds determine when the
 * interrupt will be triggered based on the percentage of the counter's
 * capacity that has been reached. This allows for configurable alerting
 * when the counter approaches its full capacity, enabling timely actions
 * to be taken in response to the counter's status.
 *
 * @param SL_COULOMB_COUNTER_THRESHOLD_50 Set threshold to 50%.
 * @param SL_COULOMB_COUNTER_THRESHOLD_62 Set threshold to 62.5%.
 * @param SL_COULOMB_COUNTER_THRESHOLD_75 Set threshold to 75%.
 * @param SL_COULOMB_COUNTER_THRESHOLD_87 Set threshold to 87.5%.
 ******************************************************************************/
typedef enum {
  SL_COULOMB_COUNTER_THRESHOLD_50,  ///< Set threshold to 50%.
  SL_COULOMB_COUNTER_THRESHOLD_62,  ///< Set threshold to 62.5%.
  SL_COULOMB_COUNTER_THRESHOLD_75,  ///< Set threshold to 75%.
  SL_COULOMB_COUNTER_THRESHOLD_87,  ///< Set threshold to 87.5%.
} sl_coulomb_counter_threshold_t;

/// @brief Coulomb counter calibration status enum.
/***************************************************************************//**
 * @brief The `sl_coulomb_counter_calibration_status_t` is an enumeration that
 * defines the various states of the calibration process for a Coulomb
 * counter. It includes states for when an error occurs, when calibration
 * is ongoing, when specific peak current settings are required for
 * different energy modes (EM0 and EM2), and when the calibration process
 * is complete. This enum is used to track and manage the calibration
 * status within the Coulomb counter driver API.
 *
 * @param SL_COULOMB_COUNTER_CALIBRATION_ERROR An error occurred during
 * calibration.
 * @param SL_COULOMB_COUNTER_CALIBRATION_IN_PROGRESS Calibration is in progress.
 * @param SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM0 User must set peak
 * current to EM0 value.
 * @param SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM2 User must set peak
 * current to EM2 value.
 * @param SL_COULOMB_COUNTER_CALIBRATION_DONE Calibration is done.
 ******************************************************************************/
typedef enum {
  SL_COULOMB_COUNTER_CALIBRATION_ERROR,             ///< An error occured during calibration.
  SL_COULOMB_COUNTER_CALIBRATION_IN_PROGRESS,       ///< Calibration is in progress.
  SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM0,  ///< User must set peak current to EM0 value.
  SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM2,  ///< User must set peak current to EM2 value.
  SL_COULOMB_COUNTER_CALIBRATION_DONE,              ///< Calibration is done.
} sl_coulomb_counter_calibration_status_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function is used to enable a specific interrupt flag for the
 * Coulomb counter, allowing the system to respond to certain events. It
 * should be called when you want to start monitoring a particular
 * condition indicated by the interrupt flag. Ensure that the flag
 * provided is valid and corresponds to a defined interrupt condition.
 * The function returns a status code indicating success or failure,
 * which should be checked to ensure the operation was successful.
 *
 * @param flag An 8-bit unsigned integer representing the interrupt flag to be
 * enabled. Valid values are specific interrupt flags defined in the
 * API, such as SL_COULOMB_COUNTER_INT_CALIBRATION_DONE or
 * SL_COULOMB_COUNTER_INT_COUNTER_FULL. Providing an invalid flag
 * may result in an error status.
 * @return Returns an sl_status_t value, which is 0 if the operation is
 * successful or an error code if it fails.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_int_enable(uint8_t flag);

/***************************************************************************//**
 * @brief Use this function to disable a specific interrupt associated with the
 * Coulomb counter by providing the appropriate interrupt flag. This is
 * useful when you want to prevent the interrupt from triggering further
 * actions in your application. Ensure that the flag provided corresponds
 * to a valid interrupt type for the Coulomb counter. The function
 * returns a status code indicating success or failure, which should be
 * checked to ensure the operation was successful.
 *
 * @param flag An 8-bit unsigned integer representing the interrupt flag to be
 * disabled. Valid flags are defined in the API, such as
 * SL_COULOMB_COUNTER_INT_CALIBRATION_DONE or
 * SL_COULOMB_COUNTER_INT_COUNTER_FULL. Providing an invalid flag
 * may result in an error status.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. A return value of 0 indicates success, while any other
 * value indicates an error.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_int_disable(uint8_t flag);

/***************************************************************************//**
 * @brief This function checks whether a given interrupt flag is currently set
 * and stores the result in a boolean variable. It is useful for
 * determining the status of specific interrupts in the Coulomb Counter
 * system. The function should be called with a valid interrupt flag and
 * a non-null pointer to a boolean variable where the result will be
 * stored. It is important to ensure that the pointer provided for the
 * result is valid and writable.
 *
 * @param flag An 8-bit unsigned integer representing the interrupt flag to be
 * checked. Valid values are specific interrupt flags defined in the
 * API, such as SL_COULOMB_COUNTER_INT_CALIBRATION_DONE or
 * SL_COULOMB_COUNTER_INT_COUNTER_FULL.
 * @param is_set A pointer to a boolean variable where the result will be
 * stored. Must not be null. The function writes 'true' if the
 * interrupt flag is set, otherwise 'false'.
 * @return Returns an sl_status_t value indicating success or an error code if
 * the operation fails.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_int_is_set(uint8_t flag, bool *is_set);

/***************************************************************************//**
 * @brief Use this function to clear a specific interrupt flag associated with
 * the Coulomb counter. This is typically done after handling an
 * interrupt to reset the flag and prepare for future interrupts. Ensure
 * that the flag parameter corresponds to a valid interrupt flag defined
 * for the Coulomb counter. The function returns a status code indicating
 * success or an error, which should be checked to ensure the operation
 * was successful.
 *
 * @param flag An 8-bit unsigned integer representing the interrupt flag to be
 * cleared. Valid values are specific interrupt flags defined for
 * the Coulomb counter, such as
 * SL_COULOMB_COUNTER_INT_CALIBRATION_DONE or
 * SL_COULOMB_COUNTER_INT_COUNTER_FULL. Passing an invalid flag may
 * result in an error status.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. A return value of 0 indicates success, while any other
 * value indicates an error.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_int_clear(uint8_t flag);

/***************************************************************************//**
 * @brief This function prepares the Coulomb counter peripheral for operation by
 * setting up necessary configurations and initializing internal
 * structures. It must be called before any other Coulomb counter
 * operations to ensure the peripheral is properly configured. The
 * function checks for available outputs and initializes them for
 * measurement. If any output initialization fails, the function returns
 * an error status. This function should be used at the start of any
 * sequence involving Coulomb counting to ensure the peripheral is ready
 * for use.
 *
 * @return Returns SL_STATUS_OK if initialization is successful, or an error
 * code if it fails.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_init(void);

/***************************************************************************//**
 * @brief This function initiates the Coulomb counting process, provided that
 * all selected outputs have been calibrated. It should be called after
 * the initialization and calibration of the Coulomb counter. If any
 * output is not calibrated, the function will fail and return an error
 * status. Once started, the counter will begin measuring the charge
 * passing through the outputs. Calibration is not allowed while the
 * counter is running, so ensure all necessary calibrations are completed
 * beforehand.
 *
 * @return Returns SL_STATUS_OK if the counter starts successfully, or an error
 * code if it fails, such as when outputs are not calibrated.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_start(void);

/***************************************************************************//**
 * @brief This function halts the operation of the coulomb counter, which is
 * necessary before performing certain operations like calibration. It
 * should be called when you need to stop counting coulombs, for
 * instance, to prepare for recalibration or to conserve power. Ensure
 * that the counter is running before calling this function to avoid
 * unnecessary operations.
 *
 * @return Returns a status code indicating success or failure of the stop
 * operation.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_stop(void);

/***************************************************************************//**
 * @brief This function reads the coulomb counters for all configured outputs
 * and identifies which outputs require recalibration. It should be
 * called periodically to ensure accurate charge measurement, especially
 * after changes in operating conditions. The function initializes the
 * outputs_need_calibration_mask to zero and updates it with a mask of
 * outputs that need recalibration. It returns an error status if reading
 * any output fails. Ensure that the system is properly initialized and
 * calibrated before calling this function.
 *
 * @param outputs_need_calibration_mask A pointer to a variable where the
 * function will store a bitmask indicating
 * which outputs need recalibration. Must
 * not be null, and the caller retains
 * ownership.
 * @return Returns an sl_status_t indicating success or an error code if reading
 * fails. The outputs_need_calibration_mask is updated with outputs that
 * need recalibration.
 ******************************************************************************/
sl_status_t sl_coulomb_counter_read(sl_coulomb_counter_output_mask_t *outputs_need_calibration_mask);

/***************************************************************************//**
 * @brief This function calculates and returns the total charge that has passed
 * through the outputs specified by the provided mask. It should be used
 * after reading the coulomb counters with the appropriate function to
 * ensure accurate charge accumulation. The function is useful for
 * obtaining charge data for specific outputs, especially in systems
 * where multiple outputs are monitored. Ensure that the outputs have
 * been properly calibrated and that the counters have been read to
 * accumulate the necessary data before calling this function.
 *
 * @param outputs_mask A bitmask indicating which outputs to include in the
 * charge calculation. Valid masks are defined in the API
 * and correspond to specific outputs. The function will sum
 * the charge for all outputs that match the mask. If an
 * invalid mask is provided, the function will simply ignore
 * outputs not represented by the mask.
 * @return Returns a float representing the total charge for the selected
 * outputs.
 ******************************************************************************/
float sl_coulomb_counter_get_charge(sl_coulomb_counter_output_mask_t outputs_mask);

/***************************************************************************//**
 * @brief This function retrieves the total charge that has passed through all
 * outputs selected during the initialization of the Coulomb counter. It
 * should be used after the Coulomb counter has been properly initialized
 * and started, and after any necessary calibration has been completed.
 * This function is useful for obtaining a cumulative charge measurement
 * across all configured outputs. It is important to ensure that the
 * Coulomb counter has been reading and accumulating charge data before
 * calling this function to get meaningful results.
 *
 * @return Returns the total charge for the selected outputs as a floating-point
 * value.
 ******************************************************************************/
float sl_coulomb_counter_get_total_charge(void);

/***************************************************************************//**
 * @brief This function provides a bitmask indicating which Coulomb counter
 * outputs are currently available for use, based on the selected
 * peripheral configuration. It is useful for determining which outputs
 * can be monitored or controlled by the Coulomb counter driver. This
 * function should be called after the peripheral has been initialized to
 * ensure accurate information about available outputs.
 *
 * @return Returns a bitmask of type `sl_coulomb_counter_output_mask_t`
 * representing the available outputs for the selected peripheral.
 ******************************************************************************/
sl_coulomb_counter_output_mask_t sl_coulomb_counter_outputs_available(void);

/***************************************************************************//**
 * @brief Use this function to determine which outputs of the Coulomb counter
 * require recalibration. This is particularly useful after events that
 * might affect calibration accuracy, such as changes in operating mode
 * or significant variations in input voltage. The function should be
 * called when recalibration is suspected to ensure accurate charge
 * measurements. It returns a bitmask indicating the outputs that need
 * recalibration, allowing the user to take appropriate action.
 *
 * @return Returns a bitmask of outputs that need recalibration.
 ******************************************************************************/
sl_coulomb_counter_output_mask_t sl_coulomb_counter_outputs_need_calibration(void);

/***************************************************************************//**
 * @brief This function prepares the specified outputs for calibration by
 * marking them as not calibrated. It must be called when the coulomb
 * counter is not running, as calibration cannot proceed while the
 * counter is active. The function checks that the requested outputs were
 * selected during initialization and returns an error if any unselected
 * outputs are included. This function is typically used as the first
 * step in the calibration process to ensure that the outputs are ready
 * for subsequent calibration steps.
 *
 * @param outputs_mask A bitmask indicating which outputs to prepare for
 * calibration. The outputs must have been selected during
 * initialization, and the function will return an error if
 * any unselected outputs are included in the mask.
 * @return Returns SL_COULOMB_COUNTER_CALIBRATION_IN_PROGRESS if the
 * initialization is successful, or SL_COULOMB_COUNTER_CALIBRATION_ERROR
 * if the counter is running or if invalid outputs are specified.
 ******************************************************************************/
sl_coulomb_counter_calibration_status_t sl_coulomb_counter_calibrate_init(sl_coulomb_counter_output_mask_t outputs_mask);

/***************************************************************************//**
 * @brief This function is used to wait for the completion of the current step
 * in the calibration process of the Coulomb counter. It should be called
 * when the calibration process is in progress, specifically after
 * `sl_coulomb_counter_calibrate()` returns
 * `SL_COULOMB_COUNTER_CALIBRATION_IN_PROGRESS`. This function is
 * necessary when calibrating in polling mode, as it ensures that the
 * calibration step is fully completed before proceeding. It is not used
 * in interrupt mode calibration. The function returns a status
 * indicating whether the calibration is still in progress or if an error
 * has occurred.
 *
 * @return Returns `SL_COULOMB_COUNTER_CALIBRATION_IN_PROGRESS` if the
 * calibration step is successfully completed, or
 * `SL_COULOMB_COUNTER_CALIBRATION_ERROR` if an error occurs.
 ******************************************************************************/
sl_coulomb_counter_calibration_status_t sl_coulomb_counter_calibrate_wait(void);

/***************************************************************************//**
 * @brief This function is used to advance the calibration process of the
 * coulomb counter. It should be called repeatedly until the calibration
 * is complete or an error occurs. Calibration cannot be performed while
 * the counter is running, so ensure the counter is stopped before
 * calling this function. The function handles different calibration
 * states and may require the user to adjust peak current settings for
 * specific energy modes. It returns various status codes to indicate the
 * current state of the calibration process, including whether it is
 * complete, in progress, or if an error has occurred.
 *
 * @return Returns a status code indicating the current state of the calibration
 * process, such as completion, error, or a request to change peak
 * current settings.
 ******************************************************************************/
sl_coulomb_counter_calibration_status_t sl_coulomb_counter_calibrate(void);

/** @} (end addtogroup coulomb_counter) */

/* *INDENT-OFF* */
/* THE REST OF THE FILE IS DOCUMENTATION ONLY! */
/// @addtogroup coulomb_counter Coulomb Counter API
/// @{
///
///   @details
///
///   @n @section coulomb_counter_intro Introduction
///
///   DCDC converters use pulse-frequency modulation (PFM) to drive outputs.
///   The Coulomb counter counts the number of PFM pulses delivered to each
///   output. In order to get the total charge from an output, the
///   charge-per-pulse (CPP) must be determined, then the total charge is
///   computed like so: total_charge = CPP * Num_pulses.
///
///   Currently, EFP's DCDC and internal DCDC Coulomb counter (supported parts
///   only) are supported by this driver. The Coulomb counter driver provides an
///   abstraction on top of the Coulomb counter registers to make the calibration
///   and reading the counters easier.
///
///   @n @section coulomb_counter_calibration Calibration
///
///   The goal of the calibration is to compute the charge-per-pulse value for
///   each output. In order to achieve that, known loads are applied to outputs
///   and the number of pulses are counted. More details can be found in
///   application note AN1188: EFP01 Coulomb Counting.
///
///   One of the main assumption that is made to compute the CPP is that the
///   system load current remains relatively constant during calibration, that
///   means the application should not switch energy mode, start radio
///   transmissions, etc. This is critical for calibration accuracy. If this is
///   not followed, the driver will return inaccurate results.
///
///   Hardware is capable of calibrating only one output at a time. For each
///   output, there are at least two measurements, one with low current and one
///   with high current. In the worst case scenario, a measurement can take up
///   to 6.5ms (that is 2^16, the size of the counter, divided by 10MHz, the
///   oscillator frequency). So if several outputs are to be calibrated, it will
///   happen sequentially, that means the calibration time is proportional to
///   the number of outputs.
///
///   @n @section coulomb_counter_em2_output EM2 Output
///
///   EFP's VOB output has different voltage and peak current configuration for
///   EM0 and EM2 energy modes. This output can be used to power the DECOUPLE
///   line of the MCU. It also has different Coulomb counters for these two
///   energy modes. As peak current and output voltage are some of the
///   parameters that determine the CPP, the output must be calibrated for each
///   energy mode.
///
///   Typically, EM2 peak current and voltage are lower than those of EM0 for
///   improved efficiency. In order to accurately compute the CPP, the Coulomb
///   counter driver will request at some point during the calibration that the
///   EM2 peak current configuration is applied. Application should take care of
///   limiting actions that draw too much current during that time.
///
///   @n @section coulomb_counter_recalibration Recalibration
///
///   A number of conditions can affect the CPP. Due to the following changes, a
///   recalibraiton might be necessary for accurate results:
///
///   @li Significant change in input voltage
///   @li Change of output voltage
///   @li Change of output peak current
///   @li Change in energy mode
///   @li Change of DCDC operating mode
///
///   For the latter, the driver will report when such event is detected, to let
///   the application know a recalibration must occur.
///
///   @n @section coulomb_counter_configuration Configuration
///
///   The Coulomb counter can be configured by using the configuration tool
///   within Simplicity Studio. From there one can select the number of the EFP
///   instance to use, which output to measure, prescaler and threshold values.
///
///   @n @section coulomb_counter_usage Usage
///
///   The expected flow of execution is:
///
///   @li initialize the driver
///   @li calibrate
///   @li start the counters
///   @li periodically or interrupt-based event to read the counters
///
///   Initialization is done by calling @ref sl_coulomb_counter_init(). It will
///   prepare internal structure and setup communication with Coulomb counter
///   device.
///
///   Calibration is the next step and is done in two folds, first the
///   calibration is initialized by calling @ref
///   sl_coulomb_counter_calibrate_init() and then
///   @ref sl_coulomb_counter_calibrate() is called repeatedly until it reports
///   an error or completion.
///
///   When calibration is done successfully, counters can be started by calling
///   @ref sl_coulomb_counter_start().
///
///   Then, periodically counters be read by calling
///   @ref sl_coulomb_counter_read(). This function has an output parameter to
///   notify if some outputs must be recalibrated. That will happen in the
///   output operates in a different DCDC mode than when it was calibrated. Note
///   that when counters are read, they are cleared afterwards, so if the
///   function is called too often, it will keep reading zeroes. This might be
///   an issue for output that have a slowly increasing count of pulses, like
///   VOB in EM2. To prevent that, one can wait for the interrupt threshold to
///   be raised before reading registers.
///
///   Finally, to get the value of all outputs in Coulomb, call
///   @ref sl_coulomb_counter_get_total_charge(). If interested in only one or
///   more outputs, use @ref sl_coulomb_counter_get_charge() instead.
///
///   Here is a complete example:
///
///   @code{.c}
///sl_coulomb_counter_calibration_status_t status;
///sl_coulomb_counter_output_mask_t outputs;
///sl_status_t err;
///float charge;
///
///err = sl_coulomb_counter_init();
///if (err != SL_STATUS_OK) {
///  /* handle error */
///}
///
////* get outputs that need to be calibrated */
///outputs = sl_coulomb_counter_outputs_need_calibration();
///status = sl_coulomb_counter_calibrate_init(outputs);
///
///while (status == SL_COULOMB_COUNTER_CALIBRATION_IN_PROGRESS
///       || status == SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM0
///       || status == SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM2) {
///
///  status = sl_coulomb_counter_calibrate();
///
///  if (status == SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM0) {
///    /* Apply EM0 peak current configuration */
///  } else if (status == SL_COULOMB_COUNTER_CALIBRATION_PEAK_CURRENT_EM2) {
///    /* Apply EM2 peak current configuration */
///  } else if (status == SL_COULOMB_COUNTER_CALIBRATION_IN_PROGRESS) {
///    /*
///     * wait for hardware to notify that operation is complete. When using
///     * interrupts, one can wait on the interrupt flag instead.
///     */
///    status = sl_coulomb_counter_calibrate_wait();
///  }
///}
///
///if (status != SL_COULOMB_COUNTER_CALIBRATION_DONE) {
///  /* handle error */
///}
///
///err = sl_coulomb_counter_start();
///if (err != SL_STATUS_OK) {
///  /* handle error */
///}
///
////*
/// * outputs is a bitmask of outputs that need recalibration because DCDC
/// * operation mode changed. If some bits are set, Coulomb counter must be stop
/// * and corresponding outputs recalibrated.
/// */
///err = sl_coulomb_counter_read(&outputs);
///if (err != SL_STATUS_OK) {
///  /* handle error */
///}
///
///float = sl_coulomb_counter_get_total_charge();
///   @endcode
///
/// @} (end addtogroup coulomb_counter)

#ifdef __cplusplus
}
#endif

#endif /* SL_COULOMB_COUNTER_H */
