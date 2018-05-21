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
//    GLOBAL
//    APEX_642CL_GLOBAL
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_GLOBAL_BASE_ADDRESS + APEX_642CL_GLOBAL_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Global
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLGLOBALMAP_H
#define APEX642CLGLOBALMAP_H

#include "apex_642cl_global_map_type.h"


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    GLOBAL
//    APEX_642CL_GLOBAL
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_GLOBAL_BASE_ADDRESS + APEX_642CL_GLOBAL_OFFSET
//    Base Addr:    APEX_642CL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Global
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
struct _APEX_642CL_GLOBAL_TAG
{
   APEX_642CL_GLOBAL_RST_SET_DCR                                RstSet;                    // 32'h00000000
   APEX_642CL_GLOBAL_RST_SET_DCR                                RstClear;                  // 32'h00000004
   APEX_642CL_GLOBAL_RST_STATUS_DCR                             RstStatus;                 // 32'h00000008
   APEX_642CL_GLOBAL_INT_STATUS_DCR                             IntStatus;                 // 32'h0000000c
   APEX_642CL_GLOBAL_INT_MASK_SET_DCR                           IntMaskSet;                // 32'h00000010
   APEX_642CL_GLOBAL_INT_MASK_SET_DCR                           IntMaskClear;              // 32'h00000014
   APEX_642CL_GLOBAL_INT_MASK_STATUS_DCR                        IntMaskStatus;             // 32'h00000018
   APEX_642CL_GLOBAL_MISC_DCR                                   Misc;                      // 32'h0000001c
   APEX_642CL_GLOBAL_PDOWN_DCR                                  Pdown;                     // 32'h00000020
   APEX_642CL_GLOBAL_VERSION_DCR                                Version;                   // 32'h00000024
   APEX_642CL_GLOBAL_BUILD_NUMBER_DCR                           BuildNumber;               // 32'h00000028
   APEX_642CL_GLOBAL_BUILD_DATE_DCR                             BuildDate;                 // 32'h0000002c
   APEX_642CL_GLOBAL_SWCFG_DCR                                  Swcfg[8];                  // 32'h00000030
};

typedef struct _APEX_642CL_GLOBAL_TAG                           APEX_642CL_GLOBAL_DCR;



#endif /* APEX642CLGLOBALMAP_H */
