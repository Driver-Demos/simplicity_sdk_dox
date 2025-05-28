/***************************************************************************/ /**
 * @file
 * @brief CPC Security Endpoint of the Secondary
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

#ifndef SL_CPC_SECURITY_SECONDARY_H
#define SL_CPC_SECURITY_SECONDARY_H

#include <stdint.h>
#include "sl_status.h"
#include "sl_enum.h"
#include "psa/crypto.h"
#include "sl_slist.h"
#include "psa_crypto_storage.h"
#include "sl_cpc_security_config.h"

/// The security state enabled bit mask
#define SL_CPC_SECURITY_STATE_ENABLE_MASK (1 << 0)

/// The security state bounded bit mask
#define SL_CPC_SECURITY_STATE_BOUND_MASK  (1 << 1)

// Binding types
#define  SL_CPC_SECURITY_BINDING_KEY_CUSTOMER_SPECIFIC    0x03 ///< Security binding key customer specific.
#define  SL_CPC_SECURITY_BINDING_KEY_ECDH                 0x02 ///< Security binding key ECDH.
#define  SL_CPC_SECURITY_BINDING_KEY_PLAINTEXT_SHARE      0x01 ///< Security binding key plaintex share.
#define  SL_CPC_SECURITY_BINDING_KEY_NONE                 0x00 ///< Security binding key none.

#define SL_CPC_SECURITY_OK_TO_UNBIND 0xAAAAAAAAAAAAAAAA        ///< Security ok to unbind.

/***************************************************************************/ /**
 * @addtogroup cpc-security CPC-Security
 * @{
 ******************************************************************************/

/// @brief Enumeration representing security state.
SL_ENUM_GENERIC(sl_cpc_security_state_t, uint8_t)
{
  SL_CPC_SECURITY_STATE_NOT_READY     = 0x00,         ///< Security is not yet setup, no I-frame allowed
  SL_CPC_SECURITY_STATE_DISABLED      = 0x01,         ///< Security is not enabled, cleartext I-frame are allowed
  SL_CPC_SECURITY_STATE_INITIALIZING  = 0x02,         ///< Security is being initialized, raw I-frame are only allowed on security endpoint
  SL_CPC_SECURITY_STATE_RESETTING     = 0x03,         ///< Security is being reset
  SL_CPC_SECURITY_STATE_WAITING_ON_TX_COMPLETE = 0x4, ///< Response to setup the security session is about to be sent, waiting for write completion
  SL_CPC_SECURITY_STATE_INITIALIZED   = 0x05,         ///< Security is initialized, only encrypted I-frame are allowed
};

/***************************************************************************//**
 * Typedef for the user-supplied callback function, which is called when
 * unbinding (authorized).
 *
 * @param data   User-specific argument.
 ******************************************************************************/
typedef void (*sl_cpc_unbind_notification_t)(void *data);

/***************************************************************************//**
 * @brief The `sl_cpc_unbind_notification_handle_t` structure is designed to
 * manage unbind notifications within the CPC security framework. It
 * contains a callback function pointer (`fnct`) that is invoked when an
 * unbind event occurs, a list node (`node`) for integrating the handle
 * into a singly linked list, and a user-specific data pointer (`data`)
 * that is passed to the callback function. This structure facilitates
 * the registration and management of unbind event notifications,
 * allowing users to define custom behavior when such events are
 * triggered.
 *
 * @param fnct Notification Callback function pointer.
 * @param node Single list node for linking in a list.
 * @param data User-specific argument passed to the callback.
 ******************************************************************************/
typedef struct {
  sl_cpc_unbind_notification_t fnct;      ///< Notification Callback
  sl_slist_node_t node;                   ///< Single list node
  void *data;                             ///< User-specific argument
} sl_cpc_unbind_notification_handle_t;

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************//**
 * @brief This function is used to obtain the current setup status of the CPC
 * security subsystem. It can be called at any time to check the state of
 * the security, which is useful for determining if security features are
 * enabled, initializing, or in other states. This function is thread-
 * safe and can be used in concurrent environments without additional
 * synchronization.
 *
 * @return Returns the current security state as an enumeration of type
 * `sl_cpc_security_state_t`, which indicates the specific state of the
 * security subsystem.
 ******************************************************************************/
sl_cpc_security_state_t sl_cpc_security_get_state(void);

/***************************************************************************//**
 * @brief This function is a user-defined callback intended to supply a binding
 * key for the CPC security subsystem. It must be implemented by the user
 * when the configuration is set to use a customer-specific binding key.
 * The function is expected to provide a 16-byte encryption key and its
 * size. It is crucial to override this function in your application to
 * ensure the security subsystem can operate correctly, as the default
 * implementation does not provide a key.
 *
 * @param key A pointer to a pointer where the function should store the address
 * of the encryption key. The caller does not take ownership of the
 * key, but the key must remain valid for the duration of its use.
 * @param key_size_in_bytes A pointer to a uint16_t where the function should
 * store the size of the encryption key in bytes. Only
 * a key size of 16 bytes is supported.
 * @return None
 ******************************************************************************/
void sl_cpc_security_fetch_user_specified_binding_key(uint8_t **key, uint16_t *key_size_in_bytes);

/***************************************************************************//**
 * @brief This function determines whether an unbind request should be
 * authorized, based on the encryption status of the link. It is declared
 * as a weak symbol, meaning that if no strong definition is provided by
 * the user, unbind requests will be denied by default. Users can
 * override this function to provide custom logic for authorizing unbind
 * requests. The function should return a specific value to allow
 * unbinding, otherwise, it will deny the request.
 *
 * @param is_link_encrypted A boolean indicating whether the link is encrypted.
 * The function does not use this parameter in its
 * default implementation, but it may be used in a
 * user-provided override to influence the decision to
 * authorize an unbind request.
 * @return Returns a 64-bit integer indicating whether unbinding is allowed. To
 * authorize unbinding, return SL_CPC_SECURITY_OK_TO_UNBIND; otherwise,
 * return any other value to deny the request.
 ******************************************************************************/
uint64_t sl_cpc_security_on_unbind_request(bool is_link_encrypted);

/***************************************************************************//**
 * @brief This function is used to unbind a device from its current security
 * binding, which involves notifying all registered observers of the
 * unbind event and erasing the binding key. It should be called when the
 * device needs to be unbound, such as during a reset or reconfiguration
 * process. The function will invoke any registered unbind notification
 * callbacks, passing the user-specific data to them. It is important to
 * ensure that any necessary unbind observers are registered before
 * calling this function to handle the unbind event appropriately.
 *
 * @return Returns a status code indicating the success or failure of the unbind
 * operation.
 ******************************************************************************/
sl_status_t sl_cpc_security_unbind(void);

/***************************************************************************//**
 * @brief Use this function to register a callback that will be invoked when an
 * unbind event occurs. This is useful for handling unbind notifications
 * in a custom manner. The function requires a valid handle and callback
 * function pointer. It must be called with non-null parameters to
 * succeed. If either the handle or callback is null, the function will
 * return an error status. This function is typically used in scenarios
 * where the application needs to be notified of unbind events to perform
 * specific actions.
 *
 * @param handle A pointer to an sl_cpc_unbind_notification_handle_t structure.
 * This must not be null and is used to store the callback and
 * user data.
 * @param callback A pointer to a function of type sl_cpc_unbind_notification_t.
 * This callback will be called when an unbind event occurs. It
 * must not be null.
 * @param data A pointer to user-specific data that will be passed to the
 * callback function. This can be null if no user data is needed.
 * @return Returns SL_STATUS_OK on success, or SL_STATUS_NULL_POINTER if either
 * the handle or callback is null.
 ******************************************************************************/
sl_status_t sl_cpc_security_unbind_subscribe(sl_cpc_unbind_notification_handle_t *handle,
                                             sl_cpc_unbind_notification_t callback,
                                             void *data);

/***************************************************************************//**
 * @brief Use this function to remove a previously registered callback for
 * unbind notifications. It should be called when the callback is no
 * longer needed, such as during cleanup or when the application no
 * longer requires unbind notifications. The function requires a valid
 * handle that was used in a prior subscription. If the handle is null,
 * the function returns an error status indicating a null pointer.
 *
 * @param handle A pointer to the unbind notification handle. Must not be null.
 * The handle should have been previously used in a successful
 * call to sl_cpc_security_unbind_subscribe. If null, the function
 * returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on success, or SL_STATUS_NULL_POINTER if the
 * handle is null.
 ******************************************************************************/
sl_status_t sl_cpc_security_unbind_unsubscribe(sl_cpc_unbind_notification_handle_t *handle);

#ifdef __cplusplus
}
#endif

/** @} (end addtogroup cpc_security_secondary) */

#endif /* SL_CPC_SECURITY_SECONDARY_H */
