/*****************************************************************************
 *
 * NXP Confidential Proprietary
 *
 * Copyright (c) 2016 Freescale Semiconductor;
 * Copyright 2017 NXP
 * All Rights Reserved
 *
 *****************************************************************************
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

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <string>

#include "s32vs234.h"
#include "vdb_log.h"
#include "isp_hw.h"
#include "isp_jpegdec.h"
#include "jpegdcd_linux.h"

#define VDB_LOG_NOTE_OFF

const std::string JPEGDecDeviceName("jpegdcd");
const std::string JPEGDecDeviceFile("/dev/jpegdcd");

static int32_t sJpegDecFd = -1;    ///< JPEG driver file descriptor


/*============================================================================*/
/**
* @brief          Open JPEG Decoder device
* @details        The function opens the JPEG Decoder device.
*
* @param[in]      void
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_Open_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_Open(void)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        sJpegDecFd = open(JPEGDecDeviceFile.c_str(), 0);
        if (sJpegDecFd == -1)
        {
            VDB_LOG_ERROR("Can't open device file: %s.\n", JPEGDecDeviceName.c_str());
            lRet = JPEGDEC_LIB_FAILURE;
        } // if open failed
    } // if not opened yet
    else
    {
        VDB_LOG_WARNING("Device file: %s opened already.\n", JPEGDecDeviceName.c_str());
    } // else from if not opened yet

    return lRet;
} // JPEGDEC_Reserve()


/*============================================================================*/
/**
* @brief          Close JPEG Decoder device
* @details        The function closes the JPEG Decoder device.
*
* @param[in]      void
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_Close_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_Close(void)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd != -1)
    {
        if (close(sJpegDecFd) != 0)
        {
            VDB_LOG_ERROR("Failed to close device file %s.\n", JPEGDecDeviceName.c_str());
            lRet = JPEGDEC_LIB_FAILURE;
        } // if open failed
        else
        {
            sJpegDecFd = -1;
        } // else if open failed
    } // not openned yet
    else
    {
        VDB_LOG_WARNING("JpegDec already closed.\n");
    } // else from if not openned yet

    return lRet;
} // JPEGDEC_Release()


/*============================================================================*/
/**
* @brief          Workaround for setting timeout for decoding operation
* @details        The function sets the timeout to 1 an after this
*                 it resets the JPEG decoder
*
* @param[in]      void
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_ConfigStart_Activity
*
* @note           It shall be used before to program the JPEG core registers
*/
JPEGDEC_LIB_RESULT JPEGDEC_ConfigStart(void)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_CFGSTART) < 0L)
        {
            VDB_LOG_ERROR("Failed to start JPEGDEC configuration.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if ioctl failed
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_ConfigStart()


/*============================================================================*/
/**
* @brief          Set timeout for decode operation
* @details        The function sets the JPEG error timeout measured
*                 in JPEG clock cycles.
*
* @param[in]      acTimeout    timeout for decode operation
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre            Call JPEGdcd_jpeg_configstart
* @post
*
* @implements     JPEGDEC_TimeoutSet_Activity
*
* @note           It shall be called after all the JPEG registers are
*                  programmed and before the JPEG decoder to be enabled
*/
JPEGDEC_LIB_RESULT JPEGDEC_TimeoutSet(const uint32_t acTimeout)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_TIMEOUT_SET,
                  &acTimeout) < 0L)
        {
            VDB_LOG_ERROR("Failed to set timeout.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if ioctl failed
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_TimeoutSet(const uint32_t acTimeout)


/*============================================================================*/
/**
* @brief          Set the parameters of the data source
* @details        The function sets address and size of the input data packet
*                 for the specified input stream
*
* @param[in]      apcJpegDecInConfig   STREAM_DATA_CONFIG structure
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_InConfig_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_InConfig(const STREAM_DATA_CONFIG *apcJpegDecInConfig)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(apcJpegDecInConfig != NULL)
    {
        if(sJpegDecFd == -1)
        {
            VDB_LOG_ERROR("JpegDec device file not opened.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if JpegDec not opened
        else
        {
            if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_INPUTSTREAM_CFG,
                      apcJpegDecInConfig) < 0L)
            {
                VDB_LOG_ERROR("Failed to config JPEGDEC input data.\n");
                lRet = JPEGDEC_LIB_FAILURE;
            }
        } // else from if JpegDec not opened
    } // if apcJpegDecInConfig exists

    return lRet;
} // JPEGDEC_InConfig(const STREAM_DATA_CONFIG *apcJpegDecInConfig)


/*============================================================================*/
/**
* @brief          Set the watermark level.
* @details        The function sets the watermark level for
*                 specified FIFO
*
* @param[in]      acWtmLevel        The watermark level
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_WtmLevelSet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_WtmLevelSet(const uint8_t acWtmLevel)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_FIFO_WATERMARK_SET,
                  &acWtmLevel) < 0L)
        {
            VDB_LOG_ERROR("Failed to config JPEGDEC watermark level.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        }
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_WtmLevelSet(const uint8_t acWtmLevel)


/*============================================================================*/
/**
* @brief          Set the parameters for output data
* @details        The function sets for each image component the buffer
*                 address and the number of lines per component used in
*                 the output circular component buffer
*
* @param[in]      apcJpegDecOutConfig    OUTPUT_BUFFCFG structure
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_OutConfig_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_OutConfig(const OUTPUT_BUFFCFG *apcJpegDecOutConfig)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(apcJpegDecOutConfig != NULL)
    {
        if(sJpegDecFd == -1)
        {
            VDB_LOG_ERROR("JpegDec device file not opened.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if JpegDec not opened
        else
        {
            if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_OUTSTREAM_CFG,
                      apcJpegDecOutConfig) < 0L)
            {
                VDB_LOG_ERROR("Failed to config JPEGDEC output.\n");
                lRet = JPEGDEC_LIB_FAILURE;
            }
        } // else from if JpegDec not opened
    }// if apcJpegDecCfg exists

    return lRet;
} // JPEGDEC_OutConfig(const OUTPUT_BUFFCFG *apcJpegDecOutConfig)


/*============================================================================*/
/**
* @brief          Change resampling mode
* @details        The function sets sampling mode according to
*                 outputregsmpl_mode variable
*
* @param[in]      acOutSampleMode    boolean
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_OutSampleModeSet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_OutSampleModeSet(const JPEGDCD_BOOL acOutSampleMode)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_OUTSAMPLEMODE_SET,
                  &acOutSampleMode) < 0L)
        {
            VDB_LOG_ERROR("Failed to config JPEGDEC output.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if ioctl failed
        else
        {
            // restart JPEG configuration
            lRet = JPEGDEC_ConfigStart();
        } // else from if ioctl failed
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_OutSampleModeSet(const JPEGDCD_BOOL acOutSampleMode)


/*============================================================================*/
/**
* @brief          Set bankstride
* @details        The function sets the address offset of each
*                 start of macroblock
*
* @param[in]      acBytesNum    Address offset
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_BankStrideSet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_BankStrideSet(const uint8_t acBytesNum)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_BANKSTRIDE_SET,
                  &acBytesNum) < 0L)
        {
            VDB_LOG_ERROR("Failed to config JPEGDEC bankstride.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        }
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_BankStrideSet(const uint8_t acBytesNum)


/*============================================================================*/
/**
* @brief          Set the configuration parameters for data stream
* @details        The function sets the image size and the components
*                 sampling factors for the selected input stream
*
* @param[in]      acpStreamConfig     STREAM_JPEG_PARAM structure
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_StreamConfig_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_StreamConfig(const STREAM_JPEG_PARAM *acpStreamConfig)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(acpStreamConfig != NULL)
    {
        if(sJpegDecFd == -1)
        {
            VDB_LOG_ERROR("JpegDec device file not opened.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if JpegDec not opened
        else
        {
            if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_STREAMCFG_SET,
                      acpStreamConfig) < 0L)
            {
                VDB_LOG_ERROR("Failed to config JPEGDEC stream.\n");
                lRet = JPEGDEC_LIB_FAILURE;
            }
        } // else from if JpegDec not opened
    } // if stram config valid
    else
    {
        VDB_LOG_ERROR("Stream config structure is NULL.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    }

  return lRet;
} // JPEGDEC_StreamConfig(const STREAM_JPEG_PARAM *acpStreamConfig)


/*============================================================================*/
/**
* @brief          Set the JPEG Decoder mode
* @details        The function sets the mode:
*                 - 1 compatibility mode is also set
*                 - otherwise (2,3,4) the context switch mode is set
*
* @param[in]      acMode    JPEG Decoder working mode
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_ModeSet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_ModeSet(const COMPAT_MODE acMode)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_MODE_SET,
                  &acMode) < 0L)
        {
            VDB_LOG_ERROR("Failed to config JPEGDEC mode.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        }
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_ModeSet(const COMPAT_MODE acMode)


/*============================================================================*/
/**
* @brief          Set the numbers of streams
* @details        The function sets the number of active streams
*
* @param[in]      acNumOfStreams      Number of active streams
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_StreamNumSet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_StreamNumSet(const uint8_t acNumOfStreams)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_STREAMNUM_SET,
                  &acNumOfStreams) < 0L)
        {
            VDB_LOG_ERROR("Failed to config JPEGDEC stream count.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        }
    } // else from if JpegDec not opened

      return lRet;
} // JPEGDEC_StreamNumSet(const uint8_t acNumOfStreams)


/*============================================================================*/
/**
* @brief          SW reset
* @details        The function sets and resets the SW_RST bit resetting
*                 the Input DMA Wrapper, the Output DMA Wrapper and
*                 the JPEG Decoder Core
*
* @param[in]      void
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_SwReset_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_SwReset(void)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_SW_RESET) < 0L)
        {
              VDB_LOG_ERROR("Failed to SW reset the JPEGDEC.\n");
              lRet = JPEGDEC_LIB_FAILURE;
        }
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_SwReset()


/*============================================================================*/
/**
* @brief          Set the restart interval
* @details        The function sets the restart interval in terms
*                 of image lines
*
* @param[in]      acRstInterval   Number of lines
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_RstIntervalSet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_RstIntervalSet(const uint16_t acRstInterval)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_RSTINTERVAL_SET,
                  &acRstInterval) < 0L)
        {
            VDB_LOG_ERROR("Failed to config JPEGDEC reset interval.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        }
    } // else from if JpegDec not opened

      return lRet;
} // JPEGDEC_RstIntervalSet(const uint16_t acRstInterval)


/*============================================================================*/
/**
* @brief          Start decode operations
* @details        The function sets the GO bit in order to start the
*                 decoding activity according to JPEGDCD_BOOL input variable
*
* @param[in]      void
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre            Call JPEGdcd_jpeg_swreset & JPEGdcd_jpeg_configstart &
*                 JPEGdcd_jpeg_timeoutset
* @post
*
* @implements     JPEGDEC_Start_Activity
*
* @note           To be used only after hw or sw reset to enable JPEG core
*/
JPEGDEC_LIB_RESULT JPEGDEC_Start(void)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        JPEGDCD_BOOL lVal  = JPEGDCD_ON;
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_DCDENABLE,
                &lVal) < 0L)
        {
            VDB_LOG_ERROR("Failed to start JPEGDEC operation.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if ioctl returned failure
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_Start()


/*============================================================================*/
/**
* @brief          Stops decode operations
* @details        The function clears the GO bit in order to stop the
*                 decoding activity according to JPEGDCD_BOOL input variable
*
* @param[in]      void
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre            Call JPEGdcd_jpeg_swreset & JPEGdcd_jpeg_configstart &
*                 JPEGdcd_jpeg_timeoutset
* @post
*
* @implements     JPEGDEC_Stop_Activity
*
* @note           To be used only after hw or sw reset to enable JPEG core
*/
JPEGDEC_LIB_RESULT JPEGDEC_Stop(void)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        JPEGDCD_BOOL lVal  = JPEGDCD_OFF;
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_DCDENABLE,
                &lVal) < 0L)
        {
            VDB_LOG_ERROR("Failed to start JPEGDEC operation.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if ioctl returned failure
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_Stop()


/*============================================================================*/
/**
* @brief          Set interrupts
* @details        The function enables or disables the CONFIG_ERR, START_OF_FRAME,
*                 END_OF_FRAME, MCU_ROW_DONE, STR4_FIFO_WATERMARK,
*                 STREAM3_FIFO_WATERMARK, STREAM2_FIFO_WATERMARK and
*                 STREAM1_FIFO_WATERMARK interrupts according to the
*                 mask sent as parameter.
*
* @param[in]      aIrqMask   Interrupt mask
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_IrqMaskSet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_IrqMaskSet(const uint32_t aIrqMask)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_IRQ_CONTROL,
                  &aIrqMask) < 0L)
        {
            VDB_LOG_ERROR("Failed to set IRQ mask.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if ioctl returned failure
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_IrqMaskSet(const uint32_t aIrqMask)


/*============================================================================*/
/**
* @brief          Get enabled interrupts
* @details        The function returns the interrupts which are
*                 enabled at the moment.
*
* @param[out]     aIrqMask   Interrupt mask
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_IrqMaskGet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_IrqMaskGet(uint32_t &aIrqMask)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_IRQ_GET,
                  &aIrqMask) < 0L)
        {
            VDB_LOG_ERROR("Failed to get IRQ mask.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        } // if ioctl returned failure
    } // else from if JpegDec not opened

  return lRet;
} // JPEGDEC_IrqMaskGet(uint32_t &aIrqMask)


/*============================================================================*/
/**
* @brief          Get the status of configuration
* @details        The function returns the JPEG configuration error flag
*                 and the value of the error byte
*
* @param[out]     arCfgErr     JPEGDCD_CFGERR structure
*
* @return         JPEGDEC_LIB_RESULT
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGDEC_CfgErrGet_Activity
*
* @note
*/
JPEGDEC_LIB_RESULT JPEGDEC_CfgErrGet(JPEGDCD_CFGERR &arCfgErr)
{
    JPEGDEC_LIB_RESULT lRet = JPEGDEC_LIB_SUCCESS;

    if(sJpegDecFd == -1)
    {
        VDB_LOG_ERROR("JpegDec device file not opened.\n");
        lRet = JPEGDEC_LIB_FAILURE;
    } // if JpegDec not opened
    else
    {
        if (ioctl(sJpegDecFd, JPEGDCD_IOCTL_JPEG_DCDCFGERR_GET,
                  &arCfgErr) < 0L)
        {
            VDB_LOG_ERROR("Failed to get configuration errors.\n");
            lRet = JPEGDEC_LIB_FAILURE;
        }
    } // else from if JpegDec not opened

    return lRet;
} // JPEGDEC_CfgErrGet(JPEGDCD_CFGERR &arCfgErr)
