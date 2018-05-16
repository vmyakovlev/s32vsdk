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
#include "MessagingManagerTypesPriv.hpp"
#include "oal.h"
#include "global_errors.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/***************************************************************************/
/* Local Literals & definitions */
static const char gscMessagingQueueSemaphoreName[] = "MsgQ";


/***************************************************************************/
/* Externally defined global data */


/***************************************************************************/
/* Locally defined global data */


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
 * \todo
 * \bug
 **************************************************************************/
MessageQueue::MessageQueue()
   :  mQueueId(0),
      mIsSendingAllowed(false),
      mIsReceivingAllowed(false),
      mIsInitialized(false)
{
   //GDC_OAL_SEMAPHORE mSemaphoreReceive;
   for(int32_t i = 0; i < gcMsgMgrNumberOfSupportedPriorities; ++i)
   {
      mNodeBack[i].mpMessage           = 0;
      mNodeBack[i].mMessageSizeInBytes = 0;
      mNodeBack[i].mpNodePrevious      = 0;
      mNodeBack[i].mpNodeNext          = 0;

      mNodeFront[i].mpMessage           = 0;
      mNodeFront[i].mMessageSizeInBytes = 0;
      mNodeFront[i].mpNodePrevious      = 0;
      mNodeFront[i].mpNodeNext          = 0;

      mNumberOfNodes[i] = 0;
   }
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
 * \todo
 * \bug
 **************************************************************************/
MessageQueue::~MessageQueue()
{
   LIB_RESULT libResult = LIB_SUCCESS;

   if(mIsInitialized == true)
   {
      MessageBase* pMessage = 0;
      int32_t      messageSizeInBytes = 0;

      do
      {
         libResult = MessageQueue::MessageReceive(pMessage,
                                                  messageSizeInBytes,
                                                  0);
      }
      while(libResult == LIB_SUCCESS &&
            pMessage != 0);

      libResult = GDC_OAL_SemaphoreDelete(mSemaphoreReceive); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
   }
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
 * \todo
 * \bug
 **************************************************************************/
LIB_RESULT MessageQueue::Initialize(const int32_t cQueueId)
{
   LIB_RESULT libResult = LIB_SUCCESS;

   if(mIsInitialized == false)
   {
      mQueueId = cQueueId;

      char pCString[32];

      snprintf(pCString, 32,
               "%s%i",gscMessagingQueueSemaphoreName, cQueueId);

      libResult = GDC_OAL_SemaphoreCreate(&mSemaphoreReceive,
                                      (char*)pCString,
                                      GDC_OAL_SEMAPHORE_COUNTING,
                                      0); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

      if(libResult == LIB_SUCCESS)
      {
         libResult = GDC_OAL_SemaphoreCreate(&mThreadSafetySemaphore,
                                         (char*)pCString,
                                         GDC_OAL_SEMAPHORE_COUNTING,
                                         1); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

         if(libResult == LIB_SUCCESS)
         {
            for(int32_t i = 0; i < gcMsgMgrNumberOfSupportedPriorities; ++i)
            {
               // NULL <- BACK <--> FRONT -> NULL
               mNodeBack[i].mpMessage           = 0;
               mNodeBack[i].mMessageSizeInBytes = 0;
               mNodeBack[i].mpNodePrevious      = 0;
               mNodeBack[i].mpNodeNext          = &mNodeFront[i];

               mNodeFront[i].mpMessage           = 0;
               mNodeFront[i].mMessageSizeInBytes = 0;
               mNodeFront[i].mpNodePrevious      = &mNodeBack[i];
               mNodeFront[i].mpNodeNext          = 0;

               mNumberOfNodes[i] = 0;
            }

            mIsSendingAllowed   = true;
            mIsReceivingAllowed = true;
            mIsInitialized      = true;

            libResult = LIB_SUCCESS;
         }
      }
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
 * \todo
 * \bug
 **************************************************************************/
LIB_RESULT MessageQueue::MessageSend(MessageBase* const cpMessageToBeSent,
                                     const int32_t cMessageSizeInBytes,
                                     const int32_t cPriority)
{
   MSG_MGR_ERROR(mIsInitialized == true);

   LIB_RESULT libResult  = LIB_FAILURE;
   LIB_RESULT lLibResultInt = LIB_FAILURE;

   if(cpMessageToBeSent != 0 &&
      cMessageSizeInBytes >= 0 &&
      (uint32_t)cMessageSizeInBytes >= sizeof(MessageBase) &&
      mIsSendingAllowed == true)
   {
      const int32_t cAdjustedPriority = (cPriority <= gcMsgMgrNumberOfSupportedPriorities) ?
                                         cPriority : gcMsgMgrNumberOfSupportedPriorities;

      MessageNode* pNode = (MessageNode*)malloc(sizeof(MessageNode)); MSG_MGR_ERROR(pNode != 0);
      
      //SASHBY DEBUG_printf("[A] - 0x%X - 0x%X [%d]\n", pNode, (int32_t)pNode + sizeof(MessageNode), sizeof(MessageNode));

      if(pNode != 0)
      {
         pNode->mpMessage           = cpMessageToBeSent;
         pNode->mMessageSizeInBytes = cMessageSizeInBytes;


         libResult = GDC_OAL_SemaphoreObtain(mThreadSafetySemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

         if(libResult == LIB_SUCCESS)
         {
            if(mIsSendingAllowed == true) // This check MUST be inside the lock!!!
            {
               // NULL <- BACK <--> FRONT -> NULL
               MessageNode& rNodeBack = mNodeBack[cAdjustedPriority];

               pNode->mpNodePrevious = &(rNodeBack);
               pNode->mpNodeNext     = rNodeBack.mpNodeNext;

               rNodeBack.mpNodeNext->mpNodePrevious = pNode;
               rNodeBack.mpNodeNext                 = pNode;
               // NULL <- BACK <--> NODE <--> FRONT -> NULL

               ++mNumberOfNodes[cAdjustedPriority];

               libResult = LIB_SUCCESS;
            }

            lLibResultInt = GDC_OAL_SemaphoreRelease(mThreadSafetySemaphore); MSG_MGR_ERROR(lLibResultInt == LIB_SUCCESS);
         }

         if(libResult == LIB_SUCCESS)
         {
            libResult = GDC_OAL_SemaphoreRelease(mSemaphoreReceive); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
         }
      }
   }
   else if(cpMessageToBeSent == 0) { libResult = LIB_INVALID_POINTER; }
   else if(cMessageSizeInBytes >= 0 &&
         (uint32_t)cMessageSizeInBytes < sizeof(MessageBase)) { libResult = LIB_INVALID_SIZE; }
   else if(mIsSendingAllowed != true) { libResult = LIB_SENDING_NOT_ALLOWED; }

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
 * \todo
 * \bug
 **************************************************************************/
LIB_RESULT MessageQueue::MessageReceive(MessageBase*& rpMessageToBeReceived,
                                        int32_t&       rReturnedMessageSizeInBytes,
                                        const int32_t  cSuspensionTimeInMicroseconds)
{
   MSG_MGR_ERROR(mIsInitialized == true);

   LIB_RESULT libResult = LIB_FAILURE;
   LIB_RESULT lLibResultInt = LIB_FAILURE;

   libResult = GDC_OAL_SemaphoreObtain(mSemaphoreReceive,
                                      cSuspensionTimeInMicroseconds);

   if(libResult == LIB_SUCCESS)
   {
      MessageNode* pNode = 0;


      libResult = GDC_OAL_SemaphoreObtain(mThreadSafetySemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

      if(libResult == LIB_SUCCESS)
      {
         // Traverse queues from highest priority to lowest
         // The lower the numerical value, the higher the priority
         for(int32_t i = 0; i < gcMsgMgrNumberOfSupportedPriorities; ++i)
         {
            if(mNumberOfNodes[i] > 0)
            {
               // NULL <- BACK <--> NODE <--> FRONT -> NULL
               pNode = mNodeFront[i].mpNodePrevious;

               pNode->mpNodePrevious->mpNodeNext = &(mNodeFront[i]);

               mNodeFront[i].mpNodePrevious = pNode->mpNodePrevious;
               // NULL <- BACK <--> FRONT -> NULL

               --mNumberOfNodes[i];

               break;
            }
         }

         lLibResultInt = GDC_OAL_SemaphoreRelease(mThreadSafetySemaphore); MSG_MGR_ERROR(lLibResultInt == LIB_SUCCESS);
      }

      if(pNode != 0)
      {
         MSG_MGR_ERROR(pNode->mpMessage != 0 &&
                       pNode->mMessageSizeInBytes >= 0 &&
                       (uint32_t)(pNode->mMessageSizeInBytes) >= sizeof(MessageBase));

         rpMessageToBeReceived       = pNode->mpMessage;
         rReturnedMessageSizeInBytes = pNode->mMessageSizeInBytes;

         //SASHBY //DEBUG_printf("[F] - 0x%X\n", pNode);

         free(pNode);

         pNode = 0;

         libResult = LIB_SUCCESS;
      }
      else
      {
         // Should NEVER happen
         // If we successfully obtained the semaphore; that means there MUST be a message to receive!!!
         MSG_MGR_ERROR(false);
      }
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
 * \todo
 * \bug
 **************************************************************************/
LIB_RESULT MessageQueue::MessageReceiveV2(MessageBase*& rpMessageToBeReceived,
                                          int32_t&       rReturnedMessageSizeInBytes,
                                          const int32_t  cPriority)
{
   MSG_MGR_ERROR(mIsInitialized == true);

   LIB_RESULT libResult = LIB_FAILURE;
   LIB_RESULT lLibResultInt = LIB_FAILURE;

   if(cPriority < gcMsgMgrNumberOfSupportedPriorities)
   {
      libResult = GDC_OAL_SemaphoreTryObtain(mSemaphoreReceive);

      if(libResult == LIB_SUCCESS)
      {
         MessageNode* pNode = 0;

         libResult = GDC_OAL_SemaphoreObtain(mThreadSafetySemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

         if(libResult == LIB_SUCCESS)
         {

            if(mNumberOfNodes[cPriority] > 0)
            {
               // NULL <- BACK <--> NODE <--> FRONT -> NULL
               pNode = mNodeFront[cPriority].mpNodePrevious;

               pNode->mpNodePrevious->mpNodeNext = &(mNodeFront[cPriority]);

               mNodeFront[cPriority].mpNodePrevious = pNode->mpNodePrevious;
               // NULL <- BACK <--> FRONT -> NULL

               --mNumberOfNodes[cPriority];
            }

            lLibResultInt = GDC_OAL_SemaphoreRelease(mThreadSafetySemaphore); MSG_MGR_ERROR(lLibResultInt == LIB_SUCCESS);
         }

         if(pNode != 0)
         {
            MSG_MGR_ERROR(pNode->mpMessage != 0 &&
                          pNode->mMessageSizeInBytes >= 0 &&
                          (uint32_t)(pNode->mMessageSizeInBytes) >= sizeof(MessageBase));

            rpMessageToBeReceived       = pNode->mpMessage;
            rReturnedMessageSizeInBytes = pNode->mMessageSizeInBytes;

            free(pNode);

            pNode = 0;

            libResult = LIB_SUCCESS;
         }
         else
         {
            // Should NEVER happen
            // If we successfully obtained the semaphore; that means there MUST be a message to receive!!!
            MSG_MGR_ERROR(false);
         }
      }
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
 * \todo
 * \bug
 **************************************************************************/
LIB_RESULT MessageQueue::MessagePeek(const MessageBase*& rpcMessageToBePeeked,
                                     int32_t&       rReturnedMessageSizeInBytes,
                                     const int32_t  cPriority)
{
   MSG_MGR_ERROR(mIsInitialized == true);

   LIB_RESULT libResult = LIB_FAILURE;
   LIB_RESULT lLibResultInt = LIB_FAILURE;

   if(cPriority < gcMsgMgrNumberOfSupportedPriorities)
   {
      MessageNode* pNode = 0;

      libResult = GDC_OAL_SemaphoreObtain(mThreadSafetySemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

      if(libResult == LIB_SUCCESS)
      {

         if(mNumberOfNodes[cPriority] > 0)
         {
            // NULL <- BACK <--> NODE <--> FRONT -> NULL
            pNode = mNodeFront[cPriority].mpNodePrevious;
         }

         lLibResultInt = GDC_OAL_SemaphoreRelease(mThreadSafetySemaphore); MSG_MGR_ERROR(lLibResultInt == LIB_SUCCESS);
      }


      if(pNode != 0)
      {
         MSG_MGR_ERROR(pNode->mpMessage != 0 &&
                       pNode->mMessageSizeInBytes >= 0 &&
                       (uint32_t)(pNode->mMessageSizeInBytes) >= sizeof(MessageBase));

         rpcMessageToBePeeked        = pNode->mpMessage;
         rReturnedMessageSizeInBytes = pNode->mMessageSizeInBytes;

         libResult = LIB_SUCCESS;
      }
      else
      {
         rpcMessageToBePeeked        = 0;
         rReturnedMessageSizeInBytes = 0;
         
         libResult = LIB_FAILURE;
      }
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
 * \todo
 * \bug
 **************************************************************************/
LIB_RESULT MessageQueue::GetQueueSize(int32_t& rReturnedQueueSize)
{
   MSG_MGR_ERROR(mIsInitialized == true);

   LIB_RESULT libResult = LIB_FAILURE;
   LIB_RESULT lLibResultInt = LIB_FAILURE;

   rReturnedQueueSize = 0;

   libResult = GDC_OAL_SemaphoreObtain(mThreadSafetySemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

   if(libResult == LIB_SUCCESS)
   {
      // Traverse queues from highest priority to lowest
      // The lower the numerical value, the higher the priority
      for(int32_t i = 0; i < gcMsgMgrNumberOfSupportedPriorities; ++i)
      {
         rReturnedQueueSize += mNumberOfNodes[i];
      }

      lLibResultInt = GDC_OAL_SemaphoreRelease(mThreadSafetySemaphore); MSG_MGR_ERROR(lLibResultInt == LIB_SUCCESS);
   }

   libResult = LIB_SUCCESS;

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
 * \todo
 * \bug
 **************************************************************************/
LIB_RESULT MessageQueue::SetFlowControl(const bool cIsSendingAllowed,
                                        const bool cIsReceivingAllowed)
{
   MSG_MGR_ERROR(mIsInitialized == true);

   LIB_RESULT libResult = LIB_FAILURE;

   mIsSendingAllowed   = cIsSendingAllowed;
   mIsReceivingAllowed = cIsReceivingAllowed;

   libResult = LIB_SUCCESS;

   return libResult;
}

