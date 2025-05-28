/***************************************************************************//**
 * @file
 * @brief IO Stream STDIO Component.
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

#ifndef SL_IOSTREAM_STDIO_H
#define SL_IOSTREAM_STDIO_H

#include "sl_iostream.h"
#include "sl_status.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup iostream
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup iostream_stdio I/O Stream STDIO
 * @brief I/O Stream STDIO
 * @details
 * ## Overview
 *
 *   This is a component for internal testing in simulation. It allows to use the
 *   standard process standard input/output (stdio). Note that this component cannot be
 *   used with the component "iostream_retarget_stdio".
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
 *   The power manager is not supported. This module doesn't add any requirement on
 *   energy mode
 *
 * @{
 ******************************************************************************/

extern sl_iostream_t *sl_iostream_stdio_handle;
extern sl_iostream_instance_info_t sl_iostream_instance_stdio_info;

/***************************************************************************//**
 * @brief This function initializes the standard input/output (STDIO) stream and
 * sets it as the default stream for the system. It is intended for use
 * in environments where standard I/O operations are required, such as
 * internal testing in simulation. This function should be called during
 * the initialization phase of your application if you intend to use the
 * STDIO stream. Note that if multiple streams are initialized, the last
 * one initialized will be set as the system default. This function does
 * not support power management integration, and it is incompatible with
 * the 'iostream_retarget_stdio' component.
 *
 * @return Returns SL_STATUS_OK to indicate successful initialization.
 ******************************************************************************/
sl_status_t sl_iostream_stdio_init(void);

/** @} (end addtogroup iostream_stdio) */
/** @} (end addtogroup iostream) */

#ifdef __cplusplus
}
#endif

#endif // SL_IOSTREAM_STDIO_H
