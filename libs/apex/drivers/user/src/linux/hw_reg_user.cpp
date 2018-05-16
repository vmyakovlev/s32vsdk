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

//#include <stdio.h>
//#include <stdlib.h

#include <hw_reg_user.h>
#include <oal.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define APEX_642CL_MCDMA_BASE_ADDRESS                                            0x00100000
#define APEX_642CL_MOTION_COMP_BASE_ADDRESS                                      0x00110000
#define APEX_642CL_STREAM_DMA_BASE_ADDRESS                                       0x00120000
#define APEX_642CL_SEQ_BASE_ADDRESS                                              0x00130000
#define APEX_642CL_GLOBAL_BASE_ADDRESS                                           0x00140000
#define APEX_642CL_HOST_DEBUG_BASE_ADDRESS                                       0x00150000
#define APEX_642CL_DMEM_FIFO_STATUS_0_BASE_ADDRESS                               0x00160000
#define APEX_642CL_DMEM_FIFO_STATUS_1_BASE_ADDRESS                               0x00170000
#define APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS                                      0x00160000
#define APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS                                      0x00170000
#define APEX_642CL_CMEM_IF_BASE_ADDRESS                                          0x00180000
#define APEX_642CL_HRSZ_BASE_ADDRESS                                             0x00190000
#define APEX_642CL_CMEM_BASE_ADDRESS                                             0x01000000
#define APEX_642CL_VMEM_BASE_ADDRESS                                             0x01000000
#define APEX_642CL_SMEM_BASE_ADDRESS                                             0x00000000

#define APEX_0_BASE 0x74000000
#define APEX_1_BASE 0x78000000
#define HW_REG_NUM_2 (HW_REG_NUM * 2)
static
unsigned long gsVirtualHwPtrArray [HW_REG_NUM_2] = { 0 };

static
unsigned char gsVirtualMappingComplete = 0;

static
const
unsigned long gsPhysicalHwPtrArray[HW_REG_NUM_2][2] =
{
   {APEX_0_BASE + APEX_642CL_SMEM_BASE_ADDRESS,         0xFFFFF},  // HW_REG_SMEM
   {APEX_0_BASE + APEX_642CL_MCDMA_BASE_ADDRESS,        0xFFFF},  // HW_REG_MCDMA
   {APEX_0_BASE + APEX_642CL_MOTION_COMP_BASE_ADDRESS,  0xFFFF},  // HW_REG_MODMA
   {APEX_0_BASE + APEX_642CL_STREAM_DMA_BASE_ADDRESS,   0xFFFF},  // HW_REG_STIO
   {APEX_0_BASE + APEX_642CL_SEQ_BASE_ADDRESS,          0xFFFF},  // HW_REG_USEQ
   {APEX_0_BASE + APEX_642CL_GLOBAL_BASE_ADDRESS,       0xFFFF},  // HW_REG_GLOBAL
   {APEX_0_BASE + APEX_642CL_HOST_DEBUG_BASE_ADDRESS,   0xFFFF},  // HW_REG_DEBUG
   {APEX_0_BASE + APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS,  0xFFFF},  // HW_REG_DMEM_FIFO_0
   {APEX_0_BASE + APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS,  0xFFFF},  // HW_REG_DMEM_FIFO_1
   {APEX_0_BASE + APEX_642CL_CMEM_IF_BASE_ADDRESS,      0xFFFF},  // HW_REG_CMEM_IF
   {APEX_0_BASE + APEX_642CL_HRSZ_BASE_ADDRESS,         0xFFFF},  // HW_REG_HOR_RESIZER
   {APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS,            0x40000}, // HW_REG_CMEM_GOC_63_00
   {APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00080000, 0x40000}, // HW_REG_CMEM_SOC_31_00 & HW_REG_CMEM_SOC_63_32
   {APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00100000, 0x40000}, // HW_REG_CMEM_SOC_63_00
   {APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00300000, 0x40000}, // HW_REG_CMEM_BOC_31_00
   {APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00310000, 0x40000}, // HW_REG_CMEM_BOC_63_32
   {APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00340000, 0x40000},  // HW_REG_CMEM_BOC_63_00
   
   {APEX_1_BASE + APEX_642CL_SMEM_BASE_ADDRESS,         0xFFFFF},  // HW_REG_SMEM
   {APEX_1_BASE + APEX_642CL_MCDMA_BASE_ADDRESS,        0xFFFF},  // HW_REG_MCDMA
   {APEX_1_BASE + APEX_642CL_MOTION_COMP_BASE_ADDRESS,  0xFFFF},  // HW_REG_MODMA
   {APEX_1_BASE + APEX_642CL_STREAM_DMA_BASE_ADDRESS,   0xFFFF},  // HW_REG_STIO
   {APEX_1_BASE + APEX_642CL_SEQ_BASE_ADDRESS,          0xFFFF},  // HW_REG_USEQ
   {APEX_1_BASE + APEX_642CL_GLOBAL_BASE_ADDRESS,       0xFFFF},  // HW_REG_GLOBAL
   {APEX_1_BASE + APEX_642CL_HOST_DEBUG_BASE_ADDRESS,   0xFFFF},  // HW_REG_DEBUG
   {APEX_1_BASE + APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS,  0xFFFF},  // HW_REG_DMEM_FIFO_0
   {APEX_1_BASE + APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS,  0xFFFF},  // HW_REG_DMEM_FIFO_1
   {APEX_1_BASE + APEX_642CL_CMEM_IF_BASE_ADDRESS,      0xFFFF},  // HW_REG_CMEM_IF
   {APEX_1_BASE + APEX_642CL_HRSZ_BASE_ADDRESS,         0xFFFF},  // HW_REG_HOR_RESIZER
   {APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS,            0x40000}, // HW_REG_CMEM_GOC_63_00
   {APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00080000, 0x40000}, // HW_REG_CMEM_SOC_31_00 & HW_REG_CMEM_SOC_63_32
   {APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00100000, 0x40000}, // HW_REG_CMEM_SOC_63_00
   {APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00300000, 0x40000}, // HW_REG_CMEM_BOC_31_00
   {APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00310000, 0x40000}, // HW_REG_CMEM_BOC_63_32
   {APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00340000, 0x40000} 
};

int fd = -1;

int MapPhysicalToVirtual(unsigned long lPhysicalAddr, unsigned long lSizeInBytes, unsigned long *lVirtualAddr)
{
  if (fd < 0)
    fd = open("/dev/mem", O_RDWR);
  if (fd < 0)
     return -1;
  *lVirtualAddr = (unsigned long)mmap(0, lSizeInBytes, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, lPhysicalAddr);
   
   //close(fd);
  
   return 0;
}

int UnmapVirtualHwPtrArray()
{
   int i;
   for (i = 0; i < HW_REG_NUM_2; i++)
   {
      if (gsVirtualHwPtrArray[i] != 0)
      {
          munmap((void *)gsVirtualHwPtrArray[i], gsPhysicalHwPtrArray[i][1]);
      }
   }

   return 0;
}

int InitVirtualHwPtrArray()
{
   int i;
   int rVal = 0;

   if (0 == gsVirtualMappingComplete)
   {
      for  (i = 0; i < HW_REG_NUM_2; i++)
      {
         rVal = MapPhysicalToVirtual(
            gsPhysicalHwPtrArray[i][0], //physical address
            gsPhysicalHwPtrArray[i][1], //desired size in bytes
            &(gsVirtualHwPtrArray [i]));   //resulting virtual address
         if (rVal)
         {
            break;
         }
      }
   }
   
   if (0 == rVal)
   {
      gsVirtualMappingComplete = 1;
   }
   else
   {
      UnmapVirtualHwPtrArray();
   }
   
   return rVal;
}

int DeinitVirtualHwPtrArray()
{
   int rVal = 0;

   gsVirtualMappingComplete = 0;

   rVal = UnmapVirtualHwPtrArray();

   return rVal; 
}

void* ReturnVirtualHwPtr (int lApexID, HW_REG_ID lHwRegId)
{
	return (void*)(gsVirtualHwPtrArray[lHwRegId + lApexID*HW_REG_NUM]);
}

void* ReturnPhysicalHwPtr (int lApexID, HW_REG_ID lHwRegId)
{
	return (void*)(gsPhysicalHwPtrArray[lHwRegId + lApexID*HW_REG_NUM][0]);
}
