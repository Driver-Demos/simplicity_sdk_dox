/**************************************************************************//**
 * @file
 * @brief Device Manager API Definition
 ******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories, Inc. www.silabs.com</b>
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
#ifndef SL_DEVICE_PERIPHERAL_H
#define SL_DEVICE_PERIPHERAL_H

#include "sl_device_peripheral_types.h"
#include "sl_code_classification.h"

#if defined(DEVICE_PERIPHERAL_INTERNAL_PRESENT)
#include "sli_device_peripheral_internal.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup device_peripheral Device Abstraction Peripheral
 * @details
 * ## Overview
 *
 * The Device Abstraction Peripheral module defines the different peripherals
 * that exist across all Silicon Labs devices and their utility functions.
 *
 * @{
 ******************************************************************************/

// ----------------------------------------------------------------------------
// DEFINES

/***************************************************************************//**
 * @name Peripheral Defines
 * Those defines can be used as constant of type sl_peripheral_t.
 * The values of those defines are device specific.
 * @{
 ******************************************************************************/

/// Define pointer to ACMP0 peripheral structure.
#define SL_PERIPHERAL_ACMP0 (&sl_peripheral_val_acmp0)

/// Define pointer to ACMP1 peripheral structure.
#define SL_PERIPHERAL_ACMP1 (&sl_peripheral_val_acmp1)

/// Define pointer to ADC0 peripheral structure.
#define SL_PERIPHERAL_ADC0 (&sl_peripheral_val_adc0)

/// Define pointer to AES peripheral structure.
#define SL_PERIPHERAL_AES (&sl_peripheral_val_aes)

/// Define pointer to AMUXCP0 peripheral structure.
#define SL_PERIPHERAL_AMUXCP0 (&sl_peripheral_val_amuxcp0)

/// Define pointer to BUFC peripheral structure.
#define SL_PERIPHERAL_BUFC (&sl_peripheral_val_bufc)

/// Define pointer to BURAM peripheral structure.
#define SL_PERIPHERAL_BURAM (&sl_peripheral_val_buram)

/// Define pointer to BURTC peripheral structure.
#define SL_PERIPHERAL_BURTC (&sl_peripheral_val_burtc)

/// Define pointer to CMU peripheral structure.
#define SL_PERIPHERAL_CMU (&sl_peripheral_val_cmu)

/// Define pointer to CRYPTOACC peripheral structure.
#define SL_PERIPHERAL_CRYPTOACC (&sl_peripheral_val_cryptoacc)

/// Define pointer to DCDC peripheral structure.
#define SL_PERIPHERAL_DCDC (&sl_peripheral_val_dcdc)

/// Define pointer to DEVINFO peripheral structure.
#define SL_PERIPHERAL_DEVINFO (&sl_peripheral_val_devinfo)

/// Define pointer to DMEM peripheral structure.
#define SL_PERIPHERAL_DMEM (&sl_peripheral_val_dmem)

/// Define pointer to DMEM0 peripheral structure.
#define SL_PERIPHERAL_DMEM0 (&sl_peripheral_val_dmem0)

/// Define pointer to DMEM1 peripheral structure.
#define SL_PERIPHERAL_DMEM1 (&sl_peripheral_val_dmem1)

/// Define pointer to DPLL0 peripheral structure.
#define SL_PERIPHERAL_DPLL0 (&sl_peripheral_val_dpll0)

/// Define pointer to EMU peripheral structure.
#define SL_PERIPHERAL_EMU (&sl_peripheral_val_emu)

/// Define pointer to ETAMPDET peripheral structure.
#define SL_PERIPHERAL_ETAMPDET (&sl_peripheral_val_etampdet)

/// Define pointer to EUART0 peripheral structure.
#define SL_PERIPHERAL_EUART0 (&sl_peripheral_val_euart0)

/// Define pointer to EUSART0 peripheral structure.
#define SL_PERIPHERAL_EUSART0 (&sl_peripheral_val_eusart0)

/// Define pointer to EUSART1 peripheral structure.
#define SL_PERIPHERAL_EUSART1 (&sl_peripheral_val_eusart1)

/// Define pointer to EUSART2 peripheral structure.
#define SL_PERIPHERAL_EUSART2 (&sl_peripheral_val_eusart2)

/// Define pointer to EUSART3 peripheral structure.
#define SL_PERIPHERAL_EUSART3 (&sl_peripheral_val_eusart3)

/// Define pointer to EUSART4 peripheral structure.
#define SL_PERIPHERAL_EUSART4 (&sl_peripheral_val_eusart4)

/// Define pointer to FSRCO peripheral structure.
#define SL_PERIPHERAL_FSRCO (&sl_peripheral_val_fsrco)

/// Define pointer to GPCRC0 peripheral structure.
#define SL_PERIPHERAL_GPCRC0 (&sl_peripheral_val_gpcrc0)

/// Define pointer to GPIO peripheral structure.
#define SL_PERIPHERAL_GPIO (&sl_peripheral_val_gpio)

/// Define pointer to HFRCO0 peripheral structure.
#define SL_PERIPHERAL_HFRCO0 (&sl_peripheral_val_hfrco0)

/// Define pointer to HFRCOEM23 peripheral structure.
#define SL_PERIPHERAL_HFRCOEM23 (&sl_peripheral_val_hfrcoem23)

/// Define pointer to HFXO0 peripheral structure.
#define SL_PERIPHERAL_HFXO0 (&sl_peripheral_val_hfxo0)

/// Define pointer to HOSTMAILBOX peripheral structure.
#define SL_PERIPHERAL_HOSTMAILBOX (&sl_peripheral_val_hostmailbox)

/// Define pointer to HOSTPORTAL peripheral structure.
#define SL_PERIPHERAL_HOSTPORTAL (&sl_peripheral_val_hostportal)

/// Define pointer to I2C0 peripheral structure.
#define SL_PERIPHERAL_I2C0 (&sl_peripheral_val_i2c0)

/// Define pointer to I2C1 peripheral structure.
#define SL_PERIPHERAL_I2C1 (&sl_peripheral_val_i2c1)

/// Define pointer to I2C2 peripheral structure.
#define SL_PERIPHERAL_I2C2 (&sl_peripheral_val_i2c2)

/// Define pointer to I2C3 peripheral structure.
#define SL_PERIPHERAL_I2C3 (&sl_peripheral_val_i2c3)

/// Define pointer to IADC0 peripheral structure.
#define SL_PERIPHERAL_IADC0 (&sl_peripheral_val_iadc0)

/// Define pointer to ICACHE0 peripheral structure.
#define SL_PERIPHERAL_ICACHE0 (&sl_peripheral_val_icache0)

/// Define pointer to KEYSCAN peripheral structure.
#define SL_PERIPHERAL_KEYSCAN (&sl_peripheral_val_keyscan)

/// Define pointer to L1ICACHE0 peripheral structure.
#define SL_PERIPHERAL_L1ICACHE0 (&sl_peripheral_val_l1icache0)

/// Define pointer to L2ICACHE0 peripheral structure.
#define SL_PERIPHERAL_L2ICACHE0 (&sl_peripheral_val_l2icache0)

/// Define pointer to LCD peripheral structure.
#define SL_PERIPHERAL_LCD (&sl_peripheral_val_lcd)

/// Define pointer to LCDRF peripheral structure.
#define SL_PERIPHERAL_LCDRF (&sl_peripheral_val_lcdrf)

/// Define pointer to LDMA0 peripheral structure.
#define SL_PERIPHERAL_LDMA0 (&sl_peripheral_val_ldma0)

/// Define pointer to LDMAXBAR0 peripheral structure.
#define SL_PERIPHERAL_LDMAXBAR0 (&sl_peripheral_val_ldmaxbar0)

/// Define pointer to LEDDRV0 peripheral structure.
#define SL_PERIPHERAL_LEDDRV0 (&sl_peripheral_val_leddrv0)

/// Define pointer to LESENSE peripheral structure.
#define SL_PERIPHERAL_LESENSE (&sl_peripheral_val_lesense)

/// Define pointer to LETIMER0 peripheral structure.
#define SL_PERIPHERAL_LETIMER0 (&sl_peripheral_val_letimer0)

/// Define pointer to LFRCO peripheral structure.
#define SL_PERIPHERAL_LFRCO (&sl_peripheral_val_lfrco)

/// Define pointer to LFXO peripheral structure.
#define SL_PERIPHERAL_LFXO (&sl_peripheral_val_lfxo)

/// Define pointer to LPWAES peripheral structure.
#define SL_PERIPHERAL_LPWAES (&sl_peripheral_val_lpwaes)

/// Define pointer to LPW0PORTAL peripheral structure.
#define SL_PERIPHERAL_LPW0PORTAL (&sl_peripheral_val_lpw0portal)

/// Define pointer to LVGD peripheral structure.
#define SL_PERIPHERAL_LVGD (&sl_peripheral_val_lvgd)

/// Define pointer to MPAHBRAM peripheral structure.
#define SL_PERIPHERAL_MPAHBRAM (&sl_peripheral_val_mpahbram)

/// Define pointer to MSC peripheral structure.
#define SL_PERIPHERAL_MSC (&sl_peripheral_val_msc)

/// Define pointer to MVP peripheral structure.
#define SL_PERIPHERAL_MVP (&sl_peripheral_val_mvp)

/// Define pointer to PCNT0 peripheral structure.
#define SL_PERIPHERAL_PCNT0 (&sl_peripheral_val_pcnt0)

/// Define pointer to PDM peripheral structure.
#define SL_PERIPHERAL_PDM (&sl_peripheral_val_pdm)

/// Define pointer to PFMXPPRF peripheral structure.
#define SL_PERIPHERAL_PFMXPPRF (&sl_peripheral_val_pfmxpprf)

/// Define pointer to PIXELRZ0 peripheral structure.
#define SL_PERIPHERAL_PIXELRZ0 (&sl_peripheral_val_pixelrz0)

/// Define pointer to PIXELRZ1 peripheral structure.
#define SL_PERIPHERAL_PIXELRZ1 (&sl_peripheral_val_pixelrz1)

/// Define pointer to PRORTC peripheral structure.
#define SL_PERIPHERAL_PRORTC (&sl_peripheral_val_prortc)

/// Define pointer to PRS peripheral structure.
#define SL_PERIPHERAL_PRS (&sl_peripheral_val_prs)

/// Define pointer to RADIOAES peripheral structure.
#define SL_PERIPHERAL_RADIOAES (&sl_peripheral_val_radioaes)

/// Define pointer to RFFPLL0 peripheral structure.
#define SL_PERIPHERAL_RFFPLL0 (&sl_peripheral_val_rffpll0)

/// Define pointer to RPA peripheral structure.
#define SL_PERIPHERAL_RPA (&sl_peripheral_val_rpa)

/// Define pointer to RTCC peripheral structure.
#define SL_PERIPHERAL_RTCC (&sl_peripheral_val_rtcc)

/// Define pointer to SCRATCHPAD peripheral structure.
#define SL_PERIPHERAL_SCRATCHPAD (&sl_peripheral_val_scratchpad)

/// Define pointer to SEMAILBOX peripheral structure.
#define SL_PERIPHERAL_SEMAILBOX (&sl_peripheral_val_semailbox)

/// Define pointer to SEMAPHORE0 peripheral structure.
#define SL_PERIPHERAL_SEMAPHORE0 (&sl_peripheral_val_semaphore0)

/// Define pointer to SEMAPHORE1 peripheral structure.
#define SL_PERIPHERAL_SEMAPHORE1 (&sl_peripheral_val_semaphore1)

/// Define pointer to SEPORTAL peripheral structure.
#define SL_PERIPHERAL_SEPORTAL (&sl_peripheral_val_seportal)

/// Define pointer to SEPUF peripheral structure.
#define SL_PERIPHERAL_SEPUF (&sl_peripheral_val_sepuf)

/// Define pointer to SMU peripheral structure.
#define SL_PERIPHERAL_SMU (&sl_peripheral_val_smu)

/// Define pointer to SOCPLL0 peripheral structure.
#define SL_PERIPHERAL_SOCPLL0 (&sl_peripheral_val_socpll0)

/// Define pointer to SYMCRYPTO peripheral structure.
#define SL_PERIPHERAL_SYMCRYPTO (&sl_peripheral_val_symcrypto)

/// Define pointer to SYSCFG peripheral structure.
#define SL_PERIPHERAL_SYSCFG (&sl_peripheral_val_syscfg)

/// Define pointer to SYSRTC0 peripheral structure.
#define SL_PERIPHERAL_SYSRTC0 (&sl_peripheral_val_sysrtc0)

/// Define pointer to TIMER0 peripheral structure.
#define SL_PERIPHERAL_TIMER0 (&sl_peripheral_val_timer0)

/// Define pointer to TIMER1 peripheral structure.
#define SL_PERIPHERAL_TIMER1 (&sl_peripheral_val_timer1)

/// Define pointer to TIMER2 peripheral structure.
#define SL_PERIPHERAL_TIMER2 (&sl_peripheral_val_timer2)

/// Define pointer to TIMER3 peripheral structure.
#define SL_PERIPHERAL_TIMER3 (&sl_peripheral_val_timer3)

/// Define pointer to TIMER4 peripheral structure.
#define SL_PERIPHERAL_TIMER4 (&sl_peripheral_val_timer4)

/// Define pointer to TIMER5 peripheral structure.
#define SL_PERIPHERAL_TIMER5 (&sl_peripheral_val_timer5)

/// Define pointer to TIMER6 peripheral structure.
#define SL_PERIPHERAL_TIMER6 (&sl_peripheral_val_timer6)

/// Define pointer to TIMER7 peripheral structure.
#define SL_PERIPHERAL_TIMER7 (&sl_peripheral_val_timer7)

/// Define pointer to TIMER8 peripheral structure.
#define SL_PERIPHERAL_TIMER8 (&sl_peripheral_val_timer8)

/// Define pointer to TIMER9 peripheral structure.
#define SL_PERIPHERAL_TIMER9 (&sl_peripheral_val_timer9)

/// Define pointer to ULFRCO peripheral structure.
#define SL_PERIPHERAL_ULFRCO (&sl_peripheral_val_ulfrco)

/// Define pointer to USART0 peripheral structure.
#define SL_PERIPHERAL_USART0 (&sl_peripheral_val_usart0)

/// Define pointer to USART1 peripheral structure.
#define SL_PERIPHERAL_USART1 (&sl_peripheral_val_usart1)

/// Define pointer to USART2 peripheral structure.
#define SL_PERIPHERAL_USART2 (&sl_peripheral_val_usart2)

/// Define pointer to USB peripheral structure.
#define SL_PERIPHERAL_USB (&sl_peripheral_val_usb)

/// Define pointer to USBAHB peripheral structure.
#define SL_PERIPHERAL_USBAHB (&sl_peripheral_val_usbahb)

/// Define pointer to USBPLL0 peripheral structure.
#define SL_PERIPHERAL_USBPLL0 (&sl_peripheral_val_usbpll0)

/// Define pointer to VDAC0 peripheral structure.
#define SL_PERIPHERAL_VDAC0 (&sl_peripheral_val_vdac0)

/// Define pointer to VDAC1 peripheral structure.
#define SL_PERIPHERAL_VDAC1 (&sl_peripheral_val_vdac1)

/// Define pointer to WDOG0 peripheral structure.
#define SL_PERIPHERAL_WDOG0 (&sl_peripheral_val_wdog0)

/// Define pointer to WDOG1 peripheral structure.
#define SL_PERIPHERAL_WDOG1 (&sl_peripheral_val_wdog1)

/// @} (end peripheral_defines)

// ----------------------------------------------------------------------------
// TYPEDEFS

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

#if defined(__ICCARM__)
// Disable IAR multiple typedefs declaration warning.
#pragma diag_suppress=Pe301
#endif

// Declare peripheral structure for ACMP.
/***************************************************************************//**
 * @brief The `ACMP_TypeDef` is a typedef for a structure that represents the
 * Analog Comparator peripheral in Silicon Labs devices, allowing for the
 * management and configuration of the ACMP hardware features.
 *
 ******************************************************************************/
typedef struct acmp_typedef ACMP_TypeDef;

// Declare peripheral structure for ADC.
/***************************************************************************//**
 * @brief The `ADC_TypeDef` is a typedef for a structure that represents the
 * Analog-to-Digital Converter (ADC) peripheral in Silicon Labs devices,
 * allowing for the configuration and control of ADC functionalities.
 *
 ******************************************************************************/
typedef struct adc_typedef ADC_TypeDef;

// Declare peripheral structure for AES.
/***************************************************************************//**
 * @brief The `AES_TypeDef` is a typedef for a structure that represents the AES
 * (Advanced Encryption Standard) peripheral in the Silicon Labs device
 * abstraction layer, allowing for easy reference and manipulation of the
 * AES hardware features.
 *
 ******************************************************************************/
typedef struct aes_typedef AES_TypeDef;

// Declare peripheral structure for AMUXCP.
/***************************************************************************//**
 * @brief The `AMUXCP_TypeDef` is a typedef for a structure that represents the
 * AMUXCP peripheral in Silicon Labs devices, allowing for easy reference
 * and manipulation of the peripheral's properties and functions.
 *
 ******************************************************************************/
typedef struct amuxcp_typedef AMUXCP_TypeDef;

// Declare peripheral structure for BUFC.
/***************************************************************************//**
 * @brief The `BUFC_TypeDef` is a typedef for a structure that represents the
 * buffer controller peripheral in Silicon Labs devices, allowing for
 * device-specific configurations and operations.
 *
 ******************************************************************************/
typedef struct bufc_typedef BUFC_TypeDef;

// Declare peripheral structure for BURAM.
/***************************************************************************//**
 * @brief The `BURAM_TypeDef` is a typedef for a structure that represents the
 * BURAM peripheral in Silicon Labs devices, allowing for the abstraction
 * and manipulation of its associated functionalities.
 *
 ******************************************************************************/
typedef struct buram_typedef BURAM_TypeDef;

// Declare peripheral structure for BURTC.
/***************************************************************************//**
 * @brief `BURTC_TypeDef` is a typedef for a structure that represents the BURTC
 * (Backup Real-Time Counter) peripheral in Silicon Labs devices,
 * allowing for the management and interaction with the BURTC hardware.
 *
 ******************************************************************************/
typedef struct burtc_typedef BURTC_TypeDef;

// Declare peripheral structure for CMU.
/***************************************************************************//**
 * @brief `CMU_TypeDef` is a typedef for a structure that represents the Clock
 * Management Unit (CMU) in Silicon Labs devices, allowing for the
 * management and configuration of clock sources and settings.
 *
 ******************************************************************************/
typedef struct cmu_typedef CMU_TypeDef;

// Declare peripheral structure for CRYPTOACC.
/***************************************************************************//**
 * @brief `CRYPTOACC_TypeDef` is a typedef for a structure that represents the
 * Crypto Accelerator peripheral in Silicon Labs devices, allowing for
 * cryptographic operations.
 *
 ******************************************************************************/
typedef struct cryptoacc_typedef CRYPTOACC_TypeDef;

// Declare peripheral structure for CRYPTOACC_PKCTRL.
/***************************************************************************//**
 * @brief The `CRYPTOACC_PKCTRL_TypeDef` is a typedef for a structure that
 * represents the control register for the CRYPTOACC peripheral, which is
 * used for cryptographic operations in Silicon Labs devices.
 *
 ******************************************************************************/
typedef struct cryptoacc_pkctrl_typedef CRYPTOACC_PKCTRL_TypeDef;

// Declare peripheral structure for CRYPTOACC_RNGCTRL.
/***************************************************************************//**
 * @brief The `CRYPTOACC_RNGCTRL_TypeDef` is a typedef for a structure that
 * represents the control register for the Random Number Generator (RNG)
 * in the CRYPTOACC peripheral, allowing for configuration and control of
 * RNG operations.
 *
 ******************************************************************************/
typedef struct cryptoacc_rngctrl_typedef CRYPTOACC_RNGCTRL_TypeDef;

// Declare peripheral structure for DCDC.
/***************************************************************************//**
 * @brief `DCDC_TypeDef` is a typedef for a structure that represents the Direct
 * Current to Direct Current (DCDC) converter peripheral in Silicon Labs
 * devices, allowing for the management and control of power conversion
 * functionalities.
 *
 ******************************************************************************/
typedef struct dcdc_typedef DCDC_TypeDef;

// Declare peripheral structure for DEVINFO.
/***************************************************************************//**
 * @brief The `DEVINFO_TypeDef` is a typedef for a structure that represents
 * device information, typically used in embedded systems to access
 * device-specific data and configuration.
 *
 ******************************************************************************/
typedef struct devinfo_typedef DEVINFO_TypeDef;

// Declare peripheral structure for DPLL.
/***************************************************************************//**
 * @brief The `DPLL_TypeDef` is a typedef for a structure that represents the
 * Digital Phase-Locked Loop (DPLL) peripheral in Silicon Labs devices,
 * allowing for the configuration and control of the DPLL hardware.
 *
 ******************************************************************************/
typedef struct dpll_typedef DPLL_TypeDef;

// Declare peripheral structure for EMU.
/***************************************************************************//**
 * @brief The `EMU_TypeDef` is a typedef for a structure that represents the EMU
 * (Energy Management Unit) peripheral in Silicon Labs devices, allowing
 * for the management and configuration of energy-related
 * functionalities.
 *
 ******************************************************************************/
typedef struct emu_typedef EMU_TypeDef;

// Declare peripheral structure for EMU_CFGNS.
/***************************************************************************//**
 * @brief `EMU_CFGNS_TypeDef` is a typedef for a structure that represents the
 * configuration namespace for the EMU (Energy Management Unit) in
 * Silicon Labs devices, allowing for organized management of EMU
 * settings.
 *
 ******************************************************************************/
typedef struct emu_cfgns_typedef EMU_CFGNS_TypeDef;

// Declare peripheral structure for ETAMPDET.
/***************************************************************************//**
 * @brief The `ETAMPDET_TypeDef` is a typedef for a structure that represents
 * the ETAMPDET peripheral in the Silicon Labs device abstraction layer,
 * allowing for easier reference and manipulation of the peripheral's
 * properties and functions.
 *
 ******************************************************************************/
typedef struct etampdet_typedef ETAMPDET_TypeDef;

// Declare peripheral structure for EUSART.
/***************************************************************************//**
 * @brief The `EUSART_TypeDef` is a typedef for a structure that represents the
 * configuration and control registers for the Enhanced Universal
 * Synchronous/Asynchronous Receiver/Transmitter (EUSART) peripheral,
 * which is used for serial communication in embedded systems.
 *
 ******************************************************************************/
typedef struct eusart_typedef EUSART_TypeDef;

// Declare peripheral structure for FSRCO.
/***************************************************************************//**
 * @brief The `FSRCO_TypeDef` is a typedef for a structure that represents the
 * FSRCO peripheral in the Silicon Labs device abstraction layer,
 * allowing for easy reference and manipulation of the peripheral's
 * properties and functions.
 *
 ******************************************************************************/
typedef struct fsrco_typedef FSRCO_TypeDef;

// Declare peripheral structure for GPCRC.
/***************************************************************************//**
 * @brief `GPCRC_TypeDef` is a typedef for a structure that represents the
 * General Purpose CRC (Cyclic Redundancy Check) peripheral in Silicon
 * Labs devices, allowing for efficient error detection in data
 * transmission.
 *
 ******************************************************************************/
typedef struct gpcrc_typedef GPCRC_TypeDef;

// Declare peripheral structure for GPIO.
/***************************************************************************//**
 * @brief `GPIO_TypeDef` is a typedef for a structure that represents the
 * General Purpose Input/Output (GPIO) peripheral in Silicon Labs
 * devices, allowing for the manipulation and control of GPIO pins.
 *
 ******************************************************************************/
typedef struct gpio_typedef GPIO_TypeDef;

// Declare peripheral structure for HFRCO.
/***************************************************************************//**
 * @brief The `HFRCO_TypeDef` is a typedef for a structure that represents the
 * High-Frequency RC oscillator peripheral in Silicon Labs devices,
 * allowing for configuration and control of the oscillator's parameters.
 *
 ******************************************************************************/
typedef struct hfrco_typedef HFRCO_TypeDef;

// Declare peripheral structure for HFXO.
/***************************************************************************//**
 * @brief The `HFXO_TypeDef` is a typedef for a structure that represents the
 * high-frequency crystal oscillator peripheral in Silicon Labs devices,
 * allowing for the management and configuration of the oscillator's
 * settings.
 *
 ******************************************************************************/
typedef struct hfxo_typedef HFXO_TypeDef;

// Declare peripheral structure for HOSTPORTAL.
/***************************************************************************//**
 * @brief `HOSTPORTAL_TypeDef` is a typedef for a structure named
 * `hostportal_typedef`, which is likely used to define a specific
 * peripheral or component in the context of device management within the
 * Silicon Labs API.
 *
 ******************************************************************************/
typedef struct hostportal_typedef HOSTPORTAL_TypeDef;

// Declare peripheral structure for HYDRARAM.
/***************************************************************************//**
 * @brief The `HYDRARAM_TypeDef` is a typedef for a structure named
 * `hydraram_typedef`, which is likely used to represent the
 * configuration or state of a specific hardware peripheral related to
 * the HYDRARAM component in Silicon Labs devices.
 *
 ******************************************************************************/
typedef struct hydraram_typedef HYDRARAM_TypeDef;

// Declare peripheral structure for I2C.
/***************************************************************************//**
 * @brief `I2C_TypeDef` is a typedef for a structure that represents the I2C
 * peripheral in Silicon Labs devices, allowing for the configuration and
 * control of I2C communication.
 *
 ******************************************************************************/
typedef struct i2c_typedef I2C_TypeDef;

// Declare peripheral structure for IADC.
/***************************************************************************//**
 * @brief The `IADC_TypeDef` is a typedef for a structure that represents the
 * IADC (Integrated Analog-to-Digital Converter) peripheral in Silicon
 * Labs devices, allowing for the abstraction and manipulation of its
 * associated functionalities.
 *
 ******************************************************************************/
typedef struct iadc_typedef IADC_TypeDef;

// Declare peripheral structure for ICACHE.
/***************************************************************************//**
 * @brief `ICACHE_TypeDef` is a typedef for a structure that represents the
 * cache memory interface for the device, allowing for efficient data
 * retrieval and storage.
 *
 ******************************************************************************/
typedef struct icache_typedef ICACHE_TypeDef;

// Declare peripheral structure for KEYSCAN.
/***************************************************************************//**
 * @brief The `KEYSCAN_TypeDef` is a typedef for a structure that represents the
 * key scan peripheral in Silicon Labs devices, allowing for the
 * management and interaction with key scanning functionalities.
 *
 ******************************************************************************/
typedef struct keyscan_typedef KEYSCAN_TypeDef;

// Declare peripheral structure for L2CACHE.
/***************************************************************************//**
 * @brief The `L2CACHE_TypeDef` is a typedef for a structure that represents the
 * L2 cache peripheral in the Silicon Labs device architecture, allowing
 * for efficient memory access and management.
 *
 ******************************************************************************/
typedef struct l2cache_typedef L2CACHE_TypeDef;

// Declare peripheral structure for LCD.
/***************************************************************************//**
 * @brief `LCD_TypeDef` is a typedef for a structure that represents the LCD
 * peripheral in the Silicon Labs device abstraction layer, allowing for
 * easy reference and manipulation of the LCD hardware.
 *
 ******************************************************************************/
typedef struct lcd_typedef LCD_TypeDef;

// Declare peripheral structure for LCDRF.
/***************************************************************************//**
 * @brief `LCDRF_TypeDef` is a typedef for a structure named `lcdrf_typedef`,
 * which is likely used to define the properties and functionalities
 * related to the LCD Radio Frequency (LCDRF) peripheral in the context
 * of device management for Silicon Labs devices.
 *
 ******************************************************************************/
typedef struct lcdrf_typedef LCDRF_TypeDef;

// Declare peripheral structure for LDMA.
/***************************************************************************//**
 * @brief The `LDMA_TypeDef` is a typedef for a structure that represents the
 * configuration and control registers for the LDMA (Low-DMA) peripheral
 * in Silicon Labs devices, allowing for efficient data transfer
 * operations.
 *
 ******************************************************************************/
typedef struct ldma_typedef LDMA_TypeDef;

// Declare peripheral structure for LDMAXBAR.
/***************************************************************************//**
 * @brief The `LDMAXBAR_TypeDef` is a typedef for a structure named
 * `ldmaxbar_typedef`, which is likely used to define the properties and
 * functionalities of the LDMAXBAR peripheral in the Silicon Labs device
 * architecture.
 *
 ******************************************************************************/
typedef struct ldmaxbar_typedef LDMAXBAR_TypeDef;

// Declare peripheral structure for LEDDRV.
/***************************************************************************//**
 * @brief `LEDDRV_TypeDef` is a typedef for a structure that represents the LED
 * driver peripheral in Silicon Labs devices, allowing for the
 * abstraction and management of LED functionalities.
 *
 ******************************************************************************/
typedef struct leddrv_typedef LEDDRV_TypeDef;

// Declare peripheral structure for LESENSE.
/***************************************************************************//**
 * @brief The `LESENSE_TypeDef` is a typedef for a structure that represents the
 * LESENSE peripheral in Silicon Labs devices, allowing for the
 * abstraction and management of the Low Energy Sensor interface.
 *
 ******************************************************************************/
typedef struct lesense_typedef LESENSE_TypeDef;

// Declare peripheral structure for LETIMER.
/***************************************************************************//**
 * @brief The `LETIMER_TypeDef` is a typedef for a structure that represents the
 * Low Energy Timer peripheral in Silicon Labs devices, allowing for the
 * configuration and control of timer functionalities.
 *
 ******************************************************************************/
typedef struct letimer_typedef LETIMER_TypeDef;

// Declare peripheral structure for LFRCO.
/***************************************************************************//**
 * @brief The `LFRCO_TypeDef` is a typedef for a structure that represents the
 * Low-Frequency RC oscillator peripheral in Silicon Labs devices,
 * allowing for easy reference and manipulation of its associated
 * properties and functionalities.
 *
 ******************************************************************************/
typedef struct lfrco_typedef LFRCO_TypeDef;

// Declare peripheral structure for LFXO.
/***************************************************************************//**
 * @brief The `LFXO_TypeDef` is a typedef for a structure that represents the
 * Low-Frequency Crystal Oscillator (LFXO) peripheral in Silicon Labs
 * devices, allowing for the management and configuration of the LFXO
 * hardware component.
 *
 ******************************************************************************/
typedef struct lfxo_typedef LFXO_TypeDef;

// Declare peripheral structure for LPWAES.
/***************************************************************************//**
 * @brief `LPWAES_TypeDef` is a typedef for a structure named `lpwaes_typedef`,
 * which is likely used to define the properties and functionalities of a
 * low-power AES (Advanced Encryption Standard) peripheral in a device,
 * although the specific fields of the structure are not provided in the
 * given code.
 *
 ******************************************************************************/
typedef struct lpwaes_typedef LPWAES_TypeDef;

// Declare peripheral structure for LPW0PORTAL.
/***************************************************************************//**
 * @brief The `LPW0PORTAL_TypeDef` is a typedef for a structure named
 * `lpw0portal_typedef`, which is likely used to define the properties
 * and functionalities of the LPW0 portal peripheral in the Silicon Labs
 * device architecture.
 *
 ******************************************************************************/
typedef struct lpw0portal_typedef LPW0PORTAL_TypeDef;

// Declare peripheral structure for LVGD.
/***************************************************************************//**
 * @brief `LVGD_TypeDef` is a typedef for a structure named `lvgd_typedef`,
 * which is likely used to define a specific peripheral or device
 * structure in the context of the Device Manager API for Silicon Labs
 * devices.
 *
 ******************************************************************************/
typedef struct lvgd_typedef LVGD_TypeDef;

// Declare peripheral structure for MAILBOX.
/***************************************************************************//**
 * @brief The `MAILBOX_TypeDef` is a typedef for a structure that represents a
 * mailbox in the device abstraction layer, allowing for communication
 * between different components or peripherals within the system.
 *
 ******************************************************************************/
typedef struct mailbox_typedef MAILBOX_TypeDef;

// Declare peripheral structure for MPAHBRAM.
/***************************************************************************//**
 * @brief The `MPAHBRAM_TypeDef` is a typedef for a structure that represents
 * the MPAHBRAM peripheral in the Silicon Labs device architecture,
 * serving as a placeholder for the actual structure definition which is
 * not provided in the given code.
 *
 ******************************************************************************/
typedef struct mpahbram_typedef MPAHBRAM_TypeDef;

// Declare peripheral structure for MSC.
/***************************************************************************//**
 * @brief `MSC_TypeDef` is a typedef for a structure that represents the Memory
 * System Controller (MSC) peripheral in Silicon Labs devices, allowing
 * for easier reference and manipulation of the MSC hardware.
 *
 ******************************************************************************/
typedef struct msc_typedef MSC_TypeDef;

// Declare peripheral structure for MVP.
/***************************************************************************//**
 * @brief The `MVP_TypeDef` is a typedef for a structure named `mvp_typedef`,
 * which is likely used to represent a specific peripheral in the Silicon
 * Labs device abstraction layer, although the actual structure
 * definition is not provided in the code snippet.
 *
 ******************************************************************************/
typedef struct mvp_typedef MVP_TypeDef;

// Declare peripheral structure for PCNT.
/***************************************************************************//**
 * @brief `PCNT_TypeDef` is a typedef for a structure that represents the Pulse
 * Counter peripheral in Silicon Labs devices, allowing for the
 * management and configuration of pulse counting functionalities.
 *
 ******************************************************************************/
typedef struct pcnt_typedef PCNT_TypeDef;

// Declare peripheral structure for PDM.
/***************************************************************************//**
 * @brief The `PDM_TypeDef` is a typedef for a structure that represents the
 * Peripheral Device Manager, which is likely used to manage and interact
 * with various peripheral devices in a system, although the specific
 * fields of the structure are not defined in the provided code.
 *
 ******************************************************************************/
typedef struct pdm_typedef PDM_TypeDef;

// Declare peripheral structure for PFMXPPRF.
/***************************************************************************//**
 * @brief The `PFMXPPRF_TypeDef` is a typedef for a structure named
 * `pfmxpprf_typedef`, which is likely used to define a specific
 * peripheral structure in the context of device management for Silicon
 * Labs hardware.
 *
 ******************************************************************************/
typedef struct pfmxpprf_typedef PFMXPPRF_TypeDef;

// Declare peripheral structure for PIXELRZ.
/***************************************************************************//**
 * @brief The `PIXELRZ_TypeDef` is a typedef for a structure named
 * `pixelrz_typedef`, which is likely used to define the properties and
 * functionalities related to the PIXELRZ peripheral in the Silicon Labs
 * device architecture.
 *
 ******************************************************************************/
typedef struct pixelrz_typedef PIXELRZ_TypeDef;

// Declare peripheral structure for PRS.
/***************************************************************************//**
 * @brief The `PRS_TypeDef` is a typedef for a structure that represents the
 * Programmable Routing Switch (PRS) peripheral in Silicon Labs devices,
 * allowing for the configuration and management of signal routing
 * between various peripherals.
 *
 ******************************************************************************/
typedef struct prs_typedef PRS_TypeDef;

// Declare peripheral structure for RFFPLL.
/***************************************************************************//**
 * @brief The `RFFPLL_TypeDef` is a typedef for a structure that represents the
 * configuration and control registers for the RFFPLL peripheral, which
 * is part of the device's hardware abstraction layer.
 *
 ******************************************************************************/
typedef struct rffpll_typedef RFFPLL_TypeDef;

// Declare peripheral structure for RPA.
/***************************************************************************//**
 * @brief The `RPA_TypeDef` is a typedef for a structure named `rpa_typedef`,
 * which is likely used to define a specific peripheral or component in
 * the context of the Device Manager API for Silicon Labs devices.
 *
 ******************************************************************************/
typedef struct rpa_typedef RPA_TypeDef;

// Declare peripheral structure for RTCC.
/***************************************************************************//**
 * @brief `RTCC_TypeDef` is a typedef for a structure that represents the Real-
 * Time Counter and Calendar (RTCC) peripheral in Silicon Labs devices,
 * allowing for the management of timekeeping functionalities.
 *
 ******************************************************************************/
typedef struct rtcc_typedef RTCC_TypeDef;

// Declare peripheral structure for SCRATCHPAD.
/***************************************************************************//**
 * @brief The `SCRATCHPAD_TypeDef` is a typedef for a structure that represents
 * a scratchpad memory area, typically used for temporary data storage in
 * embedded systems.
 *
 ******************************************************************************/
typedef struct scratchpad_typedef SCRATCHPAD_TypeDef;

// Declare peripheral structure for SEMAILBOX_AHBHOST.
/***************************************************************************//**
 * @brief The `SEMAILBOX_AHBHOST_TypeDef` is a typedef for a structure that
 * represents the AHB host mailbox interface in the Silicon Labs device
 * architecture, allowing for communication between different components
 * of the system.
 *
 ******************************************************************************/
typedef struct semailbox_ahbhost_typedef SEMAILBOX_AHBHOST_TypeDef;

// Declare peripheral structure for SEMAILBOX_HOST.
/***************************************************************************//**
 * @brief The `SEMAILBOX_HOST_TypeDef` is a typedef for a structure that
 * represents a mailbox host in the context of device management,
 * allowing for communication between different components or peripherals
 * within a system.
 *
 ******************************************************************************/
typedef struct semailbox_host_typedef SEMAILBOX_HOST_TypeDef;

// Declare peripheral structure for SEMAPHORE.
/***************************************************************************//**
 * @brief The `SEMAPHORE_TypeDef` is a typedef for a structure that represents a
 * semaphore, which is a synchronization primitive used to control access
 * to a common resource in concurrent programming.
 *
 ******************************************************************************/
typedef struct semaphore_typedef SEMAPHORE_TypeDef;

// Declare peripheral structure for SEPORTAL.
/***************************************************************************//**
 * @brief The `SEPORTAL_TypeDef` is a typedef for a structure that represents
 * the SEPORTAL peripheral in the Silicon Labs device abstraction layer,
 * allowing for easier reference and manipulation of the peripheral's
 * properties and functions.
 *
 ******************************************************************************/
typedef struct seportal_typedef SEPORTAL_TypeDef;

// Declare peripheral structure for SEPUF_APBCFG.
/***************************************************************************//**
 * @brief The `SEPUF_APBCFG_TypeDef` is a typedef for a structure that
 * represents the configuration of the SEPUF (Secure Peripheral Unit
 * Functionality) in Silicon Labs devices, allowing for the management
 * and configuration of peripheral functionalities.
 *
 ******************************************************************************/
typedef struct sepuf_apbcfg_typedef SEPUF_APBCFG_TypeDef;

// Declare peripheral structure for SMU.
/***************************************************************************//**
 * @brief `SMU_TypeDef` is a typedef for a structure named `smu_typedef`, which
 * is likely used to define the properties and functionalities of a
 * Security Management Unit (SMU) in the context of device management for
 * Silicon Labs hardware.
 *
 ******************************************************************************/
typedef struct smu_typedef SMU_TypeDef;

// Declare peripheral structure for SOCPLL.
/***************************************************************************//**
 * @brief The `SOCPLL_TypeDef` is a typedef for a structure that represents the
 * configuration and control registers for the System-on-Chip Phase-
 * Locked Loop (SOCPLL) peripheral, allowing for the management of clock
 * signals in Silicon Labs devices.
 *
 ******************************************************************************/
typedef struct socpll_typedef SOCPLL_TypeDef;

// Declare peripheral structure for SYMCRYPTO.
/***************************************************************************//**
 * @brief The `SYMCRYPTO_TypeDef` is a typedef for a structure named
 * `symcrypto_typedef`, which is likely used to define the properties and
 * functionalities related to a symmetric cryptography peripheral in the
 * Silicon Labs device architecture.
 *
 ******************************************************************************/
typedef struct symcrypto_typedef SYMCRYPTO_TypeDef;

// Declare peripheral structure for SYSCFG.
/***************************************************************************//**
 * @brief The `SYSCFG_TypeDef` is a typedef for a structure that represents the
 * system configuration peripheral in Silicon Labs devices, allowing
 * access to various configuration settings and control registers.
 *
 ******************************************************************************/
typedef struct syscfg_typedef SYSCFG_TypeDef;

// Declare peripheral structure for SYSCFG_CFGNS.
/***************************************************************************//**
 * @brief The `SYSCFG_CFGNS_TypeDef` is a typedef for a structure that
 * represents the configuration namespace for system configuration in
 * Silicon Labs devices, allowing for organized management of peripheral
 * settings.
 *
 ******************************************************************************/
typedef struct syscfg_cfgns_typedef SYSCFG_CFGNS_TypeDef;

// Declare peripheral structure for SYSRTC.
/***************************************************************************//**
 * @brief The `SYSRTC_TypeDef` is a typedef for a structure that represents the
 * Real-Time Clock (RTC) peripheral in Silicon Labs devices, allowing for
 * the management and configuration of RTC functionalities.
 *
 ******************************************************************************/
typedef struct sysrtc_typedef SYSRTC_TypeDef;

// Declare peripheral structure for TIMER.
/***************************************************************************//**
 * @brief The `TIMER_TypeDef` is a typedef for a structure that represents a
 * timer peripheral in the Silicon Labs device abstraction layer,
 * allowing for the management and configuration of timer
 * functionalities.
 *
 ******************************************************************************/
typedef struct timer_typedef TIMER_TypeDef;

// Declare peripheral structure for ULFRCO.
/***************************************************************************//**
 * @brief The `ULFRCO_TypeDef` is a typedef for a structure that represents the
 * Ultra Low Frequency RC oscillator peripheral in Silicon Labs devices,
 * allowing for easy reference and manipulation of its associated
 * properties and functionalities.
 *
 ******************************************************************************/
typedef struct ulfrco_typedef ULFRCO_TypeDef;

// Declare peripheral structure for USART.
/***************************************************************************//**
 * @brief `USART_TypeDef` is a typedef for a structure that represents the
 * Universal Synchronous/Asynchronous Receiver/Transmitter (USART)
 * peripheral in Silicon Labs devices, allowing for serial communication.
 *
 ******************************************************************************/
typedef struct usart_typedef USART_TypeDef;

// Declare peripheral structure for USBAHB_AHBS.
/***************************************************************************//**
 * @brief The `USBAHB_AHBS_TypeDef` is a typedef for a structure that represents
 * the USB AHB (Advanced High-performance Bus) interface, which is
 * typically used in embedded systems to manage USB communication and
 * data transfer.
 *
 ******************************************************************************/
typedef struct usbahb_ahbs_typedef USBAHB_AHBS_TypeDef;

// Declare peripheral structure for USBPLL.
/***************************************************************************//**
 * @brief The `USBPLL_TypeDef` is a typedef for a structure that represents the
 * USB Phase-Locked Loop (PLL) peripheral in Silicon Labs devices,
 * allowing for the configuration and control of USB clock generation.
 *
 ******************************************************************************/
typedef struct usbpll_typedef USBPLL_TypeDef;

// Declare peripheral structure for USB_APBS.
/***************************************************************************//**
 * @brief `USB_APBS_TypeDef` is a typedef for a structure that represents the
 * USB APBS peripheral in the Silicon Labs device abstraction layer,
 * allowing for easier reference and manipulation of the USB peripheral's
 * properties and functions.
 *
 ******************************************************************************/
typedef struct usb_apbs_typedef USB_APBS_TypeDef;

// Declare peripheral structure for VDAC.
/***************************************************************************//**
 * @brief The `VDAC_TypeDef` is a typedef for a structure representing the
 * Variable Digital-to-Analog Converter (VDAC) peripheral in Silicon Labs
 * devices, allowing for easy reference and manipulation of the VDAC
 * hardware interface.
 *
 ******************************************************************************/
typedef struct vdac_typedef VDAC_TypeDef;

// Declare peripheral structure for WDOG.
/***************************************************************************//**
 * @brief `WDOG_TypeDef` is a typedef for a structure that represents a watchdog
 * timer peripheral in the Silicon Labs device abstraction layer,
 * allowing for the management and configuration of watchdog timer
 * functionalities.
 *
 ******************************************************************************/
typedef struct wdog_typedef WDOG_TypeDef;

#if defined(__ICCARM__)
// Disable IAR multiple typedefs declaration warning.
#pragma diag_default=Pe301
#endif

/// @endcond

// ----------------------------------------------------------------------------
// EXTERNS

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

// External declaration for ACMP0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_acmp0;

// External declaration for ACMP1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_acmp1;

// External declaration for ADC0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_adc0;

// External declaration for AES peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_aes;

// External declaration for AMUXCP0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_amuxcp0;

// External declaration for BUFC peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_bufc;

// External declaration for BURAM peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_buram;

// External declaration for BURTC peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_burtc;

// External declaration for CMU peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_cmu;

// External declaration for CRYPTOACC peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_cryptoacc;

// External declaration for DCDC peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_dcdc;

// External declaration for DEVINFO peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_devinfo;

// External declaration for DMEM peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_dmem;

// External declaration for DMEM0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_dmem0;

// External declaration for DMEM1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_dmem1;

// External declaration for DPLL0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_dpll0;

// External declaration for EMU peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_emu;

// External declaration for ETAMPDET peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_etampdet;

// External declaration for EUART0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_euart0;

// External declaration for EUSART0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_eusart0;

// External declaration for EUSART1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_eusart1;

// External declaration for EUSART2 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_eusart2;

// External declaration for EUSART3 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_eusart3;

// External declaration for EUSART4 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_eusart4;

// External declaration for FSRCO peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_fsrco;

// External declaration for GPCRC0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_gpcrc0;

// External declaration for GPIO peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_gpio;

// External declaration for HFRCO0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_hfrco0;

// External declaration for HFRCOEM23 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_hfrcoem23;

// External declaration for HFXO0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_hfxo0;

// External declaration for HOSTMAILBOX peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_hostmailbox;

// External declaration for HOSTPORTAL peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_hostportal;

// External declaration for I2C0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_i2c0;

// External declaration for I2C1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_i2c1;

// External declaration for I2C2 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_i2c2;

// External declaration for I2C3 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_i2c3;

// External declaration for IADC0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_iadc0;

// External declaration for ICACHE0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_icache0;

// External declaration for KEYSCAN peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_keyscan;

// External declaration for L1ICACHE0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_l1icache0;

// External declaration for L2ICACHE0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_l2icache0;

// External declaration for LCD peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lcd;

// External declaration for LCDRF peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lcdrf;

// External declaration for LDMA0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_ldma0;

// External declaration for LDMAXBAR0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_ldmaxbar0;

// External declaration for LEDDRV0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_leddrv0;

// External declaration for LESENSE peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lesense;

// External declaration for LETIMER0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_letimer0;

// External declaration for LFRCO peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lfrco;

// External declaration for LFXO peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lfxo;

// External declaration for LPWAES peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lpwaes;

// External declaration for LPW0PORTAL peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lpw0portal;

// External declaration for LVGD peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_lvgd;

// External declaration for MPAHBRAM peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_mpahbram;

// External declaration for MSC peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_msc;

// External declaration for MVP peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_mvp;

// External declaration for PCNT0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_pcnt0;

// External declaration for PDM peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_pdm;

// External declaration for PFMXPPRF peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_pfmxpprf;

// External declaration for PIXELRZ0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_pixelrz0;

// External declaration for PIXELRZ1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_pixelrz1;

// External declaration for PRORTC peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_prortc;

// External declaration for PRS peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_prs;

// External declaration for RADIOAES peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_radioaes;

// External declaration for RFFPLL0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_rffpll0;

// External declaration for RPA peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_rpa;

// External declaration for RTCC peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_rtcc;

// External declaration for SCRATCHPAD peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_scratchpad;

// External declaration for SEMAILBOX peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_semailbox;

// External declaration for SEMAPHORE0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_semaphore0;

// External declaration for SEMAPHORE1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_semaphore1;

// External declaration for SEPORTAL peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_seportal;

// External declaration for SEPUF peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_sepuf;

// External declaration for SMU peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_smu;

// External declaration for SOCPLL0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_socpll0;

// External declaration for SYMCRYPTO peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_symcrypto;

// External declaration for SYSCFG peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_syscfg;

// External declaration for SYSRTC0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_sysrtc0;

// External declaration for TIMER0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer0;

// External declaration for TIMER1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer1;

// External declaration for TIMER2 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer2;

// External declaration for TIMER3 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer3;

// External declaration for TIMER4 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer4;

// External declaration for TIMER5 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer5;

// External declaration for TIMER6 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer6;

// External declaration for TIMER7 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer7;

// External declaration for TIMER8 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer8;

// External declaration for TIMER9 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_timer9;

// External declaration for ULFRCO peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_ulfrco;

// External declaration for USART0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_usart0;

// External declaration for USART1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_usart1;

// External declaration for USART2 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_usart2;

// External declaration for USB peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_usb;

// External declaration for USBAHB peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_usbahb;

// External declaration for USBPLL0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_usbpll0;

// External declaration for VDAC0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_vdac0;

// External declaration for VDAC1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_vdac1;

// External declaration for WDOG0 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_wdog0;

// External declaration for WDOG1 peripheral structure.
extern const sl_peripheral_val_t sl_peripheral_val_wdog1;

/// @endcond

// ----------------------------------------------------------------------------
// FUNCTIONS

/***************************************************************************//**
 * @brief Retrieves the base address of the ACMP peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * ACMP peripheral.
 * @return Returns a pointer to the `ACMP_TypeDef` structure representing the
 * base address of the ACMP peripheral.
 ******************************************************************************/
inline ACMP_TypeDef *sl_device_peripheral_acmp_get_base_addr(const sl_peripheral_t peripheral)
{
  return (ACMP_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the ADC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * ADC peripheral.
 * @return Returns a pointer to the `ADC_TypeDef` structure representing the
 * base address of the ADC peripheral.
 ******************************************************************************/
inline ADC_TypeDef *sl_device_peripheral_adc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (ADC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the AES peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * AES peripheral.
 * @return Returns a pointer to the `AES_TypeDef` structure representing the
 * base address of the AES peripheral.
 ******************************************************************************/
inline AES_TypeDef *sl_device_peripheral_aes_get_base_addr(const sl_peripheral_t peripheral)
{
  return (AES_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the AMUXCP peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * AMUXCP peripheral.
 * @return Returns a pointer to the `AMUXCP_TypeDef` structure representing the
 * base address of the AMUXCP peripheral.
 ******************************************************************************/
inline AMUXCP_TypeDef *sl_device_peripheral_amuxcp_get_base_addr(const sl_peripheral_t peripheral)
{
  return (AMUXCP_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the BUFC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `BUFC_TypeDef` structure representing the
 * base address of the BUFC peripheral.
 ******************************************************************************/
inline BUFC_TypeDef *sl_device_peripheral_bufc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (BUFC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the BURAM peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * BURAM peripheral.
 * @return Returns a pointer to the `BURAM_TypeDef` structure representing the
 * base address of the BURAM peripheral.
 ******************************************************************************/
inline BURAM_TypeDef *sl_device_peripheral_buram_get_base_addr(const sl_peripheral_t peripheral)
{
  return (BURAM_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the BURTC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * BURTC peripheral.
 * @return Returns a pointer to the `BURTC_TypeDef` structure, which represents
 * the base address of the BURTC peripheral.
 ******************************************************************************/
inline BURTC_TypeDef *sl_device_peripheral_burtc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (BURTC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified peripheral's clock
 * management unit (CMU).
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `CMU_TypeDef` structure representing the
 * base address of the specified peripheral's clock management unit.
 ******************************************************************************/
inline CMU_TypeDef *sl_device_peripheral_cmu_get_base_addr(const sl_peripheral_t peripheral)
{
  return (CMU_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `CRYPTOACC` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `CRYPTOACC_TypeDef` structure representing
 * the base address of the `CRYPTOACC` peripheral.
 ******************************************************************************/
inline CRYPTOACC_TypeDef *sl_device_peripheral_cryptoacc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (CRYPTOACC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `CRYPTOACC_PKCTRL` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `CRYPTOACC_PKCTRL_TypeDef` structure, which
 * represents the base address of the `CRYPTOACC_PKCTRL` peripheral.
 ******************************************************************************/
inline CRYPTOACC_PKCTRL_TypeDef *sl_device_peripheral_cryptoacc_pkctrl_get_base_addr(const sl_peripheral_t peripheral)
{
  return (CRYPTOACC_PKCTRL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `CRYPTOACC_RNGCTRL` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `CRYPTOACC_RNGCTRL_TypeDef` structure
 * representing the base address of the `CRYPTOACC_RNGCTRL` peripheral.
 ******************************************************************************/
inline CRYPTOACC_RNGCTRL_TypeDef *sl_device_peripheral_cryptoacc_rngctrl_get_base_addr(const sl_peripheral_t peripheral)
{
  return (CRYPTOACC_RNGCTRL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the DCDC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * DCDC peripheral.
 * @return Returns a pointer to the `DCDC_TypeDef` structure representing the
 * base address of the DCDC peripheral.
 ******************************************************************************/
inline DCDC_TypeDef *sl_device_peripheral_dcdc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (DCDC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified peripheral device.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral device.
 * @return Returns a pointer to the `DEVINFO_TypeDef` structure representing the
 * base address of the specified peripheral.
 ******************************************************************************/
inline DEVINFO_TypeDef *sl_device_peripheral_devinfo_get_base_addr(const sl_peripheral_t peripheral)
{
  return (DEVINFO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified DPLL peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * specific peripheral for which the base address is
 * requested.
 * @return Returns a pointer to the `DPLL_TypeDef` structure representing the
 * base address of the DPLL peripheral.
 ******************************************************************************/
inline DPLL_TypeDef *sl_device_peripheral_dpll_get_base_addr(const sl_peripheral_t peripheral)
{
  return (DPLL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the EMU peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral for which the base address is requested.
 * @return Returns a pointer to the `EMU_TypeDef` structure representing the
 * base address of the EMU peripheral.
 ******************************************************************************/
inline EMU_TypeDef *sl_device_peripheral_emu_get_base_addr(const sl_peripheral_t peripheral)
{
  return (EMU_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the EMU_CFGNS peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral for which the base address is to be retrieved.
 * @return Returns a pointer to the `EMU_CFGNS_TypeDef` structure representing
 * the base address of the EMU_CFGNS peripheral.
 ******************************************************************************/
inline EMU_CFGNS_TypeDef *sl_device_peripheral_emu_cfgns_get_base_addr(const sl_peripheral_t peripheral)
{
  return (EMU_CFGNS_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `ETAMPDET` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `ETAMPDET_TypeDef` structure, which
 * represents the base address of the `ETAMPDET` peripheral.
 ******************************************************************************/
inline ETAMPDET_TypeDef *sl_device_peripheral_etampdet_get_base_addr(const sl_peripheral_t peripheral)
{
  return (ETAMPDET_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the specified EUSART peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * EUSART peripheral.
 * @return Returns a pointer to the `EUSART_TypeDef` structure representing the
 * base address of the EUSART peripheral.
 ******************************************************************************/
inline EUSART_TypeDef *sl_device_peripheral_eusart_get_base_addr(const sl_peripheral_t peripheral)
{
  return (EUSART_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the FSRCO peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * FSRCO peripheral.
 * @return Returns a pointer to the `FSRCO_TypeDef` structure, which represents
 * the base address of the FSRCO peripheral.
 ******************************************************************************/
inline FSRCO_TypeDef *sl_device_peripheral_fsrco_get_base_addr(const sl_peripheral_t peripheral)
{
  return (FSRCO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the GPCRC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * GPCRC peripheral.
 * @return Returns a pointer to the `GPCRC_TypeDef` structure, which represents
 * the base address of the GPCRC peripheral.
 ******************************************************************************/
inline GPCRC_TypeDef *sl_device_peripheral_gpcrc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (GPCRC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified GPIO peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * GPIO peripheral.
 * @return Returns a pointer to the `GPIO_TypeDef` structure representing the
 * base address of the GPIO peripheral.
 ******************************************************************************/
inline GPIO_TypeDef *sl_device_peripheral_gpio_get_base_addr(const sl_peripheral_t peripheral)
{
  return (GPIO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the HFRCO peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * HFRCO peripheral.
 * @return Returns a pointer to the `HFRCO_TypeDef` structure, which represents
 * the base address of the HFRCO peripheral.
 ******************************************************************************/
inline HFRCO_TypeDef *sl_device_peripheral_hfrco_get_base_addr(const sl_peripheral_t peripheral)
{
  return (HFRCO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Returns the base address of the HFXO peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * HFXO peripheral.
 * @return Returns a pointer to the `HFXO_TypeDef` structure representing the
 * base address of the HFXO peripheral.
 ******************************************************************************/
inline HFXO_TypeDef *sl_device_peripheral_hfxo_get_base_addr(const sl_peripheral_t peripheral)
{
  return (HFXO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `HOSTPORTAL` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `HOSTPORTAL_TypeDef` structure, which
 * represents the base address of the `HOSTPORTAL` peripheral.
 ******************************************************************************/
inline HOSTPORTAL_TypeDef *sl_device_peripheral_hostportal_get_base_addr(const sl_peripheral_t peripheral)
{
  return (HOSTPORTAL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `HYDRARAM` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * specific peripheral.
 * @return Returns a pointer to the `HYDRARAM_TypeDef` structure representing
 * the base address of the `HYDRARAM` peripheral.
 ******************************************************************************/
inline HYDRARAM_TypeDef *sl_device_peripheral_hydraram_get_base_addr(const sl_peripheral_t peripheral)
{
  return (HYDRARAM_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the specified I2C peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * I2C peripheral.
 * @return Returns a pointer to the `I2C_TypeDef` structure representing the
 * base address of the I2C peripheral.
 ******************************************************************************/
inline I2C_TypeDef *sl_device_peripheral_i2c_get_base_addr(const sl_peripheral_t peripheral)
{
  return (I2C_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the IADC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * IADC peripheral.
 * @return Returns a pointer to the `IADC_TypeDef` structure, which represents
 * the base address of the IADC peripheral.
 ******************************************************************************/
inline IADC_TypeDef *sl_device_peripheral_iadc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (IADC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the ICACHE peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * ICACHE peripheral.
 * @return Returns a pointer to the `ICACHE_TypeDef` structure representing the
 * base address of the ICACHE peripheral.
 ******************************************************************************/
inline ICACHE_TypeDef *sl_device_peripheral_icache_get_base_addr(const sl_peripheral_t peripheral)
{
  return (ICACHE_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the KEYSCAN peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `KEYSCAN_TypeDef` structure, which
 * represents the base address of the KEYSCAN peripheral.
 ******************************************************************************/
inline KEYSCAN_TypeDef *sl_device_peripheral_keyscan_get_base_addr(const sl_peripheral_t peripheral)
{
  return (KEYSCAN_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the L2 cache peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * specific peripheral from which the base address is to be
 * retrieved.
 * @return Returns a pointer to the `L2CACHE_TypeDef` structure representing the
 * base address of the L2 cache peripheral.
 ******************************************************************************/
inline L2CACHE_TypeDef *sl_device_peripheral_l2cache_get_base_addr(const sl_peripheral_t peripheral)
{
  return (L2CACHE_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LCD peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * LCD peripheral.
 * @return Returns a pointer to the `LCD_TypeDef` structure representing the
 * base address of the LCD peripheral.
 ******************************************************************************/
inline LCD_TypeDef *sl_device_peripheral_lcd_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LCD_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `LCDRF` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `LCDRF_TypeDef` structure, which represents
 * the base address of the `LCDRF` peripheral.
 ******************************************************************************/
inline LCDRF_TypeDef *sl_device_peripheral_lcdrf_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LCDRF_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LDMA peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral for which the base address is requested.
 * @return Returns a pointer to the `LDMA_TypeDef` structure representing the
 * base address of the LDMA peripheral.
 ******************************************************************************/
inline LDMA_TypeDef *sl_device_peripheral_ldma_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LDMA_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LDMAXBAR peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral for which the base address is requested.
 * @return Returns a pointer to the `LDMAXBAR_TypeDef` structure representing
 * the base address of the LDMAXBAR peripheral.
 ******************************************************************************/
inline LDMAXBAR_TypeDef *sl_device_peripheral_ldmaxbar_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LDMAXBAR_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LED driver peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * LED driver peripheral.
 * @return Returns a pointer to the `LEDDRV_TypeDef` structure, which represents
 * the base address of the LED driver peripheral.
 ******************************************************************************/
inline LEDDRV_TypeDef *sl_device_peripheral_leddrv_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LEDDRV_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `LESENSE` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `LESENSE_TypeDef` structure, which
 * represents the base address of the `LESENSE` peripheral.
 ******************************************************************************/
inline LESENSE_TypeDef *sl_device_peripheral_lesense_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LESENSE_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LETIMER peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * LETIMER peripheral.
 * @return Returns a pointer to the `LETIMER_TypeDef` structure, which
 * represents the base address of the LETIMER peripheral.
 ******************************************************************************/
inline LETIMER_TypeDef *sl_device_peripheral_letimer_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LETIMER_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LFRCO peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * LFRCO peripheral.
 * @return Returns a pointer of type `LFRCO_TypeDef*` that points to the base
 * address of the LFRCO peripheral.
 ******************************************************************************/
inline LFRCO_TypeDef *sl_device_peripheral_lfrco_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LFRCO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LFXO peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * LFXO peripheral.
 * @return Returns a pointer to the `LFXO_TypeDef` structure representing the
 * base address of the LFXO peripheral.
 ******************************************************************************/
inline LFXO_TypeDef *sl_device_peripheral_lfxo_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LFXO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LPWAES peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * LPWAES peripheral.
 * @return Returns a pointer to the `LPWAES_TypeDef` structure representing the
 * base address of the LPWAES peripheral.
 ******************************************************************************/
inline LPWAES_TypeDef *sl_device_peripheral_lpwaes_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LPWAES_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `LPW0PORTAL` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `LPW0PORTAL_TypeDef` structure, which
 * represents the base address of the `LPW0PORTAL` peripheral.
 ******************************************************************************/
inline LPW0PORTAL_TypeDef *sl_device_peripheral_lpw0portal_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LPW0PORTAL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the LVGD peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * LVGD peripheral.
 * @return Returns a pointer to the `LVGD_TypeDef` structure, which represents
 * the base address of the LVGD peripheral.
 ******************************************************************************/
inline LVGD_TypeDef *sl_device_peripheral_lvgd_get_base_addr(const sl_peripheral_t peripheral)
{
  return (LVGD_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified peripheral mailbox.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `MAILBOX_TypeDef` structure representing the
 * base address of the specified mailbox peripheral.
 ******************************************************************************/
inline MAILBOX_TypeDef *sl_device_peripheral_mailbox_get_base_addr(const sl_peripheral_t peripheral)
{
  return (MAILBOX_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the MPAHBRAM peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `MPAHBRAM_TypeDef` structure representing
 * the base address of the MPAHBRAM peripheral.
 ******************************************************************************/
inline MPAHBRAM_TypeDef *sl_device_peripheral_mpahbram_get_base_addr(const sl_peripheral_t peripheral)
{
  return (MPAHBRAM_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the Memory System Controller (MSC)
 * peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * specific peripheral from which the base address is to be
 * retrieved.
 * @return Returns a pointer to the `MSC_TypeDef` structure, which represents
 * the base address of the MSC peripheral.
 ******************************************************************************/
inline MSC_TypeDef *sl_device_peripheral_msc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (MSC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the specified peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `MVP_TypeDef` structure, which represents
 * the base address of the MVP peripheral.
 ******************************************************************************/
inline MVP_TypeDef *sl_device_peripheral_mvp_get_base_addr(const sl_peripheral_t peripheral)
{
  return (MVP_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the specified peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `PCNT_TypeDef` structure representing the
 * base address of the peripheral.
 ******************************************************************************/
inline PCNT_TypeDef *sl_device_peripheral_pcnt_get_base_addr(const sl_peripheral_t peripheral)
{
  return (PCNT_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the PDM peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * PDM peripheral.
 * @return Returns a pointer to the `PDM_TypeDef` structure representing the
 * base address of the PDM peripheral.
 ******************************************************************************/
inline PDM_TypeDef *sl_device_peripheral_pdm_get_base_addr(const sl_peripheral_t peripheral)
{
  return (PDM_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `PFMXPPRF` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `PFMXPPRF_TypeDef` structure, which
 * represents the base address of the `PFMXPPRF` peripheral.
 ******************************************************************************/
inline PFMXPPRF_TypeDef *sl_device_peripheral_pfmxpprf_get_base_addr(const sl_peripheral_t peripheral)
{
  return (PFMXPPRF_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `PIXELRZ` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `PIXELRZ_TypeDef` structure, which
 * represents the base address of the `PIXELRZ` peripheral.
 ******************************************************************************/
inline PIXELRZ_TypeDef *sl_device_peripheral_pixelrz_get_base_addr(const sl_peripheral_t peripheral)
{
  return (PIXELRZ_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the specified peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `PRS_TypeDef` structure representing the
 * base address of the specified peripheral.
 ******************************************************************************/
inline PRS_TypeDef *sl_device_peripheral_prs_get_base_addr(const sl_peripheral_t peripheral)
{
  return (PRS_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the RFFPLL peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `RFFPLL_TypeDef` structure representing the
 * base address of the RFFPLL peripheral.
 ******************************************************************************/
inline RFFPLL_TypeDef *sl_device_peripheral_rffpll_get_base_addr(const sl_peripheral_t peripheral)
{
  return (RFFPLL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the RPA peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * RPA peripheral.
 * @return Returns a pointer to the `RPA_TypeDef` structure, which represents
 * the base address of the RPA peripheral.
 ******************************************************************************/
inline RPA_TypeDef *sl_device_peripheral_rpa_get_base_addr(const sl_peripheral_t peripheral)
{
  return (RPA_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the RTCC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * RTCC peripheral.
 * @return Returns a pointer to the `RTCC_TypeDef` structure, which represents
 * the base address of the RTCC peripheral.
 ******************************************************************************/
inline RTCC_TypeDef *sl_device_peripheral_rtcc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (RTCC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the SCRATCHPAD peripheral.
 *
 * @param peripheral A pointer to the `sl_peripheral_t` structure representing
 * the specific peripheral.
 * @return Returns a pointer to the `SCRATCHPAD_TypeDef` structure, which
 * represents the base address of the SCRATCHPAD peripheral.
 ******************************************************************************/
inline SCRATCHPAD_TypeDef *sl_device_peripheral_scratchpad_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SCRATCHPAD_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `SEMAILBOX_AHBHOST` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `SEMAILBOX_AHBHOST_TypeDef` structure
 * representing the base address of the specified peripheral.
 ******************************************************************************/
inline SEMAILBOX_AHBHOST_TypeDef *sl_device_peripheral_semailbox_ahbhost_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SEMAILBOX_AHBHOST_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the SEMAILBOX host peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `SEMAILBOX_HOST_TypeDef` structure
 * representing the base address of the SEMAILBOX host peripheral.
 ******************************************************************************/
inline SEMAILBOX_HOST_TypeDef *sl_device_peripheral_semailbox_host_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SEMAILBOX_HOST_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified semaphore peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * semaphore peripheral.
 * @return Returns a pointer to the `SEMAPHORE_TypeDef` structure representing
 * the base address of the semaphore peripheral.
 ******************************************************************************/
inline SEMAPHORE_TypeDef *sl_device_peripheral_semaphore_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SEMAPHORE_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the specified `SEPORTAL` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `SEPORTAL_TypeDef` structure representing
 * the base address of the specified peripheral.
 ******************************************************************************/
inline SEPORTAL_TypeDef *sl_device_peripheral_seportal_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SEPORTAL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the SEPUF_APBCFG peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral for which the base address is to be retrieved.
 * @return Returns a pointer to the `SEPUF_APBCFG_TypeDef` structure
 * representing the base address of the SEPUF_APBCFG peripheral.
 ******************************************************************************/
inline SEPUF_APBCFG_TypeDef *sl_device_peripheral_sepuf_apbcfg_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SEPUF_APBCFG_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the specified peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `SMU_TypeDef` structure, which represents
 * the base address of the SMU peripheral.
 ******************************************************************************/
inline SMU_TypeDef *sl_device_peripheral_smu_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SMU_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the SOCPLL peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * specific peripheral.
 * @return Returns a pointer to the `SOCPLL_TypeDef` structure, which represents
 * the base address of the SOCPLL peripheral.
 ******************************************************************************/
inline SOCPLL_TypeDef *sl_device_peripheral_socpll_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SOCPLL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `SYMCRYPTO` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `SYMCRYPTO_TypeDef` structure, which
 * represents the base address of the `SYMCRYPTO` peripheral.
 ******************************************************************************/
inline SYMCRYPTO_TypeDef *sl_device_peripheral_symcrypto_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SYMCRYPTO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral whose base address is to be retrieved.
 * @return Returns a pointer to the `SYSCFG_TypeDef` structure representing the
 * base address of the specified peripheral.
 ******************************************************************************/
inline SYSCFG_TypeDef *sl_device_peripheral_syscfg_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SYSCFG_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the SYSCFG_CFGNS peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral for which the base address is to be retrieved.
 * @return Returns a pointer to the `SYSCFG_CFGNS_TypeDef` structure
 * representing the base address of the SYSCFG_CFGNS peripheral.
 ******************************************************************************/
inline SYSCFG_CFGNS_TypeDef *sl_device_peripheral_syscfg_cfgns_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SYSCFG_CFGNS_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the SYSRTC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `SYSRTC_TypeDef` structure representing the
 * base address of the SYSRTC peripheral.
 ******************************************************************************/
inline SYSRTC_TypeDef *sl_device_peripheral_sysrtc_get_base_addr(const sl_peripheral_t peripheral)
{
  return (SYSRTC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified timer peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * timer peripheral.
 * @return Returns a pointer to the `TIMER_TypeDef` structure representing the
 * base address of the specified timer peripheral.
 ******************************************************************************/
inline TIMER_TypeDef *sl_device_peripheral_timer_get_base_addr(const sl_peripheral_t peripheral)
{
  return (TIMER_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the `ULFRCO` peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the base address is to be retrieved.
 * @return Returns a pointer to the `ULFRCO_TypeDef` structure, which represents
 * the base address of the `ULFRCO` peripheral.
 ******************************************************************************/
inline ULFRCO_TypeDef *sl_device_peripheral_ulfrco_get_base_addr(const sl_peripheral_t peripheral)
{
  return (ULFRCO_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of a specified USART peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * USART peripheral.
 * @return Returns a pointer to the `USART_TypeDef` structure representing the
 * base address of the specified USART peripheral.
 ******************************************************************************/
inline USART_TypeDef *sl_device_peripheral_usart_get_base_addr(const sl_peripheral_t peripheral)
{
  return (USART_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the USBAHB_AHBS peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral for which the base address is to be retrieved.
 * @return Returns a pointer to the `USBAHB_AHBS_TypeDef` structure representing
 * the base address of the USBAHB_AHBS peripheral.
 ******************************************************************************/
inline USBAHB_AHBS_TypeDef *sl_device_peripheral_usbahb_ahbs_get_base_addr(const sl_peripheral_t peripheral)
{
  return (USBAHB_AHBS_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the USBPLL peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * USBPLL peripheral.
 * @return Returns a pointer to the `USBPLL_TypeDef` structure representing the
 * base address of the USBPLL peripheral.
 ******************************************************************************/
inline USBPLL_TypeDef *sl_device_peripheral_usbpll_get_base_addr(const sl_peripheral_t peripheral)
{
  return (USBPLL_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the USB APBS peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * USB peripheral.
 * @return Returns a pointer to the `USB_APBS_TypeDef` structure representing
 * the base address of the USB APBS peripheral.
 ******************************************************************************/
inline USB_APBS_TypeDef *sl_device_peripheral_usb_apbs_get_base_addr(const sl_peripheral_t peripheral)
{
  return (USB_APBS_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the VDAC peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * VDAC peripheral.
 * @return Returns a pointer to the `VDAC_TypeDef` structure representing the
 * base address of the VDAC peripheral.
 ******************************************************************************/
inline VDAC_TypeDef *sl_device_peripheral_vdac_get_base_addr(const sl_peripheral_t peripheral)
{
  return (VDAC_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * @brief Retrieves the base address of the watchdog peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * specific peripheral from which the base address is to be
 * retrieved.
 * @return Returns a pointer to the `WDOG_TypeDef` structure, which represents
 * the base address of the watchdog peripheral.
 ******************************************************************************/
inline WDOG_TypeDef *sl_device_peripheral_wdog_get_base_addr(const sl_peripheral_t peripheral)
{
  return (WDOG_TypeDef *)peripheral->base;
}

/***************************************************************************//**
 * The clock branch getter.
 *
 * @param peripheral  A pointer to peripheral.
 *
 * @return  The clock branch of the peripheral.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_DEVICE_PERIPHERAL, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief Retrieves the clock branch associated with a specified peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which to retrieve the clock branch.
 * @return Returns the `sl_clock_branch_t` value representing the clock branch
 * of the specified peripheral.
 ******************************************************************************/
inline sl_clock_branch_t sl_device_peripheral_get_clock_branch(const sl_peripheral_t peripheral)
{
  return peripheral->clk_branch;
}

/***************************************************************************//**
 * @brief Retrieves the bus clock associated with a specified peripheral.
 *
 * @param peripheral A pointer to a `sl_peripheral_t` structure representing the
 * peripheral from which the bus clock is to be retrieved.
 * @return Returns the bus clock of type `sl_bus_clock_t` associated with the
 * specified peripheral.
 ******************************************************************************/
inline sl_bus_clock_t sl_device_peripheral_get_bus_clock(const sl_peripheral_t peripheral)
{
  return peripheral->bus_clock;
}

/** @} (end addtogroup device_peripheral) */

#ifdef __cplusplus
}
#endif

#endif // SL_DEVICE_PERIPHERAL_H
