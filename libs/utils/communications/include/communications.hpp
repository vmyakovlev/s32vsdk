/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#ifndef PLATFORMCOMMUNICATIONS_HPP
#define PLATFORMCOMMUNICATIONS_HPP


/***************************************************************************/
/* Include files. */
#include "global_errors.h"
#include <stdint.h>

//**************************************************************************/
// Literals & definitions
const int32_t gcMsgMgrNumberOfSupportedPriorities = 3;


/**
 * \brief This struct holds the common data shared by every message sent in our framework.
 *
 * How to define a new message type:
 * <ol><li> Reserve a message id range from the file "GlobalMessageIds.hpp".
 * </li><li> Allocate a message id from within this range for your new message type.
 * </li><li> Define a child class which inherits from "MessageBase".
 * </li><li> Ensure that both the message id and type are in a common header file visible to all system actors handling it.
 * </li></ol>
 *
 *  
 */
struct MessageBase
{
   int32_t mMessageId;      ///< Unique identifier of the message type.
                             ///< Mandatory: used by receiver to determine the message type.
   int32_t mSenderId;       ///< Return queue id for reply message.
                             ///< Optional: only used when a reply is expected.
   int32_t mCorrelationId;  ///< Binds a request with a particular reply.
                             ///< Optional: only used when a reply is expected.
   int32_t mPriority;       ///< Value of the "cPriority" argument passed to "MsgMgr_MessageSend".
};



//**************************************************************************/
// Module Function Prototypes

/**
 *  \file communications.hpp
 *  \brief Interface for messaging system
 *  \addtogroup MsgMgr Messaging Manager
 *  @{
 *  \details
 * This interface contains functions used for interprocess communications.
 * The primary mechanism offered, are message queues for sending and receiving messages.
 * The queues are unidirectional, asynchronous, indirect and pass by reference.
 * For bidirectional communications, two message queues are needed.
 * The sender does not suspend if the receiver is not ready.
 * Messages are sent to the queue, not directly to the task.
 * A pointer to the original message is sent to the receiver; responsibility for deallocating
 * the message passes from sender to receiver.
 *
 * Mandatory Function Call Ordering Sequence:
 * <ol><li>#MsgMgr_Initialize to initialize this module
 * </li><li>#MsgMgr_MessageQueueCreate to create a message queue.
 * </li><li>optionally: #MsgMgr_NameRegister to map a human readable name to the message queue id
 * </li><li>#MsgMgr_MessageReceive to receive a message.
 * </li><li>optionally: #MsgMgr_NameLookup to find the message queue id based on a human readable name.
 * </li><li>#MsgMgr_MessageSend to send a message
 * </li><li>optionally: #MsgMgr_NameUnregister to remove a mapping to a message queue id
 * </li><li>#MsgMgr_MessageQueueDestroy to release all resources for that message queue.
 * </li></ol> 
 *
*/



/***************************************************************************/
/**
 * \details
 * This function is used to initialize this module.
 * It must be called prior to using this module.
 * It need only be called once.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_Initialize();



/***************************************************************************/
/**
 * \details
 * This function creates an intertask communications endpoint which can be used to send and receive messages.
 *
 *
 * \param  rReturnedInstanceId - [out] Unique identifier used for future reference to this message queue;
 *                               only valid if result is "LIB_SUCCESS".
 *
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessageQueueCreate(int32_t& rReturnedInstanceId);



/***************************************************************************/
/**
 * \details
 * This function destroys a previously created message queue.
 * The message queue must be empty prior to calling this function.
 *
 *
 * \param  cInstanceId - [in] Instance Id of a previously created, still existing message queue.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessageQueueDestroy(const int32_t cInstanceId);



/***************************************************************************/
/**
 * \details
 * This function places a message at the back of the specified queue.
 * All queued messages are of the type "MessageBase" or a child of.
 * If the function returns success; responsibility for deallocating the message passes to the receiver.
 *
 *
 * \param  cReceiverQueueId    - [in] Unique identifier of the receiving message queue
 * \param  cpMessageToBeSent   - [in] Pointer to the buffer containing the message you want to send
 * \param  cMessageSizeInBytes - [in] The number of bytes to send.
 * \param  cPriority           - [in] Priority of the message, valid values are 0,1,2.
 *                               Lower numerical values indicate higher priority.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_INVALID_POINTER  cpMessageToBeSent is an invalid value
 *    - #LIB_INVALID_SIZE     cMessageSizeInBytes is an invalid value
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessageSend(const int32_t cReceiverQueueId,
                              MessageBase* const cpMessageToBeSent,
                              const int32_t cMessageSizeInBytes,
                              const int32_t cPriority);



/***************************************************************************/
/**
 * \details
 * This function retrieves a message from the specified queue.
 * Oldest messages with the highest priority are received first.
 * If the function returns success; responsibility for deallocating the message passes to the caller.
 *
 *
 * \param  cQueueId                      - [int] Unique identifier which references the local tasks queue
 * \param  rpMessageToBeReceived         - [out] Pointer to the buffer containing the message;
 *                                         only valid if result is "LIB_SUCCESS".
 *                                         Buffer SHALL be deallocated using the function "OAL_MemoryFree".
 * \param  rReturnedMessageSizeInBytes   - [out] The size in bytes of the buffer containing the message.
 * \param  cSuspensionTimeInMicroseconds - [in] Period of time to suspend the local task if the queue is empty. 
 *                                         If value is 0 function returns immediately event if no message is received.
 *                                         If value is 0xFFFFFFFF function will wait indefinitely until message is received.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #OAL_ERR_SEM_ETIMEOUT - Function timed out with no message received.
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessageReceive(const int32_t cQueueId,
                                 MessageBase*&  rpMessageToBeReceived,
                                 int32_t&      rReturnedMessageSizeInBytes,
                                 const int32_t cSuspensionTimeInMicroseconds);



/***************************************************************************/
/**
 * \details
 * This function tries to retrieve a message from the specified queue.
 * Oldest message of the specified priority shall be returned.
 * This function variant DOES NOT suspend.
 * If the function returns success; responsibility for deallocating the message passes to the caller.
 * All messages WILL be allocated using the function "OAL_MemoryAlloc".
 * All messages SHALL be deallocated using the function "OAL_MemoryFree".
 *
 *
 * \param  cQueueId                      - [in] Unique identifier which references the local tasks queue
 * \param  rpMessageToBeReceived         - [out] Pointer to the buffer containing the message;
 *                                         only valid if result is "LIB_SUCCESS".
 *                                         Buffer SHALL be deallocated using the function "OAL_MemoryFree".
 * \param  rReturnedMessageSizeInBytes   - [out] The size in bytes of the buffer containing the message.
 * \param  cPriority                     - [in] Priority of the message, valid values are 0,1,2.
 *                                         Lower numerical values indicate higher priority.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessageReceivePriority(const int32_t cQueueId,
                                         MessageBase*&  rpMessageToBeReceived,
                                         int32_t&      rReturnedMessageSizeInBytes,
                                         const int32_t cPriority);



/***************************************************************************/
/**
 * \details
 * This function peeks at the oldest message of the specified priority from the specified queue.
 * Returned message shall be READ ONLY and SHALL NOT BE FREED by the caller.
 *
 * \param  cQueueId                      - [in] Unique identifier which references the local tasks queue
 * \param  rpMessageToBePeeked           - [out] Pointer to the buffer containing the message;
 *                                         only valid if result is "LIB_SUCCESS".
 *                                         Buffer SHALL NOT BE FREED.
 * \param  rReturnedMessageSizeInBytes   - [out] The size in bytes of the buffer containing the message.
 * \param  cPriority                     - [in] Priority of the message, valid values are 0,1,2;
 *                                         lower numerical values indicate higher priority.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessagePeekPriority(const int32_t cQueueId,
                                      const MessageBase*& rpMessageToBePeeked,
                                      int32_t& rReturnedMessageSizeInBytes,
                                      const int32_t cPriority);



/***************************************************************************/
/**
 * \details
 * This function enables/disables sends and receives from a queue.
 * If sends are enabled, calls to MsgMgr_MessageSend should succeed.
 * If sends are disable, calls to MsgMgr_MessageSend which should succeed instead return LIB_SENDING_NOT_ALLOWED.
 *
 * Only supported value for cIsReceivingAllowed is currently true.
 * if user passes false for cIsReceivingAllowed, function will return LIB_FAILURE
 *
 * \param  cQueueId              - [in] Unique identifier which references the local tasks queue
 * \param  cIsSendingAllowed     - [in] Enables/Disables sending from the queue
 * \param  cIsReceivingAllowed   - [in] Enables/Disables receiving from the queue
 *
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessageQueueSetFlowControl(const int32_t cQueueId,
                                             const bool cIsSendingAllowed,
                                             const bool cIsReceivingAllowed);



/***************************************************************************/
/**
 * \details
 * This function maps a human readable name, a cstring, to a message queue id.
 * All registered names must be unique.
 * 
 *
 * \param  cName            - [in] Name to associate with the queue id. It is a null terminated string.
 *                            Maximum name length is 31 characters, excluding null termination.
 * \param  cNameArrayLength - [in] Length in characters of the "cName" character array.
 * \param  cQueueId         - [in] Unique identifier of an existing message queue.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_ERROR_NAME_ALREADY_REGISTERED
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_NameRegister(const char   cName[],
                               const int32_t cNameArrayLength,
                               const int32_t cQueueId);



/***************************************************************************/
/**
 * \details
 * This function removes a mapping between a message queue id and a name.
 * 
 *
 * \param  cName            - [in] Name to unregister. It is a null terminated string.
 * \param  cNameArrayLength - [in] Length in characters of the "cName" character array.
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_NameUnregister(const char   cName[],
                                 const int32_t cNameArrayLength);



/***************************************************************************/
/**
 * \details
 * This function returns the message queue id mapped to "cName".
 * 
 *
 * \param  cName            - [in] Name to look for; it is a null terminated string.
 * \param  cNameArrayLength - [in] Length in characters of the "cName" character array
 * \param  rReturnedQueueId - [out] Unique identifier of the found message queue; 
 *                            only valid if result is "LIB_SUCCESS".
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_NameLookup(const char   cName[],
                             const int32_t cNameArrayLength,
                             int32_t& rReturnedQueueId);



/***************************************************************************/
/**
 * \details
 * This function returns the name mapped to "cQueueId".
 * 
 *
 * \param  cQueueId               - [int] Unique identifier of an existing message queue
 * \param  cpReturnedName         - [in/out] Pointer to a buffer where the name shall be written;
 *                                  only valid if result is "LIB_SUCCESS".
 * \param  cNameBufferSizeInBytes - [in] Size in bytes of the buffer pointed to by "cpReturnedName"
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_NameLookupReverse(const int32_t cQueueId,
                                    char* const    cpReturnedName,
                                    const int32_t cNameBufferSizeInBytes);

#endif /* PLATFORMCOMMUNICATIONS_HPP */
