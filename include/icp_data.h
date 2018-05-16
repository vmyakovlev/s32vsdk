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
#ifndef ICPDATA_H
#define ICPDATA_H
#include <stdint.h>
#include <icp_datatype.h>

namespace icp {

/**
 *  \file icp_data.h
 *  \brief ICP DataDescriptor definition
 */

/// \addtogroup datadescriptor ICP DataDescriptor definition
/// @{

/*==============================================================================*/

/**
* Enum for specification of the ROI coordinates' index in the ROI array
*/
typedef enum
{
	_roiLeft = 0,
	_roiTop = 1,
	_roiWidth = 2,
	_roiHeight = 3
} ROI_CORNERS;

/**
 * Enum for specification of allocation of the image
 */
typedef enum
{
   ALLOC_VIRT, 
   ALLOC_CONTIGUOUS, 
   ALLOC_UNKNOWN
} ALLOC;

/**
 * Class encapsulating a 1D or 2D image for ACF processing
 */
class DataDescriptor
{
public:
  /**
   * Constructor - creates an empty instance of the class
   */
  DataDescriptor();

  /**
   * OK
   */
  //DataDescriptor(DataDescriptor &copy);
  
  /**
   * Constructor - initializes the 2D image and allocates a space according to alloc type
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param width Width of the image (in pixels)
   * @param height Height of the image (in pixels)
   * @param elementType Type of the pixel
   * @param allocType Type of allocation
   */
  //DataDescriptor(uint32_t width, uint32_t height, DATATYPE elementType, ALLOC allocType = ALLOC_CONTIGUOUS);
  
  /**
   * Constructor - initializes the 2D image and allocates a space according to alloc type with specified element size
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param width Width of the image (in pixels)
   * @param height Height of the image (in pixels)
   * @param elementType Type of the pixel
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param Dimensions Number of dimensions (planes)
   * @param allocType Type of allocation
   */
  DataDescriptor(uint32_t width, uint32_t height, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t Dimensions=1, ALLOC allocType = ALLOC_CONTIGUOUS);
  
  /**
   * Constructor - initializes the 1D image and allocates a space according to alloc type
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param size The size for allocation
   * @param elementType Type of the pixel
   * @param allocType Type of allocation
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param Dimensions Number of dimensions (planes)
   * @param allocType Type of allocation
   */
  DataDescriptor(uint32_t size, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t Dimensions=1, ALLOC allocType = ALLOC_CONTIGUOUS);
  
  /**
   * Initializes the structure according to original DataDescriptor 
   * @param original Original DataDescriptor with ROI
   */
  DataDescriptor(DataDescriptor& original);
  
  /**
   * Destructor - destructs the image and free the resources if FreeOnExit is specified to be true
   * Before destructor of the last copy, the SetFreeOnExit(true) must be set to free the resources
   */
  ~DataDescriptor();
  
  /**
   * Frees all allocated resources
   */
   void Free();
   
  /**
   * After constructor init - initializes the 2D image with existing data
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param width Width of the image (in pixels)
   * @param height Height of the image (in pixels)
   * @param dataPtr Pointer to the data array
   * @param elementType Type of the pixel
   * @param allocType Type of allocation
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param Dimensions Number of dimensions (planes)
   */
  //void Init(uint32_t width, uint32_t height, void *dataPtr, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t Dimensions=1, ALLOC allocType = ALLOC_UNKNOWN);
  
  /**
   * After constructor init - initializes the 2D image with already allocated data
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param width Width of the image (in pixels)
   * @param height Height of the image (in pixels)
   * @param dataVirtPtr Pointer to the data array
   * @param dataHwPtr Pointer to the data array
   * @param elementType Type of the pixel
   * @param allocType Type of allocation
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param SpanInBytes Custom span in bytes (next row)
   * @param SpanRowInBytes Custom span to next row considering 2D pixels)
   * @param Dimensions Number of dimensions (planes)
   */  
  void InitManual(uint32_t width, uint32_t height, void *dataVirtPtr, void *dataHwPtr, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t SpanInBytes=0, uint32_t SpanRowInBytes=0, uint32_t Dimensions=1, ALLOC allocType = ALLOC_UNKNOWN);
  
  void InitManual(uint32_t width, uint32_t height, void *oalhandler, void *dataVirtPtr, void *dataHwPtr, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t SpanInBytes=0, uint32_t SpanRowInBytes=0, uint32_t Dimensions=1, ALLOC allocType = ALLOC_UNKNOWN);
  
  /**
   * After constructor init - initializes the 1D image with existing data
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param size Size of the image
   * @param dataPtr Pointer to the data array
   * @param elementType Type of the pixel
   * @param allocType Type of allocation
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param Dimensions Number of dimensions (planes)
   */
  //void Init(uint32_t size, DATATYPE elementType, void *dataPtr, uint32_t XSize=1, uint32_t YSize=1, uint32_t Dimensions=1, ALLOC allocType = ALLOC_UNKNOWN);
  
  /**
   * After constructor init - initializes the 1D image with already allocated data
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param size Size of the image
   * @param dataVirtPtr Pointer to the data array
   * @param dataHwPtr Pointer to the data array
   * @param elementType Type of the pixel
   * @param allocType Type of allocation
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param SpanInBytes Custom span in bytes (next row)
   * @param SpanRowInBytes Custom span to next row considering 2D pixels)
   * @param Dimensions Number of dimensions (planes)
   */
  void InitManual(uint32_t size, void *dataVirtPtr, void *dataHwPtr, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t SpanInBytes=0, uint32_t SpanRowInBytes=0, uint32_t Dimensions=1, ALLOC allocType = ALLOC_UNKNOWN);
  
  /**
   * After constructor init - initializes the 2D image and allocates a space according to alloc type with specified element size
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param width Width of the image (in pixels)
   * @param height Height of the image (in pixels)
   * @param elementType Type of the pixel
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param allocType Type of allocation
   * @param Dimensions Number of dimensions (planes)
   */
  void Init(uint32_t width, uint32_t height, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t Dimensions=1, ALLOC allocType = ALLOC_CONTIGUOUS);
  
  /**
   * After constructor init - initializes the 1D image and allocates a space according to alloc type
   * Before destructor, the SetFreeOnExit(true) must be set to free the resources
   * @param size The size for allocation
   * @param elementType Type of the pixel
   * @param allocType Type of allocation
   * @param XSize Width of the pixel (in data types)
   * @param YSize Height of the pixel (in data types)
   * @param Dimensions Number of dimensions (planes)
   */
  void Init(uint32_t size, DATATYPE elementType, uint32_t XSize=1, uint32_t YSize=1, uint32_t Dimensions=1, ALLOC allocType = ALLOC_CONTIGUOUS);
  
  /**
   * Initializes the structure according to original DataDescriptor 
   * ROI on destination set to None
   * Stride set accordingly
   * The resulting DataDescriptor behaves as without ROI set
   * @param original Original DataDescriptor with ROI
   */
  void InitROI(DataDescriptor &original);
  
  /**
   * Sets the ROI to the specified value
   * @param roi ROI to be set, the array of (left, right, top, bottom)
   */
  void 	SetROI(uint32_t roi[4]);
   
  /**
   * Sets the ROI to the specified value
   */
  void 	ClearROI();
   
  /**
   * Sets the ROI to the specified value
   * @param left Left coordinate
   * @param top Top coordinate
   * @param width Width of the ROI rectangle
   * @param height Height of the ROI rectangle
   */
  void 	SetROI(uint32_t left, uint32_t top, uint32_t width, uint32_t height);

  /**
   * Sets FreeOnExit flag to value. If the value is true, the data are freed on destructor
   * @param val Value of FreeOnExit to be set
   */   
  void 	SetFreeOnExit(bool val);
   
  /**
   * Returns a virtual data pointer to the pixel array
   * @return Virtual data pointer
   */
  void* GetDataPtr(uint32_t dimension = 0) const;
   
  /**
   * Returns a physical data pointer to the pixel array
   * @return Physical data pointer
   */
  void* GetDataPtrPhys(uint32_t dimension = 0);
  
  /**
   * Returns a pixel data type
   * @return Data type of the pixel
   */
  DATATYPE GetElementDataType() const;
  
  /**
   * Returns a pixel width
   * @return Pixel width
   */
  uint32_t GetElementDimX() const;
  
  /**
   * Returns a pixel height
   * @return Pixel height
   */
  uint32_t GetElementDimY() const;
  
  /**
   * Returns a width of the image in pixels
   * @return Width of the image in pixels
   */   
  uint32_t GetWidth() const;
  
  /**
   * Returns a height of the image in pixels
   * @return Height of the image in pixels
   */   
  uint32_t GetHeight() const;
  
  /**
   * Returns a number of bytes on one line (including the padding)
   * @return Span of the image (in bytes)
   */
  uint32_t GetSpan() const;
  
  /**
   * Sets a span to the image - careful with that
   * @param span Span to be set
   */
  void SetSpan(uint32_t span);
  
  /**
   * Returns a number of dimensions (parallel images) held by this structure
   * @return Number of dimensions
   */
  uint32_t GetDimensions() const;
  
  /**
   * Returns a number of bytes on one line (including the padding)
   * Does not cout element Y direction (if larger thsan 1, span gets another row of same pixel)
   * @return Span of the image (in bytes)
   */
  uint32_t GetSpanRow() const;
  
  /**
   * Returns a number of bytes on one line (including the padding)
   * @return Span of the image (in bytes)
   */
  uint32_t GetStride_ElementDataType() const;
  
  /**
   * Returns a the size of one row with regards to pixel size (in pixel size units)
   * @return Stride in X
   */
  uint32_t GetStride_ElementX() const;
  
  /**
   * Returns a the size of one row with regards to pixel size in both directions(in pixel size units)
   * @return Stride in Y
   */
  uint32_t GetStride_Element() const;
  
  /**
   * Comparison operator
   * @return True if images are equal 
   */
  bool operator==(DataDescriptor const& second) const;
  
  /**
   * Get/Set operator
   * @return Value 
   */
  void *operator()(uint32_t x, uint32_t y, uint32_t elx=0, uint32_t ely=0, uint32_t dimension = 0);

  /**
   * Fills the image with random values
   * @param seed Optional random number generator seed
   */
  void FillRandom(uint32_t seed = 0);
  
  /**
   * Fills the image with random values to all dimensions the same value
   * @param seed Optional random number generator seed
   */
  void FillRandomParallel(uint32_t seed = 0);
  
  /**
   * Fills the image with specified values
   * @param value Value to be set on all elements
   */
  void FillValue(uint8_t value);

  /**
   * Returns a ROI left value
   * @return Left boundary of ROI
   */
  uint32_t GetRoiInfoL() const;
  
  /**
   * Returns a ROI right value
   * @return Right boundary of ROI
   */
  uint32_t GetRoiInfoR() const;
  
  /**
   * Returns a ROI top value
   * @return Top boundary of ROI
   */
  uint32_t GetRoiInfoT() const;
  
  /**
   * Returns a ROI bottom value
   * @return Bottom boundary of ROI
   */
  uint32_t GetRoiInfoB() const;

  /**
   * Returns a pixel width according to its type
   * @return Pixel data type
   */
  static uint32_t ReturnElementSize(DATATYPE type);
  
  /**
   * Check if the data are present and allocated
   * @return True if all is fine
   */
  bool IsOK();
  
  /**
   * Update the data pointers with the new value
   * WARNING Does not check for size or any other traits
   * @param lpData Virtual data pointer
   * @param lpDataPhys Physical data pointer
   */
  void UpdatePtrs(void* const lpData, void* const lpDataPhys);

  /**
   * Update the width with the new value
   * WARNING Does not check for size or any other traits
   * @param lWidth 
   */
  void Update1DSize(uint32_t size);
 
  
  /**
   * Replicate the border values to the specified padding
   * @param lPadSizeInE0L Data padding left
   * @param lPadSizeInE0R Data padding right
   * @param lPadSizeInE0T Data padding top
   * @param lPadSizeInE0B Data padding bottom
   */
  void E0Replicate(int32_t lPadSizeInE0L,
                   int32_t lPadSizeInE0R,
                   int32_t lPadSizeInE0T,
                   int32_t lPadSizeInE0B);
  
private:
  /**
   * Pointer to data (virtual address)
   */
  void* 	pData;
  
  /**
   * Pointer to data (physical address)
   */
  void* 	pDataPhys;
  void*     pDataHandle;
  
  /**
   * Pixel data type
   */
  DATATYPE 	ElementDataType;
  
  /**
   * Allocation type (where are the data allocated from)
   */
  ALLOC 	AllocationType;
  
  /**
   * Pixel dimension in X
   */
  uint32_t 	ElementDimX;
  
  /**
   * Pixel dimension in Y
   */
  uint32_t 	ElementDimY;
  
  /**
   * Image width (in pixels)
   */
  uint32_t 	Width;
  
  /**
   * Image heihgt (in pixels)
   */
  uint32_t 	Height;
  
  /**
   * Image span (in bytes)
   */
  uint32_t 	Span;
  
  /**
   * Image row span (one data type row) (in bytes)
   */
  uint32_t 	SpanRow;
  
  /**
   * Data type size
   */
  uint32_t 	DataSize;
  
  /**
   * Image ROI (left, right, top, bottom)
   */
  uint32_t 	ROI[4];  
  
  /**
   * If true, the data pointer is deallocated on destructor
   */
  bool 		FreeOnExit;
  
  uint32_t Dim;
  uint32_t DimOffset;
};

/// @}

} /* namespace icp */



#endif /* ICPDATA_H */

