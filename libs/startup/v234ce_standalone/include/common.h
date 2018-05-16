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

#ifndef COMMON_H
#define COMMON_H

/********************************************************************/

/* 
 * Include the specific CPU header file 
 */
//#define THIRDEYE
//#define RAINIER_1_0
//#define CALYPSO6M_1_0
#define HALO_1_0

#define DEBUG_PRINTS

#include "typedefs.h"
#include "typedefs_UINT8.h"
#if defined RAINIER_1_0
#include "MPC5746R.h"
#elif defined CALYPSO6M_1_0
#include "MPC5748G.h"
#elif defined HALO_1_0
#include "MAC57D54H.h"
//#include "halo.h"
//#include "CM4.h"

#endif
#include "thirdeye.h"
#include "printf.h"
//#include "stdlib.h"
//#include "typedefs_UINT8.h"
#include "user_module.h"
#include "cgm_api.h"
#include "serial.h"
//#include "intc_soc_config.h"
//#include "swt_soc_config.h"
//#include "pit_soc_config.h"
//#include "cgm_soc_config.h"
//#include "edma_soc_config.h"
//#include "smpu_soc_config.h"
//#include "mc_me_soc_config.h"
//#include "io.h"
//#define TRACE_MEMORY_OPERATION 1
/********************************************************************/

#endif /* COMMON_H */
