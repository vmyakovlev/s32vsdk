#include <stdint.h>
#include <stdio.h>
#include <apex_interrupt.h>
#include <signal.h>
#include <fcntl.h>      // open 
#include <unistd.h>     // exit 
#include <sys/ioctl.h>  // ioctl
#include <errno.h>
#include <string.h>
#include "../../drivers/kernel/include/apex_ioctl.h"
#include "../../drivers/kernel/include/apex_defines.h"
#include <oal.h>
#include <global_hal.h>
#include <seq_hal.h>

#define IRQ_ARRAY_SIZE 16

//handle to kernel populated struct containing interrupt information
static void *gsInterruptArrayHandle = 0;
static APEX_INTERRUPTS *gsInterruptArray = 0;

void APEX_InterruptSignalHandler(int32_t lApexId, int32_t lBufferId)
{
#if 0
   printf("APEX_InterruptSignalHandler() -> APEX%d BUF%d (0x%08X 0x%08X 0x%08X 0x%08X)\n",
          lApexId,
          lBufferId,
          gsInterruptArray[lBufferId].seq,
          gsInterruptArray[lBufferId].mcdma,
          gsInterruptArray[lBufferId].stio,
          gsInterruptArray[lBufferId].mce);
#endif

   //The linux kernel performs the capture and clear steps.  The captured interrupt state information is stored
   //in the circular array 'gsInterruptArray'.  The current captured interrupt state information resides
   //in 'gsInterruptArray[lBufferId]'.

   APEX_INTERRUPTS lIsrState = {0, 0, 0};

   //SEQ
   lIsrState.seq = gsInterruptArray[lBufferId].seq;

   APEX_InterruptSignalHandlerCommon(lApexId, lIsrState);
}

//Interrupt signal handler for APEX 0
void APEX_InterruptSignalHandlerApex0(int /*aSigNum*/, siginfo_t *apSigInfo, void */*aUnused*/)
{
  //printf("APEX interrupt 0\n");
  APEX_InterruptSignalHandler(0, apSigInfo->si_int);
}

//Interrupt signal handler for APEX 1
void APEX_InterruptSignalHandlerApex1(int /*aSigNum*/, siginfo_t *apSigInfo, void */*aUnused*/)
{
  //printf("APEX interrupt 1\n");
  APEX_InterruptSignalHandler(1, apSigInfo->si_int);
}

//Interrupt setup - will associate a signal with the handler function + specific pid in case of multi process environment
int32_t APEX_InterruptSetup()
{
   int32_t lRetVal = 0;

   int32_t apexFD = open("/dev/apex", 0);

   if (apexFD <= 0)
   {
      printf("APEX_InterruptSetup() -> Failed to open /dev/apex\n");
      lRetVal = 1;
   }
   else //dev/apex opened successfully
   {
      //set process ID for each APEX
      int8_t lApexId = 0;
      if (ioctl(apexFD, IOCTL_APEX_PID_SET, &lApexId) < 0)
      {
         int32_t lErrno = errno;
         printf("APEX_InterruptSetup() -> ioctl 'IOCTL_APEX_PID_SET' for APEX 0 failed and returned errno %s\n", strerror(lErrno));
         lRetVal = 1;
      }
      lApexId = 1;
      if (ioctl(apexFD, IOCTL_APEX_PID_SET, &lApexId) < 0)
      {
         int32_t lErrno = errno;
         printf("APEX_InterruptSetup() -> ioctl 'IOCTL_APEX_PID_SET' for APEX 1 failed and returned errno %s\n", strerror(lErrno));
         lRetVal = 1;
      }

      //create buffer that will contain array of interrupt structs
      gsInterruptArrayHandle = OAL_MemoryAllocFlag(sizeof(APEX_INTERRUPTS)*IRQ_ARRAY_SIZE);
      if (0 != gsInterruptArrayHandle)
      {
         gsInterruptArray = (APEX_INTERRUPTS*)OAL_MemoryReturnAddress(gsInterruptArrayHandle, ACCESS_NCH_NB);
         IOCTL_APEX_INTERRUPT_BUFFER_SET_TYPE aux;
         aux.size = IRQ_ARRAY_SIZE;
         aux.start_pointer = (int64_t)gsInterruptArrayHandle;
         if (ioctl(apexFD, IOCTL_APEX_INTERRUPT_BUFFER_SET, &aux) < 0)
         {
            int32_t lErrno = errno;
            printf("APEX_InterruptSetup() -> ioctl 'IOCTL_APEX_INTERRUPT_BUFFER_SET' failed and returned errno %s\n", strerror(lErrno));
            lRetVal = 1;
         }
      }
      else
      {
         printf("APEX_InterruptSetup() -> failed to allocate memory for interrupt array\n");
         lRetVal = 1;
      }

      close(apexFD);

      //prepare internal signal handler
      struct sigaction sig;
      sig.sa_flags = SA_SIGINFO;

      //setup signal handler for APEX 0
      sig.sa_sigaction = APEX_InterruptSignalHandlerApex0;
      if(sigaction(ACF_EVENT_SIGNAL_0, &sig, NULL) != 0)
      {
         printf("APEX_InterruptSetup() -> Failed to register signal handler.\n");
         lRetVal = 1;
      }

      //setup signal handler for APEX 1
      sig.sa_sigaction = APEX_InterruptSignalHandlerApex1;
      if(sigaction(ACF_EVENT_SIGNAL_1, &sig, NULL) != 0)
      {
         printf("APEX_InterruptSetup() -> Failed to register signal handler.\n");
         lRetVal = 1;
      }
   }

   return lRetVal;
}

//Interrupt cleanup
int32_t APEX_InterruptCleanup()
{
   int32_t lRetVal = 0;

   if (gsInterruptArrayHandle) OAL_MemoryFree(gsInterruptArrayHandle);
   gsInterruptArrayHandle = 0;
   gsInterruptArray = 0;

   return lRetVal;
}
