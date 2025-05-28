/***************************************************************************//**
 * @file
 * @brief Utility and convenience functions for EFM32 microcontroller.
 * See @ref micro for documentation.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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
 * @{
 ******************************************************************************/

/** @addtogroup micro
 * See also hal/micro/cortexm3/micro.h for source code.
 *@{
 */

#ifndef __EFM_MICRO_H__
#define __EFM_MICRO_H__

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef __MICRO_H__
#error do not include this file directly - include micro/micro.h
#endif

#include "em_device.h"

// Define the priority registers of system handlers.

// For Cortex-M faults and exceptions
#define HANDLER_PRIORITY_REGISTER(handler) \
  (*(((uint8_t *)SCS_SHPR_7to4_ADDR) + handler##_VECTOR_INDEX - 4))

// The reset types have both a base type and an extended type.  The extended
//  type is a 16-bit value which has the base type in the upper 8-bits, and the
//  extended classification in the lower 8-bits
// For backwards compatibility with other platforms, only the base type is
//  returned by the halGetResetInfo() API.  For the full extended type, the
//  halGetExtendedResetInfo() API should be called.

#define RESET_BASE_TYPE(extendedType)   ((uint8_t)(((extendedType) >> 8) & 0xFF))
#define RESET_EXTENDED_FIELD(extendedType) ((uint8_t)((extendedType) & 0xFF))
#define RESET_VALID_SIGNATURE           (0xF00F)
#define RESET_INVALID_SIGNATURE         (0xC33C)

// Define the base reset cause types
#define RESET_BASE_DEF(basename, value, string)  RESET_##basename = value,
#define RESET_EXT_DEF(basename, extname, extvalue, string)     /*nothing*/
enum {
  #include "reset-def.h"
  NUM_RESET_BASE_TYPES
};

#undef RESET_BASE_DEF
#undef RESET_EXT_DEF

// Define the extended reset cause types
#define RESET_EXT_VALUE(basename, extvalue) \
  (((RESET_##basename) << 8) + extvalue)
#define RESET_BASE_DEF(basename, value, string)    /*nothing*/
#define RESET_EXT_DEF(basename, extname, extvalue, string) \
  RESET_##basename##_##extname = RESET_EXT_VALUE(basename, extvalue),
enum {
  #include "reset-def.h"
};

#undef RESET_EXT_VALUE
#undef RESET_BASE_DEF
#undef RESET_EXT_DEF

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @name Vector Table Index Definitions
 * These are numerical definitions for vector table. Only Cortex-M standard
 * exception vectors (indices 0 through 15) are represented since device
 * specific vectors can be (and often are) different across dies.
 *@{
 */

/**
 * @brief A numerical definition for a vector.
 */
#define STACK_VECTOR_INDEX          0U         ///< special case: stack pointer at reset
#define RESET_VECTOR_INDEX          1U             ///< reset vector index
#define NMI_VECTOR_INDEX            2U             ///< nmi vector index
#define HARD_FAULT_VECTOR_INDEX     3U             ///< hard fault vector index
#define MEMORY_FAULT_VECTOR_INDEX   4U             ///< memory fault vector index
#define BUS_FAULT_VECTOR_INDEX      5U             ///< bus fault vector index
#define USAGE_FAULT_VECTOR_INDEX    6U             ///< usage fault vector index
#define RESERVED07_VECTOR_INDEX     7U             ///< reserved07 vector index
#define RESERVED08_VECTOR_INDEX     8U             ///< reserved08 vector index
#define RESERVED09_VECTOR_INDEX     9U             ///< reserved09 vector index
#define RESERVED10_VECTOR_INDEX     10U            ///< reserved10 vector index
#define SVCALL_VECTOR_INDEX         11U            ///< svcall vector index
#define DEBUG_MONITOR_VECTOR_INDEX  12U            ///< debug monitor vector index
#define RESERVED13_VECTOR_INDEX     13U            ///< reserved13 vector index
#define PENDSV_VECTOR_INDEX         14U            ///< pendsv vector index
#define SYSTICK_VECTOR_INDEX        15U            ///< systick vector index

/**
 * @brief Utility macro to convert from IRQ numbers to exception numbers/
 * vector indices.
 *
 * These are different because the latter include the Cortex-M standard
 * exceptions while the former do not.
 */
#define IRQ_TO_VECTOR_NUMBER(x)     ((x) + 16U)

/**
 * @brief Number of vectors.
 *
 * EXT_IRQ_COUNT is defined in the device header but does not include the
 * Cortex-M standard exceptions.
 */
#define VECTOR_TABLE_LENGTH         (IRQ_TO_VECTOR_NUMBER(EXT_IRQ_COUNT))

/** @}  Vector Table Index Definitions */

/**
 * @brief EM2xx-compatible reset code returned by halGetEm2xxResetInfo()
 */
#define EM2XX_RESET_UNKNOWN               0      ///< EN2XX reset unknow
#define EM2XX_RESET_EXTERNAL              1        ///< EM2XX reports POWERON instead
#define EM2XX_RESET_POWERON               2        ///< EM2XX reset poweron
#define EM2XX_RESET_WATCHDOG              3        ///< EM2XX reset watchdog
#define EM2XX_RESET_ASSERT                6        ///< EM2XX reset assert
#define EM2XX_RESET_BOOTLOADER            9        ///< EM2XX reset bootloader
#define EM2XX_RESET_SOFTWARE              11       ///< EM2XX reset software

/***************************************************************************//**
 * @brief This function is used to record a specific reset cause and then
 * immediately trigger a system reset on the microcontroller. It is
 * typically called when a reset is required due to a specific condition
 * or event that needs to be logged for diagnostic purposes. The function
 * must be called with a valid extended reset cause code, which combines
 * both a base type and an extended classification. This function should
 * be used with caution, as it will immediately reset the system,
 * interrupting any ongoing processes.
 *
 * @param extendedCause A 16-bit value representing the extended reset cause.
 * The upper 8 bits contain the base type, and the lower 8
 * bits contain the extended classification. The caller
 * must ensure this value is valid and meaningful for the
 * system's reset cause logging.
 * @return None
 ******************************************************************************/
void halInternalSysReset(uint16_t extendedCause);

/***************************************************************************//**
 * @brief This function provides a 16-bit code that identifies both the base and
 * extended cause of the last reset event. It is useful for diagnosing
 * the reason for a reset, which can include various hardware and
 * software triggers. This function should be called when detailed reset
 * information is required, beyond what is provided by simpler reset
 * cause functions. It is particularly useful in debugging scenarios
 * where understanding the specific reset cause is critical.
 *
 * @return A 16-bit unsigned integer representing the base and extended reset
 * cause.
 ******************************************************************************/
uint16_t halGetExtendedResetInfo(void);

/***************************************************************************//**
 * @brief This function retrieves the reset cause of the system and translates
 * it into a format compatible with the EM2XX HAL. It should be used when
 * a platform-independent reset code is required, particularly for EZSP
 * host applications. The function maps known reset causes to their EM2XX
 * equivalents and returns them. If the reset cause is not recognized or
 * not supported by EM2XX, the function returns the platform-specific
 * code with the most significant bit set. This function assumes that the
 * system has already been initialized and that reset information is
 * available.
 *
 * @return The function returns an 8-bit unsigned integer representing the
 * EM2XX-compatible reset code. If the reset cause is not supported by
 * EM2XX, the return value is the platform-specific code with the most
 * significant bit set.
 ******************************************************************************/
uint8_t halGetEm2xxResetInfo(void);

/***************************************************************************//**
 * @brief This function provides a human-readable string that describes the
 * extended cause of the last reset event for diagnostic purposes. It
 * should be used after system initialization to understand the specific
 * reset condition that occurred. The function relies on the extended
 * reset information obtained from `halGetExtendedResetInfo()`. It is
 * useful for logging or debugging to provide insight into the reset
 * causes. The function assumes that the reset information is valid and
 * mapped to a corresponding string.
 *
 * @return A pointer to a constant character string that describes the extended
 * reset cause.
 ******************************************************************************/
/***************************************************************************//**
 * @brief The `halGetExtendedResetString` function returns a pointer to a string
 * that describes the extended cause of a reset event in the EFM32
 * microcontroller. This function is part of the utility and convenience
 * functions provided for the EFM32 microcontroller, specifically for
 * diagnostic purposes.
 *
 * @details This function is used to obtain a human-readable string that
 * describes the extended reset cause, which can be useful for logging
 * or debugging purposes.
 ******************************************************************************/
const char * halGetExtendedResetString(void);

#endif //__EFM_MICRO_H__

/** @} (end addtogroup micro) */
/** @} (end addtogroup legacyhal) */
