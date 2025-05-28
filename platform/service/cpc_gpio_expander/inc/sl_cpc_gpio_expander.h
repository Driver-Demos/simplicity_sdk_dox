/***************************************************************************/ /**
 * @file
 * @brief CPC GPIO Endpoint of the Secondary
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_CPC_GPIO_EXPANDER_H
#define SL_CPC_GPIO_EXPANDER_H

#include <stdint.h>
#include "sl_status.h"
#include "em_gpio.h"

/*******************************************************************************
 **************************   STRUCTS   ****************************************
 ******************************************************************************/
/// @brief Struct representing a cpc gpio expander handle.
/***************************************************************************//**
 * @brief The `sl_cpc_gpio_expander_handle_t` is a structure that encapsulates
 * the details necessary to manage a GPIO expander handle in a CPC
 * (Communication Protocol Controller) system. It includes information
 * about the specific GPIO port and pin, a name identifier, and
 * configuration settings, allowing for organized and efficient
 * management of GPIO resources in embedded systems.
 *
 * @param port Represents the GPIO port associated with the expander handle.
 * @param pin Specifies the pin number on the GPIO port.
 * @param name Holds a string representing the name of the GPIO expander handle.
 * @param config Stores configuration settings for the GPIO expander handle.
 ******************************************************************************/
typedef struct {
  const GPIO_Port_TypeDef port;                    ///< port
  const uint32_t pin;                              ///< pin
  const char *name;                                ///< name
  uint32_t config;                                 ///< config
} sl_cpc_gpio_expander_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************//**
 * @brief This function sets up the CPC GPIO expander endpoint, preparing it for
 * operation. It must be called before any other operations on the GPIO
 * expander are performed. If the system is running with an operating
 * system kernel, it initializes necessary OS resources such as threads
 * and semaphores. In non-OS environments, it performs basic endpoint
 * initialization. The function returns a status code indicating success
 * or failure, which should be checked to ensure the initialization was
 * successful.
 *
 * @return Returns SL_STATUS_OK on successful initialization or
 * SL_STATUS_ALLOCATION_FAILED if resource allocation fails.
 ******************************************************************************/
sl_status_t sl_cpc_gpio_expander_init(void);

/***************************************************************************//**
 * @brief This function should be called to handle the current state of the CPC
 * GPIO endpoint. It processes various endpoint states, such as
 * initialization, write completion, and reply readiness, and performs
 * the necessary actions like reading or transmitting packets. It must be
 * called regularly in the main loop to ensure the endpoint operates
 * correctly. If a restart is required, the function will handle it and
 * return immediately. This function does not take any parameters and
 * does not return a value.
 *
 * @return None
 ******************************************************************************/
void sl_cpc_gpio_expander_process_action(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup cpc_gpio_secondary) */

#endif /* SL_CPC_GPIO_EXPANDER_H */
