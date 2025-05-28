/***************************************************************************//**
 * @file
 * @brief Button Driver
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

#ifndef SL_BUTTON_H
#define SL_BUTTON_H

#include "sl_common.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup button Button API
 * @brief Generic Button API
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   DEFINES   ************************************
 ******************************************************************************/

#define BUTTON_ERROR  0xFFFF              ///< Error when trying to return state

/*******************************************************************************
 *****************************   DATA TYPES   *********************************
 ******************************************************************************/

typedef uint8_t sl_button_mode_t;       ///< BUTTON mode
typedef uint8_t sl_button_state_t;      ///< BUTTON state
/***************************************************************************//**
 * @brief The `sl_button_t` structure represents a BUTTON instance in the
 * Silicon Laboratories button driver. It encapsulates a context pointer
 * and several function pointers for operations such as initialization,
 * polling, enabling, disabling, and retrieving the state of the button.
 * This structure is designed to provide a flexible interface for
 * managing button hardware, allowing for different modes of operation
 * such as interrupt-driven or polling-based state management.
 *
 * @param context The context for this BUTTON instance.
 * @param init Member function to initialize BUTTON instance.
 * @param poll Member function to poll BUTTON.
 * @param enable Member function to enable BUTTON.
 * @param disable Member function to disable BUTTON.
 * @param get_state Member function to retrieve BUTTON state.
 ******************************************************************************/
typedef struct sl_button sl_button_t;   ///< BUTTON Instance structure

/// A BUTTON instance
/***************************************************************************//**
 * @brief The `sl_button` structure represents a button instance in a button
 * driver system, encapsulating both the context and the function
 * pointers necessary for managing the button's lifecycle and state. It
 * includes function pointers for initializing, polling, enabling,
 * disabling, and retrieving the state of the button, allowing for
 * flexible and modular button management. This structure is part of a
 * platform-level software module designed to handle various types of
 * buttons, supporting different modes of operation such as interrupt and
 * polling.
 *
 * @param context The context for this BUTTON instance.
 * @param init Member function to initialize BUTTON instance.
 * @param poll Member function to poll BUTTON.
 * @param enable Member function to enable BUTTON.
 * @param disable Member function to disable BUTTON.
 * @param get_state Member function to retrieve BUTTON state.
 ******************************************************************************/
typedef struct sl_button {
  void                  *context;                       ///< The context for this BUTTON instance
  sl_status_t           (*init)(const sl_button_t *handle);   ///< Member function to initialize BUTTON instance
  void                  (*poll)(const sl_button_t *handle);         ///< Member function to poll BUTTON
  void                  (*enable)(const sl_button_t *handle);       ///< Member function to enable BUTTON
  void                  (*disable)(const sl_button_t *handle);      ///< Member function to disable BUTTON
  sl_button_state_t     (*get_state)(const sl_button_t *handle);    ///< Member function to retrieve BUTTON state
} sl_button;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function initializes a button instance, which must be done before
 * any other operations on the button can be performed. It sets up the
 * necessary configurations for the button, such as GPIO and mode of
 * operation, and prepares it for use. The function should be called once
 * for each button instance before calling any other button-related
 * functions. If the initialization function pointer within the button
 * instance is null, the function will return an error status.
 *
 * @param handle Pointer to a button instance of type sl_button_t. This
 * parameter must not be null, and the 'init' member function
 * within the structure must be defined. If 'init' is null, the
 * function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if the initialization is successful, or
 * SL_STATUS_NULL_POINTER if the 'init' function pointer is null.
 ******************************************************************************/
sl_status_t sl_button_init(const sl_button_t *handle);

/***************************************************************************//**
 * @brief This function is used to obtain the current state of a button
 * represented by the provided handle. It should be called after the
 * button has been properly initialized using `sl_button_init`. The
 * function checks if the `get_state` member function of the button
 * instance is available and calls it to retrieve the state. If the
 * `get_state` function is not available, it returns a predefined error
 * state. This function is essential for applications that need to
 * monitor button states, especially in event-driven or polling
 * scenarios.
 *
 * @param handle A pointer to an `sl_button_t` instance representing the button
 * whose state is to be retrieved. The pointer must not be null,
 * and the button instance should be properly initialized. If the
 * `get_state` function pointer within the instance is null, the
 * function returns an error state.
 * @return Returns the current state of the button as an `sl_button_state_t`. If
 * the `get_state` function is not available, it returns `BUTTON_ERROR`.
 ******************************************************************************/
sl_button_state_t sl_button_get_state(const sl_button_t *handle);

/***************************************************************************//**
 * @brief This function is used to enable a button instance, allowing it to
 * respond to interactions. It should be called when you want the button
 * to start processing input events. The function requires a valid button
 * instance, which must have been initialized prior to calling this
 * function. If the button instance's enable function pointer is not set,
 * the function will have no effect.
 *
 * @param handle A pointer to a button instance of type sl_button_t. The handle
 * must not be null and should point to a properly initialized
 * button instance. The function will not perform any action if
 * the enable function pointer within the button instance is null.
 * @return None
 ******************************************************************************/
void sl_button_enable(const sl_button_t *handle);

/***************************************************************************//**
 * @brief Use this function to disable a specific button instance, preventing it
 * from generating events or being polled. This function should be called
 * when you want to temporarily deactivate a button without altering its
 * configuration. It is important to ensure that the button instance has
 * been properly initialized before calling this function. If the
 * button's disable function pointer is null, the function will have no
 * effect.
 *
 * @param handle A pointer to an sl_button_t instance representing the button to
 * be disabled. Must not be null. The function will not perform
 * any action if the disable function pointer within the handle is
 * null.
 * @return None
 ******************************************************************************/
void sl_button_disable(const sl_button_t *handle);

/***************************************************************************//**
 * @brief This function is used to poll the state of a button instance by
 * invoking its associated poll function. It should be called when the
 * button is configured to operate in polling mode, typically from a
 * periodic task or main loop. The function requires that the button
 * instance has been properly initialized and that the poll function
 * pointer is not null. If the poll function is null, the function does
 * nothing. This function does not return any value or modify the input
 * parameters.
 *
 * @param handle A pointer to an sl_button_t instance representing the button to
 * be polled. The pointer must not be null, and the button
 * instance should be initialized before calling this function. If
 * the poll function within the button instance is null, the
 * function will not perform any action.
 * @return None
 ******************************************************************************/
void sl_button_poll_step(const sl_button_t *handle);

/***************************************************************************//**
 * A callback called in interrupt context whenever a button changes its state.
 *
 * @remark Can be implemented by the application if required. This function
 * can contain the functionality to be executed in response to changes of state
 * in each of the buttons, or callbacks to appropriate functionality.
 *
 * @note The button state should not be updated in this function, it is updated
 * by specific button driver prior to arriving here
 *
   @param[out] handle             Pointer to button instance
 ******************************************************************************/
void sl_button_on_change(const sl_button_t *handle);

/** @} (end addtogroup button) */

// ******** THE REST OF THE FILE IS DOCUMENTATION ONLY !***********************
/// @addtogroup button Button API
/// @{
///
///   @details
///
///   @n @section buttondrv_intro Introduction
///
///   The button driver is a platfom level software module that manages the initialization
///   and reading of various types of buttons. There is currently one type of button
///   supported by the button driver:
///
///   @li @ref simple_button
///
///   All button functions are called through the generic driver, which then references
///   functions in the simple button and other potential future button drivers.
///
///   @n @section buttondrv_config Configuration
///
///   All button instances are configured with an @ref sl_button_t struct and a type specific
///   context struct. These structs are automatically generated after a button is set up
///   using Simplicity Studio's wizard, along with a function definition for initializing all
///   LEDs of that type. Specific setup for the simple button is in the following section.
///
///   - [Simple Button Configuration](/gecko-platform/<docspace-docleaf-version>/platform-driver/simple-button#simple-button-configuration)
///
///   @n @section buttondrv_usage Usage
///
///   Once the button structs are defined, the common button functions can be called being
///   passed an instance of sl_button_t, which will be redirected to calling the type specific
///   version of that function. The common functions include the following:
///
///   @li @ref sl_button_init
///   @li @ref sl_button_get_state
///   @li @ref sl_button_poll_step
///   @li @ref sl_button_on_change
///
///   @ref sl_button_init must be called before attempting to read the state of the button.
///
///   The button driver can either be used with interrupt mode, polling or polling with debounce.
///   In the case of using interrupt mode, @ref sl_button_on_change can be implemented by the
///   application if required. This function can contain functionality to be executed in response
///   to button event or callbacks to appropriate functionality.
///   In the case of polling and polling with debounce mode, @ref sl_button_poll_step is used to
///   update the state, and needs to be called from a tick function or similar by the user.
///   These mode can be configured per button instance in the instance specific config file.
///
///   Both the interrupt and polling methods obtain the button state for the user by calling
///   @ref sl_button_get_state.
///
/// @} end group button ********************************************************/

#ifdef __cplusplus
}
#endif

#endif // SL_BUTTON_H
