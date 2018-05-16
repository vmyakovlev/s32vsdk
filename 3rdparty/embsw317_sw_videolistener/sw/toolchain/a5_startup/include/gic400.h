/*==================================================================================================
*   @file    gic400.h
*   @version M4_SRC_SW_VERSION_MAJOR.M4_SRC_SW_VERSION_MINOR.M4_SRC_SW_VERSION_PATCH
*
*   @brief   AUTOSAR M4_SRC_MODULE_NAME - gic400 driver
*   @details gic400 driver
*            This file contains sample code only. It is not part of the production code deliverables
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

#ifndef GIC400_H
#define GIC400_H

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "typedefs.h"
#define INT_NUM_SOURCES (224)
extern int_function_t g_INT_vectors[INT_NUM_SOURCES];
extern int_function_t g_FIQ_vectors[INT_NUM_SOURCES];
/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#define GIC400_PPI0                     (0)
#define GIC400_PPI1                     (1)
#define GIC400_PPI2                     (2)
#define GIC400_PPI3                     (3)
#define GIC400_PPI4                     (4)
#define GIC400_PPI5                     (5)
#define GIC400_PPI6                     (6)
#define GIC400_PPI7                     (7)
#define GIC400_PPI8                     (8)
#define GIC400_PPI9                     (9)
#define GIC400_PPI10                    (10)
#define GIC400_PPI11                    (11)
#define GIC400_PPI12                    (12)
#define GIC400_PPI13                    (13)
#define GIC400_PPI14                    (14)
#define GIC400_PPI15                    (15)
#define GIC400_PPI16                    (16)
#define GIC400_PPI17                    (17)
#define GIC400_PPI18                    (18)
#define GIC400_PPI19                    (19)
#define GIC400_PPI20                    (20)
#define GIC400_PPI21                    (21)
#define GIC400_PPI22                    (22)
#define GIC400_PPI23                    (23)
#define GIC400_PPI24                    (24)
#define GIC400_PPI25                    (25)
#define GIC400_PPI26                    (26)
#define GIC400_PPI27                    (27)
#define GIC400_PPI28                    (28)
#define GIC400_PPI29                    (29)
#define GIC400_PPI30                    (30)
#define GIC400_PPI31                    (31)

#define GIC400_TARGET_CPU0              (0x01)
#define GIC400_TARGET_CPU1              (0x02)
#define GIC400_TARGET_CPU2              (0x04)
#define GIC400_TARGET_CPU3              (0x08)
#define GIC400_TARGET_CPU4              (0x10)
#define GIC400_TARGET_CPU5              (0x20)
#define GIC400_TARGET_CPU6              (0x40)
#define GIC400_TARGET_CPU7              (0x80)
                                        
#define GIC400_CONFIG_LEVEL             (0)
#define GIC400_CONFIG_EDGE              (2)
    
#define GIC400_SGI_SECURE               (0)
#define GIC400_SGI_NONSECURE            (1)
#define GIC400_SGI_FILTER_USE_LIST      (0)
#define GIC400_SGI_FILTER_NOT_THIS_CPU  (1)
#define GIC400_SGI_FILTER_THIS_CPU      (2)
#define GIC400_SGI_CPU0                 (0x01)
#define GIC400_SGI_CPU1                 (0x02)
#define GIC400_SGI_CPU2                 (0x04)
#define GIC400_SGI_CPU3                 (0x08)
#define GIC400_SGI_CPU4                 (0x10)
#define GIC400_SGI_CPU5                 (0x20)
#define GIC400_SGI_CPU6                 (0x40)
#define GIC400_SGI_CPU7                 (0x80)

#define GIC400_PENDING_IS_SET             (1)
#define GIC400_PENDING_IS_CLEAR           (0)

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

struct gic400_dist_if
{
    volatile unsigned int GICD_CTLR;             /* +0x000 - RW - Distributor Control Register                    */
    const volatile unsigned int GICD_TYPRE;      /* +0x004 - RO - Interrupt Controller Type Register              */
    const volatile unsigned int GICD_IIDR;       /* +0x008 - RO - Distributor Implementer Identification Register */

    const volatile unsigned int padding0[29];

    volatile unsigned int GICD_IGROUPR[32];      /* +0x080 - RW - Interrupt Groupt Registers (Security Registers in GICv1) */

    volatile unsigned int GICD_ISENABLER[32];    /* +0x100 - RW - Interrupt Set-Enable Registers    */
    volatile unsigned int GICD_ICENABLER[32];    /* +0x180 - RW - Interrupt Clear-Enable Registers  */
    volatile unsigned int GICD_ISPENDR[32];      /* +0x200 - RW - Interrupt Set-Pending Registers   */
    volatile unsigned int GICD_ICPENDR[32];      /* +0x280 - RW - Interrupt Clear-Pending Registers */
    volatile unsigned int GICD_ISACTIVER[32];    /* +0x300 - RW - Interrupt Set-Active Register     */
    volatile unsigned int GICD_ICACTIVER[32];    /* +0x380 - RW - Interrupt Clear-Active Register   */

    volatile unsigned char GICD_IPRIORITYR[1024]; /* +0x400 - RW - Interrupt Priority Registers         */
    volatile unsigned int GICD_ITARGETSR[256];   /* +0x800 - RW - Interrupt Processor Targets Registers */
    volatile unsigned int GICD_ICFGR[64];        /* +0xC00 - RW - Interrupt Configuration Registers     */

    const volatile unsigned int padding2[128];

    volatile unsigned int GICD_SGIR;             /* +0xF00 - WO - Software Generated Interrupt Register */

};

struct gic400_physical_cpu_if
{
    volatile unsigned int GICC_CTLR;             /* +0x000 - RW - CPU Interface Control Register              */
    volatile unsigned int GICC_PMR;              /* +0x004 - RW - Interrupt Priority Mask Register            */
    volatile unsigned int GICC_BPR;              /* +0x008 - RW - Binary Point Register                       */
    const volatile unsigned int GICC_IAR;        /* +0x00C - RO - Interrupt Acknowledge Register              */
    volatile unsigned int GICC_EOIR;             /* +0x010 - WO - End of Interrupt Register                   */
    const volatile unsigned int GICC_RPR;        /* +0x014 - RO - Running Priority Register                   */
    const volatile unsigned int GICC_HPPIR;      /* +0x018 - RO - Highest Pending Interrupt Register          */
    volatile unsigned int GICC_ABPR;             /* +0x01C - RW - Aliased Binary Point Register               */
    const volatile unsigned int GICC_AIAR;       /* +0x020 - RO - Aliased Interrupt Acknowledge Register      */
    volatile unsigned int GICC_AEOIR;            /* +0x024 - WO - Aliased End of Interrupt Register           */
    const volatile unsigned int GICC_AHPPIR;     /* +0x028 - RO - Aliased Highest Pending Interrupt Register  */

    const volatile unsigned int padding0[52];

    const volatile unsigned int GICC_IIDR;       /* +0x0FC - RO - CPU Interface Identification Register */
};

extern struct gic400_dist_if*          gic_dist;
extern struct gic400_physical_cpu_if*  gic_cpu;

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/
void gic400_setGICAddr(void* dist, void* cpu);
void gic400_enableGIC(void);
void gic400_disableGIC(void);
void gic400_enableIntID(unsigned int ID);
void gic400_disableIntID(unsigned int ID);
void gic400_setIntPriority(unsigned int ID, unsigned int priority);
unsigned int gic400_getIntPriority(unsigned int ID);
void gic400_setIntTarget(unsigned int ID, unsigned int target);
unsigned int gic400_getIntTarget(unsigned int ID);
void gic400_configureSPI(unsigned int ID, unsigned int conf);
void gic400_setIntPending(unsigned int ID);
void gic400_clearIntPending(unsigned int ID);
unsigned int gic400_getIntPending(unsigned int ID);
void gic400_sendSGI(unsigned int ID, unsigned int cpu_list, unsigned int filter_list, unsigned int SATT);
void gic400_makeIntGroup0(unsigned int ID);
void gic400_makeIntGroup1(unsigned int ID);
unsigned int gic400_getIntGroup(unsigned int ID);
void gic400_enableCPUInterface(void);
void gic400_enableNonSecureCPUInterface(void);
void gic400_disableCPUInterface(void);
void gic400_enableSecureFIQs(void);
void dgic400_isableSecureFIQs(void);
unsigned int gic400_readIntAck(void);
void gic400_writeEOI(unsigned int ID);
unsigned int gic400_readAliasedIntAck(void);
void gic400_writeAliasedEOI(unsigned int ID);
void gic400_setPriorityMask(unsigned int priority);
void gic400_setBinaryPoint(unsigned int priority);
void gic400_setAliasedBinaryPoint(unsigned int priority);
void default_interrupt_routine(void);
void default_fiq_routine(void);

void neon_memcpy(char* dst, char* src, long size);
unsigned long get_uptime_microS();
#ifdef __cplusplus
}
#endif

#endif /*GIC400_H*/
