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
 
#include <cmem_if_hal.h>
#include <hw_reg_user.h>

typedef unsigned int REG32;
#include "apex_642cl_cmem_if_map.h"

const int32_t gSmemStallCount = 0xF;
                                                                           //SmemTdm               | SmemCtrl | VecCtrl  | Vec6332Master | Vec310Master
#define VECCFGREG__CFG_321__APU_0_CU_0_31_SMEM_0_1                          (gSmemStallCount << 12 | 0x0 << 8 | 0x2 << 4 | 0x0 << 1      | 0x0 << 0)
#define VECCFGREG__CFG_642__APU_0_CU_0_63_SMEM_0_3                          (gSmemStallCount << 12 | 0x0 << 8 | 0x0 << 4 | 0x0 << 1      | 0x0 << 0)
#define VECCFGREG__CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3 (gSmemStallCount << 12 | 0xc << 8 | 0x2 << 4 | 0x1 << 1      | 0x0 << 0)

void cmem_if_hal_set_apu_cfg(int32_t lApexID, CMEM_IF_APU_CFG lApuCfg)
{
   volatile APEX_642CL_CMEM_IF_DCR *const lpCmemifRegisters = (APEX_642CL_CMEM_IF_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_CMEM_IF);

   switch (lApuCfg)
   {
   case CFG_321__APU_0_CU_0_31_SMEM_0_1:
      lpCmemifRegisters->ApuVecConfig.as32BitValue = VECCFGREG__CFG_321__APU_0_CU_0_31_SMEM_0_1;
      break;
   case CFG_642__APU_0_CU_0_63_SMEM_0_3:
      lpCmemifRegisters->ApuVecConfig.as32BitValue = VECCFGREG__CFG_642__APU_0_CU_0_63_SMEM_0_3;
      break;
   case CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3:
      lpCmemifRegisters->ApuVecConfig.as32BitValue = VECCFGREG__CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3;
      break;
   default:
	  break;
   }
}

CMEM_IF_APU_CFG cmem_if_hal_query_apu_cfg(int32_t lApexID)
{
   CMEM_IF_APU_CFG lApuCfg = CFG_UNKNOWN_CONFIGURATION;

   volatile APEX_642CL_CMEM_IF_DCR *const lpCmemifRegisters = (APEX_642CL_CMEM_IF_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_CMEM_IF);

   switch(lpCmemifRegisters->ApuVecConfig.as32BitValue)
   {
   case VECCFGREG__CFG_321__APU_0_CU_0_31_SMEM_0_1:
      lApuCfg = CFG_321__APU_0_CU_0_31_SMEM_0_1;
      break;
   case VECCFGREG__CFG_642__APU_0_CU_0_63_SMEM_0_3:
      lApuCfg = CFG_642__APU_0_CU_0_63_SMEM_0_3;
      break;
   case VECCFGREG__CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3:
      lApuCfg = CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3;
      break;
   }

   return lApuCfg;
}

void cmem_if_hal_set_apu_pmem_start(int32_t lApexID, int32_t apuid, uint32_t address)
{
   volatile APEX_642CL_CMEM_IF_DCR *const lpCmemifRegisters = (APEX_642CL_CMEM_IF_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_CMEM_IF);
   switch (apuid)
   {
   case 0:
      lpCmemifRegisters->Apu0PmStart.as32BitValue = address; //Apu0PmStart.asBitField.Addr
      break;
   case 1:
      lpCmemifRegisters->Apu1PmStart.as32BitValue = address; //Apu1PmStart.asBitField.Addr
      break;
   }
}

void cmem_if_hal_set_apu_dmem_start(int32_t lApexID, int32_t apuid, uint32_t address)
{
  
   volatile APEX_642CL_CMEM_IF_DCR *const lpCmemifRegisters = (APEX_642CL_CMEM_IF_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_CMEM_IF);
   switch (apuid)
   {
   case 0:
      lpCmemifRegisters->Apu0DmStart.as32BitValue = address; //Apu0DmStart.asBitField.Addr
      break;
   case 1:
      lpCmemifRegisters->Apu1DmStart.as32BitValue = address; //Apu1DmStart.asBitField.Addr
      break;
   }
}

uint32_t cmem_if_hal_query_apu_pmem_start(int32_t lApexID, int32_t apuid)
{
   uint32_t lAddr = 0;

   volatile APEX_642CL_CMEM_IF_DCR *const lpCmemifRegisters = (APEX_642CL_CMEM_IF_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_CMEM_IF);
   switch (apuid)
   {
   case 0:
      lAddr = lpCmemifRegisters->Apu0PmStart.as32BitValue; //Apu0PmStart.asBitField.Addr
      break;
   case 1:
      lAddr = lpCmemifRegisters->Apu1PmStart.as32BitValue; //Apu1PmStart.asBitField.Addr
      break;
   }

   return lAddr;
}

uint32_t cmem_if_hal_query_apu_dmem_start(int32_t lApexID, int32_t apuid)
{
   uint32_t lAddr = 0;

   volatile APEX_642CL_CMEM_IF_DCR *const lpCmemifRegisters = (APEX_642CL_CMEM_IF_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_CMEM_IF);
   switch (apuid)
   {
   case 0:
      lAddr = lpCmemifRegisters->Apu0DmStart.as32BitValue; //Apu0DmStart.asBitField.Addr
      break;
   case 1:
      lAddr = lpCmemifRegisters->Apu1DmStart.as32BitValue; //Apu1DmStart.asBitField.Addr
      break;
   }

   return lAddr;
}

uint32_t cmem_if_hal_query_cu_array_width(int32_t lApexID, int32_t apuid)
{
   uint32_t lArrayWidth = 0;

   volatile APEX_642CL_CMEM_IF_DCR *const lpCmemifRegisters = (APEX_642CL_CMEM_IF_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_CMEM_IF);

   switch (lpCmemifRegisters->ApuVecConfig.as32BitValue)
   {
   case VECCFGREG__CFG_321__APU_0_CU_0_31_SMEM_0_1: //there is only one APU in this configuration
      if (0 == apuid)
         lArrayWidth = 32;
      break;

   case VECCFGREG__CFG_642__APU_0_CU_0_63_SMEM_0_3:
      if (0 == apuid)
         lArrayWidth = 64;
      else if (1 == apuid)
         lArrayWidth = 0;
      break;

   case VECCFGREG__CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3:
      if (0 == apuid)
         lArrayWidth = 32;
      else if (1 == apuid)
         lArrayWidth = 32;
      break;

   default:
      lArrayWidth = 0;
   }

   return lArrayWidth;
}

int32_t RetApuMemSizes (CMEM_IF_APU_CFG apuconfig,
                        int32_t* pSmemSize,
                        int32_t* pCmemSize)
{
   int32_t lRetVal = 0;

   *pCmemSize = (1024*4); //always 4K right now

   switch(apuconfig)
   {
   case CFG_321__APU_0_CU_0_31_SMEM_0_1:
      *pSmemSize = (1024*64);
      break;
   case CFG_642__APU_0_CU_0_31_SMEM_0_1__APU_1_CU_32_63_SMEM_2_3:
      *pSmemSize = (1024*64);
      break;
   case CFG_642__APU_0_CU_0_63_SMEM_0_3:
      *pSmemSize = (1024*128);
      break;
   default:
      lRetVal = 1; //ERROR - unexpected APU configuration
      break;
   }

   return lRetVal;
}

