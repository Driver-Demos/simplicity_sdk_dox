/**************************************************************************//**
 * @file
 * @brief CMSIS system header file for EFR32MG24
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

#ifndef SYSTEM_EFR32MG24_H
#define SYSTEM_EFR32MG24_H

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
 * @addtogroup EFR32MG24 EFR32MG24
 * @{
 ******************************************************************************/

/*******************************************************************************
 ******************************   TYPEDEFS   ***********************************
 ******************************************************************************/

/* Interrupt vectortable entry */
/***************************************************************************//**
 * @brief The `tVectorEntry` is a union data structure used to represent an
 * entry in an interrupt vector table. It can either hold a function
 * pointer, `VECTOR_TABLE_Type`, which points to an interrupt handler
 * function, or a `topOfStack` pointer, which points to the top of the
 * stack. This union allows for flexible representation of vector table
 * entries, accommodating both function pointers and stack pointers as
 * needed in system initialization and interrupt handling.
 *
 * @param VECTOR_TABLE_Type A function pointer to a vector table type function.
 * @param topOfStack A pointer to the top of the stack.
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
 * @brief This function is an interrupt handler for memory management faults,
 * typically triggered by violations of memory protection rules. It is
 * automatically invoked by the system when such a fault occurs. This
 * handler is part of the system's exception handling mechanism and is
 * crucial for debugging and maintaining system stability. It should be
 * used in systems where memory protection is enforced, and developers
 * should ensure that any necessary cleanup or logging is performed
 * within this handler to aid in diagnosing the cause of the fault.
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
void SMU_SECURE_IRQHandler(void);        /**< SMU_SECURE IRQ Handler */
void SMU_S_PRIVILEGED_IRQHandler(void);  /**< SMU_S_PRIVILEGED IRQ Handler */
void SMU_NS_PRIVILEGED_IRQHandler(void); /**< SMU_NS_PRIVILEGED IRQ Handler */
void EMU_IRQHandler(void);               /**< EMU IRQ Handler */
void TIMER0_IRQHandler(void);            /**< TIMER0 IRQ Handler */
void TIMER1_IRQHandler(void);            /**< TIMER1 IRQ Handler */
void TIMER2_IRQHandler(void);            /**< TIMER2 IRQ Handler */
void TIMER3_IRQHandler(void);            /**< TIMER3 IRQ Handler */
void TIMER4_IRQHandler(void);            /**< TIMER4 IRQ Handler */
void USART0_RX_IRQHandler(void);         /**< USART0_RX IRQ Handler */
void USART0_TX_IRQHandler(void);         /**< USART0_TX IRQ Handler */
void EUSART0_RX_IRQHandler(void);        /**< EUSART0_RX IRQ Handler */
void EUSART0_TX_IRQHandler(void);        /**< EUSART0_TX IRQ Handler */
void EUSART1_RX_IRQHandler(void);        /**< EUSART1_RX IRQ Handler */
void EUSART1_TX_IRQHandler(void);        /**< EUSART1_TX IRQ Handler */
void MVP_IRQHandler(void);               /**< MVP IRQ Handler */
/***************************************************************************//**
 * @brief This function is the interrupt handler for the Backup Real-Time
 * Counter (BURTC) on the EFR32MG24 microcontroller. It should be used to
 * process interrupts generated by the BURTC, such as overflow or compare
 * match events. The function reads the interrupt flags, processes the
 * relevant timer interrupts, and clears the handled interrupt flags. It
 * is typically called automatically by the system when a BURTC interrupt
 * occurs, and it operates within an atomic context to ensure interrupt
 * handling is not disrupted by other interrupts.
 *
 * @return None
 ******************************************************************************/
void BURTC_IRQHandler(void);             /**< BURTC IRQ Handler */
void LETIMER0_IRQHandler(void);          /**< LETIMER0 IRQ Handler */
void SYSCFG_IRQHandler(void);            /**< SYSCFG IRQ Handler */
void MPAHBRAM_IRQHandler(void);          /**< MPAHBRAM IRQ Handler */
void LDMA_IRQHandler(void);              /**< LDMA IRQ Handler */
void LFXO_IRQHandler(void);              /**< LFXO IRQ Handler */
void LFRCO_IRQHandler(void);             /**< LFRCO IRQ Handler */
/***************************************************************************//**
 * @brief This function is an interrupt handler specifically for odd-numbered
 * GPIO pins. It should be used in systems where GPIO interrupts are
 * enabled and need to be managed. The function retrieves all enabled and
 * pending odd GPIO interrupts, clears them, and then dispatches the
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
 * GPIO interrupts, clears them, and then dispatches the appropriate
 * interrupt handling routine. It is typically called automatically by
 * the system when an even GPIO interrupt occurs, and should not be
 * called directly by user code. Ensure that the system is configured to
 * handle GPIO interrupts before relying on this function.
 *
 * @return None
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void);         /**< GPIO_EVEN IRQ Handler */
void I2C0_IRQHandler(void);              /**< I2C0 IRQ Handler */
void I2C1_IRQHandler(void);              /**< I2C1 IRQ Handler */
void EMUDG_IRQHandler(void);             /**< EMUDG IRQ Handler */
void AGC_IRQHandler(void);               /**< AGC IRQ Handler */
void BUFC_IRQHandler(void);              /**< BUFC IRQ Handler */
void FRC_PRI_IRQHandler(void);           /**< FRC_PRI IRQ Handler */
void FRC_IRQHandler(void);               /**< FRC IRQ Handler */
void MODEM_IRQHandler(void);             /**< MODEM IRQ Handler */
void PROTIMER_IRQHandler(void);          /**< PROTIMER IRQ Handler */
void RAC_RSM_IRQHandler(void);           /**< RAC_RSM IRQ Handler */
void RAC_SEQ_IRQHandler(void);           /**< RAC_SEQ IRQ Handler */
void HOSTMAILBOX_IRQHandler(void);       /**< HOSTMAILBOX IRQ Handler */
void SYNTH_IRQHandler(void);             /**< SYNTH IRQ Handler */
void ACMP0_IRQHandler(void);             /**< ACMP0 IRQ Handler */
void ACMP1_IRQHandler(void);             /**< ACMP1 IRQ Handler */
void WDOG0_IRQHandler(void);             /**< WDOG0 IRQ Handler */
void WDOG1_IRQHandler(void);             /**< WDOG1 IRQ Handler */
void HFXO0_IRQHandler(void);             /**< HFXO0 IRQ Handler */
void HFRCO0_IRQHandler(void);            /**< HFRCO0 IRQ Handler */
void HFRCOEM23_IRQHandler(void);         /**< HFRCOEM23 IRQ Handler */
void CMU_IRQHandler(void);               /**< CMU IRQ Handler */
void AES_IRQHandler(void);               /**< AES IRQ Handler */
void IADC_IRQHandler(void);              /**< IADC IRQ Handler */
void MSC_IRQHandler(void);               /**< MSC IRQ Handler */
void DPLL0_IRQHandler(void);             /**< DPLL0 IRQ Handler */
void EMUEFP_IRQHandler(void);            /**< EMUEFP IRQ Handler */
void DCDC_IRQHandler(void);              /**< DCDC IRQ Handler */
void PCNT0_IRQHandler(void);             /**< PCNT0 IRQ Handler */
void SW0_IRQHandler(void);               /**< SW0 IRQ Handler */
void SW1_IRQHandler(void);               /**< SW1 IRQ Handler */
void SW2_IRQHandler(void);               /**< SW2 IRQ Handler */
void SW3_IRQHandler(void);               /**< SW3 IRQ Handler */
void KERNEL0_IRQHandler(void);           /**< KERNEL0 IRQ Handler */
void KERNEL1_IRQHandler(void);           /**< KERNEL1 IRQ Handler */
void M33CTI0_IRQHandler(void);           /**< M33CTI0 IRQ Handler */
void M33CTI1_IRQHandler(void);           /**< M33CTI1 IRQ Handler */
void FPUEXH_IRQHandler(void);            /**< FPUEXH IRQ Handler */
void SETAMPERHOST_IRQHandler(void);      /**< SETAMPERHOST IRQ Handler */
void SEMBRX_IRQHandler(void);            /**< SEMBRX IRQ Handler */
/***************************************************************************//**
 * @brief This function is an interrupt handler for the SYSRTC application,
 * responsible for processing overflow and compare interrupts. It should
 * be invoked automatically by the system when the corresponding
 * interrupt occurs. The function reads the interrupt flags, processes
 * the relevant events, and clears the interrupt flags to prevent re-
 * triggering. It operates within an atomic section to ensure thread
 * safety and prevent race conditions. This function is typically used in
 * embedded systems where precise timing and event handling are critical.
 *
 * @return None
 ******************************************************************************/
void SYSRTC_APP_IRQHandler(void);        /**< SYSRTC_APP IRQ Handler */
/***************************************************************************//**
 * @brief This function is an interrupt handler for the keyscan peripheral,
 * responsible for managing various keyscan-related interrupt events. It
 * should be invoked automatically by the system when a keyscan interrupt
 * occurs. The function checks for specific interrupt flags, such as
 * wake-up, key press, no key press, and scan completion, and processes
 * these events accordingly. It clears the interrupt flags after
 * processing to ensure that subsequent interrupts can be detected. This
 * function is typically used in embedded systems where keyscan
 * functionality is required, and it must be registered in the interrupt
 * vector table to be effective.
 *
 * @return None
 ******************************************************************************/
void KEYSCAN_IRQHandler(void);           /**< KEYSCAN IRQ Handler */
void RFECA0_IRQHandler(void);            /**< RFECA0 IRQ Handler */
void RFECA1_IRQHandler(void);            /**< RFECA1 IRQ Handler */
void VDAC0_IRQHandler(void);             /**< VDAC0 IRQ Handler */
void VDAC1_IRQHandler(void);             /**< VDAC1 IRQ Handler */
void AHB2AHB0_IRQHandler(void);          /**< AHB2AHB0 IRQ Handler */
void AHB2AHB1_IRQHandler(void);          /**< AHB2AHB1 IRQ Handler */

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
 * @brief The `SystemCoreClockUpdate` function updates the CMSIS SystemCoreClock
 * variable to reflect the current core clock frequency.
 *
 * @return The function does not return any value or modify any global state
 * directly; it relies on `SystemHCLKGet()` to perform necessary
 * updates.
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
/***************************************************************************//**
 * @brief This function returns the frequency of the HFRCOEM23 oscillator in
 * Hertz. It is useful for applications that need to know the current
 * operating frequency of this oscillator to adjust timing or performance
 * parameters. The function reads the oscillator's configuration to
 * determine the frequency band and clock division settings, and returns
 * the corresponding frequency. It should be called when the HFRCOEM23
 * oscillator is active and properly configured. The function does not
 * modify any input parameters or system state beyond reading the
 * oscillator configuration.
 *
 * @return Returns the frequency of the HFRCOEM23 oscillator in Hertz as a
 * uint32_t. If the configuration is invalid or unrecognized, it returns
 * 0.
 ******************************************************************************/
uint32_t SystemHFRCOEM23ClockGet(void);
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
#endif /* SYSTEM_EFR32MG24_H */
