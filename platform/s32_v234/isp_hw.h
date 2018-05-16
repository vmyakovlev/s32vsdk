/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2013-2016 Freescale Semiconductor
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
 * \file    isp_hw.h
 * \brief   definitions of ISP HW related constants
 ****************************************************************************/

#ifndef ISPHW_H
#define ISPHW_H

#ifndef __KERNEL__
  #include <stdint.h>
#endif // #ifndef __KERNEL__

/*****************************************************************************
* constants
*****************************************************************************/

// *** DDR RAM ***
#define DDR_BASE          (uint32_t)0x80000000u

// *** SRAM RAM ***
#define SRAM_BASE         (uint32_t)0x3e800000u
#define SRAM_SIZE         (uint32_t)(4u * 1024u * 1024u)
#define SRAM_SINGLE_SIZE  (uint32_t)(3u * 1024u * 1024u)
#define SRAM_MULTI_SIZE   (uint32_t)(1024u * 1024u)

// *** Code & Data RAM ***
#ifdef CRAM_BASE
  #undef CRAM_BASE
#endif
#define CRAM_BASE         (uint32_t)0x7C000000u
#define CRAM_BASE_M0      (uint32_t)0x0u
#define CRAM_SIZE         (uint32_t)(64u * 1024u)

// *** Parameter RAM ***
#ifdef PRAM_BASE
  #undef PRAM_BASE
#endif
#define PRAM_BASE         (uint32_t)0x7C019000u
#define PRAM_SIZE         (uint32_t)(4u * 1024u)

// Sequencer view of PRAM
#define SEQ_PRAM_OFFSET   (uint32_t)0x00019000u   ///< A53 view
#ifdef SEQ_PRAM_BASE
  #undef SEQ_PRAM_BASE
#endif
#define SEQ_PRAM_BASE     (uint32_t)0x20000000u   ///< M0 view

// *** Debug Parameter RAM ***
#define DBG_PRAM_BASE     (uint32_t)0x7C018000u
#define DBG_PRAM_SIZE     (uint32_t)(4u * 1024u)

// *** Kernel RAM ***
#ifdef KRAM_BASE
  #undef KRAM_BASE
#endif
#define KRAM_BASE         (uint32_t)0x7C01A000u
#define KRAM_SIZE         (uint32_t)(4u * 1024u)

//****************************************************************************
#define SEQ_SIZE                    (uint32_t)0x28u
#define FDMA_BASE                   (uint32_t)0x7c040000u
#define FDMA_SIZE                   (uint32_t)0x38u

///< number of available CSI interfaces
#define CSI_IF_CNT                  (uint32_t)2u
#define CSI0_BASE                   (uint32_t)0x40030000u
#define CSI1_BASE                   (uint32_t)0x400A0000u
#define CSI_SIZE                    (uint32_t)0x140u

///< number of available CSI interfaces
#define VIU_IF_CNT                  (uint32_t)2u

///< I2C addressess of camera related HW
#define I2C_OV10640_CAM_ADDR        (uint8_t)0x30
#define I2C_OV10635_CAM_ADDR        (uint8_t)0x30
#define I2C_SONY_CAM_ADDR           (uint8_t)0x1A
#define I2C_MAXIM_DES_ADDR          (uint8_t)0x6A
#define I2C_HDMI_ADDR               (uint8_t)0x39  

#define I2C_MAX_9271_BASE           ((uint8_t)0x40)
#define I2C_MAX_96705_BASE           ((uint8_t)0x40)
#define I2C_MAX_OV10640_BASE        I2C_OV10640_CAM_ADDR
#define I2C_MAX_OV10635_BASE        I2C_OV10635_CAM_ADDR
#define I2C_MAX_IMX224_BASE         I2C_SONY_CAM_ADDR
#define I2C_MAX_OV490_BASE        ((uint8_t)0x24)

///< number of available IPUs engines
#define IPUS_CNT                    (uint32_t)8u
///< number of available IPUv engines
#define IPUV_CNT                    (uint32_t)4u

/****************************************************************************/
/** Defines possible ISP LOG events.
*
*****************************************************************************/
typedef enum ISP_LogEvent
{
  ISP_LOGEVT_IPUS = 0,
  ISP_LOGEVT_IPUV,
  ISP_LOGEVT_OTHER,  
  ISP_LOGEVT_SEQ_FD,
  ISP_LOGEVT_CSI_FE,
  ISP_LOGEVT_SW_EVT,
  ISP_LOGEVT_CSI_LINE,
  ISP_LOGEVT_FDMA_LINE,
  ISP_LOGEVT_MAX
} ISP_LogEvent_t;

typedef volatile uint32_t vuint32_t;
typedef volatile uint16_t vuint16_t;
typedef volatile uint8_t  vuint8_t;

/****************************************************************************/
/** Inline functions instead of macros
 * 
*****************************************************************************/
static inline uint8_t I2C_MAX_9271_ADDR(uint32_t aIdx)
{
  return (uint8_t)(I2C_MAX_9271_BASE + aIdx);
}// I2C_MAX_9271_ADDR()

static inline uint8_t I2C_MAX_96705_ADDR(uint32_t aIdx)
{
  return (uint8_t)(I2C_MAX_96705_BASE + aIdx);
}// I2C_MAX_96705_ADDR()

//****************************************************************************

static inline uint8_t I2C_MAX_OV10640_ADDR(uint32_t aIdx)  
{
  return (uint8_t)(I2C_MAX_OV10640_BASE + aIdx);
} // I2C_MAX_OV10640_ADDR()

//****************************************************************************

static inline uint8_t I2C_MAX_OV490_ADDR(uint32_t aIdx)  
{
  return (uint8_t)(I2C_MAX_OV490_BASE + aIdx);
} // I2C_MAX_OV490_ADDR()


//****************************************************************************

static inline uint8_t I2C_MAX_OV10635_ADDR(uint32_t aIdx)
{
  return (uint8_t)(I2C_MAX_OV10635_BASE + aIdx);
} // I2C_MAX_OV10635_ADDR()

//****************************************************************************

static inline uint8_t I2C_MAX_IMX224_ADDR(uint32_t aIdx)
{
  return (uint8_t)(I2C_SONY_CAM_ADDR + aIdx);
} // I2C_MAX_IMX224_ADDR()

//****************************************************************************

#ifdef __cplusplus 
extern "C" { 
#endif



#ifdef __cplusplus 
} 
#endif 

#endif /* ISPHW_H */


