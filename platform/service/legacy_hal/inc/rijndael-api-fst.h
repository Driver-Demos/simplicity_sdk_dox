/***************************************************************************//**
 * # License
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is Third Party Software licensed by Silicon Labs from a third party
 * and is governed by the sections of the MSLA applicable to Third Party
 * Software and the additional terms set forth below.
 *
 ******************************************************************************/
/**
 * rijndael-api-fst.h
 *
 * @version 2.9 (December 2000)
 *
 * Optimised ANSI C code for the Rijndael cipher (now AES)
 *
 * @author Vincent Rijmen <vincent.rijmen@esat.kuleuven.ac.be>
 * @author Antoon Bosselaers <antoon.bosselaers@esat.kuleuven.ac.be>
 * @author Paulo Barreto <paulo.barreto@terra.com.br>
 *
 * This code is hereby placed in the public domain.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ''AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Acknowledgements:
 *
 * We are deeply indebted to the following people for their bug reports,
 * fixes, and improvement suggestions to this implementation. Though we
 * tried to list all contributions, we apologise in advance for any
 * missing reference.
 *
 * Andrew Bales <Andrew.Bales@Honeywell.com>
 * Markus Friedl <markus.friedl@informatik.uni-erlangen.de>
 * John Skodon <skodonj@webquill.com>
 */

#ifndef __RIJNDAEL_API_FST_H
#define __RIJNDAEL_API_FST_H

#include <stdio.h>
#include "rijndael-alg-fst.h"

/*  Generic Defines  */
#define     DIR_ENCRYPT           0 /*  Are we encrpyting?  */
#define     DIR_DECRYPT           1 /*  Are we decrpyting?  */
#define     MODE_ECB              1 /*  Are we ciphering in ECB mode?   */
#define     MODE_CBC              2 /*  Are we ciphering in CBC mode?   */
#define     MODE_CFB1             3 /*  Are we ciphering in 1-bit CFB mode? */
#ifndef TRUE
#define     TRUE                  1
#define     FALSE                 0
#endif
#define     BITSPERBLOCK        128 /* Default number of bits in a cipher block */

/*  Error Codes  */
#define     BAD_KEY_DIR          -1 /*  Key direction is invalid, e.g., unknown value */
#define     BAD_KEY_MAT          -2 /*  Key material not of correct length */
#define     BAD_KEY_INSTANCE     -3 /*  Key passed is not valid */
#define     BAD_CIPHER_MODE      -4 /*  Params struct passed to cipherInit invalid */
#define     BAD_CIPHER_STATE     -5 /*  Cipher in wrong state (e.g., not initialized) */
#define     BAD_BLOCK_LENGTH     -6
#define     BAD_CIPHER_INSTANCE  -7
#define     BAD_DATA             -8 /*  Data contents are invalid, e.g., invalid padding */
#define     BAD_OTHER            -9 /*  Unknown error */

/*  Algorithm-specific Defines  */
#define     MAX_KEY_SIZE         64 /* # of ASCII char's needed to represent a key */
#define     MAX_IV_SIZE          16 /* # bytes needed to represent an IV  */

/*  Typedefs  */

typedef unsigned char BYTE;

/*  The structure for key information */
/***************************************************************************//**
 * @brief The `keyInstance` structure is designed to encapsulate key-related
 * information for the Rijndael cipher, which is the basis for AES
 * encryption. It includes fields to specify the direction of the key
 * (encryption or decryption), the length of the key, and the raw key
 * material in ASCII format. Additionally, it maintains the number of
 * rounds required for the encryption process, as well as the key
 * schedules for both general encryption/decryption and CFB1 mode
 * encryption. This structure is essential for managing and applying
 * cryptographic keys within the Rijndael cipher implementation.
 *
 * @param direction Indicates whether the key is used for encryption or
 * decryption.
 * @param keyLen Specifies the length of the key.
 * @param keyMaterial Holds the raw key data in ASCII format.
 * @param Nr Represents the number of rounds, which is dependent on the key
 * length.
 * @param rk Stores the key schedule for encryption and decryption.
 * @param ek Contains the key schedule specifically for CFB1 mode encryption.
 ******************************************************************************/
typedef struct {
  BYTE direction;                   /* Key used for encrypting or decrypting? */
  int keyLen;                       /* Length of the key  */
  char keyMaterial[MAX_KEY_SIZE + 1];   /* Raw key data in ASCII, e.g., user input or KAT values */
  int Nr;                         /* key-length-dependent number of rounds */
  u32 rk[4 * (MAXNR + 1)];          /* key schedule */
  u32 ek[4 * (MAXNR + 1)];          /* CFB1 key schedule (encryption only) */
} keyInstance;

/*  The structure for cipher information */
/***************************************************************************//**
 * @brief The `cipherInstance` structure is used to encapsulate information
 * about the ciphering process in the Rijndael (AES) encryption
 * algorithm. It contains a mode field that specifies the operational
 * mode of the cipher (such as ECB, CBC, or CFB1), and an Initialization
 * Vector (IV) which is used in certain modes to ensure that identical
 * plaintext blocks result in different ciphertext blocks. This structure
 * is essential for initializing and managing the state of the cipher
 * during encryption and decryption operations.
 *
 * @param mode Specifies the cipher mode, which can be MODE_ECB, MODE_CBC, or
 * MODE_CFB1.
 * @param IV An array of bytes representing the Initialization Vector used in
 * ciphering, with a maximum size defined by MAX_IV_SIZE.
 ******************************************************************************/
typedef struct {                    /* changed order of the components */
  BYTE mode;                        /* MODE_ECB, MODE_CBC, or MODE_CFB1 */
  BYTE IV[MAX_IV_SIZE];             /* A possible Initialization Vector for ciphering */
} cipherInstance;

/*  Function prototypes  */

/***************************************************************************//**
 * @brief This function sets up a key instance for use in encryption or
 * decryption operations, based on the specified direction and key
 * length. It must be called with a valid keyInstance structure, a
 * direction indicating whether the key is for encryption or decryption,
 * and a key length of 128, 192, or 256 bits. The key material should be
 * provided as a hexadecimal string, and the function will validate and
 * convert this into a format suitable for the Rijndael cipher. If any
 * parameter is invalid, the function returns an error code.
 *
 * @param key A pointer to a keyInstance structure that will be initialized.
 * Must not be null.
 * @param direction A BYTE indicating the direction of the key: DIR_ENCRYPT for
 * encryption or DIR_DECRYPT for decryption. Invalid values
 * result in a BAD_KEY_DIR error.
 * @param keyLen An integer specifying the length of the key in bits. Must be
 * 128, 192, or 256. Invalid values result in a BAD_KEY_MAT error.
 * @param keyMaterial A string containing the key material in hexadecimal
 * format. Must not be null and must match the specified key
 * length. Invalid or incorrectly formatted key material
 * results in a BAD_KEY_MAT error.
 * @return Returns TRUE on success, or an error code (BAD_KEY_INSTANCE,
 * BAD_KEY_DIR, BAD_KEY_MAT) on failure.
 ******************************************************************************/
int makeKey(keyInstance *key, BYTE direction, int keyLen, char *keyMaterial);

/***************************************************************************//**
 * @brief This function sets up a cipher instance for encryption or decryption
 * by specifying the mode of operation and an optional initialization
 * vector (IV). It must be called before any encryption or decryption
 * operations are performed. The function supports three modes: ECB, CBC,
 * and CFB1. If an IV is provided, it must be a valid hexadecimal string;
 * otherwise, the IV is set to zero. The function returns an error code
 * if an invalid mode is specified or if the IV is not a valid
 * hexadecimal string.
 *
 * @param cipher A pointer to a cipherInstance structure that will be
 * initialized. Must not be null.
 * @param mode A BYTE indicating the cipher mode. Valid values are MODE_ECB,
 * MODE_CBC, and MODE_CFB1. Invalid values result in a
 * BAD_CIPHER_MODE error.
 * @param IV A pointer to a null-terminated string representing the
 * initialization vector in hexadecimal format. Can be null, in which
 * case the IV is set to zero.
 * @return Returns TRUE on success, BAD_CIPHER_MODE if the mode is invalid, or
 * BAD_CIPHER_INSTANCE if the IV is invalid.
 ******************************************************************************/
int cipherInit(cipherInstance *cipher, BYTE mode, char *IV);

/***************************************************************************//**
 * @brief This function encrypts data using the provided cipher and key
 * instances, processing the input data in blocks according to the cipher
 * mode specified in the cipher instance. It supports ECB, CBC, and CFB1
 * modes. The function must be called with valid, initialized cipher and
 * key instances, and the key instance must be set for encryption. The
 * input data length must be a positive multiple of the block size (128
 * bits). The function writes the encrypted data to the provided output
 * buffer. If the input length is zero or negative, the function returns
 * immediately with no action. The function returns the number of bits
 * processed or an error code if the cipher or key instances are invalid.
 *
 * @param cipher A pointer to a cipherInstance structure that specifies the
 * encryption mode and, if applicable, the initialization vector
 * (IV). Must not be null and must be properly initialized.
 * @param key A pointer to a keyInstance structure that contains the encryption
 * key and related information. Must not be null, must be initialized
 * for encryption (direction must be DIR_ENCRYPT).
 * @param input A pointer to the input data to be encrypted. Must not be null
 * and should point to data of length inputLen.
 * @param inputLen The length of the input data in bits. Must be a positive
 * multiple of 128.
 * @param outBuffer A pointer to a buffer where the encrypted output will be
 * stored. Must be large enough to hold the encrypted data.
 * @return Returns the number of bits processed if successful, or an error code
 * if the cipher or key instances are invalid.
 ******************************************************************************/
int blockEncrypt(cipherInstance *cipher, keyInstance *key,
                 BYTE *input, int inputLen, BYTE *outBuffer);

/***************************************************************************//**
 * @brief This function encrypts the input data using the specified cipher and
 * key, applying padding to ensure the data length is a multiple of the
 * block size. It supports ECB and CBC modes, and must be called with a
 * properly initialized cipher and key. The function handles input data
 * that is not a multiple of the block size by adding padding, and writes
 * the encrypted data to the provided output buffer. It returns the total
 * number of bytes written to the output buffer, including padding. The
 * function requires the key to be set for encryption and will return an
 * error if the key is set for decryption or if the cipher is in an
 * invalid state.
 *
 * @param cipher A pointer to a cipherInstance structure that specifies the
 * encryption mode and, if applicable, the initialization vector.
 * Must not be null and must be properly initialized.
 * @param key A pointer to a keyInstance structure that contains the encryption
 * key and related information. Must not be null, and the direction
 * must be set to DIR_ENCRYPT.
 * @param input A pointer to the data to be encrypted. Must not be null and
 * should point to a buffer of at least inputOctets bytes.
 * @param inputOctets The number of bytes in the input data. Must be greater
 * than 0.
 * @param outBuffer A pointer to a buffer where the encrypted data will be
 * written. Must be large enough to hold the encrypted output,
 * which will be a multiple of the block size (16 bytes).
 * @return Returns the total number of bytes written to the output buffer,
 * including padding, or an error code if the operation fails.
 ******************************************************************************/
int padEncrypt(cipherInstance *cipher, keyInstance *key,
               BYTE *input, int inputOctets, BYTE *outBuffer);

/***************************************************************************//**
 * @brief This function is used to decrypt data blocks using the provided cipher
 * and key instances. It supports different modes of operation, including
 * ECB, CBC, and CFB1. The function requires that the cipher and key
 * instances are properly initialized and configured for decryption. The
 * input data must be a multiple of the block size (128 bits), and the
 * function will process the data in blocks, writing the decrypted output
 * to the specified output buffer. If the input length is zero or
 * negative, the function returns immediately with no action. The
 * function returns the number of bits successfully decrypted or an error
 * code if the cipher state is invalid.
 *
 * @param cipher A pointer to a cipherInstance structure that specifies the mode
 * of operation and, if applicable, the initialization vector
 * (IV). Must not be null and must be properly initialized for
 * decryption.
 * @param key A pointer to a keyInstance structure that contains the decryption
 * key and related information. Must not be null and must be
 * configured for decryption.
 * @param input A pointer to the input data to be decrypted. Must not be null
 * and should point to data that is a multiple of the block size
 * (128 bits).
 * @param inputLen The length of the input data in bits. Must be a positive
 * multiple of 128. If zero or negative, the function returns
 * immediately with no action.
 * @param outBuffer A pointer to a buffer where the decrypted output will be
 * stored. Must be large enough to hold the decrypted data.
 * @return Returns the number of bits successfully decrypted, or an error code
 * if the cipher state is invalid.
 ******************************************************************************/
int blockDecrypt(cipherInstance *cipher, keyInstance *key,
                 BYTE *input, int inputLen, BYTE *outBuffer);

/***************************************************************************//**
 * @brief This function is used to decrypt data that has been encrypted with
 * padding, using the specified cipher and key instances. It supports ECB
 * and CBC modes of operation. The function must be called with a
 * properly initialized cipher and key, where the key's direction is set
 * to decryption. The input data must be a non-null pointer with a length
 * that is a multiple of 16 bytes, as this is the block size for the
 * Rijndael cipher. The function will return an error if the input data
 * is not properly padded or if the cipher or key instances are in an
 * invalid state. The decrypted data is written to the provided output
 * buffer.
 *
 * @param cipher A pointer to a cipherInstance structure that specifies the mode
 * of operation (ECB or CBC) and, if applicable, the
 * initialization vector. Must not be null and must be properly
 * initialized.
 * @param key A pointer to a keyInstance structure that contains the decryption
 * key and its associated parameters. The direction field must be set
 * to DIR_DECRYPT. Must not be null.
 * @param input A pointer to the input data to be decrypted. The data length
 * must be a multiple of 16 bytes. Must not be null.
 * @param inputOctets The length of the input data in bytes. Must be greater
 * than 0 and a multiple of 16.
 * @param outBuffer A pointer to a buffer where the decrypted output will be
 * stored. Must be large enough to hold the decrypted data.
 * @return Returns the number of bytes written to the output buffer if
 * successful, or an error code if the operation fails.
 ******************************************************************************/
int padDecrypt(cipherInstance *cipher, keyInstance *key,
               BYTE *input, int inputOctets, BYTE *outBuffer);

#ifdef INTERMEDIATE_VALUE_KAT
/***************************************************************************//**
 * @brief This function is used to perform a specified number of rounds of
 * encryption or decryption on a 16-byte data block using the Rijndael
 * cipher. It requires a properly initialized cipher instance and key
 * instance, and the direction of the operation (encryption or
 * decryption) is determined by the key instance. The function is
 * intended for use in scenarios where intermediate values of the
 * encryption or decryption process are needed, such as in testing or
 * analysis. It is important to ensure that the cipher and key instances
 * are correctly initialized before calling this function, as invalid
 * instances will result in an error code. The input data must be exactly
 * 16 bytes long, and the output buffer must be able to hold at least 16
 * bytes.
 *
 * @param cipher A pointer to a cipherInstance structure that must be properly
 * initialized. Must not be null.
 * @param key A pointer to a keyInstance structure that must be properly
 * initialized with the correct direction (encryption or decryption).
 * Must not be null.
 * @param input A pointer to a 16-byte array of data to be processed. The data
 * must be exactly 16 bytes long.
 * @param inputLen The length of the input data. Must be 16 to match the block
 * size.
 * @param outBuffer A pointer to a buffer where the processed 16-byte data block
 * will be stored. Must be able to hold at least 16 bytes.
 * @param rounds The number of encryption or decryption rounds to perform. Must
 * be a valid number of rounds for the given key.
 * @return Returns TRUE (1) on success, BAD_CIPHER_STATE if the cipher or key is
 * null, or BAD_KEY_DIR if the key direction is invalid.
 ******************************************************************************/
int cipherUpdateRounds(cipherInstance *cipher, keyInstance *key,
                       BYTE *input, int inputLen, BYTE *outBuffer, int Rounds);

#endif /* INTERMEDIATE_VALUE_KAT */

#endif /* __RIJNDAEL_API_FST_H */
