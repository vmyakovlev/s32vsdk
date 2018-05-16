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
#ifndef ACFPROCESS_H
#define ACFPROCESS_H

#include <../libs/apex/acf/include/acf_process_portinfo.h>
#include <icp_data.h>
#include <acf_common.h>
#include <stdint.h>
#include <string.h>
#include <vector>

#include <umat.hpp>
#include <../libs/apex/acf/include/acf_datadesc.hpp>

class ACF_Process
{
public:
   
   ACF_Process();
   virtual ~ACF_Process();

   /////////////////////////////////////////////////////////////////////////
   /// Connect the data region described by "lDataDesc" to graph
   /// port "lPortIdentifier".
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[in] lDataDesc
   /// Description of contiguous data region.
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t ConnectIO(std::string lPortIdentifier, icp::DataDescriptor& lDataDesc);

   /////////////////////////////////////////////////////////////////////////
   /// Connect the data region described by "lUmat" to graph
   /// port "lPortIdentifier".
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[in] lUmat
   /// Description of contiguous data region.
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t ConnectIO(std::string lPortIdentifier, vsdk::UMat& lUmat);

   /////////////////////////////////////////////////////////////////////////
   /// Connect the region of interest (ROI) described by "lDataDesc",
   /// "lROI_XOffset", "lROI_YOffset", "lROI_Width", and "lROI_Height"
   /// to graph port "lPortIdentifier".
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[in] lDataDesc
   /// Description of contiguous data region that 'contains' the ROI.
   ///
   /// \param[in] lROI_XOffset
   /// The X offset of the top left corner of the ROI (relative to the top left
   /// corner of the region described by "lDataDesc").
   ///
   /// \param[in] lROI_YOffset
   /// The Y offset of the top left corner of the ROI (relative to the top left
   /// corner of the region described by "lDataDesc").
   ///
   /// \param[in] lROI_Width
   /// The width of the ROI.
   ///
   /// \param[in] lROI_Height
   /// The height of the ROI.
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t ConnectIO_ROI(std::string lPortIdentifier,
                         icp::DataDescriptor& lDataDesc,
                         int32_t lROI_XOffset,
                         int32_t lROI_YOffset,
                         int32_t lROI_Width,
                         int32_t lROI_Height);

   /////////////////////////////////////////////////////////////////////////
   /// Connect the region of interest (ROI) described by "lUmat",
   /// "lROI_XOffset", "lROI_YOffset", "lROI_Width", and "lROI_Height"
   /// to graph port "lPortIdentifier".
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[in] lUmat
   /// Description of contiguous data region that 'contains' the ROI.
   ///
   /// \param[in] lROI_XOffset
   /// The X offset of the top left corner of the ROI (relative to the top left
   /// corner of the region described by "lUmat").
   ///
   /// \param[in] lROI_YOffset
   /// The Y offset of the top left corner of the ROI (relative to the top left
   /// corner of the region described by "lUmat").
   ///
   /// \param[in] lROI_Width
   /// The width of the ROI.
   ///
   /// \param[in] lROI_Height
   /// The height of the ROI.
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t ConnectIO_ROI(std::string lPortIdentifier,
                         vsdk::UMat& lUmat,
                         int32_t lROI_XOffset,
                         int32_t lROI_YOffset,
                         int32_t lROI_Width,
                         int32_t lROI_Height);

   /////////////////////////////////////////////////////////////////////////
   /// Connect a 2D array of chunks specified by "lChunkOffsetArray" and "lSrcData" to
   /// graph input port "lPortIdentifier".  "lChunkOffsetArray" is a 2D array of
   /// 32-bit offsets (in units of bytes) that when added to the start of "lSrcData"
   /// region, result in valid pointers to the top left corners of the desired chunks of
   /// data in memory.  Note that chunk size is set via ACF_Process_Desc_APU::SetInputChunkSize(...)
   /// and all chunks are assumed to be the same size.
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[in] lSrcData
   /// Reference to region of source data.  The start of this memory region
   /// is effectively the 'base address' that will be used with the chunk
   /// offsets specified in "lChunkOffsetArray".
   ///
   /// \param[in] lChunkOffsetArray
   /// Reference to 2D array of chunk offsets.  Offsets should be relative
   /// to the start of the contiguous data region specified by "lSrcData",
   /// should address the upper left corners of the desired 2D chunks,
   /// and should be in units of bytes.
   /// NOTE: the number of offsets in the horizontal dimension must be a multiple
   /// of 4 (e.g. an offset array with dimensions 8x4 is allowed, but an array
   /// with dimensions 10x4 will result in an error).
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t ConnectIndirectInput(std::string lPortIdentifier,
                                icp::DataDescriptor& lSrcData,
                                icp::DataDescriptor& lChunkOffsetArray);
   
   /////////////////////////////////////////////////////////////////////////
   /// Connect a 2D array of chunks specified by "lChunkOffsetArray" and "lSrcData" to
   /// graph input port "lPortIdentifier".  "lChunkOffsetArray" is a 2D array of
   /// 32-bit offsets (in units of bytes) that when added to the start of "lSrcData"
   /// region, result in valid pointers to the top left corners of the desired chunks of
   /// data in memory.  Note that chunk size is set via ACF_Process_Desc_APU::SetInputChunkSize(...)
   /// and all chunks are assumed to be the same size.
   ///
   /// \param[in] lPortIdentifier
   /// Graph port identifier.
   ///
   /// \param[in] lSrcData
   /// Reference to region of source data.  The start of this memory region
   /// is effectively the 'base address' that will be used with the chunk
   /// offsets specified in "lChunkOffsetArray".
   ///
   /// \param[in] lChunkOffsetArray
   /// Reference to 2D array of chunk offsets.  Offsets should be relative
   /// to the start of the contiguous data region specified by "lSrcData",
   /// should address the upper left corners of the desired 2D chunks,
   /// and should be in units of bytes.
   /// NOTE: the number of offsets in the horizontal dimension must be a multiple
   /// of 4 (e.g. an offset array with dimensions 8x4 is allowed, but an array
   /// with dimensions 10x4 will result in an error).
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t ConnectIndirectInput(std::string lPortIdentifier,
                                vsdk::UMat& lSrcData,
                                vsdk::UMat& lChunkOffsetArray);

   /////////////////////////////////////////////////////////////////////////
   /// [DEPRECATED] Globally indicate how much data beyond 2D input borders should be taken into account for
   /// the region of interest (ROI) case.  If these are set to non-zero values,
   /// the indicated data must be available on the borders of ALL applicable inputs.
   ///
   /// \param[in] lRoiInfoL
   /// Number of source elements available beyond the left border of the 2D input region.
   /// It must be a multiple of chunk width.
   ///
   /// \param[in] lRoiInfoR
   /// Number of source elements available beyond the right border of the 2D input region.
   /// It must be a multiple of chunk width.
   ///
   /// \param[in] lRoiInfoT
   /// Number of source elements available beyond the top border of the 2D input region.
   /// It must be a multiple of chunk height.
   ///
   /// \param[in] lRoiInfoB
   /// Number of source elements available beyond the bottom border of the 2D input region.
   /// It must be a multiple of chunk height.
   ///
   /// \return
   /// 0 if successful, non-zero if an an error occurred.
   /////////////////////////////////////////////////////////////////////////
   int32_t SetRoiInfo(int32_t lRoiInfoL, int32_t lRoiInfoR, int32_t lRoiInfoT, int32_t lRoiInfoB);

   //**For development/internal use only; not to be publicly exposed
   int32_t ConnectIO(std::string lPortIdentifier, const ACF_DataDesc& lDataDesc);
   int32_t ConnectIO_ROI(std::string lPortIdentifier,
                         const ACF_DataDesc& lDataDesc,
                         int32_t lROI_XOffset,
                         int32_t lROI_YOffset,
                         int32_t lROI_Width,
                         int32_t lROI_Height);
   int32_t ConnectIndirectInput(std::string lPortIdentifier,
                                const ACF_DataDesc& lSrcData,
                                const ACF_DataDesc& lChunkOffsetArray);
   ACF_Process_PortInfo* RetPortInfo(std::string lPortIdentifier);

   virtual int32_t Initialize() = 0;
   virtual int32_t Start()      = 0;
   virtual int32_t Wait()       = 0;

   int32_t mWaitTimeoutInUs; //this is currently 'semi-exposed' and can be overridden if the default value isn't acceptable

protected:

   void SetProcessIdentifier(std::string lProcessIdentifier);
   std::string RetProcessIdentifier();
   void AddPort(std::string lPortIdentifier, icp::DATATYPE lE0DataType, int32_t lE0x, int32_t lE0y,
                int32_t lChunkWidth, int32_t lChunkHeight,
                int32_t lInputOutput,
                int32_t lNonstaticStatic,
                int32_t lDirectIndirect,
                int32_t lNonfixedFixed,
                int32_t lRegFifo,
                int32_t lLmIndex,
                int32_t lSDInfoN,
                int32_t lSDInfoS,
                int32_t lSDInfoE,
                int32_t lSDInfoW,
                int32_t lVerDepOverlapFlag);

   void FlagAsInitialized(){mInitializedFlag = 1;}
   int32_t Initialized(){return mInitializedFlag;}
   void FlagSpatialDep(){mSpatialDepFlag = 1;}


   int32_t QueryIoSizeChangeFlag() {return mIoSizeChangeFlag;}
   void ClearIoSizeChangeFlag() {mIoSizeChangeFlag = 0;}

   std::string mProcessIdentifier;
   std::vector<ACF_Process_PortInfo> mPortInfoVec;
   int32_t mProcessStartedFlag;
   int32_t mErrorState;
   int32_t mRoiInfoL;
   int32_t mRoiInfoR;
   int32_t mRoiInfoT;
   int32_t mRoiInfoB;
   int32_t mRoiInfoManualFlag; //set to 1 if the user explicitly configures ROI info with SetRoiInfo(...)

   int32_t mInitializedFlag; //set to 1 once initialization has been called
   int32_t mSpatialDepFlag; //set to 1 if there are spatial dep present in the graph (to help with 1D/2D decision)
   int32_t mIoSizeChangeFlag; //set to 1 if an IO is connected that differs from a previously connected IO in a significant way (e.g. width, height)
};

#endif /* ACFPROCESS_H */

