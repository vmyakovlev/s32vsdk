/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2011-2015 All right reserved.
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

#ifndef CMEMARRAYMAP_H
#define CMEMARRAYMAP_H

// Literal definitions

#define NUMBER_CUS_VERT           1                                    // for MV8750 (Maple 3)
#define NUMBER_CUS_HORIZ          32                                   // Tyche implementation, 32 Cu per APU
#define MEMORY_PER_CU             (int)(1024 * 4)                      // In Byte (Restricted to 48 CU (FPGA restriction) <- ???)
#define MEMORY_PER_CU_WORDS       ((MEMORY_PER_CU) >> 1)               // 2048 16-bit words
#define APEX2_CMEM_SIZE_IN_BYTE   ((MEMORY_PER_CU)*(NUMBER_CUS_HORIZ)) // In Byte

// #endif
#define APU_0_DMEM_SIZE                 0x008000

#define APU_HOST_DMEM_OFFSET_BASE       0x000000   //0x400000
#define APU_0_DMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x000000)
#define APU_1_DMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x010000)   //0x040000
#define APU_2_DMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x020000)   //0x080000
#define APU_3_DMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x030000)   //0x0C0000

#define APU_0_IMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x008000)   //0x020000
#define APU_1_IMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x018000)   //0x060000
#define APU_2_IMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x028000)   //0x0A0000
#define APU_3_IMEM_OFFSET               (APU_HOST_DMEM_OFFSET_BASE + 0x038000)   //0x0E0000

#define APU_0_VMEM_OFFSET               0x000000
#define APU_1_VMEM_OFFSET               0x020000
#define APU_2_VMEM_OFFSET               0x040000
#define APU_3_VMEM_OFFSET               0x060000

#define APU_VMEM_SLICE_OFFSET           0x080000
#define APU_VMEM_SLICE_INCREMENT        0x020000
#define APU_0_1_VMEM_SLICE_OFFSET       0x100000

#define APU_VMEM_BROADCAST_OFFSET       0x300000
#define APU_VMEM_BROADCAST_INCREMENT    0x020000
#define APU_ALL_VMEM_BROADCAST_OFFSET   0x340000 //0x380000

#define CU_PER_GROUP                    8 // CMEM interface is 64 bits for MV8750 (Maple 3)

#define MAX_CMEM_ROWS                   NUMBER_CUS_VERT
#define MAX_CMEM_COLUMNS                (NUMBER_CUS_HORIZ / CU_PER_GROUP)
#define MAX_CMEM_GROUPS                 (NUMBER_CUS_HORIZ / CU_PER_GROUP)

// For Maple 3 (MV8750) a group of CUs contains 8 CUS instead of 4
#define CU_GROUP_LOW_WORD    0
#define CU_GROUP_HIGH_WORD   1
//
// Map Definitions.
//

// For CV230x a group of CUs contains 8 16-bit CUS
// ------------------------------------------------------------------------
#define CU_GROUP_LL_WORD   0
#define CU_GROUP_L_WORD    1
#define CU_GROUP_H_WORD    2
#define CU_GROUP_HH_WORD   3

#define CU_GROUP_WIDTH_INTS    4  // 4  32-bit words (RISC) per 128-bit GOC
#define CU_GROUP_WIDTH_BYTES   16 // 16 8-bit bytess (RISC) per 128-bit GOC

typedef volatile unsigned int CMEM_ENTRY[CU_GROUP_WIDTH_INTS];

typedef CMEM_ENTRY CMEM_COLUMN[MEMORY_PER_CU / 2];

// Second map to use when calculating actual physical map location of CUs
typedef  CMEM_COLUMN CMEM_PHYSICAL_ROW[MAX_CMEM_GROUPS];       // Horizontal CMEM_COL
typedef  CMEM_PHYSICAL_ROW CMEM_PHYSICAL_ARRAY[MAX_CMEM_ROWS]; // Vertical (not used anymore)
typedef  CMEM_PHYSICAL_ARRAY CMEM_ARRAY_MAP;

//
// export reference to the physical hardware location
//
extern CMEM_ARRAY_MAP *gpCmemArrayMap;
#endif /* CMEMARRAYMAP_H */

