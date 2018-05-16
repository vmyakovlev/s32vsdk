/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* (C) Copyright CogniVue Corporation. 2011-2015 All right reserved.
* Copyright (c) 2015-2016 Freescale Semiconductor
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

#ifndef ICPTYPES_H
#define ICPTYPES_H

#include "global_errors.h"
#include <stdint.h>
#include <math.h> // for fabs() 
#include <cstring>     
#include <cstdlib>
#include <ostream>
#include <istream>

//#include "Oal_Exception.h"

#include <assert.h>
//#include "debug.h"

#ifdef _WIN32
   /* OpenCV conversion support is ONLY available
      on Windows environment */
//   #define SUPPORT_OPENCV_CONVERSIONS

   #ifdef SUPPORT_OPENCV_CONVERSIONS
   #include "core/core.hpp"
   #endif
#endif

/**
 *  \file ICP_Types.hpp
 *  \brief ICP type definitions
 */

/// \addtogroup icp_types Auxiliary Types for ICP
/// @{

/// A basic structure to hold width and height on a rectangle or an image.
class ICP_Size
{
public:
   int width;   ///< width of an image/rectangle
   int height;  ///< height of an image/rectangle

   /**
    * Construct a default ICP_Size object with zero width/height
    */
   ICP_Size() : width(0), height(0) {}
   /**
    * Construct an ICP_Size object with width, height information
    *
    * @param w width value
    * @param h height value
    */
   ICP_Size(int w, int h) : width(w), height(h) {}

   /**
    * Copy the content of one ICP_Size to another.
    *
    * @param size input structure
    * @return
    */
   ICP_Size& operator=(const ICP_Size& size)
   {
      width  = size.width;
      height = size.height;
      return *this;
   }

   bool operator==(ICP_Size& s) const
   {
      return ((width==s.width) && (height==s.height)) ? true : false;
   }

#ifdef SUPPORT_OPENCV_CONVERSIONS
   operator cv::Size2i()
   {
      cv::Size2i s(width, height);
      return s;
   }
#endif
};

/// A basic structure that describes an area on the cartesian coordinate system
/// with size and offset from the center.
template <class TYPE>
class ICP_Rectangle_
{
public:
   /**
    * Construct a default ICP_Rectangle with zero values.
    */
   ICP_Rectangle_() :
      x(0), y(0), width(0), height(0) {}
   /**
    * Construct a rectangle with coordinate and size information.
    *
    * @param initX top-left corner
    * @param initY top-right corner
    * @param initW width
    * @param initH height
    */
   ICP_Rectangle_(TYPE initX, TYPE initY, TYPE initW, TYPE initH) :
      x(initX), y(initY), width(initW), height(initH) {}

   /**
    * Copy the content of one rectangle to another.
    *
    * @param rect input structure
    * @return
    */
   ICP_Rectangle_& operator=(const ICP_Rectangle_& rect)
   {
      x = rect.x;
      y = rect.y;
      width  = rect.width;
      height = rect.height;
      return *this;
   }

#ifdef SUPPORT_OPENCV_CONVERSIONS
   /** 
    * Conversion Functions
    */
   operator cv::Rect()
   {
      Rect p(x, y, width, height);
      return p;
   }
#endif

   TYPE x;         ///< top-left corner of the rectangle (x-coordinate)
   TYPE y;         ///< top-left corner of the rectangle (y-coordinate)
   TYPE width;     ///< width of the rectangle
   TYPE height;    ///< height of the rectangle
};

typedef ICP_Rectangle_<int32_t>  ICP_Rectangle;
typedef ICP_Rectangle_<double>   ICP_Rectangle_d;
/**
 * A basic structure that describes the location of a point on the cartesian coordinate system.
 * Size of the x, y offsets are 32-bits.
 */


template <class TYPE>
class ICP_Point2
{
public:
   TYPE x; ///< horizontal offset from the center
   TYPE y; ///< vertical offset from the center

   /**
    * Construct a default point located at center (0, 0)
    */
   ICP_Point2()
      : x(0),
        y(0)
   {}

   /**
    * Construct a default point from another point
    * \param p source point
    */
   ICP_Point2(const ICP_Point2& p) : x(p.x), y(p.y)
   {}

   /**
    * Construct a default point from another point
    * \param p source point
    */
   template <class TYPE2>
   ICP_Point2(const ICP_Point2<TYPE2>& p) : x(p.x), y(p.y)
   {}

   #ifdef SUPPORT_OPENCV_CONVERSIONS
   /**
    * Construct a default point from another point
    */
   ICP_Point2(const cv::Point2d& p) : x(p.x), y(p.y)
   {}
#endif
   /**
    * Construct a point located at (initX, initY)
    * @param initX value of x offset
    * @param initY value of y offset
    */
   ICP_Point2(TYPE initX, TYPE initY ) : x(initX), y(initY)
   {}

   /**
    * Copy the content of one point to another.
    *
    * @param point source point
    * @return
    */
   ICP_Point2& operator=(const ICP_Point2& point)
   {
      x = point.x;
      y = point.y;
      return *this;
   }

   /**
    * Check the equality of two points.
    *
    * @param point input value to compare against.
    * @return true when they are equal
    */
   bool operator== (const ICP_Point2& point) const
   {
      if ( (point.x == x) &&
           (point.y == y) )
         return true;
      else
         return false;
   }

   /**
    * Check the inequality of two points.
    *
    * @param point input value to compare against.
    * @return true when they are NOT equal
    */
   bool operator!= (const ICP_Point2& point) const
   {
      return !(*this == point);
   }

   /**
    * Subtract the content of one point from another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point2& operator-=(const ICP_Point2& point)
   {
      x -= point.x;
      y -= point.y;
      return *this;
   }

   /**
    * Add the content of one point to another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point2& operator+=(const ICP_Point2& point)
   {
      x += point.x;
      y += point.y;
      return *this;
   }

   /**
    * Subtract the content of one point from another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point2 operator-(const ICP_Point2& point) const 
   {
      ICP_Point2 res = *this;
      res -= point;
      return res;
   }

   /**
    * Add the content of one point to another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point2 operator+(const ICP_Point2& point) const
   {
      ICP_Point2 res = *this;
      res += point;
      return res;
   }

#ifdef SUPPORT_OPENCV_CONVERSIONS
   /** 
    * Conversion Functions
    */
   operator cv::Point2d()
   {
      Point2d p(x, y);
      return p;
   }

   operator cv::Point2f()
   {
      Point2f p(x, y);
      return p;
   }
   
   operator cv::Point2i()
   {
      Point2i p(x, y);
      return p;
   }

   ICP_Point2& operator=(const cv::Point2i& point)
   {
      x = point.x;
      y = point.y;
      return *this;
   }
#endif

   /**
    * dot product
    * \param p2 source point
    * \return result of operation
    */
   TYPE dot(ICP_Point2<TYPE> p2)
   {
      TYPE dotValue = x*p2.x + y*p2.y;
      return dotValue;
   }
};

typedef ICP_Point2<double>    ICP_Point2d;
typedef ICP_Point2<float>     ICP_Point2f;
typedef ICP_Point2<int32_t>   ICP_Point;
typedef ICP_Point2<int16_t>   ICP_Point_16S;

template <class TYPE>
class ICP_Point3
{
public:
   TYPE x; ///< horizontal offset from the center
   TYPE y; ///< vertical offset from the center
   TYPE z; ///< vertical offset from the center

   /**
    * Construct a default point located at center (0, 0)
    */
   ICP_Point3()
      : x(0),
        y(0),
        z(0)
   {}

   /**
    * Construct a default point from another point
    */
   ICP_Point3(const ICP_Point3& p) : x(p.x), y(p.y), z(p.z)
   {}

#ifdef SUPPORT_OPENCV_CONVERSIONS
   /**
    * Construct a default point from another point
    */
   ICP_Point3(const cv::Point3d& p) : x(p.x), y(p.y), z(p.z)
   {}
#endif
   /**
    * Construct a point located at (initX, initY)
    * @param initX value of x offset
    * @param initY value of y offset
    */
   ICP_Point3(TYPE initX, TYPE initY, TYPE initZ ) : x(initX), y(initY), z(initZ)
   {}

   /**
    * Copy the content of one point to another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point3& operator=(const ICP_Point3& point)
   {
      x = point.x;
      y = point.y;
      z = point.z;
      return *this;
   }

   /**
    * Subtract the content of one point from another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point3& operator-=(const ICP_Point3& point)
   {
      x -= point.x;
      y -= point.y;
      z -= point.z;
      return *this;
   }

   /**
    * Add the content of one point to another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point3& operator+=(const ICP_Point3& point)
   {
      x += point.x;
      y += point.y;
      z += point.z;
      return *this;
   }

   /**
    * Subtract the content of one point from another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point3 operator-(const ICP_Point3& point) const 
   {
      ICP_Point3 res = *this;
      res -= point;
      return res;
   }

   /**
    * Add the content of one point to another.
    *
    * @param point input structure
    * @return
    */
   ICP_Point3 operator+(const ICP_Point3& point) const
   {
      ICP_Point3 res = *this;
      res += point;
      return res;
   }

#ifdef SUPPORT_OPENCV_CONVERSIONS
   /** 
    * Conversion Functions
    */
#endif

   /**
    *  dot product
    */
   TYPE dot(ICP_Point3<TYPE> p3)
   {
      TYPE dotValue = x*p3.x + y*p3.y + z*p3.z;
      return dotValue;
   }
};

/**
 * A basic structure that describes a line with its start and end points.
 */
class ICP_Line
{
public:
   ICP_Point point1; ///< one end of the line
   ICP_Point point2; ///< other end of the line

   /**
    * Construct a default ICP_Line with zero values
    */
   ICP_Line() : point1(), point2() {}

   /**
    * Construct a line using two points.
    * @param p1 one end of the line
    * @param p2 other end of the line
    */
   ICP_Line(const ICP_Point& p1, const ICP_Point& p2)
      : point1(p1), point2(p2) {}

   /**
    * Copy the content of one line to another.
    *
    * @param line input structure
    * @return
    */
   ICP_Line& operator=(const ICP_Line& line)
   {
      point1 = line.point1;
      point2 = line.point2;
      return *this;
   }
};

//Forward Declarations
template <class ICP_MAT_TYPE> class ICP_Mat;
template <class ICP_MAT_TYPE> void          CopyRow   (      ICP_Mat<ICP_MAT_TYPE>& matDst, unsigned int rowDst,
                                                       const ICP_Mat<ICP_MAT_TYPE>& matSrc, unsigned int rowSrc);

#ifdef _WIN32
#include <iostream>
#include <iomanip>
template <class ICP_MAT_TYPE> std::ostream& operator<<(std::ostream& stream, ICP_Mat<ICP_MAT_TYPE>& ob);
template <class ICP_MAT_TYPE> std::istream& operator>>(std::istream& stream, ICP_Mat<ICP_MAT_TYPE>& ob);
#else
#include <stdio.h>
#endif

/// ICP_MAT describes a general ICP based matrix structure.
/// It supports from 1 to 3 dimensions.
template <class ICP_MAT_TYPE>
class ICP_Mat
{
   public:

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Constructor for when the dimension is not known upon creation
      ///
      /////////////////////////////////////////////////////////////////////////
      ICP_Mat();

      // Copy constructor
      ICP_Mat(const ICP_Mat &mat);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Constructor for a 3D matrix within the ICP_Mat class
      ///
      /// \param cNumberOfRows
      /// The number of rows (unsigned integer)
      /// \param cNumberOfCols
      /// The number of columns (unsigned integer)
      /// \param cDepth
      /// The depth of matrix (unsigned integer)
      ///
      /////////////////////////////////////////////////////////////////////////
      ICP_Mat(unsigned int const cNumberOfRows,
              unsigned int const cNumberOfCols,
              unsigned int const cDepth = 1);


      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Destructor for ICP_Mat class.  Note that this method will call
      /// the 'Destroy' method if memory is allocated to a matrix
      ///
      /// \return
      /// void
      /////////////////////////////////////////////////////////////////////////
      ~ICP_Mat();


      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Configure a matrix when it was created with a constructor without parameters
      /// If the matrix was already configured by this function or by a constructor,
      /// this method does nothing.
      ///
      /// \param cNumberOfRows
      /// The number of rows (unsigned integer)
      /// \param cNumberOfCols
      /// The number of columns (unsigned integer)
      /// \param cDepth
      /// The depth of matrix if three dimensional (unsigned integer)
      /// \return LIB_RESULT_OK success LIB_RESULT_ERR failure
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT Create(unsigned int const cNumberOfRows,
                        unsigned int const cNumberOfCols,
                        unsigned int const cDepth=1);

      LIB_RESULT Create(ICP_Size _size)
      {
          return Create(_size.height, _size.width, 1);
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      /// It returns a sub-matrix starting from cStartCol and ending before cEndCol
      /// It works only on 2D matrices
      /// \param cStartCol
      ///
      /// \param cEndCol
      ///
      /// \return
      /// Null if error occurs
      ICP_Mat* GetCols(unsigned int const cStartCol,
                       unsigned int const cEndCol);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the value stored at cColIndex, cRowIndex, cDepthIndex.
      ///
      /// \param rReturnedValue
      /// value contained at the specified coordinates.
      ///
      /// \param cColIndex
      /// Column index.  Indexing starts at zero.
      ///
      /// \param cRowIndex
      /// Row index.  Indexing starts at zero.
      ///
      /// \param cDepthIndex
      /// Depth index.  Indexing starts at zero. This parameter is not necessary for 2D matrices.
      ///
      /// \return
      /// LIB_RESULT_OK when the value was found. LIB_RESULT_ERR when a coordinate is out of range
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT GetValue(ICP_MAT_TYPE& rReturnedValue,
                          unsigned int const cColIndex,
                          unsigned int const cRowIndex,
                          unsigned int const cDepthIndex=0) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that finds the minimum and maximum value in a two dimensional array,
      /// as well as their locations.
      ///
      /// \param minVal
      /// Minimum value found.
      ///
      /// \param maxVal
      /// Maximum value found.
      ///
      /// \param minLoc
      /// Location of the minimum value
      ///
      /// \param maxLoc
      /// Location of the maximum value
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT MinMaxLoc(ICP_MAT_TYPE* minVal,
                     ICP_MAT_TYPE* maxVal,
                     ICP_Point* minLoc,
                     ICP_Point* maxLoc);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Assignment operator.  All members of '*this' will be set to the values
      /// of 'lMat'.
      ///
      /// \param lMat
      /// Constant reference to a ICP_Mat object.
      ///
      /// \return
      /// Reference to an ICP_Mat object.
      /////////////////////////////////////////////////////////////////////////
      ICP_Mat& operator=(const ICP_Mat& lMat);
#ifdef SUPPORT_OPENCV_CONVERSIONS
      ICP_Mat& operator=(const cv::Mat_<ICP_MAT_TYPE> & lMat);
#endif
      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Multiply operator. rDestination = (*this)*(rcSource2);
      ///
      /// \param rcSource2
      /// Constant reference to a ICP_Mat object.
      ///
      /// \return
      /// Reference to an ICP_Mat object.
      /////////////////////////////////////////////////////////////////////////
      ICP_Mat operator*(const ICP_Mat& rcSource2) const 
      {
         ICP_Mat rDestination;
         Multiply(rcSource2, rDestination);
         return rDestination;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// This returns the value located at row, col or zero
      /// when an error occurs.
      ///
      /// \param cRowIndex
      /// Row index.  Indexing starts at zero.
      ///
      /// \param cColIndex
      /// Column index.  Indexing starts at zero.
      ///
      /// \param cDepthIndex
      /// Depth index.  Indexing starts at zero. This parameter is not necessary for 2D matrices.
      ///
      /// \return
      /// Reference to an ICP_Mat object.
      /////////////////////////////////////////////////////////////////////////
      ICP_MAT_TYPE operator()(unsigned int const cRowIndex,
                              unsigned int const cColIndex,
                              unsigned int const cDepthIndex = 0) const;

      ICP_MAT_TYPE& operator()(unsigned int const cRowIndex,
                               unsigned int const cColIndex,
                               unsigned int const cDepthIndex = 0);


      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that stores a value at colindex,rowindex, depthindex.
      ///
      /// \param cValue
      /// value to be stored in the matrix.
      ///
      /// \param cRowIndex
      /// Row index.  Indexing starts at zero.
      ///
      /// \param cColIndex
      /// Column index.  Indexing starts at zero.
      ///
      /// \param cDepthIndex
      /// Depth index.  Indexing starts at zero.  This parameter is not necessary for 2D
      /// matrices.
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT SetValue(ICP_MAT_TYPE const cValue,
                          unsigned int const cRowIndex,
                          unsigned int const cColIndex,
                          unsigned int const cDepthIndex=0);

      unsigned int mRows;     // Height or Y
      unsigned int mColumns;  // Width or X
      unsigned int mDepth;    // Z

      friend void CopyRow <> (       ICP_Mat<ICP_MAT_TYPE>& matDst, unsigned int rowDst,
                               const ICP_Mat<ICP_MAT_TYPE>& matSrc, unsigned int rowSrc);

#ifdef _WIN32
	  friend std::ostream& (operator<<)(std::ostream& stream, ICP_Mat& ob);
	  friend std::istream& (operator>>)(std::istream& stream, ICP_Mat& ob);
#endif
      
   private:
      ///matrix data
      void*         mhDataOal;
      ICP_MAT_TYPE* mpData;
      unsigned int mBufferSize;

   public:
      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that adds the elements of a source matrice to 
      /// the elements of a destination matrix
      ///
      /// \param rcSource2
      /// the source matrices
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT Add(const ICP_Mat& rcSource2,
                           ICP_Mat& rDestination) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that copies a matrix onto another
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT CopyTo(ICP_Mat& rDestination) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that multiplies each element of a matrix with each elements
      /// of another matrix
      ///
      /// \param rcSource2
      /// the source matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      ICP_MAT_TYPE DotProduct(const ICP_Mat& rcSource2) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that compares an entire matrix to a value
      ///
      /// \param cValue
      /// the value to search for
      ///
      /// \return
      /// true when the entire matrix matches the value.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      bool Equals(ICP_MAT_TYPE const cValue) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that fills a matrix with a value
      ///
      /// \param cFillValue
      /// the value to fill
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT Fill(const ICP_MAT_TYPE cFillValue);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that multiplies two matrices
      ///
      /// \param rcSource2
      /// the source matrix
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT Multiply(const ICP_Mat& rcSource2,
                                ICP_Mat& rDestination)const;
      LIB_RESULT Multiply1(const ICP_Mat& rcSource2,
                                ICP_Mat& rDestination)const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that inverts a matrix
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT Inverse(ICP_Mat& rDestination) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that inverts a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      ICP_Mat Inv() const
      {
         ICP_Mat rDestination;
         Inverse(rDestination);
         return rDestination;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that produces a numerical approximation of a matrix inversion
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      LIB_RESULT PseudoInverse(ICP_Mat& rDestination) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that produces the sum of all elements of a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      /////////////////////////////////////////////////////////////////////////
      double Sum() const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that counts the total number of elements in a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      uint32_t Total() const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that scales all elements in a matrix by a scalar
      ///
      /// \param cScalar
      /// the scaling factor
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT Scale(const ICP_MAT_TYPE    cScalar,
                             ICP_Mat& rDestination) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that turns a matrix into an identity matrix scaled by a scalar
      ///
      /// \param cScalar
      /// the scaling factor
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT SetIdentity(const ICP_MAT_TYPE cScalar);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that subtracts a matrix from another into a destination matrix
      ///
      /// \param rcSource2
      /// the source2 matrix to subtract
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT Subtract(const ICP_Mat& rcSource2,
                                ICP_Mat& rDestination) const;
      
      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that subtracts a matrix from another into a destination matrix
      ///
      /// \param rcSource1
      /// the source1 matrix
      ///
      /// \param rcSource2
      /// the source2 matrix to subtract
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT SubAbs(const ICP_Mat& rcSource1,
                      const ICP_Mat& rcSource2,
                      ICP_Mat& rDestination) const;

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that releases the memory allocated by a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT Release(void);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that releases the memory allocated by a matrix
      /// \param rcSource
      ///
      /// \param rDestination
      ///
      /// \param mode
      ///
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT SortIdx(ICP_Mat& rcSource,
                           ICP_Mat& rDestination,
                           int mode);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that compares all elements of a matrix to a threshold
      /// and sets to 0 those below the threshold
      ///
      /// \param rcSource         the source matrix
      /// \param rDestination     the destination matrix
      /// \param thres            the comparison threshold
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT Threshold(ICP_Mat& rcSource,
                           ICP_Mat& rDestination,
                           ICP_MAT_TYPE thres);

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that produces the transposition of a matrix
      ///
      /// \param rDestination
      /// the destination matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      LIB_RESULT Transpose(ICP_Mat& rDestination) const;

#ifdef SUPPORT_OPENCV_CONVERSIONS
      /** 
      * Conversion Functions
      */
      void Copy_ICP_2_CV(cv::Mat_<ICP_MAT_TYPE>& cMat) const;
      void Copy_CV_2_ICP(const cv::Mat_<ICP_MAT_TYPE>& cMat);


      // cast
      operator cv::Mat_<ICP_MAT_TYPE>() const
      {
         cv::Mat_<ICP_MAT_TYPE> cMat(mRows, mColumns);
         Copy_ICP_2_CV(cMat);
         return cMat;
      }
#endif

#ifdef _WIN32
      void outputMatrix(const char* title, std::ostream &stream=std::cout);
#else
      void outputMatrix(const char* title);
#endif

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the width (number of columns) in pixels of a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      unsigned int ReturnWidth() const 
      {
         return mColumns;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the number of columns of a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      unsigned int ReturnColumn() const
      {
         return mColumns;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the number of rows of a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      unsigned int ReturnHeight() const 
      {
         return mRows;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the number of rows of a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      unsigned int ReturnRow() const
      {
         return mRows;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the Mat element size, in bytes
      ///
      /// \return
      /// The element size, in bytes.
      ///////////////////////////////////////////////////////////////////////// 
      unsigned int ReturnElementSizeInBytes() const
      {
         return sizeof(ICP_MAT_TYPE);
      }

      unsigned int ReturnSpan() const
      {
         return mColumns;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the number of columns of a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      unsigned int ReturnStride() const
      {
         return mColumns;
      }

      unsigned int ReturnSizeInBytes() const
      {
         return mColumns * mRows * sizeof(ICP_MAT_TYPE);
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns the number of columns x the number of rows of a matrix
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      ICP_Size Size() const
      {
         ICP_Size s(mColumns, mRows);
         return s;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns a pointer to the data buffer of a matrix 
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      ICP_MAT_TYPE* ReturnPtr() const
      {
         return mpData;
      }

      /////////////////////////////////////////////////////////////////////////
      /// \par Description
      ///
      /// Function that returns a pointer to a row inside a matrix 
      /// \param cRowIndex
      ///
      /// \return
      /// true when the operation is successful.  false otherwise
      ///////////////////////////////////////////////////////////////////////// 
      ICP_MAT_TYPE* ReturnRowPtr(uint32_t const cRowIndex) const
      {
         ICP_MAT_TYPE* rData  = &(mpData[cRowIndex*mColumns]);
         return rData;
      }
};



typedef ICP_Mat<double>    ICP_Mat2d;
typedef ICP_Mat<float>     ICP_Mat2f;
typedef ICP_Mat<uint32_t>  ICP_Mat2i_32u;
typedef ICP_Mat<int32_t>   ICP_Mat2i_32s;
//typedef ICP_Mat<uint32_t>  ICP_Mat2i_16u;
//typedef ICP_Mat<int32_t>   ICP_Mat2i_16s;
typedef ICP_Mat<uint16_t>  ICP_Mat2i_16u;
typedef ICP_Mat<int16_t>   ICP_Mat2i_16s;
typedef ICP_Mat<uint8_t>   ICP_Mat2i_8u;

/// @}

#include "oal.h"


template <class ICP_MAT_TYPE>
ICP_Mat<ICP_MAT_TYPE>::ICP_Mat()
   : mRows(0),         // Height or Y
     mColumns(0),      // Width or X
     mDepth(0),        // Z
     mpData(0), 
     mBufferSize(0)
{
}

template <class ICP_MAT_TYPE>
ICP_Mat<ICP_MAT_TYPE>::ICP_Mat(const ICP_Mat<ICP_MAT_TYPE> &mat)
   : mRows(mat.mRows),         // Height or Y
     mColumns(mat.mColumns),   // Width or X
     mDepth(mat.mDepth),       // Z
     mpData(0),
     mBufferSize(0)
{
   // if the mat.mpData is good, rest should be OK
   if(mat.ReturnPtr())
   {
      mBufferSize = mRows * mColumns * mDepth * sizeof(ICP_MAT_TYPE);

      //Allocate memory required for matrix
      mhDataOal = OAL_MemoryAllocFlag(mBufferSize,
                                      OAL_MEMORY_FLAG_CONTIGUOUS |
                                      OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE));

      mpData = (ICP_MAT_TYPE*)OAL_MemoryReturnAddress(mhDataOal, ACCESS_CH_WB);

      memcpy(mpData, mat.ReturnPtr(), mBufferSize);
   }
}

template <class ICP_MAT_TYPE>
ICP_Mat<ICP_MAT_TYPE>::ICP_Mat(unsigned int const cNumberOfRows,
                 unsigned int const cNumberOfCols,
                 unsigned int const cDepth)
   : mRows(cNumberOfRows),         // Height or Y
     mColumns(cNumberOfCols),      // Width or X
     mDepth(cDepth),                // Z
     mpData(0)
{

   assert (mRows    >= 1 &&
           mColumns >= 1 &&
           mDepth   >= 1);

   mBufferSize = mRows * mColumns * mDepth * sizeof(ICP_MAT_TYPE);

   //Allocate memory required for matrix
   mhDataOal = OAL_MemoryAllocFlag(mBufferSize,
                                   OAL_MEMORY_FLAG_CONTIGUOUS |
                                   OAL_MEMORY_FLAG_ZERO |
                                   OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE));

   mpData = (ICP_MAT_TYPE*)OAL_MemoryReturnAddress(mhDataOal, ACCESS_CH_WB);
}


template <class ICP_MAT_TYPE>
ICP_Mat<ICP_MAT_TYPE>::~ICP_Mat()
{
   if(mpData != 0)
   {
      OAL_MemoryFree(mhDataOal);
      mhDataOal = 0;
      mpData = 0;
   }

   mBufferSize = 0;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Add(const ICP_Mat& rcSource2,
                              ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((mpData != 0) && (rcSource2.mpData != 0) &&
          (mRows    >= 1) && (mRows    == rcSource2.mRows) &&      // Y
          (mColumns >= 1) && (mColumns == rcSource2.mColumns) &&   // X
          (mDepth   == 1) && (mDepth   == rcSource2.mDepth));       // Z

   {
      if(&rDestination != this)
      {
         rDestination.Create(mRows,       // Rows
                             mColumns,    // Columns
                             mDepth);     // Depth
      }

      if(rDestination.mpData != 0)
      {
         for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
         {
            rDestination.mpData[c] = mpData[c] + rcSource2.mpData[c];
         }

#if 0
         int s0 = ICL_PRF_Get_Tick();
         // TODO: Inefficient should do rows first then columns!!!
         // 1. Current
         for(uint32_t x = 0; x < mColumns; ++x)
         {
            for(uint32_t y = 0; y < mRows; ++y)
            {
               const uint32_t cCurrentIndex = y*mColumns + x;

               rDestination.mpData[cCurrentIndex] = mpData[cCurrentIndex] * rcSource2.mpData[cCurrentIndex];
            }
         }
         int s1 = ICL_PRF_Get_Tick();

         // 2. Row / Column Swapped:
         for(uint32_t y = 0; y < mRows; ++y)
         {
            for(uint32_t x = 0; x < mColumns; ++x)
            {
               const uint32_t cCurrentIndex = y*mColumns + x;

               rDestination.mpData[cCurrentIndex] = mpData[cCurrentIndex] * rcSource2.mpData[cCurrentIndex];
            }
         }
         int s2 = ICL_PRF_Get_Tick();

         // 3. Single Loop
         for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
         {
            rDestination.mpData[c] = mpData[c] * rcSource2.mpData[c];
         }
         int s3 = ICL_PRF_Get_Tick();

         // 4. Use Pointers Loop
         ICP_MAT_TYPE* pDest = rDestination.mpData;
         ICP_MAT_TYPE* pThis = mpData;
         ICP_MAT_TYPE* pSrc  = rcSource2.mpData;

         for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
         {
            *pDest++ = *pThis++ * *pSrc++;
         }
         int s4 = ICL_PRF_Get_Tick();

         // 5. Use Pointers Loop
         const unsigned int cEndIndex = mRows * mColumns * mDepth;
         for(unsigned int c = 0; c < cEndIndex; ++c)
         {
            rDestination.mpData[c] = mpData[c] * rcSource2.mpData[c];
         }
         int s5 = ICL_PRF_Get_Tick();

         printf("%d %d %d %d %d\n", s1-s0, s2-s1, s3-s2, s4-s3, s5-s4);
#endif
         libResult = LIB_SUCCESS;
      }
   }

   return libResult;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Create(unsigned int const cNumberOfRows,
                           unsigned int const cNumberOfCols,
                           unsigned int const cDepth)
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert(cNumberOfRows >= 1 &&
          cNumberOfCols >= 1 &&
          cDepth        >= 1);
   {
      const uint32_t cNewDataSize = cNumberOfRows * cNumberOfCols * cDepth * sizeof(ICP_MAT_TYPE);

      mRows    = cNumberOfRows;  // Height or Y
      mColumns = cNumberOfCols;  // Width or X
      mDepth   = cDepth;         // Z

      if(mBufferSize < cNewDataSize &&
         mpData != 0)
      {
         OAL_MemoryFree((void*)mhDataOal);
         mhDataOal = 0;
         mpData = 0;
      }

      if(mpData == 0)
      {
         mBufferSize = cNewDataSize;

         //Allocate memory required for matrix
         mhDataOal = OAL_MemoryAllocFlag(mBufferSize,
                                         OAL_MEMORY_FLAG_CONTIGUOUS |
                                         OAL_MEMORY_FLAG_ZERO |
                                         OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE));

         mpData = (ICP_MAT_TYPE*)OAL_MemoryReturnAddress(mhDataOal, ACCESS_CH_WB);
      }

      if(mpData != 0)
      {
         libResult = LIB_SUCCESS;
      }
   }

   return libResult;
}

template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Release(void)
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert(mRows >= 1 &&
          mColumns >= 1 &&
          mDepth        >= 1);
   {
      mRows    = 0;  // Height or Y
      mColumns = 0;  // Width or X
      mDepth   = 0;         // Z

      if(mpData != 0)
      {
         OAL_MemoryFree((void*)mhDataOal);
         mhDataOal = 0;
         mpData = 0;
      }
      libResult = LIB_SUCCESS;
   }

   return libResult;
}

template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::CopyTo(ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((&rDestination) != this &&
          (mpData != 0) &&
          (mRows    >= 1) &&    // Y
          (mColumns >= 1) &&    // X
          (mDepth   == 1));      // Z
   {
      rDestination.Create(mRows,     // Rows
                          mColumns,  // Columns
                          mDepth);   // Depth

      if(rDestination.mpData != 0)
      {
         for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
         {
            rDestination.mpData[c] = mpData[c];
         }

         libResult = LIB_SUCCESS;
      }
   }

   return libResult;
}

template <class D, class S>
LIB_RESULT
Cast(ICP_Mat<D>& dst, const ICP_Mat<S>& src)
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((&dst) != (&src) &&
                //(src.mpData   != 0) &&
                (src.mRows    >= 1) &&    // Y
                (src.mColumns >= 1) &&    // X
                (src.mDepth   == 1));      // Z
   {
      dst.Create(src.mRows, src.mColumns, src.mDepth);

      const S* sptr = src.ReturnPtr();
            D* dptr = dst.ReturnPtr();
      if (0 != dptr)
      {
         for(unsigned int c = 0; c < src.mRows * src.mColumns * src.mDepth; ++c)
         {
            dptr[c] = (D)sptr[c];
         }

         libResult = LIB_SUCCESS;
      }
   }

   return libResult;
}


template <class ICP_MAT_TYPE>
ICP_MAT_TYPE ICP_Mat<ICP_MAT_TYPE>::DotProduct(const ICP_Mat& rcSource2) const
{
   assert((mpData != 0) && (rcSource2.mpData != 0) &&
          (mRows    >= 1) && (mRows    == rcSource2.mRows) &&      // Y
          (mColumns >= 1) && (mColumns == rcSource2.mColumns) &&   // X
          (mDepth   == 1) && (mDepth   == rcSource2.mDepth));      // Z

   ICP_MAT_TYPE result = 0;
   for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
   {
      result += mpData[c] * rcSource2.mpData[c];
   }

   return result;
}

template <class ICP_MAT_TYPE>
bool ICP_Mat<ICP_MAT_TYPE>::Equals(ICP_MAT_TYPE const cEqualValue) const
{
   assert(mpData != 0 &&
          mRows    >= 1 &&
          mColumns >= 1 &&
          mDepth   == 1);

   for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
   {
      if(fabs(mpData[c]- cEqualValue) > 0.0001) // Magic Number Bad!!!
      {
         return false;
      }
   }

   return true;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Fill(const ICP_MAT_TYPE cFillValue)
{
   assert(mpData != 0 &&
          mRows    >= 1 &&
          mColumns >= 1 &&
          mDepth   == 1);

   for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
   {
      mpData[c] = cFillValue;
   }

   return LIB_SUCCESS;
}

template <class ICP_MAT_TYPE>
ICP_Mat<ICP_MAT_TYPE>& ICP_Mat<ICP_MAT_TYPE>::operator=(const ICP_Mat& lMat)
{
   lMat.CopyTo(*this);
   return *this;
}

#ifdef SUPPORT_OPENCV_CONVERSIONS
template <class ICP_MAT_TYPE>
ICP_Mat<ICP_MAT_TYPE>& ICP_Mat<ICP_MAT_TYPE>::operator=(const cv::Mat_<ICP_MAT_TYPE> & lMat)
{
   cv::Size2i s = lMat.size();
   this->Create(s.height, s.width);

   //Copy the data.
   //OpenCV documentation says : 
   // The elements are stored in row-major order (row by row).
   //ICP says the same thing
   //Direct copy is OK. But cannot copy due to potential type mismatch
   //Need to access element at a time:
   for (int y=0; y < s.height; y++)
   {
      for(int x=0; x< s.width; x++)
      {
         (*this)(y, x) = (ICP_MAT_TYPE) lMat.at<ICP_MAT_TYPE>(y, x);
      }
   }

   return *this;
}
#endif

template <class ICP_MAT_TYPE>
ICP_Mat<ICP_MAT_TYPE>* ICP_Mat<ICP_MAT_TYPE>::GetCols(unsigned int startcol,
                          unsigned int endcol)
{
   assert(mpData != 0 &&
          mRows    >= 1 &&
          mColumns >= 1 &&
          mDepth   == 1 &&
          mColumns >  startcol &&
          mColumns >= endcol);

   ICP_Mat* subMat = new ICP_Mat();

   if(subMat != 0)
   {
      subMat->Create(mRows,
                     endcol-startcol);

      for (unsigned col = startcol; col < endcol; col++)
      {
         unsigned subMatOffset = (col-startcol) * mRows;
         unsigned thisOffset = col * mRows;
         for (unsigned row = 0; row < mRows; row++)
         {
            subMat->mpData[subMatOffset +  row] = mpData[thisOffset + row];
         }
      }
   }

   return subMat;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::GetValue(ICP_MAT_TYPE& rReturnedValue,
                             unsigned int const cRowIndex,
                             unsigned int const cColIndex,
                             unsigned int const cDepthIndex) const
{
   assert(mpData != 0 &&
          cRowIndex   < mRows &&
          cColIndex   < mColumns &&
          cDepthIndex < mDepth);

   rReturnedValue = mpData[cRowIndex*mColumns + cColIndex];

   return LIB_SUCCESS;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::MinMaxLoc(ICP_MAT_TYPE* minVal,
                              ICP_MAT_TYPE* maxVal,
                              ICP_Point* minLoc,
                              ICP_Point* maxLoc)
{
 
   ICP_MAT_TYPE tmpMinVal = mpData[0];
   ICP_MAT_TYPE tmpMaxVal = mpData[0];
   ICP_Point tmpMinLoc;
   tmpMinLoc.x = 0;
   tmpMinLoc.y = 0;
   ICP_Point tmpMaxLoc;
   tmpMaxLoc.x = 0;
   tmpMaxLoc.y = 0;
   ICP_MAT_TYPE value;

   assert(mpData != 0 &&
          mRows    >= 1 &&
          mColumns >= 1 &&
          mDepth   == 1);

   unsigned int c = 0;
   for(unsigned int d = 0; d < mDepth; ++d)
   {
      for(unsigned int y = 0; y < mRows; ++y)
      {
         for(unsigned int x = 0; x < mColumns; ++x)
         {
            value =  mpData[c];

            if (value < tmpMinVal)
            {
               tmpMinVal = value;
               tmpMinLoc.x = x;
               tmpMinLoc.y = y;
            }
            else if (value > tmpMaxVal)
            {
               tmpMaxVal = value;
               tmpMaxLoc.x = x;
               tmpMaxLoc.y = y;
            }
            c++ ;
         }
      }
   }

   if (minVal != 0)
   {
      *minVal = tmpMinVal;
   }
   if (maxVal != 0)
   {
      *maxVal = tmpMaxVal;
   }
   if (minLoc != 0)
   {
      *minLoc = tmpMinLoc;
   }
   if (maxLoc != 0)
   {
      *maxLoc = tmpMaxLoc;
   }

   return LIB_SUCCESS;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Multiply1(const ICP_Mat& rcSource2,
                                   ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((&rcSource2   ) != this &&
          (&rDestination) != this &&
          (&rcSource2)    != (&rDestination) &&
          (mpData != 0) && (rcSource2.mpData != 0) &&
          (mRows    >= 1) &&              // Y
          (mColumns >= 1) &&              // X
          (mDepth   == 1) &&              // Z
          (mColumns == rcSource2.mRows)); // Y1xX1 and Y2xX2 where X1 == Y2 results in Y1xX2 matrix

   rDestination.Create(mRows,                 // Rows or Height
                        rcSource2.mColumns,    // Columns or Width
                        mDepth);               // Depth

   if(rDestination.mpData != 0)
   {
      for(uint32_t y1 = 0; y1 < mRows; ++y1)
      {
         for(uint32_t x2 = 0; x2 < rcSource2.mColumns; ++x2)
         {
            const uint32_t cDestIndex = (y1 * rcSource2.mColumns) + x2;

            rDestination.mpData[cDestIndex] = 0;

            for(uint32_t x1 = 0; x1 < mColumns; ++x1)
            {
               const uint32_t cSrc1Index = (y1 * mColumns) + x1;
               const uint32_t cSrc2Index = (x1 * rcSource2.mColumns) + x2;

               rDestination.mpData[cDestIndex] += ((mpData[cSrc1Index]) * (rcSource2.mpData[cSrc2Index]));
            }
         }
      }
      libResult = LIB_SUCCESS;
   }

   return libResult;
}
template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Multiply(const ICP_Mat& rcSource2,
                                   ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((&rcSource2   ) != this &&
          (&rDestination) != this &&
          (&rcSource2)    != (&rDestination) &&
          (mpData != 0) && (rcSource2.mpData != 0) &&
          (mRows    >= 1) &&              // Y
          (mColumns >= 1) &&              // X
          (mDepth   == 1) &&              // Z
          (mColumns == rcSource2.mRows)); // Y1xX1 and Y2xX2 where X1 == Y2 results in Y1xX2 matrix

   rDestination.Create(mRows,                 // Rows or Height
                       rcSource2.mColumns,    // Columns or Width
                       mDepth);               // Depth

   if(rDestination.mpData != 0)
   {
      for(uint32_t y1 = 0; y1 < mRows; ++y1)
      {
         for(uint32_t x2 = 0; x2 < rcSource2.mColumns; ++x2)
         {
            ICP_MAT_TYPE val = 0;
            unsigned int index1 = y1 * mColumns;
            unsigned int index2 = x2;
            for(uint32_t x1 = 0; x1 < mColumns; ++x1)
            {
               val += ((mpData[index1]) * (rcSource2.mpData[index2]));
               index1++;
               index2+=rcSource2.mColumns;
            }
            rDestination.mpData[(y1 * rcSource2.mColumns) + x2] = val;
         }
      }

      libResult = LIB_SUCCESS;
   }

   return libResult;
}

template <class ICP_MAT_TYPE>
ICP_MAT_TYPE ICP_Mat<ICP_MAT_TYPE>::operator()(unsigned int const cRowIndex,
                          unsigned int const cColIndex,
                          unsigned int const cDepthIndex) const
{
   assert(mpData != 0 &&
          cRowIndex   < mRows &&
          cColIndex   < mColumns &&
          cDepthIndex < mDepth);

   return mpData[cRowIndex*mColumns + cColIndex];
}


template <class ICP_MAT_TYPE>
ICP_MAT_TYPE& ICP_Mat<ICP_MAT_TYPE>::operator()(unsigned int const cRowIndex,
                           unsigned int const cColIndex,
                           unsigned int const cDepthIndex)
{
   assert(mpData != 0 &&
          cRowIndex   < mRows &&
          cColIndex   < mColumns &&
          cDepthIndex < mDepth);

   return mpData[cRowIndex*mColumns + cColIndex];
}

template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Inverse(ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((&rDestination) != this &&
          (mpData != 0) &&
          (mRows    >= 1) &&              // Y
          (mColumns >= 1) &&              // X
          (mDepth   == 1));               // Z
   {
      ICP_MAT_TYPE* pDest = 0;
      // Hard coded implementation for 2x2 and 3x3.
      if(2 == mRows && 2 == mColumns)
      {
         // A =
         // [a b
         //  c d]
         //
         // Inv(A) = 1 / (ad-bc)  * [ d -b
         //                          -c  a]
         ICP_MAT_TYPE determinant = mpData[0] * mpData[3] -
                                    mpData[1] * mpData[2];

         rDestination.Create(mRows, mColumns);
         pDest = rDestination.ReturnPtr();
         pDest[0] =   mpData[3] / determinant;
         pDest[3] =   mpData[0] / determinant;
         pDest[1] = - mpData[1] / determinant;
         pDest[2] = - mpData[2] / determinant;
      }
      else if(3 == mRows && 3 == mColumns)
      {
         // A =
         // [a b c
         //  d e f
         //  g h k]
         //
         // Inv(A) = 1 / det  *  [A D G
         //                       B E H
         //                       C F K]

         // A = (ek-fh)
         ICP_MAT_TYPE A = mpData[4] * mpData[8] -
                          mpData[5] * mpData[7];
         // B = (fg-dk)
         ICP_MAT_TYPE B = mpData[5] * mpData[6] -
                          mpData[3] * mpData[8];
         // C = (dh-eg)
         ICP_MAT_TYPE C = mpData[3] * mpData[7] -
                          mpData[4] * mpData[6];

         ICP_MAT_TYPE determinant = mpData[0] * A +
                                    mpData[1] * B +
                                    mpData[2] * C ;

         rDestination.Create(mRows, mColumns);
         pDest = rDestination.ReturnPtr();
         pDest[0] = A / determinant;
         pDest[3] = B / determinant;
         pDest[6] = C / determinant;

         // D = (ch - bk)
         pDest[1] = ( mpData[2] * mpData[7] -
                      mpData[1] * mpData[8] ) / determinant;
         // E = (ak - cg)
         pDest[4] = ( mpData[0] * mpData[8] -
                      mpData[2] * mpData[6] ) / determinant;
         // F = (gb - ah)
         pDest[7] = ( mpData[6] * mpData[1] -
                      mpData[0] * mpData[7] ) / determinant;

         // G = (bf - ce)
         pDest[2] = ( mpData[1] * mpData[5] -
                      mpData[2] * mpData[4] ) / determinant;
         // H = (cd - af)
         pDest[5] = ( mpData[2] * mpData[3] -
                      mpData[0] * mpData[5] ) / determinant;
         // K = (ae - bd)
         pDest[8] = ( mpData[0] * mpData[4] -
                      mpData[1] * mpData[3] ) / determinant;
      }
      else
      {
         assert(0);
      }
   }

   return libResult;
}
template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::PseudoInverse(ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((&rDestination) != this &&
          (mpData != 0) &&
          (mRows    >= 1) &&              // Y
          (mColumns >= 1) &&              // X
          (mDepth   == 1));               // Z
   {
      ICP_Mat matA(mRows, 1);
      // Temporary Buffers are created
      // and when used, they are initially always OUTPUT. 
      // Therefore, Create() sets it up correctly.
      ICP_Mat matB(mRows, mColumns);
      ICP_Mat matC(mRows, mColumns);
      ICP_Mat matD(mRows, mColumns);

      for(uint32_t y = 0; y < mRows; ++y)
      {
         matA(y,0) = (*this)(y,0);  // Free [B, C, D, E, F, G]
      }

      if(matA.Equals(0.0) == false)
      {
         libResult = matA.Transpose(rDestination);

         const ICP_MAT_TYPE cMatA_Dot = matA.DotProduct(matA);

         if(cMatA_Dot != 0)
         {
            libResult = rDestination.Scale(1.0f/cMatA_Dot,
                                           rDestination);
         }
      }
      else
      {
         rDestination.Create(1, mColumns);

         libResult = rDestination.Fill(0.0);
      }

      uint32_t kX = 1; // Not sure why start at 1!!!

      // Create them with max size so we don't realloc all the time.
      ICP_Mat matT1(mRows, mColumns);
      ICP_Mat matT2(mRows, mColumns);
      ICP_Mat matT3(mRows, mColumns);

      while(kX < mColumns)
      {
         for(uint32_t y = 0; y < mRows; ++y)
         {
            matA(y,0) = (*this)(y,kX);          // (ak)(r, 0) = (*this)(r, k);
         }

         libResult = rDestination.Multiply(matA,
                                           matD); // dk = Mat<Type>((R_plus)->*(ak));

         matT1.Create(mRows,
                      kX);       // Mat<Type> T(_h, k);

         for(uint32_t y = 0; y < mRows; ++y)
         {
            for(uint32_t x = 0; x < kX; ++x)
            {
               matT1(y,x) = (*this)(y,x);       // (T)(r,c)= (*this)(r,c);
            }
         }

         libResult = matT1.Multiply(matD,
                                    matT2);     // Mat<Type> tmp((T)->*(dk));

         libResult = matA.Subtract(matT2,
                                   matC);      // ck = Mat<Type>((ak)-(tmp));

         if(matC.Equals(0.0) == false)
         {
            libResult = matC.Transpose(matB);               // (ck).tr()

            const ICP_MAT_TYPE cMatC_Dot = matC.DotProduct(matC);  // (ck).dot(ck)

            if(cMatC_Dot != 0)
            {
               libResult = matB.Scale(1.0f/cMatC_Dot,
                                      matB);       // bk= Mat<Type>((ck).tr()*( 1.0/( ((ck).dot(ck)) ) ));
            }
         }
         else
         {
            libResult = matD.Transpose(matT2);                 // (dk).tr()

            const ICP_MAT_TYPE cMatD_Dot = matD.DotProduct(matD);     // (dk).dot(dk)

            if(cMatD_Dot != -1.0)
            {
               libResult = matT2.Scale(1.0f/(1.0f+cMatD_Dot),
                                       matT2);   // (dk).tr()*( 1.0/( 1.0+((dk).dot(dk)) )  )

               libResult = matT2.Multiply(rDestination,
                                          matB);               // bk= Mat<Type>(((dk).tr()*( 1.0/( 1.0+((dk).dot(dk)) )  ))->*(R_plus));
            }
         }

         libResult = matD.Multiply(matB,
                                   matT2);         // tmp=Mat<Type>((dk)->*(bk));

         libResult = rDestination.Subtract(matT2,
                                           matT3);  // Mat<Type> N((R_plus)-(tmp));

         rDestination.Create(matT3.mRows + 1,
                             matT3.mColumns);  // R_plus = Mat<Type>(N.height()+1, N.width());

         for(uint32_t y = 0; y < matT3.mRows; ++y)
         {
            for(uint32_t x = 0; x < matT3.mColumns; ++x)
            {
               rDestination(y,x) = matT3(y,x);
            }
         }

         for(uint32_t x = 0; x < matT3.mColumns; ++x)
         {
            rDestination(rDestination.mRows - 1, x) = matB(0,x);
         }

         ++kX;
      }
   }

   return libResult;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Scale(const ICP_MAT_TYPE    cScalar,
                                ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((mpData != 0) &&
          (mRows    >= 1) &&    // Y
          (mColumns >= 1) &&    // X
          (mDepth   == 1));      // Z

   if(&rDestination != this)
   {
      rDestination.Create(mRows,    // Rows
                          mColumns, // Columns
                          mDepth);  // Depth
   }

   if(rDestination.mpData != 0)
   {
      for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
      {
         rDestination.mpData[c] *= cScalar;
      }

      libResult = LIB_SUCCESS;
   }

   return libResult;
}

template <class ICP_MAT_TYPE>
double ICP_Mat<ICP_MAT_TYPE>::Sum() const
{
   double sumValue = 0;

   assert(mpData != 0 &&
          mRows    >= 1 &&
          mColumns >= 1 &&
          mDepth   == 1);

   for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
   {
      sumValue += mpData[c];
   }

   return sumValue;
}

template <class ICP_MAT_TYPE>
uint32_t ICP_Mat<ICP_MAT_TYPE>::Total() const
{
   uint32_t totalValue = mRows*mColumns*mDepth;
   return totalValue;
}

template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::SetIdentity(const ICP_MAT_TYPE cScalar)
{
   assert(mpData != 0 &&
          mRows    >= 1 &&
          mColumns >= 1 &&
          mDepth   == 1);

   memset((void*)mpData,
          0,
          sizeof(ICP_MAT_TYPE) * mRows * mColumns * mDepth);

   for(uint32_t x = 0; x < mRows && x < mColumns; ++x)
   {
      mpData[x*mColumns + x] = cScalar;
   }

   return LIB_SUCCESS;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::SetValue(ICP_MAT_TYPE const cValue,
                             unsigned int const cRowIndex,
                             unsigned int const cColIndex,
                             unsigned int const cDepthIndex)
{
   assert(mpData != 0 &&
          cRowIndex   < mRows &&
          cColIndex   < mColumns &&
          cDepthIndex < mDepth);

   mpData[cColIndex + (cRowIndex*mColumns)] = cValue;

   return LIB_SUCCESS;
}


template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Subtract(const ICP_Mat& rcSource2,
                                   ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((mpData != 0) && (rcSource2.mpData != 0) &&
          (mRows    >= 1) && (mRows    == rcSource2.mRows) &&      // Y
          (mColumns >= 1) && (mColumns == rcSource2.mColumns) &&   // X
          (mDepth   == 1) && (mDepth   == rcSource2.mDepth));       // Z

   if(&rDestination != this)
   {
      rDestination.Create(mRows,       // Rows
                          mColumns,    // Columns
                          mDepth);     // Depth
   }

   if(rDestination.mpData != 0)
   {
      for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
      {
         rDestination.mpData[c] = mpData[c] - rcSource2.mpData[c];
      }

      libResult = LIB_SUCCESS;
   }

   return libResult;
}

template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::SubAbs(const ICP_Mat& rcSource1,
                                       const ICP_Mat& rcSource2,
                                       ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((mpData != 0) && (rcSource2.mpData != 0) &&
          (mRows    >= 1) && (mRows    == rcSource2.mRows) &&      // Y
          (mColumns >= 1) && (mColumns == rcSource2.mColumns) &&   // X
          (mDepth   == 1) && (mDepth   == rcSource2.mDepth));       // Z

   if(&rDestination != this)
   {
      rDestination.Create(mRows,       // Rows
                          mColumns,    // Columns
                          mDepth);     // Depth
   }

   if(rDestination.mpData != 0)
   {
      uint8_t * src1ptr = (uint8_t *)rcSource1.mpData;
      uint8_t * src2ptr = (uint8_t *)rcSource2.mpData;
      uint8_t * destptr = (uint8_t *)rDestination.mpData;
      
      for(unsigned int c = 0; c < mRows * mColumns * mDepth; ++c)
      {
         *destptr++ = abs(*src1ptr++ - *src2ptr++);
      }

      libResult = LIB_SUCCESS;
   }

   return libResult;
}
template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::SortIdx(ICP_Mat& rcSource,
                                           ICP_Mat& rDestination,
                                           int mode)
{
   LIB_RESULT libResult = LIB_FAILURE;

   mode = mode; 

   assert((mpData != 0) && (rcSource.mpData != 0) &&
          (mRows    >= 1) && (mRows    == rcSource.mRows) &&      // Y
          (mColumns >= 1) && (mColumns == rcSource.mColumns) &&   // X
          (mDepth   == 1) && (mDepth   == rcSource.mDepth));       // Z

   if(&rDestination != this)
   {
      rDestination.Create(mRows,       // Rows
                          mColumns,    // Columns
                          mDepth);     // Depth
   }

   if(rDestination.mpData != 0)
   {
      for(unsigned int c = 0; c < mColumns ; ++c)
      {
         // find maximum value
         //
         int index = 0;
         float aData = rcSource.mpData[index + c * mColumns];
         bool newEntry = true;
         for(unsigned int r = 0; r < mRows ; r++)
         {
            if ((aData < rcSource.mpData[r]))
            {
               index = r;
               aData = rcSource.mpData[r];
            }
         }
         // put index in the mpData
         rDestination.mpData[0] = index;
         
         for (unsigned int j = 1; j < mRows; j++)
         {
            aData = 0;
            index = 0;
            for(unsigned int r = 0; r < mRows ; r++)
            {
               if (aData < rcSource.mpData[r])
               {
                  newEntry = true;
                  for (unsigned int k = 0; k < j; k++)
                  {
                     if ((unsigned int) rDestination.mpData[k] == r)
                     {
                        newEntry = false;
                     }
                  }
                  if (newEntry)
                  {
                     index = r;
                     aData = rcSource.mpData[r];
                  }
               }
            }
            rDestination.mpData[j] = index;
         }
      }

      libResult = LIB_SUCCESS;
   }

   return libResult;
}

template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Threshold(ICP_Mat& rcSource,
                                            ICP_Mat& rDestination,
                                            ICP_MAT_TYPE thr)
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((mpData != 0) && (rcSource.mpData != 0) &&
          (mRows    >= 1) && (mRows    == rcSource.mRows) &&      // Y
          (mColumns >= 1) && (mColumns == rcSource.mColumns) &&   // X
          (mDepth   == 1) && (mDepth   == rcSource.mDepth));      // Z

   if(&rDestination != this)
   {
      rDestination.Create(mRows,       // Rows
                          mColumns,    // Columns
                          mDepth);     // Depth
   }

   for(unsigned int r = 0; r < mRows * mColumns ; ++r)
   {
      if (rcSource.mpData[r] > thr)
      {
         rDestination.mpData[r] = thr;
      }
   }


   return libResult;
}

template <class ICP_MAT_TYPE>
LIB_RESULT ICP_Mat<ICP_MAT_TYPE>::Transpose(ICP_Mat& rDestination) const
{
   LIB_RESULT libResult = LIB_FAILURE;

   assert((&rDestination) != this &&
          (mpData != 0) &&
          (mRows    >= 1) &&    // Y
          (mColumns >= 1) &&    // X
          (mDepth   == 1));      // Z

   // Intentionally swapping rows and columns to create an NxM matrix based on original MxN
   rDestination.Create(mColumns,  // Rows
                       mRows,     // Columns
                       mDepth);   // Depth

   if(rDestination.mpData != 0)
   {
      // Compiler can optimize this part.
      // It can completely remove the multiplications in the indexing
      // and translate into something similar to the version below.
      // Profiled it and results were exactly the same.
      // Keeping this comment and the code for reference....
      for(uint32_t x = 0; x < mColumns; ++x)
      {
         for(uint32_t y = 0; y < mRows; ++y)
         {
            const uint32_t cSrcIndex = y*mColumns + x;
            const uint32_t cDstIndex = x*mRows + y;

            rDestination.mpData[cDstIndex] = mpData[cSrcIndex];
         }
      }

#if 0
      unsigned int index=0;
      unsigned int destIndex= 0;
      for(unsigned int y = 0; y < mRows; ++y)
      {
         destIndex = y;
         for(unsigned int x = 0; x < mColumns; ++x)
         {
            rDestination.mpData[destIndex+mColumns] = mpData[index++];
         }
      }
#endif

      libResult = LIB_SUCCESS;
   }

   return libResult;
}

template <class ICP_MAT_TYPE>
void
CopyRow(       ICP_Mat<ICP_MAT_TYPE>& matDst, unsigned int rowDst, 
         const ICP_Mat<ICP_MAT_TYPE>& matSrc, unsigned int rowSrc)
{
   assert((matDst.ReturnPtr() != 0) &&
          (matSrc.ReturnPtr() != 0) &&
          (matDst.ReturnColumn() == matSrc.ReturnColumn()) &&    // Y
          (matDst.ReturnRow() >= rowDst) &&
          (matSrc.ReturnRow() >= rowSrc));
   {
      memcpy(matDst.ReturnRowPtr(rowDst), matSrc.ReturnRowPtr(rowSrc), matSrc.ReturnColumn()*sizeof(ICP_MAT_TYPE));
   }

   return;
}

#ifdef SUPPORT_OPENCV_CONVERSIONS
/** 
* Conversion Functions
*/
template <class ICP_MAT_TYPE>
void ICP_Mat<ICP_MAT_TYPE>::Copy_ICP_2_CV(cv::Mat_<ICP_MAT_TYPE>& cMat) const
{

   if(((mpData != 0) &&
       (mRows    >= 1) &&    // Y
       (mColumns >= 1) &&    // X
       (mDepth   >= 1)))      // Z
   {

      if(cMat.data != 0)
      {
         for(uint32_t x = 0; x<mRows ; ++x)
         {
            ICP_MAT_TYPE* rDestination = (ICP_MAT_TYPE*)cMat.row(x).data;
            ICP_MAT_TYPE* rSource      = (ICP_MAT_TYPE*)ReturnRowPtr(x);
            for(uint32_t y = 0; y < mColumns; ++y)
            {
               rDestination[y]   = rSource[y];
            }
         }

      }
   }
}

template <class ICP_MAT_TYPE>
void ICP_Mat<ICP_MAT_TYPE>::Copy_CV_2_ICP(const cv::Mat_<ICP_MAT_TYPE>& cMat)
{

   assert((mpData   != 0) &&
          (mRows    >= 1) &&
          (mColumns >= 1) &&
          (mDepth   >= 1) &&
          (mRows    == cMat.rows) &&
          (mColumns    == cMat.cols));
   {

      if(cMat.data != 0)
      {
         for(uint32_t x = 0; x<mRows ; ++x)
         {
            ICP_MAT_TYPE* rSource      = (ICP_MAT_TYPE*)cMat.row(x).data;
            ICP_MAT_TYPE* rDestination = (ICP_MAT_TYPE*)ReturnRowPtr(x);
            for(uint32_t y = 0; y < mColumns; ++y)
            {
               rDestination[y]   = rSource[y];
            }
         }

      }
   }
}
#endif

#ifdef _WIN32 //Win32

// Writing a Basic Matrix into a stream.
template <class ICP_MAT_TYPE>
std::ostream &operator<<(std::ostream &stream, ICP_Mat<ICP_MAT_TYPE> &ob)
{
   if(((ob.mpData != 0) &&
       (ob.mRows    >= 1) &&    // Y
       (ob.mColumns >= 1) &&    // X
       (ob.mDepth   >= 1)))      // Z
   {
      stream << ob.mRows << "\t" <<ob. mColumns << std::endl;

      for(uint32_t d = 0; d < ob.mDepth; ++d)
      {
         for(uint32_t y = 0; y < ob.mRows; ++y)
         {
            for(uint32_t x = 0; x < ob.mColumns; ++x)
            {
               stream << std::scientific << std::setprecision(7)<< ob.mpData[d*(ob.mRows * ob.mColumns) + y*ob.mColumns + x] << "\t";
            }
            stream << std::endl;
         }
      }
   }

   return stream;
}

template <class ICP_MAT_TYPE>
void ICP_Mat<ICP_MAT_TYPE>::outputMatrix(const char* title, std::ostream &stream)
{
   if(((mpData != 0) &&
       (mRows    >= 1) &&    // Y
       (mColumns >= 1) &&    // X
       (mDepth   >= 1)))      // Z
   {
      if (title != 0)
      {
         stream << title << std::endl;
      }

      stream << (*this);
      stream << std::endl;
   }

   return;
}

// Reading the same Basic Matrix from a stream
template <class ICP_MAT_TYPE>
std::istream &operator>>(std::istream &stream, ICP_Mat<ICP_MAT_TYPE> &ob)
{
   if(((ob.mpData != 0) &&
       (ob.mRows    >= 1) &&    // Y
       (ob.mColumns >= 1) &&    // X
       (ob.mDepth   >= 1)))      // Z
   {
      unsigned int rows, columns;
      stream >> rows >> std::ws >> columns >> std::ws;

      ob.Create(rows, columns);

      for(uint32_t d = 0; d < 1; ++d)
      {
         for(uint32_t y = 0; y < ob.mRows; ++y)
         {
            for(uint32_t x = 0; x < ob.mColumns; ++x)
            {
               stream >> ob.mpData[d*(ob.mRows * ob.mColumns) + y*ob.mColumns + x] >> std::ws;
            }
            stream >> std::ws;
         }
      }
   }

   return stream;
}


#else  //ARM Version

template <class ICP_MAT_TYPE>
void ICP_Mat<ICP_MAT_TYPE>::outputMatrix(const char* title)
{
   assert((mpData != 0) &&
          (mRows    >= 1) &&    // Y
          (mColumns >= 1) &&    // X
          (mDepth   >= 1));      // Z
   {
      if (title != 0)
      {
         printf("%s\n", title);
      }

      for(uint32_t d = 0; d < mDepth; ++d)
      {
         for(uint32_t y = 0; y < mRows; ++y)
         {
            for(uint32_t x = 0; x < mColumns; ++x)
            {
               printf("%16.7f\t", mpData[y*mColumns + x]);
            }
            printf("\n");
         }
      }
   }

   return;
}

#endif //Win32 / ARM

#endif /* ICPTYPES_H */
