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

#include <stdio.h>
#include <stdlib.h>
#include <hw_reg_user.h>
#include <apex_642cl_map.h>

static
unsigned long gsVirtualHwPtrArray [HW_REG_NUM] = { 0 };

static
unsigned char gsVirtualMappingComplete = 0;

static
const
unsigned long gsPhysicalHwPtrArray[HW_REG_NUM][2] =
{
   {APEX_642CL_SMEM_BASE_ADDRESS,         0xFFFF},  // HW_REG_SMEM

   {APEX_642CL_MCDMA_BASE_ADDRESS,        0xFFFF},  // HW_REG_MCDMA
   {APEX_642CL_MOTION_COMP_BASE_ADDRESS,  0xFFFF},  // HW_REG_MODMA
   {APEX_642CL_STREAM_DMA_BASE_ADDRESS,   0xFFFF},  // HW_REG_STIO
   {APEX_642CL_SEQ_BASE_ADDRESS,          0xFFFF},  // HW_REG_USEQ
   {APEX_642CL_GLOBAL_BASE_ADDRESS,       0xFFFF},  // HW_REG_GLOBAL
   {APEX_642CL_HOST_DEBUG_BASE_ADDRESS,   0xFFFF},  // HW_REG_DEBUG
   {APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS,  0xFFFF},  // HW_REG_DMEM_FIFO_0
   {APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS,  0xFFFF},  // HW_REG_DMEM_FIFO_1
   {APEX_642CL_CMEM_IF_BASE_ADDRESS,      0xFFFF},  // HW_REG_CMEM_IF
   {APEX_642CL_HRSZ_BASE_ADDRESS,         0xFFFF},  // HW_REG_HOR_RESIZER

   {APEX_642CL_CMEM_BASE_ADDRESS,            0x40000}, // HW_REG_CMEM_GOC_63_00

   {APEX_642CL_CMEM_BASE_ADDRESS+0x00080000, 0x40000}, // HW_REG_CMEM_SOC_31_00 & HW_REG_CMEM_SOC_63_32
   {APEX_642CL_CMEM_BASE_ADDRESS+0x00100000, 0x40000}, // HW_REG_CMEM_SOC_63_00

   {APEX_642CL_CMEM_BASE_ADDRESS+0x00300000, 0x40000}, // HW_REG_CMEM_BOC_31_00
   {APEX_642CL_CMEM_BASE_ADDRESS+0x00310000, 0x40000}, // HW_REG_CMEM_BOC_63_32
   {APEX_642CL_CMEM_BASE_ADDRESS+0x00340000, 0x40000}  // HW_REG_CMEM_BOC_63_00
};

void* ReturnVirtualHwPtr (int lApexID, HW_REG_ID lHwRegId)
{
	if (lApexID == 0) return (void*)(0x0000000074000000 + gsVirtualHwPtrArray[lHwRegId]);
	if (lApexID == 1) return (void*)(0x0000000078000000 + gsVirtualHwPtrArray[lHwRegId]);
	return (void*)0x0;
}

void* ReturnPhysicalHwPtr (int lApexID, HW_REG_ID lHwRegId)
{
   if (lApexID == 0) return (void*)(0x0000000074000000 + gsPhysicalHwPtrArray[lHwRegId][0]);
   if (lApexID == 1) return (void*)(0x0000000078000000 + gsPhysicalHwPtrArray[lHwRegId][0]);
   return (void*)0x0;
}
