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
#ifndef UMATTYPES_HPP
#define UMATTYPES_HPP

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include <stdint.h>
#include <algorithm>

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
namespace vsdk
{
  /*================================================================================================*/
  /**
  * @brief          Class encapsulating range definition
  */
  class Range
  {
    public:
      /**
      * @brief          Default constructor
      */
      Range();
      
      /**
      * @brief          Constructor with specified values
      *
      * @param[in]      _start      Starting index of the range
      * @param[in]      _end        Last index of the range
      */
      Range(int32_t _start, int32_t _end);
      
      /**
      * @brief          Function returns size of the range
      *
      * @return         Computed size of the range
      */
      int32_t size() const;
      
      /**
      * @brief          Function returns true is range is empty
      *
      * @return         True if size of the range is 0
      */
      int8_t empty() const;
      
      /**
      * @brief          Function returns min to max range
      *
      * @return         Range which specifies whole original range
      */
      static vsdk::Range all();

      /**
      * @brief          Min and max index
      *
      */
      int32_t start, end;
  }; /* class Range */
  
  /*================================================================================================*/
  /**
  * @brief          Structure encapsulating sizes of the matrix
  */
  struct MatSize
  {
    /**
    * @brief          Default constructor
    *
    * @param[in]      _p            Array of sizes passed to constructor
    *
    */
    explicit MatSize(int32_t* _p);
    
    /**
    * @brief          Get array element operator - returns the size on specified index (const)
    *
    * @param[in]      i             Index to return
    *
    * @return         value of the returned size
    */
    const int32_t& operator[](int32_t i) const;
    
    /**
    * @brief          Get array element operator - returns the size on specified index
    *
    * @param[in]      i             Index to return
    *
    * @return         value of the returned size
    */
    int32_t& operator[](int32_t i);
    
    /**
    * @brief          Pointer retype operator
    *
    * @return         Value of the returned pointer
    */
    operator const int32_t*() const;
    
    /**
    * @brief          Comparison operator
    *
    * @param[in]      sz            Second MatSize to compare
    *
    * @return         True if objects are equal
    */
    int8_t operator == (const vsdk::MatSize& sz) const;
    
    /**
    * @brief          Comparison operator
    *
    * @param[in]      sz            Second MatSize to compare
    *
    * @return         True if objects are not equal
    */
    int8_t operator != (const vsdk::MatSize& sz) const;

    /**
    * @brief          Internal structure pointer
    */
    int32_t* p;
  }; /* struct MatSize */

  /*================================================================================================*/
  /**
  * @brief          Structure encapsulating Matrix step values
  */
  struct MatStep
  {
    /**
    * @brief          Default constructor
    */
    MatStep();
    
    /**
    * @brief          Constructor with specified value
    *
    * @param[in]      s             First step value
    *
    */
    explicit MatStep(uint64_t s);
    
    /**
    * @brief          Get array element operator - returns the step on specified index (const)
    *
    * @param[in]      i             Index to return
    *
    * @return         value of the returned step
    */
    const uint64_t& operator[](int32_t i) const;
    
    /**
    * @brief          Get array element operator - returns the step on specified index
    *
    * @param[in]      i              Index to return
    *
    * @return         value of the returned step
    */
    uint64_t& operator[](int32_t i);
    
    /**
    * @brief          Pointer retype operator
    *
    * @return         value of the returned pointer
    */
    operator uint64_t() const;
    
    /**
    * @brief          Assignment operator
    *
    * @param[in]      s              Step value to be assigned
    *
    * @return         Resulting MatStep object
    */
    MatStep& operator = (uint64_t s);

    /**
    * @brief          Internal buffer pointer
    */
    uint64_t* p;
    
    /**
    * @brief          Internal buffer
    */
    uint64_t buf[3];
  protected:
    /**
    * @brief          Assignment copy operator
    *
    * @return         Resulting MatStep object
    */
    MatStep& operator = (const vsdk::MatStep&);
  }; /* struct MatStep */
  
  /*================================================================================================*/
  /**
  * @brief          Class describing 2D point type
  */
  template<typename _Tp> class Point_
  {
    public:
      /**
      * @brief          Type of point element
      */
      typedef _Tp value_type;

      /**
      * @brief          Default constructor
      */
      Point_();
      
      /**
      * @brief          Constructor - 2D point
      *
      * @param[in]      _x           X value
      * @param[in]      _y           Y value
      */
      Point_(_Tp _x, _Tp _y);
      
      /**
      * @brief          Copy constructor
      *
      * @param[in]      pt           Point to be copied
      */
      Point_(const Point_& pt);
      
      /**
      * @brief          Assignment operator
      *
      * @param[in]      pt           Point to be assigned
      */
      Point_& operator = (const Point_& pt);
      
      /**
      * @brief          Point coordinates
      */
      _Tp x, y;
  }; /* Point_ */

  typedef Point_<int32_t> Point2i;
  typedef Point2i Point;
  
  /*================================================================================================*/
  /**
  * @brief          Class describing a rectangle
  */
  template<typename _Tp> class Rect_
  {
    public:
      /**
      * @brief          Type of point element
      */
      typedef _Tp value_type;

      /**
      * @brief          Default constructor
      */
      Rect_();
      
      /**
      * @brief          Constructor - 2D point
      *
      * @param[in]      _x           X origin value
      * @param[in]      _y           Y origin value
      * @param[in]      _width       Width
      * @param[in]      _height      Height
      */
      Rect_(_Tp _x, _Tp _y, _Tp _width, _Tp _height);
      
      /**
      * @brief          Copy constructor
      *
      * @param[in]      r            Rectangle to be copied from
      */
      Rect_(const Rect_& r);
      
      /**
      * @brief          Constructor - defined by two corner points
      *
      * @param[in]      pt1          Point 1
      * @param[in]      pt2          Point 2
      */
      Rect_(const vsdk::Point_<_Tp>& pt1, const vsdk::Point_<_Tp>& pt2);

      /**
      * @brief        Assignment operator
      *
      * @param[in]    r              Rectangle to be assigned
      */
      Rect_& operator = ( const Rect_& r );
      
      /**
      * @brief        Top left corner
      *
      * @return       Top left corner
      */
      Point_<_Tp> tl() const;
      
      /**
      * @brief        Bottom right corner
      *
      * @return       Bottom right corner
      */
      Point_<_Tp> br() const;

      /**
      * @brief        The top-left corner, as well as width and height of the rectangle
      */
      _Tp x, y, width, height; 
  }; /* Rect_ */
  typedef Rect_<int> Rect2i;
  typedef Rect2i Rect;

  #include "umat_types_inline.hpp"

} /* namespace vsdk */
#endif /* UMATTYPES_HPP */

