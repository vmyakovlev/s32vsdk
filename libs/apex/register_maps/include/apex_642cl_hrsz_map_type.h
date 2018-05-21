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
//    HRSZ
//    APEX_642CL_HRSZ
// 
// Device Block Parameters:
//    Addr:         APEX_642CL_HRSZ_BASE_ADDRESS + APEX_642CL_HRSZ_OFFSET
//    Base Addr:    APEX_642CL_BASE_ADDRESS
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         1
//    Size:       65536
// 
// Description:
//    Horizontal Resizer
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#ifndef APEX642CLHRSZMAPTYPE_H
#define APEX642CLHRSZMAPTYPE_H


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Sub-Block Name:
//    CORE
//    APEX_642CL_HRSZ_CORE
// 
// Device Sub-Block Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_OFFSET
//    Base Addr:    APEX_642CL_HRSZ
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Nb:         2
//    Size:       128
// 
// Description:
//    Core
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0                                                   (APEX_642CL_HRSZ_CORE_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_BASE_ADDRESS                                      (APEX_642CL_HRSZ_BASE_ADDRESS)
#define APEX_642CL_HRSZ_CORE_0_OFFSET                                            (0x00000000)
#define APEX_642CL_HRSZ_CORE_0_SIZE                                              (0x00000080)

#define APEX_642CL_HRSZ_CORE_1                                                   (APEX_642CL_HRSZ_CORE_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_BASE_ADDRESS                                      (APEX_642CL_HRSZ_BASE_ADDRESS)
#define APEX_642CL_HRSZ_CORE_1_OFFSET                                            (0x00000080)
#define APEX_642CL_HRSZ_CORE_1_SIZE                                              (0x00000080)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    FLUSH_INIT
//    APEX_642CL_HRSZ_CORE_FLUSH_INIT
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_FLUSH_INIT_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_FLUSH_INIT_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Flush FIFOs and Init Data processing
// 
// Device Control Register Bits Description:
//    FLUSH_0              [00]      Flush Input FIFO 
//    FLUSH_1              [01]      Flush PPF FIFO 
//    FLUSH_2              [02]      Flush Output FIFO 
//    INIT                 [03]      Initialize Datapath 
//    START                [04]      Start Datapath 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_FLUSH_INIT                                        (APEX_642CL_HRSZ_CORE_0_FLUSH_INIT_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_FLUSH_INIT_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_FLUSH_INIT_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_FLUSH_INIT_OFFSET                                 (0x00000000)
#define APEX_642CL_HRSZ_CORE_0_FLUSH_INIT_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    INSTRUCTION
//    APEX_642CL_HRSZ_CORE_INSTRUCTION
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_INSTRUCTION_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_INSTRUCTION_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Configuration
// 
// Device Control Register Bits Description:
//    CLK_GATE_ENB         [00]      System Clock Gate Enable.
//                                   When HRSZ Core resource is not utilized, application can place HRSZ core into low power mode.
//                                   0: Disabled (Low Power enabled; No sys_clk provided to core)
//                                   1: Enabled (Active Power enabled; sys_clk provided to core)
//                                    
//    LPF_EN               [01]      Low Pass Filter Enable
//                                   0 - Disabled
//                                   1 - Enabled
//                                    
//    H_AVG_MODE           [04:02]   Average: Horizontal Mode.
//                                   000 : No Average
//                                   001 : 1/2 Average
//                                   010 : 1/4 Average
//                                   011 : 1/8 Average
//                                   100 : 1/16 Average
//                                   101 : 1/32 Average
//                                    
//    PPF_EN               [05]      Poly Phase Filter Enable
//                                   0 - Disabled
//                                   1 - Enabled
//                                    
//    PPF_TAP_SELECT       [06]      The Number of Taps for the First Horizontal Poly-Phase Filtered Scaling
//                                   0 : 4 Taps
//                                   1 : 8 Taps
//                                    
//    RESERVED_0           [07]      Reserved 
//    IN_FORMAT            [11:08]   Integer Mode (Instruction[31]==0x0)
//                                        Input Format Wide Dynamic Range Select.
//                                        0000 - Unsigned 8-bit
//                                        0001 - Unsigned 10-bit
//                                        0010 - Unsigned 12-bit
//                                        0011 - Unsigned 14-bit
//                                        0100 - Unsigned 16-bit
//                                        0101 - Signed 8-bit
//                                        0110 - Signed 9-bit
//                                        0111 - Signed 10-bit
//                                        1000 - Signed 16-bit
//                                        1001 - 1111 - Reserved
//                                   Fixed Mode (Instruction[31]==0x1)
//                                        Fixed decimal point shift
//                                        0000 - Right Shift by 0
//                                        0001 - Right Shift by 1
//                                        0010 - Right Shift by 2
//                                        0011 - Right Shift by 3
//                                        0100 - Right Shift by 4
//                                        0101 - Right Shift by 5
//                                        0110 - Right Shift by 6
//                                        0111 - Right Shift by 7
//                                        1000 - Left Shift by 0
//                                        1001 - Left Shift by 1
//                                        1010 - Left Shift by 2
//                                        1011 - Left Shift by 3
//                                        1100 - Left Shift by 4
//                                        1101 - Left Shift by 5
//                                        1110 - Left Shift by 6
//                                        1111 - Left Shift by 7 
//    OUT_FORMAT           [15:12]   Integer Mode (Instruction[27]==0x0)
//                                        Output Format Wide Dynamic Range Select.
//                                        0000 - Unsigned 8-bit
//                                        0001 - Unsigned 10-bit
//                                        0010 - Unsigned 12-bit
//                                        0011 - Unsigned 14-bit
//                                        0100 - Unsigned 16-bit
//                                        0101 - Signed 8-bit
//                                        0110 - Signed 9-bit
//                                        0111 - Signed 10-bit
//                                        1000 - Signed 16-bit
//                                        1001 - 1111 - Reserved
//                                   Fixed Mode (Instruction[27]==0x1)
//                                        Fixed decimal point shift
//                                        0000 - Right Shift by 0
//                                        0001 - Right Shift by 1
//                                        0010 - Right Shift by 2
//                                        0011 - Right Shift by 3
//                                        0100 - Right Shift by 4
//                                        0101 - Right Shift by 5
//                                        0110 - Right Shift by 6
//                                        0111 - Right Shift by 7
//                                        1000 - Left Shift by 0
//                                        1001 - Left Shift by 1
//                                        1010 - Left Shift by 2
//                                        1011 - Left Shift by 3
//                                        1100 - Left Shift by 4
//                                        1101 - Left Shift by 5
//                                        1110 - Left Shift by 6
//                                        1111 - Left Shift by 7 
//    LPF_ROUND_FACTOR     [19:16]   LPF Rounding Factor
//                                   Rounding of the algorithm can be controlled to match precision (decimal point location) of the filter coefficients.
//                                   0: r_bsum[19:3]
//                                   1: r_bsum[20:4]
//                                   2: r_bsum[21:5]
//                                   3: r_bsum[22:6]
//                                   4: r_bsum[23:7]
//                                   5: r_bsum[24:8]
//                                   6: r_bsum[25:9]
//                                   7: r_bsum[26:10]
//                                   8: r_bsum[27:11]
//                                   9: r_bsum[28:12]
//                                   10: r_bsum[29:13]
//                                   11-15: reserved
//                                    
//    PPF_ROUND_FACTOR     [23:20]   PPF Rounding Factor
//                                   Rounding of the algorithm can be controlled to match precision (decimal point location) of the filter coefficients.
//                                   0: r_sum[21:5] + r_sum[4]
//                                   1: r_sum[22:6] + r_sum[5]
//                                   2: r_sum[23:7] + r_sum[6]
//                                   3: r_sum[24:8] + r_sum[7]
//                                   4: r_sum[25:9] + r_sum[8]
//                                   5: r_sum[26:10] + r_sum[9]
//                                   6: r_sum[27:11] + r_sum[10]
//                                   7: r_sum[28:12] + r_sum[11]
//                                   8-15: reserved
//                                    
//    RESERVED1            [24]      Reserved 
//    OUT_16bit_8bit       [25]      Fixed Point Output Data Format identification for conversion from internal s17 format
//                                   0 - 8bit
//                                   1 - 16bit 
//    OUT_Signed_Unsigned  [26]      Fixed Point Output Data Format identification for conversion from internal s17 format
//                                   0 - Unsigned
//                                   1 - Signed 
//    OUT_Fixed_Integer    [27]      Output Data Format identification for conversion from internal s17 format
//                                   0 - Integer
//                                   1 - Fixed Point 
//    RESERVED2            [28]      Reserved 
//    IN_16bit_8bit        [29]      Fixed Point Input Data Format identification for conversion to internal s17 format
//                                   0 - 8bit
//                                   1 - 16bit 
//    IN_Signed_Unsigned   [30]      Fixed Point Input Data Format identification for conversion to internal s17 format
//                                   0 - Unsigned
//                                   1 - Signed 
//    IN_Fixed_Integer     [31]      Input Data Format identification for conversion to internal s17 format
//                                   0 - Integer
//                                   1 - Fixed Point 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_INSTRUCTION                                       (APEX_642CL_HRSZ_CORE_0_INSTRUCTION_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_INSTRUCTION_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_INSTRUCTION_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_INSTRUCTION_OFFSET                                (0x00000004)
#define APEX_642CL_HRSZ_CORE_0_INSTRUCTION_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LPF_COEF
//    APEX_642CL_HRSZ_CORE_LPF_COEF
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_LPF_COEF_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_LPF_COEF_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         3
//    Size:       4
// 
// Description:
//    9-Tap Low Pass Filter Coefficients
// 
// Device Control Register Bits Description:
//    Y_COEF               [09:00]   Y Coefficient 
//    RESERVED             [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_0                                        (APEX_642CL_HRSZ_CORE_0_LPF_COEF_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_LPF_COEF_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_0_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_0_OFFSET                                 (0x00000008)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_0_DEFAULT                                (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_1                                        (APEX_642CL_HRSZ_CORE_0_LPF_COEF_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_LPF_COEF_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_1_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_1_OFFSET                                 (0x0000000c)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_1_DEFAULT                                (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_2                                        (APEX_642CL_HRSZ_CORE_0_LPF_COEF_2_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_LPF_COEF_2_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_2_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_2_OFFSET                                 (0x00000010)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_2_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LPF_COEF_3
//    APEX_642CL_HRSZ_CORE_LPF_COEF_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_LPF_COEF_3_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_LPF_COEF_3_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    9-Tap Low Pass Filter Coefficients
// 
// Device Control Register Bits Description:
//    Y_COEF               [09:00]   Y Coefficient 
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_3                                        (APEX_642CL_HRSZ_CORE_0_LPF_COEF_3_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_LPF_COEF_3_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_3_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_3_OFFSET                                 (0x00000014)
#define APEX_642CL_HRSZ_CORE_0_LPF_COEF_3_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_FILTER
//    APEX_642CL_HRSZ_CORE_PPF_FILTER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_FILTER_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_FILTER_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         10
//    Size:       4
// 
// Description:
//    Poly Phase Filter for Horizontal Scaling
// 
// Device Control Register Bits Description:
//    FILTER               [09:00]   10-bit Filter Coefficient 
//    RESERVED             [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_0                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_0_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_0_OFFSET                               (0x00000018)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_0_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_1                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_1_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_1_OFFSET                               (0x0000001c)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_1_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_2                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_2_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_2_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_2_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_2_OFFSET                               (0x00000020)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_2_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_3                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_3_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_3_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_3_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_3_OFFSET                               (0x00000024)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_3_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_4                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_4_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_4_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_4_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_4_OFFSET                               (0x00000028)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_4_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_5                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_5_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_5_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_5_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_5_OFFSET                               (0x0000002c)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_5_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_6                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_6_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_6_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_6_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_6_OFFSET                               (0x00000030)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_6_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_7                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_7_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_7_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_7_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_7_OFFSET                               (0x00000034)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_7_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_8                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_8_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_8_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_8_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_8_OFFSET                               (0x00000038)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_8_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_9                                      (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_9_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_9_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_9_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_9_OFFSET                               (0x0000003c)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_9_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_FILTER_10
//    APEX_642CL_HRSZ_CORE_PPF_FILTER_10
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_FILTER_10_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000040
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Poly Phase Filter for Horizontal Scaling
// 
// Device Control Register Bits Description:
//    FILTER               [09:00]   10-bit Filter Coefficient 
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_10                                     (APEX_642CL_HRSZ_CORE_0_PPF_FILTER_10_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_FILTER_10_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_10_BASE_ADDRESS                        (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_10_OFFSET                              (0x00000040)
#define APEX_642CL_HRSZ_CORE_0_PPF_FILTER_10_DEFAULT                             (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    OUT_CLAMP
//    APEX_642CL_HRSZ_CORE_OUT_CLAMP
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_OUT_CLAMP_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_OUT_CLAMP_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000044
//    Default:      32'hffff0000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Output Clamp Level
// 
// Device Control Register Bits Description:
//    MIN                  [15:00]   Minimum Level
//                                   Based upon Output Format (unsigned/signed N bits).
//                                   The lower N bits represent the minimum value for the signed/unsigned output format. 
//    MAX                  [31:16]   Maximum Level
//                                   Based upon Output Format (unsigned/signed N bits).
//                                   The lower N bits represent the maximum value for the signed/unsigned output format. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_OUT_CLAMP                                         (APEX_642CL_HRSZ_CORE_0_OUT_CLAMP_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_OUT_CLAMP_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_OUT_CLAMP_BASE_ADDRESS                            (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_OUT_CLAMP_OFFSET                                  (0x00000044)
#define APEX_642CL_HRSZ_CORE_0_OUT_CLAMP_DEFAULT                                 (0xffff0000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI1_SRC_RES
//    APEX_642CL_HRSZ_CORE_WOI1_SRC_RES
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000048
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 1's Source Resolution
// 
// Device Control Register Bits Description:
//    HEIGHT               [15:00]   Vertical Height 
//    WIDTH                [31:16]   Horizontal Width 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_WOI1_SRC_RES                                      (APEX_642CL_HRSZ_CORE_0_WOI1_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_WOI1_SRC_RES_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_WOI1_SRC_RES_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_WOI1_SRC_RES_OFFSET                               (0x00000048)
#define APEX_642CL_HRSZ_CORE_0_WOI1_SRC_RES_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI1_CROP_0
//    APEX_642CL_HRSZ_CORE_WOI1_CROP_0
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI1_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI1_CROP_0_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h0000004c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 1's CROP Start Coordinate
// 
// Device Control Register Bits Description:
//    V_OFFSET             [15:00]   Vertical Offset 
//    H_OFFSET             [31:16]   Horizontal Offset 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_0                                       (APEX_642CL_HRSZ_CORE_0_WOI1_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_WOI1_CROP_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_0_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_0_OFFSET                                (0x0000004c)
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_0_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI1_CROP_1
//    APEX_642CL_HRSZ_CORE_WOI1_CROP_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI1_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI1_CROP_1_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000050
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 1's CROP Resolution
// 
// Device Control Register Bits Description:
//    V_VALID              [15:00]   Vertical Size 
//    H_VALID              [31:16]   Horizontal Size 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_1                                       (APEX_642CL_HRSZ_CORE_0_WOI1_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_WOI1_CROP_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_1_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_1_OFFSET                                (0x00000050)
#define APEX_642CL_HRSZ_CORE_0_WOI1_CROP_1_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_IMG1_RES
//    APEX_642CL_HRSZ_CORE_PPF_IMG1_RES
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000054
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Resolution of 1st Internal Image from AVG
//    PPF Source resolution size is output resolution size from AVERAGE block}
// 
// Device Control Register Bits Description:
//    RESERVED             [15:00]   Vertical Size (Not Used) 
//    WIDTH                [31:16]   Horizontal Size 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_PPF_IMG1_RES                                      (APEX_642CL_HRSZ_CORE_0_PPF_IMG1_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_IMG1_RES_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_IMG1_RES_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_IMG1_RES_OFFSET                               (0x00000054)
#define APEX_642CL_HRSZ_CORE_0_PPF_IMG1_RES_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_SCALE
//    APEX_642CL_HRSZ_CORE_PPF_SCALE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_SCALE_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_SCALE_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000058
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    PPF Scale Factor
// 
// Device Control Register Bits Description:
//    HSCALE_START_FACTOR  [15:00]   Horizontal Scale Start Offset 
//    HSCALE_FACTOR        [31:16]   Horizontal Scale Factor
//                                   Factor = (source size * 1024)/scaled size
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_PPF_SCALE                                         (APEX_642CL_HRSZ_CORE_0_PPF_SCALE_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_PPF_SCALE_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_PPF_SCALE_BASE_ADDRESS                            (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_PPF_SCALE_OFFSET                                  (0x00000058)
#define APEX_642CL_HRSZ_CORE_0_PPF_SCALE_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI2_SRC_RES
//    APEX_642CL_HRSZ_CORE_WOI2_SRC_RES
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h0000005c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Resolution of 2nd Internal Image from PPF; WOI2's SRC Resolution
// 
// Device Control Register Bits Description:
//    HEIGHT               [15:00]   Vertical Height 
//    WIDTH                [31:16]   Horizontal Width 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_WOI2_SRC_RES                                      (APEX_642CL_HRSZ_CORE_0_WOI2_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_WOI2_SRC_RES_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_WOI2_SRC_RES_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_WOI2_SRC_RES_OFFSET                               (0x0000005c)
#define APEX_642CL_HRSZ_CORE_0_WOI2_SRC_RES_DEFAULT                              (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI2_CROP_0
//    APEX_642CL_HRSZ_CORE_WOI2_CROP_0
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI2_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI2_CROP_0_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000060
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 2's CROP Start Coordinate
// 
// Device Control Register Bits Description:
//    V_OFFSET             [15:00]   Vertical Offset 
//    H_OFFSET             [31:16]   Horizontal Offset 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_0                                       (APEX_642CL_HRSZ_CORE_0_WOI2_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_WOI2_CROP_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_0_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_0_OFFSET                                (0x00000060)
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_0_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI2_CROP_1
//    APEX_642CL_HRSZ_CORE_WOI2_CROP_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI2_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI2_CROP_1_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000064
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 2's CROP Resolution (Destination Resolution)
// 
// Device Control Register Bits Description:
//    V_VALID              [15:00]   Vertical Size 
//    H_VALID              [31:16]   Horizontal Size 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_1                                       (APEX_642CL_HRSZ_CORE_0_WOI2_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_WOI2_CROP_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_1_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_1_OFFSET                                (0x00000064)
#define APEX_642CL_HRSZ_CORE_0_WOI2_CROP_1_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IRQ_STATUS
//    APEX_642CL_HRSZ_CORE_IRQ_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_IRQ_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_IRQ_STATUS_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000070
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Status
// 
// Device Control Register Bits Description:
//    DONE                 [00]      Scaler Done 
//    IFIFO_RD_ERROR       [01]      IFIFO Read Error 
//    IFIFO_WR_ERROR       [02]      IFIFO Write Error 
//    OFIFO_WR_ERROR       [03]      OFIFO Read Error 
//    OFIFO_RD_ERROR       [04]      OFIFO Write Error 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_IRQ_STATUS                                        (APEX_642CL_HRSZ_CORE_0_IRQ_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_IRQ_STATUS_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_IRQ_STATUS_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_IRQ_STATUS_OFFSET                                 (0x00000070)
#define APEX_642CL_HRSZ_CORE_0_IRQ_STATUS_DEFAULT                                (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IRQ_MASK
//    APEX_642CL_HRSZ_CORE_IRQ_MASK
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_IRQ_MASK_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_IRQ_MASK_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000074
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Mask.
//    Write 0x0 to respective bits to enable IRQ generation.
//    
// 
// Device Control Register Bits Description:
//    DONE                 [00]      Scaler Done 
//    IFIFO_RD_ERROR       [01]      IFIFO Read Error 
//    IFIFO_WR_ERROR       [02]      IFIFO Write Error 
//    OFIFO_WR_ERROR       [03]      OFIFO Read Error 
//    OFIFO_RD_ERROR       [04]      OFIFO Write Error 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_IRQ_MASK                                          (APEX_642CL_HRSZ_CORE_0_IRQ_MASK_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_IRQ_MASK_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_IRQ_MASK_BASE_ADDRESS                             (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_IRQ_MASK_OFFSET                                   (0x00000074)
#define APEX_642CL_HRSZ_CORE_0_IRQ_MASK_DEFAULT                                  (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IRQ_CLEAR
//    APEX_642CL_HRSZ_CORE_IRQ_CLEAR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_IRQ_CLEAR_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000078
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Clear
//    Write 0x1 to clear respective status.
//    
// 
// Device Control Register Bits Description:
//    DONE                 [00]      Scaler Done 
//    IFIFO_RD_ERROR       [01]      IFIFO Read Error 
//    IFIFO_WR_ERROR       [02]      IFIFO Write Error 
//    OFIFO_WR_ERROR       [03]      OFIFO Read Error 
//    OFIFO_RD_ERROR       [04]      OFIFO Write Error 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_IRQ_CLEAR                                         (APEX_642CL_HRSZ_CORE_0_IRQ_CLEAR_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_IRQ_CLEAR_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_IRQ_CLEAR_BASE_ADDRESS                            (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_IRQ_CLEAR_OFFSET                                  (0x00000078)
#define APEX_642CL_HRSZ_CORE_0_IRQ_CLEAR_DEFAULT                                 (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    FIFO_STATUS
//    APEX_642CL_HRSZ_CORE_FIFO_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_FIFO_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_FIFO_STATUS_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h0000007c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Clear
//    Write 0x1 to clear respective status.
// 
// Device Control Register Bits Description:
//    OFIFO_RD_BYTE_CNT    [09:00]   Output FIFO Read Byte Count 
//    RESERVED_0           [13:10]   Reserved 
//    OFIFO_FULL           [14]      Output FIFO Full Status 
//    OFIFO_EMPTY          [15]      Output FIFO Empty Status 
//    IFIFO_WR_BYTE_CNT    [25:16]   Input FIFO Write Byte Count 
//    RESERVED_1           [29:26]   Reserved 
//    IFIFO_FULL           [30]      Input FIFO Full Status 
//    IFIFO_EMPTY          [31]      Input FIFO Empty Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_0_FIFO_STATUS                                       (APEX_642CL_HRSZ_CORE_0_FIFO_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_0_FIFO_STATUS_OFFSET)
#define APEX_642CL_HRSZ_CORE_0_FIFO_STATUS_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_0)
#define APEX_642CL_HRSZ_CORE_0_FIFO_STATUS_OFFSET                                (0x0000007c)
#define APEX_642CL_HRSZ_CORE_0_FIFO_STATUS_DEFAULT                               (0x00000000)


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    FLUSH_INIT
//    APEX_642CL_HRSZ_CORE_FLUSH_INIT
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_FLUSH_INIT_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_FLUSH_INIT_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000000
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Flush FIFOs and Init Data processing
// 
// Device Control Register Bits Description:
//    FLUSH_0              [00]      Flush Input FIFO 
//    FLUSH_1              [01]      Flush PPF FIFO 
//    FLUSH_2              [02]      Flush Output FIFO 
//    INIT                 [03]      Initialize Datapath 
//    START                [04]      Start Datapath 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_FLUSH_INIT                                        (APEX_642CL_HRSZ_CORE_1_FLUSH_INIT_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_FLUSH_INIT_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_FLUSH_INIT_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_FLUSH_INIT_OFFSET                                 (0x00000000)
#define APEX_642CL_HRSZ_CORE_1_FLUSH_INIT_DEFAULT                                (0x00000000)


union _APEX_642CL_HRSZ_CORE_FLUSH_INIT_TAG
{
   struct _asApex642clHrszCoreFlushInitBitFieldTag
   {
      REG32  Flush0               :  1; //  00     APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_FLUSH_0
      REG32  Flush1               :  1; //  01     APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_FLUSH_1
      REG32  Flush2               :  1; //  02     APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_FLUSH_2
      REG32  Init                 :  1; //  03     APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_INIT
      REG32  Start                :  1; //  04     APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_START
      REG32  Reserved             : 27; //  31:05  APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_FLUSH_INIT_TAG                               APEX_642CL_HRSZ_CORE_FLUSH_INIT_DCR;

extern volatile APEX_642CL_HRSZ_CORE_FLUSH_INIT_DCR                              gApex642clHrszCoreFlushInit;
extern volatile APEX_642CL_HRSZ_CORE_FLUSH_INIT_DCR*                             gpApex642clHrszCoreFlushInit;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_FLUSH_0                              0
#define APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_FLUSH_1                              1
#define APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_FLUSH_2                              2
#define APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_INIT                                 3
#define APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_START                                4
#define APEX_642CL_HRSZ_CORE_FLUSH_INIT_BIT_RESERVED                             5

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    INSTRUCTION
//    APEX_642CL_HRSZ_CORE_INSTRUCTION
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_INSTRUCTION_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_INSTRUCTION_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000004
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Configuration
// 
// Device Control Register Bits Description:
//    CLK_GATE_ENB         [00]      System Clock Gate Enable.
//                                   When HRSZ Core resource is not utilized, application can place HRSZ core into low power mode.
//                                   0: Disabled (Low Power enabled; No sys_clk provided to core)
//                                   1: Enabled (Active Power enabled; sys_clk provided to core)
//                                    
//    LPF_EN               [01]      Low Pass Filter Enable
//                                   0 - Disabled
//                                   1 - Enabled
//                                    
//    H_AVG_MODE           [04:02]   Average: Horizontal Mode.
//                                   000 : No Average
//                                   001 : 1/2 Average
//                                   010 : 1/4 Average
//                                   011 : 1/8 Average
//                                   100 : 1/16 Average
//                                   101 : 1/32 Average
//                                    
//    PPF_EN               [05]      Poly Phase Filter Enable
//                                   0 - Disabled
//                                   1 - Enabled
//                                    
//    PPF_TAP_SELECT       [06]      The Number of Taps for the First Horizontal Poly-Phase Filtered Scaling
//                                   0 : 4 Taps
//                                   1 : 8 Taps
//                                    
//    RESERVED_0           [07]      Reserved 
//    IN_FORMAT            [11:08]   Integer Mode (Instruction[31]==0x0)
//                                        Input Format Wide Dynamic Range Select.
//                                        0000 - Unsigned 8-bit
//                                        0001 - Unsigned 10-bit
//                                        0010 - Unsigned 12-bit
//                                        0011 - Unsigned 14-bit
//                                        0100 - Unsigned 16-bit
//                                        0101 - Signed 8-bit
//                                        0110 - Signed 9-bit
//                                        0111 - Signed 10-bit
//                                        1000 - Signed 16-bit
//                                        1001 - 1111 - Reserved
//                                   Fixed Mode (Instruction[31]==0x1)
//                                        Fixed decimal point shift
//                                        0000 - Right Shift by 0
//                                        0001 - Right Shift by 1
//                                        0010 - Right Shift by 2
//                                        0011 - Right Shift by 3
//                                        0100 - Right Shift by 4
//                                        0101 - Right Shift by 5
//                                        0110 - Right Shift by 6
//                                        0111 - Right Shift by 7
//                                        1000 - Left Shift by 0
//                                        1001 - Left Shift by 1
//                                        1010 - Left Shift by 2
//                                        1011 - Left Shift by 3
//                                        1100 - Left Shift by 4
//                                        1101 - Left Shift by 5
//                                        1110 - Left Shift by 6
//                                        1111 - Left Shift by 7 
//    OUT_FORMAT           [15:12]   Integer Mode (Instruction[27]==0x0)
//                                        Output Format Wide Dynamic Range Select.
//                                        0000 - Unsigned 8-bit
//                                        0001 - Unsigned 10-bit
//                                        0010 - Unsigned 12-bit
//                                        0011 - Unsigned 14-bit
//                                        0100 - Unsigned 16-bit
//                                        0101 - Signed 8-bit
//                                        0110 - Signed 9-bit
//                                        0111 - Signed 10-bit
//                                        1000 - Signed 16-bit
//                                        1001 - 1111 - Reserved
//                                   Fixed Mode (Instruction[27]==0x1)
//                                        Fixed decimal point shift
//                                        0000 - Right Shift by 0
//                                        0001 - Right Shift by 1
//                                        0010 - Right Shift by 2
//                                        0011 - Right Shift by 3
//                                        0100 - Right Shift by 4
//                                        0101 - Right Shift by 5
//                                        0110 - Right Shift by 6
//                                        0111 - Right Shift by 7
//                                        1000 - Left Shift by 0
//                                        1001 - Left Shift by 1
//                                        1010 - Left Shift by 2
//                                        1011 - Left Shift by 3
//                                        1100 - Left Shift by 4
//                                        1101 - Left Shift by 5
//                                        1110 - Left Shift by 6
//                                        1111 - Left Shift by 7 
//    LPF_ROUND_FACTOR     [19:16]   LPF Rounding Factor
//                                   Rounding of the algorithm can be controlled to match precision (decimal point location) of the filter coefficients.
//                                   0: r_bsum[19:3]
//                                   1: r_bsum[20:4]
//                                   2: r_bsum[21:5]
//                                   3: r_bsum[22:6]
//                                   4: r_bsum[23:7]
//                                   5: r_bsum[24:8]
//                                   6: r_bsum[25:9]
//                                   7: r_bsum[26:10]
//                                   8: r_bsum[27:11]
//                                   9: r_bsum[28:12]
//                                   10: r_bsum[29:13]
//                                   11-15: reserved
//                                    
//    PPF_ROUND_FACTOR     [23:20]   PPF Rounding Factor
//                                   Rounding of the algorithm can be controlled to match precision (decimal point location) of the filter coefficients.
//                                   0: r_sum[21:5] + r_sum[4]
//                                   1: r_sum[22:6] + r_sum[5]
//                                   2: r_sum[23:7] + r_sum[6]
//                                   3: r_sum[24:8] + r_sum[7]
//                                   4: r_sum[25:9] + r_sum[8]
//                                   5: r_sum[26:10] + r_sum[9]
//                                   6: r_sum[27:11] + r_sum[10]
//                                   7: r_sum[28:12] + r_sum[11]
//                                   8-15: reserved
//                                    
//    RESERVED1            [24]      Reserved 
//    OUT_16bit_8bit       [25]      Fixed Point Output Data Format identification for conversion from internal s17 format
//                                   0 - 8bit
//                                   1 - 16bit 
//    OUT_Signed_Unsigned  [26]      Fixed Point Output Data Format identification for conversion from internal s17 format
//                                   0 - Unsigned
//                                   1 - Signed 
//    OUT_Fixed_Integer    [27]      Output Data Format identification for conversion from internal s17 format
//                                   0 - Integer
//                                   1 - Fixed Point 
//    RESERVED2            [28]      Reserved 
//    IN_16bit_8bit        [29]      Fixed Point Input Data Format identification for conversion to internal s17 format
//                                   0 - 8bit
//                                   1 - 16bit 
//    IN_Signed_Unsigned   [30]      Fixed Point Input Data Format identification for conversion to internal s17 format
//                                   0 - Unsigned
//                                   1 - Signed 
//    IN_Fixed_Integer     [31]      Input Data Format identification for conversion to internal s17 format
//                                   0 - Integer
//                                   1 - Fixed Point 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_INSTRUCTION                                       (APEX_642CL_HRSZ_CORE_1_INSTRUCTION_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_INSTRUCTION_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_INSTRUCTION_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_INSTRUCTION_OFFSET                                (0x00000004)
#define APEX_642CL_HRSZ_CORE_1_INSTRUCTION_DEFAULT                               (0x00000000)


union _APEX_642CL_HRSZ_CORE_INSTRUCTION_TAG
{
   struct _asApex642clHrszCoreInstructionBitFieldTag
   {
      REG32  ClkGateEnb           :  1; //  00     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_CLK_GATE_ENB
      REG32  LpfEn                :  1; //  01     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_EN
      REG32  HAvgMode             :  3; //  04:02  APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE
      REG32  PpfEn                :  1; //  05     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_EN
      REG32  PpfTapSelect         :  1; //  06     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_TAP_SELECT
      REG32  Reserved0            :  1; //  07     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_RESERVED_0
      REG32  InFormat             :  4; //  11:08  APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT
      REG32  OutFormat            :  4; //  15:12  APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT
      REG32  LpfRoundFactor       :  4; //  19:16  APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR
      REG32  PpfRoundFactor       :  4; //  23:20  APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR
      REG32  Reserved1            :  1; //  24     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_RESERVED1
      REG32  Out16bit8bit         :  1; //  25     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_16BIT_8BIT
      REG32  OutSignedUnsigned    :  1; //  26     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_SIGNED_UNSIGNED
      REG32  OutFixedInteger      :  1; //  27     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FIXED_INTEGER
      REG32  Reserved2            :  1; //  28     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_RESERVED2
      REG32  In16bit8bit          :  1; //  29     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_16BIT_8BIT
      REG32  InSignedUnsigned     :  1; //  30     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_SIGNED_UNSIGNED
      REG32  InFixedInteger       :  1; //  31     APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FIXED_INTEGER
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_INSTRUCTION_TAG                              APEX_642CL_HRSZ_CORE_INSTRUCTION_DCR;

extern volatile APEX_642CL_HRSZ_CORE_INSTRUCTION_DCR                             gApex642clHrszCoreInstruction;
extern volatile APEX_642CL_HRSZ_CORE_INSTRUCTION_DCR*                            gpApex642clHrszCoreInstruction;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_CLK_GATE_ENB                        0
#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_EN                              1

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_EN_ENUM                           
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_EN_ID_ENABLE                         =  1,          // Low Pass Filter Enable ENABLE
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_EN_ID_DISABLE                        =  0           // Low Pass Filter Enable DISABLE
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE                          2

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE_ENUM                       
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE_ID_NONE                       =  0,          // Average: Horizontal Mode NONE
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE_ID_ONE_HALF                   =  1,          // Average: Horizontal Mode ONE_HALF
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE_ID_ONE_FOURTH                 =  2,          // Average: Horizontal Mode ONE_FOURTH
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE_ID_ONE_EIGHT                  =  3,          // Average: Horizontal Mode ONE_EIGHT
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE_ID_ONE_SIXTEEN                =  4,          // Average: Horizontal Mode ONE_SIXTEEN
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_H_AVG_MODE_ID_ONE_THIRTY_TWO             =  5           // Average: Horizontal Mode ONE_THIRTY_TWO
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_EN                              5

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_EN_ENUM                           
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_EN_ID_ENABLE                         =  1,          // Poly Phase Filter Enable ENABLE
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_EN_ID_DISABLE                        =  0           // Poly Phase Filter Enable DISABLE
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_TAP_SELECT                      6

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_TAP_SELECT_ENUM                   
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_TAP_SELECT_ID_4_TAPS                 =  0,          // First Horizontal Poly-Phase Filtered Scaling with 4_TAPS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_TAP_SELECT_ID_8_TAPS                 =  1           // First Horizontal Poly-Phase Filtered Scaling with 8_TAPS
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_RESERVED_0                          7
#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT                           8

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ENUM                        
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_UNSIGNED_8_BITS             =  0,          // Dynamic range input format UNSIGNED_8_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_UNSIGNED_10_BITS            =  1,          // Dynamic range input format UNSIGNED_10_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_UNSIGNED_12_BITS            =  2,          // Dynamic range input format UNSIGNED_12_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_UNSIGNED_14_BITS            =  3,          // Dynamic range input format UNSIGNED_14_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_UNSIGNED_16_BITS            =  4,          // Dynamic range input format UNSIGNED_16_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_SIGNED_8_BITS               =  5,          // Dynamic range input format SIGNED_8_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_SIGNED_9_BITS               =  5,          // Dynamic range input format SIGNED_9_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_SIGNED_10_BITS              =  7,          // Dynamic range input format SIGNED_10_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FORMAT_ID_SIGNED_16_BITS              =  8           // Dynamic range input format SIGNED_16_BITS
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT                          12

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ENUM                       
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_UNSIGNED_8_BITS            =  0,          // Dynamic range output format UNSIGNED_8_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_UNSIGNED_10_BITS           =  1,          // Dynamic range output format UNSIGNED_10_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_UNSIGNED_12_BITS           =  2,          // Dynamic range output format UNSIGNED_12_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_UNSIGNED_14_BITS           =  3,          // Dynamic range output format UNSIGNED_14_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_UNSIGNED_16_BITS           =  4,          // Dynamic range output format UNSIGNED_16_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_SIGNED_8_BITS              =  5,          // Dynamic range output format SIGNED_8_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_SIGNED_9_BITS              =  5,          // Dynamic range output format SIGNED_9_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_SIGNED_10_BITS             =  7,          // Dynamic range output format SIGNED_10_BITS
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FORMAT_ID_SIGNED_16_BITS             =  8           // Dynamic range output format SIGNED_16_BITS
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR                    16

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ENUM                 
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_19_3            =  0,          // LPF Rounding Factor BSUM_19_3
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_20_4            =  1,          // LPF Rounding Factor BSUM_20_4
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_21_5            =  2,          // LPF Rounding Factor BSUM_21_5
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_22_6            =  3,          // LPF Rounding Factor BSUM_22_6
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_23_7            =  4,          // LPF Rounding Factor BSUM_23_7
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_24_8            =  5,          // LPF Rounding Factor BSUM_24_8
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_25_9            =  5,          // LPF Rounding Factor BSUM_25_9
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_26_10           =  7,          // LPF Rounding Factor BSUM_26_10
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_27_11           =  8,          // LPF Rounding Factor BSUM_27_11
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_28_12           =  9,          // LPF Rounding Factor BSUM_28_12
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_LPF_ROUND_FACTOR_ID_BSUM_29_13           =  10          // LPF Rounding Factor BSUM_29_13
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR                    20

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ENUM                 
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_21_5             =  0,          // PPF Rounding Factor SUM_21_5
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_22_6             =  1,          // PPF Rounding Factor SUM_22_6
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_23_7             =  2,          // PPF Rounding Factor SUM_23_7
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_24_8             =  3,          // PPF Rounding Factor SUM_24_8
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_25_9             =  4,          // PPF Rounding Factor SUM_25_9
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_26_10            =  5,          // PPF Rounding Factor SUM_26_10
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_27_11            =  5,          // PPF Rounding Factor SUM_27_11
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_PPF_ROUND_FACTOR_ID_SUM_28_12            =  7           // PPF Rounding Factor SUM_28_12
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_RESERVED1                           24
#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_16BIT_8BIT                      25

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_16BIT_8BIT_ENUM                   
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_16BIT_8BIT_ID_BIT_8                  =  0,          // 8bit Fixed Point Output Data
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_16BIT_8BIT_ID_BIT_16                 =  1           // 16bit Fixed Point Output Data
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_SIGNED_UNSIGNED                 26

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_SIGNED_UNSIGNED_ENUM              
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_SIGNED_UNSIGNED_ID_UNSIGNED          =  0,          // Unsigned Fixed Point Output data
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_SIGNED_UNSIGNED_ID_SIGNED            =  1           // Signed Fixed Point Output data
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FIXED_INTEGER                   27

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FIXED_INTEGER_ENUM                
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FIXED_INTEGER_ID_INTEGER             =  0,          // Integer type Output Data
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_OUT_FIXED_INTEGER_ID_FIXED               =  1           // Fixed Point type Output Data
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_RESERVED2                           28
#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_16BIT_8BIT                       29

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_16BIT_8BIT_ENUM                    
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_16BIT_8BIT_ID_BIT_8                   =  0,          // 8bit Fixed Point Input Data
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_16BIT_8BIT_ID_BIT_16                  =  1           // 16bit Fixed Point Input Data
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_SIGNED_UNSIGNED                  30

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_SIGNED_UNSIGNED_ENUM               
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_SIGNED_UNSIGNED_ID_UNSIGNED           =  0,          // Unsigned Fixed Point Input data
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_SIGNED_UNSIGNED_ID_SIGNED             =  1           // Signed Fixed Point Input data
};
                                                                             

#define APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FIXED_INTEGER                    31

// Device Control Register Bits Id and Description
enum APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FIXED_INTEGER_ENUM                 
{
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FIXED_INTEGER_ID_INTEGER              =  0,          // Integer type Input Data
   APEX_642CL_HRSZ_CORE_INSTRUCTION_BIT_IN_FIXED_INTEGER_ID_FIXED                =  1           // Fixed Point type Input Data
};
                                                                             


/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LPF_COEF
//    APEX_642CL_HRSZ_CORE_LPF_COEF
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_LPF_COEF_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_LPF_COEF_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000008
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         3
//    Size:       4
// 
// Description:
//    9-Tap Low Pass Filter Coefficients
// 
// Device Control Register Bits Description:
//    Y_COEF               [09:00]   Y Coefficient 
//    RESERVED             [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_0                                        (APEX_642CL_HRSZ_CORE_1_LPF_COEF_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_LPF_COEF_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_0_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_0_OFFSET                                 (0x00000008)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_0_DEFAULT                                (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_1                                        (APEX_642CL_HRSZ_CORE_1_LPF_COEF_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_LPF_COEF_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_1_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_1_OFFSET                                 (0x0000000c)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_1_DEFAULT                                (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_2                                        (APEX_642CL_HRSZ_CORE_1_LPF_COEF_2_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_LPF_COEF_2_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_2_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_2_OFFSET                                 (0x00000010)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_2_DEFAULT                                (0x00000000)


union _APEX_642CL_HRSZ_CORE_LPF_COEF_TAG
{
   struct _asApex642clHrszCoreLpfCoefBitFieldTag
   {
      REG32S YCoef0               : 10; //  9:0    APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_Y_COEF_0
      REG32S YCoef1               : 10; //  19:10  APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_Y_COEF_1
      REG32S YCoef2               : 10; //  29:20  APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_Y_COEF_2
      REG32  Reserved             :  2; //  31:30  APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_LPF_COEF_TAG                                 APEX_642CL_HRSZ_CORE_LPF_COEF_DCR;

extern volatile APEX_642CL_HRSZ_CORE_LPF_COEF_DCR                                gApex642clHrszCoreLpfCoef;
extern volatile APEX_642CL_HRSZ_CORE_LPF_COEF_DCR*                               gpApex642clHrszCoreLpfCoef;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_Y_COEF_0                               0
#define APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_Y_COEF_1                               10
#define APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_Y_COEF_2                               20
#define APEX_642CL_HRSZ_CORE_LPF_COEF_BIT_RESERVED                               30

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    LPF_COEF_3
//    APEX_642CL_HRSZ_CORE_LPF_COEF_3
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_LPF_COEF_3_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_LPF_COEF_3_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000014
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    9-Tap Low Pass Filter Coefficients
// 
// Device Control Register Bits Description:
//    Y_COEF               [09:00]   Y Coefficient 
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_3                                        (APEX_642CL_HRSZ_CORE_1_LPF_COEF_3_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_LPF_COEF_3_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_3_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_3_OFFSET                                 (0x00000014)
#define APEX_642CL_HRSZ_CORE_1_LPF_COEF_3_DEFAULT                                (0x00000000)


union _APEX_642CL_HRSZ_CORE_LPF_COEF_3_TAG
{
   struct _asApex642clHrszCoreLpfCoef3BitFieldTag
   {
      REG32S YCoef0               : 10; //  9:0    APEX_642CL_HRSZ_CORE_LPF_COEF_3_BIT_Y_COEF_0
      REG32S YCoef1               : 10; //  19:10  APEX_642CL_HRSZ_CORE_LPF_COEF_3_BIT_Y_COEF_1
      REG32  Reserved             : 12; //  31:20  APEX_642CL_HRSZ_CORE_LPF_COEF_3_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_LPF_COEF_3_TAG                               APEX_642CL_HRSZ_CORE_LPF_COEF_3_DCR;

extern volatile APEX_642CL_HRSZ_CORE_LPF_COEF_3_DCR                              gApex642clHrszCoreLpfCoef3;
extern volatile APEX_642CL_HRSZ_CORE_LPF_COEF_3_DCR*                             gpApex642clHrszCoreLpfCoef3;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_LPF_COEF_3_BIT_Y_COEF_0                             0
#define APEX_642CL_HRSZ_CORE_LPF_COEF_3_BIT_Y_COEF_1                             10
#define APEX_642CL_HRSZ_CORE_LPF_COEF_3_BIT_RESERVED                             20

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_FILTER
//    APEX_642CL_HRSZ_CORE_PPF_FILTER
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_FILTER_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_FILTER_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000018
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         10
//    Size:       4
// 
// Description:
//    Poly Phase Filter for Horizontal Scaling
// 
// Device Control Register Bits Description:
//    FILTER               [09:00]   10-bit Filter Coefficient 
//    RESERVED             [31:30]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_0                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_0_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_0_OFFSET                               (0x00000018)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_0_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_1                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_1_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_1_OFFSET                               (0x0000001c)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_1_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_2                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_2_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_2_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_2_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_2_OFFSET                               (0x00000020)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_2_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_3                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_3_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_3_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_3_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_3_OFFSET                               (0x00000024)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_3_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_4                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_4_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_4_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_4_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_4_OFFSET                               (0x00000028)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_4_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_5                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_5_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_5_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_5_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_5_OFFSET                               (0x0000002c)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_5_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_6                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_6_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_6_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_6_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_6_OFFSET                               (0x00000030)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_6_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_7                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_7_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_7_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_7_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_7_OFFSET                               (0x00000034)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_7_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_8                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_8_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_8_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_8_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_8_OFFSET                               (0x00000038)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_8_DEFAULT                              (0x00000000)

#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_9                                      (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_9_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_9_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_9_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_9_OFFSET                               (0x0000003c)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_9_DEFAULT                              (0x00000000)


union _APEX_642CL_HRSZ_CORE_PPF_FILTER_TAG
{
   struct _asApex642clHrszCorePpfFilterBitFieldTag
   {
      REG32S Filter0              : 10; //  9:0    APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_FILTER_0
      REG32S Filter1              : 10; //  19:10  APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_FILTER_1
      REG32S Filter2              : 10; //  29:20  APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_FILTER_2
      REG32  Reserved             :  2; //  31:30  APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_PPF_FILTER_TAG                               APEX_642CL_HRSZ_CORE_PPF_FILTER_DCR;

extern volatile APEX_642CL_HRSZ_CORE_PPF_FILTER_DCR                              gApex642clHrszCorePpfFilter;
extern volatile APEX_642CL_HRSZ_CORE_PPF_FILTER_DCR*                             gpApex642clHrszCorePpfFilter;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_FILTER_0                             0
#define APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_FILTER_1                             10
#define APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_FILTER_2                             20
#define APEX_642CL_HRSZ_CORE_PPF_FILTER_BIT_RESERVED                             30

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_FILTER_10
//    APEX_642CL_HRSZ_CORE_PPF_FILTER_10
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_FILTER_10_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000040
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Poly Phase Filter for Horizontal Scaling
// 
// Device Control Register Bits Description:
//    FILTER               [09:00]   10-bit Filter Coefficient 
//    RESERVED             [31:20]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_10                                     (APEX_642CL_HRSZ_CORE_1_PPF_FILTER_10_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_FILTER_10_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_10_BASE_ADDRESS                        (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_10_OFFSET                              (0x00000040)
#define APEX_642CL_HRSZ_CORE_1_PPF_FILTER_10_DEFAULT                             (0x00000000)


union _APEX_642CL_HRSZ_CORE_PPF_FILTER_10_TAG
{
   struct _asApex642clHrszCorePpfFilter10BitFieldTag
   {
      REG32S Filter0              : 10; //  9:0    APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BIT_FILTER_0
      REG32S Filter1              : 10; //  19:10  APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BIT_FILTER_1
      REG32  Reserved             : 12; //  31:20  APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_PPF_FILTER_10_TAG                            APEX_642CL_HRSZ_CORE_PPF_FILTER_10_DCR;

extern volatile APEX_642CL_HRSZ_CORE_PPF_FILTER_10_DCR                           gApex642clHrszCorePpfFilter10;
extern volatile APEX_642CL_HRSZ_CORE_PPF_FILTER_10_DCR*                          gpApex642clHrszCorePpfFilter10;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BIT_FILTER_0                          0
#define APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BIT_FILTER_1                          10
#define APEX_642CL_HRSZ_CORE_PPF_FILTER_10_BIT_RESERVED                          20

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    OUT_CLAMP
//    APEX_642CL_HRSZ_CORE_OUT_CLAMP
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_OUT_CLAMP_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_OUT_CLAMP_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000044
//    Default:      32'hffff0000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Output Clamp Level
// 
// Device Control Register Bits Description:
//    MIN                  [15:00]   Minimum Level
//                                   Based upon Output Format (unsigned/signed N bits).
//                                   The lower N bits represent the minimum value for the signed/unsigned output format. 
//    MAX                  [31:16]   Maximum Level
//                                   Based upon Output Format (unsigned/signed N bits).
//                                   The lower N bits represent the maximum value for the signed/unsigned output format. 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_OUT_CLAMP                                         (APEX_642CL_HRSZ_CORE_1_OUT_CLAMP_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_OUT_CLAMP_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_OUT_CLAMP_BASE_ADDRESS                            (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_OUT_CLAMP_OFFSET                                  (0x00000044)
#define APEX_642CL_HRSZ_CORE_1_OUT_CLAMP_DEFAULT                                 (0xffff0000)


union _APEX_642CL_HRSZ_CORE_OUT_CLAMP_TAG
{
   struct _asApex642clHrszCoreOutClampBitFieldTag
   {
      REG32  Min                  : 16; //  15:00  APEX_642CL_HRSZ_CORE_OUT_CLAMP_BIT_MIN
      REG32  Max                  : 16; //  31:16  APEX_642CL_HRSZ_CORE_OUT_CLAMP_BIT_MAX
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_OUT_CLAMP_TAG                                APEX_642CL_HRSZ_CORE_OUT_CLAMP_DCR;

extern volatile APEX_642CL_HRSZ_CORE_OUT_CLAMP_DCR                               gApex642clHrszCoreOutClamp;
extern volatile APEX_642CL_HRSZ_CORE_OUT_CLAMP_DCR*                              gpApex642clHrszCoreOutClamp;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_OUT_CLAMP_BIT_MIN                                   0
#define APEX_642CL_HRSZ_CORE_OUT_CLAMP_BIT_MAX                                   16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI1_SRC_RES
//    APEX_642CL_HRSZ_CORE_WOI1_SRC_RES
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000048
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 1's Source Resolution
// 
// Device Control Register Bits Description:
//    HEIGHT               [15:00]   Vertical Height 
//    WIDTH                [31:16]   Horizontal Width 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_WOI1_SRC_RES                                      (APEX_642CL_HRSZ_CORE_1_WOI1_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_WOI1_SRC_RES_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_WOI1_SRC_RES_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_WOI1_SRC_RES_OFFSET                               (0x00000048)
#define APEX_642CL_HRSZ_CORE_1_WOI1_SRC_RES_DEFAULT                              (0x00000000)


union _APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_TAG
{
   struct _asApex642clHrszCoreWoi1SrcResBitFieldTag
   {
      REG32  Height               : 16; //  15:00  APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_BIT_HEIGHT
      REG32  Width                : 16; //  31:16  APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_BIT_WIDTH
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_TAG                             APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_DCR;

extern volatile APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_DCR                            gApex642clHrszCoreWoi1SrcRes;
extern volatile APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_DCR*                           gpApex642clHrszCoreWoi1SrcRes;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_BIT_HEIGHT                             0
#define APEX_642CL_HRSZ_CORE_WOI1_SRC_RES_BIT_WIDTH                              16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI1_CROP_0
//    APEX_642CL_HRSZ_CORE_WOI1_CROP_0
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI1_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI1_CROP_0_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h0000004c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 1's CROP Start Coordinate
// 
// Device Control Register Bits Description:
//    V_OFFSET             [15:00]   Vertical Offset 
//    H_OFFSET             [31:16]   Horizontal Offset 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_0                                       (APEX_642CL_HRSZ_CORE_1_WOI1_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_WOI1_CROP_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_0_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_0_OFFSET                                (0x0000004c)
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_0_DEFAULT                               (0x00000000)


union _APEX_642CL_HRSZ_CORE_WOI1_CROP_0_TAG
{
   struct _asApex642clHrszCoreWoi1Crop0BitFieldTag
   {
      REG32  VOffset              : 16; //  15:00  APEX_642CL_HRSZ_CORE_WOI1_CROP_0_BIT_V_OFFSET
      REG32  HOffset              : 16; //  31:16  APEX_642CL_HRSZ_CORE_WOI1_CROP_0_BIT_H_OFFSET
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_WOI1_CROP_0_TAG                              APEX_642CL_HRSZ_CORE_WOI1_CROP_0_DCR;

extern volatile APEX_642CL_HRSZ_CORE_WOI1_CROP_0_DCR                             gApex642clHrszCoreWoi1Crop0;
extern volatile APEX_642CL_HRSZ_CORE_WOI1_CROP_0_DCR*                            gpApex642clHrszCoreWoi1Crop0;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_WOI1_CROP_0_BIT_V_OFFSET                            0
#define APEX_642CL_HRSZ_CORE_WOI1_CROP_0_BIT_H_OFFSET                            16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI1_CROP_1
//    APEX_642CL_HRSZ_CORE_WOI1_CROP_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI1_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI1_CROP_1_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000050
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 1's CROP Resolution
// 
// Device Control Register Bits Description:
//    V_VALID              [15:00]   Vertical Size 
//    H_VALID              [31:16]   Horizontal Size 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_1                                       (APEX_642CL_HRSZ_CORE_1_WOI1_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_WOI1_CROP_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_1_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_1_OFFSET                                (0x00000050)
#define APEX_642CL_HRSZ_CORE_1_WOI1_CROP_1_DEFAULT                               (0x00000000)


union _APEX_642CL_HRSZ_CORE_WOI1_CROP_1_TAG
{
   struct _asApex642clHrszCoreWoi1Crop1BitFieldTag
   {
      REG32  VValid               : 16; //  15:00  APEX_642CL_HRSZ_CORE_WOI1_CROP_1_BIT_V_VALID
      REG32  HValid               : 16; //  31:16  APEX_642CL_HRSZ_CORE_WOI1_CROP_1_BIT_H_VALID
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_WOI1_CROP_1_TAG                              APEX_642CL_HRSZ_CORE_WOI1_CROP_1_DCR;

extern volatile APEX_642CL_HRSZ_CORE_WOI1_CROP_1_DCR                             gApex642clHrszCoreWoi1Crop1;
extern volatile APEX_642CL_HRSZ_CORE_WOI1_CROP_1_DCR*                            gpApex642clHrszCoreWoi1Crop1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_WOI1_CROP_1_BIT_V_VALID                             0
#define APEX_642CL_HRSZ_CORE_WOI1_CROP_1_BIT_H_VALID                             16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_IMG1_RES
//    APEX_642CL_HRSZ_CORE_PPF_IMG1_RES
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000054
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Resolution of 1st Internal Image from AVG
//    PPF Source resolution size is output resolution size from AVERAGE block}
// 
// Device Control Register Bits Description:
//    RESERVED             [15:00]   Vertical Size (Not Used) 
//    WIDTH                [31:16]   Horizontal Size 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_PPF_IMG1_RES                                      (APEX_642CL_HRSZ_CORE_1_PPF_IMG1_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_IMG1_RES_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_IMG1_RES_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_IMG1_RES_OFFSET                               (0x00000054)
#define APEX_642CL_HRSZ_CORE_1_PPF_IMG1_RES_DEFAULT                              (0x00000000)


union _APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_TAG
{
   struct _asApex642clHrszCorePpfImg1ResBitFieldTag
   {
      REG32  Reserved             : 16; //  15:00  APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_BIT_RESERVED
      REG32  Width                : 16; //  31:16  APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_BIT_WIDTH
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_TAG                             APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_DCR;

extern volatile APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_DCR                            gApex642clHrszCorePpfImg1Res;
extern volatile APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_DCR*                           gpApex642clHrszCorePpfImg1Res;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_BIT_RESERVED                           0
#define APEX_642CL_HRSZ_CORE_PPF_IMG1_RES_BIT_WIDTH                              16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PPF_SCALE
//    APEX_642CL_HRSZ_CORE_PPF_SCALE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_PPF_SCALE_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_PPF_SCALE_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000058
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    PPF Scale Factor
// 
// Device Control Register Bits Description:
//    HSCALE_START_FACTOR  [15:00]   Horizontal Scale Start Offset 
//    HSCALE_FACTOR        [31:16]   Horizontal Scale Factor
//                                   Factor = (source size * 1024)/scaled size
//                                    
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_PPF_SCALE                                         (APEX_642CL_HRSZ_CORE_1_PPF_SCALE_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_PPF_SCALE_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_PPF_SCALE_BASE_ADDRESS                            (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_PPF_SCALE_OFFSET                                  (0x00000058)
#define APEX_642CL_HRSZ_CORE_1_PPF_SCALE_DEFAULT                                 (0x00000000)


union _APEX_642CL_HRSZ_CORE_PPF_SCALE_TAG
{
   struct _asApex642clHrszCorePpfScaleBitFieldTag
   {
      REG32  HscaleStartFactor    : 16; //  15:00  APEX_642CL_HRSZ_CORE_PPF_SCALE_BIT_HSCALE_START_FACTOR
      REG32  HscaleFactor         : 16; //  31:16  APEX_642CL_HRSZ_CORE_PPF_SCALE_BIT_HSCALE_FACTOR
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_PPF_SCALE_TAG                                APEX_642CL_HRSZ_CORE_PPF_SCALE_DCR;

extern volatile APEX_642CL_HRSZ_CORE_PPF_SCALE_DCR                               gApex642clHrszCorePpfScale;
extern volatile APEX_642CL_HRSZ_CORE_PPF_SCALE_DCR*                              gpApex642clHrszCorePpfScale;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_PPF_SCALE_BIT_HSCALE_START_FACTOR                   0
#define APEX_642CL_HRSZ_CORE_PPF_SCALE_BIT_HSCALE_FACTOR                         16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI2_SRC_RES
//    APEX_642CL_HRSZ_CORE_WOI2_SRC_RES
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h0000005c
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    Resolution of 2nd Internal Image from PPF; WOI2's SRC Resolution
// 
// Device Control Register Bits Description:
//    HEIGHT               [15:00]   Vertical Height 
//    WIDTH                [31:16]   Horizontal Width 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_WOI2_SRC_RES                                      (APEX_642CL_HRSZ_CORE_1_WOI2_SRC_RES_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_WOI2_SRC_RES_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_WOI2_SRC_RES_BASE_ADDRESS                         (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_WOI2_SRC_RES_OFFSET                               (0x0000005c)
#define APEX_642CL_HRSZ_CORE_1_WOI2_SRC_RES_DEFAULT                              (0x00000000)


union _APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_TAG
{
   struct _asApex642clHrszCoreWoi2SrcResBitFieldTag
   {
      REG32  Height               : 16; //  15:00  APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_BIT_HEIGHT
      REG32  Width                : 16; //  31:16  APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_BIT_WIDTH
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_TAG                             APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_DCR;

extern volatile APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_DCR                            gApex642clHrszCoreWoi2SrcRes;
extern volatile APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_DCR*                           gpApex642clHrszCoreWoi2SrcRes;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_BIT_HEIGHT                             0
#define APEX_642CL_HRSZ_CORE_WOI2_SRC_RES_BIT_WIDTH                              16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI2_CROP_0
//    APEX_642CL_HRSZ_CORE_WOI2_CROP_0
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI2_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI2_CROP_0_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000060
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 2's CROP Start Coordinate
// 
// Device Control Register Bits Description:
//    V_OFFSET             [15:00]   Vertical Offset 
//    H_OFFSET             [31:16]   Horizontal Offset 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_0                                       (APEX_642CL_HRSZ_CORE_1_WOI2_CROP_0_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_WOI2_CROP_0_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_0_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_0_OFFSET                                (0x00000060)
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_0_DEFAULT                               (0x00000000)


union _APEX_642CL_HRSZ_CORE_WOI2_CROP_0_TAG
{
   struct _asApex642clHrszCoreWoi2Crop0BitFieldTag
   {
      REG32  VOffset              : 16; //  15:00  APEX_642CL_HRSZ_CORE_WOI2_CROP_0_BIT_V_OFFSET
      REG32  HOffset              : 16; //  31:16  APEX_642CL_HRSZ_CORE_WOI2_CROP_0_BIT_H_OFFSET
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_WOI2_CROP_0_TAG                              APEX_642CL_HRSZ_CORE_WOI2_CROP_0_DCR;

extern volatile APEX_642CL_HRSZ_CORE_WOI2_CROP_0_DCR                             gApex642clHrszCoreWoi2Crop0;
extern volatile APEX_642CL_HRSZ_CORE_WOI2_CROP_0_DCR*                            gpApex642clHrszCoreWoi2Crop0;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_WOI2_CROP_0_BIT_V_OFFSET                            0
#define APEX_642CL_HRSZ_CORE_WOI2_CROP_0_BIT_H_OFFSET                            16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    WOI2_CROP_1
//    APEX_642CL_HRSZ_CORE_WOI2_CROP_1
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_WOI2_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_WOI2_CROP_1_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000064
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    WOI 2's CROP Resolution (Destination Resolution)
// 
// Device Control Register Bits Description:
//    V_VALID              [15:00]   Vertical Size 
//    H_VALID              [31:16]   Horizontal Size 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_1                                       (APEX_642CL_HRSZ_CORE_1_WOI2_CROP_1_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_WOI2_CROP_1_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_1_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_1_OFFSET                                (0x00000064)
#define APEX_642CL_HRSZ_CORE_1_WOI2_CROP_1_DEFAULT                               (0x00000000)


union _APEX_642CL_HRSZ_CORE_WOI2_CROP_1_TAG
{
   struct _asApex642clHrszCoreWoi2Crop1BitFieldTag
   {
      REG32  VValid               : 16; //  15:00  APEX_642CL_HRSZ_CORE_WOI2_CROP_1_BIT_V_VALID
      REG32  HValid               : 16; //  31:16  APEX_642CL_HRSZ_CORE_WOI2_CROP_1_BIT_H_VALID
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_WOI2_CROP_1_TAG                              APEX_642CL_HRSZ_CORE_WOI2_CROP_1_DCR;

extern volatile APEX_642CL_HRSZ_CORE_WOI2_CROP_1_DCR                             gApex642clHrszCoreWoi2Crop1;
extern volatile APEX_642CL_HRSZ_CORE_WOI2_CROP_1_DCR*                            gpApex642clHrszCoreWoi2Crop1;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_WOI2_CROP_1_BIT_V_VALID                             0
#define APEX_642CL_HRSZ_CORE_WOI2_CROP_1_BIT_H_VALID                             16

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IRQ_STATUS
//    APEX_642CL_HRSZ_CORE_IRQ_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_IRQ_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_IRQ_STATUS_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000070
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Status
// 
// Device Control Register Bits Description:
//    DONE                 [00]      Scaler Done 
//    IFIFO_RD_ERROR       [01]      IFIFO Read Error 
//    IFIFO_WR_ERROR       [02]      IFIFO Write Error 
//    OFIFO_WR_ERROR       [03]      OFIFO Read Error 
//    OFIFO_RD_ERROR       [04]      OFIFO Write Error 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_IRQ_STATUS                                        (APEX_642CL_HRSZ_CORE_1_IRQ_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_IRQ_STATUS_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_IRQ_STATUS_BASE_ADDRESS                           (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_IRQ_STATUS_OFFSET                                 (0x00000070)
#define APEX_642CL_HRSZ_CORE_1_IRQ_STATUS_DEFAULT                                (0x00000000)


union _APEX_642CL_HRSZ_CORE_IRQ_STATUS_TAG
{
   struct _asApex642clHrszCoreIrqStatusBitFieldTag
   {
      REG32  Done                 :  1; //  00     APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_DONE
      REG32  IfifoRdError         :  1; //  01     APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_IFIFO_RD_ERROR
      REG32  IfifoWrError         :  1; //  02     APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_IFIFO_WR_ERROR
      REG32  OfifoWrError         :  1; //  03     APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_OFIFO_WR_ERROR
      REG32  OfifoRdError         :  1; //  04     APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_OFIFO_RD_ERROR
      REG32  Reserved             : 27; //  31:05  APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_IRQ_STATUS_TAG                               APEX_642CL_HRSZ_CORE_IRQ_STATUS_DCR;

extern volatile APEX_642CL_HRSZ_CORE_IRQ_STATUS_DCR                              gApex642clHrszCoreIrqStatus;
extern volatile APEX_642CL_HRSZ_CORE_IRQ_STATUS_DCR*                             gpApex642clHrszCoreIrqStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_DONE                                 0
#define APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_IFIFO_RD_ERROR                       1
#define APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_IFIFO_WR_ERROR                       2
#define APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_OFIFO_WR_ERROR                       3
#define APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_OFIFO_RD_ERROR                       4
#define APEX_642CL_HRSZ_CORE_IRQ_STATUS_BIT_RESERVED                             5

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IRQ_MASK
//    APEX_642CL_HRSZ_CORE_IRQ_MASK
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_IRQ_MASK_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_IRQ_MASK_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000074
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Mask.
//    Write 0x0 to respective bits to enable IRQ generation.
//    
// 
// Device Control Register Bits Description:
//    DONE                 [00]      Scaler Done 
//    IFIFO_RD_ERROR       [01]      IFIFO Read Error 
//    IFIFO_WR_ERROR       [02]      IFIFO Write Error 
//    OFIFO_WR_ERROR       [03]      OFIFO Read Error 
//    OFIFO_RD_ERROR       [04]      OFIFO Write Error 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_IRQ_MASK                                          (APEX_642CL_HRSZ_CORE_1_IRQ_MASK_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_IRQ_MASK_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_IRQ_MASK_BASE_ADDRESS                             (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_IRQ_MASK_OFFSET                                   (0x00000074)
#define APEX_642CL_HRSZ_CORE_1_IRQ_MASK_DEFAULT                                  (0x00000000)


union _APEX_642CL_HRSZ_CORE_IRQ_MASK_TAG
{
   struct _asApex642clHrszCoreIrqMaskBitFieldTag
   {
      REG32  Done                 :  1; //  00     APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_DONE
      REG32  IfifoRdError         :  1; //  01     APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_IFIFO_RD_ERROR
      REG32  IfifoWrError         :  1; //  02     APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_IFIFO_WR_ERROR
      REG32  OfifoWrError         :  1; //  03     APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_OFIFO_WR_ERROR
      REG32  OfifoRdError         :  1; //  04     APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_OFIFO_RD_ERROR
      REG32  Reserved             : 27; //  31:05  APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_IRQ_MASK_TAG                                 APEX_642CL_HRSZ_CORE_IRQ_MASK_DCR;

extern volatile APEX_642CL_HRSZ_CORE_IRQ_MASK_DCR                                gApex642clHrszCoreIrqMask;
extern volatile APEX_642CL_HRSZ_CORE_IRQ_MASK_DCR*                               gpApex642clHrszCoreIrqMask;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_DONE                                   0
#define APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_IFIFO_RD_ERROR                         1
#define APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_IFIFO_WR_ERROR                         2
#define APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_OFIFO_WR_ERROR                         3
#define APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_OFIFO_RD_ERROR                         4
#define APEX_642CL_HRSZ_CORE_IRQ_MASK_BIT_RESERVED                               5

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    IRQ_CLEAR
//    APEX_642CL_HRSZ_CORE_IRQ_CLEAR
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_IRQ_CLEAR_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h00000078
//    Default:      32'h00000000
//    Type:         wo
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Clear
//    Write 0x1 to clear respective status.
//    
// 
// Device Control Register Bits Description:
//    DONE                 [00]      Scaler Done 
//    IFIFO_RD_ERROR       [01]      IFIFO Read Error 
//    IFIFO_WR_ERROR       [02]      IFIFO Write Error 
//    OFIFO_WR_ERROR       [03]      OFIFO Read Error 
//    OFIFO_RD_ERROR       [04]      OFIFO Write Error 
//    RESERVED             [31:05]   Reserved 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_IRQ_CLEAR                                         (APEX_642CL_HRSZ_CORE_1_IRQ_CLEAR_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_IRQ_CLEAR_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_IRQ_CLEAR_BASE_ADDRESS                            (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_IRQ_CLEAR_OFFSET                                  (0x00000078)
#define APEX_642CL_HRSZ_CORE_1_IRQ_CLEAR_DEFAULT                                 (0x00000000)


union _APEX_642CL_HRSZ_CORE_IRQ_CLEAR_TAG
{
   struct _asApex642clHrszCoreIrqClearBitFieldTag
   {
      REG32  Done                 :  1; //  00     APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_DONE
      REG32  IfifoRdError         :  1; //  01     APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_IFIFO_RD_ERROR
      REG32  IfifoWrError         :  1; //  02     APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_IFIFO_WR_ERROR
      REG32  OfifoWrError         :  1; //  03     APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_OFIFO_WR_ERROR
      REG32  OfifoRdError         :  1; //  04     APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_OFIFO_RD_ERROR
      REG32  Reserved             : 27; //  31:05  APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_RESERVED
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_IRQ_CLEAR_TAG                                APEX_642CL_HRSZ_CORE_IRQ_CLEAR_DCR;

extern volatile APEX_642CL_HRSZ_CORE_IRQ_CLEAR_DCR                               gApex642clHrszCoreIrqClear;
extern volatile APEX_642CL_HRSZ_CORE_IRQ_CLEAR_DCR*                              gpApex642clHrszCoreIrqClear;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_DONE                                  0
#define APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_IFIFO_RD_ERROR                        1
#define APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_IFIFO_WR_ERROR                        2
#define APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_OFIFO_WR_ERROR                        3
#define APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_OFIFO_RD_ERROR                        4
#define APEX_642CL_HRSZ_CORE_IRQ_CLEAR_BIT_RESERVED                              5

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    FIFO_STATUS
//    APEX_642CL_HRSZ_CORE_FIFO_STATUS
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_CORE_FIFO_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_FIFO_STATUS_OFFSET
//    Base Addr:    APEX_642CL_HRSZ_CORE
//    Offset:       32'h0000007c
//    Default:      32'h00000000
//    Type:         ro
//    Nb:         1
//    Size:       4
// 
// Description:
//    IRQ Clear
//    Write 0x1 to clear respective status.
// 
// Device Control Register Bits Description:
//    OFIFO_RD_BYTE_CNT    [09:00]   Output FIFO Read Byte Count 
//    RESERVED_0           [13:10]   Reserved 
//    OFIFO_FULL           [14]      Output FIFO Full Status 
//    OFIFO_EMPTY          [15]      Output FIFO Empty Status 
//    IFIFO_WR_BYTE_CNT    [25:16]   Input FIFO Write Byte Count 
//    RESERVED_1           [29:26]   Reserved 
//    IFIFO_FULL           [30]      Input FIFO Full Status 
//    IFIFO_EMPTY          [31]      Input FIFO Empty Status 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_1_FIFO_STATUS                                       (APEX_642CL_HRSZ_CORE_1_FIFO_STATUS_BASE_ADDRESS + APEX_642CL_HRSZ_CORE_1_FIFO_STATUS_OFFSET)
#define APEX_642CL_HRSZ_CORE_1_FIFO_STATUS_BASE_ADDRESS                          (APEX_642CL_HRSZ_CORE_1)
#define APEX_642CL_HRSZ_CORE_1_FIFO_STATUS_OFFSET                                (0x0000007c)
#define APEX_642CL_HRSZ_CORE_1_FIFO_STATUS_DEFAULT                               (0x00000000)


union _APEX_642CL_HRSZ_CORE_FIFO_STATUS_TAG
{
   struct _asApex642clHrszCoreFifoStatusBitFieldTag
   {
      REG32  OfifoRdByteCnt       : 10; //  09:00  APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_OFIFO_RD_BYTE_CNT
      REG32  Reserved0            :  4; //  13:10  APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_RESERVED_0
      REG32  OfifoFull            :  1; //  14     APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_OFIFO_FULL
      REG32  OfifoEmpty           :  1; //  15     APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_OFIFO_EMPTY
      REG32  IfifoWrByteCnt       : 10; //  25:16  APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_IFIFO_WR_BYTE_CNT
      REG32  Reserved1            :  4; //  29:26  APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_RESERVED_1
      REG32  IfifoFull            :  1; //  30     APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_IFIFO_FULL
      REG32  IfifoEmpty           :  1; //  31     APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_IFIFO_EMPTY
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_CORE_FIFO_STATUS_TAG                              APEX_642CL_HRSZ_CORE_FIFO_STATUS_DCR;

extern volatile APEX_642CL_HRSZ_CORE_FIFO_STATUS_DCR                             gApex642clHrszCoreFifoStatus;
extern volatile APEX_642CL_HRSZ_CORE_FIFO_STATUS_DCR*                            gpApex642clHrszCoreFifoStatus;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_OFIFO_RD_BYTE_CNT                   0
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_RESERVED_0                          10
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_OFIFO_FULL                          14
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_OFIFO_EMPTY                         15
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_IFIFO_WR_BYTE_CNT                   16
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_RESERVED_1                          26
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_IFIFO_FULL                          30
#define APEX_642CL_HRSZ_CORE_FIFO_STATUS_BIT_IFIFO_EMPTY                         31

/////////////////////////////////////////////////////////////////////////////////////////////
// Device Control Register Name:
//    PUSH_POP_CORE
//    APEX_642CL_HRSZ_PUSH_POP_CORE
// 
// Device Control Register Parameters:
//    Addr:         APEX_642CL_HRSZ_PUSH_POP_CORE_BASE_ADDRESS + APEX_642CL_HRSZ_PUSH_POP_CORE_OFFSET
//    Base Addr:    APEX_642CL_HRSZ
//    Offset:       32'h00000100
//    Default:      32'h00000000
//    Type:         rw
//    Nb:         2
//    Size:       128
// 
// Description:
//    Core Manual FIFO Access
//    
//    Writing to this range of 32 registers will PUSH data into the Input FIFO of Core.
//    Reading from this range of 32 registers will POP data from the Output FIFO of Core.
// 
// Device Control Register Bits Description:
//    FIFO                 [31:00]   Push/Pop Input/Output FIFO respectively 
// 
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_PUSH_POP_CORE_0                                          (APEX_642CL_HRSZ_PUSH_POP_CORE_0_BASE_ADDRESS + APEX_642CL_HRSZ_PUSH_POP_CORE_0_OFFSET)
#define APEX_642CL_HRSZ_PUSH_POP_CORE_0_BASE_ADDRESS                             (APEX_642CL_HRSZ_BASE_ADDRESS)
#define APEX_642CL_HRSZ_PUSH_POP_CORE_0_OFFSET                                   (0x00000100)
#define APEX_642CL_HRSZ_PUSH_POP_CORE_0_DEFAULT                                  (0x00000000)

#define APEX_642CL_HRSZ_PUSH_POP_CORE_1                                          (APEX_642CL_HRSZ_PUSH_POP_CORE_1_BASE_ADDRESS + APEX_642CL_HRSZ_PUSH_POP_CORE_1_OFFSET)
#define APEX_642CL_HRSZ_PUSH_POP_CORE_1_BASE_ADDRESS                             (APEX_642CL_HRSZ_BASE_ADDRESS)
#define APEX_642CL_HRSZ_PUSH_POP_CORE_1_OFFSET                                   (0x00000180)
#define APEX_642CL_HRSZ_PUSH_POP_CORE_1_DEFAULT                                  (0x00000000)


union _APEX_642CL_HRSZ_PUSH_POP_CORE_TAG
{
   struct _asApex642clHrszPushPopCoreBitFieldTag
   {
      REG32  Fifo                 : 32; //  31:00  APEX_642CL_HRSZ_PUSH_POP_CORE_BIT_FIFO
   } asBitField;
   
   REG32 as32BitValue;
};

typedef union _APEX_642CL_HRSZ_PUSH_POP_CORE_TAG                                 APEX_642CL_HRSZ_PUSH_POP_CORE_DCR;

extern volatile APEX_642CL_HRSZ_PUSH_POP_CORE_DCR                                gApex642clHrszPushPopCore;
extern volatile APEX_642CL_HRSZ_PUSH_POP_CORE_DCR*                               gpApex642clHrszPushPopCore;

// Bits and Id's defines
/////////////////////////////////////////////////////////////////////////////////////////////
#define APEX_642CL_HRSZ_PUSH_POP_CORE_BIT_FIFO                                   0

#endif /* APEX642CLHRSZMAPTYPE_H */
