/*
 * Copyright (c) 2016 Freescale Semiconductor
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

/*==============================================================================
Revision History:
                              Modification     Tracking
Author (core ID)              Date D/M/Y       Number     Description of Changes
---------------------------   ----------    ------------  ----------------------
Roman Kubica                  14/10/2016                   Initial Version
---------------------------   ----------    ------------  ----------------------
==============================================================================*/

#ifdef __cplusplus
extern "C"{
#endif

/*==============================================================================
*                                        INCLUDE FILES
==============================================================================*/
/**
* @file          h264enc_func.c
* @implements
*/

#include <linux/kernel.h>	// We're doing kernel work
#include <linux/module.h>	// Specifically, a module
#include <linux/interrupt.h>    // For interrupts
#include <asm/siginfo.h>	// siginfo
#include <linux/sched.h>	// send_sig_info
#include <linux/wait.h>
#include <linux/slab.h>	// for malloc and free
#include <linux/delay.h>	// for msleep
#include <asm/io.h>		// for ioremap
#include "h264enc.h"
#include "h264enc_func.h"
#include "vdb_log.h"

/*==============================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==============================================================================*/

/*==============================================================================
*                                       LOCAL MACROS
==============================================================================*/

#define BR_TOO_HIGH_INT_EN_V 1
#define OUT_CBUF_OVF_INT_EN_V 1
#define MBRW_FF_OVF_EN_V 1
#define ENC_OOS_INT_EN_V 1
#define BS_NEW_ROW_INT_EN_V  1
#define FRAME_DONE_INT_EN_V 1

/*==============================================================================
*                            LOCAL CONSTANTS
==============================================================================*/

/*==============================================================================
*                            LOCAL VARIABLES
==============================================================================*/

static uint8_t gsRowDone = 0; /* status of Macroblock encoding */

static uint32_t gPixLinesReady = 0; /* number of lines ready to be fetched */

static uint32_t gIrq_counter = 0;

static uint32_t gFrame_counter = 0;

static spinlock_t gDataLock;

/*==============================================================================
*                            GLOBAL CONSTANTS
==============================================================================*/


/*==============================================================================
*                            GLOBAL VARIABLES
==============================================================================*/


static H264ENC_Regs_t *gpH264EncRegs = NULL; /* map Encoder registers */

/*==============================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==============================================================================*/

static irqreturn_t H264ENC_InpFrameDoneIrqHandler(void);
static irqreturn_t H264ENC_BStrNewRowIrqHandler(void);
static irqreturn_t H264ENC_EncOutOfSyncIrqHandler(void);
static irqreturn_t H264ENC_MRowReadIrqHandler(void);
static irqreturn_t H264ENC_MRowFetchOvfIrqHandler(void);
static irqreturn_t H264ENC_BrateTooHighIrqHandler(void);
static irqreturn_t H264ENC_OutBufAlmostFullIrqHandler(void);
static irqreturn_t H264ENC_OutBufFullIrqHandler(void);

/*============================================================================*/
// STRUCT operations
/*============================================================================*/

extern H264Enc_DrvInfo_t gH264EncDrvInfo;

/*==============================================================================
*                                LOCAL FUNCTIONS
==============================================================================*/

/**
* @brief         Maps H264 Encoder
* @details       Maps H264 Encoder
*
* @return        int32_t
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*/
int32_t H264EncMap(void)
{
  int32_t lRet;

  gH264EncDrvInfo.mpH264encCBregs = 
    (H264ENC_Regs_t*)ioremap(H264ENC_BASE, H264ENC_SIZE);
  gpH264EncRegs = gH264EncDrvInfo.mpH264encCBregs; /* map CTRL BLK registers */
 
  if(gH264EncDrvInfo.mpH264encCBregs == NULL)  /* if(gpH264EncRegs == NULL) */
  {
    VDB_LOG_ERROR("Failed to map H264ENC registers.\n");
    lRet = H264ENC_DRV_FAILURE;
  } /* if H264Enc registers not mapped */
  else
  {
    VDB_LOG_NOTE("H264 Encoder registers mapped OK.\n");
    lRet = H264ENC_DRV_SUCCESS;
  } /* else from if registers not mapped */

  return lRet;
} /* H264EncMap() */



/*============================================================================*/
/**
* @brief         Initialize Encoder
* @details       Initialize Encoder
*
* @return        int32_t
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*/

int32_t H264EncDrvDataInit(void)
{
  int32_t lRet = H264ENC_DRV_SUCCESS;
  uint32_t reg_temp;
  uint32_t val_temp;
  
  if(H264EncMap() != H264ENC_DRV_SUCCESS)   /* map H264Enc related blocks */
  {
    lRet = H264ENC_DRV_FAILURE;
  } /* if H264Enc mapping failed */
  else
  {
    spin_lock_init(&gDataLock);
    gPixLinesReady = 0;
  
    reg_temp = reg_read32(&reg_base->MCR); /* put H264Enc regs to default */
    val_temp = (((uint32_t)1 << H264ENC_MCR_SW_RESET_SHIFT) & 
               H264ENC_MCR_SW_RESET_MASK); 
    reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;   
    reg_temp |= val_temp;                     
    reg_write32(&reg_base->MCR,reg_temp); 
            
    msleep(100);
      
    reg_temp  = reg_read32(&reg_base->MCR);
    val_temp  = (((uint32_t)0u << H264ENC_MCR_SW_RESET_SHIFT) & 
                H264ENC_MCR_SW_RESET_MASK);     
    reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;   
    reg_temp |= val_temp; 
    reg_write32(&reg_base->MCR,reg_temp);  
  } /* else from if H264Enc mapping failed */

  return lRet;
} /* H264EncDrvDataInit() */

/*============================================================================*/

/**
* @brief         Unmaps H264 Encoder
* @details       UnMaps H264 Encoder
*
* @return        int32_t
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*/

void H264EncUnmap(void)
{
  iounmap(gpH264EncRegs);

  gH264EncDrvInfo.mpH264encCBregs = NULL;

} /* H264EncUnmap() */


/*============================================================================*/

/**
* @brief         Free H264 Encoder irqs
* @details       Free H264 Encoder irqs
*
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*
*/

void H264EncIrqsFree(void)
{
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_FRAME_DONE_INT, NULL);
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_NEW_ROW_INT, NULL);
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_CB_ALARM_INT, NULL);
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_LINE_INT, NULL);
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_LCOUNT_ALARM_INT, NULL);
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_VI_OOS_INT, NULL);
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_MBRW_DONE_INT, NULL); 
  free_irq(gH264EncDrvInfo.mIrqNums.mH264ENC_MBRW_FF_OVF_INT, NULL);
} /* H264EncIrqsFree() */

/*============================================================================*/

/**
* @brief        Close Encoder irqs
* @details      Free H264 Encoder irqs, unmaps H264Encder registers
*
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*/

void H264ENC_Close(void)
{
  H264EncIrqsFree();
  H264EncUnmap();
} /* H264ENC_Close() */

/*============================================================================*/

/**
* @brief        H264 encoder irqs handler.
* @details      Handles H264  encoder irqs
*
* @param[in]    aIrq   irq number
* @param[in]    apDev_id devide ID
*
* @return       irqreturn_t
*
* @api
* @isr
*
*
* @pre
* @post
*
* @note
*/

irqreturn_t H264ENC_S32v234IrqHandler(int32_t aIrq, void *apDev_id)
{    
  uint32_t reg_temp;
  
  irqreturn_t lRet = IRQ_HANDLED;
  
  reg_temp = reg_read32(&reg_base->ISR); 
  reg_temp &= ISR_HANDLER_MASK;
  
  while((reg_temp) > 0x0U)
  {  
    reg_temp = reg_read32(&reg_base->ISR); 
    reg_temp &= H264ENC_ISR_VI_FRAME_DONE_INT_MASK;
    
    if((reg_temp)== H264ENC_ISR_VI_FRAME_DONE_INT_MASK) 
    {  
      gFrame_counter++;
      gIrq_counter++;
      lRet = H264ENC_InpFrameDoneIrqHandler();
    } // if Input frame done    
    
    reg_temp = reg_read32(&reg_base->ISR);
    reg_temp &= H264ENC_ISR_BS_NEW_ROW_INT_MASK;
    
    if((reg_temp) == H264ENC_ISR_BS_NEW_ROW_INT_MASK)
    {
      gIrq_counter++;
      lRet = H264ENC_BStrNewRowIrqHandler();
    } // if bistream new row   
      
    reg_temp = reg_read32(&reg_base->ISR);   
    reg_temp &= H264ENC_ISR_ENC_OOS_INT_MASK;
    
    if((reg_temp) == H264ENC_ISR_ENC_OOS_INT_MASK)
    {
      gIrq_counter++;
      lRet = H264ENC_EncOutOfSyncIrqHandler();
    } // if encoder out of sync

    reg_temp = reg_read32(&reg_base->ISR);  
    reg_temp &= H264ENC_ISR_MBRW_DONE_INT_MASK;
    
    if((reg_temp) == H264ENC_ISR_MBRW_DONE_INT_MASK)           
    {     
      gIrq_counter++;
      lRet = H264ENC_MRowReadIrqHandler();
    } // if Macroblock read from SRAM
 
    reg_temp = reg_read32(&reg_base->ISR);  
    reg_temp &= H264ENC_ISR_BR_TOO_HIGH_INT_MASK;
    
    if((reg_temp) == H264ENC_ISR_BR_TOO_HIGH_INT_MASK)
    {
      gIrq_counter++;
      lRet = H264ENC_BrateTooHighIrqHandler();
    } // if bit rate too high
   
    reg_temp = reg_read32(&reg_base->ISR);  
    reg_temp &= H264ENC_ISR_FIFO_AFULL_INT_MASK;
    
    if((reg_temp) == H264ENC_ISR_FIFO_AFULL_INT_MASK)
    {
      gIrq_counter++;
      lRet = H264ENC_OutBufAlmostFullIrqHandler();
    } // if bit rate too high

    reg_temp = reg_read32(&reg_base->ISR);
    reg_temp &= H264ENC_ISR_BS_FIFO_FULL_INT_MASK;
    
    if((reg_temp) == H264ENC_ISR_BS_FIFO_FULL_INT_MASK)
    {
      gIrq_counter++;
      lRet = H264ENC_OutBufFullIrqHandler();
    } // if bit rate too high//   
  } // while there is some ISR   

  return lRet;
} //H264ENC_S32v234IrqHandler()

/*============================================================================*/

/**
* @brief        Handler for MBRW_FF_OVF_INT iterrupt
* @details      Added separately, because flag stays 1 till FIFO is empty
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/

irqreturn_t H264ENC_FF_OVF_IrqHandler(int32_t aIrq, void *apDev_id)
{    
  uint32_t reg_temp;
  
  irqreturn_t lRet = IRQ_HANDLED;
  
    reg_temp = reg_read32(&reg_base->ISR);   
    reg_temp &= H264ENC_ISR_MBRW_FF_OVF_INT_MASK;
    
    if((reg_temp) == H264ENC_ISR_MBRW_FF_OVF_INT_MASK)
    {
      gIrq_counter++;
      lRet = H264ENC_MRowFetchOvfIrqHandler();
    } // if Macroblock fetch fifo overflow

  return lRet;
 
}
/*============================================================================*/

/**
* @brief        Framedone IRQ handler
* @details      Framedone IRQ handler
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/

static irqreturn_t H264ENC_InpFrameDoneIrqHandler(void)
{
  irqreturn_t lRet = IRQ_HANDLED;
  
  reg_write32(&reg_base->ISR,H264ENC_ISR_VI_FRAME_DONE_INT_MASK); 
    
  return lRet;
  
}

/*============================================================================*/
/**
* @brief        Bitstream New Row IRQ handler
* @details      Bitstream New Row IRQ handler
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/

static irqreturn_t H264ENC_BStrNewRowIrqHandler(void)
{  
  irqreturn_t lRet = IRQ_HANDLED;
    
  reg_write32(&reg_base->ISR,H264ENC_ISR_BS_NEW_ROW_INT_MASK);   
  
  gsRowDone = 1;
  
  return lRet;
}

/*============================================================================*/
/**
* @brief        Encoder Out Of Sync IRQ handler
* @details      Set when Encoder and Wrapper are out of Sync.
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/


static irqreturn_t H264ENC_EncOutOfSyncIrqHandler(void)
{  
  irqreturn_t lRet = IRQ_HANDLED;

  uint32_t reg_temp  = reg_read32(&reg_base->MCR);
  uint32_t val_temp  = (((uint32_t)1u << H264ENC_MCR_SW_RESET_SHIFT) & 
                      H264ENC_MCR_SW_RESET_MASK); 
  reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;   
  reg_temp |= val_temp;                
  reg_write32(&reg_base->MCR,reg_temp);    
      
  reg_temp  = reg_read32(&reg_base->MCR);
  val_temp  = (((uint32_t)0u << H264ENC_MCR_SW_RESET_SHIFT) & 
              H264ENC_MCR_SW_RESET_MASK);      
  reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->MCR,reg_temp);

  reg_write32(&reg_base->ISR,H264ENC_ISR_ENC_OOS_INT_MASK); 
  
  return lRet;
}

/*============================================================================*/
/**
* @brief        Encoder macroblock has been read 
* @details      Set when Encoder macroblock has been read.
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/


static irqreturn_t H264ENC_MRowReadIrqHandler(void)       
{  
  irqreturn_t lRet = IRQ_HANDLED;
   
  unsigned long lFlag;
  spin_lock_irqsave((&gDataLock), (lFlag));
  
  gPixLinesReady -= (gPixLinesReady > 16u) ? 16u : gPixLinesReady;
  
  if(gPixLinesReady > 0u)
  {     
    reg_write32(&reg_base->FETCH_MBRW,(uint32_t)1); 
      
  }
  spin_unlock_irqrestore((&gDataLock), (lFlag));
  
  reg_write32(&reg_base->ISR,H264ENC_ISR_MBRW_DONE_INT_MASK); 

  return lRet;
}

/*============================================================================*/
/**
* @brief        Macroblock Row Fifo Overflow Interrupt handler
* @details      Set when more than two fetch macorlockrow triggers comes.
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/

static irqreturn_t H264ENC_MRowFetchOvfIrqHandler(void)
{   
  irqreturn_t lRet = IRQ_HANDLED;
  
  reg_write32(&reg_base->ISR,H264ENC_ISR_MBRW_FF_OVF_INT_MASK); 
  
  return lRet;
}


/*============================================================================*/
/**
* @brief        Bitrate too high interrupt handler
* @details      Set when fallback QP buffer overflows.
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/

static irqreturn_t H264ENC_BrateTooHighIrqHandler(void)
{    
  irqreturn_t lRet = IRQ_HANDLED;
 
  uint32_t reg_temp  = reg_read32(&reg_base->MCR);
  uint32_t val_temp  = ((uint32_t)((uint32_t)1u << H264ENC_MCR_SW_RESET_SHIFT)
                        & H264ENC_MCR_SW_RESET_MASK); 
  reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;   
  reg_temp |= val_temp;  
  reg_write32(&reg_base->MCR,reg_temp);  
  
  reg_write32(&reg_base->ISR,H264ENC_ISR_BR_TOO_HIGH_INT_MASK);  
  
  return lRet;  
}

/*============================================================================*/
/**
* @brief        Bistream FIFO almost full interupt handler.
* @details      Bistream FIFO almost full interupt handler.
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/

static irqreturn_t H264ENC_OutBufAlmostFullIrqHandler(void)
{  
  irqreturn_t lRet = IRQ_HANDLED;  

  reg_write32(&reg_base->ISR,H264ENC_ISR_FIFO_AFULL_INT_MASK); 
  
  return lRet;  
}

/*============================================================================*/
/**
* @brief        Bistream FIFO full interupt handler.
* @details      Bistream FIFO full interupt handler.
*
* @api
* @isr
*
* @return      irqreturn_t
*
* @pre
* @post
*
* @note
*/

static irqreturn_t H264ENC_OutBufFullIrqHandler(void)
{  
  irqreturn_t lRet = IRQ_HANDLED;

  uint32_t reg_temp  = reg_read32(&reg_base->MCR);
  uint32_t val_temp  = ((uint32_t)((uint32_t)1u << H264ENC_MCR_SW_RESET_SHIFT)
                       & H264ENC_MCR_SW_RESET_MASK);   
  reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;   
  reg_temp |= val_temp;             
  reg_write32(&reg_base->MCR,reg_temp);
  
  reg_write32(&reg_base->ISR,H264ENC_ISR_FIFO_AFULL_INT_MASK);  

  return lRet;  
}

/*============================================================================*/
/**
* @brief        H264ENC Configuration.
* @details      Set encoding parameters.
*
* @param apcConfig structure
*
* @api
* @isr
*
*
* @pre
* @post
*
* @implements     H264ENC_Config_Activity
*
* @note
*/

void H264ENC_Config(const H264ENC_Config_t *apcConfig)
{ 
  uint32_t reg_temp;
  uint32_t val_temp;
  
  reg_temp  = reg_read32(&reg_base->MCR);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mBitWidth << 
              H264ENC_MCR_VI_BIT_WIDTH_SHIFT) & H264ENC_MCR_VI_BIT_WIDTH_MASK); 
  reg_temp &= H264ENC_MCR_VI_BIT_WIDTH_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->MCR,reg_temp);   
    
  reg_temp  = reg_read32(&reg_base->MCR);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mColorFormat << 
              H264ENC_MCR_VI_COLOR_FMT_SHIFT) & H264ENC_MCR_VI_COLOR_FMT_MASK); 
  reg_temp &= H264ENC_MCR_VI_COLOR_FMT_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->MCR,reg_temp);  
  
  reg_temp  = reg_read32(&reg_base->MCR);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mFetchTrig << 
              H264ENC_MCR_FETCH_TRIG_SHIFT) & H264ENC_MCR_FETCH_TRIG_MASK); 
  reg_temp &= H264ENC_MCR_FETCH_TRIG_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->MCR,reg_temp);   
  
  reg_temp  = reg_read32(&reg_base->MCR);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mDataModeChroma << 
              H264ENC_MCR_DATA_MODE_CHR_SHIFT) & 
              H264ENC_MCR_DATA_MODE_CHR_MASK); 
  reg_temp &= H264ENC_MCR_DATA_MODE_CHR_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->MCR,reg_temp);  
    
  reg_temp  = reg_read32(&reg_base->VCR);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mFrameWidth << 
              (H264ENC_VCR_FR_WIDTH_SHIFT - 1)) & H264ENC_VCR_FR_WIDTH_MASK); 
              /* -1 because of division by 2 */
  reg_temp &= H264ENC_VCR_FR_WIDTH_MASK_NEG;   
  reg_temp |= val_temp;
  reg_write32(&reg_base->VCR,reg_temp);   
  
  reg_temp  = reg_read32(&reg_base->VCR);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mFrameHeight << 
              (H264ENC_VCR_FR_HEIGHT_SHIFT - 1)) & H264ENC_VCR_FR_HEIGHT_MASK); 
  reg_temp &= H264ENC_VCR_FR_HEIGHT_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->VCR,reg_temp); 
       
  reg_write32(&reg_base->ULLVC_FRAME_RATE,apcConfig->mFrameRate);  
  reg_write32(&reg_base->ULLVC_QP_INIT,apcConfig->mQpInit); 
  reg_write32(&reg_base->ULLVC_QP_FALLBACK_LIMIT,apcConfig->mQpFallback); 
    
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_RANGE);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpMin << 
              ULLVC_QP_RANGE_MIN_SHIFT) & ULLVC_QP_RANGE_MIN_MASK); 
  reg_temp &= ULLVC_QP_RANGE_MIN_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_RANGE,reg_temp);  
  
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_RANGE);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpMax << 
              ULLVC_QP_RANGE_MAX_SHIFT) & ULLVC_QP_RANGE_MAX_MASK); 
  reg_temp &= ULLVC_QP_RANGE_MAX_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_RANGE,reg_temp);    
   
  reg_write32(&reg_base->ULLVC_BITS_PER_MB_ROW,apcConfig->mBitsPerMbrw);  
  reg_write32(&reg_base->ULLVC_QP_INC_THLD[0],apcConfig->mQpIncThld0); 
  reg_write32(&reg_base->ULLVC_QP_INC_THLD[1],apcConfig->mQpIncThld1); 
  reg_write32(&reg_base->ULLVC_QP_INC_THLD[2],apcConfig->mQpIncThld2); 
  
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_INC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncOffset << 
              ULLVC_QP_INC_OFFSET_SHIFT) & ULLVC_QP_INC_OFFSET_MASK); 
  reg_temp &= ULLVC_QP_INC_OFFSET_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_INC,reg_temp); 
    
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_INC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncDiff0 << 
              ULLVC_QP_INC_DIFF_0_SHIFT) & ULLVC_QP_INC_DIFF_0_MASK); 
  reg_temp &= ULLVC_QP_INC_DIFF_0_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_INC,reg_temp); 
  
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_INC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncDiff1 << 
              ULLVC_QP_INC_DIFF_1_SHIFT) & ULLVC_QP_INC_DIFF_1_MASK); 
  reg_temp &= ULLVC_QP_INC_DIFF_1_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_INC,reg_temp); 
  
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_INC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncDiff2 << 
              ULLVC_QP_INC_DIFF_2_SHIFT) & ULLVC_QP_INC_DIFF_2_MASK); 
  reg_temp &= ULLVC_QP_INC_DIFF_2_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_INC,reg_temp); 
    
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_DEC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncOffset << 
              ULLVC_QP_DEC_OFFSET_SHIFT) & ULLVC_QP_DEC_OFFSET_MASK); 
  reg_temp &= ULLVC_QP_DEC_OFFSET_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_DEC,reg_temp); 
    
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_DEC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncDiff0 << 
              ULLVC_QP_DEC_DIFF_0_SHIFT) & ULLVC_QP_DEC_DIFF_0_MASK); 
  reg_temp &= ULLVC_QP_DEC_DIFF_0_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_DEC,reg_temp); 
  
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_DEC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncDiff1 << 
              ULLVC_QP_DEC_DIFF_1_SHIFT) & ULLVC_QP_DEC_DIFF_1_MASK); 
  reg_temp &= ULLVC_QP_DEC_DIFF_1_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_DEC,reg_temp); 
    
  reg_temp  = reg_read32(&reg_base->ULLVC_QP_DEC);
  val_temp  = ((uint32_t)((uint32_t)apcConfig->mQpIncDiff2 << 
              ULLVC_QP_DEC_DIFF_2_SHIFT) & ULLVC_QP_DEC_DIFF_2_MASK); 
  reg_temp &= ULLVC_QP_DEC_DIFF_2_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->ULLVC_QP_DEC,reg_temp); 
    
  reg_write32(&reg_base->ULLVC_QP_DEC_THLD[0],apcConfig->mQpDecThld0); 
  reg_write32(&reg_base->ULLVC_QP_DEC_THLD[1],apcConfig->mQpDecThld1); 
  reg_write32(&reg_base->ULLVC_QP_DEC_THLD[2],apcConfig->mQpDecThld2); 
  
  reg_write32(&reg_base->ULLVC_WAIT_FRAMES,apcConfig->mWaitFrames); 
  reg_write32(&reg_base->ULLVC_DISABLE_DBF,apcConfig->mDisableDbf); 
};

/*============================================================================*/
/**
* @brief        H264ENC Buffers Configuration.
* @details      Set buffers parameters, address, sizes.
*
* @param apcConfig
*
* @api
* @isr
*
*
* @pre
* @post
*
* @implements     H264ENC_BufferSet_Activity
*
* @note
*/

void H264ENC_BuffersSet(const H264ENC_Buffers_t *apcBuffers)
{  
  uint32_t reg_temp;   
  uint32_t val_temp;
      
  reg_write32(&reg_base->VI_CBUF_Y_ADDR,apcBuffers->mY.mPhysAddr); 
  reg_write32(&reg_base->VI_CBUF_CB_ADDR,apcBuffers->mU.mPhysAddr); 
  reg_write32(&reg_base->VI_CBUF_CR_ADDR,apcBuffers->mV.mPhysAddr); 
     
  reg_temp  = reg_read32(&reg_base->VI_NRLINES);
  val_temp  = ((uint32_t)((uint32_t)apcBuffers->mLineNum << (VI_NRLINES_SHIFT))
              & VI_NRLINES_MASK); 
  reg_temp &= VI_NRLINES_MASK_NEG;   
  reg_temp |= val_temp;           
  reg_write32(&reg_base->VI_NRLINES,reg_temp);  
  
  /* Output */
  reg_write32(&reg_base->OUT_CBUF_START_ADDR,apcBuffers->mBitstream.mPhysAddr); 
  reg_write32(&reg_base->OUT_CBUF_END_ADDR,apcBuffers->mBitstream.mPhysAddr + \
    apcBuffers->mBitstream.mSize - 8u);
  reg_write32(&reg_base->OUT_CBUF_ALARM_ADDR, apcBuffers->mBitstream.mPhysAddr 
    & 0x3fffffu);    
  
  if(apcBuffers->mBankStride < 8u) /* Rateflow control */
  {
    reg_temp  = reg_read32(&reg_base->RATE_FLOW_CTRL);
    val_temp  = ((uint32_t)((uint32_t)1 << RATE_FLOW_CTRL_ENABLE_SHIFT) &
                RATE_FLOW_CTRL_ENABLE_MASK); 
    reg_temp &= RATE_FLOW_CTRL_ENABLE_MASK_NEG;   
    reg_temp |= val_temp;           
    reg_write32(&reg_base->RATE_FLOW_CTRL,reg_temp); 
    
    reg_temp  = reg_read32(&reg_base->RATE_FLOW_CTRL);
    val_temp  = ((uint32_t)((uint32_t)apcBuffers->mBankStride << 
                RATE_FLOW_CB_STRIDE_SHIFT) & 
                RATE_FLOW_CB_STRIDE_MASK); 
    reg_temp &= RATE_FLOW_CB_STRIDE_MASK_NEG;   
    reg_temp |= val_temp;           
    reg_write32(&reg_base->RATE_FLOW_CTRL,reg_temp); 
    
  } /* if rate flow control on */
  else
  {
    reg_temp  = reg_read32(&reg_base->RATE_FLOW_CTRL);
    val_temp  = ((uint32_t)((uint32_t)0u << RATE_FLOW_CTRL_ENABLE_SHIFT) & 
                RATE_FLOW_CTRL_ENABLE_MASK); 
    reg_temp &= RATE_FLOW_CTRL_ENABLE_MASK_NEG;   
    reg_temp |= val_temp;           
    reg_write32(&reg_base->RATE_FLOW_CTRL,reg_temp);
  }   
      
};

/*============================================================================*/
/**
* @brief        Provides MBRW encoding status.
* @details      Provides MBRW encoding status.
*
* @api
* @isr
*
*
* @return uint8_t
*
* @pre
* @post
*
* @implements H264ENC_RowDoneStatusGet_Activity
*
* @note
*/

uint8_t H264ENC_RowDoneStatusGet(void)
{  
  uint8_t lRet = 0;

  if(gsRowDone == 1u)
  {
    lRet = 1;
    gsRowDone = 0;
  }
  return lRet;
}

/*============================================================================*/
/**
* @brief        Provides encoding status.
* @details      Provides encoding status.
*
* @api
* @isr
*
*
* @return uint8_t
*
* @pre
* @post
*
* @implements H264ENC_StatusGet_activity
*
* @note
*/

void H264ENC_StatusGet(H264ENC_Status_t *apcStatus)
{  
  unsigned long lFlag;
  uint32_t reg_temp  = reg_read32(&reg_base->OUT_CBUF_CURR_ADDR);
  
  reg_temp &= OUT_CBUF_CURR_ADDR_MASK;
  
  apcStatus->mOutCurrentAddress = reg_temp;
  apcStatus->mOutAlarmAddress =  reg_read32(&reg_base->OUT_CBUF_ALARM_ADDR);
  apcStatus->mLastFrmEndAddress = reg_read32(&reg_base->OUT_CBUF_VEND_ADDR) 
    + 8u;
  
  apcStatus->mInterruptStatus      = reg_read32(&reg_base->ISR);
  apcStatus->mOutLineCounter       = reg_read32(&reg_base->LINE_CNTR_STAT);
  
 
  spin_lock_irqsave((&gDataLock), (lFlag));
  
  apcStatus->mInLineCounter        = gPixLinesReady;
  spin_unlock_irqrestore((&gDataLock), (lFlag));
  
  apcStatus->mFrameCounter         = gFrame_counter;
  apcStatus->mIrqCounter           = gIrq_counter;
}

/*============================================================================*/
/**
* @brief        Fetches MBRW for encoding.
* @details      Fetches MBRW for encoding and starts encoding if MBRW is ready.
*
* @api
* @isr
*
*
*
* @pre
* @post
*
* @implements      H264ENC_LinesFetch_Activity
*
* @note
*/

void H264ENC_LinesFetch(uint32_t aMoreLines)
{  
  
  unsigned long lFlag;
  spin_lock_irqsave((&gDataLock), (lFlag));
  
  if(gPixLinesReady == 0u) /* invoke fetch if 0 before */
  {   
    reg_write32(&reg_base->FETCH_MBRW,(uint32_t)1);  
  }

  gPixLinesReady += aMoreLines;

  spin_unlock_irqrestore((&gDataLock), (lFlag));
  
}

/*============================================================================*/
/**
* @brief        H264 Encoder Bitstream alarm shift.
* @details      H264 Encoder Bitstream alarm shift.
*
* @param   aAlarmShift
*
* @api
* @isr
*
*
*
* @pre
* @post
*
*
* @note
*/

void H264ENC_BitstreamAlarmShift(uint32_t aAlarmShift)
{    
  uint32_t lRegAlarm = reg_read32(&reg_base->OUT_CBUF_ALARM_ADDR);
  uint32_t reg_temp  = reg_read32(&reg_base->OUT_CBUF_END_ADDR);
  uint32_t lRegEnd   = reg_temp + 8u;
  
  uint32_t lRegStart = reg_read32(&reg_base->OUT_CBUF_START_ADDR);
  uint32_t lWrap     = lRegAlarm & 0x400000u;
  
  lRegAlarm = (lRegAlarm & 0x3fffffu) + aAlarmShift;
  
  if(lRegAlarm >= lRegEnd)
  {
    lRegAlarm  = lRegStart + lRegAlarm - lRegEnd;
    lRegAlarm += lWrap + 0x400000u;
  } // if wrapp
  else
  {
    lRegAlarm += lWrap;
  } // else from if wrapp
  
  /* reprogram alarm address - should be done after data were fetched */
  reg_write32(&reg_base->OUT_CBUF_ALARM_ADDR,lRegAlarm);   
  
}

/*============================================================================*/
/**
* @brief        H264 Encoder Stop function.
* @details      Reset current encoder operation
*
*
* @api
* @isr
*
*
*
* @pre
* @post
*
* @implements H264ENC_Stop_Activity
*
* @note
*/

void H264ENC_Stop(void)
{ 
  uint32_t reg_temp = reg_read32(&reg_base->MCR);
  uint32_t val_temp = ((uint32_t)((uint32_t)1 << H264ENC_MCR_SW_RESET_SHIFT) & 
                      H264ENC_MCR_SW_RESET_MASK);  
  reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;  
  reg_temp |= val_temp;                
  reg_write32(&reg_base->MCR,reg_temp); 
  
  reg_write32(&reg_base->ISR, 0xFFFFFFFFU); 
  
}

/*============================================================================*/
/**
* @brief        H264 Encoder ResetVars functin.
* @details      H264 Encoder ResetVars functin. Resets internal variables.
*
*
* @api
* @isr
*
*
*
* @pre
* @post
*
* @implements H264ENC_Reset_Vars_Activity
*
* @note
*/

void H264ENC_ResetVars(void)
{
  uint32_t reg_temp;
  uint32_t val_temp;
  unsigned long lFlag;
  
  gsRowDone = 0;
  
  spin_lock_irqsave((&gDataLock), (lFlag));
  gPixLinesReady = 0; // number of lines ready to be fetched
  spin_unlock_irqrestore((&gDataLock), (lFlag));
  
  gIrq_counter = 0;
  gFrame_counter = 0;

  reg_temp = reg_read32(&reg_base->MCR);
  val_temp = ((uint32_t)((uint32_t)1 << H264ENC_MCR_SW_RESET_SHIFT) & 
            H264ENC_MCR_SW_RESET_MASK);  
  reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;  
  reg_temp |= val_temp;                
  reg_write32(&reg_base->MCR,reg_temp); 
        
  msleep(100);

  reg_temp  = reg_read32(&reg_base->MCR);
  val_temp  = (((uint32_t)0u << H264ENC_MCR_SW_RESET_SHIFT) & 
              H264ENC_MCR_SW_RESET_MASK); 
  reg_temp &= H264ENC_MCR_SW_RESET_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->MCR,reg_temp); 
  
}

/**
* @brief        H264 Encoder enable interrupts.
* @details      H264 Encoder enable interrupts.
*
*
* @api
* @isr
*
*
*
* @pre
* @post
*
* @note
*/

void H264_ENC_enable_interrupts(void)
{  
  uint32_t reg_temp;
  uint32_t val_temp;
  
  reg_temp  = reg_read32(&reg_base->IER);
  val_temp  = ((uint32_t)((uint32_t)BR_TOO_HIGH_INT_EN_V << 
              IER_BR_TOO_HIGH_INT_EN_SHIFT) & IER_BR_TOO_HIGH_INT_EN_MASK); 
  reg_temp &= IER_BR_TOO_HIGH_INT_EN_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->IER,reg_temp);   
          
  reg_temp  = reg_read32(&reg_base->IER);
  val_temp  = ((uint32_t)((uint32_t)OUT_CBUF_OVF_INT_EN_V << 
              IER_OUT_CBUF_OVF_INT_EN_SHIFT) & IER_OUT_CBUF_OVF_INT_EN_MASK); 
  reg_temp &= IER_OUT_CBUF_OVF_INT_EN_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->IER,reg_temp);  
    
  reg_temp  = reg_read32(&reg_base->IER);
  val_temp  = ((uint32_t)((uint32_t)MBRW_FF_OVF_EN_V << 
              IER_MBRW_FF_OVF_INT_EN_SHIFT) & IER_MBRW_FF_OVF_INT_EN_MASK); 
  reg_temp &= IER_MBRW_FF_OVF_INT_EN_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->IER,reg_temp);  
       
  reg_temp  = reg_read32(&reg_base->IER);
  val_temp  = ((uint32_t)((uint32_t)ENC_OOS_INT_EN_V << 
              IER_ENC_OOS_INT_EN_SHIFT) & IER_ENC_OOS_INT_EN_MASK); 
  reg_temp &= IER_ENC_OOS_INT_EN_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->IER,reg_temp);  
    
  reg_temp  = reg_read32(&reg_base->IER);
  val_temp  = ((uint32_t)((uint32_t)BS_NEW_ROW_INT_EN_V << 
              IER_BS_NEW_ROW_INT_EN_SHIFT) & IER_BS_NEW_ROW_INT_EN_MASK); 
  reg_temp &= IER_BS_NEW_ROW_INT_EN_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->IER,reg_temp); 
   
  reg_temp  = reg_read32(&reg_base->IER);
  val_temp  = ((uint32_t)((uint32_t)FRAME_DONE_INT_EN_V << 
               IER_VI_FRAME_DONE_INT_EN_SHIFT) & IER_VI_FRAME_DONE_INT_EN_MASK); 
  reg_temp &= IER_VI_FRAME_DONE_INT_EN_MASK_NEG;   
  reg_temp |= val_temp;                 
  reg_write32(&reg_base->IER,reg_temp);  
   
} //H264_ENC_enable_interrupts()


/*******************************************************************************
 * EOF
 ******************************************************************************/

/** @} */
