/***************************************************************************//**
 * @file
 * @brief DCDC Coulomb Counter (DCDC_COULOMB_COUNTER) peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_HAL_DCDC_COULOMB_COUNTER_H
#define SL_HAL_DCDC_COULOMB_COUNTER_H

#include "em_device.h"

#if defined(DCDC_COUNT) && (DCDC_COUNT > 0) && defined(DCDC_CCCTRL_CCEN)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "sl_hal_dcdc_coulomb_counter_compat.h"
#include "sl_enum.h"
#include "em_cmu.h"
#include "em_emu.h"

/***************************************************************************//**
 * @addtogroup dcdccoulombcounter
 * @{
 ******************************************************************************/

/*******************************************************************************
 *********************************   ENUM   ************************************
 ******************************************************************************/

/** DCDC_COULOMB_COUNTER supported energy mode. */
SL_ENUM(sl_hal_dcdc_coulomb_counter_emode_t) {
  SL_HAL_DCDC_COULOMB_COUNTER_EM0 = 0, /**< EM0/1 energy mode. */
  SL_HAL_DCDC_COULOMB_COUNTER_EM2 = 1, /**< EM2/3 energy mode. */
};

/** DCDC_COULOMB_COUNTER Calibration Load Current Level setting. */
SL_ENUM(sl_hal_dcdc_coulomb_counter_calibration_load_level_t) {
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD0 = _DCDC_CCCALCTRL_CCLVL_LOAD0, /**< Nominal Load 0.25mA. */
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD1 = _DCDC_CCCALCTRL_CCLVL_LOAD1, /**< Nominal Load 0.50mA. */
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD2 = _DCDC_CCCALCTRL_CCLVL_LOAD2, /**< Nominal Load 1.00mA. */
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD3 = _DCDC_CCCALCTRL_CCLVL_LOAD3, /**< Nominal Load 1.50mA. */
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD4 = _DCDC_CCCALCTRL_CCLVL_LOAD4, /**< Nominal Load 2.00mA. */
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD5 = _DCDC_CCCALCTRL_CCLVL_LOAD5, /**< Nominal Load 4.00mA. */
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD6 = _DCDC_CCCALCTRL_CCLVL_LOAD6, /**< Nominal Load 6.00mA. */
  SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD7 = _DCDC_CCCALCTRL_CCLVL_LOAD7, /**< Nominal Load 8.00mA. */
};

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/// DCDC_COULOMB_COUNTER configuration structure.
/***************************************************************************//**
 * @brief The `sl_hal_dcdc_coulomb_counter_config_t` structure is used to
 * configure the Coulomb Counter thresholds for different energy modes in
 * a DCDC converter. It contains two members, `counter_threshold_em0` and
 * `counter_threshold_em2`, which specify the threshold values for energy
 * modes EM0 and EM2, respectively. This configuration is crucial for
 * managing power consumption and efficiency in embedded systems that
 * utilize different energy modes.
 *
 * @param counter_threshold_em0 Coulomb Counter Threshold in EM0.
 * @param counter_threshold_em2 Coulomb Counter Threshold in EM2.
 ******************************************************************************/
typedef struct {
  uint16_t counter_threshold_em0;      ///< Coulomb Counter Threshold in EM0.
  uint16_t counter_threshold_em2;      ///< Coulomb Counter Threshold in EM2.
} sl_hal_dcdc_coulomb_counter_config_t;

/// DCDC_COULOMB_COUNTER calibration configuration structure.
/***************************************************************************//**
 * @brief The `sl_hal_dcdc_coulomb_counter_calibration_config_t` structure is
 * used to configure the calibration settings for the DCDC Coulomb
 * Counter peripheral. It includes fields for specifying the reference
 * clock, the calibration count, the energy mode, and the power load
 * level. This configuration is essential for accurately calibrating the
 * Coulomb Counter to ensure precise energy measurements in different
 * operational modes and load conditions.
 *
 * @param reference_clk Coulomb Counter Calibration Reference Clock.
 * @param cal_count Coulomb Counter Calibration Reference Count.
 * @param cal_emode Coulomb Counter Calibration Energy Mode.
 * @param cal_load_level Coulomb Counter Calibration Power Load.
 ******************************************************************************/
typedef struct {
  CMU_Select_TypeDef reference_clk;                                     ///< Coulomb Counter Calibration Reference Clock.
  int8_t cal_count;                                                     ///< Coulomb Counter Calibration Reference Count.
  sl_hal_dcdc_coulomb_counter_emode_t cal_emode;                        ///< Coulomb Counter Calibration Energy Mode.
  sl_hal_dcdc_coulomb_counter_calibration_load_level_t cal_load_level;  ///< Coulomb Counter Calibration Power Load.
} sl_hal_dcdc_coulomb_counter_calibration_config_t;

/// Suggested default values for DCDC_COULOMB_COUNTER configuration structure.
#define DCDC_COULOMB_COUNTER_CONFIG_DEFAULT                              \
  {                                                                      \
    0x8000,                         /* Coulomb Counter EM0 Threshold. */ \
    0x8000,                         /* Coulomb Counter EM2 Threshold. */ \
  }

/// Suggested default values for DCDC_COULOMB_COUNTER calibration configuration structure.
#define DCDC_COULOMB_COUNTER_CALIBRATION_CONFIG_DEFAULT                                              \
  {                                                                                                  \
    cmuSelect_HFXO,                             /* Coulomb Counter Calibration Reference Clock.   */ \
    8,                                          /* Coulomb Counter Calibration Reference Count.   */ \
    SL_HAL_DCDC_COULOMB_COUNTER_EM0,            /* Coulomb Counter Calibration DC-DC energy mode. */ \
    SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD3,      /* Coulomb Counter Calibration Load.              */ \
  }

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function sets up the DCDC Coulomb Counter module using the
 * provided configuration structure. It should be called before enabling
 * or starting the Coulomb Counter to ensure that the module is
 * configured correctly. The function will disable the Coulomb Counter if
 * it is currently enabled, apply the new configuration, and prepare the
 * module for operation. It is important to ensure that the configuration
 * structure is properly initialized before passing it to this function.
 *
 * @param p_config A pointer to a sl_hal_dcdc_coulomb_counter_config_t structure
 * containing the desired configuration for the Coulomb Counter.
 * This structure must be initialized with valid threshold
 * values for energy modes EM0 and EM2. The pointer must not be
 * null, and the caller retains ownership of the memory.
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_init(const sl_hal_dcdc_coulomb_counter_config_t *p_config);

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_enable` enables the DCDC
 * Coulomb Counter by synchronizing the DCDC module and setting the
 * control register.
 *
 * @return The function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_enable(void)
{
  EMU_DCDCSync(_DCDC_SYNCBUSY_MASK);
  DCDC->CCCTRL_SET = DCDC_CCCTRL_CCEN;
}

/***************************************************************************//**
 * @brief Use this function to disable the DCDC Coulomb Counter module when it
 * is no longer needed or before reconfiguring it. It ensures that the
 * counter is stopped if it is currently running before disabling the
 * module. This function should be called only when the DCDC Coulomb
 * Counter is enabled, as it will have no effect if the module is already
 * disabled. It is important to ensure that any necessary data has been
 * collected or processed before calling this function, as disabling the
 * module will stop any ongoing counting operations.
 *
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_disable(void);

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_wait_start` waits for the
 * DCDC Coulomb Counter to start by checking the status register until
 * the counter is running.
 *
 * @return The function does not return any value; it simply waits until the
 * Coulomb Counter is confirmed to be running.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_wait_start(void)
{
  EMU_DCDCSync(_DCDC_SYNCBUSY_MASK);
  while ((DCDC->CCSTATUS & _DCDC_CCSTATUS_CCRUNNING_MASK) == 0U) {
    /* Wait for counters to start. */
  }
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_wait_stop` waits for the
 * DCDC Coulomb Counter to stop running by checking its status register.
 *
 * @return The function does not return any value; it simply waits until the
 * Coulomb Counter has stopped running.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_wait_stop(void)
{
  EMU_DCDCSync(_DCDC_SYNCBUSY_MASK);
  while ((DCDC->CCSTATUS & _DCDC_CCSTATUS_CCRUNNING_MASK) == DCDC_CCSTATUS_CCRUNNING) {
    /* Wait for counters to stop. */
  }
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_wait_clear_counters` waits
 * for the DCDC Coulomb Counter to complete the clear command by checking
 * the status register until the clear operation is no longer busy.
 *
 * @return The function does not return any value; it simply waits until the
 * clear operation is complete.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_wait_clear_counters(void)
{
  EMU_DCDCSync(_DCDC_SYNCBUSY_MASK);
  while ((DCDC->CCSTATUS & _DCDC_CCSTATUS_CLRBSY_MASK) == DCDC_CCSTATUS_CLRBSY) {
    /* Wait for counters to clear. */
  }
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_start` initiates the
 * operation of the DCDC Coulomb Counter by sending a start command to
 * the peripheral.
 *
 * @return The function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_start(void)
{
  EMU_DCDCSync(_DCDC_SYNCBUSY_MASK);
  DCDC->CCCMD_SET = DCDC_CCCMD_START;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_stop` stops the operation of
 * the DCDC Coulomb Counter by issuing a stop command to the peripheral.
 *
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_stop(void)
{
  EMU_DCDCSync(_DCDC_SYNCBUSY_MASK);
  DCDC->CCCMD_SET = DCDC_CCCMD_STOP;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_clear_counters` clears the
 * counters of the DCDC Coulomb Counter peripheral.
 *
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_clear_counters(void)
{
  EMU_DCDCSync(_DCDC_SYNCBUSY_MASK);
  DCDC->CCCMD_SET = DCDC_CCCMD_CLR;
}

/***************************************************************************//**
 * @brief This function is used to obtain the current count from the DCDC
 * Coulomb Counter for a specified energy mode. It is useful for
 * monitoring energy consumption in different modes of operation. The
 * function requires the energy mode to be specified, which determines
 * which counter value is returned. It is important to ensure that the
 * DCDC Coulomb Counter module is properly initialized and enabled before
 * calling this function to get accurate results.
 *
 * @param emode Specifies the energy mode for which the Coulomb counter value is
 * requested. It must be one of the defined values in the
 * `sl_hal_dcdc_coulomb_counter_emode_t` enumeration, such as
 * `SL_HAL_DCDC_COULOMB_COUNTER_EM0` or
 * `SL_HAL_DCDC_COULOMB_COUNTER_EM2`. Invalid values may lead to
 * undefined behavior.
 * @return Returns a `uint32_t` representing the Coulomb counter value for the
 * specified energy mode.
 ******************************************************************************/
uint32_t sl_hal_dcdc_coulomb_counter_get_count(sl_hal_dcdc_coulomb_counter_emode_t emode);

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_get_status` retrieves the
 * current status of the DCDC Coulomb Counter by returning the value of
 * the `CCSTATUS` register.
 *
 * @return The function returns a `uint32_t` value representing the current
 * status of the DCDC Coulomb Counter as stored in the `CCSTATUS`
 * register.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_dcdc_coulomb_counter_get_status(void)
{
  return DCDC->CCSTATUS;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_enable_interrupts` enables
 * specified interrupts for the DCDC Coulomb Counter by setting the
 * corresponding bits in the interrupt enable register.
 *
 * @param flags A 32-bit unsigned integer representing the interrupt sources to
 * enable, where each bit corresponds to a specific interrupt
 * source.
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_enable_interrupts(uint32_t flags)
{
  DCDC->CCIEN_SET = flags;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_disable_interrupts` disables
 * specified interrupts for the DCDC Coulomb Counter by clearing the
 * corresponding bits in the interrupt enable register.
 *
 * @param flags A 32-bit unsigned integer representing the interrupt sources to
 * disable, where each bit corresponds to a specific interrupt.
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_disable_interrupts(uint32_t flags)
{
  DCDC->CCIEN_CLR = flags;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_clear_interrupts` clears
 * specified interrupt flags for the DCDC Coulomb Counter peripheral.
 *
 * @param flags A 32-bit unsigned integer representing the interrupt flags to be
 * cleared; multiple flags can be combined using a bitwise OR
 * operation.
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_clear_interrupts(uint32_t flags)
{
  DCDC->CCIF_CLR = flags;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_get_interrupts` retrieves
 * the current pending interrupt flags for the DCDC Coulomb Counter.
 *
 * @return The function returns a `uint32_t` value representing the pending
 * interrupt flags for the DCDC Coulomb Counter.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_dcdc_coulomb_counter_get_interrupts(void)
{
  return DCDC->CCIF;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_get_enabled_interrupts`
 * retrieves the currently enabled and pending interrupt flags for the
 * DCDC Coulomb Counter.
 *
 * @return The function returns a `uint32_t` value representing the bitwise AND
 * of the enabled and pending interrupt flags for the DCDC Coulomb
 * Counter.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_dcdc_coulomb_counter_get_enabled_interrupts(void)
{
  uint32_t ccif = DCDC->CCIF & _DCDC_CCIF_MASK;
  uint32_t ccien = DCDC->CCIEN & _DCDC_CCIEN_MASK;

  return ccif & ccien;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_set_interrupts` sets one or
 * more pending DCDC Coulomb Counter interrupts by writing the specified
 * flags to the CCIF_SET register.
 *
 * @param flags A 32-bit unsigned integer representing the interrupt sources to
 * set as pending, typically a combination of interrupt flags OR-ed
 * together.
 * @return The function does not return any value; it performs a hardware
 * register write operation to set interrupts.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_set_interrupts(uint32_t flags)
{
  DCDC->CCIF_SET = flags;
}

/***************************************************************************//**
 * @brief This function sets up the calibration process for the DCDC Coulomb
 * Counter using the provided configuration. It configures the necessary
 * clock and PRS channels, sets up the calibration circuit, and enables
 * the calibration load. This function should be called when calibration
 * of the Coulomb Counter is required, typically during initialization or
 * when recalibration is necessary. It assumes that the DCDC Coulomb
 * Counter module is already enabled and that the provided configuration
 * is valid. The function will assert if no free PRS channel is
 * available, indicating a critical setup failure.
 *
 * @param config A structure of type
 * sl_hal_dcdc_coulomb_counter_calibration_config_t containing the
 * calibration configuration. This includes the reference clock,
 * calibration count, energy mode, and load level. The caller must
 * ensure that this structure is correctly populated before
 * calling the function.
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_cal_init(sl_hal_dcdc_coulomb_counter_calibration_config_t config);

/***************************************************************************//**
 * @brief Use this function to initiate the calibration process of the DCDC
 * Coulomb Counter. It is typically called after configuring the
 * calibration settings using the appropriate configuration structure.
 * This function does not return any status, so it is assumed that the
 * calibration process starts successfully if the preconditions are met.
 * Ensure that the DCDC Coulomb Counter is properly initialized and
 * enabled before calling this function.
 *
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_cal_start(void);

/***************************************************************************//**
 * @brief This function is used to stop the ongoing calibration sequence of the
 * DCDC Coulomb Counter. It should be called when the calibration process
 * needs to be halted, ensuring that the calibration load is also
 * disabled as a part of this operation. This function is typically used
 * in scenarios where calibration is no longer needed or needs to be
 * paused. It is important to ensure that the calibration process is
 * active before calling this function to avoid unnecessary operations.
 *
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_cal_stop(void);

/***************************************************************************//**
 * @brief This function is used to enable the calibration load in the DCDC
 * Coulomb Counter peripheral. It should be called when calibration of
 * the Coulomb Counter is required, allowing the system to use known on-
 * chip calibration loads for accurate measurement. This function does
 * not require any parameters and does not return any value. It is
 * typically used in conjunction with other calibration functions to
 * ensure the Coulomb Counter is properly calibrated for accurate energy
 * measurement.
 *
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_enable_cal_load(void);

/***************************************************************************//**
 * @brief Use this function to disable the calibration load of the DCDC Coulomb
 * Counter when calibration is not needed or after calibration is
 * complete. This function is typically called to conserve power or to
 * reset the calibration state. It should be used in conjunction with
 * other calibration functions to manage the calibration process
 * effectively. Ensure that the DCDC Coulomb Counter is properly
 * initialized and enabled before calling this function.
 *
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_disable_cal_load(void);

/***************************************************************************//**
 * @brief This function configures the calibration load level for the DCDC
 * Coulomb Counter based on the specified energy mode. It should be used
 * when adjusting the load current for calibration purposes. The function
 * must be called with valid energy mode and load level parameters to
 * ensure proper operation. It synchronizes the DCDC settings and adjusts
 * the DCDC configuration specifically for EM2 mode if required.
 *
 * @param emode Specifies the energy mode for which the calibration load level
 * is being set. Must be a valid value of type
 * sl_hal_dcdc_coulomb_counter_emode_t, such as
 * SL_HAL_DCDC_COULOMB_COUNTER_EM0 or
 * SL_HAL_DCDC_COULOMB_COUNTER_EM2.
 * @param load_level Indicates the desired calibration load level. Must be a
 * valid value of type
 * sl_hal_dcdc_coulomb_counter_calibration_load_level_t, such
 * as SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD0 through
 * SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD7.
 * @return None
 ******************************************************************************/
void sl_hal_dcdc_coulomb_counter_set_cal_load_level(sl_hal_dcdc_coulomb_counter_emode_t emode,
                                                    sl_hal_dcdc_coulomb_counter_calibration_load_level_t load_level);

/***************************************************************************//**
 * @brief This function is used to obtain the calibration load current value for
 * a given load level, which is stored in the device information
 * (DEVINFO) during production testing. It is useful for applications
 * that need to adjust or verify the load current settings of the DCDC
 * Coulomb Counter. The function should be called with a valid load level
 * enumeration value, and it asserts if the load level is not recognized
 * or if the calibration load setting was not measured during production
 * testing.
 *
 * @param load_level An enumeration value of type
 * sl_hal_dcdc_coulomb_counter_calibration_load_level_t
 * representing the desired calibration load level. Valid
 * values range from SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD0 to
 * SL_HAL_DCDC_COULOMB_COUNTER_CAL_LOAD7. The function asserts
 * if an invalid load level is provided.
 * @return Returns a uint16_t representing the calibration load current for the
 * specified load level. The value can be converted to microamperes by
 * dividing by 5, with each LSB representing 200 nA.
 ******************************************************************************/
uint16_t sl_hal_dcdc_coulomb_counter_get_cal_load_current(sl_hal_dcdc_coulomb_counter_calibration_load_level_t load_level);

/***************************************************************************//**
 * @brief This function returns the frequency in Hertz of the currently selected
 * clock source for the CMU Calibration Up-Counter. It is useful for
 * applications that need to know the reference frequency used in
 * calibration processes. The function checks the current configuration
 * of the CMU's calibration control register to determine which clock
 * source is selected and retrieves the corresponding frequency. It
 * should be called when the calibration source is configured, and the
 * caller should handle cases where the source is disabled.
 *
 * @return The function returns a uint32_t representing the frequency in Hertz
 * of the selected CMU Calibration Up-Counter clock source. If the
 * source is disabled, the behavior is undefined.
 ******************************************************************************/
uint32_t sl_hal_dcdc_coulomb_counter_get_cal_reference_freq(void);

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_set_calhalt` sets the
 * calibration halt flag for the DCDC Coulomb Counter.
 *
 * @return The function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_dcdc_coulomb_counter_set_calhalt(void)
{
  DCDC->CCCALCTRL_SET = DCDC_CCCALCTRL_CCCALHALT;
}

/***************************************************************************//**
 * @brief The function `sl_hal_dcdc_coulomb_counter_calhalt_is_set` checks if
 * the calibration halt flag is set in the DCDC Coulomb Counter's control
 * register.
 *
 * @return The function returns `true` if the calibration halt flag is set,
 * otherwise it returns `false`.
 ******************************************************************************/
__STATIC_INLINE bool sl_hal_dcdc_coulomb_counter_calhalt_is_set(void)
{
  return (DCDC->CCCALCTRL & _DCDC_CCCALCTRL_CCCALHALT_MASK) == DCDC_CCCALCTRL_CCCALHALT;
}

/** @} (end addtogroup dcdccoulombcounter) */

#ifdef __cplusplus
}
#endif

#endif /* defined(DCDC_COULOMB_COUNTER_COUNT) && (DCDC_COULOMB_COUNTER_COUNT > 0) && defined(DCDC_CCCTRL_CCEN) */
#endif /* SL_HAL_DCDC_COULOMB_COUNTER_H */
