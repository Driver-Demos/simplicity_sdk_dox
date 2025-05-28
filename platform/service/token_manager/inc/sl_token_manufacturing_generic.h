/***************************************************************************//**
 * @file
 * @brief Cortex-M3 Generic MFG token system
 *******************************************************************************
 * # License
 * <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_TOKEN_MFG_GENERIC_H_
#define SL_TOKEN_MFG_GENERIC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//--- User Data ---
typedef uint8_t tokTypeMfgEmberEui64[8];
typedef uint8_t tokTypeMfgCustomEui64[8];
typedef uint16_t tokTypeMfgCustomVersion;
typedef uint8_t tokTypeMfgString[16];
typedef uint8_t tokTypeMfgBoardName[16];
typedef uint16_t tokTypeMfgManufId;
typedef uint16_t tokTypeMfgPhyConfig;
typedef uint16_t tokTypeMfgAshConfig;
typedef uint16_t tokTypeMfgSynthFreqOffset;
typedef uint16_t tokTypeMfgCcaThreshold;
typedef uint8_t tokTypeMfgEzspStorage[8];
typedef uint16_t tokTypeMfgXoTune;
typedef uint8_t tokTypeMfgZwaveCountryFreq;
typedef uint8_t tokTypeMfgZwaveHardwareVersion;
typedef uint8_t tokTypeMfgZwavePseudoRandomNumber[16];
typedef uint8_t tokTypeMfgSerialNumber[16];
typedef uint8_t tokTypeMfgLfxoTune;
typedef uint16_t tokTypeMfgCTune;
typedef uint8_t tokTypeMfgKitSignature[4];
typedef uint8_t tokTypeMfgEui64[8];

//--- Lock Bits ---
// Main block Page Lock Words (PLWs)
typedef uint32_t tokTypeLockBitsPlw[4];
// Configuration Word Zero (CLW0)
typedef uint32_t tokTypeLockBitsClw0;
// Mass erase Lock Word (MLW)
typedef uint32_t tokTypeLockBitsMlw;
// User data page Lock Word (ULWs)
typedef uint32_t tokTypeLockBitsUlw;
// Debug Lock Word (DLW)
typedef uint32_t tokTypeLockBitsDlw;

//--- Lock Bits Data ---
// Smart Energy 1.0 (ECC 163k1 based crypto - 80-bit symmetric equivalent)
/***************************************************************************//**
 * @brief The `tokTypeMfgCbkeData` structure is designed to store cryptographic
 * keying material used in manufacturing processes, specifically for
 * Smart Energy 1.0 applications. It includes a certificate, a CA public
 * key, and a private key, all of which are essential for secure
 * communications. The `flags` field is used to indicate the
 * initialization status of the data, ensuring that the cryptographic
 * material is properly set up before use.
 *
 * @param certificate An array of 48 bytes representing a certificate.
 * @param caPublicKey An array of 22 bytes representing a CA public key.
 * @param privateKey An array of 21 bytes representing a private key.
 * @param flags A byte where the bottom bit indicates initialization status,
 * with 1 for uninitialized and 0 for initialized; other bits
 * should be 0 at initialization.
 ******************************************************************************/
typedef struct {
  uint8_t certificate[48];
  uint8_t caPublicKey[22];
  uint8_t privateKey[21];
  // The bottom flag bit is 1 for uninitialized, 0 for initialized.
  // The other flag bits should be set to 0 at initialization.
  uint8_t flags;
} tokTypeMfgCbkeData;
typedef uint16_t tokTypeMfgSecurityConfig;
/***************************************************************************//**
 * @brief The `tokTypeMfgInstallationCode` structure is designed to store a
 * manufacturing installation code, which includes a flag field for
 * initialization and size information, a value array for the code
 * itself, and a CRC field for ensuring data integrity. The flags field
 * is particularly important as it not only indicates whether the
 * structure is initialized but also encodes the size of the value
 * string, with special handling for certain bits due to historical bugs.
 * This structure is part of a larger system for managing manufacturing
 * tokens in embedded systems.
 *
 * @param flags A 16-bit field where the bottom bit indicates initialization
 * status, bits 1 and 2 indicate the size of the value string, and
 * other bits are reserved or set to 0.
 * @param value A 16-byte array that stores the installation code value.
 * @param crc A 16-bit field used to store the cyclic redundancy check for data
 * integrity verification.
 ******************************************************************************/
typedef struct {
  // The bottom flag bit is 1 for uninitialized, 0 for initialized.
  // Bits 1 and 2 give the size of the value string:
  // 0 = 6 bytes, 1 = 8 bytes, 2 = 12 bytes, 3 = 16 bytes.
  // The other flag bits should be set to 0 at initialization.
  // Special flags support.  Due to a bug in the way some customers
  // had programmed the flags field, we will also examine the upper
  // bits 9 and 10 for the size field.  Those bits are also reserved.
  uint16_t flags;
  uint8_t value[16];
  uint16_t crc;
} tokTypeMfgInstallationCode;
/***************************************************************************//**
 * @brief The `tokTypeMfgSecureBootloaderKey` is a structure designed to store a
 * 128-bit AES key, which is used in secure bootloader operations. This
 * key is crucial for cryptographic operations, ensuring the integrity
 * and authenticity of the bootloader process in Smart Energy 1.2
 * systems, which utilize ECC 283k1 based cryptography for a 128-bit
 * symmetric security equivalent.
 *
 * @param data An array of 16 uint8_t elements representing an AES-128 key.
 ******************************************************************************/
typedef struct {
  uint8_t data[16];  // AES-128 key
} tokTypeMfgSecureBootloaderKey;
// Smart Energy 1.2 (ECC 283k1 based crypto - 128-bit symmetric equivalent)
/***************************************************************************//**
 * @brief The `tokTypeMfgCbke283k1Data` structure is used to store cryptographic
 * data for Smart Energy 1.2 applications, specifically using ECC 283k1
 * based cryptography, which is equivalent to 128-bit symmetric
 * encryption. It includes fields for a certificate, a CA public key, and
 * a private key, along with a flags byte to indicate initialization
 * status. This structure is crucial for managing secure communications
 * and ensuring data integrity in manufacturing token systems.
 *
 * @param certificate An array of 74 bytes representing the certificate data.
 * @param caPublicKey An array of 37 bytes representing the CA public key.
 * @param privateKey An array of 36 bytes representing the private key.
 * @param flags A byte where the bottom bit indicates initialization status,
 * with other bits set to 0 at initialization.
 ******************************************************************************/
typedef struct {
  uint8_t certificate[74];
  uint8_t caPublicKey[37];
  uint8_t privateKey[36];
  // The bottom flag bit is 1 for uninitialized, 0 for initialized.
  // The other flag bits should be set to 0 at initialization.
  uint8_t flags;
} tokTypeMfgCbke283k1Data;
typedef uint8_t tokTypeMfgBootloadAesKey[16];
/***************************************************************************//**
 * @brief The `tokTypeMfgSignedBootloaderKeyX` structure is designed to store a
 * 32-byte ECDSA key point on the P256 curve, which is commonly used in
 * cryptographic operations for secure bootloader implementations. This
 * structure is part of a manufacturing token system, likely used to
 * securely store cryptographic keys necessary for verifying the
 * integrity and authenticity of bootloader code.
 *
 * @param data An array of 32 bytes representing an ECDSA key point on the P256
 * curve.
 ******************************************************************************/
typedef struct {
  uint8_t data[32];  // ECDSA key point on P256 curve
} tokTypeMfgSignedBootloaderKeyX;
/***************************************************************************//**
 * @brief The `tokTypeMfgSignedBootloaderKeyY` structure is designed to store a
 * cryptographic key used in secure bootloader operations. It contains a
 * single member, `data`, which is an array of 32 bytes. This array
 * represents an ECDSA (Elliptic Curve Digital Signature Algorithm) key
 * point on the P256 curve, which is a widely used elliptic curve for
 * cryptographic operations. The structure is part of a system that
 * manages manufacturing tokens, which are used for secure operations in
 * embedded systems.
 *
 * @param data An array of 32 bytes representing an ECDSA key point on the P256
 * curve.
 ******************************************************************************/
typedef struct {
  uint8_t data[32];  // ECDSA key point on P256 curve
} tokTypeMfgSignedBootloaderKeyY;
// Network join key with max length of 32 bytes
/***************************************************************************//**
 * @brief The `tokTypeMfgThreadJoinKey` structure is used to store a network
 * join key for a Thread network, with a maximum length of 32 bytes. It
 * includes a byte array to hold the key itself and a length field to
 * specify the actual length of the key in use. This structure is part of
 * a manufacturing token system, which is likely used for secure network
 * operations in embedded systems.
 *
 * @param joinKey An array of 32 bytes representing the network join key.
 * @param joinKeyLength A 16-bit unsigned integer indicating the length of the
 * join key.
 ******************************************************************************/
typedef struct {
  uint8_t joinKey[32];
  uint16_t joinKeyLength;
} tokTypeMfgThreadJoinKey;
typedef uint8_t tokTypeMfgNvm3CryptoKey[16];
typedef uint8_t tokTypeMfgZwavePrivateKey[32];
typedef uint8_t tokTypeMfgZwavePublicKey[32];
typedef uint8_t tokTypeMfgQRCode[90];
typedef uint8_t tokTypeMfgZwaveInitialized[1];

typedef uint16_t tokTypeMfgNvdataVersion;
typedef uint8_t tokTypeMfgEui64Hash[8];
typedef uint8_t tokTypeMfgRadioBandsSupported;
typedef int8_t tokTypeMfgRadioCrystalOffset;

/***************************************************************************//**
 * Available on series 1, series 2 and unix
 ******************************************************************************/
// Get manufacturing token string
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token string, which
 * is a predefined data structure that holds specific information about
 * the device. It should be called when the application needs to access
 * the manufacturing token string, typically after the system has been
 * initialized. The caller must ensure that the provided pointer is valid
 * and points to a buffer that can accommodate the expected data size.
 *
 * @param data A pointer to a `tokTypeMfgString` buffer where the manufacturing
 * token string will be stored. The buffer must be large enough to
 * hold 16 bytes. The caller retains ownership of the buffer and
 * must ensure it is not null.
 * @return The function does not return a value. Instead, it populates the
 * provided buffer with the manufacturing token string data.
 ******************************************************************************/
void sl_get_mfg_token_string(tokTypeMfgString *data);

// Set manufacturing token string data
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token string data,
 * which is typically called during the initialization or configuration
 * phase of a device. It is important to ensure that the `data` parameter
 * is properly allocated and contains a valid string of up to 16 bytes.
 * If the provided data is null or exceeds the expected length, the
 * behavior is undefined, and the function may not execute as intended.
 *
 * @param data A pointer to a `tokTypeMfgString` array, which must not be null.
 * The array should contain a string of up to 16 bytes. If the
 * string is shorter than 16 bytes, it should be null-terminated.
 * Providing a null pointer or a string longer than 16 bytes may
 * lead to undefined behavior.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_string_data(tokTypeMfgString *data);

// Get manufacturing token board name
/***************************************************************************//**
 * @brief This function is used to obtain the board name from the manufacturing
 * token system. It should be called when the board name is needed,
 * typically after the system has been initialized. The provided pointer
 * must point to a valid memory location that can hold the board name,
 * which is expected to be a string of up to 16 characters. If the
 * pointer is null or points to an invalid location, the behavior is
 * undefined.
 *
 * @param data A pointer to a `tokTypeMfgBoardName` array, which is expected to
 * hold the board name string. The caller must ensure that this
 * pointer is not null and points to a valid memory location capable
 * of storing at least 16 bytes. If the pointer is null, the
 * function's behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_board_name(tokTypeMfgBoardName *data);

// Set manufacturing token board name
/***************************************************************************//**
 * @brief This function is used to store the specified board name into the
 * manufacturing token system. It should be called when there is a need
 * to update or initialize the board name for the device. The input data
 * must be a valid pointer to a `tokTypeMfgBoardName` structure, which is
 * expected to hold a string of up to 16 bytes. It is important to ensure
 * that the data provided is properly formatted and does not exceed the
 * defined size, as this may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgBoardName` structure that contains the
 * board name to be set. The structure must be allocated and
 * initialized by the caller. It must not be null, and the string
 * length should not exceed 16 bytes, including the null terminator.
 * If the input is invalid, the behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_board_name_data(tokTypeMfgBoardName *data);

// Get manufacturing token manufacturing ID
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing ID from the device's
 * manufacturing token system. It should be called when the manufacturing
 * ID is needed, typically during the initialization or configuration
 * phase of the application. The caller must ensure that the provided
 * pointer is valid and points to a memory location that can hold a
 * `tokTypeMfgManufId` value. The function does not perform any checks on
 * the validity of the pointer, and passing a null pointer may lead to
 * undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgManufId` variable where the retrieved
 * manufacturing ID will be stored. The pointer must not be null and
 * should point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the manufacturing ID.
 ******************************************************************************/
void sl_get_mfg_token_manuf_id(tokTypeMfgManufId *data);

// Set manufacturing token manufacturing ID
/***************************************************************************//**
 * @brief This function is used to set the manufacturing ID data in the
 * manufacturing token system. It should be called when there is a need
 * to update the manufacturing ID, typically during the initialization or
 * configuration phase of the device. The input data must be valid and
 * properly formatted as defined by the `tokTypeMfgManufId` type. If the
 * provided data is invalid or null, the behavior is undefined.
 *
 * @param data Pointer to a `tokTypeMfgManufId` type that holds the
 * manufacturing ID data. This pointer must not be null and should
 * point to a valid memory location containing the manufacturing ID
 * value.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_manuf_id_data(tokTypeMfgManufId *data);

// Get manufacturing token ASH config
/***************************************************************************//**
 * @brief This function is used to obtain the ASH configuration manufacturing
 * token, which is essential for device configuration and operation. It
 * should be called when the application needs to access the ASH
 * configuration data, typically after the system has been initialized.
 * The caller must ensure that the provided pointer is valid and points
 * to a memory location that can accommodate the expected data.
 *
 * @param data A pointer to a `tokTypeMfgAshConfig` structure where the
 * retrieved ASH configuration data will be stored. The pointer must
 * not be null, and the caller retains ownership of the memory. If
 * the pointer is invalid or null, the behavior is undefined.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the ASH configuration token.
 ******************************************************************************/
void sl_get_mfg_token_ash_config(tokTypeMfgAshConfig *data);

// Set manufacturing token ASH config
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token ASH configuration
 * data, which is essential for configuring device-specific parameters.
 * It should be called when the application needs to update the ASH
 * configuration, typically during the initialization phase or when
 * reconfiguring the device. The input data must be a valid pointer to a
 * `tokTypeMfgAshConfig` structure, which contains the necessary
 * configuration values. Ensure that the data provided is properly
 * initialized before calling this function to avoid undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgAshConfig` structure that holds the ASH
 * configuration data. This pointer must not be null and should
 * point to a valid memory location containing the configuration
 * values. If the pointer is null or invalid, the behavior of the
 * function is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_ash_config_data(tokTypeMfgAshConfig *data);

// Get manufacturing token CCA threshold
/***************************************************************************//**
 * @brief This function is used to obtain the current CCA (Clear Channel
 * Assessment) threshold value from the manufacturing token system. It
 * should be called when the application needs to read the CCA threshold
 * for configuration or monitoring purposes. The caller must ensure that
 * the `data` pointer is valid and points to a memory location that can
 * hold the `tokTypeMfgCcaThreshold` value. If the provided pointer is
 * null, the behavior is undefined.
 *
 * @param data A pointer to a `tokTypeMfgCcaThreshold` variable where the
 * retrieved threshold value will be stored. The pointer must not be
 * null and should point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the current CCA threshold value.
 ******************************************************************************/
void sl_get_mfg_token_cca_threshold(tokTypeMfgCcaThreshold *data);

// Set manufacturing token CCA threshold
/***************************************************************************//**
 * @brief This function is used to set the CCA (Clear Channel Assessment)
 * threshold data for the manufacturing token. It should be called when
 * there is a need to update the CCA threshold value, typically during
 * the initialization or configuration phase of the device. The input
 * data must be a valid pointer to a `tokTypeMfgCcaThreshold` structure,
 * which contains the threshold value. It is important to ensure that the
 * data pointer is not null before calling this function, as passing a
 * null pointer may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgCcaThreshold` structure that holds the
 * CCA threshold value. The pointer must not be null, and the caller
 * retains ownership of the data. If the pointer is null, the
 * function will not perform any operation.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_cca_threshold_data(tokTypeMfgCcaThreshold *data);

// Get manufacturing token EZSP storage
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token associated
 * with EZSP storage. It should be called when the application needs to
 * access or manipulate the EZSP storage data. The caller must ensure
 * that the provided pointer is valid and points to a buffer that can
 * hold the expected data. The function does not perform any internal
 * validation on the pointer, so passing a null pointer or an invalid
 * address may lead to undefined behavior.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgEzspStorage` where the
 * retrieved manufacturing token will be stored. The buffer must be
 * allocated by the caller and should be large enough to hold the
 * data. It must not be null.
 * @return The function does not return a value. Instead, it populates the
 * buffer pointed to by `data` with the manufacturing token for EZSP
 * storage.
 ******************************************************************************/
void sl_get_mfg_token_ezsp_storage(tokTypeMfgEzspStorage *data);

// Set manufacturing token EZSP storage
/***************************************************************************//**
 * @brief This function is used to store EZSP (Ember Zigbee Stack Protocol)
 * related manufacturing token data. It should be called when there is a
 * need to update the EZSP storage information, typically during the
 * initialization or configuration phase of the application. Ensure that
 * the data being passed is valid and properly initialized before calling
 * this function, as it directly modifies the underlying storage. Invalid
 * or uninitialized data may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgEzspStorage` structure that holds the
 * EZSP storage data. This pointer must not be null and should point
 * to a valid memory location containing the data to be stored.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_ezsp_storage_data(tokTypeMfgEzspStorage *data);

// Get manufacturing token bootloader AES key
/***************************************************************************//**
 * @brief This function is used to obtain the bootloader AES key, which is
 * essential for secure bootloading operations. It should be called when
 * the application requires access to this key, typically during the
 * initialization phase of a secure boot process. The caller must ensure
 * that the provided pointer is valid and points to a buffer that can
 * hold the key data. The function does not perform any checks on the
 * validity of the pointer, and passing a null pointer may lead to
 * undefined behavior.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgBootloadAesKey` where
 * the retrieved AES key will be stored. The buffer must be large
 * enough to hold 16 bytes, as this is the size of the AES key. The
 * caller retains ownership of the buffer and must ensure it is not
 * null.
 * @return The function does not return a value. Instead, it populates the
 * provided buffer with the bootloader AES key.
 ******************************************************************************/
void sl_get_mfg_token_bootload_aes_key(tokTypeMfgBootloadAesKey *data);

// Set manufacturing token bootloader AES key
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token that stores the
 * AES key used for bootloading. It should be called when there is a need
 * to update or initialize the bootloader AES key data. The input data
 * must be a valid pointer to a `tokTypeMfgBootloadAesKey` structure,
 * which contains the AES key. It is important to ensure that the
 * provided pointer is not null before calling this function, as passing
 * a null pointer may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgBootloadAesKey` structure that holds
 * the AES key data. The structure must be properly initialized and
 * must not be null. If the pointer is null, the function will not
 * perform any operation.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_bootload_aes_key_data(tokTypeMfgBootloadAesKey *data);

// Get manufacturing token thread join key
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token that contains
 * the thread join key, which is essential for network joining processes.
 * It should be called when the application needs to access the thread
 * join key for configuration or initialization purposes. The caller must
 * ensure that the provided pointer is valid and points to a
 * `tokTypeMfgThreadJoinKey` structure that has been allocated and is
 * ready to receive the data.
 *
 * @param data A pointer to a `tokTypeMfgThreadJoinKey` structure where the
 * retrieved thread join key will be stored. This pointer must not
 * be null and should point to a valid memory location that can hold
 * the data.
 * @return The function does not return a value. Instead, it populates the
 * provided `data` structure with the thread join key information.
 ******************************************************************************/
void sl_get_mfg_token_thread_join_key(tokTypeMfgThreadJoinKey *data);

// Set manufacturing token thread join key
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token for the thread
 * join key, which is essential for network joining processes. It should
 * be called when the application needs to configure or update the thread
 * join key data. The input data must be properly initialized before
 * calling this function, and it is important to ensure that the data
 * structure is valid and contains the correct information. Invalid or
 * uninitialized data may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgThreadJoinKey` structure that holds the
 * thread join key data. This structure must be properly initialized
 * and must not be null. The function does not take ownership of the
 * data, and the caller is responsible for ensuring that the data is
 * valid before passing it to the function.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_thread_join_key_data(tokTypeMfgThreadJoinKey *data);

// Get manufacturing token EUI64
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token EUI64, which
 * is a unique identifier for the device. It should be called when the
 * application needs to access the EUI64 value, typically after the
 * system has been initialized. The caller must ensure that the provided
 * pointer is valid and points to a buffer that can hold the EUI64 data.
 * The function does not return a value, but it populates the provided
 * buffer with the EUI64 data.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgEui64` where the EUI64
 * value will be stored. The buffer must be large enough to hold 8
 * bytes. The caller retains ownership of the buffer and must ensure
 * it is not null.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_eui64(tokTypeMfgEui64 *data);

// Set manufacturing token EUI64
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token EUI64 data, which
 * is typically required during the initialization or configuration of a
 * device. It should be called when the device is ready to store its
 * unique EUI64 identifier, ensuring that the provided data is valid and
 * properly formatted. The function does not return a value, but it will
 * affect the internal state of the manufacturing token system.
 *
 * @param data A pointer to an array of 8 bytes representing the EUI64 data.
 * This pointer must not be null, and the data should be properly
 * initialized before calling the function. If the data is invalid
 * or improperly formatted, the behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_eui64_data(tokTypeMfgEui64 *data);

/***************************************************************************//**
 * Only available on series 1, series 2
 ******************************************************************************/
#ifndef SL_ZIGBEE_TEST
// Get manufacturing token Ember EUI64
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token that contains
 * the Ember EUI64 identifier. It should be called when the application
 * needs to access the unique Ember EUI64 value, typically after the
 * system has been initialized. The provided pointer must point to a
 * valid memory location that can hold the EUI64 data, which is an array
 * of 8 bytes. It is important to ensure that the pointer is not null
 * before calling this function.
 *
 * @param data A pointer to a `tokTypeMfgEmberEui64` array, which is expected to
 * hold 8 bytes of data. The caller must ensure that this pointer is
 * valid and not null before calling the function. The function does
 * not take ownership of the memory pointed to by `data`.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the Ember EUI64 value.
 ******************************************************************************/
void sl_get_mfg_token_ember_eui64(tokTypeMfgEmberEui64 *data);

// Set manufacturing token Ember EUI64
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token for Ember EUI64,
 * which is a unique identifier for the device. It should be called when
 * the application needs to update or initialize the Ember EUI64 value.
 * The input data must be a valid pointer to an array of 8 bytes,
 * representing the EUI64 value. It is important to ensure that the data
 * provided is correctly formatted, as invalid data may lead to undefined
 * behavior.
 *
 * @param data A pointer to an array of 8 bytes representing the Ember EUI64.
 * Must not be null and should contain valid data. The function does
 * not perform validation on the contents of the array.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_ember_eui64_data(tokTypeMfgEmberEui64 *data);

// Get manufacturing token Custom version
/***************************************************************************//**
 * @brief This function is used to obtain the custom version of the
 * manufacturing token, which may be necessary for device identification
 * or configuration purposes. It should be called when the application
 * needs to access the custom version information, typically after the
 * system has been initialized. The provided pointer must point to a
 * valid memory location that can hold the custom version data.
 *
 * @param data A pointer to a `tokTypeMfgCustomVersion` variable where the
 * retrieved custom version will be stored. The caller must ensure
 * that this pointer is not null and points to a valid memory
 * location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the custom version information.
 ******************************************************************************/
void sl_get_mfg_token_custom_version(tokTypeMfgCustomVersion *data);

// Set manufacturing token Custom version
/***************************************************************************//**
 * @brief This function is used to set the custom version data for the
 * manufacturing token. It should be called when there is a need to
 * update the custom version information stored in the manufacturing
 * token. Ensure that the `data` parameter is properly initialized before
 * calling this function, as it will directly modify the token data.
 * Invalid or uninitialized data may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgCustomVersion` type that holds the
 * custom version data. This pointer must not be null and should
 * point to a valid memory location containing the version
 * information.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_custom_version_data(tokTypeMfgCustomVersion *data);

// Get manufacturing token custom EUI64
/***************************************************************************//**
 * @brief This function is used to obtain the custom EUI64 manufacturing token,
 * which is a unique identifier for the device. It should be called when
 * the application needs to access this specific token, typically after
 * the system has been initialized. The provided pointer must point to a
 * valid memory location that can hold the EUI64 data, which is an array
 * of 8 bytes. If the pointer is null, the behavior is undefined.
 *
 * @param data A pointer to a `tokTypeMfgCustomEui64` array, which must not be
 * null. The function will write the custom EUI64 value into the
 * memory location pointed to by this parameter.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_custom_eui64(tokTypeMfgCustomEui64 *data);

// Set manufacturing token custom EUI64
/***************************************************************************//**
 * @brief This function is used to set the custom EUI64 data for the
 * manufacturing token. It should be called when there is a need to
 * update the custom EUI64 value, typically during the initialization or
 * configuration phase of the device. The input data must be a valid
 * pointer to an array of 8 bytes, representing the custom EUI64 value.
 * It is important to ensure that the provided pointer is not null before
 * calling this function, as passing a null pointer may lead to undefined
 * behavior.
 *
 * @param data A pointer to an array of 8 bytes representing the custom EUI64
 * value. Must not be null. The function expects a valid memory
 * address that contains the EUI64 data. If the pointer is null, the
 * behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_custom_eui64_data(tokTypeMfgCustomEui64 *data);

// Get manufacturing token PHY config
/***************************************************************************//**
 * @brief This function is used to obtain the PHY configuration token from the
 * manufacturing token storage. It should be called when the PHY
 * configuration is needed for device initialization or configuration.
 * The caller must ensure that the provided pointer is valid and points
 * to a memory location that can hold the `tokTypeMfgPhyConfig` data. If
 * the pointer is null or invalid, the behavior is undefined.
 *
 * @param data A pointer to a `tokTypeMfgPhyConfig` variable where the retrieved
 * PHY configuration will be stored. The pointer must not be null
 * and should point to a valid memory location capable of holding
 * the data.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by the `data` parameter with the PHY configuration
 * token.
 ******************************************************************************/
void sl_get_mfg_token_phy_config(tokTypeMfgPhyConfig *data);

// Set manufacturing token PHY config
/***************************************************************************//**
 * @brief This function is used to store PHY configuration data in the
 * manufacturing token system. It should be called when there is a need
 * to update the PHY configuration settings, typically during the
 * initialization or configuration phase of the device. The input data
 * must be valid and properly formatted as defined by the
 * `tokTypeMfgPhyConfig` type. If the provided data pointer is null, the
 * function will not perform any operation.
 *
 * @param data Pointer to a `tokTypeMfgPhyConfig` structure containing the PHY
 * configuration data to be set. Must not be null; otherwise, the
 * function will not execute any operation.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_phy_config_data(tokTypeMfgPhyConfig *data);

// Get manufacturing token synth frequency offset
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token that
 * represents the synth frequency offset. It should be called when the
 * application needs to access this specific manufacturing data. The
 * caller must ensure that the provided pointer is valid and points to a
 * memory location that can hold the `tokTypeMfgSynthFreqOffset` data.
 * The function does not perform any internal validation on the pointer,
 * so passing a null pointer or an invalid address may lead to undefined
 * behavior.
 *
 * @param data A pointer to a `tokTypeMfgSynthFreqOffset` variable where the
 * retrieved frequency offset will be stored. The pointer must not
 * be null and should point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the synth frequency offset value.
 ******************************************************************************/
void sl_get_mfg_token_synth_freq_offset(tokTypeMfgSynthFreqOffset *data);

// Set manufacturing token synth frequency offset
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token that stores the
 * synth frequency offset data. It should be called when there is a need
 * to update the synth frequency offset in the manufacturing token
 * system. The input data must be a valid pointer to a
 * `tokTypeMfgSynthFreqOffset` type, which is a 16-bit unsigned integer.
 * Ensure that the data pointer is not null before calling this function,
 * as passing a null pointer may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgSynthFreqOffset` type that holds the
 * synth frequency offset value. The pointer must not be null, and
 * it should point to a valid memory location containing a 16-bit
 * unsigned integer. If the pointer is null, the function will not
 * perform any operation.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_synth_freq_offset_data(tokTypeMfgSynthFreqOffset *data);

// Get manufacturing token XO tune
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token that
 * represents the XO tune value. It should be called when the application
 * needs to access this specific manufacturing data, typically after the
 * system has been initialized. The provided pointer must point to a
 * valid memory location that can hold the `tokTypeMfgXoTune` data. It is
 * important to ensure that the memory is allocated and accessible before
 * calling this function.
 *
 * @param data A pointer to a `tokTypeMfgXoTune` variable where the retrieved XO
 * tune value will be stored. The pointer must not be null and
 * should point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the XO tune value.
 ******************************************************************************/
void sl_get_mfg_token_xo_tune(tokTypeMfgXoTune *data);

// Set manufacturing token XO tune
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token for XO tune data,
 * which is typically called during the initialization or configuration
 * phase of a device. It is important to ensure that the `data` parameter
 * is properly initialized before calling this function, as it will be
 * used to store the XO tune information. The function does not return a
 * value, and it is expected that the caller manages the memory for the
 * `data` parameter.
 *
 * @param data Pointer to a `tokTypeMfgXoTune` structure that holds the XO tune
 * data. This pointer must not be null and should point to a valid
 * memory location containing the XO tune information.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_xo_tune_data(tokTypeMfgXoTune *data);

// Get manufacturing token Zwave country frequency
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave country frequency value
 * stored in the manufacturing token. It should be called when the
 * application needs to access the specific frequency settings for Z-Wave
 * communication, typically after the system has been initialized. The
 * provided pointer must point to a valid memory location that can hold
 * the frequency data.
 *
 * @param data A pointer to a `tokTypeMfgZwaveCountryFreq` type, which is an
 * 8-bit unsigned integer. The caller must ensure that this pointer
 * is not null and points to a valid memory location capable of
 * storing the frequency value.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the Z-Wave country frequency.
 ******************************************************************************/
void sl_get_mfg_token_zwave_country_freq(tokTypeMfgZwaveCountryFreq *data);

// Set manufacturing token Zwave country frequency
/***************************************************************************//**
 * @brief This function is used to store Z-Wave country frequency data in the
 * manufacturing token system. It should be called when there is a need
 * to update or initialize the country frequency settings for Z-Wave
 * communication. Ensure that the `data` parameter is properly
 * initialized before calling this function, as it will directly modify
 * the underlying token storage. Invalid or uninitialized data may lead
 * to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgZwaveCountryFreq` structure that holds
 * the country frequency data. This pointer must not be null and
 * should point to a valid memory location containing the frequency
 * information.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zwave_country_freq_data(tokTypeMfgZwaveCountryFreq *data);

// Get manufacturing token Zwave HW version
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave hardware version from the
 * manufacturing token system. It should be called when the hardware
 * version information is needed, typically after the system has been
 * initialized. The caller must ensure that the provided pointer is valid
 * and points to a buffer that can hold the hardware version data. The
 * function does not return a value, but it populates the provided buffer
 * with the hardware version information.
 *
 * @param data A pointer to a `tokTypeMfgZwaveHardwareVersion` type, which is
 * expected to be a valid memory location where the hardware version
 * will be stored. The caller is responsible for ensuring that this
 * pointer is not null and that it points to sufficient memory to
 * hold the hardware version data.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_zwave_hw_version(tokTypeMfgZwaveHardwareVersion *data);

// Set manufacturing token Zwave HW version
/***************************************************************************//**
 * @brief This function is used to store the Z-Wave hardware version data in the
 * manufacturing token system. It should be called when the hardware
 * version needs to be updated or initialized. Ensure that the `data`
 * parameter is properly allocated and contains valid information before
 * calling this function. The function does not return a value, but it
 * will affect the state of the manufacturing token system.
 *
 * @param data Pointer to a `tokTypeMfgZwaveHardwareVersion` structure that
 * holds the hardware version data. This pointer must not be null
 * and should point to a valid memory location containing the
 * hardware version information.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zwave_hw_version_data(tokTypeMfgZwaveHardwareVersion *data);

// Get manufacturing token Zwave Pseudo random number
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave pseudo random number from
 * the manufacturing token system. It should be called when the
 * application requires access to this specific token, typically after
 * the system has been initialized. The provided pointer must point to a
 * valid memory location that can hold the token data, which is an array
 * of 16 bytes. If the pointer is null or invalid, the behavior is
 * undefined.
 *
 * @param data A pointer to a `tokTypeMfgZwavePseudoRandomNumber` array, which
 * must not be null. The caller is responsible for ensuring that the
 * memory allocated for this array is sufficient to hold 16 bytes of
 * data.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the Z-Wave pseudo random number.
 ******************************************************************************/
void sl_get_mfg_token_zwave_pseudo_random_number(tokTypeMfgZwavePseudoRandomNumber *data);

// Set manufacturing token Zwave Pseudo random number
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token for the Z-Wave
 * pseudo random number. It should be called when there is a need to
 * update or initialize the pseudo random number used in Z-Wave
 * operations. The input data must be valid and properly formatted, as it
 * will be directly written to the manufacturing token storage. Ensure
 * that this function is called in an appropriate context where the
 * manufacturing token system is initialized and ready to accept new
 * data.
 *
 * @param data A pointer to a `tokTypeMfgZwavePseudoRandomNumber` structure that
 * holds the pseudo random number data. This structure must not be
 * null and should contain valid data formatted as an array of 16
 * bytes. If the pointer is null or the data is invalid, the
 * behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zwave_pseudo_random_number_data(tokTypeMfgZwavePseudoRandomNumber *data);

// Get manufacturing token serial number
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token serial number,
 * which is typically required for device identification and tracking. It
 * should be called when the serial number is needed, and the caller must
 * ensure that the provided pointer is valid and points to a buffer
 * capable of holding the serial number data. The function does not
 * perform any checks on the validity of the buffer size, so it is the
 * caller's responsibility to ensure that the buffer is appropriately
 * sized to avoid buffer overflows.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgSerialNumber` where the
 * serial number will be stored. The buffer must be large enough to
 * hold 16 bytes. Caller retains ownership of the buffer and must
 * ensure it is not null.
 * @return The function does not return a value. Instead, it populates the
 * provided buffer with the serial number data.
 ******************************************************************************/
void sl_get_mfg_token_serial_number(tokTypeMfgSerialNumber *data);

// Set manufacturing token serial number
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token serial number
 * data, which is typically required during the initialization or
 * configuration of a device. It should be called when the device is
 * ready to store its serial number, ensuring that the provided data is
 * valid and properly formatted. The function does not return a value,
 * but it will affect the internal state of the manufacturing token
 * system.
 *
 * @param data A pointer to a `tokTypeMfgSerialNumber` array, which must not be
 * null. The array should contain valid serial number data,
 * formatted as a byte array of length 16. If the pointer is null,
 * the behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_serial_number_data(tokTypeMfgSerialNumber *data);

// Get manufacturing token LFXO tune
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token that contains
 * the LFXO tuning value. It should be called when the application needs
 * to access this specific configuration data, typically after the system
 * has been initialized. The caller must ensure that the provided pointer
 * is valid and points to a buffer that can hold the expected data. If
 * the pointer is null or invalid, the behavior is undefined.
 *
 * @param data A pointer to a `tokTypeMfgLfxoTune` type where the retrieved LFXO
 * tuning value will be stored. The caller must ensure that this
 * pointer is not null and points to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the LFXO tuning value.
 ******************************************************************************/
void sl_get_mfg_token_lfxo_tune(tokTypeMfgLfxoTune *data);

// Set manufacturing token LFXO tune
/***************************************************************************//**
 * @brief This function is used to store LFXO tune data in the manufacturing
 * token system. It should be called when there is a need to update the
 * LFXO tune settings, typically during device configuration or
 * initialization. The input data must be a valid pointer to a
 * `tokTypeMfgLfxoTune` structure, which contains the necessary tuning
 * information. Ensure that the data provided is properly initialized
 * before calling this function.
 *
 * @param data A pointer to a `tokTypeMfgLfxoTune` structure that holds the LFXO
 * tune data. This pointer must not be null and should point to a
 * valid memory location containing the tune data. If the pointer is
 * null or invalid, the behavior of the function is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_lfxo_tune_data(tokTypeMfgLfxoTune *data);

// Get manufacturing token CTune
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token CTune value,
 * which is typically required for configuration or calibration purposes.
 * It should be called when the application needs to access the CTune
 * data, and the provided pointer must point to a valid memory location
 * that can hold the token data. Ensure that the memory allocated for the
 * `data` parameter is sufficient to store the `tokTypeMfgCTune` type.
 *
 * @param data A pointer to a `tokTypeMfgCTune` variable where the retrieved
 * CTune value will be stored. The pointer must not be null and
 * should point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the retrieved CTune value.
 ******************************************************************************/
void sl_get_mfg_token_ctune(tokTypeMfgCTune *data);

// Set manufacturing token CTune
/***************************************************************************//**
 * @brief This function is used to store the specified CTune data into the
 * manufacturing token system. It should be called when there is a need
 * to update the CTune value, typically during the initialization or
 * configuration phase of the device. Ensure that the `data` parameter
 * points to a valid `tokTypeMfgCTune` structure containing the
 * appropriate CTune value before calling this function.
 *
 * @param data A pointer to a `tokTypeMfgCTune` structure that holds the CTune
 * data to be set. This pointer must not be null and should point to
 * a valid memory location containing the CTune value.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_ctune_data(tokTypeMfgCTune *data);

// Get manufacturing token kit signature
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing kit signature, which
 * is a specific token that may be required for device identification or
 * validation purposes. It should be called when the application needs to
 * access this token, and the provided pointer must point to a valid
 * memory location that can hold the token data. Ensure that the memory
 * allocated for the token is sufficient to store the expected data size.
 *
 * @param data A pointer to a `tokTypeMfgKitSignature` type, which is an array
 * of 4 bytes. The caller must ensure that this pointer is not null
 * and points to a valid memory location that can accommodate the
 * token data.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the manufacturing kit signature.
 ******************************************************************************/
void sl_get_mfg_token_kit_signature(tokTypeMfgKitSignature *data);

// Set manufacturing token kit signature
/***************************************************************************//**
 * @brief This function is used to store the provided kit signature data into
 * the manufacturing token system. It should be called when there is a
 * need to update or initialize the kit signature, typically during the
 * device setup or configuration phase. Ensure that the `data` parameter
 * is properly allocated and contains valid kit signature information
 * before calling this function.
 *
 * @param data A pointer to a `tokTypeMfgKitSignature` structure that holds the
 * kit signature data. This pointer must not be null and should
 * point to a valid memory location containing the kit signature.
 * The function does not take ownership of the memory pointed to by
 * `data`.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_kit_signature_data(tokTypeMfgKitSignature *data);

// Get manufacturing token lockbits PLW
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token lock bits PLW,
 * which are essential for configuring the security and access control of
 * the device. It should be called when the application needs to read the
 * lock bits for verification or configuration purposes. Ensure that the
 * `data` parameter is properly allocated before calling this function,
 * as it will be populated with the retrieved lock bits. The function
 * does not perform any validation on the `data` pointer, so it must not
 * be null.
 *
 * @param data A pointer to a `tokTypeLockBitsPlw` array where the retrieved
 * lock bits will be stored. The caller must ensure that this
 * pointer is valid and points to allocated memory sufficient to
 * hold the lock bits.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the lock bits.
 ******************************************************************************/
void sl_get_mfg_token_lockbits_plw(tokTypeLockBitsPlw *data);

// Set manufacturing token lockbits PLW
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token lockbits PLW
 * data, which is essential for configuring the security and access
 * control of the device. It should be called when the application needs
 * to update the lockbits configuration, typically during the
 * initialization or configuration phase. Ensure that the `data`
 * parameter is properly initialized before calling this function, as it
 * will directly modify the lockbits settings. Invalid or uninitialized
 * data may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeLockBitsPlw` structure that holds the
 * lockbits PLW data. This parameter must not be null and should
 * point to a valid memory location containing the lockbits data.
 * The function expects the data to be properly initialized;
 * otherwise, it may result in incorrect configuration.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_lockbits_plw_data(tokTypeLockBitsPlw *data);

// Get manufacturing token lockbits CLOW0
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token lock bits
 * specifically for Configuration Word Zero (CLW0). It should be called
 * when the application needs to access the lock bits data, typically
 * after the system has been initialized. The provided pointer must point
 * to a valid memory location that can hold the lock bits data. It is
 * important to ensure that the pointer is not null before calling this
 * function, as passing a null pointer may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeLockBitsClw0` type where the retrieved
 * lock bits will be stored. The pointer must not be null and must
 * point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the lock bits information.
 ******************************************************************************/
void sl_get_mfg_token_lockbits_clw0(tokTypeLockBitsClw0 *data);

// Set manufacturing token lockbits CLOW0
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token lockbits for
 * Configuration Word Zero (CLW0). It should be called when there is a
 * need to update the lockbits data, typically during the manufacturing
 * process. Ensure that the `data` parameter is properly initialized
 * before calling this function, as it will directly modify the lockbits.
 * Invalid or uninitialized data may lead to undefined behavior.
 *
 * @param data Pointer to a `tokTypeLockBitsClw0` structure that holds the
 * lockbits data. Must not be null and should be properly
 * initialized before use.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_lockbits_clw0_data(tokTypeLockBitsClw0 *data);

// Get manufacturing token lockbits MLW
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token lock bits
 * specifically for the mass erase lock word (MLW). It should be called
 * when the application needs to access the lock bits data, typically
 * after the system has been initialized. The caller must ensure that the
 * provided pointer is valid and points to a memory location that can
 * hold the lock bits data.
 *
 * @param data A pointer to a `tokTypeLockBitsMlw` structure where the retrieved
 * lock bits will be stored. The pointer must not be null and should
 * point to a valid memory location.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_lockbits_mlw(tokTypeLockBitsMlw *data);

// Set manufacturing token lockbits MLW
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token lock bits for the
 * mass erase lock word (MLW). It should be called when there is a need
 * to update the MLW data, typically during the manufacturing process.
 * Ensure that the `data` parameter is properly initialized before
 * calling this function, as it will directly modify the lock bits.
 * Invalid or uninitialized data may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeLockBitsMlw` structure that contains the
 * lock bits data to be set. This pointer must not be null and
 * should point to a valid memory location containing the
 * appropriate data.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_lockbits_mlw_data(tokTypeLockBitsMlw *data);

// Get manufacturing token lockbits ULW
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token lock bits
 * specifically for the User Lock Word (ULW). It should be called when
 * the application needs to access the lock bits data, typically after
 * the system has been initialized. The caller must ensure that the
 * provided pointer is valid and points to a memory location that can
 * hold the lock bits data.
 *
 * @param data A pointer to a `tokTypeLockBitsUlw` structure where the retrieved
 * lock bits will be stored. The pointer must not be null and should
 * point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the lock bits information.
 ******************************************************************************/
void sl_get_mfg_token_lockbits_ulw(tokTypeLockBitsUlw *data);

// Set manufacturing token lockbits ULW
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token lockbits for the
 * user data page (ULW). It should be called when there is a need to
 * update the lockbits configuration, typically during the initialization
 * or configuration phase of the device. Ensure that the `data` parameter
 * is properly initialized before calling this function, as it will
 * directly modify the lockbits. Invalid or uninitialized data may lead
 * to undefined behavior.
 *
 * @param data A pointer to a `tokTypeLockBitsUlw` structure that contains the
 * lockbits data to be set. This pointer must not be null and should
 * point to a valid memory location containing the lockbits
 * information.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_lockbits_ulw_data(tokTypeLockBitsUlw *data);

// Get manufacturing token lockbits DLW
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token lock bits
 * specifically for debug purposes. It should be called when the
 * application needs to access the lock bits data, which is essential for
 * ensuring the integrity and security of the device. The caller must
 * ensure that the `data` pointer is valid and points to a properly
 * allocated `tokTypeLockBitsDlw` structure before invoking this
 * function. If the provided pointer is null, the behavior is undefined.
 *
 * @param data A pointer to a `tokTypeLockBitsDlw` structure where the retrieved
 * lock bits will be stored. This pointer must not be null and
 * should point to a valid memory location that can hold the lock
 * bits data.
 * @return The function does not return a value. Instead, it populates the
 * provided `data` structure with the lock bits information.
 ******************************************************************************/
void sl_get_mfg_token_lockbits_dlw(tokTypeLockBitsDlw *data);

// Set manufacturing token lockbits DLW
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token lock bits for the
 * Debug Lock Word (DLW). It should be called when there is a need to
 * update the lock bits configuration, typically during the
 * initialization or configuration phase of the device. Ensure that the
 * `data` parameter is properly initialized before calling this function,
 * as it will directly modify the lock bits based on the provided data.
 *
 * @param data A pointer to a `tokTypeLockBitsDlw` structure that contains the
 * lock bits data to be set. This pointer must not be null and
 * should point to a valid memory location containing the
 * appropriate data.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_lockbits_dlw_data(tokTypeLockBitsDlw *data);

// Get manufacturing token CBKE Data
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token data related
 * to CBKE (Certificate-Based Key Establishment). It should be called
 * when the application needs to access the CBKE data for cryptographic
 * operations or device identity verification. The provided pointer must
 * point to a valid `tokTypeMfgCbkeData` structure, which will be
 * populated with the retrieved data. Ensure that the pointer is not null
 * before calling this function.
 *
 * @param data A pointer to a `tokTypeMfgCbkeData` structure where the retrieved
 * data will be stored. The pointer must not be null, and the caller
 * retains ownership of the memory allocated for this structure.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_cbke_data(tokTypeMfgCbkeData *data);

// Set manufacturing token CBKE Data
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token data for CBKE
 * (Certificate-Based Key Establishment). It should be called when there
 * is a need to update or initialize the CBKE data structure with new
 * values. The input data must be properly initialized before calling
 * this function, and it is important to ensure that the data structure
 * is not null. If the provided data is invalid, the function may not
 * perform as expected.
 *
 * @param data A pointer to a `tokTypeMfgCbkeData` structure that holds the CBKE
 * data. This pointer must not be null and should point to a valid,
 * initialized structure. The function does not take ownership of
 * the data, and the caller is responsible for ensuring the validity
 * of the data before calling the function.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_cbke_data(tokTypeMfgCbkeData *data);

// Get manufacturing token security config
/***************************************************************************//**
 * @brief This function is used to obtain the security configuration from the
 * manufacturing token. It should be called when the application needs to
 * access security-related settings that are stored in the manufacturing
 * token. The caller must ensure that the `data` pointer is valid and
 * points to a buffer that can hold the security configuration data. The
 * function does not return a value, but it populates the provided buffer
 * with the security configuration data.
 *
 * @param data A pointer to a `tokTypeMfgSecurityConfig` structure where the
 * security configuration will be stored. The caller must ensure
 * that this pointer is not null and that the memory it points to is
 * allocated and valid.
 * @return The function populates the memory pointed to by `data` with the
 * security configuration data. There is no return value.
 ******************************************************************************/
void sl_get_mfg_token_security_config(tokTypeMfgSecurityConfig *data);

// Set manufacturing token security config
/***************************************************************************//**
 * @brief This function is used to configure the security settings for the
 * manufacturing token. It should be called when there is a need to
 * update the security configuration, typically during the initialization
 * or setup phase of the application. Ensure that the provided data is
 * valid and properly initialized before calling this function, as it may
 * not handle uninitialized or invalid data gracefully.
 *
 * @param data A pointer to a `tokTypeMfgSecurityConfig` structure that contains
 * the security configuration data. This pointer must not be null
 * and should point to a valid memory location containing
 * initialized data.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_security_config_data(tokTypeMfgSecurityConfig *data);

// Get manufacturing token installation code
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token installation
 * code, which is essential for device identification and configuration.
 * It should be called when the installation code is needed, typically
 * during the initialization or configuration phase of the device. The
 * caller must ensure that the provided pointer is valid and points to a
 * properly allocated `tokTypeMfgInstallationCode` structure, as the
 * function will write the installation code data into this structure.
 *
 * @param data A pointer to a `tokTypeMfgInstallationCode` structure where the
 * installation code will be stored. The pointer must not be null
 * and should point to a valid memory location that can hold the
 * installation code data.
 * @return The function does not return a value. Instead, it populates the
 * provided `data` structure with the installation code information.
 ******************************************************************************/
void sl_get_mfg_token_installation_code(tokTypeMfgInstallationCode *data);

// Set manufacturing token installation code
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token installation code
 * data, which is essential for device configuration and identification.
 * It should be called when the installation code needs to be updated,
 * typically during the initialization or configuration phase of the
 * device. Ensure that the provided data is valid and properly
 * initialized before calling this function, as invalid data may lead to
 * undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgInstallationCode` structure that
 * contains the installation code data. This pointer must not be
 * null and should point to a valid memory location containing
 * initialized data. The function does not take ownership of the
 * data, and the caller is responsible for ensuring the validity of
 * the data before calling the function.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_installation_code_data(tokTypeMfgInstallationCode *data);

// Get manufacturing token secure bootloader key
/***************************************************************************//**
 * @brief This function is used to obtain the secure bootloader key from the
 * manufacturing tokens. It should be called when the secure bootloader
 * key is needed for cryptographic operations or secure firmware updates.
 * The caller must ensure that the provided pointer is valid and points
 * to a `tokTypeMfgSecureBootloaderKey` structure that has been
 * allocated. The function does not perform any checks on the validity of
 * the pointer, and passing a null or invalid pointer may lead to
 * undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgSecureBootloaderKey` structure where
 * the retrieved key will be stored. This pointer must not be null
 * and should point to a valid memory location that can hold the key
 * data.
 * @return The function does not return a value. Instead, it populates the
 * provided `data` structure with the secure bootloader key.
 ******************************************************************************/
void sl_get_mfg_token_secure_bootloader_key(tokTypeMfgSecureBootloaderKey *data);

// Set manufacturing token secure bootloader key
/***************************************************************************//**
 * @brief This function is used to store the secure bootloader key data in the
 * manufacturing token system. It should be called when there is a need
 * to update or initialize the secure bootloader key, typically during
 * the device setup or configuration phase. Ensure that the provided data
 * is valid and properly initialized before calling this function, as it
 * does not perform any validation on the input data.
 *
 * @param data A pointer to a `tokTypeMfgSecureBootloaderKey` structure
 * containing the secure bootloader key data. The caller must ensure
 * that this pointer is not null and that the data is properly
 * initialized before passing it to the function.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_secure_bootloader_key_data(tokTypeMfgSecureBootloaderKey *data);

// Get manufacturing token CBKE 283K1 data
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token data specific
 * to the CBKE 283K1 cryptographic scheme. It should be called when the
 * application needs to access this specific token data, typically after
 * the system has been initialized. The provided pointer must point to a
 * valid `tokTypeMfgCbke283k1Data` structure that is large enough to hold
 * the retrieved data. If the pointer is null or invalid, the behavior is
 * undefined.
 *
 * @param data A pointer to a `tokTypeMfgCbke283k1Data` structure where the
 * retrieved token data will be stored. The pointer must not be null
 * and must point to a valid memory location that can accommodate
 * the data.
 * @return The function does not return a value. Instead, it populates the
 * provided `data` structure with the manufacturing token information.
 ******************************************************************************/
void sl_get_mfg_token_cbke283k1_data(tokTypeMfgCbke283k1Data *data);

// Set manufacturing token CBKE 283K1 data
/***************************************************************************//**
 * @brief This function is used to store the specified CBKE 283K1 data into the
 * manufacturing token system. It should be called when there is a need
 * to update or initialize the CBKE 283K1 data, typically during the
 * device setup or configuration phase. Ensure that the `data` parameter
 * is properly initialized before calling this function, as it will
 * directly affect the stored token data.
 *
 * @param data A pointer to a `tokTypeMfgCbke283k1Data` structure that contains
 * the CBKE 283K1 data to be set. This pointer must not be null and
 * should point to a valid memory location containing the data to be
 * stored.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_cbke283k1_data(tokTypeMfgCbke283k1Data *data);

// Get manufacturing token signed bootloader key X
/***************************************************************************//**
 * @brief This function is used to obtain the signed bootloader key X from the
 * manufacturing token system. It should be called when the application
 * needs to access the secure bootloader key for cryptographic operations
 * or firmware updates. The caller must ensure that the provided pointer
 * is valid and points to a buffer that can hold the key data. The
 * function does not perform any checks on the validity of the pointer,
 * and passing a null pointer may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgSignedBootloaderKeyX` structure where
 * the retrieved key will be stored. The caller must ensure that
 * this pointer is not null and that the memory it points to is
 * allocated and valid for writing. The function does not handle
 * null pointers or invalid memory addresses.
 * @return The function does not return a value. Instead, it populates the
 * provided `data` structure with the signed bootloader key X.
 ******************************************************************************/
void sl_get_mfg_token_signed_bootloader_key_x(tokTypeMfgSignedBootloaderKeyX *data);

// Set manufacturing token signed bootloader key X
/***************************************************************************//**
 * @brief This function is used to set the signed bootloader key X manufacturing
 * token, which is essential for secure boot processes. It should be
 * called when the application needs to update or initialize the
 * bootloader key with a new value. Ensure that the `data` parameter is
 * properly initialized before calling this function, as it will directly
 * modify the manufacturing token. Invalid or uninitialized data may lead
 * to undefined behavior.
 *
 * @param data Pointer to a `tokTypeMfgSignedBootloaderKeyX` structure
 * containing the new bootloader key data. This structure must be
 * properly initialized and must not be null. The function does not
 * perform any validation on the contents of the structure, so it is
 * the caller's responsibility to ensure that the data is valid.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_signed_bootloader_key_x_data(tokTypeMfgSignedBootloaderKeyX *data);

// Get manufacturing token signed bootloader key Y
/***************************************************************************//**
 * @brief This function is used to obtain the signed bootloader key Y from the
 * manufacturing token system. It should be called when the application
 * needs to access this specific key, typically during the boot process
 * or when verifying firmware integrity. The caller must ensure that the
 * provided pointer is valid and points to a buffer that can hold the key
 * data. The function does not perform any checks on the validity of the
 * pointer, so passing a null pointer may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgSignedBootloaderKeyY` structure where
 * the retrieved key will be stored. The caller must ensure that
 * this pointer is not null and that the memory it points to is
 * allocated and large enough to hold the key data.
 * @return The function does not return a value. Instead, it populates the
 * provided `data` structure with the signed bootloader key Y.
 ******************************************************************************/
void sl_get_mfg_token_signed_bootloader_key_y(tokTypeMfgSignedBootloaderKeyY *data);

// Set manufacturing token signed bootloader key Y
/***************************************************************************//**
 * @brief This function is used to set the signed bootloader key Y manufacturing
 * token, which is essential for secure boot processes. It should be
 * called when the application needs to update or initialize the
 * bootloader key Y. The input data must be properly formatted and
 * initialized before calling this function. If the provided data is
 * invalid or uninitialized, the behavior is undefined.
 *
 * @param data Pointer to a `tokTypeMfgSignedBootloaderKeyY` structure
 * containing the signed bootloader key Y data. The structure must
 * be properly initialized and must not be null. Passing a null
 * pointer or an uninitialized structure may lead to undefined
 * behavior.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_signed_bootloader_key_y_data(tokTypeMfgSignedBootloaderKeyY *data);

// Get manufacturing token NVM3 crypto key
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token that contains
 * the NVM3 crypto key. It should be called when the application needs to
 * access this specific key for cryptographic operations. The caller must
 * ensure that the provided pointer is valid and points to a buffer that
 * can hold the expected data. The function does not perform any checks
 * on the validity of the pointer, so passing a null pointer or an
 * invalid memory address may lead to undefined behavior.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgNvm3CryptoKey` where the
 * retrieved crypto key will be stored. The buffer must be large
 * enough to hold 16 bytes. Caller retains ownership of the buffer
 * and must ensure it is not null.
 * @return The function does not return a value. Instead, it populates the
 * buffer pointed to by `data` with the NVM3 crypto key.
 ******************************************************************************/
void sl_get_mfg_token_nvm3_crypto_key(tokTypeMfgNvm3CryptoKey *data);

// Set manufacturing token NVM3 crypto key
/***************************************************************************//**
 * @brief This function is used to store a specified NVM3 crypto key in the
 * manufacturing token system. It should be called when there is a need
 * to update or initialize the NVM3 crypto key data. The input data must
 * be a valid pointer to a `tokTypeMfgNvm3CryptoKey` structure, which
 * contains the key data. It is important to ensure that the provided
 * pointer is not null before calling this function, as passing a null
 * pointer may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgNvm3CryptoKey` structure that holds the
 * NVM3 crypto key data. The structure must be properly initialized
 * and must not be null. If the pointer is null, the function will
 * not perform any operation.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_nvm3_crypto_key_data(tokTypeMfgNvm3CryptoKey *data);

// Get manufacturing token zwave private key
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave private key from the
 * manufacturing token system. It should be called when the application
 * needs to access the private key for cryptographic operations or secure
 * communications. The caller must ensure that the provided pointer is
 * valid and points to a buffer that can hold the private key data. The
 * function does not perform any checks on the validity of the buffer
 * size, so it is the caller's responsibility to allocate sufficient
 * memory.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgZwavePrivateKey`, which
 * is expected to be at least 32 bytes in size. The caller retains
 * ownership of this buffer and must ensure it is not null before
 * calling the function.
 * @return The function does not return a value. Instead, it populates the
 * provided buffer with the Z-Wave private key data.
 ******************************************************************************/
void sl_get_mfg_token_zw_prk(tokTypeMfgZwavePrivateKey *data);

// Set manufacturing token zwave private key
/***************************************************************************//**
 * @brief This function is used to store a Z-Wave private key in the
 * manufacturing token system. It should be called when the application
 * needs to set or update the private key used for Z-Wave communications.
 * The input data must be a valid pointer to a
 * `tokTypeMfgZwavePrivateKey` structure, which is expected to contain
 * the private key data. It is important to ensure that the data provided
 * is properly initialized and valid, as invalid data may lead to
 * undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgZwavePrivateKey` structure that holds
 * the private key data. This pointer must not be null, and the
 * structure should be properly initialized before passing it to the
 * function. The function does not take ownership of the data, and
 * the caller is responsible for ensuring the validity of the data.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zw_prk_data(tokTypeMfgZwavePrivateKey *data);

// Get manufacturing token zwave public key
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave public key from the
 * manufacturing token system. It should be called when the application
 * needs to access the public key for cryptographic operations or device
 * identification. The caller must ensure that the provided pointer is
 * valid and points to a buffer that can hold the public key data. The
 * function does not perform any checks on the validity of the buffer
 * size, so it is the caller's responsibility to ensure that the buffer
 * is appropriately sized to avoid buffer overflows.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgZwavePublicKey` where
 * the public key will be stored. The buffer must be large enough to
 * hold 32 bytes of data. Caller retains ownership of the buffer and
 * must ensure it is not null.
 * @return The function returns nothing and populates the provided buffer with
 * the Z-Wave public key data.
 ******************************************************************************/
void sl_get_mfg_token_zw_puk(tokTypeMfgZwavePublicKey *data);

// Set manufacturing token zwave public key
/***************************************************************************//**
 * @brief This function is used to set the Z-Wave public key manufacturing
 * token, which is essential for secure communication in Z-Wave networks.
 * It should be called when the application needs to update or initialize
 * the public key used for Z-Wave operations. Ensure that the `data`
 * parameter is properly initialized before calling this function, as it
 * will directly modify the manufacturing token. Invalid or uninitialized
 * data may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgZwavePublicKey` structure that holds
 * the public key data. This pointer must not be null and should
 * point to a valid memory location containing the public key. The
 * function expects the public key to be properly formatted and
 * initialized; otherwise, it may result in incorrect token
 * settings.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zw_puk_data(tokTypeMfgZwavePublicKey *data);

// Get manufacturing token ZW QR code
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave QR code manufacturing
 * token, which is stored in the provided buffer. It should be called
 * when the application needs to access the QR code data for Z-Wave
 * functionality. The caller must ensure that the buffer passed to the
 * function is properly allocated and can hold the expected data size. If
 * the provided buffer is null, the behavior is undefined.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgQRCode` that will
 * receive the QR code data. The buffer must be allocated by the
 * caller and should be large enough to hold the QR code data. Must
 * not be null.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_zw_qr_code(tokTypeMfgQRCode *data);

// Set manufacturing token ZW QR code
/***************************************************************************//**
 * @brief This function is used to store Z-Wave QR code data in the
 * manufacturing token system. It should be called when there is a need
 * to update or initialize the QR code data associated with the device.
 * The input data must be a valid pointer to a `tokTypeMfgQRCode`
 * structure, which contains the QR code information. It is important to
 * ensure that the data being passed is properly formatted and does not
 * exceed the expected size, as the function does not perform validation
 * on the contents of the data.
 *
 * @param data A pointer to a `tokTypeMfgQRCode` structure that holds the QR
 * code data. The structure must be properly allocated and
 * initialized by the caller. The function does not take ownership
 * of the data, and it must not be null.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zw_qr_code_data(tokTypeMfgQRCode *data);

// Get manufacturing token ZW initialized
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave initialization token, which
 * indicates whether the Z-Wave module has been initialized. It should be
 * called when the application needs to check the initialization status
 * of the Z-Wave module. The caller must ensure that the `data` pointer
 * is valid and points to a buffer that can hold the token data. The
 * function does not modify the state of the system or the token itself.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgZwaveInitialized` where
 * the retrieved token data will be stored. The buffer must be
 * allocated by the caller and must not be null.
 * @return The function returns nothing and populates the buffer pointed to by
 * `data` with the Z-Wave initialization token.
 ******************************************************************************/
void sl_get_mfg_token_zw_initialized(tokTypeMfgZwaveInitialized *data);

// Set manufacturing token ZW initialized
/***************************************************************************//**
 * @brief This function is used to set the Z-Wave initialized manufacturing
 * token data, which is essential for the proper configuration of Z-Wave
 * devices. It should be called when the device is being initialized or
 * configured, ensuring that the token reflects the current state of the
 * Z-Wave initialization. The input data must be valid and properly
 * formatted, as it directly affects the device's operation. It is
 * important to ensure that this function is called in the appropriate
 * context, typically after any necessary initialization routines.
 *
 * @param data Pointer to a `tokTypeMfgZwaveInitialized` structure that holds
 * the initialization data. This pointer must not be null and should
 * point to a valid memory location containing the appropriate data.
 * If the pointer is null or points to invalid memory, the behavior
 * of the function is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zw_initialized_data(tokTypeMfgZwaveInitialized *data);

// Get manufacturing token ZW QR code external
/***************************************************************************//**
 * @brief This function is used to obtain the Z-Wave QR code manufacturing
 * token, which is typically required for device identification or
 * configuration purposes. It should be called after the necessary
 * initialization of the manufacturing token system. The provided pointer
 * must point to a valid `tokTypeMfgQRCode` structure, which will be
 * populated with the QR code data. If the pointer is null, the behavior
 * is undefined.
 *
 * @param data A pointer to a `tokTypeMfgQRCode` structure where the QR code
 * data will be stored. This pointer must not be null, and the
 * caller retains ownership of the memory allocated for this
 * structure.
 * @return None
 ******************************************************************************/
void sl_get_mfg_token_zw_qr_code_ext(tokTypeMfgQRCode *data);

// Set manufacturing token ZW QR code external
/***************************************************************************//**
 * @brief This function is used to set the external Z-Wave QR code manufacturing
 * token data. It should be called when there is a need to update or
 * initialize the QR code data associated with the device's manufacturing
 * information. Ensure that the `data` parameter is properly allocated
 * and populated with valid QR code information before calling this
 * function. The function does not perform any validation on the contents
 * of the `data` parameter, so it is the caller's responsibility to
 * ensure that the data is correct.
 *
 * @param data A pointer to a `tokTypeMfgQRCode` structure that holds the QR
 * code data. This pointer must not be null and should point to a
 * valid memory location containing the QR code information.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_zw_qr_code_ext_data(tokTypeMfgQRCode *data);

/***************************************************************************//**
 * Only available on unix
 ******************************************************************************/
#else
// Get manufacturing token NVDATA version
/***************************************************************************//**
 * @brief This function is used to obtain the current version of the
 * manufacturing token related to NVDATA. It should be called when the
 * application needs to check or utilize the NVDATA version for
 * configuration or compatibility purposes. The caller must ensure that
 * the provided pointer is valid and points to a memory location that can
 * hold the version data. The function does not perform any internal
 * validation on the pointer, so passing a null pointer or an invalid
 * address may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgNvdataVersion` variable where the
 * retrieved version will be stored. This pointer must not be null
 * and should point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the current NVDATA version.
 ******************************************************************************/
void sl_get_mfg_token_nvdata_version(tokTypeMfgNvdataVersion *data);

// Set manufacturing token NVDATA version
/***************************************************************************//**
 * @brief This function is used to set the NVDATA version data for the
 * manufacturing token. It should be called when there is a need to
 * update the version information stored in the token. Ensure that the
 * `data` parameter is properly initialized before calling this function,
 * as it will directly modify the token's stored value. Invalid or
 * uninitialized data may lead to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgNvdataVersion` structure that holds the
 * version data. This pointer must not be null and should point to a
 * valid memory location containing the version information.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_nvdata_version_data(tokTypeMfgNvdataVersion *data);

// Get manufacturing token EUI64 Hash
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token EUI64 hash,
 * which is a unique identifier for the device. It should be called when
 * the application needs to access this specific token, typically after
 * the system has been initialized. The caller must ensure that the
 * provided pointer is valid and points to a buffer that can hold the
 * expected data.
 *
 * @param data A pointer to a buffer of type `tokTypeMfgEui64Hash` where the
 * EUI64 hash will be stored. The buffer must be large enough to
 * hold 8 bytes. The caller retains ownership of the buffer and must
 * ensure it is not null.
 * @return The function does not return a value. Instead, it populates the
 * provided buffer with the EUI64 hash data.
 ******************************************************************************/
void sl_get_mfg_token_eui64_hash(tokTypeMfgEui64Hash *data);

// Set manufacturing token EUI64 Hash
/***************************************************************************//**
 * @brief This function is used to set the manufacturing token for the EUI64
 * hash, which is typically required during the initialization or
 * configuration of a device. It should be called when the application
 * needs to store a specific EUI64 hash value, ensuring that the provided
 * data is valid and properly formatted. The function does not return a
 * value, but it is important to ensure that the input data is correctly
 * initialized before calling this function to avoid undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgEui64Hash` structure that holds the
 * EUI64 hash data. This pointer must not be null and should point
 * to a valid memory location containing the hash data. The function
 * expects the data to be properly initialized; otherwise, the
 * behavior is undefined.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_eui64_hash_data(tokTypeMfgEui64Hash *data);

// Get manufacturing token Radio bands supported
/***************************************************************************//**
 * @brief This function is used to obtain the radio bands supported by the
 * device as specified in the manufacturing token. It should be called
 * when the application needs to know which radio bands are available for
 * operation. The caller must ensure that the `data` pointer is valid and
 * points to a memory location that can hold the result. The function
 * does not perform any checks on the contents of the `data` pointer, so
 * it is the caller's responsibility to allocate sufficient memory.
 *
 * @param data A pointer to a `tokTypeMfgRadioBandsSupported` structure where
 * the supported radio bands will be stored. The pointer must not be
 * null and must point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the supported radio bands.
 ******************************************************************************/
void sl_get_mfg_token_radio_bands_supported(tokTypeMfgRadioBandsSupported *data);

// Set manufacturing token Radio bands supported
/***************************************************************************//**
 * @brief This function is used to store the supported radio bands in the
 * manufacturing token system. It should be called when the application
 * needs to update the radio bands supported by the device. The input
 * data must be a valid pointer to a `tokTypeMfgRadioBandsSupported`
 * structure, which contains the necessary information. It is important
 * to ensure that the data being passed is properly initialized before
 * calling this function.
 *
 * @param data A pointer to a `tokTypeMfgRadioBandsSupported` structure that
 * holds the radio bands information. The pointer must not be null
 * and should point to a valid memory location containing the
 * supported radio bands data.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_radio_bands_supported_data(tokTypeMfgRadioBandsSupported *data);

// Get manufacturing token Radio crytal offset
/***************************************************************************//**
 * @brief This function is used to obtain the manufacturing token that
 * represents the radio crystal offset. It should be called when the
 * application needs to access this specific manufacturing data,
 * typically after the system has been initialized. The provided pointer
 * must point to a valid memory location that can hold the data, and the
 * function will populate this memory with the retrieved value. Ensure
 * that the pointer is not null before calling this function to avoid
 * undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgRadioCrystalOffset` variable where the
 * retrieved radio crystal offset will be stored. The pointer must
 * not be null and should point to a valid memory location.
 * @return The function does not return a value. Instead, it populates the
 * memory pointed to by `data` with the radio crystal offset.
 ******************************************************************************/
void sl_get_mfg_token_radio_crystal_offset(tokTypeMfgRadioCrystalOffset *data);

// Set manufacturing token Radio crytal offset
/***************************************************************************//**
 * @brief This function is used to store a specified radio crystal offset value
 * in the manufacturing token system. It should be called when there is a
 * need to update the radio crystal offset, typically during device
 * configuration or initialization. The input data must be a valid
 * pointer to a `tokTypeMfgRadioCrystalOffset` structure, which holds the
 * offset value. It is important to ensure that the data pointer is not
 * null before calling this function, as passing a null pointer may lead
 * to undefined behavior.
 *
 * @param data A pointer to a `tokTypeMfgRadioCrystalOffset` structure that
 * contains the radio crystal offset value to be set. The pointer
 * must not be null, and the value should be within the valid range
 * for the `tokTypeMfgRadioCrystalOffset` type, which is typically
 * defined by the application context. If the pointer is null, the
 * function will not perform any operation.
 * @return None
 ******************************************************************************/
void sl_set_mfg_token_radio_crystal_offset_data(tokTypeMfgRadioCrystalOffset *data);
#endif  // SL_ZIGBEE_TEST

#ifdef __cplusplus
}
#endif

#endif /* SL_TOKEN_MFG_GENERIC_H_ */
