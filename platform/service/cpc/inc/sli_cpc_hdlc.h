/***************************************************************************/ /**
 * @file
 * @brief CPC HDLC
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

#ifndef SLI_CPC_HDLC_H
#define SLI_CPC_HDLC_H

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "sl_status.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#define SLI_CPC_HDLC_HEADER_SIZE      5
#define SLI_CPC_HDLC_HEADER_RAW_SIZE  7

#define SLI_CPC_HDLC_FLAG_VAL  0x14

#define SLI_CPC_HDLC_FLAG_POS     0
#define SLI_CPC_HDLC_ADDRESS_POS  1
#define SLI_CPC_HDLC_LENGTH_POS   2
#define SLI_CPC_HDLC_CONTROL_POS  4
#define SLI_CPC_HDLC_HCS_POS      5

#define SLI_CPC_HDLC_FRAME_TYPE_INFORMATION   0
#define SLI_CPC_HDLC_FRAME_TYPE_SUPERVISORY   2
#define SLI_CPC_HDLC_FRAME_TYPE_UNNUMBERED    3

#define SLI_CPC_HDLC_CONTROL_FRAME_TYPE_SHIFT  6
#define SLI_CPC_HDLC_CONTROL_P_F_SHIFT  3
#define SLI_CPC_HDLC_CONTROL_SEQ_SHIFT  4
#define SLI_CPC_HDLC_CONTROL_SUPERVISORY_FNCT_ID_SHIFT  4
#define SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_SHIFT  0

#define SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_MASK  0x37

#define SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_INFORMATION  0x00
#define SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_POLL_FINAL   0x04
#define SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_RESET_SEQ    0x31
#define SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_ACKNOWLEDGE  0x0E

#define SLI_CPC_HDLC_ACK_SUPERVISORY_FUNCTION      0
#define SLI_CPC_HDLC_REJECT_SUPERVISORY_FUNCTION   1
#define SLI_CPC_HDLC_REJECT_PAYLOAD_SIZE  1

#define SLI_CPC_HDLC_FCS_SIZE  2

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_flag` retrieves the flag value from a
 * specified position in an HDLC header buffer.
 *
 * @param header_buf A pointer to a buffer containing the HDLC header from which
 * the flag value is to be extracted.
 * @return The function returns an 8-bit unsigned integer representing the flag
 * value from the HDLC header.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_flag(const uint8_t *header_buf)
{
  return header_buf[SLI_CPC_HDLC_FLAG_POS];
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_address` retrieves the address value
 * from a given HDLC header buffer.
 *
 * @param header_buf A pointer to a buffer containing the HDLC header from which
 * the address value is to be extracted.
 * @return The function returns an 8-bit unsigned integer representing the
 * address value extracted from the HDLC header.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_address(const uint8_t *header_buf)
{
  return header_buf[SLI_CPC_HDLC_ADDRESS_POS];
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_length` extracts and returns the
 * payload length from an HDLC header buffer.
 *
 * @param header_buf A pointer to a buffer containing the HDLC header from which
 * the payload length is to be extracted.
 * @return The function returns a 16-bit unsigned integer representing the
 * payload length extracted from the HDLC header.
 ******************************************************************************/
static inline uint16_t sli_cpc_hdlc_get_length(const uint8_t *header_buf)
{
  uint16_t length = 0;

  length  = (uint16_t)header_buf[SLI_CPC_HDLC_LENGTH_POS];
  length |= (uint16_t)header_buf[SLI_CPC_HDLC_LENGTH_POS + 1] << 8;

  return length;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_control` retrieves the control value
 * from a specified position in an HDLC header buffer.
 *
 * @param header_buf A pointer to a buffer containing the HDLC header from which
 * the control value is to be extracted.
 * @return The function returns an 8-bit unsigned integer representing the
 * control value extracted from the HDLC header.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_control(const uint8_t *header_buf)
{
  return header_buf[SLI_CPC_HDLC_CONTROL_POS];
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_hcs` extracts the Header Check Sequence
 * (HCS) value from a given HDLC header buffer.
 *
 * @param header_buf A pointer to a buffer containing the HDLC header from which
 * the HCS value is to be extracted.
 * @return The function returns a 16-bit unsigned integer representing the HCS
 * value extracted from the HDLC header.
 ******************************************************************************/
static inline uint16_t sli_cpc_hdlc_get_hcs(const uint8_t *header_buf)
{
  uint16_t hcs = 0;

  hcs  = (uint16_t)header_buf[SLI_CPC_HDLC_HCS_POS];
  hcs |= (uint16_t)header_buf[SLI_CPC_HDLC_HCS_POS + 1] << 8;

  return hcs;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_fcs` extracts the Frame Check Sequence
 * (FCS) from a given payload buffer in an HDLC frame.
 *
 * @param payload_buf A pointer to the buffer containing the payload from which
 * the FCS is to be extracted.
 * @param payload_length The length of the payload, used to locate the position
 * of the FCS within the buffer.
 * @return The function returns a 16-bit unsigned integer representing the FCS
 * extracted from the payload buffer.
 ******************************************************************************/
static inline uint16_t sli_cpc_hdlc_get_fcs(const uint8_t *payload_buf, uint16_t payload_length)
{
  uint16_t fcs = 0;

  fcs  = (uint16_t)payload_buf[payload_length];
  fcs |= (uint16_t)payload_buf[payload_length + 1] << 8;

  return fcs;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_frame_type` extracts and determines the
 * frame type from a given HDLC control byte.
 *
 * @param control A uint8_t representing the control value specified in the HDLC
 * header.
 * @return The function returns a uint8_t representing the HDLC frame type,
 * which is either the extracted type or
 * SLI_CPC_HDLC_FRAME_TYPE_INFORMATION if the extracted type is 0 or 1.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_frame_type(uint8_t control)
{
  uint8_t type = control >> SLI_CPC_HDLC_CONTROL_FRAME_TYPE_SHIFT;

  if (type == 1 || type == 0) {
    type = SLI_CPC_HDLC_FRAME_TYPE_INFORMATION;
  }

  return type;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_seq` extracts the sequence number from
 * a given HDLC control byte.
 *
 * @param control A uint8_t value representing the control byte from an HDLC
 * header.
 * @return The function returns a uint8_t value representing the sequence number
 * extracted from the control byte.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_seq(uint8_t control)
{
  return (control >> SLI_CPC_HDLC_CONTROL_SEQ_SHIFT) & 0x07;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_ack` extracts the acknowledgment (ACK)
 * value from a given HDLC control byte.
 *
 * @param control A uint8_t representing the control byte from an HDLC header,
 * which contains various control information including the ACK
 * value.
 * @return The function returns a uint8_t value representing the ACK value
 * extracted from the control byte.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_ack(uint8_t control)
{
  return control & 0x07;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_supervisory_function` extracts the
 * supervisory function ID from a given HDLC control byte.
 *
 * @param control A uint8_t value representing the control byte from an HDLC
 * header.
 * @return The function returns a uint8_t value representing the supervisory
 * function ID extracted from the control byte.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_supervisory_function(uint8_t control)
{
  return (control >> SLI_CPC_HDLC_CONTROL_SUPERVISORY_FNCT_ID_SHIFT) & 0x03;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_get_unnumbered_type` extracts the
 * unnumbered type from a given HDLC control byte.
 *
 * @param control A uint8_t value representing the control byte from an HDLC
 * header.
 * @return The function returns a uint8_t value representing the unnumbered type
 * extracted from the control byte.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_get_unnumbered_type(uint8_t control)
{
  return (control >> SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_SHIFT) & SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_MASK;
}

/***************************************************************************//**
 * @brief This function constructs an HDLC header and writes it into the
 * provided buffer. It should be used when preparing data for
 * transmission over an HDLC link. The function requires a buffer to
 * store the header, an address, a payload length, a control value, and a
 * flag indicating whether to compute the CRC for the header. The buffer
 * must be large enough to hold the header, and the function will
 * populate it with the appropriate values. If compute_crc is true, the
 * function calculates and includes the CRC in the header; otherwise, it
 * assumes the CRC will be handled externally. This function does not
 * return a value, and it is the caller's responsibility to ensure the
 * buffer is valid and sufficiently sized.
 *
 * @param header_buf Pointer to a buffer where the HDLC header will be written.
 * Must not be null and must have space for at least 7 bytes
 * if compute_crc is true, otherwise 5 bytes.
 * @param address The address value to be included in the header. It is an 8-bit
 * unsigned integer.
 * @param length The length of the payload, specified as a 16-bit unsigned
 * integer.
 * @param control The control value to be included in the header, specified as
 * an 8-bit unsigned integer.
 * @param compute_crc Boolean flag indicating whether to compute and include the
 * CRC in the header. If true, the CRC is computed and
 * included; if false, it is assumed to be handled
 * externally.
 * @return None
 ******************************************************************************/
void sli_cpc_hdlc_create_header(uint8_t *header_buf,
                                uint8_t address,
                                uint16_t length,
                                uint8_t control,
                                bool compute_crc);

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_create_control_data` constructs an HDLC
 * control byte using sequence, acknowledgment, and poll/final bit
 * values.
 *
 * @param seq The sequence number to be included in the control byte, shifted to
 * the appropriate position.
 * @param ack The acknowledgment number to be included in the control byte.
 * @param poll_final A boolean indicating whether the poll/final bit should be
 * set in the control byte.
 * @return The function returns a uint8_t representing the constructed HDLC
 * control byte.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_create_control_data(uint8_t seq, uint8_t ack, bool poll_final)
{
  uint8_t control = SLI_CPC_HDLC_FRAME_TYPE_INFORMATION << SLI_CPC_HDLC_CONTROL_FRAME_TYPE_SHIFT;

  control |= seq << SLI_CPC_HDLC_CONTROL_SEQ_SHIFT;
  control |= ack;
  control |= (uint8_t)((uint8_t)poll_final << SLI_CPC_HDLC_CONTROL_P_F_SHIFT);

  return control;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_create_control_supervisory` constructs a
 * control byte for a supervisory HDLC frame using the provided
 * acknowledgment and supervisory function identifiers.
 *
 * @param ack An 8-bit unsigned integer representing the acknowledgment value to
 * be included in the control byte.
 * @param supervisory_function An 8-bit unsigned integer representing the
 * supervisory function identifier to be included in
 * the control byte.
 * @return The function returns an 8-bit unsigned integer representing the
 * constructed control byte for a supervisory HDLC frame.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_create_control_supervisory(uint8_t ack, uint8_t supervisory_function)
{
  uint8_t control = SLI_CPC_HDLC_FRAME_TYPE_SUPERVISORY << SLI_CPC_HDLC_CONTROL_FRAME_TYPE_SHIFT;

  control |= supervisory_function << SLI_CPC_HDLC_CONTROL_SUPERVISORY_FNCT_ID_SHIFT;
  control |= ack;

  return control;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_create_control_unnumbered` creates a
 * control value for an HDLC unnumbered frame by combining a frame type
 * and a specific unnumbered type.
 *
 * @param type A `uint8_t` representing the specific unnumbered type to be
 * encoded in the control value.
 * @return The function returns a `uint8_t` representing the control value for
 * an HDLC unnumbered frame.
 ******************************************************************************/
static inline uint8_t sli_cpc_hdlc_create_control_unnumbered(uint8_t type)
{
  uint8_t control = SLI_CPC_HDLC_FRAME_TYPE_UNNUMBERED << SLI_CPC_HDLC_CONTROL_FRAME_TYPE_SHIFT;

  control |= (uint8_t)(type << SLI_CPC_HDLC_CONTROL_UNNUMBERED_TYPE_SHIFT);

  return control;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_is_poll_final` checks if the Poll/Final
 * (P/F) bit is set in the HDLC control byte.
 *
 * @param control A `uint8_t` value representing the control byte from an HDLC
 * header.
 * @return A boolean value: `true` if the Poll/Final bit is set in the control
 * byte, otherwise `false`.
 ******************************************************************************/
static inline bool sli_cpc_hdlc_is_poll_final(uint8_t control)
{
  if (control & (1 << SLI_CPC_HDLC_CONTROL_P_F_SHIFT)) {
    return true;
  }
  return false;
}

/***************************************************************************//**
 * @brief The function `sli_cpc_hdlc_set_control_ack` updates the ACK number in
 * a frame's control header by modifying specific bits.
 *
 * @param control A pointer to a uint8_t variable representing the control byte
 * of an HDLC frame header.
 * @param ack A uint8_t value representing the new ACK number to be set in the
 * control byte.
 * @return The function does not return a value; it modifies the control byte in
 * place.
 ******************************************************************************/
static inline void sli_cpc_hdlc_set_control_ack(uint8_t *control,
                                                uint8_t ack)
{
  *control &= (uint8_t) ~0x07;
  *control |= ack;
}

/***************************************************************************//**
 * @brief This function is used to determine the payload size from a given HDLC
 * header. It should be called when you need to extract the payload
 * length from a header that follows the HDLC protocol. The function
 * first checks if the header contains a valid flag and a correct header
 * checksum (HCS). If either of these checks fail, the function returns
 * -1, indicating an invalid header. This function is useful in scenarios
 * where you need to validate and process HDLC frames.
 *
 * @param header A pointer to a buffer containing the HDLC header. The buffer
 * must not be null and should be at least
 * SLI_CPC_HDLC_HEADER_SIZE bytes long. The function expects the
 * header to contain a valid flag and checksum; otherwise, it
 * returns -1.
 * @return Returns the payload size as an integer if the header is valid, or -1
 * if the header is invalid.
 ******************************************************************************/
int sli_cpc_hdlc_extract_payload_size(const uint8_t *header);

/** @} (end addtogroup cpc) */

#ifdef __cplusplus
}
#endif

#endif // SLI_CPC_HDLC_H
