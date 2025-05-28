/**************************************************************************//**
 * @file
 * @brief CMSIS system header file for EFR32BG22
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

#ifndef SYSTEM_EFR32BG22_H
#define SYSTEM_EFR32BG22_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "sl_code_classification.h"

/***************************************************************************//**
 * @addtogroup Parts
 * @{
 ******************************************************************************/
/***************************************************************************//**
 * @addtogroup EFR32BG22 EFR32BG22
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   TYPEDEFS   ***********************************
 ******************************************************************************/

/* Interrupt vectortable entry */
/***************************************************************************//**
 * @brief The `tVectorEntry` is a union data structure used to represent an
 * entry in an interrupt vector table for the EFR32BG22 system. It can
 * either hold a function pointer, `VECTOR_TABLE_Type`, which points to
 * an interrupt handler function, or a `topOfStack` pointer, which points
 * to the top of the stack. This union allows for flexible representation
 * of vector table entries, accommodating both function pointers and
 * stack pointers as needed in the system's interrupt handling mechanism.
 *
 * @param VECTOR_TABLE_Type A function pointer to a void function, representing
 * an entry in the vector table.
 * @param topOfStack A pointer to void, representing the top of the stack.
 ******************************************************************************/
typedef union {
  void (*VECTOR_TABLE_Type)(void);
  void *topOfStack;
} tVectorEntry;

/*******************************************************************************
 **************************   GLOBAL VARIABLES   *******************************
 ******************************************************************************/

#if !defined(SYSTEM_NO_STATIC_MEMORY)
extern uint32_t SystemCoreClock;     /**< System Clock Frequency (Core Clock) */
extern uint32_t SystemHfrcoFreq;     /**< System HFRCO frequency */
#endif

/*Re-direction of IRQn.*/
#if defined (SL_TRUSTZONE_SECURE)
#define SMU_PRIVILEGED_IRQn    SMU_S_PRIVILEGED_IRQn
#else
#define SMU_PRIVILEGED_IRQn    SMU_NS_PRIVILEGED_IRQn
#endif /* SL_TRUSTZONE_SECURE */

/*Re-direction of IRQHandler.*/
#if defined (SL_TRUSTZONE_SECURE)
#define SMU_PRIVILEGED_IRQHandler    SMU_S_PRIVILEGED_IRQHandler
#else
#define SMU_PRIVILEGED_IRQHandler    SMU_NS_PRIVILEGED_IRQHandler
#endif /* SL_TRUSTZONE_SECURE */

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void Reset_Handler(void);           /**< Reset Handler */
void NMI_Handler(void);             /**< NMI Handler */
/***************************************************************************//**
 * @brief This function is an interrupt handler that is automatically invoked
 * when a memory management fault occurs in the system. It is part of the
 * Cortex-M exception handling mechanism and is typically used to manage
 * faults related to memory protection unit (MPU) violations. This
 * function should be used in systems where memory protection is
 * critical, and it is essential to handle such faults to prevent system
 * crashes or undefined behavior. It is generally called by the system
 * and not directly by user code.
 *
 * @return None
 ******************************************************************************/
void MemManage_Handler(void);       /**< MPU Fault Handler */
void BusFault_Handler(void);        /**< Bus Fault Handler */
void UsageFault_Handler(void);      /**< Usage Fault Handler */
void SecureFault_Handler(void);     /**< Secure Fault Handler */
void SVC_Handler(void);             /**< SVCall Handler */
void DebugMon_Handler(void);        /**< Debug Monitor Handler */
void PendSV_Handler(void);          /**< PendSV Handler */
void SysTick_Handler(void);         /**< SysTick Handler */

/* Part Specific Interrupts */
void CRYPTOACC_IRQHandler(void);         /**< CRYPTOACC IRQ Handler */
void TRNG_IRQHandler(void);              /**< TRNG IRQ Handler */
void PKE_IRQHandler(void);               /**< PKE IRQ Handler */
void SMU_SECURE_IRQHandler(void);        /**< SMU_SECURE IRQ Handler */
void SMU_S_PRIVILEGED_IRQHandler(void);  /**< SMU_S_PRIVILEGED IRQ Handler */
void SMU_NS_PRIVILEGED_IRQHandler(void); /**< SMU_NS_PRIVILEGED IRQ Handler */
void EMU_IRQHandler(void);               /**< EMU IRQ Handler */
void TIMER0_IRQHandler(void);            /**< TIMER0 IRQ Handler */
void TIMER1_IRQHandler(void);            /**< TIMER1 IRQ Handler */
void TIMER2_IRQHandler(void);            /**< TIMER2 IRQ Handler */
void TIMER3_IRQHandler(void);            /**< TIMER3 IRQ Handler */
/***************************************************************************//**
 * @brief This function is the interrupt handler for the Real-Time Counter and
 * Calendar (RTCC) peripheral. It should be used to process RTCC-related
 * interrupts, specifically overflow and compare match events. The
 * function reads the interrupt flags, determines which events have
 * occurred, clears the relevant interrupt flags, and processes the
 * events accordingly. It is typically called automatically by the system
 * when an RTCC interrupt occurs and should not be called directly by
 * user code. The function operates within an atomic section to ensure
 * interrupt handling is not disrupted by other interrupts.
 *
 * @return None
 ******************************************************************************/
void RTCC_IRQHandler(void);              /**< RTCC IRQ Handler */
void USART0_RX_IRQHandler(void);         /**< USART0_RX IRQ Handler */
void USART0_TX_IRQHandler(void);         /**< USART0_TX IRQ Handler */
void USART1_RX_IRQHandler(void);         /**< USART1_RX IRQ Handler */
void USART1_TX_IRQHandler(void);         /**< USART1_TX IRQ Handler */
/***************************************************************************//**
 * @brief This function is called to handle the interrupt request for the Backup
 * Real-Time Counter (BURTC). It processes the interrupt by first
 * entering an atomic section to safely read and clear the interrupt
 * flags. The function then processes the timer interrupt based on the
 * flags that were set. This handler should be used in systems where the
 * BURTC is utilized, and it is essential to ensure that the interrupt
 * flags are correctly managed to prevent missed interrupts. The function
 * does not take any parameters and does not return a value, as it is
 * designed to be called by the system's interrupt vector table.
 *
 * @return None
 ******************************************************************************/
void BURTC_IRQHandler(void);             /**< BURTC IRQ Handler */
void LETIMER0_IRQHandler(void);          /**< LETIMER0 IRQ Handler */
void SYSCFG_IRQHandler(void);            /**< SYSCFG IRQ Handler */
void LDMA_IRQHandler(void);              /**< LDMA IRQ Handler */
void LFXO_IRQHandler(void);              /**< LFXO IRQ Handler */
void LFRCO_IRQHandler(void);             /**< LFRCO IRQ Handler */
/***************************************************************************//**
 * @brief This function is an interrupt handler for odd-numbered GPIO pins. It
 * should be used in systems where GPIO interrupts are enabled and need
 * to be managed. The function retrieves all enabled and pending
 * interrupts for odd GPIO pins, clears them, and then dispatches the
 * interrupt handling to the appropriate handler. It is typically called
 * automatically by the system when an odd GPIO interrupt occurs, and it
 * assumes that the interrupt system and GPIO configuration are properly
 * set up beforehand.
 *
 * @return None
 ******************************************************************************/
/***************************************************************************//**
 * @brief This function is an interrupt handler for even-numbered GPIO pins. It
 * should be used in systems where GPIO interrupts are enabled and need
 * to be managed. The function retrieves all enabled and pending even
 * GPIO interrupts, clears them, and then dispatches the interrupt
 * handling to the appropriate handlers. It is typically called
 * automatically by the system when an even GPIO interrupt occurs, and it
 * assumes that the interrupt system and GPIO configuration are properly
 * set up beforehand.
 *
 * @return None
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void);         /**< GPIO_EVEN IRQ Handler */
void I2C0_IRQHandler(void);              /**< I2C0 IRQ Handler */
void I2C1_IRQHandler(void);              /**< I2C1 IRQ Handler */
void EMUDG_IRQHandler(void);             /**< EMUDG IRQ Handler */
void EMUSE_IRQHandler(void);             /**< EMUSE IRQ Handler */
void AGC_IRQHandler(void);               /**< AGC IRQ Handler */
void BUFC_IRQHandler(void);              /**< BUFC IRQ Handler */
void FRC_PRI_IRQHandler(void);           /**< FRC_PRI IRQ Handler */
void FRC_IRQHandler(void);               /**< FRC IRQ Handler */
void MODEM_IRQHandler(void);             /**< MODEM IRQ Handler */
void PROTIMER_IRQHandler(void);          /**< PROTIMER IRQ Handler */
void RAC_RSM_IRQHandler(void);           /**< RAC_RSM IRQ Handler */
void RAC_SEQ_IRQHandler(void);           /**< RAC_SEQ IRQ Handler */
void RDMAILBOX_IRQHandler(void);         /**< RDMAILBOX IRQ Handler */
void RFSENSE_IRQHandler(void);           /**< RFSENSE IRQ Handler */
void PRORTC_IRQHandler(void);            /**< PRORTC IRQ Handler */
void SYNTH_IRQHandler(void);             /**< SYNTH IRQ Handler */
void WDOG0_IRQHandler(void);             /**< WDOG0 IRQ Handler */
void HFXO0_IRQHandler(void);             /**< HFXO0 IRQ Handler */
void HFRCO0_IRQHandler(void);            /**< HFRCO0 IRQ Handler */
void CMU_IRQHandler(void);               /**< CMU IRQ Handler */
void AES_IRQHandler(void);               /**< AES IRQ Handler */
void IADC_IRQHandler(void);              /**< IADC IRQ Handler */
void MSC_IRQHandler(void);               /**< MSC IRQ Handler */
void DPLL0_IRQHandler(void);             /**< DPLL0 IRQ Handler */
void PDM_IRQHandler(void);               /**< PDM IRQ Handler */
void SW0_IRQHandler(void);               /**< SW0 IRQ Handler */
void SW1_IRQHandler(void);               /**< SW1 IRQ Handler */
void SW2_IRQHandler(void);               /**< SW2 IRQ Handler */
void SW3_IRQHandler(void);               /**< SW3 IRQ Handler */
void KERNEL0_IRQHandler(void);           /**< KERNEL0 IRQ Handler */
void KERNEL1_IRQHandler(void);           /**< KERNEL1 IRQ Handler */
void M33CTI0_IRQHandler(void);           /**< M33CTI0 IRQ Handler */
void M33CTI1_IRQHandler(void);           /**< M33CTI1 IRQ Handler */
void EMUEFP_IRQHandler(void);            /**< EMUEFP IRQ Handler */
void DCDC_IRQHandler(void);              /**< DCDC IRQ Handler */
void EUART0_RX_IRQHandler(void);         /**< EUART0_RX IRQ Handler */
void EUART0_TX_IRQHandler(void);         /**< EUART0_TX IRQ Handler */

#if (__FPU_PRESENT == 1)
void FPUEH_IRQHandler(void);        /**< FPU IRQ Handler */
#endif

SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemHCLKGet(void);

/**************************************************************************//**
 * @brief
 *   Update CMSIS SystemCoreClock variable.
 *
 * @details
 *   CMSIS defines a global variable SystemCoreClock that shall hold the
 *   core frequency in Hz. If the core frequency is dynamically changed, the
 *   variable must be kept updated in order to be CMSIS compliant.
 *
 *   Notice that only if changing the core clock frequency through the EMLIB
 *   CMU API, this variable will be kept updated. This function is only
 *   provided for CMSIS compliance and if a user modifies the the core clock
 *   outside the EMLIB CMU API.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The `SystemCoreClockGet` function retrieves the current system core
 * clock frequency by calling `SystemHCLKGet`.
 *
 * @return The function returns a `uint32_t` value representing the current
 * system core clock frequency.
 ******************************************************************************/
static __INLINE uint32_t SystemCoreClockGet(void)
{
  return SystemHCLKGet();
}

/**************************************************************************//**
 * @brief
 *   Update CMSIS SystemCoreClock variable.
 *
 * @details
 *   CMSIS defines a global variable SystemCoreClock that shall hold the
 *   core frequency in Hz. If the core frequency is dynamically changed, the
 *   variable must be kept updated in order to be CMSIS compliant.
 *
 *   Notice that only if changing the core clock frequency through the EMLIB
 *   CMU API, this variable will be kept updated. This function is only
 *   provided for CMSIS compliance and if a user modifies the the core clock
 *   outside the EMLIB CMU API.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The `SystemCoreClockUpdate` function is intended to update the CMSIS
 * `SystemCoreClock` variable to reflect the current core clock
 * frequency.
 *
 * @return The function does not return any value or directly update the
 * `SystemCoreClock` variable; it relies on `SystemHCLKGet()` for any
 * necessary updates.
 ******************************************************************************/
static __INLINE void SystemCoreClockUpdate(void)
{
  SystemHCLKGet();
}

void     SystemInit(void);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemHFRCODPLLClockGet(void);
void     SystemHFRCODPLLClockSet(uint32_t freq);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemSYSCLKGet(void);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemMaxCoreClockGet(void);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemFSRCOClockGet(void);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemHFXOClockGet(void);
void     SystemHFXOClockSet(uint32_t freq);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemCLKIN0Get(void);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemLFXOClockGet(void);
void     SystemLFXOClockSet(uint32_t freq);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemLFRCOClockGet(void);
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_SYSTEM, SL_CODE_CLASS_TIME_CRITICAL)
uint32_t SystemULFRCOClockGet(void);

/** @} End of group */
/** @} End of group Parts */

#ifdef __cplusplus
}
#endif
#endif /* SYSTEM_EFR32BG22_H */
