/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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
 * \file    seq_graph_meta.h
 * \brief   ISP Graph metadata formats
 * \author
 * \version 0.2
 * \date    11-Feb-2016
 ****************************************************************************/
 
#ifndef SEQGRAPHMETA_H
#define SEQGRAPHMETA_H

#define	MIPI_VIRT_CHN_NUM	4

#define CA53 
#include "seq_hw.h"

#define NUM_MIPI 2
#define NUM_VIU 2

typedef struct Object_t
{
  const void* mcpAddr;
  const uint32_t mcSize;
  const uint32_t mcOffset;
} Object_t;

typedef struct GraphLimits_t
{
  const uint32_t mcCSINumLines; // obsolete
} GraphLimits_t;

typedef enum CSI_CameraType_t
{
    CSI_NONE,
    CSI_SONY224,
    CSI_SONY290,
    CSI_SONY390,
    CSI_OV2775,
    CSI_OV9716,
    CSI_OV10640,
    CSI_GENERIC,
    CSI_MAX_GENERIC,
    CSI_MAX_SONY224,
    CSI_MAX_OV9716,
    CSI_MAX_OV10635,
    CSI_MAX_OV10640,
    CSI_PAN34420,
	CSI_MAX9286_96705,
} CSI_CameraType_t;

typedef enum VIU_CameraType_t
{
    VIU_NONE,
    VIU_OV10635,
    VIU_OV10640
} VIU_CameraType_t;

typedef struct GraphMipiCSIParam_t
{
    enum CSI_CameraType_t mcCameraType;
    const uint16_t mcNumCameras;
    const uint16_t mcOutputBytesOffset[MIPI_VIRT_CHN_NUM];
    const uint16_t mcBytesToTransfer[MIPI_VIRT_CHN_NUM];
} GraphMipiCSIParam_t;

typedef struct GraphViuParam_t
{
    enum VIU_CameraType_t mcCameraType;
    const uint16_t mcOutputBytesOffset;
    const uint16_t mcBytesToTransfer;
} GraphViuParam_t; 

typedef struct GraphMetadata_t
{
  const uint32_t  mMaxConsumer;
  const uint32_t  mGraphSize;
  const Object_t *mpObjectList;
  const uint32_t  mObjCnt;
  const uint32_t *mpPointerList;
  const uint32_t  mPtrCnt;
  const uint32_t *mpBufferList;
  const uint32_t  mBfrCnt;
  const GraphLimits_t mcGraphLimits;
  const GraphMipiCSIParam_t mcMipiCSIParams[NUM_MIPI];
  const GraphViuParam_t mcViuParams[NUM_VIU];
  const char    **mcppFdmaChannelNames;
  const char    **mcppBufferNames;
} GraphMetadata_t;

#endif /* SEQGRAPHMETA_H */

