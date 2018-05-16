/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
* Copyright 2017 NXP 
* All Rights Reserved
*
******************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/

#ifndef SWTREPORT_H
#define SWTREPORT_H

#include <stdio.h>

////////////////////////////////////////////////////////////////////////
/// \defgroup SWT_TOOLS          S/W Verification Tools
////////////////////////////////////////////////////////////////////////
///@{ 

////////////////////////////////////////////////////////////////////////
/// \defgroup SWT_TOOLS_REPORT   S/W Tools - Test Cases Report
////////////////////////////////////////////////////////////////////////
///@{ 

//! Initialize SWT Report
int   SWT_InitTestReport();

//! Print SWT Report
int   SWT_PrintTestReport();

//! Get stack information.
int   SWT_LOG_GetStack();
//! Get heap information.
int   SWT_LOG_GetHeap();
//! Get time.
int   SWT_LOG_GetTime();

int   SWT_LOG_ResetCnt(int apuId);

// (temporary, to be replaced)
#ifndef _MODULE_
#define _MODULE_ "APU2"
#endif
// disable report result database
// #define gSWT_RPT_DBASE (0)

#ifndef gSWT_RPT_DBASE
#define SWT_RPT_DBASE_ADD(_R)                                  \
      do {                                                     \
      if (gSWT_RPT_DBASE)                                      \
      {                                                        \
         ++gSWT_RPT_DBASE_SIZE;                                \
         (gSWT_RPT_DBASE->function = &sSWT_DESC_FUNCTION);     \
         (gSWT_RPT_DBASE->level    = gSWT_RPT_LEVEL);          \
         (gSWT_RPT_DBASE->rval     = (_R));                    \
         (gSWT_RPT_DBASE->exec     = sSWT_TIME);               \
         ++gSWT_RPT_DBASE;                                     \
         if (gSWT_RPT_DBASE >= gSWT_RPT_DBASE_END)             \
         {                                                     \
             gSWT_RPT_DBASE = gSWT_RPT_DBASE_BEG;              \
         }                                                     \
         --gSWT_RPT_LEVEL;                                     \
      }                                                        \
      } while (0)
#else

#define SWT_RPT_DBASE_ADD(_R)

#endif


//! 
#define SWT_LOG_MODULE_REG(_B)                                 \
   static const SWT_RPT_MODULE sSWT_DESC_MODULE =              \
   {                                                           \
      _MODULE_,   /* __MODULE__ is for arm only" */            \
      _B,                                                      \
      __FILE__,                                                \
   }

//!
#define SWT_LOG_MODULE_FILENAME_REG(_B, _F)                    \
   static const SWT_RPT_MODULE sSWT_DESC_MODULE =              \
   {                                                           \
      _MODULE_,   /* __MODULE__ is for arm only" */            \
      _B,         /*module name*/                              \
      _F,         /*file name*/                                \
   }

/*
   hwv_message("%-32.32s\n", __prettyfunc__);
*/
#ifndef __INTEGRITY__
//!
#define SWT_LOG_FUNCTION_REG()                                 \
   if (gSWT_RPT_DBASE) ++gSWT_RPT_LEVEL;                       \
   static       int          sSWT_TIME;                        \
   static const SWT_RPT_FCTN sSWT_DESC_FUNCTION =              \
   {                                                           \
      __func__,                                                \
      (&sSWT_DESC_MODULE),                                     \
   };                                                          \
   sSWT_TIME = SWT_LOG_GetTime()

#else

#define SWT_LOG_FUNCTION_REG()

#endif

#if 1

#ifndef __INTEGRITY__

#define SWT_LOG_FUNCTION_RVAL(_R)                              \
      sSWT_TIME = SWT_LOG_GetTime() - sSWT_TIME;               \
      printf("%-32.32s| %-6.6s| %8i| %6i| %8i| %6i\n",         \
         __func__,                                             \
         ((_R) == 0)?("DONE"):("ERROR"),                       \
         (_R),                                                 \
         sSWT_TIME,                                            \
         0,                                                    \
         SWT_LOG_GetStack());                                  \
      SWT_RPT_DBASE_ADD(_R) 
#else

#define SWT_LOG_FUNCTION_RVAL(_R)

#endif

#else

#define SWT_LOG_FUNCTION_RVAL(_R)                              \
      sSWT_TIME = SWT_LOG_GetTime() - sSWT_TIME;               \
      if (0 == (_R))                                           \
         printf("%-32.32s: DONE\n",                            \
            __func__);                                         \
      else                                                     \
         printf("%-32.32s: ERROR (%i)\n",                      \
            __func__, (_R));                                   \
      hwv_message("   INFO: %6i, %6i\n",                       \
         sSWT_TIME,                                            \
         SWT_LOG_GetStack());                                  \
      SWT_RPT_DBASE_ADD(_R) 

#endif

#define SWT_LOG_EVENT_ADD(_M)                                  \
      if (gSWT_RPT_EVENT)                                      \
      {                                                        \
         (gSWT_RPT_EVENT->opcode    = (_M));                   \
         (gSWT_RPT_EVENT->function  = &sSWT_DESC_FUNCTION);    \
         (gSWT_RPT_EVENT->line      = __LINE__);               \
         ++gSWT_RPT_EVENT;                                     \
      }

#define SWT_LOG_FCTN_BEG()                                     \
    SWT_LOG_EVENT(0x0000)
   
#define SWT_LOG_FCTN_END()                                     \
   SWT_LOG_EVENT(0xFFFF)

/*
#define SWT_LOG_ERROR(_E)                                      \
   SWT_RPT_LOG_EVENT(_E)
*/   
     
#define MODULE_NAME_SIZE      16
#define BLOCK_NAME_SIZE       32
#define FILE_NAME_SIZE       128
#define FUNCTION_NAME_SIZE    64

//! Report Module Function Descriptor
struct SWT_RPT_MODULE
{
   // Replaced structure with fixed length c string
   //    (dangerous and error prone)
   // const char*             module;        //!< Module Name
   // const char*             block;         //!< Module Block Name
   // const char*             file;          //!< Module Filename
   const char             module[MODULE_NAME_SIZE];      //!< Module Name
   const char             block[BLOCK_NAME_SIZE];        //!< Module Block Name
   const char             file[FILE_NAME_SIZE];          //!< Module Filename
};
   
//! Report Test Function Descriptor
struct SWT_RPT_FCTN
{
#ifdef __llvm__
   const char*              function;//[FUNCTION_NAME_SIZE]; //!< Report Test Function Name
#else
   const char              function[FUNCTION_NAME_SIZE]; //!< Report Test Function Name
#endif	
   const SWT_RPT_MODULE*   module;                       //!< Report Test Function Module
   //       int               line;          //!< Report Test Function Line (Number)
   // const char*             time;          //!< Report Test Function Compile Time
};

//! Report Event Logger
struct SWT_RPT_EVENT
{
   int                     opcode;
   const SWT_RPT_FCTN*     function;
   int                     line;
}; 

//! Report Test Function Results
struct SWT_RPT_RESULT
{
   const SWT_RPT_FCTN*     function;      //!< Report Function
   int                     level;         //!< Report Function Level
   int                     rval;          //!< Report Function Return Value / Number of Error(s)
   int                     exec;          //!< Report Function Execution Time (ms)
};

#ifndef gSWT_RPT_EVENT
//! Report Event Logger
extern SWT_RPT_EVENT*   gSWT_RPT_EVENT;
#endif

//! Report Function Database (Level)
extern int              gSWT_RPT_LEVEL;

#ifndef gSWT_RPT_DBASE
//! Report Function Database (Functions)
extern SWT_RPT_RESULT*  gSWT_RPT_DBASE;
extern int              gSWT_RPT_DBASE_SIZE;
extern SWT_RPT_RESULT* gSWT_RPT_DBASE_BEG;
extern SWT_RPT_RESULT* gSWT_RPT_DBASE_END;
#endif

///@} // SWT_TOOLS_REPORT

///@} // SWT_TOOLS

#endif /* SWTREPORT_H */
