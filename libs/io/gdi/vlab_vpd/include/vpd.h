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
* \brief VLAB Virtual probe display library functionality declaration
* \author Tomas Babinec
* \version 0.1
* \date 03-July-2014
****************************************************************************/


#ifndef VPD_H
#define VPD_H

/****************************************************************************/
/** Maps VPD registers to virtual address space
 *
 * \note For INTEGRITY OS only.
 *
 * \return pointer to the register structure, NULL if failed
 ****************************************************************************/
vpd_regs_t * VPD_RegsMap(void);

/****************************************************************************/
/** Maps VPD video data area to virtual address space
 *
 * \note For INTEGRITY OS only.
 *
 * \return pointer to the data, NULL if failed
 ****************************************************************************/
uint8_t * VPD_DataMap(void);

#endif /* VPD_H */
