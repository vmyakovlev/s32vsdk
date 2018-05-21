/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Block Name:
//    CMEM_IF
//    APEX_642CL_CMEM_IF
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_CMEM_IF_BASE_ADDRESS + APEX_642CL_CMEM_IF_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    CMEM Interface
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLCMEMIFMAP_H
#define APEX642CLCMEMIFMAP_H

#include "apex_642cl_cmem_if_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    CMEM_IF
//    APEX_642CL_CMEM_IF
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_CMEM_IF_BASE_ADDRESS + APEX_642CL_CMEM_IF_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    CMEM Interface
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_CMEM_IF_TAG
{
   APEX_642CL_CMEM_IF_APU0_PM_START_DCR                         Apu0PmStart;               // 32'h00000000
   APEX_642CL_CMEM_IF_APU1_PM_START_DCR                         Apu1PmStart;               // 32'h00000004
   APEX_642CL_CMEM_IF_APU0_DM_START_DCR                         Apu0DmStart;               // 32'h00000008
   APEX_642CL_CMEM_IF_APU1_DM_START_DCR                         Apu1DmStart;               // 32'h0000000c
   APEX_642CL_CMEM_IF_APU_VEC_CONFIG_DCR                        ApuVecConfig;              // 32'h00000010
   APEX_642CL_CMEM_IF_APU_0_CLK_DCR                             Apu0Clk;                   // 32'h00000014
   APEX_642CL_CMEM_IF_APU_1_CLK_DCR                             Apu1Clk;                   // 32'h00000018
   APEX_642CL_CMEM_IF_APU_0_CLK_GATE_DCR                        Apu0ClkGate;               // 32'h0000001c
   APEX_642CL_CMEM_IF_APU_1_CLK_GATE_DCR                        Apu1ClkGate;               // 32'h00000020
   APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_DCR                        Vec0ClkGate;               // 32'h00000024
   APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_DCR                        Vec1ClkGate;               // 32'h00000028
   REG32                                                        Reserved_0000002c[2];
   APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                        AxiRdError0;               // 32'h00000034
   APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                        AxiWrError0;               // 32'h00000038
   APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                        AxiRdError1;               // 32'h0000003c
   APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                        AxiWrError1;               // 32'h00000040
   APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                        AxiRdError2;               // 32'h00000044
   APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                        AxiWrError2;               // 32'h00000048
   REG32                                                        Reserved_0000004c[3];
   APEX_642CL_CMEM_IF_AXI_ERR_RESP_DCR                          AxiErrResp;                // 32'h00000058
};

typedef struct _APEX_642CL_CMEM_IF_TAG                          APEX_642CL_CMEM_IF_DCR;



#endif /* APEX642CLCMEMIFMAP_H */
