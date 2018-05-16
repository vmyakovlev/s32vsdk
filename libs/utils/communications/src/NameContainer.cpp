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
#include <stdlib.h>

/***************************************************************************/
/* Local Literals & definitions */



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
NameContainer::NameContainer()
{
   mNodeSentinel.mName[0]        = 0;
   mNodeSentinel.mMessageQueueId = 0; // SASHBY : hmm Message Queue Id 0 SHOULD be an invalid id...
   mNodeSentinel.mpNodeNext      = 0;
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
NameContainer::~NameContainer()
{
   NameNode* lpNodeCurrent = &mNodeSentinel;
   NameNode* lpNodeTarget  = 0;

   // Traverse container and free everything as we go
   while(lpNodeCurrent->mpNodeNext != 0)
   {
      lpNodeTarget = lpNodeCurrent->mpNodeNext;
      
      lpNodeCurrent->mpNodeNext = lpNodeTarget->mpNodeNext;

      free((void*)lpNodeTarget);

      lpNodeTarget = 0; // Unnecessary but I null pointers out of habit; compiler will optimize it out
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
LIB_RESULT NameContainer::Insert(const char   cName[],
                                 const int32_t cNameArrayLength,
                                 const int32_t cQueueId)
{
   LIB_RESULT lLibResult = LIB_SUCCESS;

   if(cNameArrayLength > 1) // Basically, more room than just the null termination
   {
      NameNode* lpNodeCurrent = &mNodeSentinel;

      // Go looking for an existing node with the same key values
      while(lpNodeCurrent->mpNodeNext != 0)
      {
         const int lcStrncmpResult = strncmp(lpNodeCurrent->mpNodeNext->mName,
                                             cName,
                                             gcMsgMgrNameArraySizeInBytes);
      
         if(lcStrncmpResult == 0) // 0 means both strings are equal
         {
            lLibResult = LIB_ERROR_NAME_ALREADY_REGISTERED;
            break;
         }

         lpNodeCurrent = lpNodeCurrent->mpNodeNext;
      }

      // If none are found proceed with insertion
      if(lLibResult == LIB_SUCCESS)
      {
         NameNode* pNodeNew = (NameNode*)malloc(sizeof(NameNode));

         if(pNodeNew != 0)
         {
            strncpy(pNodeNew->mName,   // Destination
                    cName,             // Source
                    gcMsgMgrNameArraySizeInBytes);

            pNodeNew->mMessageQueueId = cQueueId;
            pNodeNew->mpNodeNext      = mNodeSentinel.mpNodeNext;
            
            mNodeSentinel.mpNodeNext = pNodeNew;
         }
         else
         {
            lLibResult = LIB_FAILURE;
            MSG_MGR_ERROR(false);
         }
      }
   }

   return lLibResult;
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
LIB_RESULT NameContainer::Remove(const char   cName[],
                                 const int32_t)
{
   LIB_RESULT lLibResult = LIB_FAILURE;

   NameNode* lpNodeCurrent = &mNodeSentinel;

   // Go looking for an existing node with the same key values
   while(lpNodeCurrent->mpNodeNext != 0)
   {
      const int lcStrncmpResult = strncmp(lpNodeCurrent->mpNodeNext->mName,
                                          cName,
                                          gcMsgMgrNameArraySizeInBytes);
   
      if(lcStrncmpResult == 0) // 0 means both strings are equal
      {
         NameNode* lpNodeTarget = lpNodeCurrent->mpNodeNext;

         lpNodeCurrent->mpNodeNext = lpNodeTarget->mpNodeNext;

         free((void*)lpNodeTarget);
         lLibResult = LIB_SUCCESS;
         break;
      }
      
      lpNodeCurrent = lpNodeCurrent->mpNodeNext;
   }



   return lLibResult;
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
LIB_RESULT NameContainer::Remove(const int32_t cQueueId)
{
   LIB_RESULT lLibResult = LIB_FAILURE;

   NameNode* lpNodeCurrent = &mNodeSentinel;

   // Go looking for an existing node with the same key values
   while(lpNodeCurrent->mpNodeNext != 0)
   {
      if(lpNodeCurrent->mpNodeNext->mMessageQueueId == cQueueId) // 0 means both strings are equal
      {
         NameNode* lpNodeTarget = lpNodeCurrent->mpNodeNext;

         lpNodeCurrent->mpNodeNext = lpNodeTarget->mpNodeNext;

         free((void*)lpNodeTarget);

         // Don't break since there may be more than one node with the same queue id
      }
      
      lpNodeCurrent = lpNodeCurrent->mpNodeNext;
   }

   lLibResult = LIB_SUCCESS; // Always succeed?

   return lLibResult;
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
LIB_RESULT NameContainer::Iterator(NameIterator& rReturnedIterator)
{
   LIB_RESULT lLibResult = LIB_FAILURE;

   rReturnedIterator.mpNodeCurrent = &mNodeSentinel;

   lLibResult = LIB_SUCCESS; // Always succeed?

   return lLibResult;
}
