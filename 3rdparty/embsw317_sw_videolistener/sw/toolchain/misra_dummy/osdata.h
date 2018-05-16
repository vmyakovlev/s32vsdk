#ifndef OSDATA_H 
#define OSDATA_H
/******************************************************************************
*
*   Freescale(TM) and the Freescale logo are trademarks of Freescale Semiconductor, Inc. 
*   All other product or service names are the property of their respective owners. 
*   (C) Freescale Semiconductor, Inc. 2009-2012
*
*   THIS SOURCE CODE IS CONFIDENTIAL AND PROPRIETARY AND MAY NOT
*   BE USED OR DISTRIBUTED WITHOUT THE WRITTEN PERMISSION OF
*   Freescale Semiconductor, Inc.
*
* Filename:     $Source: /u/OSEK/src/autosar/src/osmpc560xEv3/ssc/inc/osdata.h,v $
* Author:       $Author: b22804 $
* Revision:     $Revision: 1.6 $
*
* History:      Use the CVS command rlog to display revision history
*               information.
*
* Description:  Data for AUTOSAR OS
*
* Notes:
*
******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#ifndef MISRA
    #error Hey, you are including MISRA dummyfile!!! You really do not want it!!!
#endif

/**
* @page misra_violations MISRA-C:2004 violations
*
* @section osdata_h_REF_1
* Violates MISRA 2004 Required Rule 1.2, No reliance should be placed on undefined or unspecified
* behaviour.
* Dummy file -> dummy comment :]
*/

/* Task stacks */
#if (OSNXTSKS > 0) || defined(OSMEMPROTECTION)
/*   
OsStack structure:  
+------------------------------------------------+  &OsStack[0]
|          Protection pad (in SC3..4)            |
+------------------------------------------------+  &OsStack[OSSTACKPROTECTIONPAD/4]
|       Common stack for basic tasks (in SC3..4) |
+------------------------------------------------+  &OsStack[(OSSTACKPROTECTIONPAD + OSCOMMONSTKSIZE)/4]
| Stacks for extended tasks  or for ISRs with MP |
+------------------------------------------------+  &OsStack[(OSSTACKPROTECTIONPAD + OSCOMMONSTKSIZE + OSSTKSIZE)/4]
*/


#define OSSTKBASICSIZE       0ul
#define OSSTACKPROTECTIONPAD 0ul


/* SysGen places stacks for extended tasks from this address */
#define OSRUNNABLESTKBEG &OsStacks[(OSSTKBASICSIZE)/4]

#define OSSTACK_START_SEC_VAR_UNSPECIFIED       /* switch to stack section */
#include "ossections.h"

#if defined(OSSTKSIZE)  /* It is defined if OSNXTSKS > 0 or ( defined(OSMEMPROTECTION) and defined(OSISRENTRYEXIT) ) */
extern OSDWORD OsStacks[(OSSTKSIZE + OSSTKBASICSIZE)/4];
#else
#if (OSSTKBASICSIZE > 0)
extern OSDWORD OsStacks[OSSTKBASICSIZE/4];
#endif
#endif


#define OS_START_SEC_VAR_UNSPECIFIED            /* restore data section */
#include "ossections.h"

#endif /* (OSNXTSKS > 0) || defined(OSMEMPROTECTION) */


/* OS global 'fast' variables */

#define OS_START_SEC_VAR_FAST_UNSPECIFIED
#include "ossections.h"

#if defined(OSTERMINATION)
OSDECLAREVAR(extern OSNEAR OSBYTE, OsKilled_);  /* flags that Task or ISR was killed and some TP context */
#define OsKilled OSUSEVAR(OsKilled_)

#if defined(OSHOOKPROTECTION)
OSDECLAREVAR(extern OSNEAR TaskType, OsViolatorTaskId_);        /* Task Id of task which has violated */
#define OsViolatorTaskId OSUSEVAR(OsViolatorTaskId_)
OSDECLAREVAR(extern OSNEAR ISRType,  OsViolatorISRId_);         /* ISR2 Id of ISR2 which has violated */
#define OsViolatorISRId OSUSEVAR(OsViolatorISRId_)
#endif /* defined(OSHOOKPROTECTION) */
#endif /* defined(OSTERMINATION) */

#if defined(OSORTICURRENTSERVICEID)
OSDECLAREVAR(extern OSNEAR volatile OSBYTE, OsOrtiRunningServiceId_);
#define OsOrtiRunningServiceId OSUSEVAR(OsOrtiRunningServiceId_)
OSDECLAREVAR(extern OSNEAR volatile OSBYTE, OsOrtiOldServiceId_);
#define OsOrtiOldServiceId OSUSEVAR(OsOrtiOldServiceId_)
#endif  /* defined(OSORTICURRENTSERVICEID) */

#if defined(OSCOUNTER)
#if defined(OSALMINCCOUNTER) 
OSDECLAREVAR(extern OSNEAR OSDWORD, OsCtrIncCounter_);
#define OsCtrIncCounter OSUSEVAR(OsCtrIncCounter_)
#endif  /* defined(OSALMINCCOUNTER) */
#endif /* defined(OSCOUNTER) */

#if defined(OSHWCOUNTER)
/** @violates @ref osdata_h_REF_1 MISRA rule 1.2 */
OSDECLAREVAR(extern OSNEAR OSHWTickType, OsTimVal_);
#define OsTimVal OSUSEVAR(OsTimVal_)
#endif

#if defined(OSGETSERVICEID) || defined(OSPARAMETERACCESS)
OSDECLAREVAR(extern OSNEAR OSServiceIdType, OsService_);        /* for OSErrorGetServiceId() from ErrorHook */
#define OsService OSUSEVAR(OsService_)
OSDECLAREVAR(extern OSNEAR OSDWORD,         OsObjId_);          /* for first parameter                      */
#define OsObjId OSUSEVAR(OsObjId_)
#endif /* defined(OSGETSERVICEID) || defined(OSPARAMETERACCESS) */

#if(OSORTIDEBUGLEVEL > 0)
extern OSBYTE OsOrtiRunning;   /* running task for ORTI, it can not be doubled in MultiCore environment */
OSDECLAREVAR(extern OSNEAR StatusType, OsLastError_);
#define OsLastError OSUSEVAR(OsLastError_)
#endif

#if defined(OSISRENTRYEXIT) && defined(OSORTIRUNNINGISRID)
/* ORTI support for tracing ISR */
OSDECLAREVAR(extern OSNEAR volatile OSWORD, OSISRId_);  /* the id of currently running ISR */
#define OSISRId OSUSEVAR(OSISRId_)
#endif

#if defined(OSISRENTRYEXIT)
OSDECLAREVAR(extern OSNEAR OSBYTE, OsSuspendLevel_);    /* level of the nested Suspend/Resume pairs */
#define OsSuspendLevel OSUSEVAR(OsSuspendLevel_)
#endif/* defined(OSISRENTRYEXIT) */
                                            
OSDECLAREVAR(extern OSNEAR OSDWORD, OsOldMsr_);         /* Saved MSR for EnableAll/DisableAllInterrupts */
#define OsOldMsr OSUSEVAR(OsOldMsr_)

OSDECLAREVAR(extern OSNEAR OSINTMASKTYPE, OsOldInterruptMask_); /* Saved interrupt mask for SuspendOS/ResumeOSInterrupts services */
                                                                /* and for Hooks */
#define OsOldInterruptMask OSUSEVAR(OsOldInterruptMask_)

OSDECLAREVAR(extern OSNEAR OSBYTE, OsSuspendLevelAll_);         /* level of the nested Suspend/ResumeAll pairs */
#define OsSuspendLevelAll OSUSEVAR(OsSuspendLevelAll_)

OSDECLAREVAR(extern OSNEAR OSINTMASKTYPE, OsOldIntMaskAll_);    /* SuspendAll/ResumeAllInterrupts services */
#define OsOldIntMaskAll OSUSEVAR(OsOldIntMaskAll_)

#if defined(OSISRENTRYEXIT) && defined (OSHOOKERROR_EXT)
OSDECLAREVAR(extern OSNEAR OSDWORD, OsIntCheck_);       /* check correct order of the nested Suspend/Resume pairs */
#define OsIntCheck OSUSEVAR(OsIntCheck_)
#endif

#if defined(OSUSEISRLEVEL)
OSDECLAREVAR(extern OSNEAR OSISRLEVELTYPE, OsIsrLevel_); /* num of active ISRs & context bits */
#define OsIsrLevel OSUSEVAR(OsIsrLevel_)
#if defined(OSISRENTRYEXIT) || !defined(OSSC1)
OSDECLAREVAR(extern OSNEAR OSDWORD, OsInitialMSR_);     /* saved initial MSR */
#define OsInitialMSR OSUSEVAR(OsInitialMSR_)
#endif
#endif /* defined(OSUSEISRLEVEL) */

#if defined(OSRESOURCEISR)   
OSDECLAREVAR(extern OSNEAR OSWORD, OsISRResourceCounter_);
#define OsISRResourceCounter OSUSEVAR(OsISRResourceCounter_)
#endif

OSDECLAREVAR(extern OSNEAR OSDWORD, OsSchedulerVector1_);       /* Mask of activated tasks(priorities) */
#define OsSchedulerVector1 OSUSEVAR(OsSchedulerVector1_)
#if defined(OSEXTPRIORS)
OSDECLAREVAR(extern OSNEAR OSDWORD, OsSchedulerVector2_);       /* Mask of activated tasks(priorities) Extended vector */
#define OsSchedulerVector2 OSUSEVAR(OsSchedulerVector2_)
#endif  /* defined(OSEXTPRIORS) */

#if defined(OSUSETCB)
OSDECLAREVAR(extern OSNEAR OSTSKCBPTR, OsRunning_);     /* running task (0, if no task is running) */
#else   /* defined(OSUSETCB) */
OSDECLAREVAR(extern OSNEAR OSTASKTYPE, OsRunning_);     /* running task (0, if no task is running) */
#endif  /* defined(OSUSETCB) */
#define OsRunning OSUSEVAR(OsRunning_)

#if (OSNMSGS > 0)
#if defined(OSCOMGETSERVICEID)
OSDECLAREVAR(extern OSNEAR OSDWORD, OsComObjId_);
#define OsComObjId OSUSEVAR(OsComObjId_)
OSDECLAREVAR(extern OSNEAR COMServiceIdType, OsComService_);
#define OsComService OSUSEVAR(OsComService_)
#endif
OSDECLAREVAR(extern OSNEAR COMApplicationModeType, OsCOMmode_);
#define OsCOMmode OSUSEVAR(OsCOMmode_)
#endif /* (OSNMSGS > 0) */

#if defined(OSTIMINGPROTECTION)

/* Note. In some cases TP uses forced software interrupt invoked with MSR(EE)=0
       . But when MSR(EE) will be 1 a low priority interrupt may occurr before 
         TP interrupt because of known INTC "feature". As a result TP handler 
         cannot use OSISRLEVELMASKED to define current TP violator. 
         So special TP object variables are introduced. */

#if (OSNTPBGTS > 0) 
OSDECLAREVAR(extern OSNEAR OSObjectType, OsTPBudgetObject_);    /* TP object (TASK or ISR2) with current budget */
#define OsTPBudgetObject OSUSEVAR(OsTPBudgetObject_)
#endif
#if (OSNTPTSKINTLOCKTIME > 0) || (OSNTPISRINTLOCKTIME > 0)
OSDECLAREVAR(extern OSNEAR OSObjectType, OsTPIntLockObject_);   /* TP object (TASK or ISR2) with current interrupt locking time */
#define OsTPIntLockObject OSUSEVAR(OsTPIntLockObject_)
#endif
#if (OSNTPTSKRESLOCKTIME > 0) || (OSNTPISRRESLOCKTIME > 0)
OSDECLAREVAR(extern OSNEAR OSObjectType, OsTPResLockObject_);   /* TP object (TASK or ISR2) with current resource locking time */
#define OsTPResLockObject OSUSEVAR(OsTPResLockObject_)
#endif
OSDECLAREVAR(extern OSNEAR OSBYTE, OsTPForceReason_);           /* reason of TP forced interrupt */ 
#define OsTPForceReason OSUSEVAR(OsTPForceReason_)
OSDECLAREVAR(extern OSNEAR OSTPTICKTYPE, OsTPTimVal_);          /* current STM count value */ 
#define OsTPTimVal OSUSEVAR(OsTPTimVal_)
#if (OSNTPISRARRIV > 0) || (OSNTPTSKARRIV > 0)
OSDECLAREVAR(extern OSNEAR OSDWORD, OsTPOverflowCnt_);          /* TP overflow counter */
#define OsTPOverflowCnt OSUSEVAR(OsTPOverflowCnt_)
#endif

#if defined(OSHOOKPROTECTION)
OSDECLAREVAR(extern OSNEAR ProtectionReturnType, OsTPHookAction_);       /* Protection hook action desired by user when
                                                                            inter-arrival rate violation occurs */
#define OsTPHookAction OSUSEVAR(OsTPHookAction_)
#endif

#endif /* defined(OSTIMINGPROTECTION) */





#define OS_STOP_SEC_VAR_FAST_UNSPECIFIED
#include "ossections.h"


/*  Global OS variables */

#if defined(OSUSETCB)
/* +1 - for null task */
extern OSTSKCB OsTaskTable[OSNTSKS+1];          /* Task control blocks table */

#else   /* defined(OSUSETCB) */
extern OSBYTE OsTaskStatus[OSNTSKS];            /* Tasks status*/
#if defined(OSINRES)
extern TaskType OsPrio2Task[OSNTSKS];           /* References from priority to task */ 
#endif  /* defined(OSINRES) */
#endif  /* defined(OSUSETCB) */

#if (OSNISR > 0)
extern OS_ISRTYPE OsIsrTable[OSNISR+1];         /* ISR table */
#endif

#if (OSNMSGS > 0)
#if OSNRCVMSGS == 0
#undef OSNRCVMSGS
#define OSNRCVMSGS 1            /* to prevent definition of arrays with size = 0    */
#endif
/**** OS Globals variables - initialized in OSMsgInit   ****/
extern OSMSGCBtype    OsMessages[OSNRCVMSGS];     /* control blocks for receiving messages        */
extern OSMSGCBtype*   OsRcvPtr[OSNSNDMSGS];       /* sending messages pointer to (1st) receiving  */
#endif  /* OSNMSGS > 0 */

#if defined(OSTIMINGPROTECTION)
#define OSNIPLSP  (OSNIPLS+1)
#else 
#define OSNIPLSP  OSNIPLS
#endif

#if OSNIPLSP > 0
#if defined(OSTERMINATION)
extern OSJMP_BUF OsISRbufs[OSNIPLSP];
#endif
extern OS_ISRTYPEPTR OsIsrArray[OSNIPLSP];  /* its index is OsIsrLevel */
#endif /* OSNIPLSP > 0 */

#if defined(OSRESOURCE)
#define OSNALLRES_ OSNALLRES
extern OSRESCB OsResources[OSNALLRES_];
#endif /* defined(OSRESOURCE) */

#if defined(OSCOUNTER)
/*  Configuration Data  */
extern const OSCTR OsCountersCfg[OSNCTRS];
/* Counters table */
extern OSCTRCB   OsCounters[OSNCTRS];
#if defined(OSALMINCCOUNTER) 
extern OSWORD  OsCtrIncValue[OSNCTRS-OSNHWCTRS];    /* Counter increment value inspired by corresponding alarm action */
#endif
#endif

#if (OSNALMS > 0)
#if (OSNUSERALMS > 0) && (OSNSCTALMS > 0)
extern OSALLALARMS OsAllAlarms;
#elif (OSNUSERALMS == 0)
extern OSSCTALMCB  OsSCTAlarms[OSNSCTALMS];
#else
extern OSALMCB  OsAlarms[OSNUSERALMS];
#endif /* (OSNUSERALMS > 0) && (OSNSCTALMS > 0) */
#endif /* (OSNALMS > 0) */

#if defined(OSSCHEDULETABLES)
extern OSSCTCB  OsScheduleTables[OSNSCTS];
#endif

#if defined(OSTIMINGPROTECTION)
/* TP control blocks of resource locking time for tasks */
#if (OSNTPTSKRESLOCKTIME > 0) 
extern OSTPRESLOCK OsTPTskResLockTime[OSNTPTSKRESLOCKS];
#endif /* (OSNTPTSKRESLOCKTIME > 0) */ 

/* TP control blocks of resource locking time for ISR2s */
#if (OSNTPISRRESLOCKTIME > 0)
extern OSTPRESLOCK OsTPISRResLockTime[OSNTPISRRESLOCKS];
#endif /* (OSNTPISRRESLOCKTIME > 0) */ 

#endif /* defined(OSTIMINGPROTECTION) */


#if (OSORTIDEBUGLEVEL > 0) || (OSNAPPMODES > 1)
extern AppModeType OsCurAppMode;
#endif 


#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* OSDATA_H */
