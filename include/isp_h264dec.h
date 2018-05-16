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
* \file     h264dcd_public.h
* \brief    public declarations for H264DEC user space library
* \author   Tomas Babinec
* \version  0.1
* \date     7-July-2016
* \note
****************************************************************************/

#ifndef H264DECPUBLIC_H
#define H264DECPUBLIC_H

#include "../libs/isp/h264dec/kernel/common/include/h264dcd_types.h"
#include "../libs/isp/h264dec/kernel/linux/include/h264dcd_linux.h"

//*****************************************************************************
// constants
//*****************************************************************************
#define H264DEC_STRM_CNT 4 ///< maximum number of H264decoder supported streams

//*****************************************************************************
// types
//*****************************************************************************

/****************************************************************************/
/** H264 decoder user space library return values.
 *
 ****************************************************************************/
enum H264DEC_LIB_RESULT
{
    H264DEC_LIB_SUCCESS = 0,
    H264DEC_LIB_FAILURE = 1
}; // H264DEC_LIB_RESULT

//*****************************************************************************
// public functionality declarations
//*****************************************************************************

/****************************************************************************/
/** Prepares the H264DEC driver to be used.
 *
 * In Linux the h264dcd special device file is opened.
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_Open(void);

/****************************************************************************/
/** Deinitializes the H264DEC driver use.
 *
 * In Linux the h264dcd special device file is closed.
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_Close(void);

/****************************************************************************/
/** Configures H264 decoder input.
 *
 * \param arcH264DecInConfig  structure containing input configuration
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_InConfig(
                     const H264DCD_STREAM_DATA_CONFIG &arcH264DecInConfig);

/****************************************************************************/
/** Sets up wattermark level.
 *
 * \param arcWtmLevel  wattermark level to be set
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_WtmLevelSet(const H264DCD_FIFO_WTM &arcWtmLevel);

/****************************************************************************/
/** Configures H264 timeout.
 *
 * \param acTimeout  structure containing input configuration
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_TimeoutSet(const uint16_t acTimeout);

/****************************************************************************/
/** Enables H264 timeout.
 *
 * \param arcToEn  which stream timeout to be enabled
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_TimeoutEnable(const H264DCD_TIMEOUT_ENABLE &arcToEn);

/****************************************************************************/
/** Invokes SW reset.
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_SwReset(void);

/****************************************************************************/
/** Configures H264 decoder output.
 *
 * \param arcH264DecOutConfig  structure containing output configuration
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_OutConfig(
                     const H264DCD_OUTPUT_BUFFCFG &arcH264DecOutConfig);

/****************************************************************************/
/** Sets sample mode of the output related registers.
 *
 * \param arcOutSampleMode  mode to be set
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_OutSampleModeSet(
                     const H264DCD_OUTPUT_SAMPLEMODE &arcOutSampleMode);

/****************************************************************************/
/** Sets the color format and component samples' bit with for given stram.
 *
 * \param arcOutColData  pointer to config structure
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_OutColorDataSet(
                     const H264DCD_OUTPUT_COLOURDATA &arcOutColData);

/****************************************************************************/
/** Set the data flow mode.
 *
 * \param acDataFlowMode  data flow mode (MODE_0/1/2)
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_DataFlowModeSet(
                     const H264DCD_DATAFLOW_MODE acDataFlowMode);

/****************************************************************************/
/** Set the bank stride.
 *
 * \param arcBankStride  bank stride config
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_BankStrideSet(
                     const H264DCD_OUTPUT_BKSTRIDE &arcBankStride);

/****************************************************************************/
/** Configures encoded frame size in terms of macroblocks.
 *
 * \param arcFrmSize  frame size
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_InFrameSizeSet(
                     const H264DCD_INFRAME_SIZE &arcFrmSize);

/****************************************************************************/
/** Stops the selected channel switching it to idle mode.
 *
 * \param arcChnlCtrl  parameters in a structure
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_ChannelStop(
                     const H264DCD_CH_CONTROL &arcChnlCtrl);

/****************************************************************************/
/** Sets up deblocking and the memory type (SRAM/DDR) used for reference data.
 *
 * \param arcDeblockCfg  parameters in a structure
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_DeblockSet(
                     const H264DCD_DECOD_CONFIG &arcDeblockCfg);

/****************************************************************************/
/** Sets up 3 thresholding levels for the decoder core.
 *
 * \param arcThreshLevels  parameters in a structure
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_ThreshLevelsSet(
                     const H264DCD_DECOD_THRLEVELS &arcThreshLevels);

/****************************************************************************/
/** Sets up reference memory address.
 *
 * \param arcRefMemory  parameters in a structure
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_RefMemorySet(
                     const H264DCD_DECOD_REFMEM &arcRefMemory);

/****************************************************************************/
/** Gets status of specified channel fifo.
 *
 * \param arStatus  fifo status struct for particular channel
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_FifoStatusGet(H264DCD_STREAM_FIFO_STATUS &arStatus);

/****************************************************************************/
/** Sets up IRQ mask.
 *
 * \param acIrqMask  number of lines till reset
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_IrqMaskSet(const uint32_t acIrqMask);

/****************************************************************************/
/** Reads current IRQ mask setup.
 *
 * \param arIrqMask  number of lines till reset
 *
 * \return H264DEC_LIB_SUCCESS if all ok
 *         H264DEC_LIB_FAILURE if the close fails
 ****************************************************************************/
H264DEC_LIB_RESULT H264DEC_IrqMaskGet(uint32_t &arIrqMask);

#endif /* H264DECPUBLIC_H */

//****************************************************************************
/* EOF */