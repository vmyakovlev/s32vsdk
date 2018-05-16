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
#ifndef UMATMAT_HPP
#define UMATMAT_HPP

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "umat_defines.hpp"
#include "umat_types.hpp"
#include "umat_data.hpp"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
namespace vsdk
{
  /*================================================================================================*/
  /**
  * @brief          A Mat class copying and enhancing the behavior from OpenCV
  */
  class Mat
  {
    public:
      
      enum { MAGIC_VAL  = 0x42FF0000, AUTO_STEP = 0, CONTINUOUS_FLAG = VSDK_CV_MAT_CONT_FLAG, SUBMATRIX_FLAG = VSDK_CV_SUBMAT_FLAG };
      enum { MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7 };
      
      /**
      * @brief          Constructor, creates and initializes the Mat
      * 
      * These are various constructors that form a matrix. As noted in the AutomaticAllocation, often
      * the default constructor is enough, and the proper matrix will be allocated by an OpenCV function.
      * The constructed matrix can further be assigned to another matrix or matrix expression or can be
      * allocated with Mat::create . In the former case, the old content is de-referenced.
      */
      Mat();

      /**
      * @overload
      *
      * @param[in]      rows         Number of rows in a 2D array.
      * @param[in]      cols         Number of columns in a 2D array.
      * @param[in]      type         Array type. Use CV_8UC1, ..., CV_64FC4 to create 1-4 channel matrices, or
      *                              CV_8UC(n), ..., CV_64FC(n) to create multi-channel (up to CV_CN_MAX channels) matrices.
      */
      Mat(int32_t rows, int32_t cols, int32_t type);

      /**
      * @overload
      *
      * @param[in]      ndims        Array dimensionality.
      * @param[in]      sizes        Array of integers specifying an n-dimensional array shape.
      * @param[in]      type         Array type. Use CV_8UC1, ..., CV_64FC4 to create 1-4 channel matrices, or
      *                              CV_8UC(n), ..., CV_64FC(n) to create multi-channel (up to CV_CN_MAX channels) matrices.
      */
      Mat(int32_t ndims, const int32_t* sizes, int32_t type);
      
      /**
      * @overload
      *
      * @param[in]      m            Array that (as a whole or partly) is assigned to the constructed matrix. No data is copied
      *                              by these constructors. Instead, the header pointing to m data or its sub-array is constructed and
      *                              associated with it. The reference counter, if any, is incremented. So, when you modify the matrix
      *                              formed using such a constructor, you also modify the corresponding elements of m . If you want to
      *                              have an independent copy of the sub-array, use Mat::clone() .
      */
      Mat(const vsdk::Mat& m);
      
      /**
      * @overload
      *
      * @param[in]      rows         Number of rows in a 2D array.
      * @param[in]      cols         Number of columns in a 2D array.
      * @param[in]      type         Array type. Use CV_8UC1, ..., CV_64FC4 to create 1-4 channel matrices, or
      *                              CV_8UC(n), ..., CV_64FC(n) to create multi-channel (up to CV_CN_MAX channels) matrices.
      * @param[in]      data         Pointer to the user data. Matrix constructors that take data and step parameters do not
      *                              allocate matrix data. Instead, they just initialize the matrix header that points to the specified
      *                              data, which means that no data is copied. This operation is very efficient and can be used to
      *                              process external data using OpenCV functions. The external data is not automatically deallocated, so
      *                              you should take care of it.
      * @param[in]      step         Number of bytes each matrix row occupies. The value should include the padding bytes at
      *                              the end of each row, if any. If the parameter is missing (set to AUTO_STEP ), no padding is assumed
      *                              and the actual step is calculated as cols*elemSize(). See Mat::elemSize.
      */
      Mat(int32_t rows, int32_t cols, int32_t type, void* data, uint64_t step=vsdk::Mat::AUTO_STEP);
      
      /**
      * @overload
      *
      * @param[in]      ndims        Array dimensionality.
      * @param[in]      sizes        Array of integers specifying an n-dimensional array shape.
      * @param[in]      type         Array type. Use CV_8UC1, ..., CV_64FC4 to create 1-4 channel matrices, or
      *                              CV_8UC(n), ..., CV_64FC(n) to create multi-channel (up to CV_CN_MAX channels) matrices.
      * @param[in]      data         Pointer to the user data. Matrix constructors that take data and step parameters do not
      *                              allocate matrix data. Instead, they just initialize the matrix header that points to the specified
      *                              data, which means that no data is copied. This operation is very efficient and can be used to
      *                              process external data using OpenCV functions. The external data is not automatically deallocated, so
      *                              you should take care of it.
      * @param[in]      steps        Array of ndims-1 steps in case of a multi-dimensional array (the last step is always
      *                              set to the element size). If not specified, the matrix is assumed to be continuous.
      */
      Mat(int32_t ndims, const int32_t* sizes, int32_t type, void* data, const uint64_t* steps=0);
      
      /**
      * @overload
      *
      * @param[in]      m            Array that (as a whole or partly) is assigned to the constructed matrix. No data is copied
      *                              by these constructors. Instead, the header pointing to m data or its sub-array is constructed and
      *                              associated with it. The reference counter, if any, is incremented. So, when you modify the matrix
      *                              formed using such a constructor, you also modify the corresponding elements of m . If you want to
      *                              have an independent copy of the sub-array, use Mat::clone() .
      * @param[in]      rowRange     Range of the m rows to take. As usual, the range start is inclusive and the range
      *                              end is exclusive. Use Range::all() to take all the rows.
      * @param[in]      colRange     Range of the m columns to take. Use Range::all() to take all the columns.
      */
      Mat(const vsdk::Mat& m, const vsdk::Range& rowRange, const vsdk::Range& colRange=vsdk::Range::all());

      /**
      * @overload
      *
      * @param[in]      m            Array that (as a whole or partly) is assigned to the constructed matrix. No data is copied
      *                              by these constructors. Instead, the header pointing to m data or its sub-array is constructed and
      *                              associated with it. The reference counter, if any, is incremented. So, when you modify the matrix
      *                              formed using such a constructor, you also modify the corresponding elements of m . If you want to
      *                              have an independent copy of the sub-array, use Mat::clone() .
      * @param[in]      roi          Region of interest.
      */
      Mat(const vsdk::Mat& m, const vsdk::Rect& roi);

      /**
      * @overload
      *
      * @param[in]      m            Array that (as a whole or partly) is assigned to the constructed matrix. No data is copied
      *                              by these constructors. Instead, the header pointing to m data or its sub-array is constructed and
      *                              associated with it. The reference counter, if any, is incremented. So, when you modify the matrix
      *                              formed using such a constructor, you also modify the corresponding elements of m . If you want to
      *                              have an independent copy of the sub-array, use Mat::clone() .
      * @param[in]      ranges       Array of selected ranges of m along each dimensionality.
      */
      Mat(const vsdk::Mat& m, const vsdk::Range* ranges);
      
      /**
      * @brief          Destructor - calls release() 
      */
      ~Mat();

      /**
      * @brief          assignment operators
      * 
      * These are available assignment operators. Since they all are very different, make sure to read the
      * operator parameters description.
      * 
      * @param[in]      m            Assigned, right-hand-side matrix. Matrix assignment is an O(1) operation. This means that
      *                              no data is copied but the data is shared and the reference counter, if any, is incremented. Before
      *                              assigning new data, the old data is de-referenced via Mat::release .
      */
      vsdk::Mat& operator = (const vsdk::Mat& m);
      
      #ifdef VSDK_UMAT_USE_OPENCV
        
        /**
        * @overload
        *
        * @brief          OpenCV compatibility overload
        */
        vsdk::Mat& operator = (const cv::Mat& m);
        
        /**
        * @overload
        *
        * @brief          OpenCV compatibility overload
        */
        Mat(const cv::Mat& m);
        
        /**
        * @overload
        *
        * @brief          OpenCV compatibility overload
        */
        operator cv::Mat() const;
      #endif
      
      /**
      * @brief          Creates a matrix header for the specified matrix row.
      * 
      * @param[in]      y            A 0-based row index.
      */
      vsdk::Mat row(int32_t y) const;
      
      /**
      * @brief          Creates a matrix header for the specified matrix column.
      * 
      * @param[in]      x            A 0-based column index.
      */
      vsdk::Mat col(int32_t x) const;
      
      /**
      * @brief          Creates a matrix header for the specified row span.
      * 
      * @param[in]      startrow     An inclusive 0-based start index of the row span.
      * @param[in]      endrow       An exclusive 0-based ending index of the row span.
      */
      vsdk::Mat rowRange(int32_t startrow, int32_t endrow) const;

      /**
      * @overload
      * 
      * @param[in]      r            Range structure containing both the start and the end indices.
      */
      vsdk::Mat rowRange(const vsdk::Range& r) const;

      /**
      * @brief          Creates a matrix header for the specified column span.
      *  
      * @param[in]      startcol     An inclusive 0-based start index of the column span.
      * @param[in]      endcol       An exclusive 0-based ending index of the column span.
      */
      vsdk::Mat colRange(int32_t startcol, int32_t endcol) const;

      /**
      * @overload
      *  
      * @param[in]      r            Range structure containing both the start and the end indices.
      */
      vsdk::Mat colRange(const vsdk::Range& r) const;
      
      /**
      * @brief          Extracts a diagonal from a matrix
      *  
      * @param[in]      d            index of the diagonal, with the following values:
      *                               - `d=0` is the main diagonal.
      *                               - `d>0` is a diagonal from the lower half. For example, d=1 means the diagonal is set
      *                               immediately below the main one.
      *                               - `d<0` is a diagonal from the upper half. For example, d=-1 means the diagonal is set
      *                               immediately above the main one.
      */
      vsdk::Mat diag(int32_t d=0) const;
      
      /**
      * @brief          Allocates new array data if needed.
      *  
      * @param[in]      rows          New number of rows.
      * @param[in]      cols          New number of columns.
      * @param[in]      type          New matrix type.
      */
      void create(int32_t rows, int32_t cols, int32_t type);

      /**
      * @overload
      *  
      * @param[in]      ndims         New array dimensionality.
      * @param[in]      sizes         Array of integers specifying a new array shape.
      * @param[in]      type          New matrix type.
      */
      void create(int32_t ndims, const int32_t* sizes, int32_t type);
      
      /**
      * @brief          Increments the reference counter.
      */
      void addref();

      /**
      * @brief          Decrements the reference counter and deallocates the matrix if needed.
      */
      void release();

      /**
      * @brief          Deallocates the matrix data
      */
      void deallocate();
      
      /**
      * @brief          Internal use function; properly re-allocates _size, _step arrays
      */
      void copySize(const vsdk::Mat& m);
      
      /**
      * @brief          Extracts a rectangular submatrix.
      *  
      * @param[in]      rowRange      Start and end row of the extracted submatrix. The upper boundary is not included. To
      *                               select all the rows, use Range::all().
      * @param[in]      colRange      Start and end column of the extracted submatrix. The upper boundary is not included.
      *                               To select all the columns, use Range::all().
      */
      vsdk::Mat operator()( vsdk::Range rowRange, vsdk::Range colRange ) const;

      /**
      * @overload
      * 
      * @param[in]      roi           Extracted submatrix specified as a rectangle.
      */
      vsdk::Mat operator()( const vsdk::Rect& roi ) const;

      /**
      * @overload
      * 
      * @param[in]      ranges        Array of selected ranges along each array dimension.
      */
      vsdk::Mat operator()( const vsdk::Range* ranges ) const;
      
      /**
      * @brief          Reports whether the matrix is continuous or not.
      */
      int8_t isContinuous() const;

      /**
      * @brief          Returns true if the matrix is a submatrix of another matrix
      */
      int8_t isSubmatrix() const;

      /**
      * @brief          Returns the matrix element size in bytes.
      */
      uint64_t elemSize() const;

      /**
      * @brief          Returns the size of each matrix element channel in bytes.
      */
      uint64_t elemSize1() const;

      /**
      * @brief          Returns the type of a matrix element.
      */
      int32_t type() const;

      /**
      * @brief          Returns the depth of a matrix element.
      */
      int32_t depth() const;

      /**
      * @brief          Returns the number of matrix channels.
      */
      int32_t channels() const;

      /**
      * @brief          Returns a normalized step.
      */
      uint64_t step1(int32_t i=0) const;

      /**
      * @brief          Returns true if the array has no elements.
      */
      int8_t empty() const;

      /**
      * @brief          Returns the total number of array elements.
      */
      uint64_t total() const;

      /**
      * @brief          Returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1); negative number otherwise
      */
      int32_t checkVector(int32_t elemChannels, int32_t depth=-1, int8_t requireContinuous=true) const;
      
      /**
      * @brief          Returns a pointer to the specified matrix row.
      * 
      * @param[in]      i0            A 0-based row index.
      */
      uint8_t* ptr(int32_t i0=0);
      
      /**
      * @overload
      */
      const uint8_t* ptr(int32_t i0=0) const;

      /**
      * @overload
      */
      uint8_t* ptr(int32_t i0, int32_t i1);
      
      /**
      * @overload
      */
      const uint8_t* ptr(int32_t i0, int32_t i1) const;

      /**
      * @overload
      */
      uint8_t* ptr(int32_t i0, int32_t i1, int32_t i2);
      
      /**
      * @overload
      */
      const uint8_t* ptr(int32_t i0, int32_t i1, int32_t i2) const;

      /**
      * @overload
      */
      uint8_t* ptr(const int32_t* idx);
      
      /**
      * @overload
      */
      const uint8_t* ptr(const int32_t* idx) const;
      
      /**
      * @overload
      */
      template<typename _Tp> _Tp* ptr(int32_t i0=0);
      
      /**
      * @overload
      */
      template<typename _Tp> const _Tp* ptr(int32_t i0=0) const;
      
      /**
      * @overload
      */
      template<typename _Tp> _Tp* ptr(int32_t i0, int32_t i1);
      
      /**
      * @overload
      */
      template<typename _Tp> const _Tp* ptr(int32_t i0, int32_t i1) const;
      
      /**
      * @overload
      */
      template<typename _Tp> _Tp* ptr(int32_t i0, int32_t i1, int32_t i2);
      
      /**
      * @overload
      */
      template<typename _Tp> const _Tp* ptr(int32_t i0, int32_t i1, int32_t i2) const;
      
      /**
      * @brief          Returns a reference to the specified array element.
      *  
      * @param[in]      i0            Index along the dimension 0
      */
      template<typename _Tp> _Tp& at(int32_t i0=0);
      
      /**
      * @overload
      *  
      * @param[in]      i0            Index along the dimension 0
      */
      template<typename _Tp> const _Tp& at(int32_t i0=0) const;
      
      /**
      * @overload
      *  
      * @param[in]      i0            Index along the dimension 0
      * @param[in]      i1            Index along the dimension 1
      */
      template<typename _Tp> _Tp& at(int32_t i0, int32_t i1);
      
      /**
      * @overload
      *  
      * @param[in]      i0            Index along the dimension 0
      * @param[in]      i1            Index along the dimension 1
      */
      template<typename _Tp> const _Tp& at(int32_t i0, int32_t i1) const;

      /**
      * @overload
      *  
      * @param[in]      i0            Index along the dimension 0
      * @param[in]      i1            Index along the dimension 1
      * @param[in]      i2            Index along the dimension 2
      */
      template<typename _Tp> _Tp& at(int32_t i0, int32_t i1, int32_t i2);
      
      /**
      * @overload
      *  
      * @param[in]      i0            Index along the dimension 0
      * @param[in]      i1            Index along the dimension 1
      * @param[in]      i2            Index along the dimension 2
      */
      template<typename _Tp> const _Tp& at(int32_t i0, int32_t i1, int32_t i2) const;

      /**
      * @overload
      *  
      * @param[in]      idx            Array of Mat::dims indices.
      */
      template<typename _Tp> _Tp& at(const int32_t* idx);
      
      /**
      * @overload
      *  
      * @param[in]      idx            Array of Mat::dims indices.
      */
      template<typename _Tp> const _Tp& at(const int32_t* idx) const;

      /**
      * @overload
      *  
      * @brief          Special versions for 2D arrays (especially convenient for referencing image pixels)
      * 
      * @param[in]      pt             Element position specified as Point(j,i) .
      */
      template<typename _Tp> _Tp& at(vsdk::Point pt);
      
      /**
      * @overload
      *  
      * @brief          Special versions for 2D arrays (especially convenient for referencing image pixels)
      * 
      * @param[in]      pt             Element position specified as Point(j,i) .
      */
      template<typename _Tp> const _Tp& at(vsdk::Point pt) const;
      
      /**
      * @brief          includes several bit-fields:
      *                  - the magic signature
      *                  - continuity flag
      *                  - depth
      *                  - number of channels
      */
      int32_t flags;
      
      /**
      * @brief          the matrix dimensionality, >= 2
      */
      int32_t dims;
      
      /**
      * @brief          the number of rows and columns or (-1, -1) when the matrix has more than 2 dimensions
      */
      int32_t rows, cols;
      
      /**
      * @brief          pointer to the data
      */
      uint8_t* data;

      /**
      * @brief          helper fields used in locateROI and adjustROI
      */
      const uint8_t* datastart;
      const uint8_t* dataend;
      const uint8_t* datalimit;

      /**
      * @brief          custom allocator
      */
      vsdk::UMatAllocator* allocator;
      
      /**
      * @brief          Interaction with UMat
      */
      vsdk::UMatData* u;

      /**
      * @brief          Sizes along each dimension (in elements)
      */
      vsdk::MatSize size;
      
      /**
      * @brief          Size in bytes to the next element along each dimansion (size in bytes)
      */
      vsdk::MatStep step;
      
      /*==================================================================================================
      *                                OPENCV DIFFERENCES
      ==================================================================================================*/
      /**
      * NOT IMPLEMENTED
      *     void locateROI( Size& wholeSize, Point& ofs ) const;
      *     Mat& adjustROI( int dtop, int dbottom, int dleft, int dright );
      *     Mat(Size size, int type);
      *     Mat(int rows, int cols, int type, const Scalar& s);
      *     Mat(Size size, int type, const Scalar& s);
      *     Mat(int ndims, const int* sizes, int type, const Scalar& s);
      *     Mat(Size size, int type, void* data, size_t step=AUTO_STEP);
      *     template<typename _Tp> explicit Mat(const std::vector<_Tp>& vec, bool copyData=false);
      *     template<typename _Tp, int n> explicit Mat(const Vec<_Tp, n>& vec, bool copyData=true);
      *     template<typename _Tp, int m, int n> explicit Mat(const Matx<_Tp, m, n>& mtx, bool copyData=true);
      *     template<typename _Tp> explicit Mat(const Point_<_Tp>& pt, bool copyData=true);
      *     template<typename _Tp> explicit Mat(const Point3_<_Tp>& pt, bool copyData=true);
      *     template<typename _Tp> explicit Mat(const MatCommaInitializer_<_Tp>& commaInitializer);
      *     explicit Mat(const cuda::GpuMat& m);
      *     Mat& operator = (const MatExpr& expr);
      *     static Mat diag(const Mat& d);
      *     Mat clone() const;
      *     void copyTo( OutputArray m ) const;
      *     void copyTo( OutputArray m, InputArray mask ) const;
      *     void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;
      *     void assignTo( Mat& m, int type=-1 ) const;
      *     Mat& operator = (const Scalar& s);
      *     Mat& setTo(InputArray value, InputArray mask=noArray());
      *     Mat reshape(int cn, int rows=0) const;
      *     Mat reshape(int cn, int newndims, const int* newsz) const;
      *     MatExpr t() const;
      *     MatExpr inv(int method=DECOMP_LU) const;
      *     MatExpr mul(InputArray m, double scale=1) const;
      *     Mat cross(InputArray m) const;
      *     double dot(InputArray m) const;
      *     static MatExpr zeros(int rows, int cols, int type);
      *     static MatExpr zeros(Size size, int type);
      *     static MatExpr zeros(int ndims, const int* sz, int type);
      *     static MatExpr ones(int rows, int cols, int type);
      *     static MatExpr ones(Size size, int type);
      *     static MatExpr ones(int ndims, const int* sz, int type);
      *     static MatExpr eye(int rows, int cols, int type);
      *     static MatExpr eye(Size size, int type);
      *     void reserve(size_t sz);
      *     void resize(size_t sz);
      *     void resize(size_t sz, const Scalar& s);
      *     void push_back_(const void* elem);
      *     template<typename _Tp> void push_back(const _Tp& elem);
      *     template<typename _Tp> void push_back(const Mat_<_Tp>& elem);
      *     void push_back(const Mat& m);
      *     void pop_back(size_t nelems=1);
      *     template<typename _Tp> operator std::vector<_Tp>() const;
      *     template<typename _Tp, int n> operator Vec<_Tp, n>() const;
      *     template<typename _Tp, int m, int n> operator Matx<_Tp, m, n>() const;
      *     template<int n> uchar* ptr(const Vec<int, n>& idx);
      *     template<int n> const uchar* ptr(const Vec<int, n>& idx) const;
      *     template<typename _Tp, int n> _Tp* ptr(const Vec<int, n>& idx);
      *     template<typename _Tp, int n> const _Tp* ptr(const Vec<int, n>& idx) const;
      *     template<typename _Tp, int n> _Tp& at(const Vec<int, n>& idx);
      *     template<typename _Tp, int n> const _Tp& at(const Vec<int, n>& idx) const;
      *     template<typename _Tp> MatIterator_<_Tp> begin();
      *     template<typename _Tp> MatConstIterator_<_Tp> begin() const;
      *     template<typename _Tp> MatIterator_<_Tp> end();
      *     template<typename _Tp> MatConstIterator_<_Tp> end() const;
      *     template<typename _Tp, typename Functor> void forEach(const Functor& operation);
      *     template<typename _Tp, typename Functor> void forEach(const Functor& operation) const;
      *     template<typename _Tp, typename Functor> void forEach_impl(const Functor& operation);
      *     template<typename _Tp> _Tp* ptr(const int32_t* idx);
      *     template<typename _Tp> const _Tp* ptr(const int32_t* idx) const;
      *     void create(Size size, int32_t type);
      */
  }; /* class Mat */
  
  /*================================================================================================*/
  /**
  * @brief          n-ary multi-dimensional array iterator
  */
  class NAryMatIterator
  {
  public:
      /**
      * @brief          The default constructor
      */
      NAryMatIterator();
      
      /**
      * @brief          The full constructor taking arbitrary number of n-dim matrices
      * 
      * @param[in]      arrays
      * @param[in]      ptrs  
      * @param[in]      narrays
      */
      NAryMatIterator(const vsdk::Mat** arrays, uint8_t** ptrs, int32_t narrays=-1);
      
      /**
      * @brief          The full constructor taking arbitrary number of n-dim matrices
      * 
      * @param[in]      arrays
      * @param[in]      planes  
      * @param[in]      narrays
      */
      NAryMatIterator(const vsdk::Mat** arrays, vsdk::Mat* planes, int32_t narrays=-1);
      
      /**
      * @brief          The separate iterator initialization method
      * 
      * @param[in]      arrays
      * @param[in]      planes  
      * @param[in]      ptrs
      * @param[in]      narrays
      */
      void init(const vsdk::Mat** arrays, vsdk::Mat* planes, uint8_t** ptrs, int32_t narrays=-1);

      /**
      * @brief          Proceeds to the next plane of every iterated matrix
      */
      NAryMatIterator& operator ++();
      
      /**
      * @brief          Proceeds to the next plane of every iterated matrix (postfix increment operator)
      */
      NAryMatIterator operator ++(int32_t);

      /**
      * @brief          The iterated arrays
      */
      const vsdk::Mat** arrays;
      
      /**
      * @brief          The current planes
      */
      vsdk::Mat* planes;
      
      /**
      * @brief          Data pointers
      */
      uint8_t** ptrs;
      
      /**
      * @brief          The number of arrays
      */
      int32_t narrays;
      
      /**
      * @brief          The number of hyper-planes that the iterator steps through
      */
      size_t nplanes;
      
      /**
      * @brief          The size of each segment (in elements)
      */
      size_t size;
      
  protected:
    
      /**
      * @brief          
      */
      int32_t iterdepth;
      
      /**
      * @brief          
      */
      size_t idx;
  }; /* class NAryMatIterator */
  
  #include "umat_mat_inline.hpp"
  
} /* namespace vsdk */
#endif /* UMATMAT_HPP */
