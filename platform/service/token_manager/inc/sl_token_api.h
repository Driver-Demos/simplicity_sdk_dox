/***************************************************************************//**
 * @file sl_token_api.h
 * @brief Header file for the Legacy HAL "shim" layer support for the token API
 * @version 5.6.0
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories, Inc, www.silabs.com</b>
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
#ifndef _TOKEN_API_H_
#define _TOKEN_API_H_

#include STACK_TYPES_HEADER

#include "sl_status.h"

#ifdef TOKEN_MANAGER_TEST
void token_manager_test_assert_handler(const char *filename, int linenumber);
#define ASSERT_HANDLER token_manager_test_assert_handler
#else
void halInternalAssertFailed(const char * filename, int linenumber);
#define ASSERT_HANDLER halInternalAssertFailed
#endif
#ifndef __SOURCEFILE__
#define __SOURCEFILE__ __FILE__
#endif // __SOURCEFILE__
#undef  assert // Replace assert from #include <assert.h> with this one
#define assert(condition)  \
  do { if (!(condition)) { \
         ASSERT_HANDLER(__SOURCEFILE__, __LINE__); } } while (0)

#include "sl_token_manufacturing_api.h"

#define DEFINETYPES
#include "stack/config/token-stack.h"
#undef DEFINETYPES

#define DEFINETOKENS

#define TOKEN_DEF(name, creator, iscnt, isidx, type, arraysize, ...) \
  TOKEN_##name,
enum {
    #include "stack/config/token-stack.h"
  TOKEN_COUNT
};
#undef TOKEN_DEF

//Define the size (in bytes) of all tokens/
#define TOKEN_DEF(name, creator, iscnt, isidx, type, arraysize, ...) \
  TOKEN_##name##_SIZE = sizeof(type),
enum {
  #include "stack/config/token-stack.h"
};
#undef TOKEN_DEF

#undef DEFINETOKENS

#define halCommonGetToken(data, token) \
  halInternalGetTokenData(data, token, 0x7F, token##_SIZE)

#define halCommonGetIndexedToken(data, token, index) \
  halInternalGetTokenData(data, token, index, token##_SIZE)

#define halStackGetIndexedToken(data, token, index, size) \
  halInternalGetTokenData(data, token, index, size)

#define halCommonSetToken(token, data) \
  halInternalSetTokenData(token, 0x7F, data, token##_SIZE)

#define halCommonSetIndexedToken(token, index, data) \
  halInternalSetTokenData(token, index, data, token##_SIZE)

#define halStackSetIndexedToken(token, index, data, size) \
  halInternalSetTokenData(token, index, data, size)

#define halCommonIncrementCounterToken(token) \
  halInternalIncrementCounterToken(token)

/***************************************************************************//**
 * @brief This function initializes the token system used by the stack, which is
 * essential for managing persistent data storage. It should be called
 * during the system initialization phase before any token operations are
 * performed. The function maps various NVM3 error codes to corresponding
 * status codes, allowing the caller to handle initialization errors
 * appropriately. It is important to check the return value to ensure
 * that the initialization was successful and to handle any specific
 * error conditions that may arise.
 *
 * @return Returns an sl_status_t code indicating the result of the
 * initialization, with specific codes for different error conditions
 * and a success code if initialization is successful.
 ******************************************************************************/
sl_status_t halStackInitTokens(void);

void halInternalGetTokenData(void *data, uint16_t token, uint8_t index, uint8_t len);

void halInternalSetTokenData(uint16_t token, uint8_t index, void *data, uint8_t len);

/***************************************************************************//**
 * @brief This function is used to increment the value of a counter token
 * identified by the given token ID. It is typically called when there is
 * a need to update the count associated with a specific token, such as
 * tracking the number of occurrences of an event. The function assumes
 * that the token ID provided corresponds to a valid counter token. It
 * should be used in contexts where token management is required, and the
 * caller must ensure that the token ID is valid and corresponds to a
 * counter token.
 *
 * @param token An 8-bit unsigned integer representing the ID of the counter
 * token to be incremented. The token ID must correspond to a valid
 * counter token. If an invalid token ID is provided, the behavior
 * is undefined.
 * @return None
 ******************************************************************************/
void halInternalIncrementCounterToken(uint8_t token);

/***************************************************************************//**
 * @brief This function sets up the manufacturing token system, which is
 * necessary for certain hardware configurations, specifically for
 * Silicon Labs Series 3 devices. It should be called during the
 * initialization phase of your application to ensure that the token
 * system is properly configured. This function is only relevant and
 * performs actions when compiled for the specified hardware series. It
 * is important to ensure that the hardware is supported before calling
 * this function, as it may assert if initialization fails.
 *
 * @return None
 ******************************************************************************/
void sl_token_mfg_init(void);

#endif // _TOKEN_API_H_
