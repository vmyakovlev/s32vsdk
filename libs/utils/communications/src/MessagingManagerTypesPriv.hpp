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

#ifndef MESSAGINGMANAGERTYPESPRIV_HPP
#define MESSAGINGMANAGERTYPESPRIV_HPP

/***************************************************************************/
/* Include files. */
#include "GDC_OAL_Semaphore.hpp"
#include "communications.hpp"
#include "oal.h"
#include "global_errors.h"
#include <stdint.h>
#include <stdio.h>

//**************************************************************************/
// Literals & definitions
#define MSG_MGR_ERROR(expr) if(!(expr)) { printf("%s : %s [%d]\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); }
//#define MSG_MGR_ERROR(expr)

const int32_t gcMsgMgrNameArraySizeInBytes        = 32; // Max name size is 31 characters, since need 1 for null termination.
const int32_t gcMsgMgrMaximumNumberOfQueues       = 256; //64



//**************************************************************************/
// Class declaration.



struct MessageNode
{
   MessageBase* mpMessage;
   int32_t      mMessageSizeInBytes;
   MessageNode* mpNodePrevious;
   MessageNode* mpNodeNext;
};

/**
 * \brief This class is responsible for ...
 *
 * 
 */
 class MessageQueue
 {
 public:
   MessageQueue();
   virtual ~MessageQueue();
   
   LIB_RESULT Initialize(const int32_t cQueueId);

   LIB_RESULT MessageSend(MessageBase* const cpMessageToBeSent,
                          const int32_t cMessageSizeInBytes,
                          const int32_t cPriority);

   LIB_RESULT MessageReceive(MessageBase*& rpMessageToBeReceived,
                             int32_t&      rReturnedMessageSizeInBytes,
                             const int32_t cSuspensionTimeInMilliseconds);

   LIB_RESULT MessageReceiveV2(MessageBase*& rpMessageToBeReceived,
                               int32_t&      rReturnedMessageSizeInBytes,
                               const int32_t cPriority);

   LIB_RESULT MessagePeek(const MessageBase*& rpcMessageToBePeeked,
                          int32_t&       rReturnedMessageSizeInBytes,
                          const int32_t  cPriority);

   LIB_RESULT GetQueueSize(int32_t& rReturnedQueueSize);

   LIB_RESULT SetFlowControl(const bool cIsSendingAllowed,
                             const bool cIsReceivingAllowed);
 
 private:
   int32_t       mQueueId;
   GDC_OAL_SEMAPHORE mSemaphoreReceive;
   GDC_OAL_SEMAPHORE mThreadSafetySemaphore;
   MessageNode   mNodeBack[gcMsgMgrNumberOfSupportedPriorities];      // Sentinel
   MessageNode   mNodeFront[gcMsgMgrNumberOfSupportedPriorities];     // Sentinel
   int32_t       mNumberOfNodes[gcMsgMgrNumberOfSupportedPriorities]; // Number of nodes in a queue; excluding sentinels
   bool          mIsSendingAllowed;   // Flow control
   bool          mIsReceivingAllowed; // Flow control
   bool          mIsInitialized;

 };


/**
 * \brief This class is responsible for ...
 *
 * 
 */
struct Name
{
   char   mName[gcMsgMgrNameArraySizeInBytes];
   int32_t mMessageQueueId;
};



/**
 * \brief This class is responsible for ...
 *
 * 
 */
struct NameNode : public Name
{
   NameNode* mpNodeNext;
};



/**
 * \brief This class is an iterator which facilitates traversal
 *
 */
class NameIterator
{
   friend class NameContainer;

public:
   NameIterator();
   virtual ~NameIterator();

   bool HasNext();

   Name* PeekNext();

   Name* Next();

private:
   NameNode* mpNodeCurrent;

private:
   NameIterator(const NameIterator&);            // Do not implement
   NameIterator& operator=(const NameIterator&); // Do not implement
};



/**
 * \brief This class is responsible for ...
 *
 * 
 */
class NameContainer
{
public:
   NameContainer();
   virtual ~NameContainer();

   LIB_RESULT Insert(const char   cName[],
                     const int32_t cNameArrayLength,
                     const int32_t cQueueId);

   LIB_RESULT Remove(const char   cName[],
                     const int32_t cNameArrayLength);

   LIB_RESULT Remove(const int32_t cQueueId);

   LIB_RESULT Iterator(NameIterator& rReturnedIterator);

private:
   NameNode mNodeSentinel;

private:
   NameContainer(const NameContainer&);            // Do not implement
   NameContainer& operator=(const NameContainer&); // Do not implement
};


#endif /* MESSAGINGMANAGERTYPESPRIV_HPP */
