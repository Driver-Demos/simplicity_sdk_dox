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

#include "sl_button.h"
#include <stddef.h>

/**
 * @brief The `sl_button_init` function initializes a button by invoking its specific initialization function if available, otherwise returns a null pointer status.
 *
 * @param handle A pointer to an `sl_button_t` structure representing the button to be initialized.
 * @return Returns the status of the initialization operation, either the result of the `init` function or `SL_STATUS_NULL_POINTER` if the `init` function is not defined.
 */
sl_status_t sl_button_init(const sl_button_t *handle)
{
  if (handle->init != NULL) {
    return handle->init(handle);
  } else {
    return SL_STATUS_NULL_POINTER;
  }
}

/**
 * @brief The `sl_button_get_state` function retrieves the current state of a button using a function pointer from the button handle, or returns an error state if the function pointer is not set.
 *
 * @param handle A pointer to an `sl_button_t` structure, which contains a function pointer to retrieve the button's state.
 * @return The function returns the current state of the button as `sl_button_state_t` if the `get_state` function pointer is valid, otherwise it returns `BUTTON_ERROR`.
 */
sl_button_state_t sl_button_get_state(const sl_button_t *handle)
{
  if (handle->get_state != NULL) {
    return handle->get_state(handle);
  } else {
    return (sl_button_state_t)BUTTON_ERROR;
  }
}

/**
 * @brief The `sl_button_poll_step` function calls the `poll` method of a button handle if it is not NULL.
 *
 * @param handle A pointer to an `sl_button_t` structure, which represents a button and contains function pointers for various button operations.
 * @return This function does not return any value; it performs an action by calling the `poll` function if available.
 */
void sl_button_poll_step(const sl_button_t *handle)
{
  if (handle->poll != NULL) {
    handle->poll(handle);
  }
}

/**
 * @brief The `sl_button_enable` function enables a button by invoking its associated enable function if it exists.
 *
 * @param handle A pointer to an `sl_button_t` structure representing the button to be enabled.
 * @return This function does not return any value.
 */
void sl_button_enable(const sl_button_t *handle)
{
  if (handle->enable != NULL) {
    handle->enable(handle);
  }
}

/**
 * @brief The `sl_button_disable` function disables a button by invoking its `disable` method if it is not NULL.
 *
 * @param handle A pointer to an `sl_button_t` structure representing the button to be disabled.
 * @return This function does not return any value.
 */
void sl_button_disable(const sl_button_t *handle)
{
  if (handle->disable != NULL) {
    handle->disable(handle);
  }
}

/**
 * @brief The `sl_button_on_change` function is a weakly defined placeholder for handling button state changes, which currently does nothing.
 *
 * @param handle A pointer to an `sl_button_t` structure representing the button whose state has changed.
 * @return The function does not produce any output or perform any operations.
 */
SL_WEAK void sl_button_on_change(const sl_button_t *handle)
{
  (void)handle;
}
