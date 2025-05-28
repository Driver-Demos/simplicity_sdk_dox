/***************************************************************************//**
 * @file
 * @brief IO Stream SWO Component.
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
#ifndef SL_IOSTREAM_SWO_H
#define SL_IOSTREAM_SWO_H

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
 * @addtogroup iostream_swo I/O Stream SWO
 * @brief I/O Stream SWO
 * @details
 * ## Overview
 *
 *   Serial Wire Output (SWO) is an output stream only, which means it is impossible
 *   to receive data. SWO has a dedicated pin allowing the CPU to output data in
 *   real-time.
 *
 *   IOStream-SWO uses channel 0 for its own purpose.The module also set the SWO output
 *   frequency around 863 kHz and uses the UART encoding .
 *
 * ## Initialization
 *
 *   The stream sets itself as the default stream at the end of the initialization
 *   function.You must reconfigure the default interface if you have multiple streams
 *   in your project else the last stream initialized will be set as the system default
 *   stream.
 *
 * ## Communication channel connection
 *
 *   For connecting to the SWO channel you can use the tools provided by Segger or you
 *   can open a telnet session and connect to the port 4091 using your host IP
 *   address when the debugger is connected using USB and using J-Link debugger IP address
 *   when your debugger is connected over ethernet.
 *
 * @{
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Global Variables

extern sl_iostream_t *sl_iostream_swo_handle;                       ///< sl_iostream_swo_handle
extern sl_iostream_instance_info_t sl_iostream_instance_swo_info;   ///< sl_iostream_instance_swo_info

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function sets up the Serial Wire Output (SWO) stream as the
 * default I/O stream for the system. It should be called during the
 * initialization phase of your application to enable SWO functionality.
 * The function configures the necessary hardware and software components
 * to allow real-time data output via the SWO pin. If the system includes
 * an operating system kernel, a mutex is created to manage access to the
 * SWO stream. Additionally, if a power manager is present, the function
 * subscribes to energy mode transition events. The function must be
 * called before any SWO data output operations are performed. It returns
 * a status code indicating success or failure of the initialization
 * process.
 *
 * @return Returns SL_STATUS_OK on successful initialization or SL_STATUS_FAIL
 * if an error occurs during the process.
 ******************************************************************************/
sl_status_t sl_iostream_swo_init(void);

/***************************************************************************//**
 * @brief This function is used to de-initialize the Serial Wire Output (SWO)
 * stream component, ensuring that any resources allocated during
 * initialization are properly released. It should be called when the SWO
 * stream is no longer needed, typically during the shutdown or
 * reconfiguration of the system. The function handles necessary cleanup
 * operations, such as releasing mutexes and unsubscribing from power
 * management events, if applicable. It is important to ensure that no
 * other operations are being performed on the SWO stream when this
 * function is called to avoid undefined behavior.
 *
 * @return Returns SL_STATUS_OK if the de-initialization is successful, or
 * SL_STATUS_FAIL if an error occurs during the process.
 ******************************************************************************/
sl_status_t sl_iostream_swo_deinit(void);

/** @} (end addtogroup iostream) */
/** @} (end addtogroup iostream_swo) */

#ifdef __cplusplus
}
#endif

#endif // SL_IOSTREAM_SWO_H
