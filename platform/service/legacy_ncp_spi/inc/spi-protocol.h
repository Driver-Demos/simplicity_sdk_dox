/***************************************************************************//**
 * @file
 * @brief Internal SPI Protocol implementation for use below the EZSP
 * application.
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
#ifndef __SPI_PROTOCOL_H__
#define __SPI_PROTOCOL_H__

// SPI prefix bytes
#define SPIP_RESET               0x00
#define SPIP_OVERSIZED_EZSP      0x01
#define SPIP_ABORTED_TRANSACTION 0x02
#define SPIP_MISSING_FT          0x03
#define SPIP_UNSUPPORTED_COMMAND 0x04
#define SPIP_VERSION             0x82
#define SPIP_ALIVE               0xC1

// All prefixes less than or eqaul to this are error responses.
#define SPIP_MAX_ERROR_PREFIX    SPIP_UNSUPPORTED_COMMAND

// The frame terminator byte.
#define SPIP_FRAME_TERMINATOR    0xA7

// Legacy name for the above.
#define FRAME_TERMINATOR         SPIP_FRAME_TERMINATOR

/***************************************************************************//**
 * @brief The `halHostFrame` is a global pointer to a byte that represents the
 * start of the payload length in the SPI protocol buffer. It is used by
 * upper layers to read commands and write responses during SPI
 * communication.
 *
 * @details This variable is used as the primary access point for upper layers
 * to interact with the command/response buffer in the SPI protocol.
 ******************************************************************************/
extern uint8_t *halHostFrame;

/***************************************************************************//**
 * @brief The `spipFlagWakeFallingEdge` is a global boolean variable that
 * indicates whether the Host has initiated a wake handshake. This flag
 * is set to true when the wake handshake process begins.
 *
 * @details This variable is used to track the state of the wake handshake
 * process in the SPI protocol implementation.
 ******************************************************************************/
extern bool spipFlagWakeFallingEdge;

/***************************************************************************//**
 * @brief This function prepares the SPI protocol for communication by
 * initializing necessary flags and buffers. It should be called during
 * the system startup to ensure that the SPI protocol is ready for
 * operation. The function sets up the initial state, including error
 * response buffers and command buffers, and powers up the serial
 * interface. It is essential to call this function before any SPI
 * communication to ensure proper protocol operation.
 *
 * @return None
 ******************************************************************************/
void halHostSerialInit(void);

/***************************************************************************//**
 * @brief This function should be called to reinitialize the SPI protocol when
 * the system wakes up from a low-power state. It configures the
 * necessary GPIO pins and SPI settings to ensure proper communication.
 * This function must be called after the system has been powered up and
 * before any SPI communication is attempted. It handles the setup of
 * interrupts and GPIO configurations to manage noise and crosstalk,
 * ensuring reliable operation.
 *
 * @return None
 ******************************************************************************/
void halHostSerialPowerup(void);

/***************************************************************************//**
 * @brief This function is used to safely shut down the SPI protocol interface
 * when the system is entering a low-power sleep mode. It ensures that
 * any ongoing SPI transmissions are completed before disabling the SPI
 * driver and associated interrupts. This function should be called
 * before the system enters sleep to prevent data loss or corruption. It
 * is important to ensure that the SPI protocol is not in use by other
 * processes when this function is called.
 *
 * @return None
 ******************************************************************************/
void halHostSerialPowerdown(void);

/***************************************************************************//**
 * @brief This function is used by the upper application to notify the host
 * about pending callbacks by manipulating the nHOST_INT signal. It
 * should be called with 'haveData' set to true when there is a callback
 * to deliver, prompting the SPIP to schedule the assertion of nHOST_INT.
 * Conversely, it should be called with 'haveData' set to false when
 * there are no more callbacks pending, allowing the SPIP to deassert
 * nHOST_INT. The function ensures proper timing and multiplexing of the
 * nHOST_INT signal with other SPIP-generated assertions. It is important
 * to call this function appropriately to maintain correct communication
 * with the host.
 *
 * @param haveData A boolean value where true indicates that there is a callback
 * pending and nHOST_INT should be asserted, while false
 * indicates no pending callbacks and nHOST_INT should be
 * deasserted.
 * @return None
 ******************************************************************************/
void halHostCallback(bool haveData);

/***************************************************************************//**
 * @brief Use this function to determine if the SPI Protocol Interface (SPIP) is
 * currently engaged in an operation and cannot accept new commands. This
 * is useful for ensuring that commands are only sent when the interface
 * is ready, preventing potential data corruption or communication
 * errors. The function returns a boolean value indicating the busy
 * status of the SPIP. It is important to check this status before
 * attempting to initiate communication with the SPIP.
 *
 * @return A boolean value: true if the SPIP is busy, false otherwise.
 ******************************************************************************/
bool halHostSerialBusy(void);

/***************************************************************************//**
 * @brief Use this function to clear the wake flag of the Network Co-Processor
 * (NCP) to indicate that the nWAKE handshake has been received at the
 * application or EZSP level. This function should be called when the
 * wake handshake process is complete, ensuring that the system state
 * accurately reflects the handshake status. It is important to ensure
 * that this function is called in the appropriate context where the wake
 * flag needs to be reset, as it directly affects the communication state
 * between the host and the NCP.
 *
 * @return None
 ******************************************************************************/
void halNcpClearWakeFlag(void);

/**
 * Test function used by haltest. Nothing to see here...
 *
 * @param test
 *
 * @param params
 */
void spipTest(uint16_t test, uint16_t params);

#endif // __SPI_PROTOCOL_H__
