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
#include <stdio.h>

typedef unsigned int REG32;
#include <apex_642cl_global_map.h>

#define APU_COM_WAIT_CC 100
#include <time_helper.h>

#ifdef __cplusplus
extern "C" {
#endif

// cxu: SHARED DMEM divided into message queue and command queue
// size: in WORDs
// entire shared memory:            2048  32-bit
// message queue size:              8     32-bit
// command queue counter size:      8     32-bit
// command queue size:              2032  32-bit
/*
#define APU_COM_DMEM_SIZE        8
#define APU_DMEM_CMD_CNT_SIZE    8
#define APU_DMEM_CMD_QUE_SIZE    2032
*/

/***********************************************************************
// COM IMPLEMENTATION
***********************************************************************/

// HOST APIs ONLY
int
apumk_hal_host_com_rx(int apexID, int apuid, int32_t* msg, int timeoutInUs)
{
   int rval = 0;

   volatile int* APU0_DMEM_SHARED = (volatile int*)((long)ReturnVirtualHwPtr(apexID, HW_REG_SMEM) + cmem_if_hal_query_apu_dmem_start(apexID, apuid) + DMEM_SHARED_OFFSET);
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);

   uint64_t timeoutProbe = RET_TICKS;
   int timeoutDetected = 0;

   while (1)
   {
      if ((APU0_DMEM_SHARED[4] > APU0_DMEM_SHARED[5]))
      {
         if (msg)
            *msg = APU0_DMEM_SHARED[6];
         // SEND ACKNOWLEDGE
         ++APU0_DMEM_SHARED[5];
         break;
      }

      if (1 == timeoutDetected)
      {
         printf(" *** apumk_hal_host_com_rx(): TIMEOUT!!! [%1i][%2i] %6i %6i %08X %08X <%08X>\n",
                apexID, apuid,
                APU0_DMEM_SHARED[4], APU0_DMEM_SHARED[5], APU0_DMEM_SHARED[6], APU0_DMEM_SHARED[7],
                lpApex2GlobalReg->Misc.as32BitValue);
         rval = -1;
         break;
      }

      //timeout detection
      if (CONVERT_TICKS_TO_US(RET_TICKS - timeoutProbe) >= timeoutInUs)
         timeoutDetected = 1;

      _wait_cc(APU_COM_WAIT_CC);
   }

   return rval;
}

int
apumk_hal_host_com_tx(int apexID, int apuid, int32_t msg, int timeoutInUs)
{
   int rval = 0;

   volatile int* APU0_DMEM_SHARED = (volatile int*)((long)ReturnVirtualHwPtr(apexID, HW_REG_SMEM) + cmem_if_hal_query_apu_dmem_start(apexID, apuid) + DMEM_SHARED_OFFSET);
   volatile APEX_642CL_GLOBAL_DCR *const lpApex2GlobalReg = (APEX_642CL_GLOBAL_DCR*)ReturnVirtualHwPtr(apexID, HW_REG_GLOBAL);

   APU0_DMEM_SHARED[2] = msg;
   // SEND A MESSAGE
   ++APU0_DMEM_SHARED[0];

   uint64_t timeoutProbe = RET_TICKS;
   int timeoutDetected = 0;

   while (1)
   {
      if (!(APU0_DMEM_SHARED[0] > APU0_DMEM_SHARED[1]))
         break;

      if (1 == timeoutDetected)
      {
         printf(" *** apumk_hal_host_com_tx(): TIMEOUT!!! [%1i][%2i] %6i %6i %08X %08X <%08X>\n",
                apexID, apuid,
                APU0_DMEM_SHARED[0], APU0_DMEM_SHARED[1], APU0_DMEM_SHARED[2], APU0_DMEM_SHARED[3],
                lpApex2GlobalReg->Misc.as32BitValue);
         rval = -1;
         break;
      }

      //timeout detection
      if (CONVERT_TICKS_TO_US(RET_TICKS - timeoutProbe) >= timeoutInUs)
         timeoutDetected = 1;

      _wait_cc(APU_COM_WAIT_CC);
   }

   return rval;
}

#ifdef __cplusplus
}
#endif
