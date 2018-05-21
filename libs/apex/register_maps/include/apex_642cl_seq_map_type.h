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
//    SEQ
//    APEX_642CL_SEQ
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_SEQ_BASE_ADDRESS + APEX_642CL_SEQ_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Sequencer
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLSEQMAPTYPE_H
#define APEX642CLSEQMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ISR
//    APEX_642CL_SEQ_ISR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_ISR_BASE_ADDRESS + APEX_642CL_SEQ_ISR_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Source Register
// 
// Device Control Register Bits Description:
//    PROC_DEC             [00]      Processor X Decrement was successful
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_1           [19:16]   Reserved 
//    RES_Z_IDLE           [20]      Resource X became zero and all enabled SCBs are idle 
//    RESERVED_2           [27:24]   Reserved 
//    RES_Z                [28]      Resource X became zero 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ISR                                                       (APEX_642CL_SEQ_ISR_BASE_ADDRESS + APEX_642CL_SEQ_ISR_OFFSET)
#define APEX_642CL_SEQ_ISR_BASE_ADDRESS                                          (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ISR_OFFSET                                                (0x00000000)
#define APEX_642CL_SEQ_ISR_DEFAULT                                               (0x00000000)


union _APEX_642CL_SEQ_ISR_TAG
{
   struct _asApex642clSeqIsrBitFieldTag
   {
      REG32  ProcDec0             :  1; //  0      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_0
      REG32  ProcDec1             :  1; //  1      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_1
      REG32  ProcDec2             :  1; //  2      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_2
      REG32  ProcDec3             :  1; //  3      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_3
      REG32  ProcDec4             :  1; //  4      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_4
      REG32  ProcDec5             :  1; //  5      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_5
      REG32  ProcDec6             :  1; //  6      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_6
      REG32  ProcDec7             :  1; //  7      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_7
      REG32  ProcDec8             :  1; //  8      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_8
      REG32  ProcDec9             :  1; //  9      APEX_642CL_SEQ_ISR_BIT_PROC_DEC_9
      REG32  ProcDec10            :  1; //  10     APEX_642CL_SEQ_ISR_BIT_PROC_DEC_10
      REG32  ProcDec11            :  1; //  11     APEX_642CL_SEQ_ISR_BIT_PROC_DEC_11
      REG32  ProcDec12            :  1; //  12     APEX_642CL_SEQ_ISR_BIT_PROC_DEC_12
      REG32  ProcDec13            :  1; //  13     APEX_642CL_SEQ_ISR_BIT_PROC_DEC_13
      REG32  ProcDec14            :  1; //  14     APEX_642CL_SEQ_ISR_BIT_PROC_DEC_14
      REG32  ProcDec15            :  1; //  15     APEX_642CL_SEQ_ISR_BIT_PROC_DEC_15
      REG32  Reserved1            :  4; //  19:16  APEX_642CL_SEQ_ISR_BIT_RESERVED_1
      REG32  ResZIdle0            :  1; //  20     APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_0
      REG32  ResZIdle1            :  1; //  21     APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_1
      REG32  ResZIdle2            :  1; //  22     APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_2
      REG32  ResZIdle3            :  1; //  23     APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_3
      REG32  Reserved2            :  4; //  27:24  APEX_642CL_SEQ_ISR_BIT_RESERVED_2
      REG32  ResZ0                :  1; //  28     APEX_642CL_SEQ_ISR_BIT_RES_Z_0
      REG32  ResZ1                :  1; //  29     APEX_642CL_SEQ_ISR_BIT_RES_Z_1
      REG32  ResZ2                :  1; //  30     APEX_642CL_SEQ_ISR_BIT_RES_Z_2
      REG32  ResZ3                :  1; //  31     APEX_642CL_SEQ_ISR_BIT_RES_Z_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_ISR_TAG                                            APEX_642CL_SEQ_ISR_DCR;

extern volatile APEX_642CL_SEQ_ISR_DCR                                           gApex642clSeqIsr;
extern volatile APEX_642CL_SEQ_ISR_DCR*                                          gpApex642clSeqIsr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_0                                        0
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_1                                        1
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_2                                        2
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_3                                        3
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_4                                        4
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_5                                        5
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_6                                        6
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_7                                        7
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_8                                        8
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_9                                        9
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_10                                       10
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_11                                       11
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_12                                       12
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_13                                       13
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_14                                       14
#define APEX_642CL_SEQ_ISR_BIT_PROC_DEC_15                                       15

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ISR_BIT_PROC_DEC_ENUM                                       
{
   APEX_642CL_SEQ_ISR_BIT_PROC_DEC_ID_ACTIVE                                     =  1,          // Processor Decrement successful InterruptACTIVE
   APEX_642CL_SEQ_ISR_BIT_PROC_DEC_ID_INACTIVE                                   =  0           // Processor Decrement successful Interrupt INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_ISR_BIT_RESERVED_1                                        16
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_0                                      20
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_1                                      21
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_2                                      22
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_3                                      23

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_ENUM                                     
{
   APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_ID_ACTIVE                                   =  1,          // Resource became zero and all enabled SCBs are idle ACTIVE
   APEX_642CL_SEQ_ISR_BIT_RES_Z_IDLE_ID_INACTIVE                                 =  0           // Resource became zero and all enabled SCBs are idle INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_ISR_BIT_RESERVED_2                                        24
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_0                                           28
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_1                                           29
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_2                                           30
#define APEX_642CL_SEQ_ISR_BIT_RES_Z_3                                           31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ISR_BIT_RES_Z_ENUM                                          
{
   APEX_642CL_SEQ_ISR_BIT_RES_Z_ID_ACTIVE                                        =  1,          // Resource became zero ACTIVE
   APEX_642CL_SEQ_ISR_BIT_RES_Z_ID_INACTIVE                                      =  0           // Resource became zero INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IMR
//    APEX_642CL_SEQ_IMR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_IMR_BASE_ADDRESS + APEX_642CL_SEQ_IMR_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000004
//    Default:      32'hf0f0ffff
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Mask Register for Global Interrupt signal
// 
// Device Control Register Bits Description:
//    PROC_DEC             [00]      Processor X Dec was successful Interrupt Mask
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_1           [19:16]   Reserved 
//    RES_Z_IDLE           [20]      Resource X became zero and all enabled SCBs are idle Interrupt Mask 
//    RESERVED_2           [27:24]   Reserved 
//    RES_Z                [28]      Resource X became zero Interrupt Mask 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_IMR                                                       (APEX_642CL_SEQ_IMR_BASE_ADDRESS + APEX_642CL_SEQ_IMR_OFFSET)
#define APEX_642CL_SEQ_IMR_BASE_ADDRESS                                          (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_IMR_OFFSET                                                (0x00000004)
#define APEX_642CL_SEQ_IMR_DEFAULT                                               (0xf0f0ffff)


union _APEX_642CL_SEQ_IMR_TAG
{
   struct _asApex642clSeqImrBitFieldTag
   {
      REG32  ProcDec0             :  1; //  0      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_0
      REG32  ProcDec1             :  1; //  1      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_1
      REG32  ProcDec2             :  1; //  2      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_2
      REG32  ProcDec3             :  1; //  3      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_3
      REG32  ProcDec4             :  1; //  4      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_4
      REG32  ProcDec5             :  1; //  5      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_5
      REG32  ProcDec6             :  1; //  6      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_6
      REG32  ProcDec7             :  1; //  7      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_7
      REG32  ProcDec8             :  1; //  8      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_8
      REG32  ProcDec9             :  1; //  9      APEX_642CL_SEQ_IMR_BIT_PROC_DEC_9
      REG32  ProcDec10            :  1; //  10     APEX_642CL_SEQ_IMR_BIT_PROC_DEC_10
      REG32  ProcDec11            :  1; //  11     APEX_642CL_SEQ_IMR_BIT_PROC_DEC_11
      REG32  ProcDec12            :  1; //  12     APEX_642CL_SEQ_IMR_BIT_PROC_DEC_12
      REG32  ProcDec13            :  1; //  13     APEX_642CL_SEQ_IMR_BIT_PROC_DEC_13
      REG32  ProcDec14            :  1; //  14     APEX_642CL_SEQ_IMR_BIT_PROC_DEC_14
      REG32  ProcDec15            :  1; //  15     APEX_642CL_SEQ_IMR_BIT_PROC_DEC_15
      REG32  Reserved1            :  4; //  19:16  APEX_642CL_SEQ_IMR_BIT_RESERVED_1
      REG32  ResZIdle0            :  1; //  20     APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_0
      REG32  ResZIdle1            :  1; //  21     APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_1
      REG32  ResZIdle2            :  1; //  22     APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_2
      REG32  ResZIdle3            :  1; //  23     APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_3
      REG32  Reserved2            :  4; //  27:24  APEX_642CL_SEQ_IMR_BIT_RESERVED_2
      REG32  ResZ0                :  1; //  28     APEX_642CL_SEQ_IMR_BIT_RES_Z_0
      REG32  ResZ1                :  1; //  29     APEX_642CL_SEQ_IMR_BIT_RES_Z_1
      REG32  ResZ2                :  1; //  30     APEX_642CL_SEQ_IMR_BIT_RES_Z_2
      REG32  ResZ3                :  1; //  31     APEX_642CL_SEQ_IMR_BIT_RES_Z_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_IMR_TAG                                            APEX_642CL_SEQ_IMR_DCR;

extern volatile APEX_642CL_SEQ_IMR_DCR                                           gApex642clSeqImr;
extern volatile APEX_642CL_SEQ_IMR_DCR*                                          gpApex642clSeqImr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_0                                        0
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_1                                        1
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_2                                        2
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_3                                        3
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_4                                        4
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_5                                        5
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_6                                        6
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_7                                        7
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_8                                        8
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_9                                        9
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_10                                       10
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_11                                       11
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_12                                       12
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_13                                       13
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_14                                       14
#define APEX_642CL_SEQ_IMR_BIT_PROC_DEC_15                                       15

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_BIT_PROC_DEC_ENUM                                       
{
   APEX_642CL_SEQ_IMR_BIT_PROC_DEC_ID_ACTIVE                                     =  1,          // Processor Decrement successful Interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_BIT_PROC_DEC_ID_INACTIVE                                   =  0           // Processor Decrement successful Interrupt Mask INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_IMR_BIT_RESERVED_1                                        16
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_0                                      20
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_1                                      21
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_2                                      22
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_3                                      23

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_ENUM                                     
{
   APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_ID_ACTIVE                                   =  1,          // Resource became zero and all enabled SCBs are idle Interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_BIT_RES_Z_IDLE_ID_INACTIVE                                 =  0           // Resource became zero and all enabled SCBs are idle Interrupt Mask INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_IMR_BIT_RESERVED_2                                        24
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_0                                           28
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_1                                           29
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_2                                           30
#define APEX_642CL_SEQ_IMR_BIT_RES_Z_3                                           31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_BIT_RES_Z_ENUM                                          
{
   APEX_642CL_SEQ_IMR_BIT_RES_Z_ID_ACTIVE                                        =  1,          // Resource became zero interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_BIT_RES_Z_ID_INACTIVE                                      =  0           // Resource became zero interrupt Mask INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ICR
//    APEX_642CL_SEQ_ICR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_ICR_BASE_ADDRESS + APEX_642CL_SEQ_ICR_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rwc1
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Clear Register
//    Writing a '1' to the interrupt bit location will clear the interrupt.
// 
// Device Control Register Bits Description:
//    PROC_DEC             [00]      Processor X Dec was successful Interrupt Clear
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_1           [19:16]   Reserved 
//    RES_Z_IDLE           [20]      Resource X became zero and all enabled SCBs are idle Interrupt Clear 
//    RESERVED_2           [27:24]   Reserved 
//    RES_Z                [28]      Resource X became zero Interrupt Clear 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ICR                                                       (APEX_642CL_SEQ_ICR_BASE_ADDRESS + APEX_642CL_SEQ_ICR_OFFSET)
#define APEX_642CL_SEQ_ICR_BASE_ADDRESS                                          (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ICR_OFFSET                                                (0x00000008)
#define APEX_642CL_SEQ_ICR_DEFAULT                                               (0x00000000)


union _APEX_642CL_SEQ_ICR_TAG
{
   struct _asApex642clSeqIcrBitFieldTag
   {
      REG32  ProcDec0             :  1; //  0      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_0
      REG32  ProcDec1             :  1; //  1      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_1
      REG32  ProcDec2             :  1; //  2      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_2
      REG32  ProcDec3             :  1; //  3      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_3
      REG32  ProcDec4             :  1; //  4      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_4
      REG32  ProcDec5             :  1; //  5      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_5
      REG32  ProcDec6             :  1; //  6      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_6
      REG32  ProcDec7             :  1; //  7      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_7
      REG32  ProcDec8             :  1; //  8      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_8
      REG32  ProcDec9             :  1; //  9      APEX_642CL_SEQ_ICR_BIT_PROC_DEC_9
      REG32  ProcDec10            :  1; //  10     APEX_642CL_SEQ_ICR_BIT_PROC_DEC_10
      REG32  ProcDec11            :  1; //  11     APEX_642CL_SEQ_ICR_BIT_PROC_DEC_11
      REG32  ProcDec12            :  1; //  12     APEX_642CL_SEQ_ICR_BIT_PROC_DEC_12
      REG32  ProcDec13            :  1; //  13     APEX_642CL_SEQ_ICR_BIT_PROC_DEC_13
      REG32  ProcDec14            :  1; //  14     APEX_642CL_SEQ_ICR_BIT_PROC_DEC_14
      REG32  ProcDec15            :  1; //  15     APEX_642CL_SEQ_ICR_BIT_PROC_DEC_15
      REG32  Reserved1            :  4; //  19:16  APEX_642CL_SEQ_ICR_BIT_RESERVED_1
      REG32  ResZIdle0            :  1; //  20     APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_0
      REG32  ResZIdle1            :  1; //  21     APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_1
      REG32  ResZIdle2            :  1; //  22     APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_2
      REG32  ResZIdle3            :  1; //  23     APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_3
      REG32  Reserved2            :  4; //  27:24  APEX_642CL_SEQ_ICR_BIT_RESERVED_2
      REG32  ResZ0                :  1; //  28     APEX_642CL_SEQ_ICR_BIT_RES_Z_0
      REG32  ResZ1                :  1; //  29     APEX_642CL_SEQ_ICR_BIT_RES_Z_1
      REG32  ResZ2                :  1; //  30     APEX_642CL_SEQ_ICR_BIT_RES_Z_2
      REG32  ResZ3                :  1; //  31     APEX_642CL_SEQ_ICR_BIT_RES_Z_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_ICR_TAG                                            APEX_642CL_SEQ_ICR_DCR;

extern volatile APEX_642CL_SEQ_ICR_DCR                                           gApex642clSeqIcr;
extern volatile APEX_642CL_SEQ_ICR_DCR*                                          gpApex642clSeqIcr;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_0                                        0
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_1                                        1
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_2                                        2
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_3                                        3
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_4                                        4
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_5                                        5
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_6                                        6
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_7                                        7
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_8                                        8
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_9                                        9
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_10                                       10
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_11                                       11
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_12                                       12
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_13                                       13
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_14                                       14
#define APEX_642CL_SEQ_ICR_BIT_PROC_DEC_15                                       15

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ICR_BIT_PROC_DEC_ENUM                                       
{
   APEX_642CL_SEQ_ICR_BIT_PROC_DEC_ID_ACTIVE                                     =  1,          // Processor Decrement successful Interrupt Clear ACTIVE
   APEX_642CL_SEQ_ICR_BIT_PROC_DEC_ID_INACTIVE                                   =  0           // Processor Decrement successful Interrupt Clear INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_ICR_BIT_RESERVED_1                                        16
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_0                                      20
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_1                                      21
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_2                                      22
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_3                                      23

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_ENUM                                     
{
   APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_ID_ACTIVE                                   =  1,          // Resource became zero and all enabled SCBs are idle Interrupt Clear ACTIVE
   APEX_642CL_SEQ_ICR_BIT_RES_Z_IDLE_ID_INACTIVE                                 =  0           // Resource became zero and all enabled SCBs are idle Interrupt Clear INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_ICR_BIT_RESERVED_2                                        24
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_0                                           28
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_1                                           29
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_2                                           30
#define APEX_642CL_SEQ_ICR_BIT_RES_Z_3                                           31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ICR_BIT_RES_Z_ENUM                                          
{
   APEX_642CL_SEQ_ICR_BIT_RES_Z_ID_ACTIVE                                        =  1,          // Resource became zero interrupt Clear ACTIVE
   APEX_642CL_SEQ_ICR_BIT_RES_Z_ID_INACTIVE                                      =  0           // Resource became zero interrupt Clear INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ENABLE
//    APEX_642CL_SEQ_ENABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_ENABLE_BASE_ADDRESS + APEX_642CL_SEQ_ENABLE_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h0000000c
//    Default:      32'h00000000
//    Type:         rw1
//    Nb:         1
//    Size:       4
// 
// Description:
//    Sequencer Enable Register
// 
// Device Control Register Bits Description:
//    ON                   [00]      Sequencer On [0..5]
//                                   
//                                   if any of the 6bits is 1
//                                      - Sequencer is enabled
//                                   
//                                   if all 6 bits are 0
//                                      - Sequencer is in sleep mode
//                                   
//                                   Only write '1' affects SEQ On value, to turn Off SEQ.DISABLE register needs to be used
//                                    
//    RESERVED             [30:06]   Reserved 
//    DEBUG                [31]      Debug Mode
//                                   Puts the Sequencer in Debug Mode and is used in conjunction with DEBUG_STEP register to step through.
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ENABLE                                                    (APEX_642CL_SEQ_ENABLE_BASE_ADDRESS + APEX_642CL_SEQ_ENABLE_OFFSET)
#define APEX_642CL_SEQ_ENABLE_BASE_ADDRESS                                       (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ENABLE_OFFSET                                             (0x0000000c)
#define APEX_642CL_SEQ_ENABLE_DEFAULT                                            (0x00000000)


union _APEX_642CL_SEQ_ENABLE_TAG
{
   struct _asApex642clSeqEnableBitFieldTag
   {
      REG32  On0                  :  1; //  0      APEX_642CL_SEQ_ENABLE_BIT_ON_0
      REG32  On1                  :  1; //  1      APEX_642CL_SEQ_ENABLE_BIT_ON_1
      REG32  On2                  :  1; //  2      APEX_642CL_SEQ_ENABLE_BIT_ON_2
      REG32  On3                  :  1; //  3      APEX_642CL_SEQ_ENABLE_BIT_ON_3
      REG32  On4                  :  1; //  4      APEX_642CL_SEQ_ENABLE_BIT_ON_4
      REG32  On5                  :  1; //  5      APEX_642CL_SEQ_ENABLE_BIT_ON_5
      REG32  Reserved             : 25; //  30:06  APEX_642CL_SEQ_ENABLE_BIT_RESERVED
      REG32  Debug                :  1; //  31     APEX_642CL_SEQ_ENABLE_BIT_DEBUG
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_ENABLE_TAG                                         APEX_642CL_SEQ_ENABLE_DCR;

extern volatile APEX_642CL_SEQ_ENABLE_DCR                                        gApex642clSeqEnable;
extern volatile APEX_642CL_SEQ_ENABLE_DCR*                                       gpApex642clSeqEnable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ENABLE_BIT_ON_0                                           0
#define APEX_642CL_SEQ_ENABLE_BIT_ON_1                                           1
#define APEX_642CL_SEQ_ENABLE_BIT_ON_2                                           2
#define APEX_642CL_SEQ_ENABLE_BIT_ON_3                                           3
#define APEX_642CL_SEQ_ENABLE_BIT_ON_4                                           4
#define APEX_642CL_SEQ_ENABLE_BIT_ON_5                                           5

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ENABLE_BIT_ON_ENUM                                          
{
   APEX_642CL_SEQ_ENABLE_BIT_ON_ID_ACTIVE                                        =  1,          // Sequencer On is ACTIVE
   APEX_642CL_SEQ_ENABLE_BIT_ON_ID_INACTIVE                                      =  0           // Sequencer On is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_ENABLE_BIT_RESERVED                                       6
#define APEX_642CL_SEQ_ENABLE_BIT_DEBUG                                          31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_ENABLE_BIT_DEBUG_ENUM                                       
{
   APEX_642CL_SEQ_ENABLE_BIT_DEBUG_ID_ACTIVE                                     =  1,          // Debug mode ACTIVE
   APEX_642CL_SEQ_ENABLE_BIT_DEBUG_ID_INACTIVE                                   =  0           // Debug mode INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SCB_ENABLE
//    APEX_642CL_SEQ_SCB_ENABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_SCB_ENABLE_BASE_ADDRESS + APEX_642CL_SEQ_SCB_ENABLE_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000010
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    SCB Enable Register
//    
//    If a SCB is not 'Enabled' all sequencer related actions for that SCB are ignored.
//    To 'Disable' any SCB  the SCB_DISABLE Register should be used.
//    
// 
// Device Control Register Bits Description:
//    DMA                  [00]      DMA Channel Enable
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_0           [15:08]   Reserved 
//    STREAM_IN            [16]      Stream In DMA Channel Enable
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    STREAM_OUT           [20]      Stream Out DMA Channel Enable
//                                   
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_1           [23:22]   Reserved 
//    MC                   [24]      Motion Compensation Engine 0 On
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_2           [27:26]   Reserved 
//    HS                   [28]      Horizontal Resizer Enable
//                                   
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_3           [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_ENABLE                                                (APEX_642CL_SEQ_SCB_ENABLE_BASE_ADDRESS + APEX_642CL_SEQ_SCB_ENABLE_OFFSET)
#define APEX_642CL_SEQ_SCB_ENABLE_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_SCB_ENABLE_OFFSET                                         (0x00000010)
#define APEX_642CL_SEQ_SCB_ENABLE_DEFAULT                                        (0x00000000)


union _APEX_642CL_SEQ_SCB_ENABLE_TAG
{
   struct _asApex642clSeqScbEnableBitFieldTag
   {
      REG32  Dma0                 :  1; //  0      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_0
      REG32  Dma1                 :  1; //  1      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_1
      REG32  Dma2                 :  1; //  2      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_2
      REG32  Dma3                 :  1; //  3      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_3
      REG32  Dma4                 :  1; //  4      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_4
      REG32  Dma5                 :  1; //  5      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_5
      REG32  Dma6                 :  1; //  6      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_6
      REG32  Dma7                 :  1; //  7      APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_7
      REG32  Reserved0            :  8; //  15:08  APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_0
      REG32  StreamIn0            :  1; //  16     APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_0
      REG32  StreamIn1            :  1; //  17     APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_1
      REG32  StreamIn2            :  1; //  18     APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_2
      REG32  StreamIn3            :  1; //  19     APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_3
      REG32  StreamOut0           :  1; //  20     APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_OUT_0
      REG32  StreamOut1           :  1; //  21     APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_OUT_1
      REG32  Reserved1            :  2; //  23:22  APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_1
      REG32  Mc0                  :  1; //  24     APEX_642CL_SEQ_SCB_ENABLE_BIT_MC_0
      REG32  Mc1                  :  1; //  25     APEX_642CL_SEQ_SCB_ENABLE_BIT_MC_1
      REG32  Reserved2            :  2; //  27:26  APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_2
      REG32  Hs0                  :  1; //  28     APEX_642CL_SEQ_SCB_ENABLE_BIT_HS_0
      REG32  Hs1                  :  1; //  29     APEX_642CL_SEQ_SCB_ENABLE_BIT_HS_1
      REG32  Reserved3            :  2; //  31:30  APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_SCB_ENABLE_TAG                                     APEX_642CL_SEQ_SCB_ENABLE_DCR;

extern volatile APEX_642CL_SEQ_SCB_ENABLE_DCR                                    gApex642clSeqScbEnable;
extern volatile APEX_642CL_SEQ_SCB_ENABLE_DCR*                                   gpApex642clSeqScbEnable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_0                                      0
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_1                                      1
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_2                                      2
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_3                                      3
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_4                                      4
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_5                                      5
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_6                                      6
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_7                                      7

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_ENUM                                     
{
   APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_ID_ACTIVE                                   =  1,          // DMA channel ACTIVE
   APEX_642CL_SEQ_SCB_ENABLE_BIT_DMA_ID_INACTIVE                                 =  0           // DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_0                                 8
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_0                                16
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_1                                17
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_2                                18
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_3                                19

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_ENUM                               
{
   APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_ID_ACTIVE                             =  1,          // Stream In DMA channel ACTIVE
   APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_IN_ID_INACTIVE                           =  0           // Stream In DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_OUT_0                               20
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_OUT_1                               21

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_OUT_ENUM                              
{
   APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_OUT_ID_ACTIVE                            =  1,          // Stream Out DMA channel ACTIVE
   APEX_642CL_SEQ_SCB_ENABLE_BIT_STREAM_OUT_ID_INACTIVE                          =  0           // Stream Out DMA channel INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_1                                 22
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_MC_0                                       24
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_MC_1                                       25

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_ENABLE_BIT_MC_ENUM                                      
{
   APEX_642CL_SEQ_SCB_ENABLE_BIT_MC_ID_ACTIVE                                    =  1,          // Motion compensation engine is ACTIVE
   APEX_642CL_SEQ_SCB_ENABLE_BIT_MC_ID_INACTIVE                                  =  0           // Motion compensation engine is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_2                                 26
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_HS_0                                       28
#define APEX_642CL_SEQ_SCB_ENABLE_BIT_HS_1                                       29

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_ENABLE_BIT_HS_ENUM                                      
{
   APEX_642CL_SEQ_SCB_ENABLE_BIT_HS_ID_ACTIVE                                    =  1,          // Horizontal Resizer is ACTIVE
   APEX_642CL_SEQ_SCB_ENABLE_BIT_HS_ID_INACTIVE                                  =  0           // Horizontal Resizer is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_ENABLE_BIT_RESERVED_3                                 30

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SCB_STATUS
//    APEX_642CL_SEQ_SCB_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_SCB_STATUS_BASE_ADDRESS + APEX_642CL_SEQ_SCB_STATUS_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    SCB Wait Register
// 
// Device Control Register Bits Description:
//    SCB                  [00]      Status signal that the SCB is waiting for a trigger
//                                   
//                                   0: SCB not waiting trigger
//                                   1: SCB waiting trigger and resources not available
//                                    
//    RESERVED_0           [15:08]   Reserved 
//    SCB_16               [16]      See link 
//    SCB_17               [17]      See link 
//    SCB_18               [18]      See link 
//    SCB_19               [19]      See link 
//    SCB_20               [20]      See link 
//    SCB_21               [21]      See link 
//    RESERVED_1           [23:22]   Reserved 
//    SCB_24               [24]      See link 
//    SCB_25               [25]      See link 
//    RESERVED_2           [27:26]   Reserved 
//    SCB_28               [28]      See link 
//    SCB_29               [29]      See link 
//    RESERVED_3           [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_STATUS                                                (APEX_642CL_SEQ_SCB_STATUS_BASE_ADDRESS + APEX_642CL_SEQ_SCB_STATUS_OFFSET)
#define APEX_642CL_SEQ_SCB_STATUS_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_SCB_STATUS_OFFSET                                         (0x00000014)
#define APEX_642CL_SEQ_SCB_STATUS_DEFAULT                                        (0x00000000)


union _APEX_642CL_SEQ_SCB_STATUS_TAG
{
   struct _asApex642clSeqScbStatusBitFieldTag
   {
      REG32  Scb0                 :  1; //  0      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_0
      REG32  Scb1                 :  1; //  1      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_1
      REG32  Scb2                 :  1; //  2      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_2
      REG32  Scb3                 :  1; //  3      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_3
      REG32  Scb4                 :  1; //  4      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_4
      REG32  Scb5                 :  1; //  5      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_5
      REG32  Scb6                 :  1; //  6      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_6
      REG32  Scb7                 :  1; //  7      APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_7
      REG32  Reserved0            :  8; //  15:08  APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_0
      REG32  Scb16                :  1; //  16     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_16
      REG32  Scb17                :  1; //  17     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_17
      REG32  Scb18                :  1; //  18     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_18
      REG32  Scb19                :  1; //  19     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_19
      REG32  Scb20                :  1; //  20     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_20
      REG32  Scb21                :  1; //  21     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_21
      REG32  Reserved1            :  2; //  23:22  APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_1
      REG32  Scb24                :  1; //  24     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_24
      REG32  Scb25                :  1; //  25     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_25
      REG32  Reserved2            :  2; //  27:26  APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_2
      REG32  Scb28                :  1; //  28     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_28
      REG32  Scb29                :  1; //  29     APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_29
      REG32  Reserved3            :  2; //  31:30  APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_SCB_STATUS_TAG                                     APEX_642CL_SEQ_SCB_STATUS_DCR;

extern volatile APEX_642CL_SEQ_SCB_STATUS_DCR                                    gApex642clSeqScbStatus;
extern volatile APEX_642CL_SEQ_SCB_STATUS_DCR*                                   gpApex642clSeqScbStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_0                                      0
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_1                                      1
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_2                                      2
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_3                                      3
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_4                                      4
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_5                                      5
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_6                                      6
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_7                                      7

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_ENUM                                     
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_ID_WAITING                                  =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_ID_NOT_WAITING                              =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_0                                 8
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_16                                     16

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_16_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_16_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_16_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_17                                     17

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_17_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_17_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_17_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_18                                     18

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_18_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_18_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_18_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_19                                     19

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_19_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_19_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_19_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_20                                     20

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_20_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_20_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_20_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_21                                     21

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_21_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_21_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_21_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_1                                 22
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_24                                     24

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_24_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_24_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_24_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_25                                     25

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_25_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_25_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_25_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_2                                 26
#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_28                                     28

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_28_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_28_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_28_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_29                                     29

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_29_ENUM                                  
{
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_29_ID_WAITING                               =  1,          // SCB waiting trigger and resources not available
   APEX_642CL_SEQ_SCB_STATUS_BIT_SCB_29_ID_NOT_WAITING                           =  0           // SCB not waiting trigger
};
                                                                             

#define APEX_642CL_SEQ_SCB_STATUS_BIT_RESERVED_3                                 30

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DEBUG_STEP
//    APEX_642CL_SEQ_DEBUG_STEP
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_DEBUG_STEP_BASE_ADDRESS + APEX_642CL_SEQ_DEBUG_STEP_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    SCB Debug Step Register
//    
//    In Debug mode the sequencer issues starts and triggers only if there is a step received for the associated
//    SCB. Writing a '1' to the SCB bit position allows that SCB to issue a Start/trigger if dependent resources are
//    available
//    
// 
// Device Control Register Bits Description:
//    SCB                  [00]      Allow step for associated SCB
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_0           [15:08]   Reserved 
//    SCB_16               [16]      See link 
//    SCB_17               [17]      See link 
//    SCB_18               [18]      See link 
//    SCB_19               [19]      See link 
//    SCB_20               [20]      See link 
//    SCB_21               [21]      See link 
//    RESERVED_1           [23:22]   Reserved 
//    SCB_24               [24]      See link 
//    SCB_25               [25]      See link 
//    RESERVED_2           [27:26]   Reserved 
//    SCB_28               [28]      See link 
//    SCB_29               [29]      See link 
//    RESERVED_3           [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_DEBUG_STEP                                                (APEX_642CL_SEQ_DEBUG_STEP_BASE_ADDRESS + APEX_642CL_SEQ_DEBUG_STEP_OFFSET)
#define APEX_642CL_SEQ_DEBUG_STEP_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DEBUG_STEP_OFFSET                                         (0x00000018)
#define APEX_642CL_SEQ_DEBUG_STEP_DEFAULT                                        (0x00000000)


union _APEX_642CL_SEQ_DEBUG_STEP_TAG
{
   struct _asApex642clSeqDebugStepBitFieldTag
   {
      REG32  Scb0                 :  1; //  0      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_0
      REG32  Scb1                 :  1; //  1      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_1
      REG32  Scb2                 :  1; //  2      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_2
      REG32  Scb3                 :  1; //  3      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_3
      REG32  Scb4                 :  1; //  4      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_4
      REG32  Scb5                 :  1; //  5      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_5
      REG32  Scb6                 :  1; //  6      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_6
      REG32  Scb7                 :  1; //  7      APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_7
      REG32  Reserved0            :  8; //  15:08  APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_0
      REG32  Scb16                :  1; //  16     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_16
      REG32  Scb17                :  1; //  17     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_17
      REG32  Scb18                :  1; //  18     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_18
      REG32  Scb19                :  1; //  19     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_19
      REG32  Scb20                :  1; //  20     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_20
      REG32  Scb21                :  1; //  21     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_21
      REG32  Reserved1            :  2; //  23:22  APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_1
      REG32  Scb24                :  1; //  24     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_24
      REG32  Scb25                :  1; //  25     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_25
      REG32  Reserved2            :  2; //  27:26  APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_2
      REG32  Scb28                :  1; //  28     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_28
      REG32  Scb29                :  1; //  29     APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_29
      REG32  Reserved3            :  2; //  31:30  APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_DEBUG_STEP_TAG                                     APEX_642CL_SEQ_DEBUG_STEP_DCR;

extern volatile APEX_642CL_SEQ_DEBUG_STEP_DCR                                    gApex642clSeqDebugStep;
extern volatile APEX_642CL_SEQ_DEBUG_STEP_DCR*                                   gpApex642clSeqDebugStep;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_0                                      0
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_1                                      1
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_2                                      2
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_3                                      3
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_4                                      4
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_5                                      5
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_6                                      6
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_7                                      7

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_ENUM                                     
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_ID_ACTIVE                                   =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_ID_INACTIVE                                 =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_0                                 8
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_16                                     16

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_16_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_16_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_16_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_17                                     17

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_17_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_17_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_17_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_18                                     18

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_18_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_18_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_18_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_19                                     19

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_19_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_19_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_19_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_20                                     20

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_20_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_20_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_20_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_21                                     21

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_21_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_21_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_21_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_1                                 22
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_24                                     24

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_24_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_24_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_24_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_25                                     25

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_25_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_25_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_25_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_2                                 26
#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_28                                     28

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_28_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_28_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_28_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_29                                     29

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_29_ENUM                                  
{
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_29_ID_ACTIVE                                =  1,          // Debug step is ACTIVE
   APEX_642CL_SEQ_DEBUG_STEP_BIT_SCB_29_ID_INACTIVE                              =  0           // Debug step is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_DEBUG_STEP_BIT_RESERVED_3                                 30

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RESOURCE_NZ
//    APEX_642CL_SEQ_RESOURCE_NZ
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_RESOURCE_NZ_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_NZ_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h0000001c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    SCB Idle Register
// 
// Device Control Register Bits Description:
//    RES_NZ               [00]      Individual bits for each resource
//                                   
//                                   0: Resource coint is zero
//                                   1: Resource count is greater than zero 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_NZ                                               (APEX_642CL_SEQ_RESOURCE_NZ_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_NZ_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_NZ_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_NZ_OFFSET                                        (0x0000001c)
#define APEX_642CL_SEQ_RESOURCE_NZ_DEFAULT                                       (0x00000000)


union _APEX_642CL_SEQ_RESOURCE_NZ_TAG
{
   struct _asApex642clSeqResourceNzBitFieldTag
   {
      REG32  ResNz0               :  1; //  0      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_0
      REG32  ResNz1               :  1; //  1      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_1
      REG32  ResNz2               :  1; //  2      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_2
      REG32  ResNz3               :  1; //  3      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_3
      REG32  ResNz4               :  1; //  4      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_4
      REG32  ResNz5               :  1; //  5      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_5
      REG32  ResNz6               :  1; //  6      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_6
      REG32  ResNz7               :  1; //  7      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_7
      REG32  ResNz8               :  1; //  8      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_8
      REG32  ResNz9               :  1; //  9      APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_9
      REG32  ResNz10              :  1; //  10     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_10
      REG32  ResNz11              :  1; //  11     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_11
      REG32  ResNz12              :  1; //  12     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_12
      REG32  ResNz13              :  1; //  13     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_13
      REG32  ResNz14              :  1; //  14     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_14
      REG32  ResNz15              :  1; //  15     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_15
      REG32  ResNz16              :  1; //  16     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_16
      REG32  ResNz17              :  1; //  17     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_17
      REG32  ResNz18              :  1; //  18     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_18
      REG32  ResNz19              :  1; //  19     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_19
      REG32  ResNz20              :  1; //  20     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_20
      REG32  ResNz21              :  1; //  21     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_21
      REG32  ResNz22              :  1; //  22     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_22
      REG32  ResNz23              :  1; //  23     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_23
      REG32  ResNz24              :  1; //  24     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_24
      REG32  ResNz25              :  1; //  25     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_25
      REG32  ResNz26              :  1; //  26     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_26
      REG32  ResNz27              :  1; //  27     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_27
      REG32  ResNz28              :  1; //  28     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_28
      REG32  ResNz29              :  1; //  29     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_29
      REG32  ResNz30              :  1; //  30     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_30
      REG32  ResNz31              :  1; //  31     APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_31
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_RESOURCE_NZ_TAG                                    APEX_642CL_SEQ_RESOURCE_NZ_DCR;

extern volatile APEX_642CL_SEQ_RESOURCE_NZ_DCR                                   gApex642clSeqResourceNz;
extern volatile APEX_642CL_SEQ_RESOURCE_NZ_DCR*                                  gpApex642clSeqResourceNz;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_0                                  0
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_1                                  1
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_2                                  2
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_3                                  3
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_4                                  4
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_5                                  5
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_6                                  6
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_7                                  7
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_8                                  8
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_9                                  9
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_10                                 10
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_11                                 11
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_12                                 12
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_13                                 13
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_14                                 14
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_15                                 15
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_16                                 16
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_17                                 17
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_18                                 18
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_19                                 19
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_20                                 20
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_21                                 21
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_22                                 22
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_23                                 23
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_24                                 24
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_25                                 25
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_26                                 26
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_27                                 27
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_28                                 28
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_29                                 29
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_30                                 30
#define APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_31                                 31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_ENUM                                 
{
   APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_ID_NONZERO                              =  1,          // Resource is NONZERO
   APEX_642CL_SEQ_RESOURCE_NZ_BIT_RES_NZ_ID_ZERO                                 =  0           // Resource is ZERO
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SCB_IDLE
//    APEX_642CL_SEQ_SCB_IDLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_SCB_IDLE_BASE_ADDRESS + APEX_642CL_SEQ_SCB_IDLE_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000020
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    SCB Idle Register
//    A write '1' resets the status of the SCB to idle
// 
// Device Control Register Bits Description:
//    SCB                  [00]      Individual bits for each resource
//                                   
//                                   0: SCB is not Idle
//                                   1: SCB is Idle 
//    RESERVED_0           [15:08]   Reserved 
//    SCB_16               [16]      See link 
//    SCB_17               [17]      See link 
//    SCB_18               [18]      See link 
//    SCB_19               [19]      See link 
//    SCB_20               [20]      See link 
//    SCB_21               [21]      See link 
//    RESERVED_1           [23:22]   Reserved 
//    SCB_24               [24]      See link 
//    SCB_25               [25]      See link 
//    RESERVED_2           [27:26]   Reserved 
//    SCB_28               [28]      See link 
//    SCB_29               [29]      See link 
//    RESERVED_3           [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_IDLE                                                  (APEX_642CL_SEQ_SCB_IDLE_BASE_ADDRESS + APEX_642CL_SEQ_SCB_IDLE_OFFSET)
#define APEX_642CL_SEQ_SCB_IDLE_BASE_ADDRESS                                     (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_SCB_IDLE_OFFSET                                           (0x00000020)
#define APEX_642CL_SEQ_SCB_IDLE_DEFAULT                                          (0x00000000)


union _APEX_642CL_SEQ_SCB_IDLE_TAG
{
   struct _asApex642clSeqScbIdleBitFieldTag
   {
      REG32  Scb0                 :  1; //  0      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_0
      REG32  Scb1                 :  1; //  1      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_1
      REG32  Scb2                 :  1; //  2      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_2
      REG32  Scb3                 :  1; //  3      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_3
      REG32  Scb4                 :  1; //  4      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_4
      REG32  Scb5                 :  1; //  5      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_5
      REG32  Scb6                 :  1; //  6      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_6
      REG32  Scb7                 :  1; //  7      APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_7
      REG32  Reserved0            :  8; //  15:08  APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_0
      REG32  Scb16                :  1; //  16     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_16
      REG32  Scb17                :  1; //  17     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_17
      REG32  Scb18                :  1; //  18     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_18
      REG32  Scb19                :  1; //  19     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_19
      REG32  Scb20                :  1; //  20     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_20
      REG32  Scb21                :  1; //  21     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_21
      REG32  Reserved1            :  2; //  23:22  APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_1
      REG32  Scb24                :  1; //  24     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_24
      REG32  Scb25                :  1; //  25     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_25
      REG32  Reserved2            :  2; //  27:26  APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_2
      REG32  Scb28                :  1; //  28     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_28
      REG32  Scb29                :  1; //  29     APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_29
      REG32  Reserved3            :  2; //  31:30  APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_3
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_SCB_IDLE_TAG                                       APEX_642CL_SEQ_SCB_IDLE_DCR;

extern volatile APEX_642CL_SEQ_SCB_IDLE_DCR                                      gApex642clSeqScbIdle;
extern volatile APEX_642CL_SEQ_SCB_IDLE_DCR*                                     gpApex642clSeqScbIdle;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_0                                        0
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_1                                        1
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_2                                        2
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_3                                        3
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_4                                        4
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_5                                        5
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_6                                        6
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_7                                        7

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_ENUM                                       
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_ID_IDLE                                       =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_ID_NOT_IDLE                                   =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_0                                   8
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_16                                       16

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_16_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_16_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_16_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_17                                       17

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_17_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_17_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_17_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_18                                       18

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_18_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_18_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_18_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_19                                       19

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_19_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_19_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_19_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_20                                       20

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_20_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_20_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_20_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_21                                       21

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_21_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_21_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_21_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_1                                   22
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_24                                       24

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_24_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_24_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_24_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_25                                       25

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_25_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_25_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_25_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_2                                   26
#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_28                                       28

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_28_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_28_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_28_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_29                                       29

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_29_ENUM                                    
{
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_29_ID_IDLE                                    =  1,          // SCB is IDLE
   APEX_642CL_SEQ_SCB_IDLE_BIT_SCB_29_ID_NOT_IDLE                                =  0           // SCB is NOT_IDLE
};
                                                                             

#define APEX_642CL_SEQ_SCB_IDLE_BIT_RESERVED_3                                   30

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SEQ_DISABLE
//    APEX_642CL_SEQ_SEQ_DISABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_SEQ_DISABLE_BASE_ADDRESS + APEX_642CL_SEQ_SEQ_DISABLE_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000028
//    Default:      32'h00000000
//    Type:         rw1c
//    Nb:         1
//    Size:       4
// 
// Description:
//    Sequencer Disable Register
// 
// Device Control Register Bits Description:
//    SEQ                  [00]      Writing a '1' will change corresponding SEQ Enable bit to zero
//                                   
//                                   0: Inactive
//                                   1: Set Sequencer On X to 0 
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SEQ_DISABLE                                               (APEX_642CL_SEQ_SEQ_DISABLE_BASE_ADDRESS + APEX_642CL_SEQ_SEQ_DISABLE_OFFSET)
#define APEX_642CL_SEQ_SEQ_DISABLE_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_SEQ_DISABLE_OFFSET                                        (0x00000028)
#define APEX_642CL_SEQ_SEQ_DISABLE_DEFAULT                                       (0x00000000)


union _APEX_642CL_SEQ_SEQ_DISABLE_TAG
{
   struct _asApex642clSeqSeqDisableBitFieldTag
   {
      REG32  Seq0                 :  1; //  0      APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_0
      REG32  Seq1                 :  1; //  1      APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_1
      REG32  Seq2                 :  1; //  2      APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_2
      REG32  Seq3                 :  1; //  3      APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_3
      REG32  Seq4                 :  1; //  4      APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_4
      REG32  Seq5                 :  1; //  5      APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_5
      REG32  Reserved             : 26; //  31:06  APEX_642CL_SEQ_SEQ_DISABLE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_SEQ_DISABLE_TAG                                    APEX_642CL_SEQ_SEQ_DISABLE_DCR;

extern volatile APEX_642CL_SEQ_SEQ_DISABLE_DCR                                   gApex642clSeqSeqDisable;
extern volatile APEX_642CL_SEQ_SEQ_DISABLE_DCR*                                  gpApex642clSeqSeqDisable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_0                                     0
#define APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_1                                     1
#define APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_2                                     2
#define APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_3                                     3
#define APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_4                                     4
#define APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_5                                     5

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_ENUM                                    
{
   APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_ID_TOGGLE_DISABLE                          =  1,          // Sequencer On TOGGLE_DISABLE
   APEX_642CL_SEQ_SEQ_DISABLE_BIT_SEQ_ID_INACTIVE                                =  0           // Sequencer On INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SEQ_DISABLE_BIT_RESERVED                                  6

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    SCB_DISABLE
//    APEX_642CL_SEQ_SCB_DISABLE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_SCB_DISABLE_BASE_ADDRESS + APEX_642CL_SEQ_SCB_DISABLE_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h0000002c
//    Default:      32'h00000000
//    Type:         rw1c
//    Nb:         1
//    Size:       4
// 
// Description:
//    SCB Disable Register
// 
// Device Control Register Bits Description:
//    DMA                  [00]      Writing "1" to any bit will change corresponding DMA Channel SCB Enable bit to zero
//                                   
//                                   0: Inactive
//                                   1: Set SCB Enable X to 0  
//    RESERVED_0           [15:08]   Reserved 
//    STREAM_IN            [16]      Writing "1" to any bit will change corresponding Stream In SCB Enable bit to zero
//                                   
//                                   0: Inactive
//                                   1: Set SCB Enable X to 0 
//    STREAM_OUT           [20]      Writing "1" to any bit will change corresponding Stream Out SCB Enable bit to zero
//                                   
//                                   0: Inactive
//                                   1: Set SCB Enable X to 0 
//    RESERVED_1           [23:22]   Reserved 
//    MC                   [24]      Writing "1" to any bit will change corresponding Motion Comp SCB Enable bit to zero
//                                   
//                                   0: Inactive
//                                   1: Set SCB Enable X to 0 
//    HS                   [28]      Writing "1" to any bit will change corresponding Horizontal Resizer SCB Enable bit to zero
//                                   
//                                   0: Inactive
//                                   1: Set SCB Enable X to 0 
//    RESERVED_2           [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_DISABLE                                               (APEX_642CL_SEQ_SCB_DISABLE_BASE_ADDRESS + APEX_642CL_SEQ_SCB_DISABLE_OFFSET)
#define APEX_642CL_SEQ_SCB_DISABLE_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_SCB_DISABLE_OFFSET                                        (0x0000002c)
#define APEX_642CL_SEQ_SCB_DISABLE_DEFAULT                                       (0x00000000)


union _APEX_642CL_SEQ_SCB_DISABLE_TAG
{
   struct _asApex642clSeqScbDisableBitFieldTag
   {
      REG32  Dma0                 :  1; //  0      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_0
      REG32  Dma1                 :  1; //  1      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_1
      REG32  Dma2                 :  1; //  2      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_2
      REG32  Dma3                 :  1; //  3      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_3
      REG32  Dma4                 :  1; //  4      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_4
      REG32  Dma5                 :  1; //  5      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_5
      REG32  Dma6                 :  1; //  6      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_6
      REG32  Dma7                 :  1; //  7      APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_7
      REG32  Reserved0            :  8; //  15:08  APEX_642CL_SEQ_SCB_DISABLE_BIT_RESERVED_0
      REG32  StreamIn0            :  1; //  16     APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_0
      REG32  StreamIn1            :  1; //  17     APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_1
      REG32  StreamIn2            :  1; //  18     APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_2
      REG32  StreamIn3            :  1; //  19     APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_3
      REG32  StreamOut0           :  1; //  20     APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_OUT_0
      REG32  StreamOut1           :  1; //  21     APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_OUT_1
      REG32  Reserved1            :  2; //  23:22  APEX_642CL_SEQ_SCB_DISABLE_BIT_RESERVED_1
      REG32  Mc0                  :  1; //  24     APEX_642CL_SEQ_SCB_DISABLE_BIT_MC_0
      REG32  Mc1                  :  1; //  25     APEX_642CL_SEQ_SCB_DISABLE_BIT_MC_1
      REG32  Hs0                  :  1; //  28     APEX_642CL_SEQ_SCB_DISABLE_BIT_HS_0
      REG32  Hs1                  :  1; //  29     APEX_642CL_SEQ_SCB_DISABLE_BIT_HS_1
      REG32  Reserved2            :  2; //  31:30  APEX_642CL_SEQ_SCB_DISABLE_BIT_RESERVED_2
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_SCB_DISABLE_TAG                                    APEX_642CL_SEQ_SCB_DISABLE_DCR;

extern volatile APEX_642CL_SEQ_SCB_DISABLE_DCR                                   gApex642clSeqScbDisable;
extern volatile APEX_642CL_SEQ_SCB_DISABLE_DCR*                                  gpApex642clSeqScbDisable;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_0                                     0
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_1                                     1
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_2                                     2
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_3                                     3
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_4                                     4
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_5                                     5
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_6                                     6
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_7                                     7

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_ENUM                                    
{
   APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_ID_TOGGLE_DISABLE                          =  1,          // SCB Enable TOGGLE_DISABLE
   APEX_642CL_SEQ_SCB_DISABLE_BIT_DMA_ID_INACTIVE                                =  0           // SCB Enable INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_DISABLE_BIT_RESERVED_0                                8
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_0                               16
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_1                               17
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_2                               18
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_3                               19

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_ENUM                              
{
   APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_ID_TOGGLE_DISABLE                    =  1,          // Stream In Enable TOGGLE_DISABLE
   APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_IN_ID_INACTIVE                          =  0           // Stream In Enable INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_OUT_0                              20
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_OUT_1                              21

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_OUT_ENUM                             
{
   APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_OUT_ID_TOGGLE_DISABLE                   =  1,          // Stream Out Enable TOGGLE_DISABLE
   APEX_642CL_SEQ_SCB_DISABLE_BIT_STREAM_OUT_ID_INACTIVE                         =  0           // Stream Out Enable INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_DISABLE_BIT_RESERVED_1                                22
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_MC_0                                      24
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_MC_1                                      25

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_DISABLE_BIT_MC_ENUM                                     
{
   APEX_642CL_SEQ_SCB_DISABLE_BIT_MC_ID_TOGGLE_DISABLE                           =  1,          // Motion compensation engine is TOGGLE_DISABLE
   APEX_642CL_SEQ_SCB_DISABLE_BIT_MC_ID_INACTIVE                                 =  0           // Motion compensation engine is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_DISABLE_BIT_HS_0                                      28
#define APEX_642CL_SEQ_SCB_DISABLE_BIT_HS_1                                      29

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_SCB_DISABLE_BIT_HS_ENUM                                     
{
   APEX_642CL_SEQ_SCB_DISABLE_BIT_HS_ID_TOGGLE_DISABLE                           =  1,          // Horizontal Resizer engine is TOGGLE_DISABLE
   APEX_642CL_SEQ_SCB_DISABLE_BIT_HS_ID_INACTIVE                                 =  0           // Horizontal Resizer engine is INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_SCB_DISABLE_BIT_RESERVED_2                                30

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IMR_2
//    APEX_642CL_SEQ_IMR_2
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_IMR_2_BASE_ADDRESS + APEX_642CL_SEQ_IMR_2_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000044
//    Default:      32'hf0f0ffff
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Mask Register for Interrupt signal connected to APU0
// 
// Device Control Register Bits Description:
//    PROC_DEC             [00]      Processor X Dec was successful Interrupt Mask
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_1           [19:16]   Reserved 
//    RES_Z_IDLE           [20]      Resource X became zero and all enabled SCBs are idle Interrupt Mask 
//    RESERVED_2           [27:24]   Reserved 
//    RES_Z                [28]      Resource X became zero Interrupt Mask 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_IMR_2                                                     (APEX_642CL_SEQ_IMR_2_BASE_ADDRESS + APEX_642CL_SEQ_IMR_2_OFFSET)
#define APEX_642CL_SEQ_IMR_2_BASE_ADDRESS                                        (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_IMR_2_OFFSET                                              (0x00000044)
#define APEX_642CL_SEQ_IMR_2_DEFAULT                                             (0xf0f0ffff)


extern volatile APEX_642CL_SEQ_IMR_DCR                                           gApex642clSeqImr2;
extern volatile APEX_642CL_SEQ_IMR_DCR*                                          gpApex642clSeqImr2;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_0                                      0
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_1                                      1
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_2                                      2
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_3                                      3
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_4                                      4
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_5                                      5
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_6                                      6
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_7                                      7
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_8                                      8
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_9                                      9
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_10                                     10
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_11                                     11
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_12                                     12
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_13                                     13
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_14                                     14
#define APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_15                                     15

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_ENUM                                     
{
   APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_ID_ACTIVE                                   =  1,          // Processor Decrement successful Interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_2_BIT_PROC_DEC_ID_INACTIVE                                 =  0           // Processor Decrement successful Interrupt Mask INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_IMR_2_BIT_RESERVED_1                                      16
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_IDLE_0                                    20
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_IDLE_1                                    21
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_IDLE_2                                    22
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_IDLE_3                                    23

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_2_BIT_RES_Z_IDLE_ENUM                                   
{
   APEX_642CL_SEQ_IMR_2_BIT_RES_Z_IDLE_ID_ACTIVE                                 =  1,          // Resource became zero and all enabled SCBs are idle Interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_2_BIT_RES_Z_IDLE_ID_INACTIVE                               =  0           // Resource became zero and all enabled SCBs are idle Interrupt Mask INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_IMR_2_BIT_RESERVED_2                                      24
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_0                                         28
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_1                                         29
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_2                                         30
#define APEX_642CL_SEQ_IMR_2_BIT_RES_Z_3                                         31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_2_BIT_RES_Z_ENUM                                        
{
   APEX_642CL_SEQ_IMR_2_BIT_RES_Z_ID_ACTIVE                                      =  1,          // Resource became zero interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_2_BIT_RES_Z_ID_INACTIVE                                    =  0           // Resource became zero interrupt Mask INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IMR_3
//    APEX_642CL_SEQ_IMR_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_IMR_3_BASE_ADDRESS + APEX_642CL_SEQ_IMR_3_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000048
//    Default:      32'hf0f0ffff
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Interrupt Mask Register for Interrupt signal connected to APU1
// 
// Device Control Register Bits Description:
//    PROC_DEC             [00]      Processor X Dec was successful Interrupt Mask
//                                   
//                                   0: Inactive
//                                   1: Active
//                                    
//    RESERVED_1           [19:16]   Reserved 
//    RES_Z_IDLE           [20]      Resource X became zero and all enabled SCBs are idle Interrupt Mask 
//    RESERVED_2           [27:24]   Reserved 
//    RES_Z                [28]      Resource X became zero Interrupt Mask 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_IMR_3                                                     (APEX_642CL_SEQ_IMR_3_BASE_ADDRESS + APEX_642CL_SEQ_IMR_3_OFFSET)
#define APEX_642CL_SEQ_IMR_3_BASE_ADDRESS                                        (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_IMR_3_OFFSET                                              (0x00000048)
#define APEX_642CL_SEQ_IMR_3_DEFAULT                                             (0xf0f0ffff)


extern volatile APEX_642CL_SEQ_IMR_DCR                                           gApex642clSeqImr3;
extern volatile APEX_642CL_SEQ_IMR_DCR*                                          gpApex642clSeqImr3;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_0                                      0
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_1                                      1
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_2                                      2
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_3                                      3
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_4                                      4
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_5                                      5
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_6                                      6
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_7                                      7
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_8                                      8
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_9                                      9
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_10                                     10
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_11                                     11
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_12                                     12
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_13                                     13
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_14                                     14
#define APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_15                                     15

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_ENUM                                     
{
   APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_ID_ACTIVE                                   =  1,          // Processor Decrement successful Interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_3_BIT_PROC_DEC_ID_INACTIVE                                 =  0           // Processor Decrement successful Interrupt Mask INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_IMR_3_BIT_RESERVED_1                                      16
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_IDLE_0                                    20
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_IDLE_1                                    21
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_IDLE_2                                    22
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_IDLE_3                                    23

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_3_BIT_RES_Z_IDLE_ENUM                                   
{
   APEX_642CL_SEQ_IMR_3_BIT_RES_Z_IDLE_ID_ACTIVE                                 =  1,          // Resource became zero and all enabled SCBs are idle Interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_3_BIT_RES_Z_IDLE_ID_INACTIVE                               =  0           // Resource became zero and all enabled SCBs are idle Interrupt Mask INACTIVE
};
                                                                             

#define APEX_642CL_SEQ_IMR_3_BIT_RESERVED_2                                      24
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_0                                         28
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_1                                         29
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_2                                         30
#define APEX_642CL_SEQ_IMR_3_BIT_RES_Z_3                                         31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_IMR_3_BIT_RES_Z_ENUM                                        
{
   APEX_642CL_SEQ_IMR_3_BIT_RES_Z_ID_ACTIVE                                      =  1,          // Resource became zero interrupt Mask ACTIVE
   APEX_642CL_SEQ_IMR_3_BIT_RES_Z_ID_INACTIVE                                    =  0           // Resource became zero interrupt Mask INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    ARM_CORRECTION_RES
//    APEX_642CL_SEQ_ARM_CORRECTION_RES
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_ARM_CORRECTION_RES_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000080
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         8
//    Size:       4
// 
// Description:
//    ARM Correction Register
//    
//    Value to increment the fractional resource by, to correct for drift due to number
//    of significant bits
//    
// 
// Device Control Register Bits Description:
//    FRACTION_INC         [11:00]   Value added to fractional portion with rollover to integer 
//    INTEGER_INC          [15:12]   Value added to integer portion 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_0                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_0_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_0_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_0_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_0_OFFSET                               (0x00000080)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_0_DEFAULT                              (0x00000000)

#define APEX_642CL_SEQ_ARM_CORRECTION_RES_1                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_1_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_1_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_1_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_1_OFFSET                               (0x00000084)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_1_DEFAULT                              (0x00000000)

#define APEX_642CL_SEQ_ARM_CORRECTION_RES_2                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_2_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_2_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_2_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_2_OFFSET                               (0x00000088)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_2_DEFAULT                              (0x00000000)

#define APEX_642CL_SEQ_ARM_CORRECTION_RES_3                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_3_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_3_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_3_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_3_OFFSET                               (0x0000008c)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_3_DEFAULT                              (0x00000000)

#define APEX_642CL_SEQ_ARM_CORRECTION_RES_4                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_4_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_4_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_4_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_4_OFFSET                               (0x00000090)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_4_DEFAULT                              (0x00000000)

#define APEX_642CL_SEQ_ARM_CORRECTION_RES_5                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_5_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_5_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_5_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_5_OFFSET                               (0x00000094)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_5_DEFAULT                              (0x00000000)

#define APEX_642CL_SEQ_ARM_CORRECTION_RES_6                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_6_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_6_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_6_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_6_OFFSET                               (0x00000098)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_6_DEFAULT                              (0x00000000)

#define APEX_642CL_SEQ_ARM_CORRECTION_RES_7                                      (APEX_642CL_SEQ_ARM_CORRECTION_RES_7_BASE_ADDRESS + APEX_642CL_SEQ_ARM_CORRECTION_RES_7_OFFSET)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_7_BASE_ADDRESS                         (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_7_OFFSET                               (0x0000009c)
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_7_DEFAULT                              (0x00000000)


union _APEX_642CL_SEQ_ARM_CORRECTION_RES_TAG
{
   struct _asApex642clSeqArmCorrectionResBitFieldTag
   {
      REG32  FractionInc          : 12; //  11:00  APEX_642CL_SEQ_ARM_CORRECTION_RES_BIT_FRACTION_INC
      REG32  IntegerInc           :  4; //  15:12  APEX_642CL_SEQ_ARM_CORRECTION_RES_BIT_INTEGER_INC
      REG32  Reserved             : 16; //  31:16  APEX_642CL_SEQ_ARM_CORRECTION_RES_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_ARM_CORRECTION_RES_TAG                             APEX_642CL_SEQ_ARM_CORRECTION_RES_DCR;

extern volatile APEX_642CL_SEQ_ARM_CORRECTION_RES_DCR                            gApex642clSeqArmCorrectionRes;
extern volatile APEX_642CL_SEQ_ARM_CORRECTION_RES_DCR*                           gpApex642clSeqArmCorrectionRes;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_BIT_FRACTION_INC                       0
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_BIT_INTEGER_INC                        12
#define APEX_642CL_SEQ_ARM_CORRECTION_RES_BIT_RESERVED                           16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PROC_DEC
//    APEX_642CL_SEQ_PROC_DEC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_PROC_DEC_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000100
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         16
//    Size:       4
// 
// Description:
//    Processor Decrement Register
//    
//    Bit fields correspond to resources,  (bit[0] = resource_0, bit[17] resource_17, etc ) any bit field 
//    that is set will result in that resource's value being decremented.
//    If the resource value to be decremented is already zero the decrement action will be buffered 
//    and will only be performed once the resource value is non-zero.An interrupt status is set when 
//    a Processor Decrement request is succesfully executed.The association of PROC_DEC_x register 
//    to a specific Processor in the system is not HW controlled and is up to the user.It is discouraged 
//    to have multiple Processors in the system use the same PROC_DEC_X register to control two 
//    separate threads as two Processors accessing the same register will only result in only one of 
//    the decrement sets being executed.
//    The read back value is the pending decrements.
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_PROC_DEC_0                                                (APEX_642CL_SEQ_PROC_DEC_0_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_0_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_0_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_0_OFFSET                                         (0x00000100)
#define APEX_642CL_SEQ_PROC_DEC_0_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_1                                                (APEX_642CL_SEQ_PROC_DEC_1_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_1_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_1_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_1_OFFSET                                         (0x00000104)
#define APEX_642CL_SEQ_PROC_DEC_1_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_2                                                (APEX_642CL_SEQ_PROC_DEC_2_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_2_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_2_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_2_OFFSET                                         (0x00000108)
#define APEX_642CL_SEQ_PROC_DEC_2_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_3                                                (APEX_642CL_SEQ_PROC_DEC_3_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_3_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_3_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_3_OFFSET                                         (0x0000010c)
#define APEX_642CL_SEQ_PROC_DEC_3_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_4                                                (APEX_642CL_SEQ_PROC_DEC_4_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_4_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_4_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_4_OFFSET                                         (0x00000110)
#define APEX_642CL_SEQ_PROC_DEC_4_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_5                                                (APEX_642CL_SEQ_PROC_DEC_5_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_5_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_5_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_5_OFFSET                                         (0x00000114)
#define APEX_642CL_SEQ_PROC_DEC_5_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_6                                                (APEX_642CL_SEQ_PROC_DEC_6_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_6_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_6_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_6_OFFSET                                         (0x00000118)
#define APEX_642CL_SEQ_PROC_DEC_6_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_7                                                (APEX_642CL_SEQ_PROC_DEC_7_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_7_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_7_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_7_OFFSET                                         (0x0000011c)
#define APEX_642CL_SEQ_PROC_DEC_7_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_8                                                (APEX_642CL_SEQ_PROC_DEC_8_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_8_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_8_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_8_OFFSET                                         (0x00000120)
#define APEX_642CL_SEQ_PROC_DEC_8_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_9                                                (APEX_642CL_SEQ_PROC_DEC_9_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_9_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_9_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_9_OFFSET                                         (0x00000124)
#define APEX_642CL_SEQ_PROC_DEC_9_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_10                                               (APEX_642CL_SEQ_PROC_DEC_10_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_10_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_10_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_10_OFFSET                                        (0x00000128)
#define APEX_642CL_SEQ_PROC_DEC_10_DEFAULT                                       (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_11                                               (APEX_642CL_SEQ_PROC_DEC_11_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_11_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_11_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_11_OFFSET                                        (0x0000012c)
#define APEX_642CL_SEQ_PROC_DEC_11_DEFAULT                                       (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_12                                               (APEX_642CL_SEQ_PROC_DEC_12_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_12_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_12_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_12_OFFSET                                        (0x00000130)
#define APEX_642CL_SEQ_PROC_DEC_12_DEFAULT                                       (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_13                                               (APEX_642CL_SEQ_PROC_DEC_13_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_13_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_13_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_13_OFFSET                                        (0x00000134)
#define APEX_642CL_SEQ_PROC_DEC_13_DEFAULT                                       (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_14                                               (APEX_642CL_SEQ_PROC_DEC_14_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_14_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_14_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_14_OFFSET                                        (0x00000138)
#define APEX_642CL_SEQ_PROC_DEC_14_DEFAULT                                       (0x00000000)

#define APEX_642CL_SEQ_PROC_DEC_15                                               (APEX_642CL_SEQ_PROC_DEC_15_BASE_ADDRESS + APEX_642CL_SEQ_PROC_DEC_15_OFFSET)
#define APEX_642CL_SEQ_PROC_DEC_15_BASE_ADDRESS                                  (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_DEC_15_OFFSET                                        (0x0000013c)
#define APEX_642CL_SEQ_PROC_DEC_15_DEFAULT                                       (0x00000000)


union _APEX_642CL_SEQ_PROC_DEC_TAG
{
   struct _asApex642clSeqProcDecBitFieldTag
   {
      REG32  Res0                 :  1; //  0      APEX_642CL_SEQ_PROC_DEC_BIT_RES_0
      REG32  Res1                 :  1; //  1      APEX_642CL_SEQ_PROC_DEC_BIT_RES_1
      REG32  Res2                 :  1; //  2      APEX_642CL_SEQ_PROC_DEC_BIT_RES_2
      REG32  Res3                 :  1; //  3      APEX_642CL_SEQ_PROC_DEC_BIT_RES_3
      REG32  Res4                 :  1; //  4      APEX_642CL_SEQ_PROC_DEC_BIT_RES_4
      REG32  Res5                 :  1; //  5      APEX_642CL_SEQ_PROC_DEC_BIT_RES_5
      REG32  Res6                 :  1; //  6      APEX_642CL_SEQ_PROC_DEC_BIT_RES_6
      REG32  Res7                 :  1; //  7      APEX_642CL_SEQ_PROC_DEC_BIT_RES_7
      REG32  Res8                 :  1; //  8      APEX_642CL_SEQ_PROC_DEC_BIT_RES_8
      REG32  Res9                 :  1; //  9      APEX_642CL_SEQ_PROC_DEC_BIT_RES_9
      REG32  Res10                :  1; //  10     APEX_642CL_SEQ_PROC_DEC_BIT_RES_10
      REG32  Res11                :  1; //  11     APEX_642CL_SEQ_PROC_DEC_BIT_RES_11
      REG32  Res12                :  1; //  12     APEX_642CL_SEQ_PROC_DEC_BIT_RES_12
      REG32  Res13                :  1; //  13     APEX_642CL_SEQ_PROC_DEC_BIT_RES_13
      REG32  Res14                :  1; //  14     APEX_642CL_SEQ_PROC_DEC_BIT_RES_14
      REG32  Res15                :  1; //  15     APEX_642CL_SEQ_PROC_DEC_BIT_RES_15
      REG32  Res16                :  1; //  16     APEX_642CL_SEQ_PROC_DEC_BIT_RES_16
      REG32  Res17                :  1; //  17     APEX_642CL_SEQ_PROC_DEC_BIT_RES_17
      REG32  Res18                :  1; //  18     APEX_642CL_SEQ_PROC_DEC_BIT_RES_18
      REG32  Res19                :  1; //  19     APEX_642CL_SEQ_PROC_DEC_BIT_RES_19
      REG32  Res20                :  1; //  20     APEX_642CL_SEQ_PROC_DEC_BIT_RES_20
      REG32  Res21                :  1; //  21     APEX_642CL_SEQ_PROC_DEC_BIT_RES_21
      REG32  Res22                :  1; //  22     APEX_642CL_SEQ_PROC_DEC_BIT_RES_22
      REG32  Res23                :  1; //  23     APEX_642CL_SEQ_PROC_DEC_BIT_RES_23
      REG32  Res24                :  1; //  24     APEX_642CL_SEQ_PROC_DEC_BIT_RES_24
      REG32  Res25                :  1; //  25     APEX_642CL_SEQ_PROC_DEC_BIT_RES_25
      REG32  Res26                :  1; //  26     APEX_642CL_SEQ_PROC_DEC_BIT_RES_26
      REG32  Res27                :  1; //  27     APEX_642CL_SEQ_PROC_DEC_BIT_RES_27
      REG32  Res28                :  1; //  28     APEX_642CL_SEQ_PROC_DEC_BIT_RES_28
      REG32  Res29                :  1; //  29     APEX_642CL_SEQ_PROC_DEC_BIT_RES_29
      REG32  Res30                :  1; //  30     APEX_642CL_SEQ_PROC_DEC_BIT_RES_30
      REG32  Res31                :  1; //  31     APEX_642CL_SEQ_PROC_DEC_BIT_RES_31
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_PROC_DEC_TAG                                       APEX_642CL_SEQ_PROC_DEC_DCR;

extern volatile APEX_642CL_SEQ_PROC_DEC_DCR                                      gApex642clSeqProcDec;
extern volatile APEX_642CL_SEQ_PROC_DEC_DCR*                                     gpApex642clSeqProcDec;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_0                                        0
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_1                                        1
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_2                                        2
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_3                                        3
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_4                                        4
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_5                                        5
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_6                                        6
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_7                                        7
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_8                                        8
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_9                                        9
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_10                                       10
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_11                                       11
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_12                                       12
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_13                                       13
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_14                                       14
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_15                                       15
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_16                                       16
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_17                                       17
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_18                                       18
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_19                                       19
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_20                                       20
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_21                                       21
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_22                                       22
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_23                                       23
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_24                                       24
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_25                                       25
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_26                                       26
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_27                                       27
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_28                                       28
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_29                                       29
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_30                                       30
#define APEX_642CL_SEQ_PROC_DEC_BIT_RES_31                                       31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_PROC_DEC_BIT_RES_ENUM                                       
{
   APEX_642CL_SEQ_PROC_DEC_BIT_RES_ID_ACTIVE                                     =  1,          // Decrement Resource 
   APEX_642CL_SEQ_PROC_DEC_BIT_RES_ID_INACTIVE                                   =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DMA_DEC
//    APEX_642CL_SEQ_DMA_DEC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_DMA_DEC_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000180
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         8
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_DMA_DEC_0                                                 (APEX_642CL_SEQ_DMA_DEC_0_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_0_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_0_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_0_OFFSET                                          (0x00000180)
#define APEX_642CL_SEQ_DMA_DEC_0_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_DEC_1                                                 (APEX_642CL_SEQ_DMA_DEC_1_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_1_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_1_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_1_OFFSET                                          (0x00000184)
#define APEX_642CL_SEQ_DMA_DEC_1_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_DEC_2                                                 (APEX_642CL_SEQ_DMA_DEC_2_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_2_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_2_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_2_OFFSET                                          (0x00000188)
#define APEX_642CL_SEQ_DMA_DEC_2_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_DEC_3                                                 (APEX_642CL_SEQ_DMA_DEC_3_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_3_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_3_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_3_OFFSET                                          (0x0000018c)
#define APEX_642CL_SEQ_DMA_DEC_3_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_DEC_4                                                 (APEX_642CL_SEQ_DMA_DEC_4_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_4_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_4_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_4_OFFSET                                          (0x00000190)
#define APEX_642CL_SEQ_DMA_DEC_4_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_DEC_5                                                 (APEX_642CL_SEQ_DMA_DEC_5_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_5_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_5_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_5_OFFSET                                          (0x00000194)
#define APEX_642CL_SEQ_DMA_DEC_5_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_DEC_6                                                 (APEX_642CL_SEQ_DMA_DEC_6_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_6_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_6_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_6_OFFSET                                          (0x00000198)
#define APEX_642CL_SEQ_DMA_DEC_6_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_DEC_7                                                 (APEX_642CL_SEQ_DMA_DEC_7_BASE_ADDRESS + APEX_642CL_SEQ_DMA_DEC_7_OFFSET)
#define APEX_642CL_SEQ_DMA_DEC_7_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_DEC_7_OFFSET                                          (0x0000019c)
#define APEX_642CL_SEQ_DMA_DEC_7_DEFAULT                                         (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_DEC_DCR                                      gApex642clSeqDmaDec;
extern volatile APEX_642CL_SEQ_PROC_DEC_DCR*                                     gpApex642clSeqDmaDec;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_0                                         0
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_1                                         1
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_2                                         2
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_3                                         3
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_4                                         4
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_5                                         5
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_6                                         6
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_7                                         7
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_8                                         8
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_9                                         9
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_10                                        10
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_11                                        11
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_12                                        12
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_13                                        13
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_14                                        14
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_15                                        15
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_16                                        16
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_17                                        17
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_18                                        18
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_19                                        19
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_20                                        20
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_21                                        21
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_22                                        22
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_23                                        23
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_24                                        24
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_25                                        25
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_26                                        26
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_27                                        27
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_28                                        28
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_29                                        29
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_30                                        30
#define APEX_642CL_SEQ_DMA_DEC_BIT_RES_31                                        31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DMA_DEC_BIT_RES_ENUM                                        
{
   APEX_642CL_SEQ_DMA_DEC_BIT_RES_ID_ACTIVE                                      =  1,          // Decrement Resource 
   APEX_642CL_SEQ_DMA_DEC_BIT_RES_ID_INACTIVE                                    =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STREAM_IN_DEC
//    APEX_642CL_SEQ_STREAM_IN_DEC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_STREAM_IN_DEC_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_DEC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000001c0
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_IN_DEC_0                                           (APEX_642CL_SEQ_STREAM_IN_DEC_0_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_DEC_0_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_DEC_0_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_DEC_0_OFFSET                                    (0x000001c0)
#define APEX_642CL_SEQ_STREAM_IN_DEC_0_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_STREAM_IN_DEC_1                                           (APEX_642CL_SEQ_STREAM_IN_DEC_1_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_DEC_1_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_DEC_1_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_DEC_1_OFFSET                                    (0x000001c4)
#define APEX_642CL_SEQ_STREAM_IN_DEC_1_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_STREAM_IN_DEC_2                                           (APEX_642CL_SEQ_STREAM_IN_DEC_2_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_DEC_2_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_DEC_2_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_DEC_2_OFFSET                                    (0x000001c8)
#define APEX_642CL_SEQ_STREAM_IN_DEC_2_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_STREAM_IN_DEC_3                                           (APEX_642CL_SEQ_STREAM_IN_DEC_3_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_DEC_3_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_DEC_3_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_DEC_3_OFFSET                                    (0x000001cc)
#define APEX_642CL_SEQ_STREAM_IN_DEC_3_DEFAULT                                   (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_DEC_DCR                                      gApex642clSeqStreamInDec;
extern volatile APEX_642CL_SEQ_PROC_DEC_DCR*                                     gpApex642clSeqStreamInDec;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_0                                   0
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_1                                   1
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_2                                   2
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_3                                   3
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_4                                   4
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_5                                   5
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_6                                   6
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_7                                   7
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_8                                   8
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_9                                   9
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_10                                  10
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_11                                  11
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_12                                  12
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_13                                  13
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_14                                  14
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_15                                  15
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_16                                  16
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_17                                  17
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_18                                  18
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_19                                  19
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_20                                  20
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_21                                  21
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_22                                  22
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_23                                  23
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_24                                  24
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_25                                  25
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_26                                  26
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_27                                  27
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_28                                  28
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_29                                  29
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_30                                  30
#define APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_31                                  31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_ENUM                                  
{
   APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_ID_ACTIVE                                =  1,          // Decrement Resource 
   APEX_642CL_SEQ_STREAM_IN_DEC_BIT_RES_ID_INACTIVE                              =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STREAM_OUT_DEC
//    APEX_642CL_SEQ_STREAM_OUT_DEC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_STREAM_OUT_DEC_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_OUT_DEC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000001d0
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_OUT_DEC_0                                          (APEX_642CL_SEQ_STREAM_OUT_DEC_0_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_OUT_DEC_0_OFFSET)
#define APEX_642CL_SEQ_STREAM_OUT_DEC_0_BASE_ADDRESS                             (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_OUT_DEC_0_OFFSET                                   (0x000001d0)
#define APEX_642CL_SEQ_STREAM_OUT_DEC_0_DEFAULT                                  (0x00000000)

#define APEX_642CL_SEQ_STREAM_OUT_DEC_1                                          (APEX_642CL_SEQ_STREAM_OUT_DEC_1_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_OUT_DEC_1_OFFSET)
#define APEX_642CL_SEQ_STREAM_OUT_DEC_1_BASE_ADDRESS                             (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_OUT_DEC_1_OFFSET                                   (0x000001d4)
#define APEX_642CL_SEQ_STREAM_OUT_DEC_1_DEFAULT                                  (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_DEC_DCR                                      gApex642clSeqStreamOutDec;
extern volatile APEX_642CL_SEQ_PROC_DEC_DCR*                                     gpApex642clSeqStreamOutDec;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_0                                  0
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_1                                  1
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_2                                  2
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_3                                  3
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_4                                  4
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_5                                  5
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_6                                  6
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_7                                  7
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_8                                  8
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_9                                  9
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_10                                 10
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_11                                 11
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_12                                 12
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_13                                 13
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_14                                 14
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_15                                 15
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_16                                 16
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_17                                 17
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_18                                 18
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_19                                 19
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_20                                 20
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_21                                 21
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_22                                 22
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_23                                 23
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_24                                 24
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_25                                 25
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_26                                 26
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_27                                 27
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_28                                 28
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_29                                 29
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_30                                 30
#define APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_31                                 31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_ENUM                                 
{
   APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_ID_ACTIVE                               =  1,          // Decrement Resource 
   APEX_642CL_SEQ_STREAM_OUT_DEC_BIT_RES_ID_INACTIVE                             =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MOTION_COMP_DEC
//    APEX_642CL_SEQ_MOTION_COMP_DEC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_MOTION_COMP_DEC_BASE_ADDRESS + APEX_642CL_SEQ_MOTION_COMP_DEC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000001e0
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_MOTION_COMP_DEC_0                                         (APEX_642CL_SEQ_MOTION_COMP_DEC_0_BASE_ADDRESS + APEX_642CL_SEQ_MOTION_COMP_DEC_0_OFFSET)
#define APEX_642CL_SEQ_MOTION_COMP_DEC_0_BASE_ADDRESS                            (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_MOTION_COMP_DEC_0_OFFSET                                  (0x000001e0)
#define APEX_642CL_SEQ_MOTION_COMP_DEC_0_DEFAULT                                 (0x00000000)

#define APEX_642CL_SEQ_MOTION_COMP_DEC_1                                         (APEX_642CL_SEQ_MOTION_COMP_DEC_1_BASE_ADDRESS + APEX_642CL_SEQ_MOTION_COMP_DEC_1_OFFSET)
#define APEX_642CL_SEQ_MOTION_COMP_DEC_1_BASE_ADDRESS                            (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_MOTION_COMP_DEC_1_OFFSET                                  (0x000001e4)
#define APEX_642CL_SEQ_MOTION_COMP_DEC_1_DEFAULT                                 (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_DEC_DCR                                      gApex642clSeqMotionCompDec;
extern volatile APEX_642CL_SEQ_PROC_DEC_DCR*                                     gpApex642clSeqMotionCompDec;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_0                                 0
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_1                                 1
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_2                                 2
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_3                                 3
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_4                                 4
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_5                                 5
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_6                                 6
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_7                                 7
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_8                                 8
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_9                                 9
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_10                                10
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_11                                11
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_12                                12
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_13                                13
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_14                                14
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_15                                15
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_16                                16
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_17                                17
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_18                                18
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_19                                19
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_20                                20
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_21                                21
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_22                                22
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_23                                23
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_24                                24
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_25                                25
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_26                                26
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_27                                27
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_28                                28
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_29                                29
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_30                                30
#define APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_31                                31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_ENUM                                
{
   APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_ID_ACTIVE                              =  1,          // Decrement Resource 
   APEX_642CL_SEQ_MOTION_COMP_DEC_BIT_RES_ID_INACTIVE                            =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    HORIZ_RESIZER_DEC
//    APEX_642CL_SEQ_HORIZ_RESIZER_DEC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BASE_ADDRESS + APEX_642CL_SEQ_HORIZ_RESIZER_DEC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000001f0
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_0                                       (APEX_642CL_SEQ_HORIZ_RESIZER_DEC_0_BASE_ADDRESS + APEX_642CL_SEQ_HORIZ_RESIZER_DEC_0_OFFSET)
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_0_BASE_ADDRESS                          (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_0_OFFSET                                (0x000001f0)
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_0_DEFAULT                               (0x00000000)

#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_1                                       (APEX_642CL_SEQ_HORIZ_RESIZER_DEC_1_BASE_ADDRESS + APEX_642CL_SEQ_HORIZ_RESIZER_DEC_1_OFFSET)
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_1_BASE_ADDRESS                          (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_1_OFFSET                                (0x000001f4)
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_1_DEFAULT                               (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_DEC_DCR                                      gApex642clSeqHorizResizerDec;
extern volatile APEX_642CL_SEQ_PROC_DEC_DCR*                                     gpApex642clSeqHorizResizerDec;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_0                               0
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_1                               1
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_2                               2
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_3                               3
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_4                               4
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_5                               5
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_6                               6
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_7                               7
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_8                               8
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_9                               9
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_10                              10
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_11                              11
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_12                              12
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_13                              13
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_14                              14
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_15                              15
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_16                              16
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_17                              17
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_18                              18
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_19                              19
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_20                              20
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_21                              21
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_22                              22
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_23                              23
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_24                              24
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_25                              25
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_26                              26
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_27                              27
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_28                              28
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_29                              29
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_30                              30
#define APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_31                              31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_ENUM                              
{
   APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_ID_ACTIVE                            =  1,          // Decrement Resource 
   APEX_642CL_SEQ_HORIZ_RESIZER_DEC_BIT_RES_ID_INACTIVE                          =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PROC_INC
//    APEX_642CL_SEQ_PROC_INC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_PROC_INC_BASE_ADDRESS + APEX_642CL_SEQ_PROC_INC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000200
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         6
//    Size:       4
// 
// Description:
//    Processor Increment Register
//    
//    Bit fields correspond to resources,  (bit[0] = resource_0, bit[17] resource_17, etc. ) any bit field that is set will result
//    in that resource's value being incremented. Fractional resources will increment by the Increment value configured in
//    their associated Increment field, INC,  RESOURCE_FRAC[31:16].
//    
//    The association of PROC_INC_x register to a specific Processor in the system is not HW controlled and is up to the
//    user. It is discouraged to have multiple Processors in the system use the same PROC_INC_X register to control two
//    separate threads as two Processors accessing the same register will only result in only one of the increment sets being
//    propagated.
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_PROC_INC_0                                                (APEX_642CL_SEQ_PROC_INC_0_BASE_ADDRESS + APEX_642CL_SEQ_PROC_INC_0_OFFSET)
#define APEX_642CL_SEQ_PROC_INC_0_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_INC_0_OFFSET                                         (0x00000200)
#define APEX_642CL_SEQ_PROC_INC_0_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_INC_1                                                (APEX_642CL_SEQ_PROC_INC_1_BASE_ADDRESS + APEX_642CL_SEQ_PROC_INC_1_OFFSET)
#define APEX_642CL_SEQ_PROC_INC_1_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_INC_1_OFFSET                                         (0x00000204)
#define APEX_642CL_SEQ_PROC_INC_1_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_INC_2                                                (APEX_642CL_SEQ_PROC_INC_2_BASE_ADDRESS + APEX_642CL_SEQ_PROC_INC_2_OFFSET)
#define APEX_642CL_SEQ_PROC_INC_2_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_INC_2_OFFSET                                         (0x00000208)
#define APEX_642CL_SEQ_PROC_INC_2_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_INC_3                                                (APEX_642CL_SEQ_PROC_INC_3_BASE_ADDRESS + APEX_642CL_SEQ_PROC_INC_3_OFFSET)
#define APEX_642CL_SEQ_PROC_INC_3_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_INC_3_OFFSET                                         (0x0000020c)
#define APEX_642CL_SEQ_PROC_INC_3_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_INC_4                                                (APEX_642CL_SEQ_PROC_INC_4_BASE_ADDRESS + APEX_642CL_SEQ_PROC_INC_4_OFFSET)
#define APEX_642CL_SEQ_PROC_INC_4_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_INC_4_OFFSET                                         (0x00000210)
#define APEX_642CL_SEQ_PROC_INC_4_DEFAULT                                        (0x00000000)

#define APEX_642CL_SEQ_PROC_INC_5                                                (APEX_642CL_SEQ_PROC_INC_5_BASE_ADDRESS + APEX_642CL_SEQ_PROC_INC_5_OFFSET)
#define APEX_642CL_SEQ_PROC_INC_5_BASE_ADDRESS                                   (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_PROC_INC_5_OFFSET                                         (0x00000214)
#define APEX_642CL_SEQ_PROC_INC_5_DEFAULT                                        (0x00000000)


union _APEX_642CL_SEQ_PROC_INC_TAG
{
   struct _asApex642clSeqProcIncBitFieldTag
   {
      REG32  Res0                 :  1; //  0      APEX_642CL_SEQ_PROC_INC_BIT_RES_0
      REG32  Res1                 :  1; //  1      APEX_642CL_SEQ_PROC_INC_BIT_RES_1
      REG32  Res2                 :  1; //  2      APEX_642CL_SEQ_PROC_INC_BIT_RES_2
      REG32  Res3                 :  1; //  3      APEX_642CL_SEQ_PROC_INC_BIT_RES_3
      REG32  Res4                 :  1; //  4      APEX_642CL_SEQ_PROC_INC_BIT_RES_4
      REG32  Res5                 :  1; //  5      APEX_642CL_SEQ_PROC_INC_BIT_RES_5
      REG32  Res6                 :  1; //  6      APEX_642CL_SEQ_PROC_INC_BIT_RES_6
      REG32  Res7                 :  1; //  7      APEX_642CL_SEQ_PROC_INC_BIT_RES_7
      REG32  Res8                 :  1; //  8      APEX_642CL_SEQ_PROC_INC_BIT_RES_8
      REG32  Res9                 :  1; //  9      APEX_642CL_SEQ_PROC_INC_BIT_RES_9
      REG32  Res10                :  1; //  10     APEX_642CL_SEQ_PROC_INC_BIT_RES_10
      REG32  Res11                :  1; //  11     APEX_642CL_SEQ_PROC_INC_BIT_RES_11
      REG32  Res12                :  1; //  12     APEX_642CL_SEQ_PROC_INC_BIT_RES_12
      REG32  Res13                :  1; //  13     APEX_642CL_SEQ_PROC_INC_BIT_RES_13
      REG32  Res14                :  1; //  14     APEX_642CL_SEQ_PROC_INC_BIT_RES_14
      REG32  Res15                :  1; //  15     APEX_642CL_SEQ_PROC_INC_BIT_RES_15
      REG32  Res16                :  1; //  16     APEX_642CL_SEQ_PROC_INC_BIT_RES_16
      REG32  Res17                :  1; //  17     APEX_642CL_SEQ_PROC_INC_BIT_RES_17
      REG32  Res18                :  1; //  18     APEX_642CL_SEQ_PROC_INC_BIT_RES_18
      REG32  Res19                :  1; //  19     APEX_642CL_SEQ_PROC_INC_BIT_RES_19
      REG32  Res20                :  1; //  20     APEX_642CL_SEQ_PROC_INC_BIT_RES_20
      REG32  Res21                :  1; //  21     APEX_642CL_SEQ_PROC_INC_BIT_RES_21
      REG32  Res22                :  1; //  22     APEX_642CL_SEQ_PROC_INC_BIT_RES_22
      REG32  Res23                :  1; //  23     APEX_642CL_SEQ_PROC_INC_BIT_RES_23
      REG32  Res24                :  1; //  24     APEX_642CL_SEQ_PROC_INC_BIT_RES_24
      REG32  Res25                :  1; //  25     APEX_642CL_SEQ_PROC_INC_BIT_RES_25
      REG32  Res26                :  1; //  26     APEX_642CL_SEQ_PROC_INC_BIT_RES_26
      REG32  Res27                :  1; //  27     APEX_642CL_SEQ_PROC_INC_BIT_RES_27
      REG32  Res28                :  1; //  28     APEX_642CL_SEQ_PROC_INC_BIT_RES_28
      REG32  Res29                :  1; //  29     APEX_642CL_SEQ_PROC_INC_BIT_RES_29
      REG32  Res30                :  1; //  30     APEX_642CL_SEQ_PROC_INC_BIT_RES_30
      REG32  Res31                :  1; //  31     APEX_642CL_SEQ_PROC_INC_BIT_RES_31
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_PROC_INC_TAG                                       APEX_642CL_SEQ_PROC_INC_DCR;

extern volatile APEX_642CL_SEQ_PROC_INC_DCR                                      gApex642clSeqProcInc;
extern volatile APEX_642CL_SEQ_PROC_INC_DCR*                                     gpApex642clSeqProcInc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_0                                        0
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_1                                        1
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_2                                        2
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_3                                        3
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_4                                        4
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_5                                        5
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_6                                        6
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_7                                        7
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_8                                        8
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_9                                        9
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_10                                       10
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_11                                       11
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_12                                       12
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_13                                       13
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_14                                       14
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_15                                       15
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_16                                       16
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_17                                       17
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_18                                       18
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_19                                       19
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_20                                       20
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_21                                       21
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_22                                       22
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_23                                       23
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_24                                       24
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_25                                       25
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_26                                       26
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_27                                       27
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_28                                       28
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_29                                       29
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_30                                       30
#define APEX_642CL_SEQ_PROC_INC_BIT_RES_31                                       31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_PROC_INC_BIT_RES_ENUM                                       
{
   APEX_642CL_SEQ_PROC_INC_BIT_RES_ID_ACTIVE                                     =  1,          // Increment Resource
   APEX_642CL_SEQ_PROC_INC_BIT_RES_ID_INACTIVE                                   =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    DMA_INC
//    APEX_642CL_SEQ_DMA_INC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_DMA_INC_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000280
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         8
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_DMA_INC_0                                                 (APEX_642CL_SEQ_DMA_INC_0_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_0_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_0_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_0_OFFSET                                          (0x00000280)
#define APEX_642CL_SEQ_DMA_INC_0_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_INC_1                                                 (APEX_642CL_SEQ_DMA_INC_1_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_1_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_1_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_1_OFFSET                                          (0x00000284)
#define APEX_642CL_SEQ_DMA_INC_1_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_INC_2                                                 (APEX_642CL_SEQ_DMA_INC_2_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_2_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_2_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_2_OFFSET                                          (0x00000288)
#define APEX_642CL_SEQ_DMA_INC_2_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_INC_3                                                 (APEX_642CL_SEQ_DMA_INC_3_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_3_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_3_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_3_OFFSET                                          (0x0000028c)
#define APEX_642CL_SEQ_DMA_INC_3_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_INC_4                                                 (APEX_642CL_SEQ_DMA_INC_4_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_4_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_4_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_4_OFFSET                                          (0x00000290)
#define APEX_642CL_SEQ_DMA_INC_4_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_INC_5                                                 (APEX_642CL_SEQ_DMA_INC_5_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_5_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_5_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_5_OFFSET                                          (0x00000294)
#define APEX_642CL_SEQ_DMA_INC_5_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_INC_6                                                 (APEX_642CL_SEQ_DMA_INC_6_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_6_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_6_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_6_OFFSET                                          (0x00000298)
#define APEX_642CL_SEQ_DMA_INC_6_DEFAULT                                         (0x00000000)

#define APEX_642CL_SEQ_DMA_INC_7                                                 (APEX_642CL_SEQ_DMA_INC_7_BASE_ADDRESS + APEX_642CL_SEQ_DMA_INC_7_OFFSET)
#define APEX_642CL_SEQ_DMA_INC_7_BASE_ADDRESS                                    (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_DMA_INC_7_OFFSET                                          (0x0000029c)
#define APEX_642CL_SEQ_DMA_INC_7_DEFAULT                                         (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_INC_DCR                                      gApex642clSeqDmaInc;
extern volatile APEX_642CL_SEQ_PROC_INC_DCR*                                     gpApex642clSeqDmaInc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_0                                         0
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_1                                         1
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_2                                         2
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_3                                         3
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_4                                         4
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_5                                         5
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_6                                         6
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_7                                         7
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_8                                         8
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_9                                         9
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_10                                        10
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_11                                        11
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_12                                        12
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_13                                        13
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_14                                        14
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_15                                        15
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_16                                        16
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_17                                        17
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_18                                        18
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_19                                        19
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_20                                        20
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_21                                        21
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_22                                        22
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_23                                        23
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_24                                        24
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_25                                        25
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_26                                        26
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_27                                        27
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_28                                        28
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_29                                        29
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_30                                        30
#define APEX_642CL_SEQ_DMA_INC_BIT_RES_31                                        31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_DMA_INC_BIT_RES_ENUM                                        
{
   APEX_642CL_SEQ_DMA_INC_BIT_RES_ID_ACTIVE                                      =  1,          // Increment Resource
   APEX_642CL_SEQ_DMA_INC_BIT_RES_ID_INACTIVE                                    =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STREAM_IN_INC
//    APEX_642CL_SEQ_STREAM_IN_INC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_STREAM_IN_INC_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_INC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000002c0
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_IN_INC_0                                           (APEX_642CL_SEQ_STREAM_IN_INC_0_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_INC_0_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_INC_0_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_INC_0_OFFSET                                    (0x000002c0)
#define APEX_642CL_SEQ_STREAM_IN_INC_0_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_STREAM_IN_INC_1                                           (APEX_642CL_SEQ_STREAM_IN_INC_1_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_INC_1_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_INC_1_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_INC_1_OFFSET                                    (0x000002c4)
#define APEX_642CL_SEQ_STREAM_IN_INC_1_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_STREAM_IN_INC_2                                           (APEX_642CL_SEQ_STREAM_IN_INC_2_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_INC_2_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_INC_2_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_INC_2_OFFSET                                    (0x000002c8)
#define APEX_642CL_SEQ_STREAM_IN_INC_2_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_STREAM_IN_INC_3                                           (APEX_642CL_SEQ_STREAM_IN_INC_3_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_IN_INC_3_OFFSET)
#define APEX_642CL_SEQ_STREAM_IN_INC_3_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_IN_INC_3_OFFSET                                    (0x000002cc)
#define APEX_642CL_SEQ_STREAM_IN_INC_3_DEFAULT                                   (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_INC_DCR                                      gApex642clSeqStreamInInc;
extern volatile APEX_642CL_SEQ_PROC_INC_DCR*                                     gpApex642clSeqStreamInInc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_0                                   0
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_1                                   1
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_2                                   2
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_3                                   3
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_4                                   4
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_5                                   5
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_6                                   6
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_7                                   7
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_8                                   8
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_9                                   9
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_10                                  10
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_11                                  11
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_12                                  12
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_13                                  13
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_14                                  14
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_15                                  15
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_16                                  16
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_17                                  17
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_18                                  18
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_19                                  19
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_20                                  20
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_21                                  21
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_22                                  22
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_23                                  23
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_24                                  24
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_25                                  25
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_26                                  26
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_27                                  27
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_28                                  28
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_29                                  29
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_30                                  30
#define APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_31                                  31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_ENUM                                  
{
   APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_ID_ACTIVE                                =  1,          // Increment Resource
   APEX_642CL_SEQ_STREAM_IN_INC_BIT_RES_ID_INACTIVE                              =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    STREAM_OUT_INC
//    APEX_642CL_SEQ_STREAM_OUT_INC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_STREAM_OUT_INC_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_OUT_INC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000002d0
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_OUT_INC_0                                          (APEX_642CL_SEQ_STREAM_OUT_INC_0_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_OUT_INC_0_OFFSET)
#define APEX_642CL_SEQ_STREAM_OUT_INC_0_BASE_ADDRESS                             (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_OUT_INC_0_OFFSET                                   (0x000002d0)
#define APEX_642CL_SEQ_STREAM_OUT_INC_0_DEFAULT                                  (0x00000000)

#define APEX_642CL_SEQ_STREAM_OUT_INC_1                                          (APEX_642CL_SEQ_STREAM_OUT_INC_1_BASE_ADDRESS + APEX_642CL_SEQ_STREAM_OUT_INC_1_OFFSET)
#define APEX_642CL_SEQ_STREAM_OUT_INC_1_BASE_ADDRESS                             (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_STREAM_OUT_INC_1_OFFSET                                   (0x000002d4)
#define APEX_642CL_SEQ_STREAM_OUT_INC_1_DEFAULT                                  (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_INC_DCR                                      gApex642clSeqStreamOutInc;
extern volatile APEX_642CL_SEQ_PROC_INC_DCR*                                     gpApex642clSeqStreamOutInc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_0                                  0
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_1                                  1
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_2                                  2
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_3                                  3
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_4                                  4
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_5                                  5
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_6                                  6
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_7                                  7
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_8                                  8
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_9                                  9
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_10                                 10
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_11                                 11
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_12                                 12
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_13                                 13
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_14                                 14
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_15                                 15
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_16                                 16
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_17                                 17
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_18                                 18
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_19                                 19
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_20                                 20
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_21                                 21
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_22                                 22
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_23                                 23
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_24                                 24
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_25                                 25
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_26                                 26
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_27                                 27
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_28                                 28
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_29                                 29
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_30                                 30
#define APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_31                                 31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_ENUM                                 
{
   APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_ID_ACTIVE                               =  1,          // Increment Resource
   APEX_642CL_SEQ_STREAM_OUT_INC_BIT_RES_ID_INACTIVE                             =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MOTION_COMP_INC
//    APEX_642CL_SEQ_MOTION_COMP_INC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_MOTION_COMP_INC_BASE_ADDRESS + APEX_642CL_SEQ_MOTION_COMP_INC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000002e0
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         2
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_MOTION_COMP_INC_0                                         (APEX_642CL_SEQ_MOTION_COMP_INC_0_BASE_ADDRESS + APEX_642CL_SEQ_MOTION_COMP_INC_0_OFFSET)
#define APEX_642CL_SEQ_MOTION_COMP_INC_0_BASE_ADDRESS                            (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_MOTION_COMP_INC_0_OFFSET                                  (0x000002e0)
#define APEX_642CL_SEQ_MOTION_COMP_INC_0_DEFAULT                                 (0x00000000)

#define APEX_642CL_SEQ_MOTION_COMP_INC_1                                         (APEX_642CL_SEQ_MOTION_COMP_INC_1_BASE_ADDRESS + APEX_642CL_SEQ_MOTION_COMP_INC_1_OFFSET)
#define APEX_642CL_SEQ_MOTION_COMP_INC_1_BASE_ADDRESS                            (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_MOTION_COMP_INC_1_OFFSET                                  (0x000002e4)
#define APEX_642CL_SEQ_MOTION_COMP_INC_1_DEFAULT                                 (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_INC_DCR                                      gApex642clSeqMotionCompInc;
extern volatile APEX_642CL_SEQ_PROC_INC_DCR*                                     gpApex642clSeqMotionCompInc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_0                                 0
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_1                                 1
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_2                                 2
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_3                                 3
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_4                                 4
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_5                                 5
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_6                                 6
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_7                                 7
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_8                                 8
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_9                                 9
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_10                                10
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_11                                11
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_12                                12
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_13                                13
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_14                                14
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_15                                15
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_16                                16
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_17                                17
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_18                                18
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_19                                19
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_20                                20
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_21                                21
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_22                                22
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_23                                23
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_24                                24
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_25                                25
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_26                                26
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_27                                27
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_28                                28
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_29                                29
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_30                                30
#define APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_31                                31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_ENUM                                
{
   APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_ID_ACTIVE                              =  1,          // Increment Resource
   APEX_642CL_SEQ_MOTION_COMP_INC_BIT_RES_ID_INACTIVE                            =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    HORIZ_RESIZER_INC
//    APEX_642CL_SEQ_HORIZ_RESIZER_INC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_HORIZ_RESIZER_INC_BASE_ADDRESS + APEX_642CL_SEQ_HORIZ_RESIZER_INC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h000002f0
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       4
// 
// Description:
//    Decrement/Increment Register
//    
//    Decrement Registers designate which resources will be decremented when the SCB
//    is given a "Start" trigger and the Increment registers designate which resources
//    will be incremented when the SCB returns a "Done" response
//    
// 
// Device Control Register Bits Description:
//    RES                  [00]      Associated Resource X 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_0                                       (APEX_642CL_SEQ_HORIZ_RESIZER_INC_0_BASE_ADDRESS + APEX_642CL_SEQ_HORIZ_RESIZER_INC_0_OFFSET)
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_0_BASE_ADDRESS                          (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_0_OFFSET                                (0x000002f0)
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_0_DEFAULT                               (0x00000000)

#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_1                                       (APEX_642CL_SEQ_HORIZ_RESIZER_INC_1_BASE_ADDRESS + APEX_642CL_SEQ_HORIZ_RESIZER_INC_1_OFFSET)
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_1_BASE_ADDRESS                          (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_1_OFFSET                                (0x000002f4)
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_1_DEFAULT                               (0x00000000)


extern volatile APEX_642CL_SEQ_PROC_INC_DCR                                      gApex642clSeqHorizResizerInc;
extern volatile APEX_642CL_SEQ_PROC_INC_DCR*                                     gpApex642clSeqHorizResizerInc;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_0                               0
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_1                               1
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_2                               2
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_3                               3
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_4                               4
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_5                               5
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_6                               6
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_7                               7
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_8                               8
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_9                               9
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_10                              10
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_11                              11
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_12                              12
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_13                              13
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_14                              14
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_15                              15
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_16                              16
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_17                              17
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_18                              18
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_19                              19
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_20                              20
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_21                              21
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_22                              22
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_23                              23
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_24                              24
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_25                              25
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_26                              26
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_27                              27
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_28                              28
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_29                              29
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_30                              30
#define APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_31                              31

// Device Control Register Bits Id and Description
enum APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_ENUM                              
{
   APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_ID_ACTIVE                            =  1,          // Increment Resource
   APEX_642CL_SEQ_HORIZ_RESIZER_INC_BIT_RES_ID_INACTIVE                          =  0           // INACTIVE
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RESOURCE_FIX
//    APEX_642CL_SEQ_RESOURCE_FIX
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_RESOURCE_FIX_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000300
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         20
//    Size:       4
// 
// Description:
//    Resource Register
//    
//    All resource counts are at zero at Reset which also would keep the Sequencer
//    inactive. 
//    
// 
// Device Control Register Bits Description:
//    COUNT                [05:00]   Count of available slots [0..63] in resource 
//    RESERVED             [31:06]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_FIX_0                                            (APEX_642CL_SEQ_RESOURCE_FIX_0_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_0_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_0_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_0_OFFSET                                     (0x00000300)
#define APEX_642CL_SEQ_RESOURCE_FIX_0_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_1                                            (APEX_642CL_SEQ_RESOURCE_FIX_1_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_1_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_1_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_1_OFFSET                                     (0x00000304)
#define APEX_642CL_SEQ_RESOURCE_FIX_1_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_2                                            (APEX_642CL_SEQ_RESOURCE_FIX_2_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_2_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_2_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_2_OFFSET                                     (0x00000308)
#define APEX_642CL_SEQ_RESOURCE_FIX_2_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_3                                            (APEX_642CL_SEQ_RESOURCE_FIX_3_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_3_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_3_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_3_OFFSET                                     (0x0000030c)
#define APEX_642CL_SEQ_RESOURCE_FIX_3_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_4                                            (APEX_642CL_SEQ_RESOURCE_FIX_4_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_4_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_4_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_4_OFFSET                                     (0x00000310)
#define APEX_642CL_SEQ_RESOURCE_FIX_4_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_5                                            (APEX_642CL_SEQ_RESOURCE_FIX_5_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_5_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_5_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_5_OFFSET                                     (0x00000314)
#define APEX_642CL_SEQ_RESOURCE_FIX_5_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_6                                            (APEX_642CL_SEQ_RESOURCE_FIX_6_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_6_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_6_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_6_OFFSET                                     (0x00000318)
#define APEX_642CL_SEQ_RESOURCE_FIX_6_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_7                                            (APEX_642CL_SEQ_RESOURCE_FIX_7_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_7_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_7_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_7_OFFSET                                     (0x0000031c)
#define APEX_642CL_SEQ_RESOURCE_FIX_7_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_8                                            (APEX_642CL_SEQ_RESOURCE_FIX_8_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_8_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_8_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_8_OFFSET                                     (0x00000320)
#define APEX_642CL_SEQ_RESOURCE_FIX_8_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_9                                            (APEX_642CL_SEQ_RESOURCE_FIX_9_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_9_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_9_BASE_ADDRESS                               (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_9_OFFSET                                     (0x00000324)
#define APEX_642CL_SEQ_RESOURCE_FIX_9_DEFAULT                                    (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_10                                           (APEX_642CL_SEQ_RESOURCE_FIX_10_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_10_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_10_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_10_OFFSET                                    (0x00000328)
#define APEX_642CL_SEQ_RESOURCE_FIX_10_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_11                                           (APEX_642CL_SEQ_RESOURCE_FIX_11_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_11_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_11_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_11_OFFSET                                    (0x0000032c)
#define APEX_642CL_SEQ_RESOURCE_FIX_11_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_12                                           (APEX_642CL_SEQ_RESOURCE_FIX_12_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_12_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_12_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_12_OFFSET                                    (0x00000330)
#define APEX_642CL_SEQ_RESOURCE_FIX_12_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_13                                           (APEX_642CL_SEQ_RESOURCE_FIX_13_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_13_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_13_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_13_OFFSET                                    (0x00000334)
#define APEX_642CL_SEQ_RESOURCE_FIX_13_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_14                                           (APEX_642CL_SEQ_RESOURCE_FIX_14_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_14_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_14_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_14_OFFSET                                    (0x00000338)
#define APEX_642CL_SEQ_RESOURCE_FIX_14_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_15                                           (APEX_642CL_SEQ_RESOURCE_FIX_15_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_15_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_15_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_15_OFFSET                                    (0x0000033c)
#define APEX_642CL_SEQ_RESOURCE_FIX_15_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_16                                           (APEX_642CL_SEQ_RESOURCE_FIX_16_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_16_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_16_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_16_OFFSET                                    (0x00000340)
#define APEX_642CL_SEQ_RESOURCE_FIX_16_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_17                                           (APEX_642CL_SEQ_RESOURCE_FIX_17_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_17_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_17_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_17_OFFSET                                    (0x00000344)
#define APEX_642CL_SEQ_RESOURCE_FIX_17_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_18                                           (APEX_642CL_SEQ_RESOURCE_FIX_18_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_18_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_18_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_18_OFFSET                                    (0x00000348)
#define APEX_642CL_SEQ_RESOURCE_FIX_18_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_FIX_19                                           (APEX_642CL_SEQ_RESOURCE_FIX_19_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FIX_19_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FIX_19_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FIX_19_OFFSET                                    (0x0000034c)
#define APEX_642CL_SEQ_RESOURCE_FIX_19_DEFAULT                                   (0x00000000)


union _APEX_642CL_SEQ_RESOURCE_FIX_TAG
{
   struct _asApex642clSeqResourceFixBitFieldTag
   {
      REG32  Count                :  6; //  05:00  APEX_642CL_SEQ_RESOURCE_FIX_BIT_COUNT
      REG32  Reserved             : 26; //  31:06  APEX_642CL_SEQ_RESOURCE_FIX_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_RESOURCE_FIX_TAG                                   APEX_642CL_SEQ_RESOURCE_FIX_DCR;

extern volatile APEX_642CL_SEQ_RESOURCE_FIX_DCR                                  gApex642clSeqResourceFix;
extern volatile APEX_642CL_SEQ_RESOURCE_FIX_DCR*                                 gpApex642clSeqResourceFix;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_FIX_BIT_COUNT                                    0
#define APEX_642CL_SEQ_RESOURCE_FIX_BIT_RESERVED                                 6

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RESOURCE_FRAC
//    APEX_642CL_SEQ_RESOURCE_FRAC
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_RESOURCE_FRAC_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000350
//    Default:      32'h10000000
//    Type:         rw
//    Nb:         8
//    Size:       4
// 
// Description:
//    Fractional Resource Register
//    
//    Special resources that can increment in fractions and decrement integer (1) 
//    
// 
// Device Control Register Bits Description:
//    COUNT                [15:00]   4bit fixed point with 12 bit quotient 
//    INC                  [31:16]   Increment Value 4bit fixed point with 12bit quotient 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_FRAC_0                                           (APEX_642CL_SEQ_RESOURCE_FRAC_0_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_0_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_0_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_0_OFFSET                                    (0x00000350)
#define APEX_642CL_SEQ_RESOURCE_FRAC_0_DEFAULT                                   (0x10000000)

#define APEX_642CL_SEQ_RESOURCE_FRAC_1                                           (APEX_642CL_SEQ_RESOURCE_FRAC_1_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_1_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_1_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_1_OFFSET                                    (0x00000354)
#define APEX_642CL_SEQ_RESOURCE_FRAC_1_DEFAULT                                   (0x10000000)

#define APEX_642CL_SEQ_RESOURCE_FRAC_2                                           (APEX_642CL_SEQ_RESOURCE_FRAC_2_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_2_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_2_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_2_OFFSET                                    (0x00000358)
#define APEX_642CL_SEQ_RESOURCE_FRAC_2_DEFAULT                                   (0x10000000)

#define APEX_642CL_SEQ_RESOURCE_FRAC_3                                           (APEX_642CL_SEQ_RESOURCE_FRAC_3_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_3_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_3_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_3_OFFSET                                    (0x0000035c)
#define APEX_642CL_SEQ_RESOURCE_FRAC_3_DEFAULT                                   (0x10000000)

#define APEX_642CL_SEQ_RESOURCE_FRAC_4                                           (APEX_642CL_SEQ_RESOURCE_FRAC_4_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_4_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_4_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_4_OFFSET                                    (0x00000360)
#define APEX_642CL_SEQ_RESOURCE_FRAC_4_DEFAULT                                   (0x10000000)

#define APEX_642CL_SEQ_RESOURCE_FRAC_5                                           (APEX_642CL_SEQ_RESOURCE_FRAC_5_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_5_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_5_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_5_OFFSET                                    (0x00000364)
#define APEX_642CL_SEQ_RESOURCE_FRAC_5_DEFAULT                                   (0x10000000)

#define APEX_642CL_SEQ_RESOURCE_FRAC_6                                           (APEX_642CL_SEQ_RESOURCE_FRAC_6_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_6_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_6_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_6_OFFSET                                    (0x00000368)
#define APEX_642CL_SEQ_RESOURCE_FRAC_6_DEFAULT                                   (0x10000000)

#define APEX_642CL_SEQ_RESOURCE_FRAC_7                                           (APEX_642CL_SEQ_RESOURCE_FRAC_7_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_FRAC_7_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_FRAC_7_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_FRAC_7_OFFSET                                    (0x0000036c)
#define APEX_642CL_SEQ_RESOURCE_FRAC_7_DEFAULT                                   (0x10000000)


union _APEX_642CL_SEQ_RESOURCE_FRAC_TAG
{
   struct _asApex642clSeqResourceFracBitFieldTag
   {
      REG32  Count                : 16; //  15:00  APEX_642CL_SEQ_RESOURCE_FRAC_BIT_COUNT
      REG32  Inc                  : 16; //  31:16  APEX_642CL_SEQ_RESOURCE_FRAC_BIT_INC
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_RESOURCE_FRAC_TAG                                  APEX_642CL_SEQ_RESOURCE_FRAC_DCR;

extern volatile APEX_642CL_SEQ_RESOURCE_FRAC_DCR                                 gApex642clSeqResourceFrac;
extern volatile APEX_642CL_SEQ_RESOURCE_FRAC_DCR*                                gpApex642clSeqResourceFrac;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_FRAC_BIT_COUNT                                   0
#define APEX_642CL_SEQ_RESOURCE_FRAC_BIT_INC                                     16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    RESOURCE_ZERO
//    APEX_642CL_SEQ_RESOURCE_ZERO
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_SEQ_RESOURCE_ZERO_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_ZERO_OFFSET
//    Base Addr:    APEX_642CL_SEQ
//    Offset:       32'h00000370
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         4
//    Size:       4
// 
// Description:
//    16-Bit Resource Register
//    
//    If decremented to zero an interrupt is issued. Can also be "Incremented to zero"
//    by forcing a roll-over on the 16 bit boundary 
//    
// 
// Device Control Register Bits Description:
//    COUNT                [15:00]   16bit count of available slots [0..65536] in resource 
//    RESERVED             [31:16]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_ZERO_0                                           (APEX_642CL_SEQ_RESOURCE_ZERO_0_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_ZERO_0_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_ZERO_0_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_ZERO_0_OFFSET                                    (0x00000370)
#define APEX_642CL_SEQ_RESOURCE_ZERO_0_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_ZERO_1                                           (APEX_642CL_SEQ_RESOURCE_ZERO_1_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_ZERO_1_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_ZERO_1_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_ZERO_1_OFFSET                                    (0x00000374)
#define APEX_642CL_SEQ_RESOURCE_ZERO_1_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_ZERO_2                                           (APEX_642CL_SEQ_RESOURCE_ZERO_2_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_ZERO_2_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_ZERO_2_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_ZERO_2_OFFSET                                    (0x00000378)
#define APEX_642CL_SEQ_RESOURCE_ZERO_2_DEFAULT                                   (0x00000000)

#define APEX_642CL_SEQ_RESOURCE_ZERO_3                                           (APEX_642CL_SEQ_RESOURCE_ZERO_3_BASE_ADDRESS + APEX_642CL_SEQ_RESOURCE_ZERO_3_OFFSET)
#define APEX_642CL_SEQ_RESOURCE_ZERO_3_BASE_ADDRESS                              (APEX_642CL_SEQ_BASE_ADDRESS)
#define APEX_642CL_SEQ_RESOURCE_ZERO_3_OFFSET                                    (0x0000037c)
#define APEX_642CL_SEQ_RESOURCE_ZERO_3_DEFAULT                                   (0x00000000)


union _APEX_642CL_SEQ_RESOURCE_ZERO_TAG
{
   struct _asApex642clSeqResourceZeroBitFieldTag
   {
      REG32  Count                : 16; //  15:00  APEX_642CL_SEQ_RESOURCE_ZERO_BIT_COUNT
      REG32  Reserved             : 16; //  31:16  APEX_642CL_SEQ_RESOURCE_ZERO_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_SEQ_RESOURCE_ZERO_TAG                                  APEX_642CL_SEQ_RESOURCE_ZERO_DCR;

extern volatile APEX_642CL_SEQ_RESOURCE_ZERO_DCR                                 gApex642clSeqResourceZero;
extern volatile APEX_642CL_SEQ_RESOURCE_ZERO_DCR*                                gpApex642clSeqResourceZero;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_SEQ_RESOURCE_ZERO_BIT_COUNT                                   0
#define APEX_642CL_SEQ_RESOURCE_ZERO_BIT_RESERVED                                16

#endif /* APEX642CLSEQMAPTYPE_H */
