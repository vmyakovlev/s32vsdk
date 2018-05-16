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
#ifndef UMATTYPESINLINE_HPP
#define UMATTYPESINLINE_HPP

/*==================================================================================================
*                                INLINE FUNCTION DEFINITIONS
==================================================================================================*/

/*================================================================================================*/
/* Default constructor                                                                            */
/*================================================================================================*/
inline vsdk::Range::Range()
: start(0), 
  end(0) 
{
}

/*================================================================================================*/
/* Constructor with specified values                                                              */
/*================================================================================================*/
inline vsdk::Range::Range(int32_t _start, int32_t _end) 
: start(_start), 
  end(_end) 
{
}

/*================================================================================================*/
/* Function returns size of the range                                                             */
/*================================================================================================*/
inline int32_t vsdk::Range::size() const
{
    return end - start;
}

/*================================================================================================*/
/* Function returns true is range is empty                                                        */
/*================================================================================================*/
inline int8_t vsdk::Range::empty() const
{
    return start == end;
}

/*================================================================================================*/
/* Function returns min to max range                                                              */
/*================================================================================================*/
inline vsdk::Range vsdk::Range::all()
{
    return vsdk::Range(INT32_MIN, INT32_MAX);
}

/*================================================================================================*/
/* Equal operator                                                                                 */
/*================================================================================================*/
static inline bool operator == (const vsdk::Range& r1, const vsdk::Range& r2)
{
    return r1.start == r2.start && r1.end == r2.end;
}

/*================================================================================================*/
/* Non equal operator                                                                             */
/*================================================================================================*/
static inline bool operator != (const vsdk::Range& r1, const vsdk::Range& r2)
{
  return !(r1 == r2);
}

/*================================================================================================*/
/* Default constructor                                                                            */
/*================================================================================================*/
inline vsdk::MatSize::MatSize(int32_t* _p) 
: p(_p) 
{
}

/*================================================================================================*/
/* Get array element operator - returns the size on specified index (const)                       */
/*================================================================================================*/
inline const int32_t& vsdk::MatSize::operator[](int32_t i) const
{
  return p[i];
}

/*================================================================================================*/
/* Get array element operator - returns the size on specified index                               */
/*================================================================================================*/
inline int32_t& vsdk::MatSize::operator[](int32_t i)
{
  return p[i];
}

/*================================================================================================*/
/* Pointer retype operator                                                                        */
/*================================================================================================*/
inline vsdk::MatSize::operator const int32_t*() const
{
  return p;
}

/*================================================================================================*/
/* Comparison operator                                                                            */
/*================================================================================================*/
inline int8_t vsdk::MatSize::operator == (const vsdk::MatSize& sz) const
{
  int32_t d = p[-1];
  int32_t dsz = sz.p[-1];
  if( d != dsz )
    return false;
  if( d == 2 )
    return p[0] == sz.p[0] && p[1] == sz.p[1];

  for( int32_t i = 0; i < d; i++ )
    if( p[i] != sz.p[i] )
      return false;
  return true;
}

/*================================================================================================*/
/* Comparison operator                                                                            */
/*================================================================================================*/
inline int8_t vsdk::MatSize::operator != (const vsdk::MatSize& sz) const
{
  return !(*this == sz);
}

/*================================================================================================*/
/* Default constructor                                                                            */
/*================================================================================================*/
inline vsdk::MatStep::MatStep()
{
  p = buf; p[0] = p[1] = p[2] = 1;
}

/*================================================================================================*/
/* Constructor with specified value                                                               */
/*================================================================================================*/
inline vsdk::MatStep::MatStep(uint64_t s)
{
  p = buf; p[0] = s; p[1] = 0; p[2] = 1;
}

/*================================================================================================*/
/* Get array element operator - returns the step on specified index (const)                       */
/*================================================================================================*/
inline const uint64_t& vsdk::MatStep::operator[](int32_t i) const
{
  return p[i];
}

/*================================================================================================*/
/* Get array element operator - returns the step on specified index                               */
/*================================================================================================*/
inline uint64_t& vsdk::MatStep::operator[](int32_t i)
{
  return p[i];
}

/*================================================================================================*/
/* Get array element operator - returns the step on specified index                               */
/*================================================================================================*/
inline vsdk::MatStep::operator uint64_t() const
{
  return buf[0];
}

/*================================================================================================*/
/* Assignment operator                                                                            */
/*================================================================================================*/
inline vsdk::MatStep& vsdk::MatStep::operator = (uint64_t s)
{
  buf[0] = s;
  return *this;
}

/*================================================================================================*/
/* Default constructor                                                                            */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Point_<_Tp>::Point_()
: x(0), y(0) 
{
}

/*================================================================================================*/
/* Constructor - 2D point                                                                         */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Point_<_Tp>::Point_(_Tp _x, _Tp _y)
: x(_x), y(_y) 
{
}

/*================================================================================================*/
/* Copy constructor                                                                               */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Point_<_Tp>::Point_(const Point_& pt)
: x(pt.x), y(pt.y) 
{
}
    
/*================================================================================================*/
/* Assignment operator                                                                            */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Point_<_Tp>& Point_<_Tp>::operator = (const Point_& pt)
{
  x = pt.x; y = pt.y;
  return *this;
}

/*================================================================================================*/
/* Default constructor                                                                            */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Rect_<_Tp>::Rect_()
: x(0), 
  y(0), 
  width(0), 
  height(0) 
{
}

/*================================================================================================*/
/* Constructor - 2D point                                                                         */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Rect_<_Tp>::Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height)
: x(_x), 
  y(_y), 
  width(_width), 
  height(_height) 
{
}

/*================================================================================================*/
/* Copy constructor                                                                               */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Rect_<_Tp>::Rect_(const vsdk::Rect_<_Tp>& r)
: x(r.x), 
  y(r.y), 
  width(r.width), 
  height(r.height) 
{
}
    
/*================================================================================================*/
/* Constructor - defined by two corner points                                                     */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Rect_<_Tp>::Rect_(const vsdk::Point_<_Tp>& pt1, const vsdk::Point_<_Tp>& pt2)
{
  x = std::min(pt1.x, pt2.x);
  y = std::min(pt1.y, pt2.y);
  width = std::max(pt1.x, pt2.x) - x;
  height = std::max(pt1.y, pt2.y) - y;
}

/*================================================================================================*/
/* Assignment operator                                                                            */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Rect_<_Tp>& Rect_<_Tp>::operator = ( const vsdk::Rect_<_Tp>& r )
{
  x = r.x;
  y = r.y;
  width = r.width;
  height = r.height;
  return *this;
}

/*================================================================================================*/
/* Top left corner                                                                                */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Point_<_Tp> vsdk::Rect_<_Tp>::tl() const
{
  return vsdk::Point_<_Tp>(x,y);
}

/*================================================================================================*/
/* Bottom right corner                                                                            */
/*================================================================================================*/
template<typename _Tp> inline vsdk::Point_<_Tp> vsdk::Rect_<_Tp>::br() const
{
  return vsdk::Point_<_Tp>(x + width, y + height);
}

#endif /* UMATTYPESINLINE_HPP */


