/***************************************************************************//**
 * @file
 * @brief Header for ASH common functions
 *
 * See @ref ash for documentation.
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
#ifndef __ASH_COMMON_H__
#define __ASH_COMMON_H__

/** @addtogroup ash
 *
 * Use the Asynchronous Serial Host (ASH) Framework interfaces on a host
 * microcontroller when it communicates with an Ember chip via EZSP-UART.
 *
 * See ash-common.h for source code.
 *
 *@{
 */

/***************************************************************************//**
 * @brief This function is used to build an ASH frame by encoding a single byte
 * at a time. It handles byte stuffing for control and data fields,
 * computes and appends a CRC, and adds an ending flag byte. When
 * starting a new frame, the `len` parameter should be non-zero, and all
 * subsequent calls should use zero. The function returns the next
 * encoded byte, which may include escape bytes, CRC bytes, or the end
 * flag byte. The `offset` parameter is updated to indicate the position
 * of the next input byte, and is set to 0xFF when the frame is complete.
 *
 * @param len Indicates the length of the frame to be encoded. A non-zero value
 * starts a new frame, and subsequent calls should use zero. The
 * length includes the control byte and data field, but not the flag
 * or CRC. The function does not validate this length.
 * @param byte The next byte of data to add to the frame. The same byte may need
 * to be passed multiple times as escape bytes, CRC, and end flag
 * bytes are output.
 * @param offset Pointer to the offset of the next input byte. It is set to 0
 * when starting a new frame and to 0xFF when the last byte of the
 * frame is returned. Must not be null.
 * @return Returns the next encoded output byte in the frame.
 ******************************************************************************/
uint8_t ashEncodeByte(uint8_t len, uint8_t byte, uint8_t *offset);

/***************************************************************************//**
 * @brief This function is used to decode and validate an ASH frame by
 * processing data one byte at a time. It handles byte stuffing, checks
 * the CRC, and identifies the end flag. The function can terminate the
 * frame early if it encounters specific control bytes like CAN or SUB.
 * It should be called repeatedly with each byte of the frame until the
 * entire frame is processed. The function maintains state across calls,
 * so it should be used consistently for a single frame. The caller must
 * ensure that the output buffer is large enough to hold the decoded
 * data, and the function will not output more bytes than the maximum
 * valid frame length.
 *
 * @param byte The next byte of data to add to the frame. It must be a valid
 * byte of the ASH frame being decoded.
 * @param out Pointer to where the function will write an output byte. Must not
 * be null and should point to a valid memory location.
 * @param outLen Pointer to a variable where the function will store the number
 * of bytes output so far. Must not be null and should point to a
 * valid memory location.
 * @return Returns a status code indicating the result of the decoding process,
 * such as success, in progress, or various error conditions.
 ******************************************************************************/
sl_zigbee_ezsp_status_t ashDecodeByte(uint8_t byte, uint8_t *out, uint8_t *outLen);

/***************************************************************************//**
 * @brief This function modifies the contents of a given array by XORing each
 * byte with a pseudo-random sequence generated from an 8-bit seed. It is
 * useful for reducing the likelihood of byte-stuffing in data frames by
 * altering repeated byte patterns. The function can be called with a
 * seed of zero to initialize the random sequence, or with a non-zero
 * seed to continue a sequence from a previous call. This allows for
 * processing data in chunks, such as linked buffers, by using the
 * returned seed value for subsequent calls.
 *
 * @param seed An 8-bit value used to initialize or continue the pseudo-random
 * sequence. A value of zero initializes the sequence, while a non-
 * zero value continues from a previous invocation.
 * @param buf A pointer to the array whose contents will be randomized. The
 * caller must ensure this pointer is valid and points to a writable
 * memory region.
 * @param len The number of bytes in the array to modify. Must be a non-negative
 * value.
 * @return Returns the last value of the pseudo-random sequence, which can be
 * used as the seed for subsequent calls if processing data in chunks.
 ******************************************************************************/
uint8_t ashRandomizeArray(uint8_t seed, uint8_t *buf, uint8_t len);

/***************************************************************************//**
 * @brief This function initializes and starts the ASH acknowledgment timer,
 * which is used to track the time elapsed for receiving an
 * acknowledgment in the ASH protocol. It should be called when an
 * acknowledgment is expected, and it ensures that the timer is running
 * by setting it to a non-zero value. If the timer is inadvertently set
 * to zero, it is adjusted to a maximum value to indicate it is active.
 * This function does not require any parameters and does not return a
 * value.
 *
 * @return None
 ******************************************************************************/
void ashStartAckTimer(void);

/** @brief Stops and clears ashAckTimer.
 *
 */
void ashStopAckTimer(void);

#define ashStopAckTimer() do { ashAckTimer = 0; } while (false)

/** @brief Indicates whether or not ashAckTimer is currently running.
 *  The timer may be running even if expired.
 *
 */
#define ashAckTimerIsRunning() (ashAckTimer != 0)

/** @brief Indicates whether or not ashAckTimer is currently running.
 *  The timer may be running even if expired.
 *
 */
#define ashAckTimerIsNotRunning() (ashAckTimer == 0)

/***************************************************************************//**
 * @brief This function determines whether the ASH acknowledgment timer has
 * expired by comparing the current time with the timer's start time and
 * the acknowledgment period. It should be used to check the status of
 * the acknowledgment timer, which is relevant in communication protocols
 * where timing is critical. The function returns false if the timer is
 * not running, indicating that it has not expired. This function is
 * useful in scenarios where actions need to be taken based on whether
 * the timer has expired or not.
 *
 * @return Returns a boolean value: true if the timer has expired, false
 * otherwise.
 ******************************************************************************/
bool ashAckTimerHasExpired(void);

/***************************************************************************//**
 * @brief This function is used to adapt the acknowledgement timer period
 * according to the observed delay in receiving an ACK. It should be
 * called when there is a need to adjust the timer, either because the
 * timer has expired or to refine the period based on recent ACK timings.
 * If the timer has expired, the period is doubled. If the timer is
 * running and has not expired, the elapsed time is used to adjust the
 * period using a simple IIR filter. The function ensures that the period
 * remains within predefined minimum and maximum limits. The
 * acknowledgement timer is always stopped by this function, regardless
 * of the input.
 *
 * @param expired A boolean indicating whether the timer has expired. If true,
 * the period is doubled; if false, the period is adjusted based
 * on the elapsed time.
 * @return None
 ******************************************************************************/
void ashAdjustAckPeriod(bool expired);

/** @brief Sets the acknowledgement timer period (in msec)
 *  and stops the timer.
 *
 */
#define ashSetAckPeriod(msec) \
  do { ashAckPeriod = msec; ashAckTimer = 0; } while (false)

/** @brief Returns the acknowledgement timer period (in msec).
 *
 */
#define ashGetAckPeriod() (ashAckPeriod)

/** @brief Sets the acknowledgement timer period (in msec),
 *  and starts the timer running.
 */
#define ashSetAndStartAckTimer(msec) \
  do { ashSetAckPeriod(msec); ashStartAckTimer(); }  while (false)

// Define the units used by the Not Ready timer as 2**n msecs
#define ASH_NR_TIMER_BIT    4 ///< log2 of msecs per NR timer unit

/***************************************************************************//**
 * @brief This function initializes and starts the Not Ready (NR) timer used in
 * the Asynchronous Serial Host (ASH) framework. It is typically used
 * when the host microcontroller communicates with an Ember chip via
 * EZSP-UART and needs to manage periods when the host cannot process
 * callbacks. The timer is set based on the current millisecond tick and
 * a configurable or default NR time value. If the calculated timer value
 * is zero, it is set to a maximum value of 0xFF to ensure the timer
 * runs. This function should be called when the system needs to manage
 * callback readiness over extended periods.
 *
 * @return None
 ******************************************************************************/
void ashStartNrTimer(void);

/** @brief Stops the Not Ready timer.
 */
#define ashStopNrTimer()  do { ashNrTimer = 0; } while (false)

/***************************************************************************//**
 * @brief This function determines whether the Not Ready timer has either
 * expired or been stopped. It is useful for managing the timing of
 * operations that depend on the Not Ready timer, such as refreshing
 * nFlag on the host or resuming callback sending on the NCP. If the
 * timer has expired, it will be stopped by this function. This function
 * should be called to check the status of the Not Ready timer when its
 * expiration or stopping is relevant to the application's logic.
 *
 * @return Returns true if the Not Ready timer has expired or is stopped;
 * otherwise, returns false.
 ******************************************************************************/
bool ashNrTimerHasExpired(void);

/** @brief Indicates whether or not ashNrTimer is currently running.
 *
 */
#define ashNrTimerIsNotRunning() (ashAckTimer == 0)

/***************************************************************************//**
 * @brief The `ashDecodeInProgress` is a global boolean variable that indicates
 * whether the decoding of an ASH frame is currently in progress. It is
 * set to false to signal the start of decoding a new frame.
 *
 * @details This variable is used to manage the state of the ASH frame decoding
 * process, ensuring that new frames are only decoded when the previous
 * decoding process is complete.
 ******************************************************************************/
extern bool ashDecodeInProgress; ///< set false to start decoding a new frame

// ASH timers (units)
/***************************************************************************//**
 * @brief The `ashAckTimer` is a global variable representing a timer used to
 * track the time elapsed since the last acknowledgment (ACK) was
 * received in milliseconds. It is part of the Asynchronous Serial Host
 * (ASH) framework, which facilitates communication between a host
 * microcontroller and an Ember chip via EZSP-UART.
 *
 * @details This variable is used to determine if the acknowledgment timer is
 * running, has expired, or needs to be adjusted based on the observed
 * ACK delay.
 ******************************************************************************/
extern uint16_t ashAckTimer;        ///< rec'd ack timer (msecs)
/***************************************************************************//**
 * @brief The `ashAckPeriod` is a global variable that represents the period of
 * the acknowledgment timer in milliseconds. It is used to determine the
 * time interval for receiving acknowledgments in the ASH protocol.
 *
 * @details This variable is used to set and adjust the timeout period for
 * acknowledgment timers in the ASH protocol, ensuring timely
 * communication between devices.
 ******************************************************************************/
extern uint16_t ashAckPeriod;       ///< rec'd ack timer period (msecs)
/***************************************************************************//**
 * @brief The `ashNrTimer` is a global variable representing a 'Not Ready'
 * timer, measured in units of 16 milliseconds. It is used to manage
 * timing for situations where the host or NCP (Network Co-Processor) is
 * not ready to process callbacks or resume sending callbacks,
 * respectively.
 *
 * @details This variable is used to track the duration for which the system is
 * not ready to handle callbacks, and it is manipulated by functions
 * like `ashStartNrTimer` and `ashStopNrTimer` to control its
 * operation.
 ******************************************************************************/
extern uint8_t ashNrTimer;          ///< not ready timer (16 msec units)

#endif //__ASH_COMMON_H__

/** @} END addtogroup
 */
