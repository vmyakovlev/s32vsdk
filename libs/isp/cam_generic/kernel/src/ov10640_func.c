/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
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
/**
 * \file    sony_func.c
 * \brief   Omnivision 10640 MIPI/CSI camera functionality.
 * \author  Tomas Babinec
 * \version 0.1
 * \date    14-December-2015
 ****************************************************************************/
#if !defined(__STANDALONE__)
  #include <linux/kernel.h>     // We're doing kernel work
  #include <linux/module.h>     // Specifically, a module 
  #include <linux/interrupt.h>  // For interrupts
  #include <asm/siginfo.h>      // siginfo
  #include <linux/sched.h>      // send_sig_info
  #include <linux/types.h>
  #include <linux/delay.h>      // for msleep
  #include <linux/i2c.h>
  #include <asm/io.h>           // for ioremap
  
#else
  #include <stdint.h>
  #include <stddef.h>
  #include "config.h"
  #include "i2c.h"
  #include "seq_sa_kernel_api.h"
#endif // #if !defined(__STANDALONE__)

#include "s32vs234.h"
#include "cam.h"
#include "csi_func.h" 
#include "ov10640_func.h"
#include "ov10640_types.h"

#include "vdb_log.h"
 
//****************************************************************************
// consts
//****************************************************************************


//****************************************************************************
// macros
//****************************************************************************

//****************************************************************************
// static global variables
//****************************************************************************
  
static CamGeneric_t *sppCam[2] = {NULL};

//****************************************************************************
// functions declarations
//****************************************************************************

static int32_t Siul2Init(enum I2C_BUS_IDX aI2cBusIdx);

/****************************************************************************/
/** CSI frame IRQ handler.
 * 
 * \param aCsiIdx index of CSI interface the camera is connected to.
 * \param aIrqType type of the irq to handle
 ****************************************************************************/
static void OV10640_DRV_IrqHandler(enum CSI_IDX aCsi_Idx, uint32_t aIrqType);

//****************************************************************************

int32_t OV10640_DRV_Setup(const CamI2cClient_t aClient)
{ 
  int32_t           lRet       = OV10640_DRV_SUCCESS;
  CamGeneric_t *const lpCam = aClient;
  enum I2C_BUS_IDX  lI2cBusIdx = (enum I2C_BUS_IDX)lpCam->mCfg.mI2cBusIdx;
  
  if(I2C_BUS_IDX_0 == lI2cBusIdx)
  {
    if(sppCam[0] == NULL)
    {
#ifndef __STANDALONE__
      // initialize pin config
      lRet = Siul2Init(I2C_BUS_IDX_0);

      if(OV10640_DRV_SUCCESS == lRet)
      {
        msleep(100);
#endif
        CSI_IrqHandlerRegister(CSI_IDX_0, OV10640_DRV_IrqHandler);
        sppCam[0] = lpCam;
#ifndef __STANDALONE__
      } // if driver data init failed                 
#endif
    } // if not yet used
  }
  else if(I2C_BUS_IDX_1 == lI2cBusIdx)
  {
    if(sppCam[1] == NULL)
    {
#ifndef __STANDALONE__
      // initialize pin config
      lRet = Siul2Init(I2C_BUS_IDX_1);

      if(OV10640_DRV_SUCCESS == lRet)
      {
        msleep(100);
#endif
        CSI_IrqHandlerRegister(CSI_IDX_1, OV10640_DRV_IrqHandler);
        sppCam[1] = lpCam;
#ifndef __STANDALONE__
      } // if driver data init failed                 
#endif
    } // if not yet used
  }
  else
  {
    VDB_LOG_ERROR("CSI slot already in use by Sony IMX224 camera.\n");
    lRet = OV10640_DRV_FAILURE;
  } // else from if not yet used

  return lRet;
} // OV10640_DRV_Setup()

//****************************************************************************

int32_t OV10640_DRV_Close(const CamI2cClient_t aClient)
{
  int32_t lRet = OV10640_DRV_SUCCESS;
  CamGeneric_t *const lpCam = aClient;
  enum I2C_BUS_IDX  lI2cBusIdx = (enum I2C_BUS_IDX)lpCam->mCfg.mI2cBusIdx;

  if(I2C_BUS_IDX_0 == lI2cBusIdx)
  {
   if(sppCam[0] != NULL)
   {
     CSI_IrqHandlerRegister(CSI_IDX_0, NULL);
     sppCam[0] = NULL;
   } // if csi index in use
  }
  else if(I2C_BUS_IDX_1 == lI2cBusIdx)
  {
   if(sppCam[1] != NULL)
   {
     CSI_IrqHandlerRegister(CSI_IDX_1, NULL);
     sppCam[1] = NULL;
   } // if csi index in use
  }
  else
  {
    VDB_LOG_WARNING("No camera using %u csi slot.\n", lI2cBusIdx);
  } // else from if csi index in use 
 
  return lRet;
} //OV10640_DRV_Close()

//****************************************************************************

static int32_t Siul2Init(enum I2C_BUS_IDX aI2cBusIdx)
{
  int32_t          lRet        = OV10640_DRV_SUCCESS;
  CSI_Siul2Regs_t *lpSiul2Regs = NULL;
    
  lpSiul2Regs = (CSI_Siul2Regs_t*) ioremap((uintptr_t)&SIUL2, sizeof(CSI_Siul2Regs_t));
  if(lpSiul2Regs == NULL)
  {
    VDB_LOG_ERROR("Failed to map SUIL2 registers.\n");
    lRet = OV10640_DRV_FAILURE;
  } // if SUIL2 registers not mapped
  else
  {
    // *** turn on I2C bus ***
    switch(aI2cBusIdx)
    {
#ifdef OLD_EVB
      case I2C_BUS_IDX_0:
          ///////////////// I2C_0 - MIPI camera ////////////////////////////////////////

          // SDA    
        lpSiul2Regs->MSCR[15].B.MUX_MODE = 1;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[15].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[15].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[15].B.ODE = 1;   // open drain enabled
        lpSiul2Regs->MSCR[15].B.DSE = 7; // maximum drive strenght

        lpSiul2Regs->IMCR[269].B.DAISY = 2;  // PA15 selected as input*/

          // SCL
        lpSiul2Regs->MSCR[16].B.MUX_MODE = 1;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[16].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[16].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[16].B.ODE = 1;   // open drain enabled*/
        lpSiul2Regs->MSCR[16].B.DSE = 7; // maximum drive strenght

        lpSiul2Regs->IMCR[268].B.DAISY = 2;  // PB0 selected as input*/
        break;

      case I2C_BUS_IDX_1:
        // SDA
        lpSiul2Regs->MSCR[17].B.MUX_MODE = 0;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[17].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[17].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[17].B.ODE = 1;   // open drain enabled

        lpSiul2Regs->IMCR[271].B.DAISY = 2;      // PB1 selected as input

        // SCL
        lpSiul2Regs->MSCR[18].B.MUX_MODE = 0;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[18].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[18].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[18].B.ODE = 1;   // open drain enabled
        
        lpSiul2Regs->IMCR[271].B.DAISY = 2;      // PB2 selected as input
        break;
#else
      case I2C_BUS_IDX_0:
        ///////////////// I2C_0 - MIPI camera ////////////////////////////////////////

        // SDA    
        lpSiul2Regs->MSCR[99].B.MUX_MODE = 2;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[99].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[99].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[99].B.ODE = 1;   // open drain enabled
        lpSiul2Regs->MSCR[99].B.DSE = 7; // maximum drive strenght

        lpSiul2Regs->IMCR[269].B.DAISY = 3;  // PA15 selected as input*/

        // SCL
        lpSiul2Regs->MSCR[100].B.MUX_MODE = 2;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[100].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[100].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[100].B.ODE = 1;   // open drain enabled*/
        lpSiul2Regs->MSCR[100].B.DSE = 7; // maximum drive strenght

        lpSiul2Regs->IMCR[268].B.DAISY = 3;  // PB0 selected as input*/
        break;

      case I2C_BUS_IDX_1:
        ///////////////// I2C_1 - MIPI camera ////////////////////////////////////////

        // SDA    
        lpSiul2Regs->MSCR[101].B.MUX_MODE = 2;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[101].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[101].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[101].B.ODE = 1;   // open drain enabled
        lpSiul2Regs->MSCR[101].B.DSE = 7; // maximum drive strenght

        lpSiul2Regs->IMCR[271].B.DAISY = 3;  // PA15 selected as input*/

        // SCL
        lpSiul2Regs->MSCR[102].B.MUX_MODE = 2;  // GPIO mux mode selected
        lpSiul2Regs->MSCR[102].B.OBE = 1;   // output buffer enabled
        lpSiul2Regs->MSCR[102].B.IBE = 1;   // input buffer enabled
        lpSiul2Regs->MSCR[102].B.ODE = 1;   // open drain enabled*/
        lpSiul2Regs->MSCR[102].B.DSE = 7; // maximum drive strenght

        lpSiul2Regs->IMCR[270].B.DAISY = 3;  // PB0 selected as input*/
        break;
#endif // ifdef OLD_EVB      

      default:
        lRet = OV10640_DRV_FAILURE;
        break;      
    } // switch(aIdx)
    
    // *** turn on Ov10640 cam ***
    if(aI2cBusIdx == I2C_BUS_IDX_0)
    {
      // wake up camera
      lpSiul2Regs->MSCR[38].B.MUX_MODE = 0;  // GPIO mux mode selected
      lpSiul2Regs->MSCR[38].B.OBE = 1;   // output buffer enabled
      lpSiul2Regs->MSCR[38].B.DSE = 7;   // Drive Strenght

      lpSiul2Regs->GPDO[9].B.PDO_4n2 = 1;
    } // if I2C_BUS_IDX_0
    else if(aI2cBusIdx == I2C_BUS_IDX_1)
    {
      lpSiul2Regs->MSCR[31].B.MUX_MODE = 0;  // GPIO mux mode selected
      lpSiul2Regs->MSCR[31].B.OBE = 1;   // output buffer enabled
      lpSiul2Regs->MSCR[31].B.DSE = 7;   // Drive Strenght

      lpSiul2Regs->GPDO[7].B.PDO_4n3 = 1;
    } // if I2C_BUS_IDX_1
    else
    {
      // error
    }
   
    iounmap(lpSiul2Regs);
  } // if SUIL2 registers not mapped
  return lRet;
} // Siul2Init(enum I2C_BUS_IDX_0 aIdx)

//*****************************************************************************

static void OV10640_DRV_IrqHandler(enum CSI_IDX aCsi_Idx, uint32_t aIrqType)
{
  if(aCsi_Idx == CSI_IDX_0)
  {
    if (0x00000002U == aIrqType)
    {
	}
  } // if CSI0
  
  if(aCsi_Idx == CSI_IDX_1)
  {
  } // if CSI1
} // OV10640_DRV_IrqHandler()

//*****************************************************************************

/* EOF */