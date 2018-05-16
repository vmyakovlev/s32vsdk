/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
#define BUID_LIB 1

#include "swt_arm_report.h"
#ifndef APEX2_EMULATE 
#ifndef __INTEGRITY__
#include <printf.h>
#endif
#else
#include <stdio.h>
#endif
#include <oal.h>
#include "common_time_measure.h"

////////////////////////////////////////////////////////////////////////
// Gloabal Variables
//! Report Event Logger
////////////////////////////////////////////////////////////////////////

#define REG32 uint32_t

#define LOG_DATA_SIZE (1024 * 64)

static bool gswt_initialized = false; 
static unsigned int * SWT_LOG_BASE_ADDRESS = (0x00000000);

#if __GNUG__
#define sprintf_s(buffer, str, ...) sprintf(buffer, str, ##__VA_ARGS__)
#endif
#if !(BUID_LIB == 1)
// Global Variables

int                      gSWT_ARM_DBASE_INITIALIZE = 0;
void*                    gSWT_ARM_DBASE_TBL_OAL = NULL;
SWT_ARM_RESULT*          gSWT_ARM_DBASE_TBL = NULL;
SWT_ARM_RESULT*          gSWT_ARM_DBASE = NULL;     // = gSWT_ARM_DBASE_TBL;
int                      gSWT_ARM_DBASE_SIZE = 0;
SWT_ARM_RESULT*          gSWT_ARM_DBASE_BEG = NULL; // = gSWT_ARM_DBASE_TBL;
SWT_ARM_RESULT*          gSWT_ARM_DBASE_END = NULL; //  = &gSWT_ARM_DBASE_TBL[SWT_ARM_DBASE_TBL_SIZE];
int                      gSWT_TIME = 0;
void*                    gSWT_DESC_MODULE_TBL_OAL = NULL;
SWT_ARM_MODULE*          gSWT_DESC_MODULE_TBL = NULL;
void*                    gSWT_DESC_FUNCTION_TBL_OAL = NULL;
SWT_ARM_FCTN*            gSWT_DESC_FUNCTION_TBL = NULL;
SWT_ARM_MODULE*          gSWT_DESC_MODULE_END = NULL;
SWT_ARM_FCTN*            gSWT_DESC_FUNCTION_END = NULL;
SWT_ARM_MODULE*          gSWT_DESC_MODULE = NULL;
SWT_ARM_FCTN*            gSWT_DESC_FUNCTION = NULL;
#endif

//! Report Event Logger

/*!
   Initialize SWT Report.
 */
#ifdef __LINUX__

int
SWT_ARM_InitTestReport()
{
   int rval = 0;

   gSWT_ARM_DBASE_SIZE  = 0;
   gSWT_ARM_DBASE_TBL_OAL     = OAL_MemoryAllocFlag(SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_RESULT), 
                                                                   OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
   gSWT_ARM_DBASE_TBL     = (SWT_ARM_RESULT*)OAL_MemoryReturnAddress(gSWT_ARM_DBASE_TBL_OAL, ACCESS_CH_WB);
   gSWT_ARM_DBASE_BEG   = &gSWT_ARM_DBASE_TBL[0];
   gSWT_ARM_DBASE_END   = &gSWT_ARM_DBASE_TBL[SWT_ARM_DBASE_TBL_SIZE];
   gSWT_ARM_DBASE       = gSWT_ARM_DBASE_BEG;
   
   gSWT_DESC_MODULE_TBL_OAL     = OAL_MemoryAllocFlag(SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_MODULE), 
                                                                   OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
   gSWT_DESC_MODULE_TBL     = (SWT_ARM_MODULE*)OAL_MemoryReturnAddress(gSWT_DESC_MODULE_TBL_OAL, ACCESS_CH_WB);
   gSWT_DESC_FUNCTION_TBL_OAL   = OAL_MemoryAllocFlag(SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_FCTN), 
                                                                   OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
   gSWT_DESC_FUNCTION_TBL     = (SWT_ARM_FCTN*)OAL_MemoryReturnAddress(gSWT_DESC_FUNCTION_TBL_OAL, ACCESS_CH_WB);

   gSWT_DESC_MODULE         = gSWT_DESC_MODULE_TBL;
   gSWT_DESC_FUNCTION       = gSWT_DESC_FUNCTION_TBL;
   gSWT_DESC_MODULE_END     = &gSWT_DESC_MODULE_TBL[SWT_ARM_DBASE_TBL_SIZE];
   gSWT_DESC_FUNCTION_END   = &gSWT_DESC_FUNCTION_TBL[SWT_ARM_DBASE_TBL_SIZE];
   
   printf("gSWT_ARM_DBASE_SIZE = %6i (0x%08X)\n", sizeof(gSWT_ARM_DBASE_SIZE), (&gSWT_ARM_DBASE_SIZE));
   printf("gSWT_ARM_DBASE      = %6i (0x%08X)\n", sizeof(gSWT_ARM_DBASE),      (&gSWT_ARM_DBASE));
   printf("gSWT_ARM_DBASE_TBL  = %6i (0x%08X)\n", sizeof(gSWT_ARM_DBASE_TBL),  (&gSWT_ARM_DBASE_TBL[0]));
   printf("SWT_ARM_RESULT      = 0x%08X, %8i\n",   gSWT_ARM_DBASE_TBL, SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_RESULT));
   printf("gSWT_DESC_FUNCTION  = 0x%08X, %8i\n\n", gSWT_DESC_FUNCTION, SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_FCTN));
   printf("gSWT_DESC_MODULE    = 0x%08X, %8i\n\n", gSWT_DESC_MODULE,   SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_MODULE));
   gSWT_ARM_DBASE_INITIALIZE = SWT_ARM_DBASE_TBL_SIZE;
   return rval;
}

/*! 
   Print SWT Report
 */
int   
SWT_ARM_PrintTestReport()
{
   int rval = 0;
   
   //*****************************************************
   //gSWT_ARM_DBASE_TBL  = (SWT_ARM_RESULT*)(0x80000000+0x07C10);
   //gSWT_ARM_DBASE      = (SWT_ARM_RESULT*)(0x80000000+0x07C04);
   //gSWT_ARM_DBASE_SIZE = *((int*)(0x80000000+0x07C00));
   //gSWT_ARM_DBASE_BEG  = (SWT_ARM_RESULT*)(0x80000000+0x07C08);
   //gSWT_ARM_DBASE_END  = (SWT_ARM_RESULT*)(0x80000000+0x07C0C);
   //*****************************************************

   printf("gSWT_ARM_DBASE_SIZE       : %6i\n", SWT_ARM_DBASE_TBL_SIZE);
   printf("gSWT_ARM_DBASE            : %6i (%08X)\n", sizeof(gSWT_ARM_DBASE),      (&gSWT_ARM_DBASE));
   printf("gSWT_ARM_DBASE_TBL        : %6i (%08X)\n", sizeof(gSWT_ARM_DBASE_TBL),  (&gSWT_ARM_DBASE_TBL[0]));
   printf("gSWT_ARM_DBASE_SIZE result= %6i\n", gSWT_ARM_DBASE_SIZE);
   printf("gSWT_DESC_FUNCTION        = %08X\n", gSWT_DESC_FUNCTION);
   printf("gSWT_DESC_MODULE          = %08X\n", gSWT_DESC_MODULE);
   
#ifndef gSWT_ARM_DBASE   
   // for (int t=0; (&gSWT_ARM_DBASE_TBL[t]) < gSWT_ARM_DBASE; ++t)
   
   for (int t=1; t <= gSWT_ARM_DBASE_SIZE; t++)
   {
       const int ti = t % SWT_ARM_DBASE_TBL_SIZE;
      //
      const char* module     = gSWT_ARM_DBASE_TBL[ti].function->module->module;
      const char* block      = gSWT_ARM_DBASE_TBL[ti].function->module->block;
      const char* file       = gSWT_ARM_DBASE_TBL[ti].function->module->file;
      const int   src_width  = gSWT_ARM_DBASE_TBL[ti].function->src_width;
      const int   src_height = gSWT_ARM_DBASE_TBL[ti].function->src_height;
      const int   dst_width  = gSWT_ARM_DBASE_TBL[ti].function->dst_width;
      const int   dst_hegiht = gSWT_ARM_DBASE_TBL[ti].function->dst_height;
      const int   params1    = gSWT_ARM_DBASE_TBL[ti].function->params1;
      const int   params2    = gSWT_ARM_DBASE_TBL[ti].function->params2;
      const int   params3    = gSWT_ARM_DBASE_TBL[ti].function->params3;
      const int   params4    = gSWT_ARM_DBASE_TBL[ti].function->params4;
      const int   params5    = gSWT_ARM_DBASE_TBL[ti].function->params5;
      const int   params6    = gSWT_ARM_DBASE_TBL[ti].function->params6;
      //
      const char* function   = gSWT_ARM_DBASE_TBL[ti].function->function;
      const char* note       = gSWT_ARM_DBASE_TBL[ti].function->note;
      const int   level      = gSWT_ARM_DBASE_TBL[ti].level;
      const int   rval       = gSWT_ARM_DBASE_TBL[ti].rval;
      const int   exec       = gSWT_ARM_DBASE_TBL[ti].exec;
      
      char test_name[256];
      if (strlen(note))
      {
         sprintf(test_name, "%s::%s::%s::%s", module, block, file, note);
      }
      else
      {
      sprintf(test_name, "%s::%s::%s", module, block, file);
      }
      printf("[%4i][%2i] %-64.64s| %-6.6s| %8i| %9i| (%5i,%5i)| (%5i,%5i)| %6i, %6i, %6i, %6i, %6i, %6i|\n",
         t,
         level,
         test_name,
         ((rval) == 0)?("PASS"):("FAIL"),
         (rval),
         exec,
         src_width,
         src_height,
         dst_width,
         dst_hegiht,
         params1,
         params2,
         params3,
         params4,
         params5,
         params6
         );   // Stack
      //release memory
      //delete gSWT_ARM_DBASE_TBL[ti].function->module;
      //delete gSWT_ARM_DBASE_TBL[ti].function;
   }
#endif
   OAL_MemoryFree(gSWT_ARM_DBASE_TBL_OAL);
   OAL_MemoryFree(gSWT_DESC_MODULE_TBL_OAL);
   OAL_MemoryFree(gSWT_DESC_FUNCTION_TBL_OAL);
   return rval;
}


#else // __STANDALONE__

int
SWT_ARM_InitTestReport()
{
   int rval = 0;

   gSWT_ARM_DBASE_SIZE  = 0;
   gSWT_ARM_DBASE_TBL   = (SWT_ARM_RESULT*)OAL_MemoryAllocFlag(SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_RESULT), 
                                                                   OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
   gSWT_ARM_DBASE_BEG   = &gSWT_ARM_DBASE_TBL[0];
   gSWT_ARM_DBASE_END   = &gSWT_ARM_DBASE_TBL[SWT_ARM_DBASE_TBL_SIZE];
   gSWT_ARM_DBASE       = gSWT_ARM_DBASE_BEG;
   
   gSWT_DESC_MODULE_TBL     = (SWT_ARM_MODULE*)OAL_MemoryAllocFlag(SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_MODULE), 
                                                                   OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);
   gSWT_DESC_FUNCTION_TBL   = (SWT_ARM_FCTN*)  OAL_MemoryAllocFlag(SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_FCTN), 
                                                                   OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS | OAL_MEMORY_FLAG_ZERO);

   gSWT_DESC_MODULE         = gSWT_DESC_MODULE_TBL;
   gSWT_DESC_FUNCTION       = gSWT_DESC_FUNCTION_TBL;
   gSWT_DESC_MODULE_END     = &gSWT_DESC_MODULE_TBL[SWT_ARM_DBASE_TBL_SIZE];
   gSWT_DESC_FUNCTION_END   = &gSWT_DESC_FUNCTION_TBL[SWT_ARM_DBASE_TBL_SIZE];
   
   DEBUG_printf("gSWT_ARM_DBASE_SIZE = %6i (0x%08X)\n", sizeof(gSWT_ARM_DBASE_SIZE), (&gSWT_ARM_DBASE_SIZE));
   DEBUG_printf("gSWT_ARM_DBASE      = %6i (0x%08X)\n", sizeof(gSWT_ARM_DBASE),      (&gSWT_ARM_DBASE));
   DEBUG_printf("gSWT_ARM_DBASE_TBL  = %6i (0x%08X)\n", sizeof(gSWT_ARM_DBASE_TBL),  (&gSWT_ARM_DBASE_TBL[0]));
   DEBUG_printf("SWT_ARM_RESULT      = 0x%08X, %8i\n",   gSWT_ARM_DBASE_TBL, SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_RESULT));
   DEBUG_printf("gSWT_DESC_FUNCTION  = 0x%08X, %8i\n\n", gSWT_DESC_FUNCTION, SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_FCTN));
   DEBUG_printf("gSWT_DESC_MODULE    = 0x%08X, %8i\n\n", gSWT_DESC_MODULE,   SWT_ARM_DBASE_TBL_SIZE * sizeof (SWT_ARM_MODULE));
   gSWT_ARM_DBASE_INITIALIZE = SWT_ARM_DBASE_TBL_SIZE;
   return rval;
}

/*! 
   Print SWT Report
 */
int   
SWT_ARM_PrintTestReport()
{
   int rval = 0;
   
   //*****************************************************
   //gSWT_ARM_DBASE_TBL  = (SWT_ARM_RESULT*)(0x80000000+0x07C10);
   //gSWT_ARM_DBASE      = (SWT_ARM_RESULT*)(0x80000000+0x07C04);
   //gSWT_ARM_DBASE_SIZE = *((int*)(0x80000000+0x07C00));
   //gSWT_ARM_DBASE_BEG  = (SWT_ARM_RESULT*)(0x80000000+0x07C08);
   //gSWT_ARM_DBASE_END  = (SWT_ARM_RESULT*)(0x80000000+0x07C0C);
   //*****************************************************

   DEBUG_printf("gSWT_ARM_DBASE_SIZE       : %6i\n", SWT_ARM_DBASE_TBL_SIZE);
   DEBUG_printf("gSWT_ARM_DBASE            : %6i (%08X)\n", sizeof(gSWT_ARM_DBASE),      (&gSWT_ARM_DBASE));
   DEBUG_printf("gSWT_ARM_DBASE_TBL        : %6i (%08X)\n", sizeof(gSWT_ARM_DBASE_TBL),  (&gSWT_ARM_DBASE_TBL[0]));
   DEBUG_printf("gSWT_ARM_DBASE_SIZE result= %6i\n", gSWT_ARM_DBASE_SIZE);
   DEBUG_printf("gSWT_DESC_FUNCTION        = %08X\n", gSWT_DESC_FUNCTION);
   DEBUG_printf("gSWT_DESC_MODULE          = %08X\n", gSWT_DESC_MODULE);
   
#ifndef gSWT_ARM_DBASE   
   // for (int t=0; (&gSWT_ARM_DBASE_TBL[t]) < gSWT_ARM_DBASE; ++t)

   for (int t=1; t <= gSWT_ARM_DBASE_SIZE; t++)
   {
       const int ti = t % SWT_ARM_DBASE_TBL_SIZE;
      //
      const char* module     = gSWT_ARM_DBASE_TBL[ti].function->module->module;
      const char* block      = gSWT_ARM_DBASE_TBL[ti].function->module->block;
      const char* file       = gSWT_ARM_DBASE_TBL[ti].function->module->file;
      const int   src_width  = gSWT_ARM_DBASE_TBL[ti].function->src_width;
      const int   src_height = gSWT_ARM_DBASE_TBL[ti].function->src_height;
      const int   dst_width  = gSWT_ARM_DBASE_TBL[ti].function->dst_width;
      const int   dst_hegiht = gSWT_ARM_DBASE_TBL[ti].function->dst_height;
      const int   params1    = gSWT_ARM_DBASE_TBL[ti].function->params1;
      const int   params2    = gSWT_ARM_DBASE_TBL[ti].function->params2;
      const int   params3    = gSWT_ARM_DBASE_TBL[ti].function->params3;
      const int   params4    = gSWT_ARM_DBASE_TBL[ti].function->params4;
      const int   params5    = gSWT_ARM_DBASE_TBL[ti].function->params5;
      const int   params6    = gSWT_ARM_DBASE_TBL[ti].function->params6;
      //
      const char* function   = gSWT_ARM_DBASE_TBL[ti].function->function;
      const char* note       = gSWT_ARM_DBASE_TBL[ti].function->note;
      const int   level      = gSWT_ARM_DBASE_TBL[ti].level;
      const int   rval       = gSWT_ARM_DBASE_TBL[ti].rval;
      const int   exec       = gSWT_ARM_DBASE_TBL[ti].exec;
      
      char test_name[256];
      if (strlen(note))
#ifndef APEX2_EMULATE
      {
         sprintf(test_name, "%s::%s::%s::%s", module, block, file, note);
      }
      else
      {
         sprintf(test_name, "%s::%s::%s", module, block, file);
      }
#else
      {
         sprintf_s(test_name, "%s::%s::%s::%s", module, block, file, note);
      }
      else
      {
         sprintf_s(test_name, "%s::%s::%s", module, block, file);
      }
#endif
      DEBUG_printf("[%4i][%2i] %-64.64s| %-6.6s| %8i| %9i| (%5i,%5i)| (%5i,%5i)| %6i, %6i, %6i, %6i, %6i, %6i|\n",
         t,
         level,
         test_name,
         ((rval) == 0)?("PASS"):("FAIL"),
         (rval),
         exec,
         src_width,
         src_height,
         dst_width,
         dst_hegiht,
         params1,
         params2,
         params3,
         params4,
         params5,
         params6
         );   // Stack
      //release memory
      //delete gSWT_ARM_DBASE_TBL[ti].function->module;
      //delete gSWT_ARM_DBASE_TBL[ti].function;
   }
#endif
   OAL_MemoryFree(gSWT_ARM_DBASE_TBL);
   OAL_MemoryFree(gSWT_DESC_MODULE_TBL);
   OAL_MemoryFree(gSWT_DESC_FUNCTION_TBL);
   return rval;
}
#endif  //#ifdef __LINUX__

/*!
   Return stack information.
 */
int 
SWT_ARM_LOG_GetStack()
{
   // return OAL_GetStackInformation(0, 0);
   return 0;
}

/*!
   Return heap information.
 */
int
SWT_ARM_LOG_GetHeap()
{
   // return OAL_GetUsedHeapSpace();
   return 0;
}

/*!
   Return time information.
 */
int
SWT_ARM_LOG_GetTime()
{
   #ifdef __STANDALONE__
      return FSL_Ticks();
   #else
      return (int) (FSL_Ticks() /1000);
   #endif      
}

/*!
   Rest performance counter.
 */
int
SWT_ARM_LOG_ResetCnt()
{
   gSWT_ARM_DBASE_SIZE = 0;
   gSWT_ARM_DBASE_BEG  = &gSWT_ARM_DBASE_TBL[0];
   gSWT_ARM_DBASE_END  = &gSWT_ARM_DBASE_TBL[SWT_ARM_DBASE_TBL_SIZE];
   gSWT_ARM_DBASE      = gSWT_ARM_DBASE_BEG;
   gSWT_ARM_LEVEL      = 0;
   return 0;
}
