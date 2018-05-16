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

#include <stdlib.h>

#include "umat_data.hpp"
#include "umat_mutex.hpp"
#include "umat_debug_out.hpp"

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
/* Locks for umats                                                                                */
/*================================================================================================*/
enum { UMAT_NLOCKS = 31 };
static vsdk::Mutex umatLocks[UMAT_NLOCKS];

/*================================================================================================*/
/* Creates a UMatData object with specified allocator, which will accompany                       */
/* the object whole its life                                                                      */
/*================================================================================================*/
vsdk::UMatData::UMatData(const vsdk::UMatAllocator* allocator)
{
  umat_printf("UMatData: Constructor\n");
  prevAllocator = currAllocator = allocator;
  urefcount = refcount = mapcount = 0;
  data = origdata = 0;
  size = 0;
  flags = 0;
  handle = 0;
  userdata = 0;
  allocatorFlags_ = 0;
  originalUMatData = NULL;
}

/*================================================================================================*/
/* Destroys a UMatData object. Checks whenever the data should be unmapped                        */
/* or freed                                                                                       */
/*================================================================================================*/
vsdk::UMatData::~UMatData()
{
  umat_printf("UMatData: Destructor\n");
  prevAllocator = currAllocator = 0;
  urefcount = refcount = 0;
  data = origdata = 0;
  size = 0;
  flags = 0;
  handle = 0;
  userdata = 0;
  allocatorFlags_ = 0;
  if (originalUMatData)
  {
    vsdk::UMatData* u = originalUMatData;
    VSDK_CV_XADD(&(u->urefcount), -1);
    VSDK_CV_XADD(&(u->refcount), -1);
    bool showWarn = false;
    if (u->refcount == 0)
    {
      if (u->urefcount > 0)
        showWarn = true;
        
      // simulate Mat::deallocate
      if (u->mapcount != 0)
      {
        umat_printf("    Map count is not equal to zero, need to unmap.\n");
        u->currAllocator->unmap(u);
      }
      else
      {
        umat_printf("    Map count is equal to zero, doing nothing.\n");
        // we don't do "map", so we can't do "unmap"
      }
    }

    if (u->refcount == 0 && u->urefcount == 0) // oops, we need to free resources
    {
      umat_printf("    WARNING, see below:\n");
      showWarn = true;
      // simulate UMat::deallocate
      u->currAllocator->deallocate(u);
    }
#ifdef DEBUG_OUTPUT
    if (showWarn)
    {
      static int warn_message_showed = 0;
      if (warn_message_showed++ < 100)
      {
        umat_printf("\n    ! OPENCV warning: getUMat()/getMat() call chain possible problem."
                    "\n    !                 Base object is dead, while nested/derived object is still alive or processed."
                    "\n    !                 Please check lifetime of UMat/Mat objects!\n");
        
      }
    }
#else
    (void)showWarn;
#endif
    originalUMatData = NULL;
  }
}
 
/*================================================================================================*/
/* Tries to obtain a mutex for this structure                                                     */
/*================================================================================================*/
void vsdk::UMatData::lock()
{
  umat_printf("UMatData: Lock\n");
  umatLocks[(uint64_t)(void*)this % UMAT_NLOCKS].lock();
}

/*================================================================================================*/
/* Frees the mutex for this structure                                                             */
/*================================================================================================*/
void vsdk::UMatData::unlock()
{
  umat_printf("UMatData: Unlock\n");
  umatLocks[(uint64_t)(void*)this % UMAT_NLOCKS].unlock();
}