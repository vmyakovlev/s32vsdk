/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor
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

/*******************************************************************************
 *  \file orb_apu.h
 *  \brief ACP kernel implementation API
 ******************************************************************************/

#ifndef ORBAPU_H
#define ORBAPU_H

#include <stdio.h>
#include <stdint.h>

#ifdef APEX2_EMULATE
#include "apu_lib.hpp"
using namespace APEX2;
#endif

#ifdef ACF_KERNEL_IMPLEMENTATION
/*!
   Calculates the intensity centroid orientation, the output is an index
   from [0, 39], if you multiply this output with 9 you will get the
   orientation in degrees. At this moment the resolution is 9 degrees.

   \param apOut    - [Output] Out data
   \param apIn     - [Input]  Input image, on this img the scores were calc
   \param aInStr   - [Input]  Input image stride
   \param aPatchSz - [Input]  The length of the patch
   \param aRadius  - [Input]  Radius of calculus
 */
void apu_orb_orientation(vec16u *apOut,
                         vec08u *apIn,
                         int aInStr,
                         int aPatchSz,
                         int aRadius);

/*!
    Calculates the feature descriptor inside an image patch.

    \param apIn       - [Input] Pointer to he center the patch
    \param aInStr     - [Input] Chunk stride
    \param apOrntn    - [Input] Patch Orientation [0, 255] ~ [0, 2*pi]
    \param apSmplPtrn - [Input] Brief sampling pattern
    \param apOut      - [Output] Output descriptor
    \param aDscrSzB   - [Input] Descriptor sizes are 16, 32 and 64 Bytes
    \param aPatchSz   - [Input] Patch Size Dimensions
 */
void apu_orb_rbrief(vec08u *apIn,
                    int aInStr,
                    vec16u *apOrntn,
                    int8_t *apSmplPtrn,
                    vec08u *apOut,
                    int8_t aDscrSzB,
                    int8_t aPatchSz);
#endif /* ACF_KERNEL_IMPLEMENTATION */
#endif  /* ORBAPU_H */
