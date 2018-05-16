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
#ifndef UMATMUTEX_HPP
#define UMATMUTEX_HPP

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <stdint.h>

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
namespace vsdk
{
  /*================================================================================================*/
  /**
  * @brief          Mutex class used in UMat structures keeping an eye on mutual exclusivness
  */
  class Mutex
  {
    public:
      /**
      * @brief          Constructor, creates and initializes the mutex
      */
      Mutex();
      
      /**
      * @brief          Destructor
      */
      ~Mutex();
      
      /**
      * @brief          Constructor, creates and initializes the mutex based on existing one
      *
      * @param[in]      m            An existing Mutex instance
      */
      Mutex(const vsdk::Mutex& m);
      
      /**
      * @brief          Assignment operator
      *
      * @param[in]      m            An existing Mutex instance
      * 
      * @return         New instance to be assigned
      */
      Mutex& operator = (const vsdk::Mutex& m);

      /**
      * @brief          Obtain the mutex
      */
      void lock();
      
      /**
      * @brief          Try to obtain the mutex.
      * 
      * @return         True if the lock was obtained.
      */
      int8_t trylock();
      
      /**
      * @brief          Free the mutex
      */
      void unlock();
    
      /**
      * @brief          Internal implementation structure - specific for the architecture
      *                 Forward definition
      */
      struct Impl;
    
    protected:
      /**
      * @brief          Internal implementation structure - specific for the architecture
      */
      Impl* impl;
  }; /* Class Mutex */
} /* namespace vsdk */

#endif /* UMATMUTEX_HPP */
