/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2010 All right reserved.
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

/* Information for doxygen */
/**
 *  \file   <<FileName>>
 *  \brief  <<One line Description>>
 *
 *  <<Replace this with Multiline description. This should describe the
 *  file in general.>>
 */ 

#ifndef MESSAGINGMANAGERPRIV_HPP
#define MESSAGINGMANAGERPRIV_HPP

/***************************************************************************/
/* Include files. */
#include "MessagingManagerTypesPriv.hpp"
#include "oal.h"
#include "global_errors.h"
#include <stdint.h>


//**************************************************************************/
// Literals & definitions


//**************************************************************************/
// Class declaration.

/**
 * \brief <<This class is responsbile for blah blah blah.>>
 *
 * <<And here is the detailed description for the class if you need to write
 * one. And probably you should.>>
 */
class MessagingManager
{
public:
   static inline MessagingManager* Instance()
   {
      return &msInstance;
   }

   virtual ~MessagingManager();

   LIB_RESULT Initialize();

   LIB_RESULT MessageQueueCreate(int32_t& rReturnedInstanceId);

   LIB_RESULT MessageQueueDestroy(const int32_t cInstanceId);

   LIB_RESULT MessageQueueGetSize(const int32_t cInstanceId,
                                  int32_t& rReturnedQueueSize);

   LIB_RESULT MessageQueueSetFlowControl(const int32_t cInstanceId,
                                         const bool cIsSendingAllowed,
                                         const bool cIsReceivingAllowed);

   bool MessageQueueIsValid(const int32_t cInstanceId);

   LIB_RESULT MessageSend(const int32_t cReceiverQueueId,
                          MessageBase* const cpMessageToBeSent,
                          const int32_t cMessageSizeInBytes,
                          const int32_t cPriority);

   LIB_RESULT MessageReceive(const int32_t cQueueId,
                             MessageBase*& rpMessageToBeReceived,
                             int32_t&       rReturnedMessageSizeInBytes,
                             const int32_t cSuspensionTimeInMicroseconds);

   LIB_RESULT MessageReceiveV2(const int32_t cQueueId,
                               MessageBase*& rpMessageToBeReceived,
                               int32_t&       rReturnedMessageSizeInBytes,
                               const int32_t cPriority);

   LIB_RESULT MessagePeek(const int32_t cQueueId,
                          const MessageBase*& rpcMessageToBePeeked,
                          int32_t& rReturnedMessageSizeInBytes,
                          const int32_t cPriority);

   LIB_RESULT NameRegister(const char   cName[],
                           const int32_t cNameArrayLength,
                           const int32_t cQueueId);

   LIB_RESULT NameUnregister(const char   cName[],
                             const int32_t cNameArrayLength);

   LIB_RESULT NameLookup(const char   cName[],
                         const int32_t cNameArrayLength,
                         int32_t& rReturnedQueueId);

   LIB_RESULT NameLookupReverse(const int32_t cQueueId,
                                char* const cpReturnedName,
                                const int32_t cNameBufferSizeInBytes);

private: // Data member used by Message Passing functionality
   MessageQueue* mpMessageQueues[gcMsgMgrMaximumNumberOfQueues + 1];    // +1 because a queue id of 0 is invalid.
   int32_t       mFreeMessageQueues[gcMsgMgrMaximumNumberOfQueues + 1]; // +1 because a queue id of 0 is invalid.
   int32_t       mNumberOfFreeMessageQueues;
   GDC_OAL_SEMAPHORE mMessageQueueSemaphore;

private: // Data members used by Name Server functionality
   NameContainer  mNameContainer;
   GDC_OAL_SEMAPHORE  mNameSemaphore;
   bool           mIsInitialized;

private:
   MessagingManager();
   MessagingManager( const MessagingManager& );            // Do not implement
   MessagingManager& operator=( const MessagingManager& ); // Do not implement

   static MessagingManager msInstance;
}; // End of MessagingManager declaration.

#endif /* MESSAGINGMANAGERPRIV_HPP */
