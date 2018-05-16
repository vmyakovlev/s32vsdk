/*
 * Copyright (c) 2016 Freescale Semiconductor
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
 
/*==============================================================================
Revision History:
                        Modification      Tracking
Author (core ID)        Date D/M/Y         Number     Description of Changes
Cristian Tomescu        04-Nov-2016       VSDK-488       Inital version.
Cristian Tomescu        22-Dec-2016       VSDK-317     MISRA 2012 compliant.
Cristian Tomescu        25-Jan-2017       VSDK-709     Adding compiling switch.
Cristian Tomescu        08-May-2017       VSDK-560     Fix MISRA violations
==============================================================================*/

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
*                                 INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==============================================================================*/
/**
* @file            viulite_core.c
*/
#include <linux/types.h>
#include "viulite_core.h"

/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/


/*==============================================================================
*                               LOCAL MACROS
==============================================================================*/


/*==============================================================================
*                       STRUCTURES AND OTHER TYPEDEFS
==============================================================================*/


/*==============================================================================
*                                   LOCAL MACROS
==============================================================================*/


/*==============================================================================
*                                  LOCAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                                  LOCAL VARIABLES
==============================================================================*/


/*==============================================================================
*                                 GLOBAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                                 GLOBAL VARIABLES
==============================================================================*/


/*==============================================================================
*                                LOCAL FUNCTION PROTOTYPES
==============================================================================*/
static void VIU_Isr(VIU_Registers_t *const viu_address);
/*==============================================================================
*                                LOCAL FUNCTIONS
==============================================================================*/


/*==============================================================================
*                                       GLOBAL FUNCTIONS
==============================================================================*/
/*============================================================================*/
/**
* @brief          Sets the format of the input video data.
* @details        The function sets the format of the input video data.
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[in]      in_format   VIU_INPUT_FORMAT struct
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     viulite_set_videoinputformat_Activity
*
* @note
*/
void viulite_set_videoinputformat(VIU_Registers_t *const reg_base,
           const VIU_INPUT_FORMAT *const in_format)
{
  uint32_t  reg_value;

  reg_value = reg_base->SCR;
  reg_value &= ~SCR_VIDEOIN_MASK;

  reg_value |= ((((uint32_t)(in_format->mode)) << SCR_ITUMODE_OFFSET)
          & SCR_ITUMODE_MASK);

  reg_value |= ((((uint32_t)(in_format->width)) << SCR_INWIDTH_OFFSET)
          & SCR_INWIDTH_MASK);

  reg_value |= ((((uint32_t)(in_format->clocks_per_pixell))
          << SCR_CPP_OFFSET) & SCR_CPP_MASK);

  reg_base->SCR = reg_value;
}

/*============================================================================*/
/**
* @brief          Get the parameters used to configure the video data format
* @details        The function returns the parameters used to configure the
*                 video data format
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[out]     in_format   VIU_INPUT_FORMAT struct
*
* @return         void
* @api
* @isr
*
* @pre                                Sets the format of the input video data
* @post
*
* @implements     viulite_get_videoinputformat_Activity
*
* @note
*/
void viulite_get_videoinputformat(const VIU_Registers_t *const reg_base,
                    VIU_INPUT_FORMAT *const in_format)
{
  uint32_t reg_value, t_param;
  reg_value = reg_base->SCR;

  t_param = (reg_value & SCR_ITUMODE_MASK) >> SCR_ITUMODE_OFFSET;

  switch (t_param)
  {
    case 0:
      in_format->mode = PARALLEL_MODE;
      break;

    case 1:
      in_format->mode = ITU_MODE;
      break;

    default:
      /* nothing to be done */
      break;
  } /* t_param 1 */

  t_param = (reg_value & SCR_INWIDTH_MASK) >> SCR_INWIDTH_OFFSET;

  switch (t_param)
  {
    case 0:
      in_format->width = WIDE_8_BITS;
      break;

    case 1:
      in_format->width = WIDE_10_BITS;
      break;

    case 2:
      in_format->width = WIDE_12_BITS;
      break;

    case 3:
      in_format->width = WIDE_14_BITS;
      break;

    case 4:
      in_format->width = WIDE_16_BITS;
      break;

    case 6:
      in_format->width = WIDE_20_BITS;
      break;

    default:
      /* nothing to be done */
      break;
  } /* t_param 2 */

  t_param = (reg_value & SCR_CPP_MASK) >> SCR_CPP_OFFSET;

  switch (t_param)
  {
    case 0:
      in_format->clocks_per_pixell = CPP_1_PP;
      break;

    case 1:
      in_format->clocks_per_pixell = CPP_2_PP;
      break;

    case 2:
      in_format->clocks_per_pixell = CPP_3_PP;
      break;

    default:
      /* nothing to be done */
      break;
  } /* t_param 3 */
} /* viulite_get_videoinputformat */

/*============================================================================*/
/**
* @brief          The function configures the data interface.
* @details        The function configures the data interface: polarity of
*                 synchro signals and endianness type. 
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[in]      data_interface   VIU_DATA_INTERFACE struct
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     viulite_set_datainterface_Activity
*
* @note
*/
void viulite_set_datainterface(VIU_Registers_t *const reg_base,
                   const VIU_DATA_INTERFACE *const data_interface)
{
  uint32_t  reg_value;

  reg_value = reg_base->SCR;
  reg_value &= ~SCR_PCLK_POL_MASK;
  reg_value &= ~SCR_VSYNC_POL_MASK;
  reg_value &= ~SCR_HSYNC_POL_MASK;
  reg_value &= ~SCR_ENDIANNESS_MASK;

  reg_value |= ((((uint32_t)(data_interface->pclk_pol))  <<
          SCR_PCLK_POL_OFFSET)  & SCR_PCLK_POL_MASK);
  reg_value |= ((((uint32_t)(data_interface->vsync_pol)) <<
          SCR_VSYNC_POL_OFFSET) & SCR_VSYNC_POL_MASK);
  reg_value |= ((((uint32_t)(data_interface->hsync_pol)) <<
          SCR_HSYNC_POL_OFFSET) & SCR_HSYNC_POL_MASK);
  reg_value |= ((((uint32_t)(data_interface->endianness)) <<
          SCR_ENDIANNESS_OFFSET) & SCR_ENDIANNESS_MASK);

  reg_base->SCR = reg_value;
}

/*============================================================================*/
/**
* @brief          The function returns the data interface parameters
* @details        The function get configures the data interface: polarity of
*                 synchro signals and endianness type. 
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[out]     data_interface   VIU_DATA_INTERFACE struct
*
* @return         void
* @api
* @isr
*
* @pre
* @post
*
* @implements     viulite_get_datainterface_Activity
*
* @note
*/
void viulite_get_datainterface(const VIU_Registers_t *const reg_base,
                VIU_DATA_INTERFACE *const data_interface)
{
  uint32_t  reg_value;

  reg_value = reg_base->SCR;

  data_interface->pclk_pol  = (uint8_t)((reg_value & SCR_PCLK_POL_MASK)
          >> SCR_PCLK_POL_OFFSET);
  data_interface->vsync_pol = (uint8_t)((reg_value & SCR_VSYNC_POL_MASK)
          >> SCR_VSYNC_POL_OFFSET);
  data_interface->hsync_pol = (uint8_t)((reg_value & SCR_HSYNC_POL_MASK)
          >> SCR_HSYNC_POL_OFFSET);
  data_interface->endianness = (uint8_t)((reg_value & SCR_ENDIANNESS_MASK)
          >> SCR_ENDIANNESS_OFFSET);
}

/*============================================================================*/
/**
* @brief          The function sets the DMA transfer parameters.
* @details        The function sets the DMA transfer parameters: size,
*                 destination buffer address, expected number of pixels per line
*                 and expected number of lines per frame and eventually the
*                 increment and alpha value.
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[in]      dmaconfig   DMA_CONFIG structure
* @return         void
* @api
* @isr
*
* @pre            - Setup the VIU reg virtual base address
*                 - OPEN VIU: get file descriptor
* @post
*
* @implements     viulite_dma_config_Activity
*
* @note
*/
void viulite_dma_config(VIU_Registers_t *const reg_base,
                        const DMA_CONFIG *const dmaconfig)
{
  uint32_t  reg_value;

  reg_base->DMA_ADDR = dmaconfig->buff_addr;
  reg_base->DMA_SIZE = (uint32_t)dmaconfig->tx_size;

  reg_value = (uint32_t) (((uint32_t)(dmaconfig->nmb_lines)) << 16)
          | (uint32_t)(dmaconfig->nmb_pixells);
  reg_base->INVSZ = reg_value;

  reg_base->DMA_INC = (uint32_t)dmaconfig->frame_inc;
  reg_base->ALPHA = (uint32_t)dmaconfig->alpha_val;
}

/*============================================================================*/
 /**
* @brief          The function starts the DMA transfer.
* @details        The function starts the DMA transfer of an image frame to the
*                 memory using the configuring data previously set
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @return         void
* @api
* @isr
*
* @pre            - Setup the VIU reg virtual base address
*                 - OPEN VIU: get file descriptor
* @post
*
* @implements     viulite_dma_start_Activity
*
* @note
*/
void viulite_dma_start(VIU_Registers_t *const reg_base)
{
  uint32_t  reg_value;
  reg_value = reg_base->SCR;

  if ((uint32_t)0 == (reg_value & SCR_DMA_ACT_MASK))
  {
    reg_value |= SCR_DMA_ACT_MASK;
    reg_base->SCR = reg_value;
  }
}

/*============================================================================*/
/**
* @brief          The function stops the DMA transfer.
* @details        The function stops the DMA transfer of the data frames to the
*                 memory.
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @return         void
* @api
* @isr
*
* @pre            - Setup the VIU reg virtual base address
*                 - Run viulite_dma_start
* @post
*
* @note
*
*/
void viulite_dma_stop(VIU_Registers_t *const reg_base)
{

  uint32_t  reg_value;
  /* Clear pending interrupts */
  reg_value = reg_base->INTR;
  reg_value |= INTR_STATUS_MASK;
  reg_base->INTR = reg_value;

  reg_value = reg_base->SCR;

  /* DMA already stopped ? */

  if ((uint32_t)0 != (reg_value & SCR_DMA_ACT_MASK))
  {
    reg_value &= ~SCR_DMA_ACT_MASK;
    reg_base->SCR = reg_value;

    /* Soft reset */
    reg_value |= SCR_SWRESET_MASK;
    reg_base->SCR = reg_value;

    reg_value &= ~SCR_SWRESET_MASK;
    reg_base->SCR = reg_value;
  }
}


/*============================================================================*/
/**
* @brief          The function reads the DMA STATUS flag and returns its value.
* @details        The function reads the DMA STATUS flag and returns its value.
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[out]     dmastatus   DMA status output
* @return         void
* @api
* @isr
*
* @pre            - Setup the VIU reg virtual base address
*
* @post
*
* @implements     viulite_dma_getstatus_Activity
*
* @note
*/
void viulite_dma_getstatus(const VIU_Registers_t *const reg_base,
                                 VIU_BOOL *const dmastatus)
{
  uint32_t  reg_value, t_param;
  reg_value = reg_base->SCR;
  t_param = (reg_value & SCR_DMA_ACT_MASK); 

  if (0U == t_param)
  {
    *dmastatus = VIU_OFF;
  }
  else
  {
    *dmastatus = VIU_ON;
  }
}

/*============================================================================*/
/**
* @brief          The function sets and resets the reset bit.
* @details        The function sets and resets the reset bit. To be used
*                  in case of errors.
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_sw_reset_Activity
*
* @note
*/
void viulite_sw_reset(VIU_Registers_t *const reg_base)
{
  uint32_t  reg_value;
  reg_value = reg_base->SCR;

  reg_value &= ~SCR_SWRESET_MASK;
  reg_base->SCR = reg_value;

  reg_value |= SCR_SWRESET_MASK;
  reg_base->SCR = reg_value;

  reg_value &= ~SCR_SWRESET_MASK;
  reg_base->SCR = reg_value;
}

/*============================================================================*/
/**
* @brief          The function enables the ECC functionality
* @details        The function enables the ECC functionality
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[in]      itu_errset  itu error ON or OFF
* @return         void
* @api
* @isr
*
* @pre
*
*            @post
*
* @implements     viulite_enable_ituerror_Activity
*
* @note
*/
void viulite_enable_ituerror(VIU_Registers_t *const reg_base,
                             const VIU_BOOL itu_errset)
{
  uint32_t  reg_value;
  reg_value = reg_base->SCR;
  reg_value &= ~SCR_ECCEN_MASK;

  if (VIU_ON == itu_errset)
  {
      reg_value |= SCR_ECCEN_MASK;
  }

  reg_base->SCR = reg_value;
}

/*============================================================================*/
/**
* @brief          The function reads the ERROR_CODE field and returns its value
* @details        The function reads the ERROR_CODE field and returns its value
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[out]     itu_errcode Itu error status
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_get_ituerror_Activity
*
* @note
*/
void viulite_get_ituerror(const VIU_Registers_t *const reg_base,
                          VIU_ITU_ERR *const itu_errcode)
{
  uint32_t  reg_value, t_param;

  reg_value = reg_base->INTR;
  t_param = (reg_value & INTR_ITUERR_MASK) >> INTR_ITUERR_OFFSET;

  switch (t_param)
  {
    case 0:
      *itu_errcode = NO_ERR;
      break;

    case 1:
      *itu_errcode = DMA_VERT_ERR;
      break;

    case 2:
      *itu_errcode = DMA_ACT_ERR;
      break;

    case 3:
      *itu_errcode = LINE_TOO_LONG_ERR;
      break;

    case 4:
      *itu_errcode = LINES_TOO_MANY_ERR;
      break;

    case 5:
      *itu_errcode = LINE_TOO_SHORT_ERR;
      break;

    case 6:
      *itu_errcode = LINES_TOO_LESS_ERR;
      break;

    case 7:
      *itu_errcode = FIFO_OVFLW_ERR;
      break;

    case 8:
      *itu_errcode = FIFO_UNFLW_ERR;
      break;

    case 9:
      *itu_errcode = ECC_1BIT_ERR;
      break;

    case 10:
      *itu_errcode = ECC_2BITS_ERR;
      break;

    case 11:
      *itu_errcode = SUBCH_DATA_ERR;
      break;

    case 12:
      *itu_errcode = SUBCH_REEN_ERR;
      break;

    default:
      /* nothing to be done */
      break;
  } /* t_param */
}

/*===========================================================================*/
/**
* @brief          The function enables one or more of the interrupt types
* @details        The function enables one or more of the interrupt types:
*                 FRAME_END, LINE_END, ERROR, DMA_END, VSTART, HSYNC, VSYNC,
*                 FIELD according to the input. 
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[in]      irq_mask    Input the mask for interrupt
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_enable_irqs_Activity
*
* @note
*/
void viulite_enable_irqs(VIU_Registers_t *const reg_base,
                         const uint8_t irq_mask)
{
  uint32_t  reg_value;
  reg_value = reg_base->INTR;

  reg_value &= ~INTR_ENABLE_MASK;
  reg_value |= (uint32_t)irq_mask;

  reg_base->INTR = reg_value;
}

/*===========================================================================*/
/**
* @brief          The function returns the status of the interrupt flags
* @details        The function returns the status of the interrupt flags:
*                 FRAME_END, LINE_END, ERROR, DMA_END, VSTART, HSYNC, VSYNC,
*                 FIELD, packed in a byte
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[out]     intr_status Interrupt status
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_get_irqstatus_Activity
*
* @note
*/
void viulite_get_irqstatus(const VIU_Registers_t *reg_base,
                           uint8_t *const intr_status)
{
  uint32_t  reg_value;
  reg_value = reg_base->INTR;

  *intr_status = (uint8_t)((reg_value & INTR_STATUS_MASK)
          >> INTR_STATUS_OFFSET);
}

/*===========================================================================*/
/**
* @brief          The function resets the status of the interrupt flags 
* @details        The function resets the status of the interrupt flags
*                 (FRAME_END, LINE_END, ERROR, DMA_END, VSTART, HSYNC, VSYNC,
*                 FIELD) in the input mask.  
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[in]      intr_status Input the mask for interrupt
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_reset_irqstatus_Activity
*
* @note
*/
void viulite_reset_irqstatus(VIU_Registers_t *const reg_base,
                             const uint8_t intr_status)
{
  uint32_t  reg_value;
  reg_value = reg_base->INTR;

  reg_value &= ~INTR_STATUS_MASK;
  reg_value |= ((uint32_t)intr_status << INTR_STATUS_OFFSET);

  reg_base->INTR = reg_value;
}

/*============================================================================*/
/**
* @brief          The function returns the status of VSYNC and HSYNC pulses.
* @details        The function returns the status of VSYNC and HSYNC pulses. 
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[out]     sync_status Viu sync signals status
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_get_syncsignals_Activity
*
* @note
*/
void viulite_get_syncsignals(const VIU_Registers_t *const reg_base,
                             VIU_SYNC_STATUS *const sync_status)
{
  uint32_t  reg_value;
  reg_value = reg_base->SCR;

  sync_status->vsync = (uint8_t)((reg_value & SCR_VSYNC_MASK)
          >> SCR_VSYNC_OFFSET);
  sync_status->hsync = (uint8_t)((reg_value & SCR_HSYNC_MASK)
          >> SCR_HSYNC_OFFSET);
}

/*============================================================================*/
/**
* @brief          The function returns the transferred field number.
* @details        The function returns the transferred field number extracted
*                 from ITU 656 data stream.
*
* @param[in]      reg_base    The base address of the VIU registers bank
* @param[out]     field_n     Field number
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_get_fieldnum_Activity
*
* @note
*/
void viulite_get_fieldnum(const VIU_Registers_t *const reg_base,
                          uint8_t *const field_n)
{
  uint32_t  reg_value;
  reg_value = reg_base->SCR;

  *field_n = (uint8_t)((reg_value & SCR_FIELDNO_MASK)
          >> SCR_FIELDNO_OFFSET);
}

/*============================================================================*/
/**
* @brief          The function returns the number of pixels detected per line.
* @details        The function returns the number of pixels detected per line
*                 and the number of lines detected per input frame (register
*                 VIULite_DINFSZ).
*
* @param[in]      reg_base         Destination buffer address
* @param[out]     frame_params     Viu frame size
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_get_framesize_Activity
*
* @note           A text note, much like @details but the text appears in a
*                 "note box" in the final document. Optional tag.
*/
void viulite_get_framesize(const VIU_Registers_t *const reg_base,
                           VIU_FRAME_SIZE *const frame_params)
{
  uint32_t  reg_value;
  reg_value = reg_base->DINVSZ;

  frame_params->nmb_pixells = (uint16_t)(reg_value & VIU_LOW16_MASK);
  frame_params->nmb_lines = (uint16_t)((reg_value & VIU_HIGH16_MASK)
          >> 16U);
}

/*============================================================================*/
/**
* @brief          The function sets the origin and the size of the clipped area.
* @details        The function sets the origin and the size of the clipped area
*                 (x_origin, y_origin, x_size, y_size).
*
* @param[in]      reg_base         Destination buffer address
* @param[in]      image_params     Viu image parameter
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_set_clippingdata_Activity
*
* @note
*/
void viulite_set_clippingdata(VIU_Registers_t *const reg_base,
                              const VIU_IMAGE_PARAMS *const image_params)
{
  uint32_t  reg_value;

  reg_value = ((uint32_t)(image_params->x_origin) & VIU_LOW16_MASK) |
          ((uint32_t)(image_params->y_origin) << 16);
  reg_base->ACTORG = reg_value;

  reg_value = ((uint32_t)(image_params->x_size) & VIU_LOW16_MASK) |
          ((uint32_t)(image_params->y_size) << 16);
  reg_base->ACTSIZE = reg_value;
}

/*============================================================================*/
/**
* @brief          The function gets the origin and the size of the clipped area
* @details        The function gets the origin and the size of the clipped area
*                 (x_origin, y_origin, x_size, y_size).
*
* @param[in]      reg_base         Destination buffer address
* @param[out]     image_params     Viu image parameter
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @implements     viulite_get_clippingdata_Activity
*
* @note
*/
void viulite_get_clippingdata(const VIU_Registers_t *const reg_base,
                              VIU_IMAGE_PARAMS *const image_params)
{
  uint32_t  reg_value;

  reg_value = reg_base->ACTORG;
  image_params->x_origin = (uint16_t)(reg_value & VIU_LOW16_MASK);
  image_params->y_origin = (uint16_t)((reg_value & VIU_HIGH16_MASK) >> 16U);

  reg_value = reg_base->ACTSIZE;
  image_params->x_size = (uint16_t)(reg_value & VIU_LOW16_MASK);
  image_params->y_size = (uint16_t)((reg_value & VIU_HIGH16_MASK) >> 16U);
}

#ifndef __LINUX__
/*============================================================================*/
/**
* @brief          This function is the service routine for the viu irq
* @details        This function is the service routine for the viu irq
*
* @param[in]      viu_address  The base address of the VIU which raised the irq
* @param[in]
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
static void VIU_Isr(VIU_Registers_t *const viu_address)
{
  uint32_t  int_status, reg_value;

  int_status = viu_address->INTR;

  if ((uint32_t)0 != (int_status & (INTR_VSYNC_MASK | INTR_VSYNC_EN)))
  {
      /* start a new frame transfer */
      reg_value = viu_address->SCR;
      reg_value |= SCR_DMA_ACT_MASK;
      viu_address->SCR = reg_value;

      int_status |= INTR_VSYNC_MASK;
  }

  if ((uint32_t)0 != (int_status & 
                      (INTR_DMA_END_MASK | INTR_DMA_END_EN)))
  {
    int_status |= INTR_DMA_END_MASK;
  }

  if ((uint32_t)0 != (int_status & (INTR_ERR_MASK | INTR_ERR_EN)))
  {
      int_status |= INTR_ERR_MASK;
  }

  if ((uint32_t)0 != (int_status & (INTR_HSYNC_MASK | INTR_HSYNC_EN)))
  {
      int_status |= INTR_HSYNC_MASK;
  }

  if ((uint32_t)0 != (int_status & 
                      (INTR_LINE_END_MASK | INTR_LINE_END_EN)))
  {
      int_status |= INTR_LINE_END_MASK;
  }

  if ((uint32_t)0 != (int_status & 
                     (INTR_FRAME_END_MASK | INTR_FRAME_END_EN)))
  {
      int_status |= INTR_FRAME_END_MASK;
  }

  if ((uint32_t)0 != (int_status & (INTR_FIELD_MASK | INTR_FIELD_EN)))
  {
      int_status |= INTR_FIELD_MASK;
  }

  if ((uint32_t)0 != (int_status & (INTR_VSTART_MASK | INTR_VSTART_EN)))
  {
      int_status |= INTR_VSTART_MASK;
  }

  /* clear all pending irqs */
  viu_address->INTR = int_status;
}

/*============================================================================*/
/**
* @brief          This function is the interrupt service routine for VIU0
* @details        This function is the interrupt service routine for VIU0
*
* @param[in]      void
* @param[out]
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
void VIU0_Isr(void)
{
  VIU_Isr(VIU0_BASEADDR);
}/* VIU0_Isr() */

/*============================================================================*/
/**
* @brief          This function is the interrupt service routine for VIU1
* @details        This function is the interrupt service routine for VIU1
*
* @param[in]      void
* @param[out]
* @return         void
* @api
* @isr
*
* @pre
*
* @post
*
* @note
*/
void VIU1_Isr(void)
{
  VIU_Isr(VIU1_BASEADDR);
}/* VIU1_Isr() */
#endif
/* ifndef _LINUX_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/

/** @} */