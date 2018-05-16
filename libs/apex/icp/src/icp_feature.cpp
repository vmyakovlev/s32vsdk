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
*  @file FeatureDescriptor.cpp
*  @brief ICP Feature Descriptor
************************************************************************************/
#include "icp_feature.h"
#include "icp_types.h"

namespace icp {
	
FeatureDescriptor::FeatureDescriptor(): mpData(0), mpDataPhys(0), mMaxElements(0),
                                    mNumElements(0)
{
}

FeatureDescriptor::FeatureDescriptor(
               void* const lpData,
               void* const lpDataPhys,
               int32_t maxElements): mpData(lpData), mpDataPhys(lpDataPhys),
                                     mMaxElements(maxElements), mNumElements(0)
{
}

FeatureDescriptor::~FeatureDescriptor()
{
}


void FeatureDescriptor::Init(
             void* const lpData,
             void* const lpDataPhys,
             int32_t maxElements)
{
   mpData         = lpData;
   mpDataPhys     = lpDataPhys;
   mMaxElements   = maxElements;
   mNumElements   = 0;
}

void* FeatureDescriptor::GetDataPtr() const
{
   return mpData;
}

void* FeatureDescriptor::GetDataPtrPhys() const
{
   return mpDataPhys;
}

int32_t FeatureDescriptor::GetSpan() const
{
   return (int32_t)sizeof(Feature);
}

int32_t FeatureDescriptor::GetSize() const
{
   return mMaxElements;
}

int32_t FeatureDescriptor::GetCount() const
{
   return mNumElements;
}

int32_t FeatureDescriptor::SetCount(int32_t count)
{
   if (count < 0 || count > mMaxElements)
      return -1;
   mNumElements = count;
   return 0;
}

int32_t FeatureDescriptor::Add(int16_t x, int16_t y, int16_t strength)
{
   if (mNumElements >= mMaxElements)
      return -1;
    
   ((Feature*)mpData)[mNumElements].position.x = x;
   ((Feature*)mpData)[mNumElements].position.y = y;
   ((Feature*)mpData)[mNumElements].strength = strength;
   ++mNumElements;
   return 0;
}

int32_t FeatureDescriptor::Remove(int32_t ind)
{
   if (ind >= mNumElements || ind < 0 || mNumElements == 0)
      return -1;
   
   for (int32_t i = ind; i < mNumElements-1; ++i) {
      ((Feature*)mpData)[i].position.x = ((Feature*)mpData)[i+1].position.x;
      ((Feature*)mpData)[i].position.y = ((Feature*)mpData)[i+1].position.y;
      ((Feature*)mpData)[i].strength = ((Feature*)mpData)[i+1].strength;
   }
   --mNumElements;
   return 0;
}

int32_t FeatureDescriptor::Set(int32_t ind, int16_t x, int16_t y, int16_t strength)
{
   if (ind >= mMaxElements || ind < 0)
      return -1;
    
   ((Feature*)mpData)[ind].position.x = x;
   ((Feature*)mpData)[ind].position.y = y;
   ((Feature*)mpData)[ind].strength = strength;
   return 0;
}

Feature& FeatureDescriptor::GetFeature(int32_t ind) const {
   if (ind >= mMaxElements || ind < 0)
      return ((Feature*)mpData)[0];
   return ((Feature*)mpData)[ind];
}

Feature& FeatureDescriptor::operator[] (int32_t ind) {
   return GetFeature(ind);
}


const Feature& FeatureDescriptor::operator[] (const int32_t ind) const {
   return GetFeature(ind);
}

}
//  ----------------------------------------------------------

