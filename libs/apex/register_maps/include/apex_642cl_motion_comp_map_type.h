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
#ifndef APEX642CLMOTIONCOMPMAPTYPE_H
#define APEX642CLMOTIONCOMPMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ISR
//    APEX_642CL_MOTION_COMP_ISR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_ISR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ISR_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
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
// Device Control Register Bits Description:
//    CH                   [00]      Asserted when the MC channel is complete
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ISR                                               (APEX_642CL_MOTION_COMP_ISR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ISR_OFFSET)
#define APEX_642CL_MOTION_COMP_ISR_BASE_ADDRESS                                  (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_ISR_OFFSET                                        (0x00000000)
#define APEX_642CL_MOTION_COMP_ISR_DEFAULT                                       (0x00000000)


union _APEX_642CL_MOTION_COMP_ISR_TAG
{
   struct _asApex642clMotionCompIsrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MOTION_COMP_ISR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MOTION_COMP_ISR_BIT_CH_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_MOTION_COMP_ISR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_ISR_TAG                                    APEX_642CL_MOTION_COMP_ISR_DCR;

extern volatile APEX_642CL_MOTION_COMP_ISR_DCR                                   gApex642clMotionCompIsr;
extern volatile APEX_642CL_MOTION_COMP_ISR_DCR*                                  gpApex642clMotionCompIsr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ISR_BIT_CH_0                                      0
#define APEX_642CL_MOTION_COMP_ISR_BIT_CH_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_ISR_BIT_CH_ENUM                                     
{
   APEX_642CL_MOTION_COMP_ISR_BIT_CH_ID_ACTIVE                                   =  1,          // DMA channel done error source ACTIVE
   APEX_642CL_MOTION_COMP_ISR_BIT_CH_ID_INACTIVE                                 =  0           // DMA channel done error source INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_ISR_BIT_RESERVED                                  2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ESR
//    APEX_642CL_MOTION_COMP_ESR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_ESR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ESR_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    AXI Channel Error Source Register
//    
//    This error source does not generate an interrupt but can be used for debug
//    purposes. It does need to be cleared in a similar fashion to an interrupt source
//    
// 
// Device Control Register Bits Description:
//    CH                   [00]      Asserted when the MC channel experience a AXI error
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ESR                                               (APEX_642CL_MOTION_COMP_ESR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ESR_OFFSET)
#define APEX_642CL_MOTION_COMP_ESR_BASE_ADDRESS                                  (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_ESR_OFFSET                                        (0x00000004)
#define APEX_642CL_MOTION_COMP_ESR_DEFAULT                                       (0x00000000)


union _APEX_642CL_MOTION_COMP_ESR_TAG
{
   struct _asApex642clMotionCompEsrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MOTION_COMP_ESR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MOTION_COMP_ESR_BIT_CH_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_MOTION_COMP_ESR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_ESR_TAG                                    APEX_642CL_MOTION_COMP_ESR_DCR;

extern volatile APEX_642CL_MOTION_COMP_ESR_DCR                                   gApex642clMotionCompEsr;
extern volatile APEX_642CL_MOTION_COMP_ESR_DCR*                                  gpApex642clMotionCompEsr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ESR_BIT_CH_0                                      0
#define APEX_642CL_MOTION_COMP_ESR_BIT_CH_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_ESR_BIT_CH_ENUM                                     
{
   APEX_642CL_MOTION_COMP_ESR_BIT_CH_ID_ACTIVE                                   =  1,          // AXI channel error source ACTIVE
   APEX_642CL_MOTION_COMP_ESR_BIT_CH_ID_INACTIVE                                 =  0           // AXI channel error source INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_ESR_BIT_RESERVED                                  2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IMR
//    APEX_642CL_MOTION_COMP_IMR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_IMR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_IMR_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h00000008
//    Default:      32'h00000003
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Mask Register
//    
//    The interrupt mask register provides a mechanism to individually mask one or more
//    of the MC engine interrupt sources
//    
// 
// Device Control Register Bits Description:
//    CH                   [00]      Mask the channel done event from the channel
//                                   
//                                   0: Interrupt is not mask
//                                   1: Interrupt is mask
//                                    
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_IMR                                               (APEX_642CL_MOTION_COMP_IMR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_IMR_OFFSET)
#define APEX_642CL_MOTION_COMP_IMR_BASE_ADDRESS                                  (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_IMR_OFFSET                                        (0x00000008)
#define APEX_642CL_MOTION_COMP_IMR_DEFAULT                                       (0x00000003)


union _APEX_642CL_MOTION_COMP_IMR_TAG
{
   struct _asApex642clMotionCompImrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MOTION_COMP_IMR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MOTION_COMP_IMR_BIT_CH_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_MOTION_COMP_IMR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_IMR_TAG                                    APEX_642CL_MOTION_COMP_IMR_DCR;

extern volatile APEX_642CL_MOTION_COMP_IMR_DCR                                   gApex642clMotionCompImr;
extern volatile APEX_642CL_MOTION_COMP_IMR_DCR*                                  gpApex642clMotionCompImr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_IMR_BIT_CH_0                                      0
#define APEX_642CL_MOTION_COMP_IMR_BIT_CH_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_IMR_BIT_CH_ENUM                                     
{
   APEX_642CL_MOTION_COMP_IMR_BIT_CH_ID_ACTIVE                                   =  1,          // Mask channel done interrupt source ACTIVE
   APEX_642CL_MOTION_COMP_IMR_BIT_CH_ID_INACTIVE                                 =  0           // Mask channel done interrupt source INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_IMR_BIT_RESERVED                                  2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ICR
//    APEX_642CL_MOTION_COMP_ICR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_ICR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ICR_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw1c
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Clear Register
//    
//    The interrupt clear register provides the mechanism for clearing the raw interrupt
//    sources. Writing a '1' to the interrupt bit location will clear the interrupt
//    
// 
// Device Control Register Bits Description:
//    CH                   [00]      Clear the channel done interrupt when '1' is written
//                                   
//                                   0: nothing
//                                   1: clear Interrupt
//                                    
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ICR                                               (APEX_642CL_MOTION_COMP_ICR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ICR_OFFSET)
#define APEX_642CL_MOTION_COMP_ICR_BASE_ADDRESS                                  (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_ICR_OFFSET                                        (0x0000000c)
#define APEX_642CL_MOTION_COMP_ICR_DEFAULT                                       (0x00000000)


union _APEX_642CL_MOTION_COMP_ICR_TAG
{
   struct _asApex642clMotionCompIcrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MOTION_COMP_ICR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MOTION_COMP_ICR_BIT_CH_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_MOTION_COMP_ICR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_ICR_TAG                                    APEX_642CL_MOTION_COMP_ICR_DCR;

extern volatile APEX_642CL_MOTION_COMP_ICR_DCR                                   gApex642clMotionCompIcr;
extern volatile APEX_642CL_MOTION_COMP_ICR_DCR*                                  gpApex642clMotionCompIcr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ICR_BIT_CH_0                                      0
#define APEX_642CL_MOTION_COMP_ICR_BIT_CH_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_ICR_BIT_CH_ENUM                                     
{
   APEX_642CL_MOTION_COMP_ICR_BIT_CH_ID_ACTIVE                                   =  1,          // Clear channel done interrupt source ACTIVE
   APEX_642CL_MOTION_COMP_ICR_BIT_CH_ID_INACTIVE                                 =  0           // Clear channel done interrupt source INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_ICR_BIT_RESERVED                                  2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ECR
//    APEX_642CL_MOTION_COMP_ECR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_ECR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ECR_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw1c
//    Nb:         1
//    Size:       4
// 
// Description:
//    Error Clear Register
//    
//    The error clear register provides the mechanism for clearing the error sources.
//    Writing a '1' to the interrupt bit location will clear the interrupt. This will
//    clear both the channel error and the port error
//    
// 
// Device Control Register Bits Description:
//    CH                   [00]      Clear the MC channel error when written '1'
//                                   
//                                   0: nothing
//                                   1: clear error
//                                    
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ECR                                               (APEX_642CL_MOTION_COMP_ECR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_ECR_OFFSET)
#define APEX_642CL_MOTION_COMP_ECR_BASE_ADDRESS                                  (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_ECR_OFFSET                                        (0x00000010)
#define APEX_642CL_MOTION_COMP_ECR_DEFAULT                                       (0x00000000)


union _APEX_642CL_MOTION_COMP_ECR_TAG
{
   struct _asApex642clMotionCompEcrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MOTION_COMP_ECR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MOTION_COMP_ECR_BIT_CH_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_MOTION_COMP_ECR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_ECR_TAG                                    APEX_642CL_MOTION_COMP_ECR_DCR;

extern volatile APEX_642CL_MOTION_COMP_ECR_DCR                                   gApex642clMotionCompEcr;
extern volatile APEX_642CL_MOTION_COMP_ECR_DCR*                                  gpApex642clMotionCompEcr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_ECR_BIT_CH_0                                      0
#define APEX_642CL_MOTION_COMP_ECR_BIT_CH_1                                      1

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_ECR_BIT_CH_ENUM                                     
{
   APEX_642CL_MOTION_COMP_ECR_BIT_CH_ID_ACTIVE                                   =  1,          // Clear interrupt error and port source ACTIVE
   APEX_642CL_MOTION_COMP_ECR_BIT_CH_ID_INACTIVE                                 =  0           // Clear interrupt error and port source INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_ECR_BIT_RESERVED                                  2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DMA_CONTROL
//    APEX_642CL_MOTION_COMP_DMA_CONTROL
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_DMA_CONTROL_BASE_ADDRESS + APEX_642CL_MOTION_COMP_DMA_CONTROL_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    MC Control Register
//    
//    This register is for MC control purposes
//    
// 
// Device Control Register Bits Description:
//    DMA_SUSPEND          [00]      This bit when set, is used to suspend the MC from any AXI or side channel
//                                   activities. Resetting to '0' removes the MC from being suspended. Bit 31
//                                   provides the status of the MC
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED             [30:01]   Reserved 
//    DMA_SUSPEND_STATUS   [31]      This bit refelcts the status of the DMA suspend operation.  
//                                   
//                                   0: MC is Active, not suspended
//                                   1: MC is Inactive suspended
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_DMA_CONTROL                                       (APEX_642CL_MOTION_COMP_DMA_CONTROL_BASE_ADDRESS + APEX_642CL_MOTION_COMP_DMA_CONTROL_OFFSET)
#define APEX_642CL_MOTION_COMP_DMA_CONTROL_BASE_ADDRESS                          (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_DMA_CONTROL_OFFSET                                (0x00000014)
#define APEX_642CL_MOTION_COMP_DMA_CONTROL_DEFAULT                               (0x00000000)


union _APEX_642CL_MOTION_COMP_DMA_CONTROL_TAG
{
   struct _asApex642clMotionCompDmaControlBitFieldTag
   {
      REG32  DmaSuspend           :  1; //  00     APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND
      REG32  Reserved             : 30; //  30:01  APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_RESERVED
      REG32  DmaSuspendStatus     :  1; //  31     APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_DMA_CONTROL_TAG                            APEX_642CL_MOTION_COMP_DMA_CONTROL_DCR;

extern volatile APEX_642CL_MOTION_COMP_DMA_CONTROL_DCR                           gApex642clMotionCompDmaControl;
extern volatile APEX_642CL_MOTION_COMP_DMA_CONTROL_DCR*                          gpApex642clMotionCompDmaControl;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND                       0

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_ENUM                    
{
   APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_ID_ACTIVE                  =  1,          // MC suspend ACTIVE
   APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_ID_INACTIVE                =  0           // MC suspend INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_RESERVED                          1
#define APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS                31

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS_ENUM             
{
   APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS_ID_ACTIVE           =  1,          // MC is ACTIVE, not suspended
   APEX_642CL_MOTION_COMP_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS_ID_INACTIVE         =  0           // MC is INACTIVE, suspended
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CH_ENABLE
//    APEX_642CL_MOTION_COMP_CH_ENABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_ENABLE_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENABLE_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Enable Register
//    
//    The channel enable registers is used to enable the active DMA channels. When
//    written "1" the channel is active and will move data as long as it is not
//    "paused". Once a channel is enabled it cannot be disabled through this register
//    (Writing a "0" to any bit location has no effect). The register bits, when read,
//    reflect the currently active channels. A channel will become inactive when the
//    channel is complete or if the channel is cleared and reset via the "channel clear"
//    register.
//    
// 
// Device Control Register Bits Description:
//    CH                   [00]      Enable for DMA channel
//                                   
//                                   0: nothing
//                                   1: Enable DMA channel
//                                    
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_ENABLE                                         (APEX_642CL_MOTION_COMP_CH_ENABLE_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENABLE_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_ENABLE_BASE_ADDRESS                            (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_CH_ENABLE_OFFSET                                  (0x00000018)
#define APEX_642CL_MOTION_COMP_CH_ENABLE_DEFAULT                                 (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_ENABLE_TAG
{
   struct _asApex642clMotionCompChEnableBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_CH_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_ENABLE_TAG                              APEX_642CL_MOTION_COMP_CH_ENABLE_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_ENABLE_DCR                             gApex642clMotionCompChEnable;
extern volatile APEX_642CL_MOTION_COMP_CH_ENABLE_DCR*                            gpApex642clMotionCompChEnable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_CH_0                                0
#define APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_CH_1                                1

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_CH_ENUM                               
{
   APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_CH_ID_ACTIVE                             =  1,          // Enable DMA channel ACTIVE
   APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_CH_ID_INACTIVE                           =  0           // Enable DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_CH_ENABLE_BIT_RESERVED                            2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CH_CLEAR
//    APEX_642CL_MOTION_COMP_CH_CLEAR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_CLEAR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_CLEAR_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Clear Register
//    
//    The channel clear register provides a mechanism for software to disable and halt a
//    channel. The channel configuration is not cleared and retains the previously
//    programmed information. The channel state machine and any of its counters are
//    returned to the idle state and the channel enable bit (as viewed in the "channel
//    enable" register) is cleared. If a bus transaction is in progress, the transaction
//    (whether it be a burst or single beat) will complete before the channel is
//    disabled. Writing a '0' has no effect
//    
//    The bit is cleared by the hardware when the clear is done.
//    
// 
// Device Control Register Bits Description:
//    CH                   [00]      Clear for MC channel
//                                   
//                                   0: nothing
//                                   1: clear MC channel enable
//                                    
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_CLEAR                                          (APEX_642CL_MOTION_COMP_CH_CLEAR_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_CLEAR_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_CLEAR_BASE_ADDRESS                             (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_CH_CLEAR_OFFSET                                   (0x0000001c)
#define APEX_642CL_MOTION_COMP_CH_CLEAR_DEFAULT                                  (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_CLEAR_TAG
{
   struct _asApex642clMotionCompChClearBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_CH_1
      REG32  Reserved             : 30; //  31:02  APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_CLEAR_TAG                               APEX_642CL_MOTION_COMP_CH_CLEAR_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_CLEAR_DCR                              gApex642clMotionCompChClear;
extern volatile APEX_642CL_MOTION_COMP_CH_CLEAR_DCR*                             gpApex642clMotionCompChClear;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_CH_0                                 0
#define APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_CH_1                                 1

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_CH_ENUM                                
{
   APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_CH_ID_ACTIVE                              =  1,          // Clear MC channel ACTIVE
   APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_CH_ID_INACTIVE                            =  0           // Clear MC channel INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_CH_CLEAR_BIT_RESERVED                             2

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
#define APEX_642CL_MOTION_COMP_PERF                                              (APEX_642CL_MOTION_COMP_PERF_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_OFFSET)
#define APEX_642CL_MOTION_COMP_PERF_BASE_ADDRESS                                 (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_PERF_OFFSET                                       (0x00000020)
#define APEX_642CL_MOTION_COMP_PERF_SIZE                                         (0x0000000c)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONTROL
//    APEX_642CL_MOTION_COMP_PERF_CONTROL
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_PERF_CONTROL_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_CONTROL_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_PERF
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Performance Counter Control
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      This bit enables/disables the performance counter operation. 
//    RESERVED             [31:01]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_PERF_CONTROL                                      (APEX_642CL_MOTION_COMP_PERF_CONTROL_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_CONTROL_OFFSET)
#define APEX_642CL_MOTION_COMP_PERF_CONTROL_BASE_ADDRESS                         (APEX_642CL_MOTION_COMP_PERF)
#define APEX_642CL_MOTION_COMP_PERF_CONTROL_OFFSET                               (0x00000000)
#define APEX_642CL_MOTION_COMP_PERF_CONTROL_DEFAULT                              (0x00000000)


union _APEX_642CL_MOTION_COMP_PERF_CONTROL_TAG
{
   struct _asApex642clMotionCompPerfControlBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_MOTION_COMP_PERF_CONTROL_BIT_ENABLE
      REG32  Reserved             : 31; //  31:01  APEX_642CL_MOTION_COMP_PERF_CONTROL_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_PERF_CONTROL_TAG                           APEX_642CL_MOTION_COMP_PERF_CONTROL_DCR;

extern volatile APEX_642CL_MOTION_COMP_PERF_CONTROL_DCR                          gApex642clMotionCompPerfControl;
extern volatile APEX_642CL_MOTION_COMP_PERF_CONTROL_DCR*                         gpApex642clMotionCompPerfControl;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_PERF_CONTROL_BIT_ENABLE                           0

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_PERF_CONTROL_BIT_ENABLE_ENUM                        
{
   APEX_642CL_MOTION_COMP_PERF_CONTROL_BIT_ENABLE_ID_ACTIVE                      =  1,          // Performance Counter ACTIVE
   APEX_642CL_MOTION_COMP_PERF_CONTROL_BIT_ENABLE_ID_INACTIVE                    =  0           // Performance Counter INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_PERF_CONTROL_BIT_RESERVED                         1

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TOTAL
//    APEX_642CL_MOTION_COMP_PERF_TOTAL
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_PERF_TOTAL_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_TOTAL_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_PERF
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Performance Total Count
// 
// Device Control Register Bits Description:
//    COUNT                [31:00]   This is a free running counter that provides the total count of the performance window. The counter  is cleared and starts counting when the performance counter is enabled. It holds the last count value when the performance counter is disabled. A read of the register provides the current count. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_PERF_TOTAL                                        (APEX_642CL_MOTION_COMP_PERF_TOTAL_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_TOTAL_OFFSET)
#define APEX_642CL_MOTION_COMP_PERF_TOTAL_BASE_ADDRESS                           (APEX_642CL_MOTION_COMP_PERF)
#define APEX_642CL_MOTION_COMP_PERF_TOTAL_OFFSET                                 (0x00000004)
#define APEX_642CL_MOTION_COMP_PERF_TOTAL_DEFAULT                                (0x00000000)


union _APEX_642CL_MOTION_COMP_PERF_TOTAL_TAG
{
   struct _asApex642clMotionCompPerfTotalBitFieldTag
   {
      REG32  Count                : 32; //  31:00  APEX_642CL_MOTION_COMP_PERF_TOTAL_BIT_COUNT
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_PERF_TOTAL_TAG                             APEX_642CL_MOTION_COMP_PERF_TOTAL_DCR;

extern volatile APEX_642CL_MOTION_COMP_PERF_TOTAL_DCR                            gApex642clMotionCompPerfTotal;
extern volatile APEX_642CL_MOTION_COMP_PERF_TOTAL_DCR*                           gpApex642clMotionCompPerfTotal;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_PERF_TOTAL_BIT_COUNT                              0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BUSRD
//    APEX_642CL_MOTION_COMP_PERF_BUSRD
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_PERF_BUSRD_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_BUSRD_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_PERF
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Performance Bus read Count
// 
// Device Control Register Bits Description:
//    COUNT                [31:00]   This counter (when enabled) increments whenever the AXI read channel is active. The counter  is cleared and starts counting when the performance counter is enabled. It holds the last count value when the performance counter is disabled. A read of the register provides the current count. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_PERF_BUSRD                                        (APEX_642CL_MOTION_COMP_PERF_BUSRD_BASE_ADDRESS + APEX_642CL_MOTION_COMP_PERF_BUSRD_OFFSET)
#define APEX_642CL_MOTION_COMP_PERF_BUSRD_BASE_ADDRESS                           (APEX_642CL_MOTION_COMP_PERF)
#define APEX_642CL_MOTION_COMP_PERF_BUSRD_OFFSET                                 (0x00000008)
#define APEX_642CL_MOTION_COMP_PERF_BUSRD_DEFAULT                                (0x00000000)


union _APEX_642CL_MOTION_COMP_PERF_BUSRD_TAG
{
   struct _asApex642clMotionCompPerfBusrdBitFieldTag
   {
      REG32  Count                : 32; //  31:00  APEX_642CL_MOTION_COMP_PERF_BUSRD_BIT_COUNT
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_PERF_BUSRD_TAG                             APEX_642CL_MOTION_COMP_PERF_BUSRD_DCR;

extern volatile APEX_642CL_MOTION_COMP_PERF_BUSRD_DCR                            gApex642clMotionCompPerfBusrd;
extern volatile APEX_642CL_MOTION_COMP_PERF_BUSRD_DCR*                           gpApex642clMotionCompPerfBusrd;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_PERF_BUSRD_BIT_COUNT                              0

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
#define APEX_642CL_MOTION_COMP_CH_0                                              (APEX_642CL_MOTION_COMP_CH_0_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_BASE_ADDRESS                                 (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_CH_0_OFFSET                                       (0x00000100)
#define APEX_642CL_MOTION_COMP_CH_0_SIZE                                         (0x00000100)

#define APEX_642CL_MOTION_COMP_CH_1                                              (APEX_642CL_MOTION_COMP_CH_1_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_BASE_ADDRESS                                 (APEX_642CL_MOTION_COMP_BASE_ADDRESS)
#define APEX_642CL_MOTION_COMP_CH_1_OFFSET                                       (0x00000200)
#define APEX_642CL_MOTION_COMP_CH_1_SIZE                                         (0x00000100)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MOTION_COMP_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Address Register
//    
//    This register contains the address of the first entry in the linked list. The
//    linked list address is always on a 32 bit boundary
//    
//    When this register is read it reflects the address of the current linked list
//    entry.
//    
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      This bit enables the linked list mode of operation for the motion comp engine.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ADDR                 [31:01]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_0_LINKED_LIST                                  (APEX_642CL_MOTION_COMP_CH_0_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_LINKED_LIST_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_LINKED_LIST_BASE_ADDRESS                     (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_LINKED_LIST_OFFSET                           (0x00000000)
#define APEX_642CL_MOTION_COMP_CH_0_LINKED_LIST_DEFAULT                          (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MOTION_COMP_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Configuration Register
//    
//    This register is used to program the various configurable parameters of each DMA
//    channel
//    
// 
// Device Control Register Bits Description:
//    RESERVED_1           [00]      Reserved 
//    NUM_GOC              [10:01]   Number of GOCs (N-1) 
//    NUM_LINES            [22:11]   Number of lines (N-1) 
//    NUM_WORDS            [26:23]   Number of Words per pointer (N-1) 
//    RESERVED_2           [27]      Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_0_CONFIG                                       (APEX_642CL_MOTION_COMP_CH_0_CONFIG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_CONFIG_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_CONFIG_BASE_ADDRESS                          (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_CONFIG_OFFSET                                (0x00000004)
#define APEX_642CL_MOTION_COMP_CH_0_CONFIG_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENGINE_LINE_POINTER
//    APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    This register does not need to be programmed if linked lists are enabled. It will
//    reflect the contents of the last linked list entry when read
//    
// 
// Device Control Register Bits Description:
//    OFFSET               [31:00]   Pointer Increment used to advance to the next line of the macro block. This is a
//                                   32 bit signed value. Bit 31 is the sign.
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_POINTER                          (APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_POINTER_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_POINTER_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_POINTER_BASE_ADDRESS             (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_POINTER_OFFSET                   (0x00000008)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_POINTER_DEFAULT                  (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENGINE_LINE_MACRO_POINTER
//    APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    This register does not need to be programmed if linked lists are enabled. It will
//    reflect the contents of the last linked list entry when read
//    
// 
// Device Control Register Bits Description:
//    OFFSET               [31:00]   macro pointer 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_0                  (APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_0_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_0_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_0_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_0_OFFSET           (0x0000000c)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_0_DEFAULT          (0x00000000)

#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_1                  (APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_1_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_1_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_1_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_1_OFFSET           (0x00000010)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_1_DEFAULT          (0x00000000)

#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_2                  (APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_2_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_2_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_2_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_2_OFFSET           (0x00000014)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_2_DEFAULT          (0x00000000)

#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_3                  (APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_3_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_3_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_3_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_3_OFFSET           (0x00000018)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LINE_MACRO_POINTER_3_DEFAULT          (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENGINE_LIST
//    APEX_642CL_MOTION_COMP_CH_ENGINE_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    This register does not need to be programmed if linked list sare enabled. It will
//    reflect the contents of the last linked list entry when read
//    
// 
// Device Control Register Bits Description:
//    RESERVED             [01:00]   Reserved 
//    OFFSET               [31:02]   This is a signed offset that can be applied the addresses located in the pointer
//                                   list table.
//                                   
//                                   Hardware appends 0 to ptr_list_offset[1:0]. If this field is 0x0, then no offset
//                                   is applied. If the field is non_zero, an offset is applied.
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LIST                                  (APEX_642CL_MOTION_COMP_CH_0_ENGINE_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_ENGINE_LIST_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LIST_BASE_ADDRESS                     (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LIST_OFFSET                           (0x0000001c)
#define APEX_642CL_MOTION_COMP_CH_0_ENGINE_LIST_DEFAULT                          (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DEBUG
//    APEX_642CL_MOTION_COMP_CH_DEBUG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_DEBUG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_DEBUG_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Address Register
//    
//    This register contains the address of the first entry in the linked list. The
//    linked list address is always on a 32 bit boundary
//    
//    When this register is read it reflects the address of the current linked list
//    entry.
//    
// 
// Device Control Register Bits Description:
//    LIST_CS              [03:00]   Linked list control state machine. 
//    FSM_CS               [07:04]   Engine state machine. 
//    FSM_FIFO_CS          [10:08]   Engine fifo state machine. 
//    RESERVED             [31:11]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_0_DEBUG                                        (APEX_642CL_MOTION_COMP_CH_0_DEBUG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_0_DEBUG_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_0_DEBUG_BASE_ADDRESS                           (APEX_642CL_MOTION_COMP_CH_0)
#define APEX_642CL_MOTION_COMP_CH_0_DEBUG_OFFSET                                 (0x00000020)
#define APEX_642CL_MOTION_COMP_CH_0_DEBUG_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MOTION_COMP_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Address Register
//    
//    This register contains the address of the first entry in the linked list. The
//    linked list address is always on a 32 bit boundary
//    
//    When this register is read it reflects the address of the current linked list
//    entry.
//    
// 
// Device Control Register Bits Description:
//    ENABLE               [00]      This bit enables the linked list mode of operation for the motion comp engine.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ADDR                 [31:01]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_1_LINKED_LIST                                  (APEX_642CL_MOTION_COMP_CH_1_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_LINKED_LIST_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_LINKED_LIST_BASE_ADDRESS                     (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_LINKED_LIST_OFFSET                           (0x00000000)
#define APEX_642CL_MOTION_COMP_CH_1_LINKED_LIST_DEFAULT                          (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_LINKED_LIST_TAG
{
   struct _asApex642clMotionCompChLinkedListBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BIT_ENABLE
      REG32  Addr                 : 31; //  31:01  APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_LINKED_LIST_TAG                         APEX_642CL_MOTION_COMP_CH_LINKED_LIST_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_LINKED_LIST_DCR                        gApex642clMotionCompChLinkedList;
extern volatile APEX_642CL_MOTION_COMP_CH_LINKED_LIST_DCR*                       gpApex642clMotionCompChLinkedList;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BIT_ENABLE                         0

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BIT_ENABLE_ENUM                      
{
   APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BIT_ENABLE_ID_ACTIVE                    =  1,          // Linked List mode ACTIVE
   APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BIT_ENABLE_ID_INACTIVE                  =  0           // Linked List mode INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_CH_LINKED_LIST_BIT_ADDR                           1

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MOTION_COMP_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Configuration Register
//    
//    This register is used to program the various configurable parameters of each DMA
//    channel
//    
// 
// Device Control Register Bits Description:
//    RESERVED_1           [00]      Reserved 
//    NUM_GOC              [10:01]   Number of GOCs (N-1) 
//    NUM_LINES            [22:11]   Number of lines (N-1) 
//    NUM_WORDS            [26:23]   Number of Words per pointer (N-1) 
//    RESERVED_2           [27]      Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_1_CONFIG                                       (APEX_642CL_MOTION_COMP_CH_1_CONFIG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_CONFIG_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_CONFIG_BASE_ADDRESS                          (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_CONFIG_OFFSET                                (0x00000004)
#define APEX_642CL_MOTION_COMP_CH_1_CONFIG_DEFAULT                               (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_CONFIG_TAG
{
   struct _asApex642clMotionCompChConfigBitFieldTag
   {
      REG32  Reserved1            :  1; //  00     APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_RESERVED_1
      REG32  NumGoc               : 10; //  10:01  APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_NUM_GOC
      REG32  NumLines             : 12; //  22:11  APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_NUM_LINES
      REG32  NumWords             :  4; //  26:23  APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_NUM_WORDS
      REG32  Reserved2            :  1; //  27     APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_RESERVED_2
      REG32  LastSegment          :  1; //  28     APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LAST_SEGMENT
      REG32  LoopOnLast           :  1; //  29     APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LOOP_ON_LAST
      REG32  InterruptEnable      :  1; //  30     APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_INTERRUPT_ENABLE
      REG32  EnablePause          :  1; //  31     APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_ENABLE_PAUSE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_CONFIG_TAG                              APEX_642CL_MOTION_COMP_CH_CONFIG_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_CONFIG_DCR                             gApex642clMotionCompChConfig;
extern volatile APEX_642CL_MOTION_COMP_CH_CONFIG_DCR*                            gpApex642clMotionCompChConfig;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_RESERVED_1                          0
#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_NUM_GOC                             1
#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_NUM_LINES                           11
#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_NUM_WORDS                           23
#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_RESERVED_2                          27
#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LAST_SEGMENT                        28

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LAST_SEGMENT_ENUM                     
{
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LAST_SEGMENT_ID_ACTIVE                   =  1,          // Last segment ACTIVE
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LAST_SEGMENT_ID_INACTIVE                 =  0           // Last segment INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LOOP_ON_LAST                        29

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LOOP_ON_LAST_ENUM                     
{
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LOOP_ON_LAST_ID_ACTIVE                   =  1,          // Loop on last ACTIVE
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_LOOP_ON_LAST_ID_INACTIVE                 =  0           // Loop on last INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_INTERRUPT_ENABLE                    30

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_INTERRUPT_ENABLE_ENUM                 
{
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_INTERRUPT_ENABLE_ID_ACTIVE               =  1,          // Interrupt ACTIVE
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_INTERRUPT_ENABLE_ID_INACTIVE             =  0           // Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_ENABLE_PAUSE                        31

// Device Control Register Bits Id and Description
enum APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_ENABLE_PAUSE_ENUM                     
{
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_ENABLE_PAUSE_ID_ACTIVE                   =  1,          // Link list ACTIVE
   APEX_642CL_MOTION_COMP_CH_CONFIG_BIT_ENABLE_PAUSE_ID_INACTIVE                 =  0           // Link list INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENGINE_LINE_POINTER
//    APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    This register does not need to be programmed if linked lists are enabled. It will
//    reflect the contents of the last linked list entry when read
//    
// 
// Device Control Register Bits Description:
//    OFFSET               [31:00]   Pointer Increment used to advance to the next line of the macro block. This is a
//                                   32 bit signed value. Bit 31 is the sign.
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_POINTER                          (APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_POINTER_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_POINTER_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_POINTER_BASE_ADDRESS             (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_POINTER_OFFSET                   (0x00000008)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_POINTER_DEFAULT                  (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_TAG
{
   struct _asApex642clMotionCompChEngineLinePointerBitFieldTag
   {
      REG32  Offset               : 32; //  31:00  APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_BIT_OFFSET
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_TAG                 APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_DCR                gApex642clMotionCompChEngineLinePointer;
extern volatile APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_DCR*               gpApex642clMotionCompChEngineLinePointer;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_POINTER_BIT_OFFSET                 0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENGINE_LINE_MACRO_POINTER
//    APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    This register does not need to be programmed if linked lists are enabled. It will
//    reflect the contents of the last linked list entry when read
//    
// 
// Device Control Register Bits Description:
//    OFFSET               [31:00]   macro pointer 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_0                  (APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_0_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_0_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_0_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_0_OFFSET           (0x0000000c)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_0_DEFAULT          (0x00000000)

#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_1                  (APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_1_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_1_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_1_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_1_OFFSET           (0x00000010)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_1_DEFAULT          (0x00000000)

#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_2                  (APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_2_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_2_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_2_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_2_OFFSET           (0x00000014)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_2_DEFAULT          (0x00000000)

#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_3                  (APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_3_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_3_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_3_BASE_ADDRESS     (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_3_OFFSET           (0x00000018)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LINE_MACRO_POINTER_3_DEFAULT          (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_TAG
{
   struct _asApex642clMotionCompChEngineLineMacroPointerBitFieldTag
   {
      REG32  Offset               : 32; //  31:00  APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_BIT_OFFSET
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_TAG           APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_DCR          gApex642clMotionCompChEngineLineMacroPointer;
extern volatile APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_DCR*         gpApex642clMotionCompChEngineLineMacroPointer;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_ENGINE_LINE_MACRO_POINTER_BIT_OFFSET           0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENGINE_LIST
//    APEX_642CL_MOTION_COMP_CH_ENGINE_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    This register does not need to be programmed if linked list sare enabled. It will
//    reflect the contents of the last linked list entry when read
//    
// 
// Device Control Register Bits Description:
//    RESERVED             [01:00]   Reserved 
//    OFFSET               [31:02]   This is a signed offset that can be applied the addresses located in the pointer
//                                   list table.
//                                   
//                                   Hardware appends 0 to ptr_list_offset[1:0]. If this field is 0x0, then no offset
//                                   is applied. If the field is non_zero, an offset is applied.
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LIST                                  (APEX_642CL_MOTION_COMP_CH_1_ENGINE_LIST_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_ENGINE_LIST_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LIST_BASE_ADDRESS                     (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LIST_OFFSET                           (0x0000001c)
#define APEX_642CL_MOTION_COMP_CH_1_ENGINE_LIST_DEFAULT                          (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_TAG
{
   struct _asApex642clMotionCompChEngineListBitFieldTag
   {
      REG32  Reserved             :  2; //  01:00  APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_BIT_RESERVED
      REG32  Offset               : 30; //  31:02  APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_BIT_OFFSET
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_TAG                         APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_DCR                        gApex642clMotionCompChEngineList;
extern volatile APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_DCR*                       gpApex642clMotionCompChEngineList;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_BIT_RESERVED                       0
#define APEX_642CL_MOTION_COMP_CH_ENGINE_LIST_BIT_OFFSET                         2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DEBUG
//    APEX_642CL_MOTION_COMP_CH_DEBUG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MOTION_COMP_CH_DEBUG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_DEBUG_OFFSET
//    Base Addr:    APEX_642CL_MOTION_COMP_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Address Register
//    
//    This register contains the address of the first entry in the linked list. The
//    linked list address is always on a 32 bit boundary
//    
//    When this register is read it reflects the address of the current linked list
//    entry.
//    
// 
// Device Control Register Bits Description:
//    LIST_CS              [03:00]   Linked list control state machine. 
//    FSM_CS               [07:04]   Engine state machine. 
//    FSM_FIFO_CS          [10:08]   Engine fifo state machine. 
//    RESERVED             [31:11]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_1_DEBUG                                        (APEX_642CL_MOTION_COMP_CH_1_DEBUG_BASE_ADDRESS + APEX_642CL_MOTION_COMP_CH_1_DEBUG_OFFSET)
#define APEX_642CL_MOTION_COMP_CH_1_DEBUG_BASE_ADDRESS                           (APEX_642CL_MOTION_COMP_CH_1)
#define APEX_642CL_MOTION_COMP_CH_1_DEBUG_OFFSET                                 (0x00000020)
#define APEX_642CL_MOTION_COMP_CH_1_DEBUG_DEFAULT                                (0x00000000)


union _APEX_642CL_MOTION_COMP_CH_DEBUG_TAG
{
   struct _asApex642clMotionCompChDebugBitFieldTag
   {
      REG32  ListCs               :  4; //  03:00  APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_LIST_CS
      REG32  FsmCs                :  4; //  07:04  APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_FSM_CS
      REG32  FsmFifoCs            :  3; //  10:08  APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_FSM_FIFO_CS
      REG32  Reserved             : 21; //  31:11  APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MOTION_COMP_CH_DEBUG_TAG                               APEX_642CL_MOTION_COMP_CH_DEBUG_DCR;

extern volatile APEX_642CL_MOTION_COMP_CH_DEBUG_DCR                              gApex642clMotionCompChDebug;
extern volatile APEX_642CL_MOTION_COMP_CH_DEBUG_DCR*                             gpApex642clMotionCompChDebug;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_LIST_CS                              0
#define APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_FSM_CS                               4
#define APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_FSM_FIFO_CS                          8
#define APEX_642CL_MOTION_COMP_CH_DEBUG_BIT_RESERVED                             11

#endif /* APEX642CLMOTIONCOMPMAPTYPE_H */
