/***************************************************************************//**
 * @file
 * @brief IO Stream UART Component.
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

#ifndef SL_IOSTREAM_UART_TYPE_H
#define SL_IOSTREAM_UART_TYPE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#include "em_device.h"
#include "sl_status.h"
#include "sl_iostream.h"
#include "dmadrv.h"

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif

#if (defined(SL_CATALOG_KERNEL_PRESENT))
#include "cmsis_os2.h"
#include "sl_cmsis_os2_common.h"
#endif

/***************************************************************************//**
 * @addtogroup iostream
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup iostream_uart I/O Stream UART
 * @brief I/O Stream UART
 * @details
 * ## Overview
 *
 *   UART layer provides a set of standard APIs that can be used with all type of
 *   UART interface.
 *
 * ## Initialization
 *
 *   Each UART stream type provides its initalization with parameters specific to them.
 * @note  Each UART stream requires a dedicated (L)DMA channel through DMADRV.
 *
 * ## Configuration
 *
 * ### RX Buffer Size
 *
 *    The `SL_IOSTREAM_<Peripheral>_<Instance>_RX_BUFFER_SIZE` parameter should
 *    be chosen based on the time it takes for the MCU to consume the RX buffer,
 *    otherwise flow control will have to be asserted to avoid data loss.
 *    If hardware flow control is unavailable, data will be dropped.
 *
 *    Let's assume that the MCU is able to call the IOStream Read function with
 *    a maximum delay of 1ms. We have:
 *    ```
 *    (1) MaximumDelay = RXBufferSize * TimePerDatum
 *
 *    Where TimePerDatum is defined by (#startBits + #dataBits + #parityBits + #stopBits)/BaudRate.
 *    With default settings, we have 1 start & stop bit, 8 data bits and no parity.
 *
 *    Rewriting (1), we have:
 *
 *    (2) MaximumDelay = RXBufferSize * 10/BaudRate <=> RXBufferSize = MaximumDelay*Baudrate/10
 *
 *    With a baudrate of 921,600 and a maximum consumption delay of 1ms, we have
 *    a recommended RXBufferSize of:
 *
 *    (3) RXBufferSize = 0.001*921600/10 = 93Bytes
 *    ```
 *    This should ensure that flow control does not have to be asserted, slowing
 *    down the bus and if unavailable, that no data will be dropped.
 *
 * ### Baudrate
 *
 *    IOStream UART leverages the DMA in order consume data from the UART peripheral.
 *    When user reads data from IOStream, the internal reception buffer gets more
 *    room for receiving new data. However, this update process pauses the DMA,
 *    meaning that any new data coming from the bus will remain in the UART peripheral's
 *    FIFO. If too much data is received before the update can complete, hardware
 *    flow control will have be enforces, or data will be dropped if unavailable.
 *
 *    Measurements show that the DMA update executes in ~23.7us with -O3 optimization
 *    (-Os gives slightly worst performance of ~32.7us). With these, the following
 *    equations can be followed to understand the maximum baudrate supported by
 *    the current design, when no flow control is available:
 *    ```
 *    The number of bytes received during the ring buffer update is given by:
 *
 *    (1) UpdateTime = FIFOSize * TimePerDatum
 *
 *    Where TimePerDatum is defined by (#startBits + #dataBits + #parityBits + #stopBits)/BaudRate.
 *    With default settings, we have 1 start & stop bit, 8 data bits and no parity.
 *    Rewriting (1), we have:
 *
 *    (2) UpdateTime = FIFOSize * 10/Baudrate
 *
 *    For USART, we have a FIFO of size 2. This means that for an update time of
 *    23.7us, we have a max baudrate of:
 *
 *    (3) 23.7 = 2 * 10/Baudrate <=> Baudrate = 2*10/23.7 = 843,881Baud.
 *    ```
 *    The maximum "real" baudrate is then 460,800 Baud, otherwise hardware flow
 *    control will have to be asserted, or data will be dropped.
 *
 *    To achieve higher throughput without flow control assertion, users can use
 *    the EUSART peripheral, which boasts a 16Bytes FIFO, allow for baudrate of
 *    upwards of 921600 without data loss with no hardware flow control.
 *
 * @{
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Data Types

#define uartFlowControlNone  0                     ///< uart flow control none
#define uartFlowControlSoftware    0xFFFF          ///< uart flow control software
#define UARTXON     0x11                           ///< uartx on
#define UARTXOFF    0x13                           ///< uartx off

/// @brief I/O Stream UART stream object
/***************************************************************************//**
 * @brief The `sl_iostream_uart_t` structure defines a UART stream interface for
 * I/O operations, providing function pointers for initialization,
 * deinitialization, and configuration of UART-specific features such as
 * automatic line conversion and energy mode restrictions. It includes
 * conditional members that are available based on the presence of a
 * Power Manager or Kernel, allowing for flexible power management and
 * read blocking configurations. This structure is part of a larger
 * framework for managing UART streams in embedded systems, facilitating
 * efficient data transmission and reception with optional power-saving
 * features.
 *
 * @param stream Represents the base stream interface for the UART.
 * @param deinit Function pointer for deinitializing the UART stream.
 * @param set_auto_cr_lf Function pointer to set automatic carriage return and
 * line feed conversion.
 * @param get_auto_cr_lf Function pointer to get the status of automatic
 * carriage return and line feed conversion.
 * @param set_rx_energy_mode_restriction Function pointer to set energy mode
 * restrictions for receiving data,
 * available only with Power Manager.
 * @param get_rx_energy_mode_restriction Function pointer to get energy mode
 * restrictions for receiving data,
 * available only with Power Manager.
 * @param sleep_on_isr_exit Function pointer to determine sleep behavior on ISR
 * exit, available only with Power Manager and without
 * Kernel.
 * @param set_read_block Function pointer to set read blocking mode, available
 * only with Kernel.
 * @param get_read_block Function pointer to get read blocking mode, available
 * only with Kernel.
 ******************************************************************************/
typedef struct {
  sl_iostream_t stream;                                               ///< stream
  sl_status_t (*deinit)(void *stream);                                ///< uart deinit
  void (*set_auto_cr_lf)(void *context, bool on);                     ///< set_auto_cr_lf
  bool (*get_auto_cr_lf)(void *context);                              ///< get_auto_cr_lf
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  void (*set_rx_energy_mode_restriction)(void *context, bool on);     ///< set_rx_energy_mode_restriction. Available only when Power Manager present.
  bool (*get_rx_energy_mode_restriction)(void *context);              ///< get_rx_energy_mode_restriction. Available only when Power Manager present.
#if !defined(SL_CATALOG_KERNEL_PRESENT) || defined(DOXYGEN)
  sl_power_manager_on_isr_exit_t (*sleep_on_isr_exit)(void *context); ///< sleep_on_isr_exit. Available only when Power Manager present and kernel not present.
#endif
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
  void (*set_read_block)(void *context, bool on);                     ///< set_read_block. Available only when kernel present.
  bool (*get_read_block)(void *context);                              ///< get_read_block. Available only when kernel present.
#endif
} sl_iostream_uart_t;

/// @brief I/O Stream (L)DMA Config
/***************************************************************************//**
 * @brief The `sl_iostream_dma_config_t` structure is used to configure DMA
 * (Direct Memory Access) settings for an I/O stream in a UART (Universal
 * Asynchronous Receiver-Transmitter) context. It contains a peripheral
 * signal that triggers the DMA transfer and a pointer to the source data
 * register of the I/O stream peripheral. This configuration is essential
 * for setting up the DMA to handle data transfers efficiently between
 * the peripheral and memory, reducing CPU load and improving data
 * throughput.
 *
 * @param peripheral_signal Peripheral signal to trigger a DMA transfer on.
 * @param src Pointer to IO Stream peripheral data register.
 ******************************************************************************/
typedef struct {
  DMADRV_PeripheralSignal_t peripheral_signal;  ///< Peripheral signal to trigger a DMA transfer on
  uint8_t *src;                                 ///< Pointer to IO Stream peripheral data register
} sl_iostream_dma_config_t;

/// @brief I/O Steam (L)DMA Context
/***************************************************************************//**
 * @brief The `sl_iostream_dma_context_t` structure is designed to manage the
 * context for DMA operations within an IO stream, specifically for UART
 * interfaces. It includes configuration settings for the DMA, the
 * channel being used, and descriptors for handling DMA reception and
 * wrapping operations. This structure is essential for ensuring
 * efficient data transfer and management in systems utilizing DMA for IO
 * operations.
 *
 * @param cfg DMA Configuration settings for the IO stream.
 * @param channel Specifies the DMA channel used.
 * @param rx_resume_desc Descriptor for resuming DMA reception.
 * @param wrap_desc Descriptor for wrapping DMA operations.
 ******************************************************************************/
typedef struct {
  sl_iostream_dma_config_t cfg;                       ///< DMA Configuration
  uint8_t channel;                                    ///< DMA Channel
  #if defined(EMDRV_DMADRV_LDMA)
  LDMA_Descriptor_t rx_resume_desc;                   ///< DMA reception resume descriptor
  LDMA_Descriptor_t wrap_desc;                        ///< DMA wrap descriptor
  #elif defined(EMDRV_DMADRV_LDMA_S3)
  sl_hal_ldma_descriptor_t rx_resume_desc;            ///< DMA reception resume descriptor
  sl_hal_ldma_descriptor_t wrap_desc;                 ///< DMA wrap descriptor
  #endif
} sl_iostream_dma_context_t;

/// @brief I/O Stream UART config
/***************************************************************************//**
 * @brief The `sl_iostream_uart_config_t` structure is used to configure the
 * UART stream in the Silicon Labs I/O Stream library. It includes
 * settings for DMA configuration, interrupt request numbers for both
 * receiving and transmitting data, and a buffer for receiving UART data.
 * Additionally, it provides options for line feed conversion, reception
 * during sleep, and software flow control, allowing for flexible and
 * efficient UART communication management.
 *
 * @param dma_cfg DMA configuration settings for the UART stream.
 * @param rx_irq_number Interrupt request number for receiving data.
 * @param tx_irq_number Interrupt request number for transmitting data.
 * @param rx_buffer Pointer to the buffer used for receiving UART data.
 * @param rx_buffer_length Length of the UART receive buffer.
 * @param lf_to_crlf Flag indicating if line feed should be converted to
 * carriage return line feed.
 * @param rx_when_sleeping Flag indicating if reception should occur when the
 * system is sleeping.
 * @param sw_flow_control Flag indicating if software flow control is enabled.
 ******************************************************************************/
typedef struct {
  sl_iostream_dma_config_t dma_cfg;                     ///< DMA Config
  IRQn_Type rx_irq_number;                              ///< rx_irq_number
  IRQn_Type tx_irq_number;                              ///< tx_irq_number
  uint8_t *rx_buffer;                                   ///< UART Rx Buffer
  size_t rx_buffer_length;                              ///< UART Rx Buffer length
  bool lf_to_crlf;                                      ///< lf_to_crlf
  bool rx_when_sleeping;                                ///< rx_when_sleeping
  bool sw_flow_control;                                 ///< sw_flow_control
} sl_iostream_uart_config_t;

/// @brief I/O Stream UART context
/***************************************************************************//**
 * @brief The `sl_iostream_uart_context_t` structure is a comprehensive data
 * structure used to manage UART (Universal Asynchronous Receiver-
 * Transmitter) operations in an I/O stream context. It includes various
 * fields for handling DMA operations, buffer management, and function
 * pointers for transmission and deinitialization. The structure also
 * supports features like line feed conversion, software flow control,
 * and energy management, with additional fields for kernel and power
 * manager integration. This makes it suitable for complex embedded
 * systems requiring efficient UART communication and power management.
 *
 * @param dma DMA Context for managing Direct Memory Access operations.
 * @param rx_buffer Pointer to the UART receive buffer.
 * @param rx_buffer_len Length of the UART receive buffer.
 * @param rx_read_ptr Pointer to the next byte to be read from the receive
 * buffer.
 * @param tx Function pointer for transmitting a character.
 * @param tx_completed Function pointer for handling the Tx Completed event.
 * @param deinit Function pointer for deinitializing the UART context.
 * @param lf_to_crlf Flag indicating if line feed should be converted to
 * carriage return line feed.
 * @param sw_flow_control Flag indicating if software flow control is enabled.
 * @param ctrl_char_scan_ptr Pointer to the last control character scan
 * position.
 * @param xon Flag indicating if the transmitter is enabled.
 * @param remote_xon Flag indicating if the remote transmitter is enabled.
 * @param rx_irq_number Interrupt request number for receiving data.
 * @param tx_irq_number Interrupt request number for transmitting data,
 * available when Power Manager is present.
 * @param tx_idle Flag indicating if the transmitter is idle, available when
 * Power Manager is present.
 * @param em_req_added Flag indicating if energy mode request is added,
 * available when Power Manager is present.
 * @param rx_em Energy mode for receiving, available when Power Manager is
 * present.
 * @param tx_em Energy mode for transmitting, available when Power Manager is
 * present.
 * @param block Flag indicating if blocking mode is enabled, available when
 * kernel is present.
 * @param read_lock Mutex for read operations, available when kernel is present.
 * @param read_lock_cb Control block for the read lock mutex, available when
 * kernel is present.
 * @param rx_data_flag Event flag for received data, available when kernel is
 * present.
 * @param rx_data_flag_cb Control block for the rx data flag, available when
 * kernel is present.
 * @param write_lock Mutex for write operations, available when kernel is
 * present.
 * @param write_lock_cb Control block for the write lock mutex, available when
 * kernel is present.
 * @param sleep Function pointer for handling sleep on ISR exit, available when
 * kernel is not present and Power Manager is present.
 ******************************************************************************/
typedef struct {
  sl_iostream_dma_context_t dma;            ///< DMA Context
  uint8_t *rx_buffer;                       ///< UART Rx Buffer
  size_t rx_buffer_len;                     ///< UART Rx Buffer length
  uint8_t *rx_read_ptr;                     ///< Address of the next byte to be read
  sl_status_t (*tx)(void *context, char c); ///< Tx function pointer
  void (*tx_completed)(void *context, bool enable); ///< Pointer to a function handling the Tx Completed event
  sl_status_t (*deinit)(void *context);     ///< DeInit function pointer
  bool lf_to_crlf;                          ///< lf_to_crlf
  bool sw_flow_control;                     ///< software flow control
  uint8_t *ctrl_char_scan_ptr;              ///< Pointer to where the last control character scan ended
  volatile bool xon;                        ///< Transmitter enabled
  bool remote_xon;                          ///< Remote Transmitter enabled
  IRQn_Type rx_irq_number;                  ///< Receive IRQ Number
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  IRQn_Type tx_irq_number;                  ///< Transmit IRQ Number
  volatile bool tx_idle;                    ///< tx_idle. Available only when Power Manager present.
  bool em_req_added;                        ///< em_req_added. Available only when Power Manager present.
  sl_power_manager_em_t rx_em;              ///< rx_em. Available only when Power Manager present.
  sl_power_manager_em_t tx_em;              ///< tx_em. Available only when Power Manager present.
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
  bool block;                                ///< block. Available only when kernel present.
  osMutexId_t read_lock;                     ///< read_lock. Available only when kernel present.
  __ALIGNED(4) uint8_t read_lock_cb[osMutexCbSize];         ///< read_lock control block. Available only when kernel present.
  osEventFlagsId_t rx_data_flag;              ///< rx_data_flag. Available only when kernel present.
  __ALIGNED(4) uint8_t rx_data_flag_cb[osEventFlagsCbSize];   ///< rx_data_flag control block. Available only when kernel present.
  osMutexId_t write_lock;                    ///< write_lock. Available only when kernel present.
  __ALIGNED(4) uint8_t write_lock_cb[osMutexCbSize];        ///< write_lock control block. Available only when kernel present.
#elif defined(SL_CATALOG_POWER_MANAGER_PRESENT) || defined(DOXYGEN)
  sl_power_manager_on_isr_exit_t sleep;      ///< sleep. Available only when kernel not present and Power Manager present.
#endif
} sl_iostream_uart_context_t;

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief The `sl_iostream_uart_deinit` function deinitializes a UART stream
 * object, potentially adjusting power management settings if applicable.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object to be deinitialized.
 * @return The function returns an `sl_status_t` value indicating the result of
 * the deinitialization process.
 ******************************************************************************/
__STATIC_INLINE sl_status_t sl_iostream_uart_deinit(sl_iostream_uart_t *iostream_uart)
{
  #if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
  iostream_uart->set_rx_energy_mode_restriction(iostream_uart->stream.context, false);
  #endif
  return iostream_uart->deinit(iostream_uart);
}

/***************************************************************************//**
 * @brief The function `sl_iostream_uart_set_auto_cr_lf` configures a UART
 * stream to enable or disable automatic conversion of line feed (LF)
 * characters to carriage return and line feed (CRLF) sequences.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object.
 * @param on A boolean value indicating whether to enable (true) or disable
 * (false) automatic LF to CRLF conversion.
 * @return The function does not return any value.
 ******************************************************************************/
__STATIC_INLINE void sl_iostream_uart_set_auto_cr_lf(sl_iostream_uart_t *iostream_uart,
                                                     bool on)
{
  iostream_uart->set_auto_cr_lf(iostream_uart->stream.context, on);
}

/***************************************************************************//**
 * @brief The function `sl_iostream_uart_get_auto_cr_lf` retrieves the current
 * setting for automatic line conversion from LF to CRLF for a given UART
 * stream.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object.
 * @return A boolean value indicating whether automatic LF to CRLF conversion is
 * enabled for the specified UART stream.
 ******************************************************************************/
__STATIC_INLINE bool sl_iostream_uart_get_auto_cr_lf(sl_iostream_uart_t *iostream_uart)
{
  return iostream_uart->get_auto_cr_lf(iostream_uart->stream.context);
}

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
/***************************************************************************//**
 * @brief This function is used to prepare a UART stream for entering a low-
 * power sleep mode. It should be called before the system enters sleep
 * to ensure that the UART stream is correctly configured to handle new
 * data detection while in sleep mode. This function is particularly
 * relevant in systems where power management is a concern and the UART
 * needs to remain responsive to incoming data even when the system is in
 * a low-power state. It is important to ensure that the `iostream_uart`
 * parameter is properly initialized and not null before calling this
 * function.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream. This parameter must be a
 * valid, non-null pointer to a properly initialized UART
 * stream object.
 * @return None
 ******************************************************************************/
void sl_iostream_uart_prepare_for_sleep(sl_iostream_uart_t *iostream_uart);

/***************************************************************************//**
 * @brief This function is used to restore the UART stream to its normal
 * operational state after the system wakes up from a low-power mode. It
 * should be called when the system resumes from sleep to ensure that the
 * UART stream is ready to handle data transmission and reception. This
 * function is particularly relevant in systems where power management is
 * implemented, and the UART stream might have been paused or altered
 * during sleep. It is important to ensure that the `iostream_uart`
 * parameter is properly initialized and not null before calling this
 * function.
 *
 * @param iostream_uart A pointer to an initialized `sl_iostream_uart_t`
 * structure representing the UART stream. Must not be
 * null. The function assumes that the structure is
 * properly set up and ready for use.
 * @return None
 ******************************************************************************/
void sl_iostream_uart_wakeup(sl_iostream_uart_t *iostream_uart);

/***************************************************************************//**
 * @brief The function `sl_iostream_uart_set_rx_energy_mode_restriction`
 * configures the energy mode restriction for receiving data on a UART
 * stream, allowing or disallowing data reception when the system is in a
 * low power state.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object.
 * @param on A boolean value indicating whether to enable (true) or disable
 * (false) the energy mode restriction for receiving data.
 * @return The function does not return any value; it performs an action by
 * configuring the energy mode restriction for the UART stream.
 ******************************************************************************/
__STATIC_INLINE void sl_iostream_uart_set_rx_energy_mode_restriction(sl_iostream_uart_t *iostream_uart,
                                                                     bool on)
{
  iostream_uart->set_rx_energy_mode_restriction(iostream_uart->stream.context, on);
}

/***************************************************************************//**
 * @brief The function `sl_iostream_uart_get_rx_energy_mode_restriction`
 * retrieves the current energy mode restriction status for receiving
 * data on a UART stream.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object.
 * @return The function returns a boolean value indicating whether the energy
 * mode restriction for receiving data is enabled (true) or disabled
 * (false).
 ******************************************************************************/
__STATIC_INLINE bool sl_iostream_uart_get_rx_energy_mode_restriction(sl_iostream_uart_t *iostream_uart)
{
  return iostream_uart->get_rx_energy_mode_restriction(iostream_uart->stream.context);
}
#endif

#if defined(SL_CATALOG_KERNEL_PRESENT)
/***************************************************************************//**
 * @brief The function `sl_iostream_uart_set_read_block` configures the read
 * blocking mode of a UART stream.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object.
 * @param on A boolean value indicating whether the read API should be blocking
 * (`true`) or non-blocking (`false`).
 * @return This function does not return a value; it modifies the behavior of
 * the UART stream's read operation.
 ******************************************************************************/
__STATIC_INLINE void sl_iostream_uart_set_read_block(sl_iostream_uart_t *iostream_uart,
                                                     bool on)
{
  iostream_uart->set_read_block(iostream_uart->stream.context, on);
}

/***************************************************************************//**
 * @brief The function `sl_iostream_uart_get_read_block` retrieves the current
 * read blocking mode configuration for a given UART stream.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object.
 * @return A boolean value indicating whether the read API is in blocking mode
 * (true) or non-blocking mode (false).
 ******************************************************************************/
__STATIC_INLINE bool sl_iostream_uart_get_read_block(sl_iostream_uart_t *iostream_uart)
{
  return iostream_uart->get_read_block(iostream_uart->stream.context);
}
#endif

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT) && !defined(SL_CATALOG_KERNEL_PRESENT)
/***************************************************************************//**
 * @brief The function `sl_iostream_uart_sleep_on_isr_exit` determines the power
 * management action to take upon exiting an ISR for a given UART stream.
 *
 * @param iostream_uart A pointer to an `sl_iostream_uart_t` structure
 * representing the UART stream object.
 * @return The function returns a value of type
 * `sl_power_manager_on_isr_exit_t`, which indicates the power
 * management action to take (e.g., sleep, wakeup, or ignore) after an
 * ISR exit.
 ******************************************************************************/
__STATIC_INLINE sl_power_manager_on_isr_exit_t sl_iostream_uart_sleep_on_isr_exit(sl_iostream_uart_t *iostream_uart)
{
  return iostream_uart->sleep_on_isr_exit(iostream_uart->stream.context);
}

#endif

/** @} (end addtogroup iostream_uart) */
/** @} (end addtogroup iostream) */

#ifdef __cplusplus
}
#endif

#endif // SL_IOSTREAM_UART_H
