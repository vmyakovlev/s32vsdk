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

#ifndef JPEGDECPUBLIC_H
#define JPEGDECPUBLIC_H

#include "../libs/isp/jpegdec/kernel/common/include/jpegdcd_types.h"

/****************************************************************************/
/** JPEG decoder user space library return values.
 *
 ****************************************************************************/
enum JPEGDEC_LIB_RESULT
{
  JPEGDEC_LIB_SUCCESS = 0,
  JPEGDEC_LIB_FAILURE = 1
}; // JPEGDEC_LIB_RESULT


//*****************************************************************************
// public functionality declarations
//*****************************************************************************
/****************************************************************************/
/** Prepares the JPEG Decoder driver to be used.
 *
 * In Linux the jpegdcd special device file is opened.
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if opening of the device file fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_Open(void);


/****************************************************************************/
/** Release the JPEG Decoder driver use.
 *
 * In Linux the jpegdcd special device file is closed.
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_Close(void);


/****************************************************************************/
/** Initializes JPEG Decoder configuration.
 *
 * Should be invoked as the first call before the JPEG registers are configured.
 * Part of HW workaround.
 *
 * \note NOTE
 * For correct behaviour of JPEG_E_STATUS[TIMEOUT_ERR], following programming
 * sequence is recommended:
 * 1. Set the JPEG_TIMEOUT[TIMEOUT_VAL] to 1.
 * 2. Set the JPEG_W_CTRL[SW_RST] bit to 1. This will clear the counter.
 * 3. Set the JPEG_W_CTRL[SW_RST] bit to 0. Now timeout counter will stop at
 *    value 1.
 * 4. Program the rest of the JPEG registers.
 * 5. Program the JPEG_TIMEOUT register to required value.
 * 6. Set the JPEG_CTRL[GO] bit to 1.
 *
 *
 * In Linux the jpegdcd special device file is closed.
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_ConfigStart(void);


/****************************************************************************/
/** Configures JPEG Decoder timeout.
 *
 * Should be called after all JPEG configuration was done, just before asserting
 * GO bit.
 * Part of HW workaround.
 *
 * \param acTimeout  structure containing input configuration
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_TimeoutSet(const uint32_t acTimeout);


/****************************************************************************/
/** Configures JPEG Decoder input.
 *
 * \param apcJpegDecInConfig  structure containing input configuration
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_InConfig(const STREAM_DATA_CONFIG *apcJpegDecInConfig);


/****************************************************************************/
/** Sets up watermark level.
 *
 * \param acWtmLevel  watermark level to be set
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_WtmLevelSet(const uint8_t acWtmLevel);


/****************************************************************************/
/** Configures JPEG Decoder output.
 *
 * \param apcJpegDecOutConfig  structure containing output configuration
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_OutConfig(const OUTPUT_BUFFCFG *apcJpegDecOutConfig);


/****************************************************************************/
/** Sets sample mode of the output related registers.
 *
 * \param acOutSampleMode  JPEGDCD_ON = all registers are sampled at frame done
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_OutSampleModeSet(const JPEGDCD_BOOL acOutSampleMode);


/****************************************************************************/
/** Modifies bank stride setup.
 *
 * \param acBytesNum  bank stride in bytes
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_BankStrideSet(const uint8_t acBytesNum);


/****************************************************************************/
/** Sets the image size and the components sampling factors.
 *
 * \param acpStreamConfig  parameters in a structure
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_StreamConfig(const STREAM_JPEG_PARAM *acpStreamConfig);


/****************************************************************************/
/** Sets the mode used.
 *
 * \param acMode  mode to be used for the image decoding
 *                CONTEXT_SWITCH => stream switching per reset interval
 *                COMPATIBILITY  => stream switching per frame
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_ModeSet(const COMPAT_MODE acMode);

/****************************************************************************/
/** Sets the number of streams used.
 *
 * \param acNumOfStreams  number of streams used
 *                        if 1 the compatibility mode is also set.
 *                        if 2,3 or 4 the context switch mode is set
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_StreamNumSet(const uint8_t acNumOfStreams);

/****************************************************************************/
/** Invokes SW reset.
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_SwReset(void);


/****************************************************************************/
/** Sets the stream reset interval in terms of number of image lines.
 *
 * \param acRstInterval  number of lines till reset
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_RstIntervalSet(const uint16_t acRstInterval);


/****************************************************************************/
/** Invokes decoding.
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_Start(void);


/****************************************************************************/
/** Stops decoding.
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_Stop(void);


/****************************************************************************/
/** Sets up IRQ mask.
 *
 * \param aIrqMask  number of lines till reset
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_IrqMaskSet(const uint32_t aIrqMask);


/****************************************************************************/
/** Reads current IRQ mask setup.
 *
 * \param aIrqMask  number of lines till reset
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_IrqMaskGet(uint32_t &aIrqMask);


/****************************************************************************/
/** Reads configuration status register.
 *
 * \param arCfgErr  configuration error structure
 *
 * \return JPEGDEC_LIB_SUCCESS if all ok
 *         JPEGDEC_LIB_FAILURE if the close fails
 ****************************************************************************/
JPEGDEC_LIB_RESULT JPEGDEC_CfgErrGet(JPEGDCD_CFGERR &arCfgErr);


#endif /* JPEGDECPUBLIC_H */

