/***************************************************************************//**
 * @file
 * @brief See @ref crc for detailed documentation.
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
#ifndef __CRC_H__
#define __CRC_H__

/***************************************************************************//**
 * @addtogroup legacyhal
 * @{
 ******************************************************************************/

/** @addtogroup crc Cyclic Redundancy Check
 * @brief Functions that provide access to cyclic redundancy code (CRC)
 * calculation. See crc.h for source code.
 *@{
 */

/***************************************************************************//**
 * @brief This function computes a 16-bit CRC using the CITT CRC 16 polynomial
 * on a single byte of data. It is designed to be called iteratively,
 * starting with an initial CRC value, and then repeatedly for each byte
 * of data to be processed. This allows for the accumulation of the CRC
 * result over a sequence of bytes, which is useful for error-checking in
 * data transmission or storage. The function is suitable for use in
 * environments where hardware CRC calculation is not available or
 * desired.
 *
 * @param newByte The new byte to be processed by the CRC calculation. It is an
 * 8-bit unsigned integer representing the next piece of data in
 * the sequence.
 * @param prevResult The previous CRC result, which is a 16-bit unsigned
 * integer. This value should be the result of the previous
 * call to this function or an initial CRC value for the first
 * call.
 * @return The function returns a 16-bit unsigned integer representing the
 * updated CRC result after processing the new byte.
 ******************************************************************************/
uint16_t halCommonCrc16(uint8_t newByte, uint16_t prevResult);

/***************************************************************************//**
 * @brief This function computes a 32-bit CRC using a specified polynomial on a
 * single byte of data, which is useful for error-checking in data
 * transmission or storage. It should be called initially with a starting
 * CRC value, typically `CRC32_START`, and then repeatedly for each byte
 * of data to be processed. The function is designed to be used
 * iteratively, updating the CRC value with each call. It is important to
 * ensure that the initial value and each subsequent call's result are
 * correctly managed to maintain the integrity of the CRC calculation.
 *
 * @param newByte The new byte to be processed in the CRC calculation. It is an
 * 8-bit unsigned integer and should represent the next byte of
 * data in the sequence.
 * @param prevResult The previous CRC result, which is a 32-bit unsigned
 * integer. This value should be the result of the last CRC
 * calculation or an initial value like `CRC32_START` for the
 * first call.
 * @return The function returns a 32-bit unsigned integer representing the
 * updated CRC result after processing the new byte.
 ******************************************************************************/
uint32_t halCommonCrc32(uint8_t newByte, uint32_t prevResult);

// Commonly used initial and expected final CRC32 values
#define INITIAL_CRC             0xFFFFFFFFL        ///< initial crc
#define CRC32_START             INITIAL_CRC        ///< crc32 start
#define CRC32_END               0xDEBB20E3L  ///< For CRC32 POLYNOMIAL run LSB-MSB

/** @} (end addtogroup crc) */
/** @} (end addtogroup legacyhal) */

#endif //__CRC_H__
