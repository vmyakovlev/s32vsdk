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
//    MOTION_COMP
//    APEX_642CL_MOTION_COMP
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_BASE_ADDRESS + APEX_642CL_MOTION_COMP_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Motion Comp DMA
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLMOTIONCOMPMAP_H
#define APEX642CLMOTIONCOMPMAP_H

#include "apex_642cl_motion_comp_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    PERF
//    APEX_642CL_MOTION_COMP_PERF
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_PERF_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Nb:         1
//    Size:       12
// 
// Description:
//    Performance
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_MOTION_COMP_PERF_TAG
{
   APEX_642CL_MOTION_COMP_PERF_CONTROL_DCR                      Control;                   // 32'h00000000
   APEX_642CL_MOTION_COMP_PERF_TOTAL_DCR                        Total;                     // 32'h00000004
   APEX_642CL_MOTION_COMP_PERF_BUSRD_DCR                        Busrd;                     // 32'h00000008
};

typedef struct _APEX_642CL_MOTION_COMP_PERF_TAG                 APEX_642CL_MOTION_COMP_PERF_DCR;



/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    CH
//    APEX_642CL_MOTION_COMP_CH
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h00000100
//    Default:      32'h00000000
//    Nb:         2
//    Size:       256
// 
// Description:
//    DMA Channel 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_MOTION_COMP_CH_TAG
{
   APEX_642CL_MOTION_COMP_CH_LINKED_LIST_DCR                    LinkedList;                // 32'h00000000
   APEX_642CL_MOTION_COMP_CH_CONFIG_DCR                         Config;                    // 32'h00000004
   APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_DCR            EngineLinePointer;         // 32'h00000008
   APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_DCR      EngineLineMacroPointer[4]; // 32'h0000000c
   APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_DCR                    EngineList;                // 32'h0000001c
   APEX_642CL_MOTION_COMP_CH_DEBUG_DCR                          Debug;                     // 32'h00000020
   REG32                                                        Reserved_00000024[55];
};

typedef struct _APEX_642CL_MOTION_COMP_CH_TAG                   APEX_642CL_MOTION_COMP_CH_DCR;



/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    MOTION_COMP
//    APEX_642CL_MOTION_COMP
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_BASE_ADDRESS + APEX_642CL_MOTION_COMP_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Motion Comp DMA
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_MOTION_COMP_TAG
{
   APEX_642CL_MOTION_COMP_ISR_DCR                               Isr;                       // 32'h00000000
   APEX_642CL_MOTION_COMP_ESR_DCR                               Esr;                       // 32'h00000004
   APEX_642CL_MOTION_COMP_IMR_DCR                               Imr;                       // 32'h00000008
   APEX_642CL_MOTION_COMP_ICR_DCR                               Icr;                       // 32'h0000000c
   APEX_642CL_MOTION_COMP_ECR_DCR                               Ecr;                       // 32'h00000010
   APEX_642CL_MOTION_COMP_DMA_CONTROL_DCR                       DmaControl;                // 32'h00000014
   APEX_642CL_MOTION_COMP_CH_ENABLE_DCR                         ChEnable;                  // 32'h00000018
   APEX_642CL_MOTION_COMP_CH_CLEAR_DCR                          ChClear;                   // 32'h0000001c
   APEX_642CL_MOTION_COMP_PERF_DCR                              Perf;                      // 32'h00000020
   REG32                                                        Reserved_0000002c[53];
   APEX_642CL_MOTION_COMP_CH_DCR                                Ch[2];                     // 32'h00000100
};

typedef struct _APEX_642CL_MOTION_COMP_TAG                      APEX_642CL_MOTION_COMP_DCR;



#endif /* APEX642CLMOTIONCOMPMAP_H */
