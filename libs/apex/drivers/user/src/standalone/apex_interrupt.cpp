#include <stdint.h>
#include <apex_interrupt.h>
#include <oal.h>
#include <oal_irq.h>
#include <seq_hal.h>

extern APEX_INTERRUPTS gApexImrState;

void APEX_InterruptSignalHandler(int32_t lApexId)
{
   //printf("APEX_InterruptSignalHandler() called!\n");

   //handle all relevant APEX interrupts here!!!
   //1) capture state
   //2) clear interrupts
   //3) 'handle' interrupts (i.e. release applicable semaphores!)
   
   APEX_INTERRUPTS lIsrState = {0, 0, 0};

   //SEQ
   lIsrState.seq = seq_hal_query_interrupts(lApexId) & ~gApexImrState.seq; //capture
   seq_hal_clear_interrupts(lApexId, lIsrState.seq); //clear

   APEX_InterruptSignalHandlerCommon(lApexId, lIsrState);
}

//Interrupt signal handler for APEX 0
void APEX_InterruptSignalHandlerApex0()
{
  //printf("APEX interrupt 0\n");
  APEX_InterruptSignalHandler(0);
}

//Interrupt signal handler for APEX 1
void APEX_InterruptSignalHandlerApex1()
{
  //printf("APEX interrupt 1\n");
  APEX_InterruptSignalHandler(1);
}

//Interrupt setup
#define APEX0_INT_ID 141
#define APEX1_INT_ID 142
int32_t APEX_InterruptSetup()
{
   int32_t lRetVal = OAL_SUCCESS;

   //setup for each APEX
   lRetVal |= OAL_IRQInit(APEX_InterruptSignalHandlerApex0, APEX0_INT_ID, 0xA0, OAL_IRQ_LEVEL);
   lRetVal |= OAL_IRQInit(APEX_InterruptSignalHandlerApex1, APEX1_INT_ID, 0xA0, OAL_IRQ_LEVEL);

   if (OAL_SUCCESS == lRetVal)
   {
      //enable for each APEX
      OAL_IRQEnable(APEX0_INT_ID);
      OAL_IRQEnable(APEX1_INT_ID);
   }

   return lRetVal;
}

//Interrupt cleanup
int32_t APEX_InterruptCleanup()
{
   int32_t lRetVal = OAL_SUCCESS;

   //cleanup for APEX 0
   OAL_IRQDisable(APEX0_INT_ID);
   lRetVal |= OAL_IRQDelete(APEX0_INT_ID);

   //cleanup for APEX 1
   OAL_IRQDisable(APEX1_INT_ID);
   lRetVal |= OAL_IRQDelete(APEX1_INT_ID);

   return lRetVal;
}
