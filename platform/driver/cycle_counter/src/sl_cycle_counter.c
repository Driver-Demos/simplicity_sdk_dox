/*******************************************************************************
 * @file
 * @brief Cycle counter interface.
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

#include <stddef.h>
#include "sl_cycle_counter.h"

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/**
 * @brief The `sl_cycle_counter_enable` function enables the cycle counter by configuring specific registers in the ARM Cortex-M debug and trace units.
 *
 * @return The function does not return any value; it performs hardware register configurations to enable the cycle counter.
 */
void sl_cycle_counter_enable(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  ITM->LAR          = 0xc5acce55;
  DWT->CTRL        |= DWT_CTRL_CYCCNTENA_Msk;
}

/**
 * @brief The `sl_cycle_counter_init` function initializes a cycle counter handle by setting its start, cycles, and max fields to zero.
 *
 * @param handle A pointer to an `sl_cycle_counter_handle_t` structure that will be initialized.
 * @return The function returns `SL_STATUS_OK` if the initialization is successful, or `SL_STATUS_NULL_POINTER` if the provided handle is NULL.
 */
sl_status_t sl_cycle_counter_init(sl_cycle_counter_handle_t *handle)
{
  if (handle == NULL) {
    return SL_STATUS_NULL_POINTER;
  }

  handle->start   = 0;
  handle->cycles  = 0;
  handle->max     = 0;
  return SL_STATUS_OK;
}

/**
 * @brief The function `sl_cycle_counter_start` initializes the start time of a cycle counter by capturing the current value of the cycle counter register.
 *
 * @param handle A pointer to an `sl_cycle_counter_handle_t` structure, which holds the state of the cycle counter including the start time.
 * @return The function does not return a value; it modifies the `start` field of the provided `sl_cycle_counter_handle_t` structure.
 */
void sl_cycle_counter_start(sl_cycle_counter_handle_t *handle)
{
  handle->start = DWT->CYCCNT;
}

/**
 * @brief The `sl_cycle_counter_stop` function calculates the number of cycles elapsed since the cycle counter was started and updates the maximum cycle count if the current count exceeds the previous maximum.
 *
 * @param handle A pointer to an `sl_cycle_counter_handle_t` structure that holds the cycle counter data, including start, cycles, and max values.
 * @return The function does not return a value; it updates the `handle` structure with the elapsed cycles and potentially a new maximum cycle count.
 */
void sl_cycle_counter_stop(sl_cycle_counter_handle_t *handle)
{
  handle->cycles = DWT->CYCCNT - handle->start;

  if (handle->cycles > handle->max) {
    handle->max = handle->cycles;
  }
}

/**
 * @brief The `sl_cycle_counter_reset` function resets a cycle counter instance by reinitializing it.
 *
 * @param handle A pointer to an `sl_cycle_counter_handle_t` structure that represents the cycle counter instance to be reset.
 * @return The function returns an `sl_status_t` value indicating the success or failure of the reset operation, specifically `SL_STATUS_OK` if successful or `SL_STATUS_NULL_POINTER` if the handle is NULL.
 */
sl_status_t sl_cycle_counter_reset(sl_cycle_counter_handle_t *handle)
{
  return sl_cycle_counter_init(handle);
}

/**
 * @brief The function `sl_cycle_counter_get_cycles` retrieves the number of cycles recorded in the last cycle counter session from a given handle.
 *
 * @param handle A pointer to an `sl_cycle_counter_handle_t` structure, which contains the cycle counter data.
 * @return The function returns a `uint32_t` value representing the number of cycles recorded in the last session, or 0 if the handle is NULL.
 */
uint32_t sl_cycle_counter_get_cycles(sl_cycle_counter_handle_t *handle)
{
  if (handle == NULL) {
    return 0u;
  }

  return handle->cycles;
}

/**
 * @brief The function `sl_cycle_counter_get_max` retrieves the maximum cycle count recorded by a cycle counter instance.
 *
 * @param handle A pointer to an `sl_cycle_counter_handle_t` structure, which holds the state of a cycle counter instance.
 * @return The function returns a `uint32_t` value representing the maximum cycle count recorded, or 0 if the handle is NULL.
 */
uint32_t sl_cycle_counter_get_max(sl_cycle_counter_handle_t *handle)
{
  if (handle == NULL) {
    return 0u;
  }

  return handle->max;
}

/***************************************************************************//**
 * Read the current value of the cycle counter.
 *****************************************************************************/
extern __INLINE uint32_t sl_cycle_counter_get_counter(void);

/***************************************************************************//**
 * Pause cycle counter.
 ******************************************************************************/
extern __INLINE void sl_cycle_counter_pause(void);

/***************************************************************************//**
 * Resume cycle counter.
 ******************************************************************************/
extern __INLINE void sl_cycle_counter_resume(void);
