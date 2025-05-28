/***************************************************************************//**
 * @file
 * @brief Keyscan (KEYSCAN) peripheral API
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

#ifndef SL_HAL_KEYSCAN_H
#define SL_HAL_KEYSCAN_H

#include "em_device.h"

#if defined(KEYSCAN_COUNT) && (KEYSCAN_COUNT > 0)

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "sl_hal_keyscan_compat.h"
#include "sl_enum.h"

/***************************************************************************//**
 * @addtogroup keyscan
 * @{
 ******************************************************************************/

/*******************************************************************************
 *********************************   ENUM   ************************************
 ******************************************************************************/

/// KEYSCAN configuration delay values
SL_ENUM(sl_hal_keyscan_delay_t) {
  SL_HAL_KEYSCAN_DELAY_2MS = 0, ///< 2 ms delay.
  SL_HAL_KEYSCAN_DELAY_4MS,   ///< 4 ms delay.
  SL_HAL_KEYSCAN_DELAY_6MS,   ///< 6 ms delay.
  SL_HAL_KEYSCAN_DELAY_8MS,   ///< 8 ms delay.
  SL_HAL_KEYSCAN_DELAY_10MS,  ///< 10 ms delay.
  SL_HAL_KEYSCAN_DELAY_12MS,  ///< 12 ms delay.
  SL_HAL_KEYSCAN_DELAY_14MS,  ///< 14 ms delay.
  SL_HAL_KEYSCAN_DELAY_16MS,  ///< 16 ms delay.
  SL_HAL_KEYSCAN_DELAY_18MS,  ///< 18 ms delay.
  SL_HAL_KEYSCAN_DELAY_20MS,  ///< 20 ms delay.
  SL_HAL_KEYSCAN_DELAY_22MS,  ///< 22 ms delay.
  SL_HAL_KEYSCAN_DELAY_24MS,  ///< 24 ms delay.
  SL_HAL_KEYSCAN_DELAY_26MS,  ///< 26 ms delay.
  SL_HAL_KEYSCAN_DELAY_28MS,  ///< 28 ms delay.
  SL_HAL_KEYSCAN_DELAY_30MS,  ///< 30 ms delay.
  SL_HAL_KEYSCAN_DELAY_32MS   ///< 32 ms delay.
};

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/// KEYSCAN configuration structure.
/***************************************************************************//**
 * @brief The `sl_hal_keyscan_config_t` structure is used to configure the
 * keyscan peripheral in a hardware abstraction layer. It includes
 * parameters such as the clock divider, the number of columns and rows
 * for the keyscan matrix, and various delay settings for scanning,
 * debouncing, and stabilizing. Additionally, it provides options to
 * enable single press detection and automatic start of the keyscan
 * process. This structure allows for flexible configuration of the
 * keyscan hardware to suit different application requirements.
 *
 * @param clock_divider Clock divider value.
 * @param column_number Number of columns to set for keyscan (maximum 8).
 * @param row_number Number of rows to set for keyscan (maximum 6).
 * @param scan_delay Scan delay.
 * @param debounce_delay Debounce delay.
 * @param stable_delay Stable delay.
 * @param single_press_enable Enable Single Press feature.
 * @param auto_start_enable Enable auto-start feature.
 ******************************************************************************/
typedef struct {
  uint32_t            clock_divider;       ///< Clock divider value.
  uint8_t             column_number;       ///< Number of columns to set for keyscan (maximum 8).
  uint8_t             row_number;          ///< Number of rows  to set for keyscan (maximum 6).
  sl_hal_keyscan_delay_t  scan_delay;      ///< Scan delay.
  sl_hal_keyscan_delay_t  debounce_delay;  ///< Debounce delay.
  sl_hal_keyscan_delay_t  stable_delay;    ///< Stable delay.
  bool                single_press_enable; ///< Enable Single Press feature.
  bool                auto_start_enable;   ///< Enable auto-start feature.
} sl_hal_keyscan_config_t;

/// Suggested default values for KEYSCAN configuration structure.
#define KEYSCAN_CONFIG_DEFAULT                                           \
  {                                                                      \
    0x1387F,                  /* Clock divider default value = 79999. */ \
    3u,                       /* 3 columns by default. */                \
    6u,                       /* 6 rows by default. */                   \
    SL_HAL_KEYSCAN_DELAY_2MS, /* value 0 = 2ms Scan Delay*/              \
    SL_HAL_KEYSCAN_DELAY_2MS, /* value 0 = 2ms Debounce Delay */         \
    SL_HAL_KEYSCAN_DELAY_2MS, /* value 0 = 2ms Row Stable Delay */       \
    false,                    /* Multi-press by default.*/               \
    false,                    /* No auto-start by default. */            \
  }

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * Initializes KEYSCAN module.
 *
 * @param[in] p_config  A pointer to the KEYSCAN initialization structure
 *                      variable.
 ******************************************************************************/
void sl_hal_keyscan_init(const sl_hal_keyscan_config_t *p_config);

/***************************************************************************//**
 * @brief This function is used to enable the KEYSCAN module, which is typically
 * required before starting any key scanning operations. It ensures that
 * the module is ready by waiting for any ongoing synchronization
 * processes to complete before enabling it. This function should be
 * called after the module has been initialized and configured, and
 * before any scanning operations are initiated. It does not return any
 * value and does not require any parameters.
 *
 * @return None
 ******************************************************************************/
void sl_hal_keyscan_enable(void);

/***************************************************************************//**
 * @brief This function is used to disable the KEYSCAN peripheral module. It
 * should be called when the key scanning functionality is no longer
 * needed or before reconfiguring the module. The function first checks
 * if the module is already disabled and exits immediately if so. If the
 * module is running, it stops the scan and waits for any ongoing
 * synchronization to complete before disabling the module. Note that the
 * function does not wait for the disabling process to finish before
 * returning; use `sl_hal_keyscan_wait_ready` to ensure the module is
 * fully disabled.
 *
 * @return None
 ******************************************************************************/
void sl_hal_keyscan_disable(void);

/***************************************************************************//**
 * @brief The `sl_hal_keyscan_wait_ready` function waits for the KEYSCAN
 * peripheral to complete its reset, disable, or synchronization
 * operations before proceeding.
 *
 * @return The function does not return any value; it simply ensures that the
 * KEYSCAN peripheral is ready by waiting for certain conditions to be
 * cleared.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_wait_ready(void)
{
  while ((KEYSCAN->SWRST & _KEYSCAN_SWRST_RESETTING_MASK) || (KEYSCAN->EN & _KEYSCAN_EN_DISABLING_MASK) || (KEYSCAN->STATUS & _KEYSCAN_STATUS_SYNCBUSY_MASK)) {
    // Wait for resetting, for disabling or for all synchronizations to finish
  }
}

/***************************************************************************//**
 * @brief The `sl_hal_keyscan_wait_sync` function waits for the KEYSCAN
 * peripheral to complete all synchronization of register changes and
 * commands.
 *
 * @return The function does not return any value or output; it simply ensures
 * that the synchronization process is complete before proceeding.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_wait_sync(void)
{
  while ((KEYSCAN->EN != 0U) && (KEYSCAN->STATUS & KEYSCAN_STATUS_SYNCBUSY)) {
    // Wait for all synchronizations to finish
  }
}

/***************************************************************************//**
 * @brief The `sl_hal_keyscan_start_scan` function initiates a key scanning
 * process by sending a start command to the KEYSCAN peripheral after
 * ensuring synchronization and readiness.
 *
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_start_scan(void)
{
  sl_hal_keyscan_wait_sync();
  KEYSCAN->CMD = KEYSCAN_CMD_KEYSCANSTART;
  sl_hal_keyscan_wait_ready();
}

/***************************************************************************//**
 * @brief The `sl_hal_keyscan_stop_scan` function stops the key scanning process
 * by issuing a stop command to the KEYSCAN peripheral after ensuring all
 * previous operations are synchronized.
 *
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_stop_scan(void)
{
  sl_hal_keyscan_wait_sync();
  KEYSCAN->CMD = KEYSCAN_CMD_KEYSCANSTOP;
  sl_hal_keyscan_wait_ready();
}

/***************************************************************************//**
 * @brief Use this function to reset the KEYSCAN peripheral, ensuring it is in a
 * known state. This is useful when you need to reinitialize the
 * peripheral or recover from an error state. The function stops any
 * ongoing scan, enables the peripheral, waits for synchronization, and
 * issues a software reset command. It does not wait for the reset to
 * complete; use `sl_hal_keyscan_wait_ready` if you need to ensure the
 * reset has finished before proceeding.
 *
 * @return None
 ******************************************************************************/
void sl_hal_keyscan_reset(void);

/***************************************************************************//**
 * @brief The `sl_hal_keyscan_get_status` function retrieves the current status
 * of the KEYSCAN peripheral by returning the value of its STATUS
 * register.
 *
 * @return The function returns a `uint32_t` value representing the current
 * contents of the KEYSCAN STATUS register.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_keyscan_get_status(void)
{
  return KEYSCAN->STATUS;
}

/***************************************************************************//**
 * @brief The function `sl_hal_keyscan_enable_interrupts` enables specified
 * KEYSCAN interrupt sources by setting the corresponding bits in the
 * KEYSCAN interrupt enable register.
 *
 * @param flags A 32-bit unsigned integer representing the KEYSCAN interrupt
 * sources to enable, where each bit corresponds to a specific
 * interrupt source.
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_enable_interrupts(uint32_t flags)
{
  KEYSCAN->IEN_SET = flags;
}

/***************************************************************************//**
 * @brief The function `sl_hal_keyscan_disable_interrupts` disables specified
 * KEYSCAN interrupts by clearing the corresponding interrupt enable
 * bits.
 *
 * @param flags A 32-bit unsigned integer representing the KEYSCAN interrupt
 * sources to disable, where each bit corresponds to a specific
 * interrupt source.
 * @return The function does not return any value; it performs a hardware
 * register operation to disable interrupts.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_disable_interrupts(uint32_t flags)
{
  KEYSCAN->IEN_CLR = flags;
}

/***************************************************************************//**
 * @brief The `sl_hal_keyscan_clear_interrupts` function clears specified
 * pending interrupts for the KEYSCAN peripheral by writing to the
 * interrupt flag clear register.
 *
 * @param flags A 32-bit unsigned integer representing the KEYSCAN interrupt
 * sources to clear, typically a set of interrupt flags OR-ed
 * together.
 * @return The function does not return any value; it performs a hardware
 * register operation to clear interrupts.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_clear_interrupts(uint32_t flags)
{
  KEYSCAN->IF_CLR = flags;
}

/***************************************************************************//**
 * @brief The function `sl_hal_keyscan_get_interrupts` retrieves the current
 * pending interrupt flags from the KEYSCAN peripheral.
 *
 * @return The function returns a `uint32_t` value representing the pending
 * interrupt flags from the KEYSCAN peripheral.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_keyscan_get_interrupts(void)
{
  return KEYSCAN->IF;
}

/***************************************************************************//**
 * @brief The function `sl_hal_keyscan_get_enabled_interrupts` retrieves the
 * currently enabled and pending interrupt flags for the KEYSCAN
 * peripheral.
 *
 * @return The function returns a `uint32_t` value representing the bitwise AND
 * of the enabled interrupt sources and the pending interrupt flags,
 * indicating which interrupts are both enabled and pending.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_keyscan_get_enabled_interrupts(void)
{
  return KEYSCAN->IF & KEYSCAN->IEN;
}

/***************************************************************************//**
 * @brief The function `sl_hal_keyscan_set_interrupts` sets one or more pending
 * KEYSCAN interrupts using the provided flags.
 *
 * @param flags A 32-bit unsigned integer representing the KEYSCAN interrupt
 * sources to set as pending, typically using a set of interrupt
 * flags OR-ed together.
 * @return This function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_keyscan_set_interrupts(uint32_t flags)
{
  KEYSCAN->IF_SET = flags;
}

/** @} (end addtogroup keyscan) */

#ifdef __cplusplus
}
#endif

#endif /* defined(KEYSCAN_COUNT) && (KEYSCAN_COUNT > 0) */
#endif /* SL_HAL_KEYSCAN_H */
