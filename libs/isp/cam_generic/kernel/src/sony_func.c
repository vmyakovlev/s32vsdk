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
/**
 * \file    sony_func.c
 * \brief   Sony IMX224MQV MIPI/CSI camera functionality.
 * \author  Tomas Babinec
 * \version 0.1
 * \date    31-August-2015
 ****************************************************************************/
#include "config.h"

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
  #include "i2c.h"
  #include "seq_sa_kernel_api.h"
#endif // #if !defined(__STANDALONE__)

#include "s32vs234.h"
#include "cam.h"
#include "csi_func.h"  
#include "sony_func.h"
#include "sony_types.h"
#include "vdb_log.h"
 
//****************************************************************************
// consts
//****************************************************************************

//****************************************************************************
// macros
//****************************************************************************

//****************************************************************************
// static variables
//****************************************************************************

// defines the state of the exposure update
static SONY_ExpState_t sExpState[2] = {SONY_EXP_READY, SONY_EXP_READY};

static CamGeneric_t * sppSonyCam[2] = {NULL};

//****************************************************************************
// functions declarations
//****************************************************************************

static int32_t Siul2InitS(enum I2C_BUS_IDX aI2cBusIdx);

//****************************************************************************

int32_t SONY_DRV_Setup(const CamI2cClient_t aClient)
{ 
  int32_t lRet = SONY_DRV_SUCCESS;
  CamGeneric_t *const lpCam = aClient;
  enum I2C_BUS_IDX  lI2cBusIdx = (enum I2C_BUS_IDX)lpCam->mCfg.mI2cBusIdx;

  if(I2C_BUS_IDX_0 == lI2cBusIdx)
  {
    if( sppSonyCam[0] == NULL)
    {
#ifndef __STANDALONE__
      // initialize pin config
	  lRet = Siul2InitS(I2C_BUS_IDX_0);
      if(SONY_DRV_SUCCESS == lRet)
      {
        msleep(100);
#endif
        CSI_IrqHandlerRegister(CSI_IDX_0, SONY_DRV_IrqHandler);
        sppSonyCam[CSI_IDX_0]    = lpCam;
        sExpState[CSI_IDX_0] = SONY_EXP_READY;
#ifndef __STANDALONE__
      } // if driver data init failed                 
#endif
    } // if not yet used
  }
  else if (I2C_BUS_IDX_1 == lI2cBusIdx)
  {
    if( sppSonyCam[1] == NULL)
    {
#ifndef __STANDALONE__
      // initialize pin config
	  lRet = Siul2InitS(I2C_BUS_IDX_1);
      if(SONY_DRV_SUCCESS == lRet)
      {
        msleep(100);
#endif
       CSI_IrqHandlerRegister(CSI_IDX_1, SONY_DRV_IrqHandler);
       sppSonyCam[CSI_IDX_1] = lpCam;
       sExpState[CSI_IDX_1] = SONY_EXP_READY;
#ifndef __STANDALONE__
      } // if driver data init failed                 
#endif
    } // if not yet used
  }
  else
  {
    VDB_LOG_ERROR("CSI slot already in use by Sony IMX224 camera.\n");
    lRet = SONY_DRV_FAILURE;
  } // else from if not yet used

  return lRet;
} // SONY_DRV_Setup()

//****************************************************************************

int32_t SONY_DRV_Close(const CamI2cClient_t aClient)
{
  int32_t lRet = SONY_DRV_SUCCESS;
  CamGeneric_t *const lpCam = aClient;
  enum I2C_BUS_IDX lI2cBusIdx = (enum I2C_BUS_IDX)lpCam->mCfg.mI2cBusIdx;
  
  if(I2C_BUS_IDX_0 == lI2cBusIdx)
  {
    if( sppSonyCam[0] != NULL)
    {
      CSI_IrqHandlerRegister(CSI_IDX_0, NULL);
       sppSonyCam[0] = NULL;
    } // if csi index in use
  }
  else if(I2C_BUS_IDX_1 == lI2cBusIdx)
  {
    if( sppSonyCam[1] != NULL)
    {
      CSI_IrqHandlerRegister(CSI_IDX_1, NULL);
       sppSonyCam[1] = NULL;
    } // if csi index in use
  }
  else
  {
    VDB_LOG_WARNING("No camera using %u csi slot.\n", lI2cBusIdx);
  } // else from if csi index in use 

  return lRet;
} //SONY_DRV_Close()

//****************************************************************************

static int32_t Siul2InitS(enum I2C_BUS_IDX aI2cBusIdx)
{
  int32_t          lRet        = SONY_DRV_SUCCESS;
  CAM_Siul2Regs_t *lpSiul2Regs = NULL;
    
  lpSiul2Regs = (CAM_Siul2Regs_t*) ioremap((uintptr_t)&SIUL2, 
                                           sizeof(CAM_Siul2Regs_t));
  if(lpSiul2Regs == NULL)
  {
    VDB_LOG_ERROR("Failed to map SUIL2 registers.\n");
    lRet = SONY_DRV_FAILURE;
  } // if SUIL2 registers not mapped
  else
  {
    // *** turn on I2C bus ***
    switch(aI2cBusIdx)
    {
#ifdef OLD_EVB
      case I2C_BUS_IDX_0:
          ///////////////// I2C_0 - MIPI camera ////////////////////////////////

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
        lRet = SONY_DRV_FAILURE;
        break;      
    } // switch(aIdx)
    
    // *** turn on Sony cam ***
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
} // Siul2InitS(enum I2C_BUS_IDX aIdx)

//*****************************************************************************

void SONY_DRV_IrqHandler(enum CSI_IDX aCsiIdx, uint32_t aIrqType)
{
  if(0U != (aIrqType & 0x00000002U)) //EOF
  {
    if(sExpState[aCsiIdx] == SONY_EXP_SENT)
    {
      sExpState[aCsiIdx] = SONY_EXP_APPLIED; ///< move to updated state
    } // if SENT
    else
    {
      if(sExpState[aCsiIdx] == SONY_EXP_APPLIED)
      {
        sExpState[aCsiIdx] = SONY_EXP_USED; ///< move to used state
      } // if APPLIED
      else
      {
      } // if APPLIED
    } // if SENT
  } // if EOF
  
  if(0U != (aIrqType & 0x00000001U)) //SOF
  {
    if(sExpState[aCsiIdx] == SONY_EXP_USED)
    {
      sExpState[aCsiIdx] = SONY_EXP_READY; ///< move to ready state
    } // if USED
    else
    {
    } // if USED
  } // if SOF
  else
  {
  } // if SOF
  
} // SONY_DRV_IrqHandler()

//*****************************************************************************

void SONY_DRV_ExpSent(enum CSI_IDX aCsiIdx)
{    
  sExpState[aCsiIdx] = SONY_EXP_SENT;
} // SONY_DRV_ExpSent()

//*****************************************************************************

SONY_ExpState_t SONY_DRV_ExpStateGet(enum CSI_IDX aCsiIdx)
{    
  return sExpState[aCsiIdx];
} // SONY_DRV_ExpStateGet()

//****************************************************************************

/* EOF */
