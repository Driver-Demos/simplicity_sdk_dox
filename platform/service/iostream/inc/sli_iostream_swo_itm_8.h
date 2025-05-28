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
#ifndef SLI_IOSTREAM_SWO_ITM_8_H
#define SLI_IOSTREAM_SWO_ITM_8_H

#include "sl_iostream.h"
#include "sl_status.h"
#include "sl_iostream_swo_itm_8.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @brief This function initializes the Serial Wire Output (SWO) Instrumentation
 * Trace Macrocell (ITM) for an I/O stream with an 8-bit configuration.
 * It should be called before any operations are performed on the SWO ITM
 * stream to ensure proper setup. If the system is configured with a
 * kernel, it returns a context pointer that can be used for further
 * operations; otherwise, it returns NULL. The function ensures that the
 * initialization is performed only once, even if called multiple times.
 * It may also set up additional resources like mutexes and power
 * management subscriptions if the relevant features are enabled in the
 * system.
 *
 * @return Returns a pointer to the context if a kernel is present, otherwise
 * returns NULL.
 ******************************************************************************/
void *sli_iostream_swo_itm_8_init(void);

/***************************************************************************//**
 * @brief This function is used to send a data packet through the SWO ITM
 * interface, formatted with specific protocol requirements. It should be
 * called when there is a need to transmit data with a defined message
 * type and sequence number. The function requires a valid context if the
 * kernel is present, and it handles synchronization using a mutex. The
 * buffer provided must contain the data to be sent, and the function
 * will format this data into a packet with additional protocol-specific
 * bytes. The function assumes the kernel is running if the context is
 * provided and will acquire and release a mutex accordingly. It returns
 * a status indicating success or failure of the write operation.
 *
 * @param context A pointer to the context used for synchronization when the
 * kernel is present. Must be valid if the kernel is running;
 * otherwise, it can be null.
 * @param buffer A pointer to the data to be written. Must not be null and
 * should point to a valid memory region containing the data to be
 * sent.
 * @param buffer_length The length of the data in the buffer. Must be a non-
 * negative value representing the number of bytes to send.
 * @param type The message type to be included in the packet. Must be a valid
 * value of type sl_iostream_swo_itm_8_msg_type_t.
 * @param seq_nbr The sequence number to be included in the packet. Must be a
 * valid 8-bit unsigned integer.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, and SL_STATUS_FAIL is
 * returned if the operation fails, such as when mutex acquisition
 * fails.
 ******************************************************************************/
sl_status_t sli_iostream_swo_itm_8_write(void *context,
                                         const void *buffer,
                                         size_t buffer_length,
                                         sl_iostream_swo_itm_8_msg_type_t type,
                                         uint8_t seq_nbr);

#ifdef __cplusplus
}
#endif

#endif /* SLI_IOSTREAM_SWO_ITM_8_H */
