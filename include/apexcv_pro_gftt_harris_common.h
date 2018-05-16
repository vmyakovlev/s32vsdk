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

/*!*********************************************************************************
*  \file apexcv_pro_gftt_harries_common.h
*  \brief Apex GFTT/Harris Corner Detector.
***********************************************************************************/
#ifndef APEXCVPROGFTTHARRIESCOMMON_H
#define APEXCVPROGFTTHARRIESCOMMON_H

#define BOX_SIZE                 7  // default box size
#define NMS_SIZE                 5  // default nms size
#define DEFAULT_COVARIANCE       6  // default covariance value

#define USE_HARRIS 1
#define USE_GFTT 0

// combination setting for box filter size and nms filter size

#define MAX_FILTER_Y_7  7
#define MAX_FILTER_X_7  7
#define MAX_FILTER_Y_5  5
#define MAX_FILTER_X_5  5
#define MAX_FILTER_Y_3  3
#define MAX_FILTER_X_3  3

#define FLT_MASK_7      7
#define FLT_MASK_5      5
#define FLT_MASK_3      3

#define MAX_NMS_R       7
#define NMS_MASK        7

#define MAX_BLOCK_WIDTH         32  // maximum chunk-width + padding
#define MAX_BLOCK_HEIGHT        16
#define MAX_CORNER_PER_CHUNK    20
#define MAX_CORNERS           4096

#define MAX_MIN_DISTANCE         5

#define PORT_OUTPUT_DELAY_BOX7_NMS5 ((MAX_FILTER_Y_7>>1) + (NMS_SIZE>>1))
#define PORT_OUTPUT_DELAY_BOX5_NMS5 ((MAX_FILTER_Y_5>>1) + (NMS_SIZE>>1))
#define PORT_OUTPUT_DELAY_BOX3_NMS5 ((MAX_FILTER_Y_3>>1) + (NMS_SIZE>>1))
#endif /* APEXCVPROGFTTHARRIESCOMMON_H */

