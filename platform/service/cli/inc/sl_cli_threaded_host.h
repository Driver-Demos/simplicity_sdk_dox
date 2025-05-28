/***************************************************************************//**
 * @file
 * @brief Threading of CLI for Hosts
 * @version x.y.z
 *******************************************************************************
 * # License
 * <b>Copyright 2021 Silicon Laboratories Inc. www.silabs.com</b>
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
#if (defined(EZSP_HOST) || defined(ZIGBEE_PRO_COMPLIANCE_ON_HOST))
#include <assert.h>
#include <pthread.h>
#ifdef __APPLE__
#include <dispatch/dispatch.h>
#else
#include <errno.h>
#include <semaphore.h>
#endif
#include "sl_cli.h"

#define SL_CLI_THREADED_HOST_PIPE_DATA_LENGTH 2

/***************************************************************************//**
 * @brief The 'semaphore' struct is a platform-dependent abstraction for a
 * semaphore, which is a synchronization primitive used to control access
 * to a common resource in concurrent programming. On Apple systems, it
 * uses 'dispatch_semaphore_t' from the Grand Central Dispatch (GCD)
 * library, while on other systems, it uses 'sem_t' from the POSIX
 * semaphore library. This struct is part of a threading implementation
 * for a Command Line Interface (CLI) on host systems, ensuring that
 * resources are accessed in a thread-safe manner.
 *
 * @param sem A platform-dependent semaphore object, either a
 * dispatch_semaphore_t for Apple systems or a sem_t for other
 * systems.
 ******************************************************************************/
struct semaphore {
#ifdef __APPLE__
  dispatch_semaphore_t    sem;
#else // __APPLE__
  sem_t                   sem;
#endif // __APPLE__
};

/***************************************************************************//**
 * @brief The `thread_event` is a global variable of type `struct semaphore`,
 * which is used to manage synchronization between threads in a multi-
 * threaded environment. It is defined as an external variable,
 * indicating that its actual definition is located in another source
 * file. The semaphore structure contains a platform-specific semaphore
 * object, either a `dispatch_semaphore_t` for Apple platforms or a
 * `sem_t` for other platforms.
 *
 * @details This variable is used to coordinate thread execution by signaling
 * and waiting on events in a multi-threaded application.
 ******************************************************************************/
extern struct semaphore thread_event;

/***************************************************************************//**
 * @brief This function is used to attempt a non-blocking decrement of the
 * semaphore associated with the provided semaphore structure. It is
 * useful in scenarios where the caller wants to proceed only if the
 * semaphore can be decremented immediately, without waiting. This
 * function should be called only when the semaphore has been properly
 * initialized. The behavior of this function may vary slightly depending
 * on the platform, as it uses platform-specific semaphore handling
 * mechanisms.
 *
 * @param s A pointer to a semaphore structure. This must not be null and should
 * point to a valid, initialized semaphore. The function will attempt
 * to decrement the semaphore associated with this structure.
 * @return None
 ******************************************************************************/
void sema_wait(struct semaphore *s);

/***************************************************************************//**
 * @brief This function processes the current input buffer of a CLI handle and
 * updates the command history if the buffer is not empty. It should be
 * called when there is input to be processed in the CLI context. The
 * function ensures that the prompt is requested for the next input
 * cycle. It is important to ensure that the handle is properly
 * initialized and that the input buffer contains valid data before
 * calling this function. The function will clear the input buffer after
 * processing.
 *
 * @param handle A pointer to an sl_cli_handle_t structure representing the CLI
 * context. The handle must be valid and properly initialized. The
 * input buffer within the handle should contain the input to be
 * processed. The function will modify the handle's state,
 * including clearing the input buffer.
 * @return None
 ******************************************************************************/
void sli_cli_handle_input_and_history(sl_cli_handle_t handle);

void sli_cli_threaded_host_init(void);

/***************************************************************************//**
 * @brief This function is used to determine whether the CLI input has been
 * processed. It is typically called in scenarios where the CLI is being
 * managed in a threaded environment, such as on host systems that
 * support EZSP or Zigbee Pro compliance. The function returns a boolean
 * value indicating the handling status of the input, which can be used
 * to make decisions about further processing or waiting for input. It is
 * important to ensure that the CLI system is properly initialized and
 * running in a compatible environment before calling this function.
 *
 * @return Returns a boolean value: true if the input has been handled, false
 * otherwise.
 ******************************************************************************/
bool sli_cli_is_input_handled(void);

/***************************************************************************//**
 * @brief This function provides the file descriptor used for reading data from
 * a pipe in a threaded CLI environment on host systems. It is typically
 * used in scenarios where the CLI is running in a separate thread and
 * needs to communicate with other parts of the application through a
 * pipe. This function is only available when the application is compiled
 * with either the EZSP_HOST or ZIGBEE_PRO_COMPLIANCE_ON_HOST
 * preprocessor definitions. It is important to ensure that the
 * environment is correctly set up to use this function, as it relies on
 * specific threading and inter-process communication mechanisms.
 *
 * @return Returns an integer representing the file descriptor for reading from
 * the CLI pipe.
 ******************************************************************************/
int sli_cli_get_pipe_read_fd(void);
#endif // EZSP_HOST || ZIGBEE_PRO_COMPLIANCE_ON_HOST
