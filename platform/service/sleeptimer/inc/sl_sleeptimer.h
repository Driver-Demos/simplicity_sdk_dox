/***************************************************************************//**
 * @file
 * @brief SLEEPTIMER API definition.
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

/***************************************************************************//**
 * @addtogroup sleeptimer Sleep Timer
 * @{
 ******************************************************************************/

#ifndef SL_SLEEPTIMER_H
#define SL_SLEEPTIMER_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "sl_status.h"
#include "sl_common.h"
#include "sl_code_classification.h"

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
#define SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG (0x01)
#define SL_SLEEPTIMER_ANY_FLAG                                  (0xFF)

#define SLEEPTIMER_ENUM(name) typedef uint8_t name; enum name##_enum

/// @endcond

/// Timestamp, wall clock time in seconds.
typedef uint32_t sl_sleeptimer_timestamp_t;

// Timestamp, 64 bits wall clock in seconds.
typedef uint64_t sl_sleeptimer_timestamp_64_t;     ///< sl sleeptimer timestamp 64 t

/// Time zone offset from UTC(second).
typedef int32_t sl_sleeptimer_time_zone_offset_t;

// Forward declaration
/***************************************************************************//**
 * @brief The `sl_sleeptimer_timer_handle_t` structure is used to manage
 * individual timers within the sleeptimer system. It contains fields for
 * user data, priority, and option flags, as well as pointers for
 * managing a linked list of timers. The structure also includes a
 * callback function to be executed when the timer expires, and fields
 * for managing periodic timeouts and conversion errors. This structure
 * is integral to the sleeptimer's ability to handle multiple timers and
 * execute user-defined actions upon timer expiration.
 *
 * @param callback_data User data to pass to callback function.
 * @param priority Priority of timer.
 * @param option_flags Option flags.
 * @param next Pointer to next element in list.
 * @param callback Function to call when timer expires.
 * @param timeout_periodic Periodic timeout.
 * @param delta Delay relative to previous element in list.
 * @param timeout_expected_tc Expected tick count of the next timeout (only used
 * for periodic timer).
 * @param conversion_error The error when converting ms to ticks (thousandths of
 * ticks).
 * @param accumulated_error Accumulated conversion error (thousandths of ticks).
 ******************************************************************************/
typedef struct sl_sleeptimer_timer_handle sl_sleeptimer_timer_handle_t;

/***************************************************************************//**
 * Typedef for the user supplied callback function which is called when
 * a timer expires.
 *
 * @param handle The timer handle.
 *
 * @param data An extra parameter for the user application.
 ******************************************************************************/
typedef void (*sl_sleeptimer_timer_callback_t)(sl_sleeptimer_timer_handle_t *handle, void *data);

/// @brief Timer structure for sleeptimer
/***************************************************************************//**
 * @brief The `sl_sleeptimer_timer_handle` structure is used to manage and
 * control individual timers within the sleeptimer API. It contains
 * various fields to store user data, priority, and options for the
 * timer, as well as pointers to the next timer in a list and the
 * callback function to be executed upon timer expiration. Additionally,
 * it holds information about the periodic timeout, delay relative to
 * other timers, expected tick count for the next timeout, and error
 * metrics related to time conversion. This structure is essential for
 * creating and managing both one-shot and periodic timers in a power-
 * efficient manner.
 *
 * @param callback_data User data to pass to callback function.
 * @param priority Priority of timer.
 * @param option_flags Option flags.
 * @param next Pointer to next element in list.
 * @param callback Function to call when timer expires.
 * @param timeout_periodic Periodic timeout.
 * @param delta Delay relative to previous element in list.
 * @param timeout_expected_tc Expected tick count of the next timeout (only used
 * for periodic timer).
 * @param conversion_error The error when converting ms to ticks (thousandths of
 * ticks).
 * @param accumulated_error Accumulated conversion error (thousandths of ticks).
 ******************************************************************************/
struct sl_sleeptimer_timer_handle {
  void *callback_data;                     ///< User data to pass to callback function.
  uint8_t priority;                        ///< Priority of timer.
  uint16_t option_flags;                   ///< Option flags.
  sl_sleeptimer_timer_handle_t *next;      ///< Pointer to next element in list.
  sl_sleeptimer_timer_callback_t callback; ///< Function to call when timer expires.
  uint32_t timeout_periodic;               ///< Periodic timeout.
  uint32_t delta;                          ///< Delay relative to previous element in list.
  uint32_t timeout_expected_tc;            ///< Expected tick count of the next timeout (only used for periodic timer).
  uint16_t conversion_error;               ///< The error when converting ms to ticks (thousandths of ticks)
  uint16_t accumulated_error;              ///< Accumulated conversion error (thousandths of ticks)
};

/// @brief Month enum.
SLEEPTIMER_ENUM(sl_sleeptimer_month_t) {
  MONTH_JANUARY = 0,
  MONTH_FEBRUARY = 1,
  MONTH_MARCH   = 2,
  MONTH_APRIL = 3,
  MONTH_MAY = 4,
  MONTH_JUNE = 5,
  MONTH_JULY = 6,
  MONTH_AUGUST = 7,
  MONTH_SEPTEMBER = 8,
  MONTH_OCTOBER = 9,
  MONTH_NOVEMBER = 10,
  MONTH_DECEMBER = 11,
};

/// @brief Week Day enum.
SLEEPTIMER_ENUM(sl_sleeptimer_weekDay_t) {
  DAY_SUNDAY = 0,
  DAY_MONDAY = 1,
  DAY_TUESDAY = 2,
  DAY_WEDNESDAY = 3,
  DAY_THURSDAY = 4,
  DAY_FRIDAY = 5,
  DAY_SATURDAY = 6,
};

/// @brief Time and Date structure.
/***************************************************************************//**
 * @brief The `sl_sleeptimer_date_t` structure is used to represent a specific
 * date and time, including fields for seconds, minutes, hours, day of
 * the month, month, year, day of the week, day of the year, and time
 * zone offset. It is designed to work with the sleeptimer API, providing
 * a comprehensive representation of time that can be used for various
 * timekeeping and scheduling functionalities within the Silicon Labs
 * sleeptimer framework.
 *
 * @param sec Second (0-59).
 * @param min Minute of month (0-59).
 * @param hour Hour (0-23).
 * @param month_day Day of month (1-31).
 * @param month Month (0-11).
 * @param year Year, based on a 1900 Epoch.
 * @param day_of_week Day of week (0-6).
 * @param day_of_year Day of year (1-366).
 * @param time_zone Offset, in seconds, from UTC.
 ******************************************************************************/
typedef  struct  time_date {
  uint8_t sec;                                ///< Second (0-59)
  uint8_t min;                                ///< Minute of month (0-59)
  uint8_t hour;                               ///< Hour (0-23)
  uint8_t month_day;                          ///< Day of month (1-31)
  sl_sleeptimer_month_t month;                ///< Month (0-11)
  uint16_t year;                              ///< Year, based on a 1900 Epoch.
  sl_sleeptimer_weekDay_t day_of_week;        ///< Day of week (0-6)
  uint16_t day_of_year;                       ///< Day of year (1-366)
  sl_sleeptimer_time_zone_offset_t time_zone; ///< Offset, in seconds, from UTC
} sl_sleeptimer_date_t;

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function sets up the Sleeptimer module, preparing it for use in
 * managing timers. It must be called before any other Sleeptimer
 * functions to ensure proper operation. The function checks if the
 * Sleeptimer is already initialized and performs necessary hardware
 * initializations if not. It returns an error if the timer frequency is
 * zero, indicating an invalid configuration. This function is typically
 * called once during system startup.
 *
 * @return Returns SL_STATUS_OK if the initialization is successful, or
 * SL_STATUS_INVALID_CONFIGURATION if the timer frequency is zero.
 ******************************************************************************/
sl_status_t sl_sleeptimer_init(void);

/***************************************************************************//**
 * Starts a 32 bits timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout Timer timeout, in timer ticks.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to start a one-shot timer that will trigger a
 * user-defined callback function after a specified timeout period,
 * measured in timer ticks. It is essential to ensure that the timer
 * handle is not null and that the timer is not already running before
 * calling this function. The function should not be called from an
 * interrupt with a higher priority than BASEPRI. The priority parameter
 * allows for prioritization of callbacks when multiple timers expire
 * simultaneously. Option flags can be used to modify the timer's
 * behavior.
 *
 * @param handle Pointer to a sl_sleeptimer_timer_handle_t structure that will
 * be used to manage the timer. Must not be null.
 * @param timeout The duration in timer ticks after which the callback will be
 * triggered. Must be a valid 32-bit unsigned integer.
 * @param callback Function pointer to the callback that will be executed when
 * the timer expires. Must be a valid function pointer.
 * @param callback_data Pointer to user data that will be passed to the callback
 * function. Can be null if no user data is needed.
 * @param priority Unsigned 8-bit integer representing the priority of the
 * callback. Lower values indicate higher priority.
 * @param option_flags Unsigned 16-bit integer representing a bitwise OR of
 * option flags that modify the timer's behavior. Valid
 * flags include
 * SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 * or 0 for no flags.
 * @return Returns SL_STATUS_OK if the timer is successfully started,
 * SL_STATUS_NULL_POINTER if the handle is null, or SL_STATUS_NOT_READY
 * if the timer is already running.
 ******************************************************************************/
sl_status_t sl_sleeptimer_start_timer(sl_sleeptimer_timer_handle_t *handle,
                                      uint32_t timeout,
                                      sl_sleeptimer_timer_callback_t callback,
                                      void *callback_data,
                                      uint8_t priority,
                                      uint16_t option_flags);

/***************************************************************************//**
 * Restarts a 32 bits timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout Timer timeout, in timer ticks.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to restart a 32-bit timer with a new timeout,
 * callback, and other parameters. It should be called when you need to
 * reset a timer's countdown and potentially change its behavior. The
 * function first attempts to stop the timer if it is running, then
 * reinitializes it with the provided parameters. It is important to
 * ensure that the timer handle is not null before calling this function,
 * as a null handle will result in an error. This function cannot be
 * called from an interrupt with a higher priority than BASEPRI.
 *
 * @param handle Pointer to the timer handle. Must not be null. The caller
 * retains ownership.
 * @param timeout The timer timeout in timer ticks. Must be a valid 32-bit
 * unsigned integer.
 * @param callback Function pointer to the callback to be called when the timer
 * expires. Must be a valid function pointer.
 * @param callback_data Pointer to user data to be passed to the callback. Can
 * be null if no data is needed.
 * @param priority Priority of the callback. A lower number indicates higher
 * priority. Must be a valid 8-bit unsigned integer.
 * @param option_flags Bit array of option flags for the timer. Can be a bitwise
 * OR of predefined flags or 0 for no flags.
 * @return Returns SL_STATUS_OK if successful, or an error code if the operation
 * fails.
 ******************************************************************************/
sl_status_t sl_sleeptimer_restart_timer(sl_sleeptimer_timer_handle_t *handle,
                                        uint32_t timeout,
                                        sl_sleeptimer_timer_callback_t callback,
                                        void *callback_data,
                                        uint8_t priority,
                                        uint16_t option_flags);

/***************************************************************************//**
 * Starts a 32 bits periodic timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout Timer periodic timeout, in timer ticks.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to start a periodic timer that triggers a user-
 * defined callback function at specified intervals. It requires a valid
 * timer handle, a timeout value in timer ticks, a callback function, and
 * optional user data to be passed to the callback. The function also
 * accepts a priority level for the callback and option flags to modify
 * timer behavior. The timer must not be already running when this
 * function is called, and it cannot be invoked from an interrupt with a
 * higher priority than BASEPRI. If the handle is null or the timer is
 * already running, the function returns an error status.
 *
 * @param handle Pointer to a sl_sleeptimer_timer_handle_t structure that will
 * be used to manage the timer. Must not be null.
 * @param timeout The periodic timeout interval in timer ticks. Must be a valid
 * non-zero value.
 * @param callback Function pointer to the callback that will be executed when
 * the timer expires. Must not be null.
 * @param callback_data Pointer to user-defined data that will be passed to the
 * callback function. Can be null if no data is needed.
 * @param priority Priority level for the callback execution. Lower values
 * indicate higher priority.
 * @param option_flags Bitwise OR of option flags that modify timer behavior.
 * Valid flags include
 * SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 * or 0 for no flags.
 * @return Returns SL_STATUS_OK if the timer is successfully started, or an
 * error code if the handle is null or the timer is already running.
 ******************************************************************************/
sl_status_t sl_sleeptimer_start_periodic_timer(sl_sleeptimer_timer_handle_t *handle,
                                               uint32_t timeout,
                                               sl_sleeptimer_timer_callback_t callback,
                                               void *callback_data,
                                               uint8_t priority,
                                               uint16_t option_flags);

/***************************************************************************//**
 * Restarts a 32 bits periodic timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout Timer periodic timeout, in timer ticks.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to restart a periodic timer with a specified
 * timeout, callback function, callback data, priority, and option flags.
 * It is essential to ensure that the timer handle is not null before
 * calling this function, as a null handle will result in an error. The
 * function stops the timer if it is running and then restarts it with
 * the new parameters. This function should not be called from an
 * interrupt with a higher priority than BASEPRI.
 *
 * @param handle Pointer to the timer handle. Must not be null. The caller
 * retains ownership.
 * @param timeout The periodic timeout for the timer, specified in timer ticks.
 * Must be a valid 32-bit unsigned integer.
 * @param callback Function pointer to the callback that will be called when the
 * timer expires. Must be a valid function pointer.
 * @param callback_data Pointer to user data that will be passed to the callback
 * function. Can be null if no data is needed.
 * @param priority Priority of the callback. A lower number indicates higher
 * priority. Must be a valid 8-bit unsigned integer.
 * @param option_flags Bit array of option flags for the timer. Can be a bitwise
 * OR of predefined flags like
 * SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 * or 0 for no flags.
 * @return Returns SL_STATUS_OK if the timer is successfully restarted, or an
 * error code if the operation fails.
 ******************************************************************************/
sl_status_t sl_sleeptimer_restart_periodic_timer(sl_sleeptimer_timer_handle_t *handle,
                                                 uint32_t timeout,
                                                 sl_sleeptimer_timer_callback_t callback,
                                                 void *callback_data,
                                                 uint8_t priority,
                                                 uint16_t option_flags);

/***************************************************************************//**
 * Stops a timer.
 *
 * @param handle Pointer to handle to timer.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to stop a previously started sleep timer. It is
 * essential to ensure that the timer handle provided is valid and not
 * null before calling this function. This function should not be called
 * from an interrupt with a higher priority than BASEPRI. If the timer is
 * the first in the list, the function will update the timer comparator
 * accordingly. The function returns an error code if the operation
 * fails, such as when the handle is null.
 *
 * @param handle A pointer to the sl_sleeptimer_timer_handle_t structure
 * representing the timer to be stopped. Must not be null. If
 * null, the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if the timer is successfully stopped, or an
 * error code if the operation fails.
 ******************************************************************************/
sl_status_t sl_sleeptimer_stop_timer(sl_sleeptimer_timer_handle_t *handle);

/***************************************************************************//**
 * Gets the status of a timer.
 *
 * @param handle Pointer to handle to timer.
 * @param running Pointer to the status of the timer.
 *
 * @note A non periodic timer is considered not running during its callback.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function determines whether a given timer, identified by its
 * handle, is currently active. It is useful for checking the status of a
 * timer before performing operations that depend on the timer's state.
 * The function must be called with valid pointers for both the timer
 * handle and the running status output. If either pointer is null, the
 * function returns an error status. The function is thread-safe and can
 * be called from within an interrupt context.
 *
 * @param handle A pointer to the timer handle to check. Must not be null. If
 * null, the function returns SL_STATUS_NULL_POINTER.
 * @param running A pointer to a boolean variable where the running status will
 * be stored. Must not be null. If null, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if the operation is successful, or
 * SL_STATUS_NULL_POINTER if any input pointer is null. The boolean
 * pointed to by 'running' is set to true if the timer is running, false
 * otherwise.
 ******************************************************************************/
sl_status_t sl_sleeptimer_is_timer_running(const sl_sleeptimer_timer_handle_t *handle,
                                           bool *running);

/***************************************************************************//**
 * Gets remaining time until timer expires.
 *
 * @param handle Pointer to handle to timer.
 * @param time Time left in timer ticks.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to determine how much time is left before a
 * specified timer expires. It is useful for applications that need to
 * monitor or adjust timing based on the remaining duration of a timer.
 * The function requires a valid timer handle and a pointer to store the
 * remaining time. It must be called with valid pointers, and it will
 * return an error if the timer is not ready or if any pointer is null.
 *
 * @param handle A pointer to the timer handle whose remaining time is to be
 * retrieved. Must not be null. If the handle does not correspond
 * to a valid or ready timer, an error is returned.
 * @param time A pointer to a uint32_t where the remaining time in timer ticks
 * will be stored. Must not be null. If the function fails, the
 * value pointed to by this parameter is not modified.
 * @return Returns SL_STATUS_OK if successful, SL_STATUS_NULL_POINTER if any
 * input pointer is null, or SL_STATUS_NOT_READY if the timer is not
 * ready.
 ******************************************************************************/
sl_status_t sl_sleeptimer_get_timer_time_remaining(const sl_sleeptimer_timer_handle_t *handle,
                                                   uint32_t *time);

/**************************************************************************//**
 * Gets the time remaining until the first timer with the matching set of flags
 * expires.
 *
 * @param option_flags Set of flags to match:
 *          - SL_SLEEPTIMER_ANY_TIMER_FLAG
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *
 * @param time_remaining Time left in timer ticks.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to determine how much time is left before the first
 * timer that matches the given option flags expires. This is useful for
 * managing or adjusting operations based on timer events. The function
 * must be called with a valid pointer for the time_remaining parameter,
 * where the remaining time will be stored. If no timer matches the
 * specified flags, the function returns an error status. Ensure that the
 * sleeptimer system is initialized before calling this function.
 *
 * @param option_flags A set of flags to match against timers. It can be a
 * specific flag or SL_SLEEPTIMER_ANY_FLAG to match any
 * timer.
 * @param time_remaining A pointer to a uint32_t where the remaining time in
 * timer ticks will be stored. Must not be null.
 * @return Returns SL_STATUS_OK if a matching timer is found and time_remaining
 * is set. Returns SL_STATUS_EMPTY if no matching timer is found.
 ******************************************************************************/
sl_status_t sl_sleeptimer_get_remaining_time_of_first_timer(uint16_t option_flags,
                                                            uint32_t *time_remaining);

/***************************************************************************//**
 * Gets current 32 bits global tick count.
 *
 * @return Current tick count.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the current tick count from the
 * sleeptimer, which is a 32-bit value representing the number of ticks
 * since the sleeptimer was initialized. It is useful for timekeeping and
 * measuring elapsed time in applications that require precise timing.
 * The function is safe to call from any context, including interrupt
 * service routines, as it ensures atomic access to the tick count.
 *
 * @return Returns the current 32-bit tick count as a uint32_t value.
 ******************************************************************************/
uint32_t sl_sleeptimer_get_tick_count(void);

/***************************************************************************//**
 * Gets current 64 bits global tick count.
 *
 * @return Current tick count.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function provides the current tick count as a 64-bit value, which
 * is useful for applications requiring high-resolution time tracking. It
 * should be called when precise timing information is needed, such as
 * for profiling or synchronization tasks. The function is thread-safe
 * and can be called from interrupt contexts. It is important to ensure
 * that the sleeptimer is initialized before calling this function to
 * guarantee accurate results.
 *
 * @return Returns the current 64-bit tick count as a uint64_t value.
 ******************************************************************************/
uint64_t sl_sleeptimer_get_tick_count64(void);

/***************************************************************************//**
 * Get timer frequency.
 *
 * @return Timer frequency in hertz.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to obtain the frequency of the timer in hertz,
 * which is essential for understanding the timing resolution and
 * behavior of the sleeptimer. It can be called at any time after the
 * sleeptimer has been initialized to determine the current operating
 * frequency of the timer. This information is useful for applications
 * that need to perform time-based calculations or adjustments.
 *
 * @return Returns the current timer frequency as a 32-bit unsigned integer,
 * representing the frequency in hertz.
 ******************************************************************************/
uint32_t sl_sleeptimer_get_timer_frequency(void);

/***************************************************************************//**
 * @brief This function is used to convert a 32-bit Unix timestamp into a human-
 * readable date format, taking into account a specified time zone
 * offset. It is useful when you need to display or process date and time
 * information based on a Unix timestamp. The function requires a valid
 * timestamp and time zone offset, and it populates the provided date
 * structure with the corresponding date and time details. Ensure that
 * the date pointer is not null before calling this function.
 *
 * @param time A 32-bit Unix timestamp representing the number of seconds since
 * January 1, 1970. It must be a valid timestamp within the
 * supported range.
 * @param time_zone An integer representing the time zone offset in seconds from
 * UTC. This value adjusts the timestamp to the desired time
 * zone.
 * @param date A pointer to an sl_sleeptimer_date_t structure where the
 * converted date will be stored. Must not be null, and the caller
 * is responsible for allocating this structure.
 * @return Returns an sl_status_t indicating success (SL_STATUS_OK) or an error
 * code if the conversion fails.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_time_to_date(sl_sleeptimer_timestamp_t time,
                                               sl_sleeptimer_time_zone_offset_t time_zone,
                                               sl_sleeptimer_date_t *date);

/***************************************************************************//**
 * @brief This function is used to convert a 64-bit Unix timestamp into a human-
 * readable date format, taking into account a specified time zone
 * offset. It is useful when you need to display or process date and time
 * information based on a timestamp. The function requires a valid
 * timestamp and time zone offset, and it populates the provided date
 * structure with the corresponding date and time details. It must be
 * called with a valid pointer to a date structure, and the function will
 * return an error if the timestamp is invalid.
 *
 * @param time A 64-bit Unix timestamp representing the number of seconds since
 * the Unix epoch. It must be a valid timestamp, or the function
 * will return an error.
 * @param time_zone An offset in seconds from UTC to apply to the timestamp.
 * This value adjusts the timestamp to the desired time zone.
 * @param date A pointer to an sl_sleeptimer_date_t structure where the
 * converted date will be stored. This pointer must not be null, and
 * the caller retains ownership of the memory.
 * @return Returns SL_STATUS_OK if the conversion is successful, or
 * SL_STATUS_INVALID_PARAMETER if the timestamp is invalid.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_time_to_date_64(sl_sleeptimer_timestamp_64_t time,
                                                  sl_sleeptimer_time_zone_offset_t time_zone,
                                                  sl_sleeptimer_date_t *date);

/***************************************************************************//**
 * @brief This function is used to convert a given date, represented by a
 * sl_sleeptimer_date_t structure, into a 32-bit Unix timestamp, which is
 * the number of seconds since January 1, 1970. It is important to ensure
 * that the date provided falls within the supported range of January 1,
 * 1970, to January 19, 2038. The function must be called with a valid
 * date structure, and the output timestamp is stored in the provided
 * pointer. The function returns a status code indicating success or the
 * type of error encountered.
 *
 * @param date Pointer to a sl_sleeptimer_date_t structure representing the date
 * to be converted. Must not be null and should contain valid date
 * values within the supported range.
 * @param time Pointer to a sl_sleeptimer_timestamp_t where the converted 32-bit
 * Unix timestamp will be stored. Must not be null.
 * @return Returns an sl_status_t indicating SL_STATUS_OK on success or an error
 * code if the conversion fails.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_date_to_time(sl_sleeptimer_date_t *date,
                                               sl_sleeptimer_timestamp_t *time);

/***************************************************************************//**
 * @brief This function converts a given date, represented by a
 * `sl_sleeptimer_date_t` structure, into a 64-bit Unix timestamp, which
 * is stored in the provided `sl_sleeptimer_timestamp_64_t` pointer. It
 * is essential to ensure that the date provided is valid according to
 * the supported range, which spans from January 1, 1900, to December 31,
 * 11899. The function must be called with a properly initialized date
 * structure, and the `time` pointer must not be null. If the date is
 * invalid, the function returns an error status.
 *
 * @param date A pointer to a `sl_sleeptimer_date_t` structure representing the
 * date to convert. The date must be valid and within the supported
 * range. The caller retains ownership and must ensure the pointer
 * is not null.
 * @param time A pointer to a `sl_sleeptimer_timestamp_64_t` where the converted
 * 64-bit Unix timestamp will be stored. The pointer must not be
 * null, and the caller retains ownership.
 * @return Returns `SL_STATUS_OK` if the conversion is successful, or
 * `SL_STATUS_INVALID_PARAMETER` if the date is invalid.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_date_to_time_64(sl_sleeptimer_date_t *date,
                                                  sl_sleeptimer_timestamp_64_t *time);

/***************************************************************************//**
 * @brief This function is used to convert a date represented by a
 * `sl_sleeptimer_date_t` structure into a string format specified by the
 * user. It is useful for displaying date information in a human-readable
 * format. The function requires a valid date structure and a format
 * string similar to those used in the `strftime` function. The output
 * string buffer must be large enough to hold the resulting formatted
 * string, including the null terminator. If the date is invalid, the
 * function returns 0, indicating an error.
 *
 * @param str A pointer to the buffer where the formatted date string will be
 * stored. The buffer must be large enough to hold the resulting
 * string, including the null terminator. The caller retains
 * ownership.
 * @param size The size of the buffer pointed to by `str`. It must be large
 * enough to store the formatted string and the null terminator.
 * @param format A pointer to a format string that specifies how the date should
 * be formatted. The format string should follow the conventions
 * used by `strftime`. The caller retains ownership.
 * @param date A pointer to a `sl_sleeptimer_date_t` structure containing the
 * date to be converted. The date must be valid, or the function
 * will return 0.
 * @return Returns the number of characters written to the output string,
 * excluding the null terminator. Returns 0 if the date is invalid or an
 * error occurs.
 ******************************************************************************/
uint32_t sl_sleeptimer_convert_date_to_str(char *str,
                                           size_t size,
                                           const uint8_t *format,
                                           sl_sleeptimer_date_t *date);

/***************************************************************************//**
 * @brief Use this function to configure the time zone offset for the
 * sleeptimer, which is specified in seconds from UTC. This function
 * should be called when you need to adjust the time zone settings for
 * applications relying on the sleeptimer's timekeeping features. It is
 * important to ensure that the offset value is correctly calculated and
 * provided in seconds to reflect the desired time zone.
 *
 * @param offset Time zone offset in seconds from UTC. The caller is responsible
 * for ensuring the offset is correctly calculated and valid for
 * the intended time zone.
 * @return None
 ******************************************************************************/
void sl_sleeptimer_set_tz(sl_sleeptimer_time_zone_offset_t offset);

/***************************************************************************//**
 * @brief This function is used to obtain the current time zone offset in
 * seconds from Coordinated Universal Time (UTC). It is useful when you
 * need to adjust time-related calculations based on the local time zone.
 * The function is thread-safe and can be called at any time to get the
 * most recent time zone offset set in the system.
 *
 * @return Returns the current time zone offset in seconds as an integer of type
 * sl_sleeptimer_time_zone_offset_t.
 ******************************************************************************/
sl_sleeptimer_time_zone_offset_t sl_sleeptimer_get_tz(void);

/***************************************************************************//**
 * @brief This function provides the current time as a 32-bit Unix timestamp,
 * which represents the number of seconds since January 1, 1970. It is
 * useful for applications that require timekeeping or need to log events
 * with a timestamp. The function should be called when the current time
 * is needed in a 32-bit format. It is important to note that this
 * function is only available when the SL_SLEEPTIMER_WALLCLOCK_CONFIG is
 * set to 1, enabling wall clock functionalities.
 *
 * @return Returns the current time as a 32-bit Unix timestamp.
 ******************************************************************************/
sl_sleeptimer_timestamp_t sl_sleeptimer_get_time(void);

/***************************************************************************//**
 * @brief This function provides the current time as a 64-bit Unix timestamp,
 * representing the number of seconds since January 1, 1970. It is useful
 * for applications that require precise timekeeping over long durations.
 * The function should be called when a high-resolution timestamp is
 * needed, and it is safe to use in concurrent environments as it handles
 * atomic operations internally. Ensure that the sleeptimer is
 * initialized before calling this function to get accurate results.
 *
 * @return Returns the current time as a 64-bit Unix timestamp.
 ******************************************************************************/
sl_sleeptimer_timestamp_64_t sl_sleeptimer_get_time_64(void);

/***************************************************************************//**
 * @brief This function sets the current time of the sleeptimer using a 32-bit
 * Unix timestamp. It is typically used to synchronize the sleeptimer
 * with an external time source. The function should be called only when
 * the sleeptimer is initialized and configured. It is important to
 * ensure that the provided timestamp is within the valid range for
 * 32-bit Unix timestamps. The function returns a status code indicating
 * success or the type of error encountered.
 *
 * @param time A 32-bit Unix timestamp representing the time to set. The value
 * should be within the range of valid Unix timestamps for 32-bit
 * integers. The caller retains ownership of this value.
 * @return Returns an sl_status_t value indicating SL_STATUS_OK if the operation
 * was successful, or an error code if it failed.
 ******************************************************************************/
sl_status_t sl_sleeptimer_set_time(sl_sleeptimer_timestamp_t time);

/***************************************************************************//**
 * @brief This function is used to set the current time of the sleeptimer using
 * a 64-bit Unix timestamp. It is important to ensure that the provided
 * timestamp is valid and within the acceptable range for Unix
 * timestamps. The function should be called when there is a need to
 * update the sleeptimer's current time to a specific 64-bit timestamp.
 * If the provided timestamp is invalid, the function will return an
 * error status. This function is typically used in applications where
 * precise timekeeping is required, and it must be called in a context
 * where atomic operations are safe.
 *
 * @param time A 64-bit Unix timestamp representing the desired current time. It
 * must be a valid timestamp within the Unix time range. If the
 * timestamp is invalid, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @return Returns SL_STATUS_OK if the time is set successfully, or
 * SL_STATUS_INVALID_PARAMETER if the input timestamp is invalid.
 ******************************************************************************/
sl_status_t sl_sleeptimer_set_time_64(sl_sleeptimer_timestamp_64_t time);

/***************************************************************************//**
 * @brief This function is used to obtain the current date and time, including
 * time zone information, and store it in a provided date structure. It
 * should be called when the current date and time are needed in a
 * structured format. The function requires that the sleeptimer is
 * properly initialized and configured to provide accurate timekeeping.
 * It is important to ensure that the provided date structure is valid
 * and writable, as the function will populate it with the current date
 * and time information.
 *
 * @param date A pointer to an sl_sleeptimer_date_t structure where the current
 * date and time will be stored. Must not be null, and the caller
 * must ensure it points to a valid memory location.
 * @return Returns an sl_status_t indicating success or an error code if the
 * operation fails.
 ******************************************************************************/
sl_status_t sl_sleeptimer_get_datetime(sl_sleeptimer_date_t *date);

/***************************************************************************//**
 * @brief This function sets the current date and time for the sleeptimer using
 * the provided date structure. It should be used when you need to update
 * the sleeptimer's internal clock to a specific date and time. The
 * function requires a valid date structure, and it will return an error
 * if the date is invalid. It is important to ensure that the date
 * structure is correctly populated before calling this function. The
 * function also sets the time zone based on the provided date structure.
 *
 * @param date A pointer to an sl_sleeptimer_date_t structure containing the
 * date and time to set. The structure must be valid and correctly
 * populated. The function will return SL_STATUS_INVALID_PARAMETER
 * if the date is invalid. The caller retains ownership of the
 * structure.
 * @return Returns SL_STATUS_OK if the date and time were successfully set, or
 * an error code if the operation failed.
 ******************************************************************************/
sl_status_t sl_sleeptimer_set_datetime(sl_sleeptimer_date_t *date);

/***************************************************************************//**
 * @brief This function constructs a date-time structure using the specified
 * year, month, day, hour, minute, second, and time zone offset. It is
 * useful for setting up a date-time representation in applications that
 * require precise timekeeping. The function must be called with a valid
 * pointer to a date structure, and all parameters should be within their
 * expected ranges. If the input parameters are invalid, the function
 * returns an error status. The function also calculates the day of the
 * year and the day of the week based on the provided date.
 *
 * @param date Pointer to a sl_sleeptimer_date_t structure to be populated. Must
 * not be null.
 * @param year The year component of the date. If less than 1900, it is assumed
 * to be based on the NTP epoch.
 * @param month The month component, represented as an enum
 * sl_sleeptimer_month_t, with values ranging from 0 (January) to
 * 11 (December).
 * @param month_day The day of the month, with valid values ranging from 1 to
 * 31.
 * @param hour The hour of the day, with valid values ranging from 0 to 23.
 * @param min The minute of the hour, with valid values ranging from 0 to 59.
 * @param sec The second of the minute, with valid values ranging from 0 to 59.
 * @param tz_offset The time zone offset from UTC, in seconds.
 * @return Returns SL_STATUS_OK if the date-time structure is successfully
 * built, SL_STATUS_NULL_POINTER if the date pointer is null, or
 * SL_STATUS_INVALID_PARAMETER if the input parameters are invalid.
 ******************************************************************************/
sl_status_t sl_sleeptimer_build_datetime(sl_sleeptimer_date_t *date,
                                         uint16_t year,
                                         sl_sleeptimer_month_t month,
                                         uint8_t month_day,
                                         uint8_t hour,
                                         uint8_t min,
                                         uint8_t sec,
                                         sl_sleeptimer_time_zone_offset_t tzOffset);

/***************************************************************************//**
 * @brief This function constructs a date-time structure using the provided
 * year, month, day, hour, minute, second, and time zone offset. It is
 * essential to ensure that the year is specified based on a 0 epoch and
 * is greater than 1900. The function will populate the provided date
 * structure with the calculated values, including the day of the year
 * and the day of the week. It must be called with a valid pointer to a
 * date structure, and all parameters must be within their expected
 * ranges to avoid errors. The function returns a status code indicating
 * success or the type of error encountered.
 *
 * @param date Pointer to a sl_sleeptimer_date_t structure to be populated. Must
 * not be null.
 * @param year Year based on 0 epoch. Must be greater than 1900.
 * @param month Month of the year, with values ranging from 0 (January) to 11
 * (December).
 * @param month_day Day of the month, with valid values from 1 to 31.
 * @param hour Hour of the day, with valid values from 0 to 23.
 * @param min Minute of the hour, with valid values from 0 to 59.
 * @param sec Second of the minute, with valid values from 0 to 59.
 * @param tz_offset Time zone offset from UTC in seconds.
 * @return Returns SL_STATUS_OK if successful, or an error code such as
 * SL_STATUS_NULL_POINTER or SL_STATUS_INVALID_PARAMETER if inputs are
 * invalid.
 ******************************************************************************/
sl_status_t sl_sleeptimer_build_datetime_64(sl_sleeptimer_date_t *date,
                                            uint16_t year,
                                            sl_sleeptimer_month_t month,
                                            uint8_t month_day,
                                            uint8_t hour,
                                            uint8_t min,
                                            uint8_t sec,
                                            sl_sleeptimer_time_zone_offset_t tzOffset);

/***************************************************************************//**
 * @brief This function is used to convert a given Unix timestamp into an NTP
 * timestamp, which is useful for applications that require
 * synchronization with NTP servers. It should be called with a valid
 * Unix timestamp that falls within the supported range. The function
 * will return an error if the timestamp is out of range or if the
 * conversion results in an invalid NTP time. Ensure that the output
 * pointer is valid and writable before calling this function.
 *
 * @param time A Unix timestamp represented as a 32-bit unsigned integer. It
 * must be within the range from January 1, 1970, 00:00:00 to
 * February 7, 2036, 06:28:15. If the value exceeds this range, the
 * function returns an error.
 * @param ntp_time A pointer to a 32-bit unsigned integer where the converted
 * NTP timestamp will be stored. This pointer must not be null
 * and should point to a valid memory location that the caller
 * owns.
 * @return Returns SL_STATUS_OK if the conversion is successful. If the input
 * timestamp is invalid or out of range, it returns
 * SL_STATUS_INVALID_PARAMETER.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_unix_time_to_ntp(sl_sleeptimer_timestamp_t time,
                                                   uint32_t *ntp_time);

/***************************************************************************//**
 * @brief This function is used to convert a Network Time Protocol (NTP)
 * timestamp into a Unix timestamp, which represents the number of
 * seconds since January 1, 1970. It should be used when you need to
 * translate time data from NTP format to Unix format for compatibility
 * with Unix-based systems or applications. The function requires a valid
 * NTP timestamp within the supported range and a pointer to store the
 * resulting Unix timestamp. If the conversion is successful, the Unix
 * timestamp is stored at the provided pointer location. The function
 * returns an error status if the input is invalid or out of range.
 *
 * @param ntp_time The NTP timestamp to be converted. It must be within the
 * supported range of NTP timestamps, specifically from
 * 0x83AA7E80 to 0xFFFFFFFF, which corresponds to January 1,
 * 1970, to February 7, 2036.
 * @param time A pointer to a sl_sleeptimer_timestamp_t where the converted Unix
 * timestamp will be stored. This pointer must not be null, and the
 * caller is responsible for ensuring it points to a valid memory
 * location.
 * @return Returns SL_STATUS_OK if the conversion is successful, or
 * SL_STATUS_INVALID_PARAMETER if the input is invalid or out of range.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_ntp_time_to_unix(uint32_t ntp_time,
                                                   sl_sleeptimer_timestamp_t *time);

/***************************************************************************//**
 * @brief This function is used to convert a given Unix timestamp into a Zigbee
 * timestamp format. It is important to ensure that the input Unix
 * timestamp falls within the supported range, which is from January 1,
 * 2000, 00:00:00 to January 19, 2038, 03:14:00. The function will return
 * an error if the converted Zigbee timestamp is invalid. This function
 * should be used when there is a need to interface with systems or
 * protocols that utilize Zigbee time format.
 *
 * @param time A Unix timestamp represented as a 32-bit unsigned integer. It
 * must be within the range that can be converted to a valid Zigbee
 * timestamp.
 * @param zigbee_time A pointer to a 32-bit unsigned integer where the converted
 * Zigbee timestamp will be stored. Must not be null.
 * @return Returns SL_STATUS_OK if the conversion is successful and the Zigbee
 * timestamp is valid. Returns SL_STATUS_INVALID_PARAMETER if the
 * conversion results in an invalid Zigbee timestamp.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_unix_time_to_zigbee(sl_sleeptimer_timestamp_t time,
                                                      uint32_t *zigbee_time);

/***************************************************************************//**
 * @brief This function is used to convert a timestamp from the Zigbee format to
 * the Unix format. It is useful when working with systems that require
 * synchronization or interoperability between Zigbee and Unix time
 * representations. The function must be called with a valid Zigbee
 * timestamp, and it will return an error if the resulting Unix timestamp
 * is invalid. The conversion takes into account the offset between the
 * Zigbee epoch and the Unix epoch.
 *
 * @param zigbee_time A 32-bit unsigned integer representing the Zigbee
 * timestamp. It must be within the range that can be
 * converted to a valid Unix timestamp.
 * @param time A pointer to a sl_sleeptimer_timestamp_t where the converted Unix
 * timestamp will be stored. Must not be null.
 * @return Returns SL_STATUS_OK if the conversion is successful and the Unix
 * timestamp is valid. Returns SL_STATUS_INVALID_PARAMETER if the
 * resulting Unix timestamp is invalid.
 ******************************************************************************/
sl_status_t sl_sleeptimer_convert_zigbee_time_to_unix(uint32_t zigbee_time,
                                                      sl_sleeptimer_timestamp_t *time);

/***************************************************************************//**
 * @brief The function `sl_sleeptimer_set_tz_ahead_utc` calculates the time zone
 * offset in seconds for a time zone that is ahead of UTC.
 *
 * @param hours The number of hours ahead of UTC.
 * @param minutes The number of minutes ahead of UTC.
 * @return The function returns the time zone offset in seconds as an
 * `sl_sleeptimer_time_zone_offset_t` type.
 ******************************************************************************/
__STATIC_INLINE sl_sleeptimer_time_zone_offset_t sl_sleeptimer_set_tz_ahead_utc(uint8_t hours,
                                                                                uint8_t minutes)
{
  return ((hours * 3600u) + (minutes * 60u));
}

/***************************************************************************//**
 * @brief The function `sl_sleeptimer_set_tz_behind_utc` calculates the time
 * zone offset in seconds for a location behind UTC based on given hours
 * and minutes.
 *
 * @param hours The number of hours behind UTC.
 * @param minutes The number of minutes behind UTC.
 * @return The function returns the time zone offset in seconds as a negative
 * integer, indicating the time zone is behind UTC.
 ******************************************************************************/
__STATIC_INLINE sl_sleeptimer_time_zone_offset_t sl_sleeptimer_set_tz_behind_utc(uint8_t hours,
                                                                                 uint8_t minutes)
{
  return -(sl_sleeptimer_time_zone_offset_t)((hours * 3600u) + (minutes * 60u));
}

/***************************************************************************//**
 * @brief This function introduces an active delay for a specified duration in
 * milliseconds. It is useful when a precise delay is needed in the
 * execution flow. The function should be used in contexts where active
 * waiting is acceptable, as it will block the calling thread until the
 * delay period elapses. Ensure that the sleeptimer is properly
 * initialized before calling this function to avoid unexpected behavior.
 *
 * @param time_ms The duration of the delay in milliseconds. It is a 16-bit
 * unsigned integer, allowing a range from 0 to 65535
 * milliseconds. The function does not handle invalid values
 * explicitly, so ensure the input is within the valid range.
 * @return None
 ******************************************************************************/
void sl_sleeptimer_delay_millisecond(uint16_t time_ms);

/***************************************************************************//**
 * Converts milliseconds in ticks.
 *
 * @param time_ms Number of milliseconds.
 *
 * @return Corresponding ticks number.
 *
 * @note The result is "rounded" to the superior tick number.
 *       This function is light and cannot fail so it should be privilegied to
 *       perform a millisecond to tick conversion.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to convert a time duration specified in
 * milliseconds into the equivalent number of timer ticks, based on the
 * current timer frequency. It is useful when you need to work with
 * timer-based operations that require tick values instead of
 * milliseconds. The conversion rounds up to the nearest tick to ensure
 * the duration is not underestimated. This function is efficient and
 * cannot fail, making it preferable for simple millisecond to tick
 * conversions.
 *
 * @param time_ms The number of milliseconds to convert. It is a 16-bit unsigned
 * integer, allowing values from 0 to 65535. The function does
 * not handle invalid values as it assumes the input is within
 * the valid range.
 * @return Returns the equivalent number of timer ticks as a 32-bit unsigned
 * integer.
 ******************************************************************************/
uint32_t sl_sleeptimer_ms_to_tick(uint16_t time_ms);

/***************************************************************************//**
 * Converts 32-bits milliseconds in ticks.
 *
 * @param time_ms Number of milliseconds.
 * @param tick Pointer to the converted tick number.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  The result is "rounded" to the superior tick number.
 *        If possible the sl_sleeptimer_ms_to_tick() function should be used.
 *
 * @note  This function converts the delay expressed in milliseconds to timer
 *        ticks (represented on 32 bits). This means that the value that can
 *        be passed to the argument 'time_ms' is limited. The maximum
 *        timeout value that can be passed to this function can be retrieved
 *        by calling sl_sleeptimer_get_max_ms32_conversion().
 *        If the value passed to 'time_ms' is too large,
 *        SL_STATUS_INVALID_PARAMETER will be returned.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Use this function to convert a time duration specified in milliseconds
 * to the equivalent number of timer ticks, which is useful for
 * configuring timers in systems that operate on tick-based timing. The
 * function should be called with a valid millisecond value that does not
 * exceed the maximum allowable conversion value, which can be obtained
 * using sl_sleeptimer_get_max_ms32_conversion(). If the provided
 * millisecond value is too large, the function will return an error
 * status.
 *
 * @param time_ms The number of milliseconds to convert. Must be a 32-bit
 * unsigned integer and should not exceed the maximum conversion
 * limit. If the value is too large, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @param tick A pointer to a 32-bit unsigned integer where the converted tick
 * value will be stored. Must not be null, and the caller is
 * responsible for providing a valid memory location.
 * @return Returns SL_STATUS_OK if the conversion is successful, or
 * SL_STATUS_INVALID_PARAMETER if the input millisecond value is too
 * large.
 ******************************************************************************/
sl_status_t sl_sleeptimer_ms32_to_tick(uint32_t time_ms,
                                       uint32_t *tick);

/***************************************************************************//**
 * @brief This function returns the maximum number of milliseconds that can be
 * accurately converted to timer ticks using a 32-bit representation. It
 * is useful for determining the upper limit of millisecond values that
 * can be passed to functions like sl_sleeptimer_ms32_to_tick() without
 * causing an overflow or error. This function should be called when you
 * need to ensure that a millisecond value is within the allowable range
 * for 32-bit conversions.
 *
 * @return Returns the maximum allowable millisecond value for 32-bit conversion
 * as a uint32_t.
 ******************************************************************************/
uint32_t sl_sleeptimer_get_max_ms32_conversion(void);

/***************************************************************************//**
 * Converts ticks in milliseconds.
 *
 * @param tick Number of tick.
 *
 * @return Corresponding milliseconds number.
 *
 * @note The result is rounded to the inferior millisecond.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to convert a given number of timer ticks into
 * the equivalent duration in milliseconds. It is useful when you need to
 * translate timer-based events into real-time durations. The function
 * assumes that the timer frequency has been set and is non-zero. If the
 * timer frequency is zero, the function will return zero milliseconds.
 * This function should be used when you need to understand the real-time
 * duration represented by a specific number of ticks.
 *
 * @param tick The number of timer ticks to convert. It must be a non-negative
 * integer, and the conversion assumes a non-zero timer frequency.
 * @return Returns the equivalent duration in milliseconds as a 32-bit unsigned
 * integer. If the timer frequency is zero, the function returns zero.
 ******************************************************************************/
uint32_t sl_sleeptimer_tick_to_ms(uint32_t tick);

/***************************************************************************//**
 * Converts 64-bit ticks in milliseconds.
 *
 * @param tick Number of tick.
 * @param ms Pointer to the converted milliseconds number.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note The result is rounded to the inferior millisecond.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to convert a 64-bit tick count into
 * milliseconds, which is useful for applications that need to work with
 * time durations in a more human-readable format. It requires a valid
 * tick count and a non-null pointer to store the result in milliseconds.
 * The function will return an error if the tick count is too large to
 * convert safely or if the timer frequency is zero, indicating an
 * invalid state. It is important to ensure that the timer frequency is
 * set correctly before calling this function.
 *
 * @param tick A 64-bit unsigned integer representing the number of ticks to be
 * converted. It must be within a range that allows safe conversion
 * to milliseconds, specifically less than or equal to UINT64_MAX /
 * 1000.
 * @param ms A pointer to a 64-bit unsigned integer where the converted
 * milliseconds will be stored. This pointer must not be null, and the
 * caller is responsible for ensuring it points to valid memory.
 * @return Returns SL_STATUS_OK if the conversion is successful, or
 * SL_STATUS_INVALID_PARAMETER if the input is invalid or conversion
 * cannot be performed.
 ******************************************************************************/
sl_status_t sl_sleeptimer_tick64_to_ms(uint64_t tick,
                                       uint64_t *ms);

/***************************************************************************//**
 * Allow sleep after ISR exit.
 *
 * @return true if sleep is allowed after ISR exit. False otherwise.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function determines whether the power manager's early restore
 * timer is the most recent one set to expire. It is useful in scenarios
 * where precise power management and timing are critical, such as in
 * low-power applications. The function should be called when there is a
 * need to verify the status of the early restore timer in relation to
 * other timers. It operates atomically to ensure thread safety and
 * returns a boolean indicating the timer's status.
 *
 * @return Returns a boolean value: true if the early restore timer is the
 * latest to expire, false otherwise.
 ******************************************************************************/
bool sl_sleeptimer_is_power_manager_early_restore_timer_latest_to_expire(void);

/**************************************************************************//**
 * Starts a 32 bits timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout_ms Timer timeout, in milliseconds.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  This function converts the delay expressed in milliseconds to timer
 *        ticks (represented on 32 bits). This means that the value that can
 *        be passed to the argument 'timeout_ms' is limited. The maximum
 *        timeout value that can be passed to this function can be retrieved
 *        by calling sl_sleeptimer_get_max_ms32_conversion().
 *        If the value passed to 'timeout_ms' is too large,
 *        SL_STATUS_INVALID_PARAMETER will be returned.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The `sl_sleeptimer_start_timer_ms` function initializes and starts a
 * timer with a specified timeout in milliseconds, converting it to ticks
 * before starting the timer.
 *
 * @param handle A pointer to the timer handle structure that will be used to
 * manage the timer.
 * @param timeout_ms The timeout duration in milliseconds for which the timer
 * should run.
 * @param callback A function pointer to the callback function that will be
 * called when the timer expires.
 * @param callback_data A pointer to user data that will be passed to the
 * callback function.
 * @param priority The priority level of the callback, where 0 is the highest
 * priority.
 * @param option_flags A set of option flags that modify the behavior of the
 * timer, such as precision requirements.
 * @return The function returns an `sl_status_t` status code indicating success
 * (`SL_STATUS_OK`) or an error code if the operation fails.
 ******************************************************************************/
__STATIC_INLINE sl_status_t sl_sleeptimer_start_timer_ms(sl_sleeptimer_timer_handle_t *handle,
                                                         uint32_t timeout_ms,
                                                         sl_sleeptimer_timer_callback_t callback,
                                                         void *callback_data,
                                                         uint8_t priority,
                                                         uint16_t option_flags)
{
  sl_status_t status;
  uint32_t timeout_tick;

  status = sl_sleeptimer_ms32_to_tick(timeout_ms, &timeout_tick);
  if (status != SL_STATUS_OK) {
    return status;
  }

  return sl_sleeptimer_start_timer(handle, timeout_tick, callback, callback_data, priority, option_flags);
}

/**************************************************************************//**
 * Restarts a 32 bits timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout_ms Timer timeout, in milliseconds.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  This function converts the delay expressed in milliseconds to timer
 *        ticks (represented on 32 bits). This means that the value that can
 *        be passed to the argument 'timeout_ms' is limited. The maximum
 *        timeout value that can be passed to this function can be retrieved
 *        by calling sl_sleeptimer_get_max_ms32_conversion().
 *        If the value passed to 'timeout_ms' is too large,
 *        SL_STATUS_INVALID_PARAMETER will be returned.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The function `sl_sleeptimer_restart_timer_ms` restarts a 32-bit timer
 * with a specified timeout in milliseconds, converting it to ticks
 * before restarting the timer.
 *
 * @param handle A pointer to the timer handle that identifies the timer to be
 * restarted.
 * @param timeout_ms The timeout duration in milliseconds for the timer.
 * @param callback A callback function that will be called when the timer
 * expires.
 * @param callback_data A pointer to user data that will be passed to the
 * callback function.
 * @param priority The priority of the callback, where 0 is the highest
 * priority.
 * @param option_flags A bit array of option flags for the timer, which can
 * include flags like
 * SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG.
 * @return The function returns an `sl_status_t` status code, which is
 * `SL_STATUS_OK` if the timer was successfully restarted, or an error
 * code if the conversion or restart failed.
 ******************************************************************************/
__STATIC_INLINE sl_status_t sl_sleeptimer_restart_timer_ms(sl_sleeptimer_timer_handle_t *handle,
                                                           uint32_t timeout_ms,
                                                           sl_sleeptimer_timer_callback_t callback,
                                                           void *callback_data,
                                                           uint8_t priority,
                                                           uint16_t option_flags)
{
  sl_status_t status;
  uint32_t timeout_tick;

  status = sl_sleeptimer_ms32_to_tick(timeout_ms, &timeout_tick);
  if (status != SL_STATUS_OK) {
    return status;
  }

  return sl_sleeptimer_restart_timer(handle, timeout_tick, callback, callback_data, priority, option_flags);
}

/***************************************************************************//**
 * Starts a 32 bits periodic timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout_ms Timer periodic timeout, in milliseconds.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  This function converts the delay expressed in milliseconds to timer
 *        ticks (represented on 32 bits). This means that the value that can
 *        be passed to the argument 'timeout_ms' is limited. The maximum
 *        timeout value that can be passed to this function can be retrieved
 *        by calling sl_sleeptimer_get_max_ms32_conversion().
 *        If the value passed to 'timeout_ms' is too large,
 *        SL_STATUS_INVALID_PARAMETER will be returned.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to start a periodic timer that triggers a
 * callback function at regular intervals specified by the timeout in
 * milliseconds. It is essential to ensure that the timer handle is not
 * null and that the timer is not already running before calling this
 * function. The function converts the timeout from milliseconds to ticks
 * and handles any conversion errors internally. It is important to note
 * that this function cannot be called from an interrupt with a higher
 * priority than BASEPRI. The maximum timeout value is limited, and
 * exceeding it will result in an error.
 *
 * @param handle Pointer to the timer handle. Must not be null and should not
 * point to a running timer.
 * @param timeout_ms The periodic timeout interval in milliseconds. Must be
 * within the valid range; otherwise, an error is returned.
 * @param callback Function pointer to the callback that is called when the
 * timer expires. Must be a valid function.
 * @param callback_data Pointer to user data passed to the callback function.
 * Can be null if no data is needed.
 * @param priority Priority of the callback. Lower values indicate higher
 * priority. Must be a valid priority level.
 * @param option_flags Bit array of option flags for the timer. Can be a
 * combination of predefined flags or 0 for no flags.
 * @return Returns SL_STATUS_OK if the timer is successfully started, or an
 * error code if it fails.
 ******************************************************************************/
sl_status_t sl_sleeptimer_start_periodic_timer_ms(sl_sleeptimer_timer_handle_t *handle,
                                                  uint32_t timeout_ms,
                                                  sl_sleeptimer_timer_callback_t callback,
                                                  void *callback_data,
                                                  uint8_t priority,
                                                  uint16_t option_flags);

/***************************************************************************//**
 * Restarts a 32 bits periodic timer.
 *
 * @param handle Pointer to handle to timer.
 * @param timeout_ms Timer periodic timeout, in milliseconds.
 * @param callback Callback function that will be called when
 *        initial/periodic timeout expires.
 * @param callback_data Pointer to user data that will be passed to callback.
 * @param priority Priority of callback. Useful in case multiple timer expire
 *        at the same time. 0 = highest priority.
 * @param option_flags Bit array of option flags for the timer.
 *        Valid bit-wise OR of one or more of the following:
 *          - SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 *        or 0 for not flags.
 *
 * @return SL_STATUS_OK if successful. Error code otherwise.
 *
 * @note  This function converts the delay expressed in milliseconds to timer
 *        ticks (represented on 32 bits). This means that the value that can
 *        be passed to the argument 'timeout_ms' is limited. The maximum
 *        timeout value that can be passed to this function can be retrieved
 *        by calling sl_sleeptimer_get_max_ms32_conversion().
 *        If the value passed to 'timeout_ms' is too large,
 *        SL_STATUS_INVALID_PARAMETER will be returned.
 *
 * @note This function cannot be called from an interrupt with a higher
 *       priority than BASEPRI.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SLEEPTIMER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to restart a periodic timer with a specified
 * timeout in milliseconds. It should be called when you need to reset
 * the timer's countdown and ensure it continues to trigger at regular
 * intervals. The function requires a valid timer handle and a timeout
 * value that is within the permissible range, which can be checked using
 * sl_sleeptimer_get_max_ms32_conversion(). The callback function
 * provided will be called each time the timer expires, and the priority
 * determines the order of execution if multiple timers expire
 * simultaneously. This function cannot be called from an interrupt with
 * a higher priority than BASEPRI.
 *
 * @param handle Pointer to the timer handle. Must not be null. The caller
 * retains ownership.
 * @param timeout_ms The periodic timeout in milliseconds. Must be within the
 * range allowed by sl_sleeptimer_get_max_ms32_conversion().
 * If too large, SL_STATUS_INVALID_PARAMETER is returned.
 * @param callback Function pointer to the callback to be called when the timer
 * expires. Must not be null.
 * @param callback_data Pointer to user data passed to the callback function.
 * Can be null.
 * @param priority Priority of the callback. 0 is the highest priority.
 * Determines execution order if multiple timers expire
 * simultaneously.
 * @param option_flags Bit array of option flags for the timer. Can be a bitwise
 * OR of
 * SL_SLEEPTIMER_NO_HIGH_PRECISION_HF_CLOCKS_REQUIRED_FLAG
 * or 0 for no flags.
 * @return Returns SL_STATUS_OK if successful, or an error code otherwise.
 ******************************************************************************/
sl_status_t sl_sleeptimer_restart_periodic_timer_ms(sl_sleeptimer_timer_handle_t *handle,
                                                    uint32_t timeout_ms,
                                                    sl_sleeptimer_timer_callback_t callback,
                                                    void *callback_data,
                                                    uint8_t priority,
                                                    uint16_t option_flags);

/***************************************************************************//**
 * @brief This function is used to obtain the precision of the sleeptimer's
 * clock, expressed in parts per million (PPM). It is useful for
 * applications that require knowledge of the clock's accuracy for
 * precise timing operations. The function can be called at any time to
 * get the current clock accuracy, and it does not require any prior
 * initialization of the sleeptimer module.
 *
 * @return Returns a 16-bit unsigned integer representing the clock accuracy in
 * PPM.
 ******************************************************************************/
uint16_t sl_sleeptimer_get_clock_accuracy(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup sleeptimer) */

/* *INDENT-OFF* */
/* THE REST OF THE FILE IS DOCUMENTATION ONLY! */
/// @addtogroup sleeptimer Sleep Timer
/// @{
///
///   @details
///   Sleep Timer can be used for creating timers which are tightly integrated with power management.
///   The Power Manager requires precision timing to have all clocks ready on time, so that wakeup
///   happens a little bit earlier to prepare the system to be ready at the right time.
///   Sleep Timer uses one Hardware Timer and creates multiple software timer instances. It is important
///   to note that when sleeptimer is used with WTIMER/TIMER, the MCU cannot go to EM2 energy mode
///   because WTIMER/TIMER uses a high frequency clock source which is not retained in low energy mode.
///
///   The sleeptimer.c and sleeptimer.h source files for the SLEEPTIMER device driver library are in the
///   service/sleeptimer folder.
///
///   @n @section sleeptimer_intro Introduction
///
///   The Sleeptimer driver provides software timers, delays, timekeeping and date functionalities using a low-frequency real-time clock peripheral.
///
///   All Silicon Labs microcontrollers equipped with the RTC or RTCC peripheral are currently supported. Only one instance of this driver can be initialized by the application.
///
///   @n @section sleeptimer_functionalities_overview Functionalities overview
///
///   @n @subsection software_timers Software Timers
///
///   This functionality allows the user to create periodic and one shot timers. A user callback can be associated with a timer and is called when the timer expires.
///
///   Timer structures must be allocated by the user. The function is called from within an interrupt handler with interrupts enabled.
///
///   As sleeptimer callback functions are executed in ISR, they should be kept simple and short.
///   For periodic timers, the sleeptimer's callbacks need to be shorter than the timer's period to allow the application to exit the interrupt context.
///
///   @n @subsection timekeeping Timekeeping
///
///   A 64-bits tick counter is accessible through the @li uint64_t sl_sleeptimer_get_tick_count64(void) API. It keeps the tick count since the initialization of the driver
///
///   The `SL_SLEEPTIMER_WALLCLOCK_CONFIG` configuration enables a UNIX timestamp (seconds count since January 1, 1970, 00:00:00).
///
///   This timestamp can be retrieved/modified using the following API:
///
///   @li sl_sleeptimer_timestamp_t sl_sleeptimer_get_time(void);
///   @li sl_status_t sl_sleeptimer_set_time(sl_sleeptimer_timestamp_t time);
///
///   Convenience conversion functions are provided to convert UNIX timestamp to/from NTP and Zigbee cluster format :
///
///   @li sl_status_t sl_sleeptimer_convert_unix_time_to_ntp(sl_sleeptimer_timestamp_t time, uint32_t *ntp_time);
///   @li sl_status_t sl_sleeptimer_convert_ntp_time_to_unix(uint32_t ntp_time, sl_sleeptimer_timestamp_t *time);
///   @li sl_status_t sl_sleeptimer_convert_unix_time_to_zigbee(sl_sleeptimer_timestamp_t time, uint32_t *zigbee_time);
///   @li sl_status_t sl_sleeptimer_convert_zigbee_time_to_unix(uint32_t zigbee_time, sl_sleeptimer_timestamp_t *time);
///
///   @n @subsection date Date
///
///   The previously described internal timestamp can also be retrieved/modified in a date format sl_sleeptimer_date_t.
///
///   @n <b>API :</b> @n
///
///   @li sl_status_t sl_sleeptimer_get_datetime(sl_sleeptimer_date_t *date);
///   @li sl_status_t sl_sleeptimer_set_datetime(sl_sleeptimer_date_t *date);
///
///   @n @subsection frequency_setup Frequency Setup and Tick Count
///
///   This driver works with a configurable time unit called tick.
///
///   The frequency of the ticks is based on the clock source and the internal frequency divider.
///
///   WTIMER/TIMER peripherals uses high frequency oscillator. To have a reasonable tick frequency, divider is set to maximum value (1024).
///
///   One of the following clock sources must be enabled before initializing the sleeptimer:
///
///   @li LFXO: external crystal oscillator. Typically running at 32.768 kHz.
///   @li LFRCO: internal oscillator running at 32.768 kHz
///   @li ULFRCO: Ultra low-frequency oscillator running at 1.000 kHz
///   @li HFXO: High Frequency Crystal Oscillator at 39 Mhz. HFXO is only needed when Sleeptimer runs on TIMER or WTIMER.
///
///   The frequency divider is selected with the `SL_SLEEPTIMER_FREQ_DIVIDER` configuration. Its value must be a power of two within the range of 1 to 32. The number of ticks per second (sleeptimer frequency) is dictated by the following formula:
///
///   Tick (seconds) = 1 / (clock_frequency / frequency_divider)
///
///   The highest resolution for a tick is 30.5 us. It is achieved with a 32.768 kHz clock and a divider of 1.
///
///   @n @section sleeptimer_getting_started Getting Started
///
///   @n @subsection  clock_selection Clock Selection
///
///   The sleeptimer relies on the hardware timer to operate. The hardware timer peripheral must be properly clocked from the application. Selecting the appropriate timer is crucial for design considerations. Each timer can potentially be used as a sleeptimer and is also available to the user. However, note that if a timer is used by the sleeptimer, it can't be used by the application and vice versa.
///
///   For WTIMER/TIMER peripherals, the user must select the appropriate oscillator if it is not the default wanted clock source.
///
///   When WTIMER/TIMER is selected, sleeptimer uses channel 0 and it is not possible to use other channels of the same instance for other purposes.
///
///   When SYSRTC is chosen, the Peripheral Reflex System (PRS) channel 1 and 2 will be used by sleeptimer and become unavailable. PRS_GetFreeChannel() can be used to retrieve an unallocated channel.
///
///   @n @subsection  Clock Selection in a Project without Micrium OS
///
///   When RTC, RTCC, or BURTC is selected, the clock source for the peripheral must be configured and enabled in the application before initializing the sleeptimer module or any communication stacks. Most of the time, it consists in enabling the desired oscillators and setting up the clock source for the peripheral, like in the following example:
///
///   @code{.c}
///   CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFRCO);
///   CMU_ClockEnable(cmuClock_RTCC, true);
///   @endcode
///
///   @n @subsection  clock_branch_select Clock Branch Select
///
///   | Clock  | Enum                    | Description                       | Frequency |
///   |--------|-------------------------|-----------------------------------|-----------|
///   | LFXO   | <b>cmuSelect_LFXO</b>   | Low-frequency crystal oscillator  |32.768 Khz |
///   | LFRCO  | <b>cmuSelect_LFRCO</b>  | Low-frequency RC oscillator       |32.768 Khz |
///   | ULFRCO | <b>cmuSelect_ULFRCO</b> | Ultra low-frequency RC oscillator |1 Khz      |
///
///   @n @subsection  timer_clock_enable Timer Clock Enable
///
///   | Module             | Enum                  | Description                                        |
///   |--------------------|-----------------------|----------------------------------------------------|
///   | RTCC               | <b>cmuClock_RTCC</b>  | Real-time counter and calendar clock (LF E branch) |
///   | RTC                | <b>cmuClock_RTC</b>   | Real time counter clock (LF A branch)              |
///   | BURTC              | <b>cmuClock_BURTC</b> | BURTC clock (EM4 Group A branch)                   |
///
///   When the Radio internal RTC (PRORTC) is selected, it is not necessary to configure the clock source for the peripheral. However, it is important to enable the desired oscillator before initializing the sleeptimer module or any communication stacks. The best oscillator available (LFXO being the first choice) will be used by the sleeptimer at initalization. The following example shows how the desired oscilator should be enabled:
///
///   @code{.c}
///   CMU_OscillatorEnable(cmuSelect_LFXO, true, true);
///   @endcode
///
///   @n @subsection  clock_micrium_os Clock Selection in a Project with Micrium OS
///
///   When Micrium OS is used, a BSP (all instances) is provided that sets up some parts of the clock tree. The sleeptimer clock source will be enabled by this bsp. However, the desired oscillator remains configurable from the file <b>bsp_cfg.h</b>.
///
///   The configuration `BSP_LF_CLK_SEL` determines which oscillator will be used by the sleeptimer's hardware timer peripheral. It can take the following values:
///
///   | Config                   | Description                       | Frequency |
///   |--------------------------|-----------------------------------|-----------|
///   | <b>BSP_LF_CLK_LFXO</b>   | Low-frequency crystal oscillator  |32.768 Khz |
///   | <b>BSP_LF_CLK_LFRCO</b>  | Low-frequency RC oscillator       |32.768 Khz |
///   | <b>BSP_LF_CLK_ULFRCO</b> | Ultra low-frequency RC oscillator |1 Khz      |
///
///   @n @section sleeptimer_conf Configuration Options
///
///   `SL_SLEEPTIMER_PERIPHERAL` can be set to one of the following values:
///
///   | Config                            | Description                                                                                          |
///   | --------------------------------- |------------------------------------------------------------------------------------------------------|
///   | `SL_SLEEPTIMER_PERIPHERAL_DEFAULT`| Selects either RTC or RTCC, depending of what is available on the platform.                          |
///   | `SL_SLEEPTIMER_PERIPHERAL_RTCC`   | Selects RTCC                                                                                         |
///   | `SL_SLEEPTIMER_PERIPHERAL_RTC`    | Selects RTC                                                                                          |
///   | `SL_SLEEPTIMER_PERIPHERAL_PRORTC` | Selects Internal radio RTC. Available only on EFR32XG13, EFR32XG14, EFR32XG21 and EFR32XG22 families.|
///   | `SL_SLEEPTIMER_PERIPHERAL_BURTC`  | Selects BURTC. Not available on Series 0 devices.                                                    |
///
///   `SL_SLEEPTIMER_WALLCLOCK_CONFIG` must be set to 1 to enable timestamp and date functionnalities.
///
///   `SL_SLEEPTIMER_FREQ_DIVIDER` must be a power of 2 within the range 1 to 32. When `SL_SLEEPTIMER_PERIPHERAL` is set to `SL_SLEEPTIMER_PERIPHERAL_PRORTC`, `SL_SLEEPTIMER_FREQ_DIVIDER` must be set to 1.
///
///   `SL_SLEEPTIMER_PRORTC_HAL_OWNS_IRQ_HANDLER` is only meaningful when `SL_SLEEPTIMER_PERIPHERAL` is set to `SL_SLEEPTIMER_PERIPHERAL_PRORTC`. Set to 1 if no communication stack is used in your project. Otherwise, must be set to 0.
///
///   @n @section sleeptimer_api The API
///
///   This section contains brief descriptions of the API functions. For
///   more information about input and output parameters and return values,
///   click on the hyperlinked function names. Most functions return an error
///   code, `SL_STATUS_OK` is returned on success,
///   see sl_status.h for other error codes.
///
///   The application code must include the @em sl_sleeptimer.h header file.
///
///   All API functions can be called from within interrupt handlers.
///
///   @ref sl_sleeptimer_init() @n
///    These functions initialize the sleeptimer driver. Typically,
///     sl_sleeptimer_init() is called once in the startup code.
///
///   @ref sl_sleeptimer_start_timer() @n
///    Start a one shot 32 bits timer. When a timer expires, a user-supplied callback function
///    is called. A pointer to this function is passed to
///     sl_sleeptimer_start_timer(). See @ref sl_sleeptimer_timer_callback_t for
///    details of the callback prototype.
///
///   @ref sl_sleeptimer_restart_timer() @n
///    Restart a one shot 32 bits timer. When a timer expires, a user-supplied callback function
///    is called. A pointer to this function is passed to
///     sl_sleeptimer_start_timer(). See @ref sl_sleeptimer_timer_callback_t for
///    details of the callback prototype.
///
///   @ref sl_sleeptimer_start_periodic_timer() @n
///    Start a periodic 32 bits timer. When a timer expires, a user-supplied callback function
///    is called. A pointer to this function is passed to
///     sl_sleeptimer_start_timer(). See @ref sl_sleeptimer_timer_callback_t for
///    details of the callback prototype.
///
///   @ref sl_sleeptimer_restart_periodic_timer() @n
///    Restart a periodic 32 bits timer. When a timer expires, a user-supplied callback function
///    is called. A pointer to this function is passed to
///     sl_sleeptimer_start_timer(). See @ref sl_sleeptimer_timer_callback_t for
///    details of the callback prototype.
///
///   @ref sl_sleeptimer_stop_timer() @n
///    Stop a timer.
///
///   @ref sl_sleeptimer_get_timer_time_remaining() @n
///    Get the time remaining before the timer expires.
///
///   @ref sl_sleeptimer_delay_millisecond() @n
///    Delay for the given number of milliseconds. This is an "active wait" delay function.
///
///   @ref sl_sleeptimer_is_timer_running() @n
///    Check if a timer is running.
///
///   @ref sl_sleeptimer_get_time(), @ref sl_sleeptimer_set_time() @n
///    Get or set wallclock time.
///
///   @ref sl_sleeptimer_ms_to_tick(), @ref sl_sleeptimer_ms32_to_tick(),
///   @ref sl_sleeptimer_tick_to_ms(), @ref sl_sleeptimer_tick64_to_ms() @n
///    Convert between milliseconds and RTC/RTCC
///    counter ticks.
///
///   @n @anchor callback <b>The timer expiry callback function:</b> @n
///   The callback function, prototyped as @ref sl_sleeptimer_timer_callback_t(), is called from
///   within the RTC peripheral interrupt handler on timer expiration.
///    sl_sleeptimer_timer_callback_t(sl_sleeptimer_timer_handle_t *handle, void *data)
///
///   @n @section sleeptimer_example Example
///   @code{.c}
///#include "sl_sleeptimer.h"
///
///void my_timer_callback(sl_sleeptimer_timer_handle_t *handle, void *data)
///{
///  //Code executed when the timer expire.
///}
///
///int start_timer(void)
///{
///  sl_status_t status;
///  sl_sleeptimer_timer_handle_t my_timer;
///  uint32_t timer_timeout = 300;
///
///  // We assume the sleeptimer is initialized properly
///
///  status = sl_sleeptimer_start_timer(&my_timer,
///                                     timer_timeout,
///                                     my_timer_callback,
///                                     (void *)NULL,
///                                     0,
///                                     0);
///  if(status != SL_STATUS_OK) {
///    return -1;
///  }
///  return 1;
///}
///   @endcode
///
/// @} (end addtogroup sleeptimer)

#endif // SL_SLEEPTIMER_H
