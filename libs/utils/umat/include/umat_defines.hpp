/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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

/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2014, Itseez Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentationUMatDataAutoLock
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/
#ifndef UMATDEFINES_HPP
#define UMATDEFINES_HPP

#ifdef __INTEGRITY__
  #define INT32_MIN (-2147483647-1)
  #define INT32_MAX   (2147483647)
#endif
/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#define VSDK_UMAT_USE_OPENCV
#ifdef VSDK_UMAT_USE_OPENCV
#include <opencv2/opencv.hpp>
#endif

#include <stdint.h>
/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
namespace vsdk
{  
  #if defined _MSC_VER && !defined RC_INVOKED
    #include <intrin.h>
    #define VSDK_CV_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
  #elif defined __INTEGRITY__
      inline int32_t  VSDK_CV_XADD(int32_t*  addr, int32_t  delta) { int32_t  tmp = *addr; *addr += delta; return tmp; }
      inline uint32_t VSDK_CV_XADD(uint32_t* addr, uint32_t delta) { uint32_t tmp = *addr; *addr += delta; return tmp; }
  #else
      #define VSDK_CV_XADD(addr, delta)    (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
  #endif
  
  #define OAL_USAGE_CACHED 0x10000000
  #define OAL_USAGE_NONCACHED 0x20000000
  #define OAL_USAGE_FORCE_NOTFLUSH 0x40000000
  
  #define VSDK_CV_MAT_CONT_FLAG_SHIFT  14
  #define VSDK_CV_MAT_CONT_FLAG        (1 << VSDK_CV_MAT_CONT_FLAG_SHIFT)
  #define VSDK_CV_IS_MAT_CONT(flags)   ((flags) & VSDK_CV_MAT_CONT_FLAG)
  #define VSDK_CV_IS_CONT_MAT          VSDK_CV_IS_MAT_CONT
  #define VSDK_CV_SUBMAT_FLAG_SHIFT    15
  #define VSDK_CV_SUBMAT_FLAG          (1 << VSDK_CV_SUBMAT_FLAG_SHIFT)
  
  #define VSDK_CV_AUTOSTEP             0x7fffffff
  
  #define VSDK_CV_MAX_DIM              32
  #define VSDK_CV_CN_MAX               512
  #define VSDK_CV_CN_SHIFT             3
  #define VSDK_CV_DEPTH_MAX            (1 << VSDK_CV_CN_SHIFT)
  
  #define VSDK_CV_MAT_DEPTH_MASK       (VSDK_CV_DEPTH_MAX - 1)
  #define VSDK_CV_MAT_DEPTH(flags)     ((flags) & VSDK_CV_MAT_DEPTH_MASK)
  
  #define VSDK_CV_MAT_CN_MASK          ((VSDK_CV_CN_MAX - 1) << VSDK_CV_CN_SHIFT)
  #define VSDK_CV_MAT_CN(flags)        ((((flags) & VSDK_CV_MAT_CN_MASK) >> VSDK_CV_CN_SHIFT) + 1)
  
  #define VSDK_CV_ELEM_SIZE(type)      (VSDK_CV_MAT_CN(type) << ((((sizeof(size_t)/4+1)*16384|0x3a50) >> VSDK_CV_MAT_DEPTH(type)*2) & 3))
  #define VSDK_CV_ELEM_SIZE1(type)     ((((sizeof(size_t)<<28)|0x8442211) >> VSDK_CV_MAT_DEPTH(type)*4) & 15)
  
  #define VSDK_CV_MAT_TYPE_MASK        (VSDK_CV_DEPTH_MAX*VSDK_CV_CN_MAX - 1)
  #define VSDK_CV_MAT_TYPE(flags)      ((flags) & VSDK_CV_MAT_TYPE_MASK)
  
  #define VSDK_CV_MAKETYPE(depth,cn)   (VSDK_CV_MAT_DEPTH(depth) + (((cn)-1) << VSDK_CV_CN_SHIFT))
  
  #define VSDK_CV_8U   0
  #define VSDK_CV_8S   1
  #define VSDK_CV_16U  2
  #define VSDK_CV_16S  3
  #define VSDK_CV_32S  4
  #define VSDK_CV_32F  5
  #define VSDK_CV_64F  6
  #define VSDK_CV_USRTYPE1 7
  
  #define VSDK_CV_8UC1 VSDK_CV_MAKETYPE(VSDK_CV_8U,1)
  #define VSDK_CV_8UC2 VSDK_CV_MAKETYPE(VSDK_CV_8U,2)
  #define VSDK_CV_8UC3 VSDK_CV_MAKETYPE(VSDK_CV_8U,3)
  #define VSDK_CV_8UC4 VSDK_CV_MAKETYPE(VSDK_CV_8U,4)
  #define VSDK_CV_8UC(n) VSDK_CV_MAKETYPE(VSDK_CV_8U,(n))

  #define VSDK_CV_8SC1 VSDK_CV_MAKETYPE(VSDK_CV_8S,1)
  #define VSDK_CV_8SC2 VSDK_CV_MAKETYPE(VSDK_CV_8S,2)
  #define VSDK_CV_8SC3 VSDK_CV_MAKETYPE(VSDK_CV_8S,3)
  #define VSDK_CV_8SC4 VSDK_CV_MAKETYPE(VSDK_CV_8S,4)
  #define VSDK_CV_8SC(n) VSDK_CV_MAKETYPE(VSDK_CV_8S,(n))

  #define VSDK_CV_16UC1 VSDK_CV_MAKETYPE(VSDK_CV_16U,1)
  #define VSDK_CV_16UC2 VSDK_CV_MAKETYPE(VSDK_CV_16U,2)
  #define VSDK_CV_16UC3 VSDK_CV_MAKETYPE(VSDK_CV_16U,3)
  #define VSDK_CV_16UC4 VSDK_CV_MAKETYPE(VSDK_CV_16U,4)
  #define VSDK_CV_16UC(n) VSDK_CV_MAKETYPE(VSDK_CV_16U,(n))

  #define VSDK_CV_16SC1 VSDK_CV_MAKETYPE(VSDK_CV_16S,1)
  #define VSDK_CV_16SC2 VSDK_CV_MAKETYPE(VSDK_CV_16S,2)
  #define VSDK_CV_16SC3 VSDK_CV_MAKETYPE(VSDK_CV_16S,3)
  #define VSDK_CV_16SC4 VSDK_CV_MAKETYPE(VSDK_CV_16S,4)
  #define VSDK_CV_16SC(n) VSDK_CV_MAKETYPE(VSDK_CV_16S,(n))

  #define VSDK_CV_32SC1 VSDK_CV_MAKETYPE(VSDK_CV_32S,1)
  #define VSDK_CV_32SC2 VSDK_CV_MAKETYPE(VSDK_CV_32S,2)
  #define VSDK_CV_32SC3 VSDK_CV_MAKETYPE(VSDK_CV_32S,3)
  #define VSDK_CV_32SC4 VSDK_CV_MAKETYPE(VSDK_CV_32S,4)
  #define VSDK_CV_32SC(n) VSDK_CV_MAKETYPE(VSDK_CV_32S,(n))

  #define VSDK_CV_32FC1 VSDK_CV_MAKETYPE(VSDK_CV_32F,1)
  #define VSDK_CV_32FC2 VSDK_CV_MAKETYPE(VSDK_CV_32F,2)
  #define VSDK_CV_32FC3 VSDK_CV_MAKETYPE(VSDK_CV_32F,3)
  #define VSDK_CV_32FC4 VSDK_CV_MAKETYPE(VSDK_CV_32F,4)
  #define VSDK_CV_32FC(n) VSDK_CV_MAKETYPE(VSDK_CV_32F,(n))

  #define VSDK_CV_64FC1 VSDK_CV_MAKETYPE(VSDK_CV_64F,1)
  #define VSDK_CV_64FC2 VSDK_CV_MAKETYPE(VSDK_CV_64F,2)
  #define VSDK_CV_64FC3 VSDK_CV_MAKETYPE(VSDK_CV_64F,3)
  #define VSDK_CV_64FC4 VSDK_CV_MAKETYPE(VSDK_CV_64F,4)
  #define VSDK_CV_64FC(n) VSDK_CV_MAKETYPE(VSDK_CV_64F,(n))
} /* namespace vsdk */

#endif /* UMATDEFINES_HPP */
