/*
 * Copyright (c) 2012-2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if defined (__LINUX__)
    #include "io_linux.h"
#else
    #include "io_core.h"
#endif

#include "jpegdcd_types.h"
#include "jpegdcd_core.h"


/*============================================================================*/
/**
* @brief          Set the parameters of the data source
* @details        The function sets address and size of the input data packet
*                 for the specified input stream
*
* @param[in]      reg_base      The base address of the JPEG registers bank
* @param[in]      data_config   STREAM_DATA_CONFIG structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_inputstream_cfg_Activity
*
* @note
*/
void JPEGdcd_inputstream_cfg(JPEGDCD_REGS *reg_base,
                             const STREAM_DATA_CONFIG * const data_config)
{
    switch (data_config->stream_no)
    {
        case DATASTREAM_1:
            reg_write32(&reg_base->STR1_SRAM_PTR, data_config->buff_addr);
            reg_write32(&reg_base->STR1_SRAM_LEN_VAL, data_config->buff_size);
            break;
        case DATASTREAM_2:
            reg_write32(&reg_base->STR2_SRAM_PTR, data_config->buff_addr);
            reg_write32(&reg_base->STR2_SRAM_LEN_VAL, data_config->buff_size);
            break;
        case DATASTREAM_3:
            reg_write32(&reg_base->STR3_SRAM_PTR, data_config->buff_addr);
            reg_write32(&reg_base->STR3_SRAM_LEN_VAL, data_config->buff_size);
            break;
        case DATASTREAM_4:
            reg_write32(&reg_base->STR4_SRAM_PTR, data_config->buff_addr);
            reg_write32(&reg_base->STR4_SRAM_LEN_VAL, data_config->buff_size);
            break;
        default:
            /* nothing to do */
            break;
    }
}


/*============================================================================*/
/**
* @brief          Get the status of the FIFO stack
* @details        The function gets the available space in FIFO and a flag
*                 signalizing that FIFO has space greater than watermark level
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     fifo_st     STREAM_FIFO_STATUS structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_fifostatus_get_Activity
*
* @note
*/
void JPEGdcd_fifostatus_get(JPEGDCD_REGS *reg_base, STREAM_FIFO_STATUS * const fifo_st)
{
    uint32_t reg_val;

    reg_val = reg_read32(&reg_base->W_STATUS);

    switch (fifo_st->stream_no)
    {
        case DATASTREAM_1:
            fifo_st->fifo_space = (uint8_t)((reg_val & JPEGDCD_WRST_SPACEFIFO1_MASK) >> JPEGDCD_WRST_SPACEFIFO1_SHIFT);
            fifo_st->fifo_wtm = (uint8_t)((reg_val & JPEGDCD_INTR_WTRMRKFIFO1_MASK) >> JPEGDCD_INTR_WTRMRKFIFO1_SHIFT);

            if (1u == fifo_st->fifo_wtm)
            {
                reg_val &= ~JPEGDCD_INTR_FLAGS_MASK;
                reg_val |= JPEGDCD_INTR_WTRMRKFIFO1_MASK;
            }
            break;

        case DATASTREAM_2:
            fifo_st->fifo_space = (uint8_t)((reg_val & JPEGDCD_WRST_SPACEFIFO2_MASK) >> JPEGDCD_WRST_SPACEFIFO2_SHIFT);
            fifo_st->fifo_wtm = (uint8_t)((reg_val & JPEGDCD_INTR_WTRMRKFIFO2_MASK) >> JPEGDCD_INTR_WTRMRKFIFO2_SHIFT);

            if (1u == fifo_st->fifo_wtm)
            {
                reg_val &= ~JPEGDCD_INTR_FLAGS_MASK;
                reg_val |= JPEGDCD_INTR_WTRMRKFIFO2_MASK;
            }
            break;

        case DATASTREAM_3:
            fifo_st->fifo_space = (uint8_t)((reg_val & JPEGDCD_WRST_SPACEFIFO3_MASK) >> JPEGDCD_WRST_SPACEFIFO3_SHIFT);
            fifo_st->fifo_wtm = (uint8_t)((reg_val & JPEGDCD_INTR_WTRMRKFIFO3_MASK) >> JPEGDCD_INTR_WTRMRKFIFO3_SHIFT);

            if (1u == fifo_st->fifo_wtm)
            {
                reg_val &= ~JPEGDCD_INTR_FLAGS_MASK;
                reg_val |= JPEGDCD_INTR_WTRMRKFIFO3_MASK;
            }
            break;

        case DATASTREAM_4:
            fifo_st->fifo_space = (uint8_t)((reg_val & JPEGDCD_WRST_SPACEFIFO4_MASK) >> JPEGDCD_WRST_SPACEFIFO4_SHIFT);
            fifo_st->fifo_wtm = (uint8_t)((reg_val & JPEGDCD_INTR_WTRMRKFIFO4_MASK) >> JPEGDCD_INTR_WTRMRKFIFO4_SHIFT);

            if (1u == fifo_st->fifo_wtm)
            {
                reg_val &= ~JPEGDCD_INTR_FLAGS_MASK;
                reg_val |= JPEGDCD_INTR_WTRMRKFIFO4_MASK;
            }
            break;

        default:
            /* nothing to do */
            break;
    }

    reg_write32(&reg_base->W_STATUS, reg_val);
}


/*============================================================================*/
/**
* @brief          Get the packet data from FIFO.
* @details        The function gets the configuration data from FIFO
*                 for specified stream.
*
* @param[in]      reg_base      The base address of the JPEG registers bank
* @param[out]     pck_cfgdata   STREAM_DATA_CONFIG structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_fifodata_get_Activity
*
* @note
*/
void JPEGdcd_fifodata_get(const JPEGDCD_REGS *reg_base, STREAM_DATA_CONFIG * const pck_cfgdata)
{
    switch (pck_cfgdata->stream_no)
    {
        case DATASTREAM_1:
            pck_cfgdata->buff_addr = reg_read32(&reg_base->STR1_PTR_FIFO[pck_cfgdata->fifo_lev]);
            pck_cfgdata->buff_size = (uint16_t)reg_read32(&reg_base->STR1_LEN_VAL_FIFO[pck_cfgdata->fifo_lev]);
            break;
        case  DATASTREAM_2:
            pck_cfgdata->buff_addr = reg_read32(&reg_base->STR2_PTR_FIFO[pck_cfgdata->fifo_lev]);
            pck_cfgdata->buff_size = (uint16_t)reg_read32(&reg_base->STR2_LEN_VAL_FIFO[pck_cfgdata->fifo_lev]);
            break;
        case DATASTREAM_3:
            pck_cfgdata->buff_addr = reg_read32(&reg_base->STR3_PTR_FIFO[pck_cfgdata->fifo_lev]);
            pck_cfgdata->buff_size = (uint16_t)reg_read32(&reg_base->STR3_LEN_VAL_FIFO[pck_cfgdata->fifo_lev]);
            break;
        case DATASTREAM_4:
            pck_cfgdata->buff_addr = reg_read32(&reg_base->STR4_PTR_FIFO[pck_cfgdata->fifo_lev]);
            pck_cfgdata->buff_size = (uint16_t)reg_read32(&reg_base->STR4_LEN_VAL_FIFO[pck_cfgdata->fifo_lev]);
            break;
        default:
            /* nothing to do */
            break;

    }
}


/*============================================================================*/
/**
* @brief          Set the watermark level.
* @details        The function sets the watermark level for
*                 specified FIFO
*
* @param[in]      reg_base      The base address of the JPEG registers bank
* @param[in]      wtmark        The watermark level
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_fifo_wtmklevel_set_Activity
*
* @note
*/
void JPEGdcd_fifo_wtmklevel_set(JPEGDCD_REGS *reg_base, const uint8_t wtmark)
{
    uint32_t reg_val = reg_read32(&reg_base->INTR_EN);

    reg_val &= ~JPEGDCD_INTREN_WTRMARKLEV_MASK;
    reg_val |= ((uint32_t)((uint32_t)wtmark & JPEGDCD_INTREN_WTRMARKLEV_SIZE) << JPEGDCD_INTREN_WTRMARKLEV_SHIFT);

    reg_write32(&reg_base->INTR_EN, reg_val);
}


/*============================================================================*/
/**
* @brief          Set the parameters for output data
* @details        The function sets for each image component the buffer
*                 address and the number of lines per component used in
*                 the output circular component buffer
*
* @param[in]      reg_base      The base address of the JPEG registers bank
* @param[in]      output_cfg    OUTPUT_BUFFCFG structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_output_cfg_Activity
*
* @note
*/
void JPEGdcd_output_cfg(JPEGDCD_REGS *reg_base, const OUTPUT_BUFFCFG * const output_cfg)
{
    uint32_t val_temp;

    val_temp = output_cfg->c1_outbuffer;
    reg_write32(&reg_base->BUF_C1_ADDR, val_temp);

    val_temp = output_cfg->c2_outbuffer;
    reg_write32(&reg_base->BUF_C2_ADDR, val_temp);

    val_temp = output_cfg->c3_outbuffer;
    reg_write32(&reg_base->BUF_C3_ADDR, val_temp);

    val_temp = output_cfg->c4_outbuffer;
    reg_write32(&reg_base->BUF_C4_ADDR, val_temp);

    val_temp = (uint32_t)(output_cfg->c1_b2lines) | ((uint32_t)(output_cfg->c2_c3_b2lines) << JPEGDCD_OUT_B2LINES_C23_SHIFT);
    reg_write32(&reg_base->NRLINES_B2L, val_temp);

    val_temp = output_cfg->c4_b2lines;
    reg_write32(&reg_base->NRLINES_B2L_C4, (uint32_t)val_temp);
}


/*============================================================================*/
/**
* @brief          Change resampling mode
* @details        The function sets sampling mode according to
*                 outputregsmpl_mode variable
*
* @param[in]      reg_base              The base address of the JPEG registers bank
* @param[in]      outputregsmpl_mode    boolean
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_outsamplemode_set_Activity
*
* @note
*/
void JPEGdcd_outsamplemode_set(JPEGDCD_REGS *reg_base, const JPEGDCD_BOOL outputregsmpl_mode)
{
    uint32_t reg_temp  = reg_read32(&reg_base->W_CTRL2);

    if (JPEGDCD_ON == outputregsmpl_mode)
    {
        reg_temp |= JPEGDCD_WCTRL2_SAMPLEOUTBUF_MASK;
    }
    else
    {
        reg_temp &= ~JPEGDCD_WCTRL2_SAMPLEOUTBUF_MASK;
    }

    reg_write32(&reg_base->W_CTRL2, reg_temp);
}


/*============================================================================*/
/**
* @brief          Set bankstride
* @details        The function sets the address offset of each
*                 start of macroblock
*
* @param[in]      reg_base     The base address of the JPEG registers bank
* @param[in]      bytes_num    Address offset
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_bankstride_set_Activity
*
* @note
*/
void JPEGdcd_bankstride_set(JPEGDCD_REGS *reg_base, const uint8_t bytes_num)
{
    uint32_t reg_temp  = reg_read32(&reg_base->W_CTRL1);
    uint32_t val_temp  = (((uint32_t)bytes_num << JPEGDCD_WCTRL1_BNKSTRD_SHIFT) & JPEGDCD_WCTRL1_BNKSTRD_MASK);

    reg_temp &= ~JPEGDCD_WCTRL1_BNKSTRD_MASK;
    reg_temp |= val_temp;

    reg_write32(&reg_base->W_CTRL1,reg_temp);
}


/*============================================================================*/
/**
* @brief          Get bankstride
* @details        The function gets the address offset of each
*                 start of macroblock
*
* @param[in]      reg_base     The base address of the JPEG registers bank
* @param[out]     bytes_num    Address offset
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_bankstride_get_Activity
*
* @note
*/
void JPEGdcd_bankstride_get(const JPEGDCD_REGS *reg_base, uint8_t * const bytes_num)
{
    uint32_t reg_temp  = reg_read32(&reg_base->W_CTRL1);
    uint32_t val_temp  = (reg_temp & JPEGDCD_WCTRL1_BNKSTRD_MASK) >> JPEGDCD_WCTRL1_BNKSTRD_SHIFT;

    *bytes_num = (uint8_t)val_temp;
}


/*============================================================================*/
/**
* @brief          Workaround for setting timeout for decoding operation
* @details        The function sets the timeout to 1 an after this
*                 it resets the JPEG decoder
*
* @param[in]      reg_base    The base address of the JPEG registers bank
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_configstart_Activity
*
* @note           It shall be used before to program the JPEG core registers
*/
void JPEGdcd_jpeg_configstart(JPEGDCD_REGS *reg_base)
{
    uint32_t reg_temp = reg_read32(&reg_base->W_CTRL1);
    reg_write32(&reg_base->TIMEOUT, (uint32_t)1);

    reg_temp |= JPEGDCD_WCTRL1_SWRST_MASK;
    reg_write32(&reg_base->W_CTRL1, reg_temp);

    reg_temp &= ~JPEGDCD_WCTRL1_SWRST_MASK;
    reg_write32(&reg_base->W_CTRL1,reg_temp);
}


/*============================================================================*/
/**
* @brief          Set timeout for decode operation
* @details        The function sets the JPEG error timeout measured
*                 in JPEG clock cycles.
*
* @param[in]      reg_base           The base address of the JPEG registers bank
* @param[in]      jpeg_errtimeout    timeout for decode operation
*
* @return         void
* @api
* @isr
*
* @pre            Call JPEGdcd_jpeg_configstart
* @post
*
* @implements     JPEGdcd_jpeg_timeoutset_Activity
*
* @note           It shall be called after all the JPEG registers are
*                  programmed and before the JPEG decoder to be enabled
*/
void JPEGdcd_jpeg_timeoutset(JPEGDCD_REGS *reg_base, const uint32_t jpeg_errtimeout)
{
    reg_write32(&reg_base->TIMEOUT, jpeg_errtimeout);
}


/*============================================================================*/
/**
* @brief          Start decode operations
* @details        The function sets the GO bit in order to start the
*                 decoding activity according to JPEGDCD_BOOL input variable
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[in]      go_bit      Enable decoding
*
* @return         void
* @api
* @isr
*
* @pre            Call JPEGdcd_jpeg_swreset & JPEGdcd_jpeg_configstart &
*                 JPEGdcd_jpeg_timeoutset
* @post
*
* @implements     JPEGdcd_jpeg_dcdenable_Activity
*
* @note           To be used only after hw or sw reset to enable JPEG core
*/
void JPEGdcd_jpeg_dcdenable(JPEGDCD_REGS *reg_base, const JPEGDCD_BOOL go_bit)
{
    if (JPEGDCD_ON == go_bit)
    {
        reg_write32(&reg_base->CTRL, (uint32_t)JPEGDCD_JPEG_GO_MASK);
    }
    else
    {
        reg_write32(&reg_base->CTRL,(uint32_t)0);
    }

}


/*============================================================================*/
/**
* @brief          Set the configuration parameters for data stream
* @details        The function sets the image size and the components
*                 sampling factors for the selected input stream
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[in]      image_data     jpeg_image structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_streamdatacfg_Activity
*
* @note
*/
void JPEGdcd_jpeg_streamdatacfg(JPEGDCD_REGS *reg_base, const STREAM_JPEG_PARAM * const image_data)
{
    uint32_t smplfctr_reg, size_reg, ctrl2_reg;

    size_reg = ((uint32_t)image_data->x_size << JPEGDCD_JPEG_IMGSIZE_X_SHIFT);
    size_reg |= ((uint32_t)image_data->y_size << JPEGDCD_JPEG_IMGSIZE_Y_SHIFT);

    smplfctr_reg = ((uint32_t)(image_data->c1_samplingfactor_h & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_H1_SHIFT);
    smplfctr_reg |= ((uint32_t)(image_data->c2_samplingfactor_h & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_H2_SHIFT);
    smplfctr_reg |= ((uint32_t)(image_data->c3_samplingfactor_h & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_H3_SHIFT);
    smplfctr_reg |= ((uint32_t)(image_data->c4_samplingfactor_h & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_H4_SHIFT);
    smplfctr_reg |= ((uint32_t)(image_data->c1_samplingfactor_v & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_V1_SHIFT);
    smplfctr_reg |= ((uint32_t)(image_data->c2_samplingfactor_v & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_V2_SHIFT);
    smplfctr_reg |= ((uint32_t)(image_data->c3_samplingfactor_v & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_V3_SHIFT);
    smplfctr_reg |= ((uint32_t)(image_data->c4_samplingfactor_v & JPEGDCD_JPEG_SAMPFAC_MASK) << JPEGDCD_JPEG_SAMPFAC_V4_SHIFT);

    ctrl2_reg = reg_read32(&reg_base->W_CTRL2);

    switch (image_data->stream_no)
    {
        case DATASTREAM_1:
            reg_write32(&reg_base->IMG_SZ_STR1, size_reg);
            reg_write32(&reg_base->SMPL_FCTR_STR1, smplfctr_reg);

            if (JPEGDCD_OFF == image_data->wr_config)
            {
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGWRCFG1_MASK;
            }
            else
            {
                ctrl2_reg |= JPEGDCD_WCTRL2_JPEGWRCFG1_MASK;
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGCOMPNUM1_MASK;
                ctrl2_reg |= (((uint32_t)(image_data->component_num) << JPEGDCD_WCTRL2_JPEGCOMPNUM1_SHIFT) & JPEGDCD_WCTRL2_JPEGCOMPNUM1_MASK);

                if (PREC_8BITS == image_data->precision)
                {
                    ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGPRE1_MASK;
                }
                else
                {
                    ctrl2_reg |= JPEGDCD_WCTRL2_JPEGPRE1_MASK;
                }
            }
            reg_write32(&reg_base->W_CTRL2, ctrl2_reg);
            break;

        case DATASTREAM_2:
            reg_write32(&reg_base->IMG_SZ_STR2, size_reg);
            reg_write32(&reg_base->SMPL_FCTR_STR2, smplfctr_reg);

            if (JPEGDCD_OFF == image_data->wr_config)
            {
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGWRCFG2_MASK;
            }
            else
            {
                ctrl2_reg |= JPEGDCD_WCTRL2_JPEGWRCFG2_MASK;
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGCOMPNUM2_MASK;
                ctrl2_reg |= (((uint32_t)(image_data->component_num) << JPEGDCD_WCTRL2_JPEGCOMPNUM2_SHIFT) & JPEGDCD_WCTRL2_JPEGCOMPNUM2_MASK);

                if (PREC_8BITS == image_data->precision)
                {
                    ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGPRE2_MASK;
                }
                else
                {
                    ctrl2_reg |= JPEGDCD_WCTRL2_JPEGPRE2_MASK;
                }
            }
            reg_write32(&reg_base->W_CTRL2, ctrl2_reg);
            break;

        case DATASTREAM_3:
            reg_write32(&reg_base->IMG_SZ_STR3, size_reg);
            reg_write32(&reg_base->SMPL_FCTR_STR3, smplfctr_reg);

            if (JPEGDCD_OFF == image_data->wr_config)
            {
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGWRCFG3_MASK;
            }
            else
            {
                ctrl2_reg |= JPEGDCD_WCTRL2_JPEGWRCFG3_MASK;
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGCOMPNUM3_MASK;
                ctrl2_reg |= (((uint32_t)(image_data->component_num) << JPEGDCD_WCTRL2_JPEGCOMPNUM3_SHIFT) & JPEGDCD_WCTRL2_JPEGCOMPNUM3_MASK);

                if (PREC_8BITS == image_data->precision)
                {
                    ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGPRE3_MASK;
                }
                else
                {
                    ctrl2_reg |= JPEGDCD_WCTRL2_JPEGPRE3_MASK;
                }
            }
            reg_write32(&reg_base->W_CTRL2, ctrl2_reg);
            break;

        case DATASTREAM_4:
            reg_write32(&reg_base->IMG_SZ_STR4, size_reg);
            reg_write32(&reg_base->SMPL_FCTR_STR4, smplfctr_reg);

            if (JPEGDCD_OFF == image_data->wr_config)
            {
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGWRCFG4_MASK;
            }
            else
            {
                ctrl2_reg |= JPEGDCD_WCTRL2_JPEGWRCFG4_MASK;
                ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGCOMPNUM4_MASK;
                ctrl2_reg |= (((uint32_t)(image_data->component_num) << JPEGDCD_WCTRL2_JPEGCOMPNUM4_SHIFT) & JPEGDCD_WCTRL2_JPEGCOMPNUM4_MASK);

                if (PREC_8BITS == image_data->precision)
                {
                    ctrl2_reg &= ~JPEGDCD_WCTRL2_JPEGPRE4_MASK;
                }
                else
                {
                    ctrl2_reg |= JPEGDCD_WCTRL2_JPEGPRE4_MASK;
                }
            }
            reg_write32(&reg_base->W_CTRL2, ctrl2_reg);
            break;

        default:
            /* nothing to do */
            break;
    }
}


/*============================================================================*/
/**
* @brief          Set the numbers of streams
* @details        The function sets the number of active streams
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[in]      num_of_streams      Number of active streams
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_streamnum_set_Activity
*
* @note
*/
void JPEGdcd_jpeg_streamnum_set(JPEGDCD_REGS *reg_base, const uint8_t num_of_streams)
{
    uint32_t reg_val = reg_read32(&reg_base->W_CTRL1);

    reg_val &= ~JPEGDCD_WCTRL1_STREAMNUM_MASK;
    reg_val |= (((uint32_t)(num_of_streams) << JPEGDCD_WCTRL1_STREAMNUM_SHIFT) & JPEGDCD_WCTRL1_STREAMNUM_MASK);

    reg_write32(&reg_base->W_CTRL1, reg_val);
}


/*============================================================================*/
/**
* @brief          Get the numbers of streams
* @details        The function gets the number of active streams
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     num_of_streams      Number of active streams
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_streamnum_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_streamnum_get(const JPEGDCD_REGS *reg_base, uint8_t * const num_of_streams)
{
    *num_of_streams = (uint8_t)((reg_read32(&reg_base->W_CTRL1) & JPEGDCD_WCTRL1_STREAMNUM_MASK) >> JPEGDCD_WCTRL1_STREAMNUM_SHIFT);
}


/*============================================================================*/
/**
* @brief          Set the JPEG Decoder mode
* @details        The function sets the mode:
*                 - 1 compatibility mode is also set
*                 - otherwise (2,3,4) the context switch mode is set
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[in]      switch_m    JPEG Decoder working mode
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_mode_set_Activity
*
* @note
*/
void JPEGdcd_jpeg_mode_set(JPEGDCD_REGS *reg_base, const COMPAT_MODE switch_m)
{
    uint32_t reg_val = reg_read32(&reg_base->W_CTRL2);

    if (COMPATIBILITY == switch_m)
    {
        reg_val |= JPEGDCD_WCTRL2_COMMODE_MASK;
    }
    else
    {
        reg_val &= ~JPEGDCD_WCTRL2_COMMODE_MASK;
    }

    reg_write32(&reg_base->W_CTRL2, reg_val);
}


/*============================================================================*/
/**
* @brief          Get the JPEG Decoder mode
* @details        The function gets the mode
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     switch_m    JPEG Decoder working mode
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_mode_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_mode_get(const JPEGDCD_REGS *reg_base, COMPAT_MODE * const switch_m)
{
    uint32_t reg_val = reg_read32(&reg_base->W_CTRL2) & JPEGDCD_WCTRL2_COMMODE_MASK;

    if (JPEGDCD_WCTRL2_COMMODE_MASK == reg_val)
    {
        *switch_m = COMPATIBILITY;
    }
    else
    {
        *switch_m = CONTEXT_SWITCH;
    }
}


/*============================================================================*/
/**
* @brief          SW reset
* @details        The function sets and resets the SW_RST bit resetting
*                 the Input DMA Wrapper, the Output DMA Wrapper and
*                 the JPEG Decoder Core
*
* @param[in]      reg_base    The base address of the JPEG registers bank
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_swreset_Activity
*
* @note
*/
void JPEGdcd_jpeg_swreset(JPEGDCD_REGS *reg_base)
{
    uint32_t reg_temp = reg_read32(&reg_base->W_CTRL1);

    reg_temp |= JPEGDCD_WCTRL1_SWRST_MASK;
    reg_write32(&reg_base->W_CTRL1,reg_temp);

    reg_temp &= ~JPEGDCD_WCTRL1_SWRST_MASK;
    reg_write32(&reg_base->W_CTRL1,reg_temp);
}


/*============================================================================*/
/**
* @brief          Get the active stream number
* @details        The function returns the id of the stream written in
*                 the memory at that moment
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     str_no      Stream number
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_streamactive_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_streamactive_get(const JPEGDCD_REGS *reg_base, STREAM_ID * const str_no)
{
    uint32_t reg_temp = reg_read32(&reg_base->W_STATUS);
    uint32_t stream_no =(reg_temp & JPEGDCD_WRST_STREAM_MASK) >> JPEGDCD_WRST_STREAM_SHIFT;

    switch (stream_no)
    {
        case 1:
            *str_no = DATASTREAM_1;
            break;
        case 2:
            *str_no = DATASTREAM_2;
            break;
        case 3:
            *str_no = DATASTREAM_3;
            break;
        case 4:
            *str_no = DATASTREAM_4;
            break;
        default:
            /* nothing to do */
            break;
    }
}


/*============================================================================*/
/**
* @brief          Set the restart interval
* @details        The function sets the restart interval in terms
*                 of image lines
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[in]      lines_num   Number of lines
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_rstinterval_set_Activity
*
* @note
*/
void JPEGdcd_jpeg_rstinterval_set(JPEGDCD_REGS *reg_base, const uint16_t lines_num)
{
    reg_write32(&reg_base->RST_INTVL, lines_num);
}


/*============================================================================*/
/**
* @brief          Get the restart interval
* @details        The function gets the restart interval in terms
*                 of image lines
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     lines_num   Number of lines
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_rstinterval_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_rstinterval_get(const JPEGDCD_REGS *reg_base, uint16_t * const lines_num)
{
    uint32_t reg_temp = reg_read32(&reg_base->RST_INTVL);
    *lines_num = (uint16_t)reg_temp;
}


/*============================================================================*/
/**
* @brief          Get the frame status
* @details        The function gets the restart interval in terms
*                 of image lines
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     frame_st    JPEGDCD_FRAMEST structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_framest_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_framest_get(JPEGDCD_REGS *reg_base, JPEGDCD_FRAMEST * const frame_st)
{
    uint32_t reg_value, reg_temp;

    reg_temp = reg_read32(&reg_base->W_STATUS);
    reg_value = reg_temp & ~JPEGDCD_INTR_FLAGS_MASK;
    reg_temp &= (JPEGDCD_INTR_STARTFRAME_MASK | JPEGDCD_INTR_STOPFRAME_MASK);

    if ((JPEGDCD_INTR_STARTFRAME_MASK | JPEGDCD_INTR_STOPFRAME_MASK) == reg_temp)
    {
        *frame_st = JPEG_FR_REPEAT;
        reg_value |= (uint32_t)(JPEGDCD_INTR_STARTFRAME_MASK | JPEGDCD_INTR_STOPFRAME_MASK);
    }
    else if(JPEGDCD_INTR_STARTFRAME_MASK == reg_temp)
    {
        *frame_st = JPEG_FR_START;
        reg_value |= (uint32_t)(JPEGDCD_INTR_STARTFRAME_MASK);
    }
    else /* JPEGDCD_INTR_STOPFRAME_MASK */
    {
        *frame_st = JPEG_FR_END;
        reg_value |= (uint32_t)JPEGDCD_INTR_STOPFRAME_MASK;
    }

    reg_write32(&reg_base->W_STATUS, reg_value);
}


/*============================================================================*/
/**
* @brief          Get the status of configuration
* @details        The function returns the JPEG configuration error flag
*                 and the value of the error byte
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     cfg_err     JPEGDCD_CFGERR structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_cfgerr_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_cfgerr_get(const JPEGDCD_REGS *reg_base, JPEGDCD_CFGERR * const cfg_err)
{
    uint32_t reg_temp = reg_read32(&reg_base->E_STATUS);

    if (0u == (reg_temp & JPEGDCD_INTR_CFGERR_MASK))
    {
        cfg_err->jpegerr_count = 0;
    }
    else
    {
        cfg_err->jpegerr_count = 1;
    }
    cfg_err->jpegerr_byte = (uint8_t)(reg_temp & JPEGDCD_JPEGERR_BYTE);
}


/*============================================================================*/
/**
* @brief          Get the MCUROW status
* @details        The function returns the MCU row end flag status
*                 and reset it (if it is set)
*
* @param[in]      reg_base              The base address of the JPEG registers bank
* @param[out]     mcurow_end_count      boolean
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_mcurow_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_mcurow_get(JPEGDCD_REGS *reg_base, JPEGDCD_BOOL * const mcurow_end_count)
{
    uint32_t reg_value, reg_temp;

    reg_temp = reg_read32(&reg_base->W_STATUS);
    reg_value = reg_temp & ~JPEGDCD_INTR_FLAGS_MASK;
    reg_temp &= JPEGDCD_INTR_MCUROWEND_MASK;

    if (0u == reg_temp)
    {
        *mcurow_end_count = JPEGDCD_OFF;
    }
    else
    {
        *mcurow_end_count = JPEGDCD_ON;
        reg_value |= (uint32_t)(JPEGDCD_INTR_MCUROWEND_MASK);

        reg_write32(&reg_base->W_STATUS, reg_value);
    }
}


/*============================================================================*/
/**
* @brief          Get the decoded image parameters
* @details        The function returns JPEG decoded image parameters
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     dcd_image   JPEGDCD_DECODED_IMAGE structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_jpeg_dcdimage_get_Activity
*
* @note
*/
void JPEGdcd_jpeg_dcdimage_get(const JPEGDCD_REGS *reg_base, JPEGDCD_DECODED_IMAGE * const dcd_image)
{
    uint32_t reg_value, precision;

    reg_value = reg_read32(&reg_base->STATUS1);
    dcd_image->x_size = (uint16_t)reg_value;

    reg_value = reg_read32(&reg_base->STATUS2);
    dcd_image->y_size = (uint16_t)reg_value;

    reg_value = reg_read32(&reg_base->STATUS3);
    dcd_image->hmcu_num = (uint16_t)reg_value;

    reg_value = reg_read32(&reg_base->STATUS4);
    dcd_image->vmcu_num = (uint16_t)reg_value;

    reg_value = reg_read32(&reg_base->STATUS5);
    dcd_image->c1_samplingfactor_h = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEH_MASK) >> JPEGDCD_JPEGST_SAMPLEH_SHIFT);
    dcd_image->c1_samplingfactor_v = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEV_MASK) >> JPEGDCD_JPEGST_SAMPLEV_SHIFT);
    dcd_image->c1_identifier = (uint8_t)((reg_value & JPEGDCD_JPEGST_COMPID_MASK) >> JPEGDCD_JPEGST_COMPID_SHIFT);
    dcd_image->c1_quantization_table = (uint8_t)((reg_value & JPEGDCD_JPEGST_QOANTT_MASK) >> JPEGDCD_JPEGST_QOANTT_SHIFT);

    reg_value = reg_read32(&reg_base->STATUS6);
    dcd_image->c2_samplingfactor_h = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEH_MASK) >> JPEGDCD_JPEGST_SAMPLEH_SHIFT);
    dcd_image->c2_samplingfactor_v = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEV_MASK) >> JPEGDCD_JPEGST_SAMPLEV_SHIFT);
    dcd_image->c2_identifier = (uint8_t)((reg_value & JPEGDCD_JPEGST_COMPID_MASK) >> JPEGDCD_JPEGST_COMPID_SHIFT);
    dcd_image->c2_quantization_table = (uint8_t)((reg_value & JPEGDCD_JPEGST_QOANTT_MASK) >> JPEGDCD_JPEGST_QOANTT_SHIFT);

    reg_value = reg_read32(&reg_base->STATUS7);
    dcd_image->c3_samplingfactor_h = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEH_MASK) >> JPEGDCD_JPEGST_SAMPLEH_SHIFT);
    dcd_image->c3_samplingfactor_v = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEV_MASK) >> JPEGDCD_JPEGST_SAMPLEV_SHIFT);
    dcd_image->c3_identifier = (uint8_t)((reg_value & JPEGDCD_JPEGST_COMPID_MASK) >> JPEGDCD_JPEGST_COMPID_SHIFT);
    dcd_image->c3_quantization_table = (uint8_t)((reg_value & JPEGDCD_JPEGST_QOANTT_MASK) >> JPEGDCD_JPEGST_QOANTT_SHIFT);

    reg_value = reg_read32(&reg_base->STATUS8);
    dcd_image->c4_samplingfactor_h = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEH_MASK) >> JPEGDCD_JPEGST_SAMPLEH_SHIFT);
    dcd_image->c4_samplingfactor_v = (uint8_t)((reg_value & JPEGDCD_JPEGST_SAMPLEV_MASK) >> JPEGDCD_JPEGST_SAMPLEV_SHIFT);
    dcd_image->c4_identifier = (uint8_t)((reg_value & JPEGDCD_JPEGST_COMPID_MASK) >> JPEGDCD_JPEGST_COMPID_SHIFT);
    dcd_image->c4_quantization_table = (uint8_t)((reg_value & JPEGDCD_JPEGST_QOANTT_MASK) >> JPEGDCD_JPEGST_QOANTT_SHIFT);

    reg_value = reg_read32(&reg_base->STATUS9);

    precision = (reg_value & JPEGDCD_JPEGST_PRECISION_MASK) >> JPEGDCD_JPEGST_PRECISION_SHIFT;

    if (precision == 0u)
    {
        dcd_image->sample_precision = PREC_8BITS;
    }
    else
    {
        dcd_image->sample_precision = PREC_12BITS;
    }

    dcd_image->component_num = (uint8_t)((reg_value & JPEGDCD_JPEGST_NUMOFCOMP_MASK) >> JPEGDCD_JPEGST_NUMOFCOMP_SHIFT);

    reg_value = reg_read32(&reg_base->STATUS10);
    dcd_image->rst_interval = (uint16_t)reg_value;
}


/*============================================================================*/
/**
* @brief          Set interrupts
* @details        The function enables or disables the CONFIG_ERR, START_OF_FRAME,
*                 END_OF_FRAME, MCU_ROW_DONE, STR4_FIFO_WATERMARK,
*                 STREAM3_FIFO_WATERMARK, STREAM2_FIFO_WATERMARK and
*                 STREAM1_FIFO_WATERMARK interrupts according to the
*                 mask sent as parameter.
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[in]      intr_mask   Interrupt mask
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_interrupt_ctrl_Activity
*
* @note
*/
void JPEGdcd_interrupt_ctrl(JPEGDCD_REGS *reg_base, const uint32_t intr_mask)
{
    uint32_t reg_temp =  reg_read32(&reg_base->INTR_EN);

    reg_temp &= ~JPEGDCD_INTR_ALL_MASK;
    reg_temp |= intr_mask;

    reg_write32(&reg_base->INTR_EN,reg_temp);
}

/*============================================================================*/
/**
* @brief          Get enabled interrupts
* @details        The function returns the interrupts which are
*                 enabled at the moment.
*
* @param[in]      reg_base    The base address of the JPEG registers bank
* @param[out]     intr_mask   Interrupt mask
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     JPEGdcd_interrupt_get_Activity
*
* @note
*/
void JPEGdcd_interrupt_get(const JPEGDCD_REGS *reg_base, uint32_t * const intr_mask)
{
    uint32_t reg_temp =  reg_read32(&reg_base->INTR_EN);

    reg_temp &= JPEGDCD_INTR_ALL_MASK;
    *intr_mask = reg_temp;
}
