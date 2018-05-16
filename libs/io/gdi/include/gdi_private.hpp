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
 * \file     GdiPrivate.hpp
 * \brief    GDI private infrastructure and functionality implementation for Integrity.
 * \author   Tomas Babinec
 * \version  0.1
 * \date     10-01-2014
 * \note
 ****************************************************************************/

#ifndef GDIPRIVATE_HPP
#define GDIPRIVATE_HPP

#ifndef INTEGRITY

//
//  LINUX system includes
//

#include <linux/fb.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/kd.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#else

//
//  INTEGRITY system includes
//

#include <INTEGRITY.h>
#ifndef VLAB
#include <device/fbdriver.h>
#endif //ifndef __VLAB
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#endif

#include <list>
#include <map>

//
// GDI includes
//

#include "gdi_extras.hpp"
#include "gdi_image.hpp"      // own image representation
#include "gdi_com.hpp"

#include "gdi_utils.hpp"

#ifndef INTEGRITY
#endif 

//
//  OAL includes
//

//#include <oal_file.h>
#include <oal_process.h>

//  at first only one layer
#define MAX_NUM_OF_LAYERS 10

// maximum number of allowed shared window buffers
#define GDI_MAX_OSWB_CNT 100

#define GDI_MAX_OSWB_POOL_SIZE 1920*1080*2

/*****************************************************************************
 * types
 *****************************************************************************/

//  declare GDI classes
class GDI_Display; // encapsulates display info
class GDI_Window; // encapsulates window info
struct GDI_W2dMapPrivate; // encapsulates window to display mapping info
class GDI_PSWBManager;	// encapsulates window buffer sharing over POSIX shm
class GDI_Manager; // encapsulates GDI functionality with window management

/****************************************************************************/
/** GDI_Display class encapsulates one display management interface.
 *
 * Allows setting console mode (KD_GRAPHICS/KD_TEXT) and framebuffer device
 * file operations.
 ****************************************************************************/

class GDI_Display
{
  friend class GDI_Manager;
public:
  GDI_Display();
  ~GDI_Display();

  //  get screeninfos and enable framebuffer

  /****************************************************************************/
  /** Enables VLAB video probe buffer access
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT BufferEnableVlab();

  /****************************************************************************/
  /** Enables VLAB video probe buffer access
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT BufferDisableVlab();

  //  Enables framebuffer
  /****************************************************************************/
  /** Enables framebuffer for this display if possible.
   * Uniform interface for all platforms. Internaly desides which OS specific approach to use.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT FBEnable();

  //  Disables framebuffer
  /****************************************************************************/
  /** Disables framebuffer for this display.
   * Uniform interface for all platforms. Internaly desides which OS specific approach to use.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT FBDisable();

  //  find out if display is enabled
  /****************************************************************************/
  /** Find out if framebuffer device file is enabled.
   *
   * Both framebuffer file descriptor and pointer to buffer data pointer are
   * checked.
   *
   * \return true if all ok. false if file descriptor, buffer data pointer or
   * both are unset.
   ****************************************************************************/
  bool Enabled();

  // copy data to real frame-buffer
  /****************************************************************************/
  /** Copies data from draw-buffer to frame-buffer.
   *
   * Makes the new display content changes visible.
   *
   * \param arRct reference to rectangle specifying fb area to be updated.
   * \return true if all ok. False if something fails.
   ****************************************************************************/
  bool FbUpdate(GDI_Rectangle &arRct);

  // copy data to real frame-buffer
  /****************************************************************************/
  /** Copies data from draw-buffer to frame-buffer.
   *
   * Makes the new display content changes visible.
   *
   * \return true if all ok. False if something fails.
   ****************************************************************************/
  bool FbUpdate();

  //  get info if enabled
  /****************************************************************************/
  /** Finds out if display updates are allowed.
   *
   * If updates are disabled for a display, any updates to the visual content of
   * windows mapped to the display shall NOT be propagated to it.
   * DisplayColorFill function is not affected by this.
   *
   * \return true if updates enabled, false otherwise.
   ****************************************************************************/
  bool UpdatesOn();

  //  draw rectangle
  /****************************************************************************/
  /** Draws rectangle directly to framebuffer.
   *
   * \param aX    x rectangle origin
   * \param aY    y rectangle origin
   * \param aW    width of the rectangle
   * \param aH    height of the rectangle
   * \param aColor pointer to 3 byte RGB char array
   ****************************************************************************/
  void RectDraw(uint32_t aX, uint32_t aY, uint32_t aW, uint32_t aH, GDI_ColorRGBA aColor);

  //  set screen color
  /****************************************************************************/
  /** Fills whole frambuffer with one color.
   *
   * \param aColor  pointer to 3 byte RGB char array
   ****************************************************************************/
  void ColorFill(GDI_ColorRGBA aColor);

private:

#ifdef INTEGRITY
    //  Enables framebuffer for Integrity OS
  /****************************************************************************/
  /** Integrity OS implementation of framebuffer enabling functionality.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT GhsFBEnable();

  //  Disables framebuffer for Integrity OS
  /****************************************************************************/
  /** Integrity OS implementation of framebuffer disabling functionality.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT GhsFBDisable();
#else
   //  Enables framebuffer for Linux OS
  /****************************************************************************/
  /** Linux OS implementation of framebuffer enabling functionality.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT LinuxFBEnable();
	
	//  Disables framebuffer for Linux OS
  /****************************************************************************/
  /** Linux OS implementation of framebuffer disabling functionality.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT LinuxFBDisable();

	  /****************************************************************************/
  /** Sets console device mode.
   *
   * Opens console device file and sets console mode (KD_GRAPHIC/KD_TEXT).
   *
   * \param apCon  console device file path
   * \param aMode  console device mode to be set (KD_GRAPHIC/KD_TEXT).
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   *****************************************************************************/
  LIB_RESULT LinuxConsoleModeSet(char const *apCon, uint32_t aMode);

  /****************************************************************************/
  /** Opens Linux framebuffer device file.
   *
   * \param apPath  framebuffer device file path
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT LinuxFBFileOpen(const char *apPath);

  /****************************************************************************/
  /** Closes Linux framebuffer device file.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT LinuxFBFileClose();

	/****************************************************************************/
  /** Maps framebuffer device file to memory.
   *
   * Framebuffer device file has to be opend first by LinuxFBFileOpen().
   * Reads framebuffer information by call to ioctl.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT LinuxFBConnect();

  //  disable framebuffer
  /****************************************************************************/
  /** Unmaps framebuffer device file from memory.
   *
   * Does not close framebuffer device file (call LinuxFBFileClose()).
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT LinuxFBDisconnect();
#endif //ifdef INTEGRITY

#ifndef INTEGRITY
//  OAL_FILE_DESCRIPTOR mFbdevFd; 				///<  frame buffer file descriptor
  // framebuffer information
  const char * mpConPath;
  struct fb_var_screeninfo mScreenInfo; ///< screen info from ioctl
  struct fb_fix_screeninfo mFixedInfo;  ///< fixed frame buffer info from ioctl
  uint8_t mHdmiFb;
  uint8_t *mpFbMapBase;			///< base address of the mapped FB region
#else
#ifndef VLAB
  FBDriver *mpFbdrv;							///< pointer to ghs fb driver
	FBHandle mFbhandle_screen;			///< Physical screen layer handel
	FBInfo mFbinfo_screen;					///< Physical screen info
	FBHandle mFbhandle_layer1;			///< Logical screen layer handel
	FBInfo mFbinfo_layer1;					///< Logical Screen layer info
#endif // ifndef VLAB
#endif // ifndef INTEGRITY

	const char * mpFBDesignation; 	///< frame-buffer designation  

  vpd_regs_t *mpVpdRegs;    ///< pointer to video probe device registers stucture;

  GDI_Image mBufferImage;   ///< image class encapsulating visible area of framebuffer data
  GDI_Image mBufferImage2;  ///< for double buffered output
  size_t mBufLen;           ///< frame buffer data size in bytes
  uint32_t mEnabledCnt;     ///< number of DisplayControl enable messages received (should correspond to number of connected clients)
  bool mUpdatesOn;          ///< remembers if updates are allowed to this display
  GDI_ColorRGBA mBckgColor; ///< display background color
}; // GDI_Display

/****************************************************************************/
/** GDI_Window class encapsulates one window management interface.
 *
 * Implements window creation (with window data image) and display mapping
 * functionality.
 ****************************************************************************/
class GDI_Window
{
  //  declare GDI_Manager as friend
  friend class GDI_Manager;

public:
  /****************************************************************************/
  /** Operator new redefined to use OAL_MemmoryAlloc()
   *
   * \param aSize required memory size
   ****************************************************************************/
  void* operator new(size_t aSize);
  /****************************************************************************/
  /** Operator delete redefined to use OAL_MemmoryFree()
   *
   * \param apPtr pointer to memory to be freed
   ****************************************************************************/
  void operator delete(void* apPtr);

  GDI_Window();
  ~GDI_Window();

  /****************************************************************************/
  /** Creates window image data
   *
   * \param aImgDsc
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  LIB_RESULT ImageCreate(const GDI_ImageDescriptor &aImgDsc);

  /****************************************************************************/
  /** Sets window mapping
   *
   * \param mWndMapInfo
   ****************************************************************************/
  void MappingSet(GDI_W2dMapPrivate *mWndMapInfo);

  /****************************************************************************/
  /** Cancels window mapping to specified display
   *
   * \param aDisplayId
   ****************************************************************************/
  void MappingReset(GDI_DISPLAY_ID aDisplayId);

private:
  uint32_t mHandle;
  GDI_W2dMapPrivate *mDisplayMap[GDI_DISPLAY_ID_NUM]; //  mapped to which display/displays
  GDI_Image mImage; //  graphical representation of this window
  bool mImageShared;  // is the image data shared among client and server?
  bool mDelDiplayedBuffer;  // if the displayed buffer should be deleted when it's no longer displayed
};

/****************************************************************************/
/** GDI_Display struct encapsulates one window to display/framebuffer mapping
 *  information.
 *
 ****************************************************************************/
struct GDI_W2dMapPrivate
{
  /****************************************************************************/
  /** Operator new redefined to use OAL_MemmoryAlloc()
   *
   * \param aSize
   ****************************************************************************/
  void* operator new(size_t aSize);

  /****************************************************************************/
  /** Operator delete redefined to use OAL_MemmoryFree()
   *
   * \param apPtr pointer to memory to be freed
   ****************************************************************************/
  void operator delete(void* apPtr);

  /****************************************************************************/
  /** Constructor
   *
   ****************************************************************************/
  GDI_W2dMapPrivate();

  /****************************************************************************/
  /** Parametric constructor
   *
   * Uses more convenient mapping create message structure to cary all required
   * mapping info.
   *
   * \param apWindow  pointer to a window
   * \param apMsg     mapping create client message - with all required mapping
   *                  information
   ****************************************************************************/
  GDI_W2dMapPrivate(GDI_Window* apWindow, Gdi_Msg_MC* apMsg);

  /****************************************************************************/
  /** Parametric constructor
   *
   * All required has to be enumerated using methods parameters.
   *
   * \param apWindow            pointer to a window
   * \param aDisplayId          display to which the window should me mapped
   * \param aLayerId            to which layer the window should be mapped
   * \param aWindowRegionX
   * \param aWindowRegionY
   * \param aWindowRegionWidth
   * \param aWindowRegionHeight
   * \param aDisplayRegionX
   * \param aDisplayRegionY
   * \param aDisplayRegionWidth
   * \param aDisplayRegionHeight
   ****************************************************************************/
  GDI_W2dMapPrivate(GDI_Window* apWindow, GDI_DISPLAY_ID const aDisplayId,
      uint32_t const aLayerId, uint32_t const aWindowRegionX,
      uint32_t const aWindowRegionY, uint32_t const aWindowRegionWidth,
      uint32_t const aWindowRegionHeight, uint32_t const aDisplayRegionX,
      uint32_t const aDisplayRegionY, uint32_t const aDisplayRegionWidth,
      uint32_t const aDisplayRegionHeight);
  /****************************************************************************/
  /** Destructor
   ****************************************************************************/
  ~GDI_W2dMapPrivate();

  //  data mapping info
  GDI_Window *mpWnd; //  pointer to mapped window
  GDI_DISPLAY_ID mDisplayId;
  uint32_t mLayer; //  to which layer is the window mapped
  GDI_Rectangle mWindowRect;
  GDI_Rectangle mDisplayRect;
};

/****************************************************************************/
/** GDI_PSWBServer class encapsulates server part of window buffer sharing over
 *	OAL shm.
 *
 * Creates an interface for GDI window buffer sharing (server/clients) over
 * OAL shared memory regions.
 ****************************************************************************/
class GDI_OSWBServer
{
public:

private:
	static const uint32_t mscPoolSize;		///< size (in bytes) of the memory pool used for buffer allocation
	static uint32_t msPoolSizeAvailable;	///< number of bytes available for allocation
	
	static std::list<uint8_t> msFreeBufferList;	///< list of allocated indexes
	static OAL_SHARED_MEM mspIndex2ShmMap[GDI_MAX_OSWB_CNT];		///< table for mapping buffer index to OAL shm object

public:
	/****************************************************************************/
  /** Initializes sharing window buffers over OAL shared memory.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
	static LIB_RESULT Init();

	/****************************************************************************/
  /** Display control message handler.
   *
   * \param apMsg display control message structure with all required data
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
	static LIB_RESULT Close();
	static LIB_RESULT BufferGet(uint32_t aSize, uint32_t &arIdx);
	static LIB_RESULT BufferGet(uint32_t aIdx, void** appBuffer);
	static LIB_RESULT BufferDrop(uint32_t aIdx);	
	
}; // GDI_OSWBServer

/****************************************************************************/
/** GDI_PSWBclient class encapsulates client's part of window buffer sharing
 *	over OAL shm.
 *
 * Creates an interface for GDI window buffer sharing (server/clients) over
 * OAL shared memory regions.
 ****************************************************************************/
class GDI_OSWBClient
{
	public:

private:	
	static std::list<uint8_t> msFreeBufferList;	///< list of allocated indexes
	static OAL_SHARED_MEM mspIndex2ShmMap[GDI_MAX_OSWB_CNT];		///< table for mapping buffer index to OAL shm object

	/****************************************************************************/
  /** Display control message handler.
   *
   * \param apMsg display control message structure with all required data
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  //static LIB_RESULT Name2Index(const uint8_t* acpName, uint8_t &aIdx);
  static void Index2Name(uint8_t aIdx, uint8_t*& arpName);

public:
	static void Init();
	static LIB_RESULT Close();
		static LIB_RESULT BufferGet(uint32_t aSize, uint32_t aIdx);
	static LIB_RESULT BufferGet(uint32_t aIdx, void** appBuffer);
	static LIB_RESULT BufferDrop(uint32_t aIdx);
}; // GDI_OSWBClient

/****************************************************************************/
/** GDI_Manager class encapsulates GDI window drawing capability.
 *
 * Creates an interface for GDI window drawing and management.
 * Keeps track of created windows and their handles, opened displays and
 * window to display mappings.
 * Manages the Redraw event.
 ****************************************************************************/
class GDI_Manager
{
public:
  //GDI_Manager();
  //~GDI_Manager();

  //
  //  display management
  //

	static const char * const mppFBDesignations[];

private:
  static GDI_Display mDisplays[GDI_DISPLAY_ID_NUM]; ///< array of possible displays
  static const char *mpFbPath; ///< framebuffer device file path
  static const char *mpConPath; ///< console device file path

  //  enables/disables display by opening its frameBuffer device file
public:

  /****************************************************************************/
  /** Display control message handler.
   *
   * \param apMsg display control message structure with all required data
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT DisplayControl(Gdi_Msg_DC *apMsg);

  /****************************************************************************/
  /** Display control update message handler.
   *
   * \param apMsg display control message structure with all required data
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT DisplayControlUpdate(Gdi_Msg_DC *apMsg);

  /****************************************************************************/
  /** Display info get message handler.
   *
   * Sends information about specified display to client.
   *
   * \param apMsg display info message structure with all required data
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   *
   * \note Display has to be enabled to get valid resolution data.
   ****************************************************************************/
  static LIB_RESULT DisplayInfoGet(Gdi_Msg_DIG *apMsg);

  /****************************************************************************/
  /** Display color fill message handler.
   *
   * Works only with 16bit color data.
   *
   * \param apMsg display color fill message structure
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   *
   * \note For 16bit buffers only
   ****************************************************************************/
  static LIB_RESULT DisplayColorFill(Gdi_Msg_DCF *apMsg);

  /****************************************************************************/
  /** Display color fill 32 message handler.
   *
   * Works only with 32bit color data.
   *
   * \param apMsg display color fill message structure
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   *
   * \note For 32bit buffers only
   ****************************************************************************/
  static LIB_RESULT DisplayColorFill32(Gdi_Msg_DCF32 *apMsg);

  /****************************************************************************/
  /** Copies display image buffer data to specified memory.
   *
   * \param apMsg gdi message structure
   * \param data  pointer to the destination memory
   *
   * \return LIB_SUCCESS/LIB_FAILURE
   *
   *****************************************************************************/
  static LIB_RESULT DisplayBufferCopy(Gdi_Msg_DBC *apMsg, char* apData);

  /****************************************************************************/
  /** Redraw the specified display's framebuffer.
   *
   * Has to be called after any change to window content or mapping.
   *
   * \param aDisplayId  id of the display to be redrawn.
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   *
   ****************************************************************************/
  static LIB_RESULT DisplayRedraw(GDI_DISPLAY_ID const mDisplayId,
      uint32_t aLayerStart, const GDI_Rectangle &aRectangle);


  //
  //  windows management
  //

private:
  static std::map<uint32_t, GDI_Window*> mWindows; ///<  map of <window handle, window pointer> pairs
  static uint32_t mNextHandle; ///<  next window handle

public:
  //  for each display/frame-buffer an array (of MAX_NUM_OF_LAYERS length) of windows-to-frame-buffer-mappings
  static GDI_W2dMapPrivate *mWndMap[GDI_DISPLAY_ID_NUM][MAX_NUM_OF_LAYERS];

  //  create and destroy windows
  /****************************************************************************/
  /** Window create message handler.
   *
   * \param apMsg window create message struct with all required data
   * \return
   ****************************************************************************/
  static LIB_RESULT WindowCreate(Gdi_Msg_WC *apMsg);

  /****************************************************************************/
  /** Window destroy message handler
   *
   * \param apMsg window destroy message structure with all required data
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT WindowDestroy(Gdi_Msg_WD *apMsg);

  //  Create mapping
  /****************************************************************************/
  /** Mapping create message handler
   *
   * \param apMsg mapping create message structure with all required data
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT MappingCreate(Gdi_Msg_MC *apMsg);


  //  Destroy mapping
  /****************************************************************************/
  /** Mapping destroy message handler.
   *
   * \param apMsg mapping destroy message structure with all required data
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT MappingDestroy(Gdi_Msg_MD *apMsg);

  //  Modify mapping
  /****************************************************************************/
  /** Mapping modify message handler.
   *
   *  Only window and display rectangles are updated. No layer change possible.
   *
   * \param apMsg mapping create message structure with all required data
   *
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT MappingModify(Gdi_Msg_MC *apMsg);

  /****************************************************************************/
  /** Fills specified window region with defined color.
   *
   * server side
   *
   * \param apMsg color fill info packed to gdi message
   * \return LIB_SUCCESS if all ok, otherwise LIB_FAILURE
   *
   * \note works with rgb565 color format
   *****************************************************************************/
  static LIB_RESULT WindowColorFill(Gdi_Msg_WCF *apMsg);

  /****************************************************************************/
  /** Updates window content with specified data.
   *
   * Copies the specified data to window image.
   *
   * \param apMsg gdi message structure
   * \param apData  pointer to new image data
   * \return LIB_SUCCESS/LIB_FAILURE
   *****************************************************************************/
  static LIB_RESULT WindowUpdate(Gdi_Msg_WU *apMsg, uint8_t *apData);

  /****************************************************************************/
  /** Updates window region content with specified data.
   *
   * Copies the specified data to window image region.
   *
   * \param apMsg gdi message structure
   * \param apData  pointer to new image data
   * \return LIB_SUCCESS/LIB_FAILURE
   *****************************************************************************/
  static LIB_RESULT WindowUpdateRegion(Gdi_Msg_WUR *apMsg, uint8_t* apData);

#ifdef GDI_SWB
  /****************************************************************************/
  /** Gets window compatible buffer with data allocated in contiguous mem region.
   *
   * \param apMsg pointer to message data
   *
   * \return LIB_SUCCESS/LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT WindowBufferGet(Gdi_Msg_WBG *apMsg);

  /****************************************************************************/
  /** Sets window buffer allocated in contiguous mem region.
   *
   * \param apMsg pointer to message data
   * \return LIB_SUCCESS/LIB_FAILURE
   *
   * \note the buffer should be previously acquired using WindowBufferGet().
   ****************************************************************************/
  static LIB_RESULT WindowBufferSet(Gdi_Msg_WBS *apMsg);

  /****************************************************************************/
  /** Releases resources connected to this window buffer.
   *
   * \param apMsg pointer to message data
   *
   * \return LIB_SUCCESS/LIB_FAILURE
   ****************************************************************************/
  static LIB_RESULT WindowBufferDrop(Gdi_Msg_WBD *apMsg);
#endif // ifdef GDI_SWB

	//
	// Maintenance
	//

	/****************************************************************************/
  /** Initializes members.
   *
   * \return LIB_SUCCESS if all ok else LIB_FAILURE.
   ****************************************************************************/
  static LIB_RESULT Init();
	
  /****************************************************************************/
  /** Frees any forgotten resources.
   *
   * \return LIB_SUCCESS if all ok else LIB_FAILURE.
   ****************************************************************************/
  static LIB_RESULT Clean();
};

#endif /* GDIPRIVATE_HPP */
