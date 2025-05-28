/***************************************************************************//**
 * @file
 * @brief Header for ASH Network Co-Processor
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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
#ifndef __ASH_NCP_H__
#define __ASH_NCP_H__

/***************************************************************************//**
 * @brief This function is used to allocate a static buffer that is held until
 * the function `ashReleaseStaticBuffers()` is called. It is intended for
 * use in ASH network communication, providing a buffer chain that can be
 * utilized by higher layers. This function should be called when a
 * static buffer is needed for network operations, and it is important to
 * ensure that `ashReleaseStaticBuffers()` is called to release the
 * buffer when it is no longer needed. The function returns the head of
 * the allocated buffer chain, which can be used for further operations.
 *
 * @return Returns a `sli_buffer_manager_buffer_t` which is the head of the
 * allocated buffer chain.
 ******************************************************************************/
sli_buffer_manager_buffer_t ashAllocateStaticBuffers(void);

/**
 *  Releases the buffers allocated by ashAllocateStaticBuffers()
 *  if none are in use.
 *
 *  @return bool value
 */
bool ashReleaseStaticBuffers(void);

/***************************************************************************//**
 * @brief This function sets up the ASH protocol and prepares the serial port
 * for communication. It should be called to initialize the system before
 * any ASH communication can occur. The function handles necessary
 * configurations and ensures that the system is ready to send and
 * receive data frames. It is important to call this function after a
 * system reset to ensure proper initialization. The function also
 * manages internal timers and error states to ensure reliable
 * communication.
 *
 * @return None
 ******************************************************************************/
void ashStart(void);

/***************************************************************************//**
 * @brief This function is used to send a data frame to the host system. It
 * should be called when a frame is ready to be transmitted. The function
 * requires a buffer containing the data frame and a flag indicating
 * whether the frame should be sent with high priority. The buffer must
 * contain a valid data frame with a length within the acceptable range.
 * If the high priority flag is set, the frame is queued in a high-
 * priority queue, otherwise, it is placed in a standard queue. This
 * function assumes that the buffer is properly initialized and managed
 * by the caller.
 *
 * @param buffer An index of a linked buffer containing the data frame to send.
 * The buffer must not be null and must contain a valid data frame
 * with a length between ASH_MIN_DATA_FIELD_LEN and
 * ASH_MAX_DATA_FIELD_LEN. The caller retains ownership of the
 * buffer.
 * @param highPriority A boolean flag indicating whether the frame should be
 * sent with high priority. If true, the frame is added to
 * the high-priority queue; otherwise, it is added to the
 * standard queue.
 * @return None
 ******************************************************************************/
void ashSend(sli_buffer_manager_buffer_t buffer, bool highPriority);

/***************************************************************************//**
 * @brief This function is responsible for handling the transmission of data
 * frames and other control frames to the host using the ASH protocol. It
 * should be called when there is a need to manage outgoing data frames,
 * acknowledgments, and other control signals. The function handles
 * retransmissions, acknowledgments, and error conditions internally. It
 * is expected to be called in a context where the ASH protocol is
 * already initialized and running. The function does not take any
 * parameters and does not return any value, but it interacts with the
 * ASH protocol's internal state and queues.
 *
 * @return None
 ******************************************************************************/
void ashSendExec(void);

/***************************************************************************//**
 * @brief This function attempts to receive a DATA frame from the ASH protocol.
 * It should be called when the application needs to check for incoming
 * data. The function will block until a complete frame is received or an
 * error occurs. It is important to ensure that the ASH protocol is
 * initialized and running before calling this function. The caller is
 * responsible for managing the memory of the buffer where the frame is
 * stored, including freeing it when no longer needed.
 *
 * @param outBuf A pointer to a variable where the index of the linked buffers
 * containing a complete data frame will be written. The caller
 * must ensure this pointer is valid and is responsible for
 * freeing the buffers when appropriate. If no frame is returned,
 * the index is set to SL_ZIGBEE_NULL_MESSAGE_BUFFER.
 * @return The function returns a status code indicating the result of the
 * operation. Possible return values include SL_ZIGBEE_EZSP_SUCCESS,
 * SL_ZIGBEE_EZSP_ASH_IN_PROGRESS, SL_ZIGBEE_EZSP_NO_RX_DATA,
 * SL_ZIGBEE_EZSP_NO_RX_SPACE, and SL_ZIGBEE_EZSP_ASH_NCP_FATAL_ERROR.
 ******************************************************************************/
sl_zigbee_ezsp_status_t ashReceive(sli_buffer_manager_buffer_t *outBuf);

/***************************************************************************//**
 * @brief This function is used to determine if the serial output buffer has
 * available space for writing data. It should be called before
 * attempting to write data to ensure that the buffer can accommodate the
 * new data. This function is useful in managing flow control and
 * preventing buffer overflows in serial communication. It returns a
 * status indicating whether space is available or not.
 *
 * @return Returns SL_ZIGBEE_EZSP_SUCCESS if space is available, otherwise
 * returns SL_ZIGBEE_EZSP_NO_TX_SPACE.
 ******************************************************************************/
sl_zigbee_ezsp_status_t ashSerialWriteAvailable(void);

/***************************************************************************//**
 * @brief This function attempts to read a single byte from the serial port and
 * store it in the provided location. It should be used when you need to
 * retrieve incoming data from the serial interface. The function
 * requires a valid pointer to a byte-sized variable where the read data
 * will be stored. If no data is available, the function will indicate
 * this through its return value. Ensure that the pointer provided is not
 * null to avoid undefined behavior.
 *
 * @param byte Pointer to a uint8_t variable where the read byte will be stored.
 * Must not be null. The caller retains ownership of the memory. If
 * no data is available, the content of the variable remains
 * unchanged.
 * @return Returns SL_ZIGBEE_EZSP_SUCCESS if a byte is successfully read and
 * stored in the provided location, or SL_ZIGBEE_EZSP_NO_RX_DATA if no
 * data is available to read.
 ******************************************************************************/
sl_zigbee_ezsp_status_t ashSerialReadByte(uint8_t *byte);

/***************************************************************************//**
 * @brief This function is used to send a single byte to the serial output
 * buffer, which is part of the ASH protocol's communication mechanism.
 * It should be called when there is a need to transmit data over the
 * serial interface. The function assumes that the serial port has been
 * properly initialized and is ready for data transmission. It does not
 * provide feedback on the success of the operation, so it is typically
 * used in contexts where the caller can ensure that the serial buffer
 * has space available, possibly by checking with
 * `ashSerialWriteAvailable` beforehand.
 *
 * @param byte The byte to be written to the serial output buffer. It is an
 * 8-bit unsigned integer, and any value within the range of 0 to
 * 255 is valid. The caller retains ownership of the data.
 * @return None
 ******************************************************************************/
void ashSerialWriteByte(uint8_t byte);

#ifdef SLEEPY_EZSP_UART

/***************************************************************************//**
 * @brief This function determines whether the serial host is active by checking
 * for any recent activity on the serial port. It is useful in scenarios
 * where the system needs to verify if the host is ready to communicate
 * or if there has been any recent data transmission. The function should
 * be called when there is a need to confirm the host's active status,
 * especially in power-sensitive applications where the host's activity
 * state might influence power management decisions.
 *
 * @return Returns a boolean value: true if the serial host is active, false
 * otherwise.
 ******************************************************************************/
bool serialHostIsActive(void);

/***************************************************************************//**
 * @brief This function determines whether the serial interface is currently
 * waiting for the host to become active. It should be used in scenarios
 * where the host's activity status affects communication readiness. The
 * function returns true if the host is not active and the system is in a
 * state of waiting for the host to wake up. It is important to ensure
 * that the serial interface and host are properly initialized before
 * calling this function to avoid incorrect status reporting.
 *
 * @return A boolean value indicating whether the serial interface is waiting
 * for the host to wake up (true) or not (false).
 ******************************************************************************/
bool serialWaitingForHostToWake(void);

/***************************************************************************//**
 * @brief This function sets up the serial monitor to handle received data (RXD)
 * interrupts by configuring the necessary GPIO and interrupt settings.
 * It is typically used in environments where the serial communication
 * needs to be monitored for incoming data, especially in low-power or
 * sleep modes. The function disables and then re-enables the relevant
 * interrupt to ensure that any stale interrupts are cleared and the
 * system is ready to handle new ones. This function should be called
 * when the system is prepared to monitor serial data reception, and it
 * assumes that the necessary hardware components are properly
 * initialized.
 *
 * @param sleepMode An 8-bit unsigned integer representing the sleep mode. The
 * parameter is not used within the function, and its value
 * does not affect the function's behavior. The caller retains
 * ownership.
 * @return None
 ******************************************************************************/
void serialMonitorRxd(uint8_t sleepMode);

/***************************************************************************//**
 * @brief Use this function to deactivate the serial interface when it is no
 * longer needed or before entering a low-power state. This function is
 * typically called in scenarios where conserving power is critical, such
 * as in battery-operated devices. It is important to ensure that any
 * necessary data transmission or reception is completed before calling
 * this function, as it will disable the serial interface.
 *
 * @return None
 ******************************************************************************/
void serialPowerDown(void);

/***************************************************************************//**
 * @brief This function is used to ensure that the serial interface is powered
 * up when required. It should be called when there is a need to wake the
 * serial interface from a power-down state or when the serial host is
 * active. The function sends a wake signal to the host if the system is
 * in a power-down mode or if the host is active, ensuring that the
 * serial communication can proceed. It is important to call this
 * function in scenarios where the serial interface might be in a low-
 * power state and needs to be reactivated for communication.
 *
 * @return None
 ******************************************************************************/
void serialPowerUp(void);

/***************************************************************************//**
 * @brief Use this function to notify the host system that the device is awake
 * and ready for communication. It is typically called when the device
 * transitions from a low-power or sleep state to an active state,
 * ensuring that the host is aware of the device's readiness to
 * communicate. This function should be used in systems where the host
 * needs to be explicitly informed of the device's active status.
 *
 * @return None
 ******************************************************************************/
void serialSendAwakeSignal(void);

/***************************************************************************//**
 * @brief This function is used to modify the state of the serial interface by
 * inhibiting callback signals. It is typically called when the system
 * needs to prevent callbacks from being processed, possibly during
 * critical operations or when the system is in a specific state that
 * requires callbacks to be temporarily disabled. This function should be
 * used with caution, ensuring that the system can safely operate without
 * processing callbacks for the duration of the inhibition.
 *
 * @return None
 ******************************************************************************/
void serialInhibitCallbackSignal(void);

#endif

/**
 *  Informs ASH whether or not cany callbacks are pending.
 *
 */
void ashPendingCallbacks(bool pending);

//                                CAUTION
// These macro provide direct read-only access to the manufacturing tokens used
// by the ASH (EZSP-UART) protocol.
// These macros *should not be used* elsewhere due to the possibility of
// non-portability to future revisions of the manufacturing tokens.

#if defined(CORTEXM3)
#ifdef USERDATA_BASE
  #define ashConfigAddr (USERDATA_BASE | (MFG_ASH_CONFIG_LOCATION & 0x0FFF))
  #define ashReadConfig(member) (((const AshNcpConfig *)ashConfigAddr)->member)
  #define ashReadConfigOrDefault(member, defVal) \
  ((ashReadConfig(member) != 0xFFFF) ? ashReadConfig(member) : defVal)
#else
// EMZIGBEE-13500: No Manufacturing token avaiable yet
  #define ashReadConfig(member) (0xFFFF)
  #define ashReadConfigOrDefault(member, defVal) (defVal)
#endif // USERDATA_BASE
#else // for simulation, always return default value
  #define ashReadConfigOrDefault(member, defaultValue) \
  (defaultValue)
#endif

/***************************************************************************//**
 * @brief The AshNcpConfig structure is used to configure various parameters for
 * the ASH (Asynchronous Serial Host) protocol, which is part of the
 * Network Co-Processor (NCP) system. It includes settings for serial
 * communication such as baud rate and trace flags, as well as parameters
 * for managing data frame transmission and acknowledgment, including the
 * maximum number of unacknowledged frames, randomization of data
 * payloads, and adaptive timeout values. The structure also includes
 * several unused fields reserved for future use, and it defines timing
 * parameters for reboot delays and nFlag expiration.
 *
 * @param baudRate Represents the SerialBaudRate enum value for configuring the
 * baud rate.
 * @param traceFlags Controls trace output using bit flags.
 * @param unused0 Reserved for future use and currently not utilized.
 * @param txK Specifies the maximum number of frames that can be sent without
 * being acknowledged, ranging from 1 to 7.
 * @param randomize Enables the randomization of DATA frame payloads.
 * @param ackTimeInit Initial value for the adaptive received ACK timeout.
 * @param ackTimeMin Minimum value for the adaptive received ACK timeout.
 * @param ackTimeMax Maximum value for the adaptive received ACK timeout.
 * @param maxTimeouts Number of ACK timeouts required to enter the ERROR state.
 * @param unused1 Reserved for future use and currently not utilized.
 * @param rebootDelay Specifies the delay before sending a RSTACK signal.
 * @param unused2 Reserved for future use and currently not utilized.
 * @param unused3 Reserved for future use and currently not utilized.
 * @param unused4 Reserved for future use and currently not utilized.
 * @param nrTime Time after which a received nFlag expires.
 ******************************************************************************/
typedef struct
{ uint16_t
    baudRate,           ///< SerialBaudRate enum value
    traceFlags,         ///< trace output control bit flags (see defs below)
    unused0,            ///< (not used)
    txK,                ///< max frames that can be sent w/o being ACKed (1-7)
    randomize,          ///< enables randomizing DATA frame payloads
    ackTimeInit,        ///< adaptive rec'd ACK timeout initial value
    ackTimeMin,         ///<  "     "     "     "     "  minimum
    ackTimeMax,         ///<  "     "     "     "     "  maximum
    maxTimeouts,        ///< ACK timeouts needed to enter the ERROR state
    unused1,            ///< (not used)
    rebootDelay,        ///< reboot delay before sending RSTACK
    unused2,            ///< (not used)
    unused3,            ///< (not used)
    unused4,            ///< (not used)
    nrTime;             ///< time after which a rec'd nFlag expires
} AshNcpConfig;

//ASH configuration manufacturing token storage locations
//              word
//             offset
//baudRate      0x80   6=9600, 8=19200, A=38400, C=57600, F=115200
//traceFlags    0x81
//unused0       0x82
//txK           0x83
//randomize     0x84
//ackTimeInit   0x85
//ackTimeMin    0x86
//ackTimeMax    0x87
//maxTimeouts   0x88
//unused1       0x89
//rebootDelay   0x8A
//unused2       0x8B
//unused3       0x8C
//unused4       0x8D
//nrTime        0x8E

#ifdef HAL_UARTNCP_BAUD_RATE
#define ASH_BAUD_RATE HAL_UARTNCP_BAUD_RATE
#else
// EMHAL-2285 if using hardware flow control we are using FIFO mode with DMA
// which can support baud 115200
#if (SL_LEGACY_SERIAL1_MODE == SL_LEGACY_SERIAL_FIFO) && defined(EMBER_SERIAL1_RTSCTS)
#define ASH_BAUD_RATE    BAUD_115200
#else
#define ASH_BAUD_RATE    BAUD_57600
#endif
#endif
// Enable VUART trace: B0: basic frame data, B1: verbose frame data, B2: events
#define ASH_TRACE_FLAGS           7
// Define sliding window size: the maximum number of unacknowledged frames
// that can be outstanding before blocking further DATA frame transmission.
// Note that it must be the case that 1 <= K <= 7.
#define ASH_TX_K                  5
// Enable randomizing DATA frame payloads
#define ASH_RANDOMIZE          true
// Adaptive timeout values for newly-transmitted DATA frames to be ACK'ed
// Units are milliseconds. Values must be < 8192 to avoid overflow.
#define ASH_TIME_DATA_INIT     1600
#define ASH_TIME_DATA_MIN       400
#define ASH_TIME_DATA_MAX      3200
// Consecutive ACK timeouts (minus 1) needed to enter the ERROR state.
// The value 0xFF means the NCP will not disconnect due to timeouts.
#define ASH_MAX_TIMEOUTS          3
// Delay before sending RSTACK, needed by some RS-232 to TTL converters.
#ifdef SLEEPY_EZSP_ASH
  #define ASH_REBOOT_DELAY        0
#else
  #define ASH_REBOOT_DELAY       1000
#endif
// Preload value for the NR timer used in flow control. Since this timer
// is only one byte, care must be taken to avoid overflow. Each tick is
// 16 milliseconds, and the largest acceptable value is 127*16 = 2032 ms.
#define ASH_NR_TIME            1000

// Bits in traceFlags
#define TRACE_VUART_FRAMES_BASIC    1
#define TRACE_VUART_FRAMES_VERBOSE  2
#define TRACE_VUART_EVENTS          4

#endif //__ASH_NCP_H__
