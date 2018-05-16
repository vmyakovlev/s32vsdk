/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2013 Freescale Semiconductor;
* All Rights Reserved
*
*****************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
****************************************************************************/
//#include "stdafx.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <map>
#include <utility>
#include <list>
#include <math.h>
#include <stdlib.h>
#include <string>
using namespace std;

 
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


//Camera and Input Img includes
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
using namespace cv;

#include "displayData.h"


#ifdef DEF_TIME_MEASURE
extern uint64_t imgWriteTicks;
#endif

#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)
// not working 1024x768, 1600x1200, 1280x1024,  1280 × 800 WXGA
// working 512x512, 800x600, 1280x720
const int imw = 1280/*800*/, imh = 720/*600*/, crtSz = imw*imh * 3; //1440 × 900 WXGA+, 1680 × 1050 WSXGA+, and 1920 × 1200 
#ifdef __STANDALONE__
io::FrameOutputDCU output(imw, imh, io::IO_DATA_DEPTH_08, CHNL_CNT);
#else   
io::FrameOutputV234Fb output(imw, imh, io::IO_DATA_DEPTH_08, CHNL_CNT);
#endif

void *lp_buffer = NULL;
cv::Mat output_mat;


void InitDisplay() {
	lp_buffer = OAL_MemoryAllocFlag(crtSz, OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE) | OAL_MEMORY_FLAG_CONTIGUOUS);
	memset(lp_buffer, 0, crtSz);
	output_mat = cv::Mat(imh, imw, CV_8UC3, lp_buffer);
}

void ReleaseDisplay() {
	OAL_MemoryFree(lp_buffer);
}

void showTargetImg(cv::Mat& out){
	cv::Mat tmp = out.clone();
	//////////////////////////////////////////////////
	// Process the output - if standalone, compare with reference
	//                    - if OS, save the images

	if (tmp.channels() == 1) {
		cv::Mat tmp1(tmp.size(), CV_8UC3);
		cv::cvtColor(tmp, tmp1, CV_GRAY2BGR);
		tmp = tmp1;
	}
	
	//cv::Mat rot_mat = getRotationMatrix2D(cv::Point2f(640, 360), 180, 1.0);
	cv::Mat tmp1;
	double wRsz = ((double)imw )/ tmp.cols, hRsz = ((double)imh )/ tmp.rows, rszFact = hRsz< wRsz ? hRsz: wRsz;
	if (rszFact < 1.0) {
		cv::resize(tmp, tmp1, cv::Size(0, 0), rszFact, rszFact, cv::INTER_NEAREST);
	} else {
		tmp1 = tmp;
	}

	//printf("before roi, tmp1 %d %d, out_mat %d %d\n", tmp1.cols, tmp1.rows, output_mat.cols, output_mat.rows);

	//if (imh > tmp.rows)
		//tmp.copyTo(output_mat(cv::Rect(0, (imh-tmp.rows)/2, tmp.cols, tmp.rows)));
	//else
		tmp1.copyTo(output_mat(cv::Rect(0, 0, tmp1.cols, tmp1.rows)));
	//printf("after roi\n");


	//warpAffine(output_mat, output_mat, rot_mat, output_mat.size());
	output.PutFrame(output_mat.data);
	//OAL_MemoryFree(lp_buffer);
}
#endif



void showHistogram(DataDescriptor* hist, int histSz) {
	unsigned int *histVec = (unsigned int*)hist->GetDataPtr();
	const unsigned int histImageHeight = 128;
	unsigned int histMax = 0;

	for (int i = 0; i < histSz; ++i)
	{
		if (histVec[i] > histMax)
		{
			histMax = histVec[i];
		}
	}

	Mat out(histImageHeight, histSz, CV_8UC1);

	for (unsigned int i = 0; i < histSz; ++i)
	{
		float ratio = histVec[i] / float(histMax);

		unsigned int columnHeight = (unsigned int)(ratio * histImageHeight + .5f);
		for (unsigned int y = 0; y < histImageHeight; ++y)
		{
			uint8_t value = y < columnHeight ? 255 : 0;
			out.at<uint8_t>(histImageHeight - y - 1, i) = value;
		}
	}

	/* OpenCV sample program for histogram display
	int scale = 10;
	Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

	for( int h = 0; h < hbins; h++ )
		for( int s = 0; s < sbins; s++ )
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal*255/maxVal);
			rectangle( histImg, Point(h*scale, s*scale),
			Point( (h+1)*scale - 1, (s+1)*scale - 1),
			Scalar::all(intensity),
			CV_FILLED );
		}


	*/

	imshow("Histogram", out);

}

void showResults(DataDescriptor* Out) {
	Mat cvOut;
	if (Out->GetElementDimX() > 1) {
		cvOut = cv::Mat(Out->GetHeight(), Out->GetWidth(), CV_8UC3, Out->GetDataPtr());
	} else {
		cvOut = cv::Mat(Out->GetHeight(), Out->GetWidth(), CV_8UC1, Out->GetDataPtr());
	}

#if !defined(APEX2_EMULATE) && !defined(__INTEGRITY__)
	showTargetImg(cvOut);
#else
	imshow("Out", cvOut);
	waitKey();
#endif


}

//#endif
