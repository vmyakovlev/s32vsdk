/*************************************************************************************
 *  (C) Copyright CogniVue Corporation. 2014 All right reserved.
 *  
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 ************************************************************************************/

/*!**********************************************************************************
*  @file Feature32SDescriptor.cpp
*  @brief ICP Feature32S Descriptor
************************************************************************************/
#include "icp_feature_32s.h"
#include "icp_types.h"

namespace icp {
	
Feature32SDescriptor::Feature32SDescriptor(): mpData(0), mpDataPhys(0), mMaxElements(0),
                                    mNumElements(0)
{
}

Feature32SDescriptor::Feature32SDescriptor(
               void* const lpData,
               void* const lpDataPhys,
               int32_t maxElements): mpData(lpData), mpDataPhys(lpDataPhys),
                                     mMaxElements(maxElements), mNumElements(0)
{
}

Feature32SDescriptor::~Feature32SDescriptor()
{
}


void Feature32SDescriptor::Init(
             void* const lpData,
             void* const lpDataPhys,
             int32_t maxElements)
{
   mpData         = lpData;
   mpDataPhys     = lpDataPhys;
   mMaxElements   = maxElements;
   mNumElements   = 0;
}

void* Feature32SDescriptor::GetDataPtr() const
{
   return mpData;
}

void* Feature32SDescriptor::GetDataPtrPhys() const
{
   return mpDataPhys;
}

int32_t Feature32SDescriptor::GetSpan() const
{
   return (int32_t)sizeof(Feature32S);
}

int32_t Feature32SDescriptor::GetSize() const
{
   return mMaxElements;
}

int32_t Feature32SDescriptor::GetCount() const
{
   return mNumElements;
}

int32_t Feature32SDescriptor::SetCount(int32_t count)
{
   if (count < 0 || count > mMaxElements)
      return -1;
   mNumElements = count;
   return 0;
}

int32_t Feature32SDescriptor::Add(int32_t x, int32_t y, int32_t strength)
{
   if (mNumElements >= mMaxElements)
      return -1;
    
   ((Feature32S*)mpData)[mNumElements].position.x = x;
   ((Feature32S*)mpData)[mNumElements].position.y = y;
   ((Feature32S*)mpData)[mNumElements].strength = strength;
   ++mNumElements;
   return 0;
}

int32_t Feature32SDescriptor::Remove(int32_t ind)
{
   if (ind >= mNumElements || ind < 0 || mNumElements == 0)
      return -1;
   
   for (int32_t i = ind; i < mNumElements-1; ++i) {
      ((Feature32S*)mpData)[i].position.x = ((Feature32S*)mpData)[i+1].position.x;
      ((Feature32S*)mpData)[i].position.y = ((Feature32S*)mpData)[i+1].position.y;
      ((Feature32S*)mpData)[i].strength = ((Feature32S*)mpData)[i+1].strength;
   }
   --mNumElements;
   return 0;
}

int32_t Feature32SDescriptor::Set(int32_t ind, int32_t x, int32_t y, int32_t strength)
{
   if (ind >= mMaxElements || ind < 0)
      return -1;
    
   ((Feature32S*)mpData)[ind].position.x = x;
   ((Feature32S*)mpData)[ind].position.y = y;
   ((Feature32S*)mpData)[ind].strength = strength;
   return 0;
}

Feature32S& Feature32SDescriptor::GetFeature(int32_t ind) {
   if (ind >= mMaxElements || ind < 0)
      return ((Feature32S*)mpData)[0];
   return ((Feature32S*)mpData)[ind];
}

Feature32S& Feature32SDescriptor::operator[] (int32_t ind) {
   return GetFeature(ind);
}

}
//  ----------------------------------------------------------

