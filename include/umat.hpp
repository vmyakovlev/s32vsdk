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

#ifndef UMAT_HPP
#define UMAT_HPP

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "../libs/utils/umat/include/umat_defines.hpp"
#include "../libs/utils/umat/include/umat_types.hpp"
#include "../libs/utils/umat/include/umat_data.hpp"
#include "../libs/utils/umat/include/umat_debug_out.hpp"
#include "../libs/utils/umat/include/umat_mat.hpp"

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
namespace vsdk
{
  /*================================================================================================*/
  /**
  * @brief          A UMat class which is derived from cv::UMat and is fully compatible with it.
  * @details        A custom implementation of the UMat keeps compatibility with cv::UMat and is able
  *                 to be retyped back and forth in order to be used in OpenCV functions as well as in
  *                 vsdk ApexCV and others.
  *
  */
  class UMat 
  {
    public:
      enum { MAGIC_VAL  = 0x42FF0000, AUTO_STEP = 0, CONTINUOUS_FLAG = VSDK_CV_MAT_CONT_FLAG, SUBMATRIX_FLAG = VSDK_CV_SUBMAT_FLAG };
      enum { MAGIC_MASK = 0xFFFF0000, TYPE_MASK = 0x00000FFF, DEPTH_MASK = 7 };
      
      /*================================================================================================*/
      /**
      * @brief          Default constructor
      * @details        
      *
      * @param[in]      usageFlags  Usage flags, if need to be specified
      */
      UMat(vsdk::UMatUsageFlags usageFlags = USAGE_DEFAULT);
    
      /**
      * @brief          Constructs 2D matrix of the specified size and type
      *
      * @param[in]      rows        Number of rows (height)
      * @param[in]      cols        Number of cols (width)
      * @param[in]      type        Type of element: CV_8UC1, CV_64FC3, CV_32SC(12) etc.
      * @param[in]      usageFlags  Usage flags, if need to be specified
      */
      UMat(int32_t rows, int32_t cols, int32_t type, vsdk::UMatUsageFlags usageFlags = USAGE_DEFAULT);
      
      /**
      * @brief          Constructs n-dimensional matrix
      *
      * @param[in]      ndims       Number of dimensions
      * @param[in]      sizes       Number of bytes in each dimension
      * @param[in]      type        Type of element: CV_8UC1, CV_64FC3, CV_32SC(12) etc.
      * @param[in]      usageFlags  Usage flags, if need to be specified
      */
      UMat(int32_t ndims, const int32_t* sizes, int32_t type, vsdk::UMatUsageFlags usageFlags = USAGE_DEFAULT);

      /**
      * @brief          Copy constructor
      *
      * @param[in]      m           Original matrix
      */
      UMat(const vsdk::UMat& m);

      /**
      * @brief          Creates a matrix header for a part of the bigger matrix
      *
      * @param[in]      m           Original matrix
      * @param[in]      rowRange    Row range
      * @param[in]      colRange    Column range
      */
      UMat(const vsdk::UMat& m, const vsdk::Range& rowRange, const vsdk::Range& colRange=Range::all());
      
      /**
      * @brief          Creates a matrix header for a part of the bigger matrix
      *
      * @param[in]      m           Original matrix
      * @param[in]      roi         ROI specified by a Rectangle class
      */
      UMat(const vsdk::UMat& m, const vsdk::Rect& roi);
      
      /**
      * @brief          Creates a matrix header for a part of the bigger matrix
      * 
      * @param[in]      m           Original matrix
      * @param[in]      ranges      Ranges specified by a list
      */
      UMat(const vsdk::UMat& m, const vsdk::Range* ranges);
      
      /**
      * @brief          Returns a UMat from specified ROI
      *
      * @param[in]      ranges      Ranges specified by a list
      *
      * @return         ROIed UMat
      */
      vsdk::UMat operator()( const vsdk::Range* ranges ) const;
      
      /**
      * @brief          Assigned operator
      *
      * @param[in]      m           Original matrix
      *
      * \return 
      *    - Assigned matrix
      */
      vsdk::UMat& operator = (const UMat& m);
      
      /**
      * @brief          Assign ROI operator
      *
      * @param[in]      rowRange    Original matrix row range
      * @param[in]      colRange    Original matrix column range
      *
      * @return         Assigned submatrix
      */
      vsdk::UMat operator()( vsdk::Range rowRange, vsdk::Range colRange ) const;
      
      /**
      * @brief          Assign ROI operator
      *
      * @param[in]      roi         ROI rectangle
      *
      * @return         Assigned submatrix
      */
      vsdk::UMat operator()( const vsdk::Rect& roi ) const;
      
      /**
      * @brief          Destructor - calls release()
      * 
      * @param[in]      m           Original matrix
      */
      ~UMat();
      
      /**
      * @brief          Allocates new matrix data unless the matrix already has specified size and type.
      *                 previous data is unreferenced if needed.
      *
      * @param[in]      rows        Number of rows (height)
      * @param[in]      cols        Number of cols (width)
      * @param[in]      type        Type of element: CV_8UC1, CV_64FC3, CV_32SC(12) etc.
      * @param[in]      usageFlags  Usage flags, if need to be specified
      */
      void create(int32_t rows, int32_t cols, int32_t type, vsdk::UMatUsageFlags usageFlags = USAGE_DEFAULT);
      
      /**
      * @brief          Allocates new matrix data unless the matrix already has specified size and type.
      *                 previous data is unreferenced if needed.
      *
      * @param[in]      ndims       Number of dimensions
      * @param[in]      sizes       Number of bytes in each dimension
      * @param[in]      type        Type of element: CV_8UC1, CV_64FC3, CV_32SC(12) etc.
      * @param[in]      usageFlags  Usage flags, if need to be specified
      */
      void create(int32_t ndims, const int32_t* sizes, int32_t type, vsdk::UMatUsageFlags usageFlags = USAGE_DEFAULT);
      
      #ifdef VSDK_UMAT_USE_OPENCV
        /**
        * @brief          Retype operator for cv::UMat to ensure compatibility with OpenCV
        */
        operator cv::UMat() const;
        
        /**
        * @brief          Retype assignment operator for cv::UMat to ensure compatibility with OpenCV
        * 
        * @param[in]      m           OpenCV matrix to be copied from
        */
        vsdk::UMat& operator = (const cv::UMat& m);
        
        /**
        * @brief          Constructor for cv::UMat to ensure compatibility with OpenCV
        * 
        * @param[in]      m           OpenCV matrix to be copied from
        */
        UMat(const cv::UMat& m);
        
      #endif /* VSDK_UMAT_USE_OPENCV */
      
      /**
      * @brief          Increases the reference counter; use with care to avoid memleaks
      */
      void addref();
      
      /**
      * @brief          Returns element type, similar to CV_MAT_TYPE(cvmat->type)
      *
      * @return         Element type (similar to CV_MAT_TYPE)
      */
      int32_t type() const;
      
      /**
      * @brief          Returns the total number of matrix elements
      *
      * @return         Total number of matrix elements
      */
      uint64_t total() const;
      
      /**
      * @brief          Returns element size in bytes
      *
      * @return         Total element size in bytes
      */
      uint64_t elemSize() const;
      
      /**
      * @brief          Returns true iff the matrix data is continuous
      *                 (i.e. when there are no gaps between successive rows).
      *                 similar to CV_IS_MAT_CONT(cvmat->type)
      *
      * @return         Returns true iff the matrix data is continuous
      */
      int8_t isContinuous() const;
      
      /**
      * @brief          Returns true if the matrix is a submatrix of another matrix
      *
      * @return         Returns true if the matrix is a submatrix of another matrix
      */
      int8_t isSubmatrix() const;
      
      /**
      * @brief          Returns the size of element channel in bytes.
      *
      * @return         Returns the size of element channel in bytes.
      */
      uint64_t elemSize1() const;
      
      /**
      * @brief          Returns element type, similar to CV_MAT_DEPTH(cvmat->type)
      *
      * @return         Returns element type, similar to CV_MAT_DEPTH(cvmat->type)
      */
      int32_t depth() const;
      
      /**
      * @brief          Returns element type, similar to CV_MAT_CN(cvmat->type)
      *
      * @return         Returns element type, similar to CV_MAT_CN(cvmat->type)
      */
      int32_t channels() const;
      
      /**
      * @brief          Returns step/elemSize1()
      *
      * @return         Returns step/elemSize1()
      */
      uint64_t step1(int32_t i=0) const;
      
      /**
      * @brief          Returns true if matrix data is NULL
      *
      * @return         Returns true if matrix data is NULL
      */
      int8_t empty() const;
      
      /**
      * @brief          Internal use function; properly re-allocates _size, _step arrays
      */
      void copySize(const vsdk::UMat& m);
      
      /**
      * @brief          Returns a new matrix header for the specified row
      *
      * @param[in]      y           Index of a row
      *
      * @return         New row matrix structure
      */
      vsdk::UMat row(int32_t y) const;
      
      /**
      * @brief          Returns a new matrix header for the specified column
      *
      * @param[in]      x           Index of a column
      *
      * @return         New column matrix structure
      */
      vsdk::UMat col(int32_t x) const;
      
      /**
      * @brief          Returns a new matrix header for the specified row span
      *
      * @param[in]      startrow    Starting row
      * @param[in]      endrow      End row
      *
      * @return         New matrix structure
      */
      vsdk::UMat rowRange(int32_t startrow, int32_t endrow) const;
      
      /**
      * @brief          Returns a new matrix header for the specified row span
      *
      * @param[in]      r           Row range
      *
      * @return         New matrix structure
      */
      vsdk::UMat rowRange(const vsdk::Range& r) const;
      
      /**
      * @brief          Returns a new matrix header for the specified column span
      *
      * @param[in]      startcol    Starting column
      * @param[in]      endcol      End column
      *
      * @return         New matrix structure
      */
      vsdk::UMat colRange(int32_t startcol, int32_t endcol) const;
      
      /**
      * @brief          Returns a new matrix header for the specified column span
      *
      * @param[in]      r           Column range
      *
      * @return         New matrix structure
      */
      vsdk::UMat colRange(const vsdk::Range& r) const;
      
      /**
      * @brief          Returns a new matrix header for the specified diagonal
      *                 (d=0 - the main diagonal,
      *                   >0 - a diagonal from the lower half,
      *                   <0 - a diagonal from the upper half)
      *
      * @param[in]      d           Diagonal specification
      *
      * @return         New matrix structure
      */
      vsdk::UMat diag(int32_t d=0) const;
      
      /**
      * @brief          Returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1); negative number otherwise
      *
      * @param[in]      elemChannels      Query number of channels
      * @param[in]      depth             Query depth
      * @param[in]      requireContinuous Query is continuous?
      *
      * @return         Returns N if the matrix is 1-channel (N x ptdim) or ptdim-channel (1 x N) or (N x 1); negative number otherwise
      */
      int32_t checkVector(int32_t elemChannels, int32_t depth=-1, int8_t requireContinuous=true) const;
      
      /**
      * @brief          Returns UMat handle for allocated structure
      *
      * @param[in]      accessFlags Specified access flags
      *
      * @return         UMat Handle
      */
      void* handle(int32_t accessFlags) const;
      
      /**
      * @brief          Fills ofs parameter with offset for each dimension
      *
      * @param[out]     ofs        Returned dimension offsets
      */
      void  ndoffset(int64_t* ofs) const;
      
      /**
      * @brief          Decreases reference counter;
      *                 Deallocates the data when reference counter reaches 0.
      */
      void release();
      
      /**
      * @brief          Deallocates the matrix data
      */
      void deallocate();

      /**
      * @brief          Returns a Mat class with a concrete buffer mapping
      * 
      * @param          flags
      */
      vsdk::Mat getMat(int32_t flags) const;
      
      /**
      * @brief          includes several bit-fields:
      *                    - the magic signature
      *                    - continuity flag
      *                    - depth
      *                    - number of channels
      */
      int32_t flags;
      
      /**
      * @brief          The matrix dimensionality, >= 2
      */
      int32_t dims;
      
      /**
      * @brief          The number of rows and columns or (-1, -1) when the matrix has more than 2 dimensions
      */
      int32_t rows, cols;

      /**
      * @brief          Custom allocator
      */
      vsdk::UMatAllocator* allocator;
      
      /**
      * @brief          Usage flags for allocator
      */
      vsdk::UMatUsageFlags usageFlags; 

      /**
      * @brief          Black-box container of UMat data
      */
      vsdk::UMatData* u;

      /**
      * @brief          Offset of the submatrix (or 0)
      */
      uint64_t offset;
  
      /**
      * @brief          Size of the matrix
      */      
      vsdk::MatSize size;
      
      /**
      * @brief          Step in different dimensions: [0] n. of bytes between pixels, [1] n. of bytes between rows, [2] n. of rows between image row in case of y-dimensional pixel
      */      
      vsdk::MatStep step;  
      
      /*==================================================================================================
      *                                OPENCV DIFFERENCES 
      ==================================================================================================*/
      /**
      * NOT IMPLEMENTED
      *     static UMat zeros(int rows, int cols, int type);
      *     static UMat zeros(Size size, int type);
      *     static UMat zeros(int ndims, const int* sz, int type);
      *     static UMat ones(int rows, int cols, int type);
      *     static UMat ones(Size size, int type);
      *     static UMat ones(int ndims, const int* sz, int type);
      *     static UMat eye(int rows, int cols, int type);
      *     static UMat eye(Size size, int type);
      *     void locateROI( Size& wholeSize, Point& ofs ) const;
      *     UMat& adjustROI( int dtop, int dbottom, int dleft, int dright );
      *     UMat(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
      *     UMat(int rows, int cols, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);
      *     UMat(Size size, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);
      *     UMat(int ndims, const int* sizes, int type, const Scalar& s, UMatUsageFlags usageFlags = USAGE_DEFAULT);
      *     template<typename _Tp> explicit UMat(const std::vector<_Tp>& vec, bool copyData=false);
      *     template<typename _Tp, int n> explicit UMat(const Vec<_Tp, n>& vec, bool copyData=true);
      *     template<typename _Tp, int m, int n> explicit UMat(const Matx<_Tp, m, n>& mtx, bool copyData=true);
      *     template<typename _Tp> explicit UMat(const Point_<_Tp>& pt, bool copyData=true);
      *     template<typename _Tp> explicit UMat(const Point3_<_Tp>& pt, bool copyData=true);
      *     template<typename _Tp> explicit UMat(const MatCommaInitializer_<_Tp>& commaInitializer);
      *     void convertTo( OutputArray m, int rtype, double alpha=1, double beta=0 ) const;
      *     UMat clone() const;
      *     void copyTo( OutputArray m ) const;
      *     void copyTo( OutputArray m, InputArray mask ) const;
      *     void assignTo( UMat& m, int type=-1 ) const;
      *     UMat& operator = (const Scalar& s);
      *     UMat& setTo(InputArray value, InputArray mask=noArray());
      *     UMat reshape(int cn, int rows=0) const;
      *     UMat reshape(int cn, int newndims, const int* newsz) const;
      *     UMat t() const;
      *     UMat inv(int method=DECOMP_LU) const;
      *     UMat mul(InputArray m, double scale=1) const;
      *     static UMat diag(const UMat& d);
      *     double dot(InputArray m) const;
      *     void create(Size size, int type, UMatUsageFlags usageFlags = USAGE_DEFAULT);
      */
  }; /* class UMat */
  
  #include "../libs/utils/umat/include/umat_inline.hpp"

  /*================================================================================================*/
  /**
  * @brief          A UMat derivation for internal purposes. Underlying pointers can be specified.
  * @details        A custom implementation of the UMat which allows to specify the hw and virtual pointer.
  *                 This implementation does not manage cache nor memory mapping, it's just a blind wrapper
  *                 for specified buffer/mapping.
  *
  */
  class CustomPointerUMat : public vsdk::UMat
  {
    public:
      /**
      * @brief          Constructs 2D matrix of the specified size and type
      *
      * @param[in]      rows        Number of rows (height)
      * @param[in]      cols        Number of cols (width)
      * @param[in]      type        Type of element: CV_8UC1, CV_64FC3, CV_32SC(12) etc.
      * @param[in]      usageFlags  Usage flags, if need to be specified
      * @param[in]      hwptr       HW physical address of the buffer
      * @param[in]      virtualptr  virtual mapping of the buffer (can be NULL)
      */
      CustomPointerUMat(int32_t rows, int32_t cols, int32_t type, void *hwptr, void *virtualptr);
      
      /**
      * @brief          Default constructor
      * @details        
      *
      * @param[in]      usageFlags  Usage flags, if need to be specified
      */
      CustomPointerUMat(vsdk::UMatUsageFlags usageFlags = USAGE_DEFAULT);
      
      /**
      * @brief          Allocates new matrix data unless the matrix already has specified size and type.
      *                 previous data is unreferenced if needed.
      *
      * @param[in]      rows        Number of rows (height)
      * @param[in]      cols        Number of cols (width)
      * @param[in]      type        Type of element: CV_8UC1, CV_64FC3, CV_32SC(12) etc.
      * @param[in]      hwptr       Physical pointer
      * @param[in]      usageFlags  Usage flags, if need to be specified
      */
      void createCustom(int32_t ndims, const int32_t* sizes, int32_t type, void *hwptr, vsdk::UMatUsageFlags usageFlags = USAGE_DONT_ALLOCATE);
      
      /**
      * @brief          Returns a Mat class with a concrete buffer mapping
      * 
      * @param          flags
      */
      vsdk::Mat getMat(int32_t flags) const;
  }; /* class CustomPointerUMat */
  
} /* namespace vsdk */
#endif /* UMAT_HPP */

