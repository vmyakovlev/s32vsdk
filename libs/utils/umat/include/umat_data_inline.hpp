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
#ifndef UMATDATAINLINE_HPP
#define UMATDATAINLINE_HPP

/*==================================================================================================
*                                INLINE FUNCTION DEFINITIONS
==================================================================================================*/

/*================================================================================================*/
/* Query for host copy obsolete (mapped memory) flag                                              */
/*================================================================================================*/
inline int8_t vsdk::UMatData::hostCopyObsolete() const 
{ 
  return (flags & vsdk::UMatData::HOST_COPY_OBSOLETE) != 0; 
}

/*================================================================================================*/
/* Query for device copy obsolete (physical address) flag                                         */
/*================================================================================================*/
inline int8_t vsdk::UMatData::deviceCopyObsolete() const 
{ 
  return (flags & vsdk::UMatData::DEVICE_COPY_OBSOLETE) != 0; 
}

/*================================================================================================*/
/* Query for "is mapped" flag                                                                     */
/*================================================================================================*/
inline int8_t vsdk::UMatData::deviceMemMapped() const 
{ 
  return (flags & vsdk::UMatData::DEVICE_MEM_MAPPED) != 0; 
}

/*================================================================================================*/
/* Query for "copy on map" flag                                                                   */
/*================================================================================================*/
inline int8_t vsdk::UMatData::copyOnMap() const 
{
  return (flags & vsdk::UMatData::COPY_ON_MAP) != 0;
}

/*================================================================================================*/
/* Query for "is temporary UMat" flag                                                             */
/*================================================================================================*/
inline int8_t vsdk::UMatData::tempUMat() const 
{ 
  return (flags & vsdk::UMatData::TEMP_UMAT) != 0; 
}

/*================================================================================================*/
/* Query for "is copied UMat" flag                                                                */
/*================================================================================================*/
inline int8_t vsdk::UMatData::tempCopiedUMat() const 
{ 
  return (flags & vsdk::UMatData::TEMP_COPIED_UMAT) == vsdk::UMatData::TEMP_COPIED_UMAT; 
}

/*================================================================================================*/
/* Flags the host copy (mapped memory) as obsolete                                                */
/*================================================================================================*/
inline void vsdk::UMatData::markDeviceMemMapped(int8_t flag)
{
  if(flag)
    flags |= vsdk::UMatData::DEVICE_MEM_MAPPED;
  else
    flags &= ~vsdk::UMatData::DEVICE_MEM_MAPPED;
}

/*================================================================================================*/
/* Flags the device copy (physical address) as obsolete                                           */
/*================================================================================================*/
inline void vsdk::UMatData::markHostCopyObsolete(int8_t flag)
{
  if(flag)
    flags |= vsdk::UMatData::HOST_COPY_OBSOLETE;
  else
    flags &= ~vsdk::UMatData::HOST_COPY_OBSOLETE;
}

/*================================================================================================*/
/* Flags the "is mapped"                                                                          */
/*================================================================================================*/
inline void vsdk::UMatData::markDeviceCopyObsolete(int8_t flag)
{
  if(flag)
    flags |= vsdk::UMatData::DEVICE_COPY_OBSOLETE;
  else
    flags &= ~vsdk::UMatData::DEVICE_COPY_OBSOLETE;
}

/*================================================================================================*/
/* Function tries to obtain the lock for UMatData                                                 */
/*================================================================================================*/
inline vsdk::UMatDataAutoLock::UMatDataAutoLock(vsdk::UMatData* _u) : u(_u) { u->lock(); }

/*================================================================================================*/
/* Function releases the lock for assigned UMatData                                               */
/*================================================================================================*/
inline vsdk::UMatDataAutoLock::~UMatDataAutoLock() { u->unlock(); }

#endif /* UMATDATAINLINE_HPP */



