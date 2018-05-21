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
#ifndef APEX642CLSTREAMDMAMAPTYPE_H
#define APEX642CLSTREAMDMAMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ISR
//    APEX_642CL_STREAM_DMA_ISR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_ISR_BASE_ADDRESS + APEX_642CL_STREAM_DMA_ISR_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Source Register
//    
//    The interrupt source register contains the raw unmasked interrupt and can be used
//    for polling purposes (instead of the external interrupt pin) or for determining
//    which interrupt(s) have caused the external interrupt pin to assert
//    
//    
// 
// Device Control Register Bits Description:
//    OUT                  [00]      Asserted when the stream transfer is complete
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_1           [07:02]   Reserved 
//    IN                   [08]      Asserted when the stream transfer is complete
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    START                [12]      This bit is set to indicate that the channel has started. The shadow registers are
//                                   now in use so the next DMA can be configured.
//                                    
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_3           [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_ISR                                                (APEX_642CL_STREAM_DMA_ISR_BASE_ADDRESS + APEX_642CL_STREAM_DMA_ISR_OFFSET)
#define APEX_642CL_STREAM_DMA_ISR_BASE_ADDRESS                                   (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_ISR_OFFSET                                         (0x00000000)
#define APEX_642CL_STREAM_DMA_ISR_DEFAULT                                        (0x00000000)


union _APEX_642CL_STREAM_DMA_ISR_TAG
{
   struct _asApex642clStreamDmaIsrBitFieldTag
   {
      REG32  Out0                 :  1; //  0      APEX_642CL_STREAM_DMA_ISR_BIT_OUT_0
      REG32  Out1                 :  1; //  1      APEX_642CL_STREAM_DMA_ISR_BIT_OUT_1
      REG32  Reserved1            :  6; //  07:02  APEX_642CL_STREAM_DMA_ISR_BIT_RESERVED_1
      REG32  In0                  :  1; //  8      APEX_642CL_STREAM_DMA_ISR_BIT_IN_0
      REG32  In1                  :  1; //  9      APEX_642CL_STREAM_DMA_ISR_BIT_IN_1
      REG32  In2                  :  1; //  10     APEX_642CL_STREAM_DMA_ISR_BIT_IN_2
      REG32  In3                  :  1; //  11     APEX_642CL_STREAM_DMA_ISR_BIT_IN_3
      REG32  Start0               :  1; //  12     APEX_642CL_STREAM_DMA_ISR_BIT_START_0
      REG32  Start1               :  1; //  13     APEX_642CL_STREAM_DMA_ISR_BIT_START_1
      REG32  Start2               :  1; //  14     APEX_642CL_STREAM_DMA_ISR_BIT_START_2
      REG32  Start3               :  1; //  15     APEX_642CL_STREAM_DMA_ISR_BIT_START_3
      REG32  Reserved3            : 16; //  31:16  APEX_642CL_STREAM_DMA_ISR_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_ISR_TAG                                     APEX_642CL_STREAM_DMA_ISR_DCR;

extern volatile APEX_642CL_STREAM_DMA_ISR_DCR                                    gApex642clStreamDmaIsr;
extern volatile APEX_642CL_STREAM_DMA_ISR_DCR*                                   gpApex642clStreamDmaIsr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_ISR_BIT_OUT_0                                      0
#define APEX_642CL_STREAM_DMA_ISR_BIT_OUT_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ISR_BIT_OUT_ENUM                                     
{
   APEX_642CL_STREAM_DMA_ISR_BIT_OUT_ID_ACTIVE                                   =  1,          // Stream done ACTIVE
   APEX_642CL_STREAM_DMA_ISR_BIT_OUT_ID_INACTIVE                                 =  0           // Stream done INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ISR_BIT_RESERVED_1                                 2
#define APEX_642CL_STREAM_DMA_ISR_BIT_IN_0                                       8
#define APEX_642CL_STREAM_DMA_ISR_BIT_IN_1                                       9
#define APEX_642CL_STREAM_DMA_ISR_BIT_IN_2                                       10
#define APEX_642CL_STREAM_DMA_ISR_BIT_IN_3                                       11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ISR_BIT_IN_ENUM                                      
{
   APEX_642CL_STREAM_DMA_ISR_BIT_IN_ID_ACTIVE                                    =  1,          // Stream done ACTIVE
   APEX_642CL_STREAM_DMA_ISR_BIT_IN_ID_INACTIVE                                  =  0           // Stream done INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ISR_BIT_START_0                                    12
#define APEX_642CL_STREAM_DMA_ISR_BIT_START_1                                    13
#define APEX_642CL_STREAM_DMA_ISR_BIT_START_2                                    14
#define APEX_642CL_STREAM_DMA_ISR_BIT_START_3                                    15

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ISR_BIT_START_ENUM                                   
{
   APEX_642CL_STREAM_DMA_ISR_BIT_START_ID_ACTIVE                                 =  1,          // Stream start ACTIVE
   APEX_642CL_STREAM_DMA_ISR_BIT_START_ID_INACTIVE                               =  0           // Stream start INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ISR_BIT_RESERVED_3                                 16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IMR
//    APEX_642CL_STREAM_DMA_IMR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_IMR_BASE_ADDRESS + APEX_642CL_STREAM_DMA_IMR_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000004
//    Default:      32'h0000ff03
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Mask Register
//    
//    The interrupt mask register provides a mechanism to individually mask one or more
//    of the DMA engine interrupt sources
//    
// 
// Device Control Register Bits Description:
//    OUT                  [00]      Mask the interrupt event
//                                   
//                                   0: Interrupt is not mask
//                                   1: Interrupt is mask
//                                    
//    RESERVED_1           [07:02]   Reserved 
//    IN                   [08]      Mask the interrupt event
//                                   
//                                   0: Interrupt is not mask
//                                   1: Interrupt is mask
//                                    
//    START                [12]      Mask the interrupt event
//                                   
//                                   0: Interrupt is not mask
//                                   1: Interrupt is mask
//                                    
//    RESERVED_3           [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_IMR                                                (APEX_642CL_STREAM_DMA_IMR_BASE_ADDRESS + APEX_642CL_STREAM_DMA_IMR_OFFSET)
#define APEX_642CL_STREAM_DMA_IMR_BASE_ADDRESS                                   (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_IMR_OFFSET                                         (0x00000004)
#define APEX_642CL_STREAM_DMA_IMR_DEFAULT                                        (0x0000ff03)


union _APEX_642CL_STREAM_DMA_IMR_TAG
{
   struct _asApex642clStreamDmaImrBitFieldTag
   {
      REG32  Out0                 :  1; //  0      APEX_642CL_STREAM_DMA_IMR_BIT_OUT_0
      REG32  Out1                 :  1; //  1      APEX_642CL_STREAM_DMA_IMR_BIT_OUT_1
      REG32  Reserved1            :  6; //  07:02  APEX_642CL_STREAM_DMA_IMR_BIT_RESERVED_1
      REG32  In0                  :  1; //  8      APEX_642CL_STREAM_DMA_IMR_BIT_IN_0
      REG32  In1                  :  1; //  9      APEX_642CL_STREAM_DMA_IMR_BIT_IN_1
      REG32  In2                  :  1; //  10     APEX_642CL_STREAM_DMA_IMR_BIT_IN_2
      REG32  In3                  :  1; //  11     APEX_642CL_STREAM_DMA_IMR_BIT_IN_3
      REG32  Start0               :  1; //  12     APEX_642CL_STREAM_DMA_IMR_BIT_START_0
      REG32  Start1               :  1; //  13     APEX_642CL_STREAM_DMA_IMR_BIT_START_1
      REG32  Start2               :  1; //  14     APEX_642CL_STREAM_DMA_IMR_BIT_START_2
      REG32  Start3               :  1; //  15     APEX_642CL_STREAM_DMA_IMR_BIT_START_3
      REG32  Reserved3            : 16; //  31:16  APEX_642CL_STREAM_DMA_IMR_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_IMR_TAG                                     APEX_642CL_STREAM_DMA_IMR_DCR;

extern volatile APEX_642CL_STREAM_DMA_IMR_DCR                                    gApex642clStreamDmaImr;
extern volatile APEX_642CL_STREAM_DMA_IMR_DCR*                                   gpApex642clStreamDmaImr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_IMR_BIT_OUT_0                                      0
#define APEX_642CL_STREAM_DMA_IMR_BIT_OUT_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_IMR_BIT_OUT_ENUM                                     
{
   APEX_642CL_STREAM_DMA_IMR_BIT_OUT_ID_ACTIVE                                   =  1,          // Mask is ACTIVE
   APEX_642CL_STREAM_DMA_IMR_BIT_OUT_ID_INACTIVE                                 =  0           // Mask is INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_IMR_BIT_RESERVED_1                                 2
#define APEX_642CL_STREAM_DMA_IMR_BIT_IN_0                                       8
#define APEX_642CL_STREAM_DMA_IMR_BIT_IN_1                                       9
#define APEX_642CL_STREAM_DMA_IMR_BIT_IN_2                                       10
#define APEX_642CL_STREAM_DMA_IMR_BIT_IN_3                                       11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_IMR_BIT_IN_ENUM                                      
{
   APEX_642CL_STREAM_DMA_IMR_BIT_IN_ID_ACTIVE                                    =  1,          // Mask is ACTIVE
   APEX_642CL_STREAM_DMA_IMR_BIT_IN_ID_INACTIVE                                  =  0           // Mask is INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_IMR_BIT_START_0                                    12
#define APEX_642CL_STREAM_DMA_IMR_BIT_START_1                                    13
#define APEX_642CL_STREAM_DMA_IMR_BIT_START_2                                    14
#define APEX_642CL_STREAM_DMA_IMR_BIT_START_3                                    15

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_IMR_BIT_START_ENUM                                   
{
   APEX_642CL_STREAM_DMA_IMR_BIT_START_ID_ACTIVE                                 =  1,          // Mask is ACTIVE
   APEX_642CL_STREAM_DMA_IMR_BIT_START_ID_INACTIVE                               =  0           // Mask is INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_IMR_BIT_RESERVED_3                                 16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ICR
//    APEX_642CL_STREAM_DMA_ICR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_ICR_BASE_ADDRESS + APEX_642CL_STREAM_DMA_ICR_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rwc1
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Clear Register
//    
//    The interrupt clear register provides the mechanism for clearing the raw interrupt
//    sources. Writing a "1" to the interrupt bit location will clear the interrupt
//    
// 
// Device Control Register Bits Description:
//    OUT                  [00]      Clears the interrupt event
//                                   
//                                   0: no action
//                                   1: Interrupt is cleared
//                                    
//    RESERVED_1           [07:02]   Reserved 
//    IN                   [08]      Clears the interrupt event
//                                   
//                                   0: no action
//                                   1: Interrupt is cleared
//                                    
//    START                [12]      Clears the interrupt event
//                                   
//                                   0: no action
//                                   1: Interrupt is cleared
//                                    
//    RESERVED_3           [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_ICR                                                (APEX_642CL_STREAM_DMA_ICR_BASE_ADDRESS + APEX_642CL_STREAM_DMA_ICR_OFFSET)
#define APEX_642CL_STREAM_DMA_ICR_BASE_ADDRESS                                   (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_ICR_OFFSET                                         (0x00000008)
#define APEX_642CL_STREAM_DMA_ICR_DEFAULT                                        (0x00000000)


union _APEX_642CL_STREAM_DMA_ICR_TAG
{
   struct _asApex642clStreamDmaIcrBitFieldTag
   {
      REG32  Out0                 :  1; //  0      APEX_642CL_STREAM_DMA_ICR_BIT_OUT_0
      REG32  Out1                 :  1; //  1      APEX_642CL_STREAM_DMA_ICR_BIT_OUT_1
      REG32  Reserved1            :  6; //  07:02  APEX_642CL_STREAM_DMA_ICR_BIT_RESERVED_1
      REG32  In0                  :  1; //  8      APEX_642CL_STREAM_DMA_ICR_BIT_IN_0
      REG32  In1                  :  1; //  9      APEX_642CL_STREAM_DMA_ICR_BIT_IN_1
      REG32  In2                  :  1; //  10     APEX_642CL_STREAM_DMA_ICR_BIT_IN_2
      REG32  In3                  :  1; //  11     APEX_642CL_STREAM_DMA_ICR_BIT_IN_3
      REG32  Start0               :  1; //  12     APEX_642CL_STREAM_DMA_ICR_BIT_START_0
      REG32  Start1               :  1; //  13     APEX_642CL_STREAM_DMA_ICR_BIT_START_1
      REG32  Start2               :  1; //  14     APEX_642CL_STREAM_DMA_ICR_BIT_START_2
      REG32  Start3               :  1; //  15     APEX_642CL_STREAM_DMA_ICR_BIT_START_3
      REG32  Reserved3            : 16; //  31:16  APEX_642CL_STREAM_DMA_ICR_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_ICR_TAG                                     APEX_642CL_STREAM_DMA_ICR_DCR;

extern volatile APEX_642CL_STREAM_DMA_ICR_DCR                                    gApex642clStreamDmaIcr;
extern volatile APEX_642CL_STREAM_DMA_ICR_DCR*                                   gpApex642clStreamDmaIcr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_ICR_BIT_OUT_0                                      0
#define APEX_642CL_STREAM_DMA_ICR_BIT_OUT_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ICR_BIT_OUT_ENUM                                     
{
   APEX_642CL_STREAM_DMA_ICR_BIT_OUT_ID_ACTIVE                                   =  1,          // Clear is ACTIVE
   APEX_642CL_STREAM_DMA_ICR_BIT_OUT_ID_INACTIVE                                 =  0           // Clear is INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ICR_BIT_RESERVED_1                                 2
#define APEX_642CL_STREAM_DMA_ICR_BIT_IN_0                                       8
#define APEX_642CL_STREAM_DMA_ICR_BIT_IN_1                                       9
#define APEX_642CL_STREAM_DMA_ICR_BIT_IN_2                                       10
#define APEX_642CL_STREAM_DMA_ICR_BIT_IN_3                                       11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ICR_BIT_IN_ENUM                                      
{
   APEX_642CL_STREAM_DMA_ICR_BIT_IN_ID_ACTIVE                                    =  1           // Clear is ACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ICR_BIT_START_0                                    12
#define APEX_642CL_STREAM_DMA_ICR_BIT_START_1                                    13
#define APEX_642CL_STREAM_DMA_ICR_BIT_START_2                                    14
#define APEX_642CL_STREAM_DMA_ICR_BIT_START_3                                    15

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ICR_BIT_START_ENUM                                   
{
   APEX_642CL_STREAM_DMA_ICR_BIT_START_ID_ACTIVE                                 =  1,          // Clear is ACTIVE
   APEX_642CL_STREAM_DMA_ICR_BIT_START_ID_INACTIVE                               =  0           // Clear is INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ICR_BIT_RESERVED_3                                 16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENABLE
//    APEX_642CL_STREAM_DMA_ENABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_ENABLE_BASE_ADDRESS + APEX_642CL_STREAM_DMA_ENABLE_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Stream Enable Register.
//    
//    The stream enable register is used to enable the DMA. When written "1" the DMA is
//    active and will move data. Once the channel is enabled it cannot be disabled
//    through this register (Writing a "0" to any bit location has no effect). The
//    register, when read, reflects whether the DMA is active or not. The DMA will
//    become inactive when the channel is complete or if the channel is cleared and
//    reset via the "channel clear" register 
//    
// 
// Device Control Register Bits Description:
//    OUT                  [00]      Stream enable. 
//                                   
//                                   0: nothing
//                                   1: Enable channel
//                                    
//    RESERVED_1           [07:02]   Reserved 
//    IN                   [08]      Stream enable. 
//                                   
//                                   0: nothing
//                                   1: Enable channel
//                                    
//    START                [12]      When the bit is set, the next transfer is set to start after the completion of the
//                                   current transfer. If the streamINx_frame_start interrupt has occurred, since the
//                                   shadow registers are being used, the next DMA can be configured and kicked while
//                                   the current DMA is progressing
//                                   
//                                   0: nothing
//                                   1: Enable channel
//                                    
//    RESERVED_3           [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_ENABLE                                             (APEX_642CL_STREAM_DMA_ENABLE_BASE_ADDRESS + APEX_642CL_STREAM_DMA_ENABLE_OFFSET)
#define APEX_642CL_STREAM_DMA_ENABLE_BASE_ADDRESS                                (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_ENABLE_OFFSET                                      (0x0000000c)
#define APEX_642CL_STREAM_DMA_ENABLE_DEFAULT                                     (0x00000000)


union _APEX_642CL_STREAM_DMA_ENABLE_TAG
{
   struct _asApex642clStreamDmaEnableBitFieldTag
   {
      REG32  Out0                 :  1; //  0      APEX_642CL_STREAM_DMA_ENABLE_BIT_OUT_0
      REG32  Out1                 :  1; //  1      APEX_642CL_STREAM_DMA_ENABLE_BIT_OUT_1
      REG32  Reserved1            :  6; //  07:02  APEX_642CL_STREAM_DMA_ENABLE_BIT_RESERVED_1
      REG32  In0                  :  1; //  8      APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_0
      REG32  In1                  :  1; //  9      APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_1
      REG32  In2                  :  1; //  10     APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_2
      REG32  In3                  :  1; //  11     APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_3
      REG32  Start0               :  1; //  12     APEX_642CL_STREAM_DMA_ENABLE_BIT_START_0
      REG32  Start1               :  1; //  13     APEX_642CL_STREAM_DMA_ENABLE_BIT_START_1
      REG32  Start2               :  1; //  14     APEX_642CL_STREAM_DMA_ENABLE_BIT_START_2
      REG32  Start3               :  1; //  15     APEX_642CL_STREAM_DMA_ENABLE_BIT_START_3
      REG32  Reserved3            : 16; //  31:16  APEX_642CL_STREAM_DMA_ENABLE_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_ENABLE_TAG                                  APEX_642CL_STREAM_DMA_ENABLE_DCR;

extern volatile APEX_642CL_STREAM_DMA_ENABLE_DCR                                 gApex642clStreamDmaEnable;
extern volatile APEX_642CL_STREAM_DMA_ENABLE_DCR*                                gpApex642clStreamDmaEnable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_OUT_0                                   0
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_OUT_1                                   1

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ENABLE_BIT_OUT_ENUM                                  
{
   APEX_642CL_STREAM_DMA_ENABLE_BIT_OUT_ID_ACTIVE                                =  1,          // Enable DMA channel ACTIVE
   APEX_642CL_STREAM_DMA_ENABLE_BIT_OUT_ID_INACTIVE                              =  0           // Enable DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ENABLE_BIT_RESERVED_1                              2
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_0                                    8
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_1                                    9
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_2                                    10
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_3                                    11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_ENUM                                   
{
   APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_ID_ACTIVE                                 =  1,          // Enable DMA channel ACTIVE
   APEX_642CL_STREAM_DMA_ENABLE_BIT_IN_ID_INACTIVE                               =  0           // Enable DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ENABLE_BIT_START_0                                 12
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_START_1                                 13
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_START_2                                 14
#define APEX_642CL_STREAM_DMA_ENABLE_BIT_START_3                                 15

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_ENABLE_BIT_START_ENUM                                
{
   APEX_642CL_STREAM_DMA_ENABLE_BIT_START_ID_ACTIVE                              =  1,          // Enable DMA channel ACTIVE
   APEX_642CL_STREAM_DMA_ENABLE_BIT_START_ID_INACTIVE                            =  0           // Enable DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_ENABLE_BIT_RESERVED_3                              16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DISABLE
//    APEX_642CL_STREAM_DMA_DISABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_DISABLE_BASE_ADDRESS + APEX_642CL_STREAM_DMA_DISABLE_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Stream Disable Register.
// 
// Device Control Register Bits Description:
//    OUT                  [00]      Stream Disable. 
//                                   
//                                   0: nothing
//                                   1: Disable channel
//                                    
//    RESERVED_1           [07:02]   Reserved 
//    IN                   [08]      Stream Disable. 
//                                   
//                                   0: nothing
//                                   1: Disable channel
//                                    
//    RESERVED_2           [31:12]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_DISABLE                                            (APEX_642CL_STREAM_DMA_DISABLE_BASE_ADDRESS + APEX_642CL_STREAM_DMA_DISABLE_OFFSET)
#define APEX_642CL_STREAM_DMA_DISABLE_BASE_ADDRESS                               (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_DISABLE_OFFSET                                     (0x00000010)
#define APEX_642CL_STREAM_DMA_DISABLE_DEFAULT                                    (0x00000000)


union _APEX_642CL_STREAM_DMA_DISABLE_TAG
{
   struct _asApex642clStreamDmaDisableBitFieldTag
   {
      REG32  Out0                 :  1; //  0      APEX_642CL_STREAM_DMA_DISABLE_BIT_OUT_0
      REG32  Out1                 :  1; //  1      APEX_642CL_STREAM_DMA_DISABLE_BIT_OUT_1
      REG32  Reserved1            :  6; //  07:02  APEX_642CL_STREAM_DMA_DISABLE_BIT_RESERVED_1
      REG32  In0                  :  1; //  8      APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_0
      REG32  In1                  :  1; //  9      APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_1
      REG32  In2                  :  1; //  10     APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_2
      REG32  In3                  :  1; //  11     APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_3
      REG32  Reserved2            : 20; //  31:12  APEX_642CL_STREAM_DMA_DISABLE_BIT_RESERVED_2
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_DISABLE_TAG                                 APEX_642CL_STREAM_DMA_DISABLE_DCR;

extern volatile APEX_642CL_STREAM_DMA_DISABLE_DCR                                gApex642clStreamDmaDisable;
extern volatile APEX_642CL_STREAM_DMA_DISABLE_DCR*                               gpApex642clStreamDmaDisable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_DISABLE_BIT_OUT_0                                  0
#define APEX_642CL_STREAM_DMA_DISABLE_BIT_OUT_1                                  1

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_DISABLE_BIT_OUT_ENUM                                 
{
   APEX_642CL_STREAM_DMA_DISABLE_BIT_OUT_ID_ACTIVE                               =  1,          // Disable DMA channel ACTIVE
   APEX_642CL_STREAM_DMA_DISABLE_BIT_OUT_ID_INACTIVE                             =  0           // Disable DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_DISABLE_BIT_RESERVED_1                             2
#define APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_0                                   8
#define APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_1                                   9
#define APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_2                                   10
#define APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_3                                   11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_ENUM                                  
{
   APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_ID_ACTIVE                                =  1,          // Disable DMA channel ACTIVE
   APEX_642CL_STREAM_DMA_DISABLE_BIT_IN_ID_INACTIVE                              =  0           // Disable DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_DISABLE_BIT_RESERVED_2                             12

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONTROL
//    APEX_642CL_STREAM_DMA_CONTROL
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CONTROL_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CONTROL_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Miscellaneous Control Register.
// 
// Device Control Register Bits Description:
//    CLEAR_OUT            [00]      When this bit is set the stream interface is cleared and put in an idle state.
//                                   Also the fifo is flushed. This bit is NOT self resetting so a "1" and "0" pattern
//                                   must be written.  
//                                   
//                                   0: nothing
//                                   1: Flush channel
//                                    
//    RESERVED_1           [07:02]   Reserved 
//    CLEAR_IN             [08]      When this bit is set the stream interface is cleared and put in an idle state.
//                                   Also the fifo is flushed. This bit is NOT self resetting so a "1" and "0" pattern
//                                   must be written.  
//                                   
//                                   0: nothing
//                                   1: Flush channel
//                                    
//    RESERVED_2           [31:12]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CONTROL                                            (APEX_642CL_STREAM_DMA_CONTROL_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CONTROL_OFFSET)
#define APEX_642CL_STREAM_DMA_CONTROL_BASE_ADDRESS                               (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_CONTROL_OFFSET                                     (0x00000014)
#define APEX_642CL_STREAM_DMA_CONTROL_DEFAULT                                    (0x00000000)


union _APEX_642CL_STREAM_DMA_CONTROL_TAG
{
   struct _asApex642clStreamDmaControlBitFieldTag
   {
      REG32  ClearOut0            :  1; //  0      APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_OUT_0
      REG32  ClearOut1            :  1; //  1      APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_OUT_1
      REG32  Reserved1            :  6; //  07:02  APEX_642CL_STREAM_DMA_CONTROL_BIT_RESERVED_1
      REG32  ClearIn0             :  1; //  8      APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_0
      REG32  ClearIn1             :  1; //  9      APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_1
      REG32  ClearIn2             :  1; //  10     APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_2
      REG32  ClearIn3             :  1; //  11     APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_3
      REG32  Reserved2            : 20; //  31:12  APEX_642CL_STREAM_DMA_CONTROL_BIT_RESERVED_2
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CONTROL_TAG                                 APEX_642CL_STREAM_DMA_CONTROL_DCR;

extern volatile APEX_642CL_STREAM_DMA_CONTROL_DCR                                gApex642clStreamDmaControl;
extern volatile APEX_642CL_STREAM_DMA_CONTROL_DCR*                               gpApex642clStreamDmaControl;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_OUT_0                            0
#define APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_OUT_1                            1

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_OUT_ENUM                           
{
   APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_OUT_ID_ACTIVE                         =  1,          // Flush DMA channel ACTIVE
   APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_OUT_ID_INACTIVE                       =  0           // Flush DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_CONTROL_BIT_RESERVED_1                             2
#define APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_0                             8
#define APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_1                             9
#define APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_2                             10
#define APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_3                             11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_ENUM                            
{
   APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_ID_ACTIVE                          =  1,          // Flush DMA channel ACTIVE
   APEX_642CL_STREAM_DMA_CONTROL_BIT_CLEAR_IN_ID_INACTIVE                        =  0           // Flush DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_STREAM_DMA_CONTROL_BIT_RESERVED_2                             12

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
#define APEX_642CL_STREAM_DMA_CH                                                 (APEX_642CL_STREAM_DMA_CH_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_BASE_ADDRESS                                    (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_CH_OFFSET                                          (0x00000100)
#define APEX_642CL_STREAM_DMA_CH_SIZE                                            (0x00000100)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER_CFG
//    APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Transfer Configuration Register.
//    
//    This register is shadowed and can be used in a pipelined fashion
//    
// 
// Device Control Register Bits Description:
//    CU_START             [05:00]   Index of the first CU on which the line transfer should begin. Previous CUs in the
//                                   same group of CUs are masked. 
//    RESERVED_1           [07:06]   Reserved 
//    BASE_ADDRESS_0       [18:08]   Base address for word0/2. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_2           [19]      Reserved 
//    BASE_ADDRESS_1       [30:20]   Base address for word1/3. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_3           [31]      Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG                                    (APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BASE_ADDRESS                       (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_OFFSET                             (0x00000000)
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_DEFAULT                            (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_TAG
{
   struct _asApex642clStreamDmaChTransferCfgBitFieldTag
   {
      REG32  CuStart              :  6; //  05:00  APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_CU_START
      REG32  Reserved1            :  2; //  07:06  APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_RESERVED_1
      REG32  BaseAddress0         : 11; //  18:08  APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_BASE_ADDRESS_0
      REG32  Reserved2            :  1; //  19     APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_RESERVED_2
      REG32  BaseAddress1         : 11; //  30:20  APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_BASE_ADDRESS_1
      REG32  Reserved3            :  1; //  31     APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_TAG                         APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_DCR                        gApex642clStreamDmaChTransferCfg;
extern volatile APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_DCR*                       gpApex642clStreamDmaChTransferCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_CU_START                       0
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_RESERVED_1                     6
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_BASE_ADDRESS_0                 8
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_RESERVED_2                     19
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_BASE_ADDRESS_1                 20
#define APEX_642CL_STREAM_DMA_CH_TRANSFER_CFG_BIT_RESERVED_3                     31

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ADDRESS_OFFSET
//    APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Address offset Register.
//    
//    This register is shadowed and can be used in a pipelined fashion.
//    
//    Depending on how many bytes_per_CU are desired, the following  table outlines what
//    is achievable. The higher bin_word_size is always desireable since it has better
//    CMEM read/write performance.
//    
//    bin_word_size  16 
//    Bytes_per_CU  2
//    Bytes_per_CU 6,10,14,18,22,26,30
//    
//    bin_word_size  32
//    Bytes_per_CU  4
//    Bytes_per_CU 12,20,28,36,44,52,60
//    
//    bin_word_size  64 
//    Bytes_per_CU  8
//    Bytes_per_CU 16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
//    
//    The second row is the base number where entries_per_bin=0
//    
//    The third row are the other possible options realized by multiplying the base
//    number by (entries_per_bin + 1)
// 
// Device Control Register Bits Description:
//    ENTRIES_PER_BIN      [03:00]   Number of entries per BIN (N-1). 
//    BIN_WORD_SIZE        [05:04]   Size of the entries in the bin.
//                                   
//                                   0 : 16 bit
//                                   1 : 32 bit
//                                   2 : 64 bit
//                                   3 : reserved 
//    NUM_LINES            [15:06]   Total number of stream lines (N-1). 
//    BYTE_PER_LINE        [31:16]   Number of bytes necessary to complete one line. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET                                  (APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BASE_ADDRESS                     (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_OFFSET                           (0x00000004)
#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_DEFAULT                          (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_TAG
{
   struct _asApex642clStreamDmaChAddressOffsetBitFieldTag
   {
      REG32  EntriesPerBin        :  4; //  03:00  APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_ENTRIES_PER_BIN
      REG32  BinWordSize          :  2; //  05:04  APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE
      REG32  NumLines             : 10; //  15:06  APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_NUM_LINES
      REG32  BytePerLine          : 16; //  31:16  APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BYTE_PER_LINE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_TAG                       APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_DCR                      gApex642clStreamDmaChAddressOffset;
extern volatile APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_DCR*                     gpApex642clStreamDmaChAddressOffset;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_ENTRIES_PER_BIN              0
#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE                4

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ENUM             
{
   APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_16_BIT           =  0,          // Size is 16_BIT
   APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_32_BIT           =  1,          // Size is 32_BIT
   APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_64_BIT           =  2           // Size is 64_BIT
};
                                                                             

#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_NUM_LINES                    6
#define APEX_642CL_STREAM_DMA_CH_ADDRESS_OFFSET_BIT_BYTE_PER_LINE                16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_1
//    APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration1 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_INC_0        [10:00]   When used for multiscan, address Increment 0 is the increment to the base address
//                                   for word0/2 (signed value). This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_INC_1        [22:12]   When used for multiscan, address Increment 1 is the increment to the base address
//                                   for word1/3 (signed value). This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    INC_2D               [31:24]   Increment done on every 2D iterations. All words increment by the same value
//                                   (signed value). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1                                 (APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BASE_ADDRESS                    (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_OFFSET                          (0x00000008)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_DEFAULT                         (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_TAG
{
   struct _asApex642clStreamDmaChMultiscanCfg1BitFieldTag
   {
      REG32  AddressInc0          : 11; //  10:00  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_ADDRESS_INC_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_RESERVED_1
      REG32  AddressInc1          : 11; //  22:12  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_ADDRESS_INC_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_RESERVED_2
      REG32  Inc2d                :  8; //  31:24  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_INC_2D
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_TAG                      APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_DCR                     gApex642clStreamDmaChMultiscanCfg1;
extern volatile APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_DCR*                    gpApex642clStreamDmaChMultiscanCfg1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_ADDRESS_INC_0               0
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_RESERVED_1                  11
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_ADDRESS_INC_1               12
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_RESERVED_2                  23
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_1_BIT_INC_2D                      24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_2
//    APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration2 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_FIRST_0      [10:00]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective
//                                   buffer size. This applies to word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_FIRST_1      [22:12]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective buffer size. This applies to word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    COUNT_2D             [31:24]   Number of times a GOC has to be repeated before going to the next one. This allows
//                                   a user to iterate over the same GOC before going to the next one (N-1). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2                                 (APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BASE_ADDRESS                    (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_OFFSET                          (0x0000000c)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_DEFAULT                         (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_TAG
{
   struct _asApex642clStreamDmaChMultiscanCfg2BitFieldTag
   {
      REG32  AddressFirst0        : 11; //  10:00  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_RESERVED_1
      REG32  AddressFirst1        : 11; //  22:12  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_RESERVED_2
      REG32  Count2d              :  8; //  31:24  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_COUNT_2D
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_TAG                      APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_DCR                     gApex642clStreamDmaChMultiscanCfg2;
extern volatile APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_DCR*                    gpApex642clStreamDmaChMultiscanCfg2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_0             0
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_RESERVED_1                  11
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_1             12
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_RESERVED_2                  23
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_2_BIT_COUNT_2D                    24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_3
//    APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration3 Register.
// 
// Device Control Register Bits Description:
//    MAX_BUFFER_SIZE_0    [10:00]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    MAX_BUFFER_SIZE_1    [22:12]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    MODE                 [24]      0 : Mode A. Max buffer size test is applied when the start GOC is detected.
//                                   1 : Mode B. Max buffer size test is applied when the start of a new line is
//                                       detected.  
//    SCAN_ORDER           [25]      Stream direction support
//                                   
//                                   0 : GOC increments
//                                   1 : GOC decrements 
//    RESERVED_3           [31:26]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3                                 (APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BASE_ADDRESS                    (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_OFFSET                          (0x00000010)
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_DEFAULT                         (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_TAG
{
   struct _asApex642clStreamDmaChMultiscanCfg3BitFieldTag
   {
      REG32  MaxBufferSize0       : 11; //  10:00  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_RESERVED_1
      REG32  MaxBufferSize1       : 11; //  22:12  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_RESERVED_2
      REG32  Mode                 :  1; //  24     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MODE
      REG32  ScanOrder            :  1; //  25     APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_SCAN_ORDER
      REG32  Reserved3            :  6; //  31:26  APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_TAG                      APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_DCR                     gApex642clStreamDmaChMultiscanCfg3;
extern volatile APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_DCR*                    gpApex642clStreamDmaChMultiscanCfg3;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_0           0
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_RESERVED_1                  11
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_1           12
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_RESERVED_2                  23
#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MODE                        24

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MODE_ENUM                     
{
   APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MODE_ID_MODE_A                   =  0,          // Multiscan is MODE_A
   APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_MODE_ID_MODE_B                   =  1           // Multiscan is MODE_B
};
                                                                             

#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_SCAN_ORDER                  25

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ENUM               
{
   APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ID_INC_GOC            =  0,          // GOC is INC_GOC
   APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ID_DEC_GOC            =  1           // GOC is DEC_GOC
};
                                                                             

#define APEX_642CL_STREAM_DMA_CH_MULTISCAN_CFG_3_BIT_RESERVED_3                  26

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD_CFG
//    APEX_642CL_STREAM_DMA_CH_WORD_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_WORD_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_WORD_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Word 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    BASE_ADDR_OFFSET     [10:00]   Offset from the base address for each word. See the address generation section for
//                                   more detailed description of how addressed are generated. This is a 16 bit
//                                   address.
//                                    
//    SLICE_INCREMENT      [15:11]   signed increment to the base address for each written slice. This is a 16 bit
//                                   address. 
//    LINE_INCREMENT       [26:16]   signed increment to the base address.  This is a 16 bit address. 
//    RESERVED_1           [27]      Reserved 
//    LINE_MODE            [29:28]   The stream can be configured to create YUV420 from YUV422.
//                                   
//                                   This is done by dropping the chrominance components in either the odd or even
//                                   lines. To accomplish this word 1 and word 3 should be configured with the line
//                                   mode set to either "odd line" or "even line". For all other applications and
//                                   words, the line mode is set to "both lines".
//                                   
//                                   00: both lines
//                                   01: even line
//                                   10: odd line
//                                   11: reserved
//                                    
//    ENABLE               [30]      Word enable 
//    SWAP                 [31]      0 : byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0
//                                   1 :  byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_0                                      (APEX_642CL_STREAM_DMA_CH_WORD_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_WORD_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_0_BASE_ADDRESS                         (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_0_OFFSET                               (0x00000014)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_0_DEFAULT                              (0x00000000)

#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_1                                      (APEX_642CL_STREAM_DMA_CH_WORD_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_WORD_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_1_BASE_ADDRESS                         (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_1_OFFSET                               (0x00000018)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_1_DEFAULT                              (0x00000000)

#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_2                                      (APEX_642CL_STREAM_DMA_CH_WORD_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_WORD_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_2_BASE_ADDRESS                         (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_2_OFFSET                               (0x0000001c)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_2_DEFAULT                              (0x00000000)

#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_3                                      (APEX_642CL_STREAM_DMA_CH_WORD_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_WORD_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_3_BASE_ADDRESS                         (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_3_OFFSET                               (0x00000020)
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_3_DEFAULT                              (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_WORD_CFG_TAG
{
   struct _asApex642clStreamDmaChWordCfgBitFieldTag
   {
      REG32  BaseAddrOffset       : 11; //  10:00  APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_BASE_ADDR_OFFSET
      REG32  SliceIncrement       :  5; //  15:11  APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT
      REG32  LineIncrement        : 11; //  26:16  APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_INCREMENT
      REG32  Reserved1            :  1; //  27     APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_RESERVED_1
      REG32  LineMode             :  2; //  29:28  APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_MODE
      REG32  Enable               :  1; //  30     APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_ENABLE
      REG32  Swap                 :  1; //  31     APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SWAP
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_WORD_CFG_TAG                             APEX_642CL_STREAM_DMA_CH_WORD_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_WORD_CFG_DCR                            gApex642clStreamDmaChWordCfg;
extern volatile APEX_642CL_STREAM_DMA_CH_WORD_CFG_DCR*                           gpApex642clStreamDmaChWordCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_BASE_ADDR_OFFSET                   0
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT                    11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT_ENUM                 
{
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT_ID_ZERO                 =  0,          // Slice increment ZERO
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT_ID_ONE                  =  1,          // Slice increment ONE
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT_ID_TWO                  =  2,          // Slice increment TWO
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT_ID_FOUR                 =  4,          // Slice increment FOUR
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT_ID_EIGHT                =  8,          // Slice increment EIGHT
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SLICE_INCREMENT_ID_MINUS_FOUR           =  28          // Slice increment MINUS_FOUR
};
                                                                             

#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_INCREMENT                     16
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_RESERVED_1                         27
#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_MODE                          28

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_MODE_ENUM                       
{
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_MODE_ID_BOTH                       =  0,          // Line Mode is BOTH
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_MODE_ID_EVEN                       =  1,          // Line Mode is EVEN
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_LINE_MODE_ID_ODD                        =  1           // Line Mode is ODD
};
                                                                             

#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_ENABLE                             30

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_ENABLE_ENUM                          
{
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_ENABLE_ID_DISABLE                       =  0,          // Word is DISABLE
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_ENABLE_ID_ENABLE                        =  1           // Word is ENABLE
};
                                                                             

#define APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SWAP                               31

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SWAP_ENUM                            
{
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SWAP_ID_NO_SWAP                         =  0,          // Word is NO_SWAP
   APEX_642CL_STREAM_DMA_CH_WORD_CFG_BIT_SWAP_ID_SWAP                            =  1           // Word is SWAP
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_CFG
//    APEX_642CL_STREAM_DMA_CH_CU_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_CU_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    CU Configuration Register.
// 
// Device Control Register Bits Description:
//    BYTES_PER_CU         [07:00]   Bytes per CU (N-1). This must be a multiple of the bin_word_size. 
//    RESERVED_1           [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_CU_CFG                                          (APEX_642CL_STREAM_DMA_CH_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_CU_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_CU_CFG_BASE_ADDRESS                             (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_CU_CFG_OFFSET                                   (0x00000024)
#define APEX_642CL_STREAM_DMA_CH_CU_CFG_DEFAULT                                  (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_CU_CFG_TAG
{
   struct _asApex642clStreamDmaChCuCfgBitFieldTag
   {
      REG32  BytesPerCu           :  8; //  07:00  APEX_642CL_STREAM_DMA_CH_CU_CFG_BIT_BYTES_PER_CU
      REG32  Reserved1            : 24; //  31:08  APEX_642CL_STREAM_DMA_CH_CU_CFG_BIT_RESERVED_1
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_CU_CFG_TAG                               APEX_642CL_STREAM_DMA_CH_CU_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_CU_CFG_DCR                              gApex642clStreamDmaChCuCfg;
extern volatile APEX_642CL_STREAM_DMA_CH_CU_CFG_DCR*                             gpApex642clStreamDmaChCuCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_CU_CFG_BIT_BYTES_PER_CU                         0
#define APEX_642CL_STREAM_DMA_CH_CU_CFG_BIT_RESERVED_1                           8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MUXSEL_CFG
//    APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Mux Select 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    WORD_0_B             [03:00]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_1_B             [11:08]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_2_B             [19:16]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_3_B             [27:24]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_0                                    (APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_0_BASE_ADDRESS                       (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_0_OFFSET                             (0x00000028)
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_0_DEFAULT                            (0x00000000)

#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_1                                    (APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_1_BASE_ADDRESS                       (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_1_OFFSET                             (0x0000002c)
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_1_DEFAULT                            (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_TAG
{
   struct _asApex642clStreamDmaChMuxselCfgBitFieldTag
   {
      REG32  Word0B0              :  4; //  3:0    APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_0_B_0
      REG32  Word0B1              :  4; //  7:4    APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_0_B_1
      REG32  Word1B0              :  4; //  11:8   APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_1_B_0
      REG32  Word1B1              :  4; //  15:12  APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_1_B_1
      REG32  Word2B0              :  4; //  19:16  APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_2_B_0
      REG32  Word2B1              :  4; //  23:20  APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_2_B_1
      REG32  Word3B0              :  4; //  27:24  APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_3_B_0
      REG32  Word3B1              :  4; //  31:28  APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_3_B_1
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_TAG                           APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_DCR                          gApex642clStreamDmaChMuxselCfg;
extern volatile APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_DCR*                         gpApex642clStreamDmaChMuxselCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_0_B_0                       0
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_0_B_1                       4
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_1_B_0                       8
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_1_B_1                       12
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_2_B_0                       16
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_2_B_1                       20
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_3_B_0                       24
#define APEX_642CL_STREAM_DMA_CH_MUXSEL_CFG_BIT_WORD_3_B_1                       28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_SPAN
//    APEX_642CL_STREAM_DMA_CH_CU_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_CU_SPAN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_CU_SPAN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    CU Span 0 Register.
// 
// Device Control Register Bits Description:
//    ENABLE               [31:00]   Enable for CU 0 - 31
//                                   
//                                   0 : disable
//                                   1 : enable 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_0                                       (APEX_642CL_STREAM_DMA_CH_CU_SPAN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_CU_SPAN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_0_BASE_ADDRESS                          (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_0_OFFSET                                (0x00000030)
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_0_DEFAULT                               (0x00000000)

#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_1                                       (APEX_642CL_STREAM_DMA_CH_CU_SPAN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_CU_SPAN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_1_BASE_ADDRESS                          (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_1_OFFSET                                (0x00000034)
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_1_DEFAULT                               (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_CU_SPAN_TAG
{
   struct _asApex642clStreamDmaChCuSpanBitFieldTag
   {
      REG32  Enable               : 32; //  31:00  APEX_642CL_STREAM_DMA_CH_CU_SPAN_BIT_ENABLE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_CU_SPAN_TAG                              APEX_642CL_STREAM_DMA_CH_CU_SPAN_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_CU_SPAN_DCR                             gApex642clStreamDmaChCuSpan;
extern volatile APEX_642CL_STREAM_DMA_CH_CU_SPAN_DCR*                            gpApex642clStreamDmaChCuSpan;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_CU_SPAN_BIT_ENABLE                              0

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_CH_CU_SPAN_BIT_ENABLE_ENUM                           
{
   APEX_642CL_STREAM_DMA_CH_CU_SPAN_BIT_ENABLE_ID_DISABLE                        =  0,          // CU is DISABLE
   APEX_642CL_STREAM_DMA_CH_CU_SPAN_BIT_ENABLE_ID_ENABLE                         =  1           // CU is ENABLE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_1
//    APEX_642CL_STREAM_DMA_CH_STATUS_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_STATUS_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 1 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    BYTE_CNT             [15:00]   Current line byte count value. 
//    LINE_CNT             [25:16]   Current line count value (N-1) 
//    RESERVED_1           [27:26]   Reserved 
//    FIFO_CS              [31:28]   fifo state machine.
//                                   
//                                   Stream IN:
//                                      parameter     idle_st          = 4'd0;
//                                      parameter     new_cu_st = 4'd1;
//                                      parameter     cu0_st         = 4'd2;
//                                      parameter     cu1_st          = 4'd3;
//                                      parameter     cu2_st         = 4'd4;
//                                      parameter     cu3_st         = 4'd5;
//                                      parameter     wt_st            = 4'd6;
//                                      parameter     wt1_st           = 4'd7;
//                                      parameter     done_wt      = 4'd8;
//                                   
//                                   Stream OUT:
//                                   
//                                      parameter     idle_st          = 3'd0;
//                                      parameter     new_cu_st = 3'd1;
//                                      parameter     pg_st            = 3'd2;
//                                      parameter     pg_wt            = 3'd3;
//                                      parameter     done_wt       = 3'd4;
//                                      parameter     pg_wt1           = 3'd5;
//                                      parameter     pg_wt2          = 3'd6;
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_STATUS_1                                        (APEX_642CL_STREAM_DMA_CH_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_STATUS_1_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_STATUS_1_BASE_ADDRESS                           (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_STATUS_1_OFFSET                                 (0x00000038)
#define APEX_642CL_STREAM_DMA_CH_STATUS_1_DEFAULT                                (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_STATUS_1_TAG
{
   struct _asApex642clStreamDmaChStatus1BitFieldTag
   {
      REG32  ByteCnt              : 16; //  15:00  APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_BYTE_CNT
      REG32  LineCnt              : 10; //  25:16  APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_LINE_CNT
      REG32  Reserved1            :  2; //  27:26  APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_RESERVED_1
      REG32  FifoCs               :  4; //  31:28  APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_FIFO_CS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_STATUS_1_TAG                             APEX_642CL_STREAM_DMA_CH_STATUS_1_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_STATUS_1_DCR                            gApex642clStreamDmaChStatus1;
extern volatile APEX_642CL_STREAM_DMA_CH_STATUS_1_DCR*                           gpApex642clStreamDmaChStatus1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_BYTE_CNT                           0
#define APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_LINE_CNT                           16
#define APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_RESERVED_1                         26
#define APEX_642CL_STREAM_DMA_CH_STATUS_1_BIT_FIFO_CS                            28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_2
//    APEX_642CL_STREAM_DMA_CH_STATUS_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_CH_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_STATUS_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_CH
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 2 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    FIFO_BYTE_CNT        [09:00]   Current line byte count value. 
//    RESERVED_1           [15:10]   Reserved 
//    RD_CTRL_CS           [18:16]   Read Control State Machine Current State value. 
//    RESERVED_2           [19]      Reserved 
//    WR_CTRL_CS           [22:20]   Write Control State Machine Current State value. 
//    RESERVED_3           [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_STATUS_2                                        (APEX_642CL_STREAM_DMA_CH_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_CH_STATUS_2_OFFSET)
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_BASE_ADDRESS                           (APEX_642CL_STREAM_DMA_CH)
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_OFFSET                                 (0x0000003c)
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_DEFAULT                                (0x00000000)


union _APEX_642CL_STREAM_DMA_CH_STATUS_2_TAG
{
   struct _asApex642clStreamDmaChStatus2BitFieldTag
   {
      REG32  FifoByteCnt          : 10; //  09:00  APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_FIFO_BYTE_CNT
      REG32  Reserved1            :  6; //  15:10  APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RESERVED_1
      REG32  RdCtrlCs             :  3; //  18:16  APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RD_CTRL_CS
      REG32  Reserved2            :  1; //  19     APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RESERVED_2
      REG32  WrCtrlCs             :  3; //  22:20  APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_WR_CTRL_CS
      REG32  Reserved3            :  8; //  31:24  APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_CH_STATUS_2_TAG                             APEX_642CL_STREAM_DMA_CH_STATUS_2_DCR;

extern volatile APEX_642CL_STREAM_DMA_CH_STATUS_2_DCR                            gApex642clStreamDmaChStatus2;
extern volatile APEX_642CL_STREAM_DMA_CH_STATUS_2_DCR*                           gpApex642clStreamDmaChStatus2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_FIFO_BYTE_CNT                      0
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RESERVED_1                         10
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RD_CTRL_CS                         16
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RESERVED_2                         19
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_WR_CTRL_CS                         20
#define APEX_642CL_STREAM_DMA_CH_STATUS_2_BIT_RESERVED_3                         24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    STREAM_OUT
//    APEX_642CL_STREAM_DMA_STREAM_OUT
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000100
//    Default:      32'h00000000
//    Nb:         2
//    Size:       256
// 
// Description:
//    Stream Out Channel
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0                                       (APEX_642CL_STREAM_DMA_STREAM_OUT_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_BASE_ADDRESS                          (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_OFFSET                                (0x00000100)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_SIZE                                  (0x00000100)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_1                                       (APEX_642CL_STREAM_DMA_STREAM_OUT_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_BASE_ADDRESS                          (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_OFFSET                                (0x00000200)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_SIZE                                  (0x00000100)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Transfer Configuration Register.
//    
//    This register is shadowed and can be used in a pipelined fashion
//    
// 
// Device Control Register Bits Description:
//    CU_START             [05:00]   Index of the first CU on which the line transfer should begin. Previous CUs in the
//                                   same group of CUs are masked. 
//    RESERVED_1           [07:06]   Reserved 
//    BASE_ADDRESS_0       [18:08]   Base address for word0/2. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_2           [19]      Reserved 
//    BASE_ADDRESS_1       [30:20]   Base address for word1/3. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_3           [31]      Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_TRANSFER_CFG                          (APEX_642CL_STREAM_DMA_STREAM_OUT_0_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_TRANSFER_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_TRANSFER_CFG_BASE_ADDRESS             (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_TRANSFER_CFG_OFFSET                   (0x00000000)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_TRANSFER_CFG_DEFAULT                  (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ADDRESS_OFFSET
//    APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Address offset Register.
//    
//    This register is shadowed and can be used in a pipelined fashion.
//    
//    Depending on how many bytes_per_CU are desired, the following  table outlines what
//    is achievable. The higher bin_word_size is always desireable since it has better
//    CMEM read/write performance.
//    
//    bin_word_size  16 
//    Bytes_per_CU  2
//    Bytes_per_CU 6,10,14,18,22,26,30
//    
//    bin_word_size  32
//    Bytes_per_CU  4
//    Bytes_per_CU 12,20,28,36,44,52,60
//    
//    bin_word_size  64 
//    Bytes_per_CU  8
//    Bytes_per_CU 16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
//    
//    The second row is the base number where entries_per_bin=0
//    
//    The third row are the other possible options realized by multiplying the base
//    number by (entries_per_bin + 1)
// 
// Device Control Register Bits Description:
//    ENTRIES_PER_BIN      [03:00]   Number of entries per BIN (N-1). 
//    BIN_WORD_SIZE        [05:04]   Size of the entries in the bin.
//                                   
//                                   0 : 16 bit
//                                   1 : 32 bit
//                                   2 : 64 bit
//                                   3 : reserved 
//    NUM_LINES            [15:06]   Total number of stream lines (N-1). 
//    BYTE_PER_LINE        [31:16]   Number of bytes necessary to complete one line. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_ADDRESS_OFFSET                        (APEX_642CL_STREAM_DMA_STREAM_OUT_0_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_ADDRESS_OFFSET_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_ADDRESS_OFFSET_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_ADDRESS_OFFSET_OFFSET                 (0x00000004)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_ADDRESS_OFFSET_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_1
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration1 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_INC_0        [10:00]   When used for multiscan, address Increment 0 is the increment to the base address
//                                   for word0/2 (signed value). This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_INC_1        [22:12]   When used for multiscan, address Increment 1 is the increment to the base address
//                                   for word1/3 (signed value). This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    INC_2D               [31:24]   Increment done on every 2D iterations. All words increment by the same value
//                                   (signed value). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_1                       (APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_1_BASE_ADDRESS          (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_1_OFFSET                (0x00000008)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_1_DEFAULT               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_2
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration2 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_FIRST_0      [10:00]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective
//                                   buffer size. This applies to word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_FIRST_1      [22:12]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective buffer size. This applies to word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    COUNT_2D             [31:24]   Number of times a GOC has to be repeated before going to the next one. This allows
//                                   a user to iterate over the same GOC before going to the next one (N-1). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_2                       (APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_2_BASE_ADDRESS          (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_2_OFFSET                (0x0000000c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_2_DEFAULT               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_3
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration3 Register.
// 
// Device Control Register Bits Description:
//    MAX_BUFFER_SIZE_0    [10:00]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    MAX_BUFFER_SIZE_1    [22:12]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    MODE                 [24]      0 : Mode A. Max buffer size test is applied when the start GOC is detected.
//                                   1 : Mode B. Max buffer size test is applied when the start of a new line is
//                                       detected.  
//    SCAN_ORDER           [25]      Stream direction support
//                                   
//                                   0 : GOC increments
//                                   1 : GOC decrements 
//    RESERVED_3           [31:26]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_3                       (APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_3_BASE_ADDRESS          (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_3_OFFSET                (0x00000010)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MULTISCAN_CFG_3_DEFAULT               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Word 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    BASE_ADDR_OFFSET     [10:00]   Offset from the base address for each word. See the address generation section for
//                                   more detailed description of how addressed are generated. This is a 16 bit
//                                   address.
//                                    
//    SLICE_INCREMENT      [15:11]   signed increment to the base address for each written slice. This is a 16 bit
//                                   address. 
//    LINE_INCREMENT       [26:16]   signed increment to the base address.  This is a 16 bit address. 
//    RESERVED_1           [27]      Reserved 
//    LINE_MODE            [29:28]   The stream can be configured to create YUV420 from YUV422.
//                                   
//                                   This is done by dropping the chrominance components in either the odd or even
//                                   lines. To accomplish this word 1 and word 3 should be configured with the line
//                                   mode set to either "odd line" or "even line". For all other applications and
//                                   words, the line mode is set to "both lines".
//                                   
//                                   00: both lines
//                                   01: even line
//                                   10: odd line
//                                   11: reserved
//                                    
//    ENABLE               [30]      Word enable 
//    SWAP                 [31]      0 : byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0
//                                   1 :  byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_0                            (APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_0_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_0_OFFSET                     (0x00000014)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_0_DEFAULT                    (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_1                            (APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_1_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_1_OFFSET                     (0x00000018)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_1_DEFAULT                    (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_2                            (APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_2_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_2_OFFSET                     (0x0000001c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_2_DEFAULT                    (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_3                            (APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_3_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_3_OFFSET                     (0x00000020)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_WORD_CFG_3_DEFAULT                    (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    CU Configuration Register.
// 
// Device Control Register Bits Description:
//    BYTES_PER_CU         [07:00]   Bytes per CU (N-1). This must be a multiple of the bin_word_size. 
//    RESERVED_1           [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_CFG                                (APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_CFG_BASE_ADDRESS                   (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_CFG_OFFSET                         (0x00000024)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_CFG_DEFAULT                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MUXSEL_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Mux Select 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    WORD_0_B             [03:00]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_1_B             [11:08]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_2_B             [19:16]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_3_B             [27:24]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_0                          (APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_0_BASE_ADDRESS             (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_0_OFFSET                   (0x00000028)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_0_DEFAULT                  (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_1                          (APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_1_BASE_ADDRESS             (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_1_OFFSET                   (0x0000002c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_MUXSEL_CFG_1_DEFAULT                  (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_SPAN
//    APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    CU Span 0 Register.
// 
// Device Control Register Bits Description:
//    ENABLE               [31:00]   Enable for CU 0 - 31
//                                   
//                                   0 : disable
//                                   1 : enable 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_0                             (APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_0_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_0_OFFSET                      (0x00000030)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_0_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_1                             (APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_1_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_1_OFFSET                      (0x00000034)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_CU_SPAN_1_DEFAULT                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_1
//    APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 1 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    BYTE_CNT             [15:00]   Current line byte count value. 
//    LINE_CNT             [25:16]   Current line count value (N-1) 
//    RESERVED_1           [27:26]   Reserved 
//    FIFO_CS              [31:28]   fifo state machine.
//                                   
//                                   Stream IN:
//                                      parameter     idle_st          = 4'd0;
//                                      parameter     new_cu_st = 4'd1;
//                                      parameter     cu0_st         = 4'd2;
//                                      parameter     cu1_st          = 4'd3;
//                                      parameter     cu2_st         = 4'd4;
//                                      parameter     cu3_st         = 4'd5;
//                                      parameter     wt_st            = 4'd6;
//                                      parameter     wt1_st           = 4'd7;
//                                      parameter     done_wt      = 4'd8;
//                                   
//                                   Stream OUT:
//                                   
//                                      parameter     idle_st          = 3'd0;
//                                      parameter     new_cu_st = 3'd1;
//                                      parameter     pg_st            = 3'd2;
//                                      parameter     pg_wt            = 3'd3;
//                                      parameter     done_wt       = 3'd4;
//                                      parameter     pg_wt1           = 3'd5;
//                                      parameter     pg_wt2          = 3'd6;
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_1                              (APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_1_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_1_OFFSET                       (0x00000038)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_1_DEFAULT                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_2
//    APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 2 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    FIFO_BYTE_CNT        [09:00]   Current line byte count value. 
//    RESERVED_1           [15:10]   Reserved 
//    RD_CTRL_CS           [18:16]   Read Control State Machine Current State value. 
//    RESERVED_2           [19]      Reserved 
//    WR_CTRL_CS           [22:20]   Write Control State Machine Current State value. 
//    RESERVED_3           [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_2                              (APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_2_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_OUT_0)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_2_OFFSET                       (0x0000003c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_0_STATUS_2_DEFAULT                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Transfer Configuration Register.
//    
//    This register is shadowed and can be used in a pipelined fashion
//    
// 
// Device Control Register Bits Description:
//    CU_START             [05:00]   Index of the first CU on which the line transfer should begin. Previous CUs in the
//                                   same group of CUs are masked. 
//    RESERVED_1           [07:06]   Reserved 
//    BASE_ADDRESS_0       [18:08]   Base address for word0/2. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_2           [19]      Reserved 
//    BASE_ADDRESS_1       [30:20]   Base address for word1/3. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_3           [31]      Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_TRANSFER_CFG                          (APEX_642CL_STREAM_DMA_STREAM_OUT_1_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_TRANSFER_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_TRANSFER_CFG_BASE_ADDRESS             (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_TRANSFER_CFG_OFFSET                   (0x00000000)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_TRANSFER_CFG_DEFAULT                  (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_TAG
{
   struct _asApex642clStreamDmaStreamOutTransferCfgBitFieldTag
   {
      REG32  CuStart              :  6; //  05:00  APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_CU_START
      REG32  Reserved1            :  2; //  07:06  APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_RESERVED_1
      REG32  BaseAddress0         : 11; //  18:08  APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_BASE_ADDRESS_0
      REG32  Reserved2            :  1; //  19     APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_RESERVED_2
      REG32  BaseAddress1         : 11; //  30:20  APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_BASE_ADDRESS_1
      REG32  Reserved3            :  1; //  31     APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_TAG                 APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_DCR                gApex642clStreamDmaStreamOutTransferCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_DCR*               gpApex642clStreamDmaStreamOutTransferCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_CU_START               0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_RESERVED_1             6
#define APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_BASE_ADDRESS_0         8
#define APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_RESERVED_2             19
#define APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_BASE_ADDRESS_1         20
#define APEX_642CL_STREAM_DMA_STREAM_OUT_TRANSFER_CFG_BIT_RESERVED_3             31

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ADDRESS_OFFSET
//    APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Address offset Register.
//    
//    This register is shadowed and can be used in a pipelined fashion.
//    
//    Depending on how many bytes_per_CU are desired, the following  table outlines what
//    is achievable. The higher bin_word_size is always desireable since it has better
//    CMEM read/write performance.
//    
//    bin_word_size  16 
//    Bytes_per_CU  2
//    Bytes_per_CU 6,10,14,18,22,26,30
//    
//    bin_word_size  32
//    Bytes_per_CU  4
//    Bytes_per_CU 12,20,28,36,44,52,60
//    
//    bin_word_size  64 
//    Bytes_per_CU  8
//    Bytes_per_CU 16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
//    
//    The second row is the base number where entries_per_bin=0
//    
//    The third row are the other possible options realized by multiplying the base
//    number by (entries_per_bin + 1)
// 
// Device Control Register Bits Description:
//    ENTRIES_PER_BIN      [03:00]   Number of entries per BIN (N-1). 
//    BIN_WORD_SIZE        [05:04]   Size of the entries in the bin.
//                                   
//                                   0 : 16 bit
//                                   1 : 32 bit
//                                   2 : 64 bit
//                                   3 : reserved 
//    NUM_LINES            [15:06]   Total number of stream lines (N-1). 
//    BYTE_PER_LINE        [31:16]   Number of bytes necessary to complete one line. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_ADDRESS_OFFSET                        (APEX_642CL_STREAM_DMA_STREAM_OUT_1_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_ADDRESS_OFFSET_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_ADDRESS_OFFSET_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_ADDRESS_OFFSET_OFFSET                 (0x00000004)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_ADDRESS_OFFSET_DEFAULT                (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_TAG
{
   struct _asApex642clStreamDmaStreamOutAddressOffsetBitFieldTag
   {
      REG32  EntriesPerBin        :  4; //  03:00  APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_ENTRIES_PER_BIN
      REG32  BinWordSize          :  2; //  05:04  APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE
      REG32  NumLines             : 10; //  15:06  APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_NUM_LINES
      REG32  BytePerLine          : 16; //  31:16  APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BYTE_PER_LINE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_TAG               APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_DCR              gApex642clStreamDmaStreamOutAddressOffset;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_DCR*             gpApex642clStreamDmaStreamOutAddressOffset;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_ENTRIES_PER_BIN      0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE        4

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ENUM     
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_16_BIT   =  0,          // Size is 16_BIT
   APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_32_BIT   =  1,          // Size is 32_BIT
   APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_64_BIT   =  2           // Size is 64_BIT
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_NUM_LINES            6
#define APEX_642CL_STREAM_DMA_STREAM_OUT_ADDRESS_OFFSET_BIT_BYTE_PER_LINE        16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_1
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration1 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_INC_0        [10:00]   When used for multiscan, address Increment 0 is the increment to the base address
//                                   for word0/2 (signed value). This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_INC_1        [22:12]   When used for multiscan, address Increment 1 is the increment to the base address
//                                   for word1/3 (signed value). This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    INC_2D               [31:24]   Increment done on every 2D iterations. All words increment by the same value
//                                   (signed value). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_1                       (APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_1_BASE_ADDRESS          (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_1_OFFSET                (0x00000008)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_1_DEFAULT               (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_TAG
{
   struct _asApex642clStreamDmaStreamOutMultiscanCfg1BitFieldTag
   {
      REG32  AddressInc0          : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_ADDRESS_INC_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_RESERVED_1
      REG32  AddressInc1          : 11; //  22:12  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_ADDRESS_INC_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_RESERVED_2
      REG32  Inc2d                :  8; //  31:24  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_INC_2D
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_TAG              APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_DCR             gApex642clStreamDmaStreamOutMultiscanCfg1;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_DCR*            gpApex642clStreamDmaStreamOutMultiscanCfg1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_ADDRESS_INC_0       0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_RESERVED_1          11
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_ADDRESS_INC_1       12
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_RESERVED_2          23
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_1_BIT_INC_2D              24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_2
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration2 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_FIRST_0      [10:00]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective
//                                   buffer size. This applies to word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_FIRST_1      [22:12]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective buffer size. This applies to word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    COUNT_2D             [31:24]   Number of times a GOC has to be repeated before going to the next one. This allows
//                                   a user to iterate over the same GOC before going to the next one (N-1). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_2                       (APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_2_BASE_ADDRESS          (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_2_OFFSET                (0x0000000c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_2_DEFAULT               (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_TAG
{
   struct _asApex642clStreamDmaStreamOutMultiscanCfg2BitFieldTag
   {
      REG32  AddressFirst0        : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_RESERVED_1
      REG32  AddressFirst1        : 11; //  22:12  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_RESERVED_2
      REG32  Count2d              :  8; //  31:24  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_COUNT_2D
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_TAG              APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_DCR             gApex642clStreamDmaStreamOutMultiscanCfg2;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_DCR*            gpApex642clStreamDmaStreamOutMultiscanCfg2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_0     0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_RESERVED_1          11
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_1     12
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_RESERVED_2          23
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_2_BIT_COUNT_2D            24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_3
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration3 Register.
// 
// Device Control Register Bits Description:
//    MAX_BUFFER_SIZE_0    [10:00]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    MAX_BUFFER_SIZE_1    [22:12]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    MODE                 [24]      0 : Mode A. Max buffer size test is applied when the start GOC is detected.
//                                   1 : Mode B. Max buffer size test is applied when the start of a new line is
//                                       detected.  
//    SCAN_ORDER           [25]      Stream direction support
//                                   
//                                   0 : GOC increments
//                                   1 : GOC decrements 
//    RESERVED_3           [31:26]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_3                       (APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_3_BASE_ADDRESS          (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_3_OFFSET                (0x00000010)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MULTISCAN_CFG_3_DEFAULT               (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_TAG
{
   struct _asApex642clStreamDmaStreamOutMultiscanCfg3BitFieldTag
   {
      REG32  MaxBufferSize0       : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_RESERVED_1
      REG32  MaxBufferSize1       : 11; //  22:12  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_RESERVED_2
      REG32  Mode                 :  1; //  24     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MODE
      REG32  ScanOrder            :  1; //  25     APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_SCAN_ORDER
      REG32  Reserved3            :  6; //  31:26  APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_TAG              APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_DCR             gApex642clStreamDmaStreamOutMultiscanCfg3;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_DCR*            gpApex642clStreamDmaStreamOutMultiscanCfg3;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_0   0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_RESERVED_1          11
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_1   12
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_RESERVED_2          23
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MODE                24

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MODE_ENUM             
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MODE_ID_MODE_A           =  0,          // Multiscan is MODE_A
   APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_MODE_ID_MODE_B           =  1           // Multiscan is MODE_B
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_SCAN_ORDER          25

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ENUM       
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ID_INC_GOC    =  0,          // GOC is INC_GOC
   APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ID_DEC_GOC    =  1           // GOC is DEC_GOC
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_OUT_MULTISCAN_CFG_3_BIT_RESERVED_3          26

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Word 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    BASE_ADDR_OFFSET     [10:00]   Offset from the base address for each word. See the address generation section for
//                                   more detailed description of how addressed are generated. This is a 16 bit
//                                   address.
//                                    
//    SLICE_INCREMENT      [15:11]   signed increment to the base address for each written slice. This is a 16 bit
//                                   address. 
//    LINE_INCREMENT       [26:16]   signed increment to the base address.  This is a 16 bit address. 
//    RESERVED_1           [27]      Reserved 
//    LINE_MODE            [29:28]   The stream can be configured to create YUV420 from YUV422.
//                                   
//                                   This is done by dropping the chrominance components in either the odd or even
//                                   lines. To accomplish this word 1 and word 3 should be configured with the line
//                                   mode set to either "odd line" or "even line". For all other applications and
//                                   words, the line mode is set to "both lines".
//                                   
//                                   00: both lines
//                                   01: even line
//                                   10: odd line
//                                   11: reserved
//                                    
//    ENABLE               [30]      Word enable 
//    SWAP                 [31]      0 : byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0
//                                   1 :  byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_0                            (APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_0_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_0_OFFSET                     (0x00000014)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_0_DEFAULT                    (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_1                            (APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_1_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_1_OFFSET                     (0x00000018)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_1_DEFAULT                    (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_2                            (APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_2_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_2_OFFSET                     (0x0000001c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_2_DEFAULT                    (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_3                            (APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_3_BASE_ADDRESS               (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_3_OFFSET                     (0x00000020)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_WORD_CFG_3_DEFAULT                    (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_TAG
{
   struct _asApex642clStreamDmaStreamOutWordCfgBitFieldTag
   {
      REG32  BaseAddrOffset       : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_BASE_ADDR_OFFSET
      REG32  SliceIncrement       :  5; //  15:11  APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT
      REG32  LineIncrement        : 11; //  26:16  APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_INCREMENT
      REG32  Reserved1            :  1; //  27     APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_RESERVED_1
      REG32  LineMode             :  2; //  29:28  APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_MODE
      REG32  Enable               :  1; //  30     APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_ENABLE
      REG32  Swap                 :  1; //  31     APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SWAP
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_TAG                     APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_DCR                    gApex642clStreamDmaStreamOutWordCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_DCR*                   gpApex642clStreamDmaStreamOutWordCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_BASE_ADDR_OFFSET           0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT            11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT_ENUM         
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT_ID_ZERO         =  0,          // Slice increment ZERO
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT_ID_ONE          =  1,          // Slice increment ONE
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT_ID_TWO          =  2,          // Slice increment TWO
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT_ID_FOUR         =  4,          // Slice increment FOUR
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT_ID_EIGHT        =  8,          // Slice increment EIGHT
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SLICE_INCREMENT_ID_MINUS_FOUR   =  28          // Slice increment MINUS_FOUR
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_INCREMENT             16
#define APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_RESERVED_1                 27
#define APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_MODE                  28

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_MODE_ENUM               
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_MODE_ID_BOTH               =  0,          // Line Mode is BOTH
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_MODE_ID_EVEN               =  1,          // Line Mode is EVEN
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_LINE_MODE_ID_ODD                =  1           // Line Mode is ODD
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_ENABLE                     30

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_ENABLE_ENUM                  
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_ENABLE_ID_DISABLE               =  0,          // Word is DISABLE
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_ENABLE_ID_ENABLE                =  1           // Word is ENABLE
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SWAP                       31

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SWAP_ENUM                    
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SWAP_ID_NO_SWAP                 =  0,          // Word is NO_SWAP
   APEX_642CL_STREAM_DMA_STREAM_OUT_WORD_CFG_BIT_SWAP_ID_SWAP                    =  1           // Word is SWAP
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    CU Configuration Register.
// 
// Device Control Register Bits Description:
//    BYTES_PER_CU         [07:00]   Bytes per CU (N-1). This must be a multiple of the bin_word_size. 
//    RESERVED_1           [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_CFG                                (APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_CFG_BASE_ADDRESS                   (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_CFG_OFFSET                         (0x00000024)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_CFG_DEFAULT                        (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_TAG
{
   struct _asApex642clStreamDmaStreamOutCuCfgBitFieldTag
   {
      REG32  BytesPerCu           :  8; //  07:00  APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_BIT_BYTES_PER_CU
      REG32  Reserved1            : 24; //  31:08  APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_BIT_RESERVED_1
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_TAG                       APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_DCR                      gApex642clStreamDmaStreamOutCuCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_DCR*                     gpApex642clStreamDmaStreamOutCuCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_BIT_BYTES_PER_CU                 0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_CU_CFG_BIT_RESERVED_1                   8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MUXSEL_CFG
//    APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Mux Select 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    WORD_0_B             [03:00]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_1_B             [11:08]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_2_B             [19:16]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_3_B             [27:24]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_0                          (APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_0_BASE_ADDRESS             (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_0_OFFSET                   (0x00000028)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_0_DEFAULT                  (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_1                          (APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_1_BASE_ADDRESS             (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_1_OFFSET                   (0x0000002c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_MUXSEL_CFG_1_DEFAULT                  (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_TAG
{
   struct _asApex642clStreamDmaStreamOutMuxselCfgBitFieldTag
   {
      REG32  Word0B0              :  4; //  3:0    APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_0_B_0
      REG32  Word0B1              :  4; //  7:4    APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_0_B_1
      REG32  Word1B0              :  4; //  11:8   APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_1_B_0
      REG32  Word1B1              :  4; //  15:12  APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_1_B_1
      REG32  Word2B0              :  4; //  19:16  APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_2_B_0
      REG32  Word2B1              :  4; //  23:20  APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_2_B_1
      REG32  Word3B0              :  4; //  27:24  APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_3_B_0
      REG32  Word3B1              :  4; //  31:28  APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_3_B_1
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_TAG                   APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_DCR                  gApex642clStreamDmaStreamOutMuxselCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_DCR*                 gpApex642clStreamDmaStreamOutMuxselCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_0_B_0               0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_0_B_1               4
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_1_B_0               8
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_1_B_1               12
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_2_B_0               16
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_2_B_1               20
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_3_B_0               24
#define APEX_642CL_STREAM_DMA_STREAM_OUT_MUXSEL_CFG_BIT_WORD_3_B_1               28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_SPAN
//    APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    CU Span 0 Register.
// 
// Device Control Register Bits Description:
//    ENABLE               [31:00]   Enable for CU 0 - 31
//                                   
//                                   0 : disable
//                                   1 : enable 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_0                             (APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_0_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_0_OFFSET                      (0x00000030)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_0_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_1                             (APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_1_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_1_OFFSET                      (0x00000034)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_CU_SPAN_1_DEFAULT                     (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_TAG
{
   struct _asApex642clStreamDmaStreamOutCuSpanBitFieldTag
   {
      REG32  Enable               : 32; //  31:00  APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_BIT_ENABLE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_TAG                      APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_DCR                     gApex642clStreamDmaStreamOutCuSpan;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_DCR*                    gpApex642clStreamDmaStreamOutCuSpan;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_BIT_ENABLE                      0

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_BIT_ENABLE_ENUM                   
{
   APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_BIT_ENABLE_ID_DISABLE                =  0,          // CU is DISABLE
   APEX_642CL_STREAM_DMA_STREAM_OUT_CU_SPAN_BIT_ENABLE_ID_ENABLE                 =  1           // CU is ENABLE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_1
//    APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 1 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    BYTE_CNT             [15:00]   Current line byte count value. 
//    LINE_CNT             [25:16]   Current line count value (N-1) 
//    RESERVED_1           [27:26]   Reserved 
//    FIFO_CS              [31:28]   fifo state machine.
//                                   
//                                   Stream IN:
//                                      parameter     idle_st          = 4'd0;
//                                      parameter     new_cu_st = 4'd1;
//                                      parameter     cu0_st         = 4'd2;
//                                      parameter     cu1_st          = 4'd3;
//                                      parameter     cu2_st         = 4'd4;
//                                      parameter     cu3_st         = 4'd5;
//                                      parameter     wt_st            = 4'd6;
//                                      parameter     wt1_st           = 4'd7;
//                                      parameter     done_wt      = 4'd8;
//                                   
//                                   Stream OUT:
//                                   
//                                      parameter     idle_st          = 3'd0;
//                                      parameter     new_cu_st = 3'd1;
//                                      parameter     pg_st            = 3'd2;
//                                      parameter     pg_wt            = 3'd3;
//                                      parameter     done_wt       = 3'd4;
//                                      parameter     pg_wt1           = 3'd5;
//                                      parameter     pg_wt2          = 3'd6;
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_1                              (APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_1_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_1_OFFSET                       (0x00000038)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_1_DEFAULT                      (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_TAG
{
   struct _asApex642clStreamDmaStreamOutStatus1BitFieldTag
   {
      REG32  ByteCnt              : 16; //  15:00  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_BYTE_CNT
      REG32  LineCnt              : 10; //  25:16  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_LINE_CNT
      REG32  Reserved1            :  2; //  27:26  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_RESERVED_1
      REG32  FifoCs               :  4; //  31:28  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_FIFO_CS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_TAG                     APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_DCR                    gApex642clStreamDmaStreamOutStatus1;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_DCR*                   gpApex642clStreamDmaStreamOutStatus1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_BYTE_CNT                   0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_LINE_CNT                   16
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_RESERVED_1                 26
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_1_BIT_FIFO_CS                    28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_2
//    APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_OUT
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 2 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    FIFO_BYTE_CNT        [09:00]   Current line byte count value. 
//    RESERVED_1           [15:10]   Reserved 
//    RD_CTRL_CS           [18:16]   Read Control State Machine Current State value. 
//    RESERVED_2           [19]      Reserved 
//    WR_CTRL_CS           [22:20]   Write Control State Machine Current State value. 
//    RESERVED_3           [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_2                              (APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_2_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_OUT_1)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_2_OFFSET                       (0x0000003c)
#define APEX_642CL_STREAM_DMA_STREAM_OUT_1_STATUS_2_DEFAULT                      (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_TAG
{
   struct _asApex642clStreamDmaStreamOutStatus2BitFieldTag
   {
      REG32  FifoByteCnt          : 10; //  09:00  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_FIFO_BYTE_CNT
      REG32  Reserved1            :  6; //  15:10  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RESERVED_1
      REG32  RdCtrlCs             :  3; //  18:16  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RD_CTRL_CS
      REG32  Reserved2            :  1; //  19     APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RESERVED_2
      REG32  WrCtrlCs             :  3; //  22:20  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_WR_CTRL_CS
      REG32  Reserved3            :  8; //  31:24  APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_TAG                     APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_DCR                    gApex642clStreamDmaStreamOutStatus2;
extern volatile APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_DCR*                   gpApex642clStreamDmaStreamOutStatus2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_FIFO_BYTE_CNT              0
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RESERVED_1                 10
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RD_CTRL_CS                 16
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RESERVED_2                 19
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_WR_CTRL_CS                 20
#define APEX_642CL_STREAM_DMA_STREAM_OUT_STATUS_2_BIT_RESERVED_3                 24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    STREAM_IN
//    APEX_642CL_STREAM_DMA_STREAM_IN
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00000500
//    Default:      32'h00000000
//    Nb:         4
//    Size:       256
// 
// Description:
//    Stream In Channel
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0                                        (APEX_642CL_STREAM_DMA_STREAM_IN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_BASE_ADDRESS                           (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_OFFSET                                 (0x00000500)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_SIZE                                   (0x00000100)

#define APEX_642CL_STREAM_DMA_STREAM_IN_1                                        (APEX_642CL_STREAM_DMA_STREAM_IN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_BASE_ADDRESS                           (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_OFFSET                                 (0x00000600)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_SIZE                                   (0x00000100)

#define APEX_642CL_STREAM_DMA_STREAM_IN_2                                        (APEX_642CL_STREAM_DMA_STREAM_IN_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_BASE_ADDRESS                           (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_OFFSET                                 (0x00000700)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_SIZE                                   (0x00000100)

#define APEX_642CL_STREAM_DMA_STREAM_IN_3                                        (APEX_642CL_STREAM_DMA_STREAM_IN_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_BASE_ADDRESS                           (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_OFFSET                                 (0x00000800)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_SIZE                                   (0x00000100)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Transfer Configuration Register.
//    
//    This register is shadowed and can be used in a pipelined fashion
//    
// 
// Device Control Register Bits Description:
//    CU_START             [05:00]   Index of the first CU on which the line transfer should begin. Previous CUs in the
//                                   same group of CUs are masked. 
//    RESERVED_1           [07:06]   Reserved 
//    BASE_ADDRESS_0       [18:08]   Base address for word0/2. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_2           [19]      Reserved 
//    BASE_ADDRESS_1       [30:20]   Base address for word1/3. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_3           [31]      Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_TRANSFER_CFG                           (APEX_642CL_STREAM_DMA_STREAM_IN_0_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_TRANSFER_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_TRANSFER_CFG_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_TRANSFER_CFG_OFFSET                    (0x00000000)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_TRANSFER_CFG_DEFAULT                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ADDRESS_OFFSET
//    APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Address offset Register.
//    
//    This register is shadowed and can be used in a pipelined fashion.
//    
//    Depending on how many bytes_per_CU are desired, the following  table outlines what
//    is achievable. The higher bin_word_size is always desireable since it has better
//    CMEM read/write performance.
//    
//    bin_word_size  16 
//    Bytes_per_CU  2
//    Bytes_per_CU 6,10,14,18,22,26,30
//    
//    bin_word_size  32
//    Bytes_per_CU  4
//    Bytes_per_CU 12,20,28,36,44,52,60
//    
//    bin_word_size  64 
//    Bytes_per_CU  8
//    Bytes_per_CU 16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
//    
//    The second row is the base number where entries_per_bin=0
//    
//    The third row are the other possible options realized by multiplying the base
//    number by (entries_per_bin + 1)
// 
// Device Control Register Bits Description:
//    ENTRIES_PER_BIN      [03:00]   Number of entries per BIN (N-1). 
//    BIN_WORD_SIZE        [05:04]   Size of the entries in the bin.
//                                   
//                                   0 : 16 bit
//                                   1 : 32 bit
//                                   2 : 64 bit
//                                   3 : reserved 
//    NUM_LINES            [15:06]   Total number of stream lines (N-1). 
//    BYTE_PER_LINE        [31:16]   Number of bytes necessary to complete one line. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_ADDRESS_OFFSET                         (APEX_642CL_STREAM_DMA_STREAM_IN_0_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_ADDRESS_OFFSET_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_ADDRESS_OFFSET_BASE_ADDRESS            (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_ADDRESS_OFFSET_OFFSET                  (0x00000004)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_ADDRESS_OFFSET_DEFAULT                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration1 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_INC_0        [10:00]   When used for multiscan, address Increment 0 is the increment to the base address
//                                   for word0/2 (signed value). This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_INC_1        [22:12]   When used for multiscan, address Increment 1 is the increment to the base address
//                                   for word1/3 (signed value). This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    INC_2D               [31:24]   Increment done on every 2D iterations. All words increment by the same value
//                                   (signed value). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_1                        (APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_1_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_1_OFFSET                 (0x00000008)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_1_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration2 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_FIRST_0      [10:00]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective
//                                   buffer size. This applies to word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_FIRST_1      [22:12]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective buffer size. This applies to word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    COUNT_2D             [31:24]   Number of times a GOC has to be repeated before going to the next one. This allows
//                                   a user to iterate over the same GOC before going to the next one (N-1). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_2                        (APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_2_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_2_OFFSET                 (0x0000000c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_2_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_3
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration3 Register.
// 
// Device Control Register Bits Description:
//    MAX_BUFFER_SIZE_0    [10:00]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    MAX_BUFFER_SIZE_1    [22:12]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    MODE                 [24]      0 : Mode A. Max buffer size test is applied when the start GOC is detected.
//                                   1 : Mode B. Max buffer size test is applied when the start of a new line is
//                                       detected.  
//    SCAN_ORDER           [25]      Stream direction support
//                                   
//                                   0 : GOC increments
//                                   1 : GOC decrements 
//    RESERVED_3           [31:26]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_3                        (APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_3_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_3_OFFSET                 (0x00000010)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MULTISCAN_CFG_3_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Word 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    BASE_ADDR_OFFSET     [10:00]   Offset from the base address for each word. See the address generation section for
//                                   more detailed description of how addressed are generated. This is a 16 bit
//                                   address.
//                                    
//    SLICE_INCREMENT      [15:11]   signed increment to the base address for each written slice. This is a 16 bit
//                                   address. 
//    LINE_INCREMENT       [26:16]   signed increment to the base address.  This is a 16 bit address. 
//    RESERVED_1           [27]      Reserved 
//    LINE_MODE            [29:28]   The stream can be configured to create YUV420 from YUV422.
//                                   
//                                   This is done by dropping the chrominance components in either the odd or even
//                                   lines. To accomplish this word 1 and word 3 should be configured with the line
//                                   mode set to either "odd line" or "even line". For all other applications and
//                                   words, the line mode is set to "both lines".
//                                   
//                                   00: both lines
//                                   01: even line
//                                   10: odd line
//                                   11: reserved
//                                    
//    ENABLE               [30]      Word enable 
//    SWAP                 [31]      0 : byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0
//                                   1 :  byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_0                             (APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_0_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_0_OFFSET                      (0x00000014)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_0_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_1                             (APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_1_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_1_OFFSET                      (0x00000018)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_1_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_2                             (APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_2_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_2_OFFSET                      (0x0000001c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_2_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_3                             (APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_3_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_3_OFFSET                      (0x00000020)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_WORD_CFG_3_DEFAULT                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    CU Configuration Register.
// 
// Device Control Register Bits Description:
//    BYTES_PER_CU         [07:00]   Bytes per CU (N-1). This must be a multiple of the bin_word_size. 
//    RESERVED_1           [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_CFG                                 (APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_CFG_BASE_ADDRESS                    (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_CFG_OFFSET                          (0x00000024)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_CFG_DEFAULT                         (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MUXSEL_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Mux Select 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    WORD_0_B             [03:00]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_1_B             [11:08]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_2_B             [19:16]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_3_B             [27:24]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_0                           (APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_0_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_0_OFFSET                    (0x00000028)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_0_DEFAULT                   (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_1                           (APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_1_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_1_OFFSET                    (0x0000002c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_MUXSEL_CFG_1_DEFAULT                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_SPAN
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    CU Span 0 Register.
// 
// Device Control Register Bits Description:
//    ENABLE               [31:00]   Enable for CU 0 - 31
//                                   
//                                   0 : disable
//                                   1 : enable 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_0                              (APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_0_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_0_OFFSET                       (0x00000030)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_0_DEFAULT                      (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_1                              (APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_1_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_1_OFFSET                       (0x00000034)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_CU_SPAN_1_DEFAULT                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 1 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    BYTE_CNT             [15:00]   Current line byte count value. 
//    LINE_CNT             [25:16]   Current line count value (N-1) 
//    RESERVED_1           [27:26]   Reserved 
//    FIFO_CS              [31:28]   fifo state machine.
//                                   
//                                   Stream IN:
//                                      parameter     idle_st          = 4'd0;
//                                      parameter     new_cu_st = 4'd1;
//                                      parameter     cu0_st         = 4'd2;
//                                      parameter     cu1_st          = 4'd3;
//                                      parameter     cu2_st         = 4'd4;
//                                      parameter     cu3_st         = 4'd5;
//                                      parameter     wt_st            = 4'd6;
//                                      parameter     wt1_st           = 4'd7;
//                                      parameter     done_wt      = 4'd8;
//                                   
//                                   Stream OUT:
//                                   
//                                      parameter     idle_st          = 3'd0;
//                                      parameter     new_cu_st = 3'd1;
//                                      parameter     pg_st            = 3'd2;
//                                      parameter     pg_wt            = 3'd3;
//                                      parameter     done_wt       = 3'd4;
//                                      parameter     pg_wt1           = 3'd5;
//                                      parameter     pg_wt2          = 3'd6;
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_1                               (APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_1_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_1_OFFSET                        (0x00000038)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_1_DEFAULT                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 2 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    FIFO_BYTE_CNT        [09:00]   Current line byte count value. 
//    RESERVED_1           [15:10]   Reserved 
//    RD_CTRL_CS           [18:16]   Read Control State Machine Current State value. 
//    RESERVED_2           [19]      Reserved 
//    WR_CTRL_CS           [22:20]   Write Control State Machine Current State value. 
//    RESERVED_3           [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_2                               (APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_2_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_0)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_2_OFFSET                        (0x0000003c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_0_STATUS_2_DEFAULT                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Transfer Configuration Register.
//    
//    This register is shadowed and can be used in a pipelined fashion
//    
// 
// Device Control Register Bits Description:
//    CU_START             [05:00]   Index of the first CU on which the line transfer should begin. Previous CUs in the
//                                   same group of CUs are masked. 
//    RESERVED_1           [07:06]   Reserved 
//    BASE_ADDRESS_0       [18:08]   Base address for word0/2. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_2           [19]      Reserved 
//    BASE_ADDRESS_1       [30:20]   Base address for word1/3. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_3           [31]      Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_TRANSFER_CFG                           (APEX_642CL_STREAM_DMA_STREAM_IN_1_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_TRANSFER_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_TRANSFER_CFG_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_TRANSFER_CFG_OFFSET                    (0x00000000)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_TRANSFER_CFG_DEFAULT                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ADDRESS_OFFSET
//    APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Address offset Register.
//    
//    This register is shadowed and can be used in a pipelined fashion.
//    
//    Depending on how many bytes_per_CU are desired, the following  table outlines what
//    is achievable. The higher bin_word_size is always desireable since it has better
//    CMEM read/write performance.
//    
//    bin_word_size  16 
//    Bytes_per_CU  2
//    Bytes_per_CU 6,10,14,18,22,26,30
//    
//    bin_word_size  32
//    Bytes_per_CU  4
//    Bytes_per_CU 12,20,28,36,44,52,60
//    
//    bin_word_size  64 
//    Bytes_per_CU  8
//    Bytes_per_CU 16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
//    
//    The second row is the base number where entries_per_bin=0
//    
//    The third row are the other possible options realized by multiplying the base
//    number by (entries_per_bin + 1)
// 
// Device Control Register Bits Description:
//    ENTRIES_PER_BIN      [03:00]   Number of entries per BIN (N-1). 
//    BIN_WORD_SIZE        [05:04]   Size of the entries in the bin.
//                                   
//                                   0 : 16 bit
//                                   1 : 32 bit
//                                   2 : 64 bit
//                                   3 : reserved 
//    NUM_LINES            [15:06]   Total number of stream lines (N-1). 
//    BYTE_PER_LINE        [31:16]   Number of bytes necessary to complete one line. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_ADDRESS_OFFSET                         (APEX_642CL_STREAM_DMA_STREAM_IN_1_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_ADDRESS_OFFSET_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_ADDRESS_OFFSET_BASE_ADDRESS            (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_ADDRESS_OFFSET_OFFSET                  (0x00000004)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_ADDRESS_OFFSET_DEFAULT                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration1 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_INC_0        [10:00]   When used for multiscan, address Increment 0 is the increment to the base address
//                                   for word0/2 (signed value). This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_INC_1        [22:12]   When used for multiscan, address Increment 1 is the increment to the base address
//                                   for word1/3 (signed value). This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    INC_2D               [31:24]   Increment done on every 2D iterations. All words increment by the same value
//                                   (signed value). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_1                        (APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_1_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_1_OFFSET                 (0x00000008)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_1_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration2 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_FIRST_0      [10:00]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective
//                                   buffer size. This applies to word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_FIRST_1      [22:12]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective buffer size. This applies to word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    COUNT_2D             [31:24]   Number of times a GOC has to be repeated before going to the next one. This allows
//                                   a user to iterate over the same GOC before going to the next one (N-1). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_2                        (APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_2_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_2_OFFSET                 (0x0000000c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_2_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_3
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration3 Register.
// 
// Device Control Register Bits Description:
//    MAX_BUFFER_SIZE_0    [10:00]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    MAX_BUFFER_SIZE_1    [22:12]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    MODE                 [24]      0 : Mode A. Max buffer size test is applied when the start GOC is detected.
//                                   1 : Mode B. Max buffer size test is applied when the start of a new line is
//                                       detected.  
//    SCAN_ORDER           [25]      Stream direction support
//                                   
//                                   0 : GOC increments
//                                   1 : GOC decrements 
//    RESERVED_3           [31:26]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_3                        (APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_3_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_3_OFFSET                 (0x00000010)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MULTISCAN_CFG_3_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Word 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    BASE_ADDR_OFFSET     [10:00]   Offset from the base address for each word. See the address generation section for
//                                   more detailed description of how addressed are generated. This is a 16 bit
//                                   address.
//                                    
//    SLICE_INCREMENT      [15:11]   signed increment to the base address for each written slice. This is a 16 bit
//                                   address. 
//    LINE_INCREMENT       [26:16]   signed increment to the base address.  This is a 16 bit address. 
//    RESERVED_1           [27]      Reserved 
//    LINE_MODE            [29:28]   The stream can be configured to create YUV420 from YUV422.
//                                   
//                                   This is done by dropping the chrominance components in either the odd or even
//                                   lines. To accomplish this word 1 and word 3 should be configured with the line
//                                   mode set to either "odd line" or "even line". For all other applications and
//                                   words, the line mode is set to "both lines".
//                                   
//                                   00: both lines
//                                   01: even line
//                                   10: odd line
//                                   11: reserved
//                                    
//    ENABLE               [30]      Word enable 
//    SWAP                 [31]      0 : byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0
//                                   1 :  byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_0                             (APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_0_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_0_OFFSET                      (0x00000014)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_0_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_1                             (APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_1_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_1_OFFSET                      (0x00000018)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_1_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_2                             (APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_2_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_2_OFFSET                      (0x0000001c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_2_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_3                             (APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_3_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_3_OFFSET                      (0x00000020)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_WORD_CFG_3_DEFAULT                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    CU Configuration Register.
// 
// Device Control Register Bits Description:
//    BYTES_PER_CU         [07:00]   Bytes per CU (N-1). This must be a multiple of the bin_word_size. 
//    RESERVED_1           [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_CFG                                 (APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_CFG_BASE_ADDRESS                    (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_CFG_OFFSET                          (0x00000024)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_CFG_DEFAULT                         (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MUXSEL_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Mux Select 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    WORD_0_B             [03:00]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_1_B             [11:08]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_2_B             [19:16]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_3_B             [27:24]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_0                           (APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_0_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_0_OFFSET                    (0x00000028)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_0_DEFAULT                   (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_1                           (APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_1_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_1_OFFSET                    (0x0000002c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_MUXSEL_CFG_1_DEFAULT                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_SPAN
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    CU Span 0 Register.
// 
// Device Control Register Bits Description:
//    ENABLE               [31:00]   Enable for CU 0 - 31
//                                   
//                                   0 : disable
//                                   1 : enable 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_0                              (APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_0_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_0_OFFSET                       (0x00000030)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_0_DEFAULT                      (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_1                              (APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_1_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_1_OFFSET                       (0x00000034)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_CU_SPAN_1_DEFAULT                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 1 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    BYTE_CNT             [15:00]   Current line byte count value. 
//    LINE_CNT             [25:16]   Current line count value (N-1) 
//    RESERVED_1           [27:26]   Reserved 
//    FIFO_CS              [31:28]   fifo state machine.
//                                   
//                                   Stream IN:
//                                      parameter     idle_st          = 4'd0;
//                                      parameter     new_cu_st = 4'd1;
//                                      parameter     cu0_st         = 4'd2;
//                                      parameter     cu1_st          = 4'd3;
//                                      parameter     cu2_st         = 4'd4;
//                                      parameter     cu3_st         = 4'd5;
//                                      parameter     wt_st            = 4'd6;
//                                      parameter     wt1_st           = 4'd7;
//                                      parameter     done_wt      = 4'd8;
//                                   
//                                   Stream OUT:
//                                   
//                                      parameter     idle_st          = 3'd0;
//                                      parameter     new_cu_st = 3'd1;
//                                      parameter     pg_st            = 3'd2;
//                                      parameter     pg_wt            = 3'd3;
//                                      parameter     done_wt       = 3'd4;
//                                      parameter     pg_wt1           = 3'd5;
//                                      parameter     pg_wt2          = 3'd6;
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_1                               (APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_1_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_1_OFFSET                        (0x00000038)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_1_DEFAULT                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 2 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    FIFO_BYTE_CNT        [09:00]   Current line byte count value. 
//    RESERVED_1           [15:10]   Reserved 
//    RD_CTRL_CS           [18:16]   Read Control State Machine Current State value. 
//    RESERVED_2           [19]      Reserved 
//    WR_CTRL_CS           [22:20]   Write Control State Machine Current State value. 
//    RESERVED_3           [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_2                               (APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_2_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_1)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_2_OFFSET                        (0x0000003c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_1_STATUS_2_DEFAULT                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Transfer Configuration Register.
//    
//    This register is shadowed and can be used in a pipelined fashion
//    
// 
// Device Control Register Bits Description:
//    CU_START             [05:00]   Index of the first CU on which the line transfer should begin. Previous CUs in the
//                                   same group of CUs are masked. 
//    RESERVED_1           [07:06]   Reserved 
//    BASE_ADDRESS_0       [18:08]   Base address for word0/2. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_2           [19]      Reserved 
//    BASE_ADDRESS_1       [30:20]   Base address for word1/3. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_3           [31]      Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_TRANSFER_CFG                           (APEX_642CL_STREAM_DMA_STREAM_IN_2_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_TRANSFER_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_TRANSFER_CFG_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_TRANSFER_CFG_OFFSET                    (0x00000000)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_TRANSFER_CFG_DEFAULT                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ADDRESS_OFFSET
//    APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Address offset Register.
//    
//    This register is shadowed and can be used in a pipelined fashion.
//    
//    Depending on how many bytes_per_CU are desired, the following  table outlines what
//    is achievable. The higher bin_word_size is always desireable since it has better
//    CMEM read/write performance.
//    
//    bin_word_size  16 
//    Bytes_per_CU  2
//    Bytes_per_CU 6,10,14,18,22,26,30
//    
//    bin_word_size  32
//    Bytes_per_CU  4
//    Bytes_per_CU 12,20,28,36,44,52,60
//    
//    bin_word_size  64 
//    Bytes_per_CU  8
//    Bytes_per_CU 16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
//    
//    The second row is the base number where entries_per_bin=0
//    
//    The third row are the other possible options realized by multiplying the base
//    number by (entries_per_bin + 1)
// 
// Device Control Register Bits Description:
//    ENTRIES_PER_BIN      [03:00]   Number of entries per BIN (N-1). 
//    BIN_WORD_SIZE        [05:04]   Size of the entries in the bin.
//                                   
//                                   0 : 16 bit
//                                   1 : 32 bit
//                                   2 : 64 bit
//                                   3 : reserved 
//    NUM_LINES            [15:06]   Total number of stream lines (N-1). 
//    BYTE_PER_LINE        [31:16]   Number of bytes necessary to complete one line. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_ADDRESS_OFFSET                         (APEX_642CL_STREAM_DMA_STREAM_IN_2_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_ADDRESS_OFFSET_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_ADDRESS_OFFSET_BASE_ADDRESS            (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_ADDRESS_OFFSET_OFFSET                  (0x00000004)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_ADDRESS_OFFSET_DEFAULT                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration1 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_INC_0        [10:00]   When used for multiscan, address Increment 0 is the increment to the base address
//                                   for word0/2 (signed value). This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_INC_1        [22:12]   When used for multiscan, address Increment 1 is the increment to the base address
//                                   for word1/3 (signed value). This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    INC_2D               [31:24]   Increment done on every 2D iterations. All words increment by the same value
//                                   (signed value). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_1                        (APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_1_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_1_OFFSET                 (0x00000008)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_1_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration2 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_FIRST_0      [10:00]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective
//                                   buffer size. This applies to word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_FIRST_1      [22:12]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective buffer size. This applies to word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    COUNT_2D             [31:24]   Number of times a GOC has to be repeated before going to the next one. This allows
//                                   a user to iterate over the same GOC before going to the next one (N-1). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_2                        (APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_2_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_2_OFFSET                 (0x0000000c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_2_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_3
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration3 Register.
// 
// Device Control Register Bits Description:
//    MAX_BUFFER_SIZE_0    [10:00]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    MAX_BUFFER_SIZE_1    [22:12]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    MODE                 [24]      0 : Mode A. Max buffer size test is applied when the start GOC is detected.
//                                   1 : Mode B. Max buffer size test is applied when the start of a new line is
//                                       detected.  
//    SCAN_ORDER           [25]      Stream direction support
//                                   
//                                   0 : GOC increments
//                                   1 : GOC decrements 
//    RESERVED_3           [31:26]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_3                        (APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_3_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_3_OFFSET                 (0x00000010)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MULTISCAN_CFG_3_DEFAULT                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Word 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    BASE_ADDR_OFFSET     [10:00]   Offset from the base address for each word. See the address generation section for
//                                   more detailed description of how addressed are generated. This is a 16 bit
//                                   address.
//                                    
//    SLICE_INCREMENT      [15:11]   signed increment to the base address for each written slice. This is a 16 bit
//                                   address. 
//    LINE_INCREMENT       [26:16]   signed increment to the base address.  This is a 16 bit address. 
//    RESERVED_1           [27]      Reserved 
//    LINE_MODE            [29:28]   The stream can be configured to create YUV420 from YUV422.
//                                   
//                                   This is done by dropping the chrominance components in either the odd or even
//                                   lines. To accomplish this word 1 and word 3 should be configured with the line
//                                   mode set to either "odd line" or "even line". For all other applications and
//                                   words, the line mode is set to "both lines".
//                                   
//                                   00: both lines
//                                   01: even line
//                                   10: odd line
//                                   11: reserved
//                                    
//    ENABLE               [30]      Word enable 
//    SWAP                 [31]      0 : byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0
//                                   1 :  byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_0                             (APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_0_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_0_OFFSET                      (0x00000014)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_0_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_1                             (APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_1_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_1_OFFSET                      (0x00000018)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_1_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_2                             (APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_2_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_2_OFFSET                      (0x0000001c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_2_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_3                             (APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_3_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_3_OFFSET                      (0x00000020)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_WORD_CFG_3_DEFAULT                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    CU Configuration Register.
// 
// Device Control Register Bits Description:
//    BYTES_PER_CU         [07:00]   Bytes per CU (N-1). This must be a multiple of the bin_word_size. 
//    RESERVED_1           [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_CFG                                 (APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_CFG_BASE_ADDRESS                    (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_CFG_OFFSET                          (0x00000024)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_CFG_DEFAULT                         (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MUXSEL_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Mux Select 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    WORD_0_B             [03:00]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_1_B             [11:08]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_2_B             [19:16]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_3_B             [27:24]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_0                           (APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_0_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_0_OFFSET                    (0x00000028)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_0_DEFAULT                   (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_1                           (APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_1_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_1_OFFSET                    (0x0000002c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_MUXSEL_CFG_1_DEFAULT                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_SPAN
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    CU Span 0 Register.
// 
// Device Control Register Bits Description:
//    ENABLE               [31:00]   Enable for CU 0 - 31
//                                   
//                                   0 : disable
//                                   1 : enable 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_0                              (APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_0_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_0_OFFSET                       (0x00000030)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_0_DEFAULT                      (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_1                              (APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_1_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_1_OFFSET                       (0x00000034)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_CU_SPAN_1_DEFAULT                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 1 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    BYTE_CNT             [15:00]   Current line byte count value. 
//    LINE_CNT             [25:16]   Current line count value (N-1) 
//    RESERVED_1           [27:26]   Reserved 
//    FIFO_CS              [31:28]   fifo state machine.
//                                   
//                                   Stream IN:
//                                      parameter     idle_st          = 4'd0;
//                                      parameter     new_cu_st = 4'd1;
//                                      parameter     cu0_st         = 4'd2;
//                                      parameter     cu1_st          = 4'd3;
//                                      parameter     cu2_st         = 4'd4;
//                                      parameter     cu3_st         = 4'd5;
//                                      parameter     wt_st            = 4'd6;
//                                      parameter     wt1_st           = 4'd7;
//                                      parameter     done_wt      = 4'd8;
//                                   
//                                   Stream OUT:
//                                   
//                                      parameter     idle_st          = 3'd0;
//                                      parameter     new_cu_st = 3'd1;
//                                      parameter     pg_st            = 3'd2;
//                                      parameter     pg_wt            = 3'd3;
//                                      parameter     done_wt       = 3'd4;
//                                      parameter     pg_wt1           = 3'd5;
//                                      parameter     pg_wt2          = 3'd6;
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_1                               (APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_1_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_1_OFFSET                        (0x00000038)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_1_DEFAULT                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 2 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    FIFO_BYTE_CNT        [09:00]   Current line byte count value. 
//    RESERVED_1           [15:10]   Reserved 
//    RD_CTRL_CS           [18:16]   Read Control State Machine Current State value. 
//    RESERVED_2           [19]      Reserved 
//    WR_CTRL_CS           [22:20]   Write Control State Machine Current State value. 
//    RESERVED_3           [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_2                               (APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_2_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_2)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_2_OFFSET                        (0x0000003c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_2_STATUS_2_DEFAULT                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Transfer Configuration Register.
//    
//    This register is shadowed and can be used in a pipelined fashion
//    
// 
// Device Control Register Bits Description:
//    CU_START             [05:00]   Index of the first CU on which the line transfer should begin. Previous CUs in the
//                                   same group of CUs are masked. 
//    RESERVED_1           [07:06]   Reserved 
//    BASE_ADDRESS_0       [18:08]   Base address for word0/2. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_2           [19]      Reserved 
//    BASE_ADDRESS_1       [30:20]   Base address for word1/3. Every address generated is relative to base_address.
//                                   This is a 16 bit address. 
//    RESERVED_3           [31]      Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_TRANSFER_CFG                           (APEX_642CL_STREAM_DMA_STREAM_IN_3_TRANSFER_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_TRANSFER_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_TRANSFER_CFG_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_TRANSFER_CFG_OFFSET                    (0x00000000)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_TRANSFER_CFG_DEFAULT                   (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_TAG
{
   struct _asApex642clStreamDmaStreamInTransferCfgBitFieldTag
   {
      REG32  CuStart              :  6; //  05:00  APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_CU_START
      REG32  Reserved1            :  2; //  07:06  APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_RESERVED_1
      REG32  BaseAddress0         : 11; //  18:08  APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_BASE_ADDRESS_0
      REG32  Reserved2            :  1; //  19     APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_RESERVED_2
      REG32  BaseAddress1         : 11; //  30:20  APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_BASE_ADDRESS_1
      REG32  Reserved3            :  1; //  31     APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_TAG                  APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_DCR                 gApex642clStreamDmaStreamInTransferCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_DCR*                gpApex642clStreamDmaStreamInTransferCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_CU_START                0
#define APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_RESERVED_1              6
#define APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_BASE_ADDRESS_0          8
#define APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_RESERVED_2              19
#define APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_BASE_ADDRESS_1          20
#define APEX_642CL_STREAM_DMA_STREAM_IN_TRANSFER_CFG_BIT_RESERVED_3              31

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ADDRESS_OFFSET
//    APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Address offset Register.
//    
//    This register is shadowed and can be used in a pipelined fashion.
//    
//    Depending on how many bytes_per_CU are desired, the following  table outlines what
//    is achievable. The higher bin_word_size is always desireable since it has better
//    CMEM read/write performance.
//    
//    bin_word_size  16 
//    Bytes_per_CU  2
//    Bytes_per_CU 6,10,14,18,22,26,30
//    
//    bin_word_size  32
//    Bytes_per_CU  4
//    Bytes_per_CU 12,20,28,36,44,52,60
//    
//    bin_word_size  64 
//    Bytes_per_CU  8
//    Bytes_per_CU 16,24,32,40,48,56,64,72,80,88,96,104,112,120,128
//    
//    The second row is the base number where entries_per_bin=0
//    
//    The third row are the other possible options realized by multiplying the base
//    number by (entries_per_bin + 1)
// 
// Device Control Register Bits Description:
//    ENTRIES_PER_BIN      [03:00]   Number of entries per BIN (N-1). 
//    BIN_WORD_SIZE        [05:04]   Size of the entries in the bin.
//                                   
//                                   0 : 16 bit
//                                   1 : 32 bit
//                                   2 : 64 bit
//                                   3 : reserved 
//    NUM_LINES            [15:06]   Total number of stream lines (N-1). 
//    BYTE_PER_LINE        [31:16]   Number of bytes necessary to complete one line. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_ADDRESS_OFFSET                         (APEX_642CL_STREAM_DMA_STREAM_IN_3_ADDRESS_OFFSET_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_ADDRESS_OFFSET_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_ADDRESS_OFFSET_BASE_ADDRESS            (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_ADDRESS_OFFSET_OFFSET                  (0x00000004)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_ADDRESS_OFFSET_DEFAULT                 (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_TAG
{
   struct _asApex642clStreamDmaStreamInAddressOffsetBitFieldTag
   {
      REG32  EntriesPerBin        :  4; //  03:00  APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_ENTRIES_PER_BIN
      REG32  BinWordSize          :  2; //  05:04  APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE
      REG32  NumLines             : 10; //  15:06  APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_NUM_LINES
      REG32  BytePerLine          : 16; //  31:16  APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BYTE_PER_LINE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_TAG                APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_DCR               gApex642clStreamDmaStreamInAddressOffset;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_DCR*              gpApex642clStreamDmaStreamInAddressOffset;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_ENTRIES_PER_BIN       0
#define APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE         4

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ENUM      
{
   APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_16_BIT    =  0,          // Size is 16_BIT
   APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_32_BIT    =  1,          // Size is 32_BIT
   APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BIN_WORD_SIZE_ID_64_BIT    =  2           // Size is 64_BIT
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_NUM_LINES             6
#define APEX_642CL_STREAM_DMA_STREAM_IN_ADDRESS_OFFSET_BIT_BYTE_PER_LINE         16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration1 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_INC_0        [10:00]   When used for multiscan, address Increment 0 is the increment to the base address
//                                   for word0/2 (signed value). This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_INC_1        [22:12]   When used for multiscan, address Increment 1 is the increment to the base address
//                                   for word1/3 (signed value). This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    INC_2D               [31:24]   Increment done on every 2D iterations. All words increment by the same value
//                                   (signed value). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_1                        (APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_1_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_1_OFFSET                 (0x00000008)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_1_DEFAULT                (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_TAG
{
   struct _asApex642clStreamDmaStreamInMultiscanCfg1BitFieldTag
   {
      REG32  AddressInc0          : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_ADDRESS_INC_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_RESERVED_1
      REG32  AddressInc1          : 11; //  22:12  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_ADDRESS_INC_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_RESERVED_2
      REG32  Inc2d                :  8; //  31:24  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_INC_2D
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_TAG               APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_DCR              gApex642clStreamDmaStreamInMultiscanCfg1;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_DCR*             gpApex642clStreamDmaStreamInMultiscanCfg1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_ADDRESS_INC_0        0
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_RESERVED_1           11
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_ADDRESS_INC_1        12
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_RESERVED_2           23
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_1_BIT_INC_2D               24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration2 Register.
// 
// Device Control Register Bits Description:
//    ADDRESS_FIRST_0      [10:00]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective
//                                   buffer size. This applies to word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    ADDRESS_FIRST_1      [22:12]   This is the first address in the circular buffer. In multiscan mode, first address
//                                   is the address to which the Base Address is reset when it exceeds the respective buffer size. This applies to word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    COUNT_2D             [31:24]   Number of times a GOC has to be repeated before going to the next one. This allows
//                                   a user to iterate over the same GOC before going to the next one (N-1). 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_2                        (APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_2_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_2_OFFSET                 (0x0000000c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_2_DEFAULT                (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_TAG
{
   struct _asApex642clStreamDmaStreamInMultiscanCfg2BitFieldTag
   {
      REG32  AddressFirst0        : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_RESERVED_1
      REG32  AddressFirst1        : 11; //  22:12  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_RESERVED_2
      REG32  Count2d              :  8; //  31:24  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_COUNT_2D
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_TAG               APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_DCR              gApex642clStreamDmaStreamInMultiscanCfg2;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_DCR*             gpApex642clStreamDmaStreamInMultiscanCfg2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_0      0
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_RESERVED_1           11
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_ADDRESS_FIRST_1      12
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_RESERVED_2           23
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_2_BIT_COUNT_2D             24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MULTISCAN_CFG_3
//    APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Multiscan Configuration3 Register.
// 
// Device Control Register Bits Description:
//    MAX_BUFFER_SIZE_0    [10:00]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word0/2. This is a 16 bit address. 
//    RESERVED_1           [11]      Reserved 
//    MAX_BUFFER_SIZE_1    [22:12]   The maximum address for the base address. If (base_address + address_increment >
//                                   max_buffer_size) then base_address is set back to first_address. This applies to
//                                   word1/3. This is a 16 bit address. 
//    RESERVED_2           [23]      Reserved 
//    MODE                 [24]      0 : Mode A. Max buffer size test is applied when the start GOC is detected.
//                                   1 : Mode B. Max buffer size test is applied when the start of a new line is
//                                       detected.  
//    SCAN_ORDER           [25]      Stream direction support
//                                   
//                                   0 : GOC increments
//                                   1 : GOC decrements 
//    RESERVED_3           [31:26]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_3                        (APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_3_BASE_ADDRESS           (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_3_OFFSET                 (0x00000010)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MULTISCAN_CFG_3_DEFAULT                (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_TAG
{
   struct _asApex642clStreamDmaStreamInMultiscanCfg3BitFieldTag
   {
      REG32  MaxBufferSize0       : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_0
      REG32  Reserved1            :  1; //  11     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_RESERVED_1
      REG32  MaxBufferSize1       : 11; //  22:12  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_1
      REG32  Reserved2            :  1; //  23     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_RESERVED_2
      REG32  Mode                 :  1; //  24     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MODE
      REG32  ScanOrder            :  1; //  25     APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_SCAN_ORDER
      REG32  Reserved3            :  6; //  31:26  APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_TAG               APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_DCR              gApex642clStreamDmaStreamInMultiscanCfg3;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_DCR*             gpApex642clStreamDmaStreamInMultiscanCfg3;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_0    0
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_RESERVED_1           11
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MAX_BUFFER_SIZE_1    12
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_RESERVED_2           23
#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MODE                 24

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MODE_ENUM              
{
   APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MODE_ID_MODE_A            =  0,          // Multiscan is MODE_A
   APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_MODE_ID_MODE_B            =  1           // Multiscan is MODE_B
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_SCAN_ORDER           25

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ENUM        
{
   APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ID_INC_GOC     =  0,          // GOC is INC_GOC
   APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_SCAN_ORDER_ID_DEC_GOC     =  1           // GOC is DEC_GOC
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_IN_MULTISCAN_CFG_3_BIT_RESERVED_3           26

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Word 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    BASE_ADDR_OFFSET     [10:00]   Offset from the base address for each word. See the address generation section for
//                                   more detailed description of how addressed are generated. This is a 16 bit
//                                   address.
//                                    
//    SLICE_INCREMENT      [15:11]   signed increment to the base address for each written slice. This is a 16 bit
//                                   address. 
//    LINE_INCREMENT       [26:16]   signed increment to the base address.  This is a 16 bit address. 
//    RESERVED_1           [27]      Reserved 
//    LINE_MODE            [29:28]   The stream can be configured to create YUV420 from YUV422.
//                                   
//                                   This is done by dropping the chrominance components in either the odd or even
//                                   lines. To accomplish this word 1 and word 3 should be configured with the line
//                                   mode set to either "odd line" or "even line". For all other applications and
//                                   words, the line mode is set to "both lines".
//                                   
//                                   00: both lines
//                                   01: even line
//                                   10: odd line
//                                   11: reserved
//                                    
//    ENABLE               [30]      Word enable 
//    SWAP                 [31]      0 : byte7, byte6, byte5, byte4, byte3, byte2, byte1, byte0
//                                   1 :  byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_0                             (APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_0_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_0_OFFSET                      (0x00000014)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_0_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_1                             (APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_1_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_1_OFFSET                      (0x00000018)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_1_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_2                             (APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_2_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_2_OFFSET                      (0x0000001c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_2_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_3                             (APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_3_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_3_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_3_OFFSET                      (0x00000020)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_WORD_CFG_3_DEFAULT                     (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_TAG
{
   struct _asApex642clStreamDmaStreamInWordCfgBitFieldTag
   {
      REG32  BaseAddrOffset       : 11; //  10:00  APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_BASE_ADDR_OFFSET
      REG32  SliceIncrement       :  5; //  15:11  APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT
      REG32  LineIncrement        : 11; //  26:16  APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_INCREMENT
      REG32  Reserved1            :  1; //  27     APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_RESERVED_1
      REG32  LineMode             :  2; //  29:28  APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_MODE
      REG32  Enable               :  1; //  30     APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_ENABLE
      REG32  Swap                 :  1; //  31     APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SWAP
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_TAG                      APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_DCR                     gApex642clStreamDmaStreamInWordCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_DCR*                    gpApex642clStreamDmaStreamInWordCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_BASE_ADDR_OFFSET            0
#define APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT             11

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT_ENUM          
{
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT_ID_ZERO          =  0,          // Slice increment ZERO
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT_ID_ONE           =  1,          // Slice increment ONE
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT_ID_TWO           =  2,          // Slice increment TWO
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT_ID_FOUR          =  4,          // Slice increment FOUR
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT_ID_EIGHT         =  8,          // Slice increment EIGHT
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SLICE_INCREMENT_ID_MINUS_FOUR    =  28          // Slice increment MINUS_FOUR
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_INCREMENT              16
#define APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_RESERVED_1                  27
#define APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_MODE                   28

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_MODE_ENUM                
{
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_MODE_ID_BOTH                =  0,          // Line Mode is BOTH
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_MODE_ID_EVEN                =  1,          // Line Mode is EVEN
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_LINE_MODE_ID_ODD                 =  1           // Line Mode is ODD
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_ENABLE                      30

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_ENABLE_ENUM                   
{
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_ENABLE_ID_DISABLE                =  0,          // Word is DISABLE
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_ENABLE_ID_ENABLE                 =  1           // Word is ENABLE
};
                                                                             

#define APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SWAP                        31

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SWAP_ENUM                     
{
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SWAP_ID_NO_SWAP                  =  0,          // Word is NO_SWAP
   APEX_642CL_STREAM_DMA_STREAM_IN_WORD_CFG_BIT_SWAP_ID_SWAP                     =  1           // Word is SWAP
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    CU Configuration Register.
// 
// Device Control Register Bits Description:
//    BYTES_PER_CU         [07:00]   Bytes per CU (N-1). This must be a multiple of the bin_word_size. 
//    RESERVED_1           [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_CFG                                 (APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_CFG_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_CFG_BASE_ADDRESS                    (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_CFG_OFFSET                          (0x00000024)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_CFG_DEFAULT                         (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_TAG
{
   struct _asApex642clStreamDmaStreamInCuCfgBitFieldTag
   {
      REG32  BytesPerCu           :  8; //  07:00  APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BIT_BYTES_PER_CU
      REG32  Reserved1            : 24; //  31:08  APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BIT_RESERVED_1
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_TAG                        APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_DCR                       gApex642clStreamDmaStreamInCuCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_DCR*                      gpApex642clStreamDmaStreamInCuCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BIT_BYTES_PER_CU                  0
#define APEX_642CL_STREAM_DMA_STREAM_IN_CU_CFG_BIT_RESERVED_1                    8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MUXSEL_CFG
//    APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Mux Select 0 Configuration Register.
// 
// Device Control Register Bits Description:
//    WORD_0_B             [03:00]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_1_B             [11:08]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_2_B             [19:16]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
//    WORD_3_B             [27:24]   Refer to the "stream re-organization" figure in the hardware spec. for a pictorial
//                                   description on how these mux controls operate. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_0                           (APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_0_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_0_OFFSET                    (0x00000028)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_0_DEFAULT                   (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_1                           (APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_1_BASE_ADDRESS              (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_1_OFFSET                    (0x0000002c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_MUXSEL_CFG_1_DEFAULT                   (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_TAG
{
   struct _asApex642clStreamDmaStreamInMuxselCfgBitFieldTag
   {
      REG32  Word0B0              :  4; //  3:0    APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_0_B_0
      REG32  Word0B1              :  4; //  7:4    APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_0_B_1
      REG32  Word1B0              :  4; //  11:8   APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_1_B_0
      REG32  Word1B1              :  4; //  15:12  APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_1_B_1
      REG32  Word2B0              :  4; //  19:16  APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_2_B_0
      REG32  Word2B1              :  4; //  23:20  APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_2_B_1
      REG32  Word3B0              :  4; //  27:24  APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_3_B_0
      REG32  Word3B1              :  4; //  31:28  APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_3_B_1
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_TAG                    APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_DCR                   gApex642clStreamDmaStreamInMuxselCfg;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_DCR*                  gpApex642clStreamDmaStreamInMuxselCfg;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_0_B_0                0
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_0_B_1                4
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_1_B_0                8
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_1_B_1                12
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_2_B_0                16
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_2_B_1                20
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_3_B_0                24
#define APEX_642CL_STREAM_DMA_STREAM_IN_MUXSEL_CFG_BIT_WORD_3_B_1                28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CU_SPAN
//    APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    CU Span 0 Register.
// 
// Device Control Register Bits Description:
//    ENABLE               [31:00]   Enable for CU 0 - 31
//                                   
//                                   0 : disable
//                                   1 : enable 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_0                              (APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_0_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_0_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_0_OFFSET                       (0x00000030)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_0_DEFAULT                      (0x00000000)

#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_1                              (APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_1_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_1_OFFSET                       (0x00000034)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_CU_SPAN_1_DEFAULT                      (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_TAG
{
   struct _asApex642clStreamDmaStreamInCuSpanBitFieldTag
   {
      REG32  Enable               : 32; //  31:00  APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BIT_ENABLE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_TAG                       APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_DCR                      gApex642clStreamDmaStreamInCuSpan;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_DCR*                     gpApex642clStreamDmaStreamInCuSpan;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BIT_ENABLE                       0

// Device Control Register Bits Id and Description
enum APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BIT_ENABLE_ENUM                    
{
   APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BIT_ENABLE_ID_DISABLE                 =  0,          // CU is DISABLE
   APEX_642CL_STREAM_DMA_STREAM_IN_CU_SPAN_BIT_ENABLE_ID_ENABLE                  =  1           // CU is ENABLE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_1
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h00000038
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 1 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    BYTE_CNT             [15:00]   Current line byte count value. 
//    LINE_CNT             [25:16]   Current line count value (N-1) 
//    RESERVED_1           [27:26]   Reserved 
//    FIFO_CS              [31:28]   fifo state machine.
//                                   
//                                   Stream IN:
//                                      parameter     idle_st          = 4'd0;
//                                      parameter     new_cu_st = 4'd1;
//                                      parameter     cu0_st         = 4'd2;
//                                      parameter     cu1_st          = 4'd3;
//                                      parameter     cu2_st         = 4'd4;
//                                      parameter     cu3_st         = 4'd5;
//                                      parameter     wt_st            = 4'd6;
//                                      parameter     wt1_st           = 4'd7;
//                                      parameter     done_wt      = 4'd8;
//                                   
//                                   Stream OUT:
//                                   
//                                      parameter     idle_st          = 3'd0;
//                                      parameter     new_cu_st = 3'd1;
//                                      parameter     pg_st            = 3'd2;
//                                      parameter     pg_wt            = 3'd3;
//                                      parameter     done_wt       = 3'd4;
//                                      parameter     pg_wt1           = 3'd5;
//                                      parameter     pg_wt2          = 3'd6;
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_1                               (APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_1_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_1_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_1_OFFSET                        (0x00000038)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_1_DEFAULT                       (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_TAG
{
   struct _asApex642clStreamDmaStreamInStatus1BitFieldTag
   {
      REG32  ByteCnt              : 16; //  15:00  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_BYTE_CNT
      REG32  LineCnt              : 10; //  25:16  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_LINE_CNT
      REG32  Reserved1            :  2; //  27:26  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_RESERVED_1
      REG32  FifoCs               :  4; //  31:28  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_FIFO_CS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_TAG                      APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_DCR                     gApex642clStreamDmaStreamInStatus1;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_DCR*                    gpApex642clStreamDmaStreamInStatus1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_BYTE_CNT                    0
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_LINE_CNT                    16
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_RESERVED_1                  26
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_1_BIT_FIFO_CS                     28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_2
//    APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA_STREAM_IN
//    Offset:       32'h0000003c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Status 2 Register
//    
//    This is a debug register and indicates the current state of several state
//    machines within the channel.
//    
// 
// Device Control Register Bits Description:
//    FIFO_BYTE_CNT        [09:00]   Current line byte count value. 
//    RESERVED_1           [15:10]   Reserved 
//    RD_CTRL_CS           [18:16]   Read Control State Machine Current State value. 
//    RESERVED_2           [19]      Reserved 
//    WR_CTRL_CS           [22:20]   Write Control State Machine Current State value. 
//    RESERVED_3           [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_2                               (APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_2_OFFSET)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_2_BASE_ADDRESS                  (APEX_642CL_STREAM_DMA_STREAM_IN_3)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_2_OFFSET                        (0x0000003c)
#define APEX_642CL_STREAM_DMA_STREAM_IN_3_STATUS_2_DEFAULT                       (0x00000000)


union _APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_TAG
{
   struct _asApex642clStreamDmaStreamInStatus2BitFieldTag
   {
      REG32  FifoByteCnt          : 10; //  09:00  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_FIFO_BYTE_CNT
      REG32  Reserved1            :  6; //  15:10  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RESERVED_1
      REG32  RdCtrlCs             :  3; //  18:16  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RD_CTRL_CS
      REG32  Reserved2            :  1; //  19     APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RESERVED_2
      REG32  WrCtrlCs             :  3; //  22:20  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_WR_CTRL_CS
      REG32  Reserved3            :  8; //  31:24  APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_TAG                      APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_DCR;

extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_DCR                     gApex642clStreamDmaStreamInStatus2;
extern volatile APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_DCR*                    gpApex642clStreamDmaStreamInStatus2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_FIFO_BYTE_CNT               0
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RESERVED_1                  10
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RD_CTRL_CS                  16
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RESERVED_2                  19
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_WR_CTRL_CS                  20
#define APEX_642CL_STREAM_DMA_STREAM_IN_STATUS_2_BIT_RESERVED_3                  24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MMAP_STREAM_OUT_FIFO
//    APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00001000
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         2
//    Size:       4096
// 
// Description:
//    Debug: Stream Out Memory Mapped FIFO
// 
// Device Control Register Bits Description:
//    POPD                 [00]      Reading this address will return the 32-bit StreamOut data within the Output FIFO. Output FIFO will be pop'd accordingly. Software is responsible for underflow monitoring. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_0                             (APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_0_OFFSET)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_0_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_0_OFFSET                      (0x00001000)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_0_DEFAULT                     (0x00000000)

#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_1                             (APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_1_OFFSET)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_1_BASE_ADDRESS                (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_1_OFFSET                      (0x00002000)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_1_DEFAULT                     (0x00000000)


union _APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_TAG
{
   struct _asApex642clStreamDmaMmapStreamOutFifoBitFieldTag
   {
      REG32  Popd                 :  1; //  00     APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_BIT_POPD
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_TAG                    APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_DCR;

extern volatile APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_DCR                   gApex642clStreamDmaMmapStreamOutFifo;
extern volatile APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_DCR*                  gpApex642clStreamDmaMmapStreamOutFifo;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_OUT_FIFO_BIT_POPD                      0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MMAP_STREAM_IN_FIFO
//    APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_OFFSET
//    Base Addr:    APEX_642CL_STREAM_DMA
//    Offset:       32'h00005000
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         4
//    Size:       4096
// 
// Description:
//    Debug: Stream In Memory Mapped FIFO
// 
// Device Control Register Bits Description:
//    PUSHD                [00]      Writing this address will insert the 32-bit write_data into the StreamIn Input FIFO. Input FIFO will be push'd accordingly. Software is responsible for overflow monitoring. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_0                              (APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_0_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_0_OFFSET)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_0_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_0_OFFSET                       (0x00005000)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_0_DEFAULT                      (0x00000000)

#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_1                              (APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_1_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_1_OFFSET)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_1_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_1_OFFSET                       (0x00006000)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_1_DEFAULT                      (0x00000000)

#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_2                              (APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_2_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_2_OFFSET)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_2_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_2_OFFSET                       (0x00007000)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_2_DEFAULT                      (0x00000000)

#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_3                              (APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_3_BASE_ADDRESS + APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_3_OFFSET)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_3_BASE_ADDRESS                 (APEX_642CL_STREAM_DMA_BASE_ADDRESS)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_3_OFFSET                       (0x00008000)
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_3_DEFAULT                      (0x00000000)


union _APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_TAG
{
   struct _asApex642clStreamDmaMmapStreamInFifoBitFieldTag
   {
      REG32  Pushd                :  1; //  00     APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_BIT_PUSHD
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_TAG                     APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_DCR;

extern volatile APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_DCR                    gApex642clStreamDmaMmapStreamInFifo;
extern volatile APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_DCR*                   gpApex642clStreamDmaMmapStreamInFifo;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_STREAM_DMA_MMAP_STREAM_IN_FIFO_BIT_PUSHD                      0

#endif /* APEX642CLSTREAMDMAMAPTYPE_H */
