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
#include <stdio.h>

#include <oal.h>

#include "umat.hpp"
#include "umat_mutex.hpp"
#include "umat_allocator.hpp"
#include "umat_debug_out.hpp"

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

static vsdk::UMatAllocator __allocator;

/*================================================================================================*/
/* Function updates the continuity flag according to data                                         */
/*================================================================================================*/
void updateContinuityFlag(vsdk::UMat& m)
{
    int32_t i, j;
    for( i = 0; i < m.dims; i++ )
    {
        if( m.size[i] > 1 )
            break;
    }

    for( j = m.dims-1; j > i; j-- )
    {
        if( m.step[j]*m.size[j] < m.step[j-1] )
            break;
    }

    uint64_t total = (uint64_t)m.step[0]*m.size[0];
    if( j <= i && total == (uint64_t)total )
        m.flags |= vsdk::UMat::CONTINUOUS_FLAG;
    else
        m.flags &= ~vsdk::UMat::CONTINUOUS_FLAG;
}

/*================================================================================================*/
/* Function finalizes header                                                                      */
/*================================================================================================*/
void finalizeHdr(vsdk::UMat& m)
{
    updateContinuityFlag(m);
    int d = m.dims;
    if( d > 2 )
        m.rows = m.cols = -1;
}

/*================================================================================================*/
/* Function for setting size of the matrix                                                        */
/*================================================================================================*/
void setSize( vsdk::UMat& m, int32_t _dims, const int32_t* _sz,
                            const uint64_t* _steps, int8_t autoSteps = false)
{
  if( m.dims != _dims )
  {
    if( m.step.p != m.step.buf )
    {
      free(m.step.p);
      m.step.p = m.step.buf;
      m.size.p = &m.rows;
    }
    if( _dims > 2 )
    {
      m.step.p = (uint64_t*)malloc(_dims*sizeof(m.step.p[0]) + (_dims+1)*sizeof(m.size.p[0]));
      m.size.p = (int32_t*)(m.step.p + _dims) + 1;
      m.size.p[-1] = _dims;
      m.rows = m.cols = -1;
    }
  }

  m.dims = _dims;
  if( !_sz )
    return;
 
  uint64_t esz = VSDK_CV_ELEM_SIZE(m.flags), total = esz;
  int32_t i;
  for( i = _dims-1; i >= 0; i-- )
  {
    int32_t s = _sz[i];
    m.size.p[i] = s;
 
    if( _steps )
      m.step.p[i] = i < _dims-1 ? _steps[i] : esz;
    else if( autoSteps )
    {
      m.step.p[i] = total;
      int64_t total1 = (int64_t)total*s;
      if( (uint64_t)total1 != (uint64_t)total1 )
        printf( "The total matrix size does not fit to \"uint64_t\" type" );
      total = (uint64_t)total1;
    }
  }
 
  if( _dims == 1 )
  {
    m.dims = 2;
    m.cols = 1;
    m.step[1] = esz;
  }
}

/*================================================================================================*/
/* Overload                                                                                       */
/*================================================================================================*/
vsdk::UMat::UMat(const vsdk::UMat& m, const vsdk::Range& _rowRange, const vsdk::Range& _colRange)
: flags(MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  allocator(0), 
  usageFlags(USAGE_DEFAULT), 
  u(0), 
  offset(0), 
  size(&rows)
{
  if( m.dims > 2 )
  {
    vsdk::Range *rs = new vsdk::Range[m.dims];
    rs[0] = _rowRange;
    rs[1] = _colRange;
    for( int i = 2; i < m.dims; i++ )
      rs[i] = Range::all();
    *this = m(rs);
    delete rs;
    return;
  }

  *this = m;
  if( _rowRange != Range::all() && _rowRange != Range(0,rows) )
  {
    rows = _rowRange.size();
    offset += step*_rowRange.start;
    flags |= SUBMATRIX_FLAG;
  }
 
  if( _colRange != Range::all() && _colRange != Range(0,cols) )
  {
    cols = _colRange.size();
    offset += _colRange.start*elemSize();
    flags &= cols < m.cols ? ~CONTINUOUS_FLAG : -1;
    flags |= SUBMATRIX_FLAG;
  }
 
  if( rows == 1 )
    flags |= CONTINUOUS_FLAG;
 
  if( rows <= 0 || cols <= 0 )
  {
    release();
    rows = cols = 0;
  }
}

/*================================================================================================*/
/* Overload                                                                                       */
/*================================================================================================*/
vsdk::UMat::UMat(const vsdk::UMat& m, const vsdk::Range* ranges)
: flags(MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  allocator(0), 
  usageFlags(USAGE_DEFAULT), 
  u(0), 
  offset(0), 
  size(&rows)
{
  int32_t i, d = m.dims;

  *this = m;
  for( i = 0; i < d; i++ )
  {
    vsdk::Range r = ranges[i];
    if( r != Range::all() && r != Range(0, size.p[i]))
    {
      size.p[i] = r.end - r.start;
      offset += r.start*step.p[i];
      flags |= SUBMATRIX_FLAG;
    }
  }
  updateContinuityFlag(*this);
}

/*================================================================================================*/
/* Overload                                                                                       */
/*================================================================================================*/
vsdk::UMat::UMat(const vsdk::UMat& m, const vsdk::Rect& roi)
: flags(m.flags), 
  dims(2), 
  rows(roi.height), 
  cols(roi.width),
  allocator(m.allocator), 
  usageFlags(m.usageFlags), 
  u(m.u), 
  offset(m.offset + roi.y*m.step[0]), 
  size(&rows)
{
  flags &= roi.width < m.cols ? ~CONTINUOUS_FLAG : -1;
  flags |= roi.height == 1 ? CONTINUOUS_FLAG : 0;

  size_t esz = VSDK_CV_ELEM_SIZE(flags);
  offset += roi.x*esz;
  if( u )
    VSDK_CV_XADD(&(u->urefcount), 1);
  if( roi.width < m.cols || roi.height < m.rows )
    flags |= SUBMATRIX_FLAG;

  step[0] = m.step[0]; step[1] = esz;

  if( rows <= 0 || cols <= 0 )
  {
    release();
    rows = cols = 0;
  }
}

/*================================================================================================*/
/* Internal use function; properly re-allocates _size, _step arrays                               */
/*================================================================================================*/
void vsdk::UMat::copySize(const vsdk::UMat& m)
{
  setSize(*this, m.dims, 0, 0);
  for( int i = 0; i < dims; i++ )
  {
    size[i] = m.size[i];
    step[i] = m.step[i];
  }
}

/*================================================================================================*/
/* Destructor - calls release()                                                                   */
/*================================================================================================*/
vsdk::UMat::~UMat()
{
  umat_printf("UMat: Destructor, refcount %d\n", u != 0 ? u->urefcount : 0);
  release();
  if( step.p != step.buf )
      free(step.p);
}

/*================================================================================================*/
/* Deallocates the matrix data                                                                    */
/*================================================================================================*/
void vsdk::UMat::deallocate()
{
  umat_printf("UMat: Deallocate\n");
  u->currAllocator->deallocate(u);
  u = NULL;
}

/*================================================================================================*/
/* Creates the matrix - allocates if necessary                                                    */
/*================================================================================================*/
void vsdk::UMat::create(int32_t d, const int32_t* _sizes, int32_t _type, vsdk::UMatUsageFlags _usageFlags)
{
  umat_printf("UMat: Create\n");
  this->usageFlags = _usageFlags;

  int32_t i;
  _type = VSDK_CV_MAT_TYPE(_type);

  if( u && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
  {
    if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
      return;
    for( i = 0; i < d; i++ )
      if( size[i] != _sizes[i] )
        break;
    if( i == d && (d > 1 || size[1] == 1))
      return;
  }

  release();

  if( d == 0 )
    return;

  flags = (_type & VSDK_CV_MAT_TYPE_MASK) | MAGIC_VAL;
  setSize(*this, d, _sizes, 0, true);
  offset = 0;

  if( total() > 0 )
  {
    UMatAllocator *a = allocator, *a0 = &__allocator;
    if (!a)
    {
      a = a0;
      a0 = &__allocator;
    }
    #if !defined __STANDALONE__ && !defined __INTEGRITY__
    try
    {
      u = a->allocate(dims, size, _type, 0, step.p, 0, usageFlags);
    }
    catch(...)
    {
      if(a != a0)
        u = a0->allocate(dims, size, _type, 0, step.p, 0, usageFlags);
    }
    #else
    u = a->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
    #endif
  }
 
  if (size != 0 && u->handle == NULL)
  {
    *this = vsdk::UMat();   
  }
  else
  {
    finalizeHdr(*this);
    addref();
  }
}

/*================================================================================================*/
/* Returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1);          */
/* negative number otherwise                                                                      */
/*================================================================================================*/
int32_t vsdk::UMat::checkVector(int32_t _elemChannels, int32_t _depth, int8_t _requireContinuous) const
{
    return (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims == 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (int32_t)(total()*channels()/_elemChannels) : -1;
}

/*================================================================================================*/
/* Returns UMat handle for allocated structure                                                    */
/*================================================================================================*/
void* vsdk::UMat::handle(int32_t accessFlags) const
{
    if( !u )
        return 0;

    if (u->deviceCopyObsolete())
    {
        u->currAllocator->unmap(u);
    }

    if ((accessFlags & ACCESS_WRITE) != 0)
        u->markHostCopyObsolete(true);

    return u->handle;
}

/*================================================================================================*/
/* Fills the ofs with offsets for each dimension                                                  */
/*================================================================================================*/
void vsdk::UMat::ndoffset(int64_t* ofs) const
{
    // offset = step[0]*ofs[0] + step[1]*ofs[1] + step[2]*ofs[2] + ...;
    size_t val = offset;
    for( int i = 0; i < dims; i++ )
    {
        size_t s = step.p[i];
        ofs[i] = val / s;
        val -= ofs[i]*s;
    }
}

/*================================================================================================*/
/* Returns a new matrix header for the specified diagonal                                         */
/* (d=0 - the main diagonal,                                                                      */
/*  >0 - a diagonal from the lower half,                                                          */
/*  <0 - a diagonal from the upper half)                                                          */
/*================================================================================================*/
vsdk::UMat vsdk::UMat::diag(int32_t d) const
{
  vsdk::UMat m = *this;
  size_t esz = elemSize();
  int len;

  if( d >= 0 )
  {
    len = std::min(cols - d, rows);
    m.offset += esz*d;
  }
  else
  {
    len = std::min(rows + d, cols);
    m.offset -= step[0]*d;
  }

  m.size[0] = m.rows = len;
  m.size[1] = m.cols = 1;
  m.step[0] += (len > 1 ? esz : 0);

  if( m.rows > 1 )
    m.flags &= ~CONTINUOUS_FLAG;
  else
    m.flags |= CONTINUOUS_FLAG;

  if( size[0] != 1 || size[1] != 1)
    m.flags |= SUBMATRIX_FLAG;

  return m;
}

/*================================================================================================*/
/* Get Mat function                                                                               */
/*================================================================================================*/
vsdk::Mat vsdk::UMat::getMat(int32_t accessFlags) const
{
  if(!u)
    return vsdk::Mat();
 
  // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
  accessFlags |= ACCESS_RW;
  vsdk::UMatDataAutoLock autolock(u);
   
  if(VSDK_CV_XADD(&u->refcount, 1) == 0)
    u->currAllocator->map(u, accessFlags);
  
  if ( u->data != 0 && 
     (!(u->flags & (OAL_USAGE_CACHED | OAL_USAGE_NONCACHED)) ||
      (accessFlags & OAL_USAGE_CACHED    && u->flags & OAL_USAGE_CACHED) ||
      (accessFlags & OAL_USAGE_NONCACHED && u->flags & OAL_USAGE_NONCACHED)))
  {
    Mat hdr(dims, size.p, type(), u->data + offset, step.p);
    hdr.flags = flags;
    hdr.u = u;
    hdr.datastart = u->data;
    hdr.data = u->data + offset;
    hdr.datalimit = hdr.dataend = u->data + u->size;
    return hdr;
  }
  else
  {
    VSDK_CV_XADD(&u->refcount, -1);
    return Mat();
  }
}

/*================================================================================================*/
/* CustomPointerUMat default constructor                                                          */
/*================================================================================================*/
vsdk::CustomPointerUMat::CustomPointerUMat(int32_t _rows, int32_t _cols, int32_t _type, void *hwptr, void *virtualptr)
{
  flags = vsdk::UMat::MAGIC_VAL;
  dims = 0;
  rows = 0; 
  cols = 0; 
  allocator = 0;
  usageFlags = USAGE_DONT_ALLOCATE;
  u = 0;
  offset = 0;
  size = vsdk::MatSize(&rows);
  
  _type &= vsdk::UMat::TYPE_MASK;
  if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && u )
    return;
  int sz[] = {_rows, _cols};
  createCustom(2, sz, _type, hwptr,  usageFlags);
   
  u->data = (uint8_t *)virtualptr;
}


/*================================================================================================*/
/* Creates the matrix - allocates if necessary                                                    */
/*================================================================================================*/
void vsdk::CustomPointerUMat::createCustom(int32_t d, const int32_t* _sizes, int32_t _type, void *hwptr, vsdk::UMatUsageFlags _usageFlags)
{
  umat_printf("UMat: Create\n");
  this->usageFlags = _usageFlags;
  
  int32_t i;
  _type = VSDK_CV_MAT_TYPE(_type);

  if( u && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
  {
    if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
      return;
    for( i = 0; i < d; i++ )
      if( size[i] != _sizes[i] )
        break;
    if( i == d && (d > 1 || size[1] == 1))
      return;
  }

  release();

  if( d == 0 )
    return;

  flags = (_type & VSDK_CV_MAT_TYPE_MASK) | MAGIC_VAL;
  setSize(*this, d, _sizes, 0, true);
  offset = 0;

  if( total() > 0 )
  {
    
    UMatAllocator *a = allocator, *a0 = &__allocator;
    if (!a)
    {
      a = a0;
      a0 = &__allocator;
    }
    #if !defined __STANDALONE__ && !defined __INTEGRITY__
    try
    {
      u = a->allocate(dims, size, _type, hwptr, step.p, 0, usageFlags);
    }
    catch(...)
    {
      if(a != a0)
        u = a0->allocate(dims, size, _type, hwptr, step.p, 0, usageFlags);
    }
    #else
    u = a->allocate(dims, size, _type, hwptr, step.p, 0, USAGE_DEFAULT);
    #endif
  }
  
  u->flags |= UMatData::USER_ALLOCATED;
 
  finalizeHdr(*this);
  addref();
}

/*================================================================================================*/
/* Get Mat function                                                                               */
/*================================================================================================*/
vsdk::Mat vsdk::CustomPointerUMat::getMat(int32_t accessFlags) const
{
  if(!u)
    return vsdk::Mat();
 
  // TODO Support ACCESS_READ (ACCESS_WRITE) without unnecessary data transfers
  accessFlags |= ACCESS_RW;
  vsdk::UMatDataAutoLock autolock(u);
   
  VSDK_CV_XADD(&u->refcount, 1);
  
  if (u->data != 0)
  {
    Mat hdr(dims, size.p, type(), u->data + offset, step.p);
    hdr.flags = flags;
    hdr.u = u;
    hdr.datastart = u->data;
    hdr.data = u->data + offset;
    hdr.datalimit = hdr.dataend = u->data + u->size;
    return hdr;
  }
  else
  {
    VSDK_CV_XADD(&u->refcount, -1);
    return Mat();
  }
}

vsdk::CustomPointerUMat::CustomPointerUMat(vsdk::UMatUsageFlags)
{
  flags = vsdk::UMat::MAGIC_VAL;
  dims = 0;
  rows = 0; 
  cols = 0; 
  allocator = 0;
  usageFlags = USAGE_DONT_ALLOCATE;
  u = 0;
  offset = 0;
  size = vsdk::MatSize(&rows);
}

#ifdef VSDK_UMAT_USE_OPENCV
  /*================================================================================================*/
  /* OpenCV Compatibility                                                                           */
  /*================================================================================================*/
  vsdk::UMat::operator cv::UMat() const
  {
    //return *(cv::UMat*)this;
    
    cv::UMat cvumat;
    
    cvumat.flags = flags; 
    cvumat.dims = dims; 
    cvumat.rows = rows; 
    cvumat.cols = cols; 
    cvumat.allocator = allocator;
    cvumat.usageFlags = (cv::UMatUsageFlags)usageFlags;
    cvumat.u = (cv::UMatData*)u;
    cvumat.offset = offset;
    cvumat.size = cv::MatSize(&(cvumat.rows));

    cvumat.addref();
    if( dims <= 2 )
    {
      cvumat.step[0] = step[0]; 
      cvumat.step[1] = step[1];
    }
    else
    {
      cvumat.dims = 0;
      cvumat.copySize(*(cv::UMat*)this);
    }
    
    return cvumat;
  }

  /*================================================================================================*/
  /* OpenCV Compatibility                                                                           */
  /*================================================================================================*/
  vsdk::UMat::UMat(const cv::UMat& m)
  : flags(m.flags), 
    dims(m.dims), 
    rows(m.rows), 
    cols(m.cols), 
    allocator(NULL),
    usageFlags((vsdk::UMatUsageFlags)m.usageFlags), 
    u(NULL), 
    offset(m.offset), 
    size(&rows)
  {
    *this = m;
  }

  /*================================================================================================*/
  /* OpenCV Compatibility                                                                           */
  /*================================================================================================*/
  vsdk::UMat& vsdk::UMat::operator = (const cv::UMat& m)
  {
    if(!m.empty() && OAL_MemoryReturnAddress(m.u->handle, ACCESS_PHY) != 0)
    {
      const_cast<cv::UMat&>(m).addref();
      release();
      flags = m.flags;
      if( dims <= 2 && m.dims <= 2 )
      {
        dims = m.dims;
        rows = m.rows;
        cols = m.cols;
        step[0] = m.step[0];
        step[1] = m.step[1];
      }
      else
        copySize(*(vsdk::UMat *)&m);
      allocator = (vsdk::UMatAllocator*)m.allocator;
      if (usageFlags == vsdk::USAGE_DEFAULT)
        usageFlags = (vsdk::UMatUsageFlags)m.usageFlags;
      u = (vsdk::UMatData*)m.u;
      offset = m.offset;
    }
    else
    {
      if (m.empty()) 
      {
        *this = vsdk::UMat();
      }
      else
      {
        release();
        int sz[] = {m.rows, m.cols};
        create(2, sz, m.type(), (vsdk::UMatUsageFlags)m.usageFlags);
        cv::Mat cvmat = m.getMat(0);
        vsdk::Mat vsdkmat = getMat(OAL_USAGE_CACHED);
        memcpy(vsdkmat.data, cvmat.data, total() * elemSize());
      }
    }
    return *this;
  }

  
#endif /* VSDK_UMAT_USE_OPENCV */
