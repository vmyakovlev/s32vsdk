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

#ifndef KERNELSCONFIGURATION_H
#define KERNELSCONFIGURATION_H

#include <math.h>

#define HISTOGRAM_BIN_COUNT 64
#define TOPVIEW_HEIGHT 320
#define TOPVIEW_WIDTH 256
#define MAX_LUMENS 255 // maximum saturation value
#define HISTOGRAM_AREA .3 // percent of the image to histogram from the bottom

const int32_t  gcRemapDestBlockWidth   = 4; //6;    // from remap apex kernel for 720x480 --> 256x320
const int32_t  gcRemapDestBlockHeight  = 2; //8;    // from remap apex kernel for 720x480 --> 256x320
const int32_t  gcRemapSrcBlockWidth    = 16; //24;  // unused
const int32_t  gcRemapSrcBlockHeight   = 4; //6;    // unused
const uint8_t  gcNumOfAccumalators     = TOPVIEW_HEIGHT / gcRemapDestBlockWidth; // MUST divide perfectly with TOPVIEW_HIEGHT.

/* Outputs the points in the format:
 * [i+0] LEFT vote value
 * [i+1] LEFT x-value correspeonding to vote
 * [i+2] LEFT boolean valid?
 * [i+3] RIGHT vote value
 * [i+4] RIGHT x-value corresponding to vote
 * [i+5] RIGHT boolean valid?
 * where i varies from 0... gcNumOfAccumulators
 */
const uint8_t gcDataOutStride = 6;


#ifndef _TOGGLE_REG_GRAPH_
   /*!*********************************************************************************
    *  @brief This method computes a linear regression line based on input points.
    *
    *  Points which are too far below max votes (currently * 0.5) are removed.
    */
   void RegressionLine(
     const uint32_t   cMaxVotes,
     const uint32_t*  cPointsData,
     const uint8_t  cDataStride,
           uint32_t*   rLane);

   /*!*********************************************************************************
    *  @brief Completes a fast arctan. Output is in degrees.
    *
    * See "Efficient approximations for the arctangent function",
    *      Rajan, S. Sichun Wang Inkol, R. Joyal, A., May 2006
    */
   int16_t FastArcTan(float x);

#endif // end: #ifndef _TOGGLE_REG_GRAPH_
#endif /* KERNELSCONFIGURATION_H */
   


