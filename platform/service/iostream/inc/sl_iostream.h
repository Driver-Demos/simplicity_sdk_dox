/***************************************************************************//**
 * @file
 * @brief IO Stream.
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef SL_IOSTREAM_H
#define SL_IOSTREAM_H

#if defined(SL_COMPONENT_CATALOG_PRESENT)
#include "sl_component_catalog.h"
#endif
#include "sl_enum.h"
#include "sl_status.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup iostream I/O Stream
 * @brief I/O Stream can be used to read/write different formats of data to various streams.
 *        The source files for I/O Stream platform software module are present under platform/services/iostream.
 * @details
 * ## Overview
 *
 *   I/O Stream is a platform module software that provides Input/Output functionalities
 *   by creating streams. Streams are abstractions allowing a uniform way to read/write
 *   data regardless of the physical communication interface.
 *
 *   I/O Stream offers many interfaces, see submodules for a list of all types available
 *   and their specificities.You can load multiple streams in the project and you can
 *   select the interface that must be used at runtime.Some interface type can also
 *   be instantiated, meaning that you can have multiple instances of an interface
 *   type which will be normally bound to a hardware peripheral.
 *
 * ## Initialization
 *
 *   The I/O Stream core doesn't require any initialization. Instead each stream type has
 *   their own initialization and their own configuration. See I/O Stream specific type
 *   to know more about how to initialize a stream.
 *
 *   Note that most stream will set itself as the default stream during their initialization.
 *   Thus the initial default stream will be the last stream initialized.
 *
 * ## Default system-wide stream
 *
 *   Multiple streams can be initialized in your application and you can configure a default
 *   stream that must be used when no stream is specified. Also note that the default stream
 *   will be used when calling printf and you can change the default stream at runtime.
 *   The following defines should be used for the default stream:
 *
 *      SL_IOSTREAM_STDIN
 *      SL_IOSTREAM_STDOUT
 *      SL_IOSTREAM_STDERR
 *
 * ## RTOS - Task's default stream
 *
 *   In the case of an RTOS environment, each task can set its own stream. By default, the task
 *   stream will be set to the system_wide default stream. From your task, you can change the
 *   default stream assigned to your task without affecting the other tasks' stream.
 *
 * ## Printf
 *
 *   I/O Stream provides third-party printf integrations. It can work with toolchain implementation
 *   or with the tiny printf implementation for embedded system. The printf API doesn't have an
 *   argument for specifying the stream to be used, so I/O Stream provides a printf API that takes
 *   a stream as an argument and calls the configured third-party implementation of printf.
 *
 * @{
 ******************************************************************************/

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
#define SL_IOSTREAM_STDIN 0  ///< Default input stream
#define SL_IOSTREAM_STDOUT 0 ///< Default output stream
#define SL_IOSTREAM_STDERR 0 ///< Default error stream
/// @endcond

// -----------------------------------------------------------------------------
// Data Types

/// @brief Struct representing iostream operations.
/***************************************************************************//**
 * @brief The `sl_iostream_t` structure is designed to represent an input/output
 * stream in a flexible and abstract manner. It contains a context
 * pointer for holding any necessary state or configuration specific to
 * the stream, and two function pointers, `write` and `read`, which are
 * used to perform the actual data transmission operations. This
 * abstraction allows for different types of streams to be implemented,
 * such as UART, SWO, or RTT, while using the same interface for reading
 * and writing data.
 *
 * @param context A pointer to a context-specific data structure used by the
 * stream.
 * @param write A function pointer for writing data to the stream.
 * @param read A function pointer for reading data from the stream.
 ******************************************************************************/
typedef struct {
  void *context;                                                                                ///< context
  sl_status_t (*write)(void *context, const void *buffer, size_t buffer_length);                ///< write
  sl_status_t (*read)(void *context, void *buffer, size_t buffer_length, size_t *bytes_read);   ///< read
} sl_iostream_t;

/// @brief Enumeration representing the possible types of iostream instances.
SL_ENUM(sl_iostream_type_t){
  SL_IOSTREAM_TYPE_SWO = 0,              ///< SWO Instance
  SL_IOSTREAM_TYPE_RTT = 1,              ///< RTT Instance
  SL_IOSTREAM_TYPE_UART = 2,             ///< USART Instance
  SL_IOSTREAM_TYPE_VUART = 3,            ///< Vuart
  SL_IOSTREAM_TYPE_DEBUG_OUTPUT = 4,     ///< Backchannel output Instance Type
  SL_IOSTREAM_TYPE_LOOPBACK = 5,         ///< Loopback Instance
  SL_IOSTREAM_TYPE_UNDEFINED = 6,        ///< Undefined Instance Type
};

/// @brief Struct representing an I/O Stream instance.
/***************************************************************************//**
 * @brief The `sl_iostream_instance_info_t` structure is used to encapsulate
 * information about an I/O stream instance in the Silicon Laboratories
 * platform. It includes a handle to the iostream, a name for
 * identification, the type of iostream as defined by an enumeration, a
 * peripheral ID for hardware association, and a function pointer for
 * initializing the iostream instance. This structure is essential for
 * managing and interacting with different I/O stream instances, allowing
 * for flexible and dynamic stream management in embedded systems.
 *
 * @param handle A pointer to the iostream instance handle.
 * @param name A pointer to a character array representing the iostream instance
 * name.
 * @param type An enumeration value representing the iostream instance type.
 * @param periph_id An 8-bit unsigned integer representing the iostream
 * peripheral ID.
 * @param init A pointer to a function that initializes the iostream instance.
 ******************************************************************************/
typedef struct {
  sl_iostream_t *handle;        ///< iostream instance handle.
  char *name;                   ///< iostream instance name.
  sl_iostream_type_t type;      ///< iostream instance type.
  uint8_t periph_id;            ///< iostream peripheral id.
  sl_status_t (*init)(void);    ///< iostream instance init function.
} sl_iostream_instance_info_t;

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
// Special stream to be used when you want to avoid printing anything
extern sl_iostream_t sl_iostream_null;
/// @endcond

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief Use this function to designate a specific I/O stream as the default
 * for the system. This is particularly useful when multiple streams are
 * available, and you want to specify which one should be used by default
 * for operations like printf. In an RTOS environment, this function also
 * ensures that the default stream is set for the current task, allowing
 * task-specific stream configurations. The function must be called with
 * a valid stream pointer, and it is expected to be used in environments
 * where the I/O stream infrastructure is already initialized.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream to set as default. Must not be null, and should point to
 * a valid, initialized stream instance.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success.
 ******************************************************************************/
sl_status_t sl_iostream_set_default(sl_iostream_t *stream);

/***************************************************************************//**
 * @brief This function returns the default I/O stream that is currently
 * configured in the system. It is useful when you need to perform I/O
 * operations without specifying a particular stream, as it provides a
 * convenient way to access the system-wide or task-specific default
 * stream. In an RTOS environment, it may return a task-specific stream
 * if one is set; otherwise, it defaults to the system-wide stream. This
 * function should be called when you need to interact with the default
 * stream, especially in contexts where the stream is not explicitly
 * provided.
 *
 * @return Returns a pointer to the default sl_iostream_t instance, or NULL if
 * no default stream is configured.
 ******************************************************************************/
sl_iostream_t *sl_iostream_get_default(void);

/***************************************************************************//**
 * Configure the systemwide default stream.
 *
 * @param[in] stream  I/O Stream to be used.
 *
 * @return  Status result
 ******************************************************************************/
#if defined(SL_CATALOG_KERNEL_PRESENT) && !defined(SL_IOSTREAM_FORCE_BAREMETAL)
/***************************************************************************//**
 * @brief This function sets the specified I/O stream as the system-wide default
 * stream, which will be used when no specific stream is provided for I/O
 * operations. It is typically used in environments where multiple
 * streams are available, and a default needs to be established for
 * general use. This function should be called when you want to change
 * the default stream at runtime, ensuring that the provided stream is
 * valid and properly initialized before calling this function.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream to be set as the default. The stream must be valid and
 * properly initialized. Passing a null or invalid pointer may
 * lead to undefined behavior.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation. Typically, SL_STATUS_OK is returned if the operation is
 * successful.
 ******************************************************************************/
sl_status_t sl_iostream_set_system_default(sl_iostream_t *stream);
#else
#define sl_iostream_set_system_default sl_iostream_set_default
#endif

/***************************************************************************//**
 * @brief This function writes data from a buffer to a specified I/O stream. It
 * can be used to send data to any initialized stream, such as UART or a
 * virtual stream. If the stream parameter is set to SL_IOSTREAM_STDOUT,
 * the function will use the default output stream. The function requires
 * that the stream is properly initialized and configured with a valid
 * write function. If the stream or its write function is not set, the
 * function returns an error status indicating invalid configuration.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream to be used. If set to SL_IOSTREAM_STDOUT, the default
 * output stream will be used. Must not be null unless using
 * SL_IOSTREAM_STDOUT.
 * @param buffer A pointer to the data buffer containing the data to be written.
 * Must not be null.
 * @param buffer_length The length of the data in the buffer to be written. Must
 * be a non-zero value.
 * @return Returns an sl_status_t indicating the success or failure of the write
 * operation. SL_STATUS_INVALID_CONFIGURATION is returned if the stream
 * or its write function is not properly configured.
 ******************************************************************************/
sl_status_t sl_iostream_write(sl_iostream_t *stream,
                              const void *buffer,
                              size_t buffer_length);

/***************************************************************************//**
 * @brief This function is used to read data from a specified I/O stream into a
 * provided buffer. It is essential to ensure that the stream is properly
 * initialized and configured before calling this function. If the stream
 * parameter is set to SL_IOSTREAM_STDIN, the function will use the
 * default input stream. The function will attempt to read up to the
 * specified buffer length and will store the number of bytes actually
 * read in the bytes_read parameter if it is not null. It returns a
 * status code indicating success or failure, with specific handling for
 * invalid stream configurations.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream to read from. It can be set to SL_IOSTREAM_STDIN to use
 * the default input stream. Must not be null and must have a
 * valid read function configured.
 * @param buffer A pointer to a memory location where the read data will be
 * stored. Must not be null and should have enough space to hold
 * buffer_length bytes.
 * @param buffer_length The maximum number of bytes to read from the stream.
 * Must be a positive integer.
 * @param bytes_read A pointer to a size_t variable where the function will
 * store the number of bytes actually read. Can be null if the
 * caller does not need this information.
 * @return Returns an sl_status_t value indicating the result of the read
 * operation. SL_STATUS_OK on success, or an error code such as
 * SL_STATUS_INVALID_CONFIGURATION if the stream is not properly
 * configured.
 ******************************************************************************/
sl_status_t sl_iostream_read(sl_iostream_t *stream,
                             void *buffer,
                             size_t buffer_length,
                             size_t *bytes_read);

/***************************************************************************//**
 * @brief This function is used to output a single character to a specified I/O
 * stream. It is useful when you need to send individual characters to a
 * stream, such as for debugging or simple communication tasks. The
 * function requires a valid stream to be specified, which can be a
 * default stream or a specific stream instance. It is important to
 * ensure that the stream is properly initialized before calling this
 * function. The function returns a status code indicating the success or
 * failure of the operation, allowing the caller to handle any errors
 * that may occur during the write process.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream to be used. It can be a default stream like
 * SL_IOSTREAM_STDOUT or a specific stream instance. The stream
 * must be initialized and valid. If the stream is invalid, the
 * function will return an error status.
 * @param c The character to be printed to the stream. It is a single character
 * value that will be sent to the specified stream.
 * @return Returns an sl_status_t value indicating the result of the operation.
 * A successful write will return a status indicating success, while any
 * issues with the stream or write operation will return an appropriate
 * error status.
 ******************************************************************************/
sl_status_t sl_iostream_putchar(sl_iostream_t *stream,
                                char c);

/***************************************************************************//**
 * @brief This function is used to read a single character from a specified I/O
 * stream. It is useful when you need to process input character by
 * character from a stream, such as a UART or file stream. The function
 * requires a valid stream that has been properly initialized and
 * configured. The character read from the stream is stored in the
 * location pointed to by the provided character pointer. Ensure that the
 * stream is ready for reading to avoid blocking or errors. The function
 * returns a status code indicating the success or failure of the
 * operation.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream from which to read. This must be a valid, initialized
 * stream. Passing a null or uninitialized stream will result in
 * undefined behavior.
 * @param c A pointer to a character variable where the read character will be
 * stored. This pointer must not be null, and the caller is responsible
 * for ensuring it points to a valid memory location.
 * @return Returns an sl_status_t value indicating the result of the operation.
 * A successful read will return a status indicating success, while
 * errors in reading will return an appropriate error status.
 ******************************************************************************/
sl_status_t sl_iostream_getchar(sl_iostream_t *stream,
                                char *c);

/***************************************************************************//**
 * @brief This function is used to output a formatted string to a specified I/O
 * stream, which can be a default stream or a specific one provided by
 * the caller. It is useful when you need to print formatted data to
 * different output streams, such as standard output or error streams.
 * The function requires a valid format string and a corresponding list
 * of arguments. If the stream parameter is set to SL_IOSTREAM_STDOUT,
 * the function will use the default output stream. The function returns
 * a status code indicating success or failure, which can be used to
 * handle errors in writing to the stream.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream to be used. It can be SL_IOSTREAM_STDOUT to use the
 * default output stream, or a pointer to a specific stream. The
 * stream must be properly initialized before use.
 * @param format A constant character pointer to a null-terminated string that
 * specifies the format of the output. It must not be null and
 * should follow standard printf-style formatting.
 * @param argp A va_list representing the variable arguments list that
 * corresponds to the format string. It must be initialized with
 * va_start before passing to this function.
 * @return Returns an sl_status_t value indicating the result of the operation.
 * SL_STATUS_OK is returned on success, while SL_STATUS_OBJECT_WRITE
 * indicates a failure in writing to the stream.
 ******************************************************************************/
sl_status_t sl_iostream_vprintf(sl_iostream_t *stream,
                                const char *format,
                                va_list argp);

#if defined(__GNUC__)
__attribute__((format(printf, 2, 3)))
#endif

/***************************************************************************//**
 * @brief This function is used to output a formatted string to a specified I/O
 * stream, similar to the standard printf function but with the ability
 * to specify the target stream. It is useful when you need to direct
 * formatted output to different streams, such as standard output or
 * error streams, or any custom stream defined in your application. The
 * function requires a valid stream and a format string, followed by any
 * additional arguments needed by the format string. It returns a status
 * code indicating the success or failure of the operation.
 *
 * @param stream A pointer to an sl_iostream_t structure representing the I/O
 * stream to which the formatted string will be printed. It can be
 * a specific stream or one of the predefined streams like
 * SL_IOSTREAM_STDOUT or SL_IOSTREAM_STDERR. Must not be null.
 * @param format A C string that contains the text to be written, which can
 * include format specifiers that are replaced by the values
 * specified in subsequent arguments. Must not be null.
 * @param ... Additional arguments that correspond to the format specifiers in
 * the format string. The number and types of these arguments must
 * match the format specifiers.
 * @return Returns an sl_status_t value indicating the success or failure of the
 * operation.
 ******************************************************************************/
sl_status_t sl_iostream_printf(sl_iostream_t *stream,
                               const char *format,
                               ...);

/** @} (end addtogroup iostream) */

#ifdef __cplusplus
}
#endif

#endif // SL_IOSTREAM_H
