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

#ifndef APUDMEMFIFOHAL_H
#define APUDMEMFIFOHAL_H

#include <stdint.h>


typedef enum e_DMEM_FIFO_MODE
{
   DMEM_FIFO_IN   = 0,
   DMEM_FIFO_OUT  = 1
} DMEM_FIFO_MODE;

typedef enum e_DMEM_FIFO_WORDSIZE
{
   DMEM_FIFO_W32     = 2,
   DMEM_FIFO_W64     = 3,
   DMEM_FIFO_W128    = 4
} DMEM_FIFO_WORDSIZE;


#define MAX_BUFFER_NUM        16
//#define DMEMFIFO_NUMBUFF(CONFIG_REG_32)
// HAL CONFIG: SINGLE 32-BIT WRITE
int apu_dmemfifo_hal_config(  int32_t buffer_size,   int enable_addr_update,
                              int word_size,          int num_buffers, const int32_t* buff_addr,
                              const int fifo_mode);
// HAL: ONLY FOR START ADDRESS UPDATE     
int apu_dmemfifo_hal_configure(  int32_t buff_size,   int32_t enable_addr_update,
                                 int32_t word_size,   int32_t num_buffers,
                                 const int32_t fifo_mode);                        
int apu_dmemfifo_hal_buff_start(int start_buff_index, int num_buffers, const int32_t* buff_addr, const int fifo_mode);
// FIFO CLEAR
int apu_dmemfifo_hal_clr(int fifo_mode);
// READ BUFFER STATUS
int apu_dmemfifo_hal_status( int32_t* fifo_status, int fifo_mode);

// EMPTY FOR NOW: DONE ON ARM??
int apu_dmemfifo_hal_reset();

// methods related to performance counter (apu only)
void apu_dmemfifo_hal_reset_perf_counter();
int apu_dmemfifo_hal_read_perf_counter();


#endif /* APUDMEMFIFOHAL_H */
