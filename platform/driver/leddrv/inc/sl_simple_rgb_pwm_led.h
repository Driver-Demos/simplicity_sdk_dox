/***************************************************************************//**
 * @file
 * @brief Simple RGB PWM LED Driver
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

#ifndef SL_SIMPLE_RGB_PWM_LED_H
#define SL_SIMPLE_RGB_PWM_LED_H

#include "sl_led.h"
#include "sl_pwm_led.h"
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
 * @addtogroup simple_rgb_pwm_led Simple RGB PWM LED Driver
 * @brief Simple Red/Green/Blue PWM LED Driver
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/

#define SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_HIGH 0U  ///< LED Active polarity High
#define SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW  1U  ///< LED Active polarity Low

#define SL_SIMPLE_RGB_PWM_LED_COLOR_R              0U  ///< LED Red
#define SL_SIMPLE_RGB_PWM_LED_COLOR_G              1U  ///< LED Green
#define SL_SIMPLE_RGB_PWM_LED_COLOR_B              2U  ///< LED Blue

#define SL_SIMPLE_RGB_PWM_LED_NUM_CC_REQUIRED      3U  ///< Number of Timer Capture Channels required (1 for each RGB color)

/*******************************************************************************
 *****************************   DATA TYPES   **********************************
 ******************************************************************************/

/// A Simple RGB LED context
/***************************************************************************//**
 * @brief The `sl_simple_rgb_pwm_led_context_t` structure is designed to manage
 * the context for a simple RGB LED driven by PWM. It contains pointers
 * to PWM LED instances for each of the red, green, and blue colors,
 * allowing for individual control of each color channel. Additionally,
 * it includes a common PWM timer for synchronizing the LED operations,
 * as well as parameters for setting the PWM frequency and resolution,
 * which determine the dimming steps. The structure also maintains the
 * current state of the LED, indicating whether it is on or off.
 *
 * @param red PWM LED instance for red color.
 * @param green PWM LED instance for green color.
 * @param blue PWM LED instance for blue color.
 * @param timer PWM timer (common instance for all LEDs).
 * @param frequency PWM frequency.
 * @param resolution PWM resolution (dimming steps).
 * @param state Current state (ON/OFF).
 ******************************************************************************/
typedef struct {
  sl_led_pwm_t      *red;       ///< PWM LED instance for red color
  sl_led_pwm_t      *green;     ///< PWM LED instance for green color
  sl_led_pwm_t      *blue;      ///< PWM LED instance for blue color

  TIMER_TypeDef     *timer;     ///< PWM timer (common instance for all LEDs)
  uint32_t          frequency;  ///< PWM frequency
  uint32_t          resolution; ///< PWM resolution (dimming steps)
  sl_led_state_t    state;      ///< Current state (ON/OFF)
} sl_simple_rgb_pwm_led_context_t;

/// A Simple RGB LED PWM instance
/***************************************************************************//**
 * @brief The `sl_led_rgb_pwm_t` structure is a compound data type designed to
 * represent an RGB LED driven by PWM (Pulse Width Modulation). It
 * extends the functionality of a common LED driver by including function
 * pointers for setting and getting RGB color values, allowing for
 * dynamic control of the LED's color output. This structure is part of a
 * simple RGB PWM LED driver module, which provides an interface for
 * controlling the brightness and color of RGB LEDs using PWM signals.
 *
 * @param led_common Inherits from the Common LED Driver.
 * @param set_rgb_color Function pointer to set the RGB color values.
 * @param get_rgb_color Function pointer to retrieve the current RGB color
 * values.
 ******************************************************************************/
typedef struct {
  sl_led_t       led_common;                       ///< Inherit from the Common LED Driver
  void           (*set_rgb_color)(void *context,
                                  uint16_t red,
                                  uint16_t green,
                                  uint16_t blue);  ///< Member function to set RGB
  void           (*get_rgb_color)(void *context,
                                  uint16_t *red,
                                  uint16_t *green,
                                  uint16_t *blue); ///< Member function to get RGB
} sl_led_rgb_pwm_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function initializes the RGB PWM LED driver by setting up the PWM
 * channels for the red, green, and blue components of the LED. It must
 * be called before any other operations on the RGB LED to ensure that
 * the PWM channels are properly configured. The function sets the
 * initial state of the LED to off. It is important to ensure that the
 * `rgb` parameter points to a valid `sl_simple_rgb_pwm_led_context_t`
 * structure with properly initialized PWM LED instances for each color
 * component. If any of the PWM initializations fail, the function
 * returns an error status.
 *
 * @param rgb Pointer to an `sl_simple_rgb_pwm_led_context_t` structure
 * containing the PWM LED instances for red, green, and blue colors.
 * Must not be null and should be properly initialized before calling
 * this function. If the pointer is invalid or any PWM initialization
 * fails, the function returns `SL_STATUS_FAIL`.
 * @return Returns `SL_STATUS_OK` on successful initialization of all PWM
 * channels, or `SL_STATUS_FAIL` if any initialization fails.
 ******************************************************************************/
sl_status_t sl_simple_rgb_pwm_led_init(void *rgb);

/***************************************************************************//**
 * @brief This function is used to turn on an RGB LED that is controlled via
 * PWM. It should be called when you want the LED to emit light at the
 * previously set color levels. If no color levels have been set prior to
 * calling this function, the LED will turn on at maximum brightness for
 * all colors. The function requires a valid pointer to an RGB PWM LED
 * context, which must have been properly initialized. It is important to
 * ensure that the context is not null and has been set up correctly to
 * avoid undefined behavior.
 *
 * @param rgb A pointer to an sl_simple_rgb_pwm_led_context_t structure that
 * contains the PWM LED instances for red, green, and blue colors.
 * This pointer must not be null and should point to a properly
 * initialized context.
 * @return None
 ******************************************************************************/
void sl_simple_rgb_pwm_led_turn_on(void *rgb);

/***************************************************************************//**
 * @brief This function is used to turn off an RGB LED that is controlled via
 * PWM. It should be called when you want to stop the LED from emitting
 * light. The function requires a valid pointer to an RGB PWM LED
 * context, which must have been initialized prior to calling this
 * function. It stops the PWM signals for the red, green, and blue
 * components of the LED, ensuring that the LED is completely turned off.
 * This function does not return any value and does not modify the input
 * context beyond changing its state to indicate the LED is off.
 *
 * @param rgb A pointer to an sl_simple_rgb_pwm_led_context_t structure
 * representing the RGB LED context. This pointer must not be null
 * and should point to a properly initialized context. If the pointer
 * is invalid, the behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_simple_rgb_pwm_led_turn_off(void *rgb);

/***************************************************************************//**
 * @brief This function is used to toggle the current state of an RGB LED,
 * switching it from off to on or from on to off. It should be called
 * when you want to change the LED's state without explicitly setting it
 * to on or off. The function assumes that the RGB LED has been properly
 * initialized and configured before being called. It does not alter the
 * color or brightness settings of the LED, only its on/off state.
 *
 * @param rgb Pointer to the RGB PWM LED context data. Must not be null and
 * should point to a valid sl_simple_rgb_pwm_led_context_t structure.
 * The function will not perform any action if the pointer is
 * invalid.
 * @return None
 ******************************************************************************/
void sl_simple_rgb_pwm_led_toggle(void *rgb);

/***************************************************************************//**
 * @brief This function is used to obtain the current on/off state of an RGB
 * LED. It should be called when you need to check whether the LED is
 * currently active or inactive. The function requires a valid pointer to
 * the RGB LED context, which must have been initialized prior to calling
 * this function. The state returned will indicate if any of the RGB
 * components are on or if all are off. Ensure that the pointer provided
 * is not null to avoid undefined behavior.
 *
 * @param rgb A pointer to the RGB LED context data. This must be a valid, non-
 * null pointer to a previously initialized RGB LED context
 * structure. The caller retains ownership of the memory.
 * @return Returns an sl_led_state_t value indicating the current state of the
 * RGB LED: 0 if all components are off, or 1 if any component is on.
 ******************************************************************************/
sl_led_state_t sl_simple_rgb_pwm_led_get_state(void *rgb);

/***************************************************************************//**
 * @brief This function sets the color of an RGB LED by specifying the PWM duty-
 * cycle values for the red, green, and blue components. It should be
 * used when you want to change the color output of an RGB LED. The
 * function requires a valid context pointer that represents the RGB LED
 * configuration. Each color component is specified as a 16-bit value,
 * where 0 represents the minimum intensity and 65535 represents the
 * maximum intensity. Ensure that the RGB LED has been properly
 * initialized before calling this function to avoid undefined behavior.
 *
 * @param rgb Pointer to the RGB LED context data. Must not be null and should
 * point to a valid sl_simple_rgb_pwm_led_context_t structure.
 * @param red 16-bit value representing the red color intensity (PWM duty-cycle)
 * ranging from 0 to 65535.
 * @param green 16-bit value representing the green color intensity (PWM duty-
 * cycle) ranging from 0 to 65535.
 * @param blue 16-bit value representing the blue color intensity (PWM duty-
 * cycle) ranging from 0 to 65535.
 * @return None
 ******************************************************************************/
void sl_simple_rgb_pwm_led_set_color(void *rgb,
                                     uint16_t red,
                                     uint16_t green,
                                     uint16_t blue);

/***************************************************************************//**
 * @brief This function is used to obtain the current color mixing and dimming
 * levels of an RGB LED, represented as PWM duty cycles for the red,
 * green, and blue components. It should be called when you need to know
 * the last set color levels, regardless of whether the LED is currently
 * on or off. Ensure that the `rgb` parameter points to a valid RGB LED
 * context before calling this function. The function does not alter the
 * state of the LED or the color levels.
 *
 * @param rgb A pointer to the RGB LED context data. Must not be null and should
 * point to a valid `sl_simple_rgb_pwm_led_context_t` structure.
 * @param red A pointer to a `uint16_t` where the function will store the
 * current red color level (PWM duty-cycle [0-65535]). Must not be
 * null.
 * @param green A pointer to a `uint16_t` where the function will store the
 * current green color level (PWM duty-cycle [0-65535]). Must not
 * be null.
 * @param blue A pointer to a `uint16_t` where the function will store the
 * current blue color level (PWM duty-cycle [0-65535]). Must not be
 * null.
 * @return None
 ******************************************************************************/
void sl_simple_rgb_pwm_led_get_color(void *rgb,
                                     uint16_t *red,
                                     uint16_t *green,
                                     uint16_t *blue);

/*******************************************************************************
 ****************   API extensions to the Common LED Driver   ******************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function is used to set the color of an RGB LED by specifying the
 * intensity of the red, green, and blue components. It should be called
 * whenever you need to change the LED's color. The function requires a
 * valid LED handle, which must be properly initialized before use. The
 * color intensities are specified as PWM duty-cycle values ranging from
 * 0 to 65535, where 0 represents no intensity and 65535 represents full
 * intensity. Ensure that the LED handle is not null to avoid undefined
 * behavior.
 *
 * @param led_handle A pointer to an sl_led_rgb_pwm_t structure representing the
 * LED instance. Must not be null and should be initialized
 * before use.
 * @param red The intensity of the red component, specified as a PWM duty-cycle
 * value between 0 and 65535.
 * @param green The intensity of the green component, specified as a PWM duty-
 * cycle value between 0 and 65535.
 * @param blue The intensity of the blue component, specified as a PWM duty-
 * cycle value between 0 and 65535.
 * @return None
 ******************************************************************************/
void sl_led_set_rgb_color(const sl_led_rgb_pwm_t *rgb,   ///< LED Instance handle
                          uint16_t red,                           ///< LED red intensity
                          uint16_t green,                         ///< LED green intensity
                          uint16_t blue                           ///< LED blue intensity
                          );  ///< LED set RGB color

/***************************************************************************//**
 * @brief Use this function to obtain the current red, green, and blue color
 * levels of an RGB LED. This function is useful when you need to know
 * the exact color settings of the LED, regardless of whether it is
 * currently on or off. It is important to ensure that the `led_handle`
 * parameter is a valid pointer to an initialized `sl_led_rgb_pwm_t`
 * structure. The function will store the retrieved color levels in the
 * provided `red`, `green`, and `blue` pointers, which must not be null.
 *
 * @param led_handle A pointer to an `sl_led_rgb_pwm_t` structure representing
 * the LED instance. Must not be null and should point to a
 * properly initialized LED instance.
 * @param red A pointer to a `uint16_t` where the current red color level will
 * be stored. Must not be null.
 * @param green A pointer to a `uint16_t` where the current green color level
 * will be stored. Must not be null.
 * @param blue A pointer to a `uint16_t` where the current blue color level will
 * be stored. Must not be null.
 * @return None
 ******************************************************************************/
void sl_led_get_rgb_color(const sl_led_rgb_pwm_t *rgb,    ///< LED Instance handle
                          uint16_t * red,                           ///< LED red intensity
                          uint16_t * green,                         ///< LED green intensity
                          uint16_t * blue                           ///< LED blue intensity
                          );  ///< LED get RGB setting

/** @} (end group simple_rgb_pwm_led) */
/** @} (end group led) */

// ******** THE REST OF THE FILE IS DOCUMENTATION ONLY !***********************
/// @addtogroup simple_rgb_pwm_led Simple RGB PWM LED Driver
/// @{
///
///   @details
///
///   @n @section rgb_led_intro Introduction
///
///   The Simple RGB PWM LED Driver is a module for the LED driver that provides functionality
///   for controlling Red/Green/Blue LEDs that are driven by PWM.
///
///   @n @section rgb_led_config RGB PWM LED Configuration
///
///   RGB PWM LEDs use the @ref sl_led_t struct, and their own structs
///   @ref sl_simple_rgb_pwm_led_context_t and @ref sl_led_rgb_pwm_t. These are automatically
///   generated into the following files, as well as instance specific headers with macro
///   definitions in them. The samples below are for a single instance called "inst0".
///
///   @code{.c}
///// sl_simple_rgb_pwm_led_instances.c
///
///#include "sl_gpio.h"
///#include "sl_simple_rgb_pwm_led.h"
///
///#include "sl_simple_rgb_pwm_led_inst0_config.h"
///
///
///
///sl_led_pwm_t red_inst0 = {
///  .port = SIMPLE_RGB_PWM_LED_INST0_PORT,
///  .pin = SIMPLE_RGB_PWM_LED_INST0_PIN,
///  .polarity = SIMPLE_RGB_PWM_LED_INST0_POLARITY,
///  .channel = SIMPLE_RGB_PWM_LED_INST0_CHANNEL,
///#if defined(SL_SIMPLE_RGB_PWM_LED_INST0_RED_LOC)
///  .location = SIMPLE_RGB_PWM_LED_INST0_LOC,
///#endif
///};
///
///sl_led_pwm_t green_inst0 = {
///  .port = SIMPLE_RGB_PWM_LED_INST0_PORT,
///  .pin = SIMPLE_RGB_PWM_LED_INST0_PIN,
///  .polarity = SIMPLE_RGB_PWM_LED_INST0_POLARITY,
///  .channel = SIMPLE_RGB_PWM_LED_INST0_CHANNEL,
///#if defined(SL_SIMPLE_RGB_PWM_LED_INST0_RED_LOC)
///  .location = SIMPLE_RGB_PWM_LED_INST0_LOC,
///#endif
///};
///
///sl_led_pwm_t blue_inst0 = {
///  .port = SIMPLE_RGB_PWM_LED_INST0_PORT,
///  .pin = SIMPLE_RGB_PWM_LED_INST0_PIN,
///  .polarity = SIMPLE_RGB_PWM_LED_INST0_POLARITY,
///  .channel = SIMPLE_RGB_PWM_LED_INST0_CHANNEL,
///#if defined(SL_SIMPLE_RGB_PWM_LED_INST0_RED_LOC)
///  .location = SIMPLE_RGB_PWM_LED_INST0_LOC,
///#endif
///};
///
///sl_simple_rgb_pwm_led_context_t simple_rgb_pwm_inst0_context = {
///  .red = &red_inst0,
///  .green = &green_inst0,
///  .blue = &blue_inst0,
///
///  .timer = SL_SIMPLE_RGB_PWM_LED_INST0_PERIPHERAL,
///  .frequency = SL_SIMPLE_RGB_PWM_LED_INST0_FREQUENCY,
///  .resolution = SL_SIMPLE_RGB_PWM_LED_INST0_RESOLUTION,
///};
///
///const sl_led_rgb_pwm_t sl_inst0 = {
///  .led_common.context = &simple_rgb_pwm_inst0_context,
///  .led_common.init = sl_simple_rgb_pwm_led_init,
///  .led_common.turn_on = sl_simple_rgb_pwm_led_turn_on,
///  .led_common.turn_off = sl_simple_rgb_pwm_led_turn_off,
///  .led_common.toggle = sl_simple_rgb_pwm_led_toggle,
///  .led_common.get_state = sl_simple_rgb_pwm_led_get_state,
///  .set_rgb_color = sl_simple_rgb_pwm_led_set_color,
///  .get_rgb_color = sl_simple_rgb_pwm_led_get_color,
///};
///
///
///
///void sl_simple_rgb_pwm_led_init_instances(void)
///{
///
///  sl_led_init((sl_led_t *)&sl_inst0);
///
///}
///   @endcode
///
///   @note The sl_simple_rgb_pwm_led_instances.c file is shown with only one instance, but if more
///         were in use they would all appear in this .c file.
///
///   @code{.c}
///// sl_simple_rgb_pwm_led_instances.h
///
///#ifndef SL_SIMPLE_RGB_PWM_LED_INSTANCES_H
///#define SL_SIMPLE_RGB_PWM_LED_INSTANCES_H
///
///#include "sl_simple_rgb_pwm_led.h"
///
///extern const sl_led_rgb_pwm_t sl_inst0;
///
///void sl_simple_rgb_pwm_led_init_instances(void);
///
///#endif // SL_SIMPLE_RGB_PWM_LED_INIT_H
///   @endcode
///
///   @note The sl_simple_rgb_pwm_led_instances.h file is shown with only one instance, but if more
///         were in use they would all appear in this .h file.
///
///   @n @section rgb_led_usage RGB PWM LED Usage
///
///   The RGB PWM Led driver provides functionality for controlling Red/Green/Blue/White
///   LEDs that are driven by PWM. The LEDs can be turned on and off and toggled, and remember
///   their color and brightness state when being turned back on. The color and brightness can be
///   set using values of 0-65535 for red, green, blue, and white. Retrieving the state gives the
///   on/off value, while retrieving the color gives the rgb values. The following
///   code shows how to control these LEDs. An LED should always be initialized before
///   calling any other functions with it.
///
///   @code{.c}
///// initialize rgb LED
///sl_led_init(&rgb_led_inst0);
///
///// turn on LED, set color to purple, turn off, toggle (would maintain purple color)
///sl_led_turn_on(&rgb_led_inst0);
///uint16_t red = 65535; // max red
///uint16_t green = 0; // no green
///uint16_t blue = 65535; // max blue
///sl_led_set_rgb_color(&rgb_led_inst0, red, green, blue);
///sl_led_turn_off(&rgb_led_inst0);
///sl_led_toggle(&rgb_led_inst0);
///
///// get the state of the led
///sl_led_state_t state = sl_led_get_state(&rgb_led_inst0);
///   @endcode
///
/// @} end group led ********************************************************/

#ifdef __cplusplus
}
#endif

#endif // SL_SIMPLE_RGB_PWM_LED_H
