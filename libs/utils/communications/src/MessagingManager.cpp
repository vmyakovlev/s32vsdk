/******************************************************************************
 *  (C) Copyright CogniVue Corp. 2010 All right reserved.
 *    Confidential Information
 *  
 *  All parts of the CogniVue Corp. Program Source are protected by copyright law 
 *  and all rights are reserved. 
 *  This documentation may not, in whole or in part, be copied, photocopied, 
 *  reproduced, translated, or reduced to any electronic medium or machine 
 *  readable form without prior consent, in writing, from Cognivue. 
 ******************************************************************************/

/* Information for doxygen */
/**
 *  \file   <<FileName>>
 *  \brief  <<One line Description>>
 *
 *  <<Replace this with Multiline description. This should describe the
 *  file in general.>>
 */ 

/***************************************************************************/
/* Include files. */
#include "communications.hpp"
#include "MessagingManagerPriv.hpp"
#include "global_errors.h"
#include <stdint.h>


/***************************************************************************/
/* Local Literals & definitions */


/***************************************************************************/
/* Externally defined global data */


/***************************************************************************/
/* Locally defined global data */


/***************************************************************************/
/**
 * \details
 * This function initializes the component.
 * It needs to be called prior to using any of the apis.
 * Initialize should be called once during the system initialization.
 * 
 *
 * \return
 *    #LIB_SUCCESS
 *    #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_Initialize()
{
   return (MessagingManager::Instance())->Initialize();
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_MessageQueueCreate(int32_t& rReturnedInstanceId)
{
   return (MessagingManager::Instance())->MessageQueueCreate(rReturnedInstanceId);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_MessageQueueDestroy(const int32_t cInstanceId)
{
   return (MessagingManager::Instance())->MessageQueueDestroy(cInstanceId);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_MessageQueueGetSize(const int32_t cQueueId,
                                      int32_t& rReturnedQueueSize)
{
   return (MessagingManager::Instance())->MessageQueueGetSize(cQueueId,
                                                              rReturnedQueueSize);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_MessageQueueSetFlowControl(const int32_t cQueueId,
                                             const bool cIsSendingAllowed,
                                             const bool cIsReceivingAllowed)
{
   LIB_RESULT libResult = LIB_FAILURE;

   if(cIsReceivingAllowed == true)
   {
      libResult = (MessagingManager::Instance())->MessageQueueSetFlowControl(cQueueId,
                                                                             cIsSendingAllowed,
                                                                             cIsReceivingAllowed);
   }

   return libResult;
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
bool MsgMgr_MessageQueueIsValid(const int32_t cQueueId)
{
   return (MessagingManager::Instance())->MessageQueueIsValid(cQueueId);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these.
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_MessageSend(const int32_t cReceiverQueueId,
                              MessageBase* const cpMessageToBeSent,
                              const int32_t cMessageSizeInBytes,
                              const int32_t cPriority)
{
   return (MessagingManager::Instance())->MessageSend(cReceiverQueueId,
                                                      cpMessageToBeSent,
                                                      cMessageSizeInBytes,
                                                      cPriority);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_MessageReceive(const int32_t cQueueId,
                                 MessageBase*& rpMessageToBeReceived,
                                 int32_t&       rReturnedMessageSizeInBytes,
                                 const int32_t cSuspensionTimeInMicroseconds)
{
   return (MessagingManager::Instance())->MessageReceive(cQueueId,
                                                         rpMessageToBeReceived,
                                                         rReturnedMessageSizeInBytes,
                                                         cSuspensionTimeInMicroseconds);
}



/***************************************************************************/
/**
 * \details
 * This function retrieves a message from the specified queue.
 * Oldest message of the specified priority shall be returned.
 * If the function returns success; responsibility for deallocating the message passes to the caller.
 * All messages SHALL be allocated using the function "OAL_MemoryAlloc".
 * All messages SHALL be deallocated using the function "OAL_MemoryFree".
 *
 *
 * \param  cQueueId                      - Unique identifier which references the local tasks queue
 * \param  rpMessageToBeReceived         - Pointer to the buffer containing the message;
 *                                         only valid if result is "LIB_SUCCESS".
 *                                         Buffer SHALL be deallocated using the function "OAL_MemoryFree"
 * \param  rReturnedMessageSizeInBytes   - The size in bytes of the buffer containing the message.
 * \param  cPriority                     - Priority of the message, valid values are 0,1,2.
 *                                         Lower numerical values indicate higher priority.
 *               
 *
 * \return 
 *    #LIB_SUCCESS
 *    #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessageReceivePriority(const int32_t  cQueueId,
                                         MessageBase*&  rpMessageToBeReceived,
                                         int32_t&       rReturnedMessageSizeInBytes,
                                         const int32_t  cPriority)
{
   return (MessagingManager::Instance())->MessageReceiveV2(cQueueId,
                                                           rpMessageToBeReceived,
                                                           rReturnedMessageSizeInBytes,
                                                           cPriority);
}



/***************************************************************************/
/**
 * \details
 * This function peeks at the oldest message of the specified priority from the specified queue.
 * Returned message shall be READ ONLY and SHALL NOT BE FREED by the caller.
 *
 * \param  cQueueId                      - Unique identifier which references the local tasks queue
 * \param  rpMessageToBePeeked           - Pointer to the buffer containing the message;
 *                                         only valid if result is "LIB_SUCCESS".
 *                                         Buffer SHALL NOT BE FREED.
 * \param  rReturnedMessageSizeInBytes   - The size in bytes of the buffer containing the message.
 * \param  cPriority                     - Priority of the message, valid values are 0,1,2.
 *                                         Lower numerical values indicate higher priority.
 *               
 *
 * \return 
 *    #LIB_SUCCESS
 *    #LIB_FAILURE
 **************************************************************************/
LIB_RESULT MsgMgr_MessagePeekPriority(const int32_t cQueueId,
                                      const MessageBase*& rpcMessageToBePeeked,
                                      int32_t& rReturnedMessageSizeInBytes,
                                      const int32_t cPriority)
{
   return (MessagingManager::Instance())->MessagePeek(cQueueId,
                                                      rpcMessageToBePeeked,
                                                      rReturnedMessageSizeInBytes,
                                                      cPriority);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_NameRegister(const char   cName[],
                               const int32_t cNameArrayLength,
                               const int32_t cQueueId)
{
   return (MessagingManager::Instance())->NameRegister(cName,
                                                       cNameArrayLength,
                                                       cQueueId);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_NameUnregister(const char   cName[],
                                 const int32_t cNameArrayLength)
{
   return (MessagingManager::Instance())->NameUnregister(cName,
                                                         cNameArrayLength);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_NameLookup(const char   cName[],
                             const int32_t cNameArrayLength,
                             int32_t& rReturnedQueueId)
{
   return (MessagingManager::Instance())->NameLookup(cName,
                                                     cNameArrayLength,
                                                     rReturnedQueueId);
}



/***************************************************************************/
/**
 * \par Description
 * The first sentence is the summary of the function. Starting from the
 * next sentence is your detailed description. Doxygen creates a summary
 * and a detailed description based on these. 
 *
 * \param  param1  First parameter
 * \param  param2  Second parameter with a rather long description. Pls
 *                 keep your descriptions as long as possible to clearly
 *                 identify what is required from the user.
 *
 * \return ::LIB_SUCCESS
 *
 * \warning
 * \bug
 **************************************************************************/
LIB_RESULT MsgMgr_NameLookupReverse(const int32_t cQueueId,
                                    char* const cpReturnedName,
                                    const int32_t cNameBufferSizeInBytes)
{
   return (MessagingManager::Instance())->NameLookupReverse(cQueueId,
                                                            cpReturnedName,
                                                            cNameBufferSizeInBytes);
}

