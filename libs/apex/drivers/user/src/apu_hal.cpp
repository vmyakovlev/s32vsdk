/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2011 All right reserved.
 *
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 ******************************************************************************/

#include "apu_hal.h"
#include "oal.h"
#include "hw_reg_user.h"
#include "linked_list_utilities.h"
#include "cmem_array_map.h"
#include "mcdma_hal.h"
#include "global_hal.h"
#include "cmem_if_hal.h"
#include "GDC_OAL_Semaphore.hpp"
#include <string.h> //for memcpy and memset
#include <stdio.h>

void RetApuInfoFromCmemIf (int        lApexID,
                           int32_t    lApuId,
                           HW_REG_ID* lpCmemBroadcastReg,
                           uint32_t*  lpPmemOffsetInBytes,
                           uint32_t*  lpDmemOffsetInBytes,
                           uint32_t*  lpDmemSizeInBytes)
{
   CMEM_IF_APU_CFG lCmemIfCfg = cmem_if_hal_query_apu_cfg(lApexID);
   uint32_t lPmemOffset = cmem_if_hal_query_apu_pmem_start(lApexID, lApuId);
   uint32_t lDmemOffset = cmem_if_hal_query_apu_dmem_start(lApexID, lApuId);

   *lpPmemOffsetInBytes = lPmemOffset;
   *lpDmemOffsetInBytes = lDmemOffset;

   switch(lCmemIfCfg)
   {
   case CFG_321__APU_0_CU_0_31_SMEM_0_1:
      *lpCmemBroadcastReg = HW_REG_CMEM_BOC_31_00;
      *lpDmemSizeInBytes = (lPmemOffset >= lDmemOffset) ? lPmemOffset-lDmemOffset : (1024*64)-lDmemOffset;
      break;
   case CFG_642__APU_0_CU_0_63_SMEM_0_3:
      *lpCmemBroadcastReg = HW_REG_CMEM_BOC_63_00;
      *lpDmemSizeInBytes = (lPmemOffset >= lDmemOffset) ? lPmemOffset-lDmemOffset : (1024*128)-lDmemOffset;
      break;
   case CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3:
      if (0 == lApuId)
      {
         *lpCmemBroadcastReg = HW_REG_CMEM_BOC_31_00;
         *lpDmemSizeInBytes = (lPmemOffset >= lDmemOffset) ? lPmemOffset-lDmemOffset : (1024*64)-lDmemOffset;
      }
      else if (1 == lApuId)
      {
         *lpCmemBroadcastReg = HW_REG_CMEM_BOC_63_32;
         *lpDmemSizeInBytes = (lPmemOffset >= lDmemOffset) ? lPmemOffset-lDmemOffset : (1024*128)-lDmemOffset;
      }
      break;
	  
	  default:
	     break;
   }
}

void apu_hal_Reset(int apexID, unsigned int apuid)
{
   switch (apuid)
   {
   case 0:
      global_hal_Apu0Reset(apexID);
      break;
   case 1:
      global_hal_Apu1Reset(apexID);
      break;
   }
}

//create one semaphore for each APEX
static GDC_OAL_SEMAPHORE gsApuEnableSemaphore[2] = {0}; //semaphore to protect apu enable/disable (requires read-modify-write)

int32_t apu_hal_Init()
{
   int32_t lRetVal = 0;

   LIB_RESULT lRes0 = GDC_OAL_SemaphoreCreate(&gsApuEnableSemaphore[0], "apex0_apu_enable", GDC_OAL_SEMAPHORE_COUNTING, 1);
   LIB_RESULT lRes1 = GDC_OAL_SemaphoreCreate(&gsApuEnableSemaphore[1], "apex1_apu_enable", GDC_OAL_SEMAPHORE_COUNTING, 1);

   if (lRes0 != LIB_SUCCESS || lRes1 != LIB_SUCCESS)
   {
      //semaphore creation failed

      if (gsApuEnableSemaphore[0] != 0)
         GDC_OAL_SemaphoreDelete(gsApuEnableSemaphore[0]);
      if (gsApuEnableSemaphore[1] != 0)
         GDC_OAL_SemaphoreDelete(gsApuEnableSemaphore[1]);

      lRetVal = 1;
   }

   return lRetVal;
}

int32_t apu_hal_Deinit()
{
   int32_t lRetVal = 0;

   LIB_RESULT lRes0 = LIB_SUCCESS;
   LIB_RESULT lRes1 = LIB_SUCCESS;

   if (gsApuEnableSemaphore[0] != 0)
   {
      lRes0 = GDC_OAL_SemaphoreDelete(gsApuEnableSemaphore[0]);
      gsApuEnableSemaphore[0] = 0;
   }
   if (gsApuEnableSemaphore[1] != 0)
   {
      lRes1 = GDC_OAL_SemaphoreDelete(gsApuEnableSemaphore[1]);
      gsApuEnableSemaphore[1] = 0;
   }

   if(lRes0 != LIB_SUCCESS ||
      lRes1 != LIB_SUCCESS)
   {
      lRetVal = 1;
   }

   return lRetVal;
}

void apu_hal_Enable(int lApexID, unsigned int apuid, unsigned int isEnable)
{
   LIB_RESULT lLibResult = LIB_FAILURE;

   lLibResult = GDC_OAL_SemaphoreObtain(gsApuEnableSemaphore[lApexID], 0xFFFFFFFF);

   if (lLibResult == LIB_SUCCESS)
   {
      switch (apuid)
      {
      case 0:
         if (isEnable) { global_hal_Apu0Enable(lApexID); } else { global_hal_Apu0Disable(lApexID); }
         break;
      case 1:
         if (isEnable) { global_hal_Apu1Enable(lApexID); } else { global_hal_Apu1Disable(lApexID); }
         break;
      }

      lLibResult = GDC_OAL_SemaphoreRelease(gsApuEnableSemaphore[lApexID]);
   }
}

#define NUM_CUS_FOR_LOAD_CALC 128

void apu_hal_QueryLoad_MEM_REQ(const LOAD_SEGMENT_t* seg_addr, int32_t* pCmemReq, int32_t* pDmemReq, int32_t* pPmemReq)
{
   //this function basically takes the 'last' of each type encountered in the segment array and assumes it is the
   //end of memory of that type!!!

   const int32_t binNumCu = NUM_CUS_FOR_LOAD_CALC;
   const int32_t cmem_end = 0x1000;

   int32_t lIndex = 0;
   SEG_HOST_TYPE memory_type = (SEG_HOST_TYPE)seg_addr[lIndex][0];

   while (memory_type != -1)
   {
      SEG_HOST_TYPE dst_offset  = seg_addr[lIndex][2];
      SEG_HOST_TYPE dst_size    = seg_addr[lIndex][3];

      if (memory_type == 0) //PMEM
      {
         *pPmemReq = dst_offset + dst_size; //SHOULD ONLY BE ONE OF THESE
      }

      if (memory_type == 1) //DMEM
      {
         if (dst_offset == 0x10800 || dst_offset == 0x10804)
         {
            //ignore (some virtual *.bcf thing Christina can explain)
         }
         else
         {
            *pDmemReq = dst_offset + dst_size;
         }
      }

      if (memory_type == 2) //CMEM
      {
         if (dst_offset == cmem_end)
         {
            //ignore (related to CMEM_RESERVED stuff)
         }
         else
         {
            *pCmemReq = dst_offset + dst_size/binNumCu;
         }
      }

      memory_type = seg_addr[++lIndex][0];
   }
}

#define USE_MEM_FUNCTIONS 0 //in STANDALONE there are issues with memcpy and memset so they are disabled by default
#define VMEM_INIT 1
#define DMEM_INIT 1

//NOTE: this function is currently used only for debugging/testing
int32_t apu_hal_LoadSegment(int apexID, int32_t apuid,
                            const LOAD_SEGMENT_t* seg_addr)
{
   int32_t lRetVal = 0;
   int n;

   HW_REG_ID lCmemBroadcastReg = HW_REG_CMEM_BOC_31_00;
   uint32_t lPmemOffsetInBytes = 0;
   uint32_t lDmemOffsetInBytes = 0;
   uint32_t lDmemSizeInBytes = 0;
   volatile SEG_MEM_TYPE* dmem;
   volatile SEG_MEM_TYPE* base_addr[3];
   volatile SEG_MEM_TYPE* cmem;
   SEG_HOST_TYPE memory_type;

   //Disable and reset APU 'apuid'
   apu_hal_Enable(apexID, apuid, 0);
   apu_hal_Reset(apexID, apuid);

   RetApuInfoFromCmemIf (apexID, apuid, &lCmemBroadcastReg, &lPmemOffsetInBytes, &lDmemOffsetInBytes, &lDmemSizeInBytes);

   base_addr[0] = (SEG_MEM_TYPE*)((int64_t)ReturnVirtualHwPtr(apexID, HW_REG_SMEM)     + lPmemOffsetInBytes);
   base_addr[1] = (SEG_MEM_TYPE*)((int64_t)ReturnVirtualHwPtr(apexID, HW_REG_SMEM)     + lDmemOffsetInBytes);
   base_addr[2] = (SEG_MEM_TYPE*)((int64_t)ReturnVirtualHwPtr(apexID, HW_REG_CMEM_GOC) + 0);

#if (1 == VMEM_INIT) // clear VMEM (use cmem broadcast functionality)
   cmem = (volatile SEG_MEM_TYPE*)ReturnVirtualHwPtr(apexID, lCmemBroadcastReg);
#if (1 == USE_MEM_FUNCTIONS)
   memset((void*)cmem, 0, VMEM_SIZE_MAX*2*8);
#else
   for (int i=0; i<VMEM_SIZE_MAX*2*8/4; i++)
      cmem[i] = 0;
#endif //USE_MEM_FUNCTIONS
#endif //VMEM_INIT

#if (1 == DMEM_INIT) //clear DMEM
   dmem = base_addr[1];
#if (1 == USE_MEM_FUNCTIONS)
   memset((void*)dmem, 0, lDmemSizeInBytes);
#else
   for (int i=0; i<lDmemSizeInBytes/4; i++)
      dmem[i] = 0;
#endif //USE_MEM_FUNCTIONS
#endif //DMEM_INIT

   // Load the APU with HARD CODED outputs from
   // dart/bridge
   // --------------------------------------------------------------
   // Load program and data memory
   n = 0;
   memory_type = (SEG_HOST_TYPE)seg_addr[n][0];
   while (memory_type != -1)
   {
      //values directly from APU LOAD file
      SEG_MEM_TYPE* src_ptr = (SEG_MEM_TYPE*)seg_addr[n][1];
      SEG_HOST_TYPE dst_offset_in_bytes    = seg_addr[n][2];
      SEG_HOST_TYPE data_size_in_bytes     = seg_addr[n][3];

      //'adjusted' values
      SEG_HOST_TYPE dst_offset_32 = (dst_offset_in_bytes)>>2;
      SEG_HOST_TYPE data_size_32 = ((dst_offset_in_bytes + data_size_in_bytes)>>2) - (dst_offset_in_bytes>>2);

      // points to dst memory
      // 32-bit write
      volatile SEG_MEM_TYPE* dst_addr  = (volatile SEG_MEM_TYPE*)(base_addr[memory_type] + dst_offset_32);

      if (src_ptr == 0)
      {
         //ZERO INITIALIZED sections; all CMEM/DMEM is zeroed prior to loading, so zero initialized sections are skipped

         //for (int32_t i=0; i<dat_siz32; ++i)
         //   dst_addr[i] = 0;
      }
      else if  (((dst_offset_in_bytes < APU_0_DMEM_SIZE) && (memory_type == 1)) ||
                 (memory_type == 0)
               )
      {
#if (1 == USE_MEM_FUNCTIONS)
         memcpy((void*)dst_addr, (void*)src_ptr, data_size_32*4);
#else
         for (int i=0; i<data_size_32*4/4; i++)
            dst_addr[i] = src_ptr[i];
#endif //USE_MEM_FUNCTIONS
      }
      else if (memory_type == 2) //CMEM
      {
         printf("apu_hal_LoadSegment(...) -> non-zero initialized global vector variable detected (size = %d bytes); non-zero vector initialization is not supported\n", data_size_in_bytes/NUM_CUS_FOR_LOAD_CALC);
         lRetVal = 1;
      }

      memory_type = seg_addr[++n][0];
   }

   return lRetVal;
}

int32_t apu_hal_CopyLoadToOalMemory(const SEG_MEM_TYPE* pLoadPmem,
                                    int32_t loadPmemSize,
                                    const SEG_MEM_TYPE* pLoadDmem,
                                    int32_t loadDmemSize,
                                    const SEG_MEM_TYPE* pLoadCmem,
                                    int32_t loadCmemSize,
                                    void** loadOalMemoryPtr_PMEM,
                                    void** loadOalMemoryPtr_DMEM,
                                    void** loadOalMemoryPtr_CMEM)
{
   int lRetVal = 0;
   uint32_t* lpDst = 0;

   //PMEM
   if (loadPmemSize > 0)
   {
      if (*loadOalMemoryPtr_PMEM==0) *loadOalMemoryPtr_PMEM = OAL_MemoryAllocFlag(loadPmemSize*4, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);
      if (*loadOalMemoryPtr_PMEM != 0)
      {
         lpDst = (uint32_t*)OAL_MemoryReturnAddress(*loadOalMemoryPtr_PMEM, ACCESS_CH_WB);
         memcpy((void*)lpDst, (void*)pLoadPmem, loadPmemSize*4);
         OAL_MemoryFlushAndInvalidate(OAL_MemoryReturnAddress(*loadOalMemoryPtr_PMEM, ACCESS_CH_WB), loadPmemSize*4);
      }
      else //failed to allocate memory
      {
         lRetVal = 1;
      }
   }

   //DMEM
   if (loadDmemSize > 0)
   {
      if (*loadOalMemoryPtr_DMEM==0) *loadOalMemoryPtr_DMEM = OAL_MemoryAllocFlag(loadDmemSize*4, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);
      if (*loadOalMemoryPtr_DMEM != 0)
      {
         lpDst = (uint32_t*)OAL_MemoryReturnAddress(*loadOalMemoryPtr_DMEM, ACCESS_CH_WB);
         memcpy((void*)lpDst, (void*)pLoadDmem, loadDmemSize*4);
         OAL_MemoryFlushAndInvalidate(OAL_MemoryReturnAddress(*loadOalMemoryPtr_DMEM, ACCESS_CH_WB), loadDmemSize*4);
      }
      else //failed to allocate memory
      {
         lRetVal = 1;
      }
   }

   //CMEM
   if (loadCmemSize > 0)
   {
      if (*loadOalMemoryPtr_CMEM==0) *loadOalMemoryPtr_CMEM = OAL_MemoryAllocFlag(loadCmemSize*4, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);
      if (*loadOalMemoryPtr_CMEM != 0)
      {
         lpDst = (uint32_t*)OAL_MemoryReturnAddress(*loadOalMemoryPtr_CMEM, ACCESS_CH_WB);
         memcpy((void*)lpDst, (void*)pLoadCmem, loadCmemSize*4);
         OAL_MemoryFlushAndInvalidate(OAL_MemoryReturnAddress(*loadOalMemoryPtr_CMEM, ACCESS_CH_WB), loadCmemSize*4);
      }
      else //failed to allocate memory
      {
         lRetVal = 1;
      }
   }

   if (lRetVal != 0)
   {
      if (*loadOalMemoryPtr_PMEM) OAL_MemoryFree(*loadOalMemoryPtr_PMEM);
      if (*loadOalMemoryPtr_DMEM) OAL_MemoryFree(*loadOalMemoryPtr_DMEM);
      if (*loadOalMemoryPtr_CMEM) OAL_MemoryFree(*loadOalMemoryPtr_CMEM);
   }

   return lRetVal;
}

int32_t apu_hal_GenLoadLL(int lApexID, int32_t apuid,
                          const LOAD_SEGMENT_t* seg_addr,
                          const SEG_MEM_TYPE* pPmemLoadBase, //base address of *_LOAD_PMEM so the offset can be calculated from the src address in LOAD_SEGMENTS
                          const SEG_MEM_TYPE* pDmemLoadBase, //base address of *_LOAD_DMEM so the offset can be calculated from the src address in LOAD_SEGMENTS
                          const SEG_MEM_TYPE* /*pCmemLoadBase*/, //base address of *_LOAD_CMEM so the offset can be calculated from the src address in LOAD_SEGMENTS
                          uint32_t loadPhysPtr_PMEM,         //base address (physical) of *_LOAD_PMEM
                          uint32_t loadPhysPtr_DMEM,         //base address (physical) of *_LOAD_DMEM
                          uint32_t /*loadPhysPtr_CMEM*/,     //base address (physical) of *_LOAD_CMEM
                          uint32_t lPmemOffsetInBytes,
                          uint32_t lDmemOffsetInBytes,
                          uint32_t lDmemSizeInBytes,
                          CMEM_IF_APU_CFG lApuCfg,
                          void** llOalMemoryPtr,
                          int32_t lClover389Flag)
{
   int32_t lRetVal = 0;

   volatile SEG_MEM_TYPE* base_addr[3] = {(SEG_MEM_TYPE*)((uint64_t)ReturnPhysicalHwPtr(lApexID, HW_REG_SMEM)     + lPmemOffsetInBytes),
                                          (SEG_MEM_TYPE*)((uint64_t)ReturnPhysicalHwPtr(lApexID, HW_REG_SMEM)     + lDmemOffsetInBytes),
                                          (SEG_MEM_TYPE*)((uint64_t)ReturnPhysicalHwPtr(lApexID, HW_REG_CMEM_GOC) + 0)};

   //determine number of load segments
   int lNumSegments = 0;
   SEG_HOST_TYPE memory_type = (SEG_HOST_TYPE)seg_addr[lNumSegments][0];
   while (memory_type != -1)
   {
      memory_type = seg_addr[++lNumSegments][0];
   }

   //allocate memory for the linked list
   int32_t lLinkedListSizeInBytes = LLE_SIZE_1D_FILL*4*2 + LLE_SIZE_1D_MEM_MEM*4*lNumSegments;
   if (*llOalMemoryPtr==0) *llOalMemoryPtr = OAL_MemoryAllocFlag(lLinkedListSizeInBytes, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);

   if (*llOalMemoryPtr != 0)
   {
      uint32_t* lpLinkedList = (uint32_t*)OAL_MemoryReturnAddress(*llOalMemoryPtr, ACCESS_CH_WB);
      uint32_t* lpLLE = lpLinkedList; //pointer to the 'current' linked list entry

      //----------------------------------------------------------------------------------------------
      //ZERO cmem/dmem: prepend the apu load linked list with cmem/dmem 'zeroing' linked list entries
      //----------------------------------------------------------------------------------------------

      //determine the appropriate cmem broadcast register based on lApuCfg and apuid
      HW_REG_ID lCmemBroadcastReg = HW_REG_CMEM_BOC_31_00;
      switch(lApuCfg)
      {
      case CFG_321__APU_0_CU_0_31_SMEM_0_1:
         lCmemBroadcastReg = HW_REG_CMEM_BOC_31_00;
         break;
      case CFG_642__APU_0_CU_0_63_SMEM_0_3:
         lCmemBroadcastReg = HW_REG_CMEM_BOC_63_00;
         break;
      case CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3:
         if (0 == apuid)
            lCmemBroadcastReg = HW_REG_CMEM_BOC_31_00;
         else if (1 == apuid)
            lCmemBroadcastReg = HW_REG_CMEM_BOC_63_32;
         break;
      default:
         lRetVal = 1;
         break;
      }

      //clear VMEM
      LLE_1D_Fill(lpLLE, (uint32_t)(uintptr_t)ReturnPhysicalHwPtr(lApexID, lCmemBroadcastReg), VMEM_SIZE_MAX*2*8);
      if (1 == lClover389Flag)
         LLE_DMA_SetBurstToINCR1(lpLLE);
      lpLLE += LLE_SIZE_1D_FILL;

      //clear DMEM
      LLE_1D_Fill(lpLLE, (uint32_t)(uintptr_t)ReturnPhysicalHwPtr(lApexID, HW_REG_SMEM) + lDmemOffsetInBytes, lDmemSizeInBytes);
      if (1 == lClover389Flag)
         LLE_DMA_SetBurstToINCR1(lpLLE);
      lpLLE += LLE_SIZE_1D_FILL;

      //need to do this to deal with HW bug related to having mem-mem & mem-periph in same list
      (lpLLE-LLE_SIZE_1D_FILL)[2] |= 0xC0000000; //set 'pause', 'interrupt enable', but DO NOT set 'last' since we want to re-kick for the rest
      //LLE_DMA_Finalize(lpLLE-LLE_SIZE_1D_FILL);

      //----------------------------------------------------------------------------------------------
      // Load program and data memory
      //----------------------------------------------------------------------------------------------

      int n = 0;
      SEG_HOST_TYPE memory_type = (SEG_HOST_TYPE)seg_addr[n][0];
      while (memory_type != -1)
      {
         //values directly from APU LOAD file
         SEG_MEM_TYPE* src_ptr = (SEG_MEM_TYPE*)seg_addr[n][1];
         SEG_HOST_TYPE dst_offset_in_bytes    = seg_addr[n][2];
         SEG_HOST_TYPE data_size_in_bytes     = seg_addr[n][3];
         SEG_HOST_TYPE mask;

#if 0
         //'adjusted' values (32-bit alignment)
         mask = 0x3;
         SEG_HOST_TYPE dst_offset_32 = (dst_offset_in_bytes)>>2;
         SEG_HOST_TYPE data_size_32 = ((dst_offset_in_bytes&mask) + data_size_in_bytes + 3) >> 2;

         //32-bit
         SEG_HOST_TYPE aligned_data_size_in_bytes = data_size_32<<2;
         volatile SEG_MEM_TYPE* aligned_dst_addr = (volatile SEG_MEM_TYPE*)(base_addr[memory_type] + dst_offset_32);
         mask = 0x3;
#else
         //'adjusted' values (128-bit alignment)
         mask = 0xF;
         SEG_HOST_TYPE dst_offset_128 = (dst_offset_in_bytes)>>4;
         SEG_HOST_TYPE data_size_128 = ((dst_offset_in_bytes&mask) + data_size_in_bytes + 15) >> 4;

         //128-bit
         SEG_HOST_TYPE aligned_data_size_in_bytes = data_size_128<<4;
         volatile SEG_MEM_TYPE* aligned_dst_addr = (volatile SEG_MEM_TYPE*)(base_addr[memory_type] + (dst_offset_128<<2));
#endif

         //NOTE: ignoring CMEM memory_type (shouldn't be used)
         if (src_ptr == 0)
         {
            //ZERO INITIALIZED sections; all CMEM/DMEM is zeroed prior to loading, so zero initialized sections are skipped
         }
         else if  ( ((dst_offset_in_bytes < APU_0_DMEM_SIZE) && (memory_type == 1))  ||
                    (memory_type == 0))
         {
            SEG_HOST_TYPE srcPtrVal = (SEG_HOST_TYPE)src_ptr;
            SEG_MEM_TYPE* srcPtrTmp = 0;
            if (0 == memory_type) //LOAD_PMEM
            {
               srcPtrTmp = (SEG_MEM_TYPE*)((uintptr_t)loadPhysPtr_PMEM + ((srcPtrVal-(SEG_HOST_TYPE)pPmemLoadBase)&(~mask)));
            }
            else if (1 == memory_type) //LOAD_DMEM
            {
               srcPtrTmp = (SEG_MEM_TYPE*)((uintptr_t)loadPhysPtr_DMEM + ((srcPtrVal-(SEG_HOST_TYPE)pDmemLoadBase)&(~mask)));
            }

            LLE_1D_MEM_MEM(lpLLE, (uint32_t)(uintptr_t)srcPtrTmp, (uint32_t)(uintptr_t)aligned_dst_addr, aligned_data_size_in_bytes);
            if (1 == lClover389Flag)
               LLE_DMA_SetBurstToINCR1(lpLLE);
            lpLLE += LLE_SIZE_1D_MEM_MEM;
         }
         else if (2 == memory_type) //LOAD_CMEM
         {
            printf("apu_hal_GenLoadLL(...) -> non-zero initialized global vector variable detected (size = %d bytes); non-zero vector initialization is not supported\n", data_size_in_bytes/NUM_CUS_FOR_LOAD_CALC);
            lRetVal = 1;
         }

         memory_type = seg_addr[++n][0];
      }

      LLE_DMA_Finalize(lpLLE-LLE_SIZE_1D_MEM_MEM);

      OAL_MemoryFlushAndInvalidate(OAL_MemoryReturnAddress(*llOalMemoryPtr, ACCESS_CH_WB), lLinkedListSizeInBytes);
   }
   else //failed to allocate memory for linked list
   {
      lRetVal = 1;
   }

   return  lRetVal;
}

int32_t apu_hal_LoadSegments_DMA(int lApexID, int32_t apuid,
                                 int32_t dmaCh,
                                 void* pLoadSegmentsLLBufferOal)
{
   int32_t lRetVal = 0;

   //Disable and reset APU 'apuid'
   apu_hal_Enable(lApexID, apuid, 0);
   apu_hal_Reset(lApexID, apuid);

   //program dma transfer (currently includes both zeroing and loading)
   mcdma_hal_program_seq_linkedlist(lApexID, dmaCh, (uintptr_t)OAL_MemoryReturnAddress(pLoadSegmentsLLBufferOal, ACCESS_PHY));
   mcdma_hal_set_memory_fill_words(lApexID, 0, 0, 0, 0); //TODO: should add this to the linked list

   //start zeroing part
   mcdma_hal_start(lApexID, dmaCh);
   if (0 == mcdma_hal_wait_timeout(lApexID, dmaCh, 500000))
   {
      //start apu load part
      mcdma_hal_start(lApexID, dmaCh);
      if (0 == mcdma_hal_wait_timeout(lApexID, dmaCh, 500000))
      {
         //everything completed successfully
      }
      else //apu load transfer timed out
      {
         lRetVal = -2;
      }
   }
   else //zeroing timed out
   {
      lRetVal = -1;
   }

#if 0 //some debugging code to split transfers
   //zeroing part
   mcdma_hal_set_memory_fill_words(lApexID, 0, 0, 0, 0); //TODO: should add this to the linked list
   mcdma_hal_program_seq_linkedlist(lApexID, dmaCh, (uintptr_t)OAL_MemoryReturnAddress(pLoadSegmentsLLBufferOal, ACCESS_PHY));
   mcdma_hal_start(lApexID, dmaCh);
   mcdma_hal_wait(lApexID, dmaCh);

   //apu load part
   mcdma_hal_program_seq_linkedlist(lApexID, dmaCh+1, (uintptr_t)OAL_MemoryReturnAddress(pLoadSegmentsLLBufferOal, ACCESS_PHY)+(2*LLE_SIZE_1D_FILL*4));
   mcdma_hal_start(lApexID, dmaCh+1);
   mcdma_hal_wait(lApexID, dmaCh+1);
#endif

   return lRetVal;
}

