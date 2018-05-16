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

#include "oal.h"

#include "umat_data.hpp"
#include "umat_debug_out.hpp"
#include "umat_mat.hpp"


/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
/* Constructor...                                                                                 */
/*================================================================================================*/
vsdk::UMatAllocator::UMatAllocator()
{
  umat_printf("UMatAllocator: Init\n");
}

/*================================================================================================*/
/* Descructor...                                                                                  */
/*================================================================================================*/
vsdk::UMatAllocator::~UMatAllocator()
{
  umat_printf("UMatAllocator: Deinit\n");
}
 
/*================================================================================================*/
/* Function is called with specific information and should allocate the                           */
/* internal buffer for a new UMat                                                                 */
/*================================================================================================*/
vsdk::UMatData* vsdk::UMatAllocator::allocate(int32_t              dims, 
                                              const int32_t*       sizes, 
                                              int32_t              type,
                                              void*                data0, 
                                              uint64_t*            step, 
                                              int32_t              /*flags*/, 
                                              vsdk::UMatUsageFlags usageFlags) const
{
  umat_printf("UMatAllocator: Allocate\n");
      
  uint64_t total = VSDK_CV_ELEM_SIZE(type);
  for(int32_t i = dims - 1; i >= 0; --i)
  {
    if( step )
    {
      if( data0 && step[i] != VSDK_CV_AUTOSTEP )
      {
        total = step[i];
      }
      else
      {
        step[i] = total;
      }
    }
    total *= sizes[i];
  }
  
  umat_printf("    size: %ld bytes; dims: %d; type: %X;\n", total, dims, type);
  
  
  
  int alloc_flags = 0;
   
  if (usageFlags & USAGE_DEFAULT)
  {
    alloc_flags = OAL_ALLOC_AUTO;
  }
  else
  {
    if (usageFlags & USAGE_DDR0)
    {
      alloc_flags = OAL_ALLOC_DDR0;
    }
    else
    {
      if (usageFlags & USAGE_DDR1)
      {
        alloc_flags = OAL_ALLOC_DDR1;
      }
      else
      {
        if (usageFlags & USAGE_SSRAM)
        {
          alloc_flags = OAL_ALLOC_SRAM_SINGLE;
        }
        else
        {
          if (usageFlags & USAGE_MSRAM)
          {
            alloc_flags = OAL_ALLOC_SRAM_MULTI;
          }
          else
          {
            alloc_flags = OAL_ALLOC_AUTO;
          }
        }
      }
    } 
  }
  uint8_t* data = data0 ? (uint8_t*)data0 : (uint8_t*)OAL_MemoryAllocFlag(total, alloc_flags);
  
  // Create UMatData structure
  vsdk::UMatData* umatdata = new vsdk::UMatData(this);
  umatdata->handle = umatdata->origdata = data;
  umatdata->data = 0;
  umatdata->size = total;
    
  umat_printf("    Allocated handle: %p\n", umatdata->handle);
  if(data0 && !(usageFlags & USAGE_DONT_ALLOCATE))
  {
    umatdata->flags |= UMatData::USER_ALLOCATED;
    if (OAL_MemoryReturnAddress(data0, ACCESS_PHY) == 0)
      umatdata->flags |= UMatData::COPY_ON_MAP;
  }
  return umatdata;
}

/*================================================================================================*/
/* The function returns the info when the allocation will be performed for umat                                                                                           */
/*================================================================================================*/
int8_t vsdk::UMatAllocator::allocate(vsdk::UMatData*      umatdata, 
                                     int32_t              /*accessFlags*/, 
                                     vsdk::UMatUsageFlags /*usageflags*/) const
{
  umat_printf("UMatAllocator: Allocate wrapper\n");
       
  if(!umatdata)
    return false;
 
  //vsdk::UMatDataAutoLock lock(umatdata);
 
  return true;  
}

/*================================================================================================*/
/* Function frees the UMatData contents (via OAL)                                                 */
/*================================================================================================*/
void vsdk::UMatAllocator::deallocate(vsdk::UMatData* umatdata) const
{
  umat_printf("UMatAllocator: Deallocate\n");
  
  if(!umatdata)
    return;

  if( !(umatdata->flags & UMatData::USER_ALLOCATED) )
  {
    umat_printf("    Freeing handle: %p\n", umatdata->handle);
    OAL_MemoryFree(umatdata->handle);
    umatdata->handle = 0;
  }
  delete umatdata;
}

/*================================================================================================*/
/* Function maps the UMatData to the virtual space with specific access flags                     */
/*================================================================================================*/
void vsdk::UMatAllocator::map (vsdk::UMatData *umatdata, 
                               int32_t accessflags) const 
{
  if (!(accessflags & OAL_USAGE_CACHED) && !(accessflags & OAL_USAGE_NONCACHED)) accessflags |= OAL_USAGE_CACHED;
  
  if (umatdata && umatdata->refcount == 1)
  {
    if ((umatdata->flags & OAL_USAGE_CACHED    && accessflags & OAL_USAGE_NONCACHED) ||
        (umatdata->flags & OAL_USAGE_NONCACHED && accessflags & OAL_USAGE_CACHED))
    {
      
      umatdata->flags &= ~(OAL_USAGE_CACHED | OAL_USAGE_NONCACHED);
      
      umat_printf("    Unmapping...\n");
      OAL_MemoryUnmap(umatdata->handle);
    }
  }
  
  if ((OAL_USAGE_NONCACHED & accessflags) != 0)
  {
    umat_printf("UMatAllocator: ACCESS_NCH_NB map\n");
    umatdata->data = (uint8_t *)OAL_MemoryReturnAddress(umatdata->handle, ACCESS_NCH_NB);
    
    if (umatdata->data)
      umatdata->flags |= OAL_USAGE_NONCACHED;
  }
  else if ((OAL_USAGE_CACHED & accessflags) != 0)
  { 
    umat_printf("UMatAllocator: ACCESS_CH_WB map\n");
    umatdata->data = (uint8_t *)OAL_MemoryReturnAddress(umatdata->handle, ACCESS_CH_WB);      
    
    if (umatdata->data)
      umatdata->flags |= OAL_USAGE_CACHED;
  }
     
  
  if (accessflags & OAL_USAGE_FORCE_NOTFLUSH)
    umatdata->flags |= OAL_USAGE_FORCE_NOTFLUSH;
      
  if (umatdata->data)
  {
    umatdata->markDeviceMemMapped(true);
    if(accessflags & ACCESS_WRITE)
    {
      umatdata->markDeviceCopyObsolete(true);
    }
    
    if( (accessflags & ACCESS_READ) != 0 && umatdata->hostCopyObsolete() )
    {
      umatdata->markHostCopyObsolete(false);
    }
  }
}

/*================================================================================================*/
/* Function unmaps the existing virtual space mapping                                             */
/*================================================================================================*/
void vsdk::UMatAllocator::unmap (vsdk::UMatData *umatdata) const 
{ 
  vsdk::UMatDataAutoLock lock(umatdata);
  umat_printf("UMatAllocator: unmap\n");

  if(umatdata->urefcount == 0 && umatdata->refcount == 0)
  {
    deallocate(umatdata);
    umatdata = NULL;
  }
  else if(!(umatdata->flags & UMatData::USER_ALLOCATED))
  {
    if (umatdata->flags & OAL_USAGE_CACHED /* && umatdata->deviceCopyObsolete() */ && !(OAL_USAGE_FORCE_NOTFLUSH & umatdata->flags))
    {
      umat_printf("    Flushing...\n");
      OAL_MemoryFlushAndInvalidate(OAL_MemoryReturnAddress(umatdata->handle, ACCESS_CH_WB), umatdata->size);
    }

    umatdata->flags &= ~OAL_USAGE_FORCE_NOTFLUSH;
    umatdata->markDeviceMemMapped(false);
    umatdata->data = 0;
    umatdata->markDeviceCopyObsolete(false);
    umatdata->markHostCopyObsolete(true);
  }
}

/*================================================================================================*/
/* Allocator download function                                                                    */
/*================================================================================================*/
void vsdk::UMatAllocator::download(vsdk::UMatData*       u, 
                    void*                 dstptr, 
                    int32_t               dims, 
                    const uint64_t        sz[],
                    const uint64_t        srcofs[], 
                    const uint64_t        srcstep[],
                    const uint64_t        dststep[]) const
{
  if(!u)
    return;
  
  int32_t isz[VSDK_CV_MAX_DIM];
  uint8_t* srcptr = u->data;
  
  if (srcptr == NULL)
  {
    srcptr = (uint8_t *)OAL_MemoryReturnAddress(u->handle, ACCESS_CH_WB);      
  }
  
  for( int32_t i = 0; i < dims; i++ )
  {
     if( sz[i] == 0 )
       return;
       if( srcofs )
       srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
       isz[i] = (int32_t)sz[i];
   }

   vsdk::Mat src(dims, isz, VSDK_CV_8U, srcptr, srcstep);
   vsdk::Mat dst(dims, isz, VSDK_CV_8U, dstptr, dststep);
 
   const vsdk::Mat* arrays[] = { &src, &dst };
   uint8_t* ptrs[2];
   vsdk::NAryMatIterator it(arrays, ptrs, 2);
   size_t j, planesz = it.size;
 
   for( j = 0; j < it.nplanes; j++, ++it )
     memcpy(ptrs[1], ptrs[0], planesz);
   
   if (u->data == NULL)
   {
     OAL_MemoryUnmap(u->handle);      
   }
}
      
/*================================================================================================*/
/* Allocator upload function                                                                      */
/*================================================================================================*/
void vsdk::UMatAllocator::upload(vsdk::UMatData* u, 
            const void*     srcptr, 
            int32_t         dims, 
            const uint64_t  sz[],
            const uint64_t  dstofs[], 
            const uint64_t  dststep[],
            const uint64_t  srcstep[]) const
{
  if(!u)
    return;
  
  int32_t isz[VSDK_CV_MAX_DIM];
  uint8_t* dstptr = u->data;
  
  if (dstptr == NULL)
  {
    dstptr = (uint8_t *)OAL_MemoryReturnAddress(u->handle, ACCESS_CH_WB);      
  }

  for( int32_t i = 0; i < dims; i++ )
  {
    if( sz[i] == 0 )
      return;
    if( dstofs )
    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
    isz[i] = (int32_t)sz[i];
  }
 
  vsdk::Mat src(dims, isz, VSDK_CV_8U, (void*)srcptr, srcstep);
  vsdk::Mat dst(dims, isz, VSDK_CV_8U, dstptr, dststep);

  const vsdk::Mat* arrays[] = { &src, &dst };
  uint8_t* ptrs[2];
  vsdk::NAryMatIterator it(arrays, ptrs, 2);
  size_t j, planesz = it.size;

  for( j = 0; j < it.nplanes; j++, ++it )
  {
     memcpy(ptrs[1], ptrs[0], planesz);
     OAL_MemoryFlushAndInvalidate(ptrs[1], planesz);
  }
  
  if (u->data == NULL)
  {
    OAL_MemoryUnmap(u->handle);      
  }
}

/*================================================================================================*/
/* Allocator copy function                                                                        */
/*================================================================================================*/
void vsdk::UMatAllocator::copy(vsdk::UMatData* usrc, 
          vsdk::UMatData* udst, 
          int32_t         dims, 
          const uint64_t  sz[],
          const uint64_t  srcofs[], 
          const uint64_t  srcstep[],
          const uint64_t  dstofs[], 
          const uint64_t  dststep[], 
          int8_t          /*sync*/) const
{
  if(!usrc || !udst)
    return;

  int32_t isz[VSDK_CV_MAX_DIM];
  uint8_t* srcptr = usrc->data;
  uint8_t* dstptr = udst->data;
  
  if (srcptr == NULL)
  {
    srcptr = (uint8_t *)OAL_MemoryReturnAddress(usrc->handle, ACCESS_CH_WB);      
  }
  
  if (dstptr == NULL)
  {
    dstptr = (uint8_t *)OAL_MemoryReturnAddress(udst->handle, ACCESS_CH_WB);      
  }
  
  for( int i = 0; i < dims; i++ )
  {
    if( sz[i] == 0 )
      return;
    if( srcofs )
      srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
    if( dstofs )
      dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
    isz[i] = (int)sz[i];
   }
 
   vsdk::Mat src(dims, isz, VSDK_CV_8U, srcptr, srcstep);
   vsdk::Mat dst(dims, isz, VSDK_CV_8U, dstptr, dststep);
 
   const vsdk::Mat* arrays[] = { &src, &dst };
   uint8_t* ptrs[2];
   vsdk::NAryMatIterator it(arrays, ptrs, 2);
   size_t j, planesz = it.size;
 
   for( j = 0; j < it.nplanes; j++, ++it )
   {
      memcpy(ptrs[1], ptrs[0], planesz);
      OAL_MemoryFlushAndInvalidate(ptrs[1], planesz);
   }

  if (usrc->data == NULL)
  {
    OAL_MemoryUnmap(usrc->handle);      
  }
  if (udst->data == NULL)
  {
    OAL_MemoryUnmap(udst->handle);      
  }
}

#ifdef VSDK_UMAT_USE_OPENCV
  /*================================================================================================*/
  /* OpenCV compatibility overload                                                                  */
  /*================================================================================================*/
  cv::UMatData* vsdk::UMatAllocator::allocate(int        dims, 
                                const int* sizes, 
                                int        type,
                                void*          data, 
                                size_t*      step, 
                                int        flags, 
                                cv::UMatUsageFlags usageFlags) const
  {
	  return (cv::UMatData *)allocate((int32_t)dims, (const int32_t *)sizes, (int32_t)type, data, (uint64_t *)step, (int32_t)flags, (vsdk::UMatUsageFlags)usageFlags);
  }
                                
  /*================================================================================================*/
  /* OpenCV compatibility overload                                                                  */
  /*================================================================================================*/
  bool vsdk::UMatAllocator::allocate(cv::UMatData* data, int32_t accessflags, cv::UMatUsageFlags usageFlags) const
  {
    return allocate((vsdk::UMatData*)data, accessflags, (vsdk::UMatUsageFlags)usageFlags);
  }
  
  /*================================================================================================*/
  /* OpenCV compatibility overload                                                                  */
  /*================================================================================================*/
  void vsdk::UMatAllocator::deallocate(cv::UMatData* data) const
  {
    deallocate((vsdk::UMatData*)data);
  }

  /*================================================================================================*/
  /* OpenCV compatibility overload                                                                  */
  /*================================================================================================*/
  void vsdk::UMatAllocator::map(cv::UMatData* data, int32_t accessflags) const
  {
    map((vsdk::UMatData*)data, accessflags);
  }
      
  /*================================================================================================*/
  /* OpenCV compatibility overload                                                                  */
  /*================================================================================================*/
  void vsdk::UMatAllocator::unmap(cv::UMatData* data) const
  {
    unmap((vsdk::UMatData*)data);
  }
  
  /*================================================================================================*/
  /* Allocator download function                                                                    */
  /*================================================================================================*/
  void vsdk::UMatAllocator::download(cv::UMatData* data, void* dst, int dims, const size_t sz[],
                                     const size_t srcofs[], const size_t srcstep[],
                                     const size_t dststep[]) const
  {
    #if defined(APEX2_EMULATE) 
      uint64_t *sz64      = new uint64_t[dims];
      uint64_t *srcofs64  = new uint64_t[dims];
      uint64_t *srcstep64 = new uint64_t[dims];
      uint64_t *dststep64 = new uint64_t[dims];
      for (int i = 0; i < dims; ++i)
      {
        sz64[i]      = (uint32_t)sz[i];
        srcofs64[i]  = (uint32_t)srcofs[i];
        srcstep64[i] = (uint32_t)srcstep[i];
        dststep64[i] = (uint32_t)dststep[i];
      }
      
      download((vsdk::UMatData *)data, dst, dims, sz64, srcofs64, srcstep64, dststep64);
      
      delete sz64;
      delete srcofs64;
      delete srcstep64;
      delete dststep64;
    #else
      download((vsdk::UMatData *)data, dst, dims, sz, srcofs, srcstep, dststep);
    #endif
  }
  
  /*================================================================================================*/
  /* Allocator upload function                                                                      */
  /*================================================================================================*/
  void vsdk::UMatAllocator::upload(cv::UMatData* data, const void* src, int dims, const size_t sz[],
                                   const size_t dstofs[], const size_t dststep[],
                                   const size_t srcstep[]) const
  {
    #if defined(APEX2_EMULATE) 
      uint64_t *sz64      = new uint64_t[dims];
      uint64_t *dstofs64  = new uint64_t[dims];
      uint64_t *srcstep64 = new uint64_t[dims];
      uint64_t *dststep64 = new uint64_t[dims];
      for (int i = 0; i < dims; ++i)
      {
        sz64[i]      = (uint32_t)sz[i];
        dstofs64[i]  = (uint32_t)dstofs[i];
        srcstep64[i] = (uint32_t)srcstep[i];
        dststep64[i] = (uint32_t)dststep[i];
      }
      
      upload((vsdk::UMatData *)data, src, dims, sz64, dstofs64, dststep64, srcstep64);
      
      delete sz64;
      delete dstofs64;
      delete srcstep64;
      delete dststep64;
    #else
      upload((vsdk::UMatData *)data, src, dims, sz, dstofs, dststep, srcstep);
    #endif
    
  }
  
  /*================================================================================================*/
  /* Allocator copy function                                                                        */
  /*================================================================================================*/
  void vsdk::UMatAllocator::copy(cv::UMatData* srcdata, UMatData* dstdata, int dims, const size_t sz[],
                                 const size_t srcofs[], const size_t srcstep[],
                                 const size_t dstofs[], const size_t dststep[], bool sync) const
  {
    #if defined(APEX2_EMULATE) 
      uint64_t *sz64      = new uint64_t[dims];
      uint64_t *srcofs64  = new uint64_t[dims];
      uint64_t *dstofs64  = new uint64_t[dims];
      uint64_t *srcstep64 = new uint64_t[dims];
      uint64_t *dststep64 = new uint64_t[dims];
      for (int i = 0; i < dims; ++i)
      {
        sz64[i]      = (uint32_t)sz[i];
        srcofs64[i]  = (uint32_t)srcofs[i];
        dstofs64[i]  = (uint32_t)dstofs[i];
        srcstep64[i] = (uint32_t)srcstep[i];
        dststep64[i] = (uint32_t)dststep[i];
      }
      
      copy((vsdk::UMatData *)srcdata, (vsdk::UMatData *)dstdata, dims, sz64, srcofs64, srcstep64, dstofs64, dststep64, sync);
      
      delete sz64;
      delete srcofs64;
      delete dstofs64;
      delete srcstep64;
      delete dststep64;
    #else
      copy((vsdk::UMatData *)srcdata, (vsdk::UMatData *)dstdata, dims, sz, srcofs, srcstep, dstofs, dststep, sync);
    #endif
    
  }
  
  

#endif /* VSDK_UMAT_USE_OPENCV */
