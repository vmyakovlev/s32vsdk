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
/**
 * \file     gdi_public.hpp
 * \brief    Graphical Device Interface public functions api.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     30.7.2013
 * \note
 ****************************************************************************/

#ifndef GDIPUBLIC_HPP
#define GDIPUBLIC_HPP

/***************************************************************************/
/* Include files. */

#include "gdi_types_public.hpp"
#include "gdi_image.hpp"

#include "global_errors.h"
//#include "global_types.h"
#include <stdint.h>

//**************************************************************************/
// Literals & definitions

//**************************************************************************/
// GDI API declaration.

/**
 *  \file gdi_public.hpp
 *  \addtogroup Display GDI: Graphical Device Interface
 *  @{
 *  \brief Interface for controlling the display system.
 *  \details
 * This Interface contains functions used to interact with the display devices
 * attached to the processor. Support for individual displays will depend on the
 * build options and the presence of the physical devices. Both LCD and TV video line
 * out type displays are supported.
 *
 * The rendering engine supports layered windows, each with an optional chroma key value, so that
 * on screen displays (OSD), or frames can be rendered for the application.
 *
 * Mandatory Function Call Ordering Sequence:
 * <ol><li>#GDI_DisplayControl to enable display
 * </li><li>#GDI_WindowCreate to make one or more window(s) for image rendering output
 * </li><li>#GDI_MappingCreate to associate a window with a desired display
 * </li><li>optionally: #GDI_MappingCreate to associate previous window with a second display
 * </li><li>optionally: #GDI_WindowUpdate to transfer visual content to a window
 * </li><li>#GDI_MappingDestroy to disassociate a window from all displays
 * </li><li>#GDI_WindowDestroy to release all resources for that window
 * </li><li>#GDI_DisplayControl to disable display and release all its resources
 * </li></ol>
 *
 */

/***************************************************************************/
/** Enables GDI environment and communication with the server process.
 *
 * If server process is not running yet it is created.
 * Must be called in each process before any other GDI server requests are send.
 * Has to be called also in each thread if their supposed to be independent with respect to GDI use.
 * Each call to GDI_Init() has to be paired with GDI_Close() in the end.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 *
 **************************************************************************/
LIB_RESULT GDI_Init();

/***************************************************************************/
/** Disables GDI environment and communication with the server.
 *
 * Server process is terminated if no clients remained.
 * Must be called in each process/thread if the GDI_Init() has been used previously.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 *
 **************************************************************************/
LIB_RESULT GDI_Close();

/***************************************************************************/
/** Creates window according to specified GDI_ImageDescriptor.
 *
 * This function is used to create an object capable of receiving and handling
 * visual content sent by the user. The parameters describe the format of the
 * visual content the window will later receive through calls to UpdateWindow or
 * UpdateWindowRegion.
 *
 *
 * \param  acrGdiImageDescriptor  - [in] Descriptor defining width, height and the color space/memory representation of the image data
 * \param  arReturnedWindowHandle - [out] Returned unique identifier used for future references to the window
 *
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 *
 **************************************************************************/
LIB_RESULT GDI_WindowCreate(const GDI_ImageDescriptor& acrGdiImageDescriptor,
                            uint32_t& arReturnedWindowHandle);

/***************************************************************************/
/** Creates window according to specified GDI_ImageDescriptor.
 *
 * This function is used to create an object capable of receiving and handling
 * visual content sent by the user. The parameters describe the format of the
 * visual content the window will later receive through calls to UpdateWindow or
 * UpdateWindowRegion.
 *
 * \param  arcGdiMat  - [in] OpenCv Mat compatible image definition (width, height and the color space/memory representation of the image data).
 * \param  arReturnedWindowHandle - [out] Returned unique identifier used for future references to the window.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 *
 **************************************************************************/
LIB_RESULT GDI_WindowCreate(const GDI_Mat &arcGdiMat,
                            uint32_t& arReturnedWindowHandle);

/***************************************************************************/
/**
 * \details
 * This function is used to destroy a previously created window.
 * All mappings associated with this window will be destroyed.
 * All visual content buffers in the possession of the window will be freed.
 * 
 *
 * \param  aWindowHandle - [in] Unique identifier of an existing window
 *               
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_WindowDestroy(uint32_t aWindowHandle);

/***************************************************************************/
/**
 * \details
 * This function is used to specify alpha blending parameters for a window.
 * 
 *
 * \param  aWindowHandle - [in] Unique identifier of an existing window
 * \param  arcImageAlpha  - [in] The alpha blending configuration
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_WindowSetAlpha(uint32_t aWindowHandle,
                              const GDI_IMAGE_ALPHA& arcImageAlpha);

/***************************************************************************/
/**
 * \details
 * This function is used to specify the contrast for a window.
 * 
 *
 * \param  aWindowHandle - [in] Unique identifier of an existing window
 * \param  aContrast     - [in] The contrast value
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_WindowSetContrast(uint32_t aWindowHandle, uint8_t aContrast);

/***************************************************************************/
/**
 * \details
 * This function is used to specify the brightness for a window.
 * 
 *
 * \param  aWindowHandle - [in] Unique identifier of an existing window
 * \param  aBrightness   - [in] The brightness value
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_WindowSetBrightness(uint32_t aWindowHandle, uint8_t aBrightness);

/***************************************************************************/
/**
 * This function is used to update a window buffer containing pixel data.
 * If the window is mapped to a display; the visual content will be
 * propagated to the display.
 *
 *
 * \param  aWindowHandle - [in] Unique identifier of an existing window
 * \param  arcImage      - [in] A reference to an Image object containing visual content
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_WindowUpdate(uint32_t aWindowHandle, const GDI_Image& arcImage);

/***************************************************************************/
/**
 * This function is used to update a window buffer containing pixel data.
 * If the window is mapped to a display; the visual content will be
 * propagated to the display.
 *
 *
 * \param  aWindowHandle - [in] Unique identifier of an existing window.
 * \param  acrGdiMat     - [in] A reference to an OpenCv Mat inherited object containing visual content.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_WindowUpdate(uint32_t aWindowHandle, const GDI_Mat& acrGdiMat);

/***************************************************************************/
/**
 * This function is used to update a region of a window instead of the entire window area.
 * The width and height of the buffer must match the width and height of the
 * window area to be updated. No clipping will be performed.
 * The origin of the window region is the upper left corner of the window frame.
 *
 * If function returns #LIB_SUCCESS, GDI will free the buffer and will
 * set rpBuffer to NULL 
 * 
 *
 * \param  aWindowHandle       - [in] Unique identifier of an existing window
 * \param  arcWindowRectangle  - [in] Rectangle coordinates inside the window frame to be updated
 * \param  arcImage            - [in] Image with new window data. Image ROI is taken into account if set.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 *
 * \warning This function only works with packed data.
 **************************************************************************/
LIB_RESULT GDI_WindowUpdateRegion(uint32_t aWindowHandle,
                                  const GDI_Rectangle &arcWindowRectangle,
                                  const GDI_Image &arcImage);

/***************************************************************************/
/**
 * This function is used to update a region of a window instead of the entire window area.
 * The width and height of the buffer must match the width and height of the
 * window area to be updated. No clipping will be performed.
 * The origin of the window region is the upper left corner of the window frame.
 *
 * If function returns #LIB_SUCCESS, GDI will free the buffer and will
 * set rpBuffer to NULL
 *
 *
 * \param  aWindowHandle       - [in] Unique identifier of an existing window.
 * \param  arcWindowRectangle  - [in] Rectangle coordinates inside the window frame to be updated.
 * \param  arcGdiMat           - [in] OpenCv inherited Image object with new window data. Image ROI is taken into account if set.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 *
 * \warning This function only works with packed data.
 **************************************************************************/
LIB_RESULT GDI_WindowUpdateRegion(uint32_t aWindowHandle,
                                  const GDI_Rectangle &arcWindowRectangle,
                                  const GDI_Mat &arcGdiMat);

#ifdef GDI_SWB
/***************************************************************************/
/** Gets contiguous memory allocated window buffer from GDI server.
 *
 * \param aWindowHandle window to get the buffer from
 * \param arImage      image instance (empty) to be filled with buffer
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 ***************************************************************************/
LIB_RESULT GDI_WindowBufferGet(uint32_t aWindowHandle, GDI_Image &arImage);

/***************************************************************************/
/** Gets contiguous memory allocated window buffer from GDI server.
 *
 * \param aWindowHandle window to get the buffer from
 * \param arGdiMat image instance (empty) to be filled with buffer
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 ***************************************************************************/
LIB_RESULT GDI_WindowBufferGet(uint32_t aWindowHandle, GDI_Mat &arGdiMat);

/***************************************************************************/
/** Sets new buffer to the window.
 * Uses contiguous memory for non-copy data transfer.
 *
 * \param aWindowHandle window to set the buffer to
 * \param arcImage image buffer to be set to the window
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 ***************************************************************************/
LIB_RESULT GDI_WindowBufferSet(uint32_t aWindowHandle, const GDI_Image &arcImage);

/***************************************************************************/
/** Sets new buffer to the window.
 * Uses contiguous memory for non-copy data transfer.
 *
 * \param aWindowHandle window to set the buffer to
 * \param arcGdiMat image buffer to be set to the window
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 ***************************************************************************/
LIB_RESULT GDI_WindowBufferSet(uint32_t aWindowHandle, const GDI_Mat &arcGdiMat);

/***************************************************************************/
/** Instructs server to release all resources connected to this window buffer.
 *
 * \param aWindowHandle window the buffer belongs to
 * \param arImage image buffer to be dropped
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 ***************************************************************************/
LIB_RESULT GDI_WindowBufferDrop(uint32_t aWindowHandle, GDI_Image &arImage);

/***************************************************************************/
/** Instructs server to release all resources connected to this window buffer.
 *
 * \param aWindowHandle window the buffer belongs to
 * \param arGdiMat image buffer to be dropped
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE or other GDI_ERROR error codes
 ***************************************************************************/
LIB_RESULT GDI_WindowBufferDrop(uint32_t aWindowHandle, GDI_Mat &arGdiMat);
#endif // ifdef GDI_SWB

/***************************************************************************/
/**
 * This function is used to fill a window region with a color.
 * The origin of the window region is the upper left corner of the window frame.
 * 
 * 
 * \param  aGdiWindowHandle   - [in] Unique identifier of an existing window
 * \param  arcWindowRectangle - [in] The window rectangle to be filled
 * \param  aColor             - [in] Color to be used (specified as RGBA8888).
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_WindowColorFill(uint32_t aWindowHandle,
                               const GDI_Rectangle &arcWindowRectangle,
                               GDI_ColorRGBA aColor);

/***************************************************************************/
/**
 * \details
 * This function is used to map a region of a windows visual content to a region
 * of a displays surface. The layer Id represents the mappings distance from the 
 * foreground; it is used to determine who is on top for cases of overlap.
 * All mappings to a display MUST have a unique layer id.
 * All coordinates are relative to the top left corner of either the window or display frame.
 * 
 *
 * \param arcMappingInfo - [in] Mapping related data (window handle, display id, window rectangle, display rectangle)
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_MappingCreate(const GDI_W2dMapInfo &arcMappingInfo);

/***************************************************************************/
/**
 * \details
 * This function is used to modify an existing mapping.
 * The existing mapping is identified using the combination of the window handle, display id
 * and layer id. If found, the existing mappings window and display regions are modified.
 *
 * \param arcMappingInfo - [in] Mapping related data (window handle, display id, window rectangle, display rectangle)
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_MappingModify(const GDI_W2dMapInfo &arcMappingInfo);

/***************************************************************************/
/**
 * \details
 * This function is used to delete a window region from a display.
 * 
 *
 * \param  aWindowHandle  - [in] Unique identifier of an existing window
 * \param  aDisplayId     - [in] The display id given during mapping creation
 * \param  aLayerId       - [in] The layer id given during mapping creation
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_MappingDestroy(uint32_t aWindowHandle,
                              GDI_DISPLAY_ID aDisplayId,
                              uint32_t aLayerId);

/***************************************************************************/
/**
 * \details
 * This function is used to enable or disable a display.
 * 
 * 
 * \param  aDisplayId - [in] The display id
 * \param  aEnable    - [in] Boolean flag: true - enable display, false - disable display
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayControl(GDI_DISPLAY_ID aDisplayId, bool aEnable);

/***************************************************************************/
/**
 * \details
 * This function is used to enable specified display.
 * Display updates enabled by default.
 *
 *
 * \param  aDisplayId - [in] ID of the display to be enabled.
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayEnable(GDI_DISPLAY_ID aDisplayId);

/***************************************************************************/
/**
 * \details
 * This function is used to disable specified display.
 *
 *
 * \param  aDisplayId - [in] ID of the display to be disabled.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayDisable(GDI_DISPLAY_ID aDisplayId);

/***************************************************************************/
/**
 * \details
 * This function is used to enable or disable updates to a display.
 * While updates are disabled for a display, any updates to the visual content
 * of windows mapped to the display shall NOT be propagated to it.
 * 
 *
 *
 * \param  aDisplayId - [in] The display id
 * \param  aEnable    - [in] Boolean flag: true - enable updates, false - disable updates
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayControlUpdate(GDI_DISPLAY_ID aDisplayId, bool aEnable);

/***************************************************************************/
/**
 * \details
 * This function is used to enable updates to a display.
 * While updates are disabled for a display, any updates to the visual content
 * of windows mapped to the display shall NOT be propagated to it.
 *
 *
 *
 * \param  aDisplayId - [in] ID of the display.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayUpdateEnable(GDI_DISPLAY_ID aDisplayId);

/***************************************************************************/
/**
 * \details
 * This function is used to disable updates to a display.
 * While updates are disabled for a display, any updates to the visual content
 * of windows mapped to the display shall NOT be propagated to it.
 *
 *
 *
 * \param  aDisplayId - [in] ID of the display.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayUpdateDisable(GDI_DISPLAY_ID aDisplayId);

/***************************************************************************/
/**
 * \details
 * This function is used to get information about the specified display.
 * Its reason for being is to allow run time discovery of display attributes.
 * Most commonly used to determine window and mapping sizes.
 * 
 *
 * \param  aDisplayId             - [in] The display id
 * \param  arcReturnedDisplayInfo - [out] A reference to a display information data structure.
 *
 * \return 
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayGetInfo(GDI_DISPLAY_ID aDisplayId,
                              GDI_DisplayInfo &arcReturnedDisplayInfo);

/***************************************************************************/
/**
 * \details
 * This function is used to orient all window regions mapped to a specific display.
 * Currently rotations are limited to 0, 90, 180 and 270 degrees.
 * All rotations are clockwise around the axis or point of rotation.
 * The X axis represents an horizontal line in the middle of the display.
 * The Y axis represents a vertical line in the middle of the display.
 * The Z axis represents a point in the middle of the screen.
 *
 *
 * \param  aDisplayId          - [in] The display id
 * \param  aRotationInDegreesX - [in] The degrees of rotation around the X axis,
 *                               X axis is currently unsupported.
 * \param  aRotationInDegreesY - [in] The degrees of rotation around the Y axis,
 *                               valid values are 0 or 180.
 * \param  aRotationInDegreesZ - [in] The degrees of rotation around the Z point,
 *                               valid values are 0, 90, 180 or 270.
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #GDI_ERROR_UNSUPPORTED_ROTATION
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayRotate(GDI_DISPLAY_ID aDisplayId,
                             uint16_t aRotationInDegreesX,
                             uint16_t aRotationInDegreesY,
                             uint16_t aRotationInDegreesZ);

/***************************************************************************/
/**
 * \details
 * This function is used to fill the specified display with a color.
 * 
 *
 * \param  aDisplayId  - [in] The display id
 * \param  aColor      - [in] Color to be used (specified as RGBA8888).
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayColorFill(GDI_DISPLAY_ID aDisplayId,
                                GDI_ColorRGBA aColor);

/***************************************************************************/
/** Copies image data from frame buffer to client process.
 *
 * This function is used to copy the content of the display buffer into the
 * provided image buffer. The provided image has to be compatible with source
 * display image format.
 *
 * \param aDisplayId  - [in] Source display of the image data
 * \param arGdiImage  - [out] output image
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 ***************************************************************************/
LIB_RESULT GDI_DisplayBufferCopy(GDI_DISPLAY_ID aDisplayId,
                                 GDI_Image& arGdiImage);

/***************************************************************************/
/** Copies image data from frame buffer to client process.
 *
 * This function is used to copy the content of the display buffer into the
 * provided image buffer. The provided image has to be compatible with source
 * display image format.
 *
 * \param aDisplayId  - [in] Source display of the image data
 * \param arGdiMat    - [out] output image - OpenCv Mat compatible
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 ***************************************************************************/
LIB_RESULT GDI_DisplayBufferCopy(GDI_DISPLAY_ID aDisplayId, GDI_Mat& arGdiMat);

/***************************************************************************/
/**
 * \details
 *  This function is used to put a display to sleep.
 *
 * \param  cDisplayId - [in] The id of the display device
 *
 * \return
 *     - #LIB_SUCCESS
 *     - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplaySleep(GDI_DISPLAY_ID const cDisplayId);

/***************************************************************************/
/**
 * \details
 *  This function is used to wake up a display that is sleeping.
 *
 * \param  aDisplayId - [in] The id of the display device
 *
 * \return
 *     - #LIB_SUCCESS
 *     - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_DisplayWakeup(GDI_DISPLAY_ID aDisplayId);

/**
 * @}
 */

#endif /* GDIPUBLIC_HPP */
