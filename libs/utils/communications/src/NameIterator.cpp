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
#include <stdint.h>

/***************************************************************************/
/* Local Literals & definitions */



/***************************************************************************/
/* Externally defined global data */


/***************************************************************************/
/* Locally defined global data */


/***************************************************************************/
/**
 * \par Constructor
 *
 * \warning
 * \todo
 * \bug
 **************************************************************************/
NameIterator::NameIterator()
   : mpNodeCurrent(0)
{
}

/***************************************************************************/
/**
 * \par Destructor
 *
 * \warning
 * \todo
 * \bug
 **************************************************************************/
NameIterator::~NameIterator()
{
}

/***************************************************************************/
/**
 * \par Returns true if the iteration has more elements.
 * In other words, returns true if Next() would return an element rather.
 *
 * \return true   There is another element in the container
 * \return false  There is no more elements in the container
 *
 * \warning Only safe to call after Iterator has been initialized by container
 * \todo
 * \bug
 **************************************************************************/
bool NameIterator::HasNext()
{
   bool hasNext = false;

   if(mpNodeCurrent != 0 &&
      mpNodeCurrent->mpNodeNext != 0)
   {
      hasNext = true;
   }
  
   return hasNext;
}

/***************************************************************************/
/**
 * \par Returns the next element in the iteration.
 * Calling this method repeatedly until the HasNext() method returns false 
 * will return each element in the container exactly once.
 *
 * \return Name*   Pointer to the next element in the container
 *
 * \warning Only safe to call if HasNext returned true
 * \todo
 * \bug
 **************************************************************************/
Name* NameIterator::Next()
{
   MSG_MGR_ERROR(mpNodeCurrent != 0 &&
                 mpNodeCurrent->mpNodeNext != 0);

   mpNodeCurrent = mpNodeCurrent->mpNodeNext;

   Name* pName = (Name*)(mpNodeCurrent);

   return pName;
}
