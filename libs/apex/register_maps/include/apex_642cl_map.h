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
// Device Name:
//    APEX_642CL
// 
// Device Parameters:
//    Addr:         APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       0
// 
// Description:
//    APEX-642CL
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLMAP_H
#define APEX642CLMAP_H

#define APEX_642CL_BASE_ADDRESS                                                  (0x00000000)

#define APEX_642CL_MCDMA_BASE_ADDRESS                                            (APEX_642CL_BASE_ADDRESS + 0x00100000)
#define APEX_642CL_MOTION_COMP_BASE_ADDRESS                                      (APEX_642CL_BASE_ADDRESS + 0x00110000)
#define APEX_642CL_STREAM_DMA_BASE_ADDRESS                                       (APEX_642CL_BASE_ADDRESS + 0x00120000)
#define APEX_642CL_SEQ_BASE_ADDRESS                                              (APEX_642CL_BASE_ADDRESS + 0x00130000)
#define APEX_642CL_GLOBAL_BASE_ADDRESS                                           (APEX_642CL_BASE_ADDRESS + 0x00140000)
#define APEX_642CL_HOST_DEBUG_BASE_ADDRESS                                       (APEX_642CL_BASE_ADDRESS + 0x00150000)
#define APEX_642CL_DMEM_FIFO_STATUS_0_BASE_ADDRESS                               (APEX_642CL_BASE_ADDRESS + 0x00160000)
#define APEX_642CL_DMEM_FIFO_STATUS_1_BASE_ADDRESS                               (APEX_642CL_BASE_ADDRESS + 0x00170000)
#define APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS                                      (APEX_642CL_BASE_ADDRESS + 0x00160000)
#define APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS                                      (APEX_642CL_BASE_ADDRESS + 0x00170000)
#define APEX_642CL_CMEM_IF_BASE_ADDRESS                                          (APEX_642CL_BASE_ADDRESS + 0x00180000)
#define APEX_642CL_HRSZ_BASE_ADDRESS                                             (APEX_642CL_BASE_ADDRESS + 0x00190000)
#define APEX_642CL_CMEM_BASE_ADDRESS                                             (APEX_642CL_BASE_ADDRESS + 0x01000000)
#define APEX_642CL_VMEM_BASE_ADDRESS                                             (APEX_642CL_BASE_ADDRESS + 0x01000000)
#define APEX_642CL_SMEM_BASE_ADDRESS                                             (APEX_642CL_BASE_ADDRESS + 0x00000000)


struct          _APEX_642CL_MCDMA_TAG;
extern volatile _APEX_642CL_MCDMA_TAG*                   gpApex642clMcdma;

struct          _APEX_642CL_MOTION_COMP_TAG;
extern volatile _APEX_642CL_MOTION_COMP_TAG*             gpApex642clMotionComp;

struct          _APEX_642CL_STREAM_DMA_TAG;
extern volatile _APEX_642CL_STREAM_DMA_TAG*              gpApex642clStreamDma;

struct          _APEX_642CL_SEQ_TAG;
extern volatile _APEX_642CL_SEQ_TAG*                     gpApex642clSeq;

struct          _APEX_642CL_GLOBAL_TAG;
extern volatile _APEX_642CL_GLOBAL_TAG*                  gpApex642clGlobal;

struct          _APEX_642CL_HOST_DEBUG_TAG;
extern volatile _APEX_642CL_HOST_DEBUG_TAG*              gpApex642clHostDebug;

struct          _APEX_642CL_DMEM_FIFO_STATUS_TAG;
extern volatile _APEX_642CL_DMEM_FIFO_STATUS_TAG*        gpApex642clDmemFifoStatus0;
extern volatile _APEX_642CL_DMEM_FIFO_STATUS_TAG*        gpApex642clDmemFifoStatus1;

struct          _APEX_642CL_DMEM_FIFO_TAG;
extern volatile _APEX_642CL_DMEM_FIFO_TAG*               gpApex642clDmemFifo0;
extern volatile _APEX_642CL_DMEM_FIFO_TAG*               gpApex642clDmemFifo1;

struct          _APEX_642CL_CMEM_IF_TAG;
extern volatile _APEX_642CL_CMEM_IF_TAG*                 gpApex642clCmemIf;

struct          _APEX_642CL_HRSZ_TAG;
extern volatile _APEX_642CL_HRSZ_TAG*                    gpApex642clHrsz;

struct          _APEX_642CL_CMEM_TAG;
extern volatile _APEX_642CL_CMEM_TAG*                    gpApex642clCmem;

struct          _APEX_642CL_VMEM_TAG;
extern volatile _APEX_642CL_VMEM_TAG*                    gpApex642clVmem;

struct          _APEX_642CL_SMEM_TAG;
extern volatile _APEX_642CL_SMEM_TAG*                    gpApex642clSmem;

#endif /* APEX642CLMAP_H */
