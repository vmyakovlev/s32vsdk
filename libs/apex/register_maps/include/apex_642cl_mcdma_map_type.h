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
#ifndef APEX642CLMCDMAMAPTYPE_H
#define APEX642CLMCDMAMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ISR
//    APEX_642CL_MCDMA_ISR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_ISR_BASE_ADDRESS + APEX_642CL_MCDMA_ISR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
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
//    CH                   [00]      Asserted when the DMA channel is complete
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ISR                                                     (APEX_642CL_MCDMA_ISR_BASE_ADDRESS + APEX_642CL_MCDMA_ISR_OFFSET)
#define APEX_642CL_MCDMA_ISR_BASE_ADDRESS                                        (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_ISR_OFFSET                                              (0x00000000)
#define APEX_642CL_MCDMA_ISR_DEFAULT                                             (0x00000000)


union _APEX_642CL_MCDMA_ISR_TAG
{
   struct _asApex642clMcdmaIsrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_ISR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_ISR_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_ISR_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_ISR_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_ISR_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_ISR_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_ISR_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_ISR_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_ISR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_ISR_TAG                                          APEX_642CL_MCDMA_ISR_DCR;

extern volatile APEX_642CL_MCDMA_ISR_DCR                                         gApex642clMcdmaIsr;
extern volatile APEX_642CL_MCDMA_ISR_DCR*                                        gpApex642clMcdmaIsr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ISR_BIT_CH_0                                            0
#define APEX_642CL_MCDMA_ISR_BIT_CH_1                                            1
#define APEX_642CL_MCDMA_ISR_BIT_CH_2                                            2
#define APEX_642CL_MCDMA_ISR_BIT_CH_3                                            3
#define APEX_642CL_MCDMA_ISR_BIT_CH_4                                            4
#define APEX_642CL_MCDMA_ISR_BIT_CH_5                                            5
#define APEX_642CL_MCDMA_ISR_BIT_CH_6                                            6
#define APEX_642CL_MCDMA_ISR_BIT_CH_7                                            7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_ISR_BIT_CH_ENUM                                           
{
   APEX_642CL_MCDMA_ISR_BIT_CH_ID_ACTIVE                                         =  1,          // DMA channel done error source ACTIVE
   APEX_642CL_MCDMA_ISR_BIT_CH_ID_INACTIVE                                       =  0           // DMA channel done error source INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_ISR_BIT_RESERVED                                        8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ESR
//    APEX_642CL_MCDMA_ESR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_ESR_BASE_ADDRESS + APEX_642CL_MCDMA_ESR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
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
//    CH                   [00]      Asserted when the DMA channel experience a AXI error
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ESR                                                     (APEX_642CL_MCDMA_ESR_BASE_ADDRESS + APEX_642CL_MCDMA_ESR_OFFSET)
#define APEX_642CL_MCDMA_ESR_BASE_ADDRESS                                        (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_ESR_OFFSET                                              (0x00000004)
#define APEX_642CL_MCDMA_ESR_DEFAULT                                             (0x00000000)


union _APEX_642CL_MCDMA_ESR_TAG
{
   struct _asApex642clMcdmaEsrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_ESR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_ESR_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_ESR_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_ESR_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_ESR_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_ESR_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_ESR_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_ESR_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_ESR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_ESR_TAG                                          APEX_642CL_MCDMA_ESR_DCR;

extern volatile APEX_642CL_MCDMA_ESR_DCR                                         gApex642clMcdmaEsr;
extern volatile APEX_642CL_MCDMA_ESR_DCR*                                        gpApex642clMcdmaEsr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ESR_BIT_CH_0                                            0
#define APEX_642CL_MCDMA_ESR_BIT_CH_1                                            1
#define APEX_642CL_MCDMA_ESR_BIT_CH_2                                            2
#define APEX_642CL_MCDMA_ESR_BIT_CH_3                                            3
#define APEX_642CL_MCDMA_ESR_BIT_CH_4                                            4
#define APEX_642CL_MCDMA_ESR_BIT_CH_5                                            5
#define APEX_642CL_MCDMA_ESR_BIT_CH_6                                            6
#define APEX_642CL_MCDMA_ESR_BIT_CH_7                                            7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_ESR_BIT_CH_ENUM                                           
{
   APEX_642CL_MCDMA_ESR_BIT_CH_ID_ACTIVE                                         =  1,          // AXI channel error source ACTIVE
   APEX_642CL_MCDMA_ESR_BIT_CH_ID_INACTIVE                                       =  0           // AXI channel error source INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_ESR_BIT_RESERVED                                        8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    EPR
//    APEX_642CL_MCDMA_EPR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_EPR_BASE_ADDRESS + APEX_642CL_MCDMA_EPR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Port Channel Error Source Register
//    
//    This error source does not generate an interrupt but can be used for debug
//    purposes. It does need to be cleared in a similar fashion to an interrupt source
//    
// 
// Device Control Register Bits Description:
//    CH                   [00]      Asserted when the DMA channel expereience a port error
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_EPR                                                     (APEX_642CL_MCDMA_EPR_BASE_ADDRESS + APEX_642CL_MCDMA_EPR_OFFSET)
#define APEX_642CL_MCDMA_EPR_BASE_ADDRESS                                        (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_EPR_OFFSET                                              (0x00000008)
#define APEX_642CL_MCDMA_EPR_DEFAULT                                             (0x00000000)


union _APEX_642CL_MCDMA_EPR_TAG
{
   struct _asApex642clMcdmaEprBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_EPR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_EPR_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_EPR_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_EPR_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_EPR_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_EPR_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_EPR_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_EPR_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_EPR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_EPR_TAG                                          APEX_642CL_MCDMA_EPR_DCR;

extern volatile APEX_642CL_MCDMA_EPR_DCR                                         gApex642clMcdmaEpr;
extern volatile APEX_642CL_MCDMA_EPR_DCR*                                        gpApex642clMcdmaEpr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_EPR_BIT_CH_0                                            0
#define APEX_642CL_MCDMA_EPR_BIT_CH_1                                            1
#define APEX_642CL_MCDMA_EPR_BIT_CH_2                                            2
#define APEX_642CL_MCDMA_EPR_BIT_CH_3                                            3
#define APEX_642CL_MCDMA_EPR_BIT_CH_4                                            4
#define APEX_642CL_MCDMA_EPR_BIT_CH_5                                            5
#define APEX_642CL_MCDMA_EPR_BIT_CH_6                                            6
#define APEX_642CL_MCDMA_EPR_BIT_CH_7                                            7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_EPR_BIT_CH_ENUM                                           
{
   APEX_642CL_MCDMA_EPR_BIT_CH_ID_ACTIVE                                         =  1,          // Port channel error source ACTIVE
   APEX_642CL_MCDMA_EPR_BIT_CH_ID_INACTIVE                                       =  0           // Port channel error source INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_EPR_BIT_RESERVED                                        8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IMR
//    APEX_642CL_MCDMA_IMR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_IMR_BASE_ADDRESS + APEX_642CL_MCDMA_IMR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h0000000c
//    Default:      32'h000000ff
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
//    CH                   [00]      Mask the channel done event from the channel
//                                   
//                                   0: Interrupt is not mask
//                                   1: Interrupt is mask
//                                    
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_IMR                                                     (APEX_642CL_MCDMA_IMR_BASE_ADDRESS + APEX_642CL_MCDMA_IMR_OFFSET)
#define APEX_642CL_MCDMA_IMR_BASE_ADDRESS                                        (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_IMR_OFFSET                                              (0x0000000c)
#define APEX_642CL_MCDMA_IMR_DEFAULT                                             (0x000000ff)


union _APEX_642CL_MCDMA_IMR_TAG
{
   struct _asApex642clMcdmaImrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_IMR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_IMR_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_IMR_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_IMR_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_IMR_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_IMR_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_IMR_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_IMR_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_IMR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_IMR_TAG                                          APEX_642CL_MCDMA_IMR_DCR;

extern volatile APEX_642CL_MCDMA_IMR_DCR                                         gApex642clMcdmaImr;
extern volatile APEX_642CL_MCDMA_IMR_DCR*                                        gpApex642clMcdmaImr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_IMR_BIT_CH_0                                            0
#define APEX_642CL_MCDMA_IMR_BIT_CH_1                                            1
#define APEX_642CL_MCDMA_IMR_BIT_CH_2                                            2
#define APEX_642CL_MCDMA_IMR_BIT_CH_3                                            3
#define APEX_642CL_MCDMA_IMR_BIT_CH_4                                            4
#define APEX_642CL_MCDMA_IMR_BIT_CH_5                                            5
#define APEX_642CL_MCDMA_IMR_BIT_CH_6                                            6
#define APEX_642CL_MCDMA_IMR_BIT_CH_7                                            7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_IMR_BIT_CH_ENUM                                           
{
   APEX_642CL_MCDMA_IMR_BIT_CH_ID_ACTIVE                                         =  1,          // Mask channel done interrupt source ACTIVE
   APEX_642CL_MCDMA_IMR_BIT_CH_ID_INACTIVE                                       =  0           // Mask channel done interrupt source INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_IMR_BIT_RESERVED                                        8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RESERVED
//    APEX_642CL_MCDMA_RESERVED
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_RESERVED_BASE_ADDRESS + APEX_642CL_MCDMA_RESERVED_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         ni
//    Nb:         1
//    Size:       4
// 
// Description:
//    Reserved
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_RESERVED                                                (APEX_642CL_MCDMA_RESERVED_BASE_ADDRESS + APEX_642CL_MCDMA_RESERVED_OFFSET)
#define APEX_642CL_MCDMA_RESERVED_BASE_ADDRESS                                   (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_RESERVED_OFFSET                                         (0x00000010)
#define APEX_642CL_MCDMA_RESERVED_DEFAULT                                        (0x00000000)


union _APEX_642CL_MCDMA_RESERVED_TAG
{
   struct _asApex642clMcdmaReservedBitFieldTag
   {
      REG32 Data;
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_RESERVED_TAG                                     APEX_642CL_MCDMA_RESERVED_DCR;

extern volatile APEX_642CL_MCDMA_RESERVED_DCR                                    gApex642clMcdmaReserved;
extern volatile APEX_642CL_MCDMA_RESERVED_DCR*                                   gpApex642clMcdmaReserved;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ICR
//    APEX_642CL_MCDMA_ICR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_ICR_BASE_ADDRESS + APEX_642CL_MCDMA_ICR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000014
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
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ICR                                                     (APEX_642CL_MCDMA_ICR_BASE_ADDRESS + APEX_642CL_MCDMA_ICR_OFFSET)
#define APEX_642CL_MCDMA_ICR_BASE_ADDRESS                                        (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_ICR_OFFSET                                              (0x00000014)
#define APEX_642CL_MCDMA_ICR_DEFAULT                                             (0x00000000)


union _APEX_642CL_MCDMA_ICR_TAG
{
   struct _asApex642clMcdmaIcrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_ICR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_ICR_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_ICR_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_ICR_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_ICR_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_ICR_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_ICR_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_ICR_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_ICR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_ICR_TAG                                          APEX_642CL_MCDMA_ICR_DCR;

extern volatile APEX_642CL_MCDMA_ICR_DCR                                         gApex642clMcdmaIcr;
extern volatile APEX_642CL_MCDMA_ICR_DCR*                                        gpApex642clMcdmaIcr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ICR_BIT_CH_0                                            0
#define APEX_642CL_MCDMA_ICR_BIT_CH_1                                            1
#define APEX_642CL_MCDMA_ICR_BIT_CH_2                                            2
#define APEX_642CL_MCDMA_ICR_BIT_CH_3                                            3
#define APEX_642CL_MCDMA_ICR_BIT_CH_4                                            4
#define APEX_642CL_MCDMA_ICR_BIT_CH_5                                            5
#define APEX_642CL_MCDMA_ICR_BIT_CH_6                                            6
#define APEX_642CL_MCDMA_ICR_BIT_CH_7                                            7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_ICR_BIT_CH_ENUM                                           
{
   APEX_642CL_MCDMA_ICR_BIT_CH_ID_ACTIVE                                         =  1,          // Clear channel done interrupt source ACTIVE
   APEX_642CL_MCDMA_ICR_BIT_CH_ID_INACTIVE                                       =  0           // Clear channel done interrupt source INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_ICR_BIT_RESERVED                                        8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ECR
//    APEX_642CL_MCDMA_ECR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_ECR_BASE_ADDRESS + APEX_642CL_MCDMA_ECR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000018
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
//    CH                   [00]      Clear the DMA channel error when written '1'
//                                   
//                                   0: nothing
//                                   1: clear error
//                                    
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ECR                                                     (APEX_642CL_MCDMA_ECR_BASE_ADDRESS + APEX_642CL_MCDMA_ECR_OFFSET)
#define APEX_642CL_MCDMA_ECR_BASE_ADDRESS                                        (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_ECR_OFFSET                                              (0x00000018)
#define APEX_642CL_MCDMA_ECR_DEFAULT                                             (0x00000000)


union _APEX_642CL_MCDMA_ECR_TAG
{
   struct _asApex642clMcdmaEcrBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_ECR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_ECR_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_ECR_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_ECR_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_ECR_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_ECR_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_ECR_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_ECR_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_ECR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_ECR_TAG                                          APEX_642CL_MCDMA_ECR_DCR;

extern volatile APEX_642CL_MCDMA_ECR_DCR                                         gApex642clMcdmaEcr;
extern volatile APEX_642CL_MCDMA_ECR_DCR*                                        gpApex642clMcdmaEcr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_ECR_BIT_CH_0                                            0
#define APEX_642CL_MCDMA_ECR_BIT_CH_1                                            1
#define APEX_642CL_MCDMA_ECR_BIT_CH_2                                            2
#define APEX_642CL_MCDMA_ECR_BIT_CH_3                                            3
#define APEX_642CL_MCDMA_ECR_BIT_CH_4                                            4
#define APEX_642CL_MCDMA_ECR_BIT_CH_5                                            5
#define APEX_642CL_MCDMA_ECR_BIT_CH_6                                            6
#define APEX_642CL_MCDMA_ECR_BIT_CH_7                                            7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_ECR_BIT_CH_ENUM                                           
{
   APEX_642CL_MCDMA_ECR_BIT_CH_ID_ACTIVE                                         =  1,          // Clear interrupt error and port source ACTIVE
   APEX_642CL_MCDMA_ECR_BIT_CH_ID_INACTIVE                                       =  0           // Clear interrupt error and port source INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_ECR_BIT_RESERVED                                        8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DMA_CONTROL
//    APEX_642CL_MCDMA_DMA_CONTROL
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_DMA_CONTROL_BASE_ADDRESS + APEX_642CL_MCDMA_DMA_CONTROL_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMA Control Register
//    
//    This register is for DMA control purposes
//    
// 
// Device Control Register Bits Description:
//    DMA_SUSPEND          [00]      This bit when set, is used to suspend the MC_DMA from any AXI or side channel
//                                   activities. Resetting to '0' removes the MC_DMA from being suspended. Bit 31
//                                   provides the status of the MC_DMA
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    DMA_FLUSH            [01]      This bit when set will create a pulse to flush the internal FIFO (ie. setting both
//                                   pointers read and write to zero and the flag status to the appropriate value).
//                                   Read back will always be '0' for this bit.  
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED             [30:02]   Reserved 
//    DMA_SUSPEND_STATUS   [31]      This bit refelcts the status of the DMA suspend operation.  
//                                   
//                                   0: MC DMA is Active, not suspended
//                                   1: MC DMA is Inactive suspended
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_DMA_CONTROL                                             (APEX_642CL_MCDMA_DMA_CONTROL_BASE_ADDRESS + APEX_642CL_MCDMA_DMA_CONTROL_OFFSET)
#define APEX_642CL_MCDMA_DMA_CONTROL_BASE_ADDRESS                                (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_DMA_CONTROL_OFFSET                                      (0x0000001c)
#define APEX_642CL_MCDMA_DMA_CONTROL_DEFAULT                                     (0x00000000)


union _APEX_642CL_MCDMA_DMA_CONTROL_TAG
{
   struct _asApex642clMcdmaDmaControlBitFieldTag
   {
      REG32  DmaSuspend           :  1; //  00     APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND
      REG32  DmaFlush             :  1; //  01     APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_FLUSH
      REG32  Reserved             : 29; //  30:02  APEX_642CL_MCDMA_DMA_CONTROL_BIT_RESERVED
      REG32  DmaSuspendStatus     :  1; //  31     APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_DMA_CONTROL_TAG                                  APEX_642CL_MCDMA_DMA_CONTROL_DCR;

extern volatile APEX_642CL_MCDMA_DMA_CONTROL_DCR                                 gApex642clMcdmaDmaControl;
extern volatile APEX_642CL_MCDMA_DMA_CONTROL_DCR*                                gpApex642clMcdmaDmaControl;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND                             0

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_ENUM                          
{
   APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_ID_ACTIVE                        =  1,          // MC DMA suspend ACTIVE
   APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_ID_INACTIVE                      =  0           // MC DMA suspend INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_FLUSH                               1

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_FLUSH_ENUM                            
{
   APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_FLUSH_ID_ACTIVE                          =  1,          // MC DMA flush internal fifo ACTIVE
   APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_FLUSH_ID_INACTIVE                        =  0           // MC DMA flush internal fifo INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_DMA_CONTROL_BIT_RESERVED                                2
#define APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS                      31

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS_ENUM                   
{
   APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS_ID_ACTIVE                 =  1,          // MC DMA is ACTIVE, not suspended
   APEX_642CL_MCDMA_DMA_CONTROL_BIT_DMA_SUSPEND_STATUS_ID_INACTIVE               =  0           // MC DMA is INACTIVE, suspended
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CH_ENABLE
//    APEX_642CL_MCDMA_CH_ENABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_ENABLE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_ENABLE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000020
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
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_ENABLE                                               (APEX_642CL_MCDMA_CH_ENABLE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_ENABLE_OFFSET)
#define APEX_642CL_MCDMA_CH_ENABLE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_ENABLE_OFFSET                                        (0x00000020)
#define APEX_642CL_MCDMA_CH_ENABLE_DEFAULT                                       (0x00000000)


union _APEX_642CL_MCDMA_CH_ENABLE_TAG
{
   struct _asApex642clMcdmaChEnableBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_CH_ENABLE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_ENABLE_TAG                                    APEX_642CL_MCDMA_CH_ENABLE_DCR;

extern volatile APEX_642CL_MCDMA_CH_ENABLE_DCR                                   gApex642clMcdmaChEnable;
extern volatile APEX_642CL_MCDMA_CH_ENABLE_DCR*                                  gpApex642clMcdmaChEnable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_0                                      0
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_1                                      1
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_2                                      2
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_3                                      3
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_4                                      4
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_5                                      5
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_6                                      6
#define APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_7                                      7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_ENUM                                     
{
   APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_ID_ACTIVE                                   =  1,          // Enable DMA channel ACTIVE
   APEX_642CL_MCDMA_CH_ENABLE_BIT_CH_ID_INACTIVE                                 =  0           // Enable DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_CH_ENABLE_BIT_RESERVED                                  8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CH_CLEAR
//    APEX_642CL_MCDMA_CH_CLEAR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CLEAR_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CLEAR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000024
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
//    CH                   [00]      Clear for DMA channel
//                                   
//                                   0: nothing
//                                   1: clear DMA channel enable
//                                    
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_CLEAR                                                (APEX_642CL_MCDMA_CH_CLEAR_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CLEAR_OFFSET)
#define APEX_642CL_MCDMA_CH_CLEAR_BASE_ADDRESS                                   (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_CLEAR_OFFSET                                         (0x00000024)
#define APEX_642CL_MCDMA_CH_CLEAR_DEFAULT                                        (0x00000000)


union _APEX_642CL_MCDMA_CH_CLEAR_TAG
{
   struct _asApex642clMcdmaChClearBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_CH_CLEAR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_CLEAR_TAG                                     APEX_642CL_MCDMA_CH_CLEAR_DCR;

extern volatile APEX_642CL_MCDMA_CH_CLEAR_DCR                                    gApex642clMcdmaChClear;
extern volatile APEX_642CL_MCDMA_CH_CLEAR_DCR*                                   gpApex642clMcdmaChClear;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_0                                       0
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_1                                       1
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_2                                       2
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_3                                       3
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_4                                       4
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_5                                       5
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_6                                       6
#define APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_7                                       7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_ENUM                                      
{
   APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_ID_ACTIVE                                    =  1,          // Clear DMA channel ACTIVE
   APEX_642CL_MCDMA_CH_CLEAR_BIT_CH_ID_INACTIVE                                  =  0           // Clear DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_CH_CLEAR_BIT_RESERVED                                   8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CH_FLUSH
//    APEX_642CL_MCDMA_CH_FLUSH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_FLUSH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_FLUSH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Flush Register
//    
//    The channel flush register provides a mechanism for software to terminate an
//    active DMA channel gracefully. The channel flush is only applicable "peripheral to
//    memory" and "peripheral to peripheral" configurations.
//    
//    The flush takes a snapshot of the state of the sourcing peripheral and drains that
//    amount of data before the channel is terminated
//    
//    The bit is cleared by the hardware when the flush is done.
// 
// Device Control Register Bits Description:
//    CH                   [00]      Flush for DMA channel
//                                   
//                                   0: nothing
//                                   1: clear DMA channel enable
//                                    
//    RESERVED             [31:08]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_FLUSH                                                (APEX_642CL_MCDMA_CH_FLUSH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_FLUSH_OFFSET)
#define APEX_642CL_MCDMA_CH_FLUSH_BASE_ADDRESS                                   (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_FLUSH_OFFSET                                         (0x00000028)
#define APEX_642CL_MCDMA_CH_FLUSH_DEFAULT                                        (0x00000000)


union _APEX_642CL_MCDMA_CH_FLUSH_TAG
{
   struct _asApex642clMcdmaChFlushBitFieldTag
   {
      REG32  Ch0                  :  1; //  0      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_0
      REG32  Ch1                  :  1; //  1      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_1
      REG32  Ch2                  :  1; //  2      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_2
      REG32  Ch3                  :  1; //  3      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_3
      REG32  Ch4                  :  1; //  4      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_4
      REG32  Ch5                  :  1; //  5      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_5
      REG32  Ch6                  :  1; //  6      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_6
      REG32  Ch7                  :  1; //  7      APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_7
      REG32  Reserved             : 24; //  31:08  APEX_642CL_MCDMA_CH_FLUSH_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_FLUSH_TAG                                     APEX_642CL_MCDMA_CH_FLUSH_DCR;

extern volatile APEX_642CL_MCDMA_CH_FLUSH_DCR                                    gApex642clMcdmaChFlush;
extern volatile APEX_642CL_MCDMA_CH_FLUSH_DCR*                                   gpApex642clMcdmaChFlush;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_0                                       0
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_1                                       1
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_2                                       2
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_3                                       3
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_4                                       4
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_5                                       5
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_6                                       6
#define APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_7                                       7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_ENUM                                      
{
   APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_ID_ACTIVE                                    =  1,          // Flush DMA channel ACTIVE
   APEX_642CL_MCDMA_CH_FLUSH_BIT_CH_ID_INACTIVE                                  =  0           // Flush DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_CH_FLUSH_BIT_RESERVED                                   8

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    INTERRUPT
//    APEX_642CL_MCDMA_INTERRUPT
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_INTERRUPT_BASE_ADDRESS + APEX_642CL_MCDMA_INTERRUPT_OFFSET
//    Base Addr:    APEX_642CL_MCDMA
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Vector Register
// 
// Device Control Register Bits Description:
//    VECTOR               [04:00]   A vector which corresponds to the channel number that is done. The number that is reported is based on the priority that is set for a particular channel. Any channel set with a priority of '0' get reported first followed by '1' to '7'. A null vector is all '1's
//                                    
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_INTERRUPT                                               (APEX_642CL_MCDMA_INTERRUPT_BASE_ADDRESS + APEX_642CL_MCDMA_INTERRUPT_OFFSET)
#define APEX_642CL_MCDMA_INTERRUPT_BASE_ADDRESS                                  (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_INTERRUPT_OFFSET                                        (0x0000002c)
#define APEX_642CL_MCDMA_INTERRUPT_DEFAULT                                       (0x00000000)


union _APEX_642CL_MCDMA_INTERRUPT_TAG
{
   struct _asApex642clMcdmaInterruptBitFieldTag
   {
      REG32  Vector               :  5; //  04:00  APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR
      REG32  Reserved             : 27; //  31:05  APEX_642CL_MCDMA_INTERRUPT_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_INTERRUPT_TAG                                    APEX_642CL_MCDMA_INTERRUPT_DCR;

extern volatile APEX_642CL_MCDMA_INTERRUPT_DCR                                   gApex642clMcdmaInterrupt;
extern volatile APEX_642CL_MCDMA_INTERRUPT_DCR*                                  gpApex642clMcdmaInterrupt;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR                                    0

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ENUM                                 
{
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_0                                =  0,          // Interrupt  DMA_0
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_1                                =  1,          // Interrupt  DMA_1
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_2                                =  2,          // Interrupt  DMA_2
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_3                                =  3,          // Interrupt  DMA_3
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_4                                =  4,          // Interrupt  DMA_4
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_5                                =  5,          // Interrupt  DMA_5
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_6                                =  6,          // Interrupt  DMA_6
   APEX_642CL_MCDMA_INTERRUPT_BIT_VECTOR_ID_DMA_7                                =  7           // Interrupt  DMA_7
};
                                                                             

#define APEX_642CL_MCDMA_INTERRUPT_BIT_RESERVED                                  5

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
#define APEX_642CL_MCDMA_MEMORY_FILL_0                                           (APEX_642CL_MCDMA_MEMORY_FILL_0_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_0_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_BASE_ADDRESS                              (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_OFFSET                                    (0x00000030)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_SIZE                                      (0x00000010)

#define APEX_642CL_MCDMA_MEMORY_FILL_1                                           (APEX_642CL_MCDMA_MEMORY_FILL_1_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_1_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_BASE_ADDRESS                              (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_OFFSET                                    (0x00000040)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_SIZE                                      (0x00000010)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD
//    APEX_642CL_MCDMA_MEMORY_FILL_WORD
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_MEMORY_FILL_WORD_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_WORD_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_MEMORY_FILL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Memory Fill Word Register
//    
//    The fill word is associated with a source peripheral 6. It acts as an infinitely
//    deep fifo that contains the same value. 
//    
//    The content that it uses to fill the memory location is based on the wordsize.
//    
//    Word Size:
//       128: {word3, word2,word1,word0} 
//       64:  {word1,word0} 
//       32:  {word0}
//       16:  {word0[15:0]}
//       8:   {word0[7:0]}
//    
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_0                                    (APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_0_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_0_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_0_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_0)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_0_OFFSET                             (0x00000000)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_0_DEFAULT                            (0x00000000)

#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_1                                    (APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_1_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_1_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_1_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_0)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_1_OFFSET                             (0x00000004)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_1_DEFAULT                            (0x00000000)

#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_2                                    (APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_2_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_2_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_2_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_0)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_2_OFFSET                             (0x00000008)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_2_DEFAULT                            (0x00000000)

#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_3                                    (APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_3_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_3_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_3_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_0)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_3_OFFSET                             (0x0000000c)
#define APEX_642CL_MCDMA_MEMORY_FILL_0_WORD_3_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WORD
//    APEX_642CL_MCDMA_MEMORY_FILL_WORD
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_MEMORY_FILL_WORD_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_WORD_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_MEMORY_FILL
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Memory Fill Word Register
//    
//    The fill word is associated with a source peripheral 6. It acts as an infinitely
//    deep fifo that contains the same value. 
//    
//    The content that it uses to fill the memory location is based on the wordsize.
//    
//    Word Size:
//       128: {word3, word2,word1,word0} 
//       64:  {word1,word0} 
//       32:  {word0}
//       16:  {word0[15:0]}
//       8:   {word0[7:0]}
//    
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_0                                    (APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_0_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_0_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_0_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_1)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_0_OFFSET                             (0x00000000)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_0_DEFAULT                            (0x00000000)

#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_1                                    (APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_1_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_1_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_1_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_1)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_1_OFFSET                             (0x00000004)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_1_DEFAULT                            (0x00000000)

#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_2                                    (APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_2_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_2_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_2_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_1)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_2_OFFSET                             (0x00000008)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_2_DEFAULT                            (0x00000000)

#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_3                                    (APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_3_BASE_ADDRESS + APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_3_OFFSET)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_3_BASE_ADDRESS                       (APEX_642CL_MCDMA_MEMORY_FILL_1)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_3_OFFSET                             (0x0000000c)
#define APEX_642CL_MCDMA_MEMORY_FILL_1_WORD_3_DEFAULT                            (0x00000000)


union _APEX_642CL_MCDMA_MEMORY_FILL_WORD_TAG
{
   struct _asApex642clMcdmaMemoryFillWordBitFieldTag
   {
      REG32 Data;
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_MEMORY_FILL_WORD_TAG                             APEX_642CL_MCDMA_MEMORY_FILL_WORD_DCR;

extern volatile APEX_642CL_MCDMA_MEMORY_FILL_WORD_DCR                            gApex642clMcdmaMemoryFillWord;
extern volatile APEX_642CL_MCDMA_MEMORY_FILL_WORD_DCR*                           gpApex642clMcdmaMemoryFillWord;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////

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
#define APEX_642CL_MCDMA_PERF                                                    (APEX_642CL_MCDMA_PERF_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_OFFSET)
#define APEX_642CL_MCDMA_PERF_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_PERF_OFFSET                                             (0x00000050)
#define APEX_642CL_MCDMA_PERF_SIZE                                               (0x00000010)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONTROL
//    APEX_642CL_MCDMA_PERF_CONTROL
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_PERF_CONTROL_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_CONTROL_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_PERF
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
#define APEX_642CL_MCDMA_PERF_CONTROL                                            (APEX_642CL_MCDMA_PERF_CONTROL_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_CONTROL_OFFSET)
#define APEX_642CL_MCDMA_PERF_CONTROL_BASE_ADDRESS                               (APEX_642CL_MCDMA_PERF)
#define APEX_642CL_MCDMA_PERF_CONTROL_OFFSET                                     (0x00000000)
#define APEX_642CL_MCDMA_PERF_CONTROL_DEFAULT                                    (0x00000000)


union _APEX_642CL_MCDMA_PERF_CONTROL_TAG
{
   struct _asApex642clMcdmaPerfControlBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_MCDMA_PERF_CONTROL_BIT_ENABLE
      REG32  Reserved             : 31; //  31:01  APEX_642CL_MCDMA_PERF_CONTROL_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_PERF_CONTROL_TAG                                 APEX_642CL_MCDMA_PERF_CONTROL_DCR;

extern volatile APEX_642CL_MCDMA_PERF_CONTROL_DCR                                gApex642clMcdmaPerfControl;
extern volatile APEX_642CL_MCDMA_PERF_CONTROL_DCR*                               gpApex642clMcdmaPerfControl;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_PERF_CONTROL_BIT_ENABLE                                 0

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_PERF_CONTROL_BIT_ENABLE_ENUM                              
{
   APEX_642CL_MCDMA_PERF_CONTROL_BIT_ENABLE_ID_ACTIVE                            =  1,          // Performance Counter ACTIVE
   APEX_642CL_MCDMA_PERF_CONTROL_BIT_ENABLE_ID_INACTIVE                          =  0           // Performance Counter INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_PERF_CONTROL_BIT_RESERVED                               1

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TOTAL
//    APEX_642CL_MCDMA_PERF_TOTAL
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_PERF_TOTAL_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_TOTAL_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_PERF
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
#define APEX_642CL_MCDMA_PERF_TOTAL                                              (APEX_642CL_MCDMA_PERF_TOTAL_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_TOTAL_OFFSET)
#define APEX_642CL_MCDMA_PERF_TOTAL_BASE_ADDRESS                                 (APEX_642CL_MCDMA_PERF)
#define APEX_642CL_MCDMA_PERF_TOTAL_OFFSET                                       (0x00000004)
#define APEX_642CL_MCDMA_PERF_TOTAL_DEFAULT                                      (0x00000000)


union _APEX_642CL_MCDMA_PERF_TOTAL_TAG
{
   struct _asApex642clMcdmaPerfTotalBitFieldTag
   {
      REG32  Count                : 32; //  31:00  APEX_642CL_MCDMA_PERF_TOTAL_BIT_COUNT
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_PERF_TOTAL_TAG                                   APEX_642CL_MCDMA_PERF_TOTAL_DCR;

extern volatile APEX_642CL_MCDMA_PERF_TOTAL_DCR                                  gApex642clMcdmaPerfTotal;
extern volatile APEX_642CL_MCDMA_PERF_TOTAL_DCR*                                 gpApex642clMcdmaPerfTotal;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_PERF_TOTAL_BIT_COUNT                                    0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BUSRD
//    APEX_642CL_MCDMA_PERF_BUSRD
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_PERF_BUSRD_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_BUSRD_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_PERF
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
#define APEX_642CL_MCDMA_PERF_BUSRD                                              (APEX_642CL_MCDMA_PERF_BUSRD_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_BUSRD_OFFSET)
#define APEX_642CL_MCDMA_PERF_BUSRD_BASE_ADDRESS                                 (APEX_642CL_MCDMA_PERF)
#define APEX_642CL_MCDMA_PERF_BUSRD_OFFSET                                       (0x00000008)
#define APEX_642CL_MCDMA_PERF_BUSRD_DEFAULT                                      (0x00000000)


union _APEX_642CL_MCDMA_PERF_BUSRD_TAG
{
   struct _asApex642clMcdmaPerfBusrdBitFieldTag
   {
      REG32  Count                : 32; //  31:00  APEX_642CL_MCDMA_PERF_BUSRD_BIT_COUNT
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_PERF_BUSRD_TAG                                   APEX_642CL_MCDMA_PERF_BUSRD_DCR;

extern volatile APEX_642CL_MCDMA_PERF_BUSRD_DCR                                  gApex642clMcdmaPerfBusrd;
extern volatile APEX_642CL_MCDMA_PERF_BUSRD_DCR*                                 gpApex642clMcdmaPerfBusrd;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_PERF_BUSRD_BIT_COUNT                                    0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BUSWR
//    APEX_642CL_MCDMA_PERF_BUSWR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_PERF_BUSWR_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_BUSWR_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_PERF
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Performance Bus write Count
// 
// Device Control Register Bits Description:
//    COUNT                [31:00]   This counter (when enabled) increments whenever the AXI write channel is active. The counter  is cleared and starts counting when the performance counter is enabled. It holds the last count value when the performance counter is disabled. A read of the register provides the current count. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_PERF_BUSWR                                              (APEX_642CL_MCDMA_PERF_BUSWR_BASE_ADDRESS + APEX_642CL_MCDMA_PERF_BUSWR_OFFSET)
#define APEX_642CL_MCDMA_PERF_BUSWR_BASE_ADDRESS                                 (APEX_642CL_MCDMA_PERF)
#define APEX_642CL_MCDMA_PERF_BUSWR_OFFSET                                       (0x0000000c)
#define APEX_642CL_MCDMA_PERF_BUSWR_DEFAULT                                      (0x00000000)


union _APEX_642CL_MCDMA_PERF_BUSWR_TAG
{
   struct _asApex642clMcdmaPerfBuswrBitFieldTag
   {
      REG32  Count                : 32; //  31:00  APEX_642CL_MCDMA_PERF_BUSWR_BIT_COUNT
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_PERF_BUSWR_TAG                                   APEX_642CL_MCDMA_PERF_BUSWR_DCR;

extern volatile APEX_642CL_MCDMA_PERF_BUSWR_DCR                                  gApex642clMcdmaPerfBuswr;
extern volatile APEX_642CL_MCDMA_PERF_BUSWR_DCR*                                 gpApex642clMcdmaPerfBuswr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_PERF_BUSWR_BIT_COUNT                                    0

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
#define APEX_642CL_MCDMA_CH_0                                                    (APEX_642CL_MCDMA_CH_0_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_OFFSET)
#define APEX_642CL_MCDMA_CH_0_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_0_OFFSET                                             (0x00000100)
#define APEX_642CL_MCDMA_CH_0_SIZE                                               (0x00000100)

#define APEX_642CL_MCDMA_CH_1                                                    (APEX_642CL_MCDMA_CH_1_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_OFFSET)
#define APEX_642CL_MCDMA_CH_1_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_1_OFFSET                                             (0x00000200)
#define APEX_642CL_MCDMA_CH_1_SIZE                                               (0x00000100)

#define APEX_642CL_MCDMA_CH_2                                                    (APEX_642CL_MCDMA_CH_2_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_OFFSET)
#define APEX_642CL_MCDMA_CH_2_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_2_OFFSET                                             (0x00000300)
#define APEX_642CL_MCDMA_CH_2_SIZE                                               (0x00000100)

#define APEX_642CL_MCDMA_CH_3                                                    (APEX_642CL_MCDMA_CH_3_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_OFFSET)
#define APEX_642CL_MCDMA_CH_3_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_3_OFFSET                                             (0x00000400)
#define APEX_642CL_MCDMA_CH_3_SIZE                                               (0x00000100)

#define APEX_642CL_MCDMA_CH_4                                                    (APEX_642CL_MCDMA_CH_4_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_OFFSET)
#define APEX_642CL_MCDMA_CH_4_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_4_OFFSET                                             (0x00000500)
#define APEX_642CL_MCDMA_CH_4_SIZE                                               (0x00000100)

#define APEX_642CL_MCDMA_CH_5                                                    (APEX_642CL_MCDMA_CH_5_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_OFFSET)
#define APEX_642CL_MCDMA_CH_5_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_5_OFFSET                                             (0x00000600)
#define APEX_642CL_MCDMA_CH_5_SIZE                                               (0x00000100)

#define APEX_642CL_MCDMA_CH_6                                                    (APEX_642CL_MCDMA_CH_6_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_OFFSET)
#define APEX_642CL_MCDMA_CH_6_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_6_OFFSET                                             (0x00000700)
#define APEX_642CL_MCDMA_CH_6_SIZE                                               (0x00000100)

#define APEX_642CL_MCDMA_CH_7                                                    (APEX_642CL_MCDMA_CH_7_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_OFFSET)
#define APEX_642CL_MCDMA_CH_7_BASE_ADDRESS                                       (APEX_642CL_MCDMA_BASE_ADDRESS)
#define APEX_642CL_MCDMA_CH_7_OFFSET                                             (0x00000800)
#define APEX_642CL_MCDMA_CH_7_SIZE                                               (0x00000100)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_SRC                                                (APEX_642CL_MCDMA_CH_0_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_0_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_0_SRC_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_DST                                                (APEX_642CL_MCDMA_CH_0_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_0_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_0_DST_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_0_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_0_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_0_LINKED_LIST_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_CONFIG                                             (APEX_642CL_MCDMA_CH_0_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_0_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_0_CONFIG_DEFAULT                                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_TRANSFER                                           (APEX_642CL_MCDMA_CH_0_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_0_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_0_TRANSFER_DEFAULT                                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_SPAN                                               (APEX_642CL_MCDMA_CH_0_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_0_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_0_SPAN_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_WIDTH                                              (APEX_642CL_MCDMA_CH_0_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_0_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_0_WIDTH_DEFAULT                                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_BYTE                                               (APEX_642CL_MCDMA_CH_0_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_0_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_0_BYTE_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_0_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_0_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_0_STATUS_SRC_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_STATUS_DST                                         (APEX_642CL_MCDMA_CH_0_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_0_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_0_STATUS_DST_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_0_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_0_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_0_STATUS_SHADOW_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_0_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_0_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_0_STATUS_FSM_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_0_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_0_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_0_STATUS_TRANSFER_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_0_PRIORITY                                           (APEX_642CL_MCDMA_CH_0_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_0_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_0_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_0)
#define APEX_642CL_MCDMA_CH_0_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_0_PRIORITY_DEFAULT                                   (0x0000000f)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_SRC                                                (APEX_642CL_MCDMA_CH_1_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_1_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_1_SRC_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_DST                                                (APEX_642CL_MCDMA_CH_1_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_1_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_1_DST_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_1_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_1_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_1_LINKED_LIST_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_CONFIG                                             (APEX_642CL_MCDMA_CH_1_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_1_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_1_CONFIG_DEFAULT                                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_TRANSFER                                           (APEX_642CL_MCDMA_CH_1_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_1_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_1_TRANSFER_DEFAULT                                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_SPAN                                               (APEX_642CL_MCDMA_CH_1_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_1_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_1_SPAN_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_WIDTH                                              (APEX_642CL_MCDMA_CH_1_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_1_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_1_WIDTH_DEFAULT                                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_BYTE                                               (APEX_642CL_MCDMA_CH_1_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_1_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_1_BYTE_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_1_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_1_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_1_STATUS_SRC_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_STATUS_DST                                         (APEX_642CL_MCDMA_CH_1_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_1_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_1_STATUS_DST_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_1_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_1_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_1_STATUS_SHADOW_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_1_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_1_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_1_STATUS_FSM_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_1_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_1_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_1_STATUS_TRANSFER_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_1_PRIORITY                                           (APEX_642CL_MCDMA_CH_1_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_1_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_1_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_1)
#define APEX_642CL_MCDMA_CH_1_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_1_PRIORITY_DEFAULT                                   (0x0000000f)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_SRC                                                (APEX_642CL_MCDMA_CH_2_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_2_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_2_SRC_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_DST                                                (APEX_642CL_MCDMA_CH_2_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_2_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_2_DST_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_2_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_2_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_2_LINKED_LIST_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_CONFIG                                             (APEX_642CL_MCDMA_CH_2_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_2_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_2_CONFIG_DEFAULT                                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_TRANSFER                                           (APEX_642CL_MCDMA_CH_2_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_2_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_2_TRANSFER_DEFAULT                                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_SPAN                                               (APEX_642CL_MCDMA_CH_2_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_2_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_2_SPAN_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_WIDTH                                              (APEX_642CL_MCDMA_CH_2_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_2_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_2_WIDTH_DEFAULT                                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_BYTE                                               (APEX_642CL_MCDMA_CH_2_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_2_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_2_BYTE_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_2_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_2_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_2_STATUS_SRC_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_STATUS_DST                                         (APEX_642CL_MCDMA_CH_2_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_2_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_2_STATUS_DST_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_2_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_2_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_2_STATUS_SHADOW_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_2_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_2_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_2_STATUS_FSM_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_2_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_2_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_2_STATUS_TRANSFER_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_2_PRIORITY                                           (APEX_642CL_MCDMA_CH_2_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_2_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_2_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_2)
#define APEX_642CL_MCDMA_CH_2_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_2_PRIORITY_DEFAULT                                   (0x0000000f)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_SRC                                                (APEX_642CL_MCDMA_CH_3_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_3_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_3_SRC_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_DST                                                (APEX_642CL_MCDMA_CH_3_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_3_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_3_DST_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_3_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_3_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_3_LINKED_LIST_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_CONFIG                                             (APEX_642CL_MCDMA_CH_3_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_3_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_3_CONFIG_DEFAULT                                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_TRANSFER                                           (APEX_642CL_MCDMA_CH_3_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_3_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_3_TRANSFER_DEFAULT                                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_SPAN                                               (APEX_642CL_MCDMA_CH_3_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_3_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_3_SPAN_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_WIDTH                                              (APEX_642CL_MCDMA_CH_3_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_3_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_3_WIDTH_DEFAULT                                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_BYTE                                               (APEX_642CL_MCDMA_CH_3_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_3_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_3_BYTE_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_3_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_3_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_3_STATUS_SRC_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_STATUS_DST                                         (APEX_642CL_MCDMA_CH_3_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_3_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_3_STATUS_DST_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_3_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_3_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_3_STATUS_SHADOW_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_3_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_3_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_3_STATUS_FSM_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_3_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_3_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_3_STATUS_TRANSFER_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_3_PRIORITY                                           (APEX_642CL_MCDMA_CH_3_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_3_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_3_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_3)
#define APEX_642CL_MCDMA_CH_3_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_3_PRIORITY_DEFAULT                                   (0x0000000f)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_SRC                                                (APEX_642CL_MCDMA_CH_4_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_4_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_4_SRC_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_DST                                                (APEX_642CL_MCDMA_CH_4_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_4_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_4_DST_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_4_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_4_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_4_LINKED_LIST_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_CONFIG                                             (APEX_642CL_MCDMA_CH_4_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_4_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_4_CONFIG_DEFAULT                                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_TRANSFER                                           (APEX_642CL_MCDMA_CH_4_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_4_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_4_TRANSFER_DEFAULT                                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_SPAN                                               (APEX_642CL_MCDMA_CH_4_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_4_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_4_SPAN_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_WIDTH                                              (APEX_642CL_MCDMA_CH_4_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_4_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_4_WIDTH_DEFAULT                                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_BYTE                                               (APEX_642CL_MCDMA_CH_4_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_4_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_4_BYTE_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_4_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_4_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_4_STATUS_SRC_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_STATUS_DST                                         (APEX_642CL_MCDMA_CH_4_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_4_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_4_STATUS_DST_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_4_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_4_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_4_STATUS_SHADOW_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_4_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_4_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_4_STATUS_FSM_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_4_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_4_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_4_STATUS_TRANSFER_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_4_PRIORITY                                           (APEX_642CL_MCDMA_CH_4_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_4_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_4_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_4)
#define APEX_642CL_MCDMA_CH_4_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_4_PRIORITY_DEFAULT                                   (0x0000000f)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_SRC                                                (APEX_642CL_MCDMA_CH_5_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_5_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_5_SRC_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_DST                                                (APEX_642CL_MCDMA_CH_5_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_5_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_5_DST_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_5_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_5_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_5_LINKED_LIST_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_CONFIG                                             (APEX_642CL_MCDMA_CH_5_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_5_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_5_CONFIG_DEFAULT                                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_TRANSFER                                           (APEX_642CL_MCDMA_CH_5_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_5_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_5_TRANSFER_DEFAULT                                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_SPAN                                               (APEX_642CL_MCDMA_CH_5_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_5_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_5_SPAN_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_WIDTH                                              (APEX_642CL_MCDMA_CH_5_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_5_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_5_WIDTH_DEFAULT                                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_BYTE                                               (APEX_642CL_MCDMA_CH_5_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_5_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_5_BYTE_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_5_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_5_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_5_STATUS_SRC_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_STATUS_DST                                         (APEX_642CL_MCDMA_CH_5_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_5_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_5_STATUS_DST_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_5_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_5_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_5_STATUS_SHADOW_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_5_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_5_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_5_STATUS_FSM_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_5_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_5_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_5_STATUS_TRANSFER_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_5_PRIORITY                                           (APEX_642CL_MCDMA_CH_5_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_5_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_5_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_5)
#define APEX_642CL_MCDMA_CH_5_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_5_PRIORITY_DEFAULT                                   (0x0000000f)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_SRC                                                (APEX_642CL_MCDMA_CH_6_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_6_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_6_SRC_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_DST                                                (APEX_642CL_MCDMA_CH_6_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_6_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_6_DST_DEFAULT                                        (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_6_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_6_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_6_LINKED_LIST_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_CONFIG                                             (APEX_642CL_MCDMA_CH_6_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_6_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_6_CONFIG_DEFAULT                                     (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_TRANSFER                                           (APEX_642CL_MCDMA_CH_6_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_6_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_6_TRANSFER_DEFAULT                                   (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_SPAN                                               (APEX_642CL_MCDMA_CH_6_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_6_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_6_SPAN_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_WIDTH                                              (APEX_642CL_MCDMA_CH_6_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_6_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_6_WIDTH_DEFAULT                                      (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_BYTE                                               (APEX_642CL_MCDMA_CH_6_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_6_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_6_BYTE_DEFAULT                                       (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_6_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_6_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_6_STATUS_SRC_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_STATUS_DST                                         (APEX_642CL_MCDMA_CH_6_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_6_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_6_STATUS_DST_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_6_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_6_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_6_STATUS_SHADOW_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_6_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_6_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_6_STATUS_FSM_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_6_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_6_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_6_STATUS_TRANSFER_DEFAULT                            (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_6_PRIORITY                                           (APEX_642CL_MCDMA_CH_6_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_6_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_6_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_6)
#define APEX_642CL_MCDMA_CH_6_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_6_PRIORITY_DEFAULT                                   (0x0000000f)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SRC
//    APEX_642CL_MCDMA_CH_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Register
//    
//    This register contains the Source Pointer start address or Source port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Source address
//    Memory to peripheral, Source address
//    Peripheral to memory, Source port number
//    Peripheral to peripheral, Source port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The source peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamOUT0
//    2: streamOUT1
//    3: Motion Comp1
//    4: Motion Comp2
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: memfill0
//    8: memfill1
//    9: HRSZ 0
//    10: HRSZ 1
//    11: reserved
//    12: reserved
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_SRC                                                (APEX_642CL_MCDMA_CH_7_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_7_SRC_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_SRC_OFFSET                                         (0x00000000)
#define APEX_642CL_MCDMA_CH_7_SRC_DEFAULT                                        (0x00000000)


union _APEX_642CL_MCDMA_CH_SRC_TAG
{
   struct _asApex642clMcdmaChSrcBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_MCDMA_CH_SRC_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_SRC_TAG                                       APEX_642CL_MCDMA_CH_SRC_DCR;

extern volatile APEX_642CL_MCDMA_CH_SRC_DCR                                      gApex642clMcdmaChSrc;
extern volatile APEX_642CL_MCDMA_CH_SRC_DCR*                                     gpApex642clMcdmaChSrc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_SRC_BIT_ADDR                                         0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DST
//    APEX_642CL_MCDMA_CH_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    This register contains the Dest. Pointer start address or Dest. port number. The
//    value in this register depends on the type of transfer. Below are the four possible
//    transfer type and the corresponding meaning for this register;
//    
//    Memory to memory, Dest address
//    Memory to peripheral, Dest. Port number
//    Peripheral to memory, Dest address
//    Peripheral to peripheral, Dest Port number
//    
//    This field is also a shadow of the last read linked list.
//    
//    The destination peripheral port mappings are as follows:
//    
//    0: internal fifo
//    1: streamIN0
//    2: streamIN1
//    3: reserved
//    4: reserved
//    5: APU0 DMEM fifo
//    6: APU1 DMEM fifo
//    7: reserved
//    8: reserved
//    9: HRSZ 0
//    10: HRSZ 1
//    11: streamIN2
//    12 streamIN3
//    
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_DST                                                (APEX_642CL_MCDMA_CH_7_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_7_DST_BASE_ADDRESS                                   (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_DST_OFFSET                                         (0x00000004)
#define APEX_642CL_MCDMA_CH_7_DST_DEFAULT                                        (0x00000000)


union _APEX_642CL_MCDMA_CH_DST_TAG
{
   struct _asApex642clMcdmaChDstBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_MCDMA_CH_DST_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_DST_TAG                                       APEX_642CL_MCDMA_CH_DST_DCR;

extern volatile APEX_642CL_MCDMA_CH_DST_DCR                                      gApex642clMcdmaChDst;
extern volatile APEX_642CL_MCDMA_CH_DST_DCR*                                     gpApex642clMcdmaChDst;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_DST_BIT_ADDR                                         0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LINKED_LIST
//    APEX_642CL_MCDMA_CH_LINKED_LIST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_LINKED_LIST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000008
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
//    ENABLE               [00]      When '1' the linked list is enabled and the link list configuration is read from
//                                   the programmed linked_list_address.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    MODE                 [01]      This field is pertinent to linked list mode and indicates whether the linked lists
//                                   are all sequentially located in memory or whether they are scattered throughout
//                                   memory and the "next linked list" descriptor is required to indicate the location
//                                   of the next list.
//                                   
//                                   0: Sequential Mode
//                                   1: Non-Sequential mode
//                                    
//    ADDR                 [31:02]   Link Address (must be 128 bit aligned) 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_LINKED_LIST                                        (APEX_642CL_MCDMA_CH_7_LINKED_LIST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_LINKED_LIST_OFFSET)
#define APEX_642CL_MCDMA_CH_7_LINKED_LIST_BASE_ADDRESS                           (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_LINKED_LIST_OFFSET                                 (0x00000008)
#define APEX_642CL_MCDMA_CH_7_LINKED_LIST_DEFAULT                                (0x00000000)


union _APEX_642CL_MCDMA_CH_LINKED_LIST_TAG
{
   struct _asApex642clMcdmaChLinkedListBitFieldTag
   {
      REG32  Enable               :  1; //  00     APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_ENABLE
      REG32  Mode                 :  1; //  01     APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_MODE
      REG32  Addr                 : 30; //  31:02  APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_LINKED_LIST_TAG                               APEX_642CL_MCDMA_CH_LINKED_LIST_DCR;

extern volatile APEX_642CL_MCDMA_CH_LINKED_LIST_DCR                              gApex642clMcdmaChLinkedList;
extern volatile APEX_642CL_MCDMA_CH_LINKED_LIST_DCR*                             gpApex642clMcdmaChLinkedList;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_ENABLE                               0

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_ENABLE_ENUM                            
{
   APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_ENABLE_ID_ACTIVE                          =  1,          // Link list ACTIVE
   APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_ENABLE_ID_INACTIVE                        =  0           // Link list INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_MODE                                 1

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_MODE_ENUM                              
{
   APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_MODE_ID_SEQUENTIAL                        =  1,          // Link list mode SEQUENTIAL
   APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_MODE_ID_NON_SEQUENTIAL                    =  0           // Link list mode NON_SEQUENTIAL
};
                                                                             

#define APEX_642CL_MCDMA_CH_LINKED_LIST_BIT_ADDR                                 2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG
//    APEX_642CL_MCDMA_CH_CONFIG
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_CONFIG_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000000c
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
//    DST_DATA_FORMAT      [01:00]   Data format
//                                   
//                                   00: 1D mode
//                                   01: 2D mode
//                                   10: 3D mode
//                                   11: 4D mode
//                                    
//    DST_ADDR_MODE        [02]      The address can be programmed in the corresponding "address register" or it can
//                                   indirectly be stored in a memory location that the corresponding "address
//                                   register" points to.
//                                   
//                                   0: direct addressing 
//                                   1: indirect addressing
//                                    
//    DST_TRANSFER_TYPE    [03]      Transfer type.
//                                   
//                                   0: Memory based
//                                   1: Peripheral based 
//    SRC_DATA_FORMAT      [05:04]   See Link 
//    SRC_ADDR_MODE        [06]      See Link 
//    SRC_TRANSFER_TYPE    [07]      See Link 
//    WORD_SIZE            [10:08]   Bus word size
//                                   
//                                   0: 8 bit 
//                                   1: 16 bit 
//                                   2: 32 bit 
//                                   3: 64 bit 
//                                   4: 128 bit
//                                   5,6,7: reserved
//                                    
//    BURST_SIZE           [14:11]   Defines the burst length to be performed on the AXI when possible. The hardware
//                                   will automatically adapt if the programmed burst cannot be met.
//                                   
//                                   0: 1 beat
//                                   1: 2 beats
//                                   2: 3 beats
//                                   ...
//                                   15: 16 beats
//                                    
//    RESERVED             [27:15]   Reserved 
//    LAST_SEGMENT         [28]      Indicates that this link list segment is the last segment. This field is a shadow
//                                   of the last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    LOOP_ON_LAST         [29]      When combined with the "last segment" indicates whether or not to end the channel
//                                   or loop to the first link list segment and continue. This field is a shadow of the
//                                   last read linked list. Writing to this field will not configure the dma.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    INTERRUPT_ENABLE     [30]      Controls whether a "done" interrupt is asserted when the particular segment is
//                                   completed. This field is also a shadow of the last read linked list.
//                                   
//                                   0: Inactive
//                                   1: Active 
//    ENABLE_PAUSE         [31]      When '1' the DMA transfer will stop after the completion of the interrupt.
//                                   
//                                   0: Continue
//                                   1: Pause 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_CONFIG                                             (APEX_642CL_MCDMA_CH_7_CONFIG_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_CONFIG_OFFSET)
#define APEX_642CL_MCDMA_CH_7_CONFIG_BASE_ADDRESS                                (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_CONFIG_OFFSET                                      (0x0000000c)
#define APEX_642CL_MCDMA_CH_7_CONFIG_DEFAULT                                     (0x00000000)


union _APEX_642CL_MCDMA_CH_CONFIG_TAG
{
   struct _asApex642clMcdmaChConfigBitFieldTag
   {
      REG32  DstDataFormat        :  2; //  01:00  APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_DATA_FORMAT
      REG32  DstAddrMode          :  1; //  02     APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_ADDR_MODE
      REG32  DstTransferType      :  1; //  03     APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_TRANSFER_TYPE
      REG32  SrcDataFormat        :  2; //  05:04  APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_DATA_FORMAT
      REG32  SrcAddrMode          :  1; //  06     APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_ADDR_MODE
      REG32  SrcTransferType      :  1; //  07     APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_TRANSFER_TYPE
      REG32  WordSize             :  3; //  10:08  APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE
      REG32  BurstSize            :  4; //  14:11  APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE
      REG32  Reserved             : 13; //  27:15  APEX_642CL_MCDMA_CH_CONFIG_BIT_RESERVED
      REG32  LastSegment          :  1; //  28     APEX_642CL_MCDMA_CH_CONFIG_BIT_LAST_SEGMENT
      REG32  LoopOnLast           :  1; //  29     APEX_642CL_MCDMA_CH_CONFIG_BIT_LOOP_ON_LAST
      REG32  InterruptEnable      :  1; //  30     APEX_642CL_MCDMA_CH_CONFIG_BIT_INTERRUPT_ENABLE
      REG32  EnablePause          :  1; //  31     APEX_642CL_MCDMA_CH_CONFIG_BIT_ENABLE_PAUSE
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_CONFIG_TAG                                    APEX_642CL_MCDMA_CH_CONFIG_DCR;

extern volatile APEX_642CL_MCDMA_CH_CONFIG_DCR                                   gApex642clMcdmaChConfig;
extern volatile APEX_642CL_MCDMA_CH_CONFIG_DCR*                                  gpApex642clMcdmaChConfig;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_DATA_FORMAT                           0

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_DATA_FORMAT_ENUM                        
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_DATA_FORMAT_ID_1D                          =  0,          // Data format mode 1D
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_DATA_FORMAT_ID_2D                          =  1,          // Data format mode 2D
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_DATA_FORMAT_ID_3D                          =  2,          // Data format mode 3D
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_DATA_FORMAT_ID_4D                          =  3           // Data format mode 4D
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_ADDR_MODE                             2

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_ADDR_MODE_ENUM                          
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_ADDR_MODE_ID_DIRECT                        =  0,          // Mode DIRECT addressing
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_ADDR_MODE_ID_INDIRECT                      =  1           // Mode INDIRECT addressing
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_TRANSFER_TYPE                         3

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_TRANSFER_TYPE_ENUM                      
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_TRANSFER_TYPE_ID_MEM                       =  0,          // Memory based transfer type
   APEX_642CL_MCDMA_CH_CONFIG_BIT_DST_TRANSFER_TYPE_ID_PER                       =  1           // Peripheral based transfer type
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_DATA_FORMAT                           4

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_DATA_FORMAT_ENUM                        
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_DATA_FORMAT_ID_1D                          =  0,          // Data format mode 1D
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_DATA_FORMAT_ID_2D                          =  1,          // Data format mode 2D
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_DATA_FORMAT_ID_3D                          =  2,          // Data format mode 3D
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_DATA_FORMAT_ID_4D                          =  3           // Data format mode 4D
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_ADDR_MODE                             6

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_ADDR_MODE_ENUM                          
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_ADDR_MODE_ID_DIRECT                        =  0,          // Mode DIRECT addressing
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_ADDR_MODE_ID_INDIRECT                      =  1           // Mode INDIRECT addressing
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_TRANSFER_TYPE                         7

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_TRANSFER_TYPE_ENUM                      
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_TRANSFER_TYPE_ID_MEM                       =  0,          // Memory based transfer type
   APEX_642CL_MCDMA_CH_CONFIG_BIT_SRC_TRANSFER_TYPE_ID_PER                       =  1           // Peripheral based transfer type
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE                                 8

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE_ENUM                              
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE_ID_8_BIT                             =  0,          // Bus word size 8_BIT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE_ID_16_BIT                            =  1,          // Bus word size 16_BIT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE_ID_32_BIT                            =  2,          // Bus word size 32_BIT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE_ID_64_BIT                            =  3,          // Bus word size 64_BIT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_WORD_SIZE_ID_128_BIT                           =  4           // Bus word size 128_BIT
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE                                11

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ENUM                             
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_1_BEAT                           =  0,          // Burst size 1_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_2_BEAT                           =  1,          // Burst size 2_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_3_BEAT                           =  2,          // Burst size 3_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_4_BEAT                           =  3,          // Burst size 4_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_5_BEAT                           =  4,          // Burst size 5_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_6_BEAT                           =  5,          // Burst size 6_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_7_BEAT                           =  6,          // Burst size 7_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_8_BEAT                           =  7,          // Burst size 8_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_9_BEAT                           =  8,          // Burst size 9_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_10_BEAT                          =  9,          // Burst size 10_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_11_BEAT                          =  10,         // Burst size 11_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_12_BEAT                          =  11,         // Burst size 12_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_13_BEAT                          =  12,         // Burst size 13_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_14_BEAT                          =  13,         // Burst size 14_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_15_BEAT                          =  14,         // Burst size 15_BEAT
   APEX_642CL_MCDMA_CH_CONFIG_BIT_BURST_SIZE_ID_16_BEAT                          =  15          // Burst size 16_BEAT
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_RESERVED                                  15
#define APEX_642CL_MCDMA_CH_CONFIG_BIT_LAST_SEGMENT                              28

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_LAST_SEGMENT_ENUM                           
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_LAST_SEGMENT_ID_ACTIVE                         =  1,          // Last segment ACTIVE
   APEX_642CL_MCDMA_CH_CONFIG_BIT_LAST_SEGMENT_ID_INACTIVE                       =  0           // Last segment INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_LOOP_ON_LAST                              29

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_LOOP_ON_LAST_ENUM                           
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_LOOP_ON_LAST_ID_ACTIVE                         =  1,          // Loop on last ACTIVE
   APEX_642CL_MCDMA_CH_CONFIG_BIT_LOOP_ON_LAST_ID_INACTIVE                       =  0           // Loop on last INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_INTERRUPT_ENABLE                          30

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_INTERRUPT_ENABLE_ENUM                       
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_INTERRUPT_ENABLE_ID_ACTIVE                     =  1,          // Interrupt ACTIVE
   APEX_642CL_MCDMA_CH_CONFIG_BIT_INTERRUPT_ENABLE_ID_INACTIVE                   =  0           // Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_MCDMA_CH_CONFIG_BIT_ENABLE_PAUSE                              31

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_CONFIG_BIT_ENABLE_PAUSE_ENUM                           
{
   APEX_642CL_MCDMA_CH_CONFIG_BIT_ENABLE_PAUSE_ID_PAUSE                          =  1,          // Link list PAUSE
   APEX_642CL_MCDMA_CH_CONFIG_BIT_ENABLE_PAUSE_ID_CONTINUE                       =  0           // Link list CONTINUE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    TRANSFER
//    APEX_642CL_MCDMA_CH_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Transfer Size Register
//    
//    This register is used to program how much data is transferred before the the
//    channel is complete.
// 
// Device Control Register Bits Description:
//    SIZE                 [27:00]   Number of bytes to transfer.
//                                   
//                                   This field is also a shadow of the last read linked list. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_TRANSFER                                           (APEX_642CL_MCDMA_CH_7_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_7_TRANSFER_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_TRANSFER_OFFSET                                    (0x00000010)
#define APEX_642CL_MCDMA_CH_7_TRANSFER_DEFAULT                                   (0x00000000)


union _APEX_642CL_MCDMA_CH_TRANSFER_TAG
{
   struct _asApex642clMcdmaChTransferBitFieldTag
   {
      REG32  Size                 : 28; //  27:00  APEX_642CL_MCDMA_CH_TRANSFER_BIT_SIZE
      REG32  Reserved             :  4; //  31:28  APEX_642CL_MCDMA_CH_TRANSFER_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_TRANSFER_TAG                                  APEX_642CL_MCDMA_CH_TRANSFER_DCR;

extern volatile APEX_642CL_MCDMA_CH_TRANSFER_DCR                                 gApex642clMcdmaChTransfer;
extern volatile APEX_642CL_MCDMA_CH_TRANSFER_DCR*                                gpApex642clMcdmaChTransfer;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_TRANSFER_BIT_SIZE                                    0
#define APEX_642CL_MCDMA_CH_TRANSFER_BIT_RESERVED                                28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SPAN
//    APEX_642CL_MCDMA_CH_SPAN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_SPAN_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source/Destination Span Register
// 
// Device Control Register Bits Description:
//    DST                  [15:00]   Destination Span (signed). This is the number of bytes from the beginning of one
//                                   line to the next for a 2D transfer. 
//    SRC                  [31:16]   Source Span (signed). This is the number of bytes from the beginning of one line
//                                   to the next for a 2D transfer. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_SPAN                                               (APEX_642CL_MCDMA_CH_7_SPAN_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_SPAN_OFFSET)
#define APEX_642CL_MCDMA_CH_7_SPAN_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_SPAN_OFFSET                                        (0x00000014)
#define APEX_642CL_MCDMA_CH_7_SPAN_DEFAULT                                       (0x00000000)


union _APEX_642CL_MCDMA_CH_SPAN_TAG
{
   struct _asApex642clMcdmaChSpanBitFieldTag
   {
      REG32  Dst                  : 16; //  15:00  APEX_642CL_MCDMA_CH_SPAN_BIT_DST
      REG32  Src                  : 16; //  31:16  APEX_642CL_MCDMA_CH_SPAN_BIT_SRC
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_SPAN_TAG                                      APEX_642CL_MCDMA_CH_SPAN_DCR;

extern volatile APEX_642CL_MCDMA_CH_SPAN_DCR                                     gApex642clMcdmaChSpan;
extern volatile APEX_642CL_MCDMA_CH_SPAN_DCR*                                    gpApex642clMcdmaChSpan;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_SPAN_BIT_DST                                         0
#define APEX_642CL_MCDMA_CH_SPAN_BIT_SRC                                         16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WIDTH
//    APEX_642CL_MCDMA_CH_WIDTH
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_WIDTH_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Tile Width Register
// 
// Device Control Register Bits Description:
//    TILE                 [15:00]   Tile Width (N). This counter is used for 2D transfers and represents the width of
//                                   the 2D. 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_WIDTH                                              (APEX_642CL_MCDMA_CH_7_WIDTH_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_WIDTH_OFFSET)
#define APEX_642CL_MCDMA_CH_7_WIDTH_BASE_ADDRESS                                 (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_WIDTH_OFFSET                                       (0x00000018)
#define APEX_642CL_MCDMA_CH_7_WIDTH_DEFAULT                                      (0x00000000)


union _APEX_642CL_MCDMA_CH_WIDTH_TAG
{
   struct _asApex642clMcdmaChWidthBitFieldTag
   {
      REG32  Tile                 : 16; //  15:00  APEX_642CL_MCDMA_CH_WIDTH_BIT_TILE
      REG32  Reserved             : 16; //  31:16  APEX_642CL_MCDMA_CH_WIDTH_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_WIDTH_TAG                                     APEX_642CL_MCDMA_CH_WIDTH_DCR;

extern volatile APEX_642CL_MCDMA_CH_WIDTH_DCR                                    gApex642clMcdmaChWidth;
extern volatile APEX_642CL_MCDMA_CH_WIDTH_DCR*                                   gpApex642clMcdmaChWidth;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_WIDTH_BIT_TILE                                       0
#define APEX_642CL_MCDMA_CH_WIDTH_BIT_RESERVED                                   16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    BYTE
//    APEX_642CL_MCDMA_CH_BYTE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_BYTE_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Byte Status Register
//    
//    This register provides some status information associated with the progress of the
//    channel.
// 
// Device Control Register Bits Description:
//    COUNT                [27:00]   Indicates the number of bytes that are remaining to be transferred. 
//    RESERVED             [31:28]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_BYTE                                               (APEX_642CL_MCDMA_CH_7_BYTE_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_BYTE_OFFSET)
#define APEX_642CL_MCDMA_CH_7_BYTE_BASE_ADDRESS                                  (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_BYTE_OFFSET                                        (0x0000001c)
#define APEX_642CL_MCDMA_CH_7_BYTE_DEFAULT                                       (0x00000000)


union _APEX_642CL_MCDMA_CH_BYTE_TAG
{
   struct _asApex642clMcdmaChByteBitFieldTag
   {
      REG32  Count                : 28; //  27:00  APEX_642CL_MCDMA_CH_BYTE_BIT_COUNT
      REG32  Reserved             :  4; //  31:28  APEX_642CL_MCDMA_CH_BYTE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_BYTE_TAG                                      APEX_642CL_MCDMA_CH_BYTE_DCR;

extern volatile APEX_642CL_MCDMA_CH_BYTE_DCR                                     gApex642clMcdmaChByte;
extern volatile APEX_642CL_MCDMA_CH_BYTE_DCR*                                    gpApex642clMcdmaChByte;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_BYTE_BIT_COUNT                                       0
#define APEX_642CL_MCDMA_CH_BYTE_BIT_RESERVED                                    28

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SRC
//    APEX_642CL_MCDMA_CH_STATUS_SRC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SRC_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Source Address Status Register
//    
//    Indicates the current source address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Source Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_STATUS_SRC                                         (APEX_642CL_MCDMA_CH_7_STATUS_SRC_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_STATUS_SRC_OFFSET)
#define APEX_642CL_MCDMA_CH_7_STATUS_SRC_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_STATUS_SRC_OFFSET                                  (0x00000020)
#define APEX_642CL_MCDMA_CH_7_STATUS_SRC_DEFAULT                                 (0x00000000)


union _APEX_642CL_MCDMA_CH_STATUS_SRC_TAG
{
   struct _asApex642clMcdmaChStatusSrcBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_MCDMA_CH_STATUS_SRC_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_STATUS_SRC_TAG                                APEX_642CL_MCDMA_CH_STATUS_SRC_DCR;

extern volatile APEX_642CL_MCDMA_CH_STATUS_SRC_DCR                               gApex642clMcdmaChStatusSrc;
extern volatile APEX_642CL_MCDMA_CH_STATUS_SRC_DCR*                              gpApex642clMcdmaChStatusSrc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_STATUS_SRC_BIT_ADDR                                  0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_DST
//    APEX_642CL_MCDMA_CH_STATUS_DST
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_DST_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000024
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Destination Address Register
//    
//    Indicates the current destination address of the transfer.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Destination Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_STATUS_DST                                         (APEX_642CL_MCDMA_CH_7_STATUS_DST_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_STATUS_DST_OFFSET)
#define APEX_642CL_MCDMA_CH_7_STATUS_DST_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_STATUS_DST_OFFSET                                  (0x00000024)
#define APEX_642CL_MCDMA_CH_7_STATUS_DST_DEFAULT                                 (0x00000000)


union _APEX_642CL_MCDMA_CH_STATUS_DST_TAG
{
   struct _asApex642clMcdmaChStatusDstBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_MCDMA_CH_STATUS_DST_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_STATUS_DST_TAG                                APEX_642CL_MCDMA_CH_STATUS_DST_DCR;

extern volatile APEX_642CL_MCDMA_CH_STATUS_DST_DCR                               gApex642clMcdmaChStatusDst;
extern volatile APEX_642CL_MCDMA_CH_STATUS_DST_DCR*                              gpApex642clMcdmaChStatusDst;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_STATUS_DST_BIT_ADDR                                  0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_SHADOW
//    APEX_642CL_MCDMA_CH_STATUS_SHADOW
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_SHADOW_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Link List Shadow Register
//    
//    This is a shadow of the programmed linked list address. It is updated whenever the
//    linked list address register is written. For non-sequential linked lists it
//    contains the address of the first list entry.
// 
// Device Control Register Bits Description:
//    ADDR                 [31:00]   Link List Shadow Address 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_STATUS_SHADOW                                      (APEX_642CL_MCDMA_CH_7_STATUS_SHADOW_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_STATUS_SHADOW_OFFSET)
#define APEX_642CL_MCDMA_CH_7_STATUS_SHADOW_BASE_ADDRESS                         (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_STATUS_SHADOW_OFFSET                               (0x00000028)
#define APEX_642CL_MCDMA_CH_7_STATUS_SHADOW_DEFAULT                              (0x00000000)


union _APEX_642CL_MCDMA_CH_STATUS_SHADOW_TAG
{
   struct _asApex642clMcdmaChStatusShadowBitFieldTag
   {
      REG32  Addr                 : 32; //  31:00  APEX_642CL_MCDMA_CH_STATUS_SHADOW_BIT_ADDR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_STATUS_SHADOW_TAG                             APEX_642CL_MCDMA_CH_STATUS_SHADOW_DCR;

extern volatile APEX_642CL_MCDMA_CH_STATUS_SHADOW_DCR                            gApex642clMcdmaChStatusShadow;
extern volatile APEX_642CL_MCDMA_CH_STATUS_SHADOW_DCR*                           gpApex642clMcdmaChStatusShadow;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_STATUS_SHADOW_BIT_ADDR                               0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_FSM
//    APEX_642CL_MCDMA_CH_STATUS_FSM
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_FSM_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status Register
//    
//    This is a debug register and indicates the current state of several state machines
//    within the channel.
//    
// 
// Device Control Register Bits Description:
//    LIST_CTRL_CS         [04:00]   Linked list control state machine. 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_STATUS_FSM                                         (APEX_642CL_MCDMA_CH_7_STATUS_FSM_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_STATUS_FSM_OFFSET)
#define APEX_642CL_MCDMA_CH_7_STATUS_FSM_BASE_ADDRESS                            (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_STATUS_FSM_OFFSET                                  (0x0000002c)
#define APEX_642CL_MCDMA_CH_7_STATUS_FSM_DEFAULT                                 (0x00000000)


union _APEX_642CL_MCDMA_CH_STATUS_FSM_TAG
{
   struct _asApex642clMcdmaChStatusFsmBitFieldTag
   {
      REG32  ListCtrlCs           :  5; //  04:00  APEX_642CL_MCDMA_CH_STATUS_FSM_BIT_LIST_CTRL_CS
      REG32  Reserved             : 27; //  31:05  APEX_642CL_MCDMA_CH_STATUS_FSM_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_STATUS_FSM_TAG                                APEX_642CL_MCDMA_CH_STATUS_FSM_DCR;

extern volatile APEX_642CL_MCDMA_CH_STATUS_FSM_DCR                               gApex642clMcdmaChStatusFsm;
extern volatile APEX_642CL_MCDMA_CH_STATUS_FSM_DCR*                              gpApex642clMcdmaChStatusFsm;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_STATUS_FSM_BIT_LIST_CTRL_CS                          0
#define APEX_642CL_MCDMA_CH_STATUS_FSM_BIT_RESERVED                              5

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STATUS_TRANSFER
//    APEX_642CL_MCDMA_CH_STATUS_TRANSFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_STATUS_TRANSFER_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000030
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Channel Status 1 Register
// 
// Device Control Register Bits Description:
//    COLUMN_CNT           [15:00]   This is a decrementing counter that indicates the current column for 4D
//                                   transactions. 
//    ROW_CNT              [31:16]   This is a decrementing counter that indicates the current row tile for 3D and 4D
//                                   transactions. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_STATUS_TRANSFER                                    (APEX_642CL_MCDMA_CH_7_STATUS_TRANSFER_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_STATUS_TRANSFER_OFFSET)
#define APEX_642CL_MCDMA_CH_7_STATUS_TRANSFER_BASE_ADDRESS                       (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_STATUS_TRANSFER_OFFSET                             (0x00000030)
#define APEX_642CL_MCDMA_CH_7_STATUS_TRANSFER_DEFAULT                            (0x00000000)


union _APEX_642CL_MCDMA_CH_STATUS_TRANSFER_TAG
{
   struct _asApex642clMcdmaChStatusTransferBitFieldTag
   {
      REG32  ColumnCnt            : 16; //  15:00  APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BIT_COLUMN_CNT
      REG32  RowCnt               : 16; //  31:16  APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BIT_ROW_CNT
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_STATUS_TRANSFER_TAG                           APEX_642CL_MCDMA_CH_STATUS_TRANSFER_DCR;

extern volatile APEX_642CL_MCDMA_CH_STATUS_TRANSFER_DCR                          gApex642clMcdmaChStatusTransfer;
extern volatile APEX_642CL_MCDMA_CH_STATUS_TRANSFER_DCR*                         gpApex642clMcdmaChStatusTransfer;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BIT_COLUMN_CNT                       0
#define APEX_642CL_MCDMA_CH_STATUS_TRANSFER_BIT_ROW_CNT                          16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PRIORITY
//    APEX_642CL_MCDMA_CH_PRIORITY
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_MCDMA_CH_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_PRIORITY_OFFSET
//    Base Addr:    APEX_642CL_MCDMA_CH
//    Offset:       32'h00000034
//    Default:      32'h0000000f
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Channel Priority Register
// 
// Device Control Register Bits Description:
//    IRQ_PRIORITY         [03:00]   Each channel is programmed with a priority value which from '0' to '7'. A  value
//                                   of '0' is the highest and '7' is the lowest. 
//    BUS_PRIORITY         [05:04]   For channel arbitration, each active channel is assigned a priority level. The
//                                   highest priority channel receives a grant for the next transaction window.
//                                   
//                                   0: low (lowest priorty)
//                                   1: medium
//                                   2: high
//                                   3: critical (highest priority)
//                                    
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_7_PRIORITY                                           (APEX_642CL_MCDMA_CH_7_PRIORITY_BASE_ADDRESS + APEX_642CL_MCDMA_CH_7_PRIORITY_OFFSET)
#define APEX_642CL_MCDMA_CH_7_PRIORITY_BASE_ADDRESS                              (APEX_642CL_MCDMA_CH_7)
#define APEX_642CL_MCDMA_CH_7_PRIORITY_OFFSET                                    (0x00000034)
#define APEX_642CL_MCDMA_CH_7_PRIORITY_DEFAULT                                   (0x0000000f)


union _APEX_642CL_MCDMA_CH_PRIORITY_TAG
{
   struct _asApex642clMcdmaChPriorityBitFieldTag
   {
      REG32  IrqPriority          :  4; //  03:00  APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY
      REG32  BusPriority          :  2; //  05:04  APEX_642CL_MCDMA_CH_PRIORITY_BIT_BUS_PRIORITY
      REG32  Reserved             : 26; //  31:06  APEX_642CL_MCDMA_CH_PRIORITY_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_MCDMA_CH_PRIORITY_TAG                                  APEX_642CL_MCDMA_CH_PRIORITY_DCR;

extern volatile APEX_642CL_MCDMA_CH_PRIORITY_DCR                                 gApex642clMcdmaChPriority;
extern volatile APEX_642CL_MCDMA_CH_PRIORITY_DCR*                                gpApex642clMcdmaChPriority;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY                            0

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ENUM                         
{
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_0                      =  0,          // Priority LEVEL_0
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_1                      =  1,          // Priority LEVEL_1
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_2                      =  2,          // Priority LEVEL_2
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_3                      =  3,          // Priority LEVEL_3
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_4                      =  4,          // Priority LEVEL_4
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_5                      =  5,          // Priority LEVEL_5
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_6                      =  6,          // Priority LEVEL_6
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_IRQ_PRIORITY_ID_LEVEL_7                      =  7           // Priority LEVEL_7
};
                                                                             

#define APEX_642CL_MCDMA_CH_PRIORITY_BIT_BUS_PRIORITY                            4

// Device Control Register Bits Id and Description
enum APEX_642CL_MCDMA_CH_PRIORITY_BIT_BUS_PRIORITY_ENUM                         
{
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_BUS_PRIORITY_ID_LOW                          =  0,          // Priority level LOW
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_BUS_PRIORITY_ID_MEDIUM                       =  1,          // Priority level MEDIUM
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_BUS_PRIORITY_ID_HIGH                         =  2,          // Priority level HIGH
   APEX_642CL_MCDMA_CH_PRIORITY_BIT_BUS_PRIORITY_ID_CRITICAL                     =  3           // Priority level CRITICAL
};
                                                                             

#define APEX_642CL_MCDMA_CH_PRIORITY_BIT_RESERVED                                6

#endif /* APEX642CLMCDMAMAPTYPE_H */
