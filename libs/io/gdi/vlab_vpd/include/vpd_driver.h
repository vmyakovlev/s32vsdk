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
* \file gdi_srv.cpp
* \brief VLAB Virtual probe display driver
* \author Tomas Babinec
* \version 0.1
* \date 02-July-2014
****************************************************************************/

#ifndef VPDDRV_H
#define VPDDRV_H

// VLAB Video probe display registers region
#define VPD_REGS_PHYSICAL_POOL_BASE 0x16000000
#define VPD_REGS_PHYSICAL_POOL_SIZE 0x1000
#define VPD_REGS_RESOURCE_NAME "vpd_regs"

// VLAB Video probe display data region
#define VPD_DATA_PHYSICAL_POOL_BASE 0x14000000
#define VPD_DATA_PHYSICAL_POOL_SIZE 0x2000000
#define VPD_DATA_RESOURCE_NAME "vpd_data"

/****************************************************************************/
/** Virtual probe device registers for Treerunner toolbox
 *
 ****************************************************************************/
typedef struct _vpd_regs_t // vpd_regs_t -> taken from gdi_utils.h
{
  uint32_t FORMAT;
  uint32_t WIDTH;
  uint32_t HEIGHT;
  uint32_t ENABLE;
  uint32_t CONTROL;
} vpd_regs_t;

#endif /* VPDDRV_H */
