/***************************************************************************//**
 * @file
 * @brief Minimal Hal functions common across all microcontroller-specific files.
 * See @ref micro for documentation.
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

/** @addtogroup micro
 *@{
 */

#ifndef __MICRO_COMMON_H__
#define __MICRO_COMMON_H__

#ifdef SL_COMPONENT_CATALOG_PRESENT
#include "sl_component_catalog.h"
#endif

/** @brief Initializes microcontroller-specific peripherals.
 */
void halInit(void);

/** @brief Restarts the microcontroller and therefore everything else.
 */
void halReboot(void);

/** @brief The value that must be passed as the single parameter to
 *  ::halInternalDisableWatchDog() in order to successfully disable the watchdog
 *  timer.
 */
#define MICRO_DISABLE_WATCH_DOG_KEY 0xA5U

#ifdef SL_CATALOG_LEGACY_HAL_WDOG_PRESENT

/***************************************************************************//**
 * @brief This function is used to enable the watchdog timer on the
 * microcontroller, which is a safety feature that resets the system if
 * it becomes unresponsive. It should be called when the system is
 * initialized or when watchdog functionality is required to ensure
 * system reliability. The function configures the watchdog with default
 * settings, including a timeout period and a warning interrupt. It is
 * important to ensure that the watchdog is enabled in systems where it
 * is necessary to recover from software malfunctions.
 *
 * @return None
 ******************************************************************************/
void halInternalEnableWatchDog(void);

/***************************************************************************//**
 * @brief This function is used to disable the watchdog timer, which is a
 * hardware feature that resets the microcontroller if it becomes
 * unresponsive. It should be called when you need to ensure that the
 * watchdog does not reset the system, such as during critical operations
 * or debugging. To prevent accidental disabling, the function requires a
 * specific magic key to be passed as a parameter. This function is only
 * effective if the watchdog feature is present and enabled in the system
 * configuration.
 *
 * @param magicKey A uint8_t value that must be set to
 * MICRO_DISABLE_WATCH_DOG_KEY (0xA5U) to successfully disable
 * the watchdog timer. If an incorrect value is provided, the
 * function will not disable the watchdog.
 * @return None
 ******************************************************************************/
void halInternalDisableWatchDog(uint8_t magicKey);

/***************************************************************************//**
 * @brief This function checks the status of the watchdog timer and returns a
 * boolean indicating whether it is enabled. It is useful for
 * applications that need to verify the watchdog's state to ensure system
 * reliability and prevent unexpected resets. This function should be
 * called when the watchdog functionality is available, as indicated by
 * the presence of the SL_CATALOG_LEGACY_HAL_WDOG_PRESENT definition. If
 * the watchdog is not available, the function will always return false.
 *
 * @return A boolean value: true if the watchdog timer is enabled, false
 * otherwise.
 ******************************************************************************/
bool halInternalWatchDogEnabled(void);

/***************************************************************************//**
 * @brief This function is used to reset the watchdog timer, which is a
 * mechanism to ensure that the system is operating correctly. It should
 * be called periodically to prevent the watchdog from expiring and
 * causing a system reset. This function is only effective if the
 * watchdog timer is enabled and the configuration allows it. It is
 * typically used in systems where continuous operation is critical and
 * the watchdog is used as a fail-safe mechanism to reset the system in
 * case of software malfunctions.
 *
 * @return None
 ******************************************************************************/
void halResetWatchdog(void);                       ///< hal Reset Watchdog

#else // SL_CATALOG_LEGACY_HAL_WDOG_PRESENT
#define halInternalEnableWatchDog()
#define halInternalDisableWatchDog(...)
#define halInternalWatchDogEnabled(...) false
#define halResetWatchdog()
#endif // SL_CATALOG_LEGACY_HAL_WDOG_PRESENT
/**
 * @brief Change the CTUNE value. Involves switching to HFRCO and turning off
 * the HFXO temporarily.
 *
 * @param tune   Value to set CTUNE to.
 *
 */
void halInternalSetCtune(uint16_t tune);

/**
 * @brief Get the CTUNE value.
 *
 * @return Current CTUNE value.
 *
 */
uint16_t halInternalGetCtune(void);

/** @} (end addtogroup micro) */
/** @} (end addtogroup legacyhal) */

typedef uint8_t SleepModes;

#endif //__MICRO_COMMON_H__
