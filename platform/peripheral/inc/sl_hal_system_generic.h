/***************************************************************************//**
 * @file
 * @brief System API (Generic)
 *******************************************************************************
 * # License
 * <b>Copyright 2024 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef _SL_HAL_SYSTEM_GENERIC_H
#define _SL_HAL_SYSTEM_GENERIC_H

#include "sl_enum.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup system SYSTEM - System Utils
 * @{
 ******************************************************************************/

/*******************************************************************************
 ********************************   ENUMS   ************************************
 ******************************************************************************/

/// Family security capability.
SL_ENUM(sl_hal_system_security_capability_t) {
  /// Unknown security capability.
  SL_SYSTEM_SECURITY_CAPABILITY_UNKNOWN,
  /// Security capability not applicable.
  SL_SYSTEM_SECURITY_CAPABILITY_NA,
  /// Basic security capability.
  SL_SYSTEM_SECURITY_CAPABILITY_BASIC,
  /// Root of Trust security capability.
  SL_SYSTEM_SECURITY_CAPABILITY_ROT,
  /// Secure Element security capability.
  SL_SYSTEM_SECURITY_CAPABILITY_SE,
  /// Secure Vault security capability.
  SL_SYSTEM_SECURITY_CAPABILITY_VAULT
};

/// Floating point co-processor access modes.
SL_ENUM_GENERIC(sl_hal_system_fpu_access_t, uint32_t) {
  /// Access denied, any attempted access generates a NOCP UsageFault.
  SL_SYSTEM_FPU_ACCESS_DENIED          = (0x0 << 20),
  /// Privileged access only, an unprivileged access generates a NOCP UsageFault.
  SL_SYSTEM_FPU_ACCESS_PRIVILEGED_ONLY = (0x5 << 20),
  /// Reserved.
  SL_SYSTEM_FPU_ACCESS_RESERVED        = (0xA << 20),
  /// Full access.
  SL_SYSTEM_FPU_ACCESS_FULL            = (0xF << 20)
};

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/// Chip revision details.
/***************************************************************************//**
 * @brief The `sl_hal_system_chip_revision_t` structure is used to store
 * information about the revision of a chip, including its minor and
 * major revision numbers, part number, and family number. This structure
 * is essential for identifying the specific version and family of a
 * chip, which can be crucial for compatibility and functionality
 * purposes. The part number and family fields can be set to 0xFFFF if
 * the information is unavailable, allowing for flexibility in handling
 * different chip configurations.
 *
 * @param minor Minor revision number of the chip.
 * @param major Major revision number of the chip.
 * @param part_number Device part number, with a value of 0xFFFF indicating
 * unavailability.
 * @param family Device family number, with a value of 0xFFFF indicating
 * unavailability.
 ******************************************************************************/
typedef struct {
  uint8_t minor;        ///< Minor revision number.
  uint8_t major;        ///< Major revision number.
  uint16_t part_number; ///< Device part number. (0xFFFF if unavailable)
  uint16_t family;      ///< Device family number. (0xFFFF if unavailable)
} sl_hal_system_chip_revision_t;

/// ADC Calibration DEVINFO Structures.
/***************************************************************************//**
 * @brief The `sl_hal_system_devinfo_adc_cal_data_t` structure is used to store
 * calibration data for an ADC (Analog-to-Digital Converter) within a
 * system. It contains four fields, each of which is an 8-bit unsigned
 * integer representing specific calibration trim values for different
 * settings of the ADC. These settings include VROS0, VROS1, 4x gain, and
 * 0x3 internal gain, which are crucial for ensuring accurate ADC
 * performance by compensating for any inherent inaccuracies in the
 * hardware.
 *
 * @param trim_vros0 Represents a calibration trim value for the VROS0 setting.
 * @param trim_vros1 Represents a calibration trim value for the VROS1 setting.
 * @param trim_gain_4x Represents a calibration trim value for the 4x gain
 * setting.
 * @param trim_gain_0x3_int Represents a calibration trim value for the 0x3
 * internal gain setting.
 ******************************************************************************/
typedef struct sl_hal_system_devinfo_adc_cal_data_t {
  uint8_t trim_vros0;
  uint8_t trim_vros1;
  uint8_t trim_gain_4x;
  uint8_t trim_gain_0x3_int;
} sl_hal_system_devinfo_adc_cal_data_t;

/***************************************************************************//**
 * @brief The `sl_hal_system_devinfo_adc_offset_t` structure is used to store
 * ADC offset calibration data for different gain settings (1x, 2x, and
 * 4x). Each member of the structure holds an 8-bit unsigned integer
 * representing the trim value for a specific gain setting. The
 * `dummy_byte` is included, likely for alignment purposes or as a
 * reserved space for future expansion.
 *
 * @param trim_off_1x Represents the ADC offset trim value for 1x gain.
 * @param trim_off_2x Represents the ADC offset trim value for 2x gain.
 * @param trim_off_4x Represents the ADC offset trim value for 4x gain.
 * @param dummy_byte A placeholder byte, possibly for alignment or future use.
 ******************************************************************************/
typedef struct sl_hal_system_devinfo_adc_offset_t {
  uint8_t trim_off_1x;
  uint8_t trim_off_2x;
  uint8_t trim_off_4x;
  uint8_t dummy_byte;
} sl_hal_system_devinfo_adc_offset_t;

/***************************************************************************//**
 * @brief The `sl_hal_system_devinfo_adc_t` structure is designed to encapsulate
 * ADC (Analog-to-Digital Converter) device information, specifically
 * focusing on calibration and offset data. It comprises two main
 * components: `cal_data`, which stores calibration parameters such as
 * trim values for voltage references and gain, and `offset`, which
 * includes offset trim values and a placeholder byte. This structure is
 * essential for ensuring accurate ADC operations by providing necessary
 * calibration and offset adjustments.
 *
 * @param cal_data Holds calibration data for the ADC, including various trim
 * values.
 * @param offset Contains offset data for the ADC, including trim offsets and a
 * dummy byte.
 ******************************************************************************/
typedef struct sl_hal_system_devinfo_adc_t {
  sl_hal_system_devinfo_adc_cal_data_t cal_data;
  sl_hal_system_devinfo_adc_offset_t   offset;
} sl_hal_system_devinfo_adc_t;

/// Temperature DEVINFO Structure.
/***************************************************************************//**
 * @brief The `sl_hal_system_devinfo_temperature_t` structure is used to store
 * temperature-related information for a device. It contains two members:
 * `emu_temp_room`, which holds the temperature reading under room
 * conditions, and `cal_temp`, which holds a calibrated temperature
 * value. This structure is likely used in systems where temperature
 * monitoring and calibration are critical for device operation and
 * performance.
 *
 * @param emu_temp_room Represents the temperature reading at room conditions.
 * @param cal_temp Represents the calibrated temperature value.
 ******************************************************************************/
typedef struct sl_hal_system_devinfo_temperature_t {
  uint16_t emu_temp_room;
  uint16_t cal_temp;
} sl_hal_system_devinfo_temperature_t;

/// Chip features Structure.
/***************************************************************************//**
 * @brief The `sl_hal_system_features_t` structure is a simple data structure
 * used to represent three character-based features of a system. Each
 * feature is stored as a `char`, allowing for the representation of
 * small, character-based system attributes or flags. This structure is
 * likely used to encapsulate and manage system features in a compact
 * form, possibly for configuration or identification purposes within the
 * system's hardware abstraction layer.
 *
 * @param feature1 Represents a character feature of the system.
 * @param feature2 Represents another character feature of the system.
 * @param feature3 Represents a third character feature of the system.
 ******************************************************************************/
typedef struct sl_hal_system_features {
  char feature1;
  char feature2;
  char feature3;
} sl_hal_system_features_t;

/*******************************************************************************
 **************************   GLOBAL CONSTANTS   *******************************
 ******************************************************************************/

extern const sl_hal_system_devinfo_adc_t SL_HAL_SYSTEM_DEVINFO_ADC_RESET_VALUES;

extern const sl_hal_system_devinfo_temperature_t SL_HAL_SYSTEM_DEVINFO_TEMPERATURE_RESET_VALUES;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief This function is used to obtain the chip revision details, including
 * the major and minor revision numbers, as well as the part number and
 * family identifier if available. It is essential to call this function
 * when you need to identify the specific revision of the chip being
 * used, which can be critical for compatibility and feature support. The
 * function populates the provided structure with the relevant data,
 * setting fields to 0xFFFF if certain information is not available for
 * the device.
 *
 * @param rev A pointer to an sl_hal_system_chip_revision_t structure where the
 * chip revision information will be stored. The caller must ensure
 * this pointer is valid and points to a writable memory location.
 * The structure will be populated with the major and minor revision
 * numbers, and either the part number or family identifier,
 * depending on the device. If certain information is unavailable,
 * the corresponding fields will be set to 0xFFFF.
 * @return None
 ******************************************************************************/
void sl_hal_system_get_chip_revision(sl_hal_system_chip_revision_t *rev);

/***************************************************************************//**
 * @brief This function is used to obtain the revision number of the device
 * information (DEVINFO) structure. It is typically called when there is
 * a need to verify or log the specific revision of the device's
 * information for compatibility or diagnostic purposes. The function
 * does not require any parameters and will return a valid revision
 * number if the device supports this feature. If the revision
 * information is not defined for the device, the function will not
 * compile, as indicated by a preprocessor error directive.
 *
 * @return The function returns an 8-bit unsigned integer representing the
 * DEVINFO revision.
 ******************************************************************************/
uint8_t sl_hal_system_get_devinfo_rev(void);

/***************************************************************************//**
 * @brief This function is used to obtain the default factory calibration value
 * for the High-Frequency RC Oscillator (HFRCO) on the device. It is
 * typically called when initializing or configuring the HFRCO to ensure
 * it operates with the correct calibration settings. The function
 * returns a default calibration value if available; otherwise, it
 * returns zero. This function does not require any parameters and can be
 * called at any time to retrieve the calibration value.
 *
 * @return Returns the default factory calibration value for the HFRCO
 * oscillator as a 32-bit unsigned integer. If the calibration value is
 * not available, it returns zero.
 ******************************************************************************/
uint32_t sl_hal_system_get_hfrco_default_calibration(void);

/***************************************************************************//**
 * @brief This function is used to obtain the factory calibration value for the
 * High-Frequency RC Oscillator (HFRCO) that is specific to its speed
 * setting. It is useful in scenarios where precise oscillator
 * calibration is required for optimal performance. The function should
 * be called when the calibration value is needed for configuration or
 * diagnostic purposes. If the calibration information is not available,
 * the function will return 0, indicating that the calibration value
 * could not be retrieved.
 *
 * @return Returns the HFRCO speed calibration value as a 32-bit unsigned
 * integer, or 0 if the calibration value is unavailable.
 ******************************************************************************/
uint32_t sl_hal_system_get_hfrco_speed_calibration(void);

/***************************************************************************//**
 * @brief This function is used to obtain the calibration value for the High-
 * Frequency RC Oscillator (HFRCO) when used with the Digital Phase-
 * Locked Loop (DPLL) module, based on a specified frequency. It is
 * intended for use in systems where precise frequency calibration is
 * necessary. The function should be called with a valid frequency value
 * that falls within the supported range of the HFRCO DPLL frequency
 * table. If the frequency is not within a valid range or if any error
 * occurs during the retrieval process, the function returns 0,
 * indicating failure to obtain a valid calibration value.
 *
 * @param frequency The frequency for which to retrieve the calibration value.
 * It must be within the range supported by the HFRCO DPLL
 * frequency table. If the frequency is outside this range, the
 * function returns 0.
 * @return Returns the calibration value for the specified frequency band if
 * successful, or 0 if the frequency is invalid or an error occurs.
 ******************************************************************************/
uint32_t sl_hal_system_get_hfrcodpll_band_calibration(uint32_t frequency);

/***************************************************************************//**
 * @brief This function is used to obtain the factory calibration value for the
 * HFRCOEM23 oscillator based on a specified frequency. It is essential
 * for ensuring the oscillator operates correctly at the desired
 * frequency. The function should be called with a valid frequency value,
 * typically 40 MHz, to retrieve the corresponding calibration value. If
 * the frequency is not supported or if an error occurs during the
 * retrieval process, the function returns 0. This function is only
 * applicable for devices with specific configurations, and it is
 * important to ensure compatibility before use.
 *
 * @param frequency The frequency for which to retrieve the HFRCOEM23
 * calibration value. Valid input is typically 40 MHz. If an
 * unsupported frequency is provided, the function defaults to
 * a standard calibration value.
 * @return Returns the HFRCOEM23 calibration value for the specified frequency.
 * If the frequency is unsupported or an error occurs, returns 0.
 ******************************************************************************/
uint32_t sl_hal_system_get_hfrcoem23_calibration(uint32_t frequency);

/***************************************************************************//**
 * @brief This function is used to obtain the factory calibration value for the
 * High-Frequency Crystal Oscillator (HFXO) of the system. It is useful
 * in scenarios where precise oscillator calibration is required for
 * optimal performance. The function should be called when the
 * calibration value is needed, and it will return a default value of 0
 * if the calibration information is not available on the device. This
 * function does not require any parameters and is safe to call at any
 * time.
 *
 * @return Returns the HFXO factory calibration value as a 32-bit unsigned
 * integer, or 0 if the calibration information is unavailable.
 ******************************************************************************/
uint32_t sl_hal_system_get_hfxocal(void);

/***************************************************************************//**
 * @brief This function is used to determine the security capability of the
 * microcontroller unit (MCU) based on its device number. It should be
 * called when there is a need to understand the security features
 * available on the MCU, such as Secure Element, Secure Vault, or Root of
 * Trust. The function does not require any parameters and returns an
 * enumeration indicating the security capability. It is important to
 * note that the function may return an unknown capability if the device
 * number does not match any known configurations.
 *
 * @return Returns a value of type `sl_hal_system_security_capability_t`
 * indicating the security capability of the MCU.
 ******************************************************************************/
sl_hal_system_security_capability_t sl_hal_system_get_security_capability(void);

/***************************************************************************//**
 * @brief This function is used to obtain a unique 64-bit identifier for the
 * device, which can be useful for distinguishing between different
 * devices in a system. It should be called whenever a unique device
 * identifier is required, such as for device registration or tracking
 * purposes. The function does not require any initialization or
 * configuration before being called, and it does not have any side
 * effects.
 *
 * @return A 64-bit unsigned integer representing the unique identifier for the
 * device.
 ******************************************************************************/
uint64_t sl_hal_system_get_unique(void);

/***************************************************************************//**
 * @brief This function is used to obtain the production revision number of the
 * current device. It is useful for applications that need to adapt their
 * behavior based on the specific production revision of the hardware.
 * The function does not require any parameters and returns a single byte
 * representing the production revision. It should be called when the
 * production revision information is needed, and it assumes that the
 * device's production revision data is accessible through the defined
 * macros. If the location of the production revision is not defined, the
 * function will not compile.
 *
 * @return A uint8_t value representing the production revision of the device.
 ******************************************************************************/
uint8_t sl_hal_system_get_prod_rev(void);

/***************************************************************************//**
 * @brief This function provides the base address of the Static Random-Access
 * Memory (SRAM) for the system. It is useful when you need to perform
 * operations that require knowledge of the memory layout, such as direct
 * memory access or memory-mapped peripheral interactions. The function
 * does not require any parameters and can be called at any time to
 * obtain the current SRAM base address. It is a read-only operation and
 * does not modify any system state.
 *
 * @return Returns the base address of the SRAM as a 32-bit unsigned integer.
 ******************************************************************************/
uint32_t sl_hal_system_get_sram_base_address(void);

/***************************************************************************//**
 * @brief This function is used to obtain the size of the internal SRAM of the
 * device in kilobytes. It reads the device information structure to
 * determine the SRAM size. This function is particularly useful when the
 * binary is intended to run on multiple devices, as it dynamically
 * retrieves the SRAM size rather than relying on a hardcoded value. It
 * should be noted that this function assumes the presence of specific
 * device information macros, and if these are not defined, the function
 * will not compile.
 *
 * @return Returns the size of the internal SRAM in kilobytes as a 16-bit
 * unsigned integer.
 ******************************************************************************/
uint16_t sl_hal_system_get_sram_size(void);

/***************************************************************************//**
 * @brief This function is used to obtain the size of the flash memory available
 * on the device, expressed in kilobytes. It is useful for applications
 * that need to manage memory usage or perform operations based on the
 * available flash size. The function reads the device information to
 * determine the flash size, which may include internal or external flash
 * configurations. It is important to call this function when accurate
 * flash size information is required for device-specific operations.
 *
 * @return Returns the size of the flash memory in kilobytes as a 16-bit
 * unsigned integer.
 ******************************************************************************/
uint16_t sl_hal_system_get_flash_size(void);

/***************************************************************************//**
 * @brief This function provides the size of a flash memory page in bytes, which
 * is essential for operations involving flash memory management, such as
 * erasing or writing data. It should be used when the flash page size is
 * required for calculations or configurations. The function does not
 * require any parameters and handles different configurations
 * internally, returning the appropriate page size based on the device's
 * memory information or external configuration.
 *
 * @return The function returns a 32-bit unsigned integer representing the flash
 * page size in bytes.
 ******************************************************************************/
uint32_t sl_hal_system_get_flash_page_size(void);

/***************************************************************************//**
 * @brief This function is used to obtain the part number of the microcontroller
 * unit (MCU) from the device information registers. It is typically
 * called when there is a need to identify the specific part number of
 * the MCU for purposes such as diagnostics, logging, or configuration.
 * The function does not require any parameters and returns a 16-bit
 * unsigned integer representing the part number. It is important to
 * ensure that the device information registers are correctly configured
 * and accessible before calling this function.
 *
 * @return A 16-bit unsigned integer representing the MCU part number.
 ******************************************************************************/
uint16_t sl_hal_system_get_part_number(void);

/***************************************************************************//**
 * @brief This function is used to obtain the features of the current System on
 * Chip (SoC) or Microcontroller Unit (MCU). It returns a structure
 * containing three feature identifiers, which can be decoded by
 * referring to the SoC or MCU datasheet. This function is useful for
 * applications that need to adapt their behavior based on the specific
 * capabilities of the hardware. It should be called when the feature set
 * of the device needs to be known, and it assumes that the device is
 * supported by the function. If the device is not supported, the
 * function will not compile due to a preprocessor error.
 *
 * @return Returns a structure of type `sl_hal_system_features_t` containing
 * three feature identifiers as characters.
 ******************************************************************************/
sl_hal_system_features_t sl_hal_system_get_part_features(void);

/***************************************************************************//**
 * @brief This function is used to obtain temperature-related information from
 * the device's DEVINFO structure. It populates the provided structure
 * with calibration and room temperature data. This function should be
 * called when temperature information is needed for device operations or
 * diagnostics. The caller must ensure that the provided pointer is valid
 * and points to a structure capable of holding the temperature data. The
 * function does not return a value, but it modifies the contents of the
 * structure pointed to by the input parameter.
 *
 * @param info A pointer to a sl_hal_system_devinfo_temperature_t structure
 * where the temperature information will be stored. The pointer
 * must not be null, and the caller retains ownership of the memory.
 * If the function encounters an error, the structure will be set to
 * default reset values.
 * @return None
 ******************************************************************************/
void sl_hal_system_get_temperature_info(sl_hal_system_devinfo_temperature_t *info);

/***************************************************************************//**
 * @brief This function configures the access mode for the floating point co-
 * processor (FPU) in the system. It should be used when there is a need
 * to change the FPU access permissions, such as enabling or restricting
 * access based on the application's requirements. The function must be
 * called with a valid access mode from the `sl_hal_system_fpu_access_t`
 * enumeration. It is important to ensure that the system is in a safe
 * state to change FPU access, as incorrect settings may lead to usage
 * faults.
 *
 * @param access_mode Specifies the desired access mode for the FPU. Must be one
 * of the values defined in the `sl_hal_system_fpu_access_t`
 * enumeration, such as `SL_SYSTEM_FPU_ACCESS_DENIED`,
 * `SL_SYSTEM_FPU_ACCESS_PRIVILEGED_ONLY`,
 * `SL_SYSTEM_FPU_ACCESS_RESERVED`, or
 * `SL_SYSTEM_FPU_ACCESS_FULL`. Invalid values may result in
 * undefined behavior.
 * @return None
 ******************************************************************************/
void sl_hal_system_fpu_set_access_mode(sl_hal_system_fpu_access_t access_mode);

/***************************************************************************//**
 * @brief This function is used to obtain the ADC calibration information for
 * the device. It should be called when ADC calibration data is required,
 * typically during system initialization or when recalibrating the ADC.
 * The function populates the provided structure with calibration data,
 * or sets it to default reset values if the data cannot be retrieved. It
 * is important to ensure that the pointer provided is valid and not null
 * before calling this function.
 *
 * @param info A pointer to an sl_hal_system_devinfo_adc_t structure where the
 * ADC calibration information will be stored. Must not be null. If
 * the function fails to retrieve the calibration data, this
 * structure will be set to default reset values.
 * @return None
 ******************************************************************************/
void sl_hal_system_get_adc_calibration_info(sl_hal_system_devinfo_adc_t *info);

/** @} (end addtogroup system) */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _SL_HAL_SYSTEM_GENERIC_H */
