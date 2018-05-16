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

#include <seq_hal.h>
#include <hw_reg_user.h>

typedef unsigned int REG32;
#include "apex_642cl_seq_map.h"

void seq_hal_set_rsc_counter(int apexID, int lRsc, int lVal)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   volatile REG32* lCntArray = (volatile REG32*)&(lpSeqRegisters->ResourceFix[0]);
   lCntArray[lRsc] = lVal;
}

int seq_hal_ret_rsc_counter(int apexID, int lRsc)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   volatile REG32* lCntArray = (volatile REG32*)&(lpSeqRegisters->ResourceFix[0]);
#ifdef HAL_DUMMY_NO_HW
   return lCntArray[lRsc] + 1; //resource always 'available'
#else
   return lCntArray[lRsc];
#endif
}

void seq_hal_proc_inc_rscmask(int apexID, int lProcId, int lRscMask)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->ProcInc[lProcId].as32BitValue = lRscMask;
}

void seq_hal_proc_dec_rscmask(int apexID, int lProcId, int lRscMask)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->ProcDec[lProcId].as32BitValue = lRscMask;
}

void seq_hal_enable_seq(int apexID, int lProcId)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->Enable.as32BitValue = 1<<lProcId;
}

void seq_hal_disable_seq(int apexID, int lProcId)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->SeqDisable.as32BitValue = 1<<lProcId;
}

int seq_hal_query_interrupt(int apexID, int lProcId)
{
   int lRetVal = 0;
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   if (lpSeqRegisters->Isr.as32BitValue & (1<<lProcId))
      lRetVal = 1;
#ifdef HAL_DUMMY_NO_HW
   lRetVal = 1; //interrupt always available
#endif
   return lRetVal;
}

void seq_hal_clear_interrupt(int apexID, int lProcId)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->Icr.as32BitValue = (1<<lProcId);
}

void seq_hal_scb_enable(int apexID, int lScbWord)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->ScbEnable.as32BitValue = lScbWord;
}

void seq_hal_scb_disable(int apexID, int lScbWord)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->ScbDisable.as32BitValue = lScbWord;
}

void seq_hal_set_dma_inc(int apexID, int lCh, int lVal)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->DmaInc[lCh].as32BitValue = lVal;
}

void seq_hal_set_dma_dec(int apexID, int lCh, int lVal)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->DmaDec[lCh].as32BitValue = lVal;
}

void seq_hal_set_motioncomp_inc(int apexID, int lCh, int lVal)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->MotionCompInc[lCh].as32BitValue = lVal;
}

void seq_hal_set_motioncomp_dec(int apexID, int lCh, int lVal)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->MotionCompDec[lCh].as32BitValue = lVal;
}

void seq_hal_mask_interrupts(int apexID, int lMask)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->Imr.as32BitValue = lMask;
}

void seq_hal_mask_interrupts_apu(int apexID, int lApuId, int lMask)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   if (0 == lApuId)
   {
      lpSeqRegisters->Imr2.as32BitValue = lMask;
   }
   else //1 == lApuId
   {
      lpSeqRegisters->Imr3.as32BitValue = lMask;
   }
}

void seq_hal_clear_interrupts(int apexID, int lMask)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->Icr.as32BitValue = lMask;
}

int seq_hal_query_interrupts(int apexID)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
#ifdef HAL_DUMMY_NO_HW
   return lpSeqRegisters->Isr.as32BitValue | 0xFFFFFFFF; //interrupts always available
#else
   return lpSeqRegisters->Isr.as32BitValue;
#endif
}

int RetMceScbId(int lIdx)
{
   int lScbId = 0;
   switch(lIdx)
   {
   case 0: lScbId = 24; break;
   case 1: lScbId = 25; break;
   }
   return lScbId;
}

void seq_hal_set_hrsz_inc(int apexID, int lCh, int lVal)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->HorizResizerInc[lCh].as32BitValue = lVal;
}

void seq_hal_set_hrsz_dec(int apexID, int lCh, int lVal)
{
   volatile APEX_642CL_SEQ_DCR *const lpSeqRegisters = (APEX_642CL_SEQ_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_USEQ);
   lpSeqRegisters->HorizResizerDec[lCh].as32BitValue = lVal;
}

