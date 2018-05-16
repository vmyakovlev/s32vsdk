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
#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#ifdef APEX2_EMULATE
#include <tchar.h>
#include "apu_lib.hpp"
#include "apu_extras.hpp"
#include "acf_lib.hpp"
using namespace APEX2;
#else
#include <icp_data.h>
#include <apex.h>
#include <oal.h>
using namespace icp;

#endif


#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)
#define CHNL_CNT io::IO_DATA_CH3

#ifdef __STANDALONE__
#include "frame_output_dcu.h"
extern io::FrameOutputDCU output;
#else // #ifdef __STANDALONE__
#include "frame_output_v234fb.h"
extern io::FrameOutputV234Fb output;
#endif // else from #ifdef __STANDALONE__  


extern void *lp_buffer ;
extern cv::Mat output_mat;

void InitDisplay();
void ReleaseDisplay();
void showTargetImg(cv::Mat &output_mat);
#endif

void showResults(DataDescriptor* Out);

#endif /* DISPLAYDATA_H */