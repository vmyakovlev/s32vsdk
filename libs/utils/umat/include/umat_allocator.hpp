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

#ifndef UMATALLOCATOR_HPP
#define UMATALLOCATOR_HPP

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "umat_defines.hpp"

#ifdef VSDK_UMAT_USE_OPENCV
  #include <opencv2/opencv.hpp>
#endif

#include <stdint.h>

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
namespace vsdk
{
  struct UMatData;

  /**
  * @brief          UMat usage flags specify the usage of a specific UMat. 
  */
  enum UMatUsageFlags
  {
    USAGE_DEFAULT = 0,
    USAGE_ALLOCATE_HOST_MEMORY = 1 << 0,
    USAGE_ALLOCATE_DEVICE_MEMORY = 1 << 1,
    USAGE_ALLOCATE_SHARED_MEMORY = 1 << 2,
    USAGE_DONT_ALLOCATE = 1 << 3,
    
    USAGE_DDR0 = 1 << 4,
    USAGE_DDR1 = 1 << 5,
    USAGE_SSRAM = 1 << 6,
    USAGE_MSRAM = 1 << 7,
    __UMAT_USAGE_FLAGS_32BIT = 0x7fffffff // Binary compatibility hint
  };
  
  /**
  * @brief          Access flags for the mapping
  */
  enum 
  { 
    ACCESS_READ =  1<<24, 
    ACCESS_WRITE = 1<<25,
    ACCESS_RW =    3<<24, 
    ACCESS_MASK =  ACCESS_RW, 
    ACCESS_FAST =  1<<26 };
  
  /*================================================================================================*/
  /**
  * @brief          UMat Auto lock - obtains a loc on constructor, releases it when dies
  */
  struct UMatDataAutoLock
  {
    /**
    * @brief          Function tries to obtain the lock for UMatData
    * 
    * @param[in]      u           UMatData object which is supposed to be locked
    */
    explicit UMatDataAutoLock(vsdk::UMatData* u);
    
    /**
    * @brief          Function releases the lock for assigned UMatData
    */
    ~UMatDataAutoLock();
    
    /**
    * @brief          Associated UMatData object
    */
    UMatData* u;
  };

  /*================================================================================================*/
  /**
  * It's derived from OpenCV's UMatAllocator class - manages all the allocations
  * inside the UMat. It's called internally and can be also inherited further
  * in order to implement specific functionality.
  */
  class UMatAllocator 
  #ifdef VSDK_UMAT_USE_OPENCV
    : public cv::MatAllocator
  #endif
  {
    public:
      /**
      * @brief          Constructor, creates allocator...
      */
      UMatAllocator();
      
      /**
      * @brief          Descructor, destroys allocator...
      *
      */
      ~UMatAllocator();

      /**
      * @brief          Function is called with specific information and should allocate the 
      *                 internal buffer for a new UMat
      *
      * @param[in]      dims            Number of dimensions   
      * @param[in]      sizes           Size for each dimension
      * @param[in]      type            Data type
      * @param[in]      data            Existing pointer to data, if available
      * @param[in]      step            Step for each dimension
      * @param[in]      flags           Access and other defined flags
      * @param[in]      usageFlags      Usage flags
      *
      * @return         Allocated UMatData structure
      */
      vsdk::UMatData* allocate(int32_t        dims, 
                               const int32_t* sizes, 
                               int32_t        type,
                               void*          data, 
                               uint64_t*      step, 
                               int32_t        flags, 
                               vsdk::UMatUsageFlags usageFlags) const;
      
      /**
      * @brief          The function returns the info when the allocation will be performed for umat
      *
      * @param[in]      data            Existing pointer to data, if available
      * @param[in]      accessflags     Access and other defined flags
      * @param[in]      usageFlags      Usage flags
      *
      * @return         
      */
      int8_t allocate(vsdk::UMatData*      data, 
                      int32_t              accessflags, 
                      vsdk::UMatUsageFlags usageFlags) const;
                            
      /**
      * @brief          Function frees the UMatData contents (via OAL)
      *
      * @param[in]      data            Existing pointer to data, if available   
      *    
      */
      void deallocate(vsdk::UMatData* data) const;
      
      /**
      * @brief          Function maps the UMatData to the virtual space with specific access flags
      *
      * @param[in]      data            Existing pointer to data, if available
      * @param[in]      accessflags     Access and other defined flags
      */
      void map(vsdk::UMatData* data, 
               int32_t         accessflags) const;
      
      /**
      * @brief          Function unmaps the existing virtual space mapping
      *
      * @param[in]      data            Existing pointer to data, if available
      */
      void unmap(vsdk::UMatData* data) const;
      
      /**
      * @brief          Function downloads the matrix contents into dst pointer
      *
      * @param[in]      data            UMatData structure of the matrix
      * @param[in]      dst             Destination pointer
      * @param[in]      dims            Number of dimensions
      * @param[in]      sz              Sizes for each dimension
      * @param[in]      srcofs          Source offset for each dimenstion
      * @param[in]      srcstep         Source step for each dimension
      * @param[in]      dststep         Destination step
      *
      */
      void download(vsdk::UMatData*       data, 
                    void*                 dst, 
                    int32_t               dims, 
                    const uint64_t        sz[],
                    const uint64_t        srcofs[], 
                    const uint64_t        srcstep[],
                    const uint64_t        dststep[]) const;
      
      /**
      * @brief          Function uploads the matrix contents from src pointer
      *
      * @param[in]      data            UMatData structure of the matrix
      * @param[in]      src             Source pointer
      * @param[in]      dims            Number of dimensions
      * @param[in]      sz              Sizes for each dimension
      * @param[in]      dstofs          Destination offset for each dimenstion
      * @param[in]      dststep         Destination step for each dimension
      * @param[in]      srcstep         Source step
      *
      */
      void upload(vsdk::UMatData* data, 
                  const void*     src, 
                  int32_t         dims, 
                  const uint64_t  sz[],
                  const uint64_t  dstofs[], 
                  const uint64_t  dststep[],
                  const uint64_t  srcstep[]) const;

      /**
      * @brief          Copies the matrix contents to another UMat
      *
      * @param[in]      srcdata         Source UMatData structure of the matrix
      * @param[in]      dstdata         Destination UMatData structure of the matrix
      * @param[in]      dims            Number of dimensions
      * @param[in]      sz              Sizes for each dimension
      * @param[in]      srcofs          Source offset for each dimenstion
      * @param[in]      srcstep         Source step for each dimension
      * @param[in]      dstofs          Destination offset for each dimenstion
      * @param[in]      dststep         Destination step
      * @param[in]      sync            Not used now
      *
      */
      void copy(vsdk::UMatData* srcdata, 
                vsdk::UMatData* dstdata, 
                int32_t         dims, 
                const uint64_t  sz[],
                const uint64_t  srcofs[], 
                const uint64_t  srcstep[],
                const uint64_t  dstofs[], 
                const uint64_t  dststep[], 
                int8_t          sync) const;
      
                  
      #ifdef VSDK_UMAT_USE_OPENCV
      
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */
        cv::UMatData* allocate(int        dims, 
                               const int* sizes, 
                               int        type,
                               void*          data, 
                               size_t*      step, 
                               int        flags, 
                               cv::UMatUsageFlags usageFlags) const;
           
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */
        bool allocate(cv::UMatData*        data, 
                      int32_t              accessflags, 
                      cv::UMatUsageFlags usageFlags) const;
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */              
        void deallocate(cv::UMatData* data) const;
      
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */
        void map(cv::UMatData*   data, 
                 int32_t         accessflags) const;
               
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */
        void unmap(cv::UMatData* data) const;
        
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */
        void download(cv::UMatData* data, void* dst, int dims, const size_t sz[],
                      const size_t srcofs[], const size_t srcstep[],
                      const size_t dststep[]) const;
                      
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */
        void upload(cv::UMatData* data, const void* src, int dims, const size_t sz[],
                    const size_t dstofs[], const size_t dststep[],
                    const size_t srcstep[]) const;
                    
        /**
        * @overload  
        * 
        * @brief          Overload for OpenCV compatibility
        */
        void copy(cv::UMatData* srcdata, UMatData* dstdata, int dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dstofs[], const size_t dststep[], bool sync) const;
        

      #endif /* VSDK_UMAT_USE_OPENCV */
  }; // class UMatAllocator
} // namespace vsdk

#endif /* UMATALLOCATOR_HPP */
