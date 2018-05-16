/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2015 All right reserved.
 *  
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 ******************************************************************************/

#include <icp_datatype.h>

#ifdef __cplusplus
extern "C"
#endif
int32_t RetDataTypeSizeInBytes(icp::DATATYPE lDataType)
{
   int32_t lDataSizeInBytes = 1;

   switch (lDataType)
   {
   case icp::DATATYPE_08U:
   case icp::DATATYPE_08S:
      lDataSizeInBytes = 1;
      break;
   case icp::DATATYPE_16U:
   case icp::DATATYPE_16S:
      lDataSizeInBytes = 2;
      break;
   case icp::DATATYPE_32U:
   case icp::DATATYPE_32S:
      lDataSizeInBytes = 4;
      break;
   };

   return lDataSizeInBytes;
}
