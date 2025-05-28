/***************************************************************************//**
 * @file
 * @brief KEYSCAN driver HAL definition.
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

#ifndef KEYSCAN_DRIVER_HAL_H
#define KEYSCAN_DRIVER_HAL_H

#include <stdint.h>
#include <stdbool.h>
#include <em_device.h>

/*******************************************************************************
 *******************************   DEFINES  ************************************
 ******************************************************************************/

// Maximum number of columns and rows given by hardware
#define KEYSCAN_MAX_NUMBER_COLUMN  KEYSCAN_COLNUM
#define KEYSCAN_MAX_NUMBER_ROW     KEYSCAN_ROWNUM

// Set of internal event flags corresponding to the interrupt flags
#define KEYSCAN_DRIVER_EVENT_WAKEUP  (0x01)
#define KEYSCAN_DRIVER_EVENT_SCANNED (0x02)
#define KEYSCAN_DRIVER_EVENT_KEY     (0x04)
#define KEYSCAN_DRIVER_EVENT_NOKEY   (0x08)

/*******************************************************************************
 *****************************   PROTOTYPES  ***********************************
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function sets up the Keyscan peripheral by configuring its
 * hardware parameters and enabling necessary interrupts. It should be
 * called during the system initialization phase before any key scanning
 * operations are performed. The function configures the hardware with
 * predefined settings, initializes the clock, and ensures that
 * interrupts are properly set up and enabled. It does not take any
 * parameters and does not return a value, making it straightforward to
 * use as part of the initialization sequence.
 *
 * @return None
 ******************************************************************************/
void sli_keyscan_driver_hal_init(void);

/***************************************************************************//**
 * @brief This function configures the GPIO pins used by the Keyscan peripheral,
 * setting them up for both column output and row sensing based on the
 * defined number of columns and rows. It must be called before starting
 * any key scanning operations to ensure that the GPIOs are correctly
 * configured for the Keyscan hardware. The function handles the routing
 * and mode setting for each column and row pin, ensuring they are ready
 * for key scanning. It does not take any parameters and does not return
 * any value, making it a straightforward initialization step in the
 * Keyscan setup process.
 *
 * @return None
 ******************************************************************************/
void sli_keyscan_driver_hal_init_gpio(void);

/***************************************************************************//**
 * @brief This function is used to enable specific Keyscan interrupts by passing
 * a set of flags that correspond to different interrupt events. It
 * should be called when you need to configure the Keyscan peripheral to
 * respond to certain events, such as wakeup, key press, no key press, or
 * scan completion. The function expects a valid combination of
 * predefined event flags and will enable the corresponding interrupts.
 * It is important to ensure that the Keyscan peripheral is properly
 * initialized before calling this function to avoid undefined behavior.
 *
 * @param local_flags A set of interrupt flags represented as a bitmask. Valid
 * flags include KEYSCAN_DRIVER_EVENT_WAKEUP,
 * KEYSCAN_DRIVER_EVENT_SCANNED, KEYSCAN_DRIVER_EVENT_KEY,
 * and KEYSCAN_DRIVER_EVENT_NOKEY. The caller retains
 * ownership, and the parameter must be a valid combination
 * of these flags.
 * @return None
 ******************************************************************************/
void sli_keyscan_driver_hal_enable_interrupts(uint8_t local_flags);

/***************************************************************************//**
 * @brief This function is used to obtain the column and row information from
 * the keyscan peripheral when a key is detected as pressed. It should be
 * called when a key press event is expected or needs to be processed.
 * The function writes the column and row data to the provided pointers,
 * which must not be null. The row data is inverted to reflect the key
 * press status accurately. Ensure that the keyscan peripheral is
 * properly initialized and configured before calling this function.
 *
 * @param p_column Pointer to a uint8_t variable where the column information
 * will be stored. Must not be null.
 * @param p_row Pointer to a uint8_t variable where the row information will be
 * stored. Must not be null.
 * @return None
 ******************************************************************************/
void sli_keyscan_driver_hal_get_column_row(uint8_t *p_column,
                                           uint8_t *p_row);

/***************************************************************************//**
 * @brief Use this function to check whether the keyscan scanning process is
 * actively running. This can be useful for determining the current state
 * of the keyscan peripheral, especially before starting or stopping a
 * scan. The function returns a boolean value indicating the scan status,
 * where 'true' means the scan is running and 'false' means it is not. It
 * is important to ensure that the keyscan peripheral has been properly
 * initialized before calling this function to get accurate results.
 *
 * @return Returns 'true' if the keyscan scan is running, otherwise returns
 * 'false'.
 ******************************************************************************/
bool sli_keyscan_driver_hal_is_scan_running(void);

/***************************************************************************//**
 * @brief This function initiates the keyscan scanning process, optionally
 * enabling the keyscan module if specified. It should be used when you
 * want to start scanning for key presses on the keyscan peripheral. The
 * function ensures synchronization by waiting for the scan to complete.
 * It is important to call this function only after the keyscan
 * peripheral has been properly initialized. The function does not handle
 * invalid input explicitly, so the caller must ensure that the input is
 * valid.
 *
 * @param enable A boolean value indicating whether to enable the keyscan module
 * before starting the scan. If true, the keyscan module is
 * enabled; if false, the module is not enabled. The caller
 * retains ownership of this parameter.
 * @return None
 ******************************************************************************/
void sli_keyscan_driver_hal_start_scan(bool enable);

/***************************************************************************//**
 * @brief Use this function to halt the ongoing keyscan scanning process. It
 * should be called when you need to stop scanning for key presses, such
 * as when shutting down the system or when the scanning is no longer
 * required. This function ensures that the scanning process is
 * completely stopped and the keyscan module is disabled. It is important
 * to ensure that the keyscan module is currently running before calling
 * this function to avoid unnecessary operations.
 *
 * @return None
 ******************************************************************************/
void sli_keyscan_driver_hal_stop_scan(void);

/***************************************************************************//**
 * @brief Use this function to handle keyscan events triggered by interrupts. It
 * processes different types of events such as wake-up, key press
 * detection, all keys released, and scan completion without key presses.
 * This function should be called whenever a keyscan interrupt occurs,
 * with the appropriate event flags passed as an argument. It is
 * essential for managing the state of the keyscan matrix and invoking
 * registered event handlers based on the current mode (single-press or
 * multi-press). Ensure that the keyscan driver is properly initialized
 * and interrupts are enabled before calling this function.
 *
 * @param local_flags A set of event flags indicating which keyscan events to
 * process. Valid flags include KEYSCAN_DRIVER_EVENT_WAKEUP,
 * KEYSCAN_DRIVER_EVENT_SCANNED, KEYSCAN_DRIVER_EVENT_KEY,
 * and KEYSCAN_DRIVER_EVENT_NOKEY. The function will handle
 * each event type accordingly based on the flags set.
 * @return None
 ******************************************************************************/
void sli_keyscan_process_irq(uint8_t local_flags);

/***************************************************************************//**
 * @brief The function `sli_keyscan_driver_hal_is_autostart_enabled` checks if
 * the auto-start feature of the keyscan hardware is enabled.
 *
 * @return The function returns a boolean value: `true` if the auto-start
 * feature is enabled, and `false` otherwise.
 ******************************************************************************/
__STATIC_INLINE bool sli_keyscan_driver_hal_is_autostart_enabled(void)
{
  return (KEYSCAN->CFG & _KEYSCAN_CFG_AUTOSTART_MASK) ? true : false;
}

/***************************************************************************//**
 * @brief The function `sli_keyscan_driver_hal_is_singlepress_enabled` checks if
 * the single-press feature is enabled in the keyscan configuration.
 *
 * @return The function returns a boolean value: `true` if the single-press
 * feature is enabled, and `false` if it is not.
 ******************************************************************************/
__STATIC_INLINE bool sli_keyscan_driver_hal_is_singlepress_enabled(void)
{
  return (KEYSCAN->CFG & _KEYSCAN_CFG_SINGLEPRESS_MASK) ? true : false;
}

#ifdef __cplusplus
}
#endif

#endif // KEYSCAN_DRIVER_HAL_H
