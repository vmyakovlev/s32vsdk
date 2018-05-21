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
//    SEQ
//    APEX_642CL_SEQ
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_SEQ_BASE_ADDRESS + APEX_642CL_SEQ_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Sequencer
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLSEQMAP_H
#define APEX642CLSEQMAP_H

#include "apex_642cl_seq_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    SEQ
//    APEX_642CL_SEQ
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_SEQ_BASE_ADDRESS + APEX_642CL_SEQ_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Sequencer
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_SEQ_TAG
{
   APEX_642CL_SEQ_ISR_DCR                                       Isr;                       // 32'h00000000
   APEX_642CL_SEQ_IMR_DCR                                       Imr;                       // 32'h00000004
   APEX_642CL_SEQ_ICR_DCR                                       Icr;                       // 32'h00000008
   APEX_642CL_SEQ_ENABLE_DCR                                    Enable;                    // 32'h0000000c
   APEX_642CL_SEQ_SCB_ENABLE_DCR                                ScbEnable;                 // 32'h00000010
   APEX_642CL_SEQ_SCB_STATUS_DCR                                ScbStatus;                 // 32'h00000014
   APEX_642CL_SEQ_DEBUG_STEP_DCR                                DebugStep;                 // 32'h00000018
   APEX_642CL_SEQ_RESOURCE_NZ_DCR                               ResourceNz;                // 32'h0000001c
   APEX_642CL_SEQ_SCB_IDLE_DCR                                  ScbIdle;                   // 32'h00000020
   REG32                                                        Reserved_00000024;
   APEX_642CL_SEQ_SEQ_DISABLE_DCR                               SeqDisable;                // 32'h00000028
   APEX_642CL_SEQ_SCB_DISABLE_DCR                               ScbDisable;                // 32'h0000002c
   REG32                                                        Reserved_00000030[5];
   APEX_642CL_SEQ_IMR_DCR                                       Imr2;                      // 32'h00000044
   APEX_642CL_SEQ_IMR_DCR                                       Imr3;                      // 32'h00000048
   REG32                                                        Reserved_0000004c[13];
   APEX_642CL_SEQ_ARM_CORRECTION_RES_DCR                        ArmCorrectionRes[8];       // 32'h00000080
   REG32                                                        Reserved_000000a0[24];
   APEX_642CL_SEQ_PROC_DEC_DCR                                  ProcDec[16];               // 32'h00000100
   REG32                                                        Reserved_00000140[16];
   APEX_642CL_SEQ_PROC_DEC_DCR                                  DmaDec[8];                 // 32'h00000180
   REG32                                                        Reserved_000001a0[8];
   APEX_642CL_SEQ_PROC_DEC_DCR                                  StreamInDec[4];            // 32'h000001c0
   APEX_642CL_SEQ_PROC_DEC_DCR                                  StreamOutDec[2];           // 32'h000001d0
   REG32                                                        Reserved_000001d8[2];
   APEX_642CL_SEQ_PROC_DEC_DCR                                  MotionCompDec[2];          // 32'h000001e0
   REG32                                                        Reserved_000001e8[2];
   APEX_642CL_SEQ_PROC_DEC_DCR                                  HorizResizerDec[2];        // 32'h000001f0
   REG32                                                        Reserved_000001f8[2];
   APEX_642CL_SEQ_PROC_INC_DCR                                  ProcInc[6];                // 32'h00000200
   REG32                                                        Reserved_00000218[26];
   APEX_642CL_SEQ_PROC_INC_DCR                                  DmaInc[8];                 // 32'h00000280
   REG32                                                        Reserved_000002a0[8];
   APEX_642CL_SEQ_PROC_INC_DCR                                  StreamInInc[4];            // 32'h000002c0
   APEX_642CL_SEQ_PROC_INC_DCR                                  StreamOutInc[2];           // 32'h000002d0
   REG32                                                        Reserved_000002d8[2];
   APEX_642CL_SEQ_PROC_INC_DCR                                  MotionCompInc[2];          // 32'h000002e0
   REG32                                                        Reserved_000002e8[2];
   APEX_642CL_SEQ_PROC_INC_DCR                                  HorizResizerInc[2];        // 32'h000002f0
   REG32                                                        Reserved_000002f8[2];
   APEX_642CL_SEQ_RESOURCE_FIX_DCR                              ResourceFix[20];           // 32'h00000300
   APEX_642CL_SEQ_RESOURCE_FRAC_DCR                             ResourceFrac[8];           // 32'h00000350
   APEX_642CL_SEQ_RESOURCE_ZERO_DCR                             ResourceZero[4];           // 32'h00000370
};

typedef struct _APEX_642CL_SEQ_TAG                              APEX_642CL_SEQ_DCR;



#endif /* APEX642CLSEQMAP_H */
