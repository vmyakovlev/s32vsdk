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
//    STREAM_DMA
//    APEX_642CL_STREAM_DMA
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_BASE_ADDRESS + APEX_642CL_STREAM_DMA_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Stream DMA
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLSTREAMDMAMAP_H
#define APEX642CLSTREAMDMAMAP_H

#include "apex_642cl_stream_dma_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    CH
//    APEX_642CL_STREAM_DMA_CH
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000100
//    Default:      32'h00000000
//    Nb:         1
//    Size:       256
// 
// Description:
//    Stream Channel
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_STREAM_DMA_CH_TAG
{
   APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_DCR                    TransferCfg;               // 32'h00000000
   APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_DCR                  AddressOffset;             // 32'h00000004
   APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_DCR                 MultiscanCfg1;             // 32'h00000008
   APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_DCR                 MultiscanCfg2;             // 32'h0000000c
   APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_DCR                 MultiscanCfg3;             // 32'h00000010
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_DCR                        WordCfg[4];                // 32'h00000014
   APEX_642CL_STREAM_DMA_CH_CU_CFG_DCR                          CuCfg;                     // 32'h00000024
   APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_DCR                      MuxselCfg[2];              // 32'h00000028
   APEX_642CL_STREAM_DMA_CH_CU_SPAN_DCR                         CuSpan[2];                 // 32'h00000030
   APEX_642CL_STREAM_DMA_CH_STATUS_1_DCR                        Status1;                   // 32'h00000038
   APEX_642CL_STREAM_DMA_CH_STATUS_2_DCR                        Status2;                   // 32'h0000003c
   REG32                                                        Reserved_00000040[48];
};

typedef struct _APEX_642CL_STREAM_DMA_CH_TAG                    APEX_642CL_STREAM_DMA_CH_DCR;



/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    STREAM_DMA
//    APEX_642CL_STREAM_DMA
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_BASE_ADDRESS + APEX_642CL_STREAM_DMA_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Stream DMA
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_STREAM_DMA_TAG
{
   APEX_642CL_STREAM_DMA_ISR_DCR                                Isr;                       // 32'h00000000
   APEX_642CL_STREAM_DMA_IMR_DCR                                Imr;                       // 32'h00000004
   APEX_642CL_STREAM_DMA_ICR_DCR                                Icr;                       // 32'h00000008
   APEX_642CL_STREAM_DMA_ENABLE_DCR                             Enable;                    // 32'h0000000c
   APEX_642CL_STREAM_DMA_DISABLE_DCR                            Disable;                   // 32'h00000010
   APEX_642CL_STREAM_DMA_CONTROL_DCR                            Control;                   // 32'h00000014
   REG32                                                        Reserved_00000018[58];
   APEX_642CL_STREAM_DMA_CH_DCR                                 StreamOut[2];              // 32'h00000100
   REG32                                                        Reserved_00000300[128];
   APEX_642CL_STREAM_DMA_CH_DCR                                 StreamIn[4];               // 32'h00000500
   REG32                                                        Reserved_00000900[448];
   APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_DCR               MmapStreamOutFifo[2];      // 32'h00001000
   REG32                                                        Reserved_00003000[2048];
   APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_DCR                MmapStreamInFifo[4];       // 32'h00005000
};

typedef struct _APEX_642CL_STREAM_DMA_TAG                       APEX_642CL_STREAM_DMA_DCR;



#endif /* APEX642CLSTREAMDMAMAP_H */
