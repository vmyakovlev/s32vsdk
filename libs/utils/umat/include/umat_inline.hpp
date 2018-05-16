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
#ifndef UMATINLINE_HPP
#define UMATINLINE_HPP

/*==================================================================================================
*                                INLINE FUNCTION DEFINITIONS
==================================================================================================*/

/*================================================================================================*/
/* Default constructor                                                                            */
/*================================================================================================*/
inline vsdk::UMat::UMat(vsdk::UMatUsageFlags _usageFlags)
: flags(vsdk::UMat::MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  allocator(0), 
  usageFlags(_usageFlags), 
  u(0), 
  offset(0), 
  size(&rows)
{
}

/*================================================================================================*/
/* Constructs 2D matrix of the specified size and type                                            */
/*================================================================================================*/
inline vsdk::UMat::UMat(int32_t _rows, int32_t _cols, int32_t _type, vsdk::UMatUsageFlags _usageFlags)
: flags(vsdk::UMat::MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  allocator(0), 
  usageFlags(_usageFlags), 
  u(0), 
  offset(0), 
  size(&rows)
{
  create(_rows, _cols, _type, _usageFlags);
}

/*================================================================================================*/
/* Constructs n-dimensional matrix                                                                */
/*================================================================================================*/
inline vsdk::UMat::UMat(int32_t _dims, const int32_t* _sz, int32_t _type, vsdk::UMatUsageFlags _usageFlags)
: flags(vsdk::UMat::MAGIC_VAL), 
  dims(0), 
  rows(0), 
  cols(0), 
  allocator(0), 
  usageFlags(_usageFlags), 
  u(0), 
  offset(0), 
  size(&rows)
{
  create(_dims, _sz, _type, _usageFlags);
}

/*================================================================================================*/
/* Copy constructor                                                                               */
/*================================================================================================*/
inline vsdk::UMat::UMat(const vsdk::UMat& m)
: flags(m.flags), 
  dims(m.dims), 
  rows(m.rows), 
  cols(m.cols), 
  allocator(m.allocator),
  usageFlags(m.usageFlags), 
  u(m.u), 
  offset(m.offset), 
  size(&rows)
{
  addref();
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
/* Increases the reference counter; use with care to avoid memleaks                               */
/*================================================================================================*/
inline void vsdk::UMat::addref()
{
  if( u )
    VSDK_CV_XADD(&(u->urefcount), 1);
  
  umat_printf("UMat: Add ref %d\n", u->urefcount);
}

/*================================================================================================*/
/* decreases reference counter;                                                                   */
/* deallocates the data when reference counter reaches 0.                                         */
/*================================================================================================*/
inline void vsdk::UMat::release()
{
  if( u && VSDK_CV_XADD(&(u->urefcount), -1) == 1 )
    deallocate();
  for(int32_t i = 0; i < dims; i++)
    size.p[i] = 0;
  u = 0;
}

/*================================================================================================*/
/* returns element type, similar to VSDK_CV_MAT_TYPE(cvmat->type)                                 */
/*================================================================================================*/
inline int32_t vsdk::UMat::type() const
{
  return VSDK_CV_MAT_TYPE(flags);
}

/*================================================================================================*/
/* returns the total number of matrix elements                                                    */
/*================================================================================================*/
inline uint64_t vsdk::UMat::total() const
{
  if( dims <= 2 )
    return (uint64_t)rows * cols;
  uint64_t p = 1;
  for( int32_t i = 0; i < dims; i++ )
    p *= size[i];
  return p;
}

/*================================================================================================*/
/* allocates new matrix data unless the matrix already has specified size and type.               */
/* previous data is unreferenced if needed.                                                       */
/*================================================================================================*/
inline void vsdk::UMat::create(int32_t _rows, int32_t _cols, int32_t _type, vsdk::UMatUsageFlags _usageFlags)
{
  _type &= vsdk::UMat::TYPE_MASK;
  if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && u )
    return;
  int sz[] = {_rows, _cols};
  create(2, sz, _type, _usageFlags);
}

/*================================================================================================*/
/* Returns a UMat from specified ROI                                                              */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::operator()(const vsdk::Range* ranges) const
{
  return vsdk::UMat(*this, ranges);
}

/*================================================================================================*/
/* Returns element size in bytes                                                                  */
/*================================================================================================*/
inline uint64_t vsdk::UMat::elemSize() const
{
  return dims > 0 ? step.p[dims - 1] : 0;
}

/*================================================================================================*/
/* Assigned operator                                                                              */
/*================================================================================================*/
inline vsdk::UMat& vsdk::UMat::operator = (const vsdk::UMat& m)
{
  if( this != &m )
  {
    const_cast<vsdk::UMat&>(m).addref();
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
    allocator = m.allocator;
    if (usageFlags == vsdk::USAGE_DEFAULT)
      usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
  }
  return *this;
}

/*================================================================================================*/
/* Returns true iff the matrix data is continuous                                                 */
/* (i.e. when there are no gaps between successive rows).                                         */
/* similar to VSDK_CV_IS_MAT_CONT(cvmat->type)                                                    */
/*================================================================================================*/
inline int8_t vsdk::UMat::isContinuous() const
{
  return (flags & vsdk::UMat::CONTINUOUS_FLAG) != 0;
}

/*================================================================================================*/
/* Returns true if the matrix is a submatrix of another matrix                                    */
/*================================================================================================*/
inline int8_t vsdk::UMat::isSubmatrix() const
{
  return (flags & vsdk::UMat::SUBMATRIX_FLAG) != 0;
}

/*================================================================================================*/
/* Returns the size of element channel in bytes.                                                  */
/*================================================================================================*/
inline uint64_t vsdk::UMat::elemSize1() const
{
  return VSDK_CV_ELEM_SIZE1(flags);
}

/*================================================================================================*/
/* Returns element type, similar to CV_MAT_DEPTH(cvmat->type)                                     */
/*================================================================================================*/
inline int32_t vsdk::UMat::depth() const
{
  return VSDK_CV_MAT_DEPTH(flags);
}

/*================================================================================================*/
/* Returns element type, similar to VSDK_CV_MAT_CN(cvmat->type)                                   */
/*================================================================================================*/
inline int32_t vsdk::UMat::channels() const
{
  return VSDK_CV_MAT_CN(flags);
}

/*================================================================================================*/
/* Returns step/elemSize1()                                                                       */
/*================================================================================================*/
inline uint64_t vsdk::UMat::step1(int32_t i) const
{
  return step.p[i] / elemSize1();
}

/*================================================================================================*/
/* Returns true if matrix data is NULL                                                            */
/*================================================================================================*/
inline int8_t vsdk::UMat::empty() const
{
  return u == 0 || total() == 0;
}

/*================================================================================================*/
/* Returns a UMat from specified ROI                                                              */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::operator()( vsdk::Range _rowRange, vsdk::Range _colRange ) const
{
  return vsdk::UMat(*this, _rowRange, _colRange);
}

/*================================================================================================*/
/* Assign ROI operator                                                                            */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::operator()( const vsdk::Rect& roi ) const
{
  return vsdk::UMat(*this, roi);
}

/*================================================================================================*/
/* Returns a new matrix header for the specified row                                              */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::row(int32_t y) const
{
  return vsdk::UMat(*this, vsdk::Range(y, y + 1), vsdk::Range::all());
}

/*================================================================================================*/
/* Returns a new matrix header for the specified column                                           */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::col(int32_t x) const
{
  return vsdk::UMat(*this, vsdk::Range::all(), vsdk::Range(x, x + 1));
}

/*================================================================================================*/
/* Returns a new matrix header for the specified row span                                         */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::rowRange(int32_t startrow, int32_t endrow) const
{
  return vsdk::UMat(*this, vsdk::Range(startrow, endrow), vsdk::Range::all());
}

/*================================================================================================*/
/* Returns a new matrix header for the specified row span                                         */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::rowRange(const vsdk::Range& r) const
{
  return vsdk::UMat(*this, r, vsdk::Range::all());
}

/*================================================================================================*/
/* Returns a new matrix header for the specified column span                                      */
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::colRange(int32_t startcol, int32_t endcol) const
{
  return vsdk::UMat(*this, vsdk::Range::all(), vsdk::Range(startcol, endcol));
}

/*================================================================================================*/
/* Returns a new matrix header for the specified column span                                      */ 
/*================================================================================================*/
inline vsdk::UMat vsdk::UMat::colRange(const vsdk::Range& r) const
{
  return vsdk::UMat(*this, vsdk::Range::all(), r);
}

#endif /* UMATINLINE_HPP */
