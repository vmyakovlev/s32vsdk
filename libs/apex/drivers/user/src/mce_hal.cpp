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

#include <mce_hal.h>
#include <hw_reg_user.h>

typedef unsigned int REG32;
#include "apex_642cl_motion_comp_map.h"

void mce_hal_program_linkedlist(int lApexID, unsigned int chNo, unsigned int linkedListAddr)
{
   volatile APEX_642CL_MOTION_COMP_DCR *const lpMceRegisters = (APEX_642CL_MOTION_COMP_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_MODMA);
   lpMceRegisters->Ch[chNo].LinkedList.as32BitValue = (linkedListAddr | 0x01);
}

void mce_hal_program_nonseq_linkedlist(int lApexID, unsigned int chNo, unsigned int linkedListAddr)
{
   volatile APEX_642CL_MOTION_COMP_DCR *const lpMceRegisters = (APEX_642CL_MOTION_COMP_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_MODMA);
   lpMceRegisters->Ch[chNo].LinkedList.as32BitValue = (linkedListAddr | 0x03);
}

void mce_hal_enable_ch(int lApexID, unsigned int chNo)
{
   volatile APEX_642CL_MOTION_COMP_DCR *const lpMceRegisters = (APEX_642CL_MOTION_COMP_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_MODMA);
   lpMceRegisters->ChEnable.as32BitValue = (1 << chNo);
}

void mce_hal_wait_polling(int lApexID, unsigned int chNo)
{
   volatile APEX_642CL_MOTION_COMP_DCR *const lpMceRegisters = (APEX_642CL_MOTION_COMP_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_MODMA);
   while(lpMceRegisters->ChEnable.as32BitValue & (1 << chNo)) ;
}

void mce_hal_clear_int(int lApexID, unsigned int chNo)
{
   volatile APEX_642CL_MOTION_COMP_DCR *const lpMceRegisters = (APEX_642CL_MOTION_COMP_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_MODMA);
   lpMceRegisters->Icr.as32BitValue = (1 << chNo);
}

void mce_hal_clear_ch(int lApexID, unsigned int chNo)
{
   volatile APEX_642CL_MOTION_COMP_DCR *const lpMceRegisters = (APEX_642CL_MOTION_COMP_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_MODMA);
   lpMceRegisters->ChClear.as32BitValue = (1 << chNo);
}

void mce_hal_clear_err(int lApexID, unsigned int chNo)
{
   volatile APEX_642CL_MOTION_COMP_DCR *const lpMceRegisters = (APEX_642CL_MOTION_COMP_DCR*)ReturnVirtualHwPtr(lApexID, HW_REG_MODMA);
   lpMceRegisters->Ecr.as32BitValue = (1 << chNo);
}
