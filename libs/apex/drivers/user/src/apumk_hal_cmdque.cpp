/******************************************************************************
 *  (C) Copyright CogniVue Corporation. 2015 All right reserved.
 *  
 *  Confidential Information
 *
 *  All parts of the CogniVue Program Source are protected by copyright law
 *  and all rights are reserved.
 *  This documentation may not, in whole or in part, be copied, photocopied,
 *  reproduced, translated, or reduced to any electronic medium or machine
 *  readable form without prior consent, in writing, from CogniVue.
 ******************************************************************************/

//
#include "apu_microkernel_hal.h"
#include "cmem_if_hal.h"
#include <hw_reg_user.h>

#ifdef __KERNEL_MODULE
	
#else
	#include <stdint.h>
#endif


#define APU_COM_WAIT_CC          100

/*
#ifdef __cplusplus
extern "C" {
#endif


// cxu: SHARED DMEM divided into message queue and command queue
// size: in WORDs
// entire shared memory:            2048  32-bit
// message queue size:              8     32-bit
// command queue counter size:      8     32-bit
// command queue size:              2032  32-bit

#define APU_COM_DMEM_SIZE        8
#define APU_DMEM_CMD_CNT_SIZE    8
#define APU_DMEM_CMD_QUE_SIZE    2032
*/
// NEED TO MATCH WITH .bcf!!!!
//#define DMEM_SHARED_OFFSET          0x00006000
//#define DMEM_CMD_CNT_OFFSET         0x00006020
//#define DMEM_CMD_QUE_OFFSET         0x00006040


void
apumk_hal_dmem_init(int apexID, int /*apuid*/, const int que_mode)
{
   uintptr_t base_addr    = 0;
   volatile int* com_addr;
   volatile int* cnt_addr;
   volatile int* que_addr;
   int d;
   
   if (que_mode == HOST_TO_APU_MEM)
   {
      base_addr   = (uintptr_t)ReturnVirtualHwPtr(apexID, HW_REG_SMEM);
   }
   else if (que_mode == APU_TO_APU_MEM)
   {
      base_addr   = 0;
   }
   
   com_addr  = (volatile int*)(base_addr + DMEM_SHARED_OFFSET);
   cnt_addr  = (volatile int*)(base_addr + DMEM_CMD_CNT_OFFSET);
   que_addr  = (volatile int*)(base_addr + DMEM_CMD_QUE_OFFSET);

   for (d=0; d<APU_DMEM_COM_SIZE; ++d)
   {
      com_addr[d] = 0;
   }
   
   for (d=0; d<APU_DMEM_CMD_CNT_SIZE; ++d)
   {
      cnt_addr[d] = 0;
   }

   for (d=0; d<APU_DMEM_CMD_QUE_SIZE; ++d)
   {
      que_addr[d] = 0;
   }
}



/***********************************************************************
// APU PUSH COMMAND
***********************************************************************/
// apu microkernel command queue push
//       ac_hal_PushWaitTbl((int*) lpCmdBlockPtr, lSet);
//       ac_hal_PushCommand(int* list, int cmd, int p0, int p1, int p2, int p3, int p4)
//       ac_hal_PushDoneTbl((int*) lpCmdBlockPtr, lSet);

#define APU_QUE_SIZE(siz)              (( ((unsigned)siz) << CMD_TYPE_BITS ) | SIZ_COMMAND)


#define APU_CMD_W0(num_param, func)    ( ((unsigned) APU_COMMAND        <<  0) | (num_param << CMD_TYPE_BITS) | (func << 16)  )
#if (FLAG_PARAM_32BIT   == 1)
#define APU_CMD_W1(param)              (  param & 0xFFFFFFFF )
#else
#define APU_CMD_W1(param0, param1)     ( ((unsigned) (param0 & 0xFFFF)  << 16) | (param1 & 0xFFFF)                            )
#endif

#define APU_SEQ_DEC(proc_id)           ( ((unsigned) SEQ_COMMAND  <<  0) | (SEQ_CMD_DEC << CMD_TYPE_BITS)   | (proc_id << SET_INDEX_START) )
#define APU_SEQ_INC(proc_id)           ( ((unsigned) SEQ_COMMAND  <<  0) | (SEQ_CMD_INC << CMD_TYPE_BITS)   | (proc_id << SET_INDEX_START) )

//
int apumk_hal_push_cmd(int32_t* list, int kernel_id, int num_params, const int32_t* param_list)
{
   int rval             = 0;
   int cw, id;
   // number of cw currently in the cmd queue
   // list[0] encoded the total cw size
   int total_cw         = APU_QUE_RETURN_SIZE(list[0]);
   int apu_cmw          = SIZ_CMD_ARRAY(num_params);
   list[++total_cw]     = APU_CMD_W0(num_params, kernel_id);
   
#if (FLAG_PARAM_32BIT   == 1)
   for (cw=1, id=0; cw<(apu_cmw); cw++, id++)
   {
      list[++total_cw]   = APU_CMD_W1(param_list[id]);
   }
#else
   for (cw=1, id=0; cw<(apu_cmw); cw++, id+=2)
   {
      list[++total_cw]   = APU_CMD_W1(param_list[id], param_list[id+1]);
   }
#endif
   list[0]   = APU_QUE_SIZE(total_cw);
   rval      = apu_cmw;
   return rval;

}

int apumk_hal_seq_push_wait(int32_t* list, int res_set, int processor_id)
{
   int total_cw      = APU_QUE_RETURN_SIZE(list[0]);
   list[++total_cw]  = APU_SEQ_DEC(processor_id);
   list[++total_cw]  = res_set;

   list[0]           = APU_QUE_SIZE(total_cw);
   return 0;
}

int apumk_hal_seq_push_done(int32_t* list, int res_set, int processor_id)
{
   int total_cw      = APU_QUE_RETURN_SIZE(list[0]);
   list[++total_cw]  = APU_SEQ_INC(processor_id);
   list[++total_cw]  = res_set;

   list[0]           = APU_QUE_SIZE(total_cw);
   return 0;
}

int apumk_hal_push_align(int32_t* list)
{
   //
   int total_cw      = (APU_QUE_RETURN_SIZE(list[0]) + 1);
   int new_total     = ((total_cw / CONFIG_QUE_BUF_SIZ32) + 1 + 1) * CONFIG_QUE_BUF_SIZ32;
   int pad_cw        = new_total - total_cw;
   int i;
   // NOTE: PAD WITH 0
   for (i=0; i<pad_cw; i++)
   {
      list[++total_cw] = 0;
   }
   list[0]           = APU_QUE_SIZE(total_cw);
   return 0;
}

int apumk_hal_send_list(int apexID, int32_t apu_id,
                        int32_t* list)
{
   int rval             = 0;
   uintptr_t base_addr    = 0;
   volatile int* cnt_addr;
   volatile int* que_addr;
   int i, j;

   base_addr = (uintptr_t)ReturnVirtualHwPtr(apexID, HW_REG_SMEM) + cmem_if_hal_query_apu_dmem_start(apexID, apu_id);

   cnt_addr  = (volatile int*)(base_addr + DMEM_CMD_CNT_OFFSET);
   que_addr  = (volatile int*)(base_addr + DMEM_CMD_QUE_OFFSET);
   if (cnt_addr[0] == cnt_addr[1])
   {

      if (list[0] != 0)
      {
         int total_cw   = APU_QUE_RETURN_SIZE(list[0]);
         cnt_addr[0]    = total_cw;
         cnt_addr[1]    = 0;

         cnt_addr[0]    = total_cw;
         for (i=0, j=1; i<total_cw; i++, j++)
         {
            que_addr[i]   = list[j];
         }
      }
   }

   return rval;
}

int apumk_hal_send_cmd(int apexID, int32_t apu_id,
                       int32_t kernel_id,
                       int32_t num_params,
                       const int32_t* param_list)
{
   int rval = 0;
   int i;
   int64_t base_addr    = 0;
   volatile int32_t* cnt_addr;
   volatile int32_t* que_addr;
   
   // LOCAL LIST FOR 1 COMMAND
   int32_t list[MAX_NUM_PARAM + 2] = {0};
   int total_cw;
   
#ifdef __chess__
      base_addr   = 0;
#else
      base_addr   = (int64_t)ReturnVirtualHwPtr(apexID, HW_REG_SMEM) + cmem_if_hal_query_apu_dmem_start(apexID, apu_id);
#endif
   cnt_addr  = (volatile int32_t*)(base_addr + DMEM_CMD_CNT_OFFSET);
   que_addr  = (volatile int32_t*)(base_addr + DMEM_CMD_QUE_OFFSET);

   
   // list[0]: size
   // list[1]: encode cmd header
   // list [2 ~ (MAX_NUM_PARAM + 1)]: encode parameters
   apumk_hal_push_cmd(list, kernel_id, num_params, param_list);
   total_cw   = APU_QUE_RETURN_SIZE(list[0]);
   
   if (cnt_addr[0] == cnt_addr[1])
   {
      cnt_addr[0]    = total_cw;
      cnt_addr[1]    = 0;
      for (i=0; i<total_cw; i++)
      {
         que_addr[i]   = list[i+1];
      }
   }
   return rval;
}

/*
#ifdef __cplusplus
}
#endif
*/
