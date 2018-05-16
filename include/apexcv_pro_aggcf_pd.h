/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2016-2016 Freescale Semiconductor
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

#ifndef APEXCVPROAGGCFPD_H
#define APEXCVPROAGGCFPD_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "float.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>
#include "math.h"

#include <apexcv_error_codes.hpp> 
#include "apexcv_pro_util.h"
#include <umat.hpp>
#include <icp_data.h>
#include <oal.h>


namespace apexcv{


// Bounding Box Structure [x y w h score]
struct bbs
{
   int x, y, w, h;
   float score;
};


// detector paramters
struct opts
{
   // detector is trained with setting of follwing modelIDs and modelIDsPad are equal
   // so no extra padding needed
   float*   modelDs;           // model height+width without padding (eg [100 41])
   float*   modelDsPad;        // model height + width with padding (eg[128 64])
   int      stride;            // spatial stride between detection windows
   float    cascThr;           // constant cascade threshold(affects speed / accuracy)
   float    cascCal;           // cascade calibration(affects speed / accuracy)
   int      nPerOct;           // number of scales per octave
   int      nOctUp;            // number of upsampled octaves to compute
   int      nApprox;           // number of approx.scales(if - 1 nApprox = nPerOct - 1)
   float*   lambdas;           // coefficients for power law scaling(see BMVC10)
   int*     pad;               // amount to pad channels(along T / B and L / R)
   float*   minDs;             // minimum image size for channel computation
   float    smooth;            // radius for channel smoothing(using convTri)
   int      shrink;            // Channel subsampling
   int      normRad;           // Normalizing Radius
   float    normConst;         // Normalizing Constant
   bool     full;              // range of orientation [0 PI] or [0 2*PI]  
   int      nOrients;          // Number of gradient orientations to compute
   int      softBin;           // control histogram calculation method (nearest neighborhood, bilinear, trilinear interpolation)      
   double   overlap;           // non-maximal suppression overlap
   bool     luvSmooth;         // flag to control if apply low pass filtering to LUV 
   uint32_t *fxlambdas;        // Fixed Point conversion of lambda to calculation approximate scales
   uint32_t fxnormConst;       // Fixed Point conversion of normconst to apply magnitude normizlation
};

typedef struct scale_par
{
    int width;
    int height;
    int isRealscale;    // 1: real scale, 0:approximate scale
    int realScaleIndex; // realScale index used to calculate approxmiate scales
    float scaleRatio;   // scale ratio used to resize bounding box to correct size
 
    // image span       // to track the change of image span, update FidsArray if changed
    int spanSize;

}scalePar, *pScalePar;


/*!*********************************************************************************
* \brief apexcv::aggcf_detector is the host-ACF interface for aggregated channel 
* feature (aggCF) based pedestrian detection.
*
* This class provides interfaces to load pre-trained detector model, calculate aggregated channel  
* features including LUV, magnitude and histogram of gradient, then perform pedestrian
* detection and output bounding boxes of detected pedestrians  
* 
***********************************************************************************/
class AggCFDetector
{
public:

/***********************************************************************************
*  \brief Default constructor.
*  Allocate resources for aggCF.
***********************************************************************************/
   AggCFDetector();

/***********************************************************************************
*  \brief Destructor.
*   Release resources for aggCF.
***********************************************************************************/
   virtual ~AggCFDetector();

/*!*********************************************************************************
*  \brief aggCF load pre-trained detector model
***********************************************************************************/
   void InitDetectorModel(const char*);

   void InitDetectorModelBuf( char *detectorBuf, int bufSize);

/*!*********************************************************************************
*  \brief aggCF based on input image size and detector model (octaves/scales/bin)
*  calculate size of each scale
***********************************************************************************/
   void CalcScaleParameters(int inWidth, int inHight);

/*!*********************************************************************************
*  \brief aggCF calculate feature pyramid on default APEX  
***********************************************************************************/
   int CalcChannelsPyramid(vsdk::UMat &dstLUV, vsdk::UMat *outputPy);

/*!*********************************************************************************
*  \brief aggCF calculate feature pyramid and assign the task to specified APEX
***********************************************************************************/
   int CalcChannelsPyramid(vsdk::UMat &dstLUV, vsdk::UMat *outputPy, int ApexID);

/*!*********************************************************************************
*  \brief aggCF perform pedestrian detection on extracted feature pyramid using
*  detector model and return bounding boxes of detected pedestrians 
***********************************************************************************/
   std::vector<apexcv::bbs> ApplyPedDetectionDET(vsdk::UMat *channelFeatures);

/*!*********************************************************************************
*  \brief aggCF display loaded detector model parameters 
***********************************************************************************/
   void ShowDetectorParameters();

/*!*********************************************************************************
*  \brief aggCF perform non maximum suppression (NMS) on bounding boxes
***********************************************************************************/
   void ApplyPedDetectionNMS(std::vector<apexcv::bbs> &bbs, int greedy );

/*!*********************************************************************************
*  \brief aggCF init feature pyramid structure and allocate feature data buffer
***********************************************************************************/
   vsdk::UMat *InitPyramidBuf( );

/*!*********************************************************************************
*  \brief aggCF release feature data buffer
***********************************************************************************/
   void DeInitPyramidBuf( vsdk::UMat *&outPy );

/// @private
   vsdk::UMat *createImage_desc_helper(char *input_name, int width, int height);

/*!*********************************************************************************
*  \brief aggCF calculate one feature octave and assign the task to specified APEX.
*  using this function, APP has full control on how to calculate feature pyramid including
*  the order of each octave calculation within the pyramid, the assignment of octave calculationi task
*  to different APU to achieve optimal performance.
***********************************************************************************/
   int CalcChannelsOctave( vsdk::UMat &dstLUV, vsdk::UMat *outputPy, uint32_t realScaleIdx, int ApexID);

/*!*********************************************************************************
*  \brief aggCF calculate feature octaves with APU using specified octave and APEX array.
*  APEX code will decide how to dispatch octave calculation tasks to different APEX
*  for performance and load balance purpose. 
***********************************************************************************/
   int CalcChannelsOctave( vsdk::UMat &dstLUV, vsdk::UMat *outputPy, std::vector<uint32_t> &realScaleIdx, std::vector<uint32_t> &ApexID);

   //AcfProfilingInfo retProfilingInfo();
/// @private   
   APEXCV_LIB_RESULT retPyramidScalePars( std::vector<scalePar> & par )  
   {
      APEXCV_LIB_RESULT rval = APEXCV_SUCCESS;
      par = scalePars;

      if( true == this->mfailed )
      {
         rval = APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
      }
      return rval; 
   }
/// @private   
   APEXCV_LIB_RESULT retShrinkparameter(int &shrinkParam) 
   { 
      APEXCV_LIB_RESULT rval = APEXCV_SUCCESS;
      shrinkParam = dOpts.shrink;
      
      if( true == this->mfailed )
      {
         rval = APEXCV_ERROR_OBJECT_ISNOT_INITIALIZED;
      }    
       
      return rval;
   }

/*!*********************************************************************************
*  \brief aggCF return the flag to show if a detector model is loaded successfully 
***********************************************************************************/
   bool IsDetectorModelFailToLoaded() const { return mfailed; }

/// @private
   APEXCV_LIB_RESULT ConvertRGBtoLUV(vsdk::UMat &imageData, vsdk::UMat &dstLUV, int width, int height, int ApexID);

private:
   
   // init feature index array 
   void initFidsArray();
   
   // update feature index array if feature images width/stride is changed
   void updateScaleFidsArray(uint32_t scaleindex, uint32_t scaleSpan);
  
   // calculate one octave of feature pyramid, private member function called by aggcf_channelsPyramid
   int CalcChannelsOctave( vsdk::UMat &dst_luv, vsdk::UMat *output_py, uint32_t realScaleIdx);

   // calculate real scale
   int CalcChannelsRealScale( vsdk::UMat &dst_luv, vsdk::UMat *output_py, int32_t scaleIdx, int ApexID);
   
   // calculate approximate scale
   int CalcChannelsApproxScale(vsdk::UMat *output_py, int32_t scaleIdx, int ApexID);
   
   // apply low pass filtering to real scale 
   int ApplyChannelsRealScaleFiltering(vsdk::UMat *output_py, int32_t scaleIdx, int ApexID);

   // kernel calculator
   int32_t      mApexID;
    
   void *mAggcfMagoriApexP[2];
   void *mAggcfFlt3x3ApexP[2];
   void *mAggcfLUVApexP[2];
   void *mAggcfHistApexP[2];
   void *mAggcfScaleDown8P[2];
   void *mAggcfScaleDown4P[2];
   void *mAggcfScaleDown2P[2];

   
   // channel pyramid parameters
   std::vector<scalePar> scalePars; // scale parameters
   uint32_t *fids;                   // feature index array, each node of decision tree has one feature index
                                     // use to locate feature value in channel feature data
   double   *thrs;                   // decision tree threshold (decision stomp to decide go left or right along decision tree)
   uint32_t *fixThrs;                // converted fixed point threshold
   uint32_t *child;                  // decision tree child index
   double   *hs;                     // log ratio at each node to accumulate and compare with constant cascade threshold
   int      treeDepth;               // decision tree depth
   int      nTreeNodes;              // number of nodes in decision tree (define by fixed or maximum tree depth setting)
   int      nTrees;                  // number of decision trees
   opts     dOpts;                   // aggcf detection parameters
   bool     mfailed;                  // flag to indicate if detector model is loaded successfully
   uint32_t **channelIds;            // pre-calculated relative offset of feature index to each searching windows
   uint32_t mNumFeatures;            // number of features


   APEXCV_LIB_RESULT AGGCFResize16CalcCon( vsdk::UMat &in, int inOffset, int inW, int inH, vsdk::UMat &out, int outOffset, int outW, int outH, int num, uint32_t ratio, int ApexID);

   APEXCV_LIB_RESULT AGGCFMagOriCalc(vsdk::UMat &in, int inW, int inH, int ApexID);

   APEXCV_LIB_RESULT AGGCFfltCalc( vsdk::UMat &in, int inOffset, int inW, int inH,
                       vsdk::UMat &out, int outOffset, int outW, int outH, int num, uint32_t p, uint32_t normConst, int ApexID);

   APEXCV_LIB_RESULT AGGCFfltCalcCon(vsdk::UMat &in, int inOffset, int inW, int inH,
                       vsdk::UMat &out, int outOffset, int outW, int outH, int num, uint32_t p, uint32_t normConst, int ApexID);

   APEXCV_LIB_RESULT AGGCFhistCalc(vsdk::UMat &in, int inW, int inH,
                       vsdk::UMat &out, int outW, int outH, int bin, int softbin, int full, int num, int ApexID);

   APEXCV_LIB_RESULT AGGCFResize16fltCalcCon( vsdk::UMat &in, int inoffset, int inw, int inh, vsdk::UMat &out, int outoffset, int outw, int outh, int num, uint32_t ratio, int ApexID );
   
   APEXCV_LIB_RESULT AGGCFScaleDown8Calc( vsdk::UMat &in, int inOffset, int inW, int inH,
                        vsdk::UMat &out, int outOffset, int outW, int outH, int num, int ApexID );

   APEXCV_LIB_RESULT AGGCFScaleDown4Calc( vsdk::UMat &in, int inOffset, int inW, int inH,
                        vsdk::UMat &out, int outOffset, int outW, int outH, int num, int ApexID );

   APEXCV_LIB_RESULT AGGCFScaleDown2Calc( vsdk::UMat &in, int inOffset, int inW, int inH,
                        vsdk::UMat &out, int outOffset, int outW, int outH, int num, int ApexID );



};


}

#endif /* APEXCVPROAGGCFPD_H */

