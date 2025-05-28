/***************************************************************************//**
 * @file
 * @brief See @ref led for documentation.
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
#ifndef __LED_H__
#define __LED_H__

/***************************************************************************//**
 * @brief The HalBoardLedPins enumeration defines symbolic names for LED pins on
 * a board, with values that depend on the number of LEDs available
 * (BSP_LED_COUNT). It provides a flexible way to reference LED pins,
 * allowing for different configurations based on the number of LEDs, and
 * includes aliases for activity and heartbeat LEDs to standardize their
 * usage across different board setups.
 *
 * @param BOARDLED0 Represents the first LED pin, always set to 0.
 * @param BOARDLED1 Represents the second LED pin, varies based on
 * BSP_LED_COUNT.
 * @param BOARDLED2 Represents the third LED pin, varies based on BSP_LED_COUNT.
 * @param BOARDLED3 Represents the fourth LED pin, varies based on
 * BSP_LED_COUNT.
 * @param BOARD_ACTIVITY_LED Alias for the activity LED, typically set to
 * BOARDLED0.
 * @param BOARD_HEARTBEAT_LED Alias for the heartbeat LED, typically set to
 * BOARDLED1.
 ******************************************************************************/
enum HalBoardLedPins {
#if BSP_LED_COUNT <= 1
  BOARDLED0 = 0,
  BOARDLED1 = BOARDLED0,
  BOARDLED2 = BOARDLED0,
  BOARDLED3 = BOARDLED0,
  BOARD_ACTIVITY_LED  = BOARDLED0,
  BOARD_HEARTBEAT_LED = BOARDLED0
#endif
#if BSP_LED_COUNT == 2
  BOARDLED0 = 0,
  BOARDLED1 = 1,
  BOARDLED2 = BOARDLED0,
  BOARDLED3 = BOARDLED1,
  BOARD_ACTIVITY_LED  = BOARDLED0,
  BOARD_HEARTBEAT_LED = BOARDLED1
#endif
#if BSP_LED_COUNT == 3
  BOARDLED0 = 0,
  BOARDLED1 = 1,
  BOARDLED2 = 2,
  BOARDLED3 = BOARDLED1,
  BOARD_ACTIVITY_LED  = BOARDLED0,
  BOARD_HEARTBEAT_LED = BOARDLED1
#endif
#if BSP_LED_COUNT >= 4
  BOARDLED0 = 0,
  BOARDLED1 = 1,
  BOARDLED2 = 2,
  BOARDLED3 = 3,
  BOARD_ACTIVITY_LED  = BOARDLED0,
  BOARD_HEARTBEAT_LED = BOARDLED1
#endif
};

/***************************************************************************//**
 * @addtogroup legacyhal
 * @{
 ******************************************************************************/

/** @addtogroup led LED
 *  @brief  Sample API funtions for controlling LEDs
 *
 * When specifying an LED to use, always use the BOARDLEDx definitions that
 * are defined within the BOARD_HEADER.
 *
 * See led.h for source code.
 *@{
 */

/** @brief Configures GPIOs pertaining to the control of LEDs.
 */
void halInternalInitLed(void);

/** @brief Ensures that the definitions from the BOARD_HEADER
 *  are always used as parameters to the LED functions.
 */
#if defined(STACK) || defined(MINIMAL_HAL)
typedef uint8_t HalBoardLed;
#else
/***************************************************************************//**
 * @brief The `HalBoardLed` is an enumeration that defines symbolic names for
 * LED pins on a board, allowing for flexible configuration based on the
 * number of available LEDs. It provides a standardized way to reference
 * LED pins, ensuring that the correct pin is used for board activity and
 * heartbeat indications. The enumeration adapts to different board
 * configurations by assigning values to the LED pins based on the number
 * of LEDs defined by `BSP_LED_COUNT`.
 *
 * @param BOARDLED0 Represents the first LED pin, assigned the value 0.
 * @param BOARDLED1 Represents the second LED pin, with a value that varies
 * based on the number of LEDs.
 * @param BOARDLED2 Represents the third LED pin, typically assigned the same
 * value as BOARDLED0.
 * @param BOARDLED3 Represents the fourth LED pin, typically assigned the same
 * value as BOARDLED1.
 * @param BOARD_ACTIVITY_LED Alias for BOARDLED0, used to indicate board
 * activity.
 * @param BOARD_HEARTBEAT_LED Alias for BOARDLED1, used to indicate a heartbeat
 * signal.
 ******************************************************************************/
typedef enum HalBoardLedPins HalBoardLed;
#endif
// Note: Even though many compilers will use 16 bits for an enum instead of 8,
//  we choose to use an enum here.  The possible compiler inefficiency does not
//  affect stack-based parameters and local variables, which is the
//  general case for led paramters.

/***************************************************************************//**
 * @brief This function is used to signal radio activity by turning an LED on or
 * off, typically to provide visual feedback of transmission or reception
 * events. It should be called with a boolean value to indicate whether
 * the LED should be turned on or off. The function is designed to be
 * called twice in quick succession, first with the parameter set to true
 * to turn the LED on, and then with it set to false to turn the LED off.
 * This function is only effective if the hardware configuration includes
 * an activity LED and the necessary LED control definitions are present.
 *
 * @param turnOn A boolean value indicating whether to turn the activity LED on
 * (true) or off (false). The parameter must be provided, but its
 * effect depends on the presence of the appropriate hardware and
 * configuration.
 * @return None
 ******************************************************************************/
void halStackIndicateActivity(bool turnOn);

/** @} (end addtogroup led) */
/** @} (end addtogroup legacyhal) */

#endif // __LED_H__
