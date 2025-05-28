/***************************************************************************//**
 * @file
 * @brief Simple LED Driver
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

#ifndef SL_SIMPLE_LED_H
#define SL_SIMPLE_LED_H

#include "sl_led.h"
#include "sl_gpio.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup led
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup simple_led Simple LED Driver
 * @brief Simple LED Driver can be used to execute basic LED functionalities
 *        such as on, off, toggle, or retrive the on/off status on Silicon Labs
 *        devices. Subsequent sections provide more insight into this module.
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/

#define SL_SIMPLE_LED_POLARITY_ACTIVE_LOW  0U ///< LED Active polarity Low
#define SL_SIMPLE_LED_POLARITY_ACTIVE_HIGH 1U ///< LED Active polarity High

/*******************************************************************************
 *****************************   DATA TYPES   **********************************
 ******************************************************************************/

typedef uint8_t sl_led_polarity_t;    ///< LED GPIO polarities (active high/low)

/// A Simple LED instance
/***************************************************************************//**
 * @brief The `sl_simple_led_context_t` structure is used to represent the
 * context for a simple LED in the Simple LED Driver. It contains
 * information about the specific GPIO port and pin to which the LED is
 * connected, as well as the polarity that determines the initial state
 * of the LED (active high or low). This structure is essential for
 * managing the basic operations of an LED, such as turning it on, off,
 * or toggling its state, within the Silicon Labs Simple LED Driver
 * framework.
 *
 * @param port LED port.
 * @param pin LED pin.
 * @param polarity Initial state of LED.
 ******************************************************************************/
typedef struct {
  sl_gpio_port_t    port;             ///< LED port
  uint8_t           pin;              ///< LED pin
  sl_led_polarity_t polarity;         ///< Initial state of LED
} sl_simple_led_context_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function sets up the necessary configurations for a simple LED to
 * operate correctly. It must be called before any other operations on
 * the LED, such as turning it on or off, to ensure the hardware is
 * properly initialized. The function configures the GPIO pin associated
 * with the LED to the correct mode based on its polarity. It is
 * essential to provide a valid context pointer that contains the LED's
 * port, pin, and polarity information. The function returns a status
 * code indicating the success of the initialization.
 *
 * @param led_handle Pointer to a sl_simple_led_context_t structure containing
 * the LED's port, pin, and polarity. Must not be null, and
 * the structure should be properly initialized with valid
 * values before calling this function.
 * @return Returns SL_STATUS_OK to indicate successful initialization.
 ******************************************************************************/
sl_status_t sl_simple_led_init(void *led_handle);

/***************************************************************************//**
 * @brief This function is used to turn on a simple LED that is represented by
 * the provided context. It should be called when you want to activate
 * the LED, and it respects the polarity setting of the LED to determine
 * the correct GPIO operation. The function assumes that the LED has been
 * properly initialized and that the context provided is valid. It does
 * not return a value and does not handle invalid input explicitly, so
 * ensure that the context is correctly set up before calling this
 * function.
 *
 * @param led_handle Pointer to a sl_simple_led_context_t structure that
 * contains the LED's configuration, including port, pin, and
 * polarity. Must not be null, and the context should be
 * properly initialized before use.
 * @return None
 ******************************************************************************/
void sl_simple_led_turn_on(void *led_handle);

/***************************************************************************//**
 * @brief This function is used to turn off a simple LED that is controlled by
 * the provided context. It should be called when the LED needs to be
 * turned off, and it respects the LED's polarity setting to ensure the
 * correct operation. The function assumes that the context has been
 * properly initialized and configured with the correct port, pin, and
 * polarity information. It does not return a value and does not handle
 * invalid input explicitly, so the context must be valid and non-null.
 *
 * @param led_handle Pointer to a sl_simple_led_context_t structure containing
 * the LED's port, pin, and polarity information. Must not be
 * null and should be properly initialized before calling this
 * function.
 * @return None
 ******************************************************************************/
void sl_simple_led_turn_off(void *led_handle);

/***************************************************************************//**
 * @brief Use this function to change the current state of a simple LED from on
 * to off or from off to on. It is typically used when you want to invert
 * the current state of the LED without explicitly checking its current
 * status. This function should be called with a valid LED context that
 * has been properly initialized. Ensure that the context is not null and
 * points to a valid `sl_simple_led_context_t` structure.
 *
 * @param led_handle Pointer to a simple LED context, specifically an
 * `sl_simple_led_context_t` structure. Must not be null and
 * should be initialized before use. The function does not
 * check for null pointers, so passing an invalid pointer may
 * result in undefined behavior.
 * @return None
 ******************************************************************************/
void sl_simple_led_toggle(void *led_handle);

/***************************************************************************//**
 * @brief This function is used to obtain the current on/off state of a simple
 * LED. It should be called when you need to check whether the LED is
 * currently active or inactive. The function requires a valid context
 * pointer that represents the specific LED instance. The LED must be
 * properly initialized before calling this function to ensure accurate
 * results. The function handles LEDs with different polarities,
 * returning the correct state based on the LED's configuration.
 *
 * @param led_handle Pointer to a sl_simple_led_context_t structure representing
 * the LED instance. Must not be null and should point to a
 * valid, initialized LED context.
 * @return Returns sl_led_state_t indicating the current state of the LED: 1 for
 * on, 0 for off.
 ******************************************************************************/
sl_led_state_t sl_simple_led_get_state(void *led_handle);

/** @} (end group simple_led) */
/** @} (end group led) */

// ******** THE REST OF THE FILE IS DOCUMENTATION ONLY !***********************
/// @addtogroup simple_led Simple LED Driver
/// @{
///
///   @details
///
///
///   @n @section simple_led_intro Introduction
///
///   The Simple LED driver is a module of the LED driver that provides the functionality
///   to control simple on/off LEDs.
///
///   @n @section simple_led_config Simple LED Configuration
///
///   Simple LEDs use the @ref sl_led_t struct and their @ref sl_simple_led_context_t
///   struct. These are automatically generated into the following files, as well as
///   instance specific headers with macro definitions in them. The samples below
///   are for a single instance called "inst0".
///
///   @code{.c}
///// sl_simple_led_instances.c
///
///#include "sl_simple_led.h"
///#include "sl_gpio.h"
///#include "sl_simple_led_inst0_config.h"
///
///sl_simple_led_context_t simple_inst0_context = {
///  .port = SL_SIMPLE_LED_INST0_PORT,
///  .pin = SL_SIMPLE_LED_INST0_PIN,
///  .polarity = SL_SIMPLE_LED_INST0_POLARITY,
///};
///
///const sl_led_t sl_led_inst0 = {
///  .context = &simple_inst0_context,
///  .init = sl_simple_led_init,
///  .turn_on = sl_simple_led_turn_on,
///  .turn_off = sl_simple_led_turn_off,
///  .toggle = sl_simple_led_toggle,
///  .get_state = sl_simple_led_get_state,
///};
///
///void sl_simple_led_init_instances(void)
///{
///  sl_led_init(&sl_led_inst0);
///}
///   @endcode
///
///   @note The sl_simple_led_instances.c file is shown with only one instance, but if more
///         were in use they would all appear in this .c file.
///
///   @code{.c}
///// sl_simple_led_instances.h
///
///#ifndef SL_SIMPLE_LED_INSTANCES_H
///#define SL_SIMPLE_LED_INSTANCES_H
///
///#include "sl_simple_led.h"
///
///extern const sl_led_t sl_led_inst0;
///
///void sl_simple_led_init_instances(void);
///
///#endif // SL_SIMPLE_LED_INIT_H
///   @endcode
///
///   @note The sl_simple_led_instances.h file is shown with only one instance, but if more
///         were in use they would all appear in this .h file.
///
///   @n @section simple_led_usage Simple LED Usage
///
///   The simple LED driver is for LEDs with basic on off functionality, and there
///   are no additional functions beyond those in the common driver. The LEDs can be
///   turned on and off, toggled, and their on/off state can be retrieved. The following
///   code shows how to control these LEDs. An LED should always be initialized before
///   calling any other functions with it.
///
///   @code{.c}
///// initialize simple LED
///sl_simple_led_init(&simple_led_inst0);
///
///// turn on simple LED, turn off simple LED, and toggle the simple LED
///sl_simple_led_turn_on(&simple_led_inst0);
///sl_simple_led_turn_off(&simple_led_inst0);
///sl_simple_led_toggle(&simple_led_inst0);
///
///// get the state of the simple LED
///sl_led_state_t state = sl_simple_led_get_state(&simple_led_instance0);
///   @endcode
///
/// @} end group simple_led ********************************************************/

#ifdef __cplusplus
}
#endif

#endif // SL_SIMPLE_LED_H
