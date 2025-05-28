/***************************************************************************/ /**
 * @file
 * @brief CPC system endpoint primary part
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

#ifndef SL_CPC_SYSTEM_PRIMARY_H
#define SL_CPC_SYSTEM_PRIMARY_H

#include "sl_status.h"
#include "sli_cpc_system_common.h"
#include "sli_cpc_timer.h"
#include "sl_slist.h"

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************/ /**
 * @addtogroup cpc_system_primary
 * @brief CPC System Endpoint Primary
 * @details
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief The `sl_cpc_system_event_listener_t` structure is designed to manage
 * event listeners for system reset events in a CPC (Co-Processor
 * Communication) system. It contains a linked list node to allow
 * multiple listeners to be organized in a list, and a callback function
 * pointer that is triggered when a reset event is detected, providing
 * the reason for the reset as a parameter. This structure facilitates
 * the registration and management of callbacks that respond to system-
 * level events, particularly resets, in a modular and extensible manner.
 *
 * @param node A linked list node used to chain event listeners together.
 * @param on_reset_callback A function pointer to a callback function that is
 * invoked when a system reset event occurs, taking a
 * reset reason as an argument.
 ******************************************************************************/
typedef struct {
  sl_slist_node_t node;
  void (*on_reset_callback)(sl_cpc_system_status_t reset_reason);
} sl_cpc_system_event_listener_t;

/***************************************************************************//**
 * @brief This function is a weak stub intended to be overridden by the user to
 * handle unsolicited PROP_LAST_STATUS messages from the secondary
 * system. It is invoked when the primary system receives a PROPERTY-IS
 * command with the PROP_LAST_STATUS property without a registered
 * callback. This function should be implemented to appropriately respond
 * to the status received, which indicates various reset reasons. It is
 * important to ensure that this function is re-implemented as a strong
 * function to provide the desired behavior in response to these
 * unsolicited messages.
 *
 * @param status The unsolicited PROP_LAST_STATUS value indicating the reset
 * reason. It must be a valid sl_cpc_system_status_t value,
 * typically representing different reset conditions such as
 * power-on or watchdog resets.
 * @return None
 ******************************************************************************/
SL_WEAK void sl_cpc_system_unsolicited_last_status_callback(sl_cpc_system_status_t status);

/***************************************************************************//**
 * @brief This function prepares an `sl_cpc_system_event_listener_t` structure
 * for use by setting all its fields to zero. It must be called before
 * the listener is used in any other operations, such as subscribing to
 * events. This ensures that the structure is in a known state and
 * prevents undefined behavior from uninitialized data. The function does
 * not perform any dynamic memory allocation or error checking, so the
 * caller must ensure that the `listener` parameter is a valid, non-null
 * pointer to a properly allocated `sl_cpc_system_event_listener_t`
 * structure.
 *
 * @param listener A pointer to an `sl_cpc_system_event_listener_t` structure
 * that will be initialized. Must not be null. The caller
 * retains ownership and is responsible for ensuring the
 * structure is properly allocated before calling this function.
 * @return None
 ******************************************************************************/
void sl_cpc_system_event_listener_init(sl_cpc_system_event_listener_t *listener);

/***************************************************************************//**
 * @brief This function registers an event listener that will be notified when
 * specific system events occur on the secondary device. It should be
 * used to add a listener that has been initialized and is ready to
 * handle events. The listener must be properly initialized before
 * calling this function, and it should not be null. This function is
 * typically called during the setup phase of the application to ensure
 * that the listener is in place before any relevant events occur.
 *
 * @param listener A pointer to an sl_cpc_system_event_listener_t structure that
 * has been initialized. The listener must not be null, and the
 * caller retains ownership of the listener structure. If the
 * listener is null, the behavior is undefined.
 * @return Returns SL_STATUS_OK on successful subscription of the event
 * listener.
 ******************************************************************************/
sl_status_t sl_cpc_system_event_listener_subscribe(sl_cpc_system_event_listener_t *listener);

/***************************************************************************//**
 * @brief Use this function to unsubscribe an event listener that was previously
 * subscribed to receive system event notifications. This is typically
 * done when the listener is no longer needed or before the listener is
 * destroyed to prevent it from being called after it is invalid. The
 * function must be called with a valid listener that has been previously
 * subscribed. It is important to ensure that the listener is not null
 * and has been properly initialized before calling this function.
 *
 * @param listener A pointer to an sl_cpc_system_event_listener_t structure that
 * represents the event listener to be removed. The listener
 * must not be null and should have been previously subscribed
 * using sl_cpc_system_event_listener_subscribe. The caller
 * retains ownership of the listener.
 * @return Returns SL_STATUS_OK to indicate successful removal of the listener
 * from the notification list.
 ******************************************************************************/
sl_status_t sl_cpc_system_event_listener_unsubscribe(sl_cpc_system_event_listener_t *listener);

/** @} (end addtogroup cpc_system_primary) */

#ifdef __cplusplus
}
#endif

#endif /* SL_CPC_SYSTEM_PRIMARY_H_ */
