/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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

#ifndef ICPFIXED_H
#define ICPFIXED_H

// Required for INT32_MAX (as per doc) [RVDS40]
#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <limits>
#include <stdint.h>

#ifdef _DEBUG
   #ifdef _WIN32
   #include <assert.h>
   #include <iostream>
   #define FIXED_ASSERT(X) assert(X)
   #else
      //#include "OAL_Exception.h"
      #include "Float.h"	// An integer representation of a float or double.  This is used to output results since, for some reason, the SDK/TRACE32 cannot output floating point numbers
      #define FIXED_ASSERT(X) LIB_ASSERT(X)
   #endif
#else
   #define FIXED_ASSERT(X)
#endif

#ifndef ENABLE_FIXED_POINT_ROUNDING
#define ENABLE_FIXED_POINT_ROUNDING    (1)
#endif

#ifndef ENABLE_FLOAT_DIVISION
#define ENABLE_FLOAT_DIVISION    (0)
#endif

#define ENABLE_FIXED_POINT_TRIG  1

#define SQRT_STEP                   \
    tmp >>= 2;                      \
    testVal = tmp + root;           \
    if(testVal <= value)            \
    {                               \
        value -= testVal;           \
        root = (root >> 1) | tmp;   \
    }                               \
    else                            \
    {                               \
        root >>= 1;                 \
    }

// Integer square root functions
int64_t sqrt64(int64_t value);
int32_t sqrt32(int32_t value);
int16_t sqrt16(int16_t value);

//namespace icp
//{

#if (1==ENABLE_FIXED_POINT_TRIG)
   const double PI_OVER_TWO = 1.5707963267948966192313216916398;
   const double TWO_PI      = 6.283185307179586476925286766559;
   const double PI			= 3.1415926535897932384626433832795;
   const int32_t MAX_LUT_Q = 10;       // Maximum fractional bits to be fully sampled by trigonometric LUTs
   const int32_t MAX_SIN_LUT_SIZE = int32_t((int64_t(PI_OVER_TWO * (int64_t(1) << (MAX_LUT_Q + 1)) ) + 1) >> 1) + 2;    // Maximum Sin LUT sample size, which depends on MAX_LUT_Q.  (e.g. 1610 for MAX_LUT_Q = 10)
   const int32_t MAX_ASIN_LUT_SIZE = (1 << MAX_LUT_Q) + 1;     // Maximum Arcsin LUT sample size, which depends on MAX_LUT_Q.  (e.g. 1025 for MAX_LUT_Q = 10)
#endif

inline void roundValue(int64_t & val)
{
   // A helper function used throughout class Fixed<Q> for rounding values
   if (val < 0)
   {
      // Rounding for negative values
      val = 1 - val;
      val >>= 1;
      val = -val;
   }
   else
   {
      // Rounding for positive values
      ++val;
      val >>= 1;
   }
}


template <int Q>
class ICP_Fixed
{
private:
   ICP_Fixed(int32_t fpVal, bool unused) : mVal(fpVal){}     // Only the private constructor allows direct fixed point initialization,
                                                           // All public constructors assume a regularly represented number is being passed.

   inline int32_t toInt() const
   {
      if (mVal < 0) return -((-mVal) >> Q);     // If the value is negative, it must be made positive before the bit shift, then negated again.
      else return mVal >> Q;                    // If the value is positive, just do a bit shift
   }

   int32_t mVal;     // The fixed point number

   // Useful private static constants
   static const int64_t HALF;
   static const int32_t ONE;
   static const int64_t TWO;
   static const int64_t INVERSE_FACTOR;
   static const int32_t Q_PLUS_ONE;

public:

static inline void toFixed(float realValue, int32_t& fpValue)
{
   int64_t tmp(int64_t(realValue*TWO));
   roundValue(tmp);
   fpValue = int32_t(tmp);
   FIXED_ASSERT(tmp == fpValue);     // overflow check.
}

static inline void toFixed(double realValue, int32_t& fpValue)
{
   int64_t tmp(int64_t(realValue*TWO));
   roundValue(tmp);
   fpValue = int32_t(tmp);
   FIXED_ASSERT(tmp == fpValue);     // overflow check.
}

static inline void toFixedClamped(float realValue, int32_t& fpValue)
{
   if      (realValue >= max) fpValue = std::numeric_limits<int32_t>::max(); //INT32_MAX;
   else if (realValue  < min) fpValue = std::numeric_limits<int32_t>::min(); //INT32_MIN;
   else
   {
      int64_t tmp(int64_t(realValue*TWO));
      roundValue(tmp);
      fpValue = int32_t(tmp);
   }
}

static inline void toFixedClamped(double realValue, int32_t& fpValue)
{
   if      (realValue >= max) fpValue = std::numeric_limits<int32_t>::max(); //INT32_MAX;
   else if (realValue  < min) fpValue = std::numeric_limits<int32_t>::min(); //INT32_MIN;
   else
   {
      int64_t tmp(int64_t(realValue*TWO));
      roundValue(tmp);
      fpValue = int32_t(tmp);
   }
}

   /***** Public constructors **********************************************************************/
   // All initializing values are assumed to be in normal numerical representation (i.e. not fixed point).
   
   ICP_Fixed(){}
   ICP_Fixed(int16_t value) : mVal(int32_t(value) << Q) {FIXED_ASSERT(value < max && value >= min);}      // Assertion is an overflow check.  Note, values must be strictly smaller than max, but can be larger or equal to min.
   ICP_Fixed(int32_t value) : mVal(        value  << Q) {FIXED_ASSERT(value < max && value >= min);}      // Assertion is an overflow check.  Note, values must be strictly smaller than max, but can be larger or equal to min.
   ICP_Fixed(int64_t value) : mVal(int32_t(value) << Q) {FIXED_ASSERT(value < max && value >= min);}      // Assertion is an overflow check.  Note, values must be strictly smaller than max, but can be larger or equal to min.
   
#if (ENABLE_FIXED_POINT_ROUNDING)

   ICP_Fixed(float value)
   {
      int64_t tmp(int64_t(value*TWO));
      roundValue(tmp);
      mVal = int32_t(tmp);
      FIXED_ASSERT(tmp == mVal);     // overflow check.
   }
   
   ICP_Fixed(const double& value)
   {
      int64_t tmp(int64_t(value*TWO));
      roundValue(tmp);
      mVal = int32_t(tmp);
      FIXED_ASSERT(tmp == mVal);     // Overflow check.
   }
#else
   ICP_Fixed(float value) : mVal(int32_t(value * ONE))
   {
      FIXED_ASSERT(mVal == int64_t(value * ONE));
   }
   ICP_Fixed(double value): mVal(int32_t(value * ONE))
   {
      FIXED_ASSERT(mVal == int64_t(value * ONE));
   }
#endif
   
   // Constructor from an integer division, with rounding:  mVal = numerator/denominator
   ICP_Fixed(int32_t numerator, int32_t denominator)         
   {
      FIXED_ASSERT(denominator != 0);
      
      int64_t tmp(numerator);
 #if (ENABLE_FIXED_POINT_ROUNDING)
      tmp <<= Q+1;
      tmp /= denominator;
      roundValue(tmp);
#else
      tmp <<= Q;
      tmp /= denominator;
#endif

      mVal = int32_t(tmp);
      
      FIXED_ASSERT(tmp == mVal);    // Overflow check.
   }

   // Constructor from another fixed point number with different precision
   template <int K>
   ICP_Fixed(const ICP_Fixed<K>& other) : mVal(other.toFixed())
   {
      ICP_Fixed::ShiftVal(Q, K); // This shifts the value of mVal
   }

   /***** Casting Operators ******************************************************************************/
   operator int8_t() const
   {
      FIXED_ASSERT ( toInt() <= INT8_MAX && toInt() >= INT8_MIN );       // Overflow check
      return toInt();
   }
   operator int16_t() const
   {
      FIXED_ASSERT ( toInt() <= INT16_MAX && toInt() >= INT16_MIN );     // Overflow check
      return toInt();
   }
   operator int32_t() const {return toInt();}            // no overlow check required
   operator int64_t() const {return toInt();}            // no overlow check required
   operator float()   const {return float (mVal)/ONE;}   // no overlow check required
   operator double()  const {return double(mVal)/ONE;}   // no overlow check required

   /***** Explicit Type Conversions ***********************************************************************/
   int32_t toFixed() const {return mVal;}
   int8_t toInt8()   const {return *this;}
   int16_t toInt16() const {return *this;}
   int32_t toInt32() const {return *this;}
   int64_t toInt64() const {return *this;}
   float toFloat()   const {return *this;}
   double toDouble() const {return *this;}

   /***** Bit Shift Operators ****************************************************************************/
   ICP_Fixed& operator>>=(int shift)
   {
      mVal >>= shift;
      return *this;
   }

   ICP_Fixed& operator<<=(int shift)
   {
      mVal <<= shift;
      return *this;
   }

   /***** Basic Arithmetic Operators *********************************************************************/
   ICP_Fixed& negate()         // a self-naget function
   {
      FIXED_ASSERT(mVal > std::numeric_limits<int32_t>::min());
      mVal = -mVal;
      return *this;
   }

   ICP_Fixed operator-() const
   {
      return ICP_Fixed(-mVal, true);    // true is a dummy value so that this private constructor can be distinguished between the public one.
   }

   ICP_Fixed& operator+=(const ICP_Fixed &rhs)
   {
      FIXED_ASSERT(int64_t(mVal) + int64_t(rhs.mVal) == int64_t(mVal + rhs.mVal));     // overflow check
      mVal += rhs.mVal;
      return *this;
   }

   ICP_Fixed& operator-=(const ICP_Fixed &rhs)
   {
      FIXED_ASSERT(int64_t(mVal) - int64_t(rhs.mVal) == int64_t(mVal - rhs.mVal));     // overflow check
      mVal -= rhs.mVal;
      return *this;
   }

   ICP_Fixed& operator*=(const ICP_Fixed &rhs)
   {
      int64_t tmp(mVal);
      tmp *= rhs.mVal;
#if (ENABLE_FIXED_POINT_ROUNDING)
      if (tmp < 0)
      {
         tmp = HALF - tmp;             // To round a negative value
         tmp >>= Q;
         tmp = -tmp;
      }
      else
      {
         tmp += HALF;                  // To round a positive value
         tmp >>= Q;
      }
#else
      tmp >>= Q;
#endif
      mVal = int32_t(tmp);
      FIXED_ASSERT(int64_t(mVal) == tmp);    // overflow check
      return *this;
   }

   inline ICP_Fixed& operator/=(const ICP_Fixed &rhs)
   {
      FIXED_ASSERT(rhs.mVal != 0);              // division by zero check
#if (ENABLE_FLOAT_DIVISION) 
      // Because int64 division is so slow, it's actually faster to convert to float, divide, then convert back to fixed point.
      float tmp(mVal);
      tmp /= rhs.mVal;
      mVal = int32_t(tmp * ONE);
#else
      int64_t tmp(mVal);
#if (ENABLE_FIXED_POINT_ROUNDING)
      tmp <<= Q_PLUS_ONE;                 // Bit shift by Q+1 because we will round the value, then shift down one.
      tmp /= rhs.mVal;
      roundValue(tmp);
#else
      tmp <<= Q;
      tmp /= rhs.mVal;
#endif
      mVal = int32_t(tmp);
      FIXED_ASSERT(int64_t(mVal) == tmp);       // overflow check
#endif   // ENABLE_FLOAT_DIVISION

      return *this;
   }

   ICP_Fixed& operator++()
   {
      FIXED_ASSERT(mVal <= std::numeric_limits<int32_t>::max() - ONE);    // overflow check
      mVal += ONE; 
      return *this;
   }

   ICP_Fixed operator++(int)
   {
      ICP_Fixed tmp(*this);
      operator++();
      return tmp;
   }

   ICP_Fixed& operator--()
   {
      FIXED_ASSERT(mVal >= std::numeric_limits<int32_t>::min() + ONE);     // overflow check
      mVal -= ONE; 
      return *this;
   }

   ICP_Fixed operator--(int)
   {
      ICP_Fixed tmp(*this);
      operator--();
      return tmp;
   }

   template <class TYPE>
   ICP_Fixed operator+(const TYPE &rhs) const
   { 
      ICP_Fixed tmp(*this); 
      tmp += rhs;  
      return tmp; 
   }

   template <class TYPE>
   ICP_Fixed operator-(const TYPE &rhs) const
   { 
      ICP_Fixed tmp(*this); 
      tmp -= rhs;  
      return tmp; 
   }

   template <class TYPE>
   ICP_Fixed operator*(const TYPE &rhs) const 
   { 
      ICP_Fixed tmp(*this); 
      tmp *= rhs;  
      return tmp; 
   }

   template <class TYPE>
   ICP_Fixed operator/(const TYPE &rhs) const
   { 
      ICP_Fixed tmp(*this); 
      tmp /= rhs;  
      return tmp; 
   }

   // Unary Arithmetic Functions
   ICP_Fixed& abs()      // self absolute value
   {
      FIXED_ASSERT(mVal > std::numeric_limits<int32_t>::min());     // Make sure this is not the smallest value possible, since it's negation is out of range
      if (mVal < 0) mVal = -mVal;
      return *this;
   }

   ICP_Fixed& pow2()
   {
      int64_t tmp(mVal);
      tmp *= tmp;

 #if (ENABLE_FIXED_POINT_ROUNDING)
      tmp += HALF;                  // To round a positive value
      tmp >>= Q;
#else
      tmp >>= Q;
#endif

      FIXED_ASSERT(int64_t(int32_t(tmp)) == tmp);        // overflow check
      mVal = int32_t(tmp);
      return *this;
   }

   ICP_Fixed& inv()      // self inverse
   {
      int64_t tmp(INVERSE_FACTOR / mVal);
 #if (ENABLE_FIXED_POINT_ROUNDING)
      roundValue(tmp);
#endif
      mVal = int32_t(tmp);
      FIXED_ASSERT(tmp == mVal);        // overflow check
      
      return *this;
   }

   // Iterative Aritmetic Functions
   ICP_Fixed& sqrt()     // self square root
   {
      FIXED_ASSERT(mVal >= 0);         // must be non-negative number!

      int64_t root = 0,
              tmp = 0x4000000000000000ll, //int64_t(1) << 62, //0x40000000l,
              testVal,
              value = int64_t(mVal)<<Q;

      FIXED_ASSERT(value >> Q == mVal);
    
      // bit 0
      if(tmp <= value)         
      {
         value -= tmp;
         root = tmp;
      }

      SQRT_STEP;      // bit 2
      SQRT_STEP;      // bit 4
      SQRT_STEP;      // bit 6
      SQRT_STEP;      // bit 8
      SQRT_STEP;      // bit 10
      SQRT_STEP;      // bit 12
      SQRT_STEP;      // bit 14
      SQRT_STEP;      // bit 16
      SQRT_STEP;      // bit 18
      SQRT_STEP;      // bit 20
      SQRT_STEP;      // bit 22
      SQRT_STEP;      // bit 24
      SQRT_STEP;      // bit 26
      SQRT_STEP;      // bit 28
      SQRT_STEP;      // bit 30

      SQRT_STEP;      // bit 32
      SQRT_STEP;      // bit 34
      SQRT_STEP;      // bit 36
      SQRT_STEP;      // bit 38
      SQRT_STEP;      // bit 40
      SQRT_STEP;      // bit 42
      SQRT_STEP;      // bit 44
      SQRT_STEP;      // bit 46
      SQRT_STEP;      // bit 48
      SQRT_STEP;      // bit 50
      SQRT_STEP;      // bit 52
      SQRT_STEP;      // bit 54
      SQRT_STEP;      // bit 56
      SQRT_STEP;      // bit 58
      SQRT_STEP;      // bit 60
      SQRT_STEP;      // bit 62

      // round to the nearest integer, cuts max error in half
      if(root < value)
      {
         ++root;
      }
      
      mVal = int32_t(root);
      FIXED_ASSERT(mVal == root);
      
      return *this;
   }

   ICP_Fixed& sqrt32()     // self square root
      {
         FIXED_ASSERT(mVal >= 0);         // must be non-negative number!

         int32_t root = 0,
                 tmp = 0x40000000l,
                 testVal,
                 value = mVal;

         FIXED_ASSERT(value >> Q == mVal);

         // bit 0
         if(tmp <= value)
         {
            value -= tmp;
            root = tmp;
         }

         SQRT_STEP;      // bit 2
         SQRT_STEP;      // bit 4
         SQRT_STEP;      // bit 6
         SQRT_STEP;      // bit 8
         SQRT_STEP;      // bit 10
         SQRT_STEP;      // bit 12
         SQRT_STEP;      // bit 14
         SQRT_STEP;      // bit 16
         SQRT_STEP;      // bit 18
         SQRT_STEP;      // bit 20
         SQRT_STEP;      // bit 22
         SQRT_STEP;      // bit 24
         SQRT_STEP;      // bit 26
         SQRT_STEP;      // bit 28
         SQRT_STEP;      // bit 30

         // round to the nearest integer, cuts max error in half
         if(root < value)
         {
            ++root;
         }

         root <<= (Q >> 1);

         mVal = root;

         return *this;
      }

   // Useful public static constants
   static const int32_t max;
   static const int32_t min;
   static const double resolution;

#if (1==ENABLE_FIXED_POINT_TRIG)

   //static void generateSineLUT();
   //static void generateArcSineLUT();

   ICP_Fixed& sin();
   ICP_Fixed& asin();

   ICP_Fixed& cos()
   {
      FIXED_ASSERT(mVal <= std::numeric_limits<int32_t>::max() - pi_over_two);    // overflow check
      mVal += pi_over_two;

      return sin();
   }

   ICP_Fixed& acos()
   {
      asin();
      FIXED_ASSERT(pi_over_two <= std::numeric_limits<int32_t>::max() - mVal);    // overflow check
      mVal = pi_over_two - mVal;

      return *this;
   }

   // Trigonometric Static Constants
public:
   static const int32_t pi;                  // Fixed point representative of pi
   static const int32_t pi_over_two;         // Fixed point representative of pi/2
   static const int32_t two_pi;              // Fixed point representative of 2*pi
   static const int32_t lut_step;            // The number of fixed point numbers for every LUT element
   static const int32_t sin_lut_size;        // The size of the Sin LUT
   static const int32_t asin_lut_size;       // The size of the Arcsin LUT

   static const int32_t sin_lut[];
   static const int32_t asin_lut[];

#endif

   // Comparison Operators
   bool operator== (const ICP_Fixed& other) const
   {
      return mVal == other.mVal;
   }

   bool operator!= (const ICP_Fixed& other) const
   {
      return mVal != other.mVal;
   }

   bool operator> (const ICP_Fixed& other) const
   {
      return mVal > other.mVal;
   }

   bool operator< (const ICP_Fixed& other) const
   {
      return mVal < other.mVal;
   }

   bool operator>= (const ICP_Fixed& other) const
   {
      return mVal >= other.mVal;
   }

   bool operator<= (const ICP_Fixed& other) const
   {
      return mVal <= other.mVal;
   }

private:
   void ShiftVal(const int cQ, const int cK)
   {
      if (cQ>cK)
      {
         FIXED_ASSERT(int64_t(mVal) << (cQ-cK) == mVal << (cQ-cK));      // Going to less integral bits, so check for overflow.
         mVal <<= (cQ-cK);
      }
      else
      {
         mVal >>= (cK-cQ);                                    // Going to more integral bits, no check overflow check necessary.
      }
   }
};

// Private Static Constants
template <int Q> const int32_t ICP_Fixed<Q>::Q_PLUS_ONE = Q+1;                          // Used for rounding division results
template <int Q> const int32_t ICP_Fixed<Q>::ONE = int32_t(1) << Q;                     // Used for converting back to floating point.
template <int Q> const int64_t ICP_Fixed<Q>::HALF = int64_t(1) << (Q-1);                // Used for rounding multiplication results.
template <int Q> const int64_t ICP_Fixed<Q>::TWO = int64_t(1) << (Q+1);                 // Used for rounding floating point numbers to the nearest fixed-point value
#if (ENABLE_FIXED_POINT_ROUNDING)
template <int Q> const int64_t ICP_Fixed<Q>::INVERSE_FACTOR = int64_t(1) << (Q+Q+1);    // Used in the inverse function: inv()
#else
template <int Q> const int64_t ICP_Fixed<Q>::INVERSE_FACTOR = int64_t(1) << (Q+Q);      // Used in the inverse function: inv()
#endif

// Public Static Constants
template <int Q> const int32_t ICP_Fixed<Q>::max = (std::numeric_limits<int32_t>::max() >> Q) + 1;                // The maximum number that can be represented (values are strictly less than max)
template <int Q> const int32_t ICP_Fixed<Q>::min = (std::numeric_limits<int32_t>::min() >> Q);                      // The minimum number than can be represented (values are greater or equal to min)
template <int Q> const double ICP_Fixed<Q>::resolution = double(1)/ONE;                 // Thh resolution or 'granularity' of the fixed-point representation.

// Basic Arithmetic Operators
template <int Q> inline ICP_Fixed<Q> operator +(int8_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp += b);}
template <int Q> inline ICP_Fixed<Q> operator +(int16_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp += b);}
template <int Q> inline ICP_Fixed<Q> operator +(int32_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp += b);}
template <int Q> inline ICP_Fixed<Q> operator +(int64_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp += b);}
template <int Q> inline ICP_Fixed<Q> operator +(float a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp += b);}
template <int Q> inline ICP_Fixed<Q> operator +(double a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp += b);}

template <int Q> inline ICP_Fixed<Q> operator -(int8_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp -= b);}
template <int Q> inline ICP_Fixed<Q> operator -(int16_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp -= b);}
template <int Q> inline ICP_Fixed<Q> operator -(int32_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp -= b);}
template <int Q> inline ICP_Fixed<Q> operator -(int64_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp -= b);}
template <int Q> inline ICP_Fixed<Q> operator -(float a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp -= b);}
template <int Q> inline ICP_Fixed<Q> operator -(double a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp -= b);}

template <int Q> inline ICP_Fixed<Q> operator *(int8_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp *= b);}
template <int Q> inline ICP_Fixed<Q> operator *(int16_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp *= b);}
template <int Q> inline ICP_Fixed<Q> operator *(int32_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp *= b);}
template <int Q> inline ICP_Fixed<Q> operator *(int64_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp *= b);}
template <int Q> inline ICP_Fixed<Q> operator *(float a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp *= b);}
template <int Q> inline ICP_Fixed<Q> operator *(double a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp *= b);}


template <int Q> inline ICP_Fixed<Q> operator /(int8_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp /= b);}
template <int Q> inline ICP_Fixed<Q> operator /(int16_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp /= b);}
template <int Q> inline ICP_Fixed<Q> operator /(int32_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp /= b);}
template <int Q> inline ICP_Fixed<Q> operator /(int64_t a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp /= b);}
template <int Q> inline ICP_Fixed<Q> operator /(float a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp /= b);}
template <int Q> inline ICP_Fixed<Q> operator /(double a, const ICP_Fixed<Q> & b) {ICP_Fixed<Q> tmp(a); return (tmp /= b);}

// Unary Arithmetic Functions
template <int Q>
ICP_Fixed<Q> abs(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.abs());
}

template <int Q>
ICP_Fixed<Q> pow2(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.pow2());
}

template <int Q>
ICP_Fixed<Q> inv(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.inv());
}

#if (1==ENABLE_FIXED_POINT_TRIG)
// Private Static Constants related to trigonometric LUTs
template <int Q> const int32_t ICP_Fixed<Q>::pi =          int32_t((int64_t(PI          * TWO) + 1) >> 1);
template <int Q> const int32_t ICP_Fixed<Q>::pi_over_two = int32_t((int64_t(PI_OVER_TWO * TWO) + 1) >> 1);
template <int Q> const int32_t ICP_Fixed<Q>::two_pi =      int32_t((int64_t(TWO_PI      * TWO) + 1) >> 1);

template <int Q>
ICP_Fixed<Q>& ICP_Fixed<Q>::sin()
{
   // ***** 1.  Relate all angles to the domain [0,PI_OVER_TWO] ***********************************

   bool signChange;

   // Make angle positive
   if (mVal < 0)
   {
      mVal =-mVal;
      signChange = true;      // Indicate a sign change since the sine function is antisymmetric
   }
   else signChange = false;

   // Get the angle within 0 and 2PI
   if (mVal >= two_pi) mVal %= two_pi;

   // Relate all angles to the values within 0 and PI/2
   if (mVal > pi)
   {
      mVal -= pi;
      signChange = !signChange;                   // switch signs
   }

   if (mVal > pi_over_two) mVal = pi - mVal;


   // ***** 2. Get the result from the LUT with interpolation, if necessary. ******************************

   if (Q <= MAX_LUT_Q)
   {
	   // The sine table is exhaustive, so there is no need to interpolate
	   mVal = signChange ? -sin_lut[mVal] : sin_lut[mVal];
   }
   else
   {
	   // Do linear interpolation of table values
	   const int32_t interpolShift= Q - MAX_LUT_Q;		// bit shift used for interpolation
	   int32_t lutIndex (mVal >> interpolShift ),
			   remainder(mVal & (lut_step-1));

	   FIXED_ASSERT(lutIndex >=0 && lutIndex < sin_lut_size);    // make sure we are within bounds of the table array size.

	   if (remainder == 0)
	   {
		  // There is no remainder, so there is no need to interpolate
		  mVal = signChange ? -sin_lut[lutIndex] : sin_lut[lutIndex];
	   }
	   else
	   {
		  //There is a remainder, so we interpolate
		  FIXED_ASSERT(lutIndex >=0 && lutIndex < sin_lut_size-1);       // make sure we are within bounds of the table array size.
																   // We use an upper bound of sin_lut_size-1 because we must have a second value available for interpolation

		  int64_t w1(lut_step - remainder),                        // weight of the first value
				   w2(remainder),                                  // weight of the second value
				   value1(sin_lut[lutIndex]),                      // the first value
				   value2(sin_lut[++lutIndex]);                    // the second value  (and the reason why we must have lutIndex < sin_lut_size -1)

		  int64_t result = w1 * value1 + w2 * value2;              // the interpolated value 'result' lies somewhere between value1 and value2
		  result >>= interpolShift-1;							   // Bit shift for rounding
		  ++result;                                                // Add 1 for rounding
		  result >>= 1;                                            // Shift back to obtain the rounded value

		  FIXED_ASSERT(result == int32_t(result));                       // overflow check (likely redundant)
		  mVal = signChange ? -int32_t(result) : int32_t(result);
	   }
   }

   return *this;
}

template <int Q>
ICP_Fixed<Q>& ICP_Fixed<Q>::asin()
{
   // ***** 1.  Relate values to the domain [0,1] ***********************************

   bool signChange;

   // Make value positive
   if (mVal < 0)
   {
      mVal =-mVal;
      signChange = true;      // Indicate a sign change since the arcsine function is antisymmetric
   }
   else signChange = false;

   if (mVal <= ONE)     // arcsin only valid for [-1, 1]
   {
      // ***** 2. Get the result from the LUT with interpolation, if necessary. ******************************

      if (Q <= MAX_LUT_Q)
      {
   	   // The sine table is exhaustive, so there is no need to interpolate
   	   mVal = signChange ? -asin_lut[mVal] : asin_lut[mVal];
      }
      else
      {
   	   // Do linear interpolation of table values
   	   const int32_t interpolShift= Q - MAX_LUT_Q;		// bit shift used for interpolation
   	   int32_t lutIndex (mVal >> interpolShift ),
   			   remainder(mVal & (lut_step-1));

   	   FIXED_ASSERT(lutIndex >=0 && lutIndex < asin_lut_size);    // make sure we are within bounds of the table array size.

   	   if (remainder == 0)
   	   {
   		  // There is no remainder, so there is no need to interpolate
   		  mVal = signChange ? -asin_lut[lutIndex] : asin_lut[lutIndex];
   	   }
   	   else
   	   {
   		  //There is a remainder, so we interpolate
   		  FIXED_ASSERT(lutIndex >=0 && lutIndex < asin_lut_size-1);       // make sure we are within bounds of the table array size.
   																   // We use an upper bound of sin_lut_size-1 because we must have a second value available for interpolation

   		  int64_t w1(lut_step - remainder),                        // weight of the first value
   				   w2(remainder),                                  // weight of the second value
   				   value1(asin_lut[lutIndex]),                      // the first value
   				   value2(asin_lut[++lutIndex]);                    // the second value  (and the reason why we must have lutIndex < sin_lut_size -1)

   		  int64_t result = w1 * value1 + w2 * value2;              // the interpolated value 'result' lies somewhere between value1 and value2
   		  result >>= interpolShift-1;							   // Bit shift for rounding
   		  ++result;                                                // Add 1 for rounding
   		  result >>= 1;                                            // Shift back to obtain the rounded value

   		  FIXED_ASSERT(result == int32_t(result));                       // overflow check (likely redundant)
   		  mVal = signChange ? -int32_t(result) : int32_t(result);
   	   }
      }
   }
   else
   {
      FIXED_ASSERT(false);       // arcsin only valid for [-1, 1]
   }

   return *this;
}



#endif

//}     // namespace icp


// Specialized Arithmetic Functions
template <int Q>
ICP_Fixed<Q> sqrt(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.sqrt());
}

#ifdef _DEBUG
// Outputing the value
template <int Q>
std::ostream& operator<<(std::ostream& out, const ICP_Fixed<Q> & value)
{
   // I use the custom type Float because SDK cannot properly output floats and doubles.  I have no idea why this is.
#ifdef _WIN32
	out << value.toDouble();
#else
	out << Float(value.toDouble());		// convert to custom type Float because target/TRACE32 cannot properly display floating point numbers
#endif
   return out;
}
#endif

#if (1==ENABLE_FIXED_POINT_TRIG)
// Trigonometric Functions
template <int Q>
ICP_Fixed<Q> sin(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.sin());
}

template <int Q>
ICP_Fixed<Q> cos(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.cos());
}

template <int Q>
ICP_Fixed<Q> asin(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.asin());
}

template <int Q>
ICP_Fixed<Q> acos(const ICP_Fixed<Q> & val)
{
    ICP_Fixed<Q> tmp(val);
    return (tmp.acos());
}

#endif   // (1==ENABLE_FIXED_POINT_TRIG)

#endif   /* ICPFIXED_H */

