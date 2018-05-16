#include "gic400.h"
#include "timer.h"

void SystemInit(void)
{
    uint32 i;
   
   /* Configure Addresses for GIC_Distributor and Core */  
    gic400_setGICAddr((void*)0x7D001000, (void*)0x7D002000);

    /* Configure interrupt controller (GICv2 assumed) */
    gic400_enableGIC();
    gic400_enableCPUInterface();
    gic400_enableNonSecureCPUInterface();
    /* Configure priority mask to lowest value - enables all priorities */
  
    gic400_setPriorityMask(0xFF);
  
    /* fill the interrupts_vector with the default interrupt rutine */
    for (i=0; i<INT_NUM_SOURCES; i++)
    {
        g_INT_vectors[i] = (int_function_t)default_interrupt_routine; 
    }
  
    g_INT_vectors[GIC400_PPI29] = (int_function_t)tsp_generic_timer_handler;
}

int main(void)
{
	gic400_setIntPriority(GIC400_PPI29, 0xA0);
	gic400_setIntTarget(GIC400_PPI29, GIC400_TARGET_CPU0);
	gic400_configureSPI(GIC400_PPI29, GIC400_CONFIG_LEVEL);
	gic400_enableIntID(GIC400_PPI29);

	tsp_generic_timer_start();

	while (1);
	return 0;
}
