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

// Include files.
#include "global_hal.h"
#include "hw_reg_user.h"
#include <stdio.h>

typedef unsigned int REG32;
#include "apex_642cl_global_map.h"

#define TRUE 1
#define FALSE 0

typedef enum
{
   APEX2_INTERRUPT_DISABLE = 1,
   APEX2_INTERRUPT_ENABLE  = 1
} APEX2_INTERRUPT_MASK;

static REG32 gAPEX_REG_GLOBAL_MISC[2];

void global_hal_AllReset(int apexID)
{
   // any reason not to reset the whole block at once?

#if 0
   /*
   REG32  CmemIf               :  1; //  00     APEX_642CL_GLOBAL_RST_SET_BIT_CMEM_IF
   REG32  MultiDma             :  1; //  01     APEX_642CL_GLOBAL_RST_SET_BIT_MULTI_DMA
   REG32  MotionComp           :  1; //  02     APEX_642CL_GLOBAL_RST_SET_BIT_MOTION_COMP
   REG32  Stream               :  1; //  03     APEX_642CL_GLOBAL_RST_SET_BIT_STREAM

   REG32  Sequencer            :  1; //  04     APEX_642CL_GLOBAL_RST_SET_BIT_SEQUENCER
   REG32  Dmem0                :  1; //  5      APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_0
   REG32  Dmem1                :  1; //  6      APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_1
   REG32  Apu0                 :  1; //  7      APEX_642CL_GLOBAL_RST_SET_BIT_APU_0

   REG32  Apu1                 :  1; //  8      APEX_642CL_GLOBAL_RST_SET_BIT_APU_1
   REG32  HrzScaler            :  1; //  09     APEX_642CL_GLOBAL_RST_SET_BIT_HRZ_SCALER
   REG32  Vu0                  :  1; //  10     APEX_642CL_GLOBAL_RST_SET_BIT_VU_0
   REG32  Vu1                  :  1; //  11     APEX_642CL_GLOBAL_RST_SET_BIT_VU_1
   */
   {
      volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
      lpApex2GlobalReg->RstSet.as32BitValue    = 0x0FFF;
      lpApex2GlobalReg->RstClear.as32BitValue  = 0x0FFF;
   }
#else
   global_hal_cmemIFReset(apexID);
   global_hal_CmemDmaReset(apexID);
   global_hal_uSequencerReset(apexID);
   global_hal_DmaReset(apexID);
   global_hal_MotCmpDmaReset(apexID);
   global_hal_Apu0Reset(apexID);
   global_hal_Apu1Reset(apexID);
   global_hal_Vu0Reset(apexID);
   global_hal_Vu1Reset(apexID);
   global_hal_Dmem0Reset(apexID);
   global_hal_Dmem1Reset(apexID);
   global_hal_HrszReset(apexID);
#endif

   //
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   gAPEX_REG_GLOBAL_MISC[apexID] = lpApex2GlobalReg->Misc.as32BitValue;
}

void global_hal_CmemDmaReset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.Stream = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.Stream = TRUE;
}

void global_hal_uSequencerReset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.Sequencer = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.Sequencer = TRUE;
}

void global_hal_DmaReset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.MultiDma = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.MultiDma = TRUE;
}

void global_hal_MotCmpDmaReset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.MotionComp = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.MotionComp = TRUE;
}

void global_hal_cmemIFReset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.CmemIf = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.CmemIf = TRUE;
}

void global_hal_Apu0Reset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.as32BitValue = (1 << 7); //RstSet.asBitField.Apu0
   lpApex2GlobalReg->RstClear.as32BitValue = (1 << 7); //RstClear.asBitField.Apu0
}

void global_hal_Apu1Reset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.as32BitValue = (1 << 8); //RstSet.asBitField.Apu1
   lpApex2GlobalReg->RstClear.as32BitValue = (1 << 8); //RstClear.asBitField.Apu1
}

void global_hal_Vu0Reset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.Vu0 = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.Vu0 = TRUE;
}

void global_hal_Vu1Reset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.Vu1 = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.Vu1 = TRUE;
}

void global_hal_HrszReset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.HrzScaler = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.HrzScaler = TRUE;
}

void global_hal_Dmem0Reset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.Dmem0 = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.Dmem0 = TRUE;
}

void global_hal_Dmem1Reset(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->RstSet.asBitField.Dmem1 = TRUE;
   lpApex2GlobalReg->RstClear.asBitField.Dmem1 = TRUE;
}

void global_hal_Apu0Enable(int apexID)
{
   //NOTE: read-modify-write required (this is not thread safe without protection)
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   // access protected by a semaphore
   // read / modify write...
   lpApex2GlobalReg->Misc.as32BitValue |= (1 << 0); //Misc.asBitField.ApuEnable0

   //
   gAPEX_REG_GLOBAL_MISC[apexID] |= (1 << 0);
#ifndef __chess__
   REG32 reg = lpApex2GlobalReg->Misc.as32BitValue;
   if (reg != gAPEX_REG_GLOBAL_MISC[apexID])
   {
      printf(" *** APU ENABLE [%i][%i] %08X <> %08X %08X\n",
            apexID, 0,
            gAPEX_REG_GLOBAL_MISC[apexID], reg, lpApex2GlobalReg->Misc.as32BitValue);
   }
#endif
}

void global_hal_Apu1Enable(int apexID)
{
   //NOTE: read-modify-write required (this is not thread safe without protection)
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->Misc.as32BitValue |= (1 << 1); //Misc.asBitField.ApuEnable1

   //
   gAPEX_REG_GLOBAL_MISC[apexID] |= (1 << 1);
#ifndef __chess__
   REG32 reg = lpApex2GlobalReg->Misc.as32BitValue;
   if (reg != gAPEX_REG_GLOBAL_MISC[apexID])
   {
      printf(" *** APU ENABLE [%i][%i] %08X <> %08X %08X\n",
            apexID, 1,
            gAPEX_REG_GLOBAL_MISC[apexID], reg, lpApex2GlobalReg->Misc.as32BitValue);
   }
#endif
}

void global_hal_Apu0Disable(int apexID)
{
   //NOTE: read-modify-write required (this is not thread safe without protection)
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->Misc.as32BitValue &= (~(1 << 0)); //Misc.asBitField.ApuEnable0

   //
   gAPEX_REG_GLOBAL_MISC[apexID] &= (~(1 << 0));
#ifndef __chess__
   REG32 reg = lpApex2GlobalReg->Misc.as32BitValue;
   if (reg != gAPEX_REG_GLOBAL_MISC[apexID])
   {
      printf(" *** APU DISABLE[%i][%i] %08X <> %08X %08X\n",
            apexID, 0,
            gAPEX_REG_GLOBAL_MISC[apexID], reg, lpApex2GlobalReg->Misc.as32BitValue);
   }
#endif
}

void global_hal_Apu1Disable(int apexID)
{
   //NOTE: read-modify-write required (this is not thread safe without protection)
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   lpApex2GlobalReg->Misc.as32BitValue &= (~(1 << 1)); //Misc.asBitField.ApuEnable1

   //
   gAPEX_REG_GLOBAL_MISC[apexID] &= (~(1 << 1));
#ifndef __chess__
   REG32 reg = lpApex2GlobalReg->Misc.as32BitValue;
   if (reg != gAPEX_REG_GLOBAL_MISC[apexID])
   {
      printf(" *** APU DISABLE[%i][%i] %08X <> %08X %08X\n",
            apexID, 0,
            gAPEX_REG_GLOBAL_MISC[apexID], reg, lpApex2GlobalReg->Misc.as32BitValue);
   }
#endif
}

void global_hal_Apex2GlobalInterruptDisable(int apexID, APEX_INTERRUPT_BITS intBit)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);

   switch (intBit)
   {
      case APEX2_INTERRUPT_MULTI_DMA:
         lpApex2GlobalReg->IntMaskSet.asBitField.MultiDma = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_MOT_COMP_DMA:
         lpApex2GlobalReg->IntMaskSet.asBitField.MotionComp = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_SEQUENCER:
         lpApex2GlobalReg->IntMaskSet.asBitField.Sequencer = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_0:
         lpApex2GlobalReg->IntMaskSet.asBitField.StreamIn0 = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_1:
         lpApex2GlobalReg->IntMaskSet.asBitField.StreamIn1 = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_2:
         lpApex2GlobalReg->IntMaskSet.asBitField.StreamIn2 = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_3:
         lpApex2GlobalReg->IntMaskSet.asBitField.StreamIn3 = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_0:
         lpApex2GlobalReg->IntMaskSet.asBitField.StreamOut0 = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_1:
         lpApex2GlobalReg->IntMaskSet.asBitField.StreamOut1 = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_HRSZ_0:
         lpApex2GlobalReg->IntMaskSet.asBitField.HrzScaler0 = APEX2_INTERRUPT_DISABLE;
         break;

      case APEX2_INTERRUPT_HRSZ_1:
         lpApex2GlobalReg->IntMaskSet.asBitField.HrzScaler1 = APEX2_INTERRUPT_DISABLE;
         break;
		 
      default:
         break;
   }
}

void global_hal_Apex2GlobalInterruptEnable(int apexID, APEX_INTERRUPT_BITS intBit)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);

   switch (intBit)
   {
      case APEX2_INTERRUPT_MULTI_DMA:
         lpApex2GlobalReg->IntMaskClear.asBitField.MultiDma = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_MOT_COMP_DMA:
         lpApex2GlobalReg->IntMaskClear.asBitField.MotionComp = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_SEQUENCER:
         lpApex2GlobalReg->IntMaskClear.asBitField.Sequencer = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_0:
         lpApex2GlobalReg->IntMaskClear.asBitField.StreamIn0 = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_1:
         lpApex2GlobalReg->IntMaskClear.asBitField.StreamIn1 = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_2:
         lpApex2GlobalReg->IntMaskClear.asBitField.StreamIn2 = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_STREAM_IN_3:
         lpApex2GlobalReg->IntMaskClear.asBitField.StreamIn3 = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_0:
         lpApex2GlobalReg->IntMaskClear.asBitField.StreamOut0 = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_1:
         lpApex2GlobalReg->IntMaskClear.asBitField.StreamOut1 = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_HRSZ_0:
         lpApex2GlobalReg->IntMaskClear.asBitField.HrzScaler0 = APEX2_INTERRUPT_ENABLE;
         break;

      case APEX2_INTERRUPT_HRSZ_1:
         lpApex2GlobalReg->IntMaskClear.asBitField.HrzScaler1 = APEX2_INTERRUPT_ENABLE;
         break;

      default:
         break;
   }
}

int global_hal_isApex2GlobalInterruptEnabled(int apexID, APEX_INTERRUPT_BITS intBit)
{
   unsigned int maskbit=1;

   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);

   switch (intBit)
   {
      case APEX2_INTERRUPT_MULTI_DMA:
         maskbit = lpApex2GlobalReg->IntMaskStatus.asBitField.MultiDma;
         break;

      case APEX2_INTERRUPT_MOT_COMP_DMA:
         maskbit =  lpApex2GlobalReg->IntMaskStatus.asBitField.MotionComp;
         break;

      case APEX2_INTERRUPT_SEQUENCER:
         maskbit =  lpApex2GlobalReg->IntMaskStatus.asBitField.Sequencer;
         break;

      case APEX2_INTERRUPT_STREAM_IN_0:
         maskbit =  lpApex2GlobalReg->IntMaskStatus.asBitField.StreamIn0;
         break;

      case APEX2_INTERRUPT_STREAM_IN_1:
         maskbit =  lpApex2GlobalReg->IntMaskStatus.asBitField.StreamIn1;
         break;

      case APEX2_INTERRUPT_STREAM_IN_2:
         maskbit = lpApex2GlobalReg->IntMaskStatus.asBitField.StreamIn2;
         break;

      case APEX2_INTERRUPT_STREAM_IN_3:
         maskbit = lpApex2GlobalReg->IntMaskStatus.asBitField.StreamIn3;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_0:
         maskbit =  lpApex2GlobalReg->IntMaskStatus.asBitField.StreamOut0;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_1:
         maskbit = lpApex2GlobalReg->IntMaskStatus.asBitField.StreamOut1;
         break;

      case APEX2_INTERRUPT_HRSZ_0:
         maskbit = lpApex2GlobalReg->IntMaskStatus.asBitField.HrzScaler0;
         break;

      case APEX2_INTERRUPT_HRSZ_1:
         maskbit = lpApex2GlobalReg->IntMaskStatus.asBitField.HrzScaler1;
         break;

      default:
         maskbit =  0;
         break;
   }

   if( maskbit == 0 )   // IRQ is enabled
      return TRUE;

   return FALSE;
}

int global_hal_Apex2GlobalInterruptStatus(int apexID, APEX_INTERRUPT_BITS intBit)
{
   int intStatus = FALSE;

   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);

   switch (intBit)
   {
      case APEX2_INTERRUPT_MULTI_DMA:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.MultiDma;
         break;

      case APEX2_INTERRUPT_MOT_COMP_DMA:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.MotionComp;
         break;

      case APEX2_INTERRUPT_SEQUENCER:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.Sequencer;
         break;

      case APEX2_INTERRUPT_STREAM_IN_0:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.StreamIn0;
         break;

      case APEX2_INTERRUPT_STREAM_IN_1:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.StreamIn1;
         break;

      case APEX2_INTERRUPT_STREAM_IN_2:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.StreamIn2;
         break;

      case APEX2_INTERRUPT_STREAM_IN_3:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.StreamIn3;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_0:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.StreamOut0;
         break;

      case APEX2_INTERRUPT_STREAM_OUT_1:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.StreamOut1;
         break;

      case APEX2_INTERRUPT_HRSZ_0:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.HrzScaler0;
         break;

      case APEX2_INTERRUPT_HRSZ_1:
         intStatus = lpApex2GlobalReg->IntStatus.asBitField.HrzScaler1;
         break;

      default:
         break;
   }

   return intStatus;
}

int global_hal_RetHwVersion(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   return lpApex2GlobalReg->Version.asBitField.Hw;
}

int global_hal_RetRtlBuildNumber(int apexID)
{
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);
   return lpApex2GlobalReg->BuildNumber.asBitField.Num;
}
