/***************************************************************************//**
 * @file
 * @brief PWM Driver
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
#ifndef SL_PWM_H
#define SL_PWM_H

#include "em_device.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#include "sl_status.h"
#include "sl_enum.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup pwm PWM Driver
 * @brief PWM Driver
 * @{
 * The PWM driver uses one or more TIMER peripherals to generate one or more PWM
 * waveform, with configurable frequency, duty cycle, and polarity. Multiple
 * instances of the driver can be created and allocated to their own TIMER
 * channel.
 *
 * The duty cycle of the PWM waveform can be updated, while the PWM driver is
 * running by calling @ref sl_pwm_set_duty_cycle(), without causing glitches
 * in the output waveform.
 *
 * @note If several PWM driver instances are set up to use the same TIMER
 * instance, the PWM frequency of these instances must be the same.
 * ### PWM Example Code
 *
 * Basic example for generating PWM waveform:
 *
 * ```c
 *
 * #include "sl_pwm.h"
 * #include "sl_gpio.h"
 *
 * int main( void )
 * {
 *
 *   ...
 *
 *   sl_pwm_instance_t sl_pwm_led0 = {
 *     .timer    = TIMER0,
 *     .channel  = 0,
 *     .port     = SL_GPIO_PORT_A,
 *     .pin      = 0,
 *     .location = 0,
 *   };
 *
 *   sl_pwm_config_t pwm_led0_config = {
 *     .frequency = 10000,
 *     .polarity  = PWM_ACTIVE_HIGH,
 *   };
 *
 *   // Initialize PWM
 *   sl_pwm_init(&sl_pwm_led0, &pwm_led0_config);
 *
 *   // Set duty cycle to 40%
 *   sl_pwm_set_duty_cycle(&sl_pwm_led0, 40);
 *
 *   // Enable PWM output
 *   sl_pwm_start(&sl_pwm_led0);
 *
 *   ...
 *
 * }
 * ```
 *
 ******************************************************************************/

/** PWM polarity selection */
SL_ENUM(sl_pwm_polarity_t) {
  /** PWM polarity active high */
  PWM_ACTIVE_HIGH = 0,
  /** PWM polarity active low */
  PWM_ACTIVE_LOW = 1,
};

/***************************************************************************//**
 * @brief The `sl_pwm_instance_t` structure is used to define an instance of a
 * PWM driver in the Silicon Labs PWM library. It encapsulates the
 * necessary hardware configuration details, such as the TIMER instance,
 * channel, and GPIO settings, required to generate a PWM signal. This
 * structure allows for the configuration and control of PWM outputs on
 * specific GPIO pins, enabling the generation of waveforms with
 * adjustable frequency and duty cycle.
 *
 * @param timer Pointer to a TIMER instance used for PWM generation.
 * @param channel Specifies the TIMER channel used for PWM.
 * @param port Indicates the GPIO port associated with the PWM output.
 * @param pin Specifies the GPIO pin used for PWM output.
 * @param location Defines the GPIO location for the PWM signal.
 ******************************************************************************/
typedef struct sl_pwm_instance {
  TIMER_TypeDef *timer; /**< TIMER instance */
  uint8_t channel;      /**< TIMER channel */
  uint8_t port;         /**< GPIO port */
  uint8_t pin;          /**< GPIO pin */
  uint8_t location;     /**< GPIO location */
} sl_pwm_instance_t;

/***************************************************************************//**
 * @brief The `sl_pwm_config_t` structure is used to configure the PWM (Pulse
 * Width Modulation) settings for a PWM driver instance. It contains two
 * members: `frequency`, which specifies the frequency of the PWM signal,
 * and `polarity`, which determines the active polarity of the PWM
 * signal, defined by the `sl_pwm_polarity_t` enumeration. This structure
 * is essential for initializing and controlling the behavior of PWM
 * signals in embedded systems using the Silicon Laboratories PWM driver.
 *
 * @param frequency PWM frequency.
 * @param polarity PWM polarity.
 ******************************************************************************/
typedef struct sl_pwm_config {
  int frequency;              /**< PWM frequency */
  sl_pwm_polarity_t polarity; /**< PWM polarity */
} sl_pwm_config_t;

/***************************************************************************//**
 * @brief This function sets up a PWM driver instance using the provided
 * configuration, preparing it to generate PWM signals. It must be called
 * before any other operations on the PWM instance, such as starting the
 * PWM signal or setting the duty cycle. The function configures the
 * timer and GPIO settings based on the specified frequency and polarity.
 * It is essential to ensure that the `pwm` and `config` parameters are
 * correctly initialized and valid before calling this function. If
 * multiple PWM instances share the same timer, they must have the same
 * frequency.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure representing the PWM
 * driver instance. This must be properly initialized with the timer,
 * channel, port, pin, and location before calling the function. The
 * pointer must not be null.
 * @param config A pointer to an `sl_pwm_config_t` structure containing the
 * desired PWM configuration, including frequency and polarity.
 * The frequency must be a positive integer, and the polarity must
 * be a valid `sl_pwm_polarity_t` value. The pointer must not be
 * null.
 * @return Returns `SL_STATUS_OK` if the initialization is successful. If the
 * function encounters an error, such as an invalid timer number, it
 * returns `SL_STATUS_FAIL`.
 ******************************************************************************/
sl_status_t sl_pwm_init(sl_pwm_instance_t *pwm, sl_pwm_config_t *config);

/***************************************************************************//**
 * @brief Use this function to safely deinitialize a PWM driver instance when it
 * is no longer needed. This function stops the PWM signal, resets the
 * associated TIMER and GPIO configurations, and disables the clock for
 * the TIMER peripheral. It should be called to release resources and
 * ensure that the PWM instance is properly shut down. Ensure that the
 * PWM instance has been initialized before calling this function. The
 * function returns a status code indicating success or failure.
 *
 * @param pwm A pointer to an sl_pwm_instance_t structure representing the PWM
 * driver instance to be deinitialized. Must not be null. The
 * instance should have been previously initialized with
 * sl_pwm_init(). If the TIMER number in the instance is invalid, the
 * function will return SL_STATUS_FAIL.
 * @return Returns SL_STATUS_OK if the deinitialization is successful, or
 * SL_STATUS_FAIL if an invalid TIMER number is encountered.
 ******************************************************************************/
sl_status_t sl_pwm_deinit(sl_pwm_instance_t *pwm);

/***************************************************************************//**
 * @brief This function is used to start the generation of a PWM waveform on a
 * specified PWM instance. It should be called after the PWM instance has
 * been properly initialized using `sl_pwm_init`. The function enables
 * the PWM output by configuring the appropriate GPIO routing for the
 * specified timer and channel. It is important to ensure that the PWM
 * instance is correctly configured and initialized before calling this
 * function to avoid undefined behavior.
 *
 * @param pwm A pointer to an `sl_pwm_instance_t` structure representing the PWM
 * instance to be started. This parameter must not be null and should
 * point to a valid, initialized PWM instance. The function does not
 * handle null pointers and expects the caller to ensure the PWM
 * instance is properly set up.
 * @return None
 ******************************************************************************/
void sl_pwm_start(sl_pwm_instance_t *pwm);

/***************************************************************************//**
 * @brief Use this function to stop the PWM signal generation for a specified
 * PWM instance. This function is typically called when the PWM output is
 * no longer needed or before deinitializing the PWM instance. It
 * disables the PWM output for the specified channel but keeps the timer
 * running, allowing other channels using the same timer to continue
 * operating. Ensure that the PWM instance has been initialized before
 * calling this function.
 *
 * @param pwm A pointer to an sl_pwm_instance_t structure representing the PWM
 * driver instance. This parameter must not be null, and the instance
 * should be properly initialized before calling this function. If
 * the instance is invalid or not initialized, the behavior is
 * undefined.
 * @return None
 ******************************************************************************/
void sl_pwm_stop(sl_pwm_instance_t *pwm);

/***************************************************************************//**
 * @brief This function updates the duty cycle of a PWM waveform for a specified
 * PWM driver instance. It should be called when you need to change the
 * proportion of the PWM period during which the waveform is in the
 * active state, as defined by the PWM configuration. The function can be
 * used while the PWM is running, allowing for dynamic adjustments
 * without causing glitches in the output waveform. Ensure that the PWM
 * instance has been properly initialized before calling this function.
 *
 * @param pwm A pointer to an sl_pwm_instance_t structure representing the PWM
 * driver instance. This must not be null and should be initialized
 * before use.
 * @param percent An unsigned 8-bit integer representing the duty cycle
 * percentage. Valid values range from 0 to 100, where 0 means
 * the waveform is always inactive, and 100 means it is always
 * active. Values outside this range may result in undefined
 * behavior.
 * @return None
 ******************************************************************************/
void sl_pwm_set_duty_cycle(sl_pwm_instance_t *pwm, uint8_t percent);

/***************************************************************************//**
 * @brief This function is used to obtain the current duty cycle of a specified
 * PWM instance, expressed as a percentage. It is useful for monitoring
 * or debugging purposes to verify the PWM output settings. The function
 * should be called only after the PWM instance has been properly
 * initialized and started. The duty cycle is calculated based on the
 * current timer settings associated with the PWM instance.
 *
 * @param pwm A pointer to an sl_pwm_instance_t structure representing the PWM
 * instance. This parameter must not be null, and the PWM instance
 * should be initialized and started before calling this function. If
 * the pointer is invalid or the PWM instance is not properly
 * configured, the behavior is undefined.
 * @return Returns the current duty cycle as a uint8_t, representing the
 * percentage of the PWM period that the waveform is in the active
 * state.
 ******************************************************************************/
uint8_t sl_pwm_get_duty_cycle(sl_pwm_instance_t *pwm);

/** @} (end addtogroup pwm) */

#ifdef __cplusplus
}
#endif

#endif // SL_PWM_H
