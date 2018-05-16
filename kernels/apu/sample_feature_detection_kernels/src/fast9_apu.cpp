
/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* Copyright (c) 2017 NXP
*  All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
/**
* \file fast9_apu.cpp
* \brief FAST 9 corner detection implementation for APEX
* \author Igor Aleksandrowicz
* \version
* \date
****************************************************************************/

#ifdef ACF_KERNEL_IMPLEMENTATION

#include "fast9_apu.h"

#ifdef APEX2_EMULATE
#include "apu_vec.hpp"
#endif

/*****************************************************************************
* constants
*****************************************************************************/

//corner score value of non-corners
static const int16s cNonCornerScore = -0x7fff;

//offsets of the 16-pixel ring around the pixel used for corner detection
static const int   cCircleOffsets [16][2] =
{
  {0,  3}, { 1,  3}, { 2,  2}, { 3,  1}, { 3, 0}, { 3, -1}, { 2, -2}, { 1, -3},
  {0, -3}, {-1, -3}, {-2, -2}, {-3, -1}, {-3, 0}, {-3,  1}, {-2,  2}, {-1,  3}
};

/*****************************************************************************
* local functions
*****************************************************************************/

/****************************************************************************/
/** Gets pixel by index to the pixel ring.
*
* \param apcSrc pointer to the source pixel(s)
* \param aSourceStride line stride of the source data
* \param aCircleIndex index into the ring 0 <= aCircleIndexx <= 16
*
* \return vector containing requested pixels
*
*****************************************************************************/
static inline vec08u getPixelFromCircle(
  const vec08u* apcSrc, int   aSourceStride, int   aCircleIndex)
{ 
  return apcSrc[
    (cCircleOffsets[aCircleIndex][0]) +
    (cCircleOffsets[aCircleIndex][1] * aSourceStride)];
}
/****************************************************************************/
/** Classifies a source pixel in relation to the reference pixel.
*
* pixel classes:
*               0 - similar
*               1 - darker
*               2 - brighter
*
* \param acSource pointer to the source pixel(s)
* \param acReference pointer to the reference pixel(s)
* \param aThreshold threshold used for classification
*
* \return vector containing pixel classifications
*
*****************************************************************************/
static inline vec08u classifyPixel(
  const vec08u acSource, const vec08u acReference, int aThreshold)
{
  vec08u result = 0;
  
  vec16s dif = ((vec16s)acReference) - ((vec16s)acSource);
  
  //darker
  vif (dif < -aThreshold)
  {
    result = 2;
  }
  vendif 
  vif (dif > aThreshold)
  {
      result = 1;

  }
  vendif;
  
  return result;
}

/****************************************************************************/
/** Classifies a source pixel in relation to the reference pixel.
*
* pixel classes:
*               0 - similar
*               1 - darker
*               2 - brighter
*
* \param acSource pointer to the source pixel(s)
* \param acReference pointer to the reference pixel(s)
* \param aThreshold threshold used for classification
*
* \return vector containing pixel classifications
*
*****************************************************************************/
static inline vec08u classifyPixelDif(
  const vec08u acSource, const vec08u acReference, int aThreshold, vec16s *difs)
{
  vec08u result = 0;
  vec16s dif;
  dif = ((vec16s)acReference) - ((vec16s)acSource);
  
  //darker
  vif (dif < -aThreshold)
  {
    result = 2;
  }
  vendif
  vif (dif > aThreshold)
  {
       result = 1;
  }
  vendif
  *difs = dif;
  return result;
}

/****************************************************************************/
/** Performs corner ring checks on the source data.
*
* \param apcSrc pointer to the source pixel(s)
* \param aSourceStride line stride of the source data
* \param aThreshold threshold used for classification
*
* \return vector boolean containing test results (false - not a corner,
*         true - is a corner)
*
*****************************************************************************/
  static inline vbool checkCircle(
  const vec08u* apcSrc, int aSourceStride, int aThreshold)
{
  vec08u v = apcSrc[0];
  
  vec08u classifiedCirclePixels[16];
  
  vec08u check = 0xff;
  
  //get pixels used in the first phase
  for (int i = 0; i < 4; ++i)
  {
    int circleIndex0 = i * 2;
    int circleIndex1 = circleIndex0 + 8;
    
    vec08u circlePixel0 = getPixelFromCircle(
      apcSrc, aSourceStride, circleIndex0);
    vec08u circlePixel1 = getPixelFromCircle(
      apcSrc, aSourceStride, circleIndex1);
      
    vec08u classifiedCirclePixel0 = classifyPixel(
      v, circlePixel0, aThreshold);
    vec08u classifiedCirclePixel1 = classifyPixel(
      v, circlePixel1, aThreshold);
      
    classifiedCirclePixels[circleIndex0] = classifiedCirclePixel0;
    classifiedCirclePixels[circleIndex1] = classifiedCirclePixel1;
    
    check &= classifiedCirclePixel0 | classifiedCirclePixel1;
  }
                  
  //early exit 1
  if (vall(check == 0))
  {
    return 0;
  }
  
  //second phase
  //get pixels used in the second phase
  for (int i = 0; i < 4; ++i)
  {
    int circleIndex0 = i * 2 + 1;
    int circleIndex1 = circleIndex0 + 8;
    
    vec08u circlePixel0 = getPixelFromCircle(
      apcSrc, aSourceStride, circleIndex0);
    vec08u circlePixel1 = getPixelFromCircle(
      apcSrc, aSourceStride, circleIndex1);
      
    vec08u classifiedCirclePixel0 = classifyPixel(
      v, circlePixel0, aThreshold);
    vec08u classifiedCirclePixel1 = classifyPixel(
      v, circlePixel1, aThreshold);
      
    classifiedCirclePixels[circleIndex0] = classifiedCirclePixel0;
    classifiedCirclePixels[circleIndex1] = classifiedCirclePixel1;
    
    check &= classifiedCirclePixel0 | classifiedCirclePixel1;
  }
  
  //early exit 2
  if (vall(check == 0))
  {
    return 0;
  }
                  
  //check for 9 darker/brighter pixels ring
  vec08u countDarker = 0;
  vec08u countBrighter = 0;
  //25 ... we can find the first sequence pixel at 15th element
  for (int i = 0; i < 25; ++i)
  {
    //circle pixels darker than v
      vif (classifiedCirclePixels[i & 0xf] == 1)
      {
        ++countDarker;
      }
      velse
      {
        //null the count only if < 9
        vif (countDarker < 9)
        {
          countDarker = 0;
        }
        vendif
      }
      vendif

      vif (classifiedCirclePixels[i & 0xf] == 2)
      {
        ++countBrighter;
      }
      velse
      {
        //null the count only if < 9
        vif (countBrighter < 9)
        {
          countBrighter = 0;
        }
        vendif
      }
      vendif
  }


  
  //collect the results - 1 if the ring was found
  vbool result = ((countBrighter >= 9) || (countDarker>=9));

  return result;
}


/****************************************************************************/
/** Vector minimum.
*****************************************************************************/
static inline vec16s vMin(vec16s aA, vec16s aB)
{
  vec16s ret;
  
  vif (aA < aB)
  {
    ret = aA;
  }
  vendif
  vif (aA >= aB)
  {
    ret = aB;
  }
  vendif
  
  return ret;
}

/****************************************************************************/
/** Vector maximum.
*****************************************************************************/
static inline vec16s vMax(vec16s aA, vec16s aB)
{
  vec16s ret;
  
  vif (aA > aB)
  {
    ret = aA;
  }
  vendif
  vif (aA <= aB)
  {
    ret = aB;
  }
  vendif
  
  return ret;
}

/*****************************************************************************
* functions
*****************************************************************************/


/****************************************************************************/
/** Performs corner ring checks on the source data. Outputs the vector scores.
*
* \param apcSrc pointer to the source pixel(s)
* \param aSourceStride line stride of the source data
* \param aThreshold threshold used for classification
*
* \return vector containing result corner scores
*         (cNonCornerScore - not a corner)
*
*****************************************************************************/
static vec16s checkCircleWithCornerScores(
  const vec08u* apcSrc, int aSourceStride, int aThreshold)
{
  vec08u v = apcSrc[0];
  
  vec08u classifiedCirclePixels[16];
  
  vec08u check = 0xff;
  vec16s difs[16];
  //get pixels used in the first phase
  for (int i = 0; i < 4; ++i)
  {
    int circleIndex0 = i * 2;
    int circleIndex1 = circleIndex0 + 8;
    
    vec08u circlePixel0 = getPixelFromCircle(apcSrc, aSourceStride, circleIndex0);
      
    vec08u circlePixel1 = getPixelFromCircle(apcSrc, aSourceStride, circleIndex1);
      
    vec08u classifiedCirclePixel0 = classifyPixelDif( v, circlePixel0, aThreshold, &difs[circleIndex0] );
    vec08u classifiedCirclePixel1 = classifyPixelDif( v, circlePixel1, aThreshold, &difs[circleIndex1]);
      
    classifiedCirclePixels[circleIndex0] = classifiedCirclePixel0;
    classifiedCirclePixels[circleIndex1] = classifiedCirclePixel1;
    
   check &= classifiedCirclePixel0 | classifiedCirclePixel1;
  }
                  
  //early exit 1
  if (vall(check == 0))
  {
    return cNonCornerScore;
  }
  
  //second phase
  //get pixels used in the second phase
  for (int i = 0; i < 4; ++i)
  {
    int circleIndex0 = i * 2 + 1;
    int circleIndex1 = circleIndex0 + 8;
    
    vec08u circlePixel0 = getPixelFromCircle( apcSrc, aSourceStride, circleIndex0);
    vec08u circlePixel1 = getPixelFromCircle(apcSrc, aSourceStride, circleIndex1);
      
    vec08u classifiedCirclePixel0 = classifyPixelDif( v, circlePixel0, aThreshold, &difs[circleIndex0]);
    vec08u classifiedCirclePixel1 = classifyPixelDif( v, circlePixel1, aThreshold, &difs[circleIndex1]);
      
    classifiedCirclePixels[circleIndex0] = classifiedCirclePixel0;
    classifiedCirclePixels[circleIndex1] = classifiedCirclePixel1;
    
    check &= classifiedCirclePixel0 | classifiedCirclePixel1;
  }
  
  //early exit 2
  if (vall(check == 0))
  {
    return cNonCornerScore;
  }
                  
  //check for 9 darker/brighter pixels ring
  vec08u countDarker = 0;
  vec08u countBrighter = 0;
  //25 ... we can find the first sequence pixel at 15th element
  for (int i = 0; i < 25; ++i)
  {
    //circle pixels darker than v
    vif (classifiedCirclePixels[i & 0xf] == 1)
    {
      ++countDarker;
    }
    velse
    {
      //null the count only if < 9
      vif (countDarker < 9)
      {
        countDarker = 0;
      }
      vendif
    }
    vendif

    vif (classifiedCirclePixels[i & 0xf] == 2)
    {
      ++countBrighter;
    }
    velse
    {
      //null the count only if < 9
      vif (countBrighter < 9)
      {
        countBrighter = 0;
      }
      vendif
    }
    vendif
  }
  
  //collect the results 
  vbool corners = ((countBrighter >= 9) || (countDarker>=9));

  if (vany(corners))
  {  
    vec16s a0 = aThreshold;
    for (int k = 0; k < 16; k += 2)
    {
      vec16s a = vMin(difs[(k+1)&0xf], difs[(k+2)&0xf]);
      a = vMin(a, difs[(k+3)&0xf]);
      //TODO: early continue?
      vbool aa0 = a > a0;
      if (vall((!aa0) || (!corners))) continue;
      //we still need the vif - affects a0
      vif( aa0 )
      {
        a = vMin(a, difs[(k+4)&0xf]);
        a = vMin(a, difs[(k+5)&0xf]);
        a = vMin(a, difs[(k+6)&0xf]);
        a = vMin(a, difs[(k+7)&0xf]);
        a = vMin(a, difs[(k+8)&0xf]);
        a0 = vMax(a0, vMin(a, difs[k]));
        a0 = vMax(a0, vMin(a, difs[(k+9)&0xf]));
      }
      vendif
    }

    vec16s b0 = -a0;
    for (int k = 0; k < 16; k += 2)
    {
      //TODO: and instead of modulo?
      vec16s b = vMax(difs[(k+1)&0xf], difs[(k+2)&0xf]);
      b = vMax(b, difs[(k+3)&0xf]);
      b = vMax(b, difs[(k+4)&0xf]);
      b = vMax(b, difs[(k+5)&0xf]);
      vbool bb0 = b < b0;
      if (vall((!bb0) || (!corners))) continue;
      //TODO: early continue?
      //we still need the vif - affects b0
      vif( bb0 )
      {
        b = vMax(b, difs[(k+6)&0xf]);
        b = vMax(b, difs[(k+7)&0xf]);
        b = vMax(b, difs[(k+8)&0xf]);
        b0 = vMin(b0, vMax(b, difs[k]));
        b0 = vMin(b0, vMax(b, difs[(k+9)&0xf]));
      }
      vendif;
    }
    
    vec16s scores = -b0 - 1;
	
    //don't include noncorners
    vif (!corners)
    {
      scores = cNonCornerScore;
    }
    vendif;
    
    return scores;
  }
  else
  {
    return cNonCornerScore;
  }

}

void apu_fast9_unsuppressed_score(
      const vec08u* apcSrc, vec08u* apDst,
      int   aSourceStride, int aDestinationStride,
      int   aBlockWidth, int aBlockHeight,
      int08u aThreshold)
{
  int08u   CornersFound = 0;
  for (int y = 0; y < aBlockHeight; ++y)
  {
    for (int x = 0; x < aBlockWidth; ++x)
    {
      vec16s cornerScore = checkCircleWithCornerScores(&apcSrc[x], aSourceStride, aThreshold);
      vec08u vtmp = 0;  
      vif( cornerScore > cNonCornerScore)
      {
           vtmp = (vec08u)cornerScore;
      }         
      vendif;

      apDst[x] = vtmp;
    }
    apcSrc += aSourceStride;
    apDst += aDestinationStride;
  }
}

#endif //#ifdef ACF_KERNEL_IMPLEMENTATION
