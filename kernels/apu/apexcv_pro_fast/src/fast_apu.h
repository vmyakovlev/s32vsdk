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


#ifndef FASTAPU_H
#define FASTAPU_H

#ifdef APEX2_EMULATE 
#include "apu_lib.hpp" // if using the APU emulation library
using namespace APEX2;
#endif
#ifdef ACF_KERNEL_IMPLEMENTATION 

/*!
   Computation of the offsets of pixel located 
   on a digitized circle of a given circumference.

   \param offsets       - [Output] Destination block pointer
   \param sstr          - [Input]  Source block stride
   \param circumference - [Input]  Circle circumference (8, 12 or 16)
 */
void apu_fastx_offset
(
   uint32_t* offsets,           //Pointer to offset table's buffer
   int  sstr,              //Source block stride
   int  circumference     //Circle circumference
);

/*!
   Computation of FAST9 Corner score without NMS.
   Score will be either threshold (corner) or 0 (not corner)
   \param dst        - [Output]  Corner score block pointer
   \param dstr       - [Input]   Corner score block stride
   \param src        - [Input]   Source block pointer
   \param sstr       - [Input]   Source block stride
   \param bw         - [Input]   Block width
   \param bh         - [Input]   Block height
   \param threshold  - [Input]   Minimal threshold for corner detection
   \param offset     - [Input]   Pre-computed Offset table (must be computed with 16 circle conference and source block pointer stride
 */
void apu_fast
(
   vec08u*  dst,     //Destination image (grayscale)
   int dstr,         //Destination block stride         
   vec08u*  src,     //Source image (grayscale)
   int sstr,         //Source block stride
   int  bw,  int bh, //!< block width/height
   int  threshold,   //!< Threshold
   int* offset       //!< pre-computed offset positions
);

 /*!
   Computation of FAST9 Corner score, the output is a list not an image
   Score will be either threshold (corner) or 0 (not corner)
   \param list        - [Output] Coordinates inside the img, <x,y>
   \param nrOfElements - [Output] Nr of detected points
   \param firstCuId - [Input] Id of the first CU in the APU
   \param nrOfCu - [Input] Nr of CUs that are processing
   \param src        - [Input]   Source block pointer
   \param sstr       - [Input]   Source block stride
   \param bw         - [Input]   Block width
   \param bh         - [Input]   Block height
   \param threshold  - [Input]   Minimal threshold for corner detection
   \param offset     - [Input]   Pre-computed Offset table (must be computed with 16 circle conference and source block pointer stride
 */
void apu_fast_serialized
(
   unsigned short * list, 
   int * nrOfElements, //!< Out list of coordinate pairs <x, y>
   int firstCuId, 
   int nrOfCu,
   int maxOutSample,
   vec08u * src,
   int sstr,
   int bw,  
   int bh,
   int threshold,
   int * offset);

/*!
   Computation of FAST9 Corner score with NMS.
   Score will be the maximum value of threshold for which a point is detected as a corner
   \param dst        - [Output]  Corner score block pointer
   \param dstr       - [Input]   Corner score block stride
   \param src        - [Input]   Source block pointer
   \param sstr       - [Input]   Source block stride
   \param bw         - [Input]   Block width
   \param bh         - [Input]   Block height
   \param threshold  - [Input]   Minimal threshold for corner detection
   \param offset     - [Input]   Pre-computed Offset table (must be computed with 16 circle conference and source block pointer stride
 */
void apu_fast_nms
(
   vec08u*  dst, int dstr,    //!< Image of corners (binary/corner score/reponse/strenght)
   vec08u*  src, int sstr,    //!< Source image (grayscale)
   int  bw,  int bh,          //!< block width/height
   int  threshold,            //!< Threshold
   int* offset                //!< pre-computed offset positions
);

/*!
   Non-maxima supression 3x3. Taken from apu_nms_impl.h

   \param apcIn         - [input] Source chunk
   \param apOut         - [output] Destination chunk   
   \param aInStride     - [input] Source stride
   \param aOutStride    - [input] Destination stride
   \param aTileWidth    - [input] chunk width
   \param aTileHeight   - [input] chunk height
*/
void apu_nms(
      const vec08u* apcIn, 
      vec08u* apOut,
      int aInStride, 
      int aOutStride,
      int aTileWidth, 
      int aTileHeight);
#endif 
#endif /* FASTAPU_H */

/*! @} */
