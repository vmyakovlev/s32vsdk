#include <apex.h>
#include <hw_reg_user.h>
#include <global_hal.h>
#include <resource_manager_apex.h>
#include <apu_hal.h>
#include <apex_interrupt.h>

static int gsApexInitFlag = 0;

int APEX_Init()
{
   int lRetVal = 0;

   lRetVal |= InitVirtualHwPtrArray();

   if (0 == lRetVal)
   {
      global_hal_AllReset(0);
      global_hal_AllReset(1);

      lRetVal |= ResourceManagerInit_APEX(global_hal_RetHwVersion(0));
      lRetVal |= apu_hal_Init(); //required for semaphore protection related to apu enable/disable
      lRetVal |= APEX_InterruptInit();
      
      if (0 == lRetVal)
      {
         gsApexInitFlag = 1;
      }
   }

   return lRetVal;
}

int APEX_Deinit()
{
   int lRetVal = 0;

   lRetVal |= APEX_InterruptDeinit();
   lRetVal |= apu_hal_Deinit();
   lRetVal |= ResourceManagerDeinit_APEX(global_hal_RetHwVersion(0));
   lRetVal |= DeinitVirtualHwPtrArray();
   
   if (0 == lRetVal)
   {
      gsApexInitFlag = 0;
   }

   return lRetVal;
}

int RetApexInitFlag()
{
   return gsApexInitFlag;
}

//deprecated
int ACF_Init()
{
   return APEX_Init();
}

int ACF_Deinit()
{
   return APEX_Deinit();
}
