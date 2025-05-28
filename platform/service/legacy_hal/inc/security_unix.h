/***************************************************************************//**
 * @file
 * @brief This file implements the low-level routines for encrypting using AES.
 * These are normally provided by the chip on our real hardware.
 * For simulation we can either use real encryption, pseudo encryption,
 * or no encryption. This gives us more flexibility in debugging.
 * We use the Rijndael encryption algorithm to do the work for
 * real encryption.
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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
#define pseudoEncryptOffset 0x55
#define SECURITY_M    0
#define SECURITY_NONCE_TOTAL_SIZE 0

#define pseudoEncryptOffset 0x55

/***************************************************************************//**
 * @brief The `emSecurityKey` is an external array of unsigned 8-bit integers,
 * which is likely used to store a security key for encryption purposes.
 * It is declared as an external variable, indicating that its definition
 * is located in another file, and it is used across multiple files in
 * the project.
 *
 * @details The `emSecurityKey` is used to hold the security key necessary for
 * encryption operations within the system.
 ******************************************************************************/
extern uint8_t emSecurityKey[];

/***************************************************************************//**
 * @brief The `standardMic` is an external array of unsigned 8-bit integers. It
 * is likely used to store a Message Integrity Code (MIC) for encryption
 * purposes, as suggested by its name and context within the encryption
 * routines.
 *
 * @details The `standardMic` is used in encryption processes, specifically when
 * constructing a MIC parcel with the standard MIC type.
 ******************************************************************************/
extern uint8_t standardMic[];
/***************************************************************************//**
 * @brief The `PseudoEncryptMicType` is an enumeration that defines two possible
 * types of Message Integrity Codes (MIC) that can be used in the context
 * of pseudo encryption: a standard MIC and a transient key MIC. This
 * enumeration is used to specify which type of MIC should be applied
 * during the encryption process, providing flexibility in the encryption
 * strategy used for debugging or simulation purposes.
 *
 * @param USE_STANDARD_MIC Represents the use of a standard Message Integrity
 * Code (MIC) with a value of 0.
 * @param USE_TRANSIENT_KEY_MIC Represents the use of a transient key Message
 * Integrity Code (MIC) with a value of 1.
 ******************************************************************************/
typedef enum {
  USE_STANDARD_MIC      = 0,
  USE_TRANSIENT_KEY_MIC = 1,
} PseudoEncryptMicType;

// These routines use PSEUDO Encryption
/***************************************************************************//**
 * @brief The `encryptParcel` is a function pointer that takes a pointer to an
 * `sli_parcel_t` structure as an argument and returns a pointer to an
 * `sli_parcel_t` structure. It is used to perform encryption on the
 * provided parcel using AES or pseudo encryption methods, depending on
 * the configuration.
 *
 * @details This function is used to encrypt a parcel of data, typically for
 * secure transmission or storage.
 ******************************************************************************/
sli_parcel_t *encryptParcel(sli_parcel_t *clearText);
/***************************************************************************//**
 * @brief The `encryptParcelWithSpecialMic` is a function pointer that takes a
 * pointer to a `sli_parcel_t` structure representing clear text data and
 * a `PseudoEncryptMicType` enumeration value as parameters. It returns a
 * pointer to a `sli_parcel_t` structure, which is presumably the
 * encrypted version of the input data with a special Message Integrity
 * Code (MIC) applied based on the specified type.
 *
 * @details This function is used to encrypt data parcels with a special MIC,
 * allowing for different encryption behaviors based on the
 * `PseudoEncryptMicType` provided.
 ******************************************************************************/
sli_parcel_t *encryptParcelWithSpecialMic(sli_parcel_t *clearText, PseudoEncryptMicType type);
/***************************************************************************//**
 * @brief The `encryptParcelNoMic` is a function pointer that points to a
 * function taking a pointer to `sli_parcel_t` as an argument and
 * returning a pointer to `sli_parcel_t`. This function is part of a
 * suite of encryption functions that perform pseudo encryption without
 * using a Message Integrity Code (MIC).
 *
 * @details This function is used to encrypt data parcels without applying a
 * MIC, likely for scenarios where integrity checks are not required or
 * handled differently.
 ******************************************************************************/
sli_parcel_t *encryptParcelNoMic(sli_parcel_t *clearText);
/***************************************************************************//**
 * @brief The `constructMicParcel` function is a global function that returns a
 * pointer to an `sli_parcel_t` structure. It takes a
 * `PseudoEncryptMicType` enumeration as an argument, which determines
 * the type of MIC (Message Integrity Code) to be used in the parcel
 * construction.
 *
 * @details This function is used to construct a parcel with a specific type of
 * MIC, either standard or transient key, based on the provided
 * `PseudoEncryptMicType`.
 ******************************************************************************/
sli_parcel_t *constructMicParcel(PseudoEncryptMicType type);

#define standardMicParcel() \
  constructMicParcel(USE_STANDARD_MIC)
#define transientLinkKeyMicParcel() \
  constructMicParcel(USE_TRANSIENT_KEY_MIC)

#if defined(SL_ZIGBEE_TEST) || defined(ZIGBEE_STACK_ON_HOST)

enum {
  USE_REAL_ENCRYPTION   = 0,
  USE_PSEUDO_ENCRYPTION = 1,
  USE_NO_ENCRYPTION     = 2,
};

typedef uint8_t SimulatorEncryption;

/***************************************************************************//**
 * @brief The `simulatorEncryptionType` is a global variable of type
 * `SimulatorEncryption`, which is an enumeration used to specify the
 * type of encryption to be used in a simulation environment. The
 * possible values for this variable are `USE_REAL_ENCRYPTION`,
 * `USE_PSEUDO_ENCRYPTION`, and `USE_NO_ENCRYPTION`, allowing for
 * flexibility in debugging and testing scenarios.
 *
 * @details This variable is used to set and determine the encryption method
 * employed during simulation, affecting how data is encrypted or not
 * encrypted in the simulated environment.
 ******************************************************************************/
extern SimulatorEncryption simulatorEncryptionType;

void emSetSimulatorEncryption(SimulatorEncryption type);

#endif

#ifndef MAC_TEST_STACK
// These are only used in zigbee.  They cause a compile error
// in flex now that we removed the legacy buffer system header.
bool emPseudoEncryptPacket(sli_zigbee_packet_header_t *header,
                           uint8_t authenticationStartOffset,
                           uint8_t encryptionStartOffset);

bool emPseudoDecryptPacket(sli_zigbee_packet_header_t header,
                           uint8_t authenticationStartOffset,
                           uint8_t encryptionStartOffset);

void emPseudoEncryptFlatPacket(uint8_t *packet,
                               uint8_t length,
                               uint8_t authenticationStartOffset,
                               uint8_t encryptionStartOffset);

/***************************************************************************//**
 * @brief The `emPseudoEncryptUseTransientLinkKey` is a global boolean variable
 * that indicates whether pseudo encryption should utilize a transient
 * link key. It is part of the encryption simulation mechanism used in
 * the Zigbee stack for testing purposes.
 *
 * @details This variable is used to determine if a transient link key should be
 * employed during pseudo encryption operations.
 ******************************************************************************/
extern bool emPseudoEncryptUseTransientLinkKey;
#endif
