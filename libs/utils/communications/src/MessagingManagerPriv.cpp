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
#include "MessagingManagerPriv.hpp"
#include "global_errors.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <new>

/***************************************************************************/
/* Local Literals & definitions */
static const char gscMessagingManagerMessageQueueSemaphoreName[] = "MsgMgr0";
static const char gscMessagingManagerNameSemaphoreName[]         = "MsgMgr1";



/***************************************************************************/
/* Externally defined global data */


/***************************************************************************/
/* Locally defined global data */
MessagingManager MessagingManager::msInstance;


/***************************************************************************/
/**
 * \par Description
 * 
 *
 * \warning
 * \bug
 **************************************************************************/
MessagingManager::MessagingManager()
   : mIsInitialized(false)
{
}



/***************************************************************************/
/**
 * \par Description
 * 
 *
 * \warning
 * \bug
 **************************************************************************/
MessagingManager::~MessagingManager()
{
   LIB_RESULT libResult = LIB_FAILURE;

   if(mIsInitialized == true)
   {
      for(int32_t i = 1; i <= gcMsgMgrMaximumNumberOfQueues; ++i)
      {
         if(mpMessageQueues[i] != 0)
         {
            libResult = MessagingManager::MessageQueueDestroy(i);  MSG_MGR_ERROR(libResult == LIB_SUCCESS);
         }
      }

      // NameContainer resources will be freed in its destructor.

      libResult = GDC_OAL_SemaphoreDelete(mNameSemaphore);  MSG_MGR_ERROR(libResult == LIB_SUCCESS);
      libResult = GDC_OAL_SemaphoreDelete(mMessageQueueSemaphore);  MSG_MGR_ERROR(libResult == LIB_SUCCESS);

      mIsInitialized = false;
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
 * \bug
 **************************************************************************/
LIB_RESULT MessagingManager::Initialize()
{
   LIB_RESULT libResult = LIB_FAILURE;

   if(mIsInitialized == false)
   {
      // Size of arrays should be (gcMsgMgrMaximumNumberOfQueues + 1)
      for(int32_t i = 1; i <= gcMsgMgrMaximumNumberOfQueues; ++i)
      {
         mpMessageQueues[i]    = 0;
         mFreeMessageQueues[i] = (i);
      }
      
      mNumberOfFreeMessageQueues = gcMsgMgrMaximumNumberOfQueues;


      // mMessageQueueSemaphore
      libResult = GDC_OAL_SemaphoreCreate(&mMessageQueueSemaphore,
                                      (char*)gscMessagingManagerMessageQueueSemaphoreName,
                                      GDC_OAL_SEMAPHORE_COUNTING,
                                      1); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
      // mNameSemaphore
      if(libResult == LIB_SUCCESS)
      {
         libResult = GDC_OAL_SemaphoreCreate(&mNameSemaphore,
                                         (char*)gscMessagingManagerNameSemaphoreName,
                                         GDC_OAL_SEMAPHORE_COUNTING,
                                         1); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
      }

      if(libResult == LIB_SUCCESS)
      {
         mIsInitialized = true;
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
 * \bug
 **************************************************************************/
LIB_RESULT MessagingManager::MessageQueueCreate(int32_t& rReturnedInstanceId)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult    = LIB_FAILURE;
   LIB_RESULT libResultInt = LIB_FAILURE;

   rReturnedInstanceId = 0;

   libResult = GDC_OAL_SemaphoreObtain(mMessageQueueSemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
   
   if(libResult == LIB_SUCCESS)
   {
      libResult = LIB_FAILURE;
   
      const int32_t cIndexOfFreeMessageQueue = mFreeMessageQueues[mNumberOfFreeMessageQueues];
   
      if(mNumberOfFreeMessageQueues >= 1 &&
         mNumberOfFreeMessageQueues <= gcMsgMgrMaximumNumberOfQueues &&
         mFreeMessageQueues[mNumberOfFreeMessageQueues] != 0 &&
         mpMessageQueues[cIndexOfFreeMessageQueue] == 0)
      {
         MessageQueue* pMessageQueue = 0;

         pMessageQueue = (MessageQueue*)malloc(sizeof(*pMessageQueue));
         new ((void*)pMessageQueue) MessageQueue();

         if(pMessageQueue != 0)
         {   
            libResult = pMessageQueue->Initialize(cIndexOfFreeMessageQueue); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

            mpMessageQueues[cIndexOfFreeMessageQueue] = pMessageQueue;
            
            mFreeMessageQueues[mNumberOfFreeMessageQueues] = 0;

            --mNumberOfFreeMessageQueues;

            rReturnedInstanceId = cIndexOfFreeMessageQueue;
            
            libResult = LIB_SUCCESS;
         }
      }

      libResultInt = GDC_OAL_SemaphoreRelease(mMessageQueueSemaphore); MSG_MGR_ERROR(libResultInt == LIB_SUCCESS);
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
LIB_RESULT MessagingManager::MessageQueueDestroy(const int32_t cInstanceId)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult    = LIB_FAILURE;
   LIB_RESULT libResultInt = LIB_FAILURE;

   libResult = GDC_OAL_SemaphoreObtain(mMessageQueueSemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
   
   if(libResult == LIB_SUCCESS)
   { 
      libResult = LIB_FAILURE;

      if(cInstanceId >= 1 &&
         cInstanceId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
         mpMessageQueues[cInstanceId] != 0 &&
         mNumberOfFreeMessageQueues < gcMsgMgrMaximumNumberOfQueues &&
         mFreeMessageQueues[mNumberOfFreeMessageQueues + 1] == 0)
      {
         MessageQueue* pMessageQueue = mpMessageQueues[cInstanceId];

         libResult = pMessageQueue->SetFlowControl(false,
                                                   true); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
         int32_t messageQueueSize = 0;
         
         libResult = pMessageQueue->GetQueueSize(messageQueueSize); MSG_MGR_ERROR(libResult == LIB_SUCCESS);

         if(libResult == LIB_SUCCESS &&
            messageQueueSize == 0)
         {
            pMessageQueue->~MessageQueue();
            free(pMessageQueue);

            pMessageQueue = 0;

            ++mNumberOfFreeMessageQueues;

            mFreeMessageQueues[mNumberOfFreeMessageQueues] = cInstanceId;

            mpMessageQueues[cInstanceId] = 0;

            libResult = LIB_SUCCESS;
         }
         else
         {
           printf("Destroy non-empty queue [%d]\n", cInstanceId);
            libResult = LIB_FAILURE;
            // Someone tried to destroyed a non-empty queue
            // This is a good candidate for ressources leaks.
            MSG_MGR_ERROR(false);
         }
      }

      libResultInt = GDC_OAL_SemaphoreRelease(mMessageQueueSemaphore); MSG_MGR_ERROR(libResultInt == LIB_SUCCESS);
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
LIB_RESULT MessagingManager::MessageQueueGetSize(const int32_t cQueueId,
                                                 int32_t& rReturnedQueueSize)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult = LIB_FAILURE;

   if(cQueueId >= 1 &&
      cQueueId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
      mpMessageQueues[cQueueId] != 0)
   {
      MessageQueue& rMessageQueue = *(mpMessageQueues[cQueueId]);

      libResult = rMessageQueue.GetQueueSize(rReturnedQueueSize); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
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
LIB_RESULT MessagingManager::MessageQueueSetFlowControl(const int32_t cQueueId,
                                                        const bool cIsSendingAllowed,
                                                        const bool cIsReceivingAllowed)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult = LIB_FAILURE;

   if(cQueueId >= 1 &&
      cQueueId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
      mpMessageQueues[cQueueId] != 0)
   {
      MessageQueue& rMessageQueue = *(mpMessageQueues[cQueueId]);

      libResult = rMessageQueue.SetFlowControl(cIsSendingAllowed,
                                               cIsReceivingAllowed); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
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
bool MessagingManager::MessageQueueIsValid(const int32_t cInstanceId)
{
   return (cInstanceId >= 1 &&
           cInstanceId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
           mpMessageQueues[cInstanceId] != 0);
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
LIB_RESULT MessagingManager::MessageSend(const int32_t cReceiverQueueId,
                                         MessageBase* const cpMessageToBeSent,
                                         const int32_t cMessageSizeInBytes,
                                         const int32_t cPriority)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult = LIB_FAILURE;

   if(cReceiverQueueId >= 1 &&
      cReceiverQueueId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
      mpMessageQueues[cReceiverQueueId] != 0)
   {
      cpMessageToBeSent->mPriority = cPriority;

      MessageQueue& rMessageQueue = *(mpMessageQueues[cReceiverQueueId]);

      libResult = rMessageQueue.MessageSend(cpMessageToBeSent,
                                            cMessageSizeInBytes,
                                            cPriority);
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
LIB_RESULT MessagingManager::MessageReceive(const int32_t cQueueId,
                                            MessageBase*& rpMessageToBeReceived,
                                            int32_t&       rReturnedMessageSizeInBytes,
                                            const int32_t cSuspensionTimeInMicroseconds)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult = LIB_FAILURE;

   if(cQueueId >= 1 &&
      cQueueId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
      mpMessageQueues[cQueueId] != 0)
   {
      MessageQueue& rMessageQueue = *(mpMessageQueues[cQueueId]);

      libResult = rMessageQueue.MessageReceive(rpMessageToBeReceived,
                                               rReturnedMessageSizeInBytes,
                                               cSuspensionTimeInMicroseconds);
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
LIB_RESULT MessagingManager::MessageReceiveV2(const int32_t cQueueId,
                                              MessageBase*& rpMessageToBeReceived,
                                              int32_t&       rReturnedMessageSizeInBytes,
                                              const int32_t cPriority)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult = LIB_FAILURE;

   if(cQueueId >= 1 &&
      cQueueId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
      mpMessageQueues[cQueueId] != 0)
   {
      MessageQueue& rMessageQueue = *(mpMessageQueues[cQueueId]);

      libResult = rMessageQueue.MessageReceiveV2(rpMessageToBeReceived,
                                                 rReturnedMessageSizeInBytes,
                                                 cPriority);
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
LIB_RESULT MessagingManager::MessagePeek(const int32_t cQueueId,
                                         const MessageBase*& rpcMessageToBePeeked,
                                         int32_t& rReturnedMessageSizeInBytes,
                                         const int32_t cPriority)
{
   MSG_MGR_ERROR(mIsInitialized == true)

   LIB_RESULT libResult = LIB_FAILURE;

   if(cQueueId >= 1 &&
      cQueueId <= gcMsgMgrMaximumNumberOfQueues && // Within legal range of indexes
      mpMessageQueues[cQueueId] != 0)
   {
      MessageQueue& rMessageQueue = *(mpMessageQueues[cQueueId]);

      libResult = rMessageQueue.MessagePeek(rpcMessageToBePeeked,
                                            rReturnedMessageSizeInBytes,
                                            cPriority);
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
LIB_RESULT MessagingManager::NameRegister(const char   cName[],
                                          const int32_t cNameArrayLength,
                                          const int32_t cQueueId)
{              
   LIB_RESULT libResult    = LIB_FAILURE;
   LIB_RESULT libResultInt = LIB_FAILURE;
   
   // validate cQueueId first   
   libResult = GDC_OAL_SemaphoreObtain(mMessageQueueSemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
   
   if(libResult == LIB_SUCCESS)
   { 
      if(cQueueId == 0 ||
         cQueueId > gcMsgMgrMaximumNumberOfQueues || // Within legal range of indexes
         mpMessageQueues[cQueueId] == 0 ||
         cName == NULL ||
         cNameArrayLength >= gcMsgMgrNameArraySizeInBytes)  // also check for NULL cName
      {
         libResult = OAL_ERR_LOG_EINVALIDPARAM;
      }
      libResultInt = GDC_OAL_SemaphoreRelease(mMessageQueueSemaphore); MSG_MGR_ERROR(libResultInt == LIB_SUCCESS);
   }


   if(libResult == LIB_SUCCESS)
   { 

      libResult = GDC_OAL_SemaphoreObtain(mNameSemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
   
      if(libResult == LIB_SUCCESS)
      {
         libResult = mNameContainer.Insert(cName,
                                            cNameArrayLength,
                                            cQueueId);
   
         libResultInt = GDC_OAL_SemaphoreRelease(mNameSemaphore); MSG_MGR_ERROR(libResultInt == LIB_SUCCESS);
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
 * \bug
 **************************************************************************/
LIB_RESULT MessagingManager::NameUnregister(const char   cName[],
                                            const int32_t cNameArrayLength)
{
   LIB_RESULT libResult    = OAL_ERR_LOG_EINVALIDPARAM;
   LIB_RESULT libResultInt = LIB_FAILURE;

   if (cName != NULL && cNameArrayLength < gcMsgMgrNameArraySizeInBytes)
   {
      libResult = GDC_OAL_SemaphoreObtain(mNameSemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
   
      if(libResult == LIB_SUCCESS)
      {
         libResult = mNameContainer.Remove(cName,
                                         cNameArrayLength);
   
         libResultInt = GDC_OAL_SemaphoreRelease(mNameSemaphore); MSG_MGR_ERROR(libResultInt == LIB_SUCCESS);
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
 * \bug
 **************************************************************************/
LIB_RESULT MessagingManager::NameLookup(const char   cName[],
                                        const int32_t,
                                        int32_t& rReturnedQueueId)
{
   LIB_RESULT libResult    = LIB_FAILURE;
   LIB_RESULT libResultInt = LIB_FAILURE;

   libResult = GDC_OAL_SemaphoreObtain(mNameSemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
   
   if(libResult == LIB_SUCCESS)
   {
      NameIterator lIterator;

      libResult = mNameContainer.Iterator(lIterator);

      if(libResult == LIB_SUCCESS)
      {
         libResult = LIB_FAILURE;

         while(lIterator.HasNext() == true)
         {
            Name& lrName = *(lIterator.Next());

            const int lcStrncmpResult = strncmp(lrName.mName,
                                                cName,
                                                gcMsgMgrNameArraySizeInBytes);
         
            if(lcStrncmpResult == 0) // 0 means both strings are equal
            {
               rReturnedQueueId = lrName.mMessageQueueId;
               
               libResult = LIB_SUCCESS;
               break;
            }
         }
      }
   
      libResultInt = GDC_OAL_SemaphoreRelease(mNameSemaphore); MSG_MGR_ERROR(libResultInt == LIB_SUCCESS);
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
LIB_RESULT MessagingManager::NameLookupReverse(const int32_t cQueueId,
                                               char* const cpReturnedName,
                                               const int32_t cNameBufferSizeInBytes)
{
   LIB_RESULT libResult    = LIB_FAILURE;
   LIB_RESULT libResultInt = LIB_FAILURE;

   if(cpReturnedName != 0 &&
      cNameBufferSizeInBytes > 1) // Basically, more room than just the null termination
   {
      libResult = GDC_OAL_SemaphoreObtain(mNameSemaphore); MSG_MGR_ERROR(libResult == LIB_SUCCESS);
      
      if(libResult == LIB_SUCCESS)
      {
         NameIterator lIterator;

         libResult = mNameContainer.Iterator(lIterator);

         if(libResult == LIB_SUCCESS)
         {
            libResult = LIB_FAILURE;

            while(lIterator.HasNext() == true)
            {
               Name& lrName = *(lIterator.Next());
            
               if(lrName.mMessageQueueId == cQueueId) // 0 means both strings are equal
               {
                  strncpy(cpReturnedName,
                          lrName.mName,
                          cNameBufferSizeInBytes);
                  
                  libResult = LIB_SUCCESS;
                  break;
               }
            }
         }
      
         libResultInt = GDC_OAL_SemaphoreRelease(mNameSemaphore); MSG_MGR_ERROR(libResultInt == LIB_SUCCESS);
      }
   }

   return libResult;
}

