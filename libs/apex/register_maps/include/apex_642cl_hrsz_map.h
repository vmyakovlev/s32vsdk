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
//    HRSZ
//    APEX_642CL_HRSZ
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_HRSZ_BASE_ADDRESS + APEX_642CL_HRSZ_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Horizontal Resizer
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLHRSZMAP_H
#define APEX642CLHRSZMAP_H

#include "apex_642cl_hrsz_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    CORE
//    APEX_642CL_HRSZ_CORE
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_OFFSET
//    Base Addr:    APEX_642CL_HRSZ
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         2
//    Size:       128
// 
// Description:
//    Core
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_HRSZ_CORE_TAG
{
   APEX_642CL_HRSZ_CORE_FLUSH_INIT_DCR                          FlushInit;                 // 32'h00000000
   APEX_642CL_HRSZ_CORE_INSTRUCTION_DCR                         Instruction;               // 32'h00000004
   APEX_642CL_HRSZ_CORE_LPF_COEF_DCR                            LpfCoef[3];                // 32'h00000008
   APEX_642CL_HRSZ_CORE_LPF_COEF_3_DCR                          LpfCoef3;                  // 32'h00000014
   APEX_642CL_HRSZ_CORE_PPF_FILTER_DCR                          PpfFilter[10];             // 32'h00000018
   APEX_642CL_HRSZ_CORE_PPF_FILTER_10_DCR                       PpfFilter10;               // 32'h00000040
   APEX_642CL_HRSZ_CORE_OUT_CLAMP_DCR                           OutClamp;                  // 32'h00000044
   APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_DCR                        Woi1SrcRes;                // 32'h00000048
   APEX_642CL_HRSZ_CORE_WOI1_CROP_0_DCR                         Woi1Crop0;                 // 32'h0000004c
   APEX_642CL_HRSZ_CORE_WOI1_CROP_1_DCR                         Woi1Crop1;                 // 32'h00000050
   APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_DCR                        PpfImg1Res;                // 32'h00000054
   APEX_642CL_HRSZ_CORE_PPF_SCALE_DCR                           PpfScale;                  // 32'h00000058
   APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_DCR                        Woi2SrcRes;                // 32'h0000005c
   APEX_642CL_HRSZ_CORE_WOI2_CROP_0_DCR                         Woi2Crop0;                 // 32'h00000060
   APEX_642CL_HRSZ_CORE_WOI2_CROP_1_DCR                         Woi2Crop1;                 // 32'h00000064
   REG32                                                        Reserved_00000068[2];
   APEX_642CL_HRSZ_CORE_IRQ_STATUS_DCR                          IrqStatus;                 // 32'h00000070
   APEX_642CL_HRSZ_CORE_IRQ_MASK_DCR                            IrqMask;                   // 32'h00000074
   APEX_642CL_HRSZ_CORE_IRQ_CLEAR_DCR                           IrqClear;                  // 32'h00000078
   APEX_642CL_HRSZ_CORE_FIFO_STATUS_DCR                         FifoStatus;                // 32'h0000007c
};

typedef struct _APEX_642CL_HRSZ_CORE_TAG                        APEX_642CL_HRSZ_CORE_DCR;

// NB: DMA sideband confiugration uses a subset of the above
struct _APEX_642CL_HRSZ_SIDEBAND_TAG
{
   APEX_642CL_HRSZ_CORE_FLUSH_INIT_DCR                          FlushInit;                 // 32'h00000000
   APEX_642CL_HRSZ_CORE_INSTRUCTION_DCR                         Instruction;               // 32'h00000004
   APEX_642CL_HRSZ_CORE_LPF_COEF_DCR                            LpfCoef[3];                // 32'h00000008
   APEX_642CL_HRSZ_CORE_LPF_COEF_3_DCR                          LpfCoef3;                  // 32'h00000014
   APEX_642CL_HRSZ_CORE_PPF_FILTER_DCR                          PpfFilter[10];             // 32'h00000018
   APEX_642CL_HRSZ_CORE_PPF_FILTER_10_DCR                       PpfFilter10;               // 32'h00000040
   APEX_642CL_HRSZ_CORE_OUT_CLAMP_DCR                           OutClamp;                  // 32'h00000044
   APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_DCR                        Woi1SrcRes;                // 32'h00000048
   APEX_642CL_HRSZ_CORE_WOI1_CROP_0_DCR                         Woi1Crop0;                 // 32'h0000004c
   APEX_642CL_HRSZ_CORE_WOI1_CROP_1_DCR                         Woi1Crop1;                 // 32'h00000050
   APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_DCR                        PpfImg1Res;                // 32'h00000054
   APEX_642CL_HRSZ_CORE_PPF_SCALE_DCR                           PpfScale;                  // 32'h00000058
   APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_DCR                        Woi2SrcRes;                // 32'h0000005c
   APEX_642CL_HRSZ_CORE_WOI2_CROP_0_DCR                         Woi2Crop0;                 // 32'h00000060
   APEX_642CL_HRSZ_CORE_WOI2_CROP_1_DCR                         Woi2Crop1;                 // 32'h00000064
   REG32                                                        Reserved_00000068[2];
};
typedef struct _APEX_642CL_HRSZ_SIDEBAND_TAG                    APEX_642CL_HRSZ_SIDEBAND;
// NB: number of 32-bit elements of this structure (28) is coded as LLE_SIZE_HRSZ_SIDEBAND in LinkedListUtilities.h

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    HRSZ
//    APEX_642CL_HRSZ
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_HRSZ_BASE_ADDRESS + APEX_642CL_HRSZ_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Horizontal Resizer
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_HRSZ_TAG
{
   APEX_642CL_HRSZ_CORE_DCR                                     Core[2];                   // 32'h00000000
   APEX_642CL_HRSZ_PUSH_POP_CORE_DCR                            PushPopCore[2];            // 32'h00000100
};

typedef struct _APEX_642CL_HRSZ_TAG                             APEX_642CL_HRSZ_DCR;



#endif /* APEX642CLHRSZMAP_H */
