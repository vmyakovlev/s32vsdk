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
 * \file     gdi_com.hpp
 * \brief    public gdi api functionality header
 * \author   Tomas Babinec
 * \version  0.1
 * \date     30.7.2013
 * \note
 ****************************************************************************/

#ifndef GDICOM_HPP
#define GDICOM_HPP

#include "gdi_types_public.hpp"
#include "gdi_image.hpp"

/*****************************************************************************
 * defines/enums (const)
 *****************************************************************************/

//#define L_SUC 0
//#define L_FAIL 1


//#define GDI_SEM_WAIT_TIME 500000000u  ///< max time to wait for semaphore 2 zeros added
#define GDI_SEM_WAIT_TIME 5000000u  ///< max time to wait for semaphore

#define GDI_SERVER_ON     1u        ///< server on flag set
#define GDI_SERVER_OFF    0u        ///< server on flag reset

//  general shared memory size
#define GDI_SHM_GEN_SIZE 128        ///< general shared memory size in bytes
#ifndef INTEGRITY
// maximum allowed size of exchanged image data
#define GDI_SHM_IMG_DATA_SIZE 1920*1280*4 ///< image data exchange shared memory size in bytes
#else
//#define GDI_SHM_IMG_DATA_SIZE 1920*1280*2 //257*4096 ///< image data exchange shared memory size in bytes
#define GDI_SHM_IMG_DATA_SIZE 640 * 480 * 3
#endif

//  maximum count of GDI clients
#define GDI_MAX_CLIENT_CNT  100u          ///< max allowed GDI clients count

//  GDI message types
enum GDI_MSG_TYPE
{
  GDI_MSG_TEST,
  GDI_MSG_DISPLAY_CONTROL,
  GDI_MSG_DISPLAY_CONTROL_UPDATE,
  GDI_MSG_DISPLAY_INFO_GET,
  GDI_MSG_DISPLAY_COLOR_FILL,
  GDI_MSG_DISPLAY_COLOR_FILL32,
  GDI_MSG_DISPLAY_BUFFER_COPY,
  GDI_MSG_WINDOW_CREATE,
  GDI_MSG_WINDOW_DESTROY,
  GDI_MSG_WINDOW_COLOR_FILL,
  GDI_MSG_WINDOW_COLOR_FILL32,
  GDI_MSG_WINDOW_UPDATE,
  GDI_MSG_WINDOW_UPDATE_REGION,
  GDI_MSG_WINDOW_BUFFER_GET,
  GDI_MSG_WINDOW_BUFFER_SET,
  GDI_MSG_WINDOW_BUFFER_DROP,
  GDI_MSG_CLIENT_DEINIT,
  GDI_MSG_SERVER_CLOSE_FORCE,
  GDI_MSG_MAPPING_CREATE,
  GDI_MSG_MAPPING_DESTROY,
  GDI_MSG_MAPPING_MODIFY,

  GDI_MSG_UNKNOWN
};

/*****************************************************************************
 * types
 *****************************************************************************/

// client/server interconnection statistics
// should be placed at the beginnig of general shared memory
struct GDI_SrvCliStats
{
  uint32_t mClientCnt;
  uint32_t mServerAlive;
};

//
// GDI message structures
//

// Display control message

/****************************************************************************/
/** Display control message data.
 *
 * Used also for display control update message.
 *
 ****************************************************************************/
struct Gdi_Msg_DC
{
  // server input members
  GDI_DISPLAY_ID mDisplayId;
  bool mEnable;
  bool mVlab;

  // server output members
  LIB_RESULT mResult;
};

// Display info get message
/****************************************************************************/
/** Display info get message data
 *
 ****************************************************************************/
struct Gdi_Msg_DIG
{
  // server input members
  GDI_DISPLAY_ID mDid;
  GDI_DisplayInfo mDinfo;

  // server output members
  LIB_RESULT mResult;
};

// Display color fill message
/****************************************************************************/
/** Display color fill message data
 *
 ****************************************************************************/
struct Gdi_Msg_DCF
{
  // server input members
  GDI_DISPLAY_ID mDid;
  GDI_ColorRGBA mColorRGBA;

  // server output members
  LIB_RESULT mResult;
};

// Display color fill 32 message
/****************************************************************************/
/** Display color fill 32 message data
 *
 ****************************************************************************/
struct Gdi_Msg_DCF32
{
  // server input members
  GDI_DISPLAY_ID mDid;
  uint32_t mColorValue;

  // server output members
  LIB_RESULT mResult;
};

// DisplayBufferCopy
/****************************************************************************/
/** Display buffer copy message data
 *
 ****************************************************************************/
struct Gdi_Msg_DBC
{
  // server input members
  GDI_DISPLAY_ID mDid;
  GDI_ImageDescriptor mDesc;

  // server output members
  LIB_RESULT mResult;
};

// Window create message
/****************************************************************************/
/** Window create message data
 *
 ****************************************************************************/
struct Gdi_Msg_WC
{
  // server input members
  //unsigned mMsgType;    // type of message = what should be done
  GDI_ImageDescriptor mImgDesc;

  // server output members
  uint32_t mWindowHandle;
  LIB_RESULT mResult;
};

// Window destroy message
/****************************************************************************/
/** Window destroy message data
 *
 ****************************************************************************/
struct Gdi_Msg_WD
{
  // server input members
  //uint32_t mMsgType;    // type of message = what should be done
  uint32_t mWindowHandle;

  // server output members
  LIB_RESULT mResult;
};

// Mapping Create message
/****************************************************************************/
/** Mapping create message data.
 *
 *  Used also for Mapping modify message, because data are the same.
 ****************************************************************************/
struct Gdi_Msg_MC
{
  // server input members
  //unsigned mMsgType;    // type of message = what should be done
  GDI_W2dMapInfo mW2dMapInfo;

  // server output members
  LIB_RESULT mResult;
};

// Mapping Destroy message
/****************************************************************************/
/** Mapping destroy message data
 *
 ****************************************************************************/
struct Gdi_Msg_MD
{
  // server input members
  //unsigned mMsgType;    // type of message = what should be done
  uint32_t mWindowHandle;
  GDI_DISPLAY_ID mDisplayId;
  uint32_t mLayerId;

  // server output members
  LIB_RESULT mResult;
};

// Mapping Modify message
// the same data as Mapping Create

// Window Color Fill message
/****************************************************************************/
/** Window collor fill message data
 *
 ****************************************************************************/
struct Gdi_Msg_WCF
{
  // server input members
  uint32_t mWindowHandle;
  GDI_Rectangle mWindowRectangle;
  GDI_ColorRGBA mColorRGBA;

  // server output members
  LIB_RESULT mResult;
};

// Window Color Fill 32 message
/****************************************************************************/
/** Window color fill message data
 *
 ****************************************************************************/
struct Gdi_Msg_WCF32
{
  // server input members
  uint32_t mWindowHandle;
  GDI_Rectangle mWindowRectangle;
  uint32_t mColorValue;

  // server output members
  LIB_RESULT mResult;
};

// WindowUpdate
/****************************************************************************/
/** Window update message data.
 *
 ****************************************************************************/
struct Gdi_Msg_WU
{
  // server input members
  uint32_t mWindowHandle;
  GDI_ImageDescriptor mImageDesc;

  // server output members
  LIB_RESULT mResult;
};

// WindowUpdateRegion
/****************************************************************************/
/** Window update region message data.
 *
 *  Used for both overloads.
 *
 ****************************************************************************/
struct Gdi_Msg_WUR
{
  // server input members
  uint32_t mWindowHandle;
  GDI_Rectangle mWindowRectangle;
  GDI_ImageDescriptor mImageDesc;

  // server output members
  LIB_RESULT mResult;
};

// WindowBufferGet
/****************************************************************************/
/** Window buffer get message data.
 *
 ****************************************************************************/
struct Gdi_Msg_WBG
{
  // server input members
  uint32_t mWindowHandle;
  GDI_ImageDescriptor mImageDesc;
  unsigned long mHwAddress;   ///< HW address of contiguous image data memory region
  unsigned long mDataShift;   ///< effective data shift from stating address

  // server output members
  LIB_RESULT mResult;
};

// WindowBufferSet
/****************************************************************************/
/** Window buffer set message data.
 *
 ****************************************************************************/
struct Gdi_Msg_WBS
{
  // server input members
  uint32_t mWindowHandle;
  unsigned long mHwAddress;   ///< HW address of contiguous image data memory region

  // server output members
  LIB_RESULT mResult;
};

// WindowBufferDrop
/****************************************************************************/
/** Window buffer drop message data.
 *
 ****************************************************************************/
struct Gdi_Msg_WBD
{
  // server input members
  uint32_t mWindowHandle;
  unsigned long mHwAddress;   ///< HW address of contiguous image data memory region

  // server output members
  LIB_RESULT mResult;
};

/*****************************************************************************
 * functions (prototypes)
 *****************************************************************************/
#include "gdi_private.hpp"

/****************************************************************************/
/** Initializes GDI API.
 *
 * Called from user side. If no server activity detected a server creation
 * will be invoked.
 * Opens shared resources (shared memory and semaphores) from user side.
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_Init(uint32_t aThreadId);


/***************************************************************************/
/** Does the init semaphore synchronization during GDI initialization.
 *
 * \param apInitSem   temporary storage for init semaphore
 * \param apFirstProcess will be set to true if this is the first call to sdi init
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 ***************************************************************************/
LIB_RESULT GDI_InitSemSynchronize(OAL_SEMAPHORE *apInitSem, bool *apFirstProcess);

/****************************************************************************/
/** Test server client contact and synchronization.
 *
 * For testing purposes only. Will be removed in future.
 *
 * \param aData data to be send to server
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_SrvCliContactTest(int32_t data);  // TODO: remove (only for testing)


/****************************************************************************/
/** Deinitialize GDI API.
 *
 * Called from user side.
 * Closes shared resources (shared memory and semaphores) from user side.
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_Close(uint32_t aThreadId);


//
//  GDI server API calls
//

/****************************************************************************/
/** Client informs server about its deinit state.
 *
 * Called from user side. Informs server that number of clients has been
 * decremented and server should decide whether to continue (other clients out
 * there) or to terminate (this was the last client connected).
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_ClientDeInitCall();


/****************************************************************************/
/** Instructs server to control display enable/disable status.
 *
 * Called from user side. Informs server, this client wants to enable/disable
 * the specified display.
 *
 * \param aDispayId display id to be controled
 * \param aEnable   what diplay state to set
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_DisplayControlCall(GDI_DISPLAY_ID aDisplayId,
                                  bool aEnable);


/****************************************************************************/
/** Instructs server to change display update status.
 *
 * Called from user side. Informs server, changes to visual window content
 * should not propagated to display.
 *
 * \param aDispayId display id to be controlled
 * \param aEnable   what display update state to set
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_DisplayUpdateControlCall(GDI_DISPLAY_ID aDisplayId,
                                        bool aEnable);


/****************************************************************************/
/** Instructs server to return info about specified display.
 *
 * \param aDisplayId  which display to get the info about
 * \param aReturnedDisplayInfo  display info structure
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *
 ****************************************************************************/
LIB_RESULT GDI_DisplayInfoGetCall(GDI_DISPLAY_ID aDisplayId,
                                  GDI_DisplayInfo& aReturnedDisplayInfo);


/****************************************************************************/
/** Instructs server to fill whole display with specified color.
 *
 *  Works only with 16bit display buffers.
 *
 * \param aDisplayId which display to fill
 * \param aColor Color to be used (specified as RGBA8888).
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *
 ****************************************************************************/
LIB_RESULT GDI_DisplayColorFillCall(GDI_DISPLAY_ID const aDisplayId,
                                    GDI_ColorRGBA aColor);


/****************************************************************************/
/** Instructs server to copy display buffer content to client.
 *
 *  Uses shared memory to transfer image data.
 *
 *  \param aDisplayId id of the display to get the data from
 *  \param arGdiImage allocated image to which the result should be copied
 *  \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *
 *  \note Requires that the display buffer and given image have compatible
 *  image formats.
 ****************************************************************************/
LIB_RESULT GDI_DisplayBufferCopyCall(GDI_DISPLAY_ID aDisplayId,
                                     GDI_Image& arGdiImage);


/****************************************************************************/
/** Asks server for window create message handling.
 *
 * Called from user side. Instructs server to create new window with specified
 * parameters.
 *
 * \param arGdiImageDescriptor   window parameters
 * \param arReturnedWindowHandle reference to resulting window handle
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_WindowCreateCall(const GDI_ImageDescriptor& arGdiImageDescriptor,
                                uint32_t& arReturnedWindowHandle);


/****************************************************************************/
/** Asks server to destroy specified window.
 *
 * Called from user side. Instruction for server to destroy window specified
 * by its handle.
 *
 * \param aWindowHandle window handle defining, what should be destroyed
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_WindowDestroyCall(uint32_t aWindowHandle);


/****************************************************************************/
/** Instructs server to create mapping between specified window and display.
 *
 * Called from user side. Server will map specified window rectangle to the
 * given area inside a specific display.
 *
 * \param arcMappingInfo - [in] Mapping related data (window handle, display id, window rectangle, display rectangle)
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_MappingCreateCall(const GDI_W2dMapInfo &arcMappingInfo);


/****************************************************************************/
/** Instructs server to destroy specified window-display mapping.
 *
 * Called from user side.
 *
 * \param aWindowHandle which window to unmap
 * \param aDisplayId    from which display the window will be unmapped
 * \param aLayerId      from which layer the window will be unmapped
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *****************************************************************************/
LIB_RESULT GDI_MappingDestroyCall(uint32_t const aWindowHandle,
                                  GDI_DISPLAY_ID const aDisplayId,
                                  uint32_t const aLayerId);


/****************************************************************************/
/** Instructs server to modify mapping between specified window and display.
 *
 * Called from user side. If the mapping is found server will change the
 * mapping data.
 *
 * \param arcMappingInfo - [in] Mapping related data (window handle, display id, window rectangle, display rectangle)
 *
 * \return
 *    - #LIB_SUCCESS
 *    - #LIB_FAILURE
 **************************************************************************/
LIB_RESULT GDI_MappingModifyCall(const GDI_W2dMapInfo &arcMappingInfo);


/****************************************************************************/
/** Instructs server to fill specified window region with defined color.
 *
 * Called from user side.
 *
 * \param aWindowHandle Which window region to draw to
 * \param arcWindowRectangle The window rectangle to be filled
 * \param aColor Color to be used (specified as RGBA8888).
 *
 * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE.
 *
 * \note uses rgb565 color input
 *****************************************************************************/
LIB_RESULT GDI_WindowColorFillCall(uint32_t aWindowHandle,
                                   const GDI_Rectangle &arcWindowRectangle,
                                   GDI_ColorRGBA aColor);


/****************************************************************************/
/** Instructs server to update specified window content with defined data.
 *
 * Uses shared memory to transfer new image data from server to client.
 * Maximum allowed size of exchanged image data is specified by GDI_SHM_IMG_SIZE.
 *
 * \param aWindowHandle window handle to be updated
 * \param arcImg  image with new window content data
 * \return LIB_SUCCESS/LIB_FAILURE
 *****************************************************************************/
LIB_RESULT GDI_WindowUpdateCall(uint32_t aWindowHandle, const GDI_Image &arcImg);


/****************************************************************************/
/** Updates window region image data content.
 *
 * \param  aWindowHandle       - [in] Unique identifier of an existing window
 * \param  arcWindowRectangle  - [in] Rectangle coordinates inside the window frame to be updated
 * \param  arcImage            - [in] Image with new window data. Image ROI is taken into account if set.
 *
 * \return LIB_SUCCESS/LIB_FAILURE
 * \note no pixel format specified - might be unsafe
 ****************************************************************************/

LIB_RESULT GDI_WindowUpdateRegionCall(uint32_t aWindowHandle,
                                      const GDI_Rectangle &arcWindowRectangle,
                                      const GDI_Image &arcImage);

#ifdef GDI_SWB
/****************************************************************************/
/** Gets contiguous memory allocated window buffer from GDI server.
 *
 * \param  aWindowHandle       - [in] Unique identifier of an existing window
 * \param  arcImage            - [in] Image with new window data. Image ROI is taken into account if set.
 *
 * \return LIB_SUCCESS/LIB_FAILURE
 * \note no pixel format specified - might be unsafe
 ****************************************************************************/
LIB_RESULT GDI_WindowBufferGetCall(uint32_t aWindowHandle,
                                   GDI_Image &arcImage);

/****************************************************************************/
/** Sets new buffer to the window.
 * Uses contiguous memory for non-copy data transfer.
 *
 * \param aWindowHandle window to set the buffer to
 * \param aHwAddress image buffer hardware address
 *
 * \return LIB_SUCCESS/LIB_FAILURE
 ****************************************************************************/
LIB_RESULT GDI_WindowBufferSetCall(uint32_t aWindowHandle,
                                   uint64_t aHwAddress);

/****************************************************************************/
/** Instructs server to release all resources connected to this window buffer.
 * Uses contiguous memory for non-copy data transfer.
 *
 * \param aWindowHandle window to get the buffer from
 * \param aHwAddress image buffer hardware address
 *
 * \return LIB_SUCCESS/LIB_FAILURE
 ****************************************************************************/
LIB_RESULT GDI_WindowBufferDropCall(uint32_t aWindowHandle,
                                   uint64_t aHwAddress);
#endif //ifdef GDI_SWB

#endif /* GDICOM_HPP */
