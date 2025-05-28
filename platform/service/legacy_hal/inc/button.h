/***************************************************************************//**
 * @file
 * @brief See @ref button for documentation.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup legacyhal
 * @{
 ******************************************************************************/

/** @addtogroup button
 * @brief Sample API functions for using push-buttons.
 *
 * See button.h for source code.
 *@{
 */

/** @name Button State Definitions
 * A set of numerical definitions for use with the button APIs indicating the
 * state of a button.
 *@{
 */

/** @brief Button state is pressed.
 */
#define BUTTON_PRESSED  1

/** @brief Button state is released.
 */
#define BUTTON_RELEASED 0

/**@} END Button State Definitions */

/** @brief Initializes the buttons. This function is automatically called
 * by ::halInit().
 */
void halInternalInitButton(void);

/** @brief Returns the current state (pressed or released) of a button.
 *
 * @note This function is correlated with ::halButtonIsr() and so returns the
 * shadow state rather than reading the actual state of the pin.
 *
 * @param button  The button being queried, either BUTTON0 or BUTTON1 as
 * defined in the appropriate BOARD_HEADER.
 *
 * @return ::BUTTON_PRESSED if the button is pressed or ::BUTTON_RELEASED
 * if the button is not pressed.
 */
uint8_t halButtonState(uint8_t button);

/***************************************************************************//**
 * @brief This function is a callback that must be implemented by the
 * application to handle button state changes in an interrupt context. It
 * is called whenever a button configured for interrupt mode changes its
 * state. The function should contain the logic to be executed in
 * response to the button state change, such as triggering other
 * functions or updating application state. It is essential for
 * applications that need to respond to button presses or releases in
 * real-time.
 *
 * @param button The identifier of the button that has changed state, typically
 * defined as BUTTON0 or BUTTON1 in the board's header file. The
 * value must correspond to a valid button configured for
 * interrupt mode.
 * @param state The new state of the button, which can be either BUTTON_PRESSED
 * or BUTTON_RELEASED. This indicates whether the button was
 * pressed or released.
 * @return None
 ******************************************************************************/
void halButtonIsr(uint8_t button, uint8_t state);

/** @} END addtogroup
 */
/** @} (end addtogroup legacyhal) */
