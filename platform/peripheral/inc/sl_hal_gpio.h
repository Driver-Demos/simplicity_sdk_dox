/***************************************************************************//**
 * @file
 * @brief General Purpose IO (GPIO) peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_HAL_GPIO_H
#define SL_HAL_GPIO_H

#include "em_device.h"

#if defined(GPIO_PRESENT)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include "sl_assert.h"
#include "sl_device_gpio.h"
#include "sl_code_classification.h"

/* *INDENT-OFF* */
// *****************************************************************************
/// @addtogroup gpio GPIO - General Purpose Input Output
/// @brief General Purpose Input Output peripheral
///
/// @li @ref gpio_intro
///
///@n @section gpio_intro Introduction
///  This module contains functions to control the GPIO peripheral of Silicon Labs 32-bit MCUs and SoCs.
///  The GPIO peripheral is used for interrupt configuration, pin configuration and direct pin manipulation
///  as well as routing for peripheral pin connections.
///
/// @{
// *****************************************************************************
/* *INDENT-ON* */

/*******************************************************************************
 ********************************   DEFINES   **********************************
 ******************************************************************************/

/// Define for port specific pin mask
#if defined(GPIO_PA_MASK)
#define SL_HAL_GPIO_PORT_A_PIN_MASK (GPIO_PA_MASK)
#else
#define SL_HAL_GPIO_PORT_A_PIN_MASK 0
#endif
#if defined(GPIO_PB_MASK)
#define SL_HAL_GPIO_PORT_B_PIN_MASK (GPIO_PB_MASK)
#else
#define SL_HAL_GPIO_PORT_B_PIN_MASK 0
#endif
#if defined(GPIO_PC_MASK)
#define SL_HAL_GPIO_PORT_C_PIN_MASK (GPIO_PC_MASK)
#else
#define SL_HAL_GPIO_PORT_C_PIN_MASK 0
#endif
#if defined(GPIO_PD_MASK)
#define SL_HAL_GPIO_PORT_D_PIN_MASK (GPIO_PD_MASK)
#else
#define SL_HAL_GPIO_PORT_D_PIN_MASK 0
#endif
#if defined(GPIO_PE_MASK)
#define SL_HAL_GPIO_PORT_E_PIN_MASK (GPIO_PE_MASK)
#else
#define SL_HAL_GPIO_PORT_E_PIN_MASK 0
#endif
#if defined(GPIO_PF_MASK)
#define SL_HAL_GPIO_PORT_F_PIN_MASK (GPIO_PF_MASK)
#else
#define SL_HAL_GPIO_PORT_F_PIN_MASK 0
#endif
#if defined(GPIO_PG_MASK)
#define SL_HAL_GPIO_PORT_G_PIN_MASK (GPIO_PG_MASK)
#else
#define SL_HAL_GPIO_PORT_G_PIN_MASK 0
#endif
#if defined(GPIO_PH_MASK)
#define SL_HAL_GPIO_PORT_H_PIN_MASK (GPIO_PH_MASK)
#else
#define SL_HAL_GPIO_PORT_H_PIN_MASK 0
#endif
#if defined(GPIO_PI_MASK)
#define SL_HAL_GPIO_PORT_I_PIN_MASK (GPIO_PI_MASK)
#else
#define SL_HAL_GPIO_PORT_I_PIN_MASK 0
#endif
#if defined(GPIO_PJ_MASK)
#define SL_HAL_GPIO_PORT_J_PIN_MASK (GPIO_PJ_MASK)
#else
#define SL_HAL_GPIO_PORT_J_PIN_MASK 0
#endif
#if defined(GPIO_PK_MASK)
#define SL_HAL_GPIO_PORT_K_PIN_MASK (GPIO_PK_MASK)
#else
#define SL_HAL_GPIO_PORT_K_PIN_MASK 0
#endif

/// Define for port specific pin count
#if defined(GPIO_PA_COUNT)
#define SL_HAL_GPIO_PORT_A_PIN_COUNT (GPIO_PA_COUNT)
#else
#define SL_HAL_GPIO_PORT_A_PIN_COUNT 0
#endif
#if defined(GPIO_PB_COUNT)
#define SL_HAL_GPIO_PORT_B_PIN_COUNT (GPIO_PB_COUNT)
#else
#define SL_HAL_GPIO_PORT_B_PIN_COUNT 0
#endif
#if defined(GPIO_PC_COUNT)
#define SL_HAL_GPIO_PORT_C_PIN_COUNT (GPIO_PC_COUNT)
#else
#define SL_HAL_GPIO_PORT_C_PIN_COUNT 0
#endif
#if defined(GPIO_PD_COUNT)
#define SL_HAL_GPIO_PORT_D_PIN_COUNT (GPIO_PD_COUNT)
#else
#define SL_HAL_GPIO_PORT_D_PIN_COUNT 0
#endif
#if defined(GPIO_PE_COUNT)
#define SL_HAL_GPIO_PORT_E_PIN_COUNT (GPIO_PE_COUNT)
#else
#define SL_HAL_GPIO_PORT_E_PIN_COUNT 0
#endif
#if defined(GPIO_PF_COUNT)
#define SL_HAL_GPIO_PORT_F_PIN_COUNT (GPIO_PF_COUNT)
#else
#define SL_HAL_GPIO_PORT_F_PIN_COUNT 0
#endif
#if defined(GPIO_PG_COUNT)
#define SL_HAL_GPIO_PORT_G_PIN_COUNT (GPIO_PG_COUNT)
#else
#define SL_HAL_GPIO_PORT_G_PIN_COUNT 0
#endif
#if defined(GPIO_PH_COUNT)
#define SL_HAL_GPIO_PORT_H_PIN_COUNT (GPIO_PH_COUNT)
#else
#define SL_HAL_GPIO_PORT_H_PIN_COUNT 0
#endif
#if defined(GPIO_PI_COUNT)
#define SL_HAL_GPIO_PORT_I_PIN_COUNT (GPIO_PI_COUNT)
#else
#define SL_HAL_GPIO_PORT_I_PIN_COUNT 0
#endif
#if defined(GPIO_PJ_COUNT)
#define SL_HAL_GPIO_PORT_J_PIN_COUNT (GPIO_PJ_COUNT)
#else
#define SL_HAL_GPIO_PORT_J_PIN_COUNT 0
#endif
#if defined(GPIO_PK_COUNT)
#define SL_HAL_GPIO_PORT_K_PIN_COUNT (GPIO_PK_COUNT)
#else
#define SL_HAL_GPIO_PORT_K_PIN_COUNT 0
#endif

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

/// Highest GPIO port number.

#if (SL_HAL_GPIO_PORT_K_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  10
#elif (SL_HAL_GPIO_PORT_J_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  9
#elif (SL_HAL_GPIO_PORT_I_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  8
#elif (SL_HAL_GPIO_PORT_H_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  7
#elif (SL_HAL_GPIO_PORT_G_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  6
#elif (SL_HAL_GPIO_PORT_F_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  5
#elif (SL_HAL_GPIO_PORT_E_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  4
#elif (SL_HAL_GPIO_PORT_D_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  3
#elif (SL_HAL_GPIO_PORT_C_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  2
#elif (SL_HAL_GPIO_PORT_B_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  1
#elif (SL_HAL_GPIO_PORT_A_PIN_COUNT > 0)
#define SL_HAL_GPIO_PORT_MAX  0
#else
#error "Max GPIO port number is undefined for this part."
#endif

/// Highest GPIO pin number.
#define SL_HAL_GPIO_PIN_MAX 15

/// @endcond

#define SL_HAL_GPIO_PORT_SIZE(port) (             \
    (port) == 0  ? SL_HAL_GPIO_PORT_A_PIN_COUNT   \
    : (port) == 1  ? SL_HAL_GPIO_PORT_B_PIN_COUNT \
    : (port) == 2  ? SL_HAL_GPIO_PORT_C_PIN_COUNT \
    : (port) == 3  ? SL_HAL_GPIO_PORT_D_PIN_COUNT \
    : (port) == 4  ? SL_HAL_GPIO_PORT_E_PIN_COUNT \
    : (port) == 5  ? SL_HAL_GPIO_PORT_F_PIN_COUNT \
    : (port) == 6  ? SL_HAL_GPIO_PORT_G_PIN_COUNT \
    : (port) == 7  ? SL_HAL_GPIO_PORT_H_PIN_COUNT \
    : (port) == 8  ? SL_HAL_GPIO_PORT_I_PIN_COUNT \
    : (port) == 9  ? SL_HAL_GPIO_PORT_J_PIN_COUNT \
    : (port) == 10 ? SL_HAL_GPIO_PORT_K_PIN_COUNT \
    : 0)

#define SL_HAL_GPIO_PORT_MASK(port) (                 \
    ((int)port) == 0  ? SL_HAL_GPIO_PORT_A_PIN_MASK   \
    : ((int)port) == 1  ? SL_HAL_GPIO_PORT_B_PIN_MASK \
    : ((int)port) == 2  ? SL_HAL_GPIO_PORT_C_PIN_MASK \
    : ((int)port) == 3  ? SL_HAL_GPIO_PORT_D_PIN_MASK \
    : ((int)port) == 4  ? SL_HAL_GPIO_PORT_E_PIN_MASK \
    : ((int)port) == 5  ? SL_HAL_GPIO_PORT_F_PIN_MASK \
    : ((int)port) == 6  ? SL_HAL_GPIO_PORT_G_PIN_MASK \
    : ((int)port) == 7  ? SL_HAL_GPIO_PORT_H_PIN_MASK \
    : ((int)port) == 8  ? SL_HAL_GPIO_PORT_I_PIN_MASK \
    : ((int)port) == 9  ? SL_HAL_GPIO_PORT_J_PIN_MASK \
    : ((int)port) == 10 ? SL_HAL_GPIO_PORT_K_PIN_MASK \
    : 0UL)

/// Validation of port.
#define SL_HAL_GPIO_PORT_IS_VALID(port)          (SL_HAL_GPIO_PORT_MASK(port) != 0x0UL)

/// Validation of port and pin.
#define SL_HAL_GPIO_PORT_PIN_IS_VALID(port, pin) ((((SL_HAL_GPIO_PORT_MASK(port)) >> (pin)) & 0x1UL) == 0x1UL)

/// Max interrupt lines for external and EM4 interrupts.
#define SL_HAL_GPIO_INTERRUPT_MAX 15

/// Shift value for EM4WUEN
#define SL_HAL_GPIO_EM4WUEN_SHIFT _GPIO_EM4WUEN_EM4WUEN_SHIFT

/// Masks for even and odd interrupt bits.
#define SL_HAL_GPIO_INT_IF_EVEN_MASK ((_GPIO_IF_MASK) & 0x55555555UL)
#define SL_HAL_GPIO_INT_IF_ODD_MASK  ((_GPIO_IF_MASK) & 0xAAAAAAAAUL)

/// Validation of mode.
#define SL_HAL_GPIO_MODE_IS_VALID(mode)  ((mode & _GPIO_P_MODEL_MODE0_MASK) == mode)

/// Validation of interrupt number and pin.
#define SL_HAL_GPIO_INTNO_PIN_VALID(int_no, pin)    (((int_no) & ~_GPIO_EXTIPINSELL_EXTIPINSEL0_MASK) == ((pin) & ~_GPIO_EXTIPINSELL_EXTIPINSEL0_MASK))

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function configures the mode of a specified GPIO pin, allowing
 * for various input and output configurations. It should be used when
 * you need to change the operational mode of a GPIO pin, such as
 * switching between input and output modes. The function requires that
 * the GPIO pin is valid and that the GPIO configuration is unlocked. It
 * also allows setting an initial output value, which is important for
 * certain input modes that depend on the DOUT register setting. Ensure
 * that the GPIO pin is not locked before calling this function.
 *
 * @param gpio Pointer to a sl_gpio_t structure containing the port and pin
 * number. Must not be null and must point to a valid GPIO pin.
 * @param mode The desired mode for the GPIO pin, specified as a sl_gpio_mode_t
 * value. Must be a valid mode supported by the hardware.
 * @param output_value Boolean value indicating the initial output state of the
 * pin. True sets the pin high, and false sets it low. This
 * is relevant for modes that depend on the DOUT register.
 * @return None
 ******************************************************************************/
void sl_hal_gpio_set_pin_mode(const sl_gpio_t *gpio,
                              sl_gpio_mode_t mode,
                              bool output_value);

/***************************************************************************//**
 * @brief This function is used to obtain the current configuration mode of a
 * specified GPIO pin, which is part of a general-purpose input/output
 * (GPIO) peripheral. It is essential to ensure that the GPIO structure
 * provided is valid, as the function will assert if the port and pin are
 * not valid. This function is typically called when there is a need to
 * verify or log the current mode of a GPIO pin, especially in debugging
 * or configuration validation scenarios.
 *
 * @param gpio A pointer to an sl_gpio_t structure containing the port and pin
 * information. The pointer must not be null, and the port and pin
 * must be valid as per the device's GPIO configuration. Invalid
 * input will trigger an assertion failure.
 * @return Returns the current mode of the specified GPIO pin as an
 * sl_gpio_mode_t enumeration value.
 ******************************************************************************/
sl_gpio_mode_t sl_hal_gpio_get_pin_mode(const sl_gpio_t *gpio);

/***************************************************************************//**
 * @brief This function sets up a GPIO pin to trigger an external interrupt
 * based on the specified interrupt number and configuration flags. It
 * should be used when you need to associate a GPIO pin with an external
 * interrupt line, allowing the pin to generate interrupts on specified
 * edge conditions. The function must be called with a valid GPIO
 * structure, and it is recommended to disable interrupts before calling
 * this function to avoid unintended behavior. The function clears any
 * pending interrupts for the selected line and configures the interrupt
 * without enabling it. Ensure that the GPIO lock status is unlocked
 * before calling this function.
 *
 * @param gpio Pointer to a sl_gpio_t structure representing the GPIO port and
 * pin. Must not be null and must point to a valid GPIO
 * configuration.
 * @param int_no The interrupt number to associate with the GPIO pin. Must be a
 * non-negative integer or SL_GPIO_INTERRUPT_UNAVAILABLE if an
 * automatic assignment is desired.
 * @param flags Configuration flags for the interrupt, specifying edge
 * conditions. Must be a valid sl_gpio_interrupt_flag_t value.
 * @return Returns the interrupt number assigned to the GPIO pin, or
 * SL_GPIO_INTERRUPT_UNAVAILABLE if no valid interrupt number could be
 * assigned.
 ******************************************************************************/
int32_t sl_hal_gpio_configure_external_interrupt(const sl_gpio_t *gpio,
                                                 int32_t int_no,
                                                 sl_gpio_interrupt_flag_t flags);

/***************************************************************************//**
 * @brief This function configures specified GPIO pins to wake up the system
 * from EM4 mode based on a given polarity. It should be used when you
 * need certain GPIO pins to trigger a wake-up event from EM4. The
 * function assumes that the GPIO pin modes are already set correctly to
 * either input or input with pull. It also enables pin retention and
 * clears any pending interrupts for the specified pins. Ensure that the
 * pinmask and polaritymask are within valid ranges before calling this
 * function.
 *
 * @param pinmask A bitmask specifying which GPIO pins to enable for EM4 wake-
 * up. Each bit corresponds to a pin, and only pins within the
 * valid range of the _GPIO_EM4WUEN_MASK can be set. Invalid bits
 * will cause an assertion failure.
 * @param polaritymask A bitmask specifying the polarity for each GPIO pin's
 * wake-up trigger. Each bit corresponds to a pin, and only
 * pins within the valid range of the _GPIO_EM4WUPOL_MASK
 * can be set. Invalid bits will cause an assertion failure.
 * @return None
 ******************************************************************************/
void sl_hal_gpio_enable_pin_em4_wakeup(uint32_t pinmask,
                                       uint32_t polaritymask);

/***************************************************************************//**
 * @brief This function sets up a specified GPIO pin to trigger an external
 * interrupt that can wake the system from EM4 (Energy Mode 4). It should
 * be used when you need a GPIO pin to act as a wake-up source from EM4,
 * with a specific interrupt number and polarity. The function requires
 * that the GPIO pin is valid and that the GPIO configuration is
 * unlocked. If the provided interrupt number is unavailable, the
 * function will attempt to use the default EM4 interrupt number for the
 * pin. If neither is available, it returns an unavailable status. Ensure
 * that the GPIO pin mode is set correctly before calling this function.
 *
 * @param gpio Pointer to a sl_gpio_t structure representing the GPIO port and
 * pin. Must not be null and must point to a valid GPIO
 * configuration.
 * @param int_no The interrupt number to be used for the wake-up. If set to
 * SL_GPIO_INTERRUPT_UNAVAILABLE, the function will use the
 * default EM4 interrupt number for the pin. Must be a valid
 * interrupt number or SL_GPIO_INTERRUPT_UNAVAILABLE.
 * @param polarity Boolean value indicating the desired interrupt polarity. True
 * for active high, false for active low.
 * @return Returns the interrupt number used for the configuration, or
 * SL_GPIO_INTERRUPT_UNAVAILABLE if the configuration could not be
 * completed.
 ******************************************************************************/
int32_t sl_hal_gpio_configure_wakeup_em4_external_interrupt(const sl_gpio_t *gpio,
                                                            int32_t int_no,
                                                            bool polarity);

/***************************************************************************//**
 * @brief The `sl_hal_gpio_lock` function locks the GPIO configuration by
 * writing a value to the GPIO lock register.
 *
 * @return The function does not return any value.
 ******************************************************************************/
__INLINE void sl_hal_gpio_lock(void)
{
  GPIO->LOCK = ~GPIO_LOCK_LOCKKEY_UNLOCK;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_unlock` function unlocks the GPIO configuration by
 * writing a specific unlock key to the GPIO lock register.
 *
 * @return The function does not return any value; it performs an action to
 * unlock the GPIO configuration.
 ******************************************************************************/
__INLINE void sl_hal_gpio_unlock(void)
{
  GPIO->LOCK = GPIO_LOCK_LOCKKEY_UNLOCK;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_get_lock_status` function retrieves the current lock
 * status of the GPIO configuration.
 *
 * @return The function returns a `uint32_t` value representing the lock status
 * of the GPIO configuration.
 ******************************************************************************/
__INLINE uint32_t sl_hal_gpio_get_lock_status(void)
{
  return GPIO->GPIOLOCKSTATUS;
}

/***************************************************************************//**
 * Set a single pin in GPIO data out register to 1.
 *
 * @param[in] gpio Pointer to GPIO structure with port and pin
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_GPIO, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The `sl_hal_gpio_set_pin` function sets a specific GPIO pin to a high
 * state (logic 1) in the data out register.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure containing the port and pin
 * information for the GPIO pin to be set.
 * @return The function does not return a value; it performs an action by
 * setting the specified GPIO pin to high.
 ******************************************************************************/
__INLINE void sl_hal_gpio_set_pin(const sl_gpio_t *gpio)
{
  EFM_ASSERT(gpio != NULL);
  EFM_ASSERT(SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin));

  GPIO->P_SET[gpio->port].DOUT = 1UL << gpio->pin;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_set_port` function sets the data output register for
 * a specified GPIO port to a given value.
 *
 * @param port The GPIO port to access, specified as an `sl_gpio_port_t` type.
 * @param pins A 32-bit integer representing the bitmask of pins to set in the
 * data output register.
 * @return The function does not return a value; it directly modifies the
 * hardware register for the specified GPIO port.
 ******************************************************************************/
__INLINE void sl_hal_gpio_set_port(sl_gpio_port_t port,
                                   uint32_t pins)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));
  GPIO->P_SET[port].DOUT = pins;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_set_port_value` function sets specific bits of a GPIO
 * port's data output register based on a given value and mask.
 *
 * @param port The GPIO port to access, specified as an `sl_gpio_port_t` type.
 * @param val The value to write to the port's data output register, specified
 * as a `uint32_t`.
 * @param mask A mask indicating which bits of the port's data output register
 * should be modified, specified as a `uint32_t`.
 * @return The function does not return a value; it directly modifies the data
 * output register of the specified GPIO port.
 ******************************************************************************/
__INLINE void sl_hal_gpio_set_port_value(sl_gpio_port_t port,
                                         uint32_t val,
                                         uint32_t mask)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));
  GPIO->P[port].DOUT = (GPIO->P[port].DOUT & ~mask) | (val & mask);
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_set_slew_rate` function configures the slew rate for
 * pins on a specified GPIO port that are set to normal modes.
 *
 * @param port The GPIO port to configure, represented by the type
 * `sl_gpio_port_t`.
 * @param slewrate The desired slew rate value to set for the pins on the
 * specified GPIO port, represented as an 8-bit unsigned
 * integer.
 * @return The function does not return a value; it directly modifies the
 * hardware register to set the slew rate for the specified GPIO port.
 ******************************************************************************/
__INLINE void sl_hal_gpio_set_slew_rate(sl_gpio_port_t port,
                                        uint8_t slewrate)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));
  EFM_ASSERT(slewrate <= (_GPIO_P_CTRL_SLEWRATE_MASK
                          >> _GPIO_P_CTRL_SLEWRATE_SHIFT));

  GPIO->P[port].CTRL = (GPIO->P[port].CTRL
                        & ~_GPIO_P_CTRL_SLEWRATE_MASK)
                       | (slewrate << _GPIO_P_CTRL_SLEWRATE_SHIFT);
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_set_slew_rate_alternate` sets the slew rate
 * for GPIO pins configured in alternate modes on a specified port.
 *
 * @param port The GPIO port to configure, represented by the type
 * `sl_gpio_port_t`.
 * @param slewrate_alt The alternate slew rate value to set, represented as an
 * 8-bit unsigned integer.
 * @return The function does not return a value; it directly modifies the
 * hardware register to set the alternate slew rate for the specified
 * GPIO port.
 ******************************************************************************/
__INLINE void sl_hal_gpio_set_slew_rate_alternate(sl_gpio_port_t port,
                                                  uint8_t slewrate_alt)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));
  EFM_ASSERT(slewrate_alt <= (_GPIO_P_CTRL_SLEWRATEALT_MASK
                              >> _GPIO_P_CTRL_SLEWRATEALT_SHIFT));

  GPIO->P[port].CTRL = (GPIO->P[port].CTRL
                        & ~_GPIO_P_CTRL_SLEWRATEALT_MASK)
                       | (slewrate_alt << _GPIO_P_CTRL_SLEWRATEALT_SHIFT);
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_get_slew_rate` function retrieves the slew rate
 * setting for a specified GPIO port.
 *
 * @param port A GPIO port identifier of type `sl_gpio_port_t` for which the
 * slew rate is to be retrieved.
 * @return The function returns an 8-bit unsigned integer representing the slew
 * rate setting for the specified GPIO port.
 ******************************************************************************/
__INLINE uint8_t sl_hal_gpio_get_slew_rate(sl_gpio_port_t port)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));

  return (GPIO->P[port].CTRL & _GPIO_P_CTRL_SLEWRATE_MASK) >> _GPIO_P_CTRL_SLEWRATE_SHIFT;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_slew_rate_alternate` retrieves the
 * alternate slew rate setting for a specified GPIO port.
 *
 * @param port A GPIO port identifier of type `sl_gpio_port_t` for which the
 * alternate slew rate is to be retrieved.
 * @return The function returns an 8-bit unsigned integer representing the
 * alternate slew rate setting for the specified GPIO port.
 ******************************************************************************/
__INLINE uint8_t sl_hal_gpio_get_slew_rate_alternate(sl_gpio_port_t port)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));

  return (GPIO->P[port].CTRL & _GPIO_P_CTRL_SLEWRATEALT_MASK) >> _GPIO_P_CTRL_SLEWRATEALT_SHIFT;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_clear_pin` function clears a specific GPIO pin by
 * setting its corresponding bit in the data out register to 0.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure containing the port and pin
 * information for the GPIO to be cleared.
 * @return This function does not return a value; it performs an action on the
 * hardware to clear a GPIO pin.
 ******************************************************************************/
__INLINE void sl_hal_gpio_clear_pin(const sl_gpio_t *gpio)
{
  EFM_ASSERT(gpio != NULL);
  EFM_ASSERT(SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin));

  GPIO->P_CLR[gpio->port].DOUT = 1UL << gpio->pin;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_clear_port` function clears specified bits in the
 * data output register of a given GPIO port.
 *
 * @param port A `sl_gpio_port_t` type representing the GPIO port to be
 * accessed.
 * @param pins A `uint32_t` bitmask indicating which bits in the port's data
 * output register should be cleared.
 * @return The function does not return a value; it performs an inline operation
 * to clear specified bits in the GPIO port's data output register.
 ******************************************************************************/
__INLINE void sl_hal_gpio_clear_port(sl_gpio_port_t port,
                                     uint32_t pins)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));

  GPIO->P_CLR[port].DOUT = pins;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_get_pin_input` function reads and returns the input
 * value of a specified GPIO pin.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure containing the port and pin
 * information for the GPIO pin to be read.
 * @return The function returns a boolean value representing the input state of
 * the specified GPIO pin, where `true` indicates a high input and
 * `false` indicates a low input.
 ******************************************************************************/
__INLINE bool sl_hal_gpio_get_pin_input(const sl_gpio_t *gpio)
{
  EFM_ASSERT(gpio != NULL);
  EFM_ASSERT(SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin));

  bool pin_input = ((GPIO->P[gpio->port].DIN) >> gpio->pin) & 1UL;

  return pin_input;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_get_pin_output` function retrieves the current output
 * state of a specified GPIO pin.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure containing the port and pin
 * information for the GPIO pin.
 * @return The function returns a boolean value indicating the current output
 * state of the specified GPIO pin, where `true` represents a high
 * output and `false` represents a low output.
 ******************************************************************************/
__INLINE bool sl_hal_gpio_get_pin_output(const sl_gpio_t *gpio)
{
  EFM_ASSERT(gpio != NULL);
  EFM_ASSERT(SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin));

  bool pin_output = ((GPIO->P[gpio->port].DOUT) >> gpio->pin) & 1UL;

  return pin_output;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_get_port_input` function retrieves the input data
 * register value for a specified GPIO port.
 *
 * @param port A GPIO port identifier of type `sl_gpio_port_t`.
 * @return The function returns a `uint32_t` value representing the input data
 * register value for the specified GPIO port.
 ******************************************************************************/
__INLINE uint32_t sl_hal_gpio_get_port_input(sl_gpio_port_t port)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));

  return GPIO->P[port].DIN;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_port_output` retrieves the current
 * output data register value for a specified GPIO port.
 *
 * @param port A GPIO port identifier of type `sl_gpio_port_t`.
 * @return The function returns a `uint32_t` value representing the data out
 * (DOUT) register of the specified GPIO port, indicating the current
 * output state of the port's pins.
 ******************************************************************************/
__INLINE uint32_t sl_hal_gpio_get_port_output(sl_gpio_port_t port)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));

  return GPIO->P[port].DOUT;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_toggle_pin` function toggles the state of a specified
 * GPIO pin.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure containing the port and pin
 * information for the GPIO pin to be toggled.
 * @return The function does not return a value; it performs an in-place toggle
 * of the specified GPIO pin's state.
 ******************************************************************************/
__INLINE void sl_hal_gpio_toggle_pin(const sl_gpio_t *gpio)
{
  EFM_ASSERT(gpio != NULL);
  EFM_ASSERT(SL_HAL_GPIO_PORT_PIN_IS_VALID(gpio->port, gpio->pin));

  GPIO->P_TGL[gpio->port].DOUT = 1UL << gpio->pin;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_toggle_port` function toggles the state of specified
 * pins on a given GPIO port.
 *
 * @param port The GPIO port to access, represented by the type
 * `sl_gpio_port_t`.
 * @param pins A bitmask indicating which pins on the specified port should be
 * toggled.
 * @return This function does not return a value; it performs an in-place toggle
 * operation on the specified GPIO port pins.
 ******************************************************************************/
__INLINE void sl_hal_gpio_toggle_port(sl_gpio_port_t port,
                                      uint32_t pins)
{
  EFM_ASSERT(SL_HAL_GPIO_PORT_IS_VALID(port));

  GPIO->P_TGL[port].DOUT = pins;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_enable_interrupts` enables specified GPIO
 * interrupts by setting the corresponding bits in the GPIO interrupt
 * enable register.
 *
 * @param flags A 32-bit unsigned integer representing the bitmask of GPIO
 * interrupt sources to enable.
 * @return The function does not return any value; it performs a hardware
 * register write operation to enable interrupts.
 ******************************************************************************/
__INLINE void sl_hal_gpio_enable_interrupts(uint32_t flags)
{
  GPIO->IEN_SET = flags;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_disable_interrupts` function disables specified GPIO
 * interrupts by clearing the corresponding bits in the interrupt enable
 * register.
 *
 * @param flags A 32-bit unsigned integer representing the bitmask of GPIO
 * interrupt sources to disable.
 * @return The function does not return any value.
 ******************************************************************************/
__INLINE void sl_hal_gpio_disable_interrupts(uint32_t flags)
{
  GPIO->IEN_CLR = flags;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_clear_interrupts` function clears specified GPIO
 * interrupt flags.
 *
 * @param flags A 32-bit unsigned integer representing the bitwise logic OR of
 * GPIO interrupt sources to clear.
 * @return This function does not return any value.
 ******************************************************************************/
__INLINE void sl_hal_gpio_clear_interrupts(uint32_t flags)
{
  GPIO->IF_CLR = flags;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_set_interrupts` function sets one or more GPIO
 * interrupts to pending state using the specified flags.
 *
 * @param flags A 32-bit unsigned integer representing the GPIO interrupt
 * sources to set to pending.
 * @return This function does not return any value.
 ******************************************************************************/
__INLINE void sl_hal_gpio_set_interrupts(uint32_t flags)
{
  GPIO->IF_SET = flags;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_pending_interrupts` retrieves the
 * current pending GPIO interrupt flags.
 *
 * @return The function returns a `uint32_t` value representing the pending GPIO
 * interrupt flags.
 ******************************************************************************/
__INLINE uint32_t sl_hal_gpio_get_pending_interrupts(void)
{
  return GPIO->IF;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_enabled_interrupts` retrieves the
 * currently enabled GPIO interrupt sources.
 *
 * @return The function returns a `uint32_t` value representing the enabled GPIO
 * interrupt sources.
 ******************************************************************************/
__INLINE uint32_t sl_hal_gpio_get_enabled_interrupts(void)
{
  return GPIO->IEN;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_enabled_pending_interrupts` returns the
 * bitwise AND of the enabled and pending GPIO interrupt flags.
 *
 * @return The function returns a `uint32_t` value representing the enabled and
 * pending GPIO interrupt sources.
 ******************************************************************************/
__INLINE uint32_t sl_hal_gpio_get_enabled_pending_interrupts(void)
{
  uint32_t tmp;

  tmp = GPIO->IEN;

  return GPIO->IF & tmp;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_external_interrupt_number` determines
 * the first available external interrupt number for a given GPIO pin
 * based on an enabled interrupts mask.
 *
 * @param pin A uint8_t representing the GPIO pin number to check for available
 * interrupts.
 * @param enabled_interrupts_mask A uint32_t bitmask representing the currently
 * enabled GPIO interrupts.
 * @return The function returns an int32_t representing the first available
 * interrupt number for the specified pin, or -1 if no available
 * interrupt is found.
 ******************************************************************************/
__INLINE int32_t sl_hal_gpio_get_external_interrupt_number(uint8_t pin,
                                                           uint32_t enabled_interrupts_mask)
{
  uint32_t interrupt_to_check;
  uint32_t int_group_start = (pin & 0xFFC);
  int32_t int_no = -1;
  // loop through the interrupt group, starting
  // from the pin number, and take
  // the first available.
  for (uint8_t i = 0; i < 4; i++) {
    interrupt_to_check = int_group_start + ((pin + i) & 0x3);     // modulo 4
    if (((enabled_interrupts_mask >> interrupt_to_check) & 0x1) == 0) {
      int_no = interrupt_to_check;
      break;
    }
  }
  return int_no;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_em4_interrupt_number` determines the EM4
 * wake-up interrupt number for a given GPIO pin and port.
 *
 * @param gpio A pointer to an `sl_gpio_t` structure containing the port and pin
 * information for the GPIO.
 * @return The function returns an `int32_t` representing the EM4 wake-up
 * interrupt number for the specified GPIO, or -1 if no match is found.
 ******************************************************************************/
__INLINE int32_t sl_hal_gpio_get_em4_interrupt_number(const sl_gpio_t *gpio)
{
  EFM_ASSERT(gpio != NULL);
  int32_t em4_int_no;

  if (false) {
    // Check all the EM4WU Pins and check if given port, pin matches any of them.
    #if defined(GPIO_EM4WU0_PORT)
  } else if (GPIO_EM4WU0_PORT == gpio->port && GPIO_EM4WU0_PIN == gpio->pin) {
    em4_int_no = 0;
    #endif
    #if defined(GPIO_EM4WU1_PORT)
  } else if (GPIO_EM4WU1_PORT == gpio->port && GPIO_EM4WU1_PIN == gpio->pin) {
    em4_int_no = 1;
    #endif
    #if defined(GPIO_EM4WU3_PORT)
  } else if (GPIO_EM4WU3_PORT == gpio->port && GPIO_EM4WU3_PIN == gpio->pin) {
    em4_int_no = 3;
    #endif
    #if defined(GPIO_EM4WU4_PORT)
  } else if (GPIO_EM4WU4_PORT == gpio->port && GPIO_EM4WU4_PIN == gpio->pin) {
    em4_int_no = 4;
    #endif
    #if defined(GPIO_EM4WU6_PORT)
  } else if (GPIO_EM4WU6_PORT == gpio->port && GPIO_EM4WU6_PIN == gpio->pin) {
    em4_int_no = 6;
    #endif
    #if defined(GPIO_EM4WU7_PORT)
  } else if (GPIO_EM4WU7_PORT == gpio->port && GPIO_EM4WU7_PIN == gpio->pin) {
    em4_int_no = 7;
    #endif
    #if defined(GPIO_EM4WU8_PORT)
  } else if (GPIO_EM4WU8_PORT == gpio->port && GPIO_EM4WU8_PIN == gpio->pin) {
    em4_int_no = 8;
    #endif
    #if defined(GPIO_EM4WU9_PORT)
  } else if (GPIO_EM4WU9_PORT == gpio->port && GPIO_EM4WU9_PIN == gpio->pin) {
    em4_int_no = 9;
    #endif
    #if defined(GPIO_EM4WU10_PORT)
  } else if (GPIO_EM4WU10_PORT == gpio->port && GPIO_EM4WU10_PIN == gpio->pin) {
    em4_int_no = 10;
    #endif
  } else {
    em4_int_no = -1;
  }

  return em4_int_no;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_disable_pin_em4_wakeup` disables the wake-up
 * capability from EM4 mode for specified GPIO pins using a bitmask.
 *
 * @param pinmask A bitmask representing the GPIO pins to disable for EM4 wake-
 * up.
 * @return The function does not return any value; it performs an inline
 * operation to modify the GPIO configuration.
 ******************************************************************************/
__INLINE void sl_hal_gpio_disable_pin_em4_wakeup(uint32_t pinmask)
{
  EFM_ASSERT((pinmask & ~_GPIO_EM4WUEN_MASK) == 0UL);

  GPIO->EM4WUEN &= ~pinmask;
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_set_pin_em4_retention` configures the GPIO
 * pin retention mode during EM4 (Energy Mode 4) based on the provided
 * enable flag, unless the software unlatch mode is active.
 *
 * @param enable A boolean flag indicating whether to enable (true) or disable
 * (false) EM4 pin retention.
 * @return The function does not return a value; it modifies the EMU->EM4CTRL
 * register to set the EM4 I/O retention mode.
 ******************************************************************************/
__INLINE void sl_hal_gpio_set_pin_em4_retention(bool enable)
{
  // Leave configuration alone when software unlatch is used.
  uint32_t mode = EMU->EM4CTRL & _EMU_EM4CTRL_EM4IORETMODE_MASK;

  if (mode == EMU_EM4CTRL_EM4IORETMODE_SWUNLATCH) {
    return;
  }

  if (enable) {
    EMU->EM4CTRL = (EMU->EM4CTRL & ~_EMU_EM4CTRL_EM4IORETMODE_MASK)
                   | EMU_EM4CTRL_EM4IORETMODE_EM4EXIT;
  } else {
    EMU->EM4CTRL = (EMU->EM4CTRL & ~_EMU_EM4CTRL_EM4IORETMODE_MASK)
                   | EMU_EM4CTRL_EM4IORETMODE_DISABLE;
  }
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_get_pin_em4_wakeup_cause` retrieves the
 * bitmask of GPIO pins that triggered a wake-up from EM4 mode.
 *
 * @return The function returns a `uint32_t` bitmask indicating which GPIO pins
 * caused a wake-up from EM4 mode.
 ******************************************************************************/
__INLINE uint32_t sl_hal_gpio_get_pin_em4_wakeup_cause(void)
{
  return GPIO->IF & _GPIO_EM4WUEN_EM4WUEN_MASK;
}

/***************************************************************************//**
 * @brief The `sl_hal_gpio_enable_debug_swo` function enables or disables the
 * Serial Wire Output (SWO) pin for debugging purposes based on the input
 * parameter.
 *
 * @param enable A boolean value indicating whether to enable (true) or disable
 * (false) the SWO pin.
 * @return The function does not return a value; it modifies the state of the
 * SWO pin in the GPIO hardware registers.
 ******************************************************************************/
__INLINE void sl_hal_gpio_enable_debug_swo(bool enable)
{
  uint32_t bit = enable ? 0x1UL : 0x0UL;

  if (bit != 0U) {
    GPIO->TRACEROUTEPEN_SET = 1UL << _GPIO_TRACEROUTEPEN_SWVPEN_SHIFT;
  } else {
    GPIO->TRACEROUTEPEN_CLR = 1UL << _GPIO_TRACEROUTEPEN_SWVPEN_SHIFT;
  }
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_enable_debug_swd_clk` enables or disables
 * the serial wire debug clock pin based on the input parameter.
 *
 * @param enable A boolean value indicating whether to enable (true) or disable
 * (false) the serial wire debug clock pin.
 * @return The function does not return a value; it modifies the GPIO
 * configuration to enable or disable the SWD clock pin.
 ******************************************************************************/
__INLINE void sl_hal_gpio_enable_debug_swd_clk(bool enable)
{
  uint32_t bit = enable ? 0x1UL : 0x0UL;

  if (bit != 0U) {
    GPIO->DBGROUTEPEN_SET = 1UL << _GPIO_DBGROUTEPEN_SWCLKTCKPEN_SHIFT;
  } else {
    GPIO->DBGROUTEPEN_CLR = 1UL << _GPIO_DBGROUTEPEN_SWCLKTCKPEN_SHIFT;
  }
}

/***************************************************************************//**
 * @brief The function `sl_hal_gpio_enable_debug_swd_io` enables or disables the
 * serial wire data I/O pin for debugging purposes.
 *
 * @param enable A boolean value indicating whether to enable (true) or disable
 * (false) the serial wire data I/O pin.
 * @return The function does not return a value; it modifies the GPIO
 * configuration to enable or disable the SWD I/O pin.
 ******************************************************************************/
__INLINE void sl_hal_gpio_enable_debug_swd_io(bool enable)
{
  uint32_t bit = enable ? 0x1UL : 0x0UL;

  if (bit != 0U) {
    GPIO->DBGROUTEPEN_SET = 1UL << _GPIO_DBGROUTEPEN_SWDIOTMSPEN_SHIFT;
  } else {
    GPIO->DBGROUTEPEN_CLR = 1UL << _GPIO_DBGROUTEPEN_SWDIOTMSPEN_SHIFT;
  }
}

/** @} (end addtogroup gpio) */

#ifdef __cplusplus
}
#endif

#endif /* GPIO_PRESENT */
#endif /* SL_HAL_GPIO_H */
