/***************************************************************************//**
 * @file hal.h
 * @brief Header file for the Legacy HAL
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories, Inc, www.silabs.com</b>
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

#ifndef HAL_H
#define HAL_H

#ifdef TOKEN_MANAGER_TEST
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#else // TOKEN_MANAGER_TEST
#ifndef EZSP_HOST
#include "sl_zigbee.h"
#endif
#endif // TOKEN_MANAGER_TEST
#include "micro.h"
#include "serial.h"
#include "led.h"
#include "button.h"
#include "crc.h"

#include "random.h"
#if !defined(EZSP_HOST) && !defined(SL_ZIGBEE_TEST) && !defined(ZIGBEE_STACK_ON_HOST)
#include "sl_token_api.h"
#else
#include "token.h"
#endif
#ifdef CORTEXM3
#include "cortexm3/diagnostic.h"
#endif

#define SECURITY_BLOCK_SIZE   16 // in bytes

/***************************************************************************//**
 * @addtogroup legacyhal Legacy HAL
 * @brief The Legacy HAL is an implementation of the Hardware Abstraction Layer (HAL)
 *        that was used by the Zigbee EmberZNet and Flex Connect stacks before their
 *		  transition to the new component-based architecture (Zigbee version 7.0 and
 *		  Connect version 3.0).
 *        This component is not a complete duplicate of the old HAL. It only contains
 *        the most commonly used pieces to enable our networking stacks and sample
 *		  applications and to support porting customer applications to the new
 *		  component-based implementation.
 *        Currently, Zigbee and Connect sample applications are dependent on this component.
 *        Eventually, they will be ported to use new HAL components directly so that this
 *		  component can become optional or be deprecated.
 * @note This component is only designed to be used with Zigbee and Connect stack applications.
 *       Do not use with any other stacks as it will likely introduce problems.
 *
 * @{
 ******************************************************************************/

#ifndef SL_ZIGBEE_TEST
#define simulatedTimePasses()
#else
void simulatedTimePasses(void);
#endif // SL_ZIGBEE_TEST
/**
 * @brief This function will get 16u ms tick data
 *
 * @return
 */
uint16_t halCommonGetInt16uMillisecondTick(void);

/**
 * @brief This function will get 32u ms tick data
 *
 * @return
 */
uint32_t halCommonGetInt32uMillisecondTick(void);

/***************************************************************************//**
 * @brief Use this function to obtain the current system time in milliseconds,
 * represented as a 64-bit unsigned integer. This is useful for
 * applications that require precise time tracking or need to handle long
 * durations without overflow. The function is expected to be called in
 * environments where the system timer is properly initialized and
 * running. It is suitable for use in Zigbee and Connect stack
 * applications, as part of the Legacy HAL.
 *
 * @return Returns the current system time in milliseconds as a 64-bit unsigned
 * integer.
 ******************************************************************************/
uint64_t halCommonGetInt64uMillisecondTick(void);

/**
 * @brief This function will get 16u quater second tick data
 *
 * @return
 */
uint16_t halCommonGetInt16uQuarterSecondTick(void);

/***************************************************************************//**
 * @brief This function initializes and starts the system timer, which is
 * essential for time-based operations within the system. It should be
 * called during the system initialization phase to ensure that the timer
 * is ready for use. The function assumes that the underlying sleep timer
 * initialization will succeed; if it fails, the function will trigger an
 * assertion failure, indicating a critical error. This function is
 * typically used in environments where precise timing is crucial, such
 * as in embedded systems running Zigbee or Connect stack applications.
 *
 * @return Returns 0 to indicate successful initialization of the system timer.
 ******************************************************************************/
uint16_t halInternalStartSystemTimer(void);

/**
 * @brief This function will delay an amount of time in us
 *
 * @param us second in us
 */
void halCommonDelayMicroseconds(uint16_t us);

/**
 * @brief This function will delay an amount of time in ms
 *
 * @param ms second in ms
 */
void halCommonDelayMilliseconds(uint16_t ms);

/***************************************************************************//**
 * @brief This function is intended to set the system to an idle state for a
 * specified duration in milliseconds. However, due to the current
 * handling by the Power Manager, the function will always indicate that
 * the sleep was interrupted. It is primarily used in contexts where
 * legacy behavior is expected, but users should be aware that the
 * function does not perform actual idling as it might have in previous
 * implementations. This function is part of the Legacy HAL and is
 * designed for use with Zigbee and Connect stack applications.
 *
 * @param duration A pointer to a uint32_t representing the duration in
 * milliseconds for which the system should idle. The parameter
 * is not used in the current implementation, and the function
 * will not modify the value pointed to by this parameter. The
 * caller retains ownership of the memory.
 * @return Returns SL_STATUS_SUSPENDED, indicating that the intended idle
 * operation was interrupted.
 ******************************************************************************/
sl_status_t halCommonIdleForMilliseconds(uint32_t *duration);

/***************************************************************************//**
 * @brief This function is a placeholder to maintain compatibility with legacy
 * code that may expect its presence. It does not perform any operations
 * and can be safely called without any preconditions or side effects. It
 * is intended for use in systems transitioning from older hardware
 * abstraction layers to newer architectures, ensuring that existing
 * codebases remain functional without modification.
 *
 * @return None
 ******************************************************************************/
void halStackSymbolDelayAIsr(void);

/***************************************************************************//**
 * @brief This function is used to increment a boot counter token, which tracks
 * the number of times the stack has been booted, provided that the boot
 * counter feature is enabled. It is typically called during the boot
 * process of a Zigbee or Connect stack application to maintain a count
 * of boot events. This function has no effect if the boot counter
 * feature is not enabled in the build configuration.
 *
 * @return None
 ******************************************************************************/
void halStackProcessBootCount(void);

/***************************************************************************//**
 * @brief This function initializes the debug utilities, specifically targeting
 * systems where the virtual UART (VUART) is present. It should be called
 * during the system initialization phase to ensure that debug
 * capabilities are set up correctly. If the VUART is not present or
 * initialization fails, the function will return a failure status. This
 * function is particularly relevant in environments where debugging
 * through VUART is required.
 *
 * @return Returns SL_STATUS_OK if the initialization is successful and VUART is
 * present; otherwise, returns SL_STATUS_FAIL.
 ******************************************************************************/
sl_status_t sli_util_debug_init(void);

/***************************************************************************//**
 * @brief This function initializes the random number generator with entropy
 * sourced from the radio hardware. It should be called to ensure that
 * the random number generator is properly seeded before any random
 * numbers are generated, particularly in applications where randomness
 * is critical, such as cryptographic operations. The function does not
 * take any parameters and does not return a value. It is expected to be
 * used in environments where the radio hardware is available and
 * properly configured.
 *
 * @return None
 ******************************************************************************/
void sli_802154phy_radio_seed_random(void);

/** @} (end addtogroup legacyhal) */

#endif // HAL_H
