 /*
 * Copyright 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
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

#include <linux/types.h>

#if defined (DRV_LINUX_OS)
    #include "io_linux.h"
#else
    #include "io_core.h"
#endif

#include "h264dcd_types.h"
#include "h264dcd_core.h"

/*============================================================================*/
/**
* @brief          Set the parameters of the data source
* @details        The function sets address and size of the input data packet
*                 for the specified input stream
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      data_config   H264DCD_STREAM_DATA_CONFIG structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_inputstream_cfg_Activity
*
* @note
*/
void H264dcd_inputstream_cfg(H264DCD_REGS *reg_base, const H264DCD_STREAM_DATA_CONFIG *data_config)
{
    uint32_t i, reg_val = 0;

    reg_write32(&reg_base->STR_PKT_ADDR, data_config->buff_addr);

    reg_val = ((uint32_t)data_config->stream_no) << H264DCD_STREAM_NUMBER_SHIFT;
    reg_val|= ((data_config->buff_size) & H264DCD_PKT_LENGTH_MASK);

    reg_write32(&reg_base->STR_TRIG_PKT_CFG, reg_val);

    // delay
    reg_val = 0;
    for (i = 0; i < (data_config->wr_delay); i++)
    {
        reg_val ++;
    }
}

/*============================================================================*/
/**
* @brief          Get the status of the FIFO stack
* @details        The function gets the current active location of the
*                 FIFO pointer and the number of pipelined configuration under
*                 process or to be processed
*
* @param[in]      reg_base    The base address of the H264DCD registers bank
* @param[out]     fifo_st     H264DCD_STREAM_FIFO_STATUS structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_fifostatus_get_Activity
*
* @note
*/
void H264dcd_fifostatus_get(const H264DCD_REGS *reg_base, H264DCD_STREAM_FIFO_STATUS *fifo_st)
{
    uint32_t reg_val;

    switch (fifo_st->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_val = reg_read32(&reg_base->STR_01_PKT_FIFO_STAT);

            fifo_st->fifo_loc = (uint8_t)((reg_val & H264DCD_STR0_ACTIVLOC_MASK) >> H264DCD_STR0_ACTIVLOC_SHIFT);
            fifo_st->fifo_cfgnum = (uint8_t)((reg_val & H264DCD_STR0_LEVEL_MASK) >> H264DCD_STR0_LEVEL_SHIFT);
            break;

        case H264DCD_DATASTREAM_2:
            reg_val = reg_read32(&reg_base->STR_01_PKT_FIFO_STAT);

            fifo_st->fifo_loc = (uint8_t)((reg_val & H264DCD_STR1_ACTIVLOC_MASK) >> H264DCD_STR1_ACTIVLOC_SHIFT);
            fifo_st->fifo_cfgnum = (uint8_t)((reg_val & H264DCD_STR1_LEVEL_MASK) >> H264DCD_STR1_LEVEL_SHIFT);
            break;

        case H264DCD_DATASTREAM_3:
            reg_val = reg_read32(&reg_base->STR_23_PKT_FIFO_STAT);

            fifo_st->fifo_loc = (uint8_t)((reg_val & H264DCD_STR2_ACTIVLOC_MASK) >> H264DCD_STR2_ACTIVLOC_SHIFT);
            fifo_st->fifo_cfgnum = (uint8_t)((reg_val & H264DCD_STR2_LEVEL_MASK) >> H264DCD_STR2_LEVEL_SHIFT);
            break;

        case H264DCD_DATASTREAM_4:
            reg_val = reg_read32(&reg_base->STR_23_PKT_FIFO_STAT);

            fifo_st->fifo_loc = (uint8_t)((reg_val & H264DCD_STR3_ACTIVLOC_MASK) >> H264DCD_STR3_ACTIVLOC_SHIFT);
            fifo_st->fifo_cfgnum = (uint8_t)((reg_val & H264DCD_STR3_LEVEL_MASK) >> H264DCD_STR3_LEVEL_SHIFT);
            break;

        default:
            /* nothing to do */
            break;
    }
}

/*============================================================================*/
/**
* @brief          Get the packet status from FIFO.
* @details        The function gets the status of packet from FIFO
*                 for specified stream.
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     pck_status    datapck_st structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_packetstatus_get_Activity
*
* @note
*/
void H264dcd_packetstatus_get(const H264DCD_REGS *reg_base, H264DCD_STREAM_DATA_STATUS *pck_status)
{
    uint32_t reg_val = 0;

    switch (pck_status->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_val = reg_read32(&reg_base->STR_0_PKT_STATUS[pck_status->fifo_lev]);
            break;

        case H264DCD_DATASTREAM_2:
            reg_val = reg_read32(&reg_base->STR_1_PKT_STATUS[pck_status->fifo_lev]);
            break;

        case H264DCD_DATASTREAM_3:
            reg_val = reg_read32(&reg_base->STR_2_PKT_STATUS[pck_status->fifo_lev]);
            break;

        case H264DCD_DATASTREAM_4:
            reg_val = reg_read32(&reg_base->STR_3_PKT_STATUS[pck_status->fifo_lev]);
            break;

        default:
            /* nothing to do */
            break;
    }

    pck_status->pck_addr = (uint32_t)((reg_val & H264DCD_PKT_ADDR_OFFSET_MASK) >> H264DCD_PKT_ADDR_OFFSET_SHIFT);
    if (((reg_val & H264DCD_PKT_CFG_VALID_MASK) >> H264DCD_PKT_CFG_VALID_SHIFT) != 0U)
    {
        pck_status->pck_valid = H264DCD_ON;
    }
    else
    {
        pck_status->pck_valid = H264DCD_OFF;
    }
}

/*============================================================================*/
/**
* @brief          Set the watermark level.
* @details        The function sets the watermark level for
*                 specified FIFO
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      wtmark        The watermark level
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_fifo_wtmklevel_set_Activity
*
* @note
*/
void H264dcd_fifo_wtmklevel_set(H264DCD_REGS *reg_base, H264DCD_FIFO_WTM wtmark)
{
    uint32_t reg_val = reg_read32(&reg_base->STR_PKT_FIFO_WMRK);

    switch (wtmark.stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_val &= ~H264DCD_STR0_WATERMARK_MASK;
            reg_val |= (((uint32_t)wtmark.fifo_wtmark & H264DCD_STR_WATERMARK_SIZE)
                            << H264DCD_STR0_WATERMARK_SHIFT);
            break;

        case H264DCD_DATASTREAM_2:
            reg_val &= ~H264DCD_STR1_WATERMARK_MASK;
            reg_val |= (((uint32_t)wtmark.fifo_wtmark & H264DCD_STR_WATERMARK_SIZE)
                            << H264DCD_STR1_WATERMARK_SHIFT);
            break;

        case H264DCD_DATASTREAM_3:
            reg_val &= ~H264DCD_STR2_WATERMARK_MASK;
            reg_val |= (((uint32_t)wtmark.fifo_wtmark & H264DCD_STR_WATERMARK_SIZE)
                            << H264DCD_STR2_WATERMARK_SHIFT);
            break;

        case H264DCD_DATASTREAM_4:
            reg_val &= ~H264DCD_STR3_WATERMARK_MASK;
            reg_val |= (((uint32_t)wtmark.fifo_wtmark & H264DCD_STR_WATERMARK_SIZE)
                            << H264DCD_STR3_WATERMARK_SHIFT);
            break;

        default:
            /* nothing to do */
            break;
    }
    reg_write32(&reg_base->STR_PKT_FIFO_WMRK, reg_val);
}

/*============================================================================*/
/**
* @brief          Clear FIFO
* @details        The function clears the FIFO stack for specified stream
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      str_no        Stream number
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_fifo_clear_Activity
*
* @note
*/
void H264dcd_fifo_clear(H264DCD_REGS *reg_base, H264DCD_STREAM_ID str_no)
{
    uint32_t reg_val = reg_read32(&reg_base->MCR);

    switch (str_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_val |= H264DCD_FIFO0_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            reg_write32(&reg_base->MCR, reg_val);

            reg_val &= ~H264DCD_FIFO0_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            break;

        case H264DCD_DATASTREAM_2:
            reg_val |= H264DCD_FIFO1_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            reg_write32(&reg_base->MCR, reg_val);

            reg_val &= ~H264DCD_FIFO1_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            break;

        case H264DCD_DATASTREAM_3:
            reg_val |= H264DCD_FIFO2_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            reg_write32(&reg_base->MCR, reg_val);

            reg_val &= ~H264DCD_FIFO2_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            break;

        case H264DCD_DATASTREAM_4:
            reg_val |= H264DCD_FIFO3_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            reg_write32(&reg_base->MCR, reg_val);

            reg_val &= ~H264DCD_FIFO3_FLUSH_MASK;
            reg_write32(&reg_base->MCR, reg_val);
            break;

        default:
            /* nothing to do */
            break;
    }
}

/*============================================================================*/
/**
* @brief          Set timeout for H264DCD
* @details        The function sets timeout for decoding operation
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      h264_timeout  Timeout value
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_timeoutset_Activity
*
* @note
*/
void H264dcd_timeoutset(H264DCD_REGS *reg_base, uint16_t h264_timeout)
{
    uint32_t reg_val = reg_read32(&reg_base->TIMEOUT_CFG);

    reg_val &= ~H264DCD_TIMEOUT_MASK;
    reg_val |= (h264_timeout & H264DCD_TIMEOUT_MASK);

    reg_write32(&reg_base->TIMEOUT_CFG, reg_val);
}

/*============================================================================*/
/**
* @brief          Enable timeout for H264DCD
* @details        The function enables timeout for decoding operation
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      to_en         Enable flag
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_timeouten_Activity
*
* @note
*/
void H264dcd_timeouten(H264DCD_REGS *reg_base, H264DCD_TIMEOUT_ENABLE to_en)
{
    uint32_t reg_val = reg_read32(&reg_base->TIMEOUT_CFG);

    switch (to_en.stream_no)
    {
        case H264DCD_DATASTREAM_1:
            if (H264DCD_ON == to_en.to_enable)
            {
                reg_val |= H264DCD_TIMEOUT_EN_STR0_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_TIMEOUT_EN_STR0_MASK;
            }
            break;

        case H264DCD_DATASTREAM_2:
            if (H264DCD_ON == to_en.to_enable)
            {
                reg_val |= H264DCD_TIMEOUT_EN_STR1_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_TIMEOUT_EN_STR1_MASK;
            }
            break;

        case H264DCD_DATASTREAM_3:
            if (H264DCD_ON == to_en.to_enable)
            {
                reg_val |= H264DCD_TIMEOUT_EN_STR2_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_TIMEOUT_EN_STR2_MASK;
            }
            break;

        case H264DCD_DATASTREAM_4:
            if (H264DCD_ON == to_en.to_enable)
            {
                reg_val |= H264DCD_TIMEOUT_EN_STR3_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_TIMEOUT_EN_STR3_MASK;
            }
            break;

        default:
            /* nothing to do */
            break;
    }
    reg_write32(&reg_base->TIMEOUT_CFG, reg_val);
}

/*============================================================================*/
/**
* @brief          SW reset
* @details        The function sets and resets the SW_RST bit resetting
*                 the Input DMA Wrapper, the Output DMA Wrapper and
*                 the H264 Decoder Core
*
* @param[in]      reg_base    The base address of the H264 registers bank
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_swreset_Activity
*
* @note
*/
void H264dcd_swreset(H264DCD_REGS *reg_base)
{
    uint32_t reg_temp = reg_read32(&reg_base->MCR);

    reg_temp |= H264DCD_SW_RESET_MASK;
    reg_write32(&reg_base->MCR,reg_temp);

    reg_temp &= ~H264DCD_SW_RESET_MASK;
    reg_write32(&reg_base->MCR, reg_temp);
}

/*============================================================================*/
/**
* @brief          Set the parameters for output data
* @details        The function sets for the selected data stream the components
*                 buffer addresses and the number of lines per component used in
*                 the luma output circular component buffer
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      output_cfg    H264DCD_OUTPUT_BUFFCFG structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_output_cfg_Activity
*
* @note
*/
void H264dcd_output_cfg(H264DCD_REGS *reg_base, const H264DCD_OUTPUT_BUFFCFG *output_cfg)
{
    uint32_t y_buff, cb_buff, cr_buff, line_num = 0;

    y_buff = output_cfg->y_outbuffer & H264DCD_OUT_BUF_ADDR_MASK;
    cb_buff = output_cfg->cb_outbuffer & H264DCD_OUT_BUF_ADDR_MASK;
    cr_buff = output_cfg->cr_outbuffer & H264DCD_OUT_BUF_ADDR_MASK;
    line_num = ((uint32_t)output_cfg->lines_num << H264DCD_OUT_LUMA_LINES_SHIFT) & H264DCD_OUT_LUMA_LINES_MASK;

    switch (output_cfg->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_write32(&reg_base->VO_STR0_Y_ADDR, y_buff);
            reg_write32(&reg_base->VO_STR0_CB_ADDR, cb_buff);
            reg_write32(&reg_base->VO_STR0_CR_ADDR, cr_buff);
            reg_write32(&reg_base->VO_STR0_NRLINES, line_num);
            break;
        case H264DCD_DATASTREAM_2:
            reg_write32(&reg_base->VO_STR1_Y_ADDR, y_buff);
            reg_write32(&reg_base->VO_STR1_CB_ADDR, cb_buff);
            reg_write32(&reg_base->VO_STR1_CR_ADDR, cr_buff);
            reg_write32(&reg_base->VO_STR1_NRLINES, line_num);
            break;
        case H264DCD_DATASTREAM_3:
            reg_write32(&reg_base->VO_STR2_Y_ADDR, y_buff);
            reg_write32(&reg_base->VO_STR2_CB_ADDR, cb_buff);
            reg_write32(&reg_base->VO_STR2_CR_ADDR, cr_buff);
            reg_write32(&reg_base->VO_STR2_NRLINES, line_num);
            break;
        case H264DCD_DATASTREAM_4:
            reg_write32(&reg_base->VO_STR3_Y_ADDR, y_buff);
            reg_write32(&reg_base->VO_STR3_CB_ADDR, cb_buff);
            reg_write32(&reg_base->VO_STR3_CR_ADDR, cr_buff);
            reg_write32(&reg_base->VO_STR3_NRLINES, line_num);
            break;
        default:
            /* nothing to do */
            break;
    }
}

/*============================================================================*/
/**
* @brief          Change resampling mode
* @details        The function sets sampling mode according to
*                 outputregsmpl_mode variable
*
* @param[in]      reg_base              The base address of the H264DCD registers bank
* @param[in]      outputregsmpl_mode    H264DCD_OUTPUT_SAMPLEMODE structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_outsamplemode_set_Activity
*
* @note
*/
void H264dcd_outsamplemode_set(H264DCD_REGS *reg_base, H264DCD_OUTPUT_SAMPLEMODE outputregsmpl_mode)
{
    uint32_t reg_val = reg_read32(&reg_base->MCR);

    switch (outputregsmpl_mode.stream_no)
    {
        case H264DCD_DATASTREAM_1:
            if (H264DCD_ON == outputregsmpl_mode.outresmpl_mode)
            {
                reg_val |= H264DCD_OUT0_BUFFRST_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_OUT0_BUFFRST_MASK;
            }
            break;

        case H264DCD_DATASTREAM_2:
            if (H264DCD_ON == outputregsmpl_mode.outresmpl_mode)
            {
                reg_val |= H264DCD_OUT1_BUFFRST_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_OUT1_BUFFRST_MASK;
            }
            break;

        case H264DCD_DATASTREAM_3:
            if (H264DCD_ON == outputregsmpl_mode.outresmpl_mode)
            {
                reg_val |= H264DCD_OUT2_BUFFRST_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_OUT2_BUFFRST_MASK;
            }
            break;

        case H264DCD_DATASTREAM_4:
            if (H264DCD_ON == outputregsmpl_mode.outresmpl_mode)
            {
                reg_val |= H264DCD_OUT3_BUFFRST_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_OUT3_BUFFRST_MASK;
            }
            break;

        default:
            /* nothing to do */
            break;
    }

    reg_write32(&reg_base->MCR, reg_val);
}

/*============================================================================*/
/**
* @brief          Set colour format
* @details        The function sets the colour format and the component
*                 samples bit width for the selected data stream
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      out_coldata   H264DCD_OUTPUT_COLOURDATA structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_outcolourdata_set_Activity
*
* @note
*/
void H264dcd_outcolourdata_set(H264DCD_REGS *reg_base, const H264DCD_OUTPUT_COLOURDATA *out_coldata)
{
    uint32_t reg_val = reg_read32(&reg_base->MCR);

    reg_val &= ~(H264DCD_CHROMA_DATAMODE_MASK);
    reg_val |= (((uint32_t)out_coldata->data_mode_format) << H264DCD_CHROMA_DATAMODE_SHIFT);

    switch (out_coldata->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_val &= ~(H264DCD_OUT0_COLFORMAT_MASK | H264DCD_OUT0_BITWIDTH_MASK);
            reg_val |= (((uint32_t)out_coldata->col_format) << H264DCD_OUT0_COLFORMAT_SHIFT);
            reg_val |= (((uint32_t)out_coldata->bit_width_mode) << H264DCD_OUT0_BITWIDTH_SHIFT);
            break;

        case H264DCD_DATASTREAM_2:
            reg_val &= ~(H264DCD_OUT1_COLFORMAT_MASK | H264DCD_OUT1_BITWIDTH_MASK);
            reg_val |= (((uint32_t)out_coldata->col_format) << H264DCD_OUT1_COLFORMAT_SHIFT);
            reg_val |= (((uint32_t)out_coldata->bit_width_mode) << H264DCD_OUT1_BITWIDTH_SHIFT);
            break;

        case H264DCD_DATASTREAM_3:
            reg_val &= ~(H264DCD_OUT2_COLFORMAT_MASK | H264DCD_OUT2_BITWIDTH_MASK);
            reg_val |= (((uint32_t)out_coldata->col_format) << H264DCD_OUT2_COLFORMAT_SHIFT);
            reg_val |= (((uint32_t)out_coldata->bit_width_mode) << H264DCD_OUT2_BITWIDTH_SHIFT);
            break;

        case H264DCD_DATASTREAM_4:
            reg_val &= ~(H264DCD_OUT3_COLFORMAT_MASK | H264DCD_OUT3_BITWIDTH_MASK);
            reg_val |= (((uint32_t)out_coldata->col_format) << H264DCD_OUT3_COLFORMAT_SHIFT);
            reg_val |= (((uint32_t)out_coldata->bit_width_mode) << H264DCD_OUT3_BITWIDTH_SHIFT);
            break;

        default:
            /* nothing to do */
            break;
    }
    reg_write32(&reg_base->MCR, reg_val);
}

/*============================================================================*/
/**
* @brief          Set flow mode
* @details        The function sets the flow mode
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[in]      data_mode     Mode
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_flowmode_set_Activity
*
* @note
*/
void H264dcd_flowmode_set(H264DCD_REGS *reg_base, H264DCD_DATAFLOW_MODE data_mode)
{
    uint32_t reg_val = reg_read32(&reg_base->MCR);

    reg_val &= ~H264DCD_DATAFLOW_MODE_MASK;
    reg_val |= ((((uint32_t)data_mode) << H264DCD_DATAFLOW_MODE_SHIFT) & H264DCD_DATAFLOW_MODE_MASK);
    reg_write32(&reg_base->MCR, reg_val);
}

/*============================================================================*/
/**
* @brief          Get flow mode
* @details        The function gets the flow mode
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     data_mode     Mode
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_flowmode_get_Activity
*
* @note
*/
void H264dcd_flowmode_get(const H264DCD_REGS *reg_base, H264DCD_DATAFLOW_MODE *data_mode)
{
    uint32_t reg_val = reg_read32(&reg_base->MCR);

    reg_val &= H264DCD_DATAFLOW_MODE_MASK;
    switch (reg_val >> H264DCD_DATAFLOW_MODE_SHIFT)
    {
        case 0:
            *data_mode = MODE_0;
            break;
        case 1:
            *data_mode = MODE_1;
            break;
        case 2:
            *data_mode = MODE_2;
            break;
        default:
            /* nothing to do */
            break;
    }
}

/*============================================================================*/
/**
* @brief          Set bankstride
* @details        The function sets the address offset of each
*                 start of macroblock
*
* @param[in]      reg_base     The base address of the H264DCD registers bank
* @param[in]      out_bks      Address offset
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_bankstride_set_Activity
*
* @note
*/
void H264dcd_bankstride_set(H264DCD_REGS *reg_base, H264DCD_OUTPUT_BKSTRIDE out_bks)
{
    uint32_t val_temp  = 0;

    val_temp = ((((uint32_t)out_bks.bytes_num) <<
        H264DCD_BNKSTR_LENGTH_SHIFT) & H264DCD_BNKSTR_LENGTH_MASK);
    val_temp |= ((((uint32_t)out_bks.bks_enable) <<
            H264DCD_RATEFLOW_EN_SHIFT) & H264DCD_RATEFLOW_EN_MASK);

    reg_write32(&reg_base->RATE_FLOW_CNTRL, val_temp);
}

/*============================================================================*/
/**
* @brief          Get bankstride
* @details        The function gets the address offset of each
*                 start of macroblock
*
* @param[in]      reg_base     The base address of the H264DCD registers bank
* @param[out]     out_bks      Address offset
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_bankstride_get_Activity
*
* @note
*/
void H264dcd_bankstride_get(const H264DCD_REGS *reg_base, H264DCD_OUTPUT_BKSTRIDE *out_bks)
{
    uint32_t val_temp  = reg_read32(&reg_base->RATE_FLOW_CNTRL);

    out_bks->bytes_num = (uint8_t)((val_temp & H264DCD_BNKSTR_LENGTH_MASK)
                    >> H264DCD_BNKSTR_LENGTH_SHIFT);
    if (((val_temp & H264DCD_RATEFLOW_EN_MASK) >> H264DCD_RATEFLOW_EN_SHIFT) != 0U)
    {
        out_bks->bks_enable = H264DCD_ON;
    }
    else
    {
        out_bks->bks_enable = H264DCD_OFF;
    }
}

/*============================================================================*/
/**
* @brief          Channel control
* @details        The function controls channel activity
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[in]      channel_ctrl   Channel status
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_dcd_chcontrol_Activity
*
* @note
*/
void H264dcd_dcd_chcontrol(H264DCD_REGS *reg_base, H264DCD_CH_CONTROL channel_ctrl)
{
    uint32_t reg_val = reg_read32(&reg_base->ELLVC_CFG);

    switch (channel_ctrl.stream_no)
    {
        case H264DCD_DATASTREAM_1:
            if (CH_STOP == channel_ctrl.channel_status)
            {
                reg_val |= H264DCD_CH0_STOP_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_CH0_STOP_MASK;
            }
            break;

        case H264DCD_DATASTREAM_2:
            if (CH_STOP == channel_ctrl.channel_status)
            {
                reg_val |= H264DCD_CH1_STOP_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_CH1_STOP_MASK;
            }
            break;

        case H264DCD_DATASTREAM_3:
            if (CH_STOP == channel_ctrl.channel_status)
            {
                reg_val |= H264DCD_CH2_STOP_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_CH2_STOP_MASK;
            }
            break;

        case H264DCD_DATASTREAM_4:
            if (CH_STOP == channel_ctrl.channel_status)
            {
                reg_val |= H264DCD_CH3_STOP_MASK;
            }
            else
            {
                reg_val &= ~H264DCD_CH3_STOP_MASK;
            }
            break;

        default:
            /* nothing to do */
            break;
    }
    reg_write32(&reg_base->ELLVC_CFG, reg_val);
}

/*============================================================================*/
/**
* @brief          Set the coded image size
* @details        The function sets the coded image size in terms of macroblocks
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[in]      im_data       frame_size structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_dcd_inframesz_Activity
*
* @note
*/
void H264dcd_dcd_inframesz(H264DCD_REGS *reg_base, const H264DCD_INFRAME_SIZE *im_data)
{
    uint32_t reg_value = 0;

    reg_value |= ((uint32_t)im_data->x_size) << H264DCD_FRAME_WIDTH_SHIFT;
    reg_value |= ((uint32_t)im_data->y_size) << H264DCD_FRAME_HEIGHT_SHIFT;
    reg_write32(&reg_base->STR_VCR[im_data->stream_no], reg_value);
}

/*============================================================================*/
/**
* @brief          Configure decoder
* @details        The function sets the type of the used memory SRAM or DDR
*                 and enable or disable the deblocking filters
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[in]      dcd_cfg       H264DCD_DECOD_CONFIG structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_dcd_cfgset_Activity
*
* @note
*/
void H264dcd_dcd_cfgset(H264DCD_REGS *reg_base, const H264DCD_DECOD_CONFIG *dcd_cfg)
{
    uint32_t reg_val = reg_read32(&reg_base->ELLVC_CFG);

    reg_val &= ~(H264DCD_MEM_TYPE_MASK | H264DCD_DEBLOCKFILTER_OFF_MASK);

    if (MEMORY_SRAM == dcd_cfg->mem_type)
    {
        reg_val |= H264DCD_MEM_TYPE_MASK;
    }

    if (H264DCD_OFF == dcd_cfg->filter_en)
    {
        reg_val |= H264DCD_DEBLOCKFILTER_OFF_MASK;
    }

    reg_write32(&reg_base->ELLVC_CFG, reg_val);
}

/*============================================================================*/
/**
* @brief          Get the decoder configuration
* @details        The function returns the type of the used memory SRAM or DDR
*                 and enable or disable the de-blocking filters
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[out]     dcd_cfg       H264DCD_DECOD_CONFIG structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_dcd_cfgget_Activity
*
* @note
*/
void H264dcd_dcd_cfgget(const H264DCD_REGS *reg_base, H264DCD_DECOD_CONFIG *dcd_cfg)
{
    uint32_t reg_val = reg_read32(&reg_base->ELLVC_CFG);

    if (0U != (reg_val & H264DCD_MEM_TYPE_MASK))
    {
        dcd_cfg->mem_type = MEMORY_SRAM;
    }
    else
    {
        dcd_cfg->mem_type = MEMORY_DDR;
    }

    if (0U != (reg_val & H264DCD_DEBLOCKFILTER_OFF_MASK))
    {
        dcd_cfg->filter_en = H264DCD_OFF;
    }
    else
    {
        dcd_cfg->filter_en = H264DCD_ON;
    }
}

/*============================================================================*/
/**
* @brief          Set threshold level
* @details        The function sets the threshold level
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[in]      dcd_level      Threshold level
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_thrlevels_set_Activity
*
* @note
*/
void H264dcd_thrlevels_set(H264DCD_REGS *reg_base, const H264DCD_DECOD_THRLEVELS *dcd_level)
{
    reg_write32(&reg_base->ELLVC_DBG_FRAME_CYC_CNT_THR[0], dcd_level->level1);
    reg_write32(&reg_base->ELLVC_DBG_FRAME_CYC_CNT_THR[1], dcd_level->level2);
    reg_write32(&reg_base->ELLVC_DBG_FRAME_CYC_CNT_THR[2], dcd_level->level3);
}

/*============================================================================*/
/**
* @brief          Get threshold level
* @details        The function gets the threshold level
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[out]     dcd_level      Threshold level
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_thrlevels_get_Activity
*
* @note
*/
void H264dcd_thrlevels_get(const H264DCD_REGS *reg_base, H264DCD_DECOD_THRLEVELS *dcd_level)
{
    dcd_level->level1 = reg_read32(&reg_base->ELLVC_DBG_FRAME_CYC_CNT_THR[0]);
    dcd_level->level2 = reg_read32(&reg_base->ELLVC_DBG_FRAME_CYC_CNT_THR[1]);
    dcd_level->level3 = reg_read32(&reg_base->ELLVC_DBG_FRAME_CYC_CNT_THR[2]);
}

/*============================================================================*/
/**
* @brief          Set address for reference data
* @details        The function sets the address of the memory zone where the
*                 decoder reads or stores the reference data for the
*                 selected channel
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[in]      refmem_add     Memory address
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_refmemaddress_set_Activity
*
* @note
*/
void H264dcd_refmemaddress_set(H264DCD_REGS *reg_base, H264DCD_DECOD_REFMEM refmem_add)
{
    reg_write32(&reg_base->ELLVC_REF_ADDR_CH[refmem_add.stream_no], refmem_add.refmem_addr);
}

/*============================================================================*/
/**
* @brief          Get address for reference data
* @details        The function gets the address of the memory zone where the
*                 decoder reads or stores the reference data for the
*                 selected channel
*
* @param[in]      reg_base       The base address of the H264DCD registers bank
* @param[out]     refmem_add     Memory address
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_refmemaddress_get_Activity
*
* @note
*/
void H264dcd_refmemaddress_get(const H264DCD_REGS *reg_base, H264DCD_DECOD_REFMEM *refmem_add)
{
    refmem_add->refmem_addr = reg_read32(&reg_base->ELLVC_REF_ADDR_CH[refmem_add->stream_no]);
}

/*============================================================================*/
/**
* @brief          Get channel status
* @details        The function gets the status of the selected decoder channel
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     ch_st         Channel status
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_chstatus_get_Activity
*
* @note
*/
void H264dcd_chstatus_get(const H264DCD_REGS *reg_base, H264DCD_DECOD_CHANNELST *ch_st)
{
    uint32_t reg_value = reg_read32(&reg_base->ELLVC_STATE);

    switch (ch_st->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_value = (reg_value & H264DCD_CH0_STATE_MASK) >> H264DCD_CH0_STATE_SHIFT;
            break;

        case H264DCD_DATASTREAM_2:
            reg_value = (reg_value & H264DCD_CH1_STATE_MASK) >> H264DCD_CH1_STATE_SHIFT;
            break;

        case H264DCD_DATASTREAM_3:
            reg_value = (reg_value & H264DCD_CH2_STATE_MASK) >> H264DCD_CH2_STATE_SHIFT;
            break;

        case H264DCD_DATASTREAM_4:
            reg_value = (reg_value & H264DCD_CH3_STATE_MASK) >> H264DCD_CH3_STATE_SHIFT;
            break;

        default:
            /* nothing to do */
            break;
    }

    ch_st->dcd_chst = interpret_ch_status(reg_value);
}

/*============================================================================*/
/**
* @brief          Get decoded image parameters
* @details        The function gets the parameters of the decoded image for
*                 the selected channel
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     picture_st    Decoded image parameters
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_pictureparam_get_Activity
*
* @note
*/
void H264dcd_pictureparam_get(const H264DCD_REGS *reg_base, H264DCD_PIC_PARAMS *picture_st)
{
    uint32_t reg_val, reg_pic_bit, reg_pic_width, reg_pic_height = 0;

    /* set sample_bit with an invalid value */
    uint32_t sample_bit = 9999U;

    reg_pic_bit = reg_read32(&reg_base->ELLVC_PIC_BIT);
    reg_pic_width = reg_read32(&reg_base->ELLVC_PIC_WIDTH_IN_MBS);
    reg_pic_height = reg_read32(&reg_base->ELLVC_PIC_HEIGHT_IN_MBS);

    switch (picture_st->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            sample_bit = ((reg_pic_bit & H264DCD_CH0_SAMPLE_BITDEPTH_MASK)
                        >> H264DCD_CH0_SAMPLE_BITDEPTH_SHIFT);

            picture_st->width_mbc =
                    (uint8_t)((reg_pic_width & H264DCD_CH0_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH0_WIDTH_HEIGHT_SHIFT);

            picture_st->height_mbc =
                    (uint8_t)((reg_pic_height & H264DCD_CH0_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH0_WIDTH_HEIGHT_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_LEFT_CH_01);
            picture_st->crop_left = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_RGHT_CH_01);
            picture_st->crop_right = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_TOP_CH_01);
            picture_st->crop_top = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_BTTM_CH_01);
            picture_st->crop_bottom = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);
            break;

        case H264DCD_DATASTREAM_2:
            sample_bit = ((reg_pic_bit & H264DCD_CH1_SAMPLE_BITDEPTH_MASK)
                        >> H264DCD_CH1_SAMPLE_BITDEPTH_SHIFT);

            picture_st->width_mbc =
                    (uint8_t)((reg_pic_width & H264DCD_CH1_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH1_WIDTH_HEIGHT_SHIFT);

            picture_st->height_mbc =
                    (uint8_t)((reg_pic_height & H264DCD_CH1_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH1_WIDTH_HEIGHT_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_LEFT_CH_01);
            picture_st->crop_left = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_RGHT_CH_01);
            picture_st->crop_right = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_TOP_CH_01);
            picture_st->crop_top = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_BTTM_CH_01);
            picture_st->crop_bottom = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);
            break;

        case H264DCD_DATASTREAM_3:
            sample_bit = ((reg_pic_bit & H264DCD_CH2_SAMPLE_BITDEPTH_MASK)
                        >> H264DCD_CH2_SAMPLE_BITDEPTH_SHIFT);

            picture_st->width_mbc =
                    (uint8_t)((reg_pic_width & H264DCD_CH2_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH2_WIDTH_HEIGHT_SHIFT);

            picture_st->height_mbc =
                    (uint8_t)((reg_pic_height & H264DCD_CH2_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH2_WIDTH_HEIGHT_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_LEFT_CH_23);
            picture_st->crop_left = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_RGHT_CH_23);
            picture_st->crop_right = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_TOP_CH_23);
            picture_st->crop_top = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_BTTM_CH_23);
            picture_st->crop_bottom = (uint16_t)((reg_val & H264DCD_CHY_LINES_MASK)
                            >> H264DCD_CHY_LINES_SHIFT);
            break;

        case H264DCD_DATASTREAM_4:
            sample_bit = ((reg_pic_bit & H264DCD_CH3_SAMPLE_BITDEPTH_MASK)
                        >> H264DCD_CH3_SAMPLE_BITDEPTH_SHIFT);

            picture_st->width_mbc =
                    (uint8_t)((reg_pic_width & H264DCD_CH3_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH3_WIDTH_HEIGHT_SHIFT);

            picture_st->height_mbc =
                    (uint8_t)((reg_pic_height & H264DCD_CH3_WIDTH_HEIGHT_MASK)
                        >> H264DCD_CH3_WIDTH_HEIGHT_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_LEFT_CH_23);
            picture_st->crop_left = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_RGHT_CH_23);
            picture_st->crop_right = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_TOP_CH_23);
            picture_st->crop_top = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);

            reg_val = reg_read32(&reg_base->ELLVC_PIC_CROP_BTTM_CH_23);
            picture_st->crop_bottom = (uint16_t)((reg_val & H264DCD_CHX_LINES_MASK)
                            >> H264DCD_CHX_LINES_SHIFT);
            break;

        default:
            /* nothing to do */
            break;
    }

    switch (sample_bit)
    {
        case 0:
            picture_st->sample_bit = H264DCD_PREC_8BITS;
            break;
        case 1:
            picture_st->sample_bit = H264DCD_PREC_10BITS;
            break;
        case 2:
            picture_st->sample_bit = H264DCD_PREC_12BITS;
            break;
        default:
            /* nothing to do */
            break;
    }
}

/*============================================================================*/
/**
* @brief          Get decoded image status
* @details        The function gets the status of the decoded image for
*                 the selected channel
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     pic_valid     Picture status
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_picturestatus_get_Activity
*
* @note
*/
void H264dcd_picturestatus_get(const H264DCD_REGS *reg_base, H264DCD_PIC_STATUS *pic_valid)
{
    uint32_t reg_value = reg_read32(&reg_base->ELLVC_PIC_PARAM_VLD);

    switch (pic_valid->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_value &= H264DCD_CH0_PARAM_VALID_MASK;
            break;

        case H264DCD_DATASTREAM_2:
            reg_value &= H264DCD_CH1_PARAM_VALID_MASK;
            break;

        case H264DCD_DATASTREAM_3:
            reg_value &= H264DCD_CH2_PARAM_VALID_MASK;
            break;

        case H264DCD_DATASTREAM_4:
            reg_value &= H264DCD_CH3_PARAM_VALID_MASK;
            break;

        default:
            /*nothing to do */
            break;
    }

    if (0U != reg_value)
    {
        pic_valid->pic_prmvalid = H264DCD_ON;
    }
    else
    {
        pic_valid->pic_prmvalid = H264DCD_OFF;
    }
}

/*============================================================================*/
/**
* @brief          Get decoded image general parameters
* @details        The function gets the general parameters of the decoded image
*                 for the selected channel
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     poc_st        Decoded image general parameters
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_picgenparams_get_Activity
*
* @note
*/
void H264dcd_picgenparams_get(const H264DCD_REGS *reg_base, H264DCD_PIC_GENPARAMS *poc_st)
{
    uint32_t reg_val = reg_read32(&reg_base->ELLVC_STAT);

    poc_st->frame_nummax = (uint8_t)((reg_val & H264DCD_MAX_REFFRAMENUM_MASK) >>
                        H264DCD_MAX_REFFRAMENUM_SHIFT);

    if (0U != (reg_val & H264DCD_NEW_POC_CYCLE_MASK))
    {
        poc_st->frame_poccycle = H264DCD_ON;
    }
    else
    {
        poc_st->frame_poccycle = H264DCD_OFF;
    }

    poc_st->frame_count = reg_read32(&reg_base->ELLVC_POC_VALUE);
}

/*============================================================================*/
/**
* @brief          Set interrupts
* @details        The function enables or disables the ERR_REQ_EN,
*                 STREAM_TIME_OUT, END_OF_FRAME_x, MBROW_DONE_x,
*                 STRx_FIFO_WATERMARK, DEC_OOS_STRx, FRM_CYCCNT_GRT_THRy_x
*                 interrupts according to the bit mask sent as parameter
*
* @param[in]      reg_base    The base address of the H264DCD registers bank
* @param[in]      intr_mask   Interrupt mask
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_interrupt_ctrl_Activity
*
* @note
*/
void H264dcd_interrupt_ctrl(H264DCD_REGS *reg_base, uint32_t intr_mask)
{
    uint32_t reg_temp =  reg_read32(&reg_base->IER);

    reg_temp &= ~H264DCD_INT_ALL_MASK;
    reg_temp |= intr_mask;

    reg_write32(&reg_base->IER, reg_temp);
}

/*============================================================================*/
/**
* @brief          Get interrupts
* @details        The function returns which of the interrupts are enabled
*                 at the moment
*
* @param[in]      reg_base    The base address of the H264DCD registers bank
* @param[out]     intr_mask   Interrupt mask
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_interrupt_get_Activity
*
* @note
*/
void H264dcd_interrupt_get(const H264DCD_REGS *reg_base, uint32_t *intr_mask)
{
    uint32_t reg_temp =  reg_read32(&reg_base->IER);

    *intr_mask = reg_temp;
}

/*============================================================================*/
/**
* @brief          Get timeout interrupts
* @details        The function returns the timeout interrupt flag and the
*                 timeout status byte. If the flag is set the function resets it
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     timeout_irq   H264DCD_IRQ_TIMEOUT structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_timeoutirq_get_Activity
*
* @note
*/
void H264dcd_timeoutirq_get(H264DCD_REGS *reg_base, H264DCD_IRQ_TIMEOUT *timeout_irq)
{
    uint32_t reg_temp =  reg_read32(&reg_base->STAT);
    timeout_irq->timeout_st = (uint8_t)(reg_temp & H264DCD_ERR_TO_FLAG_MASK);

    reg_temp = reg_read32(&reg_base->ISR);
    reg_temp &= H264DCD_INT_TIMEOUT_MASK;

    if (0U != reg_temp)
    {
        reg_write32(&reg_base->ISR, H264DCD_INT_TIMEOUT_MASK);
        timeout_irq->timeout_flag = H264DCD_ON;
    }
    else
    {
        timeout_irq->timeout_flag = H264DCD_OFF;
    }
}

/*============================================================================*/
/**
* @brief          Get error interrupts
* @details        The function returns the decoding error interrupt flag and
*                 the 4 channel status bytes. If the flag is set the function
*                 resets it
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     error_irq     H264DCD_IRQ_ERROR structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_errorirq_get_Activity
*
* @note
*/
void H264dcd_errorirq_get(H264DCD_REGS *reg_base, H264DCD_IRQ_ERROR *error_irq)
{
    uint32_t reg_temp = reg_read32(&reg_base->ISR);
    reg_temp &= H264DCD_INT_ERR_MASK;

    if (0U != reg_temp)
    {
        reg_write32(&reg_base->ISR, H264DCD_INT_ERR_MASK);
        error_irq->error_flag = H264DCD_ON;
    }
    else
    {
        error_irq->error_flag = H264DCD_OFF;
    }

    reg_temp = reg_read32(&reg_base->ELLVC_STATE);
	error_irq->status_ch0 = interpret_ch_status((reg_temp & H264DCD_CH0_STATE_MASK)
                            >> H264DCD_CH0_STATE_SHIFT);
	error_irq->status_ch1 = interpret_ch_status((reg_temp & H264DCD_CH1_STATE_MASK)
                            >> H264DCD_CH1_STATE_SHIFT);
	error_irq->status_ch2 = interpret_ch_status((reg_temp & H264DCD_CH2_STATE_MASK)
                            >> H264DCD_CH2_STATE_SHIFT);
	error_irq->status_ch3 = interpret_ch_status((reg_temp & H264DCD_CH3_STATE_MASK)
                            >> H264DCD_CH3_STATE_SHIFT);
}

/*============================================================================*/
/**
* @brief          Get interrupts for stream
* @details        The function returns the status of the interrupt flags for
*                 the selected data stream. It shall reset all the interrupt
*                 flags which are already set
*
* @param[in]      reg_base      The base address of the H264DCD registers bank
* @param[out]     streamirq_st  H264DCD_IRQ_STREAM structure
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     H264dcd_streamirq_get_Activity
*
* @note
*/
void H264dcd_streamirq_get(H264DCD_REGS *reg_base, H264DCD_IRQ_STREAM *streamirq_st)
{
    uint32_t reg_temp = 0;
    uint32_t reg_val = reg_read32(&reg_base->ISR);

    if ((reg_val & H264DCD_INT_TIMEOUT_MASK) != 0U)
    {
        streamirq_st->timeout_flag = H264DCD_ON;
    }
    else
    {
        streamirq_st->timeout_flag = H264DCD_OFF;
    }

    switch (streamirq_st->stream_no)
    {
        case H264DCD_DATASTREAM_1:
            reg_temp = reg_val & H264DCD_CH0_THR0_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH0_THR0_MASK);
                streamirq_st->thr0_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr0_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH0_THR1_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH0_THR1_MASK);
                streamirq_st->thr1_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr1_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH0_THR2_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH0_THR2_MASK);
                streamirq_st->thr2_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr2_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR0_OOS_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR0_OOS_MASK);
                streamirq_st->oos_flag = H264DCD_ON;
            } else
            {
                streamirq_st->oos_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR0_FIFO_WTM_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR0_FIFO_WTM_MASK);
                streamirq_st->fifo_wtm_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->fifo_wtm_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR0_MBROW_DONE_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR0_MBROW_DONE_MASK);
                streamirq_st->mbrow_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->mbrow_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR0_EOF_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR0_EOF_MASK);
                streamirq_st->eof_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->eof_flag = H264DCD_OFF;
            }
            break;

        case H264DCD_DATASTREAM_2:
            reg_temp = reg_val & H264DCD_CH1_THR0_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH1_THR0_MASK);
                streamirq_st->thr0_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr0_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH1_THR1_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH1_THR1_MASK);
                streamirq_st->thr1_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr1_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH1_THR2_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH1_THR2_MASK);
                streamirq_st->thr2_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr2_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR1_OOS_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR1_OOS_MASK);
                streamirq_st->oos_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->oos_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR1_FIFO_WTM_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR1_FIFO_WTM_MASK);
                streamirq_st->fifo_wtm_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->fifo_wtm_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR1_MBROW_DONE_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR1_MBROW_DONE_MASK);
                streamirq_st->mbrow_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->mbrow_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR1_EOF_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR1_EOF_MASK);
                streamirq_st->eof_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->eof_flag = H264DCD_OFF;
            }
            break;

        case H264DCD_DATASTREAM_3:
            reg_temp = reg_val & H264DCD_CH2_THR0_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH2_THR0_MASK);
                streamirq_st->thr0_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr0_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH2_THR1_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH2_THR1_MASK);
                streamirq_st->thr1_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr1_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH2_THR2_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH2_THR2_MASK);
                streamirq_st->thr2_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr2_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR2_OOS_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR2_OOS_MASK);
                streamirq_st->oos_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->oos_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR2_FIFO_WTM_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR2_FIFO_WTM_MASK);
                streamirq_st->fifo_wtm_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->fifo_wtm_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR2_MBROW_DONE_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR2_MBROW_DONE_MASK);
                streamirq_st->mbrow_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->mbrow_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR2_EOF_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR2_EOF_MASK);
                streamirq_st->eof_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->eof_flag = H264DCD_OFF;
            }
            break;

        case H264DCD_DATASTREAM_4:
            reg_temp = reg_val & H264DCD_CH3_THR0_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH3_THR0_MASK);
                streamirq_st->thr0_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr0_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH3_THR1_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH3_THR1_MASK);
                streamirq_st->thr1_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr1_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_CH3_THR2_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_CH3_THR2_MASK);
                streamirq_st->thr2_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->thr2_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR3_OOS_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR3_OOS_MASK);
                streamirq_st->oos_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->oos_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR3_FIFO_WTM_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR3_FIFO_WTM_MASK);
                streamirq_st->fifo_wtm_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->fifo_wtm_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR3_MBROW_DONE_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR3_MBROW_DONE_MASK);
                streamirq_st->mbrow_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->mbrow_flag = H264DCD_OFF;
            }

            reg_temp = reg_val & H264DCD_INT_STR3_EOF_MASK;
            if (0U != reg_temp)
            {
                reg_write32(&reg_base->ISR, H264DCD_INT_STR3_EOF_MASK);
                streamirq_st->eof_flag = H264DCD_ON;
            }
            else
            {
                streamirq_st->eof_flag = H264DCD_OFF;
            }
            break;

        default:
            /* nothing to do */
            break;
    }
}

static H264DCD_CH_STATUS interpret_ch_status (const uint32_t value)
{
    H264DCD_CH_STATUS status = CH_IDLE_ST;
    switch (value)
    {
        case 0:
            status = CH_IDLE_ST;
            break;
        case 1:
            status = CH_NAL_LOOKING;
            break;
        case 4:
            status = CH_SPS_DECOD;
            break;
        case 5:
            status = CH_PPS_DECOD;
            break;
        case 6:
            status = CH_SLICE_DECOD;
            break;
        case 8:
            status = CH_SPS_ERROR;
            break;
        case 9:
            status = CH_PPS_ERROR;
            break;
        case 10:
            status = CH_SLICE_ERROR;
            break;
        default:
            /* nothing to do */
            break;
    }
    return status;
}
