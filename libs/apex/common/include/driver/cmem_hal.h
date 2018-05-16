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

#ifndef CMEMHAL_H
#define CMEMHAL_H

// Include files
#include <cmem_array_map.h>
#include <stdint.h>

// Local Literals and Definitions

// These functions came from Simd.c.
// Most of them are unused, but have been made macros just in case.
// TODO: They should be REMOVED in the future.
#define GetNumCus(void)       (NUMBER_CUS_VERT * NUMBER_CUS_HORIZ)
#define GetArrayWidth(void)   (NUMBER_CUS_HORIZ)
#define GetArrayHeight(void)  (NUMBER_CUS_VERT)
#define GetMemPerCUSize(void) (MEMORY_PER_CU)
#define InitSIMD(void)        (CmemInit(void))

void cmem_hal_write_4bytes4cus(unsigned int startCu, unsigned int cmemAddr, unsigned char data[4]);
void cmem_hal_read_4bytes4cus (unsigned int startCu, unsigned int cmemAddr, unsigned char data[4]);
void cmem_hal_broadcast_1byteAllCus (unsigned int cmemAddr, unsigned char val);

//for backwards compatibility and verification purposes (will be removed soon)
void     cmemarray_hal_write_mem32(unsigned int cuColumnIndex, unsigned int memoryOffset, uint32_t value);
uint32_t cmemarray_hal_read_mem32 (unsigned int cuColumnIndex, unsigned int memoryOffset);

#if 0 //TODO: everything below is unverified HAL code ... cleanup and verification required!
void         cmemarray_hal_init(void);
void         cmemarray_hal_write_mem32(unsigned int cuColumnIndex, unsigned int memoryOffset, uint32_t value);
uint32_t     cmemarray_hal_read_mem32(unsigned int cuColumnIndex, unsigned int memoryOffset);
void         cmemarray_hal_broadcast_mem8 (unsigned int memoryOffset, uint8_t value);
void         cmemarray_hal_broadcast_mem32(unsigned int memoryOffset, uint32_t value);

// support for older interface
#define WriteMem32(src, cunumY, value) Cmemarray_hal_write_mem32(cunumY, src, value)
#define ReadMem32(src, cunumY)         Cmemarray_hal_read_mem32(cunumY, src)

void Cmemarray_hal_snapshot_read(unsigned int address, unsigned int size, unsigned int ngoc, const char *filename);
int  Cmemarray_hal_snapshot_comp(unsigned int address, unsigned int size, unsigned int ngoc, const unsigned int *data);

// These functions came from test_cram.c.
void Cmemarray_hal_put_data_pattern(int cuMemOffset, int initialValue);
void Cmemarray_hal_put_data_constant(int cuMemOffset, int initialValue);

LIB_RESULT Cmemarray_hal_put_test_data(int cuMemOffset, unsigned int data[]);
LIB_RESULT Cmemarray_hal_get_test_data(int cuMemOffset, unsigned int data[]);
LIB_RESULT Cmemarray_hal_compare_test_data(int cuMemOffset, unsigned int data[]);

// From SCM.C
volatile unsigned int *Cmemarray_hal_calculate_cu_address (unsigned int columnCu, unsigned int cmemOffset);
#define Cmemarray_hal_calculate_physical_cu_address(columnCu, cmemOffset) Cmemarray_hal_calculate_cu_address(columnCu, cmemOffset)

#endif //if 0

#endif /* CMEMHAL_H */
