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

#ifndef APEXAPUMICROKERNELHAL_H
#define APEXAPUMICROKERNELHAL_H

#include <stdint.h>
// COMPILE FLAG
#define COMPILE_FOR_SIM    0
#define USE_DMEM_FIFO      1        // NOT USED ANYMORE????
#define USE_FUNC_PTR       1

//
typedef enum 
{
   HOST_TO_APU_MEM,
   APU_TO_APU_MEM,
   HOST_TO_APU_FIFO
} APUMK_QUE_MODE;


typedef enum e_DMEM_FIFO_TRANSFER
{
   DMA_XMEM_TO_FIFO     = 0,
   DMA_FIFO_TO_XMEM     = 1,
   DMA_FIFO_TO_FIFO     = 2
} DMEM_FIFO_TRANSFER;

//////////////////////////////////////////////////
//             MEMORY MAP
//////////////////////////////////////////////////

// SEQ PROCESSOR ID: reserved by apu microkernel!!!!!!
#define SEQ_APU0_PROC_ID               0


// [APU MEM MAP]
// SEQ REGISTER BASE ADDR (APU)
//#define APU_DMEM_SEQ_REG_BASE          0x00130000  //0x30030000
 
// 0x10000, DMEM fifo: 0x1_0000 - 0x1_0800
// DMEM FIFO BASE ADDR (APU)
#define DMEM_FIFO_REG_BASE             0x00160000  //0x30060000

//////////////////////////////////////////////////
//             CONFIGURATION
//////////////////////////////////////////////////
// [MICROKERNEL] 
// DMEM FIFO BUFFER START OFFSET
#define APU_DMEM_OFFSET_BASE           0x00000000  //0x00400000


// [APU LOADER]
// PMEM LOAD START ADDR (CONFIGURABLE)
// ACP:  WORD ADDR
// HOST: BYTE ADDR
#define PMEM_APU_START_ADDR            0x2000

#define DMEM_SHARED_OFFSET             0x600                         // MATCH WITH BCF
#define DMEM_CMD_CNT_OFFSET            (DMEM_SHARED_OFFSET + 0x20)   // MATCH WITH BCF
#define DMEM_CMD_QUE_OFFSET            (DMEM_SHARED_OFFSET + 0x40)   // MATCH WITH BCF
#define DMEM_SHARED_END                (DMEM_SHARED_OFFSET + 0xC00) // MATCH WITH BCF

// RESERVED MEMORY ADDR FOR MK
// MATCH WITH LOAD
#define DMEM_APUMK_STACK_OFFSET        0x0000      // 512

// [MICROKERNEL]
// VIRTUAL MEMORY (NOT TAKE SPACE)
#define DMEM_KERNEL_DB_OFFSET          0x10800     //
#define DMEM_CL_DB_OFFSET              0x10D00     //
#define DMEM_CL_MD_OFFSET              0x11144//0x10F44     //
#define DMEM_FIFO_OFFSET               (0x00010000)


// [MICROKERNEL]
// DMEM COMMAND QUEUE MEMORY MAP:
// IF CHANGE, REBUILD MICROKERNEL !!!!!
#define CONFIG_QUE_BUF_SIZ       0x0100                                          // in bytes
#define CONFIG_QUE_BUF_SIZ32     (CONFIG_QUE_BUF_SIZ >> 2)                       // in words
#define CONFIG_QUE_BUF_NUM       0x0004
#define CONFIG_QUE_BUF_MSK       (CONFIG_QUE_BUF_NUM -1)
#define QUE_ADDR_MASK_32         (CONFIG_QUE_BUF_NUM*(CONFIG_QUE_BUF_SIZ32)-1)  // power of 2
#define QUE_MEM_SIZ              (CONFIG_QUE_BUF_SIZ*CONFIG_QUE_BUF_NUM)        // BUF_SIZ * BUF_NUM <= QUE_MEM_SIZ

// cxu: SHARED DMEM divided into message queue and command queue
// size: in WORDs
// entire shared memory:            640  32-bit
// message queue size:              8     32-bit
// command queue counter size:      8     32-bit
// command queue size:              624  32-bit
#define APU_DMEM_SHARED_TOTAL    ((DMEM_SHARED_END-DMEM_SHARED_OFFSET) >> 2)
#define APU_DMEM_COM_SIZE        8
#define APU_DMEM_CMD_CNT_SIZE    8
#define APU_DMEM_CMD_QUE_SIZE    (APU_DMEM_SHARED_TOTAL - APU_DMEM_COM_SIZE - APU_DMEM_CMD_CNT_SIZE)
#define APU_DMEM_DEBUG_SIZE      32

//////////////////////////////////////////////////
//             COMMAND DEFINITION
//////////////////////////////////////////////////
// APU_W0: ( (command_type << 0) | (number of parameters << 2) | (function pointer << 16) )
// SIZE
#define CMD_TYPE_BITS                  2
#define CMD_TYPE_MASK                  ( (1<<CMD_TYPE_BITS) - 1)

#define NUL_COMMAND                    0
#define APU_COMMAND                    1
#define SEQ_COMMAND                    2
#define SIZ_COMMAND                    3

// USE BY APU MK LIB
// NUMBER OF PARAMETERS PER COMMAND = NUM_PARAM_MASK
// supported number of parameters/data pointers: [0 ~ 15]
// NUM_PARAM_BITS MUST BE <= 14 bits
#define NUM_PARAM_BITS                 10
#define MAX_NUM_PARAM                  750 //must be < (1<<NUM_PARAM_BITS)!!
#define NUM_PARAM_MASK                 ((1<<NUM_PARAM_BITS) - 1)
// FLAG FOR 32-BIT PARAMETER SUPPORT, ELSE 16-BIT
#define FLAG_PARAM_32BIT               1
#if (FLAG_PARAM_32BIT   == 1)
#define SIZ_CMD_ARRAY(num_param)     (1 + num_param)
#else
#define SIZ_CMD_ARRAY(num_param)     (1 + ((num_param+1)>>1))
#endif

// SEQ_RELATED
// SEQ: ( (command_type << 0) | (dec/inc << 2) | (set_index << 4) )
#define SEQ_CMD_TYPE_BITS              2
#define SEQ_CMD_TYPE_MASK              ( (1<<SEQ_CMD_TYPE_BITS) - 1)
#define SET_INDEX_START                (SEQ_CMD_TYPE_BITS+CMD_TYPE_BITS)
#define SEQ_CMD_DEC                    1
#define SEQ_CMD_INC                    2

#define SEQ_CMD_WORDS                  2

#define APU_QUE_RETURN_SIZE(cmd)       (((unsigned)cmd) >> CMD_TYPE_BITS )
#define APU_QUE_RETURN_TOTALSIZE(cmd)  (( ((unsigned)cmd) >> CMD_TYPE_BITS ) + 1)


//////////////////////////////////////////////////
//            APU MK HAL COMMUNICATION
//////////////////////////////////////////////////
// HOST LIB
#define HDR_POS                           28
#define HDR_MASK                          0x0F
#define MSG_MASK                          0xFFFFFFF
#define HDR_HOST_WRITE                    (1<<0)
#define HDR_HOST_DMA                      (1<<1)
#define HDR_APU                           (3)

#define HOST_MSG_SENDCMD                  COMM_MSG(HDR_HOST_WRITE, 0)
#define HOST_MSG_TRANSFERLIST(num_buf)    COMM_MSG(HDR_HOST_DMA,  num_buf)
#define APU_MSG_DONE(num)                 COMM_MSG(HDR_APU,       num)
#define COMM_MSG(hdr, msg)                (((hdr&HDR_MASK)<<28) | (msg & MSG_MASK))
#define RET_VAL(msg)                      ((msg >> 0       ) & MSG_MASK)
#define RET_HDR(msg)                      ((msg >> HDR_POS ) & HDR_MASK)

#ifdef __cplusplus
extern "C" {
#endif

int apumk_hal_host_com_rx(int apexID, int apuid, int32_t* msg, int timeoutInUs = 500000);
int apumk_hal_host_com_tx(int apexID, int apuid, int32_t  msg, int timeoutInUs = 500000);

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////
//            APU MK HAL COMMAND QUEUE GENERATION
//////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

// APU MK HAL  COMMAND
// APU LIB
// pre-compile a command list
void  apumk_hal_dmem_init     ( int apexID, int apuid, const int que_mode);
int   apumk_hal_push_cmd      ( int32_t* list, int kernel_id, int num_params, const int32_t* param_list);
int   apumk_hal_seq_push_wait ( int32_t* list, int res_set, int processor_id);
int   apumk_hal_seq_push_done ( int32_t* list, int res_set, int processor_id);
int   apumk_hal_push_align(int32_t* list);

// push the pre-compiled list into apu command queue (not start yet)
// Host write list
int   apumk_hal_send_list(int apexID, int32_t apu_id,
                          int32_t* list);
// Host write one single command
int   apumk_hal_send_cmd(int apexID, int32_t apu_id,
                         int32_t kernel_id,
                         int32_t num_params,
                         const int32_t* param_list);

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////
//            APU MK HAL DMEM FIFO COMMAND QUEUE
//////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif


int   apumk_hal_config_dmemfifo_config(int apexID, int ch, int xmem_addr,
                                       int transfer_mode);
int   apumk_hal_config_dmemfifo_dmaconfig(int apexID, int ch, int xmem_addr,
                                          int transfer_size,
                                          int transfer_mode);
// Host transfer list through DMA
int   apumk_hal_transfer_list (int apexID, int ch);
void  apumk_hal_wait_list     (int apexID, int ch);
void  apumk_hal_disable       (int apexID, int ch);

#ifdef __cplusplus
}
#endif

#endif /* APEXAPUMICROKERNELHAL_H */
