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
#ifndef APEX642CLGLOBALMAPTYPE_H
#define APEX642CLGLOBALMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RST_SET
//    APEX_642CL_GLOBAL_RST_SET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_RST_SET_BASE_ADDRESS + APEX_642CL_GLOBAL_RST_SET_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw1c
//    Nb:         1
//    Size:       4
// 
// Description:
//    Sub-Block Reset Set
//    
//    This register providees a mechanism for software to reset any or all of the
//    sub-block components. Only writing a "1" has any effect. Writing a "1" to the SET 
//    register asserts the block reset. Writing a "1" to the CLR register de-asserts the
//    block reset. The status register indicates the current setting of the block reset
// 
// Device Control Register Bits Description:
//    CMEM_IF              [00]      This bit defines the state of the cmem if reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MULTI_DMA            [01]      This bit defines the state of the multi dma reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [02]      This bit defines the state of the Mcdma reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM               [03]      This bit defines the state of the stream reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [04]      This bit defines the state of the sequencer reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    DMEM                 [05]      This bit defines the state of the dmem 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    APU                  [07]      This bit defines the state of the apu 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [09]      This bit defines the state of the HRSZ reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    VU                   [10]      This bit defines the state of the vu 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:12]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_RST_SET                                                (APEX_642CL_GLOBAL_RST_SET_BASE_ADDRESS + APEX_642CL_GLOBAL_RST_SET_OFFSET)
#define APEX_642CL_GLOBAL_RST_SET_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_RST_SET_OFFSET                                         (0x00000000)
#define APEX_642CL_GLOBAL_RST_SET_DEFAULT                                        (0x00000000)


union _APEX_642CL_GLOBAL_RST_SET_TAG
{
   struct _asApex642clGlobalRstSetBitFieldTag
   {
      REG32  CmemIf               :  1; //  00     APEX_642CL_GLOBAL_RST_SET_BIT_CMEM_IF
      REG32  MultiDma             :  1; //  01     APEX_642CL_GLOBAL_RST_SET_BIT_MULTI_DMA
      REG32  MotionComp           :  1; //  02     APEX_642CL_GLOBAL_RST_SET_BIT_MOTION_COMP
      REG32  Stream               :  1; //  03     APEX_642CL_GLOBAL_RST_SET_BIT_STREAM
      REG32  Sequencer            :  1; //  04     APEX_642CL_GLOBAL_RST_SET_BIT_SEQUENCER
      REG32  Dmem0                :  1; //  5      APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_0
      REG32  Dmem1                :  1; //  6      APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_1
      REG32  Apu0                 :  1; //  7      APEX_642CL_GLOBAL_RST_SET_BIT_APU_0
      REG32  Apu1                 :  1; //  8      APEX_642CL_GLOBAL_RST_SET_BIT_APU_1
      REG32  HrzScaler            :  1; //  09     APEX_642CL_GLOBAL_RST_SET_BIT_HRZ_SCALER
      REG32  Vu0                  :  1; //  10     APEX_642CL_GLOBAL_RST_SET_BIT_VU_0
      REG32  Vu1                  :  1; //  11     APEX_642CL_GLOBAL_RST_SET_BIT_VU_1
      REG32  Reserved             : 20; //  31:12  APEX_642CL_GLOBAL_RST_SET_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_RST_SET_TAG                                     APEX_642CL_GLOBAL_RST_SET_DCR;

extern volatile APEX_642CL_GLOBAL_RST_SET_DCR                                    gApex642clGlobalRstSet;
extern volatile APEX_642CL_GLOBAL_RST_SET_DCR*                                   gpApex642clGlobalRstSet;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_RST_SET_BIT_CMEM_IF                                    0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_CMEM_IF_ENUM                                 
{
   APEX_642CL_GLOBAL_RST_SET_BIT_CMEM_IF_ID_ACTIVE                               =  1,          // CMEM IF reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_CMEM_IF_ID_INACTIVE                             =  0           // CMEM IF reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_MULTI_DMA                                  1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_MULTI_DMA_ENUM                               
{
   APEX_642CL_GLOBAL_RST_SET_BIT_MULTI_DMA_ID_ACTIVE                             =  1,          // Multi DMA reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_MULTI_DMA_ID_INACTIVE                           =  0           // Multi DMA reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_MOTION_COMP                                2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_MOTION_COMP_ENUM                             
{
   APEX_642CL_GLOBAL_RST_SET_BIT_MOTION_COMP_ID_ACTIVE                           =  1,          // MC DMA reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_MOTION_COMP_ID_INACTIVE                         =  0           // MC DMA reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_STREAM                                     3

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_STREAM_ENUM                                  
{
   APEX_642CL_GLOBAL_RST_SET_BIT_STREAM_ID_ACTIVE                                =  1,          // Stream reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_STREAM_ID_INACTIVE                              =  0           // Stream reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_SEQUENCER                                  4

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_SEQUENCER_ENUM                               
{
   APEX_642CL_GLOBAL_RST_SET_BIT_SEQUENCER_ID_ACTIVE                             =  1,          // Sequencer reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_SEQUENCER_ID_INACTIVE                           =  0           // Sequencer reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_0                                     5
#define APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_1                                     6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_ENUM                                    
{
   APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_ID_ACTIVE                                  =  1,          // DMEM 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_DMEM_ID_INACTIVE                                =  0           // DMEM 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_APU_0                                      7
#define APEX_642CL_GLOBAL_RST_SET_BIT_APU_1                                      8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_APU_ENUM                                     
{
   APEX_642CL_GLOBAL_RST_SET_BIT_APU_ID_ACTIVE                                   =  1,          // APU 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_APU_ID_INACTIVE                                 =  0           // APU 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_HRZ_SCALER                                 9

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_HRZ_SCALER_ENUM                              
{
   APEX_642CL_GLOBAL_RST_SET_BIT_HRZ_SCALER_ID_ACTIVE                            =  1,          // HRZ_SCALER reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_HRZ_SCALER_ID_INACTIVE                          =  0           // HRZ_SCALER reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_VU_0                                       10
#define APEX_642CL_GLOBAL_RST_SET_BIT_VU_1                                       11

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_SET_BIT_VU_ENUM                                      
{
   APEX_642CL_GLOBAL_RST_SET_BIT_VU_ID_ACTIVE                                    =  1,          // VU 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_SET_BIT_VU_ID_INACTIVE                                  =  0           // VU 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_SET_BIT_RESERVED                                   12

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RST_CLEAR
//    APEX_642CL_GLOBAL_RST_CLEAR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_RST_CLEAR_BASE_ADDRESS + APEX_642CL_GLOBAL_RST_CLEAR_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw1c
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    CMEM_IF              [00]      This bit defines the state of the cmem if reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MULTI_DMA            [01]      This bit defines the state of the multi dma reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [02]      This bit defines the state of the Mcdma reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM               [03]      This bit defines the state of the stream reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [04]      This bit defines the state of the sequencer reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    DMEM                 [05]      This bit defines the state of the dmem 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    APU                  [07]      This bit defines the state of the apu 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [09]      This bit defines the state of the HRSZ reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    VU                   [10]      This bit defines the state of the vu 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:12]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_RST_CLEAR                                              (APEX_642CL_GLOBAL_RST_CLEAR_BASE_ADDRESS + APEX_642CL_GLOBAL_RST_CLEAR_OFFSET)
#define APEX_642CL_GLOBAL_RST_CLEAR_BASE_ADDRESS                                 (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_RST_CLEAR_OFFSET                                       (0x00000004)
#define APEX_642CL_GLOBAL_RST_CLEAR_DEFAULT                                      (0x00000000)


extern volatile APEX_642CL_GLOBAL_RST_SET_DCR                                    gApex642clGlobalRstClear;
extern volatile APEX_642CL_GLOBAL_RST_SET_DCR*                                   gpApex642clGlobalRstClear;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_CMEM_IF                                  0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_CMEM_IF_ENUM                               
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_CMEM_IF_ID_ACTIVE                             =  1,          // CMEM IF reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_CMEM_IF_ID_INACTIVE                           =  0           // CMEM IF reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_MULTI_DMA                                1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_MULTI_DMA_ENUM                             
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_MULTI_DMA_ID_ACTIVE                           =  1,          // Multi DMA reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_MULTI_DMA_ID_INACTIVE                         =  0           // Multi DMA reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_MOTION_COMP                              2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_MOTION_COMP_ENUM                           
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_MOTION_COMP_ID_ACTIVE                         =  1,          // MC DMA reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_MOTION_COMP_ID_INACTIVE                       =  0           // MC DMA reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_STREAM                                   3

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_STREAM_ENUM                                
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_STREAM_ID_ACTIVE                              =  1,          // Stream reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_STREAM_ID_INACTIVE                            =  0           // Stream reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_SEQUENCER                                4

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_SEQUENCER_ENUM                             
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_SEQUENCER_ID_ACTIVE                           =  1,          // Sequencer reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_SEQUENCER_ID_INACTIVE                         =  0           // Sequencer reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_DMEM_0                                   5
#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_DMEM_1                                   6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_DMEM_ENUM                                  
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_DMEM_ID_ACTIVE                                =  1,          // DMEM 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_DMEM_ID_INACTIVE                              =  0           // DMEM 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_APU_0                                    7
#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_APU_1                                    8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_APU_ENUM                                   
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_APU_ID_ACTIVE                                 =  1,          // APU 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_APU_ID_INACTIVE                               =  0           // APU 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_HRZ_SCALER                               9

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_HRZ_SCALER_ENUM                            
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_HRZ_SCALER_ID_ACTIVE                          =  1,          // HRZ_SCALER reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_HRZ_SCALER_ID_INACTIVE                        =  0           // HRZ_SCALER reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_VU_0                                     10
#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_VU_1                                     11

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_CLEAR_BIT_VU_ENUM                                    
{
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_VU_ID_ACTIVE                                  =  1,          // VU 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_CLEAR_BIT_VU_ID_INACTIVE                                =  0           // VU 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_CLEAR_BIT_RESERVED                                 12

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RST_STATUS
//    APEX_642CL_GLOBAL_RST_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_RST_STATUS_BASE_ADDRESS + APEX_642CL_GLOBAL_RST_STATUS_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Sub-Block Reset Status
//    
//    This register indicates the current setting of the block reset
// 
// Device Control Register Bits Description:
//    CMEM_IF              [00]      This bit defines the state of the cmem if reset set, clear and status
//                                   
//                                   0: Inactive
//                                   
//                                   This bit will always be '0', the cmem_if reset is a pusle to prevent the cmem_if to stay in reset 
//    MULTI_DMA            [01]      This bit defines the state of the multi dma reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [02]      This bit defines the state of the Mcdma reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM               [03]      This bit defines the state of the stream reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [04]      This bit defines the state of the sequencer reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    DMEM                 [05]      This bit defines the state of the dmem 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    APU                  [07]      This bit defines the state of the apu 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [09]      This bit defines the state of the HRSZ reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    VU                   [10]      This bit defines the state of the vu 0 reset set, clear and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:12]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_RST_STATUS                                             (APEX_642CL_GLOBAL_RST_STATUS_BASE_ADDRESS + APEX_642CL_GLOBAL_RST_STATUS_OFFSET)
#define APEX_642CL_GLOBAL_RST_STATUS_BASE_ADDRESS                                (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_RST_STATUS_OFFSET                                      (0x00000008)
#define APEX_642CL_GLOBAL_RST_STATUS_DEFAULT                                     (0x00000000)


union _APEX_642CL_GLOBAL_RST_STATUS_TAG
{
   struct _asApex642clGlobalRstStatusBitFieldTag
   {
      REG32  CmemIf               :  1; //  00     APEX_642CL_GLOBAL_RST_STATUS_BIT_CMEM_IF
      REG32  MultiDma             :  1; //  01     APEX_642CL_GLOBAL_RST_STATUS_BIT_MULTI_DMA
      REG32  MotionComp           :  1; //  02     APEX_642CL_GLOBAL_RST_STATUS_BIT_MOTION_COMP
      REG32  Stream               :  1; //  03     APEX_642CL_GLOBAL_RST_STATUS_BIT_STREAM
      REG32  Sequencer            :  1; //  04     APEX_642CL_GLOBAL_RST_STATUS_BIT_SEQUENCER
      REG32  Dmem0                :  1; //  5      APEX_642CL_GLOBAL_RST_STATUS_BIT_DMEM_0
      REG32  Dmem1                :  1; //  6      APEX_642CL_GLOBAL_RST_STATUS_BIT_DMEM_1
      REG32  Apu0                 :  1; //  7      APEX_642CL_GLOBAL_RST_STATUS_BIT_APU_0
      REG32  Apu1                 :  1; //  8      APEX_642CL_GLOBAL_RST_STATUS_BIT_APU_1
      REG32  HrzScaler            :  1; //  09     APEX_642CL_GLOBAL_RST_STATUS_BIT_HRZ_SCALER
      REG32  Vu0                  :  1; //  10     APEX_642CL_GLOBAL_RST_STATUS_BIT_VU_0
      REG32  Vu1                  :  1; //  11     APEX_642CL_GLOBAL_RST_STATUS_BIT_VU_1
      REG32  Reserved             : 20; //  31:12  APEX_642CL_GLOBAL_RST_STATUS_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_RST_STATUS_TAG                                  APEX_642CL_GLOBAL_RST_STATUS_DCR;

extern volatile APEX_642CL_GLOBAL_RST_STATUS_DCR                                 gApex642clGlobalRstStatus;
extern volatile APEX_642CL_GLOBAL_RST_STATUS_DCR*                                gpApex642clGlobalRstStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_RST_STATUS_BIT_CMEM_IF                                 0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_CMEM_IF_ENUM                              
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_CMEM_IF_ID_INACTIVE                          =  0           // CMEM IF reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_MULTI_DMA                               1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_MULTI_DMA_ENUM                            
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_MULTI_DMA_ID_ACTIVE                          =  1,          // Multi DMA reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_MULTI_DMA_ID_INACTIVE                        =  0           // Multi DMA reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_MOTION_COMP                             2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_MOTION_COMP_ENUM                          
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_MOTION_COMP_ID_ACTIVE                        =  1,          // MC DMA reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_MOTION_COMP_ID_INACTIVE                      =  0           // MC DMA reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_STREAM                                  3

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_STREAM_ENUM                               
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_STREAM_ID_ACTIVE                             =  1,          // Stream reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_STREAM_ID_INACTIVE                           =  0           // Stream reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_SEQUENCER                               4

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_SEQUENCER_ENUM                            
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_SEQUENCER_ID_ACTIVE                          =  1,          // Sequencer reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_SEQUENCER_ID_INACTIVE                        =  0           // Sequencer reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_DMEM_0                                  5
#define APEX_642CL_GLOBAL_RST_STATUS_BIT_DMEM_1                                  6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_DMEM_ENUM                                 
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_DMEM_ID_ACTIVE                               =  1,          // DMEM 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_DMEM_ID_INACTIVE                             =  0           // DMEM 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_APU_0                                   7
#define APEX_642CL_GLOBAL_RST_STATUS_BIT_APU_1                                   8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_APU_ENUM                                  
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_APU_ID_ACTIVE                                =  1,          // APU 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_APU_ID_INACTIVE                              =  0           // APU 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_HRZ_SCALER                              9

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_HRZ_SCALER_ENUM                           
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_HRZ_SCALER_ID_ACTIVE                         =  1,          // HRZ_SCALER reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_HRZ_SCALER_ID_INACTIVE                       =  0           // HRZ_SCALER reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_VU_0                                    10
#define APEX_642CL_GLOBAL_RST_STATUS_BIT_VU_1                                    11

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_RST_STATUS_BIT_VU_ENUM                                   
{
   APEX_642CL_GLOBAL_RST_STATUS_BIT_VU_ID_ACTIVE                                 =  1,          // VU 0 reset set, clear and status ACTIVE
   APEX_642CL_GLOBAL_RST_STATUS_BIT_VU_ID_INACTIVE                               =  0           // VU 0 reset set, clear and status INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_RST_STATUS_BIT_RESERVED                                12

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    INT_STATUS
//    APEX_642CL_GLOBAL_INT_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_INT_STATUS_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_STATUS_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Status
// 
// Device Control Register Bits Description:
//    MULTI_DMA            [00]      This bit represents the state of the multi dma block interrupt
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [01]      This bit represents the state of the motion compensation dma block interrupt
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [02]      This bit represents the state of the sequencer block interrupt
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_IN            [03]      This bit represents the state of the stream in 0 block interrupt
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_OUT           [07]      This bit represents the state of the stream out 0 block interrupt
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [09]      This bit represents the state of the HRSZ 0 block interrupt
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:11]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_STATUS                                             (APEX_642CL_GLOBAL_INT_STATUS_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_STATUS_OFFSET)
#define APEX_642CL_GLOBAL_INT_STATUS_BASE_ADDRESS                                (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_INT_STATUS_OFFSET                                      (0x0000000c)
#define APEX_642CL_GLOBAL_INT_STATUS_DEFAULT                                     (0x00000000)


union _APEX_642CL_GLOBAL_INT_STATUS_TAG
{
   struct _asApex642clGlobalIntStatusBitFieldTag
   {
      REG32  MultiDma             :  1; //  00     APEX_642CL_GLOBAL_INT_STATUS_BIT_MULTI_DMA
      REG32  MotionComp           :  1; //  01     APEX_642CL_GLOBAL_INT_STATUS_BIT_MOTION_COMP
      REG32  Sequencer            :  1; //  02     APEX_642CL_GLOBAL_INT_STATUS_BIT_SEQUENCER
      REG32  StreamIn0            :  1; //  3      APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_0
      REG32  StreamIn1            :  1; //  4      APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_1
      REG32  StreamIn2            :  1; //  5      APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_2
      REG32  StreamIn3            :  1; //  6      APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_3
      REG32  StreamOut0           :  1; //  7      APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_OUT_0
      REG32  StreamOut1           :  1; //  8      APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_OUT_1
      REG32  HrzScaler0           :  1; //  9      APEX_642CL_GLOBAL_INT_STATUS_BIT_HRZ_SCALER_0
      REG32  HrzScaler1           :  1; //  10     APEX_642CL_GLOBAL_INT_STATUS_BIT_HRZ_SCALER_1
      REG32  Reserved             : 21; //  31:11  APEX_642CL_GLOBAL_INT_STATUS_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_INT_STATUS_TAG                                  APEX_642CL_GLOBAL_INT_STATUS_DCR;

extern volatile APEX_642CL_GLOBAL_INT_STATUS_DCR                                 gApex642clGlobalIntStatus;
extern volatile APEX_642CL_GLOBAL_INT_STATUS_DCR*                                gpApex642clGlobalIntStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_STATUS_BIT_MULTI_DMA                               0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_STATUS_BIT_MULTI_DMA_ENUM                            
{
   APEX_642CL_GLOBAL_INT_STATUS_BIT_MULTI_DMA_ID_ACTIVE                          =  1,          // Multi DMA Interrupt ACTIVE
   APEX_642CL_GLOBAL_INT_STATUS_BIT_MULTI_DMA_ID_INACTIVE                        =  0           // Multi DMA Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_STATUS_BIT_MOTION_COMP                             1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_STATUS_BIT_MOTION_COMP_ENUM                          
{
   APEX_642CL_GLOBAL_INT_STATUS_BIT_MOTION_COMP_ID_ACTIVE                        =  1,          // Notion compensation DMA Interrupt ACTIVE
   APEX_642CL_GLOBAL_INT_STATUS_BIT_MOTION_COMP_ID_INACTIVE                      =  0           // Notion compensation DMA Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_STATUS_BIT_SEQUENCER                               2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_STATUS_BIT_SEQUENCER_ENUM                            
{
   APEX_642CL_GLOBAL_INT_STATUS_BIT_SEQUENCER_ID_ACTIVE                          =  1,          // Sequencer Interrupt ACTIVE
   APEX_642CL_GLOBAL_INT_STATUS_BIT_SEQUENCER_ID_INACTIVE                        =  0           // Sequencer Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_0                             3
#define APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_1                             4
#define APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_2                             5
#define APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_3                             6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_ENUM                            
{
   APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_ID_ACTIVE                          =  1,          // Stream In 0 Interrupt ACTIVE
   APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_IN_ID_INACTIVE                        =  0           // Stream In 0 Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_OUT_0                            7
#define APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_OUT_1                            8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_OUT_ENUM                           
{
   APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_OUT_ID_ACTIVE                         =  1,          // Stream Out 0 Interrupt ACTIVE
   APEX_642CL_GLOBAL_INT_STATUS_BIT_STREAM_OUT_ID_INACTIVE                       =  0           // Stream Out 0 Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_STATUS_BIT_HRZ_SCALER_0                            9
#define APEX_642CL_GLOBAL_INT_STATUS_BIT_HRZ_SCALER_1                            10

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_STATUS_BIT_HRZ_SCALER_ENUM                           
{
   APEX_642CL_GLOBAL_INT_STATUS_BIT_HRZ_SCALER_ID_ACTIVE                         =  1,          // HRSZ 0 Interrupt ACTIVE
   APEX_642CL_GLOBAL_INT_STATUS_BIT_HRZ_SCALER_ID_INACTIVE                       =  0           // HRSZ 0 Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_STATUS_BIT_RESERVED                                11

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    INT_MASK_SET
//    APEX_642CL_GLOBAL_INT_MASK_SET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_INT_MASK_SET_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_MASK_SET_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    Sub-Block Mask Unmask
//    
//    This register providees a mechanism for software to mask or unmask any or all of the sub-block components. 
//    Only writing a "1" has any effect. Writing a "1" to the SET  register masks the interrupt. Writing a "1" to the CLR register
//    unmasks the interrupt. The status register indicates the current setting of the block interrupt mask
// 
// Device Control Register Bits Description:
//    MULTI_DMA            [00]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [01]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [02]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_IN            [03]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_OUT           [07]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [09]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:11]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_MASK_SET                                           (APEX_642CL_GLOBAL_INT_MASK_SET_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_MASK_SET_OFFSET)
#define APEX_642CL_GLOBAL_INT_MASK_SET_BASE_ADDRESS                              (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_INT_MASK_SET_OFFSET                                    (0x00000010)
#define APEX_642CL_GLOBAL_INT_MASK_SET_DEFAULT                                   (0x00000000)


union _APEX_642CL_GLOBAL_INT_MASK_SET_TAG
{
   struct _asApex642clGlobalIntMaskSetBitFieldTag
   {
      REG32  MultiDma             :  1; //  00     APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MULTI_DMA
      REG32  MotionComp           :  1; //  01     APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MOTION_COMP
      REG32  Sequencer            :  1; //  02     APEX_642CL_GLOBAL_INT_MASK_SET_BIT_SEQUENCER
      REG32  StreamIn0            :  1; //  3      APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_0
      REG32  StreamIn1            :  1; //  4      APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_1
      REG32  StreamIn2            :  1; //  5      APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_2
      REG32  StreamIn3            :  1; //  6      APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_3
      REG32  StreamOut0           :  1; //  7      APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_OUT_0
      REG32  StreamOut1           :  1; //  8      APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_OUT_1
      REG32  HrzScaler0           :  1; //  9      APEX_642CL_GLOBAL_INT_MASK_SET_BIT_HRZ_SCALER_0
      REG32  HrzScaler1           :  1; //  10     APEX_642CL_GLOBAL_INT_MASK_SET_BIT_HRZ_SCALER_1
      REG32  Reserved             : 21; //  31:11  APEX_642CL_GLOBAL_INT_MASK_SET_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_INT_MASK_SET_TAG                                APEX_642CL_GLOBAL_INT_MASK_SET_DCR;

extern volatile APEX_642CL_GLOBAL_INT_MASK_SET_DCR                               gApex642clGlobalIntMaskSet;
extern volatile APEX_642CL_GLOBAL_INT_MASK_SET_DCR*                              gpApex642clGlobalIntMaskSet;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MULTI_DMA                             0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MULTI_DMA_ENUM                          
{
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MULTI_DMA_ID_ACTIVE                        =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MULTI_DMA_ID_INACTIVE                      =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MOTION_COMP                           1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MOTION_COMP_ENUM                        
{
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MOTION_COMP_ID_ACTIVE                      =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_MOTION_COMP_ID_INACTIVE                    =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_SEQUENCER                             2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_SET_BIT_SEQUENCER_ENUM                          
{
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_SEQUENCER_ID_ACTIVE                        =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_SEQUENCER_ID_INACTIVE                      =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_0                           3
#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_1                           4
#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_2                           5
#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_3                           6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_ENUM                          
{
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_ID_ACTIVE                        =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_IN_ID_INACTIVE                      =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_OUT_0                          7
#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_OUT_1                          8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_OUT_ENUM                         
{
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_OUT_ID_ACTIVE                       =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_STREAM_OUT_ID_INACTIVE                     =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_HRZ_SCALER_0                          9
#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_HRZ_SCALER_1                          10

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_SET_BIT_HRZ_SCALER_ENUM                         
{
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_HRZ_SCALER_ID_ACTIVE                       =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_SET_BIT_HRZ_SCALER_ID_INACTIVE                     =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_SET_BIT_RESERVED                              11

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    INT_MASK_CLEAR
//    APEX_642CL_GLOBAL_INT_MASK_CLEAR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_INT_MASK_CLEAR_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_MASK_CLEAR_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    MULTI_DMA            [00]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [01]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [02]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_IN            [03]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_OUT           [07]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [09]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:11]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR                                         (APEX_642CL_GLOBAL_INT_MASK_CLEAR_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_MASK_CLEAR_OFFSET)
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BASE_ADDRESS                            (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_OFFSET                                  (0x00000014)
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_DEFAULT                                 (0x00000000)


extern volatile APEX_642CL_GLOBAL_INT_MASK_SET_DCR                               gApex642clGlobalIntMaskClear;
extern volatile APEX_642CL_GLOBAL_INT_MASK_SET_DCR*                              gpApex642clGlobalIntMaskClear;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MULTI_DMA                           0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MULTI_DMA_ENUM                        
{
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MULTI_DMA_ID_ACTIVE                      =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MULTI_DMA_ID_INACTIVE                    =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MOTION_COMP                         1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MOTION_COMP_ENUM                      
{
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MOTION_COMP_ID_ACTIVE                    =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_MOTION_COMP_ID_INACTIVE                  =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_SEQUENCER                           2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_SEQUENCER_ENUM                        
{
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_SEQUENCER_ID_ACTIVE                      =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_SEQUENCER_ID_INACTIVE                    =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_IN_0                         3
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_IN_1                         4
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_IN_2                         5
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_IN_3                         6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_IN_ENUM                        
{
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_IN_ID_ACTIVE                      =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_IN_ID_INACTIVE                    =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_OUT_0                        7
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_OUT_1                        8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_OUT_ENUM                       
{
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_OUT_ID_ACTIVE                     =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_STREAM_OUT_ID_INACTIVE                   =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_HRZ_SCALER_0                        9
#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_HRZ_SCALER_1                        10

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_HRZ_SCALER_ENUM                       
{
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_HRZ_SCALER_ID_ACTIVE                     =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_HRZ_SCALER_ID_INACTIVE                   =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_CLEAR_BIT_RESERVED                            11

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    INT_MASK_STATUS
//    APEX_642CL_GLOBAL_INT_MASK_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_INT_MASK_STATUS_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_MASK_STATUS_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000018
//    Default:      32'hffffffff
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    MULTI_DMA            [00]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [01]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [02]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_IN            [03]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM_OUT           [07]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [09]      This bit defines the state of the  interrupt mask - unmask, mask and status
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:11]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_MASK_STATUS                                        (APEX_642CL_GLOBAL_INT_MASK_STATUS_BASE_ADDRESS + APEX_642CL_GLOBAL_INT_MASK_STATUS_OFFSET)
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BASE_ADDRESS                           (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_OFFSET                                 (0x00000018)
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_DEFAULT                                (0xffffffff)


union _APEX_642CL_GLOBAL_INT_MASK_STATUS_TAG
{
   struct _asApex642clGlobalIntMaskStatusBitFieldTag
   {
      REG32  MultiDma             :  1; //  00     APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MULTI_DMA
      REG32  MotionComp           :  1; //  01     APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MOTION_COMP
      REG32  Sequencer            :  1; //  02     APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_SEQUENCER
      REG32  StreamIn0            :  1; //  3      APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_0
      REG32  StreamIn1            :  1; //  4      APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_1
      REG32  StreamIn2            :  1; //  5      APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_2
      REG32  StreamIn3            :  1; //  6      APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_3
      REG32  StreamOut0           :  1; //  7      APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_OUT_0
      REG32  StreamOut1           :  1; //  8      APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_OUT_1
      REG32  HrzScaler0           :  1; //  9      APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_HRZ_SCALER_0
      REG32  HrzScaler1           :  1; //  10     APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_HRZ_SCALER_1
      REG32  Reserved             : 21; //  31:11  APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_INT_MASK_STATUS_TAG                             APEX_642CL_GLOBAL_INT_MASK_STATUS_DCR;

extern volatile APEX_642CL_GLOBAL_INT_MASK_STATUS_DCR                            gApex642clGlobalIntMaskStatus;
extern volatile APEX_642CL_GLOBAL_INT_MASK_STATUS_DCR*                           gpApex642clGlobalIntMaskStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MULTI_DMA                          0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MULTI_DMA_ENUM                       
{
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MULTI_DMA_ID_ACTIVE                     =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MULTI_DMA_ID_INACTIVE                   =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MOTION_COMP                        1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MOTION_COMP_ENUM                     
{
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MOTION_COMP_ID_ACTIVE                   =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_MOTION_COMP_ID_INACTIVE                 =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_SEQUENCER                          2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_SEQUENCER_ENUM                       
{
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_SEQUENCER_ID_ACTIVE                     =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_SEQUENCER_ID_INACTIVE                   =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_0                        3
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_1                        4
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_2                        5
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_3                        6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_ENUM                       
{
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_ID_ACTIVE                     =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_IN_ID_INACTIVE                   =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_OUT_0                       7
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_OUT_1                       8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_OUT_ENUM                      
{
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_OUT_ID_ACTIVE                    =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_STREAM_OUT_ID_INACTIVE                  =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_HRZ_SCALER_0                       9
#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_HRZ_SCALER_1                       10

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_HRZ_SCALER_ENUM                      
{
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_HRZ_SCALER_ID_ACTIVE                    =  1,          // Masking Action ACTIVE
   APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_HRZ_SCALER_ID_INACTIVE                  =  0           // Masking Action INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_INT_MASK_STATUS_BIT_RESERVED                           11

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MISC
//    APEX_642CL_GLOBAL_MISC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_MISC_BASE_ADDRESS + APEX_642CL_GLOBAL_MISC_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Miscellaneuos Global Control
// 
// Device Control Register Bits Description:
//    APU_ENABLE           [00]      This bit defines the state of the apu 0 enable
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_MISC                                                   (APEX_642CL_GLOBAL_MISC_BASE_ADDRESS + APEX_642CL_GLOBAL_MISC_OFFSET)
#define APEX_642CL_GLOBAL_MISC_BASE_ADDRESS                                      (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_MISC_OFFSET                                            (0x0000001c)
#define APEX_642CL_GLOBAL_MISC_DEFAULT                                           (0x00000000)


union _APEX_642CL_GLOBAL_MISC_TAG
{
   struct _asApex642clGlobalMiscBitFieldTag
   {
      REG32  ApuEnable0           :  1; //  0      APEX_642CL_GLOBAL_MISC_BIT_APU_ENABLE_0
      REG32  ApuEnable1           :  1; //  1      APEX_642CL_GLOBAL_MISC_BIT_APU_ENABLE_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_GLOBAL_MISC_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_MISC_TAG                                        APEX_642CL_GLOBAL_MISC_DCR;

extern volatile APEX_642CL_GLOBAL_MISC_DCR                                       gApex642clGlobalMisc;
extern volatile APEX_642CL_GLOBAL_MISC_DCR*                                      gpApex642clGlobalMisc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_MISC_BIT_APU_ENABLE_0                                  0
#define APEX_642CL_GLOBAL_MISC_BIT_APU_ENABLE_1                                  1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_MISC_BIT_APU_ENABLE_ENUM                                 
{
   APEX_642CL_GLOBAL_MISC_BIT_APU_ENABLE_ID_ACTIVE                               =  1,          // APU ACTIVE
   APEX_642CL_GLOBAL_MISC_BIT_APU_ENABLE_ID_INACTIVE                             =  0           // APU INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_MISC_BIT_RESERVED                                      2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PDOWN
//    APEX_642CL_GLOBAL_PDOWN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_PDOWN_BASE_ADDRESS + APEX_642CL_GLOBAL_PDOWN_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Block level power down
// 
// Device Control Register Bits Description:
//    MULTI_DMA            [00]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    MOTION_COMP          [01]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    STREAM               [02]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    SEQUENCER            [03]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    DFIFO                [04]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    HRZ_SCALER           [06]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    APU                  [07]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED_0           [09]      Reserved 
//    VU                   [10]      This bit will enable the clock gate for the clocks going to the block and enables the power savings mode.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    RESERVED_1           [31:12]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_PDOWN                                                  (APEX_642CL_GLOBAL_PDOWN_BASE_ADDRESS + APEX_642CL_GLOBAL_PDOWN_OFFSET)
#define APEX_642CL_GLOBAL_PDOWN_BASE_ADDRESS                                     (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_PDOWN_OFFSET                                           (0x00000020)
#define APEX_642CL_GLOBAL_PDOWN_DEFAULT                                          (0x00000000)


union _APEX_642CL_GLOBAL_PDOWN_TAG
{
   struct _asApex642clGlobalPdownBitFieldTag
   {
      REG32  MultiDma             :  1; //  00     APEX_642CL_GLOBAL_PDOWN_BIT_MULTI_DMA
      REG32  MotionComp           :  1; //  01     APEX_642CL_GLOBAL_PDOWN_BIT_MOTION_COMP
      REG32  Stream               :  1; //  02     APEX_642CL_GLOBAL_PDOWN_BIT_STREAM
      REG32  Sequencer            :  1; //  03     APEX_642CL_GLOBAL_PDOWN_BIT_SEQUENCER
      REG32  Dfifo0               :  1; //  4      APEX_642CL_GLOBAL_PDOWN_BIT_DFIFO_0
      REG32  Dfifo1               :  1; //  5      APEX_642CL_GLOBAL_PDOWN_BIT_DFIFO_1
      REG32  HrzScaler            :  1; //  06     APEX_642CL_GLOBAL_PDOWN_BIT_HRZ_SCALER
      REG32  Apu0                 :  1; //  7      APEX_642CL_GLOBAL_PDOWN_BIT_APU_0
      REG32  Apu1                 :  1; //  8      APEX_642CL_GLOBAL_PDOWN_BIT_APU_1
      REG32  Reserved0            :  1; //  09     APEX_642CL_GLOBAL_PDOWN_BIT_RESERVED_0
      REG32  Vu0                  :  1; //  10     APEX_642CL_GLOBAL_PDOWN_BIT_VU_0
      REG32  Vu1                  :  1; //  11     APEX_642CL_GLOBAL_PDOWN_BIT_VU_1
      REG32  Reserved1            : 20; //  31:12  APEX_642CL_GLOBAL_PDOWN_BIT_RESERVED_1
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_PDOWN_TAG                                       APEX_642CL_GLOBAL_PDOWN_DCR;

extern volatile APEX_642CL_GLOBAL_PDOWN_DCR                                      gApex642clGlobalPdown;
extern volatile APEX_642CL_GLOBAL_PDOWN_DCR*                                     gpApex642clGlobalPdown;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_PDOWN_BIT_MULTI_DMA                                    0

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_MULTI_DMA_ENUM                                 
{
   APEX_642CL_GLOBAL_PDOWN_BIT_MULTI_DMA_ID_ACTIVE                               =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_MULTI_DMA_ID_INACTIVE                             =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_MOTION_COMP                                  1

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_MOTION_COMP_ENUM                               
{
   APEX_642CL_GLOBAL_PDOWN_BIT_MOTION_COMP_ID_ACTIVE                             =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_MOTION_COMP_ID_INACTIVE                           =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_STREAM                                       2

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_STREAM_ENUM                                    
{
   APEX_642CL_GLOBAL_PDOWN_BIT_STREAM_ID_ACTIVE                                  =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_STREAM_ID_INACTIVE                                =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_SEQUENCER                                    3

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_SEQUENCER_ENUM                                 
{
   APEX_642CL_GLOBAL_PDOWN_BIT_SEQUENCER_ID_ACTIVE                               =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_SEQUENCER_ID_INACTIVE                             =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_DFIFO_0                                      4
#define APEX_642CL_GLOBAL_PDOWN_BIT_DFIFO_1                                      5

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_DFIFO_ENUM                                     
{
   APEX_642CL_GLOBAL_PDOWN_BIT_DFIFO_ID_ACTIVE                                   =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_DFIFO_ID_INACTIVE                                 =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_HRZ_SCALER                                   6

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_HRZ_SCALER_ENUM                                
{
   APEX_642CL_GLOBAL_PDOWN_BIT_HRZ_SCALER_ID_ACTIVE                              =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_HRZ_SCALER_ID_INACTIVE                            =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_APU_0                                        7
#define APEX_642CL_GLOBAL_PDOWN_BIT_APU_1                                        8

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_APU_ENUM                                       
{
   APEX_642CL_GLOBAL_PDOWN_BIT_APU_ID_ACTIVE                                     =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_APU_ID_INACTIVE                                   =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_RESERVED_0                                   9
#define APEX_642CL_GLOBAL_PDOWN_BIT_VU_0                                         10
#define APEX_642CL_GLOBAL_PDOWN_BIT_VU_1                                         11

// Device Control Register Bits Id and Description
enum APEX_642CL_GLOBAL_PDOWN_BIT_VU_ENUM                                        
{
   APEX_642CL_GLOBAL_PDOWN_BIT_VU_ID_ACTIVE                                      =  1,          // Clock gating ACTIVE
   APEX_642CL_GLOBAL_PDOWN_BIT_VU_ID_INACTIVE                                    =  0           // Clock gating INACTIVE
};
                                                                             

#define APEX_642CL_GLOBAL_PDOWN_BIT_RESERVED_1                                   12

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    VERSION
//    APEX_642CL_GLOBAL_VERSION
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_VERSION_BASE_ADDRESS + APEX_642CL_GLOBAL_VERSION_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000024
//    Default:      32'h06421309
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Hardware Version Control to trace release APEX IP
// 
// Device Control Register Bits Description:
//    SW                   [15:00]   IP Programmer Version 
//    HW                   [31:16]   ACP and CU Configuration 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_VERSION                                                (APEX_642CL_GLOBAL_VERSION_BASE_ADDRESS + APEX_642CL_GLOBAL_VERSION_OFFSET)
#define APEX_642CL_GLOBAL_VERSION_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_VERSION_OFFSET                                         (0x00000024)
#define APEX_642CL_GLOBAL_VERSION_DEFAULT                                        (0x06421309)


union _APEX_642CL_GLOBAL_VERSION_TAG
{
   struct _asApex642clGlobalVersionBitFieldTag
   {
      REG32  Sw                   : 16; //  15:00  APEX_642CL_GLOBAL_VERSION_BIT_SW
      REG32  Hw                   : 16; //  31:16  APEX_642CL_GLOBAL_VERSION_BIT_HW
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_VERSION_TAG                                     APEX_642CL_GLOBAL_VERSION_DCR;

extern volatile APEX_642CL_GLOBAL_VERSION_DCR                                    gApex642clGlobalVersion;
extern volatile APEX_642CL_GLOBAL_VERSION_DCR*                                   gpApex642clGlobalVersion;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_VERSION_BIT_SW                                         0
#define APEX_642CL_GLOBAL_VERSION_BIT_HW                                         16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BUILD_NUMBER
//    APEX_642CL_GLOBAL_BUILD_NUMBER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_BUILD_NUMBER_BASE_ADDRESS + APEX_642CL_GLOBAL_BUILD_NUMBER_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Hardware Version Control to trace release APEX IP to Cognivue Mercurial Version Control
// 
// Device Control Register Bits Description:
//    NUM                  [31:00]   Build Number 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_BUILD_NUMBER                                           (APEX_642CL_GLOBAL_BUILD_NUMBER_BASE_ADDRESS + APEX_642CL_GLOBAL_BUILD_NUMBER_OFFSET)
#define APEX_642CL_GLOBAL_BUILD_NUMBER_BASE_ADDRESS                              (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_BUILD_NUMBER_OFFSET                                    (0x00000028)
#define APEX_642CL_GLOBAL_BUILD_NUMBER_DEFAULT                                   (0x00000000)


union _APEX_642CL_GLOBAL_BUILD_NUMBER_TAG
{
   struct _asApex642clGlobalBuildNumberBitFieldTag
   {
      REG32  Num                  : 32; //  31:00  APEX_642CL_GLOBAL_BUILD_NUMBER_BIT_NUM
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_BUILD_NUMBER_TAG                                APEX_642CL_GLOBAL_BUILD_NUMBER_DCR;

extern volatile APEX_642CL_GLOBAL_BUILD_NUMBER_DCR                               gApex642clGlobalBuildNumber;
extern volatile APEX_642CL_GLOBAL_BUILD_NUMBER_DCR*                              gpApex642clGlobalBuildNumber;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_BUILD_NUMBER_BIT_NUM                                   0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BUILD_DATE
//    APEX_642CL_GLOBAL_BUILD_DATE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_BUILD_DATE_BASE_ADDRESS + APEX_642CL_GLOBAL_BUILD_DATE_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Hardware Version Control to trace release APEX IP to Cognivue Mercurial Version Control
// 
// Device Control Register Bits Description:
//    DATE                 [31:00]   Build Date 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_BUILD_DATE                                             (APEX_642CL_GLOBAL_BUILD_DATE_BASE_ADDRESS + APEX_642CL_GLOBAL_BUILD_DATE_OFFSET)
#define APEX_642CL_GLOBAL_BUILD_DATE_BASE_ADDRESS                                (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_BUILD_DATE_OFFSET                                      (0x0000002c)
#define APEX_642CL_GLOBAL_BUILD_DATE_DEFAULT                                     (0x00000000)


union _APEX_642CL_GLOBAL_BUILD_DATE_TAG
{
   struct _asApex642clGlobalBuildDateBitFieldTag
   {
      REG32  Date                 : 32; //  31:00  APEX_642CL_GLOBAL_BUILD_DATE_BIT_DATE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_BUILD_DATE_TAG                                  APEX_642CL_GLOBAL_BUILD_DATE_DCR;

extern volatile APEX_642CL_GLOBAL_BUILD_DATE_DCR                                 gApex642clGlobalBuildDate;
extern volatile APEX_642CL_GLOBAL_BUILD_DATE_DCR*                                gpApex642clGlobalBuildDate;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_BUILD_DATE_BIT_DATE                                    0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SWCFG
//    APEX_642CL_GLOBAL_SWCFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_GLOBAL_SWCFG_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_OFFSET
//    Base Addr:    APEX_642CL_GLOBAL
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         8
//    Size:       4
// 
// Description:
//    Software System Configuration.
//    Board Support Package (BSP) will initialize these 8 registers with a datastructure that defines the HW resources.
//    APU Kernel(s) can query these datastructure to perform Hardware Resource Allocation.
// 
// Device Control Register Bits Description:
//    SWDEFDS              [31:00]   Software Defined Data Structure 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_SWCFG_0                                                (APEX_642CL_GLOBAL_SWCFG_0_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_0_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_0_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_0_OFFSET                                         (0x00000030)
#define APEX_642CL_GLOBAL_SWCFG_0_DEFAULT                                        (0x00000000)

#define APEX_642CL_GLOBAL_SWCFG_1                                                (APEX_642CL_GLOBAL_SWCFG_1_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_1_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_1_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_1_OFFSET                                         (0x00000034)
#define APEX_642CL_GLOBAL_SWCFG_1_DEFAULT                                        (0x00000000)

#define APEX_642CL_GLOBAL_SWCFG_2                                                (APEX_642CL_GLOBAL_SWCFG_2_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_2_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_2_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_2_OFFSET                                         (0x00000038)
#define APEX_642CL_GLOBAL_SWCFG_2_DEFAULT                                        (0x00000000)

#define APEX_642CL_GLOBAL_SWCFG_3                                                (APEX_642CL_GLOBAL_SWCFG_3_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_3_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_3_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_3_OFFSET                                         (0x0000003c)
#define APEX_642CL_GLOBAL_SWCFG_3_DEFAULT                                        (0x00000000)

#define APEX_642CL_GLOBAL_SWCFG_4                                                (APEX_642CL_GLOBAL_SWCFG_4_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_4_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_4_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_4_OFFSET                                         (0x00000040)
#define APEX_642CL_GLOBAL_SWCFG_4_DEFAULT                                        (0x00000000)

#define APEX_642CL_GLOBAL_SWCFG_5                                                (APEX_642CL_GLOBAL_SWCFG_5_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_5_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_5_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_5_OFFSET                                         (0x00000044)
#define APEX_642CL_GLOBAL_SWCFG_5_DEFAULT                                        (0x00000000)

#define APEX_642CL_GLOBAL_SWCFG_6                                                (APEX_642CL_GLOBAL_SWCFG_6_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_6_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_6_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_6_OFFSET                                         (0x00000048)
#define APEX_642CL_GLOBAL_SWCFG_6_DEFAULT                                        (0x00000000)

#define APEX_642CL_GLOBAL_SWCFG_7                                                (APEX_642CL_GLOBAL_SWCFG_7_BASE_ADDRESS + APEX_642CL_GLOBAL_SWCFG_7_OFFSET)
#define APEX_642CL_GLOBAL_SWCFG_7_BASE_ADDRESS                                   (APEX_642CL_GLOBAL_BASE_ADDRESS)
#define APEX_642CL_GLOBAL_SWCFG_7_OFFSET                                         (0x0000004c)
#define APEX_642CL_GLOBAL_SWCFG_7_DEFAULT                                        (0x00000000)


union _APEX_642CL_GLOBAL_SWCFG_TAG
{
   struct _asApex642clGlobalSwcfgBitFieldTag
   {
      REG32  Swdefds              : 32; //  31:00  APEX_642CL_GLOBAL_SWCFG_BIT_SWDEFDS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_GLOBAL_SWCFG_TAG                                       APEX_642CL_GLOBAL_SWCFG_DCR;

extern volatile APEX_642CL_GLOBAL_SWCFG_DCR                                      gApex642clGlobalSwcfg;
extern volatile APEX_642CL_GLOBAL_SWCFG_DCR*                                     gpApex642clGlobalSwcfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_GLOBAL_SWCFG_BIT_SWDEFDS                                      0

#endif /* APEX642CLGLOBALMAPTYPE_H */
