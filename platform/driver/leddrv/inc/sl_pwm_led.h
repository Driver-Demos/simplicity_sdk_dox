/***************************************************************************//**
 * @file
 * @brief Simple PWM LED Driver
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
#ifndef SL_PWM_LED_H
#define SL_PWM_LED_H

#include "sl_led.h"
#include "em_device.h"

#include "sl_gpio.h"

#if defined(_SILICON_LABS_32B_SERIES_3)
#include "sl_hal_timer.h"
#else
#include "em_timer.h"
#endif

#include "sl_clock_manager.h"
#include "sl_device_peripheral.h"

#ifdef __cplusplus
extern "C" {
#endif

/// A Simple PWM LED
/***************************************************************************//**
 * @brief The `sl_led_pwm_t` structure is a data structure used to represent a
 * PWM (Pulse Width Modulation) LED driver. It encapsulates various
 * attributes and functions necessary for controlling a PWM LED,
 * including setting and getting the LED color, specifying the TIMER
 * channel, GPIO port and pin, PWM duty cycle, polarity, current state,
 * TIMER instance, frequency, and resolution. This structure is designed
 * to facilitate the management of LED brightness and color through PWM
 * signals, making it suitable for applications requiring precise LED
 * control.
 *
 * @param set_color Member function to set color of PWM LED.
 * @param get_color Member function to get color of PWM LED.
 * @param channel TIMER channel.
 * @param port GPIO port.
 * @param pin GPIO pin.
 * @param level PWM duty cycle [0-65535].
 * @param polarity GPIO polarity (active high/low).
 * @param state Current state (ON/OFF).
 * @param timer TIMER instance.
 * @param frequency PWM frequency.
 * @param resolution PWM resolution (dimming steps).
 ******************************************************************************/
typedef struct {
  void   (*set_color)(void *context, uint16_t color);  ///< Member function to set color of PWM LED
  void   (*get_color)(void *context, uint16_t *color); ///< Member function to get color of PWM LED
  uint8_t           channel;      /**< TIMER channel */
  sl_gpio_port_t    port;         /**< GPIO port */
  uint8_t           pin;          /**< GPIO pin */
  uint16_t          level;        /**< PWM duty cycle [0-65535] */
  uint8_t           polarity;     /**< GPIO polarity (active high/low) */
  sl_led_state_t    state;        /**< Current state (ON/OFF) */
  TIMER_TypeDef     *timer;       /**< TIMER instance */
  uint32_t          frequency;    /**< PWM frequency */
  uint32_t          resolution;   /**< PWM resolution (dimming steps) */
} sl_led_pwm_t;

/***************************************************************************//**
 * @brief This function initializes a PWM LED using the provided configuration
 * data. It must be called before any other operations on the PWM LED,
 * such as starting or setting the color. The function configures the
 * timer and GPIO settings based on the parameters in the `sl_led_pwm_t`
 * structure. It ensures that the resolution and frequency settings are
 * valid and sets up the necessary hardware routes. If the configuration
 * is invalid, the function returns an error status. This function is
 * essential for preparing the LED for subsequent operations.
 *
 * @param context A pointer to an `sl_led_pwm_t` structure containing the PWM
 * LED configuration data. This includes timer, channel, port,
 * pin, frequency, resolution, and other settings. The pointer
 * must not be null, and the structure must be properly
 * initialized with valid values. Invalid configurations will
 * result in a failure status.
 * @return Returns `SL_STATUS_OK` on successful initialization or
 * `SL_STATUS_FAIL` if the configuration is invalid or an error occurs
 * during initialization.
 ******************************************************************************/
sl_status_t sl_pwm_led_init(void *context);

/***************************************************************************//**
 * @brief This function is used to start the PWM LED by enabling the necessary
 * GPIO timer route, allowing the LED to begin operation. It should be
 * called after the PWM LED has been properly initialized using the
 * appropriate initialization function. The function does not return a
 * value and assumes that the provided context is valid and correctly
 * configured. It is important to ensure that the context points to a
 * valid `sl_led_pwm_t` structure, as invalid input may lead to undefined
 * behavior.
 *
 * @param context Pointer to PWM LED specific data. Must not be null and should
 * point to a valid `sl_led_pwm_t` structure. The caller retains
 * ownership of the data. Invalid or null input may result in
 * undefined behavior.
 * @return None
 ******************************************************************************/
void sl_pwm_led_start(void *context);

/***************************************************************************//**
 * @brief Use this function to stop the PWM LED, effectively disabling its PWM
 * output by clearing the GPIO timer route. This function should be
 * called when the LED needs to be turned off or when PWM control is no
 * longer required. It is important to ensure that the `led_handler`
 * parameter is a valid pointer to a previously initialized PWM LED
 * structure. The function does not return a value and does not provide
 * feedback on the success of the operation, so it is assumed that the
 * input is valid and the operation will succeed if the system is
 * correctly configured.
 *
 * @param context Pointer to PWM LED specific data. Must not be null and should
 * point to a valid `sl_led_pwm_t` structure. The caller retains
 * ownership of the data.
 * @return None
 ******************************************************************************/
void sl_pwm_led_stop(void *context);

/***************************************************************************//**
 * @brief This function sets the color of a PWM LED by adjusting its duty cycle,
 * which is specified by the `color` parameter. It should be called when
 * you want to change the brightness or color intensity of the LED. The
 * function assumes that the LED has been properly initialized and that
 * the PWM LED is stopped before setting the new color. After setting the
 * color, the PWM LED is restarted. This function is typically used in
 * applications where dynamic control of LED brightness or color is
 * required.
 *
 * @param led_handler A pointer to the PWM LED specific data. This must not be
 * null and should point to a valid `sl_led_pwm_t` structure.
 * The caller retains ownership of this data.
 * @param color A 16-bit unsigned integer representing the desired color level
 * or PWM duty cycle. Valid values range from 0 to 65535, where 0
 * represents the lowest intensity and 65535 represents the highest
 * intensity. Values outside this range may lead to undefined
 * behavior.
 * @return None
 ******************************************************************************/
void sl_pwm_led_set_color(void *led, uint16_t color);

/***************************************************************************//**
 * @brief This function is used to obtain the current color level, represented
 * as a PWM duty cycle, of a specified PWM LED. It should be called when
 * you need to know the current brightness or color setting of the LED.
 * The function requires a valid LED handler, which must have been
 * previously initialized, and a pointer to a variable where the color
 * level will be stored. The color level is expressed as a 16-bit value
 * ranging from 0 to 65535. Ensure that the provided pointers are valid
 * and not null to avoid undefined behavior.
 *
 * @param led Pointer to the PWM LED specific data. Must not be null and should
 * point to a valid, initialized LED handler.
 * @param color Pointer to a uint16_t variable where the current color level
 * (PWM duty cycle) will be stored. Must not be null.
 * @return None
 ******************************************************************************/
void sl_pwm_led_get_color(void *led, uint16_t *color);

#ifdef __cplusplus
}
#endif

#endif  // SL_PWM_LED_H
