/***************************************************************************//**
 * @file
 * @brief CPC Timer API.
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef SLI_CPC_TIMER_H
#define SLI_CPC_TIMER_H

#include "sl_status.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
#include "sl_sleeptimer.h"
#else
#include "cpc_types.h"
#endif

/***************************************************************************//**
 * Typedef for the handle used to identify a timer.
 *
 * @param handle The timer handle.
 ******************************************************************************/
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
typedef sl_sleeptimer_timer_handle_t sli_cpc_timer_handle_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if !(defined(SL_CATALOG_SLEEPTIMER_PRESENT))
/***************************************************************************//**
 * Converts milliseconds into ticks.
 *
 * @param time_ms Number of milliseconds.
 *
 * @return Corresponding ticks number.
 *
 * @note The result is "rounded" to the superior tick number.
 *       This function is light and cannot fail so it should be privileged to
 *       perform a millisecond to tick conversion.
 ******************************************************************************/
uint32_t cpc_timer_ms_to_tick(uint16_t time_ms);

/***************************************************************************//**
 * Converts ticks in milliseconds.
 *
 * @param tick Number of tick.
 *
 * @return Corresponding milliseconds number.
 *
 * @note The result is rounded to the inferior millisecond.
 ******************************************************************************/
uint32_t cpc_timer_tick_to_ms(uint32_t tick);

/***************************************************************************//**
 * Gets current 32 bits global tick count.
 *
 * @return Current tick count.
 ******************************************************************************/
uint32_t cpc_timer_get_tick_count(void);

/***************************************************************************//**
 * Gets current 64 bits global tick count.
 *
 * @return Current tick count.
 ******************************************************************************/
uint64_t cpc_timer_get_tick_count64(void);

/***************************************************************************//**
 * Initialize a 32 bits timer
 *
 * @param handle Pointer to handle to timer.
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t cpc_timer_init(sli_cpc_timer_handle_t *handle);

/***************************************************************************//**
 * Get the CPC timer frequency.
 *
 * @return Timer frequency in hertz.
 ******************************************************************************/
uint32_t  cpc_timer_get_timer_frequency(void);

/***************************************************************************//**
 * Starts a 32 bits timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout Timer timeout, in timer ticks.
 * @param callback Callback function that will be called when
 *        timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t cpc_timer_start_timer(sli_cpc_timer_handle_t *handle,
                                  uint32_t timeout,
                                  void (*callback)(sli_cpc_timer_handle_t *handle, void *data),
                                  void *callback_data);

/**************************************************************************//**
 * Starts a 32 bits timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout_ms Timer timeout, in milliseconds.
 * @param callback Callback function that will be called when
 *        timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 *
 * @return SL_STATUS_OK if successful.
 *****************************************************************************/
sl_status_t cpc_timer_start_timer_ms(sli_cpc_timer_handle_t *handle,
                                     uint32_t timeout_ms,
                                     void (*callback)(sli_cpc_timer_handle_t *handle, void *data),
                                     void *callback_data);

/***************************************************************************//**
 * Restarts a 32 bits timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout Timer timeout, in timer ticks.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.

 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t cpc_timer_restart_timer(sli_cpc_timer_handle_t *handle,
                                    uint32_t timeout,
                                    void (*callback)(sli_cpc_timer_handle_t *handle, void *data),
                                    void *callback_data);

/***************************************************************************//**
 * Stops a timer.
 *
 * @param handle Pointer to handle to timer.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t cpc_timer_stop_timer(sli_cpc_timer_handle_t *handle);

/***************************************************************************//**
 * Delays for the specified number of milliseconds.
 *
 * @param delay_ms Delay in milliseconds.
 *
 * @return SL_STATUS_OK if successful.
 ******************************************************************************/
sl_status_t cpc_timer_delay_millisecond(uint32_t delay_ms);
#endif

/***************************************************************************//**
 * @brief The `sli_cpc_timer_init` function initializes a CPC timer handle,
 * conditionally using the sleeptimer if available.
 *
 * @param handle A pointer to a `sli_cpc_timer_handle_t` structure that
 * represents the timer handle to be initialized.
 * @return The function returns `SL_STATUS_OK` if the sleeptimer is present,
 * otherwise it returns the result of `cpc_timer_init(handle)`.
 ******************************************************************************/
static inline sl_status_t sli_cpc_timer_init(sli_cpc_timer_handle_t *handle)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  (void)handle;
  return SL_STATUS_OK;
#else
  return cpc_timer_init(handle);
#endif
}

/***************************************************************************//**
 * @brief The function `sli_cpc_timer_get_timer_frequency` returns the timer
 * frequency in hertz, selecting between two different implementations
 * based on the presence of the sleep timer component.
 *
 * @return The function returns a `uint32_t` representing the timer frequency in
 * hertz.
 ******************************************************************************/
static inline uint32_t  sli_cpc_timer_get_timer_frequency(void)
{
  #if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_get_timer_frequency();
  #else
  return cpc_timer_get_timer_frequency();
  #endif
}

/***************************************************************************//**
 * @brief The function `sli_cpc_timer_ms_to_tick` converts a given time in
 * milliseconds to the equivalent number of timer ticks, using either the
 * Sleep Timer or CPC Timer API based on the presence of the Sleep Timer
 * component.
 *
 * @param time_ms A 16-bit unsigned integer representing the time in
 * milliseconds to be converted to ticks.
 * @return The function returns a 32-bit unsigned integer representing the
 * equivalent number of timer ticks for the given milliseconds.
 ******************************************************************************/
static inline uint32_t sli_cpc_timer_ms_to_tick(uint16_t time_ms)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_ms_to_tick(time_ms);
#else
  return cpc_timer_ms_to_tick(time_ms);
#endif
}

/***************************************************************************//**
 * @brief The function `sli_cpc_timer_tick_to_ms` converts a given number of
 * timer ticks into milliseconds, using either the sleep timer or CPC
 * timer conversion function based on the presence of the sleep timer
 * component.
 *
 * @param tick The number of timer ticks to be converted into milliseconds.
 * @return The function returns the equivalent number of milliseconds for the
 * given number of ticks.
 ******************************************************************************/
static inline uint32_t sli_cpc_timer_tick_to_ms(uint32_t tick)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_tick_to_ms(tick);
#else
  return cpc_timer_tick_to_ms(tick);
#endif
}

/***************************************************************************//**
 * @brief The function `sli_cpc_timer_get_tick_count` retrieves the current
 * 32-bit global tick count using either the sleep timer or CPC timer,
 * depending on the presence of the sleep timer component.
 *
 * @return The function returns a `uint32_t` representing the current 32-bit
 * global tick count.
 ******************************************************************************/
static inline uint32_t sli_cpc_timer_get_tick_count(void)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_get_tick_count();
#else
  return cpc_timer_get_tick_count();
#endif
}

/***************************************************************************//**
 * @brief The function `sli_cpc_timer_get_tick_count64` retrieves the current
 * 64-bit global tick count using either the sleep timer or CPC timer,
 * depending on the presence of the sleep timer component.
 *
 * @return The function returns a 64-bit unsigned integer representing the
 * current global tick count.
 ******************************************************************************/
static inline uint64_t sli_cpc_timer_get_tick_count64(void)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_get_tick_count64();
#else
  return cpc_timer_get_tick_count64();
#endif
}

/***************************************************************************//**
 * @brief The `sli_cpc_timer_start_timer` function initializes and starts a
 * timer with a specified timeout and callback function, using either the
 * sleeptimer or CPC timer implementation based on the presence of the
 * sleeptimer component.
 *
 * @param handle A pointer to a `sli_cpc_timer_handle_t` structure that
 * identifies the timer to be started.
 * @param timeout A `uint32_t` value representing the timer's timeout period in
 * ticks.
 * @param callback A function pointer to a callback function that will be
 * invoked when the timer expires.
 * @param callback_data A pointer to user-defined data that will be passed to
 * the callback function when the timer expires.
 * @return The function returns an `sl_status_t` value indicating the success or
 * failure of the timer start operation.
 ******************************************************************************/
static inline sl_status_t sli_cpc_timer_start_timer(sli_cpc_timer_handle_t *handle,
                                                    uint32_t timeout,
                                                    void (*callback)(sli_cpc_timer_handle_t *handle, void *data),
                                                    void *callback_data)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_start_timer((sl_sleeptimer_timer_handle_t *)handle,
                                   timeout,
                                   (sl_sleeptimer_timer_callback_t)callback,
                                   callback_data,
                                   0,
                                   0);
#else
  return cpc_timer_start_timer(handle,
                               timeout,
                               callback,
                               callback_data);
#endif
}

/***************************************************************************//**
 * @brief The function `sli_cpc_timer_start_timer_ms` starts a timer with a
 * specified timeout in milliseconds and a callback function to be
 * executed upon timeout.
 *
 * @param handle A pointer to a `sli_cpc_timer_handle_t` structure that
 * identifies the timer to be started.
 * @param timeout_ms The timeout duration in milliseconds after which the
 * callback function will be executed.
 * @param callback A pointer to a callback function that will be called when the
 * timer expires.
 * @param callback_data A pointer to user data that will be passed to the
 * callback function when it is called.
 * @return The function returns an `sl_status_t` value indicating the success or
 * failure of starting the timer.
 ******************************************************************************/
static inline sl_status_t sli_cpc_timer_start_timer_ms(sli_cpc_timer_handle_t *handle,
                                                       uint32_t timeout_ms,
                                                       void (*callback)(sli_cpc_timer_handle_t *handle, void *data),
                                                       void *callback_data)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_start_timer_ms((sl_sleeptimer_timer_handle_t *)handle,
                                      timeout_ms,
                                      (sl_sleeptimer_timer_callback_t)callback,
                                      callback_data,
                                      0,
                                      0);
#else
  return cpc_timer_start_timer_ms(handle,
                                  timeout_ms,
                                  callback,
                                  callback_data);
#endif
}
/***************************************************************************//**
 * @brief The function `sli_cpc_timer_restart_timer` restarts a timer with a
 * specified timeout and callback function, using either the sleeptimer
 * or CPC timer implementation based on the presence of the sleeptimer
 * component.
 *
 * @param handle A pointer to the timer handle, which identifies the timer to be
 * restarted.
 * @param timeout The timeout duration for the timer, specified in timer ticks.
 * @param callback A pointer to the callback function that will be called when
 * the timer expires.
 * @param callback_data A pointer to user data that will be passed to the
 * callback function.
 * @return The function returns an `sl_status_t` status code indicating the
 * success or failure of the timer restart operation.
 ******************************************************************************/
static inline sl_status_t sli_cpc_timer_restart_timer(sli_cpc_timer_handle_t *handle,
                                                      uint32_t timeout,
                                                      void (*callback)(sli_cpc_timer_handle_t *handle, void *data),
                                                      void *callback_data)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_restart_timer((sl_sleeptimer_timer_handle_t *)handle,
                                     timeout,
                                     (sl_sleeptimer_timer_callback_t)callback,
                                     callback_data,
                                     0,
                                     0);
#else
  return cpc_timer_restart_timer(handle,
                                 timeout,
                                 callback,
                                 callback_data);
#endif
}

/***************************************************************************//**
 * @brief The `sli_cpc_timer_stop_timer` function stops a timer using either the
 * sleeptimer or CPC timer API, depending on the presence of the
 * sleeptimer component.
 *
 * @param handle A pointer to a `sli_cpc_timer_handle_t` structure that
 * identifies the timer to be stopped.
 * @return The function returns an `sl_status_t` value indicating the success or
 * failure of the timer stop operation.
 ******************************************************************************/
static inline sl_status_t sli_cpc_timer_stop_timer(sli_cpc_timer_handle_t *handle)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  return sl_sleeptimer_stop_timer((sl_sleeptimer_timer_handle_t *)handle);
#else
  return cpc_timer_stop_timer(handle);
#endif
}

/***************************************************************************//**
 * @brief The function `sli_cpc_timer_delay_millisecond` introduces a delay for
 * a specified number of milliseconds using either the sleeptimer or CPC
 * timer based on the presence of the sleeptimer component.
 *
 * @param delay_ms A 16-bit unsigned integer representing the delay duration in
 * milliseconds.
 * @return The function does not return any value; it performs a delay
 * operation.
 ******************************************************************************/
static inline void sli_cpc_timer_delay_millisecond(uint16_t delay_ms)
{
#if (defined(SL_CATALOG_SLEEPTIMER_PRESENT))
  sl_sleeptimer_delay_millisecond(delay_ms);
#else
  cpc_timer_delay_millisecond((uint32_t)delay_ms);
#endif
}

#ifdef __cplusplus
}
#endif

#endif // SLI_CPC_TIMER_H
