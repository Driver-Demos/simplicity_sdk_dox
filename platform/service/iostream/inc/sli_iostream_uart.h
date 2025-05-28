/***************************************************************************//**
 * @file
 * @brief IO Stream UART internal APIs.
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

#ifndef SLI_IOSTREAM_UART_H
#define SLI_IOSTREAM_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#include "sl_status.h"
#include "sl_iostream.h"
#include "sl_iostream_uart.h"

#define SLI_IOSTREAM_UART_FLAG_CTS             0x01
#define SLI_IOSTREAM_UART_FLAG_RTS             0x02
#define SLI_IOSTREAM_UART_FLAG_HIGH_FREQUENCY  0x04

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function sets up the UART context and configuration for IO stream
 * operations, preparing it for data transmission and reception. It must
 * be called before any UART operations are performed to ensure the
 * context is properly initialized. The function configures the necessary
 * DMA channels and IRQs, and sets up the context with the provided
 * configuration and function pointers for transmission, completion, and
 * deinitialization. It handles initialization errors by returning a
 * status code, which should be checked by the caller to ensure
 * successful setup.
 *
 * @param uart A pointer to an sl_iostream_uart_t structure that will be
 * initialized. Must not be null.
 * @param context A pointer to an sl_iostream_uart_context_t structure that will
 * be initialized. Must not be null.
 * @param config A pointer to an sl_iostream_uart_config_t structure containing
 * configuration settings. Must not be null.
 * @param tx A function pointer for transmitting a character. Must not be null.
 * @param tx_completed A function pointer for handling transmission completion.
 * Must not be null.
 * @param deinit A function pointer for deinitializing the context. Must not be
 * null.
 * @param rx_em_req A uint8_t value representing the energy mode requirement for
 * receiving. Valid values depend on the power management
 * configuration.
 * @param tx_em_req A uint8_t value representing the energy mode requirement for
 * transmitting. Valid values depend on the power management
 * configuration.
 * @return Returns an sl_status_t indicating the success or failure of the
 * initialization process. SL_STATUS_OK on success, or an error code on
 * failure.
 ******************************************************************************/
sl_status_t sli_iostream_uart_context_init(sl_iostream_uart_t *uart,
                                           sl_iostream_uart_context_t *context,
                                           sl_iostream_uart_config_t *config,
                                           sl_status_t (*tx)(void *context, char c),
                                           void (*tx_completed)(void *context, bool enable),
                                           sl_status_t (*deinit)(void *context),
                                           uint8_t rx_em_req,
                                           uint8_t tx_em_req);

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT) && !defined(SL_IOSTREAM_UART_FLUSH_TX_BUFFER)
/***************************************************************************//**
 * @brief This function should be called when a UART transmission is completed
 * to update the transmission state and manage power requirements. It is
 * typically used in environments where power management is a concern,
 * ensuring that the system can enter a lower power state once
 * transmission is idle. The function assumes that the context provided
 * is valid and properly initialized, and it will assert if the
 * transmission completion callback is not set. It should be used in
 * systems where the power manager is present and the UART flush TX
 * buffer feature is not enabled.
 *
 * @param context A pointer to a sl_iostream_uart_context_t structure. This must
 * be a valid, non-null pointer to a context that has been
 * initialized and is currently managing a UART transmission. The
 * caller retains ownership of the context.
 * @return None
 ******************************************************************************/
void sli_uart_txc(void *context);
#endif

#ifdef __cplusplus
}
#endif

#endif /* SLI_IOSTREAM_UART_H */
