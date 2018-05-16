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

#ifndef TIMEHELPER_H
#define TIMEHELPER_H

#if defined(__chess__) || defined(__clang__) //APU

//NOTE: the '_CC' and '_US' parameters are ignored because wait(...) does not accept a variable argument :(
#define _wait_cc(_CC) wait(APU_COM_WAIT_CC) 
#define _wait_us(_US) wait(APU_COM_WAIT_CC)

#else //HOST

#include <stdint.h>
#include <common_time_measure.h> //for FSL_Ticks(), FSL_TicksToSeconds(...)
#define RET_TICKS FSL_Ticks()
#define CONVERT_TICKS_TO_US(_TICKS) ((int)(1000000.0*FSL_TicksToSeconds(_TICKS)))

#define _wait_cc(_CC)                                           \
   do {                                                         \
      static int l = 0;                                         \
      uint64_t a = RET_TICKS;                                   \
      for (uint64_t b=RET_TICKS; b<(a+_CC); b=RET_TICKS) {l++;} \
   } while(0)

#define _wait_us(_US)                                                  \
   do {                                                                \
      static int l = 0;                                                \
      uint64_t a = RET_TICKS;                                          \
      for (int t=0; t<(_US); t=CONVERT_TICKS_TO_US(RET_TICKS-a) {l++;} \
   } while(0)

//#if defined(__GNUC__)
//#define USE_USLEEP
//#include <unistd.h>
//assumpition of 500 MHz - 1000 MHz
//#define _wait_cc(_CC)  usleep((((_CC) + 127) >> 7))
//#define _wait_us(_US)  usleep(_US)
      
#endif

#endif /* TIMEHELPER_H */
