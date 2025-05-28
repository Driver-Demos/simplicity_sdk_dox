/***************************************************************************//**
 * @file
 * @brief See @ref cbh_app for detailed documentation.
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

#ifndef __BOOTLOADER_EEPROM_H__
#define __BOOTLOADER_EEPROM_H__
#include <stdint.h>

/***************************************************************************//**
 * @addtogroup legacyhal
 * @{
 ******************************************************************************/

/** @brief Definition of an EEPROM page size, in bytes.  This definition is
 *  deprecated, and should no longer be used.
 */
#define EEPROM_PAGE_SIZE      (128ul)

/** @brief Define the location of the first page in EEPROM.  This definition is
 *  deprecated, and should no longer be used.
 */
#define EEPROM_FIRST_PAGE   (0)

/** @brief Define the location of the image start in EEPROM as a function of
 * the ::EEPROM_FIRST_PAGE and ::EEPROM_PAGE_SIZE.  This definition is
 * deprecated, and should no longer be used.
 */
#define EEPROM_IMAGE_START  (EEPROM_FIRST_PAGE * EEPROM_PAGE_SIZE)

/** @brief Define EEPROM success status.
 */
#define EEPROM_SUCCESS 0U

/** @brief Define EEPROM error status.
 */
#define EEPROM_ERR 1U

/** @brief Define EEPROM error mask.
 */
#define EEPROM_ERR_MASK 0x80U

/** @brief Define EEPROM page boundary error.
 */
#define EEPROM_ERR_PG_BOUNDARY 0x81U

/** @brief Define EEPROM page size error.
 */
#define EEPROM_ERR_PG_SZ 0x82U

/** @brief Define EEPROM write data error.
 */
#define EEPROM_ERR_WRT_DATA 0x83U

/** @brief Define EEPROM image too large error.
 */
#define EEPROM_ERR_IMG_SZ 0x84U

/** @brief Define EEPROM invalid address error.
 */
#define EEPROM_ERR_ADDR 0x85U

/** @brief Define EEPROM chip initialization error.
 */
#define EEPROM_ERR_INVALID_CHIP 0x86U

/** @brief Define EEPROM erase required error.
 */
#define EEPROM_ERR_ERASE_REQUIRED 0x87U

/** @brief Define EEPROM error for no erase support.
 */
#define EEPROM_ERR_NO_ERASE_SUPPORT 0x88U

/***************************************************************************//**
 * @brief The `HalEepromInformationType` structure provides detailed information
 * about an external EEPROM device, including its version, capabilities,
 * and timing characteristics. It includes fields for the size of
 * erasable pages and the entire EEPROM, as well as a description of the
 * EEPROM and the word size in bytes. This structure is essential for
 * understanding the operational parameters and capabilities of the
 * EEPROM, facilitating its integration and use in applications.
 *
 * @param version The version of this data structure.
 * @param capabilitiesMask A bitmask describing the capabilities of this
 * particular external EEPROM.
 * @param pageEraseMs Maximum time it takes to erase a page, measured in 1024Hz
 * milliseconds.
 * @param partEraseTime Maximum time it takes to erase the entire part, measured
 * in 1024Hz milliseconds or seconds.
 * @param pageSize The size of a single erasable page in bytes.
 * @param partSize The total size of the external EEPROM in bytes.
 * @param partDescription Pointer to a string describing the attached external
 * EEPROM.
 * @param wordSizeBytes The number of bytes in a word for the external EEPROM.
 ******************************************************************************/
typedef struct {
  /** The version of this data structure */
  uint32_t version;

  /** A bitmask describing the capabilites of this particular external EEPROM */
  uint16_t capabilitiesMask;

  /** Maximum time it takes to erase a page. (in 1024Hz Milliseconds) */
  uint16_t pageEraseMs;

  /** Maximum time it takes to erase the entire part. (in 1024Hz Milliseconds).
      Can be changed to be in seconds using EEPROM_CAPABILITIES_PART_ERASE_SECONDS */
  uint16_t partEraseTime;

  /** The size of a single erasable page in bytes */
  uint32_t pageSize;

  /** The total size of the external EEPROM in bytes */
  uint32_t partSize;

  /** Pointer to a string describing the attached external EEPROM */
  char * partDescription;

  /** The number of bytes in a word for the external EEPROM **/
  uint8_t wordSizeBytes;
} HalEepromInformationType;

/** @brief The current version of the ::HalEepromInformationType data structure
 */
#define EEPROM_INFO_VERSION             (0x0202)
#define EEPROM_INFO_MAJOR_VERSION       (0x0200)   ///< eeprom info major version
#define EEPROM_INFO_MAJOR_VERSION_MASK  (0xFF00)   ///< eeprom info major version mask
// ***  Eeprom info version history: ***
// 0x0202 - Changed partEraseMs to be called partEraseTime and added an
//          EEPROM_CAPABILITIES field to indicate if partEraseTime is in
//          seconds or milliseconds.
// 0x0102 - Added a word size field to specify the number of bytes per flash
//          word in the EEPROM. Writes should always be aligned to the word
//          size and have a length that is a multiple of the word size.
// 0x0101 - Initial version
#define EEPROM_INFO_MIN_VERSION_WITH_WORD_SIZE_SUPPORT 0x0102U                                       ///< eeprom info min version with word size support

/** @brief Eeprom capabilites mask that indicates the erase API is supported
 */
#define EEPROM_CAPABILITIES_ERASE_SUPPORTED   (0x0001U)

/** @brief Eeprom capabilites mask that indicates page erasing is required
 *          before new data can be written to a device
 */
#define EEPROM_CAPABILITIES_PAGE_ERASE_REQD   (0x0002U)

/** @brief Eeprom capabilites mask that indicates that the write routine
 *          is blocking on this device
 */
#define EEPROM_CAPABILITIES_BLOCKING_WRITE    (0x0004U)

/** @brief Eeprom capabilites mask that indicates that the erase routine
 *          is blocking on this device
 */
#define EEPROM_CAPABILITIES_BLOCKING_ERASE    (0x0008U)

/** @brief Eeprom capabilities mask that indicateds that the partEraseTime
 *          field of HalEepromInformationType is in seconds instead of
 *          the usual millisecondss.
 */
#define EEPROM_CAPABILITIES_PART_ERASE_SECONDS (0x0010U)

/***************************************************************************//**
 * @brief The `halEepromInfo` function returns a pointer to a
 * `HalEepromInformationType` structure, which contains detailed
 * information about the attached external EEPROM device. This includes
 * versioning, capabilities, timing, size, and description of the EEPROM.
 *
 * @details This function is used to retrieve and provide information about the
 * external EEPROM's characteristics and capabilities.
 ******************************************************************************/
const HalEepromInformationType *halEepromInfo(void);

/***************************************************************************//**
 * @brief The EepromStateType structure is designed to maintain the state of an
 * EEPROM operation, including the current address, the number of pages,
 * and buffer management details. It includes fields for tracking the
 * current position within a page buffer and the length of data in the
 * buffer, as well as the buffer itself, which is used to temporarily
 * store data during EEPROM operations.
 *
 * @param address A 32-bit unsigned integer representing the EEPROM address.
 * @param pages A 16-bit unsigned integer indicating the number of pages.
 * @param pageBufFinger A 16-bit unsigned integer used as a pointer or index
 * within the page buffer.
 * @param pageBufLen A 16-bit unsigned integer representing the length of the
 * page buffer.
 * @param pageBuf An array of bytes with size EEPROM_PAGE_SIZE used as a buffer
 * for page data.
 ******************************************************************************/
typedef struct {
  uint32_t address;                                ///< address
  uint16_t pages;                                  ///< pages
  uint16_t pageBufFinger;                          ///< page Buf Finger
  uint16_t pageBufLen;                             ///< page Buf Len
  uint8_t pageBuf[EEPROM_PAGE_SIZE];               ///< pageBuf[EEPROM_PAGE_SIZE]
} EepromStateType;

/** @} (end addtogroup legacyhal) */

#endif //__BOOTLOADER_EEPROM_H__
