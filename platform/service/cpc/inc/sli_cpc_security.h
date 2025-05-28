/***************************************************************************/ /**
 * @file
 * @brief CPC Security Endpoint of the Secondary
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SLI_CPC_SECURITY_SECONDARY_H
#define SLI_CPC_SECURITY_SECONDARY_H

#include "sl_cpc_security.h"
#include "sli_cpc_types.h"

#define SLI_CPC_SECURITY_PROTOCOL_BINDING_TYPE_PLAINTEXT 0x00
#define SLI_CPC_SECURITY_PROTOCOL_BINDING_TYPE_ECDH      0x01

#define SLI_CPC_SECURITY_PROTOCOL_RESPONSE_MASK 0x8000

#define SLI_SECURITY_BINDING_KEY_LENGTH_BYTES         16
#define SLI_SECURITY_PUBLIC_KEY_LENGTH_BYTES          32
#define SLI_SECURITY_SESSION_KEY_LENGTH_BYTES         32
#define SLI_SECURITY_SESSION_ID_LENGTH_BYTES          7
#define SLI_SECURITY_SESSION_INIT_RANDOM_LENGTH_BYTES 64
#define SLI_SECURITY_SHA256_LENGTH_BYTES              32
#define SLI_SECURITY_TAG_LENGTH_BYTES                 8
#define SLI_SECURITY_NONCE_FRAME_COUNTER_MAX_VALUE    (1UL << 29)
#define SLI_SECURITY_NONCE_FRAME_COUNTER_PRIMARY_ENCRYPT_BITMASK (1UL << 31)

#define SLI_SECURITY_PROTOCOL_PAYLOAD_MAX_LENGTH (sizeof(sl_status_t) + SLI_SECURITY_SESSION_INIT_RANDOM_LENGTH_BYTES)
#define SLI_SECURITY_PROTOCOL_HEADER_LENGTH (sizeof(uint16_t) + sizeof(sli_cpc_security_id_t))

SL_ENUM_GENERIC(sli_cpc_security_id_t, uint16_t)
{
  BINDING_REQUEST_ID       = 0x0001,
  PLAIN_TEXT_KEY_SHARE_ID  = 0x0002,
  PUBLIC_KEY_SHARE_ID      = 0x0003,
  SESSION_INIT_ID          = 0x0004,
  UNBIND_REQUEST_ID        = 0x0005
};

SL_ENUM_GENERIC(sli_cpc_security_binding_request_t, uint8_t)
{
  PLAIN_TEXT_KEY_SHARE_BINDING_REQUEST = 0x00,
  ECDH_BINDING_REQUEST = 0x01
};

/***************************************************************************//**
 * @brief The `sli_cpc_security_protocol_cmd_info_t` structure is used to
 * encapsulate information about a security protocol command within the
 * CPC (Connected Protocol Controller) security framework. It includes
 * fields for the length of the request and response, as well as a
 * command identifier, which is crucial for managing and processing
 * security-related commands in the CPC system.
 *
 * @param request_len Specifies the length of the request in bytes.
 * @param response_len Specifies the length of the response in bytes.
 * @param command_id Identifies the command using a security ID.
 ******************************************************************************/
typedef struct {
  uint16_t request_len;
  uint16_t response_len;
  sli_cpc_security_id_t command_id;
} sli_cpc_security_protocol_cmd_info_t;

typedef __PACKED_STRUCT {
  uint16_t len;
  sli_cpc_security_id_t command_id;
  uint8_t payload[SLI_SECURITY_PROTOCOL_PAYLOAD_MAX_LENGTH];
} sli_cpc_security_protocol_cmd_t;

typedef __PACKED_STRUCT {
  sl_status_t status;
  uint8_t random2[SLI_SECURITY_SESSION_INIT_RANDOM_LENGTH_BYTES];
} sli_cpc_security_session_init_response_t;

typedef __PACKED_STRUCT {
  uint8_t endpoint_id;
  uint8_t session_id[7];
  uint32_t frame_counter;
} sli_cpc_security_nonce_t;

/***************************************************************************//**
 * Typedef for callback function to let the core know when a state change
 * occurs in the security subsystem.
 *
 * @param old_state   Previous state.
 * @param new_state   New state.
 ******************************************************************************/
typedef void (*sli_cpc_on_security_state_change_t)(sl_cpc_security_state_t old_state,
                                                   sl_cpc_security_state_t new_state);

#ifdef __cplusplus
extern "C"
{
#endif

/***************************************************************************//**
 * @brief This function sets up the security endpoint for the CPC (Co-Processor
 * Communication) framework, preparing it for secure communication. It
 * should be called before any security-related operations are performed.
 * The function can optionally accept a callback to notify the
 * application of any changes in the security state. It is important to
 * ensure that the security subsystem is properly configured before
 * calling this function, as improper configuration may lead to
 * initialization failure.
 *
 * @param state_change_cb A callback function of type
 * `sli_cpc_on_security_state_change_t` that is invoked
 * when the security state changes. This parameter can be
 * `NULL` if no callback is needed. The caller retains
 * ownership of this function pointer.
 * @return Returns `SL_STATUS_OK` on successful initialization, or an error code
 * if initialization fails.
 ******************************************************************************/
sl_status_t sli_cpc_security_init(sli_cpc_on_security_state_change_t state_change_cb);

/***************************************************************************//**
 * @brief This function manages the security operations for the CPC security
 * endpoint, ensuring that any pending security commands are processed
 * and that the endpoint is recovered if it encounters an error. It
 * should be called regularly to maintain the security endpoint's
 * functionality, especially after initialization. The function handles
 * reading security commands, processing them, and sending requests if
 * necessary. It is important to ensure that the security subsystem is
 * initialized before calling this function to avoid undefined behavior.
 *
 * @return None
 ******************************************************************************/
void sli_cpc_security_process(void);

/***************************************************************************//**
 * @brief This function encrypts a message by replacing the content in the
 * provided payload buffer with encrypted data. It requires the security
 * subsystem to be initialized beforehand. The function operates on a
 * specified endpoint and uses the provided header for authenticated but
 * non-encrypted data. Upon successful encryption, a security tag is
 * generated and stored in the provided tag buffer. This function is
 * essential for securing communications over the specified endpoint.
 *
 * @param ep A pointer to the sl_cpc_endpoint_t structure representing the
 * endpoint on which to operate. Must not be null.
 * @param header A pointer to a buffer containing authenticated and non-
 * encrypted data. Must not be null.
 * @param header_len The length of the authenticated data buffer. Must be a
 * valid size for the provided header.
 * @param payload A pointer to a buffer containing data to be encrypted. The
 * buffer is modified in place to contain the encrypted data.
 * Must not be null.
 * @param payload_len The length of cleartext data in the payload buffer. Must
 * be a valid size for the provided payload.
 * @param tag A pointer to a buffer where the security tag will be stored. Must
 * not be null.
 * @param tag_len The length of the security tag buffer. Must be sufficient to
 * store the generated tag.
 * @return Returns an sl_status_t indicating the success or failure of the
 * encryption operation.
 ******************************************************************************/
sl_status_t sli_cpc_security_encrypt(sl_cpc_endpoint_t *ep,
                                     const uint8_t *header, const size_t header_len,
                                     uint8_t *paylaod, const size_t payload_len,
                                     uint8_t *tag, const size_t tag_len);

/***************************************************************************//**
 * @brief This function is used to decrypt a message payload that has been
 * encrypted, ensuring that the security subsystem is initialized before
 * calling. It operates on a specified endpoint and requires both the
 * header and payload buffers. The function will replace the encrypted
 * content in the payload buffer with the decrypted plaintext if
 * successful. It is important to provide a buffer size to prevent
 * overflow during decryption. The function also updates the output
 * length to reflect the size of the decrypted data.
 *
 * @param ep A pointer to the sl_cpc_endpoint_t structure representing the
 * endpoint on which to operate. Must not be null.
 * @param header A pointer to a buffer containing authenticated and non-
 * encrypted data. Must not be null.
 * @param header_len The length of the authenticated data buffer. Must be a
 * valid size for the provided header.
 * @param payload A pointer to a buffer containing the encrypted data. This
 * buffer will be overwritten with the decrypted data upon
 * success. Must not be null.
 * @param buffer_size The size of the payload buffer, used to prevent overflow
 * during decryption. Must be large enough to hold the
 * decrypted data.
 * @param payload_len The length of the encrypted data in the payload buffer.
 * Must be a valid size for the provided payload.
 * @param output_len A pointer to a size_t variable where the length of the
 * decrypted plaintext data will be stored upon successful
 * decryption. Must not be null.
 * @return Returns an sl_status_t indicating the success or failure of the
 * decryption operation. On success, the payload buffer is modified to
 * contain the decrypted data, and output_len is updated with the length
 * of the decrypted data.
 ******************************************************************************/
sl_status_t sli_cpc_security_decrypt(sl_cpc_endpoint_t *ep,
                                     const uint8_t *header, const size_t header_len,
                                     uint8_t *payload, const size_t buffer_size,
                                     const size_t payload_len, size_t *output_len);

/***************************************************************************//**
 * @brief This function is used to decrement the frame counter for a specific
 * endpoint when a received encrypted message is aborted. It should be
 * called when a message is rejected and not decrypted, ensuring that the
 * frame counter accurately reflects the number of processed frames. This
 * function only affects the frame counter if the security subsystem is
 * in the initialized state. It is important to ensure that the security
 * subsystem is properly initialized before calling this function.
 *
 * @param ep A pointer to an `sl_cpc_endpoint_t` structure representing the
 * endpoint on which to operate. This parameter must not be null, and
 * the endpoint should be valid and initialized. If the security state
 * is not initialized, the function will not modify the frame counter.
 * @return None
 ******************************************************************************/
void sli_cpc_security_rollback_decrypt(sl_cpc_endpoint_t *ep);

#ifdef __cplusplus
}
#endif

#endif  // SLI_CPC_SECURITY_SECONDARY_H
