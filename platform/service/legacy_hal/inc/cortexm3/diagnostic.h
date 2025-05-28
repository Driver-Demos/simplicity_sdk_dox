/***************************************************************************//**
 * @file
 * @brief See @ref diagnostics for detailed documentation.
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
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
 *  @{
 ******************************************************************************/

/** @addtogroup diagnostics Diagnostics
 * @brief Crash and watchdog diagnostic functions.
 *
 * See diagnostic.h for source code.
 *  @{
 */

#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

/// @brief Define the reset reasons that should print out detailed crash data.
#define RESET_CRASH_REASON_MASK ((1 << RESET_UNKNOWN)    \
                                 | (1 << RESET_WATCHDOG) \
                                 | (1 << RESET_CRASH)    \
                                 | (1 << RESET_FLASH)    \
                                 | (1 << RESET_FAULT)    \
                                 | (1 << RESET_FATAL))

/// @brief Define Hal assert information type
/***************************************************************************//**
 * @brief The `HalAssertInfoType` structure is used to store information about
 * an assertion failure in the code. It contains the file name and line
 * number where the assertion occurred, which can be used for debugging
 * purposes to identify the exact location of the failure in the source
 * code.
 *
 * @param file A pointer to a constant character string representing the file
 * name where the assertion occurred.
 * @param line An unsigned 32-bit integer representing the line number in the
 * file where the assertion occurred.
 ******************************************************************************/
typedef struct {
  const char * file;                               ///< file
  uint32_t     line;                               ///< line
} HalAssertInfoType;

/// @brief note that assertInfo and dmaProt are written just before a forced reboot
/***************************************************************************//**
 * @brief The `HalCrashSpecificDataType` is a union that encapsulates specific
 * data related to a crash event in a system. It can store either
 * assertion information, represented by the `HalAssertInfoType`
 * structure, which includes the file and line number where an assertion
 * failed, or DMA protection information, which includes a channel and
 * address. This union is used to provide additional context about the
 * crash, aiding in diagnostics and debugging.
 *
 * @param assertInfo Contains file and line information for an assertion.
 * @param dmaProt Contains channel and address information for DMA protection.
 ******************************************************************************/
typedef union {
  HalAssertInfoType assertInfo;                    ///< assertInfo
  struct { uint32_t channel; uint32_t address; } dmaProt;                                            ///< dmaProt
} HalCrashSpecificDataType;

/// @brief Define crash registers as structs so a debugger can display their bit fields
/***************************************************************************//**
 * @brief The `HalCrashxPsrType` is a union that encapsulates the processor
 * status register (xPSR) in a crash diagnostic context. It provides a
 * detailed view of the processor's state at the time of a crash by
 * allowing access to individual status bits through a structured bit
 * field, as well as the entire register as a single 32-bit word. This
 * dual representation facilitates both detailed analysis and efficient
 * data handling in crash diagnostics.
 *
 * @param bits A struct containing bit fields representing various processor
 * status flags and exception codes.
 * @param word A 32-bit unsigned integer representing the entire processor
 * status register as a single word.
 ******************************************************************************/
typedef union {
  struct {
    uint32_t EXCPT          : 9;  // B0-8
    uint32_t ICIIT_LOW      : 7;  // B9-15
    uint32_t                : 8;  // B16-23
    uint32_t T              : 1;  // B24
    uint32_t ICIIT_HIGH     : 2;  // B25-26
    uint32_t Q              : 1;  // B27
    uint32_t V              : 1;  // B28
    uint32_t C              : 1;  // B29
    uint32_t Z              : 1;  // B30
    uint32_t N              : 1;  // B31
  } bits;                                          ///< bits

  uint32_t word;                                   ///< word
} HalCrashxPsrType;

/// @brief  Define crash registers as structs so a debugger can display their bit fields
/***************************************************************************//**
 * @brief The `HalCrashIcsrType` is a union that represents the Interrupt
 * Control and State Register (ICSR) in a microcontroller, providing both
 * a bit-field structure for accessing individual status flags and a
 * 32-bit word for accessing the entire register. The bit fields include
 * flags for active and pending interrupts, return to base level, and
 * various pending set/clear states, which are crucial for diagnosing and
 * handling system crashes and interrupts.
 *
 * @param bits A struct containing bit fields representing various interrupt and
 * system state flags.
 * @param word A 32-bit unsigned integer representing the entire register as a
 * single value.
 ******************************************************************************/
typedef union {
  struct {
    uint32_t VECTACTIVE     : 9;  // B0-8
    uint32_t                : 2;  // B9-10
    uint32_t RETTOBASE      : 1;  // B11
    uint32_t VECTPENDING    : 9;  // B12-20
    uint32_t                : 1;  // B21
    uint32_t ISRPENDING     : 1;  // B22
    uint32_t ISRPREEMPT     : 1;  // B23
    uint32_t                : 1;  // B24
    uint32_t PENDSTCLR      : 1;  // B25
    uint32_t PENDSTSET      : 1;  // B26
    uint32_t PENDSVCLR      : 1;  // B27
    uint32_t PENDSVSET      : 1;  // B28
    uint32_t                : 2;  // B29-30
    uint32_t NMIPENDSET     : 1;  // B31
  } bits;                                          ///< bits

  uint32_t word;                                   ///< word
} HalCrashIcsrType;

/***************************************************************************//**
 * @brief The `HalCrashShcsrType` is a union that encapsulates the System
 * Handler Control and State Register (SHCSR) in a microcontroller,
 * providing a bit-field representation of various system fault and
 * exception states. It includes flags for memory management, bus, and
 * usage faults, as well as system service call and monitor activity,
 * among others. This structure allows for easy access and manipulation
 * of individual bits related to system fault handling, while also
 * providing a complete 32-bit view of the register.
 *
 * @param bits A struct containing bit fields representing various system
 * handler control and state flags.
 * @param word A 32-bit unsigned integer representing the entire union as a
 * single word.
 ******************************************************************************/
typedef union {
  struct {
#if defined (_SILICON_LABS_32B_SERIES_1_CONFIG_1)
    uint32_t EMU_IRQn         : 1;  // B0
    uint32_t FRC_PRI_IRQn     : 1;  // B1
    uint32_t WDOG0_IRQn       : 1;  // B2
    uint32_t FRC_IRQn         : 1;  // B3
    uint32_t MODEM_IRQn       : 1;  // B4
    uint32_t RAC_SEQ_IRQn     : 1;  // B5
    uint32_t RAC_RSM_IRQn     : 1;  // B6
    uint32_t BUFC_IRQn        : 1;  // B7
    uint32_t LDMA_IRQn        : 1;  // B8
    uint32_t GPIO_EVEN_IRQn   : 1;  // B9
    uint32_t TIMER0_IRQn      : 1;  // B10
    uint32_t USART0_RX_IRQn   : 1;  // B11
    uint32_t USART0_TX_IRQn   : 1;  // B12
    uint32_t ACMP0_IRQn       : 1;  // B13
    uint32_t ADC0_IRQn        : 1;  // B14
    uint32_t IDAC0_IRQn       : 1;  // B15
    uint32_t I2C0_IRQn        : 1;  // B16
    uint32_t GPIO_ODD_IRQn    : 1;  // B17
    uint32_t TIMER1_IRQn      : 1;  // B18
    uint32_t USART1_RX_IRQn   : 1;  // B19
    uint32_t USART1_TX_IRQn   : 1;  // B20
    uint32_t LEUART0_IRQn     : 1;  // B21
    uint32_t PCNT0_IRQn       : 1;  // B22
    uint32_t CMU_IRQn         : 1;  // B23
    uint32_t MSC_IRQn         : 1;  // B24
    uint32_t CRYPTO_IRQn      : 1;  // B25
    uint32_t LETIMER0_IRQn    : 1;  // B26
    uint32_t AGC_IRQn         : 1;  // B27
    uint32_t PROTIMER_IRQn    : 1;  // B28
    uint32_t RTCC_IRQn        : 1;  // B29
    uint32_t SYNTH_IRQn       : 1;  // B30
    uint32_t CRYOTIMER_IRQn   : 1;  // B31
    uint32_t RFSENSE_IRQn     : 1;  // B32
    uint32_t FPUEH_IRQn       : 1;  // B33
    uint32_t                : 30; // B34-63
  } bits;
  uint32_t word[2];
#elif defined (_SILICON_LABS_32B_SERIES_1_CONFIG_2)
    uint32_t EMU_IRQn         : 1;  // B0
    uint32_t FRC_PRI_IRQn     : 1;  // B1
    uint32_t WDOG0_IRQn       : 1;  // B2
    uint32_t WDOG1_IRQn       : 1;  // B3
    uint32_t FRC_IRQn         : 1;  // B4
    uint32_t MODEM_IRQn       : 1;  // B5
    uint32_t RAC_SEQ_IRQn     : 1;  // B6
    uint32_t RAC_RSM_IRQn     : 1;  // B7
    uint32_t BUFC_IRQn        : 1;  // B8
    uint32_t LDMA_IRQn        : 1;  // B9
    uint32_t GPIO_EVEN_IRQn   : 1;  // B10
    uint32_t TIMER0_IRQn      : 1;  // B11
    uint32_t USART0_RX_IRQn   : 1;  // B12
    uint32_t USART0_TX_IRQn   : 1;  // B13
    uint32_t ACMP0_IRQn       : 1;  // B14
    uint32_t ADC0_IRQn        : 1;  // B15
    uint32_t IDAC0_IRQn       : 1;  // B16
    uint32_t I2C0_IRQn        : 1;  // B17
    uint32_t GPIO_ODD_IRQn    : 1;  // B18
    uint32_t TIMER1_IRQn      : 1;  // B19
    uint32_t USART1_RX_IRQn   : 1;  // B20
    uint32_t USART1_TX_IRQn   : 1;  // B21
    uint32_t LEUART0_IRQn     : 1;  // B22
    uint32_t PCNT0_IRQn       : 1;  // B23
    uint32_t CMU_IRQn         : 1;  // B24
    uint32_t MSC_IRQn         : 1;  // B25
    uint32_t CRYPTO0_IRQn     : 1;  // B26
    uint32_t LETIMER0_IRQn    : 1;  // B27
    uint32_t AGC_IRQn         : 1;  // B28
    uint32_t PROTIMER_IRQn    : 1;  // B29
    uint32_t RTCC_IRQn        : 1;  // B30
    uint32_t SYNTH_IRQn       : 1;  // B31
    uint32_t CRYOTIMER_IRQn   : 1;  // B32
    uint32_t RFSENSE_IRQn     : 1;  // B33
    uint32_t FPUEH_IRQn       : 1;  // B34
    uint32_t SMU_IRQn         : 1;  // B35
    uint32_t WTIMER0_IRQn     : 1;  // B36
    uint32_t WTIMER1_IRQn     : 1;  // B37
    uint32_t PCNT1_IRQn       : 1;  // B38
    uint32_t PCNT2_IRQn       : 1;  // B39
    uint32_t USART2_RX_IRQn   : 1;  // B40
    uint32_t USART2_TX_IRQn   : 1;  // B41
    uint32_t I2C1_IRQn        : 1;  // B42
    uint32_t USART3_RX_IRQn   : 1;  // B43
    uint32_t USART3_TX_IRQn   : 1;  // B44
    uint32_t VDAC0_IRQn       : 1;  // B45
    uint32_t CSEN_IRQn        : 1;  // B46
    uint32_t LESENSE_IRQn     : 1;  // B47
    uint32_t CRYPTO1_IRQn     : 1;  // B48
    uint32_t TRNG0_IRQn       : 1;  // B49
    uint32_t                : 14; // B50-63
  } bits;
  uint32_t word[2];
#elif defined (_SILICON_LABS_32B_SERIES_1_CONFIG_3)
    uint32_t EMU_IRQn         : 1;  // B0
    uint32_t FRC_PRI_IRQn     : 1;  // B1
    uint32_t WDOG0_IRQn       : 1;  // B2
    uint32_t WDOG1_IRQn       : 1;  // B3
    uint32_t FRC_IRQn         : 1;  // B4
    uint32_t MODEM_IRQn       : 1;  // B5
    uint32_t RAC_SEQ_IRQn     : 1;  // B6
    uint32_t RAC_RSM_IRQn     : 1;  // B7
    uint32_t BUFC_IRQn        : 1;  // B8
    uint32_t LDMA_IRQn        : 1;  // B9
    uint32_t GPIO_EVEN_IRQn   : 1;  // B10
    uint32_t TIMER0_IRQn      : 1;  // B11
    uint32_t USART0_RX_IRQn   : 1;  // B12
    uint32_t USART0_TX_IRQn   : 1;  // B13
    uint32_t ACMP0_IRQn       : 1;  // B14
    uint32_t ADC0_IRQn        : 1;  // B15
    uint32_t IDAC0_IRQn       : 1;  // B16
    uint32_t I2C0_IRQn        : 1;  // B17
    uint32_t GPIO_ODD_IRQn    : 1;  // B18
    uint32_t TIMER1_IRQn      : 1;  // B19
    uint32_t USART1_RX_IRQn   : 1;  // B20
    uint32_t USART1_TX_IRQn   : 1;  // B21
    uint32_t LEUART0_IRQn     : 1;  // B22
    uint32_t PCNT0_IRQn       : 1;  // B23
    uint32_t CMU_IRQn         : 1;  // B24
    uint32_t MSC_IRQn         : 1;  // B25
    uint32_t CRYPTO0_IRQn     : 1;  // B26
    uint32_t LETIMER0_IRQn    : 1;  // B27
    uint32_t AGC_IRQn         : 1;  // B28
    uint32_t PROTIMER_IRQn    : 1;  // B29
    uint32_t PRORTC_IRQn      : 1;  // B30
    uint32_t RTCC_IRQn        : 1;  // B31
    uint32_t SYNTH_IRQn       : 1;  // B32
    uint32_t CRYOTIMER_IRQn   : 1;  // B33
    uint32_t RFSENSE_IRQn     : 1;  // B34
    uint32_t FPUEH_IRQn       : 1;  // B35
    uint32_t SMU_IRQn         : 1;  // B36
    uint32_t WTIMER0_IRQn     : 1;  // B37
    uint32_t USART2_RX_IRQn   : 1;  // B38
    uint32_t USART2_TX_IRQn   : 1;  // B39
    uint32_t I2C1_IRQn        : 1;  // B40
    uint32_t VDAC0_IRQn       : 1;  // B41
    uint32_t CSEN_IRQn        : 1;  // B42
    uint32_t LESENSE_IRQn     : 1;  // B43
    uint32_t CRYPTO1_IRQn     : 1;  // B44
    uint32_t TRNG0_IRQn       : 1;  // B45
    uint32_t                : 18; // B46-63
  } bits;
  uint32_t word[2];
#elif defined (_SILICON_LABS_32B_SERIES_1_CONFIG_4)
    uint32_t EMU_IRQn         : 1;  // B0
    uint32_t FRC_PRI_IRQn     : 1;  // B1
    uint32_t WDOG0_IRQn       : 1;  // B2
    uint32_t WDOG1_IRQn       : 1;  // B3
    uint32_t FRC_IRQn         : 1;  // B4
    uint32_t MODEM_IRQn       : 1;  // B5
    uint32_t RAC_SEQ_IRQn     : 1;  // B6
    uint32_t RAC_RSM_IRQn     : 1;  // B7
    uint32_t BUFC_IRQn        : 1;  // B8
    uint32_t LDMA_IRQn        : 1;  // B9
    uint32_t GPIO_EVEN_IRQn   : 1;  // B10
    uint32_t TIMER0_IRQn      : 1;  // B11
    uint32_t USART0_RX_IRQn   : 1;  // B12
    uint32_t USART0_TX_IRQn   : 1;  // B13
    uint32_t ACMP0_IRQn       : 1;  // B14
    uint32_t ADC0_IRQn        : 1;  // B15
    uint32_t IDAC0_IRQn       : 1;  // B16
    uint32_t I2C0_IRQn        : 1;  // B17
    uint32_t GPIO_ODD_IRQn    : 1;  // B18
    uint32_t TIMER1_IRQn      : 1;  // B19
    uint32_t USART1_RX_IRQn   : 1;  // B20
    uint32_t USART1_TX_IRQn   : 1;  // B21
    uint32_t LEUART0_IRQn     : 1;  // B22
    uint32_t PCNT0_IRQn       : 1;  // B23
    uint32_t CMU_IRQn         : 1;  // B24
    uint32_t MSC_IRQn         : 1;  // B25
    uint32_t CRYPTO0_IRQn     : 1;  // B26
    uint32_t LETIMER0_IRQn    : 1;  // B27
    uint32_t AGC_IRQn         : 1;  // B28
    uint32_t PROTIMER_IRQn    : 1;  // B29
    uint32_t PRORTC_IRQn      : 1;  // B30
    uint32_t RTCC_IRQn        : 1;  // B31
    uint32_t SYNTH_IRQn       : 1;  // B32
    uint32_t CRYOTIMER_IRQn   : 1;  // B33
    uint32_t RFSENSE_IRQn     : 1;  // B34
    uint32_t FPUEH_IRQn       : 1;  // B35
    uint32_t SMU_IRQn         : 1;  // B36
    uint32_t WTIMER0_IRQn     : 1;  // B37
    uint32_t VDAC0_IRQn       : 1;  // B38
    uint32_t LESENSE_IRQn     : 1;  // B39
    uint32_t TRNG0_IRQn       : 1;  // B40
    uint32_t SYSCFG_IRQn      : 1;  // B41
    uint32_t                : 22; // B42-63
  } bits;
  uint32_t word[2];
#elif defined (_SILICON_LABS_32B_SERIES_2_CONFIG_1)
    uint32_t SETAMPERHOST_IRQn     : 1;  // B0
    uint32_t SEMBRX_IRQn           : 1;  // B1
    uint32_t SEMBTX_IRQn           : 1;  // B2
    uint32_t SMU_SECURE_IRQn       : 1;  // B3
    uint32_t SMU_PRIVILEGED_IRQn   : 1;  // B4
    uint32_t EMU_IRQn              : 1;  // B5
    uint32_t TIMER0_IRQn           : 1;  // B6
    uint32_t TIMER1_IRQn           : 1;  // B7
    uint32_t TIMER2_IRQn           : 1;  // B8
    uint32_t TIMER3_IRQn           : 1;  // B9
    uint32_t RTCC_IRQn             : 1;  // B10
    uint32_t USART0_RX_IRQn        : 1;  // B11
    uint32_t USART0_TX_IRQn        : 1;  // B12
    uint32_t USART1_RX_IRQn        : 1;  // B13
    uint32_t USART1_TX_IRQn        : 1;  // B14
    uint32_t USART2_RX_IRQn        : 1;  // B15
    uint32_t USART2_TX_IRQn        : 1;  // B16
    uint32_t ICACHE0_IRQn          : 1;  // B17
    uint32_t BURTC_IRQn            : 1;  // B18
    uint32_t LETIMER0_IRQn         : 1;  // B19
    uint32_t SYSCFG_IRQn           : 1;  // B20
    uint32_t LDMA_IRQn             : 1;  // B21
    uint32_t LFXO_IRQn             : 1;  // B22
    uint32_t LFRCO_IRQn            : 1;  // B23
    uint32_t ULFRCO_IRQn           : 1;  // B24
    uint32_t GPIO_ODD_IRQn         : 1;  // B25
    uint32_t GPIO_EVEN_IRQn        : 1;  // B26
    uint32_t I2C0_IRQn             : 1;  // B27
    uint32_t I2C1_IRQn             : 1;  // B28
    uint32_t EMUDG_IRQn            : 1;  // B29
    uint32_t EMUSE_IRQn            : 1;  // B30
    uint32_t AGC_IRQn              : 1;  // B31
    uint32_t BUFC_IRQn             : 1;  // B32
    uint32_t FRC_PRI_IRQn          : 1;  // B33
    uint32_t FRC_IRQn              : 1;  // B34
    uint32_t MODEM_IRQn            : 1;  // B35
    uint32_t PROTIMER_IRQn         : 1;  // B36
    uint32_t RAC_RSM_IRQn          : 1;  // B37
    uint32_t RAC_SEQ_IRQn          : 1;  // B38
    uint32_t PRORTC_IRQn           : 1;  // B39
    uint32_t SYNTH_IRQn            : 1;  // B40
    uint32_t ACMP0_IRQn            : 1;  // B41
    uint32_t ACMP1_IRQn            : 1;  // B42
    uint32_t WDOG0_IRQn            : 1;  // B43
    uint32_t WDOG1_IRQn            : 1;  // B44
    uint32_t HFXO00_IRQn           : 1;  // B45
    uint32_t HFRCO0_IRQn           : 1;  // B46
    uint32_t HFRCOEM23_IRQn        : 1;  // B47
    uint32_t CMU_IRQn              : 1;  // B48
    uint32_t AES_IRQn              : 1;  // B49
    uint32_t IADC_IRQn             : 1;  // B50
    uint32_t MSC_IRQn              : 1;  // B51
    uint32_t DPLL0_IRQn            : 1;  // B52
    uint32_t SW0_IRQn              : 1;  // B53
    uint32_t SW1_IRQn              : 1;  // B54
    uint32_t SW2_IRQn              : 1;  // B55
    uint32_t SW3_IRQn              : 1;  // B56
    uint32_t KERNEL0_IRQn          : 1;  // B57
    uint32_t KERNEL1_IRQn          : 1;  // B58
    uint32_t M33CTI0_IRQn          : 1;  // B59
    uint32_t M33CTI1_IRQn          : 1;  // B60
    uint32_t                       : 3;  // B61-63
  } bits;
  uint32_t word[2];
#elif defined (_SILICON_LABS_32B_SERIES_2_CONFIG_2)
    uint32_t CRYPTOACC_IRQn         : 1;  // B0
    uint32_t TRNG_IRQn              : 1;  // B1
    uint32_t PKE_IRQn               : 1;  // B2
    uint32_t SMU_SECURE_IRQn        : 1;  // B3
    uint32_t SMU_S_PRIVILEGED_IRQn  : 1;  // B4
    uint32_t SMU_NS_PRIVILEGED_IRQn : 1;  // B5
    uint32_t EMU_IRQn               : 1;  // B6
    uint32_t TIMER0_IRQn            : 1;  // B7
    uint32_t TIMER1_IRQn            : 1;  // B8
    uint32_t TIMER2_IRQn            : 1;  // B9
    uint32_t TIMER3_IRQn            : 1;  // B10
    uint32_t TIMER4_IRQn            : 1;  // B11
    uint32_t RTCC_IRQn              : 1;  // B12
    uint32_t USART0_RX_IRQn         : 1;  // B13
    uint32_t USART0_TX_IRQn         : 1;  // B14
    uint32_t USART1_RX_IRQn         : 1;  // B15
    uint32_t USART1_TX_IRQn         : 1;  // B16
    uint32_t ICACHE0_IRQn           : 1;  // B17
    uint32_t BURTC_IRQn             : 1;  // B18
    uint32_t LETIMER0_IRQn          : 1;  // B19
    uint32_t SYSCFG_IRQn            : 1;  // B20
    uint32_t LDMA_IRQn              : 1;  // B21
    uint32_t LFXO_IRQn              : 1;  // B22
    uint32_t LFRCO_IRQn             : 1;  // B23
    uint32_t ULFRCO_IRQn            : 1;  // B24
    uint32_t GPIO_ODD_IRQn          : 1;  // B25
    uint32_t GPIO_EVEN_IRQn         : 1;  // B26
    uint32_t I2C0_IRQn              : 1;  // B27
    uint32_t I2C1_IRQn              : 1;  // B28
    uint32_t EMUDG_IRQn             : 1;  // B29
    uint32_t EMUSE_IRQn             : 1;  // B30
    uint32_t AGC_IRQn               : 1;  // B31
    uint32_t BUFC_IRQn              : 1;  // B32
    uint32_t FRC_PRI_IRQn           : 1;  // B33
    uint32_t FRC_IRQn               : 1;  // B34
    uint32_t MODEM_IRQn             : 1;  // B35
    uint32_t PROTIMER_IRQn          : 1;  // B36
    uint32_t RAC_RSM_IRQn           : 1;  // B37
    uint32_t RAC_SEQ_IRQn           : 1;  // B38
    uint32_t RDMAILBOX_IRQn         : 1;  // B39
    uint32_t RFSENSE_IRQn           : 1;  // B40
    uint32_t PRORTC_IRQn            : 1;  // B41
    uint32_t SYNTH_IRQn             : 1;  // B42
    uint32_t WDOG0_IRQn             : 1;  // B43
    uint32_t HFXO0_IRQn             : 1;  // B44
    uint32_t HFRCO0_IRQn            : 1;  // B45
    uint32_t CMU_IRQn               : 1;  // B46
    uint32_t AES_IRQn               : 1;  // B47
    uint32_t IADC_IRQn              : 1;  // B48
    uint32_t MSC_IRQn               : 1;  // B49
    uint32_t DPLL0_IRQn             : 1;  // B50
    uint32_t PDM_IRQn               : 1;  // B51
    uint32_t SW0_IRQn               : 1;  // B52
    uint32_t SW1_IRQn               : 1;  // B53
    uint32_t SW2_IRQn               : 1;  // B54
    uint32_t SW3_IRQn               : 1;  // B55
    uint32_t KERNEL0_IRQn           : 1;  // B56
    uint32_t KERNEL1_IRQn           : 1;  // B57
    uint32_t M33CTI0_IRQn           : 1;  // B58
    uint32_t M33CTI1_IRQn           : 1;  // B59
    uint32_t EMUEFP_IRQn            : 1;  // B60
    uint32_t DCDC_IRQn              : 1;  // B61
    uint32_t EUART0_RX_IRQn         : 1;  // B62
    uint32_t EUART0_TX_IRQn         : 1;  // B63
  } bits;
  uint32_t word[2];
#elif defined(_SILICON_LABS_32B_SERIES_2_CONFIG_3)
    uint32_t SMU_SECURE_IRQn        : 1; // B0
    uint32_t SMU_S_PRIVILEGED_IRQn  : 1; // B1
    uint32_t SMU_NS_PRIVILEGED_IRQn : 1; // B2
    uint32_t EMU_IRQn               : 1; // B3
    uint32_t TIMER0_IRQn            : 1; // B4
    uint32_t TIMER1_IRQn            : 1; // B5
    uint32_t TIMER2_IRQn            : 1; // B6
    uint32_t TIMER3_IRQn            : 1; // B7
    uint32_t TIMER4_IRQn            : 1; // B8
    uint32_t USART0_RX_IRQn         : 1; // B9
    uint32_t USART0_TX_IRQn         : 1; // B10
    uint32_t EUSART0_RX_IRQn        : 1; // B11
    uint32_t EUSART0_TX_IRQn        : 1; // B12
    uint32_t EUSART1_RX_IRQn        : 1; // B13
    uint32_t EUSART1_TX_IRQn        : 1; // B14
    uint32_t EUSART2_RX_IRQn        : 1; // B15
    uint32_t EUSART2_TX_IRQn        : 1; // B16
    uint32_t ICACHE0_IRQn           : 1; // B17
    uint32_t BURTC_IRQn             : 1; // B18
    uint32_t LETIMER0_IRQn          : 1; // B19
    uint32_t SYSCFG_IRQn            : 1; // B20
    uint32_t MPAHBRAM_IRQn          : 1; // B21
    uint32_t LDMA_IRQn              : 1; // B22
    uint32_t LFXO_IRQn              : 1; // B23
    uint32_t LFRCO_IRQn             : 1; // B24
    uint32_t ULFRCO_IRQn            : 1; // B25
    uint32_t GPIO_ODD_IRQn          : 1; // B26
    uint32_t GPIO_EVEN_IRQn         : 1; // B27
    uint32_t I2C0_IRQn              : 1; // B28
    uint32_t I2C1_IRQn              : 1; // B29
    uint32_t EMUDG_IRQn             : 1; // B30
    uint32_t AGC_IRQn               : 1; // B31
    uint32_t BUFC_IRQn              : 1; // B32
    uint32_t FRC_PRI_IRQn           : 1; // B33
    uint32_t FRC_IRQn               : 1; // B34
    uint32_t MODEM_IRQn             : 1; // B35
    uint32_t PROTIMER_IRQn          : 1; // B36
    uint32_t RAC_RSM_IRQn           : 1; // B37
    uint32_t RAC_SEQ_IRQn           : 1; // B38
    uint32_t HOSTMAILBOX_IRQn       : 1; // B39
    uint32_t SYNTH_IRQn             : 1; // B40
    uint32_t ACMP0_IRQn             : 1; // B41
    uint32_t ACMP1_IRQn             : 1; // B42
    uint32_t WDOG0_IRQn             : 1; // B43
    uint32_t WDOG1_IRQn             : 1; // B44
    uint32_t HFXO0_IRQn             : 1; // B45
    uint32_t HFRCO0_IRQn            : 1; // B46
    uint32_t HFRCOEM23_IRQn         : 1; // B47
    uint32_t CMU_IRQn               : 1; // B48
    uint32_t AES_IRQn               : 1; // B49
    uint32_t IADC_IRQn              : 1; // B50
    uint32_t MSC_IRQn               : 1; // B51
    uint32_t DPLL0_IRQn             : 1; // B52
    uint32_t EMUEFP_IRQn            : 1; // B53
    uint32_t DCDC_IRQn              : 1; // B54
    uint32_t VDAC_IRQn              : 1; // B55
    uint32_t PCNT0_IRQn             : 1; // B56
    uint32_t SW0_IRQn               : 1; // B57
    uint32_t SW1_IRQn               : 1; // B58
    uint32_t SW2_IRQn               : 1; // B59
    uint32_t SW3_IRQn               : 1; // B60
    uint32_t KERNEL0_IRQn           : 1; // B61
    uint32_t KERNEL1_IRQn           : 1; // B62
    uint32_t M33CTI0_IRQn           : 1; // B63
    uint32_t M33CTI1_IRQn           : 1; // B64
    uint32_t FPUEXH_IRQn            : 1; // B65
    uint32_t SEMBRX_IRQn            : 1; // B67
    uint32_t SEMBTX_IRQn            : 1; // B68
    uint32_t LESENSE_IRQn           : 1; // B69
    uint32_t SYSRTC_APP_IRQn        : 1; // B70
    uint32_t SYSRTC_SEQ_IRQn        : 1; // B71
    uint32_t LCD_IRQn               : 1; // B72
    uint32_t KEYSCAN_IRQn           : 1; // B73
    uint32_t RFECA0_IRQn            : 1; // B74
    uint32_t RFECA1_IRQn            : 1; // B75
    uint32_t                : 20; // B76-95
  } bits;
  uint32_t word[3];
#elif defined (_SILICON_LABS_32B_SERIES_2_CONFIG_4)
    uint32_t SMU_SECURE_IRQn        : 1;  // B0
    uint32_t SMU_S_PRIVILEGED_IRQn  : 1;  // B1
    uint32_t SMU_NS_PRIVILEGED_IRQn : 1;  // B2
    uint32_t EMU_IRQn               : 1;  // B3
    uint32_t TIMER0_IRQn            : 1;  // B4
    uint32_t TIMER1_IRQn            : 1;  // B5
    uint32_t TIMER2_IRQn            : 1;  // B6
    uint32_t TIMER3_IRQn            : 1;  // B7
    uint32_t TIMER4_IRQn            : 1;  // B8
    uint32_t USART0_RX_IRQn         : 1;  // B9
    uint32_t USART0_TX_IRQn         : 1;  // B10
    uint32_t EUSART0_RX_IRQn        : 1;  // B11
    uint32_t EUSART0_TX_IRQn        : 1;  // B12
    uint32_t EUSART1_RX_IRQn        : 1;  // B13
    uint32_t EUSART1_TX_IRQn        : 1;  // B14
    uint32_t MVP_IRQn               : 1;  // B15
    uint32_t ICACHE0_IRQn           : 1;  // B16
    uint32_t BURTC_IRQn             : 1;  // B17
    uint32_t LETIMER0_IRQn          : 1;  // B18
    uint32_t SYSCFG_IRQn            : 1;  // B19
    uint32_t MPAHBRAM_IRQn          : 1;  // B20
    uint32_t LDMA_IRQn              : 1;  // B21
    uint32_t LFXO_IRQn              : 1;  // B22
    uint32_t LFRCO_IRQn             : 1;  // B23
    uint32_t ULFRCO_IRQn            : 1;  // B24
    uint32_t GPIO_ODD_IRQn          : 1;  // B25
    uint32_t GPIO_EVEN_IRQn         : 1;  // B26
    uint32_t I2C0_IRQn              : 1;  // B27
    uint32_t I2C1_IRQn              : 1;  // B28
    uint32_t EMUDG_IRQn             : 1;  // B29
    uint32_t AGC_IRQn               : 1;  // B30
    uint32_t BUFC_IRQn              : 1;  // B31
    uint32_t FRC_PRI_IRQn           : 1;  // B32
    uint32_t FRC_IRQn               : 1;  // B33
    uint32_t MODEM_IRQn             : 1;  // B34
    uint32_t PROTIMER_IRQn          : 1;  // B35
    uint32_t RAC_RSM_IRQn           : 1;  // B36
    uint32_t RAC_SEQ_IRQn           : 1;  // B37
    uint32_t HOSTMAILBOX_IRQn       : 1;  // B38
    uint32_t SYNTH_IRQn             : 1;  // B39
    uint32_t AHBSRW_BUS_ERR_IRQn    : 1;  // B40
    uint32_t ACMP0_IRQn             : 1;  // B41
    uint32_t ACMP1_IRQn             : 1;  // B42
    uint32_t WDOG0_IRQn             : 1;  // B43
    uint32_t WDOG1_IRQn             : 1;  // B44
    uint32_t SYXO0_IRQn             : 1;  // B45
    uint32_t HFRCO0_IRQn            : 1;  // B46
    uint32_t HFRCOEM23_IRQn         : 1;  // B47
    uint32_t CMU_IRQn               : 1;  // B48
    uint32_t AES_IRQn               : 1;  // B49
    uint32_t IADC_IRQn              : 1;  // B50
    uint32_t MSC_IRQn               : 1;  // B51
    uint32_t DPLL0_IRQn             : 1;  // B52
    uint32_t EMUEFP_IRQn            : 1;  // B53
    uint32_t DCDC_IRQn              : 1;  // B54
    uint32_t PCNT0_IRQn             : 1;  // B55
    uint32_t SW0_IRQn               : 1;  // B56
    uint32_t SW1_IRQn               : 1;  // B57
    uint32_t SW2_IRQn               : 1;  // B58
    uint32_t SW3_IRQn               : 1;  // B59
    uint32_t KERNEL0_IRQn           : 1;  // B60
    uint32_t KERNEL1_IRQn           : 1;  // B61
    uint32_t M33CTI0_IRQn           : 1;  // B62
    uint32_t M33CTI1_IRQn           : 1;  // B63
    uint32_t FPUEXH_IRQn            : 1;  // B64
    uint32_t SETAMPERHOST_IRQn      : 1;  // B65
    uint32_t SEMBRX_IRQn            : 1;  // B66
    uint32_t SEMBTX_IRQn            : 1;  // B67
    uint32_t SYSRTC_APP_IRQn        : 1;  // B68
    uint32_t SYSRTC_SEQ_IRQn        : 1;  // B69
    uint32_t KEYSCAN_IRQn           : 1;  // B70
    uint32_t RFECA0_IRQn            : 1;  // B71
    uint32_t RFECA1_IRQn            : 1;  // B72
    uint32_t VDAC0_IRQn             : 1;  // B73
    uint32_t VDAC1_IRQn             : 1;  // B74
    uint32_t                : 21; // B75-95
  } bits;
  uint32_t word[3];
#elif defined (_SILICON_LABS_32B_SERIES_2_CONFIG_6)
    uint32_t SMU_SECURE_IRQn        : 1;  // B0
    uint32_t SMU_S_PRIVILEGED_IRQn  : 1;  // B1
    uint32_t SMU_NS_PRIVILEGED_IRQn : 1;  // B2
    uint32_t EMU_IRQn               : 1;  // B3
    uint32_t TIMER0_IRQn            : 1;  // B4
    uint32_t TIMER1_IRQn            : 1;  // B5
    uint32_t TIMER2_IRQn            : 1;  // B6
    uint32_t TIMER3_IRQn            : 1;  // B7
    uint32_t TIMER4_IRQn            : 1;  // B8
    uint32_t TIMER5_IRQn            : 1;  // B9
    uint32_t TIMER6_IRQn            : 1;  // B10
    uint32_t TIMER7_IRQn            : 1;  // B11
    uint32_t TIMER8_IRQn            : 1;  // B12
    uint32_t TIMER9_IRQn            : 1;  // B13
    uint32_t USART0_RX_IRQn         : 1;  // B14
    uint32_t USART0_TX_IRQn         : 1;  // B15
    uint32_t USART1_RX_IRQn         : 1;  // B16
    uint32_t USART1_TX_IRQn         : 1;  // B17
    uint32_t USART2_RX_IRQn         : 1;  // B18
    uint32_t USART2_TX_IRQn         : 1;  // B19
    uint32_t EUSART0_RX_IRQn        : 1;  // B20
    uint32_t EUSART0_TX_IRQn        : 1;  // B21
    uint32_t EUSART1_RX_IRQn        : 1;  // B22
    uint32_t EUSART1_TX_IRQn        : 1;  // B23
    uint32_t EUSART2_RX_IRQn        : 1;  // B24
    uint32_t EUSART2_TX_IRQn        : 1;  // B25
    uint32_t EUSART3_RX_IRQn        : 1;  // B26
    uint32_t EUSART3_TX_IRQn        : 1;  // B27
    uint32_t MVP_IRQn               : 1;  // B28
    uint32_t ICACHE0_IRQn           : 1;  // B29
    uint32_t BURTC_IRQn             : 1;  // B30
    uint32_t LETIMER0_IRQn          : 1;  // B31
    uint32_t SYSCFG_IRQn            : 1;  // B32
    uint32_t MPAHBRAM0_IRQn         : 1;  // B33
    uint32_t MPAHBRAM1_IRQn         : 1;  // B34
    uint32_t LDMA_IRQn              : 1;  // B35
    uint32_t LFXO_IRQn              : 1;  // B36
    uint32_t LFRCO_IRQn             : 1;  // B37
    uint32_t ULFRCO_IRQn            : 1;  // B38
    uint32_t GPIO_ODD_IRQn          : 1;  // B39
    uint32_t GPIO_EVEN_IRQn         : 1;  // B40
    uint32_t I2C0_IRQn              : 1;  // B41
    uint32_t I2C1_IRQn              : 1;  // B42
    uint32_t I2C2_IRQn              : 1;  // B43
    uint32_t I2C3_IRQn              : 1;  // B44
    uint32_t EMUDG_IRQn             : 1;  // B45
    uint32_t AGC_IRQn               : 1;  // B46
    uint32_t BUFC_IRQn              : 1;  // B47
    uint32_t FRC_PRI_IRQn           : 1;  // B48
    uint32_t FRC_IRQn               : 1;  // B49
    uint32_t MODEM_IRQn             : 1;  // B50
    uint32_t PROTIMER_IRQn          : 1;  // B51
    uint32_t RAC_RSM_IRQn           : 1;  // B52
    uint32_t RAC_SEQ_IRQn           : 1;  // B53
    uint32_t HOSTMAILBOX_IRQn       : 1;  // B54
    uint32_t SYNTH_IRQn             : 1;  // B55
    uint32_t ACMP0_IRQn             : 1;  // B56
    uint32_t ACMP1_IRQn             : 1;  // B57
    uint32_t WDOG0_IRQn             : 1;  // B58
    uint32_t WDOG1_IRQn             : 1;  // B59
    uint32_t HFXO0_IRQn             : 1;  // B60
    uint32_t HFRCO0_IRQn            : 1;  // B61
    uint32_t HFRCOEM23_IRQn         : 1;  // B62
    uint32_t CMU_IRQn               : 1;  // B63
    uint32_t AES_IRQn               : 1;  // B64
    uint32_t IADC_IRQn              : 1;  // B65
    uint32_t MSC_IRQn               : 1;  // B66
    uint32_t DPLL0_IRQn             : 1;  // B67
    uint32_t EMUEFP_IRQn            : 1;  // B68
    uint32_t DCDC_IRQn              : 1;  // B69
    uint32_t PCNT0_IRQn             : 1;  // B70
    uint32_t SW0_IRQn               : 1;  // B71
    uint32_t SW1_IRQn               : 1;  // B72
    uint32_t SW2_IRQn               : 1;  // B73
    uint32_t SW3_IRQn               : 1;  // B74
    uint32_t KERNEL0_IRQn           : 1;  // B75
    uint32_t KERNEL1_IRQn           : 1;  // B76
    uint32_t M33CTI0_IRQn           : 1;  // B77
    uint32_t M33CTI1_IRQn           : 1;  // B78
    uint32_t FPUEXH_IRQn            : 1;  // B79
    uint32_t SETAMPERHOST_IRQn      : 1;  // B80
    uint32_t SEMBRX_IRQn            : 1;  // B81
    uint32_t SEMBTX_IRQn            : 1;  // B82
    uint32_t SYSRTC_APP_IRQn        : 1;  // B83
    uint32_t SYSRTC_SEQ_IRQn        : 1;  // B84
    uint32_t KEYSCAN_IRQn           : 1;  // B85
    uint32_t RFECA0_IRQn            : 1;  // B86
    uint32_t RFECA1_IRQn            : 1;  // B87
    uint32_t VDAC0_IRQn             : 1;  // B88
    uint32_t VDAC1_IRQn             : 1;  // B89
    uint32_t AHB2AHB0_IRQn          : 1;  // B90
    uint32_t AHB2AHB1_IRQn          : 1;  // B91
    uint32_t LCD_IRQn               : 1;  // B92
    uint32_t                : 3; // B93-95
  } bits;
  uint32_t word[3];
#elif defined (_SILICON_LABS_32B_SERIES_2_CONFIG_7)
    uint32_t CRYPTOACC_IRQn         : 1;  // B0
    uint32_t TRNG_IRQn              : 1;  // B1
    uint32_t PKE_IRQn               : 1;  // B2
    uint32_t SMU_SECURE_IRQn        : 1;  // B3
    uint32_t SMU_S_PRIVILEGED_IRQn  : 1;  // B4
    uint32_t SMU_NS_PRIVILEGED_IRQn : 1;  // B5
    uint32_t EMU_IRQn               : 1;  // B6
    uint32_t EMUEFP_IRQn            : 1;  // B7
    uint32_t DCDC_IRQn              : 1;  // B8
    uint32_t ETAMPDET_IRQn          : 1;  // B9
    uint32_t TIMER0_IRQn            : 1;  // B10
    uint32_t TIMER1_IRQn            : 1;  // B11
    uint32_t TIMER2_IRQn            : 1;  // B12
    uint32_t TIMER3_IRQn            : 1;  // B13
    uint32_t TIMER4_IRQn            : 1;  // B14
    uint32_t RTCC_IRQn              : 1;  // B15
    uint32_t USART0_RX_IRQn         : 1;  // B16
    uint32_t USART0_TX_IRQn         : 1;  // B17
    uint32_t USART1_RX_IRQn         : 1;  // B18
    uint32_t USART1_TX_IRQn         : 1;  // B19
    uint32_t EUSART0_RX_IRQn        : 1;  // B20
    uint32_t EUSART0_TX_IRQn        : 1;  // B21
    uint32_t ICACHE0_IRQn           : 1;  // B22
    uint32_t BURTC_IRQn             : 1;  // B23
    uint32_t LETIMER0_IRQn          : 1;  // B24
    uint32_t SYSCFG_IRQn            : 1;  // B25
    uint32_t LDMA_IRQn              : 1;  // B26
    uint32_t LFXO_IRQn              : 1;  // B27
    uint32_t LFRCO_IRQn             : 1;  // B28
    uint32_t ULFRCO_IRQn            : 1;  // B29
    uint32_t GPIO_ODD_IRQn          : 1;  // B30
    uint32_t GPIO_EVEN_IRQn         : 1;  // B31
    uint32_t I2C0_IRQn              : 1;  // B32
    uint32_t I2C1_IRQn              : 1;  // B33
    uint32_t EMUDG_IRQn             : 1;  // B34
    uint32_t EMUSE_IRQn             : 1;  // B35
    uint32_t AGC_IRQn               : 1;  // B36
    uint32_t BUFC_IRQn              : 1;  // B37
    uint32_t FRC_PRI_IRQn           : 1;  // B38
    uint32_t FRC_IRQn               : 1;  // B39
    uint32_t MODEM_IRQn             : 1;  // B40
    uint32_t PROTIMER_IRQn          : 1;  // B41
    uint32_t RAC_RSM_IRQn           : 1;  // B42
    uint32_t RAC_SEQ_IRQn           : 1;  // B43
    uint32_t RDMAILBOX_IRQn         : 1;  // B44
    uint32_t RFSENSE_IRQn           : 1;  // B45
    uint32_t SYNTH_IRQn             : 1;  // B46
    uint32_t PRORTC_IRQn            : 1;  // B47
    uint32_t ACMP0_IRQn             : 1;  // B48
    uint32_t WDOG0_IRQn             : 1;  // B49
    uint32_t HFXO0_IRQn             : 1;  // B50
    uint32_t HFRCO0_IRQn            : 1;  // B51
    uint32_t CMU_IRQn               : 1;  // B52
    uint32_t AES_IRQn               : 1;  // B53
    uint32_t IADC_IRQn              : 1;  // B54
    uint32_t MSC_IRQn               : 1;  // B55
    uint32_t DPLL0_IRQn             : 1;  // B56
    uint32_t PDM_IRQn               : 1;  // B57
    uint32_t SW0_IRQn               : 1;  // B58
    uint32_t SW1_IRQn               : 1;  // B59
    uint32_t SW2_IRQn               : 1;  // B60
    uint32_t SW3_IRQn               : 1;  // B61
    uint32_t KERNEL0_IRQn           : 1;  // B62
    uint32_t KERNEL1_IRQn           : 1;  // B63
    uint32_t M33CTI0_IRQn           : 1;  // B64
    uint32_t M33CTI1_IRQn           : 1;  // B65
    uint32_t FPUEXH_IRQn            : 1;  // B66
    uint32_t                : 29; // B67-95
  } bits;
  uint32_t word[3];
#elif defined (_SILICON_LABS_32B_SERIES_2_CONFIG_8)
    uint32_t SMU_SECURE_IRQn        : 1;  // B0
    uint32_t SMU_S_PRIVILEGED_IRQn  : 1;  // B1
    uint32_t SMU_NS_PRIVILEGED_IRQn : 1;  // B2
    uint32_t EMU_IRQn               : 1;  // B3
    uint32_t TIMER0_IRQn            : 1;  // B4
    uint32_t TIMER1_IRQn            : 1;  // B5
    uint32_t TIMER2_IRQn            : 1;  // B6
    uint32_t TIMER3_IRQn            : 1;  // B7
    uint32_t TIMER4_IRQn            : 1;  // B8
    uint32_t USART0_RX_IRQn         : 1;  // B9
    uint32_t USART0_TX_IRQn         : 1;  // B10
    uint32_t EUSART0_RX_IRQn        : 1;  // B11
    uint32_t EUSART0_TX_IRQn        : 1;  // B12
    uint32_t EUSART1_RX_IRQn        : 1;  // B13
    uint32_t EUSART1_TX_IRQn        : 1;  // B14
    uint32_t EUSART2_RX_IRQn        : 1;  // B15
    uint32_t EUSART2_TX_IRQn        : 1;  // B16
    uint32_t ICACHE0_IRQn           : 1;  // B17
    uint32_t BURTC_IRQn             : 1;  // B18
    uint32_t LETIMER0_IRQn          : 1;  // B19
    uint32_t SYSCFG_IRQn            : 1;  // B20
    uint32_t MPAHBRAM_IRQn          : 1;  // B21
    uint32_t LDMA_IRQn              : 1;  // B22
    uint32_t LFXO_IRQn              : 1;  // B23
    uint32_t LFRCO_IRQn             : 1;  // B24
    uint32_t ULFRCO_IRQn            : 1;  // B25
    uint32_t GPIO_ODD_IRQn          : 1;  // B26
    uint32_t GPIO_EVEN_IRQn         : 1;  // B27
    uint32_t I2C0_IRQn              : 1;  // B28
    uint32_t I2C1_IRQn              : 1;  // B29
    uint32_t EMUDG_IRQn             : 1;  // B30
    uint32_t AGC_IRQn               : 1;  // B31
    uint32_t BUFC_IRQn              : 1;  // B32
    uint32_t FRC_PRI_IRQn           : 1;  // B33
    uint32_t FRC_IRQn               : 1;  // B34
    uint32_t MODEM_IRQn             : 1;  // B35
    uint32_t PROTIMER_IRQn          : 1;  // B36
    uint32_t RAC_RSM_IRQn           : 1;  // B37
    uint32_t RAC_SEQ_IRQn           : 1;  // B38
    uint32_t HOSTMAILBOX_IRQn       : 1;  // B39
    uint32_t SYNTH_IRQn             : 1;  // B40
    uint32_t ACMP0_IRQn             : 1;  // B41
    uint32_t ACMP1_IRQn             : 1;  // B42
    uint32_t WDOG0_IRQn             : 1;  // B43
    uint32_t WDOG1_IRQn             : 1;  // B44
    uint32_t HFXO0_IRQn             : 1;  // B45
    uint32_t HFRCO0_IRQn            : 1;  // B46
    uint32_t HFRCOEM23_IRQn         : 1;  // B47
    uint32_t CMU_IRQn               : 1;  // B48
    uint32_t AES_IRQn               : 1;  // B49
    uint32_t IADC_IRQn              : 1;  // B50
    uint32_t MSC_IRQn               : 1;  // B51
    uint32_t DPLL0_IRQn             : 1;  // B52
    uint32_t EMUEFP_IRQn            : 1;  // B53
    uint32_t DCDC_IRQn              : 1;  // B54
    uint32_t VDAC0_IRQn             : 1;  // B55
    uint32_t PCNT0_IRQn             : 1;  // B56
    uint32_t SW0_IRQn               : 1;  // B57
    uint32_t SW1_IRQn               : 1;  // B58
    uint32_t SW2_IRQn               : 1;  // B59
    uint32_t SW3_IRQn               : 1;  // B60
    uint32_t KERNEL0_IRQn           : 1;  // B61
    uint32_t KERNEL1_IRQn           : 1;  // B62
    uint32_t M33CTI0_IRQn           : 1;  // B63
    uint32_t M33CTI1_IRQn           : 1;  // B64
    uint32_t FPUEXH_IRQn            : 1;  // B65
    uint32_t SETAMPERHOST_IRQn      : 1;  // B66
    uint32_t SEMBRX_IRQn            : 1;  // B67
    uint32_t SEMBTX_IRQn            : 1;  // B68
    uint32_t LESENSE_IRQn           : 1;  // B69
    uint32_t SYSRTC_APP_IRQn        : 1;  // B70
    uint32_t SYSRTC_SEQ_IRQn        : 1;  // B71
    uint32_t LCD_IRQn               : 1;  // B72
    uint32_t KEYSCAN_IRQn           : 1;  // B73
    uint32_t RFECA0_IRQn            : 1;  // B74
    uint32_t RFECA1_IRQn            : 1;  // B75
    uint32_t AHB2AHB0_IRQn          : 1;  // B76
    uint32_t AHB2AHB1_IRQn          : 1;  // B77
    uint32_t MVP_IRQn               : 1;  // B78
    uint32_t                : 17; // B79-95
  } bits;
  uint32_t word[3];
#elif defined(_SILICON_LABS_32B_SERIES_3_CONFIG_301)
    // These come from sixg301m114lih.h
    uint32_t SETAMPERHOST_IRQn      : 1;  /*!<  0 Si SETAMPERHOST Interrupt */
    uint32_t SEMBRX_IRQn            : 1;  /*!<  1 Si SEMBRX Interrupt */
    uint32_t SEMBTX_IRQn            : 1;  /*!<  2 Si SEMBTX Interrupt */
    uint32_t SMU_SECURE_IRQn        : 1;  /*!<  3 Si SMU_SECURE Interrupt */
    uint32_t SMU_S_PRIVILEGED_IRQn  : 1;  /*!<  4 Si SMU_S_PRIVILEGED Interrupt */
    uint32_t SMU_NS_PRIVILEGED_IRQn : 1;  /*!<  5 Si SMU_NS_PRIVILEGED Interrupt */
    uint32_t EMU_IRQn               : 1;  /*!<  6 Si EMU Interrupt */
    uint32_t EMUDG_IRQn             : 1;  /*!<  7 Si EMUDG Interrupt */
    uint32_t SYSMBLPW0CPU_IRQn      : 1;  /*!<  8 Si SYSMBLPW0CPU Interrupt */
    uint32_t ETAMPDET_IRQn          : 1;  /*!<  9 Si ETAMPDET Interrupt */
    uint32_t TIMER0_IRQn            : 1; /*!< 10 Si TIMER0 Interrupt */
    uint32_t TIMER1_IRQn            : 1; /*!< 11 Si TIMER1 Interrupt */
    uint32_t TIMER2_IRQn            : 1; /*!< 12 Si TIMER2 Interrupt */
    uint32_t TIMER3_IRQn            : 1; /*!< 13 Si TIMER3 Interrupt */
    uint32_t SYSRTC_SEQ_IRQn        : 1; /*!< 14 Si SYSRTC_SEQ Interrupt */
    uint32_t SYSRTC_APP_IRQn        : 1; /*!< 15 Si SYSRTC_APP Interrupt */
    uint32_t SYSRTC_MS_IRQn         : 1; /*!< 16 Si SYSRTC_MS Interrupt */
    uint32_t EUSART0_RX_IRQn        : 1; /*!< 17 Si EUSART0_RX Interrupt */
    uint32_t EUSART0_TX_IRQn        : 1; /*!< 18 Si EUSART0_TX Interrupt */
    uint32_t EUSART1_RX_IRQn        : 1; /*!< 19 Si EUSART1_RX Interrupt */
    uint32_t EUSART1_TX_IRQn        : 1; /*!< 20 Si EUSART1_TX Interrupt */
    uint32_t EUSART2_RX_IRQn        : 1; /*!< 21 Si EUSART2_RX Interrupt */
    uint32_t EUSART2_TX_IRQn        : 1; /*!< 22 Si EUSART2_TX Interrupt */
    uint32_t L1ICACHE0_IRQn         : 1; /*!< 23 Si L1ICACHE0 Interrupt */
    uint32_t L2ICACHE0_IRQn         : 1; /*!< 24 Si L2ICACHE0 Interrupt */
    uint32_t BURTC_IRQn             : 1; /*!< 25 Si BURTC Interrupt */
    uint32_t LETIMER0_IRQn          : 1; /*!< 26 Si LETIMER0 Interrupt */
    uint32_t PIXELRZ0_IRQn          : 1; /*!< 27 Si PIXELRZ0 Interrupt */
    uint32_t PIXELRZ1_IRQn          : 1; /*!< 28 Si PIXELRZ1 Interrupt */
    uint32_t SYSCFG_IRQn            : 1; /*!< 29 Si SYSCFG Interrupt */
    uint32_t DMEM_IRQn              : 1; /*!< 30 Si DMEM Interrupt */
    uint32_t LDMA0_CHNL0_IRQn       : 1; /*!< 31 Si LDMA0_CHNL0 Interrupt */
    uint32_t LDMA0_CHNL1_IRQn       : 1; /*!< 32 Si LDMA0_CHNL1 Interrupt */
    uint32_t LDMA0_CHNL2_IRQn       : 1; /*!< 33 Si LDMA0_CHNL2 Interrupt */
    uint32_t LDMA0_CHNL3_IRQn       : 1; /*!< 34 Si LDMA0_CHNL3 Interrupt */
    uint32_t LDMA0_CHNL4_IRQn       : 1; /*!< 35 Si LDMA0_CHNL4 Interrupt */
    uint32_t LDMA0_CHNL5_IRQn       : 1; /*!< 36 Si LDMA0_CHNL5 Interrupt */
    uint32_t LDMA0_CHNL6_IRQn       : 1; /*!< 37 Si LDMA0_CHNL6 Interrupt */
    uint32_t LDMA0_CHNL7_IRQn       : 1; /*!< 38 Si LDMA0_CHNL7 Interrupt */
    uint32_t LFXO_IRQn              : 1; /*!< 39 Si LFXO Interrupt */
    uint32_t LFRCO_IRQn             : 1; /*!< 40 Si LFRCO Interrupt */
    uint32_t ULFRCO_IRQn            : 1; /*!< 41 Si ULFRCO Interrupt */
    uint32_t GPIO_ODD_IRQn          : 1; /*!< 42 Si GPIO_ODD Interrupt */
    uint32_t GPIO_EVEN_IRQn         : 1; /*!< 43 Si GPIO_EVEN Interrupt */
    uint32_t I2C0_IRQn              : 1; /*!< 44 Si I2C0 Interrupt */
    uint32_t I2C1_IRQn              : 1; /*!< 45 Si I2C1 Interrupt */
    uint32_t I2C2_IRQn              : 1; /*!< 46 Si I2C2 Interrupt */
    uint32_t BUFC_IRQn              : 1; /*!< 47 Si BUFC Interrupt */
    uint32_t FRC_PRI_IRQn           : 1; /*!< 48 Si FRC_PRI Interrupt */
    uint32_t FRC_IRQn               : 1; /*!< 49 Si FRC Interrupt */
    uint32_t PROTIMER_IRQn          : 1; /*!< 50 Si PROTIMER Interrupt */
    uint32_t RAC_RSM_IRQn           : 1; /*!< 51 Si RAC_RSM Interrupt */
    uint32_t RAC_SEQ_IRQn           : 1; /*!< 52 Si RAC_SEQ Interrupt */
    uint32_t SYNTH_IRQn             : 1; /*!< 53 Si SYNTH Interrupt */
    uint32_t RFECA0_IRQn            : 1; /*!< 54 Si RFECA0 Interrupt */
    uint32_t RFECA1_IRQn            : 1; /*!< 55 Si RFECA1 Interrupt */
    uint32_t MODEM_IRQn             : 1; /*!< 56 Si MODEM Interrupt */
    uint32_t AGC_IRQn               : 1; /*!< 57 Si AGC Interrupt */
    uint32_t RFTIMER_IRQn           : 1; /*!< 58 Si RFTIMER Interrupt */
    uint32_t SEQACC_IRQn            : 1; /*!< 59 Si SEQACC Interrupt */
    uint32_t HFRCOLPW_IRQn          : 1; /*!< 60 Si HFRCOLPW Interrupt */
    uint32_t HFRCODPLLLPW_IRQn      : 1; /*!< 61 Si HFRCODPLLLPW Interrupt */
    uint32_t ACMP0_IRQn             : 1; /*!< 62 Si ACMP0 Interrupt */
    uint32_t ACMP1_IRQn             : 1; /*!< 63 Si ACMP1 Interrupt */
    uint32_t WDOG0_IRQn             : 1; /*!< 64 Si WDOG0 Interrupt */
    uint32_t WDOG1_IRQn             : 1; /*!< 65 Si WDOG1 Interrupt */
    uint32_t HFXO0_IRQn             : 1; /*!< 66 Si HFXO0 Interrupt */
    uint32_t HFRCO0_IRQn            : 1; /*!< 67 Si HFRCO0 Interrupt */
    uint32_t HFRCOEM23_IRQn         : 1; /*!< 68 Si HFRCOEM23 Interrupt */
    uint32_t CMU_IRQn               : 1; /*!< 69 Si CMU Interrupt */
    uint32_t RPA_IRQn               : 1; /*!< 70 Si RPA Interrupt */
    uint32_t KSURPA_IRQn            : 1; /*!< 71 Si KSURPA Interrupt */
    uint32_t KSULPWAES_IRQn         : 1; /*!< 72 Si KSULPWAES Interrupt */
    uint32_t KSUHOSTSYMCRYPTO_IRQn  : 1; /*!< 73 Si KSUHOSTSYMCRYPTO Interrupt */
    uint32_t SYMCRYPTO_IRQn         : 1; /*!< 74 Si SYMCRYPTO Interrupt */
    uint32_t AES_IRQn               : 1; /*!< 75 Si AES Interrupt */
    uint32_t ADC0_IRQn              : 1; /*!< 76 Si ADC0 Interrupt */
    uint32_t LEDDRV0_IRQn           : 1; /*!< 77 Si LEDDRV0 Interrupt */
    uint32_t DPLL0_IRQn             : 1; /*!< 78 Si DPLL0 Interrupt */
    uint32_t SOCPLL0_IRQn           : 1; /*!< 79 Si SOCPLL0 Interrupt */
    uint32_t PCNT0_IRQn             : 1; /*!< 80 Si PCNT0 Interrupt */
    uint32_t SW0_IRQn               : 1; /*!< 81 Si SW0 Interrupt */
    uint32_t SW1_IRQn               : 1; /*!< 82 Si SW1 Interrupt */
    uint32_t SW2_IRQn               : 1; /*!< 83 Si SW2 Interrupt */
    uint32_t SW3_IRQn               : 1; /*!< 84 Si SW3 Interrupt */
    uint32_t KERNEL0_IRQn           : 1; /*!< 85 Si KERNEL0 Interrupt */
    uint32_t KERNEL1_IRQn           : 1; /*!< 86 Si KERNEL1 Interrupt */
    uint32_t M33CTI0_IRQn           : 1; /*!< 87 Si M33CTI0 Interrupt */
    uint32_t M33CTI1_IRQn           : 1; /*!< 88 Si M33CTI1 Interrupt */
    uint32_t FPUEXH_IRQn            : 1; /*!< 89 Si FPUEXH Interrupt */
    uint32_t : 6; // B90-95
  } bits;
  uint32_t word[3];
#elif CORTEXM3_EMBER_MICRO
    uint32_t TIM1_IRQn      : 1;  // B0
    uint32_t TIM2_IRQn      : 1;  // B1
    uint32_t MGMT_IRQn      : 1;  // B2
    uint32_t BB_IRQn        : 1;  // B3
    uint32_t SLEEPTMR_IRQn  : 1;  // B4
    uint32_t SC1_IRQn       : 1;  // B5
    uint32_t SC2_IRQn       : 1;  // B6
    uint32_t AESCCM_IRQn    : 1;  // B7
    uint32_t MACTMR_IRQn    : 1;  // B8
    uint32_t MACTX_IRQn     : 1;  // B9
    uint32_t MACRX_IRQn     : 1;  // B10
    uint32_t ADC_IRQn       : 1;  // B11
    uint32_t IRQA_IRQn      : 1;  // B12
    uint32_t IRQB_IRQn      : 1;  // B13
    uint32_t IRQC_IRQn      : 1;  // B14
    uint32_t IRQD_IRQn      : 1;  // B15
    uint32_t DEBUG_IRQn     : 1;  // B16
    uint32_t                : 15; // B17-31
  } bits;
  uint32_t word;
#else
  #error micro not recognized
#endif
} HalCrashIntActiveType;

typedef union {
  struct {
    uint32_t MEMFAULTACT    : 1;  // B0
    uint32_t BUSFAULTACT    : 1;  // B1
    uint32_t                : 1;  // B2
    uint32_t USGFAULTACT    : 1;  // B3
    uint32_t                : 3;  // B4-6
    uint32_t SVCALLACT      : 1;  // B7
    uint32_t MONITORACT     : 1;  // B8
    uint32_t                : 1;  // B9
    uint32_t PENDSVACT      : 1;  // B10
    uint32_t SYSTICKACT     : 1;  // B11
    uint32_t USGFAULTPENDED : 1;  // B12
    uint32_t MEMFAULTPENDED : 1;  // B13
    uint32_t BUSFAULTPENDED : 1;  // B14
    uint32_t SVCALLPENDED   : 1;  // B15
    uint32_t MEMFAULTENA    : 1;  // B16
    uint32_t BUSFAULTENA    : 1;  // B17
    uint32_t USGFAULTENA    : 1;  // B18
    uint32_t                : 13; // B19-31
  } bits;

  uint32_t word;
} HalCrashShcsrType;

/***************************************************************************//**
 * @brief The `HalCrashCfsrType` is a union data structure used to represent the
 * Configurable Fault Status Register (CFSR) in a Cortex-M processor. It
 * provides a detailed breakdown of various fault conditions that can
 * occur during program execution, such as memory management faults, bus
 * faults, and usage faults. The union allows access to individual fault
 * bits through a bit-field structure, as well as access to the entire
 * 32-bit register as a single word. This structure is crucial for
 * diagnosing and handling exceptions in embedded systems.
 *
 * @param IACCVIOL Indicates an instruction access violation.
 * @param DACCVIOL Indicates a data access violation.
 * @param MUNSTKERR Indicates an unstacking error during exception return.
 * @param MSTKERR Indicates a stacking error during exception entry.
 * @param MMARVALID Indicates that the MMAR register is valid.
 * @param IBUSERR Indicates an instruction bus error.
 * @param PRECISERR Indicates a precise data bus error.
 * @param IMPRECISERR Indicates an imprecise data bus error.
 * @param UNSTKERR Indicates an unstacking error during exception return.
 * @param STKERR Indicates a stacking error during exception entry.
 * @param BFARVALID Indicates that the BFAR register is valid.
 * @param UNDEFINSTR Indicates an undefined instruction usage fault.
 * @param INVSTATE Indicates an invalid state usage fault.
 * @param INVPC Indicates an invalid PC load usage fault.
 * @param NOCP Indicates a no coprocessor usage fault.
 * @param UNALIGNED Indicates an unaligned access usage fault.
 * @param DIVBYZERO Indicates a divide by zero usage fault.
 * @param word Represents the entire 32-bit word of the CFSR register.
 ******************************************************************************/
typedef union {
  struct {
    uint32_t IACCVIOL       : 1;  // B0
    uint32_t DACCVIOL       : 1;  // B1
    uint32_t                : 1;  // B2
    uint32_t MUNSTKERR      : 1;  // B3
    uint32_t MSTKERR        : 1;  // B4
    uint32_t                : 2;  // B5-6
    uint32_t MMARVALID      : 1;  // B7
    uint32_t IBUSERR        : 1;  // B8
    uint32_t PRECISERR      : 1;  // B9
    uint32_t IMPRECISERR    : 1;  // B10
    uint32_t UNSTKERR       : 1;  // B11
    uint32_t STKERR         : 1;  // B12
    uint32_t                : 2;  // B13-14
    uint32_t BFARVALID      : 1;  // B15
    uint32_t UNDEFINSTR     : 1;  // B16
    uint32_t INVSTATE       : 1;  // B17
    uint32_t INVPC          : 1;  // B18
    uint32_t NOCP           : 1;  // B19
    uint32_t                : 4;  // B20-23
    uint32_t UNALIGNED      : 1;  // B24
    uint32_t DIVBYZERO      : 1;  // B25
    uint32_t                : 6;  // B26-31
  } bits;

  uint32_t word;
} HalCrashCfsrType;

/***************************************************************************//**
 * @brief The `HalCrashHfsrType` is a union data structure used to represent the
 * Hard Fault Status Register (HFSR) in a system. It provides a way to
 * access the register either as a whole 32-bit word or through
 * individual bit fields. The bit fields include `VECTTBL`, which
 * indicates a vector table read fault, `FORCED`, which indicates a
 * forced hard fault, and `DEBUGEVT`, which indicates a debug event. This
 * structure is useful for diagnosing and handling hard faults in
 * embedded systems.
 *
 * @param bits A struct containing bit fields representing specific hard fault
 * status flags.
 * @param word A 32-bit unsigned integer representing the entire hard fault
 * status register.
 ******************************************************************************/
typedef union {
  struct {
    uint32_t                : 1;  // B0
    uint32_t VECTTBL        : 1;  // B1
    uint32_t                : 28; // B2-29
    uint32_t FORCED         : 1;  // B30
    uint32_t DEBUGEVT       : 1;  // B31
  } bits;

  uint32_t word;
} HalCrashHfsrType;

/***************************************************************************//**
 * @brief The `HalCrashDfsrType` is a union that encapsulates the Debug Fault
 * Status Register (DFSR) used in crash diagnostics. It provides a bit-
 * field structure to access individual debug fault status flags such as
 * HALTED, BKPT, DWTTRAP, VCATCH, and EXTERNAL, which indicate specific
 * types of debug events or faults. Alternatively, the entire register
 * can be accessed as a single 32-bit word, allowing for efficient
 * manipulation and examination of the register's state in crash
 * analysis.
 *
 * @param bits A struct containing bit fields representing various debug fault
 * status flags.
 * @param word A 32-bit unsigned integer representing the entire debug fault
 * status register as a single word.
 ******************************************************************************/
typedef union {
  struct {
    uint32_t HALTED         : 1;  // B0
    uint32_t BKPT           : 1;  // B1
    uint32_t DWTTRAP        : 1;  // B2
    uint32_t VCATCH         : 1;  // B3
    uint32_t EXTERNAL       : 1;  // B4
    uint32_t                : 27; // B5-31
  } bits;

  uint32_t word;
} HalCrashDfsrType;

/***************************************************************************//**
 * @brief The `HalCrashAfsrType` is a union that encapsulates the auxiliary
 * fault status register (AFSR) used in crash diagnostics. It provides a
 * bit-field structure to access individual fault status flags such as
 * `MISSED`, `RESERVED`, `PROTECTED`, and `WRONGSIZE`, each occupying a
 * single bit, while the remaining 28 bits are unused. The union also
 * allows access to the entire 32-bit register as a single word,
 * facilitating both detailed and holistic views of the fault status.
 *
 * @param bits A struct containing bit fields representing various fault status
 * flags.
 * @param word A 32-bit unsigned integer representing the entire auxiliary fault
 * status register as a single word.
 ******************************************************************************/
typedef union {
  struct {
    uint32_t MISSED         : 1;  // B0
    uint32_t RESERVED       : 1;  // B1
    uint32_t PROTECTED      : 1;  // B2
    uint32_t WRONGSIZE      : 1;  // B3
    uint32_t                : 28; // B4-31
  } bits;

  uint32_t word;
} HalCrashAfsrType;

#define NUM_RETURNS     6U

// Define the crash data structure
/***************************************************************************//**
 * @brief The `HalCrashInfoType` structure is designed to capture and store
 * detailed information about a system crash, primarily for diagnostic
 * purposes. It includes processor register values, stack pointers, and
 * various status registers that provide insight into the state of the
 * system at the time of the crash. This structure is critical for
 * debugging and understanding the cause of system faults, as it records
 * the state of the processor and memory, including the program counter,
 * stack usage, and fault status registers. The structure is carefully
 * organized to ensure compatibility with assembly language fault
 * handlers, and it includes additional fields for specific crash data
 * and probable return addresses.
 *
 * @param R0 Processor register R0.
 * @param R1 Processor register R1.
 * @param R2 Processor register R2.
 * @param R3 Processor register R3.
 * @param R4 Processor register R4.
 * @param R5 Processor register R5.
 * @param R6 Processor register R6.
 * @param R7 Processor register R7.
 * @param R8 Processor register R8.
 * @param R9 Processor register R9.
 * @param R10 Processor register R10.
 * @param R11 Processor register R11.
 * @param R12 Processor register R12.
 * @param LR Link register.
 * @param mainSP Main stack pointer.
 * @param processSP Process stack pointer.
 * @param PC Program counter, stacked return value.
 * @param xPSR Stacked processor status register.
 * @param mainSPUsed Bytes used in main stack.
 * @param processSPUsed Bytes used in process stack.
 * @param mainStackBottom Address of the bottom of the main stack.
 * @param icsr Interrupt control state register.
 * @param shcsr System handlers control and state register.
 * @param intActive IRQ active bit register.
 * @param cfsr Configurable fault status register.
 * @param hfsr Hard fault status register.
 * @param dfsr Debug fault status register.
 * @param faultAddress Fault address register (MMAR or BFAR).
 * @param afsr Auxiliary fault status register.
 * @param returns Probable return addresses found on the stack.
 * @param data Additional data specific to the crash type.
 ******************************************************************************/
typedef struct {
  // ***************************************************************************
  // The components within this first block are written by the assembly
  // language common fault handler, and position and order is critical.
  // cstartup-iar-boot-entry.s79 also relies on the position/order here.
  // Do not edit without also modifying that code.
  // ***************************************************************************
  uint32_t R0;            // processor registers
  uint32_t R1;
  uint32_t R2;
  uint32_t R3;
  uint32_t R4;
  uint32_t R5;
  uint32_t R6;
  uint32_t R7;
  uint32_t R8;
  uint32_t R9;
  uint32_t R10;
  uint32_t R11;
  uint32_t R12;
  uint32_t LR;
  uint32_t mainSP;        // main and process stack pointers
  uint32_t processSP;
  // ***************************************************************************
  // End of the block written by the common fault handler.
  // ***************************************************************************

  uint32_t PC;              // stacked return value (if it could be read)
  HalCrashxPsrType xPSR;  // stacked processor status reg (if it could be read)
  uint32_t mainSPUsed;      // bytes used in main stack
  uint32_t processSPUsed;   // bytes used in process stack
  uint32_t mainStackBottom; // address of the bottom of the stack
  HalCrashIcsrType icsr;  // interrupt control state register
  HalCrashShcsrType shcsr;// system handlers control and state register
  HalCrashIntActiveType intActive;  // irq active bit register
  HalCrashCfsrType cfsr;  // configurable fault status register
  HalCrashHfsrType hfsr;  // hard fault status register
  HalCrashDfsrType dfsr;  // debug fault status register
  uint32_t faultAddress;    // fault address register (MMAR or BFAR)
  HalCrashAfsrType afsr;  // auxiliary fault status register
  uint32_t returns[NUM_RETURNS];  // probable return addresses found on the stack
  HalCrashSpecificDataType data;  // additional data specific to the crash type
} HalCrashInfoType;

/***************************************************************************//**
 * @brief The `HalResetCauseType` structure is used to encapsulate information
 * about the cause of a system reset. It contains two members: `reason`,
 * which indicates the specific reason for the reset, and `signature`,
 * which serves as a verification or identification marker for the reset
 * event. This structure is part of a diagnostic system that helps in
 * identifying and handling different reset scenarios in embedded
 * systems.
 *
 * @param reason A 16-bit unsigned integer representing the reason for the
 * reset.
 * @param signature A 16-bit unsigned integer used as a signature for the reset
 * cause.
 ******************************************************************************/
typedef struct {
  uint16_t reason;
  uint16_t signature;
} HalResetCauseType;

#define RESETINFO_WORDS  ((sizeof(HalResetInfoType) + 3) / 4)

// Macro evaluating to true if the last reset was a crash, false otherwise.
#define halResetWasCrash() \
  (((1 << halGetResetInfo()) & RESET_CRASH_REASON_MASK) != 0U)

// Print a summary of crash details.
/***************************************************************************//**
 * @brief This function is used to output a summary of crash information,
 * including stack usage and active interrupts, to a specified output
 * port. It is typically called after a system crash to help diagnose the
 * cause of the crash. The function does not modify any input parameters
 * and does not return any value. It is important to ensure that the
 * output port is correctly configured and available for use before
 * calling this function.
 *
 * @param port Specifies the output port to which the crash summary will be
 * printed. The parameter is not used in the current implementation,
 * but it should be a valid port identifier if future
 * implementations require it. The caller retains ownership and
 * responsibility for ensuring the port is correctly configured.
 * @return None
 ******************************************************************************/
void halPrintCrashSummary(uint8_t port);

// Print the complete, decoded crash details.
/***************************************************************************//**
 * @brief This function is used to output detailed information about the cause
 * of the last system reset, which can be due to various faults such as
 * watchdog expiration, hard faults, memory management faults, bus
 * faults, usage faults, or debug monitor faults. It is useful for
 * diagnosing the reason behind a system crash by providing specific
 * details about the fault, including instruction addresses and fault-
 * specific information. This function should be called when detailed
 * diagnostic information is needed after a system reset. It does not
 * modify any input parameters or return any values.
 *
 * @param port Specifies the output port to which the crash details will be
 * printed. The parameter is not used in the current implementation,
 * and any value can be passed.
 * @return None
 ******************************************************************************/
void halPrintCrashDetails(uint8_t port);

// Print the complete crash data.
/***************************************************************************//**
 * @brief This function is used to output detailed crash data to a specified
 * port, which is useful for debugging purposes. It should be called when
 * a detailed analysis of crash data is required, typically after a
 * system reset that was caused by a crash. The function does not perform
 * any operations based on the port parameter, as it is ignored. It is
 * important to ensure that the system is in a state where printing to
 * the standard output is possible, as the function relies on this
 * capability.
 *
 * @param port An unsigned 8-bit integer representing the output port. The
 * parameter is ignored by the function, so any value is valid. The
 * caller retains ownership.
 * @return None
 ******************************************************************************/
void halPrintCrashData(uint8_t port);

// If last reset was from an assert, return saved assert information.
/***************************************************************************//**
 * @brief This function provides access to the details of the last assertion
 * failure, if any, by returning a pointer to a structure containing the
 * file name and line number where the assertion occurred. It is useful
 * for debugging purposes to identify the location of the failure. The
 * function should be called after a reset to determine if the reset was
 * caused by an assertion failure. The returned pointer should not be
 * modified by the caller.
 *
 * @return A pointer to a `HalAssertInfoType` structure containing the file name
 * and line number of the last assertion failure, or `NULL` if no
 * assertion failure information is available.
 ******************************************************************************/
/***************************************************************************//**
 * @brief The `halGetAssertInfo` function returns a pointer to a
 * `HalAssertInfoType` structure, which contains information about the
 * file and line number where an assert occurred. This is useful for
 * debugging purposes to identify the location in the code that triggered
 * an assert condition.
 *
 * @details This variable is used to retrieve and provide information about the
 * last assert condition that occurred, specifically the file and line
 * number, for diagnostic purposes.
 ******************************************************************************/
const HalAssertInfoType *halGetAssertInfo(void);

void halInternalAssertFailed(const char *filename, int linenumber);

/***************************************************************************//**
 * @brief This function determines the cause of the most recent system reset and
 * updates internal state to reflect this information. It should be
 * called during system initialization to ensure that any reset-related
 * diagnostics or recovery actions can be taken based on the reset cause.
 * The function handles various reset causes such as power-on, external
 * pin, software, watchdog expiration, and others, and it updates
 * internal variables to store this information. It is important to call
 * this function before any other diagnostics or reset-related functions
 * to ensure accurate reset cause information is available.
 *
 * @return None
 ******************************************************************************/
void halInternalClassifyReset(void);

/** @} (end addtogroup diagnostics) */
/** @} (end addtogroup legacyhal) */

#endif // DIAGNOSTIC_H
