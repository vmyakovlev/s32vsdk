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

#ifdef ACF_KERNEL_IMPLEMENTATION

#ifndef APUBRIEF_H
#define APUBRIEF_H

#define MAX_SAMPLES 64 * 8 * 2
#define UPPER_LEFT 0xF0
#define UPPER_RIGHT 0xF1
#define LOWER_LEFT 0xF2
#define LOWER_RIGHT 0xF3
#define COMPUTE 0xF4
#define ERROR 0xFF
#define GO 1
#define NO_GO 0

#define NO_FILTERING 0
#define BOX_FILTERING 1
#define GAUSSIAN_FILTERING 2

/*!
   Remove keypoints that are close to border. Keypoints should be located 28 pixels inside of the image edge (9/2 + 48/2).
   (signed 16-bit -> signed 16-bit)

   \param[in] apIn apCenter Points to the start of the chunk
   \param[in] aStr Chunk stride
   \param[in] aFilterType Filtering: 0 - no filter, 1 - box 9x9, 2 - gauss 9x9
   \param[in] apState Current state of the algorithm
   \param[in] apSmplPtrn Contains the sampling coordinates
   \param[in] aNrOfPacketsUpLeft Upper left chunk nr of smpl coordinates
   \param[in] aNrOfPacketsUpRight Upper right chunk nr of smpl coordinates
   \param[in] aNrOfPacketsLwLeft Lower left chunk nr of smpl coordinates
   \param[in] aNrOfPacketsLwRight Lower right chunk nr of smpl coordinates
   \param[in] aDscrSzB Descriptor size in bytes: 16, 32 or 64
   \param[out] apOut Descriptor output
 */
int apu_brief_compute_descriptor(vec08u *apIn,
                                 int aStr,
                                 char aFilterType,
                                 unsigned char *apState,
                                 unsigned int *apSmplPtrn,
                                 int aNrOfPacketsUpLeft,
                                 int aNrOfPacketsUpRight,
                                 int aNrOfPacketsLwLeft,
                                 int aNrOfPacketsLwRight,
                                 vec08u *apOut,
                                 int aDscrSzB);

#endif /* APUBRIEF_H */

#endif /* ACF_KERNEL_IMPLEMENTATION */
