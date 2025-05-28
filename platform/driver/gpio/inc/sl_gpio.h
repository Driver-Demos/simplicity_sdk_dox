/***************************************************************************//**
 * @file
 * @brief General Purpose IO (GPIO) driver API
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_GPIO_H
#define SL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "sl_status.h"
#include "sl_device_gpio.h"

#ifndef EM_GPIO_H
#define     gpioPortA  0
#define     gpioPortB  1
#define     gpioPortC  2
#define     gpioPortD  3
#define     gpioPortE  4
#define     gpioPortF  5
#define     gpioPortG  6
#define     gpioPortH  7
#define     gpioPortI  8
#define     gpioPortJ  9
#define     gpioPortK  10
#endif

/* *INDENT-OFF* */
// *****************************************************************************
/// @addtogroup gpio GPIO - General Purpose Input Output
/// @brief General Purpose Input Output driver
///
/// @li @ref gpio_intro
///
///@n @section gpio_intro Introduction
///  This module contains functions to control the GPIO peripheral of Silicon Labs 32-bit MCUs and SoCs.
///  The GPIO driver is used for external and EM4 interrupt configuration, port and pin configuration.
///  as well as manages the interrupt handler.
///
/// @{
// *****************************************************************************
/* *INDENT-ON* */

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/// GPIO Pin directions.
SL_ENUM(sl_gpio_pin_direction_t) {
  /// Input direction.
  SL_GPIO_PIN_DIRECTION_IN = 0,
  /// Output direction.
  SL_GPIO_PIN_DIRECTION_OUT
};

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief The `sl_gpio_pin_config_t` structure is used to define the
 * configuration of a GPIO pin, including its mode and direction. This
 * structure is part of the GPIO driver API for Silicon Labs
 * microcontrollers, allowing users to specify how a GPIO pin should
 * behave in terms of its operational mode and whether it is set as an
 * input or output. The `mode` member determines the specific mode
 * settings for the pin, while the `direction` member specifies whether
 * the pin is configured for input or output operations.
 *
 * @param mode Specifies the mode of the GPIO pin, defined by the type
 * `sl_gpio_mode_t`.
 * @param direction Indicates the direction of the GPIO pin, defined by the type
 * `sl_gpio_pin_direction_t`.
 ******************************************************************************/
typedef struct {
  sl_gpio_mode_t mode;
  sl_gpio_pin_direction_t direction;
} sl_gpio_pin_config_t;

/*******************************************************************************
 *******************************   TYPEDEFS   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * GPIO interrupt callback function pointer.
 *
 * @param int_no The pin interrupt number to which the callback function is invoked for.
 * @param context Pointer to callback context.
 ******************************************************************************/
typedef void (*sl_gpio_irq_callback_t)(uint8_t int_no, void *context);

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function prepares the GPIO driver for use by enabling the
 * necessary bus clock and ensuring that the GPIO interrupts are enabled.
 * It should be called before any other GPIO operations to ensure that
 * the GPIO hardware is properly configured and ready for use. This
 * function does not take any parameters and always returns a success
 * status, indicating that the initialization was successful. It is
 * important to call this function once during the system initialization
 * phase to ensure that the GPIO module is correctly set up.
 *
 * @return Returns SL_STATUS_OK to indicate successful initialization.
 ******************************************************************************/
sl_status_t sl_gpio_init(void);

/***************************************************************************//**
 * @brief This function configures the direction of a specified GPIO pin to
 * either input or output. It should be used when you need to change the
 * operational mode of a GPIO pin. The function requires that the GPIO
 * configuration is not locked, as it will return an error if the
 * configuration is in a locked state. It is important to ensure that the
 * provided GPIO structure and direction are valid, as invalid parameters
 * will result in an error. This function is typically called after
 * initializing the GPIO module and before performing any operations that
 * depend on the pin's direction.
 *
 * @param gpio Pointer to a GPIO structure containing the port and pin
 * information. Must not be null. The port and pin must be valid,
 * otherwise, the function returns SL_STATUS_INVALID_PARAMETER.
 * @param pin_direction Specifies the desired direction for the GPIO pin, either
 * SL_GPIO_PIN_DIRECTION_IN for input or
 * SL_GPIO_PIN_DIRECTION_OUT for output. Must be a valid
 * direction, otherwise, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @return Returns SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if gpio is
 * null, SL_STATUS_INVALID_PARAMETER if the port, pin, or direction is
 * invalid, and SL_STATUS_INVALID_STATE if the GPIO configuration is
 * locked.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin_direction(const sl_gpio_t *gpio,
                                      sl_gpio_pin_direction_t pin_dir);

/***************************************************************************//**
 * @brief This function configures the mode of a specified GPIO pin and sets its
 * output value if applicable. It should be used when you need to change
 * the operational mode of a GPIO pin, such as switching between input
 * and output modes. The function requires that the GPIO configuration is
 * not locked, and it will return an error if the GPIO structure pointer
 * is null or if the mode or pin parameters are invalid. Ensure that the
 * GPIO configuration is unlocked before calling this function.
 *
 * @param gpio Pointer to a GPIO structure containing the port and pin to be
 * configured. Must not be null. The function will return
 * SL_STATUS_NULL_POINTER if this parameter is null.
 * @param mode The desired mode for the GPIO pin. Must be a valid mode as
 * defined by the system. If the mode is invalid, the function will
 * return SL_STATUS_INVALID_PARAMETER.
 * @param output_value Boolean value indicating the desired output state of the
 * pin if it is configured as an output. This parameter also
 * influences the pull-up/pull-down configuration for
 * certain input modes.
 * @return Returns SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if the gpio
 * parameter is null, SL_STATUS_INVALID_PARAMETER if the mode or pin is
 * invalid, and SL_STATUS_INVALID_STATE if the GPIO configuration is
 * locked.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin_mode(const sl_gpio_t *gpio,
                                 sl_gpio_mode_t mode,
                                 bool output_value);

/***************************************************************************//**
 * @brief This function is used to obtain the current configuration of a
 * specified GPIO pin, including its mode and direction. It is essential
 * to ensure that both the `gpio` and `pin_config` pointers are valid and
 * not null before calling this function. The function will return an
 * error status if the specified GPIO port and pin are invalid or if any
 * of the input pointers are null. It is typically used when you need to
 * verify or log the current settings of a GPIO pin.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure that specifies the GPIO
 * port and pin. Must not be null. The function will return
 * `SL_STATUS_INVALID_PARAMETER` if the port or pin is invalid.
 * @param pin_config A pointer to an `sl_gpio_pin_config_t` structure where the
 * pin configuration will be stored. Must not be null. The
 * function will return `SL_STATUS_NULL_POINTER` if this
 * pointer is null.
 * @return Returns `SL_STATUS_OK` if the configuration is successfully
 * retrieved. Returns `SL_STATUS_NULL_POINTER` if any input pointer is
 * null, or `SL_STATUS_INVALID_PARAMETER` if the port or pin is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_get_pin_config(const sl_gpio_t *gpio,
                                   sl_gpio_pin_config_t *pin_config);

/***************************************************************************//**
 * @brief This function is used to set a specific GPIO pin to a high state,
 * which is typically used to enable or activate a connected device or
 * signal. It should be called when the GPIO pin has been properly
 * configured and is ready to be set. The function requires a valid GPIO
 * structure pointer, which includes the port and pin information. If the
 * provided pointer is null or the port and pin combination is invalid,
 * the function will return an error status. It is important to ensure
 * that the GPIO configuration is not locked before calling this
 * function.
 *
 * @param gpio A pointer to a sl_gpio_t structure containing the port and pin to
 * be set. Must not be null. The port and pin must be valid;
 * otherwise, the function returns an error.
 * @return Returns SL_STATUS_OK if the pin is successfully set. Returns
 * SL_STATUS_NULL_POINTER if the gpio parameter is null, and
 * SL_STATUS_INVALID_PARAMETER if the port or pin is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin(const sl_gpio_t *gpio);

/***************************************************************************//**
 * @brief This function is used to clear a specific GPIO pin, setting its output
 * to a low state. It should be called when you need to ensure that a pin
 * is driven low, typically as part of controlling external hardware. The
 * function requires a valid GPIO structure pointer, which specifies the
 * port and pin to be cleared. It is important to ensure that the GPIO
 * structure is not null and that the specified port and pin are valid,
 * as the function will return an error status if these conditions are
 * not met. This function is atomic, meaning it is safe to use in
 * interrupt contexts.
 *
 * @param gpio Pointer to a GPIO structure containing the port and pin to be
 * cleared. Must not be null, and the port and pin must be valid. If
 * invalid, the function returns an error status.
 * @return Returns SL_STATUS_OK if the pin is successfully cleared,
 * SL_STATUS_NULL_POINTER if the gpio parameter is null, or
 * SL_STATUS_INVALID_PARAMETER if the port or pin is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_clear_pin(const sl_gpio_t *gpio);

/***************************************************************************//**
 * @brief Use this function to change the state of a GPIO pin from high to low
 * or from low to high. It is essential to ensure that the GPIO structure
 * provided is valid and not null before calling this function. The
 * function should be called when you need to toggle the state of a pin,
 * such as in applications requiring blinking LEDs or toggling control
 * signals. The function handles invalid parameters by returning specific
 * status codes, ensuring robust error handling.
 *
 * @param gpio A pointer to an sl_gpio_t structure containing the port and pin
 * to toggle. Must not be null, and the port and pin must be valid.
 * If invalid, the function returns SL_STATUS_NULL_POINTER or
 * SL_STATUS_INVALID_PARAMETER.
 * @return Returns SL_STATUS_OK if the pin is successfully toggled,
 * SL_STATUS_NULL_POINTER if the gpio parameter is null, or
 * SL_STATUS_INVALID_PARAMETER if the port or pin is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_toggle_pin(const sl_gpio_t *gpio);

/***************************************************************************//**
 * @brief Use this function to obtain the current output state of a specific
 * GPIO pin when it is configured in output mode. It is essential to
 * ensure that both the `gpio` and `pin_value` pointers are valid and not
 * null before calling this function. The function will return an error
 * status if the GPIO port or pin is invalid, or if any of the pointers
 * are null. This function is typically used in applications where the
 * output state of a GPIO pin needs to be monitored or verified.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure that specifies the GPIO
 * port and pin. Must not be null. The port and pin must be valid;
 * otherwise, the function returns an error.
 * @param pin_value A pointer to a boolean variable where the output state of
 * the specified pin will be stored. Must not be null. The
 * function will return an error if this pointer is null.
 * @return Returns `SL_STATUS_OK` if successful, `SL_STATUS_NULL_POINTER` if any
 * pointer is null, or `SL_STATUS_INVALID_PARAMETER` if the port or pin
 * is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_get_pin_output(const sl_gpio_t *gpio,
                                   bool *pin_value);

/***************************************************************************//**
 * @brief This function is used to obtain the current input state of a specified
 * GPIO pin, which must be configured in input mode. It is essential to
 * ensure that the GPIO structure and the pin value pointer provided are
 * not null, as the function will return an error status if they are.
 * Additionally, the specified port and pin must be valid; otherwise, an
 * invalid parameter status will be returned. This function is typically
 * called when you need to read the state of a pin that is expected to
 * receive input signals.
 *
 * @param gpio A pointer to an sl_gpio_t structure that specifies the port and
 * pin to be read. Must not be null, and the port and pin must be
 * valid.
 * @param pin_value A pointer to a boolean variable where the input state of the
 * pin will be stored. Must not be null.
 * @return Returns SL_STATUS_OK if the operation is successful,
 * SL_STATUS_NULL_POINTER if any input pointers are null, or
 * SL_STATUS_INVALID_PARAMETER if the port or pin is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_get_pin_input(const sl_gpio_t *gpio,
                                  bool *pin_value);

/***************************************************************************//**
 * @brief This function is used to set specific pins on a designated GPIO port.
 * It is essential to ensure that the port provided is valid before
 * calling this function, as an invalid port will result in an error. The
 * function operates atomically to prevent race conditions, making it
 * suitable for use in interrupt-driven environments. It returns a status
 * code indicating success or failure, which should be checked to confirm
 * the operation's success.
 *
 * @param port The GPIO port to access. Must be a valid port identifier;
 * otherwise, the function returns SL_STATUS_INVALID_PARAMETER.
 * @param pins A bitmask representing the pins to set on the specified port.
 * Each bit corresponds to a pin on the port, where a bit value of 1
 * sets the corresponding pin.
 * @return Returns SL_STATUS_OK if the operation is successful, or
 * SL_STATUS_INVALID_PARAMETER if the port is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_set_port(sl_gpio_port_t port,
                             uint32_t pins);

/***************************************************************************//**
 * @brief Use this function to clear specific pins on a designated GPIO port by
 * providing a bitmask of the pins to be cleared. This function is
 * typically used when you need to reset the state of certain pins to
 * low. It is important to ensure that the port specified is valid;
 * otherwise, the function will return an error status. The function
 * operates atomically to prevent race conditions in interrupt-driven
 * environments.
 *
 * @param port The GPIO port to access. Must be a valid port identifier;
 * otherwise, the function returns SL_STATUS_INVALID_PARAMETER.
 * @param pins A bitmask representing the pins to clear on the specified port.
 * Each bit corresponds to a pin on the port.
 * @return Returns SL_STATUS_OK if the operation is successful, or
 * SL_STATUS_INVALID_PARAMETER if the port is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_clear_port(sl_gpio_port_t port,
                               uint32_t pins);

/***************************************************************************//**
 * @brief This function is used to obtain the current output state of all pins
 * on a specified GPIO port. It is essential to ensure that the port
 * parameter is valid and that the port_value pointer is not null before
 * calling this function. The function is typically used in scenarios
 * where the current output configuration of a GPIO port needs to be read
 * for monitoring or debugging purposes. It is important to handle the
 * return status to check for any errors, such as invalid parameters or
 * null pointers.
 *
 * @param port The GPIO port to access. Must be a valid port identifier. If the
 * port is invalid, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @param port_value A pointer to a uint32_t where the output state of the
 * port's pins will be stored. Must not be null. If null, the
 * function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if successful, SL_STATUS_INVALID_PARAMETER if
 * the port is invalid, or SL_STATUS_NULL_POINTER if port_value is null.
 ******************************************************************************/
sl_status_t sl_gpio_get_port_output(sl_gpio_port_t port,
                                    uint32_t *port_value);

/***************************************************************************//**
 * @brief This function is used to obtain the current input state of all pins on
 * a specified GPIO port. It is essential to ensure that the port
 * parameter is valid and that the port_value pointer is not null before
 * calling this function. The function should be used when you need to
 * read the input states of a GPIO port, typically in applications where
 * monitoring the state of multiple pins is required. The function
 * operates atomically to ensure that the read operation is not
 * interrupted, providing a consistent snapshot of the port's input
 * state.
 *
 * @param port The GPIO port to access. Must be a valid port identifier;
 * otherwise, the function returns SL_STATUS_INVALID_PARAMETER.
 * @param port_value A pointer to a uint32_t where the input state of the port's
 * pins will be stored. Must not be null; otherwise, the
 * function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if successful, SL_STATUS_INVALID_PARAMETER if
 * the port is invalid, or SL_STATUS_NULL_POINTER if port_value is null.
 ******************************************************************************/
sl_status_t sl_gpio_get_port_input(sl_gpio_port_t port,
                                   uint32_t *port_value);

/***************************************************************************//**
 * @brief This function sets up a GPIO pin to trigger an external interrupt and
 * registers a callback function to be called when the interrupt occurs.
 * It should be used when you need to handle external events on a
 * specific GPIO pin. The function requires a valid GPIO structure and
 * interrupt flags, and it can automatically determine an interrupt
 * number if one is not provided. It is important to ensure that the GPIO
 * configuration is unlocked before calling this function. The function
 * will return an error status if any parameters are invalid or if no
 * interrupt number is available.
 *
 * @param gpio Pointer to a GPIO structure containing the port and pin to
 * configure. Must not be null and must represent a valid port and
 * pin.
 * @param int_no Pointer to an integer that serves as both input and output for
 * the interrupt number. If the caller does not have an interrupt
 * number, -1 (SL_GPIO_INTERRUPT_UNAVAILABLE) can be passed, and
 * the function will assign a valid interrupt number if available.
 * Must not be null.
 * @param flags Interrupt flags that determine the conditions under which the
 * interrupt is triggered, such as rising or falling edge. Must be
 * valid flags.
 * @param gpio_callback Pointer to a callback function that will be called when
 * the interrupt is triggered. The function should match
 * the sl_gpio_irq_callback_t signature.
 * @param context Pointer to a user-defined context that will be passed to the
 * callback function. Can be null if no context is needed.
 * @return Returns SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if gpio or
 * int_no is null, SL_STATUS_INVALID_PARAMETER if any parameters are
 * invalid, and SL_STATUS_NOT_FOUND if no interrupt number is available.
 ******************************************************************************/
sl_status_t sl_gpio_configure_external_interrupt(const sl_gpio_t *gpio,
                                                 int32_t *int_no,
                                                 sl_gpio_interrupt_flag_t flags,
                                                 sl_gpio_irq_callback_t gpio_callback,
                                                 void *context);

/***************************************************************************//**
 * @brief Use this function to disable and unregister a previously configured
 * GPIO external interrupt. It clears any pending interrupts and disables
 * the specified interrupt number, ensuring that no further callbacks are
 * triggered for that interrupt. This function should be called when an
 * external interrupt is no longer needed or before reconfiguring it. The
 * interrupt number must be valid and within the range of available
 * interrupts; otherwise, the function will return an error status.
 *
 * @param int_no The interrupt number to be deconfigured. It must be a valid
 * interrupt number within the range of available interrupts (0 to
 * SL_HAL_GPIO_INTERRUPT_MAX). If the interrupt number is invalid,
 * the function returns SL_STATUS_INVALID_PARAMETER.
 * @return Returns SL_STATUS_OK if the deconfiguration is successful, or
 * SL_STATUS_INVALID_PARAMETER if the interrupt number is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_deconfigure_external_interrupt(int32_t int_no);

/***************************************************************************//**
 * @brief This function is used to enable GPIO interrupts based on the provided
 * bitmask. It should be called when you need to activate specific
 * interrupt sources for GPIO operations. Ensure that the GPIO system is
 * properly initialized before calling this function. The function
 * operates atomically to prevent race conditions during the enabling
 * process.
 *
 * @param int_mask A bitmask specifying which GPIO interrupt sources to enable.
 * Each bit in the mask corresponds to a specific interrupt
 * source. The caller is responsible for ensuring that the mask
 * is valid and corresponds to available interrupt sources.
 * @return Returns SL_STATUS_OK if the operation is successful.
 ******************************************************************************/
sl_status_t sl_gpio_enable_interrupts(uint32_t int_mask);

/***************************************************************************//**
 * @brief Use this function to disable one or more GPIO interrupts specified by
 * the bitmask. This is typically used when you need to prevent
 * interrupts from occurring during critical sections of code or when the
 * interrupts are no longer needed. The function must be called with a
 * valid bitmask representing the interrupts to be disabled. It is
 * important to ensure that the interrupts you intend to disable are
 * correctly represented in the bitmask to avoid unintended behavior.
 *
 * @param int_mask A bitmask specifying which GPIO interrupt sources to disable.
 * Each bit in the mask corresponds to a specific interrupt
 * source. The caller must ensure that the bitmask is correctly
 * set to represent the desired interrupts to disable.
 * @return Returns SL_STATUS_OK if the operation is successful.
 ******************************************************************************/
sl_status_t sl_gpio_disable_interrupts(uint32_t int_mask);

/***************************************************************************//**
 * @brief This function sets up a GPIO pin to trigger an EM4 wakeup interrupt
 * and registers a callback function to be called when the interrupt
 * occurs. It should be used when you need a GPIO pin to wake the system
 * from EM4 sleep mode. The function requires a valid GPIO structure and
 * an interrupt number, which can be provided or generated if
 * unavailable. The polarity of the interrupt can be set to active high
 * or low. Ensure that the GPIO configuration is unlocked before calling
 * this function. The function will return specific status codes if
 * parameters are invalid or if no interrupt number is available.
 *
 * @param gpio Pointer to a GPIO structure containing the port and pin
 * configuration. Must not be null and must represent a valid GPIO
 * port and pin.
 * @param em4_int_no Pointer to an integer representing the EM4 interrupt
 * number. Can be set to -1 if unavailable, and will be
 * updated with the assigned interrupt number. Must not be
 * null.
 * @param polarity Boolean value indicating the desired wakeup polarity: true
 * for active high, false for active low.
 * @param gpio_callback Pointer to a callback function to be invoked when the
 * interrupt occurs. Can be null if no callback is needed.
 * @param context Pointer to user-defined data to be passed to the callback
 * function. Can be null.
 * @return Returns SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if gpio or
 * em4_int_no is null, SL_STATUS_INVALID_PARAMETER if the GPIO port or
 * pin is invalid, and SL_STATUS_NOT_FOUND if no interrupt number is
 * available.
 ******************************************************************************/
sl_status_t sl_gpio_configure_wakeup_em4_interrupt(const sl_gpio_t *gpio,
                                                   int32_t *em4_int_no,
                                                   bool polarity,
                                                   sl_gpio_irq_callback_t gpio_callback,
                                                   void *context);

/***************************************************************************//**
 * @brief Use this function to unregister a callback, disable and clear the
 * specified EM4 interrupt, and disable the GPIO pin wake-up from EM4. It
 * is typically called when the EM4 wake-up functionality is no longer
 * needed or before reconfiguring the interrupt. Ensure that the provided
 * interrupt number is valid and within the acceptable range before
 * calling this function.
 *
 * @param em4_int_no The EM4 wakeup interrupt number to be deconfigured. It must
 * be a valid interrupt number within the range of available
 * EM4 interrupts. If the number is invalid, the function
 * returns SL_STATUS_INVALID_PARAMETER.
 * @return Returns SL_STATUS_OK if the operation is successful, or
 * SL_STATUS_INVALID_PARAMETER if the provided interrupt number is
 * invalid.
 ******************************************************************************/
sl_status_t sl_gpio_deconfigure_wakeup_em4_interrupt(int32_t em4_int_no);

/***************************************************************************//**
 * @brief This function configures the specified GPIO pins to wake up the system
 * from EM4 (Energy Mode 4) by enabling the EM4 wake-up functionality and
 * setting the desired polarity for the wake-up signal. It should be used
 * when you need to configure GPIO pins to act as wake-up sources from
 * EM4, ensuring that the correct interrupt and polarity masks are
 * provided. The function must be called with valid masks, and it is
 * expected to be used in a context where atomic operations are necessary
 * to prevent race conditions.
 *
 * @param em4_int_mask A bitmask specifying which EM4 wake-up interrupts to
 * enable. Each bit corresponds to a specific GPIO pin. Must
 * be a valid bitmask for the target hardware.
 * @param em4_polarity_mask A bitmask specifying the polarity for each EM4 wake-
 * up interrupt. Each bit corresponds to a specific
 * GPIO pin, where the bit value determines the active
 * polarity (e.g., high or low). Must be a valid
 * bitmask for the target hardware.
 * @return Returns SL_STATUS_OK if the operation is successful.
 ******************************************************************************/
sl_status_t sl_gpio_enable_pin_em4_wakeup(uint32_t em4_int_mask,
                                          uint32_t em4_polarity_mask);

/***************************************************************************//**
 * @brief This function is used to disable the EM4 wake-up capability for GPIO
 * pins specified by the provided mask. It is typically called when the
 * wake-up functionality is no longer needed or to reconfigure the wake-
 * up settings. The function must be called with a valid mask that
 * specifies which EM4 wake-up interrupts to disable. It is important to
 * ensure that the system is in a state where disabling these wake-up
 * sources will not interfere with required wake-up functionality.
 *
 * @param em4_int_mask A bitmask specifying which EM4 wake-up interrupts to
 * disable. Each bit corresponds to a specific wake-up
 * source. The mask must be valid and correctly represent
 * the desired wake-up sources to disable.
 * @return Returns SL_STATUS_OK if the operation is successful.
 ******************************************************************************/
sl_status_t sl_gpio_disable_pin_em4_wakeup(uint32_t em4_int_mask);

/***************************************************************************//**
 * @brief This function is used to control the retention of GPIO pin settings
 * such as output enable, output value, pull enable, and pull direction
 * when the system enters EM4 (Energy Mode 4). It should be called when
 * configuring the system for low power modes where pin state retention
 * is required or should be disabled. The function must be called with a
 * valid boolean value to either enable or disable the retention feature.
 * It is important to ensure that the GPIO configuration is not locked
 * before calling this function.
 *
 * @param enable A boolean value where 'true' enables EM4 pin retention and
 * 'false' disables it. The parameter must be a valid boolean
 * value, and the function does not handle invalid inputs.
 * @return Returns SL_STATUS_OK if the operation is successful.
 ******************************************************************************/
sl_status_t sl_gpio_set_pin_em4_retention(bool enable);

/***************************************************************************//**
 * @brief This function configures the slew rate for a given GPIO port, which
 * can affect the signal transition speed on the port's pins. It should
 * be used when you need to adjust the electrical characteristics of the
 * GPIO signals for specific application requirements. The function must
 * be called with a valid port identifier; otherwise, it will return an
 * error status. It is important to ensure that the port is valid before
 * calling this function to avoid unexpected behavior.
 *
 * @param port The GPIO port to configure. Must be a valid port identifier;
 * otherwise, the function returns SL_STATUS_INVALID_PARAMETER.
 * @param slewrate The desired slew rate to set for the GPIO port. The specific
 * range or values for slewrate are not detailed in the header,
 * so it is assumed to be a valid 8-bit value.
 * @return Returns SL_STATUS_OK if the operation is successful, or
 * SL_STATUS_INVALID_PARAMETER if the port is invalid.
 ******************************************************************************/
sl_status_t sl_gpio_set_slew_rate(sl_gpio_port_t port,
                                  uint8_t slewrate);

/***************************************************************************//**
 * @brief This function is used to obtain the current slew rate setting for a
 * specified GPIO port. It is essential to ensure that the port provided
 * is valid and that the pointer to store the slew rate is not null. The
 * function should be called when the current slew rate configuration of
 * a GPIO port is needed, and it operates safely by entering an atomic
 * section to prevent concurrent access issues. If the port is invalid or
 * the pointer is null, the function will return an error status.
 *
 * @param port The GPIO port for which the slew rate is to be retrieved. Must be
 * a valid port identifier; otherwise, the function returns
 * SL_STATUS_INVALID_PARAMETER.
 * @param slewrate A pointer to a uint8_t where the slew rate of the specified
 * port will be stored. Must not be null; otherwise, the
 * function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if the operation is successful,
 * SL_STATUS_INVALID_PARAMETER if the port is invalid, or
 * SL_STATUS_NULL_POINTER if the slewrate pointer is null.
 ******************************************************************************/
sl_status_t sl_gpio_get_slew_rate(sl_gpio_port_t port,
                                  uint8_t *slewrate);

/***************************************************************************//**
 * @brief Use this function to lock the GPIO configuration, preventing any
 * changes to the GPIO settings. This is useful for ensuring that the
 * GPIO configuration remains stable and unchanged during critical
 * operations. Once locked, functions such as setting pin modes or
 * configuring interrupts will be restricted until the configuration is
 * unlocked using the appropriate unlock function. This function should
 * be called when you want to secure the current GPIO setup from
 * accidental modifications.
 *
 * @return Returns SL_STATUS_OK to indicate the operation was successful.
 ******************************************************************************/
sl_status_t sl_gpio_lock(void);

/***************************************************************************//**
 * @brief Use this function to unlock the GPIO configuration, allowing
 * subsequent configuration changes. It is essential to unlock the GPIO
 * configuration before using certain functions like setting pin modes or
 * configuring interrupts. This function should be called after the GPIO
 * configuration has been locked and before any operations that require
 * an unlocked state.
 *
 * @return Returns SL_STATUS_OK to indicate successful unlocking of the GPIO
 * configuration.
 ******************************************************************************/
sl_status_t sl_gpio_unlock(void);

/***************************************************************************//**
 * @brief This function checks whether the GPIO configuration registers are
 * currently locked or unlocked. It is useful for determining if certain
 * GPIO operations can be performed, as some functions require the GPIO
 * configuration to be unlocked. The function must be called with a valid
 * pointer to a boolean variable where the lock status will be stored. If
 * the provided pointer is null, the function will return an error
 * status.
 *
 * @param state A pointer to a boolean variable where the lock status will be
 * stored. Must not be null. If null, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if the operation is successful and the lock
 * status is stored in the provided variable. Returns
 * SL_STATUS_NULL_POINTER if the state parameter is null.
 ******************************************************************************/
sl_status_t sl_gpio_is_locked(bool *state);

/** @} (end addtogroup gpio driver) */
#ifdef __cplusplus
}
#endif

#endif /* SL_GPIO_H */
