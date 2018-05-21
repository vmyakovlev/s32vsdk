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
#ifndef APEX642CLCMEMIFMAPTYPE_H
#define APEX642CLCMEMIFMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU0_PM_START
//    APEX_642CL_CMEM_IF_APU0_PM_START
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU0_PM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU0_PM_START_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000000
//    Default:      32'h00008000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU0 Program Memory (PM) Start Address:
//    This register describes the starting address for APU0's PM.
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU0_PM_START                                         (APEX_642CL_CMEM_IF_APU0_PM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU0_PM_START_OFFSET)
#define APEX_642CL_CMEM_IF_APU0_PM_START_BASE_ADDRESS                            (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU0_PM_START_OFFSET                                  (0x00000000)
#define APEX_642CL_CMEM_IF_APU0_PM_START_DEFAULT                                 (0x00008000)


union _APEX_642CL_CMEM_IF_APU0_PM_START_TAG
{
   struct _asApex642clCmemIfApu0PmStartBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_CMEM_IF_APU0_PM_START_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU0_PM_START_TAG                              APEX_642CL_CMEM_IF_APU0_PM_START_DCR;

extern volatile APEX_642CL_CMEM_IF_APU0_PM_START_DCR                             gApex642clCmemIfApu0PmStart;
extern volatile APEX_642CL_CMEM_IF_APU0_PM_START_DCR*                            gpApex642clCmemIfApu0PmStart;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU0_PM_START_BIT_ADDR                                0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU1_PM_START
//    APEX_642CL_CMEM_IF_APU1_PM_START
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU1_PM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU1_PM_START_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000004
//    Default:      32'h00018000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU1 Program Memory (PM) Start Address:
//    This register describes the starting address for APU1's PM.
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU1_PM_START                                         (APEX_642CL_CMEM_IF_APU1_PM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU1_PM_START_OFFSET)
#define APEX_642CL_CMEM_IF_APU1_PM_START_BASE_ADDRESS                            (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU1_PM_START_OFFSET                                  (0x00000004)
#define APEX_642CL_CMEM_IF_APU1_PM_START_DEFAULT                                 (0x00018000)


union _APEX_642CL_CMEM_IF_APU1_PM_START_TAG
{
   struct _asApex642clCmemIfApu1PmStartBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_CMEM_IF_APU1_PM_START_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU1_PM_START_TAG                              APEX_642CL_CMEM_IF_APU1_PM_START_DCR;

extern volatile APEX_642CL_CMEM_IF_APU1_PM_START_DCR                             gApex642clCmemIfApu1PmStart;
extern volatile APEX_642CL_CMEM_IF_APU1_PM_START_DCR*                            gpApex642clCmemIfApu1PmStart;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU1_PM_START_BIT_ADDR                                0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU0_DM_START
//    APEX_642CL_CMEM_IF_APU0_DM_START
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU0_DM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU0_DM_START_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU0 Data Memory (DM) Start Address:
//    This register describes the starting address for APU0's DM.
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU0_DM_START                                         (APEX_642CL_CMEM_IF_APU0_DM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU0_DM_START_OFFSET)
#define APEX_642CL_CMEM_IF_APU0_DM_START_BASE_ADDRESS                            (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU0_DM_START_OFFSET                                  (0x00000008)
#define APEX_642CL_CMEM_IF_APU0_DM_START_DEFAULT                                 (0x00000000)


union _APEX_642CL_CMEM_IF_APU0_DM_START_TAG
{
   struct _asApex642clCmemIfApu0DmStartBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_CMEM_IF_APU0_DM_START_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU0_DM_START_TAG                              APEX_642CL_CMEM_IF_APU0_DM_START_DCR;

extern volatile APEX_642CL_CMEM_IF_APU0_DM_START_DCR                             gApex642clCmemIfApu0DmStart;
extern volatile APEX_642CL_CMEM_IF_APU0_DM_START_DCR*                            gpApex642clCmemIfApu0DmStart;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU0_DM_START_BIT_ADDR                                0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU1_DM_START
//    APEX_642CL_CMEM_IF_APU1_DM_START
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU1_DM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU1_DM_START_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h0000000c
//    Default:      32'h00010000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU1 Data Memory (DM) Start Address:
//    This register describes the starting address for APU1's DM.
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU1_DM_START                                         (APEX_642CL_CMEM_IF_APU1_DM_START_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU1_DM_START_OFFSET)
#define APEX_642CL_CMEM_IF_APU1_DM_START_BASE_ADDRESS                            (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU1_DM_START_OFFSET                                  (0x0000000c)
#define APEX_642CL_CMEM_IF_APU1_DM_START_DEFAULT                                 (0x00010000)


union _APEX_642CL_CMEM_IF_APU1_DM_START_TAG
{
   struct _asApex642clCmemIfApu1DmStartBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_CMEM_IF_APU1_DM_START_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU1_DM_START_TAG                              APEX_642CL_CMEM_IF_APU1_DM_START_DCR;

extern volatile APEX_642CL_CMEM_IF_APU1_DM_START_DCR                             gApex642clCmemIfApu1DmStart;
extern volatile APEX_642CL_CMEM_IF_APU1_DM_START_DCR*                            gpApex642clCmemIfApu1DmStart;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU1_DM_START_BIT_ADDR                                0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU_VEC_CONFIG
//    APEX_642CL_CMEM_IF_APU_VEC_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_VEC_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000010
//    Default:      32'h00000c22
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU Vector Configuration:
//    
// 
// Device Control Register Bits Description:
//    VEC_31_0_MASTER      [00]      APU Vector Unit Master:
//                                   
//                                   1'b0: APU0
//                                   1'b1: APU1
//                                    
//    VEC_63_32_MASTER     [01]      APU Vector Unit Master:
//                                   
//                                   1'b0: APU0
//                                   1'b1: APU1
//                                    
//    RESERVED_0           [03:02]   Reserved 
//    VEC_CTRL             [05:04]   APU Vector Configuration:
//                                   
//                                   2'b00: APU0-[63:0]CU, APU1-
//                                   2'b01: APU0-[63:32]CU, APU1-[31:0]CU
//                                   2'b10: APU0-[31:0]CU, APU1-[63:32]CU
//                                   2'b11: APU0-, APU1-[63:0]CU
//                                    
//    RESERVED_1           [07:06]   Reserved 
//    SMEM_CTRL            [11:08]   APU SMEM Control Priority:  
//                                   
//                                   Each of the 4 SMEM memories can be set to favor APU0 or APU1.  If the memories are not shared than setting
//                                   these bits will have no effect. Setting to "0" favors APU0, and setting to "1" favors APU1.  
//                                    
//    SMEM_TDM             [15:12]   External access to SMEM maximum stall count.  If the APU is monopolizing an SMEM memory macroblock, then this register describes the maximum number of cycles that the APU can stall the external access.
//                                   
//                                    
//    RESERVED_2           [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG                                        (APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_VEC_CONFIG_OFFSET)
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_OFFSET                                 (0x00000010)
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_DEFAULT                                (0x00000c22)


union _APEX_642CL_CMEM_IF_APU_VEC_CONFIG_TAG
{
   struct _asApex642clCmemIfApuVecConfigBitFieldTag
   {
      REG32  Vec310Master         :  1; //  00     APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_VEC_31_0_MASTER
      REG32  Vec6332Master        :  1; //  01     APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_VEC_63_32_MASTER
      REG32  Reserved0            :  2; //  03:02  APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_RESERVED_0
      REG32  VecCtrl              :  2; //  05:04  APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_VEC_CTRL
      REG32  Reserved1            :  2; //  07:06  APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_RESERVED_1
      REG32  SmemCtrl             :  4; //  11:08  APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_SMEM_CTRL
      REG32  SmemTdm              :  4; //  15:12  APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_SMEM_TDM
      REG32  Reserved2            : 16; //  31:16  APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_RESERVED_2
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU_VEC_CONFIG_TAG                             APEX_642CL_CMEM_IF_APU_VEC_CONFIG_DCR;

extern volatile APEX_642CL_CMEM_IF_APU_VEC_CONFIG_DCR                            gApex642clCmemIfApuVecConfig;
extern volatile APEX_642CL_CMEM_IF_APU_VEC_CONFIG_DCR*                           gpApex642clCmemIfApuVecConfig;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_VEC_31_0_MASTER                    0
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_VEC_63_32_MASTER                   1
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_RESERVED_0                         2
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_VEC_CTRL                           4
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_RESERVED_1                         6
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_SMEM_CTRL                          8
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_SMEM_TDM                           12
#define APEX_642CL_CMEM_IF_APU_VEC_CONFIG_BIT_RESERVED_2                         16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU_0_CLK
//    APEX_642CL_CMEM_IF_APU_0_CLK
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU_0_CLK_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_0_CLK_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU Frequency
// 
// Device Control Register Bits Description:
//    FREQUENCY            [07:00]   Frequency 
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_0_CLK                                             (APEX_642CL_CMEM_IF_APU_0_CLK_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_0_CLK_OFFSET)
#define APEX_642CL_CMEM_IF_APU_0_CLK_BASE_ADDRESS                                (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU_0_CLK_OFFSET                                      (0x00000014)
#define APEX_642CL_CMEM_IF_APU_0_CLK_DEFAULT                                     (0x00000000)


union _APEX_642CL_CMEM_IF_APU_0_CLK_TAG
{
   struct _asApex642clCmemIfApu0ClkBitFieldTag
   {
      REG32  Frequency            :  8; //  07:00  APEX_642CL_CMEM_IF_APU_0_CLK_BIT_FREQUENCY
      REG32  Reserved             : 24; //  31:08  APEX_642CL_CMEM_IF_APU_0_CLK_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU_0_CLK_TAG                                  APEX_642CL_CMEM_IF_APU_0_CLK_DCR;

extern volatile APEX_642CL_CMEM_IF_APU_0_CLK_DCR                                 gApex642clCmemIfApu0Clk;
extern volatile APEX_642CL_CMEM_IF_APU_0_CLK_DCR*                                gpApex642clCmemIfApu0Clk;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_0_CLK_BIT_FREQUENCY                               0
#define APEX_642CL_CMEM_IF_APU_0_CLK_BIT_RESERVED                                8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU_1_CLK
//    APEX_642CL_CMEM_IF_APU_1_CLK
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU_1_CLK_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_1_CLK_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU Frequency
// 
// Device Control Register Bits Description:
//    FREQUENCY            [07:00]   Frequency 
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_1_CLK                                             (APEX_642CL_CMEM_IF_APU_1_CLK_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_1_CLK_OFFSET)
#define APEX_642CL_CMEM_IF_APU_1_CLK_BASE_ADDRESS                                (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU_1_CLK_OFFSET                                      (0x00000018)
#define APEX_642CL_CMEM_IF_APU_1_CLK_DEFAULT                                     (0x00000000)


union _APEX_642CL_CMEM_IF_APU_1_CLK_TAG
{
   struct _asApex642clCmemIfApu1ClkBitFieldTag
   {
      REG32  Frequency            :  8; //  07:00  APEX_642CL_CMEM_IF_APU_1_CLK_BIT_FREQUENCY
      REG32  Reserved             : 24; //  31:08  APEX_642CL_CMEM_IF_APU_1_CLK_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU_1_CLK_TAG                                  APEX_642CL_CMEM_IF_APU_1_CLK_DCR;

extern volatile APEX_642CL_CMEM_IF_APU_1_CLK_DCR                                 gApex642clCmemIfApu1Clk;
extern volatile APEX_642CL_CMEM_IF_APU_1_CLK_DCR*                                gpApex642clCmemIfApu1Clk;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_1_CLK_BIT_FREQUENCY                               0
#define APEX_642CL_CMEM_IF_APU_1_CLK_BIT_RESERVED                                8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU_0_CLK_GATE
//    APEX_642CL_CMEM_IF_APU_0_CLK_GATE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_0_CLK_GATE_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU clock gating
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      Clock gating enable 
//    RESERVED             [31:01]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_0_CLK_GATE                                        (APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_0_CLK_GATE_OFFSET)
#define APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU_0_CLK_GATE_OFFSET                                 (0x0000001c)
#define APEX_642CL_CMEM_IF_APU_0_CLK_GATE_DEFAULT                                (0x00000000)


union _APEX_642CL_CMEM_IF_APU_0_CLK_GATE_TAG
{
   struct _asApex642clCmemIfApu0ClkGateBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BIT_ENABLE
      REG32  Reserved             : 31; //  31:01  APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU_0_CLK_GATE_TAG                             APEX_642CL_CMEM_IF_APU_0_CLK_GATE_DCR;

extern volatile APEX_642CL_CMEM_IF_APU_0_CLK_GATE_DCR                            gApex642clCmemIfApu0ClkGate;
extern volatile APEX_642CL_CMEM_IF_APU_0_CLK_GATE_DCR*                           gpApex642clCmemIfApu0ClkGate;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BIT_ENABLE                             0

// Device Control Register Bits Id and Description
enum APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BIT_ENABLE_ENUM                          
{
   APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BIT_ENABLE_ID_ACTIVE                        =  1,          // Clock gating ACTIVE
   APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BIT_ENABLE_ID_INACTIVE                      =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_CMEM_IF_APU_0_CLK_GATE_BIT_RESERVED                           1

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    APU_1_CLK_GATE
//    APEX_642CL_CMEM_IF_APU_1_CLK_GATE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_1_CLK_GATE_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    APU clock gating
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      Clock gating enable 
//    RESERVED             [31:01]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_1_CLK_GATE                                        (APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_APU_1_CLK_GATE_OFFSET)
#define APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_APU_1_CLK_GATE_OFFSET                                 (0x00000020)
#define APEX_642CL_CMEM_IF_APU_1_CLK_GATE_DEFAULT                                (0x00000000)


union _APEX_642CL_CMEM_IF_APU_1_CLK_GATE_TAG
{
   struct _asApex642clCmemIfApu1ClkGateBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BIT_ENABLE
      REG32  Reserved             : 31; //  31:01  APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_APU_1_CLK_GATE_TAG                             APEX_642CL_CMEM_IF_APU_1_CLK_GATE_DCR;

extern volatile APEX_642CL_CMEM_IF_APU_1_CLK_GATE_DCR                            gApex642clCmemIfApu1ClkGate;
extern volatile APEX_642CL_CMEM_IF_APU_1_CLK_GATE_DCR*                           gpApex642clCmemIfApu1ClkGate;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BIT_ENABLE                             0

// Device Control Register Bits Id and Description
enum APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BIT_ENABLE_ENUM                          
{
   APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BIT_ENABLE_ID_ACTIVE                        =  1,          // Clock gating ACTIVE
   APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BIT_ENABLE_ID_INACTIVE                      =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_CMEM_IF_APU_1_CLK_GATE_BIT_RESERVED                           1

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    VEC_0_CLK_GATE
//    APEX_642CL_CMEM_IF_VEC_0_CLK_GATE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    VEC clock gating
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      Clock gating enable 
//    RESERVED             [31:01]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_VEC_0_CLK_GATE                                        (APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_OFFSET)
#define APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_OFFSET                                 (0x00000024)
#define APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_DEFAULT                                (0x00000000)


union _APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_TAG
{
   struct _asApex642clCmemIfVec0ClkGateBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BIT_ENABLE
      REG32  Reserved             : 31; //  31:01  APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_TAG                             APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_DCR;

extern volatile APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_DCR                            gApex642clCmemIfVec0ClkGate;
extern volatile APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_DCR*                           gpApex642clCmemIfVec0ClkGate;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BIT_ENABLE                             0

// Device Control Register Bits Id and Description
enum APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BIT_ENABLE_ENUM                          
{
   APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BIT_ENABLE_ID_ACTIVE                        =  1,          // Clock gating ACTIVE
   APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BIT_ENABLE_ID_INACTIVE                      =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_CMEM_IF_VEC_0_CLK_GATE_BIT_RESERVED                           1

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    VEC_1_CLK_GATE
//    APEX_642CL_CMEM_IF_VEC_1_CLK_GATE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    VEC clock gating
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      Clock gating enable 
//    RESERVED             [31:01]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_VEC_1_CLK_GATE                                        (APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BASE_ADDRESS + APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_OFFSET)
#define APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_OFFSET                                 (0x00000028)
#define APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_DEFAULT                                (0x00000000)


union _APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_TAG
{
   struct _asApex642clCmemIfVec1ClkGateBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BIT_ENABLE
      REG32  Reserved             : 31; //  31:01  APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_TAG                             APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_DCR;

extern volatile APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_DCR                            gApex642clCmemIfVec1ClkGate;
extern volatile APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_DCR*                           gpApex642clCmemIfVec1ClkGate;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BIT_ENABLE                             0

// Device Control Register Bits Id and Description
enum APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BIT_ENABLE_ENUM                          
{
   APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BIT_ENABLE_ID_ACTIVE                        =  1,          // Clock gating ACTIVE
   APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BIT_ENABLE_ID_INACTIVE                      =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_CMEM_IF_VEC_1_CLK_GATE_BIT_RESERVED                           1

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    AXI_RD_ERROR_0
//    APEX_642CL_CMEM_IF_AXI_RD_ERROR_0
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000034
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    AXI Error Status
// 
// Device Control Register Bits Description:
//    STATUS               [31:00]   AXI Error Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_0                                        (APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_OFFSET)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_OFFSET                                 (0x00000034)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DEFAULT                                (0x00000000)


union _APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_TAG
{
   struct _asApex642clCmemIfAxiRdError0BitFieldTag
   {
      REG32  Status               : 32; //  31:00  APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_BIT_STATUS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_TAG                             APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR;

extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                            gApex642clCmemIfAxiRdError0;
extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR*                           gpApex642clCmemIfAxiRdError0;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_BIT_STATUS                             0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    AXI_WR_ERROR_0
//    APEX_642CL_CMEM_IF_AXI_WR_ERROR_0
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    STATUS               [31:00]   AXI Error Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_0                                        (APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_OFFSET)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_OFFSET                                 (0x00000038)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_DEFAULT                                (0x00000000)


extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                            gApex642clCmemIfAxiWrError0;
extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR*                           gpApex642clCmemIfAxiWrError0;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_0_BIT_STATUS                             0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    AXI_RD_ERROR_1
//    APEX_642CL_CMEM_IF_AXI_RD_ERROR_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    STATUS               [31:00]   AXI Error Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_1                                        (APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_OFFSET)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_OFFSET                                 (0x0000003c)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_DEFAULT                                (0x00000000)


extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                            gApex642clCmemIfAxiRdError1;
extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR*                           gpApex642clCmemIfAxiRdError1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_1_BIT_STATUS                             0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    AXI_WR_ERROR_1
//    APEX_642CL_CMEM_IF_AXI_WR_ERROR_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000040
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    STATUS               [31:00]   AXI Error Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_1                                        (APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_OFFSET)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_OFFSET                                 (0x00000040)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_DEFAULT                                (0x00000000)


extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                            gApex642clCmemIfAxiWrError1;
extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR*                           gpApex642clCmemIfAxiWrError1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_1_BIT_STATUS                             0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    AXI_RD_ERROR_2
//    APEX_642CL_CMEM_IF_AXI_RD_ERROR_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000044
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    STATUS               [31:00]   AXI Error Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_2                                        (APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_OFFSET)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_OFFSET                                 (0x00000044)
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_DEFAULT                                (0x00000000)


extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                            gApex642clCmemIfAxiRdError2;
extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR*                           gpApex642clCmemIfAxiRdError2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_RD_ERROR_2_BIT_STATUS                             0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    AXI_WR_ERROR_2
//    APEX_642CL_CMEM_IF_AXI_WR_ERROR_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000048
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    STATUS               [31:00]   AXI Error Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_2                                        (APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_OFFSET)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_BASE_ADDRESS                           (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_OFFSET                                 (0x00000048)
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_DEFAULT                                (0x00000000)


extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR                            gApex642clCmemIfAxiWrError2;
extern volatile APEX_642CL_CMEM_IF_AXI_RD_ERROR_0_DCR*                           gpApex642clCmemIfAxiWrError2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_WR_ERROR_2_BIT_STATUS                             0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    AXI_ERR_RESP
//    APEX_642CL_CMEM_IF_AXI_ERR_RESP
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_CMEM_IF_AXI_ERR_RESP_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_ERR_RESP_OFFSET
//    Base Addr:    APEX_642CL_CMEM_IF
//    Offset:       32'h00000058
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    AXI Error Reponse control
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      AXI error response enable 
//    RESERVED             [31:01]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_ERR_RESP                                          (APEX_642CL_CMEM_IF_AXI_ERR_RESP_BASE_ADDRESS + APEX_642CL_CMEM_IF_AXI_ERR_RESP_OFFSET)
#define APEX_642CL_CMEM_IF_AXI_ERR_RESP_BASE_ADDRESS                             (APEX_642CL_CMEM_IF_BASE_ADDRESS)
#define APEX_642CL_CMEM_IF_AXI_ERR_RESP_OFFSET                                   (0x00000058)
#define APEX_642CL_CMEM_IF_AXI_ERR_RESP_DEFAULT                                  (0x00000000)


union _APEX_642CL_CMEM_IF_AXI_ERR_RESP_TAG
{
   struct _asApex642clCmemIfAxiErrRespBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_CMEM_IF_AXI_ERR_RESP_BIT_ENABLE
      REG32  Reserved             : 31; //  31:01  APEX_642CL_CMEM_IF_AXI_ERR_RESP_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_CMEM_IF_AXI_ERR_RESP_TAG                               APEX_642CL_CMEM_IF_AXI_ERR_RESP_DCR;

extern volatile APEX_642CL_CMEM_IF_AXI_ERR_RESP_DCR                              gApex642clCmemIfAxiErrResp;
extern volatile APEX_642CL_CMEM_IF_AXI_ERR_RESP_DCR*                             gpApex642clCmemIfAxiErrResp;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_CMEM_IF_AXI_ERR_RESP_BIT_ENABLE                               0

// Device Control Register Bits Id and Description
enum APEX_642CL_CMEM_IF_AXI_ERR_RESP_BIT_ENABLE_ENUM                            
{
   APEX_642CL_CMEM_IF_AXI_ERR_RESP_BIT_ENABLE_ID_ACTIVE                          =  1,          // AXI error response ACTIVE
   APEX_642CL_CMEM_IF_AXI_ERR_RESP_BIT_ENABLE_ID_INACTIVE                        =  0           // AXI error response INACTIVE
};
                                                                             

#define APEX_642CL_CMEM_IF_AXI_ERR_RESP_BIT_RESERVED                             1

#endif /* APEX642CLCMEMIFMAPTYPE_H */
