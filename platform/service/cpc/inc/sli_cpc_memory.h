/***************************************************************************/ /**
 * @file
 * @brief CPC Internal Memory Functions
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

#ifndef SLI_CPC_MEMORY_H
#define SLI_CPC_MEMORY_H

#include "sl_status.h"

#include "sli_cpc_instance.h"

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
#include "sli_cpc_system_primary.h"
#endif

#if defined(SL_CATALOG_CPC_SECONDARY_PRESENT)
#include "sli_cpc_system_secondary.h"
#endif

#if defined(SL_CATALOG_MEMORY_MANAGER_PRESENT)
#include "sl_memory_manager.h"
#else
#include "sli_mem_pool.h"
#endif

/***************************************************************************//**
 * @brief The `sli_cpc_rx_buffer_t` structure is designed to represent a receive
 * buffer in the CPC (Communication Protocol Controller) system. It
 * contains a pointer to the `sli_cpc_instance_t` that allocated the
 * buffer, allowing for tracking and management of the buffer's lifecycle
 * within the context of a specific CPC instance. The `data` member is a
 * flexible array member, which means it can hold a variable amount of
 * data, making this structure suitable for handling incoming data of
 * varying sizes. This structure is typically used in memory management
 * operations related to receiving data in the CPC system.
 *
 * @param instance Pointer to the instance that allocated the RX buffer.
 * @param data The data itself, stored in a flexible array member.
 ******************************************************************************/
typedef struct {
  sli_cpc_instance_t *instance; ///< Pointer to instance that allocated the RX buffer
  uint8_t            data[];    ///< The data itself
} sli_cpc_rx_buffer_t;

/***************************************************************************//**
 * @brief This function sets up the necessary memory pools and semaphores for
 * managing buffers within a CPC instance. It should be called before any
 * buffer operations are performed on the instance. The function handles
 * different configurations, such as the presence of an RTOS or memory
 * manager, and initializes various memory pools accordingly. It returns
 * a status code indicating success or failure, which should be checked
 * to ensure proper initialization.
 *
 * @param inst A pointer to a `sli_cpc_instance_t` structure representing the
 * CPC instance to initialize. This parameter must not be null, and
 * the instance should be properly allocated before calling this
 * function.
 * @return Returns an `sl_status_t` status code indicating the result of the
 * initialization. Possible return values include `SL_STATUS_OK` for
 * success and `SL_STATUS_ALLOCATION_FAILED` if memory allocation fails.
 ******************************************************************************/
sl_status_t sli_cpc_memory_init(sli_cpc_instance_t *inst);

/***************************************************************************//**
 * @brief This function is used to obtain a handle for performing write
 * operations in a CPC instance. It is particularly useful in
 * environments with an RTOS, where it manages semaphore acquisition
 * based on the specified timeout. The function can operate in blocking
 * or non-blocking mode, depending on the 'block' parameter. It should be
 * called when a write buffer handle is needed, and the caller must
 * ensure that the 'inst' and 'handle' parameters are valid pointers. The
 * function returns a status code indicating the result of the operation.
 *
 * @param inst Pointer to the CPC instance to operate on. Must not be null.
 * @param handle Pointer to a location where the retrieved buffer handle will be
 * stored. Must not be null.
 * @param block Boolean indicating whether the function should block until a
 * buffer is available (true) or return immediately (false).
 * @param timeout Timeout in milliseconds for buffer availability, used only
 * when an RTOS is present and 'block' is true.
 * @return Returns an sl_status_t code indicating the result of the operation,
 * such as success or resource unavailability.
 ******************************************************************************/
sl_status_t sli_cpc_get_write_buffer_handle(sli_cpc_instance_t *inst,
                                            sl_cpc_buffer_handle_t **handle,
                                            bool block,
                                            uint32_t timeout);

/***************************************************************************//**
 * @brief This function retrieves a handle for a CPC buffer based on the
 * specified type. It is essential to call this function when a buffer
 * handle is needed for operations such as transmission or reception
 * within the CPC framework. The function requires a valid instance and a
 * non-null pointer to store the handle. It handles different types of
 * buffer handles and ensures that resources are available before
 * allocation. If resources are insufficient or an invalid type is
 * specified, it returns an appropriate error status.
 *
 * @param inst Pointer to the CPC instance to operate on. Must be a valid,
 * initialized instance.
 * @param handle Address of the variable that will receive the buffer handle
 * pointer. Must not be null.
 * @param type Type of buffer handle to allocate. Must be a valid
 * `sl_cpc_buffer_handle_type_t` value. Invalid types result in an
 * error.
 * @return Returns an `sl_status_t` indicating the result of the operation:
 * `SL_STATUS_OK` on success, `SL_STATUS_NULL_POINTER` if `handle` is
 * null, `SL_STATUS_NO_MORE_RESOURCE` if resources are exhausted, or
 * `SL_STATUS_INVALID_PARAMETER` for an invalid type.
 ******************************************************************************/
sl_status_t sli_cpc_get_buffer_handle(sli_cpc_instance_t *inst,
                                      sl_cpc_buffer_handle_t **handle,
                                      sl_cpc_buffer_handle_type_t type);

/***************************************************************************//**
 * @brief This function retrieves a handle for a CPC buffer intended for
 * receiving data. It optionally allocates a buffer for the HDLC header
 * and the RX buffer based on the provided parameters. This function
 * should be used when a buffer is needed for incoming data, and it
 * ensures that the necessary resources are allocated. It is important to
 * check the return status to handle any potential errors, such as
 * resource exhaustion or null pointer issues.
 *
 * @param inst Pointer to the CPC instance to operate on. Must not be null.
 * @param handle Address of the variable that will receive the buffer handle
 * pointer. Must not be null.
 * @param alloc_rx_buffer Boolean flag indicating whether to allocate the RX
 * buffer. If true, the function attempts to allocate the
 * RX buffer; otherwise, it does not.
 * @return Returns an sl_status_t code indicating the result of the operation.
 * Possible return values include SL_STATUS_OK for success,
 * SL_STATUS_NULL_POINTER if the handle is null, and
 * SL_STATUS_NO_MORE_RESOURCE if resources are exhausted.
 ******************************************************************************/
sl_status_t sli_cpc_get_buffer_handle_for_rx(sli_cpc_instance_t *inst,
                                             sl_cpc_buffer_handle_t **handle,
                                             bool alloc_rx_buffer);

/***************************************************************************//**
 * @brief This function is used to allocate a buffer specifically for a CPC HDLC
 * header, which is necessary for communication protocols using HDLC
 * framing. It should be called when a new HDLC header buffer is needed.
 * The function requires a valid instance of `sli_cpc_instance_t` and a
 * non-null pointer to a `uint8_t*` where the allocated buffer address
 * will be stored. If the allocation is successful, the buffer pointer is
 * set; otherwise, an error status is returned. Ensure that the `header`
 * parameter is not null before calling this function.
 *
 * @param inst A pointer to a `sli_cpc_instance_t` instance, representing the
 * CPC instance to operate on. The instance must be properly
 * initialized before calling this function.
 * @param header A pointer to a `uint8_t*` where the address of the allocated
 * HDLC header buffer will be stored. Must not be null. If null,
 * the function returns `SL_STATUS_NULL_POINTER`.
 * @return Returns `SL_STATUS_OK` if the buffer is successfully allocated and
 * the `header` pointer is set. Returns `SL_STATUS_NULL_POINTER` if
 * `header` is null, and `SL_STATUS_NO_MORE_RESOURCE` if no buffer could
 * be allocated.
 ******************************************************************************/
sl_status_t sli_cpc_get_hdlc_header_buffer(sli_cpc_instance_t *inst, uint8_t **buffer);

/***************************************************************************//**
 * @brief This function is used to obtain a handle to a buffer specifically for
 * transmitting a reject packet in the CPC system. It should be called
 * when a reject packet needs to be sent, ensuring that the necessary
 * buffer resources are allocated. The function requires a valid CPC
 * instance and provides the buffer handle through an output parameter.
 * If the buffer cannot be allocated due to resource constraints, an
 * appropriate error status is returned.
 *
 * @param inst Pointer to a valid sli_cpc_instance_t structure representing the
 * CPC instance to operate on. Must not be null.
 * @param handle Pointer to a pointer of type sl_cpc_buffer_handle_t where the
 * function will store the address of the allocated buffer handle.
 * Must not be null.
 * @return Returns an sl_status_t indicating the success or failure of the
 * operation. SL_STATUS_OK is returned on success, while
 * SL_STATUS_NO_MORE_RESOURCE is returned if resources are insufficient
 * to allocate the buffer.
 ******************************************************************************/
sl_status_t sli_cpc_get_reject_buffer(sli_cpc_instance_t *inst, sl_cpc_buffer_handle_t **handle);

/***************************************************************************//**
 * @brief This function allocates a raw receive buffer for a given CPC instance
 * and provides a pointer to it. It is typically used when a new buffer
 * is needed for receiving data. The function must be called with a valid
 * CPC instance, and the caller is responsible for managing the allocated
 * buffer, including freeing it when no longer needed. If the allocation
 * fails due to resource constraints, the function returns an error
 * status.
 *
 * @param inst A pointer to a valid sli_cpc_instance_t structure representing
 * the CPC instance for which the buffer is being allocated. Must
 * not be null.
 * @param raw_rx_buffer A pointer to a uint8_t pointer where the address of the
 * allocated raw RX buffer will be stored. Must not be
 * null.
 * @return Returns SL_STATUS_OK on successful allocation, or
 * SL_STATUS_NO_MORE_RESOURCE if no buffer could be allocated.
 ******************************************************************************/
sl_status_t sli_cpc_get_raw_rx_buffer(sli_cpc_instance_t *inst, uint8_t **raw_rx_buffer);

/***************************************************************************//**
 * @brief This function is used to release a previously allocated CPC RAW RX
 * buffer. It should be called when the buffer is no longer needed to
 * free up resources. The function requires a valid pointer to the
 * buffer; if the pointer is null, it returns an error status. This
 * function is essential for managing memory efficiently in applications
 * using CPC buffers.
 *
 * @param inst Pointer to the CPC instance associated with the buffer. The
 * function does not use this parameter, but it must be provided.
 * @param raw_rx_buffer Pointer to the RAW RX buffer to be freed. Must not be
 * null. If null, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK if the buffer is successfully freed, or
 * SL_STATUS_NULL_POINTER if the buffer pointer is null.
 ******************************************************************************/
sl_status_t sli_cpc_free_raw_rx_buffer(sli_cpc_instance_t *inst, uint8_t *raw_rx_buffer);

/***************************************************************************//**
 * @brief This function is used to release a CPC buffer handle that is no longer
 * needed. It should be called when the buffer handle is no longer in use
 * and its reference count is zero. The function checks for null pointers
 * and ensures that the handle is not in use elsewhere before proceeding
 * with the free operation. It handles different types of buffer handles
 * appropriately, freeing associated resources such as HDLC headers and
 * security tags if necessary. The function also updates internal
 * counters and may notify the driver if a receive buffer handle is
 * freed. It is important to ensure that the handle is valid and not in
 * use before calling this function to avoid errors.
 *
 * @param handle A pointer to the sl_cpc_buffer_handle_t structure to be freed.
 * Must not be null. The handle's reference count must be zero,
 * indicating it is not in use elsewhere. If the handle is null,
 * the function returns SL_STATUS_NULL_POINTER. If the handle is
 * still in use, indicated by a non-zero reference count, the
 * function returns SL_STATUS_BUSY.
 * @return Returns an sl_status_t value indicating the result of the operation:
 * SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if the handle is
 * null, or SL_STATUS_BUSY if the handle is still in use.
 ******************************************************************************/
sl_status_t sli_cpc_free_buffer_handle(sl_cpc_buffer_handle_t *handle);

/***************************************************************************//**
 * @brief This function is used to allocate a queue item and push a data buffer
 * into the CPC receive queue, then free the associated header and buffer
 * handle. It should be called when you have a buffer handle containing
 * received data that needs to be queued for processing. The function
 * requires valid pointers for the instance, handle, and head parameters.
 * If the handle or head is null, the function returns an error status.
 * The data length must accurately reflect the size of the data in the
 * handle. After execution, the buffer is owned by the user, and the
 * handle is freed.
 *
 * @param inst Pointer to the CPC instance to operate on. Must not be null.
 * @param handle Pointer to the buffer handle containing the data to be pushed.
 * Must not be null. The function will free this handle.
 * @param head Pointer to the head of the receive queue where the data will be
 * pushed. Must not be null.
 * @param data_length The length of the data in the handle. Must accurately
 * reflect the size of the data.
 * @return Returns a status code indicating the result of the operation, such as
 * success or specific error conditions.
 ******************************************************************************/
sl_status_t sli_cpc_push_back_rx_data_in_receive_queue(sli_cpc_instance_t *inst,
                                                       sl_cpc_buffer_handle_t *handle,
                                                       sl_slist_node_t **head,
                                                       uint16_t data_length);

/***************************************************************************//**
 * @brief This function is used to release a previously allocated RX buffer that
 * has not been added to the RX queue. It should be called when the
 * buffer is no longer needed to ensure proper memory management and
 * resource availability. The function requires a valid instance pointer
 * and a non-null data pointer. If the data pointer is null, the function
 * returns an error status indicating a null pointer. Upon successful
 * execution, it notifies that at least one RX buffer is available for
 * future use.
 *
 * @param inst Pointer to the CPC instance associated with the RX buffer. Must
 * not be null, and should match the instance that originally
 * allocated the buffer.
 * @param data Pointer to the data buffer to be freed. Must not be null. If
 * null, the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on successful freeing of the buffer, or
 * SL_STATUS_NULL_POINTER if the data parameter is null.
 ******************************************************************************/
sl_status_t sli_cpc_free_rx_buffer(sli_cpc_instance_t *inst, void *data);

/***************************************************************************//**
 * @brief This function is used to release a previously allocated HDLC header
 * back to the memory pool associated with the given CPC instance. It
 * should be called when the header is no longer needed to ensure
 * efficient memory management. The function requires a valid instance
 * pointer and a non-null header pointer. If the header pointer is null,
 * the function returns an error status indicating a null pointer was
 * provided.
 *
 * @param inst Pointer to the CPC instance that manages the memory pool. Must
 * not be null.
 * @param header Pointer to the HDLC header to be freed. Must not be null. If
 * null, the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on success or SL_STATUS_NULL_POINTER if the
 * header is null.
 ******************************************************************************/
sl_status_t sli_cpc_free_hdlc_header(sli_cpc_instance_t *inst, void *header);

/***************************************************************************//**
 * @brief This function is used to obtain a receive queue item from a specified
 * CPC instance. It is essential to call this function when you need to
 * handle incoming data in the CPC system. The function requires a valid
 * instance pointer and a non-null pointer to store the receive queue
 * item. If the item pointer is null, the function returns an error
 * status indicating a null pointer. If no more resources are available
 * to allocate a new item, it returns a status indicating resource
 * exhaustion. Successful execution results in the item pointer being
 * populated with a valid receive queue item.
 *
 * @param inst A pointer to the CPC instance from which to retrieve the receive
 * queue item. Must not be null.
 * @param item A pointer to a pointer where the function will store the address
 * of the allocated receive queue item. Must not be null. If null,
 * the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if the item
 * pointer is null, or SL_STATUS_NO_MORE_RESOURCE if no resources are
 * available.
 ******************************************************************************/
sl_status_t sli_cpc_get_receive_queue_item(sli_cpc_instance_t *inst, sl_cpc_receive_queue_item_t **item);

/***************************************************************************//**
 * @brief This function is used to release a receive queue item and its
 * associated data buffer back to the memory pool. It should be called
 * when the item is no longer needed to ensure proper memory management
 * and avoid memory leaks. The function checks if the item is null and
 * returns immediately if so, ensuring safe operation even if a null
 * pointer is passed. It is important to ensure that the item has been
 * fully processed before calling this function to avoid data loss.
 *
 * @param inst A pointer to the CPC instance associated with the receive queue
 * item. Must not be null, as it is used to access the memory pool
 * for freeing the item.
 * @param item A pointer to the receive queue item to be freed. Can be null, in
 * which case the function does nothing. If not null, the function
 * assumes ownership and will free the associated data buffer if it
 * exists.
 * @return None
 ******************************************************************************/
void sli_cpc_free_receive_queue_item(sli_cpc_instance_t *inst, sl_cpc_receive_queue_item_t *item);

/***************************************************************************//**
 * @brief This function is used to move a receive queue item to a postponed list
 * within a CPC instance, while also extracting the data and its length
 * from the item. It is typically called when a receive queue item needs
 * to be deferred for later processing. The function requires valid
 * pointers for the instance, the receive queue item, and the output
 * parameters for data and data length. It is important to ensure that
 * these pointers are not null before calling the function to avoid
 * undefined behavior.
 *
 * @param inst Pointer to the CPC instance to operate on. Must not be null.
 * @param rx_queue_item Pointer to the receive queue item to be postponed. Must
 * not be null.
 * @param data Pointer to a variable where the function will store the pointer
 * to the data buffer. Must not be null.
 * @param data_length Pointer to a variable where the function will store the
 * length of the data. Must not be null.
 * @return Returns SL_STATUS_OK on successful execution.
 ******************************************************************************/
sl_status_t sli_cpc_push_receive_queue_item_to_postponed_list(sli_cpc_instance_t *inst,
                                                              sl_cpc_receive_queue_item_t *rx_queue_item,
                                                              void **data,
                                                              uint16_t *data_length);

/***************************************************************************//**
 * @brief This function allocates memory for a CPC endpoint and returns a
 * pointer to it through the provided output parameter. It should be
 * called when a new endpoint is needed for communication. The function
 * requires a valid instance pointer and a non-null pointer to store the
 * endpoint. If the allocation fails due to insufficient resources, an
 * appropriate error status is returned. Ensure that the endpoint pointer
 * is checked for null before calling this function to avoid errors.
 *
 * @param inst Pointer to a valid sli_cpc_instance_t structure. The caller
 * retains ownership and must ensure it is properly initialized
 * before calling this function.
 * @param endpoint Pointer to a pointer of type sl_cpc_endpoint_t where the
 * allocated endpoint will be stored. Must not be null. If null,
 * the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on successful allocation,
 * SL_STATUS_NO_MORE_RESOURCE if allocation fails, or
 * SL_STATUS_NULL_POINTER if the endpoint parameter is null.
 ******************************************************************************/
sl_status_t sli_cpc_get_endpoint(sli_cpc_instance_t *inst, sl_cpc_endpoint_t **endpoint);

/***************************************************************************//**
 * @brief This function is used to release the resources associated with a CPC
 * endpoint. It should be called when an endpoint is no longer needed to
 * ensure that memory is properly freed. The function requires a valid
 * endpoint pointer and must be called with a non-null endpoint. It is
 * important to ensure that the endpoint is not used after this function
 * is called, as it will have been deallocated.
 *
 * @param inst Pointer to the CPC instance associated with the endpoint. The
 * caller retains ownership and it must be a valid pointer.
 * @param endpoint Pointer to the endpoint to be freed. Must not be null. The
 * function will deallocate the memory associated with this
 * endpoint.
 * @return None
 ******************************************************************************/
void sli_cpc_free_endpoint(sli_cpc_instance_t *inst, sl_cpc_endpoint_t *endpoint);

#if defined(SL_CATALOG_CPC_PRIMARY_PRESENT)
/***************************************************************************//**
 * @brief This function is used to obtain a handle for system commands
 * associated with a given CPC instance. It is essential to call this
 * function when a system command handle is needed for operations within
 * the CPC framework. The function requires a valid instance pointer and
 * a non-null pointer to store the handle. If the handle cannot be
 * allocated due to resource constraints, an appropriate status code is
 * returned. This function should be used in environments where the CPC
 * primary catalog is present.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance to operate on. This parameter must not be null.
 * @param item A pointer to a pointer of type sli_cpc_system_command_handle_t
 * where the allocated handle will be stored. This parameter must
 * not be null, and if it is, the function returns
 * SL_STATUS_NULL_POINTER.
 * @return Returns an sl_status_t value indicating the result of the operation:
 * SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if the item parameter
 * is null, or SL_STATUS_NO_MORE_RESOURCE if no handle could be
 * allocated.
 ******************************************************************************/
sl_status_t sli_cpc_get_system_command_handle(sli_cpc_instance_t *inst, sli_cpc_system_command_handle_t **item);

/***************************************************************************//**
 * @brief This function is used to release a system command handle that was
 * previously allocated for a CPC instance. It should be called when the
 * handle is no longer needed to ensure proper memory management. The
 * function requires a valid instance pointer and a non-null handle
 * pointer. If the handle pointer is null, the function returns an error
 * status indicating a null pointer was provided. Otherwise, it
 * successfully frees the handle and returns a success status.
 *
 * @param inst A pointer to the CPC instance associated with the system command
 * handle. Must not be null.
 * @param item A pointer to the system command handle to be freed. Must not be
 * null. If null, the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on success or SL_STATUS_NULL_POINTER if the item
 * is null.
 ******************************************************************************/
sl_status_t sli_cpc_free_system_command_handle(sli_cpc_instance_t *inst, sli_cpc_system_command_handle_t *item);
#endif

#if defined(SL_CATALOG_CPC_SECONDARY_PRESENT)
/***************************************************************************//**
 * @brief This function is used to allocate a system command context associated
 * with a given CPC instance. It should be called when a new system
 * command context is needed for operations within the CPC framework. The
 * function requires a valid instance pointer and a non-null pointer to
 * store the allocated context. If the allocation is successful, the
 * context is initialized and ready for use. If the allocation fails due
 * to resource constraints, an appropriate error status is returned. This
 * function must be used in environments where the CPC secondary is
 * present.
 *
 * @param inst A pointer to a sli_cpc_instance_t structure representing the CPC
 * instance. This must be a valid, initialized instance.
 * @param item A pointer to a pointer of type sli_cpc_system_cmd_context_t where
 * the allocated context will be stored. Must not be null. If null,
 * the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on successful allocation and initialization of
 * the system command context. Returns SL_STATUS_NULL_POINTER if the
 * item parameter is null, and SL_STATUS_NO_MORE_RESOURCE if no more
 * resources are available for allocation.
 ******************************************************************************/
sl_status_t sli_cpc_get_system_command_context(sli_cpc_instance_t *inst, sli_cpc_system_cmd_context_t **item);

/***************************************************************************//**
 * @brief This function is used to release a system command context that was
 * previously allocated for a CPC instance. It should be called when the
 * system command context is no longer needed to free up resources. The
 * function requires a valid instance pointer and a non-null system
 * command context pointer. If the context pointer is null, the function
 * returns an error status indicating a null pointer was provided. This
 * function is typically used in environments where CPC system command
 * contexts are dynamically managed and need to be explicitly freed to
 * prevent memory leaks.
 *
 * @param inst A pointer to the CPC instance associated with the system command
 * context. Must not be null.
 * @param item A pointer to the system command context to be freed. Must not be
 * null. If null, the function returns SL_STATUS_NULL_POINTER.
 * @return Returns SL_STATUS_OK on successful freeing of the context, or
 * SL_STATUS_NULL_POINTER if the item is null.
 ******************************************************************************/
sl_status_t sli_cpc_free_system_command_context(sli_cpc_instance_t *inst, sli_cpc_system_cmd_context_t *item);
#endif

/***************************************************************************//**
 * @brief This function is used to allocate a buffer for storing a security tag
 * associated with a CPC instance. It should be called when a security
 * tag buffer is needed, and the security feature is enabled. The
 * function requires a valid instance pointer and a non-null pointer to
 * store the allocated buffer. If the security feature is not enabled,
 * the function will return a status indicating that the feature is not
 * available. Proper error handling should be implemented to manage cases
 * where the buffer cannot be allocated due to resource constraints.
 *
 * @param inst Pointer to the CPC instance to operate on. Must be a valid,
 * initialized instance.
 * @param tag_buffer Address of the variable that will receive the pointer to
 * the allocated security tag buffer. Must not be null.
 * @return Returns an sl_status_t code indicating the result of the operation:
 * SL_STATUS_OK on success, SL_STATUS_NULL_POINTER if tag_buffer is
 * null, SL_STATUS_NO_MORE_RESOURCE if resources are exhausted, or
 * SL_STATUS_NOT_AVAILABLE if the security feature is not enabled.
 ******************************************************************************/
sl_status_t sli_cpc_get_security_tag_buffer(sli_cpc_instance_t *inst, void **tag_buffer);

/***************************************************************************//**
 * @brief This function is used to release a security tag buffer that was
 * previously allocated for a CPC instance. It should be called when the
 * buffer is no longer needed to free up resources. The function requires
 * that the security feature is enabled; otherwise, it will return a
 * status indicating that the feature is not available. It is important
 * to ensure that the `tag_buffer` is not null before calling this
 * function, as passing a null pointer will result in an error status.
 *
 * @param inst A pointer to the CPC instance associated with the security tag
 * buffer. The caller retains ownership and must ensure it is a
 * valid instance.
 * @param tag_buffer A pointer to the security tag buffer to be freed. Must not
 * be null; otherwise, the function returns an error status.
 * @return Returns an `sl_status_t` indicating the result of the operation:
 * `SL_STATUS_OK` on success, `SL_STATUS_NULL_POINTER` if `tag_buffer`
 * is null, or `SL_STATUS_NOT_AVAILABLE` if the security feature is
 * disabled.
 ******************************************************************************/
sl_status_t sli_cpc_free_security_tag_buffer(sli_cpc_instance_t *inst, void *tag_buffer);

/***************************************************************************//**
 * @brief This function is used to add a buffer handle to the end of a singly
 * linked list, which is typically used for managing buffer handles
 * within the CPC core. It increments the reference count of the buffer
 * handle, ensuring it does not exceed a maximum of two. This function
 * should be used when you need to maintain a list of buffer handles and
 * ensure that each handle is properly referenced. It is important to
 * ensure that the buffer handle is not null before calling this
 * function.
 *
 * @param head A pointer to the head of the singly linked list where the buffer
 * handle will be added. The list is modified to include the new
 * buffer handle at the end.
 * @param buf_handle A pointer to the buffer handle to be added to the list.
 * Must not be null. The function increments its reference
 * count, which must not exceed two.
 * @return None
 ******************************************************************************/
void sli_cpc_push_back_core_buffer_handle(sl_slist_node_t **head, sl_cpc_buffer_handle_t *buf_handle);

/***************************************************************************//**
 * @brief This function is used to retrieve and remove the first buffer handle
 * from a singly linked list that is managed by the CPC core. It is
 * typically called when a buffer handle is needed for further
 * processing. The function expects a pointer to the head of the list,
 * and it will return a pointer to the buffer handle if the list is not
 * empty. If the list is empty, it returns NULL. The caller must ensure
 * that the list head is valid and properly initialized before calling
 * this function.
 *
 * @param head A pointer to the head of the singly linked list from which the
 * buffer handle will be popped. The pointer must not be null, and
 * the list should be properly initialized.
 * @return Returns a pointer to the popped sl_cpc_buffer_handle_t if successful,
 * or NULL if the list is empty.
 ******************************************************************************/
/***************************************************************************//**
 * @brief The `sli_cpc_pop_core_buffer_handle` function is designed to pop a
 * buffer handle from a singly linked list owned by the CPC core. It
 * takes a pointer to the head of the list as an argument and returns a
 * pointer to the `sl_cpc_buffer_handle_t` that was removed from the
 * list.
 *
 * @details This function is used to retrieve and remove the first buffer handle
 * from a list managed by the CPC core.
 ******************************************************************************/
sl_cpc_buffer_handle_t* sli_cpc_pop_core_buffer_handle(sl_slist_node_t **head);

/***************************************************************************//**
 * @brief This function is used to remove a specified buffer handle from a
 * singly linked list and decrement its reference count. It is essential
 * to ensure that the buffer handle is not null and that its reference
 * count is greater than zero before calling this function. This function
 * is typically used in scenarios where buffer handles are managed in a
 * list and need to be removed when they are no longer needed. The
 * function does not handle cases where the buffer handle is invalid or
 * the reference count is zero, so these conditions must be checked by
 * the caller.
 *
 * @param head A pointer to the head of the singly linked list from which the
 * buffer handle will be removed. The list must be valid and
 * properly initialized.
 * @param buffer_handle A pointer to the buffer handle to be removed. Must not
 * be null and must have a reference count greater than
 * zero. The caller is responsible for ensuring these
 * conditions are met.
 * @return None
 ******************************************************************************/
void sli_cpc_remove_core_buffer_handle(sl_slist_node_t **head, sl_cpc_buffer_handle_t *buffer_handle);

/***************************************************************************//**
 * @brief This function is used to add a buffer handle to the head of a singly
 * linked list, which is managed by the CPC core. It is important to
 * ensure that the buffer handle is not null before calling this
 * function. The function increments the reference count of the buffer
 * handle and asserts that it does not exceed a certain limit, ensuring
 * proper management of buffer handle references. This function should be
 * used when you need to manage buffer handles in a list structure,
 * particularly in scenarios involving CPC core operations.
 *
 * @param head A pointer to the head of the singly linked list where the buffer
 * handle will be pushed. The list is expected to be managed by the
 * CPC core, and the caller retains ownership of the list.
 * @param buf_handle A pointer to the buffer handle to be pushed onto the list.
 * Must not be null. The function will increment the reference
 * count of this buffer handle, and it asserts that the
 * reference count does not exceed 2.
 * @return None
 ******************************************************************************/
void sli_cpc_push_core_buffer_handle(sl_slist_node_t **head, sl_cpc_buffer_handle_t *buf_handle);

/***************************************************************************//**
 * @brief This function retrieves and returns the first buffer handle from a
 * singly linked list managed by the CPC driver. It should be used when
 * you need to access and manipulate the buffer handle at the head of the
 * list. The function decreases the reference count of the buffer handle
 * before returning it. If the list is empty, the function returns NULL.
 * Ensure that the list is properly initialized and populated before
 * calling this function.
 *
 * @param head A pointer to the head of the singly linked list from which the
 * buffer handle will be popped. The pointer must not be null, and
 * it should point to a valid list structure. If the list is empty,
 * the function will return NULL.
 * @return Returns a pointer to the popped sl_cpc_buffer_handle_t if successful,
 * or NULL if the list is empty.
 ******************************************************************************/
/***************************************************************************//**
 * @brief The `sli_cpc_pop_driver_buffer_handle` function is designed to pop a
 * buffer handle from a list owned by the CPC driver. It takes a pointer
 * to the head of a singly linked list (`sl_slist_node_t **head`) and
 * returns a pointer to a `sl_cpc_buffer_handle_t` structure.
 *
 * @details This function is used to retrieve and remove the first buffer handle
 * from a list managed by the CPC driver.
 ******************************************************************************/
sl_cpc_buffer_handle_t* sli_cpc_pop_driver_buffer_handle(sl_slist_node_t **head);

/***************************************************************************//**
 * @brief This function is used to remove a specified buffer handle from a
 * singly linked list that is managed by the CPC driver. It decrements
 * the reference count of the buffer handle before removing it from the
 * list. This function should be called when a buffer handle is no longer
 * needed in the list, ensuring that the reference count is greater than
 * zero before calling. It is important to ensure that the buffer handle
 * is valid and part of the list before attempting to remove it.
 *
 * @param head A pointer to the head of the singly linked list from which the
 * buffer handle will be removed. The list must be valid and
 * properly initialized.
 * @param buffer_handle A pointer to the buffer handle to be removed from the
 * list. Must not be null and must have a reference count
 * greater than zero. The caller is responsible for
 * ensuring the buffer handle is part of the list.
 * @return None
 ******************************************************************************/
void sli_cpc_remove_driver_buffer_handle(sl_slist_node_t **head, sl_cpc_buffer_handle_t *buffer_handle);

/***************************************************************************//**
 * @brief This function appends a buffer handle to the end of a singly linked
 * list. It is used to manage buffer handles within the CPC driver,
 * ensuring that the buffer handle's reference count is incremented
 * before being added to the list. The function assumes that the buffer
 * handle is not null and that its reference count does not exceed a
 * predefined limit. It is important to ensure that the buffer handle is
 * valid and properly initialized before calling this function.
 *
 * @param head A pointer to the head of the singly linked list where the buffer
 * handle will be appended. The list is modified in place, and the
 * caller retains ownership.
 * @param buf_handle A pointer to the buffer handle to be added to the list.
 * Must not be null, and its reference count must not exceed
 * the maximum allowed value. The function increments the
 * reference count of the buffer handle.
 * @return None
 ******************************************************************************/
void sli_cpc_push_back_driver_buffer_handle(sl_slist_node_t **head, sl_cpc_buffer_handle_t *buf_handle);

/***************************************************************************//**
 * @brief This function is used to add a buffer handle to a singly linked list,
 * which is managed by the CPC driver. It is typically called when a
 * buffer handle needs to be queued for further processing or reuse. The
 * function increments the reference count of the buffer handle, ensuring
 * it does not exceed a maximum of two. This function should be used when
 * managing buffer handles in a driver context, and it assumes that the
 * buffer handle is valid and not null.
 *
 * @param head A pointer to the head of the singly linked list where the buffer
 * handle will be pushed. The list is modified to include the new
 * buffer handle.
 * @param buf_handle A pointer to the buffer handle to be pushed onto the list.
 * Must not be null, and the function will assert if it is.
 * The reference count of this buffer handle is incremented,
 * and it must not exceed two.
 * @return None
 ******************************************************************************/
void sli_cpc_push_driver_buffer_handle(sl_slist_node_t **head, sl_cpc_buffer_handle_t *buf_handle);

#endif // SLI_CPC_MEMORY_H
