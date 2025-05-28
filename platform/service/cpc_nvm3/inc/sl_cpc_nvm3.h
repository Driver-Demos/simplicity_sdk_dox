/***************************************************************************/ /**
 * @file
 * @brief CPC NVM3 of the Secondary
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

#ifndef SL_CPC_NVM3_H
#define SL_CPC_NVM3_H

#include <stdint.h>
#include "sl_status.h"

/*******************************************************************************
 **************************   STRUCTS   ****************************************
 ******************************************************************************/
/// @brief Struct representing a cpc nvm3 handle.
/***************************************************************************//**
 * @brief The `sl_cpc_nvm3_handle_t` is a simple structure that encapsulates a
 * single member, a constant character pointer named `name`. This
 * structure is used to represent a handle for CPC NVM3 operations,
 * likely serving as an identifier or label for a specific NVM3 instance
 * or configuration within the CPC (Co-Processor Communication)
 * framework. The use of a constant pointer suggests that the name is
 * intended to be immutable after initialization, ensuring the integrity
 * of the handle's identity throughout its lifecycle.
 *
 * @param name A constant character pointer representing the name associated
 * with the NVM3 handle.
 ******************************************************************************/
typedef struct {
  const char *name;
} sl_cpc_nvm3_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************//**
 * @brief This function initializes the CPC NVM3 endpoint, preparing it for
 * subsequent operations. It must be called before any other operations
 * on the CPC NVM3 endpoint are performed. If the system is configured
 * with an operating system kernel, it sets up necessary threading and
 * synchronization primitives. In non-kernel environments, it performs
 * basic endpoint initialization. The function returns a status code
 * indicating success or failure, which should be checked to ensure the
 * initialization was successful.
 *
 * @return Returns an sl_status_t value indicating the success or failure of the
 * initialization process. Possible return values include SL_STATUS_OK
 * for success and SL_STATUS_ALLOCATION_FAILED if resource allocation
 * fails.
 ******************************************************************************/
sl_status_t sl_cpc_nvm3_init(void);

/***************************************************************************//**
 * @brief This function should be called to handle any pending actions or state
 * transitions for the CPC-NVM3 endpoint. It is typically used in a loop
 * to continuously process endpoint actions, ensuring that commands are
 * executed and responses are transmitted as needed. The function manages
 * state changes and handles command completion, ensuring that the
 * endpoint operates correctly. It must be called after the endpoint has
 * been initialized with `sl_cpc_nvm3_init`. The function does not take
 * any parameters and does not return a value, but it may internally
 * manage state and buffer data for transmission.
 *
 * @return None
 ******************************************************************************/
void sl_cpc_nvm3_process_action(void);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup cpc_nvm3_secondary) */

#endif /* SL_CPC_NVM3_H */
