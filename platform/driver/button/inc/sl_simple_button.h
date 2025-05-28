/***************************************************************************//**
 * @file
 * @brief Simple Button Driver
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

#ifndef SL_SIMPLE_BUTTON_H
#define SL_SIMPLE_BUTTON_H

#include "sl_button.h"
#include "sl_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup button
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup simple_button Simple Button Driver
 * @details Simple Button Driver module provides APIs to initalize and read
 *          simple buttons. Subsequent sections provide more insight into button
 *          driver configuration and usage.
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/

#define SL_SIMPLE_BUTTON_MODE_POLL               0U   ///< BUTTON input capture using polling
#define SL_SIMPLE_BUTTON_MODE_POLL_AND_DEBOUNCE  1U   ///< BUTTON input capture using polling and debouncing
#define SL_SIMPLE_BUTTON_MODE_INTERRUPT          2U   ///< BUTTON input capture using interrupt

#define SL_SIMPLE_BUTTON_DISABLED                2U   ///< BUTTON state is disabled
#define SL_SIMPLE_BUTTON_PRESSED                 1U   ///< BUTTON state is pressed
#define SL_SIMPLE_BUTTON_RELEASED                0U   ///< BUTTON state is released

#define SL_SIMPLE_BUTTON_GET_STATE(context) (((sl_simple_button_context_t *)(context))->state)  ///< BUTTON member function to get state
#define SL_SIMPLE_BUTTON_GET_PORT(context)  (((sl_simple_button_context_t *)(context))->port)   ///< BUTTON member function to get port
#define SL_SIMPLE_BUTTON_GET_PIN(context)   (((sl_simple_button_context_t *)(context))->pin)    ///< BUTTON member function to get pin
#define SL_SIMPLE_BUTTON_GET_MODE(context)  (((sl_simple_button_context_t *)(context))->mode)   ///< BUTTON member function to get mode

/*******************************************************************************
 *****************************   DATA TYPES   *********************************
 ******************************************************************************/

/// A Simple BUTTON instance
/***************************************************************************//**
 * @brief The `sl_simple_button_context_t` structure is used to represent the
 * context of a simple button in the Simple Button Driver module. It
 * contains information about the current state of the button, a history
 * of its states, the port and pin it is connected to, and the mode of
 * operation. This structure is essential for managing button
 * interactions, allowing the driver to track button states and handle
 * input events appropriately.
 *
 * @param state Current button state.
 * @param history History of button states.
 * @param port Button port.
 * @param pin Button pin.
 * @param mode Mode of operation.
 ******************************************************************************/
typedef struct {
  sl_button_state_t     state;          ///< Current button state
  uint16_t              history;        ///< History of button states
  sl_gpio_port_t        port;           ///< Button port
  uint8_t               pin;            ///< Button pin
  sl_button_mode_t      mode;           ///< Mode of operation
} sl_simple_button_context_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function initializes a simple button driver for a specified
 * button handle. It must be called before any other operations on the
 * button to ensure proper configuration. The function sets up the GPIO
 * pin associated with the button and configures it for either polling or
 * interrupt mode based on the button's mode setting. If the button is
 * set to interrupt mode, the function attempts to configure the
 * necessary interrupts. The function returns a status code indicating
 * success or failure of the initialization process.
 *
 * @param handle Pointer to a sl_button_t structure representing the button to
 * be initialized. The handle must not be null, and it should
 * point to a valid button configuration. The caller retains
 * ownership of the handle.
 * @return Returns an sl_status_t status code indicating the success or failure
 * of the initialization process. SL_STATUS_OK is returned on successful
 * initialization.
 ******************************************************************************/
sl_status_t sl_simple_button_init(const sl_button_t *handle);

/***************************************************************************//**
 * @brief This function is used to obtain the current state of a simple button,
 * which can be either pressed or released. It is essential to call this
 * function with a valid button handle, which should have been
 * initialized using the appropriate initialization function. The
 * function is useful in scenarios where the button's state needs to be
 * checked, such as in event-driven applications or polling loops. Ensure
 * that the handle provided is not null and points to a valid button
 * instance to avoid undefined behavior.
 *
 * @param handle Pointer to a button handle of type sl_button_t. Must not be
 * null and should point to a valid, initialized button instance.
 * If the handle is invalid, the behavior is undefined.
 * @return Returns the current state of the button, which can be
 * SL_SIMPLE_BUTTON_PRESSED or SL_SIMPLE_BUTTON_RELEASED.
 ******************************************************************************/
sl_button_state_t sl_simple_button_get_state(const sl_button_t *handle);

/***************************************************************************//**
 * @brief This function is used to poll the state of a simple button, updating
 * its internal state based on the current input from the button's GPIO
 * pin. It should be called periodically when the button is configured in
 * polling mode or polling with debounce mode. The function does nothing
 * if the button is disabled. It is important to ensure that the button
 * handle provided is valid and properly initialized before calling this
 * function.
 *
 * @param handle A pointer to a sl_button_t structure representing the button to
 * be polled. The handle must not be null and should point to a
 * valid, initialized button instance. If the button is disabled,
 * the function will return immediately without making any
 * changes.
 * @return None
 ******************************************************************************/
void sl_simple_button_poll_step(const sl_button_t *handle);

/***************************************************************************//**
 * @brief This function is used to enable a simple button that is currently in a
 * disabled state. It should be called when you want to reactivate a
 * button that has been previously disabled, allowing it to register
 * input events again. The function checks the current state of the
 * button and only performs the enabling process if the button is indeed
 * disabled. It clears the button's history and reinitializes it to
 * ensure it is ready for use. This function must be called with a valid
 * button handle, and it assumes that the button has been properly
 * initialized before being disabled.
 *
 * @param handle A pointer to an sl_button_t structure representing the button
 * to be enabled. The handle must not be null and should point to
 * a valid button instance. If the button is not in a disabled
 * state, the function will return immediately without making any
 * changes.
 * @return None
 ******************************************************************************/
void sl_simple_button_enable(const sl_button_t *handle);

/***************************************************************************//**
 * @brief Use this function to disable a simple button, preventing it from
 * generating further input events. This is useful when you want to
 * temporarily ignore button presses, such as during a critical operation
 * or when the button is not needed. The function should be called with a
 * valid button handle, and it will have no effect if the button is
 * already disabled. If the button is configured to use interrupts, the
 * function will also deconfigure the associated external interrupt.
 *
 * @param handle Pointer to a valid sl_button_t structure representing the
 * button to be disabled. Must not be null. The function will not
 * perform any action if the button is already disabled.
 * @return None
 ******************************************************************************/
void sl_simple_button_disable(const sl_button_t *handle);

/** @} (end addtogroup simple_button) */
/** @} (end addtogroup button) */

// ******** THE REST OF THE FILE IS DOCUMENTATION ONLY !***********************
/// @addtogroup simple_button Simple Button Driver
/// @{
///
///   @details
///
///
///   @n @section simple_button_intro Introduction
///
///   The Simple Button driver is a module of the button driver that provides the functionality
///   to initialize and read simple buttons.
///
///   @n @section simple_button_config Simple Button Configuration
///
///   Simple buttons use the @ref sl_button_t struct and their @ref sl_simple_button_context_t
///   struct. These are automatically generated into the following files, as well as
///   instance specific headers with macro definitions in them. The samples below
///   are for a single instance called "inst0".
///
///   @code{.c}
///// sl_simple_button_instances.c
///
///#include "sl_simple_button.h"
///#include "sl_simple_button_inst0_config.h"
///
///sl_simple_button_context_t simple_inst0_context = {
///  .state = 0,
///  .history = 0,
///  .port = SL_SIMPLE_BUTTON_INST0_PORT,
///  .pin = SL_SIMPLE_BUTTON_INST0_PIN,
///  .mode = SL_SIMPLE_BUTTON_INST0_MODE,
///};
///
///const sl_button_t sl_button_inst0 = {
///  .context = &simple_inst0_context,
///  .init = sl_simple_button_init,
///  .get_state = sl_simple_button_get_state,
///  .poll = sl_simple_button_poll_step,
///};
///
///const sl_button_t *sl_simple_button_array[] = {&sl_button_inst0};
///const uint8_t simple_button_count = 1;
///
///void sl_simple_button_init_instances(void)
///{
///  sl_button_init(&sl_button_inst0);
///}
///
///void sl_simple_button_poll_instances(void)
///{
///  sl_button_poll_step(&sl_button_inst0);
///}
///   @endcode
///
///   @note The sl_simple_button_instances.c file is shown with only one instance, but if more
///         were in use they would all appear in this .c file.
///
///   @code{.c}
///// sl_simple_button_instances.h
///
///#ifndef SL_SIMPLE_BUTTON_INSTANCES_H
///#define SL_SIMPLE_BUTTON_INSTANCES_H
///
///#include "sl_simple_button.h"
///
///extern const sl_button_t sl_button_inst0;
///
///void sl_simple_button_init_instances(void);
///void sl_simple_button_poll_instances(void);
///
///#endif // SL_SIMPLE_BUTTON_INSTANCES_H
///   @endcode
///
///   @note The sl_simple_button_instances.h file is shown with only one instance, but if more
///         were in use they would all appear in this .h file.
///
///   @n @section simple_button_usage Simple Button Usage
///
///   The simple button driver has no differences in its usage from the common button driver.
///   See @ref buttondrv_usage.
///
/// @} end group simple_button ********************************************************/

#ifdef __cplusplus
}
#endif

#endif // SL_SIMPLE_BUTTON_H
