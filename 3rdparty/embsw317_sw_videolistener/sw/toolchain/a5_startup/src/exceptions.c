/*================================================================================================*/
/**
*   @file    exceptions.c
*   @version M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*
*   @brief   AUTOSAR M4_SRC_MODULE_NAME - Interrupts table for Treerunner_A53
*   @details Interrupts table for Treerunner_A53
*            This file contains sample code only. It is not part of the production code deliverables
*   @addtogroup BUILD_ENV
*   @{
*/
/*==================================================================================================
*   Project              : M4_SRC_PROJECT_NAME
*   Platform             : M4_SRC_MCU_FAMILY
*   Peripheral           : M4_SRC_USED_PERIPHERAL
*   Dependencies         : M4_SRC_AR_MODULE_DEPENDENCY
*
*   Autosar Version      : M4_SRC_AR_SPEC_VERSION_MAJOR.M4_SRC_AR_SPEC_VERSION_MINOR.M4_SRC_AR_SPEC_VERSION_PATCH
*   Autosar Revision     : M4_SRC_AR_RELEASE_REVISION
*   Autosar Conf.Variant :
*   SW Version           : M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*   Build Version        : M4_SRC_BUILD_ID
*
*   (c) Copyright M4_SRC_YEAR_ID M4_SRC_COPYRIGHTED_TO
*   All Rights Reserved.
==================================================================================================*/
/*==================================================================================================
ifdef(`M4_SRC_KEEP_REVISION_HISTORY', `dnl - DO NOT modify this M4 line!
Revision History:
                             Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ------------------------------------------
Marius Rotaru (rtrm001)       26/11/2014    ENGR00341256  Update for TreeRunner 0.8.0
---------------------------   ----------    ------------  ------------------------------------------
',)dnl - DO NOT modify this M4 line!
==================================================================================================*/

#include "gic400.h"
#include "typedefs.h"

#ifdef	EXCEPTION_RESET
#define	RESET_CORE	{	*((uint32_t *)0x4004a004) = 0x00005AF0;	\
				*((uint32_t *)0x4004a004) = 0x0000A50F; }
#else
#define	RESET_CORE
#endif

__attribute__ ((section (".intc_vector"))) 
int_function_t g_INT_vectors[INT_NUM_SOURCES];
__attribute__ ((section (".intc_vector"))) 
int_function_t g_FIQ_vectors[INT_NUM_SOURCES];
__attribute__ ((section (".intc_vector"))) 
int_function_t pfCustomEL3SyncSpxHandler = (int_function_t)(0U);


void install_custom_a53_irq_handler(uint32_t u32IntSrcNum, void (* pfHandler)(void))
{
    g_INT_vectors[u32IntSrcNum] = pfHandler;
}

void install_custom_a53_fiq_handler(uint32_t u32IntSrcNum, void (* pfHandler)(void))
{
    g_FIQ_vectors[u32IntSrcNum] = pfHandler;
}

void install_custom_a53_el3_sync_spx_handler(void (* pfHandler)(void))
{
    pfCustomEL3SyncSpxHandler = pfHandler;
}


/* ------------- CA53 Handlers ---------------*/
__attribute__ ((section (".exception_handlers"))) 
void default_interrupt_routine(void)
{
    while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void default_fiq_routine(void)
{
    while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el3_sp0_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el3_sp0_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el3_sp0_fiq_handler(void) 
{
#if 0
    uint32_t int_ack_reg;
    uint32_t int_id;
    uint32_t invalid_int=1023;
       
    int_ack_reg=gic400_readIntAck();
    int_id=(int_ack_reg & 0x000003FF);
    if(int_id == invalid_int)
    {
        while(1);
    }else
    {
        g_INT_vectors[int_id]();
        gic400_writeEOI(int_ack_reg);
        /* asm("eret") */
    }
#else
    while(1);
#endif

}

__attribute__ ((section (".exception_handlers"))) 
void curr_el3_sp0_serror_handler(void)
{
  RESET_CORE;
  while(1);
}

extern void read_ncf (void);
#if (NO_MMU == TRUE)
extern void mmu_exception_handler(unsigned int);
#endif /* NO_MMU */
__attribute__ ((section (".exception_handlers"))) 
void curr_el3_spx_sync_handler(void) 
{
#if (NO_MMU == TRUE)
  /* mmu_exception_handler(0); */
#endif /* NO_MMU */
    if (pfCustomEL3SyncSpxHandler)
    {
        pfCustomEL3SyncSpxHandler();
    }

#if 0
  read_ncf ();
  *((uint32_t *)0x40048000) = 0xFFFFFFFF;
  *((uint32_t *)0x40048300) = 0xFFFFFFFF;
  
  RESET_CORE;
  while(1);
#endif
    return;
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el3_spx_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el3_spx_fiq_handler(void) 
{
#if 0
    uint32_t int_ack_reg;
    uint32_t int_id;
    uint32_t invalid_int=1023;
       
    int_ack_reg=gic400_readIntAck();
    int_id=(int_ack_reg & 0x000003FF);
    if(int_id == invalid_int)
    {
        while(1);
    }else
    {
        g_INT_vectors[int_id]();
        gic400_writeEOI(int_ack_reg);
        /* asm("eret") */
     }    
#else
    while(1);
#endif
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el3_spx_serror_handler(void) 
{
  RESET_CORE;
  while(1);  
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch64_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch64_irq_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch64_fiq_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch64_serror_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch32_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch32_irq_handler(void) 
{

}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch32_fiq_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el3_aarch32_serror_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_sp0_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_sp0_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_sp0_fiq_handler(void) 
{
    while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_sp0_serror_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_spx_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_spx_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_spx_fiq_handler(void) 
{
#if 0
    uint32_t int_ack_reg;
    uint32_t int_id;
    uint32_t invalid_int=1023;
       
    int_ack_reg=gic400_readIntAck();
    int_id=(int_ack_reg & 0x000003FF);
    if(int_id == invalid_int)
    {
        while(1);
    }else
    {
        g_INT_vectors[int_id]();
        gic400_writeEOI(int_ack_reg);
        /* asm("eret") */
     }   
#else
    while(1);
#endif
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el2_spx_serror_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch64_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch64_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch64_fiq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch64_serror_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch32_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch32_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch32_fiq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el2_aarch32_serror_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_sp0_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_sp0_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_sp0_fiq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_sp0_serror_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_spx_sync_handler(void) 
{
  RESET_CORE;
  while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_spx_irq_handler(void) 
{

}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_spx_fiq_handler(void) 
{
#if 0
    uint32_t int_ack_reg;
    uint32_t int_id;
    uint32_t invalid_int=1023;
       
    int_ack_reg=gic400_readIntAck();
    int_id=(int_ack_reg & 0x000003FF);
    if(int_id == invalid_int)
    {
        while(1);
    }else
    {
        g_INT_vectors[int_id]();
        gic400_writeEOI(int_ack_reg);
        /* asm("eret") */
     }   
#else
    while(1);
#endif
}

__attribute__ ((section (".exception_handlers"))) 
void curr_el1_spx_serror_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch64_sync_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch64_irq_handler(void) 
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch64_fiq_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch64_serror_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch32_sync_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch32_irq_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch32_fiq_handler(void)
{
   while(1);
}

__attribute__ ((section (".exception_handlers"))) 
void lower_el1_aarch32_serror_handler(void) 
{
   while(1);
}
