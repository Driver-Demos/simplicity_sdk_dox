/***************************************************************************//**
 * @file
 * @brief LED Driver
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

#ifndef SL_LED_H
#define SL_LED_H

#include <stdint.h>
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup led LED Driver
 * @brief Generic LED Driver
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/

#define SL_LED_CURRENT_STATE_OFF  0U            ///< LED state off
#define SL_LED_CURRENT_STATE_ON   1U            ///< LED state on

/*******************************************************************************
 *****************************   DATA TYPES   **********************************
 ******************************************************************************/

typedef uint8_t sl_led_state_t;                 ///< LED state

/// A LED instance
/***************************************************************************//**
 * @brief The `sl_led_t` structure is a data type used to represent an LED
 * instance in a generic LED driver. It contains a context pointer for
 * storing instance-specific data and function pointers for initializing
 * the LED, turning it on and off, toggling its state, and retrieving its
 * current state. This structure allows for flexible and platform-
 * independent control of LED hardware by abstracting the LED operations
 * into function pointers that can be implemented for different hardware
 * configurations.
 *
 * @param context The context for this LED instance.
 * @param init Member function to initialize LED instance.
 * @param turn_on Member function to turn on LED.
 * @param turn_off Member function to turn off LED.
 * @param toggle Member function to toggle LED.
 * @param get_state Member function to retrieve LED state.
 ******************************************************************************/
typedef struct {
  void           *context;                        ///< The context for this LED instance
  sl_status_t    (*init)(void *context);          ///< Member function to initialize LED instance
  void           (*turn_on)(void *context);       ///< Member function to turn on LED
  void           (*turn_off)(void *context);      ///< Member function to turn off LED
  void           (*toggle)(void *context);        ///< Member function to toggle LED
  sl_led_state_t (*get_state)(void *context);     ///< Member function to retrieve LED state
} sl_led_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function initializes an LED instance, preparing it for subsequent
 * operations such as turning on, off, or toggling. It must be called
 * before any other operations on the LED to ensure proper setup of the
 * LED's GPIO, mode, and polarity. The function requires a valid LED
 * handle, which should be properly configured and not null. If the
 * initialization is successful, the function returns a status code
 * indicating success.
 *
 * @param led_handle Pointer to an sl_led_t instance representing the LED to
 * initialize. Must not be null and should be properly
 * configured before calling this function. The caller retains
 * ownership of the memory.
 * @return Returns an sl_status_t status code indicating the success or failure
 * of the initialization process.
 ******************************************************************************/
sl_status_t sl_led_init(const sl_led_t *led_handle);

/***************************************************************************//**
 * @brief Use this function to turn on an LED that has been previously
 * initialized. It is essential to ensure that the LED instance,
 * represented by the `led_handle`, has been properly initialized using
 * `sl_led_init` before calling this function. This function does not
 * return a status code, so it is assumed that the operation will succeed
 * if the preconditions are met. The function does not handle null
 * pointers, so the caller must ensure that `led_handle` is not null to
 * avoid undefined behavior.
 *
 * @param led_handle Pointer to an `sl_led_t` instance representing the LED to
 * be turned on. Must not be null and should point to a valid,
 * initialized LED instance. The caller retains ownership of
 * the pointer.
 * @return None
 ******************************************************************************/
void sl_led_turn_on(const sl_led_t *led_handle);

/***************************************************************************//**
 * @brief Use this function to turn off an LED that has been previously
 * initialized. It is essential to ensure that the LED has been properly
 * initialized using `sl_led_init` before calling this function. This
 * function is part of a generic LED driver and operates on an LED
 * instance specified by the `led_handle` parameter. The function does
 * not return a value and does not provide feedback on the success of the
 * operation, so it is assumed that the LED will be turned off if the
 * `led_handle` is valid.
 *
 * @param led_handle Pointer to an `sl_led_t` instance representing the LED to
 * be turned off. This parameter must not be null and should
 * point to a valid LED instance that has been initialized. If
 * the pointer is invalid or null, the behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_led_turn_off(const sl_led_t *led_handle);

/***************************************************************************//**
 * @brief Use this function to change the state of an LED from on to off or from
 * off to on. It is essential to ensure that the LED has been properly
 * initialized using `sl_led_init` before calling this function. This
 * function is useful in scenarios where you need to alternate the LED
 * state, such as in blinking patterns or status indicators. The function
 * does not return a value and assumes that the provided LED handle is
 * valid and correctly configured.
 *
 * @param led_handle Pointer to an instance of sl_led_t representing the LED to
 * toggle. Must not be null and should point to a valid,
 * initialized LED instance. The function does not perform
 * null checks, so passing an invalid pointer may result in
 * undefined behavior.
 * @return None
 ******************************************************************************/
void sl_led_toggle(const sl_led_t *led_handle);

/***************************************************************************//**
 * @brief Use this function to determine whether a specific LED is currently on
 * or off. It is essential to ensure that the LED has been properly
 * initialized using `sl_led_init` before calling this function. This
 * function is useful in scenarios where the current state of the LED
 * needs to be checked, such as in conditional logic or status reporting.
 * The function does not modify the state of the LED or any other system
 * state.
 *
 * @param led_handle A pointer to an `sl_led_t` instance representing the LED
 * whose state is to be retrieved. This pointer must not be
 * null, and the LED must have been initialized prior to
 * calling this function. If the pointer is invalid, the
 * behavior is undefined.
 * @return Returns an `sl_led_state_t` indicating the current state of the LED:
 * `1` for on and `0` for off.
 ******************************************************************************/
sl_led_state_t sl_led_get_state(const sl_led_t *led_handle);

/** @} (end group led) */

// ******** THE REST OF THE FILE IS DOCUMENTATION ONLY !***********************
/// @addtogroup led LED Driver
/// @{
///
///   @details
///
///
///   @n @section leddrv_intro Introduction
///
///   The LED driver is a platfom level software module that manages the control of
///   various types of LEDs. There are currently two types of LEDs supported by the
///   LED driver:
///
///   @li @ref simple_led
///   @li @ref simple_rgbw_pwm_led
///
///   The common LED functions are called through the generic LED driver, while other
///   functions specific to a certain type of LED are called directly through their own
///   driver.
///
///   @n @section leddrv_config Configuration
///
///   All LED instances are configured using an @ref sl_led_t struct along with a
///   type-specific context struct, and sometimes additional structs. For `sl_led_XXX`
///   functions, the `sl_led_t *led_handle` is used, while for `sl_simple_led_XXX`
///   functions, the `sl_simple_led_context_t *context` is used.
///
///   These structs are automatically generated when an LED is set up using Simplicity
///   Studio's wizard. Specific configuration setups for the various LED types are
///   described in the following sections.
///
///   - [Simple LED Configuration](/gecko-platform/<docspace-docleaf-version>/platform-driver/simple-led#simple-led-configuration)
///   - [RGBW PWM LED Configuration](/gecko-platform/<docspace-docleaf-version>/platform-driver/simple-rgb-pwm-led#rgb-pwm-led-configuration)
///
///   @n @section leddrv_usage Usage
///
///   Once the LED structs are defined, the common LED functions can be called being passed an instance
///   of sl_led_t, which will be redirected to calling the type specific version of that function. The
///   common functions include the following:
///
///   @li @ref sl_led_init
///   @li @ref sl_led_turn_on
///   @li @ref sl_led_turn_off
///   @li @ref sl_led_toggle
///   @li @ref sl_led_get_state
///
///   These functions allow for initializing the LED, turning it on and off, toggling it, and retrieving
///   its current state (on/off). Other functions specific to certain types of LEDs are called through
///   their respective APIs. The usages of the different types of LEDs are described in detail in the
///   following sections:
///
///   @li @ref simple_led_usage
///   @li @ref rgbw_led_usage
///
///   Ensure that the appropriate context type is used in the function calls:
///   - Use `sl_led_t *led_handle` for `sl_led_XXX` functions.
///   - Use `sl_simple_led_context_t *context` for `sl_simple_led_XXX` functions.
///
///   These distinctions are handled by the Simplicity Studio auto-generated code.
///
/// @} end group led ********************************************************/

#ifdef __cplusplus
}
#endif

#endif // SL_LED_H
