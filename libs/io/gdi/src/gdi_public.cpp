/*****************************************************************************
 *
 * Freescale Confidential Proprietary
 *
 * Copyright (c) 2013 Freescale Semiconductor;
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
/**
 * \file gdi_public.cpp
 * \brief GDI functionality interface exported for users.
 * \author Tomas Babinec
 * \version 0.1
 * \date 22-07-2013
 ****************************************************************************/

//  GDI interface headers
#include "gdi_public.hpp"
#include "gdi_extras.hpp"

//  GDI functionality headers

#include "gdi_com.hpp"

// log library
#include "vdb_log.h"

/*****************************************************************************
 * exported functions definitions
 ******************************************************************************/

//
// GDI lifecycle
//

LIB_RESULT GDI_Init()
{
  return GDI_Init(0);
} // GDI_Init()

LIB_RESULT GDI_Close()
{
  return GDI_Init(0);
} // GDI_Close()

//
//  Windows related functions
//
LIB_RESULT GDI_WindowCreate(const GDI_ImageDescriptor& acrGdiImageDescriptor,
                            uint32_t& arReturnedWindowHandle)
{
  return GDI_WindowCreateCall(acrGdiImageDescriptor,
                              arReturnedWindowHandle);
} // GDI_WindowCreate(ICP_Image_Descriptor)

LIB_RESULT GDI_WindowDestroy(uint32_t aWindowHandle)
{
  return GDI_WindowDestroyCall(aWindowHandle);
} // GDI_WindowDestroy

LIB_RESULT GDI_WindowSetAlpha(uint32_t /*aWindowHandle*/,
                              GDI_IMAGE_ALPHA& /*arcImageAlpha*/)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
} // GDI_WindowSetAlpha()

LIB_RESULT GDI_WindowSetContrast(uint32_t /*aWindowHandle*/, uint8_t /*aContrast*/)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
} // GDI_WindowSetContrast()

LIB_RESULT GDI_WindowSetBrightness(uint32_t /*aWindowHandle*/, uint8_t /*aBrightness*/)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
} // GDI_WindowSetBrightness()

LIB_RESULT GDI_WindowUpdate(uint32_t aWindowHandle, const GDI_Image& arcImg)
{
  return GDI_WindowUpdateCall(aWindowHandle,
                              arcImg);
} // GDI_WindowUpdate(GDI_Image&)

LIB_RESULT GDI_WindowUpdateRegion(uint32_t aWindowHandle,
                                  const GDI_Rectangle &arcWindowRectangle,
                                  const GDI_Image &arcImage)
{
  return GDI_WindowUpdateRegionCall(aWindowHandle,
                                    arcWindowRectangle,
                                    arcImage);
} // GDI_WindowUpdateRegion()

LIB_RESULT GDI_WindowBufferGet(uint32_t aWindowHandle, GDI_Image &arcImage)
{
  LIB_RESULT lres = LIB_FAILURE;
//#ifdef ARM
  lres = GDI_WindowBufferGetCall(aWindowHandle,
      arcImage);
/*#else
  VDB_LOG_ERROR("Contiguous memory supported only on ARM devices.\n");
#endif*/
  return lres;
} // GDI_WindowBufferGet(GDI_Image)

LIB_RESULT GDI_WindowBufferSet(uint32_t aWindowHandle, const GDI_Image &arcImage)
{
  LIB_RESULT lres = LIB_FAILURE;
//#ifdef ARM
  lres = GDI_WindowBufferSetCall(aWindowHandle,
      arcImage.mHwAddress);
/*#else
  VDB_LOG_ERROR("Contiguous memory supported only on ARM devices.\n");
#endif*/
  return lres;
} // GDI_WindowBufferGet(GDI_Image)

LIB_RESULT GDI_WindowBufferDrop(uint32_t aWindowHandle, GDI_Image &arcImage)
{
  LIB_RESULT lres = LIB_FAILURE;
//#ifdef ARM
  lres = GDI_WindowBufferDropCall(aWindowHandle,
      arcImage.mHwAddress);
/*#else
  VDB_LOG_ERROR("Contiguous memory supported only on ARM devices.\n");
#endif*/
  arcImage.mHwAddress = 0;
  arcImage.mData = 0;
  return lres;
} // GDI_WindowBufferDrop(GDI_Image)

LIB_RESULT GDI_WindowColorFill(uint32_t aWindowHandle,
                               const GDI_Rectangle &arcWindowRectangle,
                               GDI_ColorRGBA aColor)
{
  return GDI_WindowColorFillCall(aWindowHandle,
                                 arcWindowRectangle,
                                 aColor);
} // GDI_WindowColorFill

#ifdef GDI_OCV
LIB_RESULT GDI_WindowCreate(const GDI_Mat &arcGdiMat,
                            uint32_t& arReturnedWindowHandle)
{
  LIB_RESULT lres;

  GDI_ImageDescriptor iDsc(arcGdiMat.cols,
                           arcGdiMat.rows,
                           arcGdiMat.mPixelFormat);
  lres = GDI_WindowCreateCall(iDsc,
                              arReturnedWindowHandle);

  return lres;
} // GDI_WindowCreate(GDI_Mat)

LIB_RESULT GDI_WindowUpdate(uint32_t aWindowHandle, const GDI_Mat& arcGdiMat)
{
  return GDI_WindowUpdateCall(aWindowHandle,
                              GDI_Image(arcGdiMat));
} // GDI_WindowUpdate(GDI_Mat)

LIB_RESULT GDI_WindowUpdateRegion(uint32_t aWindowHandle,
                                  const GDI_Rectangle &arcWindowRectangle,
                                  const GDI_Mat &arcGdiMat)
{
  return GDI_WindowUpdateRegionCall(aWindowHandle,
                                    arcWindowRectangle,
                                    GDI_Image(arcGdiMat));
} // GDI_WindowUpdateRegion(GDI_Mat)

LIB_RESULT GDI_WindowBufferGet(uint32_t aWindowHandle, GDI_Mat &arcGdiMat)
{
  LIB_RESULT lres = LIB_FAILURE;
//#ifdef ARM
  GDI_Image gdi_img;
  lres = GDI_WindowBufferGetCall(aWindowHandle,
      gdi_img);
  arcGdiMat = GDI_Mat(gdi_img);
/*#else
  VDB_LOG_ERROR("Contiguous memory supported only on ARM devices.\n");
#endif*/
  return lres;
} // GDI_WindowBufferGet(GDI_Mat)

LIB_RESULT GDI_WindowBufferSet(uint32_t aWindowHandle, const GDI_Mat &arcGdiMat)
{
  LIB_RESULT lres = LIB_FAILURE;
//#ifdef ARM
  lres = GDI_WindowBufferSetCall(aWindowHandle,
      arcGdiMat.mHwAddress);
/*#else
  VDB_LOG_ERROR("Contiguous memory supported only on ARM devices.\n");
#endif*/
  return lres;
} // GDI_WindowBufferSet(GDI_Mat)

LIB_RESULT GDI_WindowBufferDrop(uint32_t aWindowHandle, GDI_Mat &arcGdiMat)
{
  LIB_RESULT lres = LIB_FAILURE;
//#ifdef ARM
  lres = GDI_WindowBufferDropCall(aWindowHandle,
     arcGdiMat.mHwAddress);
/*#else
  VDB_LOG_ERROR("Contiguous memory supported only on ARM devices.\n");
#endif*/
  arcGdiMat.mHwAddress = 0;
  arcGdiMat.data = NULL;
  return lres;
} // GDI_WindowBufferDrop(GDI_Mat)
#endif // ifdev GDI_OCV

//
//  Display related functions
//

LIB_RESULT GDI_MappingCreate(const GDI_W2dMapInfo &arcMappingInfo)
{
  return GDI_MappingCreateCall(arcMappingInfo);
} // GDI_MappingCreate

LIB_RESULT GDI_MappingModify(const GDI_W2dMapInfo &arcMappingInfo)
{
  return GDI_MappingModifyCall(arcMappingInfo);
} // GDI_MappingModify

LIB_RESULT GDI_MappingDestroy(uint32_t aWindowHandle,
                              GDI_DISPLAY_ID aDisplayId,
                              uint32_t aLayerId)
{
  return GDI_MappingDestroyCall(aWindowHandle,
                                aDisplayId,
                                aLayerId);
} // GDI_MappingDestroy

LIB_RESULT GDI_DisplayControl(GDI_DISPLAY_ID aDisplayId, bool aEnable)
{
  return GDI_DisplayControlCall(aDisplayId,
                                aEnable);
} // GDI_DisplayControl()

LIB_RESULT GDI_DisplayEnable(GDI_DISPLAY_ID aDisplayId)
{
  return GDI_DisplayControlCall(aDisplayId,
                                  true);
} // GDI_DisplayEnable()

LIB_RESULT GDI_DisplayDisable(GDI_DISPLAY_ID aDisplayId)
{
  return GDI_DisplayControlCall(aDisplayId,
                                  false);
} // GDI_DisplayDisable()

LIB_RESULT GDI_DisplayControlUpdate(GDI_DISPLAY_ID aDisplayId, bool aEnable)
{
  return GDI_DisplayUpdateControlCall(aDisplayId,
                                      aEnable);
} // GDI_DisplayControlUpdate()

LIB_RESULT GDI_DisplayUpdateEnable(GDI_DISPLAY_ID aDisplayId)
{
  return GDI_DisplayUpdateControlCall(aDisplayId,
                                        true);
} // GDI_DisplayUpdateEnable()

LIB_RESULT GDI_DisplayUpdateDisable(GDI_DISPLAY_ID aDisplayId)
{
  return GDI_DisplayUpdateControlCall(aDisplayId,
                                        false);
} // GDI_DisplayUpdateDisable()

LIB_RESULT GDI_DisplayGetInfo(GDI_DISPLAY_ID aDisplayId,
                              GDI_DisplayInfo& arReturnedDisplayInfo)
{
  return GDI_DisplayInfoGetCall(aDisplayId,
                                arReturnedDisplayInfo);

} // GDI_DisplayGetInfo()

LIB_RESULT GDI_DisplayColorFill(GDI_DISPLAY_ID aDisplayId,
                                const GDI_ColorRGBA aColor)
{
  return GDI_DisplayColorFillCall(aDisplayId,
                                  aColor);
} // GDI_DisplayColorFill()

LIB_RESULT GDI_DisplayBufferCopy(GDI_DISPLAY_ID aDisplayId,
                                 GDI_Image& arReturnedImage)
{
  return GDI_DisplayBufferCopyCall(aDisplayId,
                                   arReturnedImage);
} // GDI_DisplayBufferCoppy - with GDI_Image

#ifdef GDI_OCV

LIB_RESULT GDI_DisplayBufferCopy(GDI_DISPLAY_ID aDisplayId,
                                 GDI_Mat& arReturnedImage)
{
  GDI_Image tmp(arReturnedImage);
  return GDI_DisplayBufferCopyCall(aDisplayId,
                                   tmp);
} // GDI_DisplayBufferCoppy - with GDI_Mat
#endif

LIB_RESULT GDI_DisplaySleep(GDI_DISPLAY_ID const /*cDisplayId*/)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
} //GDI_DisplaySleep - with ICP_Image_Descriptor

LIB_RESULT GDI_DisplayWakeup(GDI_DISPLAY_ID const /*cDisplayId*/)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
} //GDI_DisplayWakeup - with ICP_Image_Descriptor

