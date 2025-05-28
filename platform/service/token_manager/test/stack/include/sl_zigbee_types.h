/***************************************************************************//**
 * @brief Ember Connect data type definitions.
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

#ifndef __EMBER_TYPES_H__
#define __EMBER_TYPES_H__

#include <stdint.h>
#include <stdbool.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "sl_zigbee_configuration_defaults.h"
#endif //DOXYGEN_SHOULD_SKIP_THIS

/**
 * @brief Size of an extended PAN identifier in bytes (8).
 */
#define EXTENDED_PAN_ID_SIZE 8

/**
 * @brief Size of EUI64 (an IEEE address) in bytes (8).
 */
#define EUI64_SIZE 8

/**
 * @brief Size of an encryption key in bytes (16).
 */
#define SL_ZIGBEE_ENCRYPTION_KEY_SIZE 16

/**
 * @brief EUI 64-bit ID (an IEEE address).
 */
typedef uint8_t sl_802154_long_addr_t[EUI64_SIZE];

/**
 * @brief 802.15.4 node ID.
 */
typedef uint16_t sl_802154_short_addr_t;

/**
 * @brief 802.15.4 PAN ID.
 */
typedef uint16_t sl_802154_pan_id_t;

/**
 * @brief A distinguished network ID that will never be assigned
 * to any node.  Used to indicate the absence of a node ID.
 */
#define SL_ZIGBEE_NULL_NODE_ID                0xFFFFu

/** @brief Broadcast address. */
#define SL_ZIGBEE_BROADCAST_ADDRESS           0xFFFF

/** @brief Special short address indicating the node should use long addressing
 * as source address. */
#define SL_ZIGBEE_USE_LONG_ADDRESS            0xFFFE

/** @brief The coordinator short address. */
#define SL_ZIGBEE_COORDINATOR_ADDRESS         0x0000

/**
 * @brief Defines the possible types of nodes and the roles that a
 * node might play in a network.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberNodeType
#else
typedef uint8_t EmberNodeType;
enum
#endif
{
  /** Device is not joined. */
  SL_ZIGBEE_UNKNOWN_DEVICE = 0,
  /** Star or extended star topology device: will relay messages and can act as
   * a parent to range extender and  end device nodes.
   */
  SL_ZIGBEE_STAR_COORDINATOR = 1,
  /** Star or extended star topology device: will relay messages and can act as
   * a parent to end device nodes.
   */
  SL_ZIGBEE_STAR_RANGE_EXTENDER = 2,
  /** Star or extended star topology device: communicates only with its parent
   * and will not relay messages.
   */
  SL_ZIGBEE_STAR_END_DEVICE = 3,
  /** Star or extended star topology device: an end device whose radio can be
   * turned off to save power. The application must call sli_zigbee_stack_poll_for_data() to
   * receive messages.
   */
  SL_ZIGBEE_STAR_SLEEPY_END_DEVICE = 4,
  /** A device able to send and receive messages from other devices in range on
   * the same PAN ID, with no star topology restrictions. Such device does not
   * relay messages. A node can be started as a direct device by using the
   * emberJoinCommissioned() API.
   */
  SL_ZIGBEE_DIRECT_DEVICE = 5,
  /** A device able to send and receive MAC-level messages. A node can be
   * started as a MAC mode device by using the emberJoinCommissioned() API or
   * the emberJoinNetworkExtended() API.
   */
  SL_802154_MODE_DEVICE = 6,
  /** A sleepy device able to send and receive MAC-level messages. A node can be
   * started as a sleepy MAC mode device by using the emberJoinCommissioned()
   * API or the emberJoinNetworkExtended() API.
   */
  SL_802154_MODE_SLEEPY_DEVICE = 7,
};

/**
 * @brief Defines the possible join states for a node.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberNetworkStatus
#else
typedef uint8_t EmberNetworkStatus;
enum
#endif
{
  /** The node is not associated with a network in any way. */
  SL_ZIGBEE_NO_NETWORK,
  /** The node is currently attempting to join a network. */
  SL_ZIGBEE_JOINING_NETWORK,
  /** The node is joined to a network. */
  SL_ZIGBEE_JOINED_NETWORK,
};

/***************************************************************************//**
 * @brief The `EmberNetworkParameters` structure is used to hold essential
 * network parameters for a Zigbee network, including the PAN identifier,
 * transmission power, and radio channel. These parameters are crucial
 * for configuring and managing the network's communication settings,
 * ensuring that devices can communicate effectively within the specified
 * network environment.
 *
 * @param panId The network's PAN identifier.
 * @param radioTxPower A power setting, in dBm.
 * @param radioChannel A radio channel, ensuring it is supported by the radio.
 ******************************************************************************/
typedef struct {
  /** The network's PAN identifier.*/
  uint16_t  panId;
  /** A power setting, in dBm.*/
  int8_t   radioTxPower;
  /** A radio channel. Be sure to specify a channel supported by the radio. */
  uint8_t   radioChannel;
} EmberNetworkParameters;

/**
 * @brief Child flags.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberChildFlags
#else
typedef uint8_t EmberChildFlags;
enum
#endif
{
  /** The child is a range extender. */
  SL_ZIGBEE_CHILD_FLAGS_DEVICE_IS_RANGE_EXTENDER_BIT       = 0x02,
  /** The child is a sleepy end device. */
  SL_ZIGBEE_CHILD_FLAGS_DEVICE_IS_SLEEPY_BIT               = 0x04,
  /** The node has data pending for the child. */
  SL_ZIGBEE_CHILD_FLAGS_HAVE_PENDING_DATA_BIT              = 0x08,
  /** The child supports AES-based security. */
  SL_ZIGBEE_CHILD_FLAGS_AES_SECURITY_CAPABLE_BIT           = 0x10,
  /** The child supports XXTEA-based security. */
  SL_ZIGBEE_CHILD_FLAGS_XXTEA_SECURITY_CAPABLE_BIT         = 0x20,
};

/**
 * @brief Message length in bytes.
 */
typedef uint8_t sl_802154_message_length_t;

/**
 * @brief Message options.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum sl_802154_message_options_t
#else
typedef uint8_t sl_802154_message_options_t;
enum
#endif
{
  /** No options. */
  SL_ZIGBEE_OPTIONS_NONE                     = 0x00,
  /** The packet should be sent out encrypted. */
  SL_ZIGBEE_OPTIONS_SECURITY_ENABLED         = 0x01,
  /** An acknowledgment should be requested for the outgoing packet. */
  SL_ZIGBEE_OPTIONS_ACK_REQUESTED            = 0x02,
  /** The packet should be sent with high priority. */
  SL_ZIGBEE_OPTIONS_HIGH_PRIORITY            = 0x04,
  /** The packet should be sent via the indirect queue. This option only applies
     to nodes with SL_802154_MODE_DEVICE or SL_802154_MODE_SLEEPY_DEVICE
     node type. */
  SL_ZIGBEE_OPTIONS_INDIRECT                 = 0x08,
};

/***************************************************************************//**
 * @brief The `EmberIncomingMessage` structure is used to describe an incoming
 * message in a Zigbee network. It contains information about the message
 * options, source node ID, destination endpoint, received signal
 * strength indicator (RSSI), message length, and a pointer to the
 * message payload. This structure is typically used in the context of
 * handling incoming messages within the Zigbee stack, providing
 * essential details needed for processing and responding to the message.
 *
 * @param options An sl_802154_message_options_t value indicating the options
 * used for the incoming packet.
 * @param source An sl_802154_short_addr_t value indicating source node ID.
 * @param endpoint The endpoint the message is destined to.
 * @param rssi The RSSI in dBm the packet was received with.
 * @param length An sl_802154_message_length_t value indicating the length in
 * bytes of the incoming message.
 * @param payload A pointer to the message payload.
 ******************************************************************************/
typedef struct {
  /**
   * An sl_802154_message_options_t value indicating the options used for the incoming
   * packet.
   */
  sl_802154_message_options_t options;
  /**
   * An sl_802154_short_addr_t value indicating source node ID.
   */
  sl_802154_short_addr_t source;
  /**
   * The endpoint the message is destined to.
   */
  uint8_t endpoint;
  /**
   * The RSSI in dBm the packet was received with.
   */
  int8_t rssi;
  /**
   * An sl_802154_message_length_t value indicating the length in bytes of the
   * incoming message.
   */
  sl_802154_message_length_t length;
  /**
   * A pointer to the message payload.
   */
  uint8_t *payload;
} EmberIncomingMessage;

/***************************************************************************//**
 * @brief The `EmberOutgoingMessage` structure is used to describe an outgoing
 * message in a Zigbee network. It contains fields for specifying
 * transmission options, destination address, endpoint, and a tag for
 * matching message send calls with their corresponding handlers.
 * Additionally, it includes the message length, a pointer to the
 * payload, and the RSSI of the acknowledgment if requested. This
 * structure is essential for managing outgoing communications in Zigbee
 * applications, providing detailed control over message transmission
 * parameters.
 *
 * @param options An sl_802154_message_options_t value indicating the options
 * used for transmitting the outgoing message.
 * @param destination An sl_802154_short_addr_t value indicating the destination
 * short ID.
 * @param endpoint The endpoint the message is destined to.
 * @param tag A tag value the application can use to match emberMessageSend()
 * calls to the corresponding sli_zigbee_stack_message_sent_handler()
 * calls.
 * @param length An sl_802154_message_length_t value indicating the length in
 * bytes of the incoming message.
 * @param payload A pointer to the message payload.
 * @param ackRssi The RSSI in dBm of the ACK corresponding to this message,
 * meaningful only if SL_ZIGBEE_OPTIONS_ACK_REQUESTED flag is set
 * in the options field.
 ******************************************************************************/
typedef struct {
  /**
   * An sl_802154_message_options_t value indicating the options used for transmitting
   * the outgoing message.
   */
  sl_802154_message_options_t options;
  /**
   * An sl_802154_short_addr_t value indicating the destination short ID.
   */
  sl_802154_short_addr_t destination;
  /**
   * The endpoint the message is destined to.
   */
  uint8_t endpoint;
  /**
   * A tag value the application can use to match emberMessageSend() calls to
   * the corresponding sli_zigbee_stack_message_sent_handler() calls.
   */
  uint8_t tag;
  /**
   * An sl_802154_message_length_t value indicating the length in bytes of the
   * incoming message.
   */
  sl_802154_message_length_t length;
  /**
   * A pointer to the message payload.
   */
  uint8_t *payload;
  /**
   * The RSSI in dBm of the ACK corresponding to this message. This field is
   * meaningful only if SL_ZIGBEE_OPTIONS_ACK_REQUESTED flag is set in the options
   * field.
   */
  int8_t ackRssi;
} EmberOutgoingMessage;

/**
 * @brief 802.15.4 addressing mode.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum sl_802154_mac_addr_mode_t
#else
typedef uint8_t sl_802154_mac_addr_mode_t;
enum
#endif
{
  SL_802154_ADDRESS_MODE_NONE  = 0x00,
  SL_802154_ADDRESS_MODE_SHORT = 0x02,
  SL_802154_ADDRESS_MODE_LONG  = 0x03,
};

/***************************************************************************//**
 * @brief The `sl_802154_mac_addr_t` structure is used to represent an 802.15.4
 * MAC address, which can be either a long (EUI64) or short address. It
 * includes a union to store the address and a mode to specify which type
 * of address is being used. This structure is essential for handling
 * MAC-level addressing in 802.15.4 networks, allowing for flexible
 * address representation depending on the network configuration.
 *
 * @param addr A union that can store either a long address (EUI64) or a short
 * address.
 * @param mode An enumeration indicating the addressing mode, either short or
 * long.
 ******************************************************************************/
typedef struct {
  union {
    uint8_t longAddress[EUI64_SIZE];
    uint16_t shortAddress;
  } addr;                                          ///< addr

  sl_802154_mac_addr_mode_t mode;                        ///< mode
} sl_802154_mac_addr_t;

/***************************************************************************//**
 * @brief The `sl_802154_mac_frame_t` structure is used to describe the
 * addressing fields of an 802.15.4 MAC frame, including source and
 * destination addresses and PAN IDs. It contains fields for both short
 * and long addresses, as well as flags to indicate whether the source
 * and destination PAN IDs are specified. This structure is essential for
 * managing MAC-level communication in 802.15.4 networks, allowing for
 * the specification of both source and destination addressing
 * information.
 *
 * @param srcAddress An sl_802154_mac_addr_t structure indicating the source
 * address of a MAC frame.
 * @param dstAddress An sl_802154_mac_addr_t structure indicating the
 * destination address of a MAC frame.
 * @param srcPanId An sl_802154_pan_id_t struct indicating the source PAN ID of
 * a MAC frame, meaningful only if srcPanIdSpecified is true.
 * @param dstPanId An sl_802154_pan_id_t struct indicating the destination PAN
 * ID of a MAC frame, meaningful only if dstPanIdSpecified is
 * true.
 * @param srcPanIdSpecified Boolean indicating if the srcPanId field is set.
 * @param dstPanIdSpecified Boolean indicating if the dstPanId field is set.
 ******************************************************************************/
typedef struct {
  /**
   * An sl_802154_mac_addr_t structure indicating the source address of a MAC frame.
   */
  sl_802154_mac_addr_t srcAddress;
  /**
   * An sl_802154_mac_addr_t structure indicating the destination address of a MAC
   * frame.
   */
  sl_802154_mac_addr_t dstAddress;
  /**
   * An sl_802154_pan_id_t struct indicating the source PAN ID of a MAC frame. This
   * field is meaningful only if srcPanIdSpecified is set to true.
   */
  sl_802154_pan_id_t srcPanId;
  /**
   * An sl_802154_pan_id_t struct indicating the destination PAN ID of a MAC frame.
   * This field is meaningful only if dstPanIdSpecified is set to true.
   */
  sl_802154_pan_id_t dstPanId;
  /**
   * True if the srcPanId field is set, false otherwise.
   */
  bool srcPanIdSpecified;
  /**
   * True if the dstPanId field is set, false otherwise.
   */
  bool dstPanIdSpecified;
} sl_802154_mac_frame_t;

/***************************************************************************//**
 * @brief The `sl_802154_incoming_mac_message_t` structure is used to describe
 * an incoming MAC frame in the 802.15.4 protocol. It contains
 * information about the message options, source and destination
 * addresses, PAN IDs, signal strength (RSSI), link quality (LQI),
 * security frame counter, payload length, and a pointer to the payload
 * itself. This structure is essential for handling and processing
 * incoming messages in a Zigbee network, providing all necessary details
 * to interpret the received data correctly.
 *
 * @param options An sl_802154_message_options_t value indicating the options
 * used for the incoming packet.
 * @param macFrame An sl_802154_mac_frame_t structure indicating the source and
 * destination addresses and source and destination PAN IDs.
 * @param rssi The RSSI in dBm the packet was received with.
 * @param lqi The LQI the packet was received with.
 * @param frameCounter The security MAC frame counter (if any).
 * @param length An sl_802154_message_length_t value indicating the length in
 * bytes of the MAC payload of the incoming message.
 * @param payload A pointer to the message MAC payload.
 ******************************************************************************/
typedef struct {
  /**
   * An sl_802154_message_options_t value indicating the options used for the incoming
   * packet.
   */
  sl_802154_message_options_t options;
  /**
   * An sl_802154_mac_frame_t structure indicating the source and destination
   * addresses and source and destination PAN IDs.
   */
  sl_802154_mac_frame_t macFrame;
  /**
   * The RSSI in dBm the packet was received with.
   */
  int8_t rssi;
  /**
   * The LQI the packet was received with.
   */
  int8_t lqi;
  /**
   * The security MAC frame counter (if any).
   */
  uint32_t frameCounter;
  /**
   * An sl_802154_message_length_t value indicating the length in bytes of the
   * MAC payload of the incoming message.
   */
  sl_802154_message_length_t length;
  /**
   * A pointer to the message MAC payload.
   */
  uint8_t *payload;
} sl_802154_incoming_mac_message_t;

/***************************************************************************//**
 * @brief The `sl_802154_outgoing_mac_message_t` structure is used to describe
 * an outgoing MAC message in the 802.15.4 protocol. It contains various
 * fields that specify the transmission options, addressing information,
 * and payload details of the message. The structure includes a tag for
 * matching send and sent handler calls, a frame counter for security
 * purposes, and an optional RSSI value for the acknowledgment message.
 * This structure is essential for managing outgoing MAC frames in a
 * Zigbee network, ensuring that messages are transmitted with the
 * correct options and addressing information.
 *
 * @param options An sl_802154_message_options_t value indicating the options
 * used for transmitting the outgoing message.
 * @param macFrame An sl_802154_mac_frame_t struct indicating the source and
 * destination addresses and source and destination PAN IDs of
 * the outgoing MAC frame.
 * @param tag A tag value the application can use to match emberMacMessageSend()
 * calls to the corresponding emberMacMessageSentHandler() calls.
 * @param frameCounter The security frame counter of the outgoing MAC frame (if
 * any).
 * @param length An sl_802154_message_length_t value indicating the length in
 * bytes of the incoming message.
 * @param payload A pointer to the message payload.
 * @param ackRssi The RSSI in dBm of the ACK corresponding to this message,
 * meaningful only if SL_ZIGBEE_OPTIONS_ACK_REQUESTED flag is set
 * in the options field.
 ******************************************************************************/
typedef struct {
  /**
   * An sl_802154_message_options_t value indicating the options used for transmitting
   * the outgoing message.
   */
  sl_802154_message_options_t options;
  /**
   * An sl_802154_mac_frame_t struct indicating the source and destination addresses
   * and source and destination PAN IDs of the outgoing MAC frame.
   */
  sl_802154_mac_frame_t macFrame;
  /**
   * A tag value the application can use to match emberMacMessageSend() calls
   * to the corresponding emberMacMessageSentHandler() calls.
   */
  uint8_t tag;
  /**
   * The security frame counter of the outgoing MAC frame (if any).
   */
  uint32_t frameCounter;
  /**
   * An sl_802154_message_length_t value indicating the length in bytes of the
   * incoming message.
   */
  sl_802154_message_length_t length;
  /**
   * A pointer to the message payload.
   */
  uint8_t *payload;
  /**
   * The RSSI in dBm of the ACK corresponding to this message. This field is
   * meaningful only if SL_ZIGBEE_OPTIONS_ACK_REQUESTED flag is set in the options
   * field.
   */
  int8_t ackRssi;
} sl_802154_outgoing_mac_message_t;

/***************************************************************************//**
 * @brief The `EmberKeyData` structure is designed to hold encryption key data
 * used in Zigbee communication. It contains a single member, `contents`,
 * which is an array of bytes that stores the key. The size of this array
 * is determined by the constant `SL_ZIGBEE_ENCRYPTION_KEY_SIZE`,
 * ensuring it is appropriately sized for encryption purposes. This
 * structure is typically used in functions that require access to
 * encryption keys, providing a standardized way to handle key data
 * within the Zigbee stack.
 *
 * @param contents An array of bytes representing the key data, with a size
 * defined by SL_ZIGBEE_ENCRYPTION_KEY_SIZE.
 ******************************************************************************/
typedef struct {
  /** This is the key byte data. */
  uint8_t contents[SL_ZIGBEE_ENCRYPTION_KEY_SIZE];
} EmberKeyData;

/** @brief This function allows the programmer to gain access
 *  to the key data bytes of the EmberKeyData structure.
 *
 * @param[in] key  A Pointer to an EmberKeyData structure.
 *
 * @return uint8_t* Returns a pointer to the first byte of the key data.
 */
#if defined DOXYGEN_SHOULD_SKIP_THIS
uint8_t* sl_zigbee_key_contents(EmberKeyData* key);
#else
#define sl_zigbee_key_contents(key) ((key)->contents)
#endif

/**
 * @brief Either marks an event as inactive or specifies the units for the
 * event execution time.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberEventUnits
#else
typedef uint8_t EmberEventUnits;
enum
#endif
{
  /** The event is not scheduled to run. */
  SL_ZIGBEE_EVENT_INACTIVE = 0,
  /** The execution time is in approximate milliseconds.  */
  SL_ZIGBEE_EVENT_MS_TIME,
  /** The execution time is in 'binary' quarter seconds (256 approximate
      milliseconds each). */
  SL_ZIGBEE_EVENT_QS_TIME,
  /** The execution time is in 'binary' minutes (65536 approximate milliseconds
      each). */
  SL_ZIGBEE_EVENT_MINUTE_TIME,
  /** The event is scheduled to run at the earliest opportunity. */
  SL_ZIGBEE_EVENT_ZERO_DELAY
};

/** brief An identifier for a task */
typedef uint8_t EmberTaskId;

//----------------------------------------------------------------
// Events and event queues.

// Forward declarations to make up for C's one-pass type checking.
struct Event_s;
struct EventQueue_s;

/** @brief The static part of an event.  Each event can be used with only one
 * event queue.
 */

/***************************************************************************//**
 * @brief The `EventActions` structure defines the static properties and actions
 * associated with an event in an event-driven system. It includes a
 * pointer to the event queue, a handler function to execute when the
 * event is triggered, an optional marker function, and a name for
 * debugging purposes. This structure is used to manage and execute
 * events within a queue, providing a mechanism for handling asynchronous
 * operations in a structured manner.
 *
 * @param queue A pointer to the EventQueue_s structure where this event is
 * queued.
 * @param handler A function pointer that is called when the event is triggered.
 * @param marker A function pointer for marking the event, which can be NULL.
 * @param name A constant character pointer used for debugging to identify the
 * event by name.
 ******************************************************************************/
typedef const struct {
  struct EventQueue_s *queue;           ///< the queue this event goes on
  void (*handler)(struct Event_s *);    ///< called when the event fires
  void (*marker)(struct Event_s *);     ///< marking fuction, can be NULL
  const char *name;                     ///< event name for debugging purposes
} EventActions;

/***************************************************************************//**
 * @brief The `Event` structure is used to represent an event in a queue,
 * containing information about the event's actions, its position in the
 * queue, and the time it is scheduled to execute. It is part of an
 * event-driven system where events are managed in a queue and executed
 * based on their scheduled time. The `actions` member points to an
 * `EventActions` structure that defines the queue, handler, marker, and
 * name for debugging purposes. The `next` member is used internally to
 * link events in a queue, and `timeToExecute` specifies when the event
 * should be executed.
 *
 * @param actions A pointer to an EventActions structure containing static data
 * for the event.
 * @param next A pointer to the next Event in the list, which must be
 * initialized to NULL.
 * @param timeToExecute A 32-bit unsigned integer representing the time at which
 * the event is scheduled to execute.
 ******************************************************************************/
typedef struct Event_s {
  EventActions *actions;                ///< static data

  // For internal use only, but the 'next' field must be initialized
  // to NULL.
  struct Event_s *next;                            ///< next
  uint32_t timeToExecute;                          ///< time To Execute
} Event;

/***************************************************************************//**
 * @brief The `EventQueue` structure is designed to manage a list of events,
 * both those triggered by interrupt service routines (ISR) and regular
 * events, in a queue format. It contains two pointers, `isrEvents` and
 * `events`, which point to the respective lists of events. This
 * structure is essential for organizing and processing events in a
 * timely manner, ensuring that both ISR and regular events are handled
 * efficiently within an embedded system.
 *
 * @param isrEvents Pointer to a list of events that are triggered by interrupt
 * service routines.
 * @param events Pointer to a list of regular events in the queue.
 ******************************************************************************/
typedef struct EventQueue_s {
  Event *isrEvents;                                ///< isr Events
  Event *events;                                   ///< events
} EventQueue;

/***************************************************************************//**
 * @brief The `EmberEventControl` structure is used to manage events within the
 * Ember framework, specifically for scheduling and controlling when
 * events should be executed. It contains information about the event's
 * current status, the task it is associated with, and the time remaining
 * before the event is triggered. This structure is crucial for event-
 * driven programming, allowing for precise timing and task management in
 * embedded systems.
 *
 * @param status The event's status, either inactive or the units for
 * timeToExecute.
 * @param taskid The task ID this event belongs to.
 * @param timeToExecute How long before the event fires, always in milliseconds.
 ******************************************************************************/
typedef struct {
  /** The event's status, either inactive or the units for timeToExecute. */
  EmberEventUnits status;
  /** The task ID this event belongs to. */
  EmberTaskId taskid;
  /** How long before the event fires.
   *  Units are always in milliseconds.
   */
  uint32_t timeToExecute;
} EmberEventControl;

// Testing the token_manager has shown the following two defines
// conflict with the compilations and are not relevant to the
// tests.  Disable them with an #if 0 but leave them here for
// completeness of information.
#if 0
/***************************************************************************//**
 * @brief The `EmberEventData` structure is used to define events within the
 * Ember framework. It consists of a control structure,
 * `EmberEventControl`, which manages the event's status and timing, and
 * a handler function pointer that specifies the procedure to execute
 * when the event is triggered. This structure is typically used in
 * conjunction with event queues to manage and execute scheduled tasks in
 * a Zigbee network.
 *
 * @param control The control structure for the event.
 * @param handler The procedure to call when the event fires.
 ******************************************************************************/
typedef const struct EmberEventData_S {
  /** The control structure for the event. */
  EmberEventControl *control;
  /** The procedure to call when the event fires. */
  void (*handler)(void);
} EmberEventData;

/***************************************************************************//**
 * @brief The `EmberTaskControl` structure is used to manage tasks within the
 * Ember framework. It contains information about when the next event for
 * a task is scheduled to occur, a list of events associated with the
 * task, and a flag indicating whether the task has other activities to
 * perform beyond handling events. This structure is crucial for
 * scheduling and managing task execution in a system that relies on
 * event-driven programming.
 *
 * @param nextEventTime The time when the next event associated with this task
 * will fire.
 * @param events The list of events associated with this task.
 * @param busy A flag that indicates the task has something to do other than
 * events.
 ******************************************************************************/
typedef struct {
  // The time when the next event associated with this task will fire
  uint32_t nextEventTime;
  // The list of events associated with this task
  EmberEventData *events;
  // A flag that indicates the task has something to do other than events
  bool busy;
} EmberTaskControl;
#endif // 0

/** @brief Defines tasks that prevent the stack from sleeping.
 */
enum {
  /** Messages are waiting for transmission. */
  SL_ZIGBEE_OUTGOING_MESSAGES = 0x0001,
  /** One or more incoming messages are processed. */
  SL_ZIGBEE_INCOMING_MESSAGES = 0x0002,
  /** The radio is currently powered on.  On sleepy devices, the radio is
   *  turned off when not in use.  On non-sleepy devices, i.e.,
   *  SL_ZIGBEE_STAR_COORDINATOR, SL_ZIGBEE_STAR_RANGE_EXTENDER,
   *  or SL_ZIGBEE_STAR_END_DEVICE, the radio is always on.
   */
  SL_ZIGBEE_RADIO_IS_ON = 0x0004,
  /** The node is currently trying to associate to a Connect network. */
  SL_ZIGBEE_ASSOCIATING = 0x0008,
  /** The node is currently performing a MAC-level scanning procedure. */
  SL_ZIGBEE_SCANNING = 0x0010,
};

/**
 * @brief Defines the events reported to the application
 * by the sli_zigbee_stack_counter_handler().
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberCounterType
#else
typedef uint8_t EmberCounterType;
enum
#endif
{
  /** Every packet that comes in over the radio (except MAC ACKs). */
  SL_ZIGBEE_COUNTER_PHY_IN_PACKETS,

  /** Every packet that goes out over the radio (except MAC ACKs). */
  SL_ZIGBEE_COUNTER_PHY_OUT_PACKETS,

  /** Incoming MAC unicasts. */
  SL_ZIGBEE_COUNTER_MAC_IN_UNICAST,

  /** Incoming MAC broadcasts. */
  SL_ZIGBEE_COUNTER_MAC_IN_BROADCAST,

  /** Outgoing MAC unicasts that do not require an ACK. */
  SL_ZIGBEE_COUNTER_MAC_OUT_UNICAST_NO_ACK,

  /** Outgoing MAC unicasts that require an ACK for which an ACK
   * was received possibly after retrying. */
  SL_ZIGBEE_COUNTER_MAC_OUT_UNICAST_ACK_SUCCESS,

  /** Outgoing unicasts for which an ACK was never received even
   * after retrying. */
  SL_ZIGBEE_COUNTER_MAC_OUT_UNICAST_ACK_FAIL,

  /** Outgoing MAC packets which were never transmitted because
   * clear channel assessment always returned busy. */
  SL_ZIGBEE_COUNTER_MAC_OUT_UNICAST_CCA_FAIL,

  /** Outgoing unicast retries.  This does not count the initial
   * transmission.  Note a single MAC transmission can result in
   * multiple retries. */
  SL_ZIGBEE_COUNTER_MAC_OUT_UNICAST_RETRY,

  SL_ZIGBEE_COUNTER_MAC_OUT_BROADCAST,

  SL_ZIGBEE_COUNTER_MAC_OUT_BROADCAST_CCA_FAIL,

  SL_ZIGBEE_COUNTER_MAC_OUT_ENCRYPT_FAIL,

  /** Dropped incoming MAC packets (out of memory) */
  SL_ZIGBEE_COUNTER_MAC_DROP_IN_MEMORY,

  /** Dropped incoming MAC packets (invalid frame counter) */
  SL_ZIGBEE_COUNTER_MAC_DROP_IN_FRAME_COUNTER,

  /** Dropped incoming MAC packets (can't decrypt) */
  SL_ZIGBEE_COUNTER_MAC_DROP_IN_DECRYPT,

  /** Outgoing NWK forwarded packets */
  SL_ZIGBEE_COUNTER_NWK_OUT_FORWARDING,

  /** Incoming NWK data frames correctly processed */
  SL_ZIGBEE_COUNTER_NWK_IN_SUCCESS,

  /** Dropped incoming NWK packets (wrong source node) */
  SL_ZIGBEE_COUNTER_NWK_DROP_IN_WRONG_SOURCE,

  /** Dropped incoming NWK packets (can't forward) */
  SL_ZIGBEE_COUNTER_NWK_DROP_IN_FORWARDING,

  /** UART in and out data */
  SL_ZIGBEE_COUNTER_UART_IN_DATA,
  SL_ZIGBEE_COUNTER_UART_IN_MANAGEMENT,
  SL_ZIGBEE_COUNTER_UART_IN_FAIL,
  SL_ZIGBEE_COUNTER_UART_OUT_DATA,
  SL_ZIGBEE_COUNTER_UART_OUT_MANAGEMENT,
  SL_ZIGBEE_COUNTER_UART_OUT_FAIL,

  // Counters for non-packet events below.
  SL_ZIGBEE_COUNTER_ROUTE_2_HOP_LOOP,
  SL_ZIGBEE_COUNTER_BUFFER_ALLOCATION_FAIL,

  /** ASHv3 */
  SL_ZIGBEE_ASH_V3_ACK_SENT,
  SL_ZIGBEE_ASH_V3_ACK_RECEIVED,
  SL_ZIGBEE_ASH_V3_NACK_SENT,
  SL_ZIGBEE_ASH_V3_NACK_RECEIVED,
  SL_ZIGBEE_ASH_V3_RESEND,
  SL_ZIGBEE_ASH_V3_BYTES_SENT,
  SL_ZIGBEE_ASH_V3_TOTAL_BYTES_RECEIVED,
  SL_ZIGBEE_ASH_V3_VALID_BYTES_RECEIVED,
  SL_ZIGBEE_ASH_V3_PAYLOAD_BYTES_SENT,

  /** A placeholder giving the number of Ember counter types. */
  SL_ZIGBEE_COUNTER_TYPE_COUNT,
};

/**
 * @brief  Buffers used by the memory buffer system.
 */
typedef uint16_t EmberBuffer;

/**
 * @brief  Return type for Ember functions.
 */
#ifndef __EMBERSTATUS_TYPE__
#define __EMBERSTATUS_TYPE__
typedef uint8_t EmberStatus;
#endif //__EMBERSTATUS_TYPE__

//------------------------------------------------------------------------------
// INTERNAL TYPES AND DEFINES
//------------------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

typedef uint16_t Buffer;
typedef uint16_t sli_buffer_manager_buffer_t;

#define NULL_BUFFER 0x0000u

/***************************************************************************//**
 * @brief The `EmOutgoingPacket` structure is used to describe an outgoing
 * packet in a Zigbee network. It contains information about the
 * destination address, the endpoint for the message, a tag for matching
 * send calls to handler calls, the payload of the message, and
 * transmission options that dictate how the message should be sent, such
 * as whether it should be encrypted or if an acknowledgment is
 * requested.
 *
 * @param destination A short address indicating the destination of the packet.
 * @param endpoint The endpoint to which the message is destined.
 * @param tag A tag value used to match message send calls to corresponding
 * handler calls.
 * @param payload A buffer containing the message payload.
 * @param txOptions Options used for transmitting the outgoing message, such as
 * encryption or acknowledgment requests.
 ******************************************************************************/
typedef struct {
  sl_802154_short_addr_t destination;
  uint8_t endpoint;
  uint8_t tag;
  Buffer payload;
  sl_802154_message_options_t txOptions;
} EmOutgoingPacket;

enum {
  /* RX status codes */
  SL_ZIGBEE_PACKET_TRACE_RX_STATUS_SUCCESS      = 0x00,
  SL_ZIGBEE_PACKET_TRACE_RX_STATUS_CRC_FAILED   = 0x01,
  SL_ZIGBEE_PACKET_TRACE_RX_STATUS_MAC_FILTERED = 0x02,
  /* TX status codes */
  SL_ZIGBEE_PACKET_TRACE_TX_STATUS_SUCCESS      = 0x80
};

/***************************************************************************//**
 * @brief The EmberMessageTrace structure is used to store information about a
 * message trace in a Zigbee network. It includes fields for the status
 * of the message, the received signal strength indicator (RSSI), the
 * channel on which the message was received, the length of the packet,
 * and a pointer to the packet data itself. This structure is useful for
 * debugging and analyzing message transmissions within the network.
 *
 * @param status A uint8_t representing the status of the message trace.
 * @param rssi An int8_t representing the received signal strength indicator in
 * dBm.
 * @param channel A uint8_t indicating the channel on which the message was
 * received.
 * @param packetLength A sl_802154_message_length_t indicating the length of the
 * packet.
 * @param packet A pointer to a uint8_t array containing the packet data.
 ******************************************************************************/
typedef struct {
  uint8_t status;
  int8_t rssi;
  uint8_t channel;
  sl_802154_message_length_t packetLength;
  uint8_t *packet;
} EmberMessageTrace;

/***************************************************************************//**
 * @brief The `EmberChildEntry` structure is used to store information about a
 * child node in a Zigbee network. It includes fields for tracking the
 * last time the child was seen, its frame counter for security, and both
 * its short and long addresses. Additionally, it contains flags for
 * various child attributes and a leave count to monitor how often the
 * child has left the network. This structure is essential for managing
 * child nodes and ensuring secure communication within the network.
 *
 * @param lastSeen Stores the timestamp of when the child was last seen.
 * @param frameCounter Holds the frame counter for security purposes.
 * @param shortId Represents the short address of the child node.
 * @param longId Represents the long address (EUI64) of the child node.
 * @param flags Contains flags that provide additional information about the
 * child.
 * @param leaveCount Tracks the number of times the child has left the network.
 ******************************************************************************/
typedef struct {
  uint32_t lastSeen;
  uint32_t frameCounter;
  sl_802154_short_addr_t shortId;
  sl_802154_long_addr_t longId;
  uint8_t flags;
  uint8_t leaveCount;
} EmberChildEntry;

typedef uint8_t EmberLibraryStatus;

typedef void (*ClientResyncCallback)(EmberStatus status);

// A library's status is an 8-bit value with information about it.
// The high bit indicates whether the library is present (1), or if it is a
// stub (0).  The lower 7-bits can be used for codes specific to the library.
// This allows a library, like the security library, to specify what additional
// features are present.
// A value of 0xFF is reserved, it indicates an error in retrieving the
// library status.
#define SL_ZIGBEE_LIBRARY_PRESENT_MASK          0x80
#define SL_ZIGBEE_LIBRARY_IS_STUB               0x00
#define SL_ZIGBEE_LIBRARY_ERROR                 0xFF

#endif //DOXYGEN_SHOULD_SKIP_THIS

#endif // __EMBER_TYPES_H__
