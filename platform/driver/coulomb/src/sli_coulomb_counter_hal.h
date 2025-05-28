/***************************************************************************//**
 * @file
 * @brief Coulomb Counter Driver Internal Header
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

#ifndef SLI_COULOMB_COUNTER_HAL_H
#define SLI_COULOMB_COUNTER_HAL_H

#include "sl_coulomb_counter.h"
#include "sl_slist.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
#define EMU_VSCALE0         0
#define EMU_VSCALE1         1
#define EMU_VSCALE2         2
#define NANOCOULOMB_TO_COULOMB  1000000000

// Constants for Calibration Current Load
#if SL_COULOMB_COUNTER_DRIVER_PERIPHERAL == SL_COULOMB_COUNTER_DRIVER_PERIPHERAL_EFP
#define CCL_LEVEL_LOW     (0x3)
#else
#define CCL_LEVEL_LOW     (0x2)
#endif
#define CCL_LEVEL_HIGH    (0x7)
/// @endcond

/// @brief Output's calibration state machine enum.
/***************************************************************************//**
 * @brief The `sli_coulomb_counter_calibration_state_t` is an enumeration that
 * defines the various states of the calibration process for a Coulomb
 * counter. It includes states for uncalibrated, settings requested for
 * different energy modes (EM0 and EM2), and stages of the calibration
 * process such as starting with low or high current loads, and
 * completion. This enum is used to track and manage the calibration
 * state within the Coulomb counter driver.
 *
 * @param SLI_COULOMB_COUNTER_CALIBRATION_NOT_CALIBRATED Indicates that the
 * calibration process has
 * not been started.
 * @param SLI_COULOMB_COUNTER_CALIBRATION_EM0_SETTINGS_REQUESTED Indicates that
 * settings for
 * EM0 mode have
 * been requested
 * for
 * calibration.
 * @param SLI_COULOMB_COUNTER_CALIBRATION_EM2_SETTINGS_REQUESTED Indicates that
 * settings for
 * EM2 mode have
 * been requested
 * for
 * calibration.
 * @param SLI_COULOMB_COUNTER_CALIBRATION_NREQ_LOW_STARTED Indicates that
 * calibration with low
 * nreq value has
 * started.
 * @param SLI_COULOMB_COUNTER_CALIBRATION_CAL_LOW_STARTED Indicates that
 * calibration with low
 * current load has
 * started.
 * @param SLI_COULOMB_COUNTER_CALIBRATION_CAL_HIGH_STARTED Indicates that
 * calibration with high
 * current load has
 * started.
 * @param SLI_COULOMB_COUNTER_CALIBRATION_COMPLETE Indicates that the
 * calibration process is
 * complete.
 ******************************************************************************/
typedef enum {
  SLI_COULOMB_COUNTER_CALIBRATION_NOT_CALIBRATED,
  SLI_COULOMB_COUNTER_CALIBRATION_EM0_SETTINGS_REQUESTED,
  SLI_COULOMB_COUNTER_CALIBRATION_EM2_SETTINGS_REQUESTED,
  SLI_COULOMB_COUNTER_CALIBRATION_NREQ_LOW_STARTED,
  SLI_COULOMB_COUNTER_CALIBRATION_CAL_LOW_STARTED,
  SLI_COULOMB_COUNTER_CALIBRATION_CAL_HIGH_STARTED,
  SLI_COULOMB_COUNTER_CALIBRATION_COMPLETE,
} sli_coulomb_counter_calibration_state_t;

/// @brief Structure for storing output state.
/***************************************************************************//**
 * @brief The `sli_coulomb_counter_output_t` structure is designed to store the
 * state and results of a Coulomb counter's output. It includes fields
 * for tracking the calibration state and mode, the charge per pulse
 * calculated during calibration, and the total accumulated charge.
 * Additionally, it contains an output mask and a node for linking in a
 * list, making it suitable for managing multiple outputs in a linked
 * list format. This structure is integral to the operation of the
 * Coulomb counter driver, facilitating the management and calibration of
 * charge measurements.
 *
 * @param calibration_state Represents the current state of the calibration
 * process.
 * @param calibration_mode Indicates the mode used during the last calibration.
 * @param cpp Stores the charge per pulse value calculated during calibration.
 * @param total_charge Holds the total accumulated charge.
 * @param mask Defines the output mask for the structure.
 * @param node A node used for linking this structure in a linked list.
 ******************************************************************************/
typedef struct sli_coulomb_counter_output_result {
  sli_coulomb_counter_calibration_state_t   calibration_state;  ///< Calibration state machine.
  uint8_t calibration_mode;                                     ///< Current mode when output was calibrated.
  float cpp;                                                    ///< Charge per pulse computed during calibration.
  float total_charge;                                           ///< Accumulated charge.
  sl_coulomb_counter_output_mask_t mask;                        ///< Output mask.
  sl_slist_node_t node;                                         ///< Node for inclusion in linked list.
} sli_coulomb_counter_output_t;

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
#define SLI_COULOMB_COUNTER_OUTPUT_INIT(_mask)                           \
  {                                                                      \
    .calibration_state = SLI_COULOMB_COUNTER_CALIBRATION_NOT_CALIBRATED, \
    .calibration_mode = 0,                                               \
    .cpp = 0,                                                            \
    .total_charge = 0,                                                   \
    .mask = _mask,                                                       \
  }
/// @endcond

/// @brief Structure for the Coulomb counter driver.
/***************************************************************************//**
 * @brief The `sli_coulomb_counter_handle_t` structure is a comprehensive data
 * structure used to manage and track the state and configuration of a
 * Coulomb counter driver. It includes fields for managing calibration
 * parameters, such as the number of pulses and nreq values for both low
 * and high current calibrations, as well as flags and settings for
 * different energy modes (EM0 and EM2). The structure also maintains a
 * linked list of outputs and tracks whether the counters are running,
 * providing a robust framework for handling the Coulomb counting process
 * in embedded systems.
 *
 * @param prescaler Prescaler value used when the peripheral is EFP.
 * @param threshold Threshold value for the Coulomb counter.
 * @param running Flag indicating if the counters are currently started.
 * @param selected_outputs Outputs selected as per the configuration header.
 * @param output_head Pointer to the head of the outputs linked list.
 * @param cal_nreq_low nreq value for calibration with low current.
 * @param cal_nreq_high nreq value for calibration with high current.
 * @param cal_count_low Number of pulses counted during low current calibration.
 * @param cal_count_high Number of pulses counted during high current
 * calibration.
 * @param em2_requested Indicates if the last mode requested by the user is EM2.
 * @param cal_em0_vscale Voltage scaling programmed for EM0 mode.
 * @param cal_em2_vscale Voltage scaling programmed for EM2 mode.
 ******************************************************************************/
typedef struct sli_coulomb_counter_handle {
#if SL_COULOMB_COUNTER_DRIVER_PERIPHERAL == SL_COULOMB_COUNTER_DRIVER_PERIPHERAL_EFP
  uint8_t                             prescaler;        ///< Prescaler value.
#endif
  uint8_t                             threshold;        ///< Threshold value.
  bool                                running;          ///< Flag to track if counters are started.
  sl_coulomb_counter_output_mask_t    selected_outputs; ///< Outputs selected in config header.
  sl_slist_node_t                     *output_head;     ///< Head of outputs linked list.

  unsigned int cal_nreq_low;   ///< nreq value for calibration with low current.
  unsigned int cal_nreq_high;  ///< nreq value for calibration with high current.
  unsigned int cal_count_low;  ///< Number of pulses for calibration with low current.
  unsigned int cal_count_high; ///< Number of pulses for calibration with high current.
  bool em2_requested;          ///< True if last mode requested to user is EM2.
  int cal_em0_vscale;          ///< Voltage scaling programmed for EM0.
  int cal_em2_vscale;          ///< Voltage scaling programmed for EM2.
} sli_coulomb_counter_handle_t;

/***************************************************************************//**
 * @brief
 *   HAL implementation to enable interrupt flag.
 *
 * @param[in] flag
 *   Interrupt flag to be enabled.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_int_enable(uint8_t flag);

/***************************************************************************//**
 * @brief
 *   HAL implementation to disable interrupt.
 *
 * @param[in] flags
 *   Interrupt flag to be disabled.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_int_disable(uint8_t flag);

/***************************************************************************//**
 * @brief
 *   HAL implementation for checking if interrupt flag is set.
 *
 * @param[in] flag
 *   Interrupt flag to be checked.
 *
 * @param[out] is_set
 *   Boolean with result.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_int_is_set(uint8_t flag, bool *is_set);

/***************************************************************************//**
 * @brief
 *   HAL implementation for clearing interrupt flag.
 *
 * @param[in] flag
 *   Interrupt flag to be checked.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_int_clear(uint8_t flag);

/***************************************************************************//**
 * @brief
 *   HAL-specific initialization. This is called by
 *   @ref sl_coulomb_counter_init().
 *
 * @param[in] handle
 *   Driver handle.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_init(const sli_coulomb_counter_handle_t *handle);

/***************************************************************************//**
 * @brief
 *   Check if output is available in EM2 or not.
 *
 * param[in] output
 *   Output to be checked
 *
 * @return
 *   True if output is available, false otherwise.
 ******************************************************************************/
bool sli_coulomb_counter_hal_output_supports_em2(const sli_coulomb_counter_output_t *output);

/***************************************************************************//**
 * @brief
 *   Return pointer to output structure that matches the mask.
 *
 * @param[in] mask
 *   Output mask
 *
 * @return
 *   A pointer to output structure, or NULL if HAL doesn't support that output.
 ******************************************************************************/
sli_coulomb_counter_output_t* sli_coulomb_counter_hal_get_output(sl_coulomb_counter_output_mask_t mask);

/***************************************************************************//**
 * @brief
 *   HAL implementation to start counting Coulombs.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_start(void);

/***************************************************************************//**
 * @brief
 *   HAL implementation to stop counting Coulombs.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_stop(void);

/***************************************************************************//**
 * @brief
 *   Read counters for that output and increment its total charge.
 *
 * @param[in] output
 *   Output to be updated
 *
 * @param[out] need_recalibration
 *   True if DCDC operating mode has changed since last calibration.
 *
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_read_output(sli_coulomb_counter_output_t *output,
                                                bool *need_recalibration);

/***************************************************************************//**
 * @brief
 *   Reset Coulomb counters for all outputs to zero.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_clear_counters(void);

/***************************************************************************//**
 * @brief
 *   Start calibration for given output.
 *
 * @param[in] output
 *   Output to be calibrated.
 *
 * @param[in] nreq
 *   Number of PFM pulses to count for calibration.
 *
 * @param[in] ccl_level
 *   Calibration current load level.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_cal_start(sli_coulomb_counter_output_t *output,
                                              int8_t nreq,
                                              int8_t ccl_level);

/***************************************************************************//**
 * @brief
 *   Stop the calibration. Remove current load that was applied for calibration.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_cal_stop(void);

/***************************************************************************//**
 * @brief
 *   Read calibration result, i.e. the number of calibration clock cycles that were
 *   counted.
 *
 * @param[out] result
 *   Calibration result.
 *
 * @return
 *   0 if successful, error code otherwise.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_cal_read_result(uint16_t *result);

/***************************************************************************//**
 * @brief This function is used to adjust the Charge-Per-Pulse (CPP) value of a
 * specified output when there is a difference in voltage scaling between
 * Energy Modes 0 (EM0) and 2 (EM2). It should be called when the voltage
 * scaling settings for these modes differ, to ensure accurate charge
 * measurement. The function requires a valid output structure that
 * supports EM2, and it modifies the CPP value within this structure. It
 * returns a status code indicating success or the type of error
 * encountered.
 *
 * @param output A pointer to a sli_coulomb_counter_output_t structure
 * representing the output for which the CPP must be adjusted.
 * Must not be null and must support EM2.
 * @param em0_vscale An integer representing the voltage scaling value for EM0.
 * Must be a valid scaling value.
 * @param em2_vscale An integer representing the voltage scaling value for EM2.
 * Must be a valid scaling value.
 * @return Returns an sl_status_t indicating success (SL_STATUS_OK) or an error
 * code if the adjustment could not be performed.
 ******************************************************************************/
sl_status_t sli_coulomb_counter_hal_cal_adjust_em2_cpp(sli_coulomb_counter_output_t *output,
                                                       int em0_vscale, int em2_vscale);

/***************************************************************************//**
 * @brief
 *   Convert calibration current load enum to current value.
 *
 * @param[in] ccl_level
 *   Calibration current load, as set in CC_CAL.CCL_LVL
 *
 * @return
 *   Current value, in uA.
 ******************************************************************************/
float sli_coulomb_counter_hal_cal_get_load_current(int8_t ccl_level);

/***************************************************************************//**
 * @brief
 *   Get calibration oscillator frequency.
 *
 * @details
 *   In case a more precise value for the oscillator frequency is available, use
 *   that to make the computation of the charge-per-pulse more accurate.
 *
 * @return
 *   calibration oscillator frequency, in Hz.
 ******************************************************************************/
float sli_coulomb_counter_hal_get_osc_frequency(void);

/***************************************************************************//**
 * @brief
 *   Perform the Charge-Per-Pulse (CPP) Calculation.
 *
 * @param[in] handle
 *   Driver handle.
 *
 * @return
 *   Computed CPP, in nanocoulombs.
 ******************************************************************************/
float sli_coulomb_counter_hal_compute_cpp(const sli_coulomb_counter_handle_t *handle);

/***************************************************************************//**
 * @brief This function calculates the maximum number of Pulse Frequency
 * Modulation (PFM) pulses, referred to as 'nreq', that can be used
 * during calibration without causing an overflow in the 16-bit register
 * pair CCC_MSBY:CCC_LSBY. It is typically used in the context of
 * calibrating a Coulomb counter to ensure accurate measurements without
 * exceeding register limits. The function clamps the result to a maximum
 * of 7 to ensure safety and asserts that the result is non-negative.
 *
 * @param ccc An integer value representing the combined value of the CCC_MSBY
 * and CCC_LSBY registers. It must be a positive integer to avoid
 * undefined behavior, as negative or zero values could lead to
 * invalid calculations.
 * @return Returns an integer representing the maximum number of PFM pulses that
 * can be used without causing an overflow, clamped to a maximum of 7.
 ******************************************************************************/
int sli_coulomb_counter_calibrate_compute_nreq(int16_t ccc);

#ifdef __cplusplus
}
#endif

#endif /* SLI_COULOMB_COUNTER_HAL_H */
