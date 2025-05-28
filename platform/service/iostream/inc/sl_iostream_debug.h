/***************************************************************************//**
 * @file
 * @brief IO Stream debug Component.
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
#ifndef SL_IOSTREAM_DEBUG_H
#define SL_IOSTREAM_DEBUG_H

#include "sl_iostream.h"
#include "sl_status.h"
#include "sl_iostream_swo_itm_8.h"

#ifdef __cplusplus
extern "C" {
#endif
// -----------------------------------------------------------------------------
// Global Variables

extern sl_iostream_t *sl_iostream_debug_handle;
extern sl_iostream_instance_info_t sl_iostream_instance_debug_info;

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function sets up the debug IO stream, configuring it to use SWO
 * stimulus 8 for output. It must be called before any debug IO
 * operations are performed to ensure that the debug stream is properly
 * initialized and set as the default system IO stream. This function is
 * typically used in systems where debug output is required, and it
 * ensures that the debug stream is ready for use. The function returns a
 * status code indicating the success or failure of the initialization
 * process.
 *
 * @return Returns SL_STATUS_OK on successful initialization or SL_STATUS_FAIL
 * if the initialization fails.
 ******************************************************************************/
sl_status_t sl_iostream_debug_init(void);

/***************************************************************************//**
 * @brief This function sets the message type for the debug stream, which
 * determines the format or type of messages that will be used in the
 * debug output. It should be called when you need to change the message
 * type for debugging purposes. The function assumes that the debug
 * stream has been initialized and is ready to accept a new message type.
 * It does not perform any validation on the input type, so it is the
 * caller's responsibility to ensure that the provided type is valid.
 *
 * @param type Specifies the message type for the debug stream. The value should
 * be a valid `sl_iostream_swo_itm_8_msg_type_t` enumeration. The
 * caller retains ownership of the value, and it must be a valid
 * type as no validation is performed by the function.
 * @return Returns `SL_STATUS_OK` to indicate that the message type was set
 * successfully.
 ******************************************************************************/
sl_status_t sl_iostream_set_debug_type(sl_iostream_swo_itm_8_msg_type_t type);

#ifdef __cplusplus
}
#endif

#endif /* SL_IOSTREAM_debug_H */
