/***************************************************************************//**
 * @file
 * @brief KEYSCAN driver API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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
 *
 * @addtogroup keyscan_driver KEYSCAN - Keypad Scanner Driver
 * @brief Keyscan driver.
 *
 * @{
 *
 *****************************************************************************/

#ifndef KEYSCAN_DRIVER_H
#define KEYSCAN_DRIVER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "em_device.h"
#include "sl_slist.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *********************************   ENUM   ************************************
 ******************************************************************************/

/// KEYSCAN status values.
/***************************************************************************//**
 * @brief The `sl_keyscan_driver_status_t` is an enumeration that defines the
 * possible status values for a keypress event in the KEYSCAN driver. It
 * includes statuses for valid keypresses, invalid keypresses, and key
 * releases, which are used to manage and interpret the state of keypress
 * events within the key scanning process.
 *
 * @param SL_KEYSCAN_STATUS_KEYPRESS_VALID Indicates a valid keypress has been
 * detected.
 * @param SL_KEYSCAN_STATUS_KEYPRESS_INVALID Indicates an invalid keypress has
 * been detected.
 * @param SL_KEYSCAN_STATUS_KEYPRESS_RELEASED Indicates that a key has been
 * released.
 ******************************************************************************/
typedef enum {
  SL_KEYSCAN_STATUS_KEYPRESS_VALID = 0,
  SL_KEYSCAN_STATUS_KEYPRESS_INVALID,
  SL_KEYSCAN_STATUS_KEYPRESS_RELEASED
} sl_keyscan_driver_status_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/// A Keyscan event handler
typedef void (*sl_keyscan_driver_process_keyscan_event)(uint8_t  *p_keyscan_matrix,
                                                        sl_keyscan_driver_status_t status);

/// An Event Handle
/***************************************************************************//**
 * @brief The `sl_keyscan_driver_process_keyscan_event_handle_t` is a structure
 * used in the KEYSCAN driver to manage event handling. It contains a
 * list node, `node`, which allows the structure to be part of a linked
 * list, and a function pointer, `on_event`, which is called when a
 * keyscan event occurs. This structure is essential for registering and
 * managing callbacks that respond to keyscan events, enabling the driver
 * to handle keypresses efficiently.
 *
 * @param node List node.
 * @param on_event Function that must be called when the event occurs.
 ******************************************************************************/
typedef struct {
  sl_slist_node_t node;                              ///< List node.
  sl_keyscan_driver_process_keyscan_event on_event;  ///< Function that must be called when the event occurs.
} sl_keyscan_driver_process_keyscan_event_handle_t;

/***************************************************************************//**
 * @brief This function prepares the keyscan driver for operation by setting up
 * necessary GPIO configurations, initializing hardware components, and
 * enabling relevant interrupts. It should be called before any other
 * keyscan operations to ensure the driver is properly configured. This
 * function does not take any parameters and returns a status code
 * indicating success or failure. It is essential to call this function
 * before starting a keyscan or registering event callbacks.
 *
 * @return Returns SL_STATUS_OK if the initialization is successful, otherwise
 * an error code.
 ******************************************************************************/
sl_status_t sl_keyscan_driver_init(void);

/***************************************************************************//**
 * @brief This function registers an event callback that will be invoked when a
 * keyscan event occurs. It should be called before initializing the
 * keyscan driver to ensure that events are captured from the start. The
 * function requires a valid event handle with a non-null callback
 * function. It is important to ensure that the event handle is properly
 * initialized and not null, as the function will assert if these
 * conditions are not met. This function is typically used in
 * applications where keyscan events need to be processed asynchronously.
 *
 * @param event_handle A pointer to an
 * sl_keyscan_driver_process_keyscan_event_handle_t
 * structure that contains the event callback function. The
 * event_handle must not be null, and the on_event member of
 * the structure must also be non-null. The caller retains
 * ownership of the event handle.
 * @return None
 ******************************************************************************/
void sl_keyscan_driver_subscribe_event(sl_keyscan_driver_process_keyscan_event_handle_t  *event_handle);

/***************************************************************************//**
 * @brief Use this function to remove an event callback that was previously
 * registered with the keyscan driver. This is necessary when the
 * callback is no longer needed or before deinitializing the driver to
 * prevent further event handling. The function must be called with a
 * valid event handle that was previously registered; otherwise, an
 * assertion will be triggered. Ensure that the event handle is not null
 * before calling this function.
 *
 * @param event_handle A pointer to the event handle that needs to be
 * unregistered. It must not be null and should have been
 * previously registered with the keyscan driver. If the
 * handle is null or not found, an assertion is triggered.
 * @return None
 ******************************************************************************/
void sl_keyscan_driver_unsubscribe_event(sl_keyscan_driver_process_keyscan_event_handle_t  *event_handle);

/***************************************************************************//**
 * @brief This function initiates the keyscan scanning process if it is not
 * already running. It is typically called after initializing the keyscan
 * driver and registering any necessary event callbacks. The function
 * ensures that a scan is not started if one is already in progress, thus
 * preventing redundant operations. It is important to ensure that the
 * keyscan driver has been properly initialized before calling this
 * function to avoid undefined behavior.
 *
 * @return Returns SL_STATUS_OK if the scan is successfully started or if it was
 * already running.
 ******************************************************************************/
sl_status_t sl_keyscan_driver_start_scan(void);

/***************************************************************************//**
 * @brief Use this function to stop an ongoing keyscan operation if it is
 * currently running. This function should be called when you need to
 * halt the key scanning process, such as when shutting down the system
 * or when the scanning is no longer needed. It checks if a scan is in
 * progress and stops it if necessary, ensuring that the keyscan matrix
 * is cleared. This function is safe to call even if no scan is currently
 * running, in which case it will simply return without making changes.
 *
 * @return Returns SL_STATUS_OK if the operation is successful or if no scan was
 * running.
 ******************************************************************************/
sl_status_t sl_keyscan_driver_stop_scan(void);

/** @} (end group keyscan_driver) */

/* *INDENT-OFF* */
/* THE REST OF THE FILE IS DOCUMENTATION ONLY! */
/// @addtogroup keyscan_driver
/// @{
///
///   @details
///
///
///   @n @section keyscan_intro Introduction
///
///   A low-energy keypad scanner (KEYSCAN) which can scan up to a 6x8 matrix of keyboard switches.
///   The KEYSCAN peripheral contains logic for debounce and settling time, allowing it to scan through
///   the switch matrix autonomously in EM0 and EM1, and interrupt the processor when a key press is detected.
///
///   @n @section keyscan_configuration Configuration Options
///
///   Some properties of the KEYSCAN driver are compile-time configurable.
///   These properties are stored in a file named keyscan_driver_config.h.
///   A template for this file, containing default values, is in the keyscan/config folder.
///   Currently the configuration options are as follows:
///
///   @li Number of columns in keyscan.
///   @li Number of rows in keyscan.
///   @li Duration of the scan period per column.
///   @li Duration of debounce period once a key press is detected.
///   @li Duration of the stable period after the debounce stage.
///   @li Enable or disable single-press functionality.
///
///   To configure KEYSCAN, provide a custom configuration file. This is an example keyscan_driver_config.h file:
///   @code{.c}
/// #ifndef KEYSCAN_DRIVER_CONFIG_H
/// #define KEYSCAN_DRIVER_CONFIG_H
///
/// // <o SL_KEYSCAN_DRIVER_COLUMN_NUMBER> Number of columns in keyscan <1-8>
/// // <i> Default: 1
/// #define SL_KEYSCAN_DRIVER_COLUMN_NUMBER 1
///
/// // <o SL_KEYSCAN_DRIVER_ROW_NUMBER> Number of rows in keyscan <3-6>
/// // <i> Default: 3
/// #define SL_KEYSCAN_DRIVER_ROW_NUMBER    3
///
/// // <o SL_KEYSCAN_DRIVER_SCAN_DELAY_MS> Scan Delay
/// //    <SL_HAL_KEYSCAN_DELAY_2MS=>     2ms
/// //    <SL_HAL_KEYSCAN_DELAY_4MS=>     4ms
/// //    <SL_HAL_KEYSCAN_DELAY_6MS=>     6ms
/// //    <SL_HAL_KEYSCAN_DELAY_8MS=>     8ms
/// //    <SL_HAL_KEYSCAN_DELAY_10MS=>    10ms
/// //    <SL_HAL_KEYSCAN_DELAY_12MS=>    12ms
/// //    <SL_HAL_KEYSCAN_DELAY_14MS=>    14ms
/// //    <SL_HAL_KEYSCAN_DELAY_16MS=>    16ms
/// //    <SL_HAL_KEYSCAN_DELAY_18MS=>    18ms
/// //    <SL_HAL_KEYSCAN_DELAY_20MS=>    20ms
/// //    <SL_HAL_KEYSCAN_DELAY_22MS=>    22ms
/// //    <SL_HAL_KEYSCAN_DELAY_24MS=>    24ms
/// //    <SL_HAL_KEYSCAN_DELAY_26MS=>    26ms
/// //    <SL_HAL_KEYSCAN_DELAY_28MS=>    28ms
/// //    <SL_HAL_KEYSCAN_DELAY_30MS=>    30ms
/// //    <SL_HAL_KEYSCAN_DELAY_32MS=>    32ms
/// // <i> Duration of the scan period per column
/// // <i> Default: SL_HAL_KEYSCAN_DELAY_2MS
/// #define SL_KEYSCAN_DRIVER_SCAN_DELAY_MS    SL_HAL_KEYSCAN_DELAY_2MS
///
/// // <o SL_KEYSCAN_DRIVER_DEBOUNCE_DELAY_MS> Debounce Delay
/// //    <SL_HAL_KEYSCAN_DELAY_2MS=>     2ms
/// //    <SL_HAL_KEYSCAN_DELAY_4MS=>     4ms
/// //    <SL_HAL_KEYSCAN_DELAY_6MS=>     6ms
/// //    <SL_HAL_KEYSCAN_DELAY_8MS=>     8ms
/// //    <SL_HAL_KEYSCAN_DELAY_10MS=>    10ms
/// //    <SL_HAL_KEYSCAN_DELAY_12MS=>    12ms
/// //    <SL_HAL_KEYSCAN_DELAY_14MS=>    14ms
/// //    <SL_HAL_KEYSCAN_DELAY_16MS=>    16ms
/// //    <SL_HAL_KEYSCAN_DELAY_18MS=>    18ms
/// //    <SL_HAL_KEYSCAN_DELAY_20MS=>    20ms
/// //    <SL_HAL_KEYSCAN_DELAY_22MS=>    22ms
/// //    <SL_HAL_KEYSCAN_DELAY_24MS=>    24ms
/// //    <SL_HAL_KEYSCAN_DELAY_26MS=>    26ms
/// //    <SL_HAL_KEYSCAN_DELAY_28MS=>    28ms
/// //    <SL_HAL_KEYSCAN_DELAY_30MS=>    30ms
/// //    <SL_HAL_KEYSCAN_DELAY_32MS=>    32ms
/// // <i> Duration of debounce period once a key press is detected.
/// // <i> Default: SL_HAL_KEYSCAN_DELAY_2MS
/// #define SL_KEYSCAN_DRIVER_DEBOUNCE_DELAY_MS    SL_HAL_KEYSCAN_DELAY_2MS
///
/// // <o SL_KEYSCAN_DRIVER_STABLE_DELAY_MS> Stable Delay
/// //    <SL_HAL_KEYSCAN_DELAY_2MS=>     2ms
/// //    <SL_HAL_KEYSCAN_DELAY_4MS=>     4ms
/// //    <SL_HAL_KEYSCAN_DELAY_6MS=>     6ms
/// //    <SL_HAL_KEYSCAN_DELAY_8MS=>     8ms
/// //    <SL_HAL_KEYSCAN_DELAY_10MS=>    10ms
/// //    <SL_HAL_KEYSCAN_DELAY_12MS=>    12ms
/// //    <SL_HAL_KEYSCAN_DELAY_14MS=>    14ms
/// //    <SL_HAL_KEYSCAN_DELAY_16MS=>    16ms
/// //    <SL_HAL_KEYSCAN_DELAY_18MS=>    18ms
/// //    <SL_HAL_KEYSCAN_DELAY_20MS=>    20ms
/// //    <SL_HAL_KEYSCAN_DELAY_22MS=>    22ms
/// //    <SL_HAL_KEYSCAN_DELAY_24MS=>    24ms
/// //    <SL_HAL_KEYSCAN_DELAY_26MS=>    26ms
/// //    <SL_HAL_KEYSCAN_DELAY_28MS=>    28ms
/// //    <SL_HAL_KEYSCAN_DELAY_30MS=>    30ms
/// //    <SL_HAL_KEYSCAN_DELAY_32MS=>    32ms
/// // <i> Duration of the stable period after the debounce stage.
/// // <i> Default: SL_HAL_KEYSCAN_DELAY_2MS
/// // #define SL_KEYSCAN_DRIVER_STABLE_DELAY_MS    SL_HAL_KEYSCAN_DELAY_2MS
/// //
/// // <q SL_KEYSCAN_DRIVER_SINGLEPRESS> keyscan single-press functionality
/// // <i> Enable or disable single-press functionality.
/// // <i> Default: 0
/// #define SL_KEYSCAN_DRIVER_SINGLEPRESS  0
///
/// #endif /* KEYSCAN_DRIVER_CONFIG_H */
///   @endcode
///
///   @n @section keyscan_usage Usage
///
///   The expected flow of execution is:
///
///   @li Initialize the driver.
///   @li Register an event callback to be called on Keyscan event.
///   @li Start the scan.
///   @li Interrupt-based event to read the keypresses.
///
///   Initialization is done by calling @ref sl_keyscan_driver_init(). It will
///   prepare the internal structure and GPIO configuration.
///
///   When initialization is done successfully, scanning can be started by calling
///   @ref sl_keyscan_driver_start_scan().
///
///
///   Here is a complete example:
///
///   @code{.c}
/// #include "keyscan_driver.h"
///
/// /*******************************************************************************
///  *********************   LOCAL FUNCTION PROTOTYPES   ***************************
///  ******************************************************************************/
///
/// static void on_event(uint8_t *p_keyscan_matrix, sl_keyscan_driver_status_t status);
/// static sl_keyscan_driver_process_keyscan_event_handle_t handle =
/// {
///   .on_event = on_event,
/// };
///
/// /*******************************************************************************
///  **************************   GLOBAL FUNCTIONS   *******************************
///  ******************************************************************************/
/// static void on_event(uint8_t *p_keyscan_matrix, sl_keyscan_driver_status_t status)
/// {
///   (void)p_keyscan_matrix;
///
///   switch (status) {
///     // Singlepress mode: key press detected.
///     case SL_KEYSCAN_STATUS_KEYPRESS_VALID:
///       break;
///     // Multipress mode: all keys are released.
///     case SL_KEYSCAN_STATUS_KEYPRESS_RELEASED:
///       break;
///     default:
///       EFM_ASSERT(false);
///       break;
///   }
/// }
///
/// int main( void )
/// {
///   // Initialize KEYSCAN.
///   sl_keyscan_driver_init();
///   // Set callback to be called when processing interrupt
///   sl_keyscan_driver_subscribe_event(&handle);
///   // Start the keypad scanner.
///   sl_keyscan_driver_start_scan();
///
///   return 0;
/// }
///   @endcode
///
/// @} (end addtogroup keyscan_driver)

#ifdef __cplusplus
}
#endif

#endif /* KEYSCAN_DRIVER_H */
