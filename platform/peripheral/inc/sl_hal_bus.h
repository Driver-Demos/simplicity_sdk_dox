/***************************************************************************//**
 * @file
 * @brief RAM and peripheral bit-field set, clear, read and write API.
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_HAL_BUS_H
#define SL_HAL_BUS_H

#include "sl_assert.h"
#include "sl_core.h"
#include "em_device.h"
#include "sl_code_classification.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup bus BUS - Bitfield Read/Write
 * @brief BUS register and RAM bit-field read/write API
 * @details
 *  API to perform field set/clear/write/read access to RAM and peripheral's registers.
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief The `sl_hal_bus_ram_write_bit` function performs a single-bit write
 * operation on a 32-bit word in RAM, setting a specified bit to a given
 * value.
 *
 * @param addr A pointer to a volatile 32-bit unsigned integer representing the
 * address of a 32-bit word in RAM.
 * @param bit An unsigned 32-bit integer representing the bit position to write,
 * ranging from 0 to 31.
 * @param val An unsigned 32-bit integer representing the value to set the bit
 * to, either 0 or 1.
 * @return The function does not return a value; it modifies the 32-bit word at
 * the specified RAM address in place.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_ram_write_bit(volatile uint32_t *addr,
                                              uint32_t bit,
                                              uint32_t val)
{
  uint32_t tmp = *addr;

  /* Make sure val is not more than 1 because only one bit needs to be set. */
  *addr = (tmp & ~(1UL << bit)) | ((val & 1UL) << bit);
}

/***************************************************************************//**
 * @brief The function `sl_hal_bus_ram_read_bit` reads a specific bit from a
 * 32-bit word in RAM and returns its value.
 *
 * @param addr A pointer to a volatile constant 32-bit unsigned integer
 * representing the address of a 32-bit word in RAM.
 * @param bit An unsigned 32-bit integer representing the bit position to read,
 * ranging from 0 to 31.
 * @return The function returns an unsigned integer representing the value of
 * the specified bit, either 0 or 1.
 ******************************************************************************/
__STATIC_INLINE unsigned int sl_hal_bus_ram_read_bit(volatile const uint32_t *addr,
                                                     uint32_t bit)
{
  return ((*addr) >> bit) & 1UL;
}

/***************************************************************************//**
 * @brief
 *   Perform a single-bit atomic write operation on a peripheral register.
 *
 * @details
 *   This function uses built-in hardware 4K-aliased addressing that allows to
 *   perform an atomic read-modify-write operation on a single register bit.
 *   See the reference manual for more details about alias addressing.
 *
 * @param[in] addr A peripheral register address.
 *
 * @param[in] bit A bit position to write, 0-31.
 *
 * @param[in] val A value to set bit to, 0 or 1.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_HAL_COMMON, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The `sl_hal_bus_reg_write_bit` function performs an atomic write
 * operation to set or clear a specific bit in a 32-bit peripheral
 * register using alias addressing if available.
 *
 * @param addr A pointer to a volatile 32-bit unsigned integer representing the
 * address of the peripheral register.
 * @param bit An unsigned 32-bit integer specifying the bit position (0-31) to
 * be written.
 * @param val An unsigned 32-bit integer representing the value to set the bit
 * to, either 0 or 1.
 * @return The function does not return a value; it performs an in-place
 * modification of the specified bit in the peripheral register.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_reg_write_bit(volatile uint32_t *addr,
                                              uint32_t bit,
                                              uint32_t val)
{
  EFM_ASSERT(bit < 32U);

#if defined(PER_REG_BLOCK_SET_OFFSET) && defined(PER_REG_BLOCK_CLR_OFFSET)
  uint32_t aliasAddr;
  if (val != 0U) {
    aliasAddr = (uint32_t)addr + PER_REG_BLOCK_SET_OFFSET;
  } else {
    aliasAddr = (uint32_t)addr + PER_REG_BLOCK_CLR_OFFSET;
  }
  *(volatile uint32_t *)aliasAddr = 1UL << bit;
#else
  uint32_t tmp = *addr;

  // Make sure val is not more than 1 because only one bit needs to be set.
  *addr = (tmp & ~(1 << bit)) | ((val & 1) << bit);
#endif
}

/***************************************************************************//**
 * @brief The function `sl_hal_bus_reg_read_bit` reads a specific bit from a
 * 32-bit peripheral register and returns its value.
 *
 * @param addr A pointer to a volatile constant 32-bit unsigned integer
 * representing the address of the peripheral register.
 * @param bit An unsigned 32-bit integer specifying the bit position to read,
 * ranging from 0 to 31.
 * @return The function returns the value of the specified bit, shifted to the
 * least significant bit position, as an unsigned integer (0 or 1).
 ******************************************************************************/
__STATIC_INLINE unsigned int sl_hal_bus_reg_read_bit(volatile const uint32_t *addr,
                                                     uint32_t bit)
{
  return ((*addr) >> bit) & 1UL;
}

/***************************************************************************//**
 * @brief The `sl_hal_bus_reg_set_mask` function performs an atomic masked set
 * operation on a peripheral register address, setting bits specified by
 * the mask to 1.
 *
 * @param addr A pointer to a volatile 32-bit unsigned integer representing the
 * peripheral register address.
 * @param mask A 32-bit unsigned integer representing the bit mask to set in the
 * register.
 * @return The function does not return a value; it modifies the register at the
 * given address by setting bits specified in the mask.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_reg_set_mask(volatile uint32_t *addr,
                                             uint32_t mask)
{
#if defined(PER_REG_BLOCK_SET_OFFSET)
  uint32_t aliasAddr = (uint32_t)addr + PER_REG_BLOCK_SET_OFFSET;
  *(volatile uint32_t *)aliasAddr = mask;
#else
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  *addr |= mask;
  CORE_EXIT_CRITICAL();
#endif
}

/***************************************************************************//**
 * @brief The `sl_hal_bus_reg_clear_mask` function performs an atomic masked
 * clear operation on a peripheral register address, clearing bits
 * specified by the mask.
 *
 * @param addr A pointer to a volatile 32-bit unsigned integer representing the
 * peripheral register address.
 * @param mask A 32-bit unsigned integer mask specifying which bits to clear in
 * the register.
 * @return The function does not return a value; it modifies the register at the
 * given address by clearing the bits specified in the mask.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_reg_clear_mask(volatile uint32_t *addr,
                                               uint32_t mask)
{
#if defined(PER_REG_BLOCK_CLR_OFFSET)
  uint32_t aliasAddr = (uint32_t)addr + PER_REG_BLOCK_CLR_OFFSET;
  *(volatile uint32_t *)aliasAddr = mask;
#else
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  *addr &= ~mask;
  CORE_EXIT_CRITICAL();
#endif
}

/***************************************************************************//**
 * @brief The function `sl_hal_bus_reg_write_mask` performs an atomic masked
 * write operation on a peripheral register, updating only the bits
 * specified by the mask with the provided value.
 *
 * @param addr A pointer to a volatile 32-bit unsigned integer representing the
 * address of the peripheral register to be modified.
 * @param mask A 32-bit unsigned integer representing the bitmask that specifies
 * which bits in the register should be updated.
 * @param val A 32-bit unsigned integer containing the value to be written to
 * the register, masked and aligned according to the mask.
 * @return The function does not return a value; it modifies the register at the
 * specified address in place.
 ******************************************************************************/
__STATIC_INLINE void sl_hal_bus_reg_write_mask(volatile uint32_t *addr,
                                               uint32_t mask,
                                               uint32_t val)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  *addr = (*addr & ~mask) | (val & mask);
  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief The `sl_hal_bus_reg_read_mask` function performs a masked read
 * operation on a peripheral register, returning the masked value.
 *
 * @param addr A pointer to a volatile constant 32-bit unsigned integer
 * representing the address of the peripheral register to be read.
 * @param mask A 32-bit unsigned integer representing the mask to be applied to
 * the register value.
 * @return The function returns a 32-bit unsigned integer which is the result of
 * the masked read operation, i.e., the value at the specified address
 * with the mask applied.
 ******************************************************************************/
__STATIC_INLINE uint32_t sl_hal_bus_reg_read_mask(volatile const uint32_t *addr,
                                                  uint32_t mask)
{
  return *addr & mask;
}

/** @} (end addtogroup bus) */

#ifdef __cplusplus
}
#endif

#endif /* SL_HAL_BUS_H */
