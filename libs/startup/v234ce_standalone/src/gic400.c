/*================================================================================================*/
/**
*   @file    gic400.c
*   @version M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*
*   @brief   AUTOSAR M4_SRC_MODULE_NAME - gic400 driver
*   @details GIC400 driver
*   This file contains sample code only. It is not part of the production code deliverables.
*
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
Marius Rotaru (rtrm001)       08/01/2015    ENGR00341256  Create GIC400 driver TreeRunner 0.8.0
---------------------------   ----------    ------------  ------------------------------------------
',)dnl - DO NOT modify this M4 line!
==================================================================================================*/

#include "gic400.h"


/*==================================================================================================
*                                        LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/
struct gic400_dist_if*          gic_dist;
struct gic400_physical_cpu_if*  gic_cpu;

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/




/*================================================================================================*/
/** 
* @brief gic400_setGICAddr
* @details Sets the address of the GIC's distributor and CPU interfaces
*/
/*================================================================================================*/
void gic400_setGICAddr(void* dist, void* cpu)
{
  gic_dist = (struct gic400_dist_if*)dist;
  gic_cpu  = (struct gic400_physical_cpu_if*)cpu;
  return;
}

/*================================================================================================*/
/** 
* @brief gic400_enableGIC
* @details Global enable of the Interrupt Distributor
*/
/*================================================================================================*/
void gic400_enableGIC(void)
{
  gic_dist->GICD_CTLR = 3;
  return;
}

/*================================================================================================*/
/** 
* @brief gic400_disableGIC
* @details Global disable of the Interrupt Distributor
*/
/*================================================================================================*/
void gic400_disableGIC(void)
{
  gic_dist->GICD_CTLR = 0;
  return;
}

/*================================================================================================*/
/** 
* @brief gic400_enableIntID
* @details Enables the interrupt source number ID
*/
/*================================================================================================*/
void gic400_enableIntID(unsigned int ID)
{
    unsigned int bank;
    
    bank = ID/32;        /* There are 32 IDs per register, need to work out which register to access */
    ID   = ID & 0x1f;    /* ... and which bit within the register */
    
    ID   = 1 << ID;      /* Move a '1' into the correct bit position */
    
    gic_dist->GICD_ISENABLER[bank] = ID;
    
    return;
}

/*================================================================================================*/
/** 
* @brief gic400_disableIntID
* @details Disables the interrupt source number ID
*/
/*================================================================================================*/
void gic400_disableIntID(unsigned int ID)
{
    unsigned int bank;
    
    bank = ID/32;        /* There are 32 IDs per register, need to work out which register to access */
    ID   = ID & 0x1f;    /* ... and which bit within the register */
    
    ID   = 1 << ID;      /* Move a '1' into the correct bit position */
    
    gic_dist->GICD_ICENABLER[bank] = ID;
    
    return;
}

/*================================================================================================*/
/** 
* @brief gic400_setIntPriority
* @details Sets the priority of the specified ID
*/
/*================================================================================================*/
void gic400_setIntPriority(unsigned int ID, unsigned int priority)
{
    if (ID > 1020)               /* Check ID in range */
        return;
    
    /* The priority registers allows byte accesses  meaning using a char array we can directly reference the correct entry */
    gic_dist->GICD_IPRIORITYR[ID] = priority;
    
    return;
}

/*================================================================================================*/
/** 
* @brief int gic400_getIntPriority
* @details Returns the priority of the specified ID
*/
/*================================================================================================*/
unsigned int gic400_getIntPriority(unsigned int ID)
{
    if (ID > 1020)               /* Check ID in range */
        return 0;
    
    return gic_dist->GICD_IPRIORITYR[ID];
} 

/*================================================================================================*/
/** 
* @brief int gic400_setIntTarget
* @details Sets the target CPUs of the specified ID. For 'target' use one of the above defines
*/
/*================================================================================================*/
void gic400_setIntTarget(unsigned int ID, unsigned int target)
{
    unsigned int bank, tmp;
    
    target = target & 0xFF;      /* Target field is 8-bits, mask off unused bit */
    bank = ID/4;                 /* There are 4 IDs per register, need to work out which register to access */
    ID   = ID & 0x3;             /* ... and which field within the register */
    ID   = ID * 8;               /* Convert from which field to a bit offset (8-bits per field) */
    
    target = target << ID;       /* Move prioity value into correct bit position */
    
    tmp = gic_dist->GICD_ITARGETSR[bank]; /* Read the current value in the register */
    tmp = tmp & ~(0xFF << ID);           /* Blank out the field holding the value we're modifying */
    tmp = tmp | target;                  /* OR in the new target */
    gic_dist->GICD_ITARGETSR[bank] = tmp;
    
    return;
}

/*================================================================================================*/
/** 
* @brief int gic400_getIntTarget
* @details Returns the target CPUs of the specified ID
*/
/*================================================================================================*/
unsigned int gic400_getIntTarget(unsigned int ID)
{
    unsigned int bank, tmp;
    
    bank = ID/4;                 /* There are 4 IDs per register, need to work out which register to access */
    ID   = ID & 0x3;             /* ... and which field within the register */
    ID   = ID * 8;               /* Convert from which field to a bit offset (8-bits per field) */
    
    tmp = gic_dist->GICD_ITARGETSR[bank];
    
    tmp = tmp >> ID;             /* Shift desired field to bit position 0 */
    tmp = tmp & 0xFF;            /* Mask off the other bits */
    
    return tmp;
}

/*================================================================================================*/
/** 
* @brief int gic400_configureSPI
* @details Configures the specified ID as being level or edge triggered 
*/
/*================================================================================================*/
void gic400_configureSPI(unsigned int ID, unsigned int conf)
{
    unsigned int bank, tmp;
    
    conf = conf & 0x3;            /* Mask out unused bits */
    
    bank = ID/16;                 /* There are 16 IDs per register, need to work out which register to access */
    ID   = ID & 0xF;              /* ... and which field within the register */
    ID   = ID * 2;                /* Convert from which field to a bit offset (2-bits per field) */
    
    conf = conf << ID;            /* Move configuration value into correct bit position */
    
    tmp = gic_dist->	GICD_ICFGR[bank]; /* Read current vlase */
    tmp = tmp & ~(0x3 << ID);         /* Clear the bits for the specified field */
    tmp = tmp | conf;                 /* OR in new configuration */
    gic_dist->	GICD_ICFGR[bank] = tmp; /* Write updated value back */
    
    return;
}

/*================================================================================================*/
/** 
* @brief int gic400_setIntPending
* @details Sets the pending bit of the specified ID  
*/
/*================================================================================================*/
void gic400_setIntPending(unsigned int ID)
{
    unsigned int bank;
    
    bank = ID/32;        /* There are 32 IDs per register, need to work out which register to access */
    ID   = ID & 0x1f;    /* ... and which bit within the register */
    
    ID   = 1 << ID;      /* Move a '1' into the correct bit position */
    
    gic_dist->GICD_ISPENDR[bank] = ID;
    
    return;
}

/*================================================================================================*/
/** 
* @brief int gic400_clearIntPending
* @details Clears the pending bit of the specified ID  
*/
/*================================================================================================*/
void gic400_clearIntPending(unsigned int ID)
{
    unsigned int bank;
    
    bank = ID/32;        /* There are 32 IDs per register, need to work out which register to access */
    ID   = ID & 0x1f;    /* ... and which bit within the register */
    
    ID   = 1 << ID;      /* Move a '1' into the correct bit position */
    
    gic_dist->GICD_ICPENDR[bank] = ID;
    
    return;
}

/*================================================================================================*/
/** 
* @brief int gic400_getIntPending
* @details Returns the value of the status bit of the specifed ID  
*/
/*================================================================================================*/
unsigned int gic400_getIntPending(unsigned int ID)
{
    unsigned int bank, tmp;
    
    bank = ID/32;        /* There are 32 IDs per register, need to work out which register to access */
    ID   = ID & 0x1f;    /* ... and which bit within the register */
    
    tmp = gic_dist->GICD_ICPENDR[bank];  /* Read the register containing the ID we are interested in */
    tmp = tmp >> ID;                     /* Shift the status bit for specified ID to position 0 */
    tmp = tmp & 0x1;                     /* Mask off the rest of the register */
    
    return tmp;
}

/*================================================================================================*/
/** 
* @brief gic400_sendSGI
* @details Send a software generate interrupt  
*/
/*================================================================================================*/
void gic400_sendSGI(unsigned int ID, unsigned int cpu_list, unsigned int filter_list, unsigned int SATT)
{
  /* Ensure unused bits are clear, and shift into correct bit position */
  ID = ID & 0xF;
  SATT = (SATT & 0x1) << 15;
  cpu_list = (cpu_list & 0xFF) << 16;
  filter_list = (filter_list & 0x3) << 24;

  /* Combine fields */
  ID = ID | SATT | cpu_list | filter_list;

  gic_dist->GICD_SGIR = ID;

  return;
}

/*================================================================================================*/
/** 
* @brief gic400_makeIntGroup0
* @details Sets the specified ID as secure  
*/
/*================================================================================================*/
void gic400_makeIntGroup0(unsigned int ID)
{
  unsigned int bank, tmp;

  bank = ID/32;        /* There are 32 IDs per register, need to work out which register to access */
  ID   = ID & 0x1f;    /* ... and which bit within the register */

  ID   = 1 << ID;      /* Move a '1' into the correct bit position */
  ID   = ~ID;          /* Invert to get mask */

  tmp = gic_dist->GICD_IGROUPR[bank];  /* Read current value*/
  tmp = tmp & ID;
  gic_dist->GICD_IGROUPR[bank] = tmp;

  return;
}

/*================================================================================================*/
/** 
* @brief gic400_makeIntGroup1
* @details Set the specified ID as non-secure   
*/
/*================================================================================================*/
void gic400_makeIntGroup1(unsigned int ID)
{
  unsigned int bank, tmp;

  bank = ID/32;        /* There are 32 IDs per register, need to work out which register to access */
  ID   = ID & 0x1f;    /* ... and which bit within the register */

  ID   = 1 << ID;      /* Move a '1' into the correct bit position */

  tmp = gic_dist->GICD_IGROUPR[bank];  /* Read current value */
  tmp = tmp | ID;                      /* Or with bit mask to set the bit */
  gic_dist->GICD_IGROUPR[bank] = tmp;  /* Write-back */
}

/*================================================================================================*/
/** 
* @brief gic400_enableCPUInterface
* @details Enables the CPU interface. Must been done one each core seperately  
*/
/*================================================================================================*/
void gic400_enableCPUInterface(void)
{
    unsigned int tmp;

    tmp = gic_cpu->GICC_CTLR;
    tmp = tmp | 0x1;          /* Set bit 0 */
    gic_cpu->GICC_CTLR = tmp;
}

/*================================================================================================*/
/** 
* @brief gic400_enableNonSecureCPUInterface
* @details Enables the group 1 (non-secure) CPU interface. This function can only be called from the Secure world
*          Must been done one each core seperately 
*/
/*================================================================================================*/
void gic400_enableNonSecureCPUInterface(void)
{
    unsigned int tmp;

    tmp = gic_cpu->GICC_CTLR;
    tmp = tmp | 0x2;          /* Set bit 1 */
    gic_cpu->GICC_CTLR = tmp;
}


/*================================================================================================*/
/** 
* @brief gic400_disableCPUInterface
* @details Disables the processor interface  
*/
/*================================================================================================*/
void gic400_disableCPUInterface(void)
{
    unsigned int tmp;

    tmp = gic_cpu->GICC_CTLR;
    tmp = tmp & 0xFFFFFFFFE;  /* Clear bit 0 */
    gic_cpu->GICC_CTLR = tmp;
}

/*================================================================================================*/
/** 
* @brief gic400_enableSecureFIQs
* @details Enables the sending of secure interrupts as FIQs  
*/
/*================================================================================================*/
void gic400_enableSecureFIQs(void)
{
    unsigned int tmp;

    tmp = gic_cpu->GICC_CTLR;
    tmp = tmp | 0x8;           /* Set bit 3 */
    gic_cpu->GICC_CTLR = tmp;
}
/*================================================================================================*/
/** 
* @brief gic400_enableSecureFIQs
* @details Disables the sending of secure interrupts as FIQs  
*/
/*================================================================================================*/
void gic400_disableSecureFIQs(void)
{
    unsigned int tmp;

    tmp = gic_cpu->GICC_CTLR;
    tmp = tmp | 0xFFFFFFFF7;  /* Clear bit 3 */
    gic_cpu->GICC_CTLR = tmp;
}

/*================================================================================================*/
/** 
* @brief gic400_readIntAck
* @details Returns the value of the Interrupt Acknowledge Register 
*/
/*================================================================================================*/
unsigned int gic400_readIntAck(void)
{
    return gic_cpu->GICC_IAR;
}

/*================================================================================================*/
/** 
* @brief gic400_writeEOI
* @details Writes ID to the End Of Interrupt register 
*/
/*================================================================================================*/
void gic400_writeEOI(unsigned int ID)
{
    gic_cpu->GICC_EOIR = ID;
    return;
}

/*================================================================================================*/
/** 
* @brief gic400_readAliasedIntAck
* @details Returns the value of the Aliased Interrupt Acknowledge Register  
*/
/*================================================================================================*/
unsigned int gic400_readAliasedIntAck(void)
{
    return gic_cpu->GICC_AIAR;
}

/*================================================================================================*/
/** 
* @brief gic400_writeAliasedEOI
* @details Writes ID to the Aliased End Of Interrupt register   
*/
/*================================================================================================*/
void gic400_writeAliasedEOI(unsigned int ID)
{
    gic_cpu->GICC_AEOIR = ID;
    return;
}

/*================================================================================================*/
/** 
* @brief gic400_setPriorityMask
* @details Sets the Priority mask register for the core run on. The reset value masks ALL interrupts!   
*/
/*================================================================================================*/
void gic400_setPriorityMask(unsigned int priority)
{
    gic_cpu->GICC_PMR = (priority & 0xFF);
    return;
}

/*================================================================================================*/
/** 
* @brief gic400_setBinaryPoint
* @details Sets the Binary Point Register for the core run on   
*/
/*================================================================================================*/
void gic400_setBinaryPoint(unsigned int priority)
{
    gic_cpu->GICC_BPR = (priority & 0xFF);
    return;
}

/*================================================================================================*/
/** 
* @brief gic400_setAliasedBinaryPoint
* @details Sets the Aliased Binary Point Register for the core run on   
*/
/*================================================================================================*/
/*  */
void gic400_setAliasedBinaryPoint(unsigned int priority)
{
    gic_cpu->GICC_ABPR = (priority & 0xFF);
    return;
}

