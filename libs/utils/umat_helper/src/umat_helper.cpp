/*****************************************************************************
*
* NXP Confidential Proprietary
*
* Copyright (c) 2017 NXP Semiconductor;
* All Rights Reserved
*
*****************************************************************************
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

#include <umat.hpp>
#include <umat_helper.hpp>
#include <icp_datatype.h>
#include <string.h> //memset

//-----------------------------------------------------------------------------------------------------------
//function for calculating buffer size information (i.e. buffer with space around it for padding or overflow)
//-----------------------------------------------------------------------------------------------------------
void CalcBufferInfo(int32_t lWidth,
                    int32_t lHeight,
                    icp::DATATYPE lElementDataType,
                    int32_t lElementDimX,
                    int32_t lElementDimY,
                    int32_t lPadL,
                    int32_t lPadR,
                    int32_t lPadT,
                    int32_t lPadB,
                    int32_t lMinAlignmentInBytes,
                    int32_t lWorstCaseOverflowRightInBytes,
                    int32_t lWorstCaseOverflowBottomInElements,
                    int32_t& lBufferWidthInElem,
                    int32_t& lBufferHeightInElem,
                    int32_t& lBufferSizeInBytes,
                    int32_t& lBufferSpan,
                    int32_t& lBufferCoreDataOffsetInBytes,
                    int32_t& lBufferCoreDataOffsetXInElem,
                    int32_t& lBufferCoreDataOffsetYInElem)
{
   //IO buffers will be allocated so they can satisfy the specified requirements (e.g. alignment, overflow space on
   //right and bottom, etc.)

   //Notes:
   //- span must always be divisible by lElementWidthInBytes (i.e. user must be be able to navigate the buffer
   //  via element and stride)
   //- there are two important alignment points: the start of buffer and the start of the core data region (and all subsequent
   //  lines assuming a 2D structure, so the span must also respect minimum alignment)

   int32_t lElementWidthInBytes = RetDataTypeSizeInBytes(lElementDataType)*lElementDimX;

   //ensure that the user-specified left padding region width is itself divisible by both lElementWidthInBytes and MIN_ALIGNMENT_IN_BYTES
   //so it does not misalign the start of the core data region
   int32_t lBufferPadLeftInBytes = lPadL*lElementWidthInBytes; //minimum number of bytes required to satisfy user-defined left padding
   while (lBufferPadLeftInBytes%lMinAlignmentInBytes != 0)
      lBufferPadLeftInBytes += lElementWidthInBytes;

   int32_t lCoreDataWidthInBytes          = lWidth*lElementWidthInBytes;
   int32_t lBufferOverflowPadRightInBytes = (((lWorstCaseOverflowRightInBytes-1)+(lElementWidthInBytes-1))/lElementWidthInBytes)*lElementWidthInBytes;
   int32_t lBufferPadRightInBytes         = lPadR*lElementWidthInBytes; //minimum number of bytes required to satisfy user-defined right padding
   lBufferPadRightInBytes += lBufferOverflowPadRightInBytes;
   while ((lCoreDataWidthInBytes + lBufferPadRightInBytes)%lMinAlignmentInBytes != 0)
      lBufferPadRightInBytes += lElementWidthInBytes;

   int32_t lCoreDataHeightInLines = lHeight*lElementDimY;
   int32_t lPadTopInLines         = lPadT*lElementDimY;
   int32_t lPadBotInLines         = lPadB*lElementDimY;
   int32_t lBufferOverflowPadBotInLines = (lWorstCaseOverflowBottomInElements-1)*lElementDimY;

   int32_t lBufferHeightInLines = lPadTopInLines + lCoreDataHeightInLines + lPadBotInLines + lBufferOverflowPadBotInLines;

   lBufferWidthInElem  = (lBufferPadLeftInBytes/lElementWidthInBytes) + lWidth + (lBufferPadRightInBytes/lElementWidthInBytes);
   lBufferHeightInElem = (lBufferHeightInLines/lElementDimY);
   lBufferSpan         = lBufferPadLeftInBytes + lCoreDataWidthInBytes + lBufferPadRightInBytes;
   lBufferSizeInBytes  = lBufferSpan*lBufferHeightInLines;

   lBufferCoreDataOffsetInBytes = lPadTopInLines*lBufferSpan + lBufferPadLeftInBytes;
   lBufferCoreDataOffsetXInElem = (lBufferPadLeftInBytes/lElementWidthInBytes);
   lBufferCoreDataOffsetYInElem = lPadTopInLines;
}

vsdk::UMat CreateAcfFriendlyUMat(int32_t lRows, int32_t lCols, int32_t lType,
                                 int32_t lPadL, int32_t lPadR, int32_t lPadT, int32_t lPadB)
{
   //values for these parameters are selected based on the needs of ACF/APEX
   const int32_t lMinAlignmentInBytes               = 8;
   const int32_t lWorstCaseOverflowRightInBytes     = 128; //currently based on ACF worst case chunk width in bytes
   const int32_t lWorstCaseOverflowBottomInElements = 32;  //currently based on ACF worst case chunk height in elements

   int32_t lBufferWidthInElem           = 0;
   int32_t lBufferHeightInElem          = 0;
   int32_t lBufferSizeInBytes           = 0;
   int32_t lBufferSpan                  = 0;
   int32_t lBufferCoreDataOffsetInBytes = 0;
   int32_t lBufferCoreDataOffsetXInElem = 0;
   int32_t lBufferCoreDataOffsetYInElem = 0;

   //convert from 'cv' datatype to 'icp' datatype
   int32_t lCvDataType = lType & 0x00000007; //TODO: is there a macro for this?
   icp::DATATYPE lIcpDataType = icp::DATATYPE_08U;
   switch (lCvDataType)
   {
    case VSDK_CV_8U:  lIcpDataType = icp::DATATYPE_08U; break;
    case VSDK_CV_8S:  lIcpDataType = icp::DATATYPE_08S; break;
    case VSDK_CV_16U: lIcpDataType = icp::DATATYPE_16U; break;
    case VSDK_CV_16S: lIcpDataType = icp::DATATYPE_16S; break;
    case VSDK_CV_32S: lIcpDataType = icp::DATATYPE_32S; break;
    default:          lIcpDataType = icp::DATATYPE_32U; break;
   }

   int32_t lElementDimX = VSDK_CV_ELEM_SIZE(lType);
   int32_t lElementDimY = 1; //uMat offers no support for specifying y dimension of element

   CalcBufferInfo(lCols, lRows, lIcpDataType, lElementDimX, lElementDimY, lPadL, lPadR, lPadT, lPadB,
                  lMinAlignmentInBytes, lWorstCaseOverflowRightInBytes, lWorstCaseOverflowBottomInElements,
                  lBufferWidthInElem, lBufferHeightInElem, lBufferSizeInBytes, lBufferSpan, lBufferCoreDataOffsetInBytes, lBufferCoreDataOffsetXInElem, lBufferCoreDataOffsetYInElem);

   //create 'base' UMat from which the ROI will be derived (it is assumed this allocation is at least 8-byte aligned!)
   vsdk::UMat lBaseUMat = vsdk::UMat(lBufferHeightInElem, lBufferWidthInElem, lType);

   //create 'ROI' UMat on top of 'base' UMat
   vsdk::UMat lRoiUMat = vsdk::UMat(lBaseUMat, vsdk::Rect(lBufferCoreDataOffsetXInElem, lBufferCoreDataOffsetYInElem, lCols, lRows));

   return lRoiUMat;
}

vsdk::UMat CreateAcfFriendlyUMat(int32_t lRows, int32_t lCols, int32_t lType)
{
   return CreateAcfFriendlyUMat(lRows, lCols, lType, 0, 0, 0, 0);
}

void FillZero(vsdk::UMat lUMat)
{
   vsdk::Mat lMat = lUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
   uint8_t* lpData = lMat.data;
   int32_t lWidthInBytes = lMat.cols*lMat.elemSize();
   int32_t lSpan = lUMat.step[0];
   int32_t lHeightInLines = lMat.rows;

   for (int y=0; y<lHeightInLines; y++)
   {
      memset((void*)((char*)lpData + (y*lSpan)), 0, lWidthInBytes);
   }
}

void FillRandom(vsdk::UMat lUMat, uint32_t* lpSeedVal)
{
   vsdk::Mat lMat = lUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
   uint8_t* lpData = lMat.data;
   int32_t lWidthInBytes = lMat.cols*lMat.elemSize();
   int32_t lSpan = lUMat.step[0];
   int32_t lHeightInLines = lMat.rows;

   for (int32_t y=0; y<lHeightInLines; y++)
   {
      for (int32_t x=0; x<lWidthInBytes; x++)
      {
         *(lpData + (y*lSpan + x)) = rand_r(lpSeedVal) % 256;
      }
   }
}

int32_t Compare(vsdk::UMat lUMatA, vsdk::UMat lUMatB)
{
   int lRetVal = 0;

   vsdk::Mat lMatA = lUMatA.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
   uint8_t* lpDataA = lMatA.data;
   int32_t lWidthInBytesA = lMatA.cols*lMatA.elemSize();
   int32_t lHeightInLinesA = lMatA.rows;
   int32_t lSpanA = lUMatA.step[0];

   vsdk::Mat lMatB = lUMatB.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);
   uint8_t* lpDataB = lMatB.data;
   int32_t lWidthInBytesB = lMatB.cols*lMatB.elemSize();
   int32_t lHeightInLinesB = lMatB.rows;
   int32_t lSpanB = lUMatB.step[0];

   if ((lWidthInBytesA  == lWidthInBytesB) && (lHeightInLinesA == lHeightInLinesB))
   {
      for (int32_t y=0; y<lHeightInLinesA; y++, lpDataA += lSpanA, lpDataB += lSpanB)
      {
         for (int32_t x=0; x<lWidthInBytesA; x++)
         {
            if (lpDataA[x] != lpDataB[x])
            {
               //printf("%d|%d(%d,%d) ",lpDataA[x], lpDataB[x], x, y);
               lRetVal = 1;
               break;
            }
         }

         if (lRetVal)
            break;
      }
   }
   else
   {
      lRetVal = 1;
   }

   return lRetVal;
}
