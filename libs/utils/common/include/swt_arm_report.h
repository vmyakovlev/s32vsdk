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

#ifndef SWTARMREPORT_H
#define SWTARMREPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oal.h>
#include <t32term.h>

#ifdef APEX2_EMULATE
#ifndef __func__
#define __func__ __FUNCTION__
#endif

#endif

#ifdef __INTEGRITY__
#ifndef __func__
#define __func__ __FUNCTION__
#endif

#endif

////////////////////////////////////////////////////////////////////////
/// \defgroup SWT_TOOLS          S/W Verification Tools
////////////////////////////////////////////////////////////////////////
///@{ 
// (temporary, to be replaced)

/*
#define SWT_ARM_LOG_ERROR(_E)                                  \
   SWT_ARM_LOG_EVENT(_E)
*/   
//typedef unsigned long uint64_t;
//typedef unsigned int  uint32_t;
//typedef unsigned long size_t;
     
#define MODULE_NAME_SIZE      32
#define BLOCK_NAME_SIZE       48
#define FILE_NAME_SIZE       128
#define FUNCTION_NAME_SIZE    64
#define SUBJECT_NAME_SIZE     64
#define NOTE_NAME_SIZE        32

//! Report Module Function Descriptor
struct SWT_ARM_MODULE
{
   // Replaced structure with fixed length c std::string
   //    (dangerous and error prone)
   // const char*             module;        //!< Module Name
   // const char*             block;         //!< Module Block Name
   // const char*             file;          //!< Module Filename
   char              module[MODULE_NAME_SIZE];      //!< Module Name
   char              block[BLOCK_NAME_SIZE];        //!< Module Block Name
   char              file[FILE_NAME_SIZE];          //!< Module Filename
};

   
//! Report Test Function Descriptor
struct SWT_ARM_FCTN
{
   char                 function[FUNCTION_NAME_SIZE]; //!< Report Test Function Name
   char                 note[NOTE_NAME_SIZE];      //!< Report Test SUBJECT Name
   SWT_ARM_MODULE*      module;                       //!< Report Test Module
   int                  src_width;
   int                  src_height;
   int                  dst_width;
   int                  dst_height;
   int                  params1;
   int                  params2;
   int                  params3;
   int                  params4;   
   int                  params5;
   int                  params6;
   //       int               line;          //!< Report Test Function Line (Number)
   // const char*             time;          //!< Report Test Function Compile Time
};

//! Report Event Logger
struct SWT_ARM_EVENT
{
   int                  opcode;
   const SWT_ARM_FCTN*  function;
   int                  line;
}; 

//! Report Test Function Results
struct SWT_ARM_RESULT
{
   SWT_ARM_FCTN*  function;      //!< Report Function
   int                  level;         //!< Report Function Level
   int                  rval;          //!< Report Function Return Value / Number of Error(s)
   int                  exec;          //!< Report Function Execution Time (ms)
};

#ifndef _MODULE_
#define _MODULE_ "APEXCV"
#endif

// disable report result database
// #define gSWT_ARM_DBASE (1)

#if (BUID_LIB == 1)
SWT_ARM_EVENT*           gSWT_ARM_EVENT = 0;
#else
extern SWT_ARM_EVENT*    gSWT_ARM_EVENT;
//! Report Function Database (Level)
#endif
#if (BUID_LIB == 1)
int                      gSWT_ARM_LEVEL  = 0;
#else
extern int               gSWT_ARM_LEVEL;
#endif

//! Report Function Database (Function)
#define SWT_ARM_DBASE_TBL_SIZE   50000

#if (BUID_LIB == 1)
int                      gSWT_ARM_DBASE_INITIALIZE = 0;
SWT_ARM_RESULT*          gSWT_ARM_DBASE_TBL;
void*          gSWT_ARM_DBASE_TBL_OAL;
SWT_ARM_RESULT*          gSWT_ARM_DBASE;     // = gSWT_ARM_DBASE_TBL;
int                      gSWT_ARM_DBASE_SIZE = 0;
SWT_ARM_RESULT*          gSWT_ARM_DBASE_BEG; // = gSWT_ARM_DBASE_TBL;
SWT_ARM_RESULT*          gSWT_ARM_DBASE_END; //  = &gSWT_ARM_DBASE_TBL[SWT_ARM_DBASE_TBL_SIZE];
uint64_t                 gSWT_TIME =0;
SWT_ARM_MODULE*          gSWT_DESC_MODULE_TBL;
void*          gSWT_DESC_MODULE_TBL_OAL;
SWT_ARM_FCTN*            gSWT_DESC_FUNCTION_TBL;
void*            gSWT_DESC_FUNCTION_TBL_OAL;
SWT_ARM_MODULE*          gSWT_DESC_MODULE_END;
SWT_ARM_FCTN*            gSWT_DESC_FUNCTION_END;
SWT_ARM_MODULE*          gSWT_DESC_MODULE;
SWT_ARM_FCTN*            gSWT_DESC_FUNCTION;
#else
extern int               gSWT_ARM_DBASE_INITIALIZE;
extern SWT_ARM_RESULT    gSWT_ARM_DBASE_TBL[]; //SWT_ARM_RESULT*   gSWT_ARM_DBASE_TBL;
extern int               gSWT_ARM_DBASE_SIZE;
extern SWT_ARM_RESULT*   gSWT_ARM_DBASE;
extern SWT_ARM_RESULT*   gSWT_ARM_DBASE_BEG;
extern SWT_ARM_RESULT*   gSWT_ARM_DBASE_END;
extern uint64_t          gSWT_TIME;
extern SWT_ARM_MODULE*   gSWT_DESC_MODULE_TBL;
extern SWT_ARM_FCTN*     gSWT_DESC_FUNCTION_TBL;
extern SWT_ARM_MODULE*   gSWT_DESC_MODULE_END;
extern SWT_ARM_FCTN*     gSWT_DESC_FUNCTION_END;
extern SWT_ARM_MODULE*   gSWT_DESC_MODULE;
extern SWT_ARM_FCTN*     gSWT_DESC_FUNCTION;
#endif

#if defined(__APEXCV_PROFILE__) && !defined(APEX2_EMULATE)
   #define SWT_ARM_DBASE_ADD(_R)\
do {                                                        \
      if (gSWT_ARM_DBASE)                                      \
      {                                                        \
         ++gSWT_ARM_DBASE_SIZE;                                \
         ++gSWT_ARM_DBASE;                                     \
         (gSWT_ARM_DBASE->function = gSWT_DESC_FUNCTION);      \
         (gSWT_ARM_DBASE->level    = gSWT_ARM_LEVEL);          \
         (gSWT_ARM_DBASE->rval     = (_R));                    \
         (gSWT_ARM_DBASE->exec     = gSWT_TIME);               \
         if (gSWT_ARM_DBASE >= gSWT_ARM_DBASE_END)             \
         {                                                     \
            gSWT_ARM_DBASE     = gSWT_ARM_DBASE_BEG;           \
         }                                                     \
         --gSWT_ARM_LEVEL;                                     \
      }                                                        \
   } while (0)

   #define SWT_ARM_DBASE_UPDATE(_R, _NOTE_)\
do {                                                                          \
      if ((gSWT_ARM_DBASE) && (gSWT_ARM_DBASE > gSWT_ARM_DBASE_BEG))          \
      {                                                                       \
         SWT_ARM_RESULT* cSWT_ARM_DBASE = gSWT_ARM_DBASE;                     \
         cSWT_ARM_DBASE;                                                    \
         memcpy(cSWT_ARM_DBASE->function->note, _NOTE_, NOTE_NAME_SIZE);     \
         (cSWT_ARM_DBASE->rval     = (_R));                                   \
      }                                                                       \
   } while (0)

#else
#define SWT_ARM_DBASE_ADD(_R)
#endif //#ifndef SWT_ARM_DBASE_ADD(_R)

////////////////////////////////////////////////////////////////////////
/// \defgroup SWT_TOOLS_REPORT   S/W Tools - Test Cases Report
////////////////////////////////////////////////////////////////////////
///@{ 

//! Initialize SWT Report
int   SWT_ARM_InitTestReport();

//! Print SWT Report
int   SWT_ARM_PrintTestReport();

//! Get stack information.
int   SWT_ARM_LOG_GetStack();
//! Get heap information.
int   SWT_ARM_LOG_GetHeap();
//! Get time.
int   SWT_ARM_LOG_GetTime();

int   SWT_ARM_LOG_ResetCnt();

#if defined(__APEXCV_PROFILE__) && !defined(APEX2_EMULATE)
//! 
#define SWT_LOG_MODULE_REG(_B)                                     \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                                \
      ++gSWT_DESC_MODULE;                                             \
      if (gSWT_DESC_MODULE >= gSWT_DESC_MODULE_END)                   \
      {                                                                \
         gSWT_DESC_MODULE   = gSWT_DESC_MODULE_TBL;                   \
      }                                                               \
      memcpy (gSWT_DESC_MODULE->module, _MODULE_, MODULE_NAME_SIZE); \
      memcpy (gSWT_DESC_MODULE->block, _B, BLOCK_NAME_SIZE);              \
      memcpy (gSWT_DESC_MODULE->file, __FILE__, sizeof (__FILE__));   \
   }                                                                                \


//!

#define SWT_ARM_LOG_MODULE_FILENAME_REG(_B, _F)                    \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                                \
      ++gSWT_DESC_MODULE;                                             \
      if (gSWT_DESC_MODULE >= gSWT_DESC_MODULE_END)                   \
      {                                                               \
         gSWT_DESC_MODULE   = gSWT_DESC_MODULE_TBL;                   \
      }                                                               \
      memcpy (gSWT_DESC_MODULE->module, _MODULE_, MODULE_NAME_SIZE); \
      memcpy (gSWT_DESC_MODULE->block, _B, BLOCK_NAME_SIZE);              \
      memcpy (gSWT_DESC_MODULE->file, _F, FUNCTION_NAME_SIZE);               \
   }                                                                                \

/*
   hwv_message("%-32.32s\n", __prettyfunc__);
*/

//!

#define SWT_ARM_LOG_FUNCTION_REG()                                      \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                                \
      if (gSWT_ARM_DBASE) ++gSWT_ARM_LEVEL;                                \
      ++gSWT_DESC_FUNCTION;                                                \
      if (gSWT_DESC_FUNCTION >= gSWT_DESC_FUNCTION_END)                    \
      {                                                                    \
         gSWT_DESC_FUNCTION   = gSWT_DESC_FUNCTION_TBL;                    \
      }                                                                    \
      memcpy (gSWT_DESC_FUNCTION->function, __func__, FUNCTION_NAME_SIZE); \
      gSWT_DESC_FUNCTION->module = gSWT_DESC_MODULE;                       \
   }                                                                                \
   gSWT_TIME = SWT_ARM_LOG_GetTime()                              


#define SWT_ARM_LOG_NOTE_FUNCTION_REG(_Note_)                                      \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                                \
      if (gSWT_ARM_DBASE) ++gSWT_ARM_LEVEL;                                \
      ++gSWT_DESC_FUNCTION;                                                \
      if (gSWT_DESC_FUNCTION >= gSWT_DESC_FUNCTION_END)                    \
      {                                                                    \
         gSWT_DESC_FUNCTION   = gSWT_DESC_FUNCTION_TBL;                    \
      }                                                                    \
      memcpy (gSWT_DESC_FUNCTION->function, __func__, FUNCTION_NAME_SIZE); \
      memcpy (gSWT_DESC_FUNCTION->note, _Note_, NOTE_NAME_SIZE);       \
      gSWT_DESC_FUNCTION->module = gSWT_DESC_MODULE;                       \
      gSWT_DESC_FUNCTION->src_width  = 0;                                  \
      gSWT_DESC_FUNCTION->src_height = 0;                                  \
      gSWT_DESC_FUNCTION->dst_width  = 0;                                  \
      gSWT_DESC_FUNCTION->dst_height = 0;                                  \
      gSWT_DESC_FUNCTION->params1    = 0;                                  \
      gSWT_DESC_FUNCTION->params2    = 0;                                  \
      gSWT_DESC_FUNCTION->params3    = 0;                                  \
      gSWT_DESC_FUNCTION->params4    = 0;                                  \
      gSWT_DESC_FUNCTION->params5    = 0;                                  \
      gSWT_DESC_FUNCTION->params6    = 0;                                  \
   }                                                                                \
   gSWT_TIME = SWT_ARM_LOG_GetTime()                              


#define SWT_ARM_LOG_IMAGE_SIZE_FUNCTION_REG(_sW, _sH, _dW, _dH, _P1, _P2, _P3, _P4, _P5, _P6) \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                       \
      ++gSWT_DESC_FUNCTION;                                                \
      if (gSWT_DESC_FUNCTION >= gSWT_DESC_FUNCTION_END)                    \
      {                                                                    \
         gSWT_DESC_FUNCTION   = gSWT_DESC_FUNCTION_TBL;                    \
      }                                                                    \
      if (gSWT_ARM_DBASE) ++gSWT_ARM_LEVEL;                                \
      memcpy (gSWT_DESC_FUNCTION->function, __func__, FUNCTION_NAME_SIZE); \
      gSWT_DESC_FUNCTION->module = gSWT_DESC_MODULE;                       \
      gSWT_DESC_FUNCTION->src_width  = _sW;                                \
      gSWT_DESC_FUNCTION->src_height = _sH;                                \
      gSWT_DESC_FUNCTION->dst_width  = _dW;                                \
      gSWT_DESC_FUNCTION->dst_height = _dH;                                \
      gSWT_DESC_FUNCTION->params1    = _P1;                                \
      gSWT_DESC_FUNCTION->params2    = _P2;                                \
      gSWT_DESC_FUNCTION->params3    = _P3;                                \
      gSWT_DESC_FUNCTION->params4    = _P4;                                \
      gSWT_DESC_FUNCTION->params5    = _P5;                                \
      gSWT_DESC_FUNCTION->params6    = _P6;                                \
   }                                                                       \
   gSWT_TIME = SWT_ARM_LOG_GetTime()                              

#ifndef __STANDALONE__
#define SWT_ARM_LOG_FUNCTION_RVAL(_R)                           \
   gSWT_TIME = SWT_ARM_LOG_GetTime() - gSWT_TIME;               \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                     \
      printf("%16s::%32s::%32s| %-6.6s| %8i| %8i| %8i| %6i\n",     \
      gSWT_DESC_FUNCTION->module->module,                       \
      gSWT_DESC_FUNCTION->module->block,                        \
      gSWT_DESC_FUNCTION->module->file,                         \
      ((_R) == 0)?("PASS"):("FAIL"),                           \
      (_R),                                                     \
      gSWT_TIME,                                                \
      0,                                                        \
      SWT_ARM_LOG_GetStack());                                 \
      SWT_ARM_DBASE_ADD(_R);                                       \
   } else                                                                    \
   {                                                                          \
      printf("%32s|%-6.6s| %8i| %8i| \n",     \
         __func__  ,                                \
         ((_R) == 0)?("PASS"):("FAIL"),                           \
         (_R),                                                     \
         gSWT_TIME);                                              \
   }                                                                           \
   gSWT_TIME = SWT_ARM_LOG_GetTime();

#define SWT_ARM_LOG_FUNCTION_RVAL_UPDATE(_R, _NOTE_)            \
   gSWT_TIME = SWT_ARM_LOG_GetTime() - gSWT_TIME;               \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                                     \
      printf("%16s::%32s::%32s| %-6.6s| %8i| %8i| %8i| %6i\n",     \
         gSWT_DESC_FUNCTION->module->module,                       \
         gSWT_DESC_FUNCTION->module->block,                        \
         gSWT_DESC_FUNCTION->module->file,                         \
         ((_R) == 0)?("PASS"):("FAIL"),                           \
         (_R),                                                     \
         gSWT_TIME,                                                \
         0,                                                        \
         SWT_ARM_LOG_GetStack());                                 \
   }else                                                                   \
   {                                                                          \
      printf("%-6.6s| %8i| %8i| \n",     \
         ((_R) == 0)?("PASS"):("FAIL"),                           \
         (_R),                                                     \
         gSWT_TIME);                                              \
   }                                                                           \
   SWT_ARM_DBASE_UPDATE(_R, _NOTE_);                            \
   gSWT_TIME = SWT_ARM_LOG_GetTime(); 

#else
#define SWT_ARM_LOG_FUNCTION_RVAL(_R)                          \
   gSWT_TIME = SWT_ARM_LOG_GetTime() - gSWT_TIME;              \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                   \
      DEBUG_printf("%16s::%32s::%32s| %-6.6s| %8i| %8i| %8i| %6i\n", \
         gSWT_DESC_FUNCTION->module->module,                         \
         gSWT_DESC_FUNCTION->module->block,                          \
         gSWT_DESC_FUNCTION->module->file,                           \
         ((_R) == 0)?("PASS"):("FAIL"),                           \
         (_R),                                                    \
         gSWT_TIME,                                               \
         0,                                                       \
         SWT_ARM_LOG_GetStack());                                 \
      SWT_ARM_DBASE_ADD(_R);                                      \
   } else                                                                    \
   {                                                                          \
      DEBUG_printf("%32s|%-6.6s| %8i| %8i| \n",     \
         __func__ ,                                 \
         ((_R) == 0)?("PASS"):("FAIL"),                           \
         (_R),                                                     \
         gSWT_TIME);                                              \
   }                                                                           \
   gSWT_TIME = SWT_ARM_LOG_GetTime();                          \

#define SWT_ARM_LOG_FUNCTION_RVAL_UPDATE(_R, _NOTE_)           \
   gSWT_TIME = SWT_ARM_LOG_GetTime() - gSWT_TIME;              \
   if (gSWT_ARM_DBASE_INITIALIZE != 0)                                   \
   {                                                                                     \
      DEBUG_printf("%16s::%32s::%32s| %-6.6s| %8i| %8i| %8i| %6i\n",             \
         gSWT_DESC_FUNCTION->module->module,                       \
         gSWT_DESC_FUNCTION->module->block,                        \
         gSWT_DESC_FUNCTION->module->file,                         \
         ((_R) == 0)?("PASS"):("FAIL"),                           \
         (_R),                                                    \
         gSWT_TIME,                                               \
         0,                                                       \
         SWT_ARM_LOG_GetStack());                                 \
      SWT_ARM_DBASE_UPDATE(_R, _NOTE_);                           \
   } else                                                                    \
   {                                                                          \
      DEBUG_printf("%-6.6s| %8i| %8i| \n",     \
         ((_R) == 0)?("PASS"):("FAIL"),                           \
         (_R),                                                     \
         gSWT_TIME);                                              \
   }                                                                           \
   gSWT_TIME = SWT_ARM_LOG_GetTime();                             
#endif

   

#define SWT_ARM_LOG_EVENT_ADD(_M)                              \
   if (gSWT_ARM_EVENT)                                         \
   {                                                           \
      (gSWT_ARM_EVENT->opcode    = (_M));                      \
      (gSWT_ARM_EVENT->function  = gSWT_DESC_FUNCTION;         \
      (gSWT_ARM_EVENT->line      = __LINE__);                  \
      ++gSWT_ARM_EVENT;                                        \
   }

#define SWT_ARM_LOG_FCTN_BEG()                                 \
   SWT_ARM_LOG_EVENT(0x0000)
   
#define SWT_ARM_LOG_FCTN_END()                                 \
   SWT_ARM_LOG_EVENT(0xFFFF)
#else
#define SWT_ARM_LOG_FUNCTION_REG()
#define SWT_ARM_LOG_NOTE_FUNCTION_REG(_Note_)
#define SWT_ARM_LOG_IMAGE_SIZE_FUNCTION_REG(_sW, _sH, _dW, _dH, _P1, _P2, _P3, _P4, _P5, _P6)
#define SWT_LOG_MODULE_REG(_B)
#define SWT_ARM_LOG_MODULE_FILENAME_REG(_B, _F)
#define SWT_ARM_LOG_FUNCTION_RVAL(_R)
#define SWT_ARM_LOG_FUNCTION_RVAL_UPDATE(_R, _NOTE_) 
#define SWT_ARM_LOG_EVENT_ADD(_M)                              
#define SWT_ARM_LOG_FCTN_END()                                
#define SWT_ARM_LOG_FUNCTION_RVAL(_R)
#define SWT_ARM_LOG_FUNCTION_RVAL_UPDATE(_R, _NOTE_)

#endif   //#ifndef SWT_LOG_MODULE_REG(_B) 


///@} // SWT_TOOLS_REPORT

///@} // SWT_TOOLS

#endif /* SWTARMREPORT_H */
