/***************************************************************************//**
 * @brief Definitions for stack tokens.
 * See @ref token_stack for documentation.
 *
 * The file token-stack.h should not be included directly.
 * It is accessed by the other token files.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

/**
 * The tokens listed here are divided into three sections (the three main
 * types of tokens mentioned in token.h):
 * - manufacturing
 * - stack
 * - application
 *
 * For a full explanation of the tokens, see hal/micro/token.h.
 * See token-stack.h for source code.
 *
 * A set of tokens is predefined in the APPLICATION DATA section at the
 * end of token-stack.h because these tokens are required by the stack.
 * They are classified as application tokens since they are sized by the
 * application via its configuration header.
 *
 * The user application can include its own tokens in a header file similar
 * to this one. The macro ::APPLICATION_TOKEN_HEADER should be defined to equal
 * the name of the header file in which the application tokens are defined.
 * See the APPLICATION DATA section at the end of token-stack.h
 * for examples of token definitions.
 *
 * Since token-stack.h contains both the typedefs and the token defs,
 * two \#defines are used to select which one is needed when this file is
 * included.
 * \#define DEFINETYPES is used to select the type definitions and
 * \#define DEFINETOKENS is used to select the token definitions.
 * See token.h and token.c to see how these are used.
 */

// The basic TOKEN_DEF macro should not be used directly since the simplified
//  definitions are safer to use.  For completeness of information, the basic
//  macro has the following format:
//
//  TOKEN_DEF(name,creator,iscnt,isidx,type,arraysize,...)
//  name - The root name used for the token
//  creator - a "creator code" used to uniquely identify the token
//  iscnt - a boolean flag that is set to identify a counter token
//  isidx - a boolean flag that is set to identify an indexed token
//  type - the basic type or typdef of the token
//  arraysize - the number of elements making up an indexed token
//  ... - initializers used when reseting the tokens to default values
//
//
// The following convenience macros are used to simplify the definition
//  process for commonly specified parameters to the basic TOKEN_DEF macro
//  DEFINE_BASIC_TOKEN(name, type, ...)
//  DEFINE_INDEXED_TOKEN(name, type, arraysize, ...)
//  DEFINE_COUNTER_TOKEN(name, type, ...)
//  DEFINE_FIXED_BASIC_TOKEN(name, type, address, ...)
//  DEFINE_FIXED_INDEXED_TOKEN(name, type, arraysize, address, ...)
//  DEFINE_FIXED_COUNTER_TOKEN(name, type, address, ...)
//  DEFINE_MFG_TOKEN(name, type, address, ...)
//

/**
 * @name Convenience Macros
 * @brief The following convenience macros are used to simplify the definition
 * process for commonly specified parameters to the basic TOKEN_DEF macro.
 * See hal/micro/token.h for a more detailed explanation.
 *@{
 */
#define DEFINE_BASIC_TOKEN(name, type, ...) \
  TOKEN_DEF(name, CREATOR_##name, 0, 0, type, 1, __VA_ARGS__)

#define DEFINE_COUNTER_TOKEN(name, type, ...) \
  TOKEN_DEF(name, CREATOR_##name, 1, 0, type, 1, __VA_ARGS__)

#define DEFINE_INDEXED_TOKEN(name, type, arraysize, ...) \
  TOKEN_DEF(name, CREATOR_##name, 0, 1, type, (arraysize), __VA_ARGS__)

#define DEFINE_FIXED_BASIC_TOKEN(name, type, address, ...) \
  TOKEN_NEXT_ADDRESS(name, (address))                      \
  TOKEN_DEF(name, CREATOR_##name, 0, 0, type, 1, __VA_ARGS__)

#define DEFINE_FIXED_COUNTER_TOKEN(name, type, address, ...) \
  TOKEN_NEXT_ADDRESS(name, (address))                        \
  TOKEN_DEF(name, CREATOR_##name, 1, 0, type, 1, __VA_ARGS__)

#define DEFINE_FIXED_INDEXED_TOKEN(name, type, arraysize, address, ...) \
  TOKEN_NEXT_ADDRESS(name, (address))                                   \
  TOKEN_DEF(name, CREATOR_##name, 0, 1, type, (arraysize), __VA_ARGS__)
/** @} END Convenience Macros */

// The creator codes are here in one list instead of next to their token
// definitions so comparision of the codes is easier.  The only requirement
// on these creator definitions is that they all must be unique.  A favorite
// method for picking creator codes is to use two ASCII characters inorder
// to make the codes more memorable.

/**
 * @name Creator Codes
 * @brief The CREATOR is used as a distinct identifier tag for the
 * token.
 *
 * The CREATOR is necessary because the token name is defined
 * differently depending on the hardware platform. Therefore, the CREATOR
 * ensures that token definitions and data stay tagged and known. The only
 * requirement is that each creator definition must be unique.
 * See hal/micro/token.h for a more detailed explanation.
 *@{
 */

// STACK CREATORS
#define CREATOR_STACK_NODE_DATA                              0xEE64
#define CREATOR_STACK_NVDATA_VERSION                         0xFF01
#define CREATOR_STACK_SECURITY_KEY                           0x0004
#define CREATOR_STACK_NONCE_COUNTER                          0xE563
#define CREATOR_STACK_CHILD_TABLE                            0xFF0D
#define CREATOR_STACK_LAST_ASSIGNED_ID                       0x0007
#define CREATOR_STACK_BOOT_COUNTER                           0xE263
#define CREATOR_STACK_PARENT_LONG_ID                         0x0009

#define CREATOR_USER_TEST_TOK_01      0x0002
#define CREATOR_USER_TEST_TOK_02      0x0010
#define CREATOR_USER_TEST_TOK_COUNTER 0x0020
#define CREATOR_USER_TEST_TOK_MAX_KEY 0xfffc

/** @} END Creator Codes  */

/**
 * @name NVM3 Object Keys
 * @brief The NVM3 object key is used as a distinct identifier tag for a
 * token stored in NVM3.
 *
 * Every token must have a defined NVM3 object key and the object key
 * must be unique. The object key defined must be in the following
 * format:
 * NVM3KEY_tokenname
 *
 * where tokenname is the name of the token without NVM3KEY_ or TOKEN_
 * prefix.

 *@{
 */

// NVM3KEY domain base keys
#define NVM3KEY_DOMAIN_USER    0x00000U
#define NVM3KEY_DOMAIN_CONNECT 0x30000U
#define NVM3KEY_DOMAIN_COMMON  0x80000U

// STACK KEYS
#define NVM3KEY_STACK_NVDATA_VERSION           (NVM3KEY_DOMAIN_CONNECT | 0x0001)
#define NVM3KEY_STACK_NODE_DATA                (NVM3KEY_DOMAIN_CONNECT | 0x0003)
#define NVM3KEY_STACK_SECURITY_KEY             (NVM3KEY_DOMAIN_CONNECT | 0x0004)
#define NVM3KEY_STACK_NONCE_COUNTER            (NVM3KEY_DOMAIN_CONNECT | 0x0005)
// This key is used for an indexed token and the subsequent 0x7F keys are also
// reserved.
#define NVM3KEY_STACK_CHILD_TABLE              (NVM3KEY_DOMAIN_CONNECT | 0x0010)
#define NVM3KEY_STACK_LAST_ASSIGNED_ID         (NVM3KEY_DOMAIN_CONNECT | 0x0090)
#define NVM3KEY_STACK_BOOT_COUNTER             (NVM3KEY_DOMAIN_CONNECT | 0x0091)
#define NVM3KEY_STACK_PARENT_LONG_ID           (NVM3KEY_DOMAIN_CONNECT | 0x0092)

#define NVM3KEY_USER_TEST_TOK_01      (NVM3KEY_DOMAIN_USER | 0x0002)
// NVM3KEY_USER_TEST_TOK_02 is an indexed token
#define NVM3KEY_USER_TEST_TOK_02      (NVM3KEY_DOMAIN_USER | 0x0010)
#define NVM3KEY_USER_TEST_TOK_COUNTER (NVM3KEY_DOMAIN_USER | 0x0020)
#define NVM3KEY_USER_TEST_TOK_MAX_KEY (NVM3KEY_DOMAIN_USER | 0xfffc)

/** @} END NVM3 Object Keys  */

//////////////////////////////////////////////////////////////////////////////
// MANUFACTURING DATA
// Since the manufacturing data is platform specific, we pull in the proper
// file here.
#if defined(CORTEXM3)
// cortexm3 handles mfg tokens seperately via token_manufacturing_api.h
#elif defined(SL_ZIGBEE_TEST)
  #include "token_manufacturing.h"
#else
// For sl_token_manager_test purposes, ignore a platform not being defined.
//  #error no platform defined
#endif

//////////////////////////////////////////////////////////////////////////////
// STACK DATA
// *the addresses of these tokens must not change*

/**
 * @brief The current version number of the stack tokens.
 * MSB is the version. LSB is a complement.
 *
 * See hal/micro/token.h for a more complete explanation.
 */
#define CURRENT_STACK_TOKEN_VERSION 0x03FC //MSB is version, LSB is complement

#ifdef DEFINETYPES

typedef uint16_t tokTypeStackNvdataVersion;
typedef uint32_t tokTypeStackNonceCounter;

/***************************************************************************//**
 * @brief The `tokTypeStackKey` structure is designed to hold a network key,
 * which is a critical component in network security protocols. The
 * structure contains a single member, `networkKey`, which is an array of
 * 16 bytes, typically used to store a 128-bit key for encryption or
 * authentication purposes in network communications. This structure is
 * part of a larger system of tokens used for managing stack data in a
 * networked environment, ensuring secure and efficient data handling.
 *
 * @param networkKey An array of 16 bytes used to store a network key.
 ******************************************************************************/
typedef struct {
  uint8_t networkKey[16];
} tokTypeStackKey;

/***************************************************************************//**
 * @brief The `tokTypeStackNodeData` structure is used to store various
 * configuration and identification parameters for a Zigbee node within a
 * network. It includes fields for network identification such as `panId`
 * and `zigbeeNodeId`, radio configuration parameters like `radioTxPower`
 * and `radioFreqChannel`, and node-specific information such as
 * `nodeType` and `stackProfile`. The `extendedPanId` is an array that
 * holds the extended PAN ID, providing a unique identifier for the
 * network.
 *
 * @param panId A 16-bit identifier for the personal area network.
 * @param radioTxPower An 8-bit signed integer representing the radio
 * transmission power.
 * @param radioFreqChannel An 8-bit unsigned integer indicating the radio
 * frequency channel.
 * @param stackProfile An 8-bit unsigned integer representing the stack profile.
 * @param nodeType An 8-bit unsigned integer indicating the type of node.
 * @param zigbeeNodeId A 16-bit identifier for the Zigbee node.
 * @param extendedPanId An array of 8 bytes representing the extended PAN ID.
 ******************************************************************************/
typedef struct {
  uint16_t panId;
  int8_t radioTxPower;
  uint8_t radioFreqChannel;
  uint8_t stackProfile;
  uint8_t nodeType;
  uint16_t zigbeeNodeId;
  uint8_t extendedPanId[8];
} tokTypeStackNodeData;

/***************************************************************************//**
 * @brief The `tokTypeStackChildTableEntry` structure is used to represent an
 * entry in a child table within a network stack, containing both long
 * and short address identifiers along with a set of flags. This
 * structure is likely used in network protocols to manage and identify
 * child nodes in a network, providing essential addressing information
 * and status flags for each child node.
 *
 * @param longId A long address identifier of type sl_802154_long_addr_t.
 * @param shortId A short address identifier of type sl_802154_short_addr_t.
 * @param flags A uint8_t field used to store flags associated with the entry.
 ******************************************************************************/
typedef struct {
  sl_802154_long_addr_t longId;
  sl_802154_short_addr_t shortId;
  uint8_t flags;
} tokTypeStackChildTableEntry;

typedef uint16_t tokTypeStackLastAllocatedId;

typedef uint32_t tokTypeStackBootCounter;

typedef sl_802154_long_addr_t tokTypeParentLongId;

typedef uint32_t tokTypeTestTok01;
typedef uint32_t tokTypeTestTok02;
typedef uint32_t tokTypeTestCounter;
typedef uint32_t tokTypeTestTokMaxKey;

#endif //DEFINETYPES

#ifdef DEFINETOKENS

DEFINE_BASIC_TOKEN(STACK_NVDATA_VERSION,
                   tokTypeStackNvdataVersion,
                   CURRENT_STACK_TOKEN_VERSION)

DEFINE_BASIC_TOKEN(STACK_SECURITY_KEY,
                   tokTypeStackKey,
                   { 0, })

DEFINE_BASIC_TOKEN(STACK_NODE_DATA,
                   tokTypeStackNodeData,
                   { 0x8888, 31, 0x32, 0x33, 0x34, 0x3637, { 0, } })

DEFINE_BASIC_TOKEN(STACK_LAST_ASSIGNED_ID,
                   tokTypeStackLastAllocatedId,
                   0x0000)

DEFINE_INDEXED_TOKEN(STACK_CHILD_TABLE,
                     tokTypeStackChildTableEntry,
                     SL_ZIGBEE_CHILD_TABLE_TOKEN_SIZE,
                     { 0, })

DEFINE_COUNTER_TOKEN(STACK_NONCE_COUNTER,
                     tokTypeStackNonceCounter,
                     0x00000000)

DEFINE_COUNTER_TOKEN(STACK_BOOT_COUNTER,
                     tokTypeStackBootCounter,
                     0x0000)

DEFINE_BASIC_TOKEN(STACK_PARENT_LONG_ID,
                   tokTypeParentLongId,
                   { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF })

DEFINE_BASIC_TOKEN(USER_TEST_TOK_01,
                   tokTypeTestTok01,
                   0x0000)
DEFINE_INDEXED_TOKEN(USER_TEST_TOK_02,
                     tokTypeTestTok02,
                     5,
                     { 0x00000000, })
DEFINE_COUNTER_TOKEN(USER_TEST_TOK_COUNTER,
                     tokTypeTestCounter,
                     0)
#if defined(USE_NVM3)
DEFINE_BASIC_TOKEN(USER_TEST_TOK_MAX_KEY,
                   tokTypeTestTokMaxKey,
                   0x0000)
#endif // defined(USE_NVM3)
#endif //DEFINETOKENS

#ifdef MAC_DEBUG_TOKEN
//////////////////////////////////////////////////////////////////////////////
// MAC DEBUG DATA
#include "stack/mac/token-mac-debug.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// APPLICATION DATA

#ifdef APPLICATION_TOKEN_HEADER
  #include APPLICATION_TOKEN_HEADER
#endif
