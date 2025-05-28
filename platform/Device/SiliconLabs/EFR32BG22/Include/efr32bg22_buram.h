/**************************************************************************//**
 * @file
 * @brief EFR32BG22 BURAM register and bit field definitions
 ******************************************************************************
 * # License
 * <b>Copyright 2025 Silicon Laboratories, Inc. www.silabs.com</b>
 ******************************************************************************
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
 *****************************************************************************/
#ifndef EFR32BG22_BURAM_H
#define EFR32BG22_BURAM_H
#define BURAM_HAS_SET_CLEAR

/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @defgroup EFR32BG22_BURAM BURAM
 * @{
 * @brief EFR32BG22 BURAM Register Declaration.
 *****************************************************************************/

/***************************************************************************//**
 * @brief The `BURAM_RET_TypeDef` is a simple data structure that consists of a
 * single 32-bit retention register, denoted as `REG`. This register is
 * used to store data that needs to be retained across power cycles or
 * resets, making it useful for applications requiring persistent data
 * storage. The use of `__IOM` indicates that the register is both
 * readable and writable, allowing for flexible data manipulation.
 *
 * @param REG A 32-bit retention register used for storing data persistently.
 ******************************************************************************/
typedef struct buram_ret_typedef{
  __IOM uint32_t REG;                                /**< Retention Register                                 */
} BURAM_RET_TypeDef;

/***************************************************************************//**
 * @brief The BURAM_TypeDef structure is designed to manage a set of retention
 * registers within the EFR32BG22 microcontroller. It includes arrays for
 * standard retention operations (RET), as well as specialized arrays for
 * setting (RET_SET), clearing (RET_CLR), and toggling (RET_TGL) register
 * values. The structure also reserves space for future expansion or
 * additional features, ensuring compatibility with potential updates or
 * changes in the hardware design.
 *
 * @param RET An array of 32 retention registers.
 * @param RESERVED0 An array of 992 reserved spaces for future use.
 * @param RET_SET An array of 32 retention registers for setting values.
 * @param RESERVED1 An array of 992 reserved spaces for future use.
 * @param RET_CLR An array of 32 retention registers for clearing values.
 * @param RESERVED2 An array of 992 reserved spaces for future use.
 * @param RET_TGL An array of 32 retention registers for toggling values.
 ******************************************************************************/
typedef struct buram_typedef{
  BURAM_RET_TypeDef RET[32U];                   /**< RetentionReg                                       */
  uint32_t          RESERVED0[992U];            /**< Reserved for future use                            */
  BURAM_RET_TypeDef RET_SET[32U];               /**< RetentionReg                                       */
  uint32_t          RESERVED1[992U];            /**< Reserved for future use                            */
  BURAM_RET_TypeDef RET_CLR[32U];               /**< RetentionReg                                       */
  uint32_t          RESERVED2[992U];            /**< Reserved for future use                            */
  BURAM_RET_TypeDef RET_TGL[32U];               /**< RetentionReg                                       */
} BURAM_TypeDef;
/** @} End of group EFR32BG22_BURAM */

/**************************************************************************//**
 * @addtogroup EFR32BG22_BURAM
 * @{
 * @defgroup EFR32BG22_BURAM_BitFields BURAM Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for BURAM RET_REG */
#define _BURAM_RET_REG_RESETVALUE        0x00000000UL                                   /**< Default value for BURAM_RET_REG             */
#define _BURAM_RET_REG_MASK              0xFFFFFFFFUL                                   /**< Mask for BURAM_RET_REG                      */
#define _BURAM_RET_REG_RETREG_SHIFT      0                                              /**< Shift value for BURAM_RETREG                */
#define _BURAM_RET_REG_RETREG_MASK       0xFFFFFFFFUL                                   /**< Bit mask for BURAM_RETREG                   */
#define _BURAM_RET_REG_RETREG_DEFAULT    0x00000000UL                                   /**< Mode DEFAULT for BURAM_RET_REG              */
#define BURAM_RET_REG_RETREG_DEFAULT     (_BURAM_RET_REG_RETREG_DEFAULT << 0)           /**< Shifted mode DEFAULT for BURAM_RET_REG      */

/** @} End of group EFR32BG22_BURAM_BitFields */
/** @} End of group EFR32BG22_BURAM */
/** @} End of group Parts */

#endif // EFR32BG22_BURAM_H
