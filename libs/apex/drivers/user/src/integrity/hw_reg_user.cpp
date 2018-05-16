/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
 * @file apexdriver_regmap.c
 *
 * \\author Rostislav Hulik (Freescale Semiconductor)
 * \date 17. 12. 2013
 *
 */
#include <bsp.h>
#include <INTEGRITY.h>
#include <modules/ghs/bspsrc/support/memoryspace.h>
#include <modules/ghs/bspsrc/support/buildmemtable.h>
//#include <modules/ghs/bspsrc/support/atags.h>

#include <hw_reg_user.h>

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

/*======================================================================================*/
/* Typedef for APEX physical map definition						*/
typedef struct {
   void * physical;
   uint32_t size;
   const char *name;
} APEX_PHMAP;

/*======================================================================================*/
/* Initialization of defined APEX register mappping					*/
APEX_PHMAP apexdriver_physical_map[HW_REG_NUM_2] = 
{
  {(void *)(APEX_0_BASE + APEX_642CL_SMEM_BASE_ADDRESS), 	    0xFFFFF, 	"apex_reg_smem_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_MCDMA_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_mcdma_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_MOTION_COMP_BASE_ADDRESS),     0xFFFF, 	"apex_reg_mcen_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_STREAM_DMA_BASE_ADDRESS),	    0xFFFF, 	"apex_reg_streamdma_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_SEQ_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_sequencer_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_GLOBAL_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_global_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_HOST_DEBUG_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_debug_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS),	    0xFFFF, 	"apex_reg_dmemfifo0_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS),	    0xFFFF, 	"apex_reg_dmemfifo1_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_CMEM_IF_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_cmemif_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_HRSZ_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_hrsz_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS), 	    0x40000, 	"apex_reg_goc_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00080000), 0x40000, 	"apex_reg_soc32_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00100000), 0x40000, 	"apex_reg_soc64_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00300000), 0x40000, 	"apex_reg_boc31_00_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00310000), 0x40000, 	"apex_reg_boc63_32_1"},
  {(void *)(APEX_0_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00340000), 0x40000, 	"apex_reg_boc63_00_1"},
  
  {(void *)(APEX_1_BASE + APEX_642CL_SMEM_BASE_ADDRESS), 	    0xFFFFF, 	"apex_reg_smem_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_MCDMA_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_mcdma_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_MOTION_COMP_BASE_ADDRESS),     0xFFFF, 	"apex_reg_mcen_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_STREAM_DMA_BASE_ADDRESS),	    0xFFFF, 	"apex_reg_streamdma_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_SEQ_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_sequencer_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_GLOBAL_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_global_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_HOST_DEBUG_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_debug_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS),	    0xFFFF, 	"apex_reg_dmemfifo0_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS),	    0xFFFF, 	"apex_reg_dmemfifo1_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_CMEM_IF_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_cmemif_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_HRSZ_BASE_ADDRESS), 	    0xFFFF, 	"apex_reg_hrsz_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS), 	    0x40000, 	"apex_reg_goc_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00080000), 0x40000, 	"apex_reg_soc32_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00100000), 0x40000, 	"apex_reg_soc64_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00300000), 0x40000, 	"apex_reg_boc31_00_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00310000), 0x40000, 	"apex_reg_boc63_32_2"},
  {(void *)(APEX_1_BASE + APEX_642CL_CMEM_BASE_ADDRESS+0x00340000), 0x40000, 	"apex_reg_boc63_00_2"}
};


MemoryReservation apexregs[HW_REG_NUM_2];
/*======================================================================================*/
/* Array containing virtual mappings for APEX registers					*/
void *	apexdriver_virtual_map[HW_REG_NUM_2] = { 0 };

/*======================================================================================*/
/* Mapping done flag 									*/
static uint8_t apexdriver_mapping_done = 0;


/*======================================================================================*/
/* Function unmaps all APEX registers							*/
/*======================================================================================*/
int UnmapVirtualHwPtrArray()
{
  return 0;
}

/*======================================================================================*/
/* Function maps all APEX registers							*/
/*======================================================================================*/
int InitVirtualHwPtrArray()
{
  //ExtendedAddress	addr;
  if (apexdriver_mapping_done == 0)
  {
    int32_t i;
    for (i = 0; i < HW_REG_NUM_2; i++)
    { 
      apexregs[i].Attributes = MEMORY_RW | MEMORY_VOLATILE;
      apexregs[i].PhysicalSpaceNumber = 0;
      apexregs[i].First = (int64_t)apexdriver_physical_map[i].physical;
      apexregs[i].Last = (int64_t)apexdriver_physical_map[i].physical+apexdriver_physical_map[i].size;
      apexregs[i].Type = Other_MemoryType;
      apexregs[i].Fixed = true;
      apexregs[i].FirstMask = BMT_MASK(1);
      apexregs[i].LastMask = BMT_MASK(1);
      apexregs[i].name = (char *)apexdriver_physical_map[i].name;

      //BMT_AllocateFromAnonymousMemoryReservation(&(apexregs[i]), &addr);	
      printf("OAL ALLOCATION FOR INTEGRITY is turned off\n");

      apexdriver_virtual_map[i] = (void *)apexdriver_physical_map[i].physical;
    }
  }
   
  apexdriver_mapping_done = 1;
  return 0;
}
   
/*======================================================================================*/
/* Function returns virtual mapping for APEX registers					*/
/*======================================================================================*/
void* ReturnVirtualHwPtr (int lApexID, HW_REG_ID lHwRegId)
{
  return apexdriver_virtual_map[lHwRegId + lApexID*HW_REG_NUM];
}

/*======================================================================================*/
/* Function returns physical address for APEX registers					*/
/*======================================================================================*/
void* ReturnPhysicalHwPtr (int lApexID, HW_REG_ID lHwRegId)
{
  return (void*)(apexdriver_physical_map[lHwRegId + lApexID*HW_REG_NUM].physical);
}

int DeinitVirtualHwPtrArray()
{
   int rVal = 0;

   rVal = UnmapVirtualHwPtrArray();

   return rVal;
}