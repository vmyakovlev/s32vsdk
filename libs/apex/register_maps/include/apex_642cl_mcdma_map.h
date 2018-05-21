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
//    MCDMA
//    APEX_642CL_MCDMA
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_MCDMA_BASE_ADDRESS + APEX_642CL_MCDMA_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Multi Channel DMA
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLMCDMAMAP_H
#define APEX642CLMCDMAMAP_H

#include "apex_642cl_mcdma_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    MEMORY_FILL
//    APEX_642CL_MCDMA_MEMORY_FILL
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_MCDMA_MEMORY_FILL_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Nb:         2
//    Size:       16
// 
// Description:
//    Memory Fill Word
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_MCDMA_MEMORY_FILL_TAG
{
   APEX_642CL_MCDMA_MEMORY_FILL_WORD_DCR                        Word[4];                   // 32'h00000000
};

typedef struct _APEX_642CL_MCDMA_MEMORY_FILL_TAG                APEX_642CL_MCDMA_MEMORY_FILL_DCR;



/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    PERF
//    APEX_642CL_MCDMA_PERF
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_MCDMA_PERF_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000050
//    Default:      32'h00000000
//    Nb:         1
//    Size:       16
// 
// Description:
//    Performance
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_MCDMA_PERF_TAG
{
   APEX_642CL_MCDMA_PERF_CONTROL_DCR                            Control;                   // 32'h00000000
   APEX_642CL_MCDMA_PERF_TOTAL_DCR                              Total;                     // 32'h00000004
   APEX_642CL_MCDMA_PERF_BUSRD_DCR                              Busrd;                     // 32'h00000008
   APEX_642CL_MCDMA_PERF_BUSWR_DCR                              Buswr;                     // 32'h0000000c
};

typedef struct _APEX_642CL_MCDMA_PERF_TAG                       APEX_642CL_MCDMA_PERF_DCR;



/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    CH
//    APEX_642CL_MCDMA_CH
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000100
//    Default:      32'h00000000
//    Nb:         8
//    Size:       256
// 
// Description:
//    DMA Channel 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_MCDMA_CH_TAG
{
   APEX_642CL_MCDMA_CH_SRC_DCR                                  Src;                       // 32'h00000000
   APEX_642CL_MCDMA_CH_DST_DCR                                  Dst;                       // 32'h00000004
   APEX_642CL_MCDMA_CH_LINKED_LIST_DCR                          LinkedList;                // 32'h00000008
   APEX_642CL_MCDMA_CH_CONFIG_DCR                               Config;                    // 32'h0000000c
   APEX_642CL_MCDMA_CH_TRANSFER_DCR                             Transfer;                  // 32'h00000010
   APEX_642CL_MCDMA_CH_SPAN_DCR                                 Span;                      // 32'h00000014
   APEX_642CL_MCDMA_CH_WIDTH_DCR                                Width;                     // 32'h00000018
   APEX_642CL_MCDMA_CH_BYTE_DCR                                 Byte;                      // 32'h0000001c
   APEX_642CL_MCDMA_CH_STATUS_SRC_DCR                           StatusSrc;                 // 32'h00000020
   APEX_642CL_MCDMA_CH_STATUS_DST_DCR                           StatusDst;                 // 32'h00000024
   APEX_642CL_MCDMA_CH_STATUS_SHADOW_DCR                        StatusShadow;              // 32'h00000028
   APEX_642CL_MCDMA_CH_STATUS_FSM_DCR                           StatusFsm;                 // 32'h0000002c
   APEX_642CL_MCDMA_CH_STATUS_TRANSFER_DCR                      StatusTransfer;            // 32'h00000030
   APEX_642CL_MCDMA_CH_PRIORITY_DCR                             Priority;                  // 32'h00000034
   REG32                                                        Reserved_00000038[50];
};

typedef struct _APEX_642CL_MCDMA_CH_TAG                         APEX_642CL_MCDMA_CH_DCR;



/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    MCDMA
//    APEX_642CL_MCDMA
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_MCDMA_BASE_ADDRESS + APEX_642CL_MCDMA_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Multi Channel DMA
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_MCDMA_TAG
{
   APEX_642CL_MCDMA_ISR_DCR                                     Isr;                       // 32'h00000000
   APEX_642CL_MCDMA_ESR_DCR                                     Esr;                       // 32'h00000004
   APEX_642CL_MCDMA_EPR_DCR                                     Epr;                       // 32'h00000008
   APEX_642CL_MCDMA_IMR_DCR                                     Imr;                       // 32'h0000000c
   APEX_642CL_MCDMA_RESERVED_DCR                                Reserved;                  // 32'h00000010
   APEX_642CL_MCDMA_ICR_DCR                                     Icr;                       // 32'h00000014
   APEX_642CL_MCDMA_ECR_DCR                                     Ecr;                       // 32'h00000018
   APEX_642CL_MCDMA_DMA_CONTROL_DCR                             DmaControl;                // 32'h0000001c
   APEX_642CL_MCDMA_CH_ENABLE_DCR                               ChEnable;                  // 32'h00000020
   APEX_642CL_MCDMA_CH_CLEAR_DCR                                ChClear;                   // 32'h00000024
   APEX_642CL_MCDMA_CH_FLUSH_DCR                                ChFlush;                   // 32'h00000028
   APEX_642CL_MCDMA_INTERRUPT_DCR                               Interrupt;                 // 32'h0000002c
   APEX_642CL_MCDMA_MEMORY_FILL_DCR                             MemoryFill[2];             // 32'h00000030
   APEX_642CL_MCDMA_PERF_DCR                                    Perf;                      // 32'h00000050
   REG32                                                        Reserved_00000060[40];
   APEX_642CL_MCDMA_CH_DCR                                      Ch[8];                     // 32'h00000100
};

typedef struct _APEX_642CL_MCDMA_TAG                            APEX_642CL_MCDMA_DCR;



#endif /* APEX642CLMCDMAMAP_H */
