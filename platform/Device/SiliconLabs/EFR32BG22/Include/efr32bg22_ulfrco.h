/**************************************************************************//**
 * @file
 * @brief EFR32BG22 ULFRCO register and bit field definitions
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
#ifndef EFR32BG22_ULFRCO_H
#define EFR32BG22_ULFRCO_H
#define ULFRCO_HAS_SET_CLEAR

/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @defgroup EFR32BG22_ULFRCO ULFRCO
 * @{
 * @brief EFR32BG22 ULFRCO Register Declaration.
 *****************************************************************************/

/***************************************************************************//**
 * @brief The ULFRCO_TypeDef is a structure representing the Ultra Low Frequency
 * RC Oscillator (ULFRCO) registers for the EFR32BG22 microcontroller. It
 * includes fields for IP version, status, interrupt flags, and interrupt
 * enable registers, with multiple reserved fields for future use. The
 * structure also provides set, clear, and toggle versions of these
 * registers, allowing for flexible manipulation of the oscillator's
 * configuration and status. This design facilitates efficient control
 * and monitoring of the ULFRCO's operation within the microcontroller.
 *
 * @param IPVERSION IP version.
 * @param RESERVED0 Reserved for future use.
 * @param STATUS Status Register.
 * @param RESERVED1 Reserved for future use.
 * @param IF Interrupt Flag Register.
 * @param IEN Interrupt Enable Register.
 * @param RESERVED2 Reserved for future use.
 * @param IPVERSION_SET IP version.
 * @param RESERVED3 Reserved for future use.
 * @param STATUS_SET Status Register.
 * @param RESERVED4 Reserved for future use.
 * @param IF_SET Interrupt Flag Register.
 * @param IEN_SET Interrupt Enable Register.
 * @param RESERVED5 Reserved for future use.
 * @param IPVERSION_CLR IP version.
 * @param RESERVED6 Reserved for future use.
 * @param STATUS_CLR Status Register.
 * @param RESERVED7 Reserved for future use.
 * @param IF_CLR Interrupt Flag Register.
 * @param IEN_CLR Interrupt Enable Register.
 * @param RESERVED8 Reserved for future use.
 * @param IPVERSION_TGL IP version.
 * @param RESERVED9 Reserved for future use.
 * @param STATUS_TGL Status Register.
 * @param RESERVED10 Reserved for future use.
 * @param IF_TGL Interrupt Flag Register.
 * @param IEN_TGL Interrupt Enable Register.
 ******************************************************************************/
typedef struct ulfrco_typedef{
  __IM uint32_t  IPVERSION;                     /**< IP version                                         */
  uint32_t       RESERVED0[1U];                 /**< Reserved for future use                            */
  __IM uint32_t  STATUS;                        /**< Status Register                                    */
  uint32_t       RESERVED1[2U];                 /**< Reserved for future use                            */
  __IOM uint32_t IF;                            /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN;                           /**< Interrupt Enable Register                          */
  uint32_t       RESERVED2[1017U];              /**< Reserved for future use                            */
  __IM uint32_t  IPVERSION_SET;                 /**< IP version                                         */
  uint32_t       RESERVED3[1U];                 /**< Reserved for future use                            */
  __IM uint32_t  STATUS_SET;                    /**< Status Register                                    */
  uint32_t       RESERVED4[2U];                 /**< Reserved for future use                            */
  __IOM uint32_t IF_SET;                        /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN_SET;                       /**< Interrupt Enable Register                          */
  uint32_t       RESERVED5[1017U];              /**< Reserved for future use                            */
  __IM uint32_t  IPVERSION_CLR;                 /**< IP version                                         */
  uint32_t       RESERVED6[1U];                 /**< Reserved for future use                            */
  __IM uint32_t  STATUS_CLR;                    /**< Status Register                                    */
  uint32_t       RESERVED7[2U];                 /**< Reserved for future use                            */
  __IOM uint32_t IF_CLR;                        /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN_CLR;                       /**< Interrupt Enable Register                          */
  uint32_t       RESERVED8[1017U];              /**< Reserved for future use                            */
  __IM uint32_t  IPVERSION_TGL;                 /**< IP version                                         */
  uint32_t       RESERVED9[1U];                 /**< Reserved for future use                            */
  __IM uint32_t  STATUS_TGL;                    /**< Status Register                                    */
  uint32_t       RESERVED10[2U];                /**< Reserved for future use                            */
  __IOM uint32_t IF_TGL;                        /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN_TGL;                       /**< Interrupt Enable Register                          */
} ULFRCO_TypeDef;
/** @} End of group EFR32BG22_ULFRCO */

/**************************************************************************//**
 * @addtogroup EFR32BG22_ULFRCO
 * @{
 * @defgroup EFR32BG22_ULFRCO_BitFields ULFRCO Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for ULFRCO IPVERSION */
#define _ULFRCO_IPVERSION_RESETVALUE           0x00000000UL                               /**< Default value for ULFRCO_IPVERSION          */
#define _ULFRCO_IPVERSION_MASK                 0xFFFFFFFFUL                               /**< Mask for ULFRCO_IPVERSION                   */
#define _ULFRCO_IPVERSION_IPVERSION_SHIFT      0                                          /**< Shift value for ULFRCO_IPVERSION            */
#define _ULFRCO_IPVERSION_IPVERSION_MASK       0xFFFFFFFFUL                               /**< Bit mask for ULFRCO_IPVERSION               */
#define _ULFRCO_IPVERSION_IPVERSION_DEFAULT    0x00000000UL                               /**< Mode DEFAULT for ULFRCO_IPVERSION           */
#define ULFRCO_IPVERSION_IPVERSION_DEFAULT     (_ULFRCO_IPVERSION_IPVERSION_DEFAULT << 0) /**< Shifted mode DEFAULT for ULFRCO_IPVERSION   */

/* Bit fields for ULFRCO STATUS */
#define _ULFRCO_STATUS_RESETVALUE              0x00000000UL                             /**< Default value for ULFRCO_STATUS             */
#define _ULFRCO_STATUS_MASK                    0x00010001UL                             /**< Mask for ULFRCO_STATUS                      */
#define ULFRCO_STATUS_RDY                      (0x1UL << 0)                             /**< Ready Status                                */
#define _ULFRCO_STATUS_RDY_SHIFT               0                                        /**< Shift value for ULFRCO_RDY                  */
#define _ULFRCO_STATUS_RDY_MASK                0x1UL                                    /**< Bit mask for ULFRCO_RDY                     */
#define _ULFRCO_STATUS_RDY_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for ULFRCO_STATUS              */
#define ULFRCO_STATUS_RDY_DEFAULT              (_ULFRCO_STATUS_RDY_DEFAULT << 0)        /**< Shifted mode DEFAULT for ULFRCO_STATUS      */
#define ULFRCO_STATUS_ENS                      (0x1UL << 16)                            /**< Enable Status                               */
#define _ULFRCO_STATUS_ENS_SHIFT               16                                       /**< Shift value for ULFRCO_ENS                  */
#define _ULFRCO_STATUS_ENS_MASK                0x10000UL                                /**< Bit mask for ULFRCO_ENS                     */
#define _ULFRCO_STATUS_ENS_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for ULFRCO_STATUS              */
#define ULFRCO_STATUS_ENS_DEFAULT              (_ULFRCO_STATUS_ENS_DEFAULT << 16)       /**< Shifted mode DEFAULT for ULFRCO_STATUS      */

/* Bit fields for ULFRCO IF */
#define _ULFRCO_IF_RESETVALUE                  0x00000000UL                             /**< Default value for ULFRCO_IF                 */
#define _ULFRCO_IF_MASK                        0x00000007UL                             /**< Mask for ULFRCO_IF                          */
#define ULFRCO_IF_RDY                          (0x1UL << 0)                             /**< Ready Interrupt Flag                        */
#define _ULFRCO_IF_RDY_SHIFT                   0                                        /**< Shift value for ULFRCO_RDY                  */
#define _ULFRCO_IF_RDY_MASK                    0x1UL                                    /**< Bit mask for ULFRCO_RDY                     */
#define _ULFRCO_IF_RDY_DEFAULT                 0x00000000UL                             /**< Mode DEFAULT for ULFRCO_IF                  */
#define ULFRCO_IF_RDY_DEFAULT                  (_ULFRCO_IF_RDY_DEFAULT << 0)            /**< Shifted mode DEFAULT for ULFRCO_IF          */
#define ULFRCO_IF_POSEDGE                      (0x1UL << 1)                             /**< Positive Edge Interrupt Flag                */
#define _ULFRCO_IF_POSEDGE_SHIFT               1                                        /**< Shift value for ULFRCO_POSEDGE              */
#define _ULFRCO_IF_POSEDGE_MASK                0x2UL                                    /**< Bit mask for ULFRCO_POSEDGE                 */
#define _ULFRCO_IF_POSEDGE_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for ULFRCO_IF                  */
#define ULFRCO_IF_POSEDGE_DEFAULT              (_ULFRCO_IF_POSEDGE_DEFAULT << 1)        /**< Shifted mode DEFAULT for ULFRCO_IF          */
#define ULFRCO_IF_NEGEDGE                      (0x1UL << 2)                             /**< Negative Edge Interrupt Flag                */
#define _ULFRCO_IF_NEGEDGE_SHIFT               2                                        /**< Shift value for ULFRCO_NEGEDGE              */
#define _ULFRCO_IF_NEGEDGE_MASK                0x4UL                                    /**< Bit mask for ULFRCO_NEGEDGE                 */
#define _ULFRCO_IF_NEGEDGE_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for ULFRCO_IF                  */
#define ULFRCO_IF_NEGEDGE_DEFAULT              (_ULFRCO_IF_NEGEDGE_DEFAULT << 2)        /**< Shifted mode DEFAULT for ULFRCO_IF          */

/* Bit fields for ULFRCO IEN */
#define _ULFRCO_IEN_RESETVALUE                 0x00000000UL                             /**< Default value for ULFRCO_IEN                */
#define _ULFRCO_IEN_MASK                       0x00000007UL                             /**< Mask for ULFRCO_IEN                         */
#define ULFRCO_IEN_RDY                         (0x1UL << 0)                             /**< Enable Ready Interrupt                      */
#define _ULFRCO_IEN_RDY_SHIFT                  0                                        /**< Shift value for ULFRCO_RDY                  */
#define _ULFRCO_IEN_RDY_MASK                   0x1UL                                    /**< Bit mask for ULFRCO_RDY                     */
#define _ULFRCO_IEN_RDY_DEFAULT                0x00000000UL                             /**< Mode DEFAULT for ULFRCO_IEN                 */
#define ULFRCO_IEN_RDY_DEFAULT                 (_ULFRCO_IEN_RDY_DEFAULT << 0)           /**< Shifted mode DEFAULT for ULFRCO_IEN         */
#define ULFRCO_IEN_POSEDGE                     (0x1UL << 1)                             /**< Enable Positive Edge Interrupt              */
#define _ULFRCO_IEN_POSEDGE_SHIFT              1                                        /**< Shift value for ULFRCO_POSEDGE              */
#define _ULFRCO_IEN_POSEDGE_MASK               0x2UL                                    /**< Bit mask for ULFRCO_POSEDGE                 */
#define _ULFRCO_IEN_POSEDGE_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for ULFRCO_IEN                 */
#define ULFRCO_IEN_POSEDGE_DEFAULT             (_ULFRCO_IEN_POSEDGE_DEFAULT << 1)       /**< Shifted mode DEFAULT for ULFRCO_IEN         */
#define ULFRCO_IEN_NEGEDGE                     (0x1UL << 2)                             /**< Enable Negative Edge Interrupt              */
#define _ULFRCO_IEN_NEGEDGE_SHIFT              2                                        /**< Shift value for ULFRCO_NEGEDGE              */
#define _ULFRCO_IEN_NEGEDGE_MASK               0x4UL                                    /**< Bit mask for ULFRCO_NEGEDGE                 */
#define _ULFRCO_IEN_NEGEDGE_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for ULFRCO_IEN                 */
#define ULFRCO_IEN_NEGEDGE_DEFAULT             (_ULFRCO_IEN_NEGEDGE_DEFAULT << 2)       /**< Shifted mode DEFAULT for ULFRCO_IEN         */

/** @} End of group EFR32BG22_ULFRCO_BitFields */
/** @} End of group EFR32BG22_ULFRCO */
/** @} End of group Parts */

#endif // EFR32BG22_ULFRCO_H
