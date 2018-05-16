#include <stdint.h>
#include <stdio.h>
#include <apex_interrupt.h>
#include <oal.h>
#include <global_hal.h>
#include <seq_hal.h>

int32_t APEX_InterruptSetup();
int32_t APEX_InterruptCleanup();

#define NUM_APEX 2 //APEX 0, APEX 1

//configured IMR state
APEX_INTERRUPTS gApexImrState = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF};

//callback format: void myCallbackFunction(void* lpParam, int32_t* lpRetVal);
static void (*gsInterruptCallbackTable[NUM_APEX][NUM_INTERRUPTS])(void*, int32_t*) = {0};
static void* gsCallbackParam[NUM_APEX][NUM_INTERRUPTS] = {0};
static int32_t* gsCallbackRetVal[NUM_APEX][NUM_INTERRUPTS] = {0};

int32_t APEX_InterruptRegisterCallback(int32_t lApexId,
                                       int32_t lInterruptId,
                                       void (*lpCallback)(void* lpParam, int32_t* lpRetVal),
                                       void* lpParam,
                                       int32_t* lpRetVal)
{
   int32_t lRetVal = 0;

   gsInterruptCallbackTable[lApexId][lInterruptId] = lpCallback;
   gsCallbackParam[lApexId][lInterruptId] = lpParam;
   gsCallbackRetVal[lApexId][lInterruptId] = lpRetVal;

   return lRetVal;
}

int32_t APEX_InterruptClearCallback(int32_t lApexId, int32_t lInterruptId)
{
   int32_t lRetVal = 0;

   gsInterruptCallbackTable[lApexId][lInterruptId] = 0;

   return lRetVal;
}

int32_t APEX_InterruptInit(void)
{
   int32_t lRetVal = 0;

   //OS specific setup
   lRetVal |= APEX_InterruptSetup();

   //------------------------------
   //HW specific mask configuration
   //------------------------------
      
   //SEQ
   //Host is only concerned with interrupt bits 30 and 31 (this must not clash with interrupts the APU is concerned with!)
   gApexImrState.seq = ~(1<<30 | 1<<31);
   seq_hal_mask_interrupts(0, gApexImrState.seq);
   seq_hal_mask_interrupts(1, gApexImrState.seq);

   //-------------------------
   //GLOBAL mask configuration
   //-------------------------
   //The default state of all global APEX interrupts following reset is disabled/masked.  Enable
   //desired global interrupt lines.
   global_hal_Apex2GlobalInterruptEnable(0, APEX2_INTERRUPT_SEQUENCER);
   global_hal_Apex2GlobalInterruptEnable(1, APEX2_INTERRUPT_SEQUENCER);

   return lRetVal;
}

int32_t APEX_InterruptDeinit(void)
{
   int32_t lRetVal = 0;

   //-------------------------
   //GLOBAL mask cleanup
   //-------------------------
   global_hal_Apex2GlobalInterruptDisable(0, APEX2_INTERRUPT_SEQUENCER);
   global_hal_Apex2GlobalInterruptDisable(1, APEX2_INTERRUPT_SEQUENCER);

   //------------------------------
   //HW specific cleanup
   //------------------------------

   //SEQ
   gApexImrState.seq = 0xFFFFFFFF;
   seq_hal_mask_interrupts(0, gApexImrState.seq);
   seq_hal_mask_interrupts(1, gApexImrState.seq);

   //OS specific cleanup
   lRetVal = APEX_InterruptCleanup();

   return lRetVal;
}

void APEX_InterruptSignalHandlerCommon(int32_t lApexId, APEX_INTERRUPTS lIsrState)
{
   if (lIsrState.seq & (1ul << 30))
   {
      //printf("APEX_InterruptSignalHandler() -> APEX%d SEQUENCER_RES_30 interrupt received (callback=%08X, param=%08X\n", lApexId, (uintptr_t)gsInterruptCallbackTable[lApexId][SEQUENCER_RES_30], (uintptr_t)gsCallbackParam[lApexId][SEQUENCER_RES_30]);
      if (0 != gsInterruptCallbackTable[lApexId][SEQUENCER_RES_30])
      {
         gsInterruptCallbackTable[lApexId][SEQUENCER_RES_30](gsCallbackParam[lApexId][SEQUENCER_RES_30],
                                                             gsCallbackRetVal[lApexId][SEQUENCER_RES_30]);
      }
   }

   if (lIsrState.seq & (1ul << 31))
   {
      //printf("APEX_InterruptSignalHandler() -> APEX%d SEQUENCER_RES_31 interrupt received (callback=%08X, param=%08X\n", lApexId, (uintptr_t)gsInterruptCallbackTable[lApexId][SEQUENCER_RES_31], (uintptr_t)gsCallbackParam[lApexId][SEQUENCER_RES_31]);
      if (0 != gsInterruptCallbackTable[lApexId][SEQUENCER_RES_31])
      {
         gsInterruptCallbackTable[lApexId][SEQUENCER_RES_31](gsCallbackParam[lApexId][SEQUENCER_RES_31],
                                                             gsCallbackRetVal[lApexId][SEQUENCER_RES_31]);
      }
   }
}
