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

#ifndef QSPIDRIVER_H
#define QSPIDRIVER_H

#include "s32vs234.h"

#define QuadSPI_X QuadSPI_0
#define ARDB (*(volatile unsigned int *) 0x71000000)
#define FLASH_BASE_ADR	0x20000000
#define FLASH_BASE_ADR2	0x24000000

#define HYPERFLASH


void QSPI_setup_hyp(void);
unsigned int quadspi_status_hyp(void);
void quadspi_read_ip_hyp(unsigned long address, unsigned long *dest, unsigned long bytes);
void quadspi_program_word_hyp(unsigned int address, unsigned int word);
void quadspi_program_hyp(unsigned int address, unsigned int *data, unsigned int bytes);
void quadspi_program_dma_hyp(unsigned int address, unsigned int *data, unsigned int bytes);
void quadspi_read_hyp(void);
void quadspi_erase_hyp(int sector);
void quadspi_id_entry_hyp(void);
void quadspi_cfi_entry_hyp(void);
void quadspi_send_instruction_hyp(unsigned int address, unsigned int cmd);


////////////////// HELPER Functions ////////////////////////////////
void quadspi_set_lut(uint32_t index, uint32_t value);
void dma_copy(unsigned int src, unsigned int dst, unsigned int nbytes, unsigned int tsize);
void dma_wait(void);


/////////////////required defines to compile //////////////////////////////////
#define BURST_SIZE 0x80 //max 256bytes!
#define FLASH_PGSZ	(128)
#define FLASH_DMA_PGSZ (512)

/* QUADSPI Instructions */
#define CMD 1
#define QSPI_ADDR 2 /* b14716 - renamed as the same is on header file already */
#define DUMMY 3
#define MODE 4
#define MODE2 5
#define MODE4 6
#define READ 7
#define WRITE 8
#define JMP_ON_CS 9
#define ADDR_DDR 10
#define MODE_DDR 11
#define MODE2_DDR 12
#define MODE4_DDR 13
#define READ_DDR 14
#define WRITE_DDR 15
#define DATA_LEARN 16
#define CMD_DDR 17
#define CADDR 18
#define CADDR_DDR 19
#define STOP 0

#define QSPI_LUT(CMD1,PAD1,OP1,CMD0,PAD0,OP0)	((((CMD1)&0x3f)<<26)|(((PAD1)&3)<<24)|(((OP1)&0xff)<<16)|(((CMD0)&0x3f)<<10)|(((PAD0)&3)<<8)|((OP0)&0xff))

#define QuadSPI_SR_BUSY_SHIFT                (0)
#define QuadSPI_SR_BUSY_MASK                 ((1) << (QuadSPI_SR_BUSY_SHIFT))
#define QuadSPI_MCR_CLR_TXF_MASK             0x800u
#define QSPI_SR_RXWE_SHIFT                (16)
#define QSPI_SR_RXWE_MASK                 ((1) << (QSPI_SR_RXWE_SHIFT))
#define QuadSPI_MCR_CLR_TXF_MASK                 0x800u
#define QuadSPI_MCR_CLR_TXF_SHIFT                11
#define QuadSPI_SR_TXFULL_MASK                   0x8000000u
#define QuadSPI_SR_TXFULL_SHIFT                  27


#endif /* QSPIDRIVER_H */
