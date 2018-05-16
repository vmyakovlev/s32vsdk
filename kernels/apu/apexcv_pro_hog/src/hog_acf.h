/*****************************************************************************
* 
* NXP Confidential Proprietary
*
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
#ifndef HOGACF_H
#define HOGACF_H

#ifdef ACF_KERNEL_METADATA

extern KERNEL_INFO _kernel_info_hog_get_gradient;
extern KERNEL_INFO _kernel_info_hog_get_cell_histograms;
extern KERNEL_INFO _kernel_info_hog_compute_block_histograms;
extern KERNEL_INFO _kernel_info_hog_get_block_histograms;
extern KERNEL_INFO _kernel_info_hog_get_scores;

#endif   // #ifdef ACF_KERNEL_METADATA

#endif /* HOGACF_H */
