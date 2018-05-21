/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2015-2016 Freescale Semiconductor
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
#ifndef APEX642CLDMEMFIFOMAPTYPE_H
#define APEX642CLDMEMFIFOMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG_IN
//    APEX_642CL_DMEM_FIFO_CONFIG_IN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_CONFIG_IN_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_CONFIG_IN_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000000
//    Default:      32'h000000a0
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO Configuration
//    
//    Although this register is comprised of a number of bit fields, the hardware that
//    uses these bit fields is in a different clock domain. To properly cross the clock
//    domain, the action of writing to this register is used to indicate that all the
//    bit fields have been update. This means that all the fields must be configured in
//    a single 32 bit write rather than in a piece meal fashion utilizing a
//    read-modify-write process.
// 
// Device Control Register Bits Description:
//    NUM_BUFFER           [03:00]   This field indicates how many buffers are in use. This field can only be updated
//                                   if the fifo is empty.
//                                   
//                                   0: 1 buffer
//                                   1: 2 buffers
//                                   etc
//                                    
//    WORD_SIZE            [06:04]   This field programs the master AXI word size that will be used. All buffer start
//                                   addresses and buffer sizes have to be aligned to the word size boundary.
//                                   
//                                   0,1: reserved
//                                   2: 32 bits
//                                   3: 64 bits
//                                   4: 128 bits
//                                   5,6,7: reserved
//                                    
//    ENA_ADDR_UPDATE      [07]      This field enables/disables the buffer start address from being over written.
//                                   Since the action of writing to the Buffer Start address also generates a pop/push
//                                   action, the application may have fixed addresses and only want to configure the
//                                   address once (thereafter just generating pops/pushes). This field allows for this
//                                   option.
//                                   
//                                   0: Buffer Start addresses are not updated
//                                   1: Buffer Start addresses are updated 
//    BUFFER_SIZE          [23:08]   This indicates the size of the buffers in terms of bytes.
//                                   
//                                   0,1,2: Reserved (minimum qty is 4 bytes)
//                                   3: 4 bytes
//                                   4: 5 bytes
//                                   etc
//                                    
//    RESERVED             [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_CONFIG_IN                                         (APEX_642CL_DMEM_FIFO_0_CONFIG_IN_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_CONFIG_IN_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_CONFIG_IN_BASE_ADDRESS                            (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_CONFIG_IN_OFFSET                                  (0x00000000)
#define APEX_642CL_DMEM_FIFO_0_CONFIG_IN_DEFAULT                                 (0x000000a0)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG_OUT
//    APEX_642CL_DMEM_FIFO_CONFIG_OUT
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_CONFIG_OUT_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_CONFIG_OUT_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000004
//    Default:      32'h000000a0
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    NUM_BUFFER           [03:00]   This field indicates how many buffers are in use. This field can only be updated
//                                   if the fifo is empty.
//                                   
//                                   0: 1 buffer
//                                   1: 2 buffers
//                                   etc
//                                    
//    WORD_SIZE            [06:04]   This field programs the master AXI word size that will be used. All buffer start
//                                   addresses and buffer sizes have to be aligned to the word size boundary.
//                                   
//                                   0,1: reserved
//                                   2: 32 bits
//                                   3: 64 bits
//                                   4: 128 bits
//                                   5,6,7: reserved
//                                    
//    ENA_ADDR_UPDATE      [07]      This field enables/disables the buffer start address from being over written.
//                                   Since the action of writing to the Buffer Start address also generates a pop/push
//                                   action, the application may have fixed addresses and only want to configure the
//                                   address once (thereafter just generating pops/pushes). This field allows for this
//                                   option.
//                                   
//                                   0: Buffer Start addresses are not updated
//                                   1: Buffer Start addresses are updated 
//    BUFFER_SIZE          [23:08]   This indicates the size of the buffers in terms of bytes.
//                                   
//                                   0,1,2: Reserved (minimum qty is 4 bytes)
//                                   3: 4 bytes
//                                   4: 5 bytes
//                                   etc
//                                    
//    RESERVED             [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_CONFIG_OUT                                        (APEX_642CL_DMEM_FIFO_0_CONFIG_OUT_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_CONFIG_OUT_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_CONFIG_OUT_BASE_ADDRESS                           (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_CONFIG_OUT_OFFSET                                 (0x00000004)
#define APEX_642CL_DMEM_FIFO_0_CONFIG_OUT_DEFAULT                                (0x000000a0)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IN_BUFFER
//    APEX_642CL_DMEM_FIFO_IN_BUFFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_IN_BUFFER_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_IN_BUFFER_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         16
//    Size:       4
// 
// Description:
//    Buffer Start Address.
//    
//    There are a maximum of 16 buffers available. The action of writing to an address
//    register also indicates a "pop" or "push" of that buffer. This means that all the
//    fields must be configured in a single 32 bit write rather than in a piece meal
//    fashion utilizing a read-modify-write process. The DMEM fifo keeps track of the
//    number of outstanding "pops' and "pushes" and assumes that the number of buffers
//    is available. The hardware will sequentially cycle through buffer1 to bufferN
//    (where N reflects the num_buffer programmed field) and then loop back to buffer1
//    again. Software can change the order of the buffers by programming different start
//    address as time progresses or the address can remain unchanged and a fixed
//    circular fifo of buffers will result.
// 
// Device Control Register Bits Description:
//    START_ADDRESS        [31:00]   This is the start address of the buffer. It must be aligned to the programmed word
//                                   size. The addressing is from the perspective of the HOST AXI. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_0                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_0_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_0_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_0_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_0_OFFSET                                (0x00000008)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_0_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_1                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_1_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_1_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_1_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_1_OFFSET                                (0x0000000c)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_1_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_2                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_2_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_2_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_2_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_2_OFFSET                                (0x00000010)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_2_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_3                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_3_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_3_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_3_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_3_OFFSET                                (0x00000014)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_3_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_4                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_4_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_4_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_4_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_4_OFFSET                                (0x00000018)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_4_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_5                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_5_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_5_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_5_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_5_OFFSET                                (0x0000001c)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_5_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_6                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_6_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_6_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_6_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_6_OFFSET                                (0x00000020)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_6_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_7                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_7_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_7_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_7_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_7_OFFSET                                (0x00000024)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_7_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_8                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_8_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_8_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_8_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_8_OFFSET                                (0x00000028)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_8_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_9                                       (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_9_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_9_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_9_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_9_OFFSET                                (0x0000002c)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_9_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_10                                      (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_10_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_10_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_10_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_10_OFFSET                               (0x00000030)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_10_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_11                                      (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_11_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_11_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_11_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_11_OFFSET                               (0x00000034)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_11_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_12                                      (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_12_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_12_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_12_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_12_OFFSET                               (0x00000038)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_12_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_13                                      (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_13_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_13_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_13_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_13_OFFSET                               (0x0000003c)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_13_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_14                                      (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_14_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_14_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_14_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_14_OFFSET                               (0x00000040)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_14_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_15                                      (APEX_642CL_DMEM_FIFO_0_IN_BUFFER_15_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_BUFFER_15_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_15_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_15_OFFSET                               (0x00000044)
#define APEX_642CL_DMEM_FIFO_0_IN_BUFFER_15_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    OUT_BUFFER
//    APEX_642CL_DMEM_FIFO_OUT_BUFFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_OUT_BUFFER_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_OUT_BUFFER_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000048
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         16
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    START_ADDRESS        [31:00]   This is the start address of the buffer. It must be aligned to the programmed word
//                                   size. The addressing is from the perspective of the HOST AXI. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_0                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_0_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_0_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_0_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_0_OFFSET                               (0x00000048)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_0_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_1                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_1_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_1_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_1_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_1_OFFSET                               (0x0000004c)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_1_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_2                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_2_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_2_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_2_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_2_OFFSET                               (0x00000050)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_2_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_3                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_3_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_3_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_3_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_3_OFFSET                               (0x00000054)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_3_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_4                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_4_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_4_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_4_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_4_OFFSET                               (0x00000058)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_4_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_5                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_5_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_5_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_5_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_5_OFFSET                               (0x0000005c)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_5_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_6                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_6_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_6_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_6_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_6_OFFSET                               (0x00000060)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_6_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_7                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_7_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_7_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_7_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_7_OFFSET                               (0x00000064)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_7_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_8                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_8_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_8_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_8_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_8_OFFSET                               (0x00000068)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_8_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_9                                      (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_9_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_9_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_9_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_9_OFFSET                               (0x0000006c)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_9_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_10                                     (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_10_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_10_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_10_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_10_OFFSET                              (0x00000070)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_10_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_11                                     (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_11_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_11_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_11_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_11_OFFSET                              (0x00000074)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_11_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_12                                     (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_12_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_12_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_12_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_12_OFFSET                              (0x00000078)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_12_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_13                                     (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_13_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_13_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_13_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_13_OFFSET                              (0x0000007c)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_13_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_14                                     (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_14_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_14_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_14_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_14_OFFSET                              (0x00000080)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_14_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_15                                     (APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_15_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_15_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_15_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_15_OFFSET                              (0x00000084)
#define APEX_642CL_DMEM_FIFO_0_OUT_BUFFER_15_DEFAULT                             (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IN_STATUS
//    APEX_642CL_DMEM_FIFO_IN_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_IN_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_IN_STATUS_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000088
//    Default:      32'h0000ffff
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO Status
// 
// Device Control Register Bits Description:
//    BUFFER_FULL          [15:00]   These flags are aserted when the DMEM fifo has filled the buffer. They are cleared
//                                   when the start address for that buffer has been written. Power-up default has the
//                                   buffers indicated as full bacause the IN buffer start addresses have not been
//                                   programmed and are invalid. 
//    CURRENT_BUFFER       [19:16]   Indicates what the current buffer is. Alternatively software can manage this
//                                   themselves. Keep in mind it doesn't necessarily start at "0" unless you have
//                                   cleared the fifo.
//                                    
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_IN_STATUS                                         (APEX_642CL_DMEM_FIFO_0_IN_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_IN_STATUS_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_IN_STATUS_BASE_ADDRESS                            (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_IN_STATUS_OFFSET                                  (0x00000088)
#define APEX_642CL_DMEM_FIFO_0_IN_STATUS_DEFAULT                                 (0x0000ffff)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    OUT_STATUS
//    APEX_642CL_DMEM_FIFO_OUT_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_OUT_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_OUT_STATUS_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h0000008c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO Status
// 
// Device Control Register Bits Description:
//    BUFFER_FULL          [15:00]   These flags are aserted when the DMEM fifo has filled the buffer. They are cleared
//                                   when the start address for that buffer has been written. Power-up default has the
//                                   buffers indicated as full bacause the IN buffer start addresses have not been
//                                   programmed and are invalid. 
//    CURRENT_BUFFER       [19:16]   Indicates what the current buffer is. Alternatively software can manage this
//                                   themselves. Keep in mind it doesn't necessarily start at "0" unless you have
//                                   cleared the fifo.
//                                    
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_OUT_STATUS                                        (APEX_642CL_DMEM_FIFO_0_OUT_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_OUT_STATUS_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_OUT_STATUS_BASE_ADDRESS                           (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_OUT_STATUS_OFFSET                                 (0x0000008c)
#define APEX_642CL_DMEM_FIFO_0_OUT_STATUS_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MAINTENANCE
//    APEX_642CL_DMEM_FIFO_MAINTENANCE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_MAINTENANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_MAINTENANCE_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000090
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO clear.
// 
// Device Control Register Bits Description:
//    CLR_IN_FIFO          [00]      This clears the OUT_buffer_full and the current buffer as well as flushing the
//                                   DMEM fifo. It is self clearing after the acton has occurred. Writing a "0" has no
//                                   impact. 
//    CLR_OUT_FIFO         [01]      This clears the IN_buffer_full flags and the current buffers as well as flushing
//                                   the DMA fifo. It is self clearing after the acton has occurred. Writing a "0" has
//                                   no impact. 
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_MAINTENANCE                                       (APEX_642CL_DMEM_FIFO_0_MAINTENANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_MAINTENANCE_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_MAINTENANCE_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_MAINTENANCE_OFFSET                                (0x00000090)
#define APEX_642CL_DMEM_FIFO_0_MAINTENANCE_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PERFORMANCE
//    APEX_642CL_DMEM_FIFO_PERFORMANCE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_PERFORMANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_PERFORMANCE_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000094
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Performance Counter
// 
// Device Control Register Bits Description:
//    PERF_CNT             [31:00]   This register provides access to a free running counter that can be used to measure performance. It increments for every APU clock. A write of any value will reset the counter to "0". When the register is read, the field reflects the current value of the free running counter. The counter wraps over at terminal count. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_0_PERFORMANCE                                       (APEX_642CL_DMEM_FIFO_0_PERFORMANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_0_PERFORMANCE_OFFSET)
#define APEX_642CL_DMEM_FIFO_0_PERFORMANCE_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_0_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_0_PERFORMANCE_OFFSET                                (0x00000094)
#define APEX_642CL_DMEM_FIFO_0_PERFORMANCE_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG_IN
//    APEX_642CL_DMEM_FIFO_CONFIG_IN
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_CONFIG_IN_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_CONFIG_IN_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000000
//    Default:      32'h000000a0
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO Configuration
//    
//    Although this register is comprised of a number of bit fields, the hardware that
//    uses these bit fields is in a different clock domain. To properly cross the clock
//    domain, the action of writing to this register is used to indicate that all the
//    bit fields have been update. This means that all the fields must be configured in
//    a single 32 bit write rather than in a piece meal fashion utilizing a
//    read-modify-write process.
// 
// Device Control Register Bits Description:
//    NUM_BUFFER           [03:00]   This field indicates how many buffers are in use. This field can only be updated
//                                   if the fifo is empty.
//                                   
//                                   0: 1 buffer
//                                   1: 2 buffers
//                                   etc
//                                    
//    WORD_SIZE            [06:04]   This field programs the master AXI word size that will be used. All buffer start
//                                   addresses and buffer sizes have to be aligned to the word size boundary.
//                                   
//                                   0,1: reserved
//                                   2: 32 bits
//                                   3: 64 bits
//                                   4: 128 bits
//                                   5,6,7: reserved
//                                    
//    ENA_ADDR_UPDATE      [07]      This field enables/disables the buffer start address from being over written.
//                                   Since the action of writing to the Buffer Start address also generates a pop/push
//                                   action, the application may have fixed addresses and only want to configure the
//                                   address once (thereafter just generating pops/pushes). This field allows for this
//                                   option.
//                                   
//                                   0: Buffer Start addresses are not updated
//                                   1: Buffer Start addresses are updated 
//    BUFFER_SIZE          [23:08]   This indicates the size of the buffers in terms of bytes.
//                                   
//                                   0,1,2: Reserved (minimum qty is 4 bytes)
//                                   3: 4 bytes
//                                   4: 5 bytes
//                                   etc
//                                    
//    RESERVED             [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_CONFIG_IN                                         (APEX_642CL_DMEM_FIFO_1_CONFIG_IN_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_CONFIG_IN_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_CONFIG_IN_BASE_ADDRESS                            (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_CONFIG_IN_OFFSET                                  (0x00000000)
#define APEX_642CL_DMEM_FIFO_1_CONFIG_IN_DEFAULT                                 (0x000000a0)


union _APEX_642CL_DMEM_FIFO_CONFIG_IN_TAG
{
   struct _asApex642clDmemFifoConfigInBitFieldTag
   {
      REG32  NumBuffer            :  4; //  03:00  APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_NUM_BUFFER
      REG32  WordSize             :  3; //  06:04  APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_WORD_SIZE
      REG32  EnaAddrUpdate        :  1; //  07     APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_ENA_ADDR_UPDATE
      REG32  BufferSize           : 16; //  23:08  APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_BUFFER_SIZE
      REG32  Reserved             :  8; //  31:24  APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_DMEM_FIFO_CONFIG_IN_TAG                                APEX_642CL_DMEM_FIFO_CONFIG_IN_DCR;

extern volatile APEX_642CL_DMEM_FIFO_CONFIG_IN_DCR                               gApex642clDmemFifoConfigIn;
extern volatile APEX_642CL_DMEM_FIFO_CONFIG_IN_DCR*                              gpApex642clDmemFifoConfigIn;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_NUM_BUFFER                            0
#define APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_WORD_SIZE                             4

// Device Control Register Bits Id and Description
enum APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_WORD_SIZE_ENUM                          
{
   APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_WORD_SIZE_ID_32_BIT                        =  2,          // Bus word size 32_BIT
   APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_WORD_SIZE_ID_64_BIT                        =  3,          // Bus word size 64_BIT
   APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_WORD_SIZE_ID_128_BIT                       =  4           // Bus word size 128_BIT
};
                                                                             

#define APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_ENA_ADDR_UPDATE                       7

// Device Control Register Bits Id and Description
enum APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_ENA_ADDR_UPDATE_ENUM                    
{
   APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_ENA_ADDR_UPDATE_ID_ENABLE                  =  1,          // Buffer Start addresses are ENABLE
   APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_ENA_ADDR_UPDATE_ID_DISABLE                 =  0           // Buffer Start addresses are DISABLE
};
                                                                             

#define APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_BUFFER_SIZE                           8
#define APEX_642CL_DMEM_FIFO_CONFIG_IN_BIT_RESERVED                              24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    CONFIG_OUT
//    APEX_642CL_DMEM_FIFO_CONFIG_OUT
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_CONFIG_OUT_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_CONFIG_OUT_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000004
//    Default:      32'h000000a0
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    NUM_BUFFER           [03:00]   This field indicates how many buffers are in use. This field can only be updated
//                                   if the fifo is empty.
//                                   
//                                   0: 1 buffer
//                                   1: 2 buffers
//                                   etc
//                                    
//    WORD_SIZE            [06:04]   This field programs the master AXI word size that will be used. All buffer start
//                                   addresses and buffer sizes have to be aligned to the word size boundary.
//                                   
//                                   0,1: reserved
//                                   2: 32 bits
//                                   3: 64 bits
//                                   4: 128 bits
//                                   5,6,7: reserved
//                                    
//    ENA_ADDR_UPDATE      [07]      This field enables/disables the buffer start address from being over written.
//                                   Since the action of writing to the Buffer Start address also generates a pop/push
//                                   action, the application may have fixed addresses and only want to configure the
//                                   address once (thereafter just generating pops/pushes). This field allows for this
//                                   option.
//                                   
//                                   0: Buffer Start addresses are not updated
//                                   1: Buffer Start addresses are updated 
//    BUFFER_SIZE          [23:08]   This indicates the size of the buffers in terms of bytes.
//                                   
//                                   0,1,2: Reserved (minimum qty is 4 bytes)
//                                   3: 4 bytes
//                                   4: 5 bytes
//                                   etc
//                                    
//    RESERVED             [31:24]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_CONFIG_OUT                                        (APEX_642CL_DMEM_FIFO_1_CONFIG_OUT_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_CONFIG_OUT_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_CONFIG_OUT_BASE_ADDRESS                           (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_CONFIG_OUT_OFFSET                                 (0x00000004)
#define APEX_642CL_DMEM_FIFO_1_CONFIG_OUT_DEFAULT                                (0x000000a0)


extern volatile APEX_642CL_DMEM_FIFO_CONFIG_IN_DCR                               gApex642clDmemFifoConfigOut;
extern volatile APEX_642CL_DMEM_FIFO_CONFIG_IN_DCR*                              gpApex642clDmemFifoConfigOut;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_NUM_BUFFER                           0
#define APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_WORD_SIZE                            4

// Device Control Register Bits Id and Description
enum APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_WORD_SIZE_ENUM                         
{
   APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_WORD_SIZE_ID_32_BIT                       =  2,          // Bus word size 32_BIT
   APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_WORD_SIZE_ID_64_BIT                       =  3,          // Bus word size 64_BIT
   APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_WORD_SIZE_ID_128_BIT                      =  4           // Bus word size 128_BIT
};
                                                                             

#define APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_ENA_ADDR_UPDATE                      7

// Device Control Register Bits Id and Description
enum APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_ENA_ADDR_UPDATE_ENUM                   
{
   APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_ENA_ADDR_UPDATE_ID_ENABLE                 =  1,          // Buffer Start addresses are ENABLE
   APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_ENA_ADDR_UPDATE_ID_DISABLE                =  0           // Buffer Start addresses are DISABLE
};
                                                                             

#define APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_BUFFER_SIZE                          8
#define APEX_642CL_DMEM_FIFO_CONFIG_OUT_BIT_RESERVED                             24

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IN_BUFFER
//    APEX_642CL_DMEM_FIFO_IN_BUFFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_IN_BUFFER_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_IN_BUFFER_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         16
//    Size:       4
// 
// Description:
//    Buffer Start Address.
//    
//    There are a maximum of 16 buffers available. The action of writing to an address
//    register also indicates a "pop" or "push" of that buffer. This means that all the
//    fields must be configured in a single 32 bit write rather than in a piece meal
//    fashion utilizing a read-modify-write process. The DMEM fifo keeps track of the
//    number of outstanding "pops' and "pushes" and assumes that the number of buffers
//    is available. The hardware will sequentially cycle through buffer1 to bufferN
//    (where N reflects the num_buffer programmed field) and then loop back to buffer1
//    again. Software can change the order of the buffers by programming different start
//    address as time progresses or the address can remain unchanged and a fixed
//    circular fifo of buffers will result.
// 
// Device Control Register Bits Description:
//    START_ADDRESS        [31:00]   This is the start address of the buffer. It must be aligned to the programmed word
//                                   size. The addressing is from the perspective of the HOST AXI. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_0                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_0_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_0_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_0_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_0_OFFSET                                (0x00000008)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_0_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_1                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_1_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_1_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_1_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_1_OFFSET                                (0x0000000c)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_1_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_2                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_2_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_2_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_2_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_2_OFFSET                                (0x00000010)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_2_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_3                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_3_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_3_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_3_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_3_OFFSET                                (0x00000014)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_3_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_4                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_4_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_4_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_4_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_4_OFFSET                                (0x00000018)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_4_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_5                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_5_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_5_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_5_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_5_OFFSET                                (0x0000001c)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_5_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_6                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_6_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_6_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_6_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_6_OFFSET                                (0x00000020)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_6_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_7                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_7_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_7_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_7_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_7_OFFSET                                (0x00000024)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_7_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_8                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_8_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_8_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_8_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_8_OFFSET                                (0x00000028)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_8_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_9                                       (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_9_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_9_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_9_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_9_OFFSET                                (0x0000002c)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_9_DEFAULT                               (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_10                                      (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_10_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_10_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_10_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_10_OFFSET                               (0x00000030)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_10_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_11                                      (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_11_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_11_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_11_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_11_OFFSET                               (0x00000034)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_11_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_12                                      (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_12_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_12_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_12_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_12_OFFSET                               (0x00000038)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_12_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_13                                      (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_13_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_13_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_13_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_13_OFFSET                               (0x0000003c)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_13_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_14                                      (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_14_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_14_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_14_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_14_OFFSET                               (0x00000040)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_14_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_15                                      (APEX_642CL_DMEM_FIFO_1_IN_BUFFER_15_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_BUFFER_15_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_15_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_15_OFFSET                               (0x00000044)
#define APEX_642CL_DMEM_FIFO_1_IN_BUFFER_15_DEFAULT                              (0x00000000)


union _APEX_642CL_DMEM_FIFO_IN_BUFFER_TAG
{
   struct _asApex642clDmemFifoInBufferBitFieldTag
   {
      REG32  StartAddress         : 32; //  31:00  APEX_642CL_DMEM_FIFO_IN_BUFFER_BIT_START_ADDRESS
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_DMEM_FIFO_IN_BUFFER_TAG                                APEX_642CL_DMEM_FIFO_IN_BUFFER_DCR;

extern volatile APEX_642CL_DMEM_FIFO_IN_BUFFER_DCR                               gApex642clDmemFifoInBuffer;
extern volatile APEX_642CL_DMEM_FIFO_IN_BUFFER_DCR*                              gpApex642clDmemFifoInBuffer;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_IN_BUFFER_BIT_START_ADDRESS                         0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    OUT_BUFFER
//    APEX_642CL_DMEM_FIFO_OUT_BUFFER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_OUT_BUFFER_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_OUT_BUFFER_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000048
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         16
//    Size:       4
// 
// Description:
//    See Link
// 
// Device Control Register Bits Description:
//    START_ADDRESS        [31:00]   This is the start address of the buffer. It must be aligned to the programmed word
//                                   size. The addressing is from the perspective of the HOST AXI. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_0                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_0_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_0_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_0_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_0_OFFSET                               (0x00000048)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_0_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_1                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_1_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_1_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_1_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_1_OFFSET                               (0x0000004c)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_1_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_2                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_2_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_2_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_2_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_2_OFFSET                               (0x00000050)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_2_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_3                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_3_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_3_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_3_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_3_OFFSET                               (0x00000054)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_3_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_4                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_4_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_4_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_4_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_4_OFFSET                               (0x00000058)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_4_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_5                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_5_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_5_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_5_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_5_OFFSET                               (0x0000005c)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_5_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_6                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_6_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_6_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_6_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_6_OFFSET                               (0x00000060)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_6_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_7                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_7_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_7_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_7_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_7_OFFSET                               (0x00000064)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_7_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_8                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_8_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_8_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_8_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_8_OFFSET                               (0x00000068)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_8_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_9                                      (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_9_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_9_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_9_BASE_ADDRESS                         (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_9_OFFSET                               (0x0000006c)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_9_DEFAULT                              (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_10                                     (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_10_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_10_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_10_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_10_OFFSET                              (0x00000070)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_10_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_11                                     (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_11_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_11_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_11_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_11_OFFSET                              (0x00000074)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_11_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_12                                     (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_12_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_12_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_12_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_12_OFFSET                              (0x00000078)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_12_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_13                                     (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_13_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_13_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_13_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_13_OFFSET                              (0x0000007c)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_13_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_14                                     (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_14_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_14_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_14_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_14_OFFSET                              (0x00000080)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_14_DEFAULT                             (0x00000000)

#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_15                                     (APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_15_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_15_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_15_BASE_ADDRESS                        (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_15_OFFSET                              (0x00000084)
#define APEX_642CL_DMEM_FIFO_1_OUT_BUFFER_15_DEFAULT                             (0x00000000)


extern volatile APEX_642CL_DMEM_FIFO_IN_BUFFER_DCR                               gApex642clDmemFifoOutBuffer;
extern volatile APEX_642CL_DMEM_FIFO_IN_BUFFER_DCR*                              gpApex642clDmemFifoOutBuffer;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_OUT_BUFFER_BIT_START_ADDRESS                        0

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IN_STATUS
//    APEX_642CL_DMEM_FIFO_IN_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_IN_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_IN_STATUS_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000088
//    Default:      32'h0000ffff
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO Status
// 
// Device Control Register Bits Description:
//    BUFFER_FULL          [15:00]   These flags are aserted when the DMEM fifo has filled the buffer. They are cleared
//                                   when the start address for that buffer has been written. Power-up default has the
//                                   buffers indicated as full bacause the IN buffer start addresses have not been
//                                   programmed and are invalid. 
//    CURRENT_BUFFER       [19:16]   Indicates what the current buffer is. Alternatively software can manage this
//                                   themselves. Keep in mind it doesn't necessarily start at "0" unless you have
//                                   cleared the fifo.
//                                    
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_IN_STATUS                                         (APEX_642CL_DMEM_FIFO_1_IN_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_IN_STATUS_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_IN_STATUS_BASE_ADDRESS                            (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_IN_STATUS_OFFSET                                  (0x00000088)
#define APEX_642CL_DMEM_FIFO_1_IN_STATUS_DEFAULT                                 (0x0000ffff)


union _APEX_642CL_DMEM_FIFO_IN_STATUS_TAG
{
   struct _asApex642clDmemFifoInStatusBitFieldTag
   {
      REG32  BufferFull           : 16; //  15:00  APEX_642CL_DMEM_FIFO_IN_STATUS_BIT_BUFFER_FULL
      REG32  CurrentBuffer        :  4; //  19:16  APEX_642CL_DMEM_FIFO_IN_STATUS_BIT_CURRENT_BUFFER
      REG32  Reserved             : 12; //  31:20  APEX_642CL_DMEM_FIFO_IN_STATUS_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_DMEM_FIFO_IN_STATUS_TAG                                APEX_642CL_DMEM_FIFO_IN_STATUS_DCR;

extern volatile APEX_642CL_DMEM_FIFO_IN_STATUS_DCR                               gApex642clDmemFifoInStatus;
extern volatile APEX_642CL_DMEM_FIFO_IN_STATUS_DCR*                              gpApex642clDmemFifoInStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_IN_STATUS_BIT_BUFFER_FULL                           0
#define APEX_642CL_DMEM_FIFO_IN_STATUS_BIT_CURRENT_BUFFER                        16
#define APEX_642CL_DMEM_FIFO_IN_STATUS_BIT_RESERVED                              20

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    OUT_STATUS
//    APEX_642CL_DMEM_FIFO_OUT_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_OUT_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_OUT_STATUS_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h0000008c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO Status
// 
// Device Control Register Bits Description:
//    BUFFER_FULL          [15:00]   These flags are aserted when the DMEM fifo has filled the buffer. They are cleared
//                                   when the start address for that buffer has been written. Power-up default has the
//                                   buffers indicated as full bacause the IN buffer start addresses have not been
//                                   programmed and are invalid. 
//    CURRENT_BUFFER       [19:16]   Indicates what the current buffer is. Alternatively software can manage this
//                                   themselves. Keep in mind it doesn't necessarily start at "0" unless you have
//                                   cleared the fifo.
//                                    
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_OUT_STATUS                                        (APEX_642CL_DMEM_FIFO_1_OUT_STATUS_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_OUT_STATUS_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_OUT_STATUS_BASE_ADDRESS                           (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_OUT_STATUS_OFFSET                                 (0x0000008c)
#define APEX_642CL_DMEM_FIFO_1_OUT_STATUS_DEFAULT                                (0x00000000)


union _APEX_642CL_DMEM_FIFO_OUT_STATUS_TAG
{
   struct _asApex642clDmemFifoOutStatusBitFieldTag
   {
      REG32  BufferFull           : 16; //  15:00  APEX_642CL_DMEM_FIFO_OUT_STATUS_BIT_BUFFER_FULL
      REG32  CurrentBuffer        :  4; //  19:16  APEX_642CL_DMEM_FIFO_OUT_STATUS_BIT_CURRENT_BUFFER
      REG32  Reserved             : 12; //  31:20  APEX_642CL_DMEM_FIFO_OUT_STATUS_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_DMEM_FIFO_OUT_STATUS_TAG                               APEX_642CL_DMEM_FIFO_OUT_STATUS_DCR;

extern volatile APEX_642CL_DMEM_FIFO_OUT_STATUS_DCR                              gApex642clDmemFifoOutStatus;
extern volatile APEX_642CL_DMEM_FIFO_OUT_STATUS_DCR*                             gpApex642clDmemFifoOutStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_OUT_STATUS_BIT_BUFFER_FULL                          0
#define APEX_642CL_DMEM_FIFO_OUT_STATUS_BIT_CURRENT_BUFFER                       16
#define APEX_642CL_DMEM_FIFO_OUT_STATUS_BIT_RESERVED                             20

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    MAINTENANCE
//    APEX_642CL_DMEM_FIFO_MAINTENANCE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_MAINTENANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_MAINTENANCE_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000090
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    DMEM FIFO clear.
// 
// Device Control Register Bits Description:
//    CLR_IN_FIFO          [00]      This clears the OUT_buffer_full and the current buffer as well as flushing the
//                                   DMEM fifo. It is self clearing after the acton has occurred. Writing a "0" has no
//                                   impact. 
//    CLR_OUT_FIFO         [01]      This clears the IN_buffer_full flags and the current buffers as well as flushing
//                                   the DMA fifo. It is self clearing after the acton has occurred. Writing a "0" has
//                                   no impact. 
//    RESERVED             [31:02]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_MAINTENANCE                                       (APEX_642CL_DMEM_FIFO_1_MAINTENANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_MAINTENANCE_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_MAINTENANCE_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_MAINTENANCE_OFFSET                                (0x00000090)
#define APEX_642CL_DMEM_FIFO_1_MAINTENANCE_DEFAULT                               (0x00000000)


union _APEX_642CL_DMEM_FIFO_MAINTENANCE_TAG
{
   struct _asApex642clDmemFifoMaintenanceBitFieldTag
   {
      REG32  ClrInFifo            :  1; //  00     APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_IN_FIFO
      REG32  ClrOutFifo           :  1; //  01     APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_OUT_FIFO
      REG32  Reserved             : 30; //  31:02  APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_DMEM_FIFO_MAINTENANCE_TAG                              APEX_642CL_DMEM_FIFO_MAINTENANCE_DCR;

extern volatile APEX_642CL_DMEM_FIFO_MAINTENANCE_DCR                             gApex642clDmemFifoMaintenance;
extern volatile APEX_642CL_DMEM_FIFO_MAINTENANCE_DCR*                            gpApex642clDmemFifoMaintenance;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_IN_FIFO                         0

// Device Control Register Bits Id and Description
enum APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_IN_FIFO_ENUM                      
{
   APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_IN_FIFO_ID_ACTIVE                    =  1,          // FIFO clear ACTIVE
   APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_IN_FIFO_ID_INACTIVE                  =  0           // FIFO clear INACTIVE
};
                                                                             

#define APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_OUT_FIFO                        1

// Device Control Register Bits Id and Description
enum APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_OUT_FIFO_ENUM                     
{
   APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_OUT_FIFO_ID_ACTIVE                   =  1,          // FIFO clear ACTIVE
   APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_CLR_OUT_FIFO_ID_INACTIVE                 =  0           // FIFO clear INACTIVE
};
                                                                             

#define APEX_642CL_DMEM_FIFO_MAINTENANCE_BIT_RESERVED                            2

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PERFORMANCE
//    APEX_642CL_DMEM_FIFO_PERFORMANCE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_DMEM_FIFO_PERFORMANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_PERFORMANCE_OFFSET
//    Base Addr:    APEX_642CL_DMEM_FIFO
//    Offset:       32'h00000094
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Performance Counter
// 
// Device Control Register Bits Description:
//    PERF_CNT             [31:00]   This register provides access to a free running counter that can be used to measure performance. It increments for every APU clock. A write of any value will reset the counter to "0". When the register is read, the field reflects the current value of the free running counter. The counter wraps over at terminal count. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_1_PERFORMANCE                                       (APEX_642CL_DMEM_FIFO_1_PERFORMANCE_BASE_ADDRESS + APEX_642CL_DMEM_FIFO_1_PERFORMANCE_OFFSET)
#define APEX_642CL_DMEM_FIFO_1_PERFORMANCE_BASE_ADDRESS                          (APEX_642CL_DMEM_FIFO_1_BASE_ADDRESS)
#define APEX_642CL_DMEM_FIFO_1_PERFORMANCE_OFFSET                                (0x00000094)
#define APEX_642CL_DMEM_FIFO_1_PERFORMANCE_DEFAULT                               (0x00000000)


union _APEX_642CL_DMEM_FIFO_PERFORMANCE_TAG
{
   struct _asApex642clDmemFifoPerformanceBitFieldTag
   {
      REG32  PerfCnt              : 32; //  31:00  APEX_642CL_DMEM_FIFO_PERFORMANCE_BIT_PERF_CNT
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_DMEM_FIFO_PERFORMANCE_TAG                              APEX_642CL_DMEM_FIFO_PERFORMANCE_DCR;

extern volatile APEX_642CL_DMEM_FIFO_PERFORMANCE_DCR                             gApex642clDmemFifoPerformance;
extern volatile APEX_642CL_DMEM_FIFO_PERFORMANCE_DCR*                            gpApex642clDmemFifoPerformance;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_DMEM_FIFO_PERFORMANCE_BIT_PERF_CNT                            0

#endif /* APEX642CLDMEMFIFOMAPTYPE_H */
