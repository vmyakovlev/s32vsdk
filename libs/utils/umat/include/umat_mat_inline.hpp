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
#ifndef UMATMATINLINE_HPP
#define UMATMATINLINE_HPP

/*================================================================================================*/
/* Function returns true is range is empty                                                        */
/* These are various constructors that form a matrix. As noted in the AutomaticAllocation, often  */
/* the default constructor is enough, and the proper matrix will be allocated by an OpenCV        */
/* function.                                                                                      */
/* The constructed matrix can further be assigned to another matrix or matrix expression or can be*/
/* allocated with Mat::create . In the former case, the old content is de-referenced.             */
/*================================================================================================*/
inline vsdk::Mat::Mat()
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
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
inline vsdk::Mat::Mat(int32_t _rows, int32_t _cols, int32_t _type)
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
  create(_rows, _cols, _type);
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
inline vsdk::Mat::Mat(int32_t _dims, const int32_t* _sz, int32_t _type)
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
  create(_dims, _sz, _type);
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
inline vsdk::Mat::Mat(const vsdk::Mat& m)
: flags(m.flags), 
  dims(m.dims), 
  rows(m.rows), 
  cols(m.cols), 
  data(m.data),
  datastart(m.datastart), 
  dataend(m.dataend), 
  datalimit(m.datalimit), 
  allocator(m.allocator),
  u(m.u), 
  size(&rows)
{
  if( u )
    VSDK_CV_XADD(&u->refcount, 1);
  if( m.dims <= 2 )
  {
    step[0] = m.step[0]; step[1] = m.step[1];
  }
  else
  {
    dims = 0;
    copySize(m);
  }
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
inline vsdk::Mat::Mat(int32_t _rows, int32_t _cols, int32_t _type, void* _data, uint64_t _step)
: flags(vsdk::Mat::MAGIC_VAL + (_type & vsdk::Mat::TYPE_MASK)), 
  dims(2), 
  rows(_rows), 
  cols(_cols),
  data((uint8_t*)_data), 
  datastart((uint8_t*)_data), 
  dataend(0), 
  datalimit(0),
  allocator(0), 
  u(0), 
  size(&rows)
{
  if (total() == 0 || data == NULL)
  {
    printf("Data in vsdk::Mat must be != NULL and size must be > 1. Exitting...\n");
    exit(-1);
  }
  
  uint64_t esz = VSDK_CV_ELEM_SIZE(_type), esz1 = VSDK_CV_ELEM_SIZE1(_type);
  uint64_t minstep = cols * esz;
  if( _step == vsdk::Mat::AUTO_STEP )
  {
    _step = minstep;
    flags |= vsdk::Mat::CONTINUOUS_FLAG;
  }
  else
  {
    if( rows == 1 ) _step = minstep;
    
    if (_step % esz1 != 0)
    {
      printf("Step must be a multiple of esz1");
    }

    flags |= _step == minstep ? vsdk::Mat::CONTINUOUS_FLAG : 0;
  }
    
  step[0] = _step;
  step[1] = esz;
  datalimit = datastart + _step * rows;
  dataend = datalimit - _step + minstep;
}

/*================================================================================================*/
/* destructor - calls release()                                                                   */
/*================================================================================================*/
inline vsdk::Mat::~Mat()
{
  release();
  if( step.p != step.buf )
    free(step.p);
}

/*================================================================================================*/
/* Assignment operators                                                                           */
/*================================================================================================*/
inline vsdk::Mat& Mat::operator = (const vsdk::Mat& m)
{
  if( this != &m )
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
    allocator = m.allocator;
    u = m.u;
  }
  return *this;
}

/*================================================================================================*/
/* Creates a matrix header for the specified matrix row.                                          */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::row(int32_t y) const
{
  return vsdk::Mat(*this, vsdk::Range(y, y + 1), vsdk::Range::all());
}

/*================================================================================================*/
/* Creates a matrix header for the specified matrix column.                                       */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::col(int32_t x) const
{
  return vsdk::Mat(*this, vsdk::Range::all(), vsdk::Range(x, x + 1));
}

/*================================================================================================*/
/* Creates a matrix header for the specified row span.                                            */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::rowRange(int32_t startrow, int32_t endrow) const
{
  return vsdk::Mat(*this, vsdk::Range(startrow, endrow), vsdk::Range::all());
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::rowRange(const vsdk::Range& r) const
{
  return vsdk::Mat(*this, r, vsdk::Range::all());
}

/*================================================================================================*/
/* Creates a matrix header for the specified column span.                                         */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::colRange(int32_t startcol, int32_t endcol) const
{
  return vsdk::Mat(*this, vsdk::Range::all(), vsdk::Range(startcol, endcol));
}

/*================================================================================================*/
/* @overload                                                                                      */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::colRange(const vsdk::Range& r) const
{
  return vsdk::Mat(*this, vsdk::Range::all(), r);
}

/*================================================================================================*/
/* Allocates new array data if needed.                                                            */
/*================================================================================================*/
inline void vsdk::Mat::create(int32_t _rows, int32_t _cols, int32_t _type)
{
  _type &= vsdk::Mat::TYPE_MASK;
  if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
    return;
  int sz[] = {_rows, _cols};
  create(2, sz, _type);
}

/*================================================================================================*/
/* Increments the reference counter.                                                              */
/*================================================================================================*/
inline void vsdk::Mat::addref()
{
  if( u )
    VSDK_CV_XADD(&u->refcount, 1);
}

/*================================================================================================*/
/* Decrements the reference counter and deallocates the matrix if needed.                         */
/*================================================================================================*/
inline void vsdk::Mat::release()
{
  if( u && VSDK_CV_XADD(&u->refcount, -1) == 1 )
    deallocate();
  u = NULL;
  datastart = dataend = datalimit = data = 0;
  for(int32_t i = 0; i < dims; i++)
    size.p[i] = 0;
}

/*================================================================================================*/
/* Extracts a rectangular submatrix.                                                              */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::operator()( vsdk::Range _rowRange, vsdk::Range _colRange ) const
{
  return vsdk::Mat(*this, _rowRange, _colRange);
}

/*================================================================================================*/
/* Extracts a rectangular submatrix.                                                              */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::operator()( const vsdk::Rect& roi ) const
{
  return vsdk::Mat(*this, roi);
}

/*================================================================================================*/
/* Extracts a rectangular submatrix.                                                              */
/*================================================================================================*/
inline vsdk::Mat vsdk::Mat::operator()(const vsdk::Range* ranges) const
{
  return vsdk::Mat(*this, ranges);
}

/*================================================================================================*/
/* Reports whether the matrix is continuous or not.                                               */
/*================================================================================================*/
inline int8_t vsdk::Mat::isContinuous() const
{
  return (flags & vsdk::Mat::CONTINUOUS_FLAG) != 0;
}

/*================================================================================================*/
/* Returns true if the matrix is a submatrix of another matrix                                    */
/*================================================================================================*/
inline int8_t vsdk::Mat::isSubmatrix() const
{
  return (flags & vsdk::Mat::SUBMATRIX_FLAG) != 0;
}

/*================================================================================================*/
/* Returns the matrix element size in bytes.                                                      */
/*================================================================================================*/
inline uint64_t vsdk::Mat::elemSize() const
{
  return dims > 0 ? step.p[dims - 1] : 0;
}

/*================================================================================================*/
/* Returns the size of each matrix element channel in bytes.                                      */
/*================================================================================================*/
inline uint64_t vsdk::Mat::elemSize1() const
{
  return VSDK_CV_ELEM_SIZE1(flags);
}

/*================================================================================================*/
/* Returns the type of a matrix element.                                                          */
/*================================================================================================*/
inline int32_t vsdk::Mat::type() const
{
  return VSDK_CV_MAT_TYPE(flags);
}

/*================================================================================================*/
/* Returns the depth of a matrix element.                                                         */
/*================================================================================================*/
inline int32_t vsdk::Mat::depth() const
{
  return VSDK_CV_MAT_DEPTH(flags);
}

/*================================================================================================*/
/* Returns the number of matrix channels.                                                         */
/*================================================================================================*/
inline int32_t vsdk::Mat::channels() const
{
  return VSDK_CV_MAT_CN(flags);
}

/*================================================================================================*/
/* Returns a normalized step.                                                                     */
/*================================================================================================*/
inline uint64_t vsdk::Mat::step1(int32_t i) const
{
  return step.p[i] / elemSize1();
}

/*================================================================================================*/
/* Returns true if the array has no elements.                                                     */
/*================================================================================================*/
inline int8_t vsdk::Mat::empty() const
{
  return data == 0 || total() == 0;
}

/*================================================================================================*/
/* Returns the total number of array elements.                                                    */
/*================================================================================================*/
inline uint64_t vsdk::Mat::total() const
{
  if( dims <= 2 )
    return (uint64_t)rows * cols;
  uint64_t p = 1;
  for( int32_t i = 0; i < dims; i++ )
    p *= size[i];
  return p;
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline uint8_t* vsdk::Mat::ptr(int32_t y)
{
  return data + step.p[0] * y;
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline const uint8_t* vsdk::Mat::ptr(int32_t y) const
{
  return data + step.p[0] * y;
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
template<typename _Tp> inline _Tp* vsdk::Mat::ptr(int32_t y)
{
  return (_Tp*)(data + step.p[0] * y);
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
template<typename _Tp> inline const _Tp* vsdk::Mat::ptr(int32_t y) const
{
  return (const _Tp*)(data + step.p[0] * y);
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline uint8_t* vsdk::Mat::ptr(int32_t i0, int32_t i1)
{
  return data + i0 * step.p[0] + i1 * step.p[1];
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline const uint8_t* vsdk::Mat::ptr(int32_t i0, int32_t i1) const
{
  return data + i0 * step.p[0] + i1 * step.p[1];
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
template<typename _Tp> inline _Tp* vsdk::Mat::ptr(int32_t i0, int32_t i1)
{
  return (_Tp*)(data + i0 * step.p[0] + i1 * step.p[1]);
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
template<typename _Tp> inline const _Tp* vsdk::Mat::ptr(int32_t i0, int32_t i1) const
{
  return (const _Tp*)(data + i0 * step.p[0] + i1 * step.p[1]);
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline uint8_t* vsdk::Mat::ptr(int32_t i0, int32_t i1, int32_t i2)
{
  return data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2];
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline const uint8_t* vsdk::Mat::ptr(int32_t i0, int32_t i1, int32_t i2) const
{
  return data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2];
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
template<typename _Tp> inline _Tp* vsdk::Mat::ptr(int32_t i0, int32_t i1, int32_t i2)
{
  return (_Tp*)(data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2]);
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
template<typename _Tp> inline const _Tp* vsdk::Mat::ptr(int32_t i0, int32_t i1, int32_t i2) const
{
  return (const _Tp*)(data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2]);
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline uint8_t* vsdk::Mat::ptr(const int32_t* idx)
{
  int32_t i, d = dims;
  uint8_t* p = data;
  
  for( i = 0; i < d; i++ )
  {
    p += idx[i] * step.p[i];
  }
  return p;
}

/*================================================================================================*/
/* Returns a pointer to the specified matrix row.                                                 */
/*================================================================================================*/
inline const uint8_t* vsdk::Mat::ptr(const int32_t* idx) const
{
  int32_t i, d = dims;
  uint8_t* p = data;
  for( i = 0; i < d; i++ )
  {
    p += idx[i] * step.p[i];
  }
  return p;
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline _Tp& vsdk::Mat::at(int32_t i0, int32_t i1)
{
  return ((_Tp*)(data + step.p[0] * i0))[i1];
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline const _Tp& vsdk::Mat::at(int32_t i0, int32_t i1) const
{
  return ((const _Tp*)(data + step.p[0] * i0))[i1];
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline _Tp& vsdk::Mat::at(vsdk::Point pt)
{
  return ((_Tp*)(data + step.p[0] * pt.y))[pt.x];
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline const _Tp& vsdk::Mat::at(vsdk::Point pt) const
{
  return ((const _Tp*)(data + step.p[0] * pt.y))[pt.x];
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline _Tp& vsdk::Mat::at(int32_t i0)
{
  if( isContinuous() || size.p[0] == 1 )
    return ((_Tp*)data)[i0];
  if( size.p[1] == 1 )
    return *(_Tp*)(data + step.p[0] * i0);
  int32_t i = i0 / cols, j = i0 - i * cols;
  return ((_Tp*)(data + step.p[0] * i))[j];
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline const _Tp& vsdk::Mat::at(int32_t i0) const
{
  if( isContinuous() || size.p[0] == 1 )
    return ((const _Tp*)data)[i0];
  if( size.p[1] == 1 )
    return *(const _Tp*)(data + step.p[0] * i0);
  int32_t i = i0 / cols, j = i0 - i * cols;
  return ((const _Tp*)(data + step.p[0] * i))[j];
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline _Tp& vsdk::Mat::at(int32_t i0, int32_t i1, int32_t i2)
{
  return *(_Tp*)ptr(i0, i1, i2);
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline const _Tp& vsdk::Mat::at(int32_t i0, int32_t i1, int32_t i2) const
{
  return *(const _Tp*)ptr(i0, i1, i2);
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline _Tp& vsdk::Mat::at(const int32_t* idx)
{
  return *(_Tp*)ptr(idx);
}

/*================================================================================================*/
/* Returns a reference to the specified array element.                                            */
/*================================================================================================*/
template<typename _Tp> inline const _Tp& vsdk::Mat::at(const int32_t* idx) const
{
  return *(const _Tp*)ptr(idx);
}

#endif /* UMATMATINLINE_HPP */
