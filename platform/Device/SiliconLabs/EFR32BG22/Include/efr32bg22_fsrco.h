/**************************************************************************//**
 * @file
 * @brief EFR32BG22 FSRCO register and bit field definitions
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
#ifndef EFR32BG22_FSRCO_H
#define EFR32BG22_FSRCO_H
#define FSRCO_HAS_SET_CLEAR

/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @defgroup EFR32BG22_FSRCO FSRCO
 * @{
 * @brief EFR32BG22 FSRCO Register Declaration.
 *****************************************************************************/

/***************************************************************************//**
 * @brief The FSRCO_TypeDef is a structure that defines the register layout for
 * the FSRCO (Frequency Synthesizer RC Oscillator) in the EFR32BG22
 * microcontroller. It includes fields for the IP version and several
 * reserved arrays for future use, ensuring compatibility with potential
 * future updates. The structure provides fields for setting, clearing,
 * and toggling the IP version, which are essential for managing the
 * oscillator's configuration and operation.
 *
 * @param IPVERSION Represents the IP version of the FSRCO.
 * @param RESERVED0 Array reserved for future use, consisting of 1023 unsigned
 * 32-bit integers.
 * @param IPVERSION_SET Represents the IP version of the FSRCO, used for setting
 * operations.
 * @param RESERVED1 Array reserved for future use, consisting of 1023 unsigned
 * 32-bit integers.
 * @param IPVERSION_CLR Represents the IP version of the FSRCO, used for
 * clearing operations.
 * @param RESERVED2 Array reserved for future use, consisting of 1023 unsigned
 * 32-bit integers.
 * @param IPVERSION_TGL Represents the IP version of the FSRCO, used for
 * toggling operations.
 ******************************************************************************/
typedef struct fsrco_typedef{
  __IM uint32_t IPVERSION;                      /**< IP Version                                         */
  uint32_t      RESERVED0[1023U];               /**< Reserved for future use                            */
  __IM uint32_t IPVERSION_SET;                  /**< IP Version                                         */
  uint32_t      RESERVED1[1023U];               /**< Reserved for future use                            */
  __IM uint32_t IPVERSION_CLR;                  /**< IP Version                                         */
  uint32_t      RESERVED2[1023U];               /**< Reserved for future use                            */
  __IM uint32_t IPVERSION_TGL;                  /**< IP Version                                         */
} FSRCO_TypeDef;
/** @} End of group EFR32BG22_FSRCO */

/**************************************************************************//**
 * @addtogroup EFR32BG22_FSRCO
 * @{
 * @defgroup EFR32BG22_FSRCO_BitFields FSRCO Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for FSRCO IPVERSION */
#define _FSRCO_IPVERSION_RESETVALUE           0x00000001UL                              /**< Default value for FSRCO_IPVERSION           */
#define _FSRCO_IPVERSION_MASK                 0xFFFFFFFFUL                              /**< Mask for FSRCO_IPVERSION                    */
#define _FSRCO_IPVERSION_IPVERSION_SHIFT      0                                         /**< Shift value for FSRCO_IPVERSION             */
#define _FSRCO_IPVERSION_IPVERSION_MASK       0xFFFFFFFFUL                              /**< Bit mask for FSRCO_IPVERSION                */
#define _FSRCO_IPVERSION_IPVERSION_DEFAULT    0x00000001UL                              /**< Mode DEFAULT for FSRCO_IPVERSION            */
#define FSRCO_IPVERSION_IPVERSION_DEFAULT     (_FSRCO_IPVERSION_IPVERSION_DEFAULT << 0) /**< Shifted mode DEFAULT for FSRCO_IPVERSION    */

/** @} End of group EFR32BG22_FSRCO_BitFields */
/** @} End of group EFR32BG22_FSRCO */
/** @} End of group Parts */

#endif // EFR32BG22_FSRCO_H
