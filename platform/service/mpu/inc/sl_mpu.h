/***************************************************************************//**
 * @file
 * @brief MPU API definition.
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

/***************************************************************************//**
 * @addtogroup mpu RAM execution disable
 * @brief RAM execution disable utilities can be used to disable execution from
 *        RAM and other selected memory regions.
 * @details
 *   RAM execution disable utilities are useful to protect against code
 *   injection attacks.
 *   These utilities make use of MPU to disable execution from RAM and other
 *   selected memory regions.
 *
 * @{
 ******************************************************************************/

#ifndef SL_MPU_H
#define SL_MPU_H

#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * Configures internal SRAM as non-executable and enable MPU.
 *
 * @note This function configures the MPU in order to make the entire RAM as
 *       non-executable (with the exception of the functions marked as ramfunc).
 ******************************************************************************/
void sl_mpu_disable_execute_from_ram(void);

/***************************************************************************//**
 * @brief This function is used to configure a specified memory address range as
 * non-executable by setting up a Memory Protection Unit (MPU) region. It
 * is useful for protecting against code injection attacks by ensuring
 * that code cannot be executed from the specified memory range. The
 * function requires the memory region to be at least 32 bytes in size.
 * It should be called when there is a need to secure a specific memory
 * region against execution, typically after system initialization. The
 * function handles overlapping regions and ensures that the MPU is
 * enabled with the new configuration.
 *
 * @param address_begin The starting address of the memory segment to be
 * configured as non-executable. Must be aligned to 32
 * bytes.
 * @param address_end The ending address of the memory segment to be configured
 * as non-executable. Must be aligned to 32 bytes.
 * @param size The size of the memory segment. Must be at least 32 bytes. If the
 * size is less than 32 bytes, the function will not configure the
 * region.
 * @return Returns SL_STATUS_OK if the configuration is successful. Returns
 * SL_STATUS_NO_MORE_RESOURCE if no more MPU regions are available, or
 * SL_STATUS_INVALID_RANGE if the specified range overlaps with an
 * existing region.
 ******************************************************************************/
sl_status_t sl_mpu_disable_execute(uint32_t address_begin,
                                   uint32_t address_end,
                                   uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* SL_MPU_H */

/** @} (end addtogroup mpu) */
