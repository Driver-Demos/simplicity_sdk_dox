/***************************************************************************/ /**
 * @file
 * @brief CPC Driver Interface
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

#ifndef SLI_CPC_DRV_H
#define SLI_CPC_DRV_H

#include "sli_cpc.h"
#include "sl_status.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

/***************************************************************************//**
 * @brief The `sli_cpc_drv_capabilities_t` structure is used to define the
 * capabilities of a CPC driver, specifically whether it can preprocess
 * HDLC headers and if UART flow control is supported. This structure is
 * part of the CPC driver interface, which is used to manage
 * communication protocols in embedded systems. The boolean fields within
 * this structure allow the driver to specify its capabilities, which can
 * be queried by other components of the system to ensure compatibility
 * and proper configuration.
 *
 * @param preprocess_hdlc_header Indicates if the HDLC header is validated by
 * the driver.
 * @param uart_flowcontrol Indicates if UART flow control is enabled.
 ******************************************************************************/
typedef struct {
  bool preprocess_hdlc_header;            ///< Is the HDLC header validated by driver
  bool uart_flowcontrol;                  ///< Is UART flow control enabled
} sli_cpc_drv_capabilities_t;

/* forward declare sli_cpc_drv_t as it's referenced in the ops structure */
struct sli_cpc_drv;
/***************************************************************************//**
 * @brief The `sli_cpc_drv_t` structure is a key component of the CPC (Co-
 * Processor Communication) driver interface, encapsulating a set of
 * operations that define the behavior and capabilities of the driver. It
 * includes a member `ops` which is a structure of function pointers,
 * each representing a specific operation such as initialization, data
 * transmission, and reception, as well as configuration and capability
 * queries. This structure allows for flexible and modular driver
 * implementation, enabling different hardware interfaces like UART, SPI,
 * and SDIO to be managed under a unified interface.
 *
 * @param ops A structure containing function pointers for various driver
 * operations.
 ******************************************************************************/
typedef struct sli_cpc_drv sli_cpc_drv_t;

/***************************************************************************//**
 * @brief The `sli_cpc_drv_ops_t` structure defines a set of function pointers
 * that represent operations for a CPC (Co-Processor Communication)
 * driver. These operations include initializing hardware and the driver,
 * managing data transmission and reception, and handling driver
 * capabilities and configurations. The structure is designed to provide
 * a flexible interface for different driver implementations, allowing
 * for operations such as reading and writing data, checking transmission
 * readiness, and managing bus bitrate settings. Some functions are
 * conditionally included based on the presence of specific features,
 * such as primary CPC catalog support.
 *
 * @param hw_init Function pointer to initialize the hardware peripheral for
 * standalone use.
 * @param init Function pointer to initialize the driver after hardware
 * initialization.
 * @param get_capabilities Function pointer to retrieve the CPC driver
 * capabilities.
 * @param start_rx Function pointer to start receiving packets.
 * @param deinit Function pointer to de-initialize the CPC driver for firmware
 * upgrades.
 * @param read Function pointer to read data from the driver.
 * @param write Function pointer to transmit data through the driver.
 * @param is_transmit_ready Function pointer to check if the driver is ready to
 * transmit.
 * @param get_bus_bitrate Function pointer to get the current bus bitrate.
 * @param set_bus_bitrate Function pointer to set the bus bitrate.
 * @param get_bus_max_bitrate Function pointer to get the maximum bus bitrate.
 * @param on_rx_buffer_handle_free Function pointer for notification when an RX
 * buffer handle is freed.
 * @param on_rx_buffer_free Function pointer for notification when an RX buffer
 * is freed.
 ******************************************************************************/
typedef struct {
  /// Initialize only the hardware peripheral to be used in a standalone manner
  /// (during the bootloader poking)
  /// @return SL_STATUS_OK if successful. Error code otherwise
  sl_status_t (*hw_init)(sli_cpc_drv_t *driver);

  /// Initialize the rest of the driver after the hardware peripheral has been
  /// initialized in sli_cpc_drv_hw_init.
  /// @return SL_STATUS_OK if successful. Error code otherwise.
  sl_status_t (*init)(sli_cpc_drv_t *driver, sli_cpc_instance_t *inst);

  /// Gets CPC driver capabilities.
  /// @param capabilities Pointer to structure that will receive the driver
  ///                     capabilities.
  ///  @return SL_STATUS_OK if successful. Error code otherwise.
  sl_status_t (*get_capabilities)(sli_cpc_drv_t *driver, sli_cpc_drv_capabilities_t *capabilities);

  /// Start recveiving packets
  /// @return SL_STATUS_OK if successful. Error code otherwise.
  sl_status_t (*start_rx)(sli_cpc_drv_t *driver);

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
  /// De-Initializes CPC driver for the firmware upgrade to take over control.
  void (*deinit)(sli_cpc_drv_t *driver);
#endif

  /// Reads data from driver.
  /// @param buffer_handle Pointer to variable that will receive receive the buffer handle.
  /// @param payload_rx_len Pointer to variable that will receive the received
  ///                       payload length, in bytes.
  /// @return SL_STATUS_OK if successful. Error code otherwise.
  sl_status_t (*read)(sli_cpc_drv_t *driver, sl_cpc_buffer_handle_t **buffer_handle);

  /// Transmits data.
  /// @param buffer_handle Pointer to the buffer handle.
  /// @return SL_STATUS_OK if successful. Error code otherwise.
  sl_status_t (*write)(sli_cpc_drv_t *driver, sl_cpc_buffer_handle_t *buffer_handle);

  /// Checks if driver is ready to transmit.
  /// @return true if driver is ready to accept a tx. false otherwise.
  bool (*is_transmit_ready)(sli_cpc_drv_t *driver);

  /// Get currently configured bus bitrate
  uint32_t (*get_bus_bitrate)(sli_cpc_drv_t *driver);

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
  /// Set bus bitrate
  void (*set_bus_bitrate)(sli_cpc_drv_t *driver, uint32_t bitrate);
#endif

  /// Get maximum bus bitrate
  uint32_t (*get_bus_max_bitrate)(sli_cpc_drv_t *driver);

  /// Notification on freed RX buffer_handle
  void (*on_rx_buffer_handle_free)(sli_cpc_drv_t *driver);

  /// Notification on freed RX buffer
  void (*on_rx_buffer_free)(sli_cpc_drv_t *driver);
} sli_cpc_drv_ops_t;

/***************************************************************************//**
 * @brief The `sli_cpc_drv` structure is a compound data type that encapsulates
 * a set of operations for a CPC (Communication Protocol Controller)
 * driver. It contains a single member, `ops`, which is a structure of
 * type `sli_cpc_drv_ops_t`. This member holds function pointers to
 * various driver operations such as initialization, reading, writing,
 * and capability querying. The structure is designed to provide a
 * flexible interface for interacting with different hardware peripherals
 * through a standardized set of operations.
 *
 * @param ops A structure containing function pointers for various driver
 * operations.
 ******************************************************************************/
struct sli_cpc_drv {
  sli_cpc_drv_ops_t ops;
};

#ifdef __cplusplus
extern "C"
{
#endif

// -----------------------------------------------------------------------------
// Core to driver commands

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
/***************************************************************************//**
 * Return a driver instance. Currently only driver instance can be enabled at
 * the same time. In the future, that limitation might be lifted.
 ******************************************************************************/
sli_cpc_drv_t* sli_cpc_drv_get_driver(void);
#else
#if defined(SL_CATALOG_CPC_DRIVER_UART_PRESENT)
extern sli_cpc_drv_t uart_driver;
#endif

#if defined(SL_CATALOG_CPC_DRIVER_SPI_PRESENT)
extern sli_cpc_drv_t spi_driver;
#endif

#if defined(SL_CATALOG_CPC_DRIVER_SDIO_PRESENT)
extern sli_cpc_drv_t sdio_driver;
#endif

#if defined(SL_CATALOG_CPC_DRIVER_EMUL_PRESENT)
extern sli_cpc_drv_t emul_driver;
#endif
#endif // defined(SL_CATALOG_CPC_PRIMARY_PRESENT)

/** @} (end addtogroup cpc) */

#ifdef __cplusplus
}
#endif

#endif // SLI_CPC_DRV_H
