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
#include "umat_mat.hpp"

/*==================================================================================================
*                                       FUNCTIONS
==================================================================================================*/

static vsdk::UMatAllocator __allocator;

/*================================================================================================*/
/* Function for setting size of the matrix                                                        */
/*================================================================================================*/
static inline void setSize( vsdk::Mat& m, int32_t _dims, const int32_t* _sz, const uint64_t* _steps, int8_t autoSteps=false )
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

  uint64_t esz = VSDK_CV_ELEM_SIZE(m.flags), esz1 = VSDK_CV_ELEM_SIZE1(m.flags), total = esz;
  int32_t i;
  for( i = _dims-1; i >= 0; i-- )
  {
    int32_t s = _sz[i];
    m.size.p[i] = s;

    if( _steps )
    {
      if (_steps[i] % esz1 != 0)
      {
        printf("Step must be a multiple of esz1");
      }

      m.step.p[i] = i < _dims-1 ? _steps[i] : esz;
    }
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
/* Function updates the continuity flag according to data                                         */
/*================================================================================================*/
static void updateContinuityFlag(vsdk::Mat& m)
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

  uint64_t t = (uint64_t)m.step[0]*m.size[0];
  if( j <= i && t == (uint64_t)t )
    m.flags |= vsdk::Mat::CONTINUOUS_FLAG;
  else
    m.flags &= ~vsdk::Mat::CONTINUOUS_FLAG;
}

/*================================================================================================*/
/* Function finalizes header                                                                      */
/*================================================================================================*/
static void finalizeHdr(vsdk::Mat& m)
{
  updateContinuityFlag(m);
  int32_t d = m.dims;
  if( d > 2 )
    m.rows = m.cols = -1;
  if(m.u)
    m.datastart = m.data = m.u->data;
  if( m.data )
  {
    m.datalimit = m.datastart + m.size[0]*m.step[0];
    if( m.size[0] > 0 )
    {
      m.dataend = m.ptr() + m.size[d-1]*m.step[d-1];
      for( int32_t i = 0; i < d-1; i++ )
        m.dataend += (m.size[i] - 1)*m.step[i];
    }
    else
      m.dataend = m.datalimit;
  }
  else
    m.dataend = m.datalimit = 0;
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
vsdk::Mat::Mat(int32_t _dims, const int32_t* _sizes, int32_t _type, void* _data, const uint64_t* _steps)
: flags(MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  data(0), 
  datastart(0), 
  dataend(0),
  datalimit(0), 
  allocator(0), 
  u(0), 
  size(&rows)
{
  flags |= VSDK_CV_MAT_TYPE(_type);
  datastart = data = (uint8_t*)_data;
  setSize(*this, _dims, _sizes, _steps, true);
  finalizeHdr(*this);
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
vsdk::Mat::Mat(const vsdk::Mat& m, const vsdk::Range& _rowRange, const vsdk::Range& _colRange)
: flags(vsdk::Mat::MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  data(0), 
  datastart(0), 
  dataend(0),
  datalimit(0), 
  allocator(0), 
  u(0), 
  size(&rows)
{
  if( m.dims > 2 )
  {
    vsdk::Range *rs = new vsdk::Range[m.dims];
    rs[0] = _rowRange;
    rs[1] = _colRange;
    for( int32_t i = 2; i < m.dims; i++ )
      rs[i] = Range::all();
    *this = m(rs);
    delete rs;
    return;
  }

  *this = m;
  if( _rowRange != vsdk::Range::all() && _rowRange != vsdk::Range(0,rows) )
  {
    rows = _rowRange.size();
    data += step*_rowRange.start;
    flags |= vsdk::Mat::SUBMATRIX_FLAG;
  }

  if( _colRange != vsdk::Range::all() && _colRange != vsdk::Range(0,cols) )
  {
    cols = _colRange.size();
    data += _colRange.start*elemSize();
    flags &= cols < m.cols ? ~vsdk::Mat::CONTINUOUS_FLAG : -1;
    flags |= vsdk::Mat::SUBMATRIX_FLAG;
  }

  if( rows == 1 )
    flags |= vsdk::Mat::CONTINUOUS_FLAG;

  if( rows <= 0 || cols <= 0 )
  {
    release();
    rows = cols = 0;
  }
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
vsdk::Mat::Mat(const vsdk::Mat& m, const vsdk::Rect& roi)
: 
  flags(m.flags), 
  dims(2), 
  rows(roi.height), 
  cols(roi.width),
  data(m.data + roi.y*m.step[0]),
  datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
  allocator(m.allocator), u(m.u), size(&rows)
{
  flags &= roi.width < m.cols ? ~vsdk::Mat::CONTINUOUS_FLAG : -1;
    flags |= roi.height == 1 ? vsdk::Mat::CONTINUOUS_FLAG : 0;

  size_t esz = VSDK_CV_ELEM_SIZE(flags);
  data += roi.x*esz;
  
  if( u )
    VSDK_CV_XADD(&u->refcount, 1);
  if( roi.width < m.cols || roi.height < m.rows )
    flags |= vsdk::Mat::SUBMATRIX_FLAG;

  step[0] = m.step[0]; step[1] = esz;

  if( rows <= 0 || cols <= 0 )
  {
    release();
    rows = cols = 0;
  }
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
vsdk::Mat::Mat(const vsdk::Mat& m, const vsdk::Range* ranges)
: flags(vsdk::Mat::MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  data(0), 
  datastart(0), 
  dataend(0),
  datalimit(0), 
  allocator(0), 
  u(0), 
  size(&rows)
{
  int32_t i, d = m.dims;
  
  *this = m;
  for( i = 0; i < d; i++ )
  {
    vsdk::Range r = ranges[i];
    if( r != vsdk::Range::all() && r != vsdk::Range(0, size.p[i]))
    {
      size.p[i] = r.end - r.start;
      data += r.start*step.p[i];
      flags |= vsdk::Mat::SUBMATRIX_FLAG;
    }
  }
  updateContinuityFlag(*this);
}

/*================================================================================================*/
/* Extracts a diagonal from a matrix                                                              */
/*================================================================================================*/
vsdk::Mat vsdk::Mat::diag(int32_t d) const
{
  vsdk::Mat m = *this;
  uint64_t esz = elemSize();
  int32_t len;

  if( d >= 0 )
  {
    len = std::min(cols - d, rows);
    m.data += esz*d;
  }
  else
  {
    len = std::min(rows + d, cols);
    m.data -= step[0]*d;
  }
  
  m.size[0] = m.rows = len;
  m.size[1] = m.cols = 1;
  m.step[0] += (len > 1 ? esz : 0);

  if( m.rows > 1 )
    m.flags &= ~vsdk::Mat::CONTINUOUS_FLAG;
  else
    m.flags |= vsdk::Mat::CONTINUOUS_FLAG;

  if( size[0] != 1 || size[1] != 1)
    m.flags |= vsdk::Mat::SUBMATRIX_FLAG;

  return m;
}

/*================================================================================================*/
/* Allocates new array data if needed.                                                            */
/*================================================================================================*/
void vsdk::Mat::create(int32_t d, const int32_t* _sizes, int32_t _type)
{
  int32_t i;
  _type = VSDK_CV_MAT_TYPE(_type);

  if( data && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
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
  flags = (_type & VSDK_CV_MAT_TYPE_MASK) | vsdk::Mat::MAGIC_VAL;
  setSize(*this, d, _sizes, 0, true);

  if( total() > 0 )
  {
    vsdk::UMatAllocator *a = allocator, *a0 = &__allocator;
    if(!a)
      a = a0;
    #if !defined __STANDALONE__ && !defined __INTEGRITY__
    try
    {
      u = a->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
      a->map(u, 0);
    }
    catch(...)
    {
      if(a != a0)
      {
        u = a0->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
        a->map(u, 0);
      }
    }
    #else
    u = a->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
    a->map(u, 0);
    #endif
  }
  addref();
  finalizeHdr(*this);
}

/*================================================================================================*/
/* Deallocates the matrix data                                                                    */
/*================================================================================================*/
void vsdk::Mat::deallocate()
{
  if(u)
    u->currAllocator->unmap(u);
  u = NULL;
}

/*================================================================================================*/
/* Internal use function; properly re-allocates _size, _step arrays                               */
/*================================================================================================*/
void vsdk::Mat::copySize(const vsdk::Mat& m)
{
  setSize(*this, m.dims, 0, 0);
  for( int32_t i = 0; i < dims; i++ )
  {
    size[i] = m.size[i];
    step[i] = m.step[i];
  }
}

/*================================================================================================*/
/* Returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1);          */
/* negative number otherwise                                                                      */
/*================================================================================================*/
int32_t vsdk::Mat::checkVector(int32_t _elemChannels, int32_t _depth, int8_t _requireContinuous) const
{
  return (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims == 2 && (((rows == 1 || cols == 1) && channels() == _elemChannels) ||
                        (cols == _elemChannels && channels() == 1))) ||
        (dims == 3 && channels() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (int32_t)(total()*channels()/_elemChannels) : -1;
}

#ifdef VSDK_UMAT_USE_OPENCV
/*================================================================================================*/
/* OpenCV compatibility overload                                                                  */
/*================================================================================================*/
vsdk::Mat& vsdk::Mat::operator = (const cv::Mat& m)
{
  if( m.u )
    VSDK_CV_XADD(&m.u->refcount, 1);
    
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
    copySize(m);
  data = m.data;
  datastart = m.datastart;
  dataend = m.dataend;
  datalimit = m.datalimit;
  allocator = (vsdk::UMatAllocator*)m.allocator;
  u = (vsdk::UMatData*)m.u;
  return *this;
}

/*================================================================================================*/
/* OpenCV compatibility overload                                                                  */
/*================================================================================================*/
vsdk::Mat::Mat(const cv::Mat& m)
: flags(vsdk::Mat::MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  data(0), 
  datastart(0), 
  dataend(0),
  datalimit(0), 
  allocator(0), 
  u(0), 
  size(&rows)
{
  *this = m;
}

/*================================================================================================*/
/* OpenCV compatibility overload                                                                  */
/*================================================================================================*/
vsdk::Mat::operator cv::Mat() const
{
  //return *(cv::UMat*)this;
  
  cv::Mat cvmat;
  
  cvmat.flags = flags; 
  cvmat.dims = dims; 
  cvmat.rows = rows; 
  cvmat.cols = cols; 
  cvmat.allocator = allocator;
  cvmat.u = (cv::UMatData*)u;
  cvmat.size = cv::MatSize(&(cvmat.rows));
  cvmat.data = data;
  cvmat.datastart = datastart;
  cvmat.dataend = dataend;
  cvmat.datalimit = datalimit;

  if( u )
    VSDK_CV_XADD(&u->refcount, 1);
  if( dims <= 2 )
  {
    cvmat.step[0] = step[0]; 
    cvmat.step[1] = step[1];
  }
  else
  {
    cvmat.dims = 0;
    cvmat.copySize(*(cv::Mat*)this);
  }
  
  return cvmat;
}
#endif

/*================================================================================================*/
/* The default constructor                                                                        */
/*================================================================================================*/
vsdk::NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

/*================================================================================================*/
/* The full constructor taking arbitrary number of n-dim matrices                                 */
/*================================================================================================*/
vsdk::NAryMatIterator::NAryMatIterator(const vsdk::Mat** _arrays, vsdk::Mat* _planes, int32_t _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

/*================================================================================================*/
/* The full constructor taking arbitrary number of n-dim matrices                                 */
/*================================================================================================*/
vsdk::NAryMatIterator::NAryMatIterator(const vsdk::Mat** _arrays, uint8_t** _ptrs, int32_t _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

/*================================================================================================*/
/* The separate iterator initialization method                                                    */
/*================================================================================================*/
void vsdk::NAryMatIterator::init(const vsdk::Mat** _arrays, vsdk::Mat* _planes, uint8_t** _ptrs, int32_t _narrays)
{
   int i, j, d1=0, i0 = -1, d = -1;

   arrays = _arrays;
   ptrs = _ptrs;
   planes = _planes;
   narrays = _narrays;
   nplanes = 0;
   size = 0;

   if( narrays < 0 )
   {
      for( i = 0; _arrays[i] != 0; i++ )
      ;
      narrays = i;
   }

   iterdepth = 0;

   for( i = 0; i < narrays; i++ )
   {
     const vsdk::Mat& A = *arrays[i];
     if( ptrs )
       ptrs[i] = A.data;

     if( !A.data )
       continue;
 
     if( i0 < 0 )
     {
       i0 = i;
       d = A.dims;

       for( d1 = 0; d1 < d; d1++ )
       if( A.size[d1] > 1 )
         break;
     }
     else
     {
     }
 
     if( !A.isContinuous() )
     {
       for( j = d-1; j > d1; j-- )
         if( A.step[j]*A.size[j] < A.step[j-1] )
           break;
       iterdepth = std::max(iterdepth, j);
     }
   }
 
   if( i0 >= 0 )
   {
     size = arrays[i0]->size[d-1];
     for( j = d-1; j > iterdepth; j-- )
     {
       int64 total1 = (int64)size*arrays[i0]->size[j-1];
       if( total1 != (int)total1 )
         break;
       size = (int)total1;
     }

     iterdepth = j;
     if( iterdepth == d1 )
       iterdepth = 0;
 
     nplanes = 1;
     for( j = iterdepth-1; j >= 0; j-- )
       nplanes *= arrays[i0]->size[j];
   }
   else
     iterdepth = 0;
 
   idx = 0;

   if( !planes )
     return;

   for( i = 0; i < narrays; i++ )
   {
     const Mat& A = *arrays[i];

     if( !A.data )
     {
       planes[i] = Mat();
       continue;
     }
     planes[i] = Mat(1, (int)size, A.type(), A.data);
  }
}

/*================================================================================================*/
/* Proceeds to the next plane of every iterated matrix                                            */
/*================================================================================================*/
vsdk::NAryMatIterator& vsdk::NAryMatIterator::operator ++()
{
    if( idx >= nplanes-1 )
        return *this;
    ++idx;

    if( iterdepth == 1 )
    {
        if( ptrs )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        if( planes )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    else
    {
        for( int i = 0; i < narrays; i++ )
        {
            const Mat& A = *arrays[i];
            if( !A.data )
                continue;
            int _idx = (int)idx;
            uchar* data = A.data;
            for( int j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                int szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A.step[j];
                _idx = t;
            }
            if( ptrs )
                ptrs[i] = data;
            if( planes )
                planes[i].data = data;
        }
    }

    return *this;
}

/*================================================================================================*/
/* Proceeds to the next plane of every iterated matrix (postfix increment operator)               */
/*================================================================================================*/
vsdk::NAryMatIterator vsdk::NAryMatIterator::operator ++(int)
{
    NAryMatIterator it = *this;
    ++*this;
    return it;
}