/***************************************************************************//**
 * @file sl_token_manager.h
 * @brief TOKEN_MANAGER API definition.
 * @version 1.0.0
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup token_manager Token Manager
 * @{
 ******************************************************************************/

#ifndef TOKEN_MANAGER_H
#define TOKEN_MANAGER_H

#include <stdbool.h>
#if defined(USE_NVM3)
#include "nvm3.h"
#endif // defined(USE_NVM3)

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NVM3KEY_MFG_TOKEN_RANGE
//To store a manufacturing token in NVM3 region we shall follow NVM3 key format.
//Each object in the NVM3 is addressed by a 20-bit key
//4MSB bits(19:16) of the NVM3 key are used to identify the static tokens
//that are stored in NVM3 memory space. The value 0xF is used along with the
//exisitng static token ID to form a 20bit NVM3 key.
#define NVM3KEY_MFG_TOKEN_RANGE_ID_SHIFT     16u
#define NVM3KEY_MFG_TOKEN_RANGE_ID           0xF
#define NVM3KEY_MFG_TOKEN_RANGE              (NVM3KEY_MFG_TOKEN_RANGE_ID << NVM3KEY_MFG_TOKEN_RANGE_ID_SHIFT)
#endif

/***************************************************************************//**
 * @brief This function prepares the Token Manager for operation by initializing
 * the necessary NVM3 structures and ensuring that all tokens are
 * correctly set up. It must be called before any other token-related
 * functions to ensure proper functionality. If the initialization fails,
 * subsequent token operations will not work. The function handles the
 * creation of token objects in NVM3 if they do not exist or if their
 * type or size is incorrect. It also checks for cache overflow, which
 * may indicate that the cache size needs adjustment.
 *
 * @return Returns SL_STATUS_OK if the initialization is successful, or an error
 * code if it fails.
 ******************************************************************************/
sl_status_t sl_token_init(void);

/***************************************************************************//**
 * @brief This function retrieves data from a specified token, which can be
 * either a data token or a manufacturing token, based on the provided
 * NVM3 key. It should be called after the token manager has been
 * initialized successfully. The function handles both basic and indexed
 * tokens, and it supports reading from manufacturing tokens when NVM3
 * override is active. It returns a status code indicating success or the
 * type of error encountered.
 *
 * @param token The NVM3 key identifying the token to read from. It must be a
 * valid key, and if it falls within the manufacturing token range,
 * NVM3 override must be active.
 * @param index The index to access within an indexed token. For non-indexed
 * tokens, this should typically be zero.
 * @param data A pointer to a buffer where the retrieved token data will be
 * stored. Must not be null, and the buffer should be large enough
 * to hold the specified length of data.
 * @param length The number of bytes to read from the token. Must be a positive
 * value and should not exceed the size of the token data.
 * @return Returns an sl_status_t value indicating SL_STATUS_OK on success or an
 * error code if the operation fails.
 ******************************************************************************/
sl_status_t sl_token_get_data(uint32_t token,
                              uint32_t index,
                              void *data,
                              uint32_t length);

/***************************************************************************//**
 * @brief This function is used to write data to a specified token identified by
 * its NVM3 key. It supports both basic and indexed tokens, and can
 * handle manufacturing tokens if the NVM3 override is active. The
 * function must be called after the token manager has been initialized.
 * It returns a status code indicating success or the type of error
 * encountered. The function does not support writing to counter tokens
 * directly, and the data length must match the expected size for the
 * token.
 *
 * @param token The NVM3 key for the token. It must be a valid key, and if it
 * represents a manufacturing token, the NVM3 override must be
 * active.
 * @param index The index to access in the indexed token. For non-indexed
 * tokens, this should be set to 0.
 * @param data A pointer to the data being written. Must not be null and should
 * point to a buffer of at least 'length' bytes.
 * @param length The size of the token data in number of bytes. Must match the
 * expected size for the token.
 * @return Returns SL_STATUS_OK if the operation is successful, or an error code
 * if it fails.
 ******************************************************************************/
sl_status_t sl_token_set_data(uint32_t token,
                              uint32_t index,
                              void *data,
                              uint32_t length);

/***************************************************************************//**
 * @brief This function is used to increment the value of a counter token
 * identified by the given NVM3 key. It is important to note that this
 * function does not support manufacturing tokens. The function should be
 * called only when the token system is active, as indicated by the
 * initialization of the token manager. If the token system is not
 * active, the increment operation will be suppressed. The function
 * returns a status code indicating the success or failure of the
 * operation, and any errors encountered will trigger a callback for
 * handling.
 *
 * @param token The NVM3KEY for the token to be incremented. It must correspond
 * to a valid counter token and not a manufacturing token. The
 * function will handle invalid tokens by returning an error
 * status.
 * @return Returns SL_STATUS_OK if the increment operation is successful, or an
 * error code if it fails.
 ******************************************************************************/
sl_status_t sl_token_increment_counter(uint32_t token);

/***************************************************************************//**
 * @brief This function deletes a specified manufacturing token from the NVM3
 * region, but only if the NVM3 override is enabled. It is used when
 * there is a need to remove a manufacturing token, which is typically
 * stored in a dedicated non-volatile memory region. The function should
 * be called only when the NVM3 override is active, as it will return an
 * error if this condition is not met. This function is useful in
 * scenarios where manufacturing tokens need to be managed dynamically.
 *
 * @param token The NVM3 key for the manufacturing token to be deleted. It must
 * be a valid key that identifies a manufacturing token in the NVM3
 * region. If the key is invalid or if the NVM3 override is not
 * active, the function will return an error.
 * @return Returns SL_STATUS_OK if the token is successfully deleted, or an
 * error code if the operation fails, such as when the NVM3 override is
 * not active.
 ******************************************************************************/
sl_status_t sl_token_delete_token(uint32_t token);

/***************************************************************************//**
 * @brief This function retrieves data associated with a specified manufacturing
 * token, which is identified by a unique token key. It is used to access
 * non-volatile manufacturing data stored in the system. The function
 * requires a valid token key and a non-zero length to operate. If the
 * length is zero, the function returns an error. The data is copied into
 * the provided buffer, and the function returns a status indicating
 * success or the type of error encountered. This function should be
 * called after the token manager has been initialized.
 *
 * @param token The NVM3KEY for the token, identifying the specific
 * manufacturing data to be accessed. Must be a valid key.
 * @param index The index to access in the indexed token. Used for tokens that
 * support indexing.
 * @param data A pointer to a buffer where the token data will be placed. Must
 * not be null and should be large enough to hold the data specified
 * by length.
 * @param length The size of the token data in bytes. Must be greater than zero;
 * otherwise, the function returns an error.
 * @return Returns SL_STATUS_OK if the data is successfully read, or an error
 * code if the operation fails.
 ******************************************************************************/
sl_status_t sl_token_get_manufacturing_data(uint32_t token,
                                            uint32_t index,
                                            void *data,
                                            uint32_t length);

/***************************************************************************//**
 * @brief This function is used to write data to a manufacturing token, which is
 * a specific type of non-volatile memory token. It should be used when
 * you need to store data that is intended to be persistent across device
 * power cycles. The function requires that the token and data length are
 * aligned to 16-bit boundaries, and it only allows writing to tokens
 * that have not been previously written since the last erase. This
 * ensures that the data is not partially overwritten. The function must
 * be called with valid token addresses, and it will return an error if
 * the token address is illegal or if the data cannot be written.
 *
 * @param token The NVM3KEY for the token, which must be a valid address for a
 * manufacturing token. The token must be aligned to a 16-bit
 * boundary.
 * @param data A pointer to the data being written. The caller retains ownership
 * of the data, and it must not be null.
 * @param length The size of the token data in number of bytes. It must be
 * aligned to a 16-bit boundary and must not exceed the size of
 * the token.
 * @return Returns SL_STATUS_OK if the operation is successful, or an error code
 * if it fails.
 ******************************************************************************/
sl_status_t sl_token_set_manufacturing_data(uint32_t token,
                                            void *data,
                                            uint32_t length);

/** @} end token_manager */

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/***************************************************************************//**
 * Internal assert failed function.
 *
 * @note Minimal assert functionality to support token manager asserts.  In
 * the future a fully formed assert component will become available and make
 * this assert code obsolete.
 *
 * @param filename
 * @param linenumber
 ******************************************************************************/
void halInternalAssertFailed(const char * filename, int linenumber);

/***************************************************************************//**
 * @brief This function is a callback that indicates whether an assertion should
 * occur when the NVM3 cache overflows. It is intended for use during
 * development to help identify and adjust cache size issues. The default
 * behavior is to assert, but users can redefine this callback to change
 * the behavior, such as suppressing the assertion in production
 * environments.
 *
 * @return Returns true if an assertion should occur on cache overflow, false
 * otherwise.
 ******************************************************************************/
__WEAK bool sl_token_assert_on_cache_overflow_callback(void);

/**
 * @description External declaration of an array of NVM3 object keys.
 * Since the token systems identify tokens through an enum (see above
 * for the enum) and these two systems need to link NVM3 object keys to
 * their tokens, this array instantiates that link.
 *
 * Provides the "key" parameter: The NVM3 object key type.  The keys are found
 * in token-stack.h.
 */
extern const uint32_t tokenNvm3Keys[];

/**
 * @description External declaration of an array of creator codes.  Since
 * the token and sim_eeprom systems identify tokens through an enum (see
 * above for the enum) and these two systems need to link creator codes to
 * their tokens, this array instantiates that link.
 *
 * Provides the "creator" parameter: The creator code type.  The codes are
 * found in token-stack.h.
 */
extern const uint16_t tokenCreators[];

/**
 * @description External declaration of an array of IsCnt flags.  Since
 * the token and sim_eeprom systems identify tokens through an enum (see
 * below for the enum) and these two systems need to know which tokens
 * are counter tokens, this array provides that information.
 *
 * Provides the "iscnt" parameter: The flag indicating if the token is a
 * counter.  The iscnt's are found in token-stack.h.
 */
extern const bool tokenIsCnt[];

/**
 * @description External declaration of an array of IsIdx flags.  Since
 * the token and sim_eeprom systems identify tokens through an enum (see
 * below for the enum) and these two systems need to know which tokens
 * are indexed tokens, this array provides that information.
 *
 * Provides the "isidx" parameter: The flag indicating if the token is an
 * indexed token. The isidx's are found in token-stack.h.
 */
extern const bool tokenIsIdx[];

/**
 * @description External declaration of an array of sizes.  Since
 * the token and sim_eeprom systems identify tokens through an enum (see
 * below for the enum) and these two systems need to know the size of each
 * token, this array provides that information.
 *
 * Provides the "type" parameter: The token type.  The types are found
 * in token-stack.h.
 */
extern const uint8_t tokenSize[];

/**
 * @description External declaration of an array of array sizes.  Since
 * the token and sim_eeprom systems identify tokens through an enum (see
 * below for the enum) and these two systems need to know the array size of
 * each token, this array provides that information.
 *
 * Provides the "arraysize" parameter: The array size.
 */
extern const uint8_t tokenArraySize[];

/**
 * @description External declaration of an array of all token default values.
 * This array is filled with pointers to the set of constant declarations of
 * all of the token default values.  Therefore, the index into this array
 * chooses which token's defaults to access, and the address offset chooses the
 * byte in the defaults to use.
 *
 * For example, to get the n-th byte of the i-th token, use:
 * uint8_t byte = *(((uint8_t *)tokenDefaults[i])+(n)
 *
 * Provides the "TOKEN_##name##_DEFAULTS" parameter: A constant declaration
 * of the token default values, generated for all tokens.
 */
extern const void * const tokenDefaults[];

/**
 * @description External declaration of a flag. When set to true
 * manufacturing token read/write happens through NVM3 in case of S3 devices.
 *
 * This flag should be set to false in case of S2 devices.
 */
extern bool nvm3OverrideActive;
#endif // DOXYGEN_SHOULD_SKIP_THIS

#ifdef __cplusplus
}
#endif

// ******** THE REST OF THE FILE IS DOCUMENTATION ONLY !***********************
/// @addtogroup token_manager Token Manager
/// @{
/// @brief Routines for working with tokens
///
///   @details
///
///
/// @n @section token_manager_intro Introduction
///
/// The token system stores such non-volatile information as the manufacturing
/// ID, channel number, transmit power, and various pieces of information
/// that the application needs to be persistent between device power cycles.
/// The token system is design to abstract implementation details and simplify
/// interacting with differing non-volatile systems.  The majority of tokens
/// are stored in NVM3 where they can be rewritten. Manufacturing tokens are
/// stored in dedicated regions of flash and are not designed to be rewritten.
///
/// By default the Token Manager pulls in the NVM3 component for storage.
/// The "Token Manager using NVM3" still requires the "Token Manager" component.
///
/// @} end token_manager ******************************************************/

#endif /* TOKEN_MANAGER_H */
