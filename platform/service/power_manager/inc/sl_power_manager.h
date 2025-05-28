/***************************************************************************//**
 * @file
 * @brief Power Manager API definition.
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

#ifndef SL_POWER_MANAGER_H
#define SL_POWER_MANAGER_H

#ifndef SL_POWER_MANAGER_DEBUG
#include "sl_power_manager_config.h"
#endif
#include "sl_slist.h"
#include "sl_status.h"
#include "sl_sleeptimer.h"
#include "sl_enum.h"
#include "sl_core.h"
#include "sl_code_classification.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup power_manager Power Manager
 *
 * @details Power manager is a platform level software module that manages
 * the system's energy modes. Its main purpose is to transition the system to a
 * low energy mode when the processor has nothing to execute. The energy mode the
 * system will transition to is determined each time the system goes to sleep
 * using requirements. These requirements are set by the different software modules
 * (drivers, stacks, application code, etc...). Power manager also ensures a
 * strict control of some power hungry resources such as the high frequency
 * external oscillator (normally called HFXO). Power manager also
 * offers a notification mechanism through which any piece of software module can be
 * notified of energy mode transitions through callbacks.
 *
 * @note Sleep Driver is deprecated. Use Power Manager for all sleep-related
 *       operations. See <a href="https://www.silabs.com/documents/
 *       public/application-notes/
 *       an1358-migrating-from-sleep-driver-to-power-manager.pdf">AN1358:
 *       Migrating from Sleep Driver to Power Manager</a> for information on how
 *       to migrate from Sleep Driver to Power Manager.
 * @note Emlib EMU functions EMU_EnterEM1()/EMU_EnterEM2()/EMU_EnterEM3() must not
 *       be used when the Power Manager is present. The Power Manager module must be
 *       the one deciding at which EM level the device sleeps to ensure the application
 *       properly works. Using both at the same time could lead to undefined behavior
 *       in the application.
 *
 * @details
 * ## Initialization
 *
 *   Power manager must be initialized prior to any call to power manager API.
 *   If sl_system is used, only sl_system_init() must be called, otherwise
 *   sl_power_manager_init() must be called manually. Note that power manager
 *   must be initialized after the clock(s), when initialized manually, as the
 *   power manager check which oscillators are used during the initialization phase.
 *
 * ## Add and remove requirements
 *
 *   The drivers should add and remove energy mode requirements, at runtime, on the
 *   lowest energy mode for them depending on their state. When calling
 *   sl_power_manager_sleep(), the lowest possible Energy mode will be automatically
 *   selected.
 *
 *   It is possible to add and remove requirements from ISR. If a specific energy mode
 *   is required in the ISR, but not required to generate the interrupt, a requirement
 *   on the energy mode can be added from the ISR. It is guaranteed that the associated
 *   clock will be active once sl_power_manager_add_requirement() returns. The EM
 *   requirement can be also be removed from an ISR.
 *
 *   Requirements should not be removed if it was not previously added.
 *
 * ## Subscribe to events
 *
 *   It possible to get notified when the system transition from a power level to
 *   another power level. This can allow to do some operations depending on which level
 *   the system goes, such as saving/restoring context.
 *
 * ## Sleep
 *
 *   When the software has no more operation and only need to wait for an event, the
 *   software must call sl_power_manager_sleep(). This is automatically done when the
 *   kernel is present, but it needs to be called from the super loop in a baremetal
 *   project.
 *
 * ## Query callback functions
 *
 * ### Is OK to sleep
 *
 *   Between the time `sl_power_manager_sleep` is called and the MCU is really put
 *   in a lower Energy mode, it is possible that an ISR occur and require the system
 *   to resume at that time instead of sleeping. So a callback is called in a critical
 *   section to validate that the MCU can go to sleep.
 *
 *   In case of an application that runs on an RTOS, the RTOS will take care of determining
 *   if it is ok to sleep. In case of a baremetal application, the function `sl_power_manager_is_ok_to_sleep()`
 *   will be generated automatically by Simplicity Studio's wizard.
 *   The function will look at multiple software modules from the SDK to take a decision.
 *   The application can contribute to the decision by defining the function `app_is_ok_to_sleep()`.
 *   If any of the software modules (including the application via `app_is_ok_to_sleep()`) return false,
 *   the process of entering in sleep will be aborted.
 *
 * ### Sleep on ISR exit
 *
 *   When the system enters sleep, the only way to wake it up is via an interrupt or
 *   exception. By default, power manager will assume that when an interrupt
 *   occurs and the corresponding ISR has been executed, the system must not go back
 *   to sleep. However, in the case where all the processing related to this interrupt
 *   is performed in the ISR, it is possible to go back to sleep by using this hook.
 *
 *   In case of an application that runs on an RTOS, the RTOS will take care of determining
 *   if the system can go back to sleep on ISR exit. Power manager will ensure the system resumes
 *   its operations as soon as a task is resumed, posted or that its delay expires.
 *   In case of a baremetal application, the function `sl_power_manager_sleep_on_isr_exit()` will be generated
 *   automatically by Simplicity Studio's wizard. The function will look at multiple software modules from the SDK
 *   to take a decision. The application can contribute to the decision by defining the
 *   function `app_sleep_on_isr_exit()`.
 *   The generated function will take a decision based on the value returned by the different software modules
 *   (including the application via `app_sleep_on_isr_exit()`):
 *
 *   `SL_POWER_MANAGER_IGNORE`: if the software module did not cause the system wakeup and/or doesn't want to contribute to the decision.
 *   `SL_POWER_MANAGER_SLEEP`: if the software module did cause the system wakeup, but the system should go back to sleep.
 *   `SL_POWER_MANAGER_WAKEUP`: if the software module did cause the system wakeup, and the system should not go back to sleep.
 *
 *   If any software module returned `SL_POWER_MANAGER_SLEEP` and none returned `SL_POWER_MANAGER_WAKEUP`,
 *   the system will go back to sleep. Any other combination will cause the system not to go back to sleep.
 *
 * ### Debugging feature
 *
 *   By setting the configuration define SL_POWER_MANAGER_DEBUG to 1, it is possible
 *   to record the requirements currently set and their owner. It is possible to print
 *   at any time a table that lists all the added requirements and their owner. This
 *   table can be printed by caling the function
 *   sl_power_manager_debug_print_em_requirements().
 *   Make sure to add the following define
 *   ```
 *   #define CURRENT_MODULE_NAME "<Module printable name here>"
 *   ```
 *   to any application code source file that adds and removes requirements.
 *
 * ## Usage Example
 *
 * ```
 * #define EM_EVENT_MASK_ALL  (SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM0   \
 *                             | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM0  \
 *                             | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM1 \
 *                             | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM1  \
 *                             | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM2 \
 *                             | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM2  \
 *                             | SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM3 \
 *                             | SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM3)
 *
 * sl_power_manager_em_transition_event_handle_t event_handle;
 * sl_power_manager_em_transition_event_info_t event_info = {
 *   .event_mask = EM_EVENT_MASK_ALL,
 *   .on_event = my_events_callback,
 * }
 *
 * void main(void)
 * {
 *   // Initialize power manager; not needed if sl_system_init() is used.
 *   sl_power_manager_init();
 *
 *   // Limit sleep level to EM1
 *   sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
 *
 *   // Subscribe to all event types; get notified for every power transition.
 *   sl_power_manager_subscribe_em_transition_event(&event_handle, &event_info);
 *   while (1) {
 *     // Actions
 *     [...]
 *     if (completed) {
 *        // Remove energy mode requirement, can go to EM2 or EM3 now, depending on the configuration
 *        sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
 *     }
 *
 *     // Sleep to lowest possible energy mode; This call is not needed when using the kernel.
 *     sl_power_manager_sleep();
 *     // Will resume after an interrupt or exception
 *   }
 * }
 *
 * void my_events_callback(sl_power_manager_em_t from,
 *                         sl_power_manager_em_t to)
 * {
 *   printf("Event:%s-%s\r\n", string_lookup_table[from], string_lookup_table[to]);
 * }
 * ```
 *
 * @{
 ******************************************************************************/

// -----------------------------------------------------------------------------
// Defines

// Current module name for debugging features
#ifndef CURRENT_MODULE_NAME
#define CURRENT_MODULE_NAME "Anonymous"            ///< current module name
#endif

// Power transition events
#define SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM0     (1 << 0)                                  ///< sl power manager event transition entering em0
#define SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM0      (1 << 1)                                  ///< sl power manager event transition leaving em0
#define SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM1     (1 << 2)                                  ///< sl power manager event transition entering em1
#define SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM1      (1 << 3)                                  ///< sl power manager event transition leaving em1
#define SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM2     (1 << 4)                                  ///< sl power manager event transition entering em2
#define SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM2      (1 << 5)                                  ///< sl power manager event transition leaving em2
#define SL_POWER_MANAGER_EVENT_TRANSITION_ENTERING_EM3     (1 << 6)                                  ///< sl power manager event transition entering em3 (DEPRECATED)
#define SL_POWER_MANAGER_EVENT_TRANSITION_LEAVING_EM3      (1 << 7)                                  ///< sl power manager event transition leaving em3  (DEPRECATED)

// -----------------------------------------------------------------------------
// Data Types

/// @brief Energy modes
/***************************************************************************//**
 * @brief The `sl_power_manager_em_t` is an enumeration that defines various
 * energy modes for a power management system. Each enumerator represents
 * a specific energy mode, ranging from full operation (Run Mode) to
 * complete shutdown (Shutoff Mode). This enumeration is used to manage
 * and transition between different energy states in a system, allowing
 * for efficient power usage by selecting the appropriate mode based on
 * system requirements and activity.
 *
 * @param SL_POWER_MANAGER_EM0 Run Mode (Energy Mode 0).
 * @param SL_POWER_MANAGER_EM1 Sleep Mode (Energy Mode 1).
 * @param SL_POWER_MANAGER_EM2 Deep Sleep Mode (Energy Mode 2).
 * @param SL_POWER_MANAGER_EM3 Stop Mode (Energy Mode 3).
 * @param SL_POWER_MANAGER_EM4 Shutoff Mode (Energy Mode 4).
 ******************************************************************************/
typedef  enum  {
  SL_POWER_MANAGER_EM0 = 0,   ///< Run Mode (Energy Mode 0)
  SL_POWER_MANAGER_EM1,       ///< Sleep Mode (Energy Mode 1)
  SL_POWER_MANAGER_EM2,       ///< Deep Sleep Mode (Energy Mode 2)
  SL_POWER_MANAGER_EM3,       ///< Stop Mode (Energy Mode 3)
  SL_POWER_MANAGER_EM4,       ///< Shutoff Mode (Energy Mode 4)
} sl_power_manager_em_t;

/// @brief Mask of all the event(s) to listen to.
typedef uint32_t sl_power_manager_em_transition_event_t;

/***************************************************************************//**
 * Typedef for the user supplied callback function which is called when
 * an energy mode transition occurs.
 *
 * @param from Energy mode we are leaving.
 * @param to   Energy mode we are entering.
 ******************************************************************************/
typedef void (*sl_power_manager_em_transition_on_event_t)(sl_power_manager_em_t from,
                                                          sl_power_manager_em_t to);

/// @brief Struct representing energy mode transition event information
/***************************************************************************//**
 * @brief The `sl_power_manager_em_transition_event_info_t` structure is used to
 * define information related to energy mode transition events in the
 * power management system. It contains a mask specifying which
 * transitions should trigger a callback and a function pointer to the
 * callback that should be executed when the specified transitions occur.
 * This structure is integral to the power manager's ability to notify
 * other software components of changes in energy modes, allowing for
 * context saving or other operations as needed.
 *
 * @param event_mask Mask of the transitions on which the callback should be
 * called.
 * @param on_event Function that must be called when the event occurs.
 ******************************************************************************/
typedef struct {
  const sl_power_manager_em_transition_event_t event_mask;  ///< Mask of the transitions on which the callback should be called.
  const sl_power_manager_em_transition_on_event_t on_event; ///< Function that must be called when the event occurs.
} sl_power_manager_em_transition_event_info_t;

/// @brief Struct representing energy mode transition event handle
/***************************************************************************//**
 * @brief The `sl_power_manager_em_transition_event_handle_t` is a structure
 * used in the Power Manager API to represent a handle for energy mode
 * transition events. It contains a list node (`node`) for linking in a
 * singly linked list and a pointer to an
 * `sl_power_manager_em_transition_event_info_t` structure (`info`) that
 * holds information about the event, such as the event mask and the
 * callback function to be called when the event occurs. This structure
 * is essential for managing and subscribing to energy mode transition
 * events within the power management system.
 *
 * @param node List node.
 * @param info Handle event info.
 ******************************************************************************/
typedef struct {
  sl_slist_node_t node;                                     ///< List node.
  const sl_power_manager_em_transition_event_info_t *info;  ///< Handle event info.
} sl_power_manager_em_transition_event_handle_t;

/// On ISR Exit Hook answer
SL_ENUM(sl_power_manager_on_isr_exit_t) {
  SL_POWER_MANAGER_IGNORE = (1UL << 0UL),     ///< The module did not trigger an ISR and it doesn't want to contribute to the decision
  SL_POWER_MANAGER_SLEEP  = (1UL << 1UL),     ///< The module was the one that caused the system wakeup and the system SHOULD go back to sleep
  SL_POWER_MANAGER_WAKEUP = (1UL << 2UL),     ///< The module was the one that caused the system wakeup and the system MUST NOT go back to sleep
};

// -----------------------------------------------------------------------------
// Internal Prototypes only to be used by Power Manager module
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function adjusts the requirement counter for a specified energy
 * mode, either incrementing or decrementing it based on the provided
 * parameters. It is used to manage the system's energy mode requirements
 * dynamically, ensuring that the system transitions to the appropriate
 * energy mode based on current needs. The function must be called with a
 * valid energy mode greater than EM0 and less than EM3, and it handles
 * edge cases where the counter might overflow or underflow by asserting
 * and returning without making changes.
 *
 * @param em Specifies the energy mode for which the requirement is being
 * updated. Must be greater than SL_POWER_MANAGER_EM0 and less than
 * SL_POWER_MANAGER_EM3. Invalid values will trigger an assertion.
 * @param add A boolean indicating whether to add (true) or remove (false) a
 * requirement for the specified energy mode. If adding would cause
 * the counter to exceed 255 or removing would cause it to drop below
 * 0, the function will assert and return without making changes.
 * @return None
 ******************************************************************************/
void sli_power_manager_update_em_requirement(sl_power_manager_em_t em,
                                             bool  add);

// To make sure that we are able to optimize out the string argument when the
// debug feature is disable, we use a pre-processor macro resulting in a no-op.
// We also make sure to always have a definition for the function regardless if
// the debug feature is enable or not for binary compatibility.
#if (SL_POWER_MANAGER_DEBUG == 1)
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to log or remove a debug requirement for a
 * specified energy mode, provided that the debug feature is enabled. It
 * is intended for use in debugging scenarios where tracking energy mode
 * requirements is necessary. The function does nothing if the energy
 * mode is SL_POWER_MANAGER_EM0 or if the debug feature is disabled. It
 * should be called when adding or removing energy mode requirements to
 * maintain accurate debug logs.
 *
 * @param em Specifies the energy mode for which the requirement is being logged
 * or removed. Must be a valid energy mode other than
 * SL_POWER_MANAGER_EM0. If SL_POWER_MANAGER_EM0 is passed, the
 * function does nothing.
 * @param add A boolean indicating whether to add (true) or remove (false) the
 * requirement for the specified energy mode.
 * @param name A string representing the name of the module or requirement
 * owner. Must not be null if the debug feature is enabled.
 * @return None
 ******************************************************************************/
void sli_power_manager_debug_log_em_requirement(sl_power_manager_em_t em,
                                                bool                  add,
                                                const char            *name);
#else
#define sli_power_manager_debug_log_em_requirement(em, add, name) /* no-op */
#endif

// -----------------------------------------------------------------------------
// Prototypes

/***************************************************************************//**
 * @brief This function sets up the Power Manager module, which is essential for
 * managing the system's energy modes. It should be called before using
 * any other power manager API functions, unless the system is
 * initialized using sl_system_init(). The function ensures that
 * necessary hardware components are initialized and configured for power
 * management. It must be called after clock initialization if done
 * manually, as it checks oscillator usage during setup. The function
 * returns a status code indicating success or failure of the
 * initialization process.
 *
 * @return Returns a status code of type sl_status_t indicating the success or
 * failure of the initialization process.
 ******************************************************************************/
sl_status_t sl_power_manager_init(void);

/***************************************************************************//**
 * Sleep at the lowest allowed energy mode.
 *
 * @note Must not be called from ISR
 * @par
 * @note This function will expect and call a callback with the following
 *       signature: `bool sl_power_manager_is_ok_to_sleep(void)`.
 *
 * @note This function can be used to cancel a sleep action and handle the
 *       possible race condition where an ISR that would cause a wakeup is
 *       triggered right after the decision to call sl_power_manager_sleep()
 *       has been made.
 *
 * @note This function must NOT be called with interrupts disabled. This means
 *       both BASEPRI and PRIMASK MUST have a value of 0 when invoking this
 *       function.
 *
 * Usage example:
 *
 * ```c
 * void main(void)
 * {
 *   sl_power_manager_init();
 *   while (1) {
 *     tick();
 *     sl_power_manager_sleep();
 *   }
 * }
 * ```
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function is used to transition the system into the lowest
 * possible energy mode when the processor has no tasks to execute,
 * effectively reducing power consumption. It should be called when the
 * system is idle and ready to enter a sleep state. The function checks
 * if it is permissible to sleep and handles any necessary pre-sleep
 * operations. It must not be called from an interrupt service routine
 * (ISR) and should only be invoked when interrupts are enabled, meaning
 * both BASEPRI and PRIMASK must be set to 0. This function is typically
 * used in a main loop to ensure the system enters a low-power state when
 * idle.
 *
 * @return None
 ******************************************************************************/
void sl_power_manager_sleep(void);

/***************************************************************************//**
 * Adds requirement on given energy mode.
 *
 * @param em  Energy mode to add the requirement to:
 *            - ::SL_POWER_MANAGER_EM1
 *            - ::SL_POWER_MANAGER_EM2 (DEPRECATED)
 *
 * @note Adding EM requirements on SL_POWER_MANAGER_EM2 is now DEPRECATED.
 *       The calls can simply be removed since the system will go to deepsleep
 *       (EM2/EM3) in the absence of EM1 requirements.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The function `sl_power_manager_add_em_requirement` adds a requirement
 * for a specified energy mode to the power manager, ensuring that the
 * system does not enter a lower energy mode than specified.
 *
 * @param em An energy mode of type `sl_power_manager_em_t` that specifies which
 * energy mode requirement to add, such as SL_POWER_MANAGER_EM1.
 * @return The function does not return a value; it performs its operations to
 * update the power manager's state and logs the action for debugging.
 ******************************************************************************/
__STATIC_INLINE void sl_power_manager_add_em_requirement(sl_power_manager_em_t em)
{
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  sli_power_manager_update_em_requirement(em, true);

  sli_power_manager_debug_log_em_requirement(em, true, (const char *)CURRENT_MODULE_NAME);
  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * Removes requirement on given energy mode.
 *
 * @param em  Energy mode to remove the requirement to:
 *            - ::SL_POWER_MANAGER_EM1
 *            - ::SL_POWER_MANAGER_EM2 (DEPRECATED)
 *
 * @note Removing EM requirements on SL_POWER_MANAGER_EM2 is now DEPRECATED.
 *       The calls can simply be removed since the system will go to deepsleep
 *       (EM2/EM3) in the absence of EM1 requirements.
 ******************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief The function `sl_power_manager_remove_em_requirement` removes a
 * previously set energy mode requirement in a critical section.
 *
 * @param em An energy mode of type `sl_power_manager_em_t` from which the
 * requirement is to be removed.
 * @return The function does not return any value; it performs its operation as
 * a side effect.
 ******************************************************************************/
__STATIC_INLINE void sl_power_manager_remove_em_requirement(sl_power_manager_em_t em)
{
  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  sli_power_manager_update_em_requirement(em, false);

  sli_power_manager_debug_log_em_requirement(em, false, (const char *)CURRENT_MODULE_NAME);
  CORE_EXIT_CRITICAL();
}

/***************************************************************************//**
 * @brief This function allows a software module to subscribe to notifications
 * for energy mode transitions by registering a callback function. It
 * should be used when a module needs to perform specific actions during
 * transitions between energy modes, such as saving or restoring context.
 * The function requires a persistent event handle and event information
 * structure, which must remain valid until the callback is triggered. It
 * is important to note that adding or removing energy mode requirements
 * from within a transition event callback is not supported. Deprecated
 * events, such as those related to EM3 transitions, should not be used.
 *
 * @param event_handle A pointer to an event handle structure that will be used
 * to manage the subscription. This must not be null and
 * should be persistent until the event is unsubscribed.
 * @param event_info A pointer to a structure containing the event mask and the
 * callback function to be called on transitions. This must
 * not be null and should be persistent until the event is
 * unsubscribed. Deprecated events should not be included in
 * the event mask.
 * @return None
 ******************************************************************************/
void sl_power_manager_subscribe_em_transition_event(sl_power_manager_em_transition_event_handle_t     *event_handle,
                                                    const sl_power_manager_em_transition_event_info_t *event_info);

/***************************************************************************//**
 * @brief Use this function to remove a previously registered event callback
 * handle from the energy mode transition notification system. This is
 * necessary when you no longer need to receive notifications for energy
 * mode transitions or before deallocating the event handle. Ensure that
 * the event handle was previously registered using the appropriate
 * subscription function. If the handle is not found, an assertion error
 * will be triggered, indicating a logical error in the program.
 *
 * @param event_handle A pointer to the event handle that was previously
 * registered. It must not be null and must have been
 * successfully registered before calling this function. The
 * function will assert if the handle is not found,
 * indicating improper use.
 * @return None
 ******************************************************************************/
void sl_power_manager_unsubscribe_em_transition_event(sl_power_manager_em_transition_event_handle_t *event_handle);

/***************************************************************************//**
 * @brief This function returns the current overhead value, in Sleeptimer ticks,
 * used for early restore time when a scheduled wake-up is set. It is
 * useful for applications that need to account for additional time
 * required to restore the system state before a scheduled wake-up. This
 * function should be called after the power manager has been
 * initialized. If the project includes the power_manager_no_deepsleep
 * component, which configures the lowest energy mode as EM1, this
 * function will always return 0, as deep sleep modes are not utilized.
 *
 * @return Returns the current overhead value for early restore time in
 * Sleeptimer ticks, or 0 if deep sleep modes are not used.
 ******************************************************************************/
int32_t sl_power_manager_schedule_wakeup_get_restore_overhead_tick(void);

/***************************************************************************//**
 * @brief This function sets the overhead tick value used for early restore time
 * in scheduled wake-up scenarios. It should be called after the power
 * manager has been initialized to ensure the value is not overwritten.
 * The overhead value can be positive or negative, allowing for
 * adjustment of the restore process timing. This function is ineffective
 * if the project includes the power_manager_no_deepsleep component,
 * which restricts the lowest energy mode to EM1.
 *
 * @param overhead_tick An integer representing the overhead value in Sleeptimer
 * ticks for early restore time. It can be positive or
 * negative. The caller retains ownership, and the function
 * does not validate the range of this value.
 * @return None
 ******************************************************************************/
void sl_power_manager_schedule_wakeup_set_restore_overhead_tick(int32_t overhead_tick);

/***************************************************************************//**
 * @brief This function is used to obtain the current minimum off-time value,
 * expressed in Sleeptimer ticks, which is relevant when scheduling a
 * wake-up. It is particularly useful in scenarios where energy
 * efficiency is critical, as it helps determine whether it is more
 * energy-efficient to keep the high-frequency clock running or to turn
 * it off temporarily. This function should be called when the system
 * needs to make decisions about power management and energy mode
 * transitions. Note that if the project includes the
 * power_manager_no_deepsleep component, which sets the lowest energy
 * mode to EM1, this function will return 0, indicating that the minimum
 * off-time is not applicable.
 *
 * @return Returns the current minimum off-time value in Sleeptimer ticks, or 0
 * if the power_manager_no_deepsleep component is present.
 ******************************************************************************/
uint32_t sl_power_manager_schedule_wakeup_get_minimum_offtime_tick(void);

/***************************************************************************//**
 * @brief This function configures the minimum duration, in Sleeptimer ticks,
 * that the system should remain off before a scheduled wake-up. It is
 * useful for optimizing energy consumption by determining when it is
 * more efficient to keep the high frequency clock off. This function
 * should be called after the power manager is initialized. If the
 * project is configured with the power_manager_no_deepsleep component,
 * which sets the lowest energy mode to EM1, this function will have no
 * effect.
 *
 * @param minimum_offtime_tick Specifies the minimum off-time in Sleeptimer
 * ticks. The value should be a non-negative
 * integer. The function does nothing if the
 * power_manager_no_deepsleep component is present.
 * @return None
 ******************************************************************************/
void sl_power_manager_schedule_wakeup_set_minimum_offtime_tick(uint32_t minimum_offtime_tick);

/***************************************************************************//**
 * @brief This function allows the user to enable or disable fast wake-up
 * capabilities when transitioning from energy modes EM2 or EM3 to a
 * higher energy mode. It should be used when there is a need to optimize
 * wake-up times from these low energy modes. The function must be called
 * after the power manager has been initialized. It has no effect if the
 * project is configured to not support deep sleep modes (EM2/EM3).
 *
 * @param enable A boolean value where 'true' enables fast wake-up and 'false'
 * disables it. The parameter must be provided by the caller, and
 * it is expected to be a valid boolean value.
 * @return None
 ******************************************************************************/
void sl_power_manager_em23_voltage_scaling_enable_fast_wakeup(bool enable);

/**************************************************************************//**
 * Determines if the HFXO interrupt was part of the last wake-up and/or if
 * the HFXO early wakeup expired during the last ISR
 * and if it was the only timer to expire in that period.
 *
 * @return true if power manager sleep can return to sleep,
 *         false otherwise.
 *
 * @note This function will always return false in case a requirement
 *       is added on SL_POWER_MANAGER_EM1, since we will
 *       never sleep at a lower level than EM1.
 *****************************************************************************/
SL_CODE_CLASSIFY(SL_CODE_COMPONENT_POWER_MANAGER, SL_CODE_CLASS_TIME_CRITICAL)
/***************************************************************************//**
 * @brief This function checks if the most recent wakeup event was triggered by
 * the high-frequency external oscillator (HFXO) interrupt or if the
 * early wakeup timer was the only timer to expire during the last
 * interrupt service routine (ISR). It is useful for determining whether
 * the system can return to sleep. The function will always return false
 * if the system is configured to not enter deep sleep (EM1 is the lowest
 * energy mode allowed). This function should be used in scenarios where
 * understanding the cause of wakeup is necessary for power management
 * decisions.
 *
 * @return Returns true if the wakeup was due to an HFXO interrupt or early
 * wakeup timer expiration, allowing the system to return to sleep;
 * returns false otherwise.
 ******************************************************************************/
bool sl_power_manager_is_latest_wakeup_internal(void);

/***************************************************************************//**
 * @brief This function transitions the system into energy mode 4 (EM4), which
 * is a deep sleep mode where the device is effectively shut down. It is
 * intended for use when the system needs to conserve maximum power and
 * will not perform any further operations until a reset occurs. Once the
 * device enters EM4, it can only be awakened by a power-on reset or an
 * external reset pin. This function should be used with caution, as it
 * does not return under normal circumstances. Ensure that all necessary
 * operations are completed before calling this function, as it will halt
 * further execution.
 *
 * @return None
 ******************************************************************************/
void sl_power_manager_enter_em4(void);

/***************************************************************************//**
 * @brief This function is used to release the pin retention state after the
 * system exits Energy Mode 4 (EM4). It should be called when the EM4 pin
 * retention feature is enabled and the system has been reset after
 * exiting EM4. This allows peripherals or GPIOs to be re-initialized and
 * regain control over the pins. If the EM4 Pin Retention feature is not
 * available on the device, calling this function will have no effect. It
 * is typically used in systems where pin states need to be retained
 * through EM4 entry and wakeup, and then released once the system is
 * ready to resume normal operation.
 *
 * @return None
 ******************************************************************************/
void sl_power_manager_em4_unlatch_pin_retention(void);

/***************************************************************************//**
 * @brief This function serves as a pre-sleep hook that is called just before
 * the device enters Energy Mode 4 (EM4). It is designed to be overridden
 * by the user if specific actions need to be performed before the device
 * transitions to EM4. The default implementation does nothing, allowing
 * developers to redefine it in their application code to suit their
 * needs. This function is particularly useful for preparing the system
 * or peripherals for the low-power state of EM4.
 *
 * @return None
 ******************************************************************************/
void sl_power_manager_em4_presleep_hook(void);

/** @} (end addtogroup power_manager) */

#ifdef __cplusplus
}
#endif

#endif // SL_POWER_MANAGER_H
