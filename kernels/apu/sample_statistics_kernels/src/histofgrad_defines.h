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
/*!
* \file apu_histofgrad_apu.h
* \brief histogram of gradients computation for APEX
* \addtogroup histogram of gradients
* @{
****************************************************************************/

#ifndef APUHISTOFGRADDEFINES_H
#define APUHISTOFGRADDEFINES_H

#define HOG_NR_FEATURES_PER_HISTO		16
#define HOG_NR_SCALES					 1
#define HOG_NR_OVERLAPPING_WINDOWS		 4
#define HOG_NR_FEATURES_PER_BOX			 (HOG_NR_FEATURES_PER_HISTO * HOG_NR_OVERLAPPING_WINDOWS * HOG_NR_SCALES)
#define HOG_OVERLAP						 2
#define HOG_LAT_DEPENDENCY				 4
#define HOG_WND_SZ						 4

#endif /* APUHISTOFGRADDEFINES_H */

/*! @} */
