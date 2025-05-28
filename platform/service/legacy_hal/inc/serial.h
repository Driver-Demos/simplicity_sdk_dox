/***************************************************************************//**
 * @file
 * @brief Serial hardware abstraction layer interfaces.
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

#ifndef SERIAL_H
#define SERIAL_H

#include <stdarg.h>
#include "sl_status.h"

#ifdef DOXYGEN_SHOULD_SKIP_THIS

/**
 * @brief Assign numerical values for variables that hold Baud Rate
 * parameters.
 */
enum SerialBaudRate
#else
  #ifndef DEFINE_BAUD
  #define DEFINE_BAUD(num) BAUD_##num
  #endif
  #ifdef CORTEXM3_EFM32_MICRO
typedef uint32_t SerialBaudRate;
  #else
typedef uint8_t SerialBaudRate;
  #endif
enum
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef CORTEXM3_EFM32_MICRO
{
  DEFINE_BAUD(300) = 300,  // BAUD_300
  DEFINE_BAUD(600) = 600,  // BAUD_600
  DEFINE_BAUD(900) = 900,  // etc...
  DEFINE_BAUD(1200) = 1200,
  DEFINE_BAUD(2400) = 2400,
  DEFINE_BAUD(4800) = 4800,
  DEFINE_BAUD(9600) = 9600,
  DEFINE_BAUD(14400) = 14400,
  DEFINE_BAUD(19200) = 19200,
  DEFINE_BAUD(28800) = 28800,
  DEFINE_BAUD(38400) = 38400,
  DEFINE_BAUD(50000) = 50000,
  DEFINE_BAUD(57600) = 57600,
  DEFINE_BAUD(76800) = 76800,
  DEFINE_BAUD(100000) = 100000,
  DEFINE_BAUD(115200) = 115200
};
#else //CORTEXM3_EFM32_MICRO
{
  DEFINE_BAUD(300) = 0,  // BAUD_300
  DEFINE_BAUD(600) = 1,  // BAUD_600
  DEFINE_BAUD(900) = 2,  // etc...
  DEFINE_BAUD(1200) = 3,
  DEFINE_BAUD(2400) = 4,
  DEFINE_BAUD(4800) = 5,
  DEFINE_BAUD(9600) = 6,
  DEFINE_BAUD(14400) = 7,
  DEFINE_BAUD(19200) = 8,
  DEFINE_BAUD(28800) = 9,
  DEFINE_BAUD(38400) = 10,
  DEFINE_BAUD(50000) = 11,
  DEFINE_BAUD(57600) = 12,
  DEFINE_BAUD(76800) = 13,
  DEFINE_BAUD(100000) = 14,
  DEFINE_BAUD(115200) = 15,
  DEFINE_BAUD(230400) = 16,   /*<! define higher baud rates for the EM2XX and EM3XX */
  DEFINE_BAUD(460800) = 17,   /*<! Note: receiving data at baud rates > 115200 */
  DEFINE_BAUD(CUSTOM) = 18    /*<! may not be reliable due to interrupt latency */
};
#endif //CORTEXM3_EFM32_MICRO

#ifdef  CORTEXM3_EFM32_MICRO

/***************************************************************************//**
 * @brief The `COM_Port_t` is an enumeration that defines various communication
 * ports, including legacy COM ports and Virtual COM Ports (VCP), as well
 * as USART ports. It provides both legacy and new definitions for these
 * ports, with specific integer and hexadecimal values assigned to each
 * port type. This enumeration is used to abstract the hardware
 * communication interfaces, allowing for easier management and selection
 * of different communication ports in the system.
 *
 * @param COM_VCP Represents the legacy COM port for Virtual COM Port with a
 * value of 0.
 * @param COM_USART0 Represents the legacy COM port for USART0 with a value of
 * 1.
 * @param COM_USART1 Represents the legacy COM port for USART1 with a value of
 * 2.
 * @param COM_USART2 Represents the legacy COM port for USART2 with a value of
 * 3.
 * @param comPortVcp Represents the Virtual COM Port with a hexadecimal value of
 * 0x10.
 * @param comPortUsart0 Represents USART0 with a hexadecimal value of 0x20.
 * @param comPortUsart1 Represents USART1 with a hexadecimal value of 0x21.
 * @param comPortUsart2 Represents USART2 with a hexadecimal value of 0x22.
 * @param comPortUsart3 Represents USART3 with a hexadecimal value of 0x23.
 ******************************************************************************/
typedef enum COM_Port {
  // Legacy COM port defines
  COM_VCP = 0,
  COM_USART0 = 1,
  COM_USART1 = 2,
  COM_USART2 = 3,

  // VCP
  comPortVcp = 0x10,

  // USARTs
  comPortUsart0 = 0x20,
  comPortUsart1 = 0x21,
  comPortUsart2 = 0x22,
  comPortUsart3 = 0x23,
} COM_Port_t;

typedef /*USART_Parity_TypeDef*/ uint8_t SerialParity;
#define PARITY_NONE usartNoParity
#define PARITY_ODD  usartOddParity
#define PARITY_EVEN usartEvenParity

#else//!CORTEXM3_EFM32_MICRO

#ifdef DOXYGEN_SHOULD_SKIP_THIS

/**
 * @brief Assign numerical values for the types of parity.
 * Use for variables that hold Parity parameters.
 */
enum SerialParity
#else
  #ifndef DEFINE_PARITY
  #define DEFINE_PARITY(val) PARITY_##val
  #endif
typedef uint8_t SerialParity;
enum
#endif //DOXYGEN_SHOULD_SKIP_THIS
{
  DEFINE_PARITY(NONE) = 0U,  // PARITY_NONE
  DEFINE_PARITY(ODD) = 1U,   // PARITY_ODD
  DEFINE_PARITY(EVEN) = 2U   // PARITY_EVEN
};

#endif//CORTEXM3_EFM32_MICRO

/***************************************************************************//**
 * @brief This function sets up a serial communication interface on a specified
 * port with the given baud rate, parity, and number of stop bits. It is
 * typically called during the initialization phase of a program to
 * prepare the serial interface for data transmission and reception. The
 * function assumes that the port number, baud rate, parity, and stop
 * bits provided are valid and supported by the hardware. It returns a
 * status indicating the success or failure of the initialization
 * process.
 *
 * @param port The port number to initialize. It must correspond to a valid and
 * available serial port on the hardware. The caller retains
 * ownership.
 * @param rate The baud rate for the serial communication. It must be a valid
 * value from the SerialBaudRate enumeration. The caller retains
 * ownership.
 * @param parity The parity setting for the serial communication. It must be a
 * valid value from the SerialParity enumeration. The caller
 * retains ownership.
 * @param stopBits The number of stop bits to use in the communication. It is
 * typically 1 or 2. The caller retains ownership.
 * @return Returns an sl_status_t value indicating the success (SL_STATUS_OK) or
 * failure of the initialization.
 ******************************************************************************/
sl_status_t sli_legacy_serial_init(uint8_t port,
                                   SerialBaudRate rate,
                                   SerialParity parity,
                                   uint8_t stopBits);

/***************************************************************************//**
 * @brief This function attempts to read a single byte from the specified serial
 * port and store it in the provided location. It should be called when a
 * byte of data is expected to be available on the port. The function
 * requires that the serial port has been properly initialized and
 * configured before calling. If the read operation is successful, the
 * byte is stored at the location pointed to by the dataByte parameter.
 * If the port is not available or no data is present, the function
 * returns an appropriate status code indicating the failure or empty
 * state.
 *
 * @param port The serial port number from which to read the byte. It must
 * correspond to a valid and initialized port. Invalid or
 * uninitialized ports will result in a failure status.
 * @param dataByte A pointer to a uint8_t variable where the read byte will be
 * stored. Must not be null, as the function will attempt to
 * write to this location. If the pointer is null, the behavior
 * is undefined.
 * @return Returns an sl_status_t value indicating the result of the operation:
 * SL_STATUS_OK if successful, SL_STATUS_EMPTY if no data is available,
 * or SL_STATUS_FAIL if the operation fails for other reasons.
 ******************************************************************************/
sl_status_t sli_legacy_serial_read_byte(uint8_t port, uint8_t *dataByte);

/***************************************************************************//**
 * @brief This function attempts to read a specified number of bytes from the
 * given serial port into the provided buffer. It should be used when you
 * need to receive data from a serial interface. The function requires
 * that the serial port is properly initialized and available. It will
 * block until the requested number of bytes is read or an error occurs.
 * If the read operation is successful, the number of bytes read is
 * stored in the provided bytesRead pointer, if it is not null. The
 * function returns a status code indicating success or failure of the
 * operation.
 *
 * @param port The serial port identifier from which to read data. It must
 * correspond to a valid and initialized serial port.
 * @param data A pointer to a buffer where the read data will be stored. Must
 * not be null and should have enough space to hold 'length' bytes.
 * @param length The number of bytes to read from the serial port. Must be a
 * positive integer.
 * @param bytesRead A pointer to a variable where the function will store the
 * number of bytes actually read. Can be null if the caller
 * does not need this information.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * read operation. SL_STATUS_OK is returned on success, while
 * SL_STATUS_FAIL is returned if the operation fails.
 ******************************************************************************/
sl_status_t sl_legacy_serial_read_data(uint8_t port,
                                       uint8_t *data,
                                       uint16_t length,
                                       uint16_t *bytesRead);

/***************************************************************************//**
 * @brief This function attempts to send a single byte of data to a specified
 * serial port. It is typically used when there is a need to transmit
 * data over a serial interface. The function requires that the serial
 * port has been properly initialized before calling. If the specified
 * port is not available or the operation fails, the function returns an
 * error status. This function is dependent on the presence of the UART
 * common IO stream component.
 *
 * @param port The identifier of the serial port to which the byte should be
 * written. It must correspond to a valid and initialized port.
 * Invalid or uninitialized ports will result in a failure status.
 * @param dataByte The byte of data to be written to the specified serial port.
 * Any 8-bit value is valid.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while SL_STATUS_FAIL
 * is returned if the operation fails.
 ******************************************************************************/
sl_status_t sli_legacy_serial_write_byte(uint8_t port, uint8_t dataByte);

sl_status_t sli_legacy_serial_printf(uint8_t port, const char * formatString, ...);

sl_status_t sli_legacy_serial_printf_line(uint8_t port, const char * formatString, ...);

sl_status_t sli_legacy_serial_print_carriage_return(uint8_t port);

sl_status_t sli_legacy_serial_printf_var_arg(uint8_t port, const char * formatString, va_list ap);

/***************************************************************************//**
 * @brief This function is used to ensure that any ongoing serial send operation
 * on the specified port has completed. It is typically called after
 * initiating a send operation to ensure that the data has been fully
 * transmitted before proceeding. This function is useful in scenarios
 * where subsequent operations depend on the completion of the send
 * operation. It must be called with a valid port identifier, and it
 * always returns a success status, indicating that the wait operation is
 * complete.
 *
 * @param port An identifier for the serial port to wait on. It must be a valid
 * port number as defined in the system. The function does not
 * perform any operation based on this parameter, and it is
 * effectively ignored.
 * @return Returns SL_STATUS_OK, indicating the wait operation is complete.
 ******************************************************************************/
sl_status_t sli_legacy_serial_wait_send(uint8_t port);

/***************************************************************************//**
 * @brief This function is used to send a formatted string to a specified serial
 * port, appending a newline character at the end of the output. It
 * should be used when a complete line of text needs to be transmitted
 * over a serial connection, ensuring that the output is properly
 * terminated with a newline. The function requires that the legacy
 * printf functionality is present; otherwise, it will return a failure
 * status. It is important to ensure that the serial port is properly
 * initialized before calling this function.
 *
 * @param port The serial port identifier to which the formatted string will be
 * sent. It must correspond to a valid and initialized serial port.
 * @param formatString A C-style format string that specifies how subsequent
 * arguments are converted for output. It must not be null
 * and should be a valid format string.
 * @return Returns an sl_status_t value indicating success or failure of the
 * operation. If the legacy printf functionality is not present, it
 * returns SL_STATUS_FAIL.
 ******************************************************************************/
sl_status_t sli_legacy_serial_guaranteed_printf(uint8_t port, const char * formatString, ...);

#endif // SERIAL_H
