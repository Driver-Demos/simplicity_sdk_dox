/*******************************************************************************
 * @file
 * @brief Cycle counter driver interface.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_CYCLE_COUNTER_H
#define SL_CYCLE_COUNTER_H

#include "em_device.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup cycle_counter Cycle Counter
 * @brief Provides an interface to the cycle counter of the DWT unit.
 *
 * @details
 * ## Initialization
 *
 *   The cycle counter must be enabled prior to any call to its API.
 *   If sl_system is used, only sl_system_init() must be called, otherwise
 *   sl_cycle_counter_enable() must be called manually.
 *
 * ## Limitations
 *
 *   The cycle counter is stored in an unsigned 32-bit integer. As the counter
 *   frequently rolls over, timing durations should be kept short.
 *
 * ## Usage Notes
 *
 *   When entering sleep in Energy Mode 1 (EM1), the DWT cycle counter continues
 *   to increment. The cycle counter is used by the emlib CORE component to
 *   measure the interrupt maximum disable time, when this feature is enabled.
 *   Since the power manager must enter sleep with PRIMASK set to 1, it will
 *   pause the cycle counter before entering EM1 and resume it on wakeup, in
 *   order to provide a realistic interrupt disable time. If this behaviour is
 *   not desired, the maximum interrupt disable feature must be disabled in the
 *   CORE component.
 *
 * ## Usage example
 * ```
 * #include "sl_cycle_counter.h"
 *
 * void main(void)
 * {
 *   sl_cycle_counter_handle_t counter;
 *
 *   // Enable the cycle counter; not needed if sl_system_init() is used.
 *   sl_cycle_counter_enable();
 *
 *   status = sl_cycle_counter_init(&counter);
 *   if (status != SL_STATUS_OK) {
 *      // handle error
 *   }
 *
 *   sl_cycle_counter_start(&counter);
 *   function_to_measure();
 *   sl_cycle_counter_stop(&counter);
 *   printf("Timing: %lu\r\n", sl_cycle_counter_get_cycles(&counter));
 *
 * }
 * ```
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   STRUCTS   **********************************
 ******************************************************************************/

/// A Cycle Counter Instance
/***************************************************************************//**
 * @brief The `sl_cycle_counter_handle_t` is a structure used to manage and
 * record cycle counts in a system, providing a way to track the number
 * of cycles elapsed during a specific operation or period. It contains
 * three members: `start`, which holds the cycle counter value at the
 * beginning of a recording; `cycles`, which stores the number of cycles
 * that have elapsed since the last recording started; and `max`, which
 * keeps track of the maximum number of cycles recorded since the last
 * reset or initialization. This structure is part of a cycle counter
 * driver interface that allows for precise timing and performance
 * measurements in embedded systems.
 *
 * @param start Cycle counter at start of recording.
 * @param cycles Cycles elapsed in last recording.
 * @param max Max recorded cycles since last reset or init.
 ******************************************************************************/
typedef struct {
  uint32_t start;    ///< Cycle counter at start of recording
  uint32_t cycles;   ///< Cycles elapsed in last recording
  uint32_t max;      ///< Max recorded cycles since last reset or init
} sl_cycle_counter_handle_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function enables the cycle counter of the Data Watchpoint and
 * Trace (DWT) unit, allowing it to start counting processor cycles. It
 * must be called before using any other cycle counter API functions
 * unless the system initialization function `sl_system_init()` is used,
 * which automatically enables the cycle counter. This function is
 * essential for applications that require precise timing measurements,
 * such as performance profiling or benchmarking. It should be used with
 * caution in environments where the cycle counter's continuous operation
 * might affect power consumption or other system behaviors.
 *
 * @return None
 ******************************************************************************/
void sl_cycle_counter_enable(void);

/***************************************************************************//**
 * @brief The `sl_cycle_counter_pause` function pauses the DWT cycle counter by
 * disabling its enable bit in the control register.
 *
 * @return This function does not return any value.
 ******************************************************************************/
__INLINE void sl_cycle_counter_pause(void)
{
  DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
}

/***************************************************************************//**
 * @brief The `sl_cycle_counter_resume` function resumes the cycle counter by
 * enabling the cycle counter bit in the DWT control register.
 *
 * @return The function does not return any value.
 ******************************************************************************/
__INLINE void sl_cycle_counter_resume(void)
{
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

/***************************************************************************//**
 * @brief This function initializes a cycle counter handle, preparing it for use
 * in cycle counting operations. It must be called before any other
 * operations on the handle, such as starting or stopping the cycle
 * counter. The function sets the initial values of the handle's fields
 * to zero, ensuring a clean state. It is important to ensure that the
 * handle pointer is not null before calling this function, as passing a
 * null pointer will result in an error.
 *
 * @param handle Pointer to a sl_cycle_counter_handle_t structure that will be
 * initialized. Must not be null. The caller retains ownership of
 * the memory.
 * @return Returns SL_STATUS_OK if the handle is successfully initialized, or
 * SL_STATUS_NULL_POINTER if the handle is null.
 ******************************************************************************/
sl_status_t sl_cycle_counter_init(sl_cycle_counter_handle_t *handle);

/***************************************************************************//**
 * @brief This function is used to begin a new cycle count recording by
 * capturing the current value of the cycle counter into the provided
 * handle. It should be called after the cycle counter has been enabled
 * and the handle has been initialized. This function is typically used
 * in performance measurement scenarios where the number of cycles taken
 * by a block of code needs to be determined. Ensure that the handle is
 * properly initialized before calling this function to avoid undefined
 * behavior.
 *
 * @param handle Pointer to an initialized sl_cycle_counter_handle_t structure.
 * Must not be null. The function will store the current cycle
 * count in the 'start' field of this structure.
 * @return None
 ******************************************************************************/
void sl_cycle_counter_start(sl_cycle_counter_handle_t *handle);

/***************************************************************************//**
 * @brief This function is used to stop the cycle counting process that was
 * previously started with a call to the start function. It calculates
 * the number of cycles that have elapsed since the start and updates the
 * provided handle with this value. Additionally, it updates the maximum
 * recorded cycles if the current elapsed cycles exceed the previous
 * maximum. This function should be called after the cycle counter has
 * been started and the operation to be measured has completed. The
 * handle must be properly initialized before calling this function.
 *
 * @param handle A pointer to an initialized sl_cycle_counter_handle_t
 * structure. This parameter must not be null, and the structure
 * should have been initialized and started prior to calling this
 * function. The function will update the cycles and max fields of
 * this structure.
 * @return None
 ******************************************************************************/
void sl_cycle_counter_stop(sl_cycle_counter_handle_t *handle);

/***************************************************************************//**
 * @brief This function resets a cycle counter handle, preparing it for a new
 * measurement cycle. It should be used when you want to clear the
 * previous cycle data and start fresh. The function must be called with
 * a valid cycle counter handle that has been previously initialized. It
 * is important to ensure that the cycle counter is enabled before
 * calling this function, either through the system initialization or by
 * manually enabling it. The function returns a status code indicating
 * success or failure of the reset operation.
 *
 * @param handle Pointer to a cycle counter handle that is to be reset. The
 * handle must be valid and previously initialized. The caller
 * retains ownership of the handle, and it must not be null. If
 * the handle is invalid, the function will return an error code.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * reset operation. SL_STATUS_OK is returned if the reset is successful;
 * otherwise, an error code is returned.
 ******************************************************************************/
sl_status_t sl_cycle_counter_reset(sl_cycle_counter_handle_t *handle);

/***************************************************************************//**
 * @brief This function retrieves the number of cycles that have elapsed during
 * the last recording session from a given cycle counter handle. It is
 * typically used after stopping a cycle counter to determine the
 * duration of the recorded interval. The function should be called with
 * a valid, initialized cycle counter handle. If the handle is null, the
 * function returns 0, indicating no cycles were recorded. This function
 * is useful for performance measurements and timing analysis in embedded
 * systems.
 *
 * @param handle A pointer to an initialized sl_cycle_counter_handle_t
 * structure. Must not be null. If null, the function returns 0.
 * @return Returns the number of cycles recorded in the last session as a 32-bit
 * unsigned integer. Returns 0 if the handle is null.
 ******************************************************************************/
uint32_t sl_cycle_counter_get_cycles(sl_cycle_counter_handle_t *handle);

/***************************************************************************//**
 * @brief This function is used to obtain the maximum number of cycles recorded
 * by a cycle counter handle since its last reset or initialization. It
 * is useful for determining the longest duration recorded by the cycle
 * counter. The function should be called with a valid, initialized cycle
 * counter handle. If the provided handle is null, the function will
 * return 0, indicating an invalid operation.
 *
 * @param handle A pointer to an initialized sl_cycle_counter_handle_t
 * structure. This parameter must not be null, as a null handle
 * will result in the function returning 0.
 * @return Returns the maximum number of cycles recorded as a 32-bit unsigned
 * integer. If the handle is null, returns 0.
 ******************************************************************************/
uint32_t sl_cycle_counter_get_max(sl_cycle_counter_handle_t *handle);

/***************************************************************************//**
 * @brief The function `sl_cycle_counter_get_counter` retrieves the current
 * value of the cycle counter from the DWT (Data Watchpoint and Trace)
 * unit.
 *
 * @return The function returns a `uint32_t` value representing the current
 * cycle count from the DWT unit's cycle counter.
 ******************************************************************************/
__INLINE uint32_t sl_cycle_counter_get_counter(void)
{
  return DWT->CYCCNT;
}

/** @} (end group cycle_counter) */

#ifdef __cplusplus
}
#endif

#endif
