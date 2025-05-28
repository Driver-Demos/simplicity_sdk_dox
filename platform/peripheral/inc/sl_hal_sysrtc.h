/***************************************************************************//**
 * @file
 * @brief System Real Time Counter (SYSRTC) peripheral API
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

#ifndef SL_HAL_SYSRTC_H
#define SL_HAL_SYSRTC_H

#include "em_device.h"

#if defined(SYSRTC_COUNT) && (SYSRTC_COUNT > 0)
#include <stdbool.h>
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "sl_hal_sysrtc_compat.h"
#include "sl_enum.h"

/***************************************************************************//**
 * @addtogroup sysrtc
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   DEFINES   ***********************************
 ******************************************************************************/

/// Minimum compare channels for SYSRTC group.
#define SYSRTC_GROUP_MIN_CHANNEL_COMPARE  1u
/// Maximum compare channels for SYSRTC group.
#define SYSRTC_GROUP_MAX_CHANNEL_COMPARE  2u

/// Minimum capture channels for SYSRTC group.
#define SYSRTC_GROUP_MIN_CHANNEL_CAPTURE  0u
/// Maximum capture channels for SYSRTC group.
#define SYSRTC_GROUP_MAX_CHANNEL_CAPTURE  1u

/// Sysrtc group number.
#if !defined(SYSRTC_GROUP_NUMBER)
#define SYSRTC_GROUP_NUMBER   1u
#endif

/// Validation of valid SYSRTC group for assert statements.
#define SYSRTC_GROUP_VALID(group)    ((unsigned)(group) < SYSRTC_GROUP_NUMBER)

/*******************************************************************************
 *********************************   ENUM   ************************************
 ******************************************************************************/

/// Capture input edge select.
SL_ENUM(sl_hal_sysrtc_capture_edge_t) {
  SL_HAL_SYSRTC_CAPTURE_EDGE_RISING = 0,  ///< Rising edges detected.
  SL_HAL_SYSRTC_CAPTURE_EDGE_FALLING,     ///< Falling edges detected.
  SL_HAL_SYSRTC_CAPTURE_EDGE_BOTH         ///< Both edges detected.
};

/// Compare match output action mode.
SL_ENUM(sl_hal_sysrtc_compare_match_out_action_t) {
  SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_CLEAR = 0, ///< Clear output.
  SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_SET,       ///< Set output.
  SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_PULSE,     ///< Generate a pulse.
  SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_TOGGLE,    ///< Toggle output.
  SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_CMPIF      ///< Export CMPIF.
};

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/// SYSRTC configuration structure.
/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_config_t` structure is a configuration data
 * structure for the System Real Time Counter (SYSRTC) peripheral. It
 * contains a single boolean member, `enable_debug_run`, which determines
 * if the SYSRTC counter should keep running when the system is halted in
 * a debug session. This configuration is crucial for applications that
 * require the SYSRTC to maintain accurate timekeeping even during
 * debugging.
 *
 * @param enable_debug_run Indicates whether the counter should continue running
 * during a debug halt.
 ******************************************************************************/
typedef struct {
  bool enable_debug_run;      ///< Counter shall keep running during debug halt.
} sl_hal_sysrtc_config_t;

/// Suggested default values for SYSRTC configuration structure.
#define SYSRTC_CONFIG_DEFAULT                        \
  {                                                  \
    false, /* Disable updating during debug halt. */ \
  }

/// Compare channel configuration structure.
/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_group_channel_compare_config_t` structure is used
 * to configure the behavior of a compare channel within a System Real
 * Time Counter (SYSRTC) group. It contains a single member,
 * `compare_match_out_action`, which determines the output action to be
 * taken when a compare match occurs. This configuration is crucial for
 * setting up how the SYSRTC handles compare events, allowing for actions
 * such as clearing, setting, pulsing, toggling, or exporting a compare
 * match interrupt flag (CMPIF).
 *
 * @param compare_match_out_action Specifies the action to take on a compare
 * match event for the channel.
 ******************************************************************************/
typedef struct {
  sl_hal_sysrtc_compare_match_out_action_t  compare_match_out_action; ///< Compare mode channel match output action.
} sl_hal_sysrtc_group_channel_compare_config_t;

/// Capture channel configuration structure.
/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_group_channel_capture_config_t` structure is used
 * to configure the capture channel of a SYSRTC group. It contains a
 * single member, `capture_input_edge`, which determines the edge type
 * (rising, falling, or both) that will trigger the capture event. This
 * configuration is essential for setting up the capture functionality of
 * the SYSRTC peripheral, allowing it to respond to specific signal
 * transitions.
 *
 * @param capture_input_edge Specifies the capture mode channel input edge using
 * the sl_hal_sysrtc_capture_edge_t enum.
 ******************************************************************************/
typedef struct {
  sl_hal_sysrtc_capture_edge_t  capture_input_edge; ///< Capture mode channel input edge.
} sl_hal_sysrtc_group_channel_capture_config_t;

/// Group configuration structure.
/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_group_config_t` structure is used to configure a
 * group of channels in the System Real Time Counter (SYSRTC) peripheral.
 * It includes boolean flags to enable or disable compare and capture
 * channels, as well as pointers to configuration structures for each
 * channel. This allows for flexible configuration of the SYSRTC's
 * behavior, enabling specific channels and setting their operational
 * parameters through associated configuration structures.
 *
 * @param compare_channel0_enable Enable/Disable compare channel 0.
 * @param compare_channel1_enable Enable/Disable compare channel 1.
 * @param capture_channel0_enable Enable/Disable capture channel 0.
 * @param p_compare_channel0_config Pointer to compare channel 0 config.
 * @param p_compare_channel1_config Pointer to compare channel 1 config.
 * @param p_capture_channel0_config Pointer to capture channel 0 config.
 ******************************************************************************/
typedef struct {
  bool compare_channel0_enable;                                                  ///< Enable/Disable compare channel 0
  bool compare_channel1_enable;                                                  ///< Enable/Disable compare channel 1
  bool capture_channel0_enable;                                                  ///< Enable/Disable capture channel 0
  sl_hal_sysrtc_group_channel_compare_config_t const *p_compare_channel0_config; ///< Pointer to compare channel 0 config
  sl_hal_sysrtc_group_channel_compare_config_t const *p_compare_channel1_config; ///< Pointer to compare channel 1 config
  sl_hal_sysrtc_group_channel_capture_config_t const *p_capture_channel0_config; ///< Pointer to capture channel 0 config
} sl_hal_sysrtc_group_config_t;

/// Suggested default values for compare channel configuration structure.
#define SYSRTC_GROUP_CHANNEL_COMPARE_CONFIG_DEFAULT \
  {                                                 \
    SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_PULSE    \
  }

/// Compare channel configuration for starting HFXO using PRS.
#define SYSRTC_GROUP_CHANNEL_COMPARE_CONFIG_EARLY_WAKEUP \
  {                                                      \
    SL_HAL_SYSRTC_COMPARE_MATCH_OUT_ACTION_CMPIF         \
  }

/// Suggested default values for capture channel configuration structure.
#define SYSRTC_GROUP_CHANNEL_CAPTURE_CONFIG_DEFAULT \
  {                                                 \
    SL_HAL_SYSRTC_CAPTURE_EDGE_RISING               \
  }

/// Suggested default values for SYSRTC group configuration structure.
#define SYSRTC_GROUP_CONFIG_DEFAULT                                          \
  {                                                                          \
    true, /* Enable compare channel 0. */                                    \
    false, /* Disable compare channel 1. */                                  \
    false, /* Disable capture channel 0. */                                  \
    NULL, /* NULL Pointer to configuration structure for compare channel 0*/ \
    NULL, /* NULL Pointer to configuration structure for compare channel 1*/ \
    NULL /* NULL Pointer to configuration structure for capture channel 0*/  \
  }

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function sets up the System Real Time Counter (SYSRTC) module
 * using the provided configuration structure. It should be called to
 * initialize the SYSRTC before enabling or starting it. The function
 * ensures that the SYSRTC is ready and not currently enabled before
 * applying the new configuration. It is important to configure any
 * compare values separately before using this function if the SYSRTC is
 * intended to start immediately after initialization. The function does
 * not start the SYSRTC; it only configures it.
 *
 * @param p_config A pointer to a sl_hal_sysrtc_config_t structure containing
 * the desired configuration settings for the SYSRTC. The
 * structure must be properly initialized before passing it to
 * the function. The pointer must not be null, and the caller
 * retains ownership of the memory.
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_init(const sl_hal_sysrtc_config_t *p_config);

/***************************************************************************//**
 * @brief This function is used to enable the System Real Time Counter (SYSRTC)
 * module and initiate its counting process. It should be called when the
 * SYSRTC needs to be activated, typically after any necessary
 * configuration has been completed. The function ensures that the module
 * is ready before enabling it, which may involve waiting for any ongoing
 * disabling operations to complete. This function does not require any
 * parameters and does not return any values. It is important to ensure
 * that the SYSRTC is properly configured before calling this function to
 * avoid unexpected behavior.
 *
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_enable(void);

/***************************************************************************//**
 * @brief This function is used to disable the System Real Time Counter (SYSRTC)
 * peripheral. It should be called when the SYSRTC is no longer needed or
 * before reconfiguring it. The function first checks if the SYSRTC is
 * enabled; if it is not, the function returns immediately without making
 * any changes. If the SYSRTC is enabled, it stops the counter and then
 * disables the module. This function should be used in contexts where it
 * is safe to stop the SYSRTC, as it will halt any ongoing timekeeping
 * operations.
 *
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_disable(void);

/***************************************************************************//**
 * Waits for the SYSRTC to complete all synchronization of register changes
 * and commands.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_wait_sync` function waits for the System Real Time
 * Counter (SYSRTC) to complete all pending synchronizations of register
 * changes and commands.
 *
 * @return The function does not return any value; it simply ensures that the
 * SYSRTC is synchronized before proceeding.
 ******************************************************************************/
__INLINE void sl_hal_sysrtc_wait_sync(void)
{
  while ((SYSRTC0->EN & SYSRTC_EN_EN) && (SYSRTC0->SYNCBUSY != 0U)) {
    // Wait for all synchronizations to finish
  }
}

/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_wait_ready` function waits for the System Real Time
 * Counter (SYSRTC) to complete any ongoing reset, disable, or
 * synchronization operations before proceeding.
 *
 * @return The function does not return any value; it simply ensures that the
 * SYSRTC is ready before allowing further operations.
 ******************************************************************************/
__INLINE void sl_hal_sysrtc_wait_ready(void)
{
  while ((SYSRTC0->SWRST & _SYSRTC_SWRST_RESETTING_MASK) || (SYSRTC0->EN & _SYSRTC_EN_DISABLING_MASK) || (SYSRTC0->SYNCBUSY != 0U)) {
    // Wait for all synchronizations to finish
  }
}

/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_start` function initiates the start of the System
 * Real Time Counter (SYSRTC) by issuing a start command after ensuring
 * synchronization.
 *
 * @return The function does not return any value; it performs an action to
 * start the SYSRTC.
 ******************************************************************************/
__INLINE void sl_hal_sysrtc_start(void)
{
  sl_hal_sysrtc_wait_sync();
  SYSRTC0->CMD = SYSRTC_CMD_START;
}

/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_stop` function stops the System Real Time Counter
 * (SYSRTC) by issuing a stop command after ensuring synchronization.
 *
 * @return The function does not return any value; it performs an action to stop
 * the SYSRTC.
 ******************************************************************************/
__INLINE void sl_hal_sysrtc_stop(void)
{
  sl_hal_sysrtc_wait_sync();
  SYSRTC0->CMD = SYSRTC_CMD_STOP;
}

/***************************************************************************//**
 * @brief Use this function to reset the System Real Time Counter (SYSRTC) to
 * its initial state. This is typically done to clear any configurations
 * or states that have been set during operation. It is important to
 * ensure that any necessary data is saved or processed before calling
 * this function, as it will reset the counter and any associated
 * settings. This function does not require any parameters and does not
 * return any values.
 *
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_reset(void);

/***************************************************************************//**
 * @brief The function `sl_hal_sysrtc_get_status` retrieves the current status
 * of the SYSRTC peripheral by returning the value of its STATUS
 * register.
 *
 * @return The function returns a `uint32_t` value representing the current
 * status of the SYSRTC peripheral as stored in the STATUS register.
 ******************************************************************************/
__INLINE uint32_t sl_hal_sysrtc_get_status(void)
{
  return SYSRTC0->STATUS;
}

/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_lock` function locks the SYSRTC registers to
 * prevent any modifications.
 *
 * @return The function does not return any value.
 ******************************************************************************/
__INLINE void sl_hal_sysrtc_lock(void)
{
  SYSRTC0->LOCK = ~SYSRTC_LOCK_LOCKKEY_UNLOCK;
}

/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_unlock` function unlocks the SYSRTC registers to
 * allow writing to them.
 *
 * @return The function does not return any value.
 ******************************************************************************/
__INLINE void sl_hal_sysrtc_unlock(void)
{
  SYSRTC0->LOCK = SYSRTC_LOCK_LOCKKEY_UNLOCK;
}

/***************************************************************************//**
 * Gets SYSRTC counter value.
 *
 * @return  Current SYSRTC counter value.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_get_counter` function retrieves the current value
 * of the System Real Time Counter (SYSRTC) after ensuring
 * synchronization.
 *
 * @return Returns the current value of the SYSRTC counter as a `uint32_t`.
 ******************************************************************************/
__INLINE uint32_t sl_hal_sysrtc_get_counter(void)
{
  // Wait for Counter to synchronize before getting value
  sl_hal_sysrtc_wait_sync();

  return SYSRTC0->CNT;
}

/***************************************************************************//**
 * @brief The `sl_hal_sysrtc_set_counter` function sets the SYSRTC counter to a
 * specified value after ensuring synchronization.
 *
 * @param value A 32-bit unsigned integer representing the new value to set the
 * SYSRTC counter to.
 * @return This function does not return a value; it performs an action by
 * setting the SYSRTC counter.
 ******************************************************************************/
__INLINE void sl_hal_sysrtc_set_counter(uint32_t value)
{
  // Wait for Counter to synchronize before getting value
  sl_hal_sysrtc_wait_sync();

  SYSRTC0->CNT = value;
}

/***************************************************************************//**
 * @brief This function is used to configure a specific System Real Time Counter
 * (SYSRTC) group with the settings provided in the configuration
 * structure. It must be called with a valid group number and a properly
 * initialized configuration structure. The function enables and
 * configures compare and capture channels based on the provided
 * configuration. It is essential to ensure that the group number is
 * within the valid range, as defined by the system, to avoid assertion
 * failures. This function does not start the SYSRTC; it only configures
 * the specified group.
 *
 * @param group_number The SYSRTC group number to configure. Must be a valid
 * group number as defined by the system. Invalid group
 * numbers will trigger an assertion failure.
 * @param p_group_config Pointer to a sl_hal_sysrtc_group_config_t structure
 * containing the configuration for the group. Must not be
 * null, and the structure should be properly initialized
 * with desired settings for compare and capture channels.
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_init_group(uint8_t group_number,
                              sl_hal_sysrtc_group_config_t const *p_group_config);

/***************************************************************************//**
 * Enables one or more SYSRTC interrupts for the given group.
 *
 * @note  Depending on the use, a pending interrupt may already be set prior to
 *        enabling the interrupt. To ignore a pending interrupt, consider using
 *        sl_hal_sysrtc_clear_group_interrupts() prior to enabling the interrupt.
 *
 * @param[in] group_number  SYSRTC group number to use.
 *
 * @param[in] flags   SYSRTC interrupt sources to enable.
 *                    Use a set of interrupt flags OR-ed together to set
 *                    multiple interrupt sources for the given SYSRTC group.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to enable one or more interrupt sources for a
 * specified System Real Time Counter (SYSRTC) group. It is important to
 * ensure that the group number is valid and within the range supported
 * by the system. The function should be called when you want to start
 * receiving interrupts for the specified flags in the given group. It is
 * advisable to clear any pending interrupts using
 * sl_hal_sysrtc_clear_group_interrupts() before enabling new interrupts
 * to avoid handling unintended pending interrupts.
 *
 * @param group_number The SYSRTC group number for which interrupts are to be
 * enabled. It must be a valid group number, less than the
 * defined SYSRTC_GROUP_NUMBER. If an invalid group number
 * is provided, the function will assert.
 * @param flags A bitmask representing the interrupt sources to enable. Multiple
 * interrupt sources can be enabled by OR-ing their respective
 * flags together. The flags should correspond to valid interrupt
 * sources for the specified group.
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_enable_group_interrupts(uint8_t group_number,
                                           uint32_t flags);

/***************************************************************************//**
 * Disables one or more SYSRTC interrupts for the given group.
 *
 * @param[in] group_number  SYSRTC group number to use.
 *
 * @param[in] flags   SYSRTC interrupt sources to disable.
 *                    Use a set of interrupt flags OR-ed together to disable
 *                    multiple interrupt sources for the given SYSRTC group.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to disable one or more interrupts for a specified
 * SYSRTC group. It is essential to ensure that the group number is
 * valid, which is determined by the defined SYSRTC_GROUP_NUMBER. This
 * function is typically used when you want to prevent certain interrupts
 * from triggering, perhaps during a critical section of code or when the
 * interrupts are no longer needed. The function does not return a value,
 * and it is expected that the group number is within the valid range;
 * otherwise, an assertion will be triggered.
 *
 * @param group_number The SYSRTC group number for which interrupts are to be
 * disabled. Must be a valid group number less than
 * SYSRTC_GROUP_NUMBER. An assertion is triggered if the
 * group number is invalid.
 * @param flags A bitmask representing the interrupt sources to disable.
 * Multiple interrupt sources can be specified by OR-ing their
 * respective flags together. The function will clear these flags
 * in the interrupt enable register for the specified group.
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_disable_group_interrupts(uint8_t group_number,
                                            uint32_t flags);

/***************************************************************************//**
 * Clears one or more pending SYSRTC interrupts for the given group.
 *
 * @param[in] group_number  SYSRTC group number to use.
 *
 * @param[in] flags   SYSRTC interrupt sources to clear.
 *                    Use a set of interrupt flags OR-ed together to clear
 *                    multiple interrupt sources for the given SYSRTC group.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to clear one or more pending interrupts for a
 * specific SYSRTC group by providing the group number and the interrupt
 * flags to clear. This function is typically used to reset the interrupt
 * status after handling an interrupt event. It is important to ensure
 * that the group number is valid, as defined by the system
 * configuration, to avoid assertion failures. The function does not
 * return any value and does not modify the input parameters.
 *
 * @param group_number The SYSRTC group number for which to clear interrupts.
 * Must be a valid group number as per the system
 * configuration; otherwise, an assertion will fail.
 * @param flags A bitmask representing the interrupt sources to clear. Multiple
 * interrupt sources can be specified by OR-ing their respective
 * flags together.
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_clear_group_interrupts(uint8_t group_number,
                                          uint32_t flags);

/***************************************************************************//**
 * Gets pending SYSRTC interrupt flags for the given group.
 *
 * @note  Event bits are not cleared by using this function.
 *
 * @param[in] group_number  SYSRTC group number to use.
 *
 * @return  Pending SYSRTC interrupt sources.
 *          Returns a set of interrupt flags OR-ed together for multiple
 *          interrupt sources in the SYSRTC group.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to obtain the current pending interrupt flags for a
 * specific SYSRTC group. This is useful for determining which interrupts
 * have been triggered but not yet handled. The function should be called
 * with a valid group number, which must be less than the defined
 * SYSRTC_GROUP_NUMBER. If an invalid group number is provided, the
 * behavior is undefined. This function does not clear the interrupt
 * flags; it only retrieves their current state.
 *
 * @param group_number The SYSRTC group number for which to retrieve pending
 * interrupt flags. Must be a valid group number less than
 * SYSRTC_GROUP_NUMBER. Providing an invalid group number
 * results in undefined behavior.
 * @return Returns a uint32_t value representing the pending interrupt flags for
 * the specified SYSRTC group. The flags are returned as a bitwise OR of
 * all pending interrupt sources for the group.
 ******************************************************************************/
uint32_t sl_hal_sysrtc_get_group_interrupts(uint8_t group_number);

/***************************************************************************//**
 * @brief This function is used to obtain the currently enabled and pending
 * interrupt flags for a specified System Real Time Counter (SYSRTC)
 * group. It is useful for determining which interrupts are both enabled
 * and pending, allowing for efficient interrupt handling. The function
 * should be called with a valid group number, which must be less than
 * the defined SYSRTC_GROUP_NUMBER. If an invalid group number is
 * provided, the behavior is undefined. This function does not clear any
 * interrupt flags.
 *
 * @param group_number The SYSRTC group number for which to retrieve the enabled
 * and pending interrupts. Must be a valid group number less
 * than SYSRTC_GROUP_NUMBER. Invalid group numbers result in
 * undefined behavior.
 * @return Returns a uint32_t value representing the bitwise AND of the enabled
 * interrupt sources and the pending interrupt flags for the specified
 * SYSRTC group.
 ******************************************************************************/
uint32_t sl_hal_sysrtc_get_group_enabled_interrupts(uint8_t group_number);

/***************************************************************************//**
 * @brief This function is used to set one or more pending interrupts for a
 * specified SYSRTC group. It is typically called when you need to
 * simulate an interrupt condition or trigger an interrupt handler
 * manually. The function requires a valid group number, which must be
 * less than the defined SYSRTC_GROUP_NUMBER. The flags parameter
 * specifies the interrupt sources to be set, and multiple interrupt
 * sources can be combined using a bitwise OR operation. The function
 * does not return a value and does not handle invalid group numbers
 * beyond asserting.
 *
 * @param group_number The SYSRTC group number for which interrupts are to be
 * set. Must be a valid group number less than
 * SYSRTC_GROUP_NUMBER. If the group number is invalid, the
 * function will assert.
 * @param flags A bitmask representing the interrupt sources to set as pending.
 * Multiple interrupt sources can be combined using a bitwise OR
 * operation.
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_set_group_interrupts(uint8_t group_number,
                                        uint32_t flags);

/***************************************************************************//**
 * Gets SYSRTC compare register value for selected channel of given group.
 *
 * @param[in] group_number  SYSRTC group number to use.
 *
 * @param[in] channel   Channel selector.
 *
 * @return  Compare register value.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to obtain the current compare register value for a
 * specific channel within a given SYSRTC group. This is useful for
 * monitoring or debugging purposes when you need to know the current
 * compare value set in the hardware. Ensure that the group number is
 * valid and within the range of available groups as defined by the
 * system configuration. The function asserts if the group number or
 * channel is invalid, which means it should be used in environments
 * where such conditions are guaranteed to be met.
 *
 * @param group_number The SYSRTC group number to query. Must be a valid group
 * number as defined by the system configuration. Invalid
 * group numbers will trigger an assertion.
 * @param channel The channel within the specified group to query. Typically,
 * this should be 0 or 1, depending on the system configuration.
 * Invalid channel numbers will trigger an assertion.
 * @return Returns the current compare register value for the specified group
 * and channel as a 32-bit unsigned integer.
 ******************************************************************************/
uint32_t sl_hal_sysrtc_get_group_compare_channel_value(uint8_t group_number,
                                                       uint8_t channel);

/***************************************************************************//**
 * Sets SYSRTC compare register value for selected channel of given group.
 *
 * @param[in] group_number  SYSRTC group number to use.
 *
 * @param[in] channel   Channel selector.
 *
 * @param[in] value   Compare register value.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to set the compare register value for a specific
 * channel within a given SYSRTC group. It is essential to ensure that
 * the group number is valid and within the defined range of available
 * groups. The function should be called when you need to update the
 * compare value for timing or event generation purposes. It is important
 * to note that the function will assert if the group number or channel
 * is invalid, so proper validation should be performed before calling
 * this function.
 *
 * @param group_number The SYSRTC group number to use. Must be a valid group
 * number as defined by the system configuration. Invalid
 * group numbers will trigger an assertion.
 * @param channel The channel selector within the specified group. Valid values
 * are typically 0 or 1, depending on the system configuration.
 * Invalid channel numbers will trigger an assertion.
 * @param value The compare register value to set. This is a 32-bit unsigned
 * integer representing the desired compare value for the specified
 * channel.
 * @return None
 ******************************************************************************/
void sl_hal_sysrtc_set_group_compare_channel_value(uint8_t group_number,
                                                   uint8_t channel,
                                                   uint32_t value);

/***************************************************************************//**
 * Gets SYSRTC input capture register value for capture channel of given group.
 *
 * @param[in] group_number  SYSRTC group number to use.
 *
 * @return  Capture register value.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_SYSRTC, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the current value from the capture
 * channel of a specified SYSRTC group. It is essential to ensure that
 * the group number provided is valid and within the range of available
 * groups as defined by the system configuration. The function will
 * assert if an invalid group number is passed, ensuring that only valid
 * group numbers are processed. This function is typically used in
 * applications where precise timing or event capture is required, and
 * the capture channel value needs to be read for further processing or
 * decision-making.
 *
 * @param group_number The index of the SYSRTC group from which to retrieve the
 * capture channel value. Must be a valid group number as
 * defined by the system configuration. If an invalid group
 * number is provided, the function will assert, preventing
 * further execution.
 * @return Returns the current value of the capture channel for the specified
 * SYSRTC group as a 32-bit unsigned integer.
 ******************************************************************************/
uint32_t sl_hal_sysrtc_get_group_capture_channel_value(uint8_t group_number);

/** @} (end addtogroup sysrtc) */

#ifdef __cplusplus
}
#endif

#endif /* defined(SYSRTC_COUNT) && (SYSRTC_COUNT > 0) */
#endif /* SL_HAL_SYSRTC_H */
