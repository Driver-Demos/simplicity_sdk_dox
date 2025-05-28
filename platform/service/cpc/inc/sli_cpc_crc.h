/***************************************************************************/ /**
 * @file
 * @brief CPC CRC Definitions
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

#ifndef SLI_CPC_CRC_H
#define SLI_CPC_CRC_H

#include "sl_status.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************//**
 * @brief This function calculates the CRC-16 XMODEM checksum for a specified
 * buffer using a software implementation. It is typically used to verify
 * data integrity by generating a checksum that can be compared against
 * an expected value. The function requires a valid buffer and its length
 * as inputs. It is important to ensure that the buffer pointer is not
 * null and that the buffer length accurately reflects the size of the
 * data to be processed. The function returns the computed CRC value,
 * which can be used for further validation or comparison.
 *
 * @param buffer Pointer to the buffer on which the CRC must be computed. Must
 * not be null, and the caller retains ownership.
 * @param buffer_length Length of the buffer in bytes. Must accurately represent
 * the size of the data to be processed.
 * @return Returns the computed CRC-16 XMODEM checksum as a 16-bit unsigned
 * integer.
 ******************************************************************************/
uint16_t sli_cpc_get_crc_sw(const uint8_t* buffer, uint16_t buffer_length);

/***************************************************************************//**
 * @brief This function calculates the CRC-16 XMODEM checksum for a given data
 * buffer, incorporating an additional security tag into the computation.
 * It is useful in scenarios where data integrity needs to be verified
 * with an extra layer of security. The function requires both the data
 * buffer and the security tag to be provided along with their respective
 * lengths. It is important to ensure that the pointers to the buffer and
 * security tag are valid and that their lengths accurately reflect the
 * size of the data to avoid undefined behavior.
 *
 * @param buffer Pointer to the data buffer for which the CRC is to be computed.
 * Must not be null, and the data it points to should be valid for
 * the specified length.
 * @param buffer_length The length of the data buffer in bytes. Must accurately
 * represent the size of the data to be processed.
 * @param security_tag Pointer to the security tag buffer to be included in the
 * CRC computation. Must not be null, and the data it points
 * to should be valid for the specified length.
 * @param security_tag_length The length of the security tag buffer in bytes.
 * Must accurately represent the size of the security
 * tag data to be processed.
 * @return Returns the computed CRC-16 XMODEM value as a 16-bit unsigned
 * integer.
 ******************************************************************************/
uint16_t sli_cpc_get_crc_sw_with_security(const uint8_t *buffer, uint16_t buffer_length,
                                          const uint8_t *security_tag, uint16_t security_tag_length);

/***************************************************************************//**
 * @brief Use this function to verify that the CRC-16 XMODEM checksum of a given
 * buffer matches an expected value. This is useful for ensuring data
 * integrity when transmitting or storing data. The function requires a
 * buffer and its length, along with the expected CRC value. It returns a
 * boolean indicating whether the computed CRC matches the expected CRC.
 * Ensure that the buffer is not null and that the buffer length is
 * correctly specified to avoid undefined behavior.
 *
 * @param buffer Pointer to the buffer containing the data for which the CRC is
 * to be validated. Must not be null.
 * @param buffer_length The length of the buffer in bytes. Must accurately
 * reflect the size of the buffer to ensure correct CRC
 * computation.
 * @param expected_crc The expected CRC-16 XMODEM value to compare against the
 * computed CRC of the buffer.
 * @return Returns true if the computed CRC matches the expected CRC, false
 * otherwise.
 ******************************************************************************/
bool sli_cpc_validate_crc_sw(const uint8_t* buffer, uint16_t buffer_length, uint16_t expected_crc);

#ifdef __cplusplus
}
#endif

#endif // SLI_CPC_CRC_H
