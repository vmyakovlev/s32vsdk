/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2016 NXP Semiconductor;
* All Rights Reserved
*
*****************************************************************************
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

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "umat_defines.hpp"

#ifndef __STANDALONE__
#ifndef APEX2_EMULATE
  #include <pthread.h>
#endif
#endif

#include "umat_mutex.hpp"


/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
/* Mutex implementation class - for pthread                                                       */
/*================================================================================================*/
struct vsdk::Mutex::Impl
{
  /*================================================================================================*/
  /* Constructor creates the mutex and initializes it                                               */
  /*================================================================================================*/
  Impl()
  {
	#ifndef __STANDALONE__
	#ifndef APEX2_EMULATE
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mt, &attr);
    pthread_mutexattr_destroy(&attr);
    #endif
    #endif

    refcount = 1;
  }
  
  /*================================================================================================*/
  /* Destructor destroys underlying pthread mutex                                                   */
  /*================================================================================================*/
  ~Impl() 
  { 
	#ifndef __STANDALONE__
	#ifndef APEX2_EMULATE
    pthread_mutex_destroy(&mt);
    #endif
    #endif
  }

  /*================================================================================================*/
  /* Locks underlying pthread mutex                                                                 */
  /*================================================================================================*/
  void lock() 
  {
	#ifndef __STANDALONE__
	#ifndef APEX2_EMULATE
    pthread_mutex_lock(&mt); 
    #endif
    #endif
  }
  
  /*================================================================================================*/
  /* Tries to lock the underlying pthread mutex                                                     */
  /*================================================================================================*/
  bool trylock() 
  { 
	#ifndef __STANDALONE__
	#ifndef APEX2_EMULATE
    return pthread_mutex_trylock(&mt) == 0; 
    #else
    return true;
    #endif
    #endif
  }
  
  /*================================================================================================*/
  /* Unlocks the underlying pthread mutex                                                           */
  /*================================================================================================*/
  void unlock() 
  { 
	#ifndef __STANDALONE__
	#ifndef APEX2_EMULATE
    pthread_mutex_unlock(&mt); 
    #endif
    #endif
  }

  /*================================================================================================*/
  /* Pthread mutec                                                                                  */
  /*================================================================================================*/
  #ifndef __STANDALONE__
  #ifndef APEX2_EMULATE
  pthread_mutex_t mt;
  #endif
  #endif
  
  /*================================================================================================*/
  /* Reference count                                                                                */
  /*================================================================================================*/
  int refcount;
};

/*================================================================================================*/
/* Constructor, creates and initializes the mutex                                                 */
/*================================================================================================*/
vsdk::Mutex::Mutex()
{
    impl = new vsdk::Mutex::Impl;
}

/*================================================================================================*/
/* Destructor                                                                                     */
/*================================================================================================*/
vsdk::Mutex::~Mutex()
{
    if( VSDK_CV_XADD(&impl->refcount, -1) == 1 )
        delete impl;
    impl = 0;
}

/*================================================================================================*/
/* Constructor, creates and initializes the mutex based on existing one                           */
/*================================================================================================*/
vsdk::Mutex::Mutex(const vsdk::Mutex& m)
{
    impl = m.impl;
    VSDK_CV_XADD(&impl->refcount, 1);
}

/*================================================================================================*/
/* Assignment operator                                                                            */
/*================================================================================================*/
vsdk::Mutex& vsdk::Mutex::operator = (const vsdk::Mutex& m)
{
    VSDK_CV_XADD(&m.impl->refcount, 1);
    if( VSDK_CV_XADD(&impl->refcount, -1) == 1 )
        delete impl;
    impl = m.impl;
    return *this;
}

/*================================================================================================*/
/* Obtain the mutex                                                                               */
/*================================================================================================*/
void vsdk::Mutex::lock() 
{ 
  impl->lock(); 
}

/*================================================================================================*/
/* Free the mutex                                                                                 */
/*================================================================================================*/
void vsdk::Mutex::unlock() 
{ 
  impl->unlock(); 
}

/*================================================================================================*/
/* Try to obtain the mutex.                                                                       */
/*================================================================================================*/
int8_t vsdk::Mutex::trylock() 
{ 
  return impl->trylock(); 
}
