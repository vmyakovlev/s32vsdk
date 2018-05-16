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

#ifndef ICPREMAPLUT_H
#define ICPREMAPLUT_H

#include "icp_types.h"
//#include "ICP_Image_Descriptor.hpp"
#include "icp_data.h"
#include "icp_datatype.h"
#include <global_errors.h>
#include "stdint.h"

typedef float FixedQ1616;
typedef ICP_Mat<FixedQ1616> MatQ1616;
const uint32_t IMAGE_WIDTH_FACTOR  = 16;     // remapped image width  must be a factor of 16
const uint32_t IMAGE_HEIGHT_FACTOR = 4;      // remapped image height must be a factor of 4

// Creating ICP_Remap_LUTs from a mapper

template <class MAPPER>
LIB_RESULT generateMapQ1616(MatQ1616& map, const MAPPER& mapper, const ICP_Rectangle& roi)
{
   LIB_RESULT result = LIB_FAILURE;

   if ( (map.ReturnWidth()) > 0 &&
		(map.ReturnHeight()) > 0 &&
		(roi.x >= 0) &&
		(roi.y >= 0) &&
		(((roi.x + roi.width)*2) <= (int)map.ReturnWidth()) &&
		((roi.y + roi.height) <= (int)map.ReturnHeight()) )
   {
      typename MAPPER::Point src, dst;

      uint32_t mapWidth = map.ReturnWidth();

      FixedQ1616* mapPtr = map.ReturnPtr() + (roi.y*mapWidth + roi.x*2);
      uint32_t lineStep = mapWidth - (roi.width*2);

      for (int y=0; y<roi.height; y++)
      {
         src.y = y;
         for (int x=0; x<roi.width; x++, mapPtr+=2)
         {
            src.x = x;
            mapper.map(src, dst);
            mapPtr[0] = dst.x;
            mapPtr[1] = dst.y;
         }
         mapPtr += lineStep;
      }
      result = LIB_SUCCESS;
   }

   return result;
}

template <class MAPPER>
LIB_RESULT generateMapQ1616(MatQ1616& map, const MAPPER& mapper)
{
	return generateMapQ1616(map, mapper, ICP_Rectangle(0, 0, map.ReturnWidth()>>1, map.ReturnHeight()));
}

template <class MAPPER>
LIB_RESULT createMapQ1616(MatQ1616& map, const MAPPER& mapper, const ICP_Size& dstImageSize)
{
   LIB_RESULT result = LIB_FAILURE;
   if (dstImageSize.width > 0 && dstImageSize.height > 0)
   {
      result = map.Create(dstImageSize.height, dstImageSize.width*2);
      if (result == LIB_SUCCESS)
      {
         result = generateMapQ1616(map, mapper);
      }
   }
   return result;
}

#endif   /* ICPREMAPLUT_H */
