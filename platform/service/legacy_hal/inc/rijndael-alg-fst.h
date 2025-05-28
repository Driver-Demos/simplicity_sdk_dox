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
 * rijndael-alg-fst.h
 *
 * @version 3.0 (December 2000)
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
 */
#ifndef __RIJNDAEL_ALG_FST_H
#define __RIJNDAEL_ALG_FST_H

#define MAXKC (256 / 32)
#define MAXKB (256 / 8)
#define MAXNR 14

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

/***************************************************************************//**
 * @brief This function initializes the round keys required for AES encryption
 * based on the provided cipher key and key size. It must be called
 * before performing encryption operations to ensure that the round keys
 * are correctly set up. The function supports key sizes of 128, 192, and
 * 256 bits, and the number of rounds is determined by the key size. The
 * caller must provide a buffer large enough to hold the round keys,
 * which is typically 4 times the number of rounds plus one. The function
 * returns the number of rounds used for the encryption process.
 *
 * @param rk An array of unsigned 32-bit integers where the round keys will be
 * stored. The array must be large enough to hold 4 times the number
 * of rounds plus one. The caller retains ownership and must ensure
 * the array is not null.
 * @param cipherKey A pointer to an array of unsigned 8-bit integers
 * representing the cipher key. The length of the array must
 * match the specified key size (16 bytes for 128 bits, 24
 * bytes for 192 bits, and 32 bytes for 256 bits). The caller
 * retains ownership and must ensure the pointer is not null.
 * @param keyBits An integer specifying the size of the cipher key in bits.
 * Valid values are 128, 192, or 256. If an invalid value is
 * provided, the function will not perform any setup and will
 * return 0.
 * @return Returns the number of rounds used for the encryption process based on
 * the key size (10 for 128 bits, 12 for 192 bits, and 14 for 256 bits).
 * Returns 0 if the key size is invalid.
 ******************************************************************************/
int rijndaelKeySetupEnc(u32 rk[] /*4*(Nr + 1)*/, const u8 cipherKey[], int keyBits);
/***************************************************************************//**
 * @brief This function initializes the decryption key schedule for the Rijndael
 * cipher, which is now known as AES. It should be called with a valid
 * cipher key and the desired key length in bits. The function prepares
 * the round keys needed for decryption and stores them in the provided
 * array. It is essential to ensure that the keyBits parameter is one of
 * the supported AES key sizes (128, 192, or 256 bits). The function
 * returns the number of rounds, which is determined by the key size.
 * This function must be called before performing decryption operations
 * with the Rijndael cipher.
 *
 * @param rk An array of unsigned 32-bit integers where the decryption round
 * keys will be stored. The array must have space for 4*(Nr + 1)
 * elements, where Nr is the number of rounds determined by the key
 * size.
 * @param cipherKey A pointer to an array of unsigned 8-bit integers
 * representing the cipher key. The length of this array should
 * correspond to the keyBits parameter (16 bytes for 128 bits,
 * 24 bytes for 192 bits, and 32 bytes for 256 bits). Must not
 * be null.
 * @param keyBits An integer specifying the length of the cipher key in bits.
 * Valid values are 128, 192, or 256. If an invalid value is
 * provided, the behavior is undefined.
 * @return Returns the number of rounds (Nr) used for the decryption process,
 * which depends on the key size.
 ******************************************************************************/
int rijndaelKeySetupDec(u32 rk[] /*4*(Nr + 1)*/, const u8 cipherKey[], int keyBits);
/***************************************************************************//**
 * @brief This function encrypts a 16-byte plaintext block using the Rijndael
 * cipher, which is the basis for the AES encryption standard. It
 * requires a precomputed round key schedule and the number of rounds to
 * perform the encryption. The function is typically used in
 * cryptographic applications where data confidentiality is required. The
 * caller must ensure that the round key schedule is correctly
 * initialized and that the plaintext and ciphertext buffers are properly
 * allocated and distinct to avoid data corruption.
 *
 * @param rk An array of unsigned 32-bit integers representing the round key
 * schedule. It must be precomputed using a key setup function and
 * have a size of at least 4*(Nr + 1). The caller retains ownership
 * and must ensure it is valid for the given number of rounds.
 * @param Nr An integer specifying the number of rounds to perform. It must be a
 * valid number of rounds for the given key schedule, typically 10,
 * 12, or 14, depending on the key size.
 * @param pt A pointer to a 16-byte array containing the plaintext block to be
 * encrypted. The array must be fully initialized, and the caller
 * retains ownership. It must not be null.
 * @param ct A pointer to a 16-byte array where the encrypted ciphertext will be
 * stored. The array must be allocated by the caller and must not
 * overlap with the plaintext array. The caller retains ownership.
 * @return None
 ******************************************************************************/
void rijndaelEncrypt(const u32 rk[] /*4*(Nr + 1)*/, int Nr, const u8 pt[16], u8 ct[16]);
/***************************************************************************//**
 * @brief This function decrypts a 16-byte block of ciphertext using the
 * Rijndael (AES) algorithm with a specified number of rounds. It
 * requires a precomputed round key schedule, which should be generated
 * using the rijndaelKeySetupDec function. The function is designed to
 * handle 128-bit blocks and expects the number of rounds to be
 * appropriate for the key size used during key setup. The decrypted
 * plaintext is written to the provided output buffer. Ensure that the
 * input ciphertext and output plaintext buffers are properly allocated
 * and that the round key schedule is correctly initialized before
 * calling this function.
 *
 * @param rk An array of unsigned 32-bit integers representing the round key
 * schedule. It must be precomputed using rijndaelKeySetupDec and
 * should have a size of 4*(Nr + 1). The caller retains ownership.
 * @param Nr An integer specifying the number of rounds to be used in the
 * decryption process. It must match the number of rounds used during
 * key setup, typically 10, 12, or 14 depending on the key size.
 * @param ct A pointer to a 16-byte array containing the ciphertext block to be
 * decrypted. Must not be null.
 * @param pt A pointer to a 16-byte array where the decrypted plaintext will be
 * stored. Must not be null and should be allocated by the caller.
 * @return None
 ******************************************************************************/
void rijndaelDecrypt(const u32 rk[] /*4*(Nr + 1)*/, int Nr, const u8 ct[16], u8 pt[16]);

#ifdef INTERMEDIATE_VALUE_KAT
/***************************************************************************//**
 * @brief This function is used to apply a specified number of encryption rounds
 * on a 16-byte data block using the Rijndael cipher, which is the basis
 * for AES. It is particularly useful for testing or intermediate value
 * analysis when full encryption is not required. The function requires a
 * precomputed round key schedule and the number of rounds to perform. It
 * modifies the input block in place, so the caller must ensure the block
 * contains valid data and that the round key schedule is correctly set
 * up for the specified number of rounds. The function does not perform
 * any validation on the input parameters, so incorrect usage may lead to
 * undefined behavior.
 *
 * @param rk An array of unsigned 32-bit integers representing the round key
 * schedule. It must be precomputed and contain at least 4*(Nr + 1)
 * elements. The caller retains ownership and must ensure it is
 * correctly initialized.
 * @param Nr An integer specifying the total number of rounds in the full
 * encryption process. It must be consistent with the key schedule
 * provided.
 * @param block A 16-byte array representing the data block to be encrypted. It
 * must contain valid data and will be modified in place. The
 * caller retains ownership.
 * @param rounds An integer specifying the number of encryption rounds to
 * perform. It must be less than or equal to Nr. If rounds equals
 * Nr, the function performs the final round transformation.
 * @return None
 ******************************************************************************/
void rijndaelEncryptRound(const u32 rk[] /*4*(Nr + 1)*/, int Nr, u8 block[16], int rounds);
/***************************************************************************//**
 * @brief This function is used to decrypt a block of data using a specified
 * number of rounds of the Rijndael cipher, which is the basis for AES.
 * It is intended for use in scenarios where intermediate decryption
 * results are needed, such as in testing or debugging. The function
 * requires a round key schedule, the total number of rounds in the
 * cipher, the data block to be decrypted, and the number of rounds to
 * perform. The function modifies the input block in place, applying the
 * decryption process up to the specified number of rounds. It is
 * important to ensure that the round key schedule and the number of
 * rounds are correctly set up before calling this function.
 *
 * @param rk An array of unsigned 32-bit integers representing the round key
 * schedule. It must be precomputed and contain 4*(Nr + 1) elements.
 * The caller retains ownership and must ensure it is not null.
 * @param Nr An integer specifying the total number of rounds in the cipher. It
 * must be a positive integer and typically corresponds to the AES
 * standard (10, 12, or 14).
 * @param block An array of 16 unsigned 8-bit integers representing the data
 * block to be decrypted. The function modifies this array in
 * place. It must not be null.
 * @param rounds An integer specifying the number of decryption rounds to
 * perform. It must be less than or equal to Nr. If it is greater
 * than Nr, only Nr rounds are performed.
 * @return None
 ******************************************************************************/
void rijndaelDecryptRound(const u32 rk[] /*4*(Nr + 1)*/, int Nr, u8 block[16], int rounds);

#endif /* INTERMEDIATE_VALUE_KAT */

#endif /* __RIJNDAEL_ALG_FST_H */
