/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2010 All right reserved.
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

#ifndef SHOWMESSAGE_H
#define SHOWMESSAGE_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <locale.h>
#include <stdio.h>

#ifdef _WIN32
   /* Windows Implementation re-directs it to printf */
   #include <stdio.h>
   #define _SHOW_MESSAGE(...) fprintf(stderr, __VA_ARGS__)

   /* Compiler: Visual C++ Check */
   #if defined(_MSC_VER)
      #define __PRETTY_FUNCTION__ __FUNCSIG__
   #endif
#else
   // _SHOW_MESSAGE Definition:
   #define _SHOW_MESSAGE(...) fprintf(stderr, __VA_ARGS__)

   /* Compiler: ARM C++ Check */
   #if defined(__ARMCC_VERSION)
   #define __PRETTY_FUNCTION__ __prettyfunc__
   //#define __FILE__ __MODULE__
   #endif
   #endif

   #if defined(REMOVE_SHOW_ERROR)
      #define SHOW_ERROR(expr)
      #define SHOW_ERROR_STOP(expr)
   #else
      #define SHOW_ERROR(expr)      /*lint -save -e506 -e560 */if(!(expr)) { _SHOW_MESSAGE("%s : %s [%d]\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); } /*lint -restore*/

      #ifdef ATD_AUTOMATION_TEST_ICP_RDK
         // same as SHOW_ERROR
         #define SHOW_ERROR_STOP(expr) /*lint -save -e506 -e560 */if(!(expr)) { _SHOW_MESSAGE("%s : %s [%d]\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); } /*lint -restore*/
      #else
         // instead, really STOP in while loop
         #define SHOW_ERROR_STOP(expr) /*lint -save -e506 -e560 */if(!(expr)) { _SHOW_MESSAGE("%s : %s [%d]\n", __FILE__, __PRETTY_FUNCTION__, __LINE__); while(1);} /*lint -restore*/
      #endif  //#ifdef ATD_AUTOMATION_TEST_ICP_RDK
   #endif   //#if defined(REMOVE_DEBUG_CODE)
#endif /* SHOWMESSAGE_H */

