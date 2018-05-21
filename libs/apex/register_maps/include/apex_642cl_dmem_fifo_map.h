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
//    DMEM_FIFO
//    APEX_642CL_DMEM_FIFO
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         2
//    Size:       65536
// 
// Description:
//    DMEM FIFO Control
//    
//    All register definitions use the following connotations:
//    
//    "IN": refers to data structures that are written by the DMEM fifo and read by the APU.
//    "OUT": refers to data structures that are written by the APU and read by the DMEM fifo.
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLDMEMFIFOMAP_H
#define APEX642CLDMEMFIFOMAP_H

#include "apex_642cl_dmem_fifo_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    DMEM_FIFO
//    APEX_642CL_DMEM_FIFO
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         2
//    Size:       65536
// 
// Description:
//    DMEM FIFO Control
//    
//    All register definitions use the following connotations:
//    
//    "IN": refers to data structures that are written by the DMEM fifo and read by the APU.
//    "OUT": refers to data structures that are written by the APU and read by the DMEM fifo.
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_DMEM_FIFO_TAG
{
   APEX_642CL_DMEM_FIFO_CONFIG_IN_DCR                           ConfigIn;                  // 32'h00000000
   APEX_642CL_DMEM_FIFO_CONFIG_IN_DCR                           ConfigOut;                 // 32'h00000004
   APEX_642CL_DMEM_FIFO_IN_BUFFER_DCR                           InBuffer[16];              // 32'h00000008
   APEX_642CL_DMEM_FIFO_IN_BUFFER_DCR                           OutBuffer[16];             // 32'h00000048
   APEX_642CL_DMEM_FIFO_IN_STATUS_DCR                           InStatus;                  // 32'h00000088
   APEX_642CL_DMEM_FIFO_OUT_STATUS_DCR                          OutStatus;                 // 32'h0000008c
   APEX_642CL_DMEM_FIFO_MAINTENANCE_DCR                         Maintenance;               // 32'h00000090
   APEX_642CL_DMEM_FIFO_PERFORMANCE_DCR                         Performance;               // 32'h00000094
};

typedef struct _APEX_642CL_DMEM_FIFO_TAG                        APEX_642CL_DMEM_FIFO_DCR;



#endif /* APEX642CLDMEMFIFOMAP_H */
