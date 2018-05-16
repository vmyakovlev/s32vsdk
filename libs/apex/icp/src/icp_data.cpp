/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
* All Rights Reserved
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
#include <icp_data.h>
#include <icp_datatype.h>

#include <oal.h>

#include <time.h>
#include <stdlib.h> 
#include <iostream>
#include <stdio.h>

#ifdef __STANDALONE__
	#include <printf.h>
#endif

namespace icp
{
  /*==============================================================================*/
  /* Returns a pixel width according to its type				  */
  /*==============================================================================*/
  uint32_t DataDescriptor::ReturnElementSize(DATATYPE type)
  {
    return (uint32_t)RetDataTypeSizeInBytes(type);
  }
  
  /*==============================================================================*/
  /* Constructor - creates an empty instance of the class			  */
  /*==============================================================================*/
  DataDescriptor::DataDescriptor()
  {
    pData           = 0;
    pDataPhys       = 0;
    pDataHandle     = 0;
    Width           = 0;
    Height          = 0;
    Span            = 0;
    SpanRow         = 0;

    DataSize        = 0;
    ElementDataType = DATATYPE_08U;
    ElementDimX     = 1;
    ElementDimY     = 1;
    Dim             = 1;
    DimOffset       = Width*Height*DataSize*ElementDimX*ElementDimY;
    
    AllocationType  = ALLOC_UNKNOWN;
	
    ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]	= Width;
    ROI[_roiHeight] = Height;
    
    FreeOnExit      = false;
  }
  
  /*======================================================================================*/
  /* Constructor - initializes the 2D image and allocates a space according to alloc type */
  /* Before destructor, the SetFreeOnExit(true) must be set to free the resources	  */
  /*======================================================================================*/
  /*DataDescriptor::DataDescriptor( uint32_t width, uint32_t height, DATATYPE elementType, ALLOC allocType)
  {
    Init(width, height, elementType, allocType);
  }*/
   
  /*======================================================================================*/
  /* Constructor - initializes the 2D image and allocates a space according to alloc type with specified element size */
  /* Before destructor, the SetFreeOnExit(true) must be set to free the resources         */
  /*======================================================================================*/
  DataDescriptor::DataDescriptor(uint32_t width, uint32_t height, DATATYPE elementType, uint32_t XSize, uint32_t YSize, uint32_t Dimensions, ALLOC allocType)
  {
	Init(width, height, elementType, XSize, YSize, Dimensions, allocType);
  }
  
  /*======================================================================================*/
  /* Constructor - initializes the 1D image and allocates a space according to alloc type */
  /* Before destructor, the SetFreeOnExit(true) must be set to free the resources	  */
  /*======================================================================================*/
  DataDescriptor::DataDescriptor( uint32_t size, DATATYPE elementType, uint32_t XSize, uint32_t YSize, uint32_t Dimensions, ALLOC allocType)
  {
    Init(size, elementType, XSize, YSize, Dimensions, allocType);
  }
  
  DataDescriptor::DataDescriptor(DataDescriptor & original)
  {
    InitROI(original);
  }
  
  void DataDescriptor::InitManual(uint32_t width, uint32_t height, void *dataVirtPtr, void *dataHwPtr, DATATYPE elementType, uint32_t XSize, uint32_t YSize, uint32_t SpanInBytes, uint32_t SpanRowInBytes, uint32_t Dimensions, ALLOC allocType)
  {
    pData           = dataVirtPtr;
    pDataPhys       = dataHwPtr;
    pDataHandle     = 0;
    Width           = width;
    Height          = height;
    AllocationType  = allocType;
    
    ElementDataType = elementType;
    ElementDimX     = XSize;
    ElementDimY     = YSize;
    
    DataSize        = ReturnElementSize(elementType);
    
    if (SpanInBytes == 0)
      Span = width * DataSize * ElementDimX * ElementDimY;
    else
      Span = SpanInBytes;
      
    if (SpanRowInBytes == 0)
      SpanRow = Span / ElementDimY;
    else
      SpanRow = SpanRowInBytes;
      
    
    Dim             = Dimensions;
    DimOffset       = Width*Height*DataSize*ElementDimX*ElementDimY;
    

	ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]	= Width;
    ROI[_roiHeight] = Height;

    FreeOnExit      = false;
  }
  
    void DataDescriptor::InitManual(uint32_t width, uint32_t height, void *handle, void *dataVirtPtr, void *dataHwPtr, DATATYPE elementType, uint32_t XSize, uint32_t YSize, uint32_t SpanInBytes, uint32_t SpanRowInBytes, uint32_t Dimensions, ALLOC allocType)
  {
    pData           = dataVirtPtr;
    pDataPhys       = dataHwPtr;
    pDataHandle     = handle;
    Width           = width;
    Height          = height;
    AllocationType  = allocType;

    ElementDataType = elementType;
    ElementDimX     = XSize;
    ElementDimY     = YSize;

    DataSize        = ReturnElementSize(elementType);

    if (SpanInBytes == 0)
      Span = width * DataSize * ElementDimX * ElementDimY;
    else
      Span = SpanInBytes;

    if (SpanRowInBytes == 0)
      SpanRow = Span / ElementDimY;
    else
      SpanRow = SpanRowInBytes;


    Dim             = Dimensions;
    DimOffset       = Width*Height*DataSize*ElementDimX*ElementDimY;


        ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]      = Width;
    ROI[_roiHeight] = Height;

    FreeOnExit      = false;
  }


   

  void DataDescriptor::InitManual(uint32_t size, void *dataVirtPtr, void *dataHwPtr, DATATYPE elementType, uint32_t XSize, uint32_t YSize, uint32_t SpanInBytes, uint32_t SpanRowInBytes, uint32_t Dimensions, ALLOC allocType)
  {
    pData           = dataVirtPtr;
    pDataPhys       = dataHwPtr;
    pDataHandle     = 0;
    Width           = size;
    Height          = 1;
    AllocationType  = allocType;
    
    ElementDataType = elementType;
    ElementDimX     = XSize;
    ElementDimY     = YSize;

    DataSize        = ReturnElementSize(elementType);
    
    if (SpanInBytes == 0)
      Span = size * DataSize * ElementDimX * ElementDimY;
    else
      Span = SpanInBytes;
      
    if (SpanRowInBytes == 0)
      SpanRow = Span / ElementDimY;
    else
      SpanRow = SpanRowInBytes;
    
    Dim             = Dimensions;
    DimOffset       = Width*Height*DataSize*ElementDimX*ElementDimY;
    
    ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]	= Width;
    ROI[_roiHeight] = Height;

    FreeOnExit      = false;
  }
  
  /*==================================================================================================*/
  /* After constructor init - initializes the 1D image and allocates a space according to alloc type  */
  /* Before destructor, the SetFreeOnExit(true) must be set to free the resource		      */
  /*==================================================================================================*/
  void DataDescriptor::Init( uint32_t size, DATATYPE elementType, uint32_t XSize, uint32_t YSize, uint32_t Dimensions, ALLOC allocType)
  {
    uint32_t OAL_AllocFlag;
    
    // Switch the allocation type
    switch (allocType)
    {
      case ALLOC_CONTIGUOUS:
        OAL_AllocFlag = OAL_MEMORY_FLAG_CONTIGUOUS;
        break;
      default:
        OAL_AllocFlag = 0;
    }          
    
    DataSize        = ReturnElementSize(elementType);
    pDataHandle     = OAL_MemoryAllocFlag(size*DataSize*XSize*YSize*Dimensions, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_AllocFlag);
    pData           = OAL_MemoryReturnAddress(pDataHandle, ACCESS_CH_WB);
    pDataPhys       = OAL_MemoryReturnAddress(pDataHandle, ACCESS_PHY);
    Width           = size;
    Height          = 1;
    AllocationType  = allocType;

    ElementDataType = elementType;
    ElementDimX     = XSize;
    ElementDimY     = YSize;

	Span			= size * DataSize * ElementDimX * ElementDimY;
    SpanRow         = Span / ElementDimY;
    Dim             = Dimensions;
    DimOffset       = Width*Height*DataSize*ElementDimX*ElementDimY;
    
    ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]	= Width;
    ROI[_roiHeight] = Height;
    //ROI[3]          = 0;
    
    FreeOnExit      = false;
  }
  
  /*======================================================================================*/
  /* After constructor init - initializes the 2D image and allocates a space according to alloc type with specified element size */
  /* Before destructor, the SetFreeOnExit(true) must be set to free the resources         */
  /*======================================================================================*/
  void DataDescriptor::Init(uint32_t width, uint32_t height, DATATYPE elementType, uint32_t XSize, uint32_t YSize, uint32_t Dimensions, ALLOC allocType)
  {
    uint32_t OAL_AllocFlag;
    
    // Switch the allocation type
    switch (allocType)
    {
      case ALLOC_CONTIGUOUS:
        OAL_AllocFlag = OAL_MEMORY_FLAG_CONTIGUOUS;
        break;
      default:
        OAL_AllocFlag = 0;
    }               

    DataSize        = ReturnElementSize(elementType);
    pDataHandle     = OAL_MemoryAllocFlag(width*height*DataSize*XSize*YSize*Dimensions, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_AllocFlag);
    pData           = OAL_MemoryReturnAddress(pDataHandle, ACCESS_CH_WB);
    pDataPhys       = OAL_MemoryReturnAddress(pDataHandle, ACCESS_PHY);
    Width           = width;
    Height          = height;
    AllocationType  = allocType;

    ElementDataType = elementType;
    ElementDimX     = XSize;
    ElementDimY     = YSize;
    
    Span            = (width * DataSize * XSize) * YSize;
    SpanRow         = Span / ElementDimY;
    Dim             = Dimensions;
    DimOffset       = Width*Height*DataSize*ElementDimX*ElementDimY;
    

	ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]	= Width;
    ROI[_roiHeight] = Height;

    FreeOnExit      = false;
  }
  
  void DataDescriptor::InitROI(DataDescriptor &original)
  {
    pData           = (int8_t *)original.pData + original.ROI[_roiTop]*original.Span + original.ROI[_roiLeft];//TODO: is it not to be multibplied with original.ElementDimX?
    pDataPhys       = (int8_t *)original.pDataPhys + original.ROI[_roiTop]*original.Span + original.ROI[_roiLeft];
   
    pDataHandle     = original.pDataHandle;
    Width           = original.ROI[_roiWidth]; // TODO: This could be buggy it is the margin at the right of the ROI
    Height          = original.ROI[_roiHeight];
    AllocationType  = original.AllocationType;
	
    ElementDataType = original.ElementDataType;
    ElementDimX     = original.ElementDimX;
    ElementDimY     = original.ElementDimY;

    DataSize        = original.DataSize;
    Span            = original.Span;
    SpanRow         = Span / ElementDimY;
    Dim             = original.Dim;
    DimOffset       = original.DimOffset;


	ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]	= Width;
    ROI[_roiHeight] = Height;

    FreeOnExit      = original.FreeOnExit; // should this not be set to false?
  }
  
  /*===============================================================================================*/
  /* Destructor - destructs the image and free the resources if FreeOnExit is specified to be true */
  /* Before destructor of the last copy, the SetFreeOnExit(true) must be set to free the resources */
  /*===============================================================================================*/
  DataDescriptor::~DataDescriptor()
  {
	  Free();
  }
  
 void DataDescriptor::Free()
 {
    ClearROI();

    if (FreeOnExit && pDataHandle != NULL)
    {
       OAL_MemoryFree(pDataHandle);
       pData = 0;
       pDataPhys = 0;
       pDataHandle = 0;
       FreeOnExit      = false;
    }
}
  /*==============================================================================*/
  /* Sets the ROI to the specified value					  */
  /*==============================================================================*/
  void 	DataDescriptor::SetROI(uint32_t roi[4])
  {
    if (roi[_roiLeft] < Width &&
        roi[_roiLeft] + roi[_roiWidth] < Width &&
        roi[_roiTop] < Height &&
        roi[_roiTop] + roi[_roiHeight] < Height)
    {
    }
    else
    {
      printf("SetROI(...) -> The ROI does not fall within the specified parent data region ");
      return;
    }
    
	ROI[_roiLeft]   = roi[_roiLeft];
    ROI[_roiTop]    = roi[_roiTop];
    ROI[_roiWidth]	= roi[_roiWidth];
    ROI[_roiHeight] = roi[_roiHeight];
  }
  
  /*==============================================================================*/
  /* Sets the ROI to the specified value					  */
  /*==============================================================================*/
  void 	DataDescriptor::ClearROI()
  {
	  /*
    ROI[0] = 0;
    ROI[1] = 0;
    ROI[2] = 0;
    ROI[3] = 0;
*/
	ROI[_roiLeft]   = 0;
    ROI[_roiTop]    = 0;
    ROI[_roiWidth]  = Width;
    ROI[_roiHeight] = Height;

  }
  
  /*==============================================================================*/
  /* Sets the ROI to the specified value					  */
  /*==============================================================================*/
  void 	DataDescriptor::SetROI(uint32_t left, uint32_t top, uint32_t width, uint32_t height)
  {
    if (left < Width &&
        left + width <= Width &&
        top < Height &&
        top + height <= Height)
	{
	}
    else
    {
      printf("SetROI(...) -> The ROI does not fall within the specified parent data region ");
      return;
    }
    
	ROI[_roiLeft]   = left;
    ROI[_roiTop]    = top;
    ROI[_roiWidth]	= width;
    ROI[_roiHeight] = height;
  }
   
  /*=======================================================================================*/
  /* Sets FreeOnExit flag to value. If the value is true, the data are freed on destructor */
  /*=======================================================================================*/
  void 	DataDescriptor::SetFreeOnExit(bool val)
  {
    FreeOnExit = val;
  }
  
  /*==============================================================================*/
  /* Returns a virtual data pointer to the pixel array				  */
  /*==============================================================================*/
  void* DataDescriptor::GetDataPtr(uint32_t Dimension) const
  {    
    if (Dimension >= Dim) return NULL;
    return ((int8_t *)pData + (DimOffset * Dimension));
  }
  
  /*==============================================================================*/
  /* Returns a physical data pointer to the pixel array				  */
  /*==============================================================================*/
  void* DataDescriptor::GetDataPtrPhys(uint32_t Dimension)
  {
    if (Dimension >= Dim) return NULL;
    return ((int8_t *)pDataPhys + (DimOffset * Dimension));
  }
  
  /*==============================================================================*/
  /* Returns a pixel data type							  */
  /*==============================================================================*/
  DATATYPE DataDescriptor::GetElementDataType() const
  {
    return ElementDataType;
  }
  
  /*==============================================================================*/
  /* Returns a pixel width							  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetElementDimX() const
  {
    return ElementDimX;
  }
  
  /*==============================================================================*/
  /* Returns a pixel height							  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetElementDimY() const
  {
    return ElementDimY;
  }
   
  /*==============================================================================*/
  /* Returns a width of the image in pixels					  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetWidth() const
  {
    return Width;
  }
  
  /*==============================================================================*/
  /* Returns a height of the image in pixels					  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetHeight() const
  {
    return Height;
  }
  
  /*==============================================================================*/
  /* Returns a number of bytes on one line (including the padding)		  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetSpan() const
  {
    return Span;
  }
  
  void DataDescriptor::SetSpan(uint32_t span)
  {
    Span = span;
	SpanRow = Span / ElementDimY;
  }
  
  /*==============================================================================*/
  /* Returns a number of dimensions (parallel images) held by this structure		  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetDimensions() const
  {
    return Dim;
  }
  
  /*==============================================================================*/
  /* Returns a number of bytes on one line (including the padding)		  		  */
  /* Does not count the element Y direction										  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetSpanRow() const
  {
    return SpanRow;
  }
  
  /*==============================================================================*/
  /* Returns a number of bytes on one line (including the padding)		  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetStride_ElementDataType() const
  {
    return Span / DataSize;
  }
  
  /*================================================================================*/
  /* Returns a the size of one row with regards to pixel size (in pixel size units) */
  /*================================================================================*/
  uint32_t DataDescriptor::GetStride_ElementX() const
  {    
    return Span / (DataSize * ElementDimX);
  }
  
  /*==================================================================================================*/
  /* Returns a the size of one row with regards to pixel size in both directions(in pixel size units) */
  /*==================================================================================================*/
  uint32_t DataDescriptor::GetStride_Element() const
  {    
    return Span / (DataSize * ElementDimY * ElementDimX);
  }
  
  /*==============================================================================*/
  /* Returns a ROI left value							  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetRoiInfoL() const
  {
    return ROI[_roiLeft];
  }
  
  /*==============================================================================*/
  /* Returns a ROI right value							  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetRoiInfoR() const
  {
	  return Width - ROI[_roiLeft] - ROI[_roiWidth]; 
  }
  
  /*==============================================================================*/
  /* Returns a ROI top value							  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetRoiInfoT() const
  {
    return ROI[_roiTop];
  }
  
  /*==============================================================================*/
  /* Returns a ROI bottom value							  */
  /*==============================================================================*/
  uint32_t DataDescriptor::GetRoiInfoB() const
  {
	  return Height - ROI[_roiTop] - ROI[_roiHeight]; 
  }
  
  /*==============================================================================*/
  /* Comparison operator							  */
  /*==============================================================================*/
  bool DataDescriptor::operator==(DataDescriptor const& second) const
  { 
    //if the comparison makes sense
    if (Width  == second.Width &&
        Height == second.Height &&
        ElementDataType == second.ElementDataType &&
        ElementDimX == second.ElementDimX &&
        ElementDimY == second.ElementDimY &&
        Dim == second.Dim)
    {
      const uint32_t width  = Width * ElementDimX * DataSize;
      const uint32_t height = Height * ElementDimY;
      const uint8_t* pDataA = (uint8_t*)pData;
      const uint8_t* pDataB = (uint8_t*)second.pData;
      const uint32_t SpanA = SpanRow;
      const uint32_t SpanB = second.SpanRow;

      for (uint32_t dim = 0; dim < Dim; ++dim)
         for (uint32_t i = 0; i < height; ++i)
            for (uint32_t j = 0; j < width; ++j)
            {
               if (*(pDataA + (DimOffset*dim) + ((i)* SpanA + (j))) != *(pDataB + (second.DimOffset*dim) + ((i)* SpanB + (j)))) return false;
            }
    }
    else
      return false;
   
    return true;
  }

  int random_number_local = 897432984;
  int random_number_local_inc = 743289748;
  extern "C"
	{
		unsigned long get_uptime_microS();
	}
  /*==============================================================================*/
  /* Fills the image with random values						  */
  /*==============================================================================*/
  void DataDescriptor::FillRandom(uint32_t seed)
  {
#ifndef __STANDALONE__
    if (0 == seed)
    {
      seed = time(NULL);
    }

    uint32_t width = Width*ElementDimX*DataSize;
    uint32_t height = Height * ElementDimY;

    uint8_t* lpData = (uint8_t*)pData;
    
    for (uint32_t dim = 0; dim < Dim; ++dim)
    for (uint32_t i = 0; i < height; ++i)
    for (uint32_t j = 0; j < width; j++)
    for (uint32_t k = 0; k < ElementDimY; k++)
    {
      *(lpData + (DimOffset*dim) + ((i) * SpanRow + (j))) = rand_r(&seed) % 256;
    }
#else
	uint32_t width = Width*ElementDimX*DataSize;
    uint32_t height = Height * ElementDimY;
    uint8_t* lpData = (uint8_t*)pData;
    
    for (uint32_t dim = 0; dim < Dim; ++dim)
    for (uint32_t i = 0; i < height; ++i)
    for (uint32_t j = 0; j < width; j++)
    for (uint32_t k = 0; k < ElementDimY; k++)
    {
		random_number_local += (int)get_uptime_microS();
      *(lpData + (DimOffset*dim) + ((i) * SpanRow + (j))) = random_number_local & 0x000000FF;
    }
#endif

  }
  
    void DataDescriptor::FillRandomParallel(uint32_t seed)
  {
#ifndef __STANDALONE__
    if (0 == seed)
    {
      seed = time(NULL);
    }

    uint32_t width = Width*ElementDimX*DataSize;
    uint32_t height = Height * ElementDimY;

    uint8_t* lpData = (uint8_t*)pData;
    
    for (uint32_t i = 0; i < height; ++i)
    for (uint32_t j = 0; j < width; j++)
    for (uint32_t k = 0; k < ElementDimY; k++)
    {
      uint8_t value = rand_r(&seed) % 256;
      for (uint32_t dim = 0; dim < Dim; ++dim)
      {
	*(lpData + (DimOffset*dim) + ((i) * SpanRow + (j))) = value;
      }
      
    }
#else
	uint32_t width = Width*ElementDimX*DataSize;
    uint32_t height = Height * ElementDimY;
    uint8_t* lpData = (uint8_t*)pData;
    
	uint8_t cnt = 0;
    for (uint32_t i = 0; i < height; ++i)
    for (uint32_t j = 0; j < width; j++)
    for (uint32_t k = 0; k < ElementDimY; k++)
    {
      uint8_t value = cnt++;
      for (uint32_t dim = 0; dim < Dim; ++dim)
      {
	*(lpData + (DimOffset*dim) + ((i) * SpanRow + (j))) = value;
      }
      
    }
#endif
  }
  
  void DataDescriptor::FillValue(uint8_t value)
  {
    uint32_t width = Width*ElementDimX*DataSize;
    uint32_t height = Height * ElementDimY;

    uint8_t* lpData = (uint8_t*)pData;
    
    for (uint32_t dim = 0; dim < Dim; ++dim)
    for (uint32_t i = 0; i < height; ++i)
    for (uint32_t j = 0; j < width; j++)
    for (uint32_t k = 0; k < ElementDimY; k++)
    {
      *(lpData + (DimOffset*dim) + ((i) * SpanRow + (j))) = value;
    }
  }
  
  void PadCorner(uint8_t* lpSrcE0,
               uint8_t* lpDstE0,
               int32_t  lPadWidthInE0,
               int32_t  lPadHeightInE0,
               int32_t  lE0WidthInBytes,
               int32_t  lE0HeightInLines,
               int32_t  lSpan)
{
   for (int32_t i=0; i<lPadWidthInE0; i++) //padding width (in e0)
   {
      for (int32_t j=0; j<lPadHeightInE0; j++) //padding height (in e0)
      {
         for (int32_t k=0; k<lE0WidthInBytes; k++) //width of e0 (in bytes)
         {
            for (int32_t l=0; l<lE0HeightInLines; l++) //height of e0 (in lines)
            {
               lpDstE0[i*lE0WidthInBytes + j*lE0HeightInLines*lSpan + k + l*lSpan] = lpSrcE0[k + l*lSpan];
            }
         }
      }
   }
}

void PadVerEdge(uint8_t* lpSrcE0,
                uint8_t* lpDstE0,
                int32_t  lPadWidthInE0,
                int32_t  lEdgeHeightInE0,
                int32_t  lE0WidthInBytes,
                int32_t  lE0HeightInLines,
                int32_t  lSpan)
{
   for (int32_t i=0; i<lPadWidthInE0; i++) //padding width (in e0)
   {
      for (int32_t j=0; j<lEdgeHeightInE0; j++) //padding height (in e0)
      {
         for (int32_t k=0; k<lE0WidthInBytes; k++) //width of e0 (in bytes)
         {
            for (int32_t l=0; l<lE0HeightInLines; l++) //height of e0 (in lines)
            {
               lpDstE0[i*lE0WidthInBytes + j*lE0HeightInLines*lSpan + k + l*lSpan] = lpSrcE0[k + (l+(j*lE0HeightInLines))*lSpan];
            }
         }
      }
   }
}

void PadHorEdge(uint8_t* lpSrcE0,
                uint8_t* lpDstE0,
                int32_t  lEdgeWidthInE0,
                int32_t  lPadHeightInE0,
                int32_t  lE0WidthInBytes,
                int32_t  lE0HeightInLines,
                int32_t  lSpan)
{
   for (int32_t i=0; i<lEdgeWidthInE0; i++) //padding width (in e0)
   {
      for (int32_t j=0; j<lPadHeightInE0; j++) //padding height (in e0)
      {
         for (int32_t k=0; k<lE0WidthInBytes; k++) //width of e0 (in bytes)
         {
            for (int32_t l=0; l<lE0HeightInLines; l++) //height of e0 (in lines)
            {
               lpDstE0[i*lE0WidthInBytes + j*lE0HeightInLines*lSpan + k + l*lSpan] = lpSrcE0[i*lE0WidthInBytes + k + l*lSpan];
            }
         }
      }
   }
}

void DataDescriptor::UpdatePtrs(void* const lpData,
                                    void* const lpDataPhys)
{
   pData     = lpData;
   pDataPhys = lpDataPhys;
}

void DataDescriptor::Update1DSize(uint32_t size)
{
   Width           = size;
   Span            = size * DataSize * ElementDimX * ElementDimY;
   SpanRow         = Span / ElementDimY;
   DimOffset       = Width*Height*DataSize*ElementDimX*ElementDimY;
   ROI[_roiWidth]  = Width;
}
   
  void DataDescriptor::E0Replicate(int32_t lPadSizeInE0L,
                                   int32_t lPadSizeInE0R,
                                   int32_t lPadSizeInE0T,
                                   int32_t lPadSizeInE0B)
{
   uint8_t* lpDataBase = (unsigned char*)pData; //pointer to start of region on which e0 replication should occur
   int32_t lE0WidthInBytes   = ElementDimX*ReturnElementSize(ElementDataType);
   int32_t lE0HeightInLines = ElementDimY;
   int32_t lWidth            = Width;
   int32_t lHeight           = Height;
   int32_t lSpan             = Span;
   int32_t lWidthInBytes     = Width*ElementDimX*ReturnElementSize(ElementDataType);
   int32_t lHeightInLines    = Height*ElementDimY;

   uint8_t* lpDataTopLeftCorner  = lpDataBase;
   uint8_t* lpDataBotLeftCorner  = lpDataBase + lSpan*(lHeightInLines-lE0HeightInLines);
   uint8_t* lpDataTopRightCorner = lpDataTopLeftCorner + lWidthInBytes - lE0WidthInBytes;
   uint8_t* lpDataBotRightCorner = lpDataBotLeftCorner + lWidthInBytes - lE0WidthInBytes;

   uint8_t* lpDst = 0;

   //top left corner
   lpDst = lpDataTopLeftCorner - lPadSizeInE0T*lE0HeightInLines*lSpan - lPadSizeInE0L*lE0WidthInBytes;
   PadCorner(lpDataTopLeftCorner, lpDst, lPadSizeInE0L, lPadSizeInE0T, lE0WidthInBytes, lE0HeightInLines, lSpan);

   //top right corner
   lpDst = lpDataTopRightCorner - lPadSizeInE0T*lE0HeightInLines*lSpan + lE0WidthInBytes;
   PadCorner(lpDataTopRightCorner, lpDst, lPadSizeInE0R, lPadSizeInE0T, lE0WidthInBytes, lE0HeightInLines, lSpan);

   //bottom left corner
   lpDst = lpDataBotLeftCorner + lE0HeightInLines*lSpan - lPadSizeInE0L*lE0WidthInBytes;
   PadCorner(lpDataBotLeftCorner, lpDst, lPadSizeInE0L, lPadSizeInE0B, lE0WidthInBytes, lE0HeightInLines, lSpan);

   //bottom right corner
   lpDst = lpDataBotRightCorner + lE0HeightInLines*lSpan + lE0WidthInBytes;
   PadCorner(lpDataBotRightCorner, lpDst, lPadSizeInE0R, lPadSizeInE0B, lE0WidthInBytes, lE0HeightInLines, lSpan);

   //top edge
   lpDst = lpDataTopLeftCorner - lPadSizeInE0T*lE0HeightInLines*lSpan;
   PadHorEdge(lpDataTopLeftCorner, lpDst, lWidth, lPadSizeInE0T, lE0WidthInBytes, lE0HeightInLines, lSpan);

   //bottom edge
   lpDst = lpDataBotLeftCorner + lE0HeightInLines*lSpan;
   PadHorEdge(lpDataBotLeftCorner, lpDst, lWidth, lPadSizeInE0B, lE0WidthInBytes, lE0HeightInLines, lSpan);

   //left edge
   lpDst = lpDataTopLeftCorner - lPadSizeInE0L*lE0WidthInBytes;
   PadVerEdge(lpDataTopLeftCorner, lpDst, lPadSizeInE0L, lHeight, lE0WidthInBytes, lE0HeightInLines, lSpan);

   //right edge
   lpDst = lpDataTopRightCorner + lE0WidthInBytes;
   PadVerEdge(lpDataTopRightCorner, lpDst, lPadSizeInE0R, lHeight, lE0WidthInBytes, lE0HeightInLines, lSpan);
}
  
  /*==============================================================================*/
  /* Check if the data are present and allocated				  */
  /*==============================================================================*/
  bool DataDescriptor::IsOK()
  {
    if (pData != NULL) return true;
    else return false;
  }
  
   /*==============================================================================*/
   /* Get/Set operator															   */
   /*==============================================================================*/
  void *DataDescriptor::operator()(uint32_t x, uint32_t y, uint32_t elx, uint32_t ely, uint32_t dim)
  {
	return &((uint8_t *)pData + (dim*DimOffset))[y * Span + SpanRow*ely + (x*DataSize*(ElementDimX)) + elx*DataSize];
  }
} // namespace icp
