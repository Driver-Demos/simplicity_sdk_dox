/***************************************************************************//**
 * @file
 * @brief IO Stream USART Component.
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

#ifndef SL_IOSTREAM_USART_H
#define SL_IOSTREAM_USART_H

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "sl_iostream.h"
#include "sl_iostream_uart.h"
#include "sl_status.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#if (defined(SL_CATALOG_KERNEL_PRESENT))
#include "cmsis_os2.h"
#include "sl_cmsis_os2_common.h"
#endif

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup iostream
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup iostream_usart I/O Stream USART
 * @brief I/O Stream USART
 * @details
 * ## Overview
 *
 *   The Universal Synchronous / Asynchronous Receiver / Transmitter controller(s) (USART)
 *   can be used as a UART and can, therefore, be connected to an external transceiver
 *   to communicate with another host using the serial link. Multiple instances can be
 *   instantiated as long as they are bound to a unique USART peripheral. The flow control
 *   is supported as well.
 *
 * ## Initialization
 *
 *   The stream sets itself as the default stream at the end of the initialization
 *   function.You must reconfigure the default interface if you have multiple streams
 *   in your project else the last stream initialized will be set as the system default
 *   stream.
 *
 * ## Power manager integration
 *
 *   On transmission side, I/O Stream-USART adds a requirement on EM1 until the transmit is
 *   really completed. Once the transmit is completed the requirement is removed and the
 *   system can go to a lower energy mode if allowed.
 *
 *   On the receive side, a requirement on EM1 is added if the application can receive
 *   data asynchronously. Meaning that the system can receive data when the MCU is into
 *   a sleep mode(will always be EM1 to allow the reception). Otherwise, If the system
 *   doesn't expect to receive anything, no requirement is added and the system can go
 *   to the lowest Energy mode allowed. You can control this behavior using a
 *   configuration per instance, see the following configuration:
 *
 *       SL_IOSTREAM_USART_<instance_name>_RESTRICT_ENERGY_MODE_TO_ALLOW_RECEPTION
 *
 * @{
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Data Types

/// @brief I/O Stream USART config
/***************************************************************************//**
 * @brief The `sl_iostream_usart_config_t` structure is used to configure the
 * USART peripheral for I/O streaming in embedded systems. It includes
 * pointers to the USART peripheral and its associated clock, as well as
 * configuration details for the transmit and receive ports and pins.
 * Additionally, it supports flow control through CTS and RTS ports and
 * pins, and includes a timeout setting for receive operations. The
 * structure also accommodates device-specific configurations for USART
 * location and index, ensuring flexibility across different hardware
 * platforms.
 *
 * @param usart Pointer to USART peripheral.
 * @param clock Peripheral Clock.
 * @param tx_port Transmit port.
 * @param tx_pin Transmit pin.
 * @param rx_port Receive port.
 * @param rx_pin Receive pin.
 * @param cts_port Flow control, CTS port.
 * @param cts_pin Flow control, CTS pin.
 * @param rts_port Flow control, RTS port.
 * @param rts_pin Flow control, RTS pin.
 * @param rx_timeout Timeout delay between two Rx events.
 * @param usart_index Usart index, available only on certain devices.
 * @param usart_tx_location USART Transmit location, available only on certain
 * devices.
 * @param usart_rx_location USART Receive location, available only on certain
 * devices.
 * @param usart_cts_location USART CTS location, available only on certain
 * devices.
 * @param usart_rts_location USART RTS location, available only on certain
 * devices.
 * @param usart_location USART location, available only on certain devices.
 ******************************************************************************/
typedef struct {
  USART_TypeDef *usart;       ///< Pointer to USART peripheral
  CMU_Clock_TypeDef clock;    ///< Peripheral Clock
  GPIO_Port_TypeDef tx_port;  ///< Transmit port
  uint8_t tx_pin;        ///< Transmit pin
  GPIO_Port_TypeDef rx_port;  ///< Receive port
  uint8_t rx_pin;        ///< Receive pin
  GPIO_Port_TypeDef cts_port; ///< Flow control, CTS port
  uint8_t  cts_pin;       ///< Flow control, CTS pin
  GPIO_Port_TypeDef rts_port; ///< Flow control, RTS port
  uint8_t rts_pin;       ///< Flow control, RTS pin
  uint8_t rx_timeout;    ///< Timeout delay between two Rx events
#if defined(GPIO_USART_ROUTEEN_TXPEN)
  uint8_t usart_index;        ///< Usart index. Available only on certain devices.
#elif defined(USART_ROUTEPEN_RXPEN)
  uint8_t usart_tx_location;  ///< USART Transmit location. Available only on certain devices.
  uint8_t usart_rx_location;  ///< USART Receive location. Available only on certain devices.
  uint8_t usart_cts_location; ///< USART CTS location. Available only on certain devices.
  uint8_t usart_rts_location; ///< USART RTS location. Available only on certain devices.
#else
  uint8_t usart_location;     ///< USART location. Available only on certain devices.
#endif
} sl_iostream_usart_config_t;

/// @brief I/O Stream USART context
/***************************************************************************//**
 * @brief The `sl_iostream_usart_context_t` structure is designed to encapsulate
 * the context for a USART I/O stream, including configuration details
 * for the USART peripheral, associated GPIO ports and pins for
 * transmission and reception, and optional flow control settings. It is
 * used to manage the state and configuration of a USART instance within
 * the Silicon Labs I/O Stream framework, supporting both synchronous and
 * asynchronous communication modes.
 *
 * @param context A context structure for UART, providing USART location
 * information.
 * @param usart Pointer to the USART peripheral.
 * @param clock Specifies the peripheral clock type.
 * @param tx_port Defines the GPIO port used for transmission.
 * @param tx_pin Specifies the GPIO pin used for transmission.
 * @param rx_port Defines the GPIO port used for reception.
 * @param rx_pin Specifies the GPIO pin used for reception.
 * @param cts_port Defines the GPIO port for CTS flow control (conditional on
 * series).
 * @param cts_pin Specifies the GPIO pin for CTS flow control (conditional on
 * series).
 * @param rts_port Defines the GPIO port for RTS flow control (conditional on
 * series).
 * @param rts_pin Specifies the GPIO pin for RTS flow control (conditional on
 * series).
 * @param flags Holds additional configuration flags (conditional on series).
 ******************************************************************************/
typedef struct {
  sl_iostream_uart_context_t context; ///< usart_location
  USART_TypeDef *usart;       ///< usart
  CMU_Clock_TypeDef clock;    ///< Peripheral Clock
  GPIO_Port_TypeDef tx_port;  ///< Transmit port
  uint8_t tx_pin;             ///< Transmit pin
  GPIO_Port_TypeDef rx_port;  ///< Receive port
  uint8_t rx_pin;             ///< Receive pin
#if (_SILICON_LABS_32B_SERIES > 0)
  GPIO_Port_TypeDef cts_port; ///< Flow control, CTS port
  uint8_t cts_pin;            ///< Flow control, CTS pin
  GPIO_Port_TypeDef rts_port; ///< Flow control, RTS port
  uint8_t rts_pin;            ///< Flow control, RTS pin
  uint8_t flags;
#endif
} sl_iostream_usart_context_t;

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function sets up a USART stream for input and output operations
 * by configuring the necessary hardware and software parameters. It must
 * be called before any USART communication can occur, ensuring that the
 * USART peripheral is properly initialized and ready for use. The
 * function requires valid configuration structures for both the UART and
 * USART, as well as a context structure to maintain state information.
 * It handles enabling clocks, setting GPIO modes, and configuring USART
 * settings, including optional hardware flow control. The function
 * returns a status code indicating success or failure, which should be
 * checked to ensure the initialization was successful.
 *
 * @param iostream_uart A pointer to an sl_iostream_uart_t structure. This must
 * not be null and is used to manage the I/O stream state.
 * @param uart_config A pointer to an sl_iostream_uart_config_t structure
 * containing UART configuration settings. This must not be
 * null and should be properly initialized before calling the
 * function.
 * @param init A pointer to a USART_InitAsync_TypeDef structure specifying USART
 * initialization parameters. This must not be null and should be
 * configured with desired USART settings.
 * @param config A pointer to an sl_iostream_usart_config_t structure containing
 * USART-specific configuration details, including pin and port
 * settings. This must not be null and should be correctly set up
 * to match the hardware configuration.
 * @param usart_context A pointer to an sl_iostream_usart_context_t structure
 * used to maintain USART state information. This must not
 * be null and is updated by the function to reflect the
 * current USART configuration.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * initialization process. SL_STATUS_OK is returned on success, while
 * other status codes indicate specific errors.
 ******************************************************************************/
sl_status_t sl_iostream_usart_init (sl_iostream_uart_t *iostream_uart,
                                    sl_iostream_uart_config_t *uart_config,
                                    USART_InitAsync_TypeDef *init,
                                    sl_iostream_usart_config_t *usart_config,
                                    sl_iostream_usart_context_t *usart_context);

/***************************************************************************//**
 * @brief This function processes interrupt requests for a USART I/O stream,
 * ensuring proper handling of transmission events. It should be called
 * within an interrupt service routine when a USART interrupt occurs. The
 * function is designed to manage transmission completion events and may
 * assert if an unhandled interrupt flag is encountered. It is important
 * to ensure that the USART I/O stream is properly initialized before
 * invoking this handler.
 *
 * @param iostream_uart A pointer to an initialized sl_iostream_uart_t structure
 * representing the I/O stream UART handle. This parameter
 * must not be null, and the caller retains ownership. The
 * function does not modify the contents of this structure.
 * @return None
 ******************************************************************************/
void sl_iostream_usart_irq_handler(sl_iostream_uart_t *iostream_uart);

/** @} (end addtogroup iostream_usart) */
/** @} (end addtogroup iostream) */

#ifdef __cplusplus
}
#endif

#endif // SL_IOSTREAM_USART_H
