/***************************************************************************//**
 * @file
 * @brief See @ref spi_protocol and micro specific modules for documentation.
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
/** @addtogroup spi_protocol
 * @brief Example host common SPI Protocol implementation for
 * interfacing with a NCP.
 *
 * For complete documentation of the SPI Protocol, refer to the NCP docs.
 *
 * @note The micro specific definitions, @ref stm32f103ret_spip, is
 * chosen by the build include path pointing at the appropriate directoy.
 *
 * See spi-protocol-common.h for source code.
 *@{
 */

#ifndef __SPI_PROTOCOL_COMMON_H__
#define __SPI_PROTOCOL_COMMON_H__

#include "app/util/ezsp/ezsp-enum.h"

/***************************************************************************//**
 * @brief The `halNcpFrame` is a global pointer variable that points to the
 * length byte at the start of the payload in the command/response
 * buffer. It is used by upper layers to write commands before starting a
 * transaction and to read responses after a transaction completes.
 *
 * @details `halNcpFrame` is used as the primary access point for upper layers
 * to interact with the command/response buffer in the SPI protocol.
 ******************************************************************************/
extern uint8_t *halNcpFrame;

/***************************************************************************//**
 * @brief The `halNcpSpipErrorByte` is a global variable of type `uint8_t` that
 * represents an error byte in a special SPI Protocol error case. It
 * provides additional details about the error encountered during SPI
 * communication with the Network Co-Processor (NCP).
 *
 * @details This variable is used to store and convey detailed error information
 * in the SPI Protocol, aiding in debugging and development.
 ******************************************************************************/
extern uint8_t halNcpSpipErrorByte;

/***************************************************************************//**
 * @brief This function sets up the necessary GPIO and SPI configurations to
 * enable communication with the Network Co-Processor (NCP) using the SPI
 * protocol. It should be called during the initialization phase of the
 * application to prepare the hardware interfaces for subsequent
 * operations. The function configures GPIO pins for chip select,
 * interrupt, reset, and wake signals, and opens the SPI device for
 * communication. It is essential to call this function before any SPI
 * transactions with the NCP to ensure that all hardware interfaces are
 * correctly initialized.
 *
 * @return None
 ******************************************************************************/
void halNcpSerialInit(void);

/***************************************************************************//**
 * @brief This function should be called to reinitialize the SPI Protocol when
 * the system is waking up from a low-power state, such as sleep or
 * powerdown. It ensures that the protocol is ready to handle
 * communication with the Network Co-Processor (NCP) by checking the
 * state of the host interrupt line. This function is typically used in
 * systems where power management is crucial, and the SPI Protocol needs
 * to be reestablished after the system resumes operation.
 *
 * @return None
 ******************************************************************************/
void halNcpSerialPowerup(void);

/**
 * @brief Shuts down the SPI Protocol when entering sleep (powerdown).
 */
void halNcpSerialPowerdown(void);

/***************************************************************************//**
 * @brief This function is used to perform a hard reset on the Network Co-
 * Processor (NCP) by manipulating the nRESET line. It ensures that the
 * NCP has successfully booted, is active, and that the SPI Protocol
 * version is correct. This function should be called when a complete
 * reset of the NCP is necessary, and it guarantees that the NCP is ready
 * to accept commands upon completion. It does not allow the NCP to enter
 * bootload mode, unlike its counterpart function that accepts a bootload
 * request parameter.
 *
 * @return Returns a sl_zigbee_ezsp_status_t value indicating the success or
 * failure of the reset operation.
 ******************************************************************************/
sl_zigbee_ezsp_status_t halNcpHardReset(void);

/***************************************************************************//**
 * @brief This function is used to reset the Network Co-Processor (NCP) by
 * manipulating the nRESET and nWAKE lines. It can optionally request the
 * NCP to enter bootload mode based on the state of the `requestBootload`
 * parameter. This function should be called when a hard reset of the NCP
 * is required, and it ensures that the NCP is ready to accept commands
 * after the reset. The function verifies the NCP's boot status,
 * activity, and SPI Protocol version. It returns a status indicating the
 * success or failure of the operation.
 *
 * @param requestBootload A boolean value indicating whether to request the NCP
 * to enter bootload mode. If true, the NCP is requested
 * to enter bootload mode; if false, it continues with a
 * normal boot.
 * @return Returns a sl_zigbee_ezsp_status_t value indicating the success or
 * failure of the reset operation. Possible return values include
 * success, startup timeout, or startup failure.
 ******************************************************************************/
sl_zigbee_ezsp_status_t halNcpHardResetReqBootload(bool requestBootload);

/***************************************************************************//**
 * @brief This function is used to wake up the Network Co-Processor (NCP) when
 * the host believes it is asleep. It initiates a wake-up handshake,
 * which may take several milliseconds to complete. The function returns
 * immediately, and the completion of the wake-up process is signaled by
 * a call to `halNcpIsAwakeIsr()`. This function should be used when the
 * host needs to ensure the NCP is ready to accept commands, typically
 * after a period of inactivity or power-down.
 *
 * @return None
 ******************************************************************************/
void halNcpWakeUp(void);

/***************************************************************************//**
 * @brief Use this function to send a command to the Network Co-Processor (NCP)
 * when the command is formatted as an EZSP frame. It sets the SPI Byte
 * to indicate an EZSP Frame and then transmits the command. This
 * function should be called after writing the command into the command
 * buffer. After calling this function, the transaction enters a wait
 * state, and the caller must repeatedly invoke `halNcpPollForResponse()`
 * to receive the response. This function is specifically for EZSP
 * commands; for other types of commands, use `halNcpSendRawCommand()`.
 *
 * @return None
 ******************************************************************************/
void halNcpSendCommand(void);

/***************************************************************************//**
 * @brief Use this function to send a raw command to the Network Co-Processor
 * (NCP) without any assumptions about the data format in the SPI buffer.
 * It is intended for cases where the command does not conform to the
 * standard EZSP frame format. This function should be called after the
 * command has been written into the command buffer. It returns
 * immediately after the command transmission is complete, and the caller
 * must then repeatedly call `halNcpPollForResponse()` to receive the
 * response. Ensure that the command buffer is correctly populated and
 * that the payload does not exceed the maximum allowed size to avoid
 * errors.
 *
 * @return None
 ******************************************************************************/
void halNcpSendRawCommand(void);

/***************************************************************************//**
 * @brief This function is used to repeatedly check for a response from the
 * Network Co-Processor (NCP) after a command has been sent using the SPI
 * protocol. It should be called in a loop until a response is received
 * or an error occurs. The function handles various error conditions,
 * such as timeouts and oversized responses, and returns a status
 * indicating the result of the polling operation. It is essential to
 * call this function after sending a command to ensure that the response
 * is properly received and processed.
 *
 * @return Returns a sl_zigbee_ezsp_status_t value indicating the success or
 * failure of the response polling operation.
 ******************************************************************************/
sl_zigbee_ezsp_status_t halNcpPollForResponse(void);

/**
 * @brief The SPI Protocol calls halNcpIsAwakeIsr() once the wakeup
 * handshaking is complete and the NCP is ready to accept a command.
 *
 * @param isAwake  true if the wake handshake completed and the NCP is awake.
 * false is the wake handshake failed and the NCP is unresponsive.
 */
void halNcpIsAwakeIsr(bool isAwake);

/***************************************************************************//**
 * @brief Use this function to determine if the Network Co-Processor (NCP) has
 * data ready to be processed by the host. This is typically called by
 * the host to check for pending callbacks from the NCP. The function
 * should be called when the host needs to know if it should expect
 * incoming data from the NCP. It returns a boolean indicating the
 * presence of data, which can be used to trigger further actions such as
 * reading the data from the NCP.
 *
 * @return Returns a boolean value: true if the NCP has data pending, false
 * otherwise.
 ******************************************************************************/
bool halNcpHasData(void);

/***************************************************************************//**
 * @brief This function is used to ensure that the SPI Protocol version used by
 * the host matches the version expected by the Network Co-Processor
 * (NCP). It sends a specific command to the NCP and waits for a
 * response. If the response matches the expected version, the function
 * returns true, indicating compatibility. This function should be called
 * when establishing communication with the NCP to ensure protocol
 * compatibility. It is important for maintaining proper communication
 * and avoiding protocol mismatches.
 *
 * @return Returns true if the SPI Protocol version matches the expected
 * version, otherwise returns false.
 ******************************************************************************/
bool halNcpVerifySpiProtocolVersion(void);

/***************************************************************************//**
 * @brief This function is used to check whether the SPI Protocol is currently
 * active by sending a specific command and evaluating the response. It
 * should be called when there is a need to confirm the active status of
 * the SPI Protocol, such as during initialization or troubleshooting.
 * The function returns a boolean indicating the active status, where
 * 'true' signifies that the protocol is active and 'false' indicates it
 * is not. It is important to ensure that the SPI interface is properly
 * initialized before calling this function to avoid erroneous results.
 *
 * @return Returns 'true' if the SPI Protocol is active, otherwise 'false'.
 ******************************************************************************/
bool halNcpVerifySpiProtocolActive(void);

/***************************************************************************//**
 * @brief Use this function to obtain the file descriptor associated with the
 * nHOST_INT line, which the Network Co-Processor (NCP) uses to signal
 * the host when it has data to transmit. This function is typically
 * called when setting up or managing communication with the NCP to
 * monitor its interrupt line. Ensure that the SPI Protocol is properly
 * initialized before calling this function to avoid invalid file
 * descriptor values.
 *
 * @return Returns a valid file descriptor for the nHOST_INT line, or -1 if the
 * descriptor is not available.
 ******************************************************************************/
int halNcpGetIntFd(void);

//@}

#endif // __SPI_PROTOCOL_COMMON_H__

/** @} END addtogroup */
