/*************************************************************************/ /**
* @file
* @brief CPC Journal
******************************************************************************
* # License
* <b>Copyright 2023 Silicon Laboratories Inc. www.silabs.com</b>
******************************************************************************
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

#ifndef SL_CPC_JOURNAL_H
#define SL_CPC_JOURNAL_H

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************/ /**
 * @addtogroup cpc CPC Journal
 * @{
 *****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include "sl_enum.h"
#include "sl_cpc_journal_config.h"

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif

#if defined(SL_CATALOG_CPC_JOURNAL_CLI_PRESENT)
#include "sl_cli.h"
#endif

#if defined(SL_CATALOG_IOSTREAM_PRESENT)
#include "sl_iostream.h"
#endif

/**
 * @brief Record a CPC journal entry.
 *
 * @param[in] string The message string to record.
 * @param[in] value The value associated with the message.
 */
#define SL_CPC_JOURNAL_RECORD_ERROR(string, value) \
  sl_cpc_journal_record(SL_CPC_JOURNAL_ERROR_LEVEL, string, value)
#define SL_CPC_JOURNAL_RECORD_WARNING(string, value) \
  sl_cpc_journal_record(SL_CPC_JOURNAL_WARNING_LEVEL, string, value)
#define SL_CPC_JOURNAL_RECORD_INFO(string, value) \
  sl_cpc_journal_record(SL_CPC_JOURNAL_INFO_LEVEL, string, value)
#define SL_CPC_JOURNAL_RECORD_DEBUG(string, value) \
  sl_cpc_journal_record(SL_CPC_JOURNAL_DEBUG_LEVEL, string, value)

/// @brief Enumeration of journal entry levels.
SL_ENUM_GENERIC(sl_cpc_journal_level_t, uint8_t){
  SL_CPC_JOURNAL_ERROR_LEVEL = 0x00,
  SL_CPC_JOURNAL_WARNING_LEVEL = 0x40,
  SL_CPC_JOURNAL_INFO_LEVEL = 0x80,
  SL_CPC_JOURNAL_DEBUG_LEVEL = 0xC0,
  SL_CPC_JOURNAL_TRACE_LEVEL = 0xC0
};

/***************************************************************************//**
 * @brief This function prepares the CPC Journal for use by setting up its
 * internal circular buffers and configuring the maximum number of
 * entries it can hold. It is essential to call this function before any
 * journal entries are recorded to ensure that the journal operates
 * correctly. The function also initializes internal state variables used
 * for tracking journal entries. It is typically called once during the
 * system initialization phase.
 *
 * @return None
 ******************************************************************************/
void sl_cpc_journal_init(void);

/***************************************************************************//**
 * @brief This function is used to log a journal entry with a given severity
 * level, message string, and associated value. It is intended for
 * internal use within the CPC Journal module to record events or
 * messages that occur during the operation of the system. The function
 * should be called whenever there is a need to document an event with a
 * specific level of importance, such as an error, warning, informational
 * message, or debug information. The function manages the circular
 * buffer to ensure that new entries are recorded, and older entries are
 * overwritten if the buffer is full. It is important to ensure that the
 * journal module is initialized before calling this function.
 *
 * @param level The severity level of the journal entry, which must be one of
 * the predefined levels: Error, Warning, Info, or Debug. The level
 * determines which circular buffer the entry is recorded in.
 * @param string A pointer to a null-terminated string containing the message to
 * log. The string must not be null, and the caller retains
 * ownership of the string.
 * @param value A 32-bit unsigned integer representing a value associated with
 * the message. This value is logged alongside the message string.
 * @return None
 ******************************************************************************/
void sl_cpc_journal_record_internal(sl_cpc_journal_level_t level, const char *string, uint32_t value);

/***************************************************************************//**
 * @brief The `sl_cpc_journal_record` function conditionally logs a journal
 * entry based on the specified log level.
 *
 * @param level The severity level of the journal entry, which can be Error,
 * Warning, Info, or Debug.
 * @param string A message string that describes the journal entry.
 * @param value An associated numerical value for the journal entry.
 * @return The function does not return any value; it performs an action of
 * logging a journal entry if the condition is met.
 ******************************************************************************/
static inline void sl_cpc_journal_record(sl_cpc_journal_level_t level, const char *string, uint32_t value)
{
  if (level <= SL_CPC_JOURNAL_LEVEL) {
    sl_cpc_journal_record_internal(level, string, value);
  }
}

#if defined(SL_CATALOG_IOSTREAM_PRESENT) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief Use this function to output the current contents of the CPC journal to
 * a specified IOStream, optionally including a CSV header. This function
 * is useful for logging or debugging purposes, allowing you to review
 * the recorded journal entries. It consumes the journal entries as they
 * are printed, meaning they will no longer be available after this
 * function is called. Ensure that the IOStream is properly initialized
 * and available before calling this function.
 *
 * @param print_csv_header A boolean flag indicating whether to print a CSV
 * header before the journal entries. If true, a header
 * is printed; otherwise, it is omitted.
 * @param stream A pointer to an sl_iostream_t object representing the IOStream
 * to which the journal entries will be printed. Must not be null,
 * and the stream should be properly initialized and ready for
 * use.
 * @return None
 ******************************************************************************/
void sl_cpc_journal_print(bool print_csv_header, sl_iostream_t *stream);
#endif

#if defined(SL_CATALOG_CPC_JOURNAL_CLI_PRESENT) || defined(DOXYGEN)
/***************************************************************************//**
 * @brief This function is used to handle a command-line interface (CLI) command
 * that prints the contents of the CPC journal. It should be called when
 * a user wants to output the journal entries via the CLI. The function
 * consumes the journal entries as they are printed, meaning they will no
 * longer be available after this operation. It is important to ensure
 * that the `arguments` parameter is correctly initialized and contains a
 * valid IOStream handle before calling this function.
 *
 * @param arguments A pointer to an `sl_cli_command_arg_t` structure containing
 * the CLI arguments. This must include a valid IOStream handle
 * in `arguments->handle->iostream_handle`. The parameter must
 * not be null, and the caller retains ownership.
 * @return None
 ******************************************************************************/
void sl_cpc_journal_print_cmd_handler(sl_cli_command_arg_t *arguments);
#endif

#ifdef __cplusplus
}
#endif

#endif // SL_CPC_JOURNAL_H
