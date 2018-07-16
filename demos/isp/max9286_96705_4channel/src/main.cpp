/*****************************************************************************
*for 1280*1080   
* Freescale Confidential Proprietary
*
* Copyright (c) 2016 Freescale Semiconductor;
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
/***************************************************************************
* 4 channel view ouput for JSDX
***************************************************************************/
#ifndef __STANDALONE__
  #include <signal.h> 
#endif // #ifdef __STANDALONE__
#include <string.h>
#include <termios.h>

#include "frame_output_v234fb.h"
#define CHNL_CNT io::IO_DATA_CH3
#include "oal.h"
#include "vdb_log.h"
#include "sdi.hpp"
#include "max9286_96705_4_c.h"
#include "vdb_log.h"
#include <pthread.h>  
#include <stdio.h> 
#include <stdlib.h>
#include "ImageStitching.h"
#include "uart_to_mcu.h"
#include "fb_helper.h"
#include "network.h"

//***************************************************************************

// Possible to set input resolution (must be supported by the DCU)
#define WIDTH           		1280 	///< width of DDR buffer in pixels
#define HEIGHT           		720 		///< height of DDR buffer in pixels
#define DDR_BUFFER_CNT  	3    	///< number of DDR buffers per ISP stream

//static uint8_t          OutPutBufferUYVY[WIDTH*HEIGHT*2];
//static uint8_t          RGB24Buffer[WIDTH*HEIGHT*3];
//***************************************************************************

/*******************************************************\u8bfb\u8868\u64cd\u4f5c**********************************************************/

//unsigned char  mem_ui[1280*720*2];
vsdk::Mat frame_map[4];
//vsdk::Mat frame_map_dis_play[4];				   //added by che
unsigned char  frame_map_display[3][1280*720*2]; //added by che
unsigned char  frame_map_displayT4[1920*1080*2];//added by che

vsdk::Mat frame_map_out;
unsigned char  mem_ui_t[1280*720*2];
unsigned char  mem_tmp_T0[1280*720*2];
unsigned char  mem_tmp_T1[1280*720*2];
unsigned char  mem_tmp_T2[1280*720*2];
unsigned char  mem_tmp_T3[1280*720*2];
unsigned char  mem_tmp_T3_tmp[1280*720*2];            //added by che
/*****************************************************************************/


UInt32_t* p_lut_front_test, *p_lut_back_test, *p_lut_left_test, *p_lut_right_test;
UInt32_t *p_wt_front_test, *p_wt_back_test, *p_wt_left_test, *p_wt_right_test;
int front_fov_height,back_fov_height,left_fov_width,right_fov_width;


Int32_t Bev_Tab_Init(UInt32_t* p_pos_fusion_lut, UInt32_t* p_wt_lut, Int32_t camid, Int32_t fov_width, Int32_t fov_height, TabBev **bev_Table[4]);

//*****************************************************************************/


#define FRM_TIME_MSR 300 ///< number of frames to measure the time and fps


#define GETTIME(time) \
  { \
  struct timeval lTime; gettimeofday(&lTime,0); \
  *time=(lTime.tv_sec*1000000+lTime.tv_usec);   \
  }


Int32_t load_config_file(CvPoint1& car_up_left,CvPoint1& car_down_right,Int32_t& front_fov_height,\
						   Int32_t& back_fov_height,Int32_t& left_fov_width,Int32_t& right_fov_width);

/************************************************************************/
/** User defined call-back function for Sequencer event handling.
  * 
  * \param  aEventType defines Sequencer event type
  * \param  apUserVal  pointer to any user defined object 
  ************************************************************************/
void SeqEventCallBack(uint32_t aEventType, void* apUserVal);

#ifndef __STANDALONE__
/************************************************************************/
/** SIGINT handler.
  * 
  * \param  aSigNo 
  ************************************************************************/
void SigintHandler(int);

/************************************************************************/
/** SIGINT handler.
  * 
  * \param  aSigNo 
  * 
  * \return SEQ_LIB_SUCCESS if all ok
  *         SEQ_LIB_FAILURE if failed
  ************************************************************************/
int32_t SigintSetup(void);

//***************************************************************************

static bool sStop = false; ///< to signal Ctrl+c from command line

#endif // #ifndef __STANDALONE__
/************************************************************************/
/** Allocates contiguous DDR buffers for one ISP stream.
  * 
  * \param  appFbsVirtual array of virtual buffer pointers to be filled
  * \param  apFbsPhysical array of buffer physical addresses to be filled
  * \param  aMemSize      size of the array in bytes
  * \param  aBufferCnt    number of buffers to be allocated
  * 
  * \return 0 if all OK
  *         < 0 otherwise
  ************************************************************************/

void *VideoCaptureTask(void * ptr);
void *DisplayTaskHD(void * ptr);

  SDI_Frame lFrame[4];
  uint32_t actualBufferIndex = 0;

  uint8_t get_frame_map_flag = 0;

/************************************************************************/
/** Frees DDR buffers for one ISP stream.
  * 
  * \param  appFbsVirtual array of virtual buffer pointers to be filled
  * \param  apFbsPhysical array of buffer physical addresses to be filled
  * \param  aBufferCnt    number of buffers to be freed
  ************************************************************************/
void DdrBuffersFree(void** appFbsVirtual, uint32_t*  apFbsPhysical,  uint32_t aBufferCnt);
int main(int, char **)  //zhy 
{	
	int ret = 0;
	int a =100;
	pthread_t id1,id2,id3,id4,id5,id6,id7,id8,id9,id10;
	pthread_t  net_staus_thread,tr_thread,rv_thread;
	ret = pthread_create(&id1, NULL, VideoCaptureTask,NULL);
	if(ret)
	{
		printf("Create VideoCaptureTask error!\n");
		return 1;
	}

	ret = pthread_create(&id2, NULL, DisplayTaskHD,NULL);
	if(ret)
	{
		printf("Create DisplayTask error!\n");
		return 1;
	}
#if 0
	ret = pthread_create(&id3, NULL, CANTXTask,NULL);
	if(ret)
	{
		printf("Create DisplayTask error!\n");
		return 1;
	}
	ret = pthread_create(&id4, NULL, CANRXTask,NULL);
	if(ret)
	{
		printf("Create DisplayTask error!\n");
		return 1;
	}
#endif
	ret = pthread_create(&id5, NULL, KeyTask,NULL);
	if(ret)
	{
		printf("Create keytask error!\n");
		return 1;
	}

	ret = pthread_create(&id6, NULL, TerminalTask,NULL);
	if(ret)
	{
		printf("Create TerminalTask error!\n");
		return 1;
	}
#if 0	
	ret = pthread_create(&id9, NULL, Gui_meg_thread,NULL);
	if(ret)	
	{		
		printf("Create Gui_meg_thread error!\n");		
		return 1;	
	}
	ret = pthread_create(&id10, NULL, Gui_draw_thread,NULL);
	if(ret)	
	{		
		printf("Create Gui_draw_thread error!\n");		
		return 1;	
	}
#endif

	ret = pthread_create(&id7, NULL, Uart_meg_thread,NULL);
	if(ret)
	{		
		printf("Create Uart_meg_thread error!\n");
		return 1;
	}
	ret = pthread_create(&id8, NULL, Uart_TX_thread,NULL);
	if(ret)	
	{		
		printf("Create Uart_TX_thread error!\n");		
		return 1;	
	}

	//ret = pthread_create(&net_staus_thread,NULL,net_status_check,NULL); 
	//if(ret)	
	//{		
	//	printf("Create net_status_check error!\n");		
	//	return 1;	
	//}
	ret = pthread_create(&tr_thread, NULL, net_tr_thread, NULL);	
	if(ret)	
	{		
		printf("Create net_tr_thread error!\n");		
		return 1;	
	}
	//ret = pthread_create(&rv_thread, NULL, net_rv_thread,NULL); 
	//if(ret)	
	//{		
	//	printf("Create net_rv_thread error!\n");		
	//	return 1;	
	//}

	
	pthread_join(id1,NULL);
	pthread_join(id2,NULL);
	///pthread_join(id3,NULL);
	///pthread_join(id4,NULL);
	pthread_join(id5,NULL);
	pthread_join(id6,NULL);
	pthread_join(id7,NULL);
	pthread_join(id8,NULL);
	pthread_join(id9,NULL);
	pthread_join(id10,NULL);
	
	return 0;

}
void *VideoCaptureTask(void *ptr1)  //zhy  
{
	int i;
	LIB_RESULT lRet = LIB_SUCCESS;
	LIB_RESULT lRes = LIB_SUCCESS; 
	///  char ch;
	OAL_Initialize(); 
	//*** Init DCU Output ***  
	// setup Ctrl+C handler
	if(SigintSetup() != SEQ_LIB_SUCCESS) 
	{
	VDB_LOG_ERROR("Failed to register Ctrl+C signal handler.");
	return    0 ;// -1;
	}  
	printf("Press Ctrl+C to terminate the demo.\n");  

	/*** set terminal to nonblock input ***/
	//TermNonBlockSet();

	io::FrameOutputV234Fb
	lDcuOutput(1920,1080,io::IO_DATA_DEPTH_08, io::IO_DATA_CH3,DCU_BPP_YCbCr422);
	printf("main.cpp line135\n");
	ReadOriginalImage();  //
	ReadLut(); 	
	// *** Initialize SDI ***
	lRes = sdi::Initialize(0); //no use ,may mask
	printf("main.cpp line140\n");
	// *** create grabber ***
	sdi_grabber *lpGrabber = new(sdi_grabber);  // 
	lpGrabber->ProcessSet(gpGraph, &gGraphMetadata);
	// *** set user defined Sequencer event handler *** 
	int32_t lCallbackUserData = 12345;
	lpGrabber->SeqEventCallBackInstall(&SeqEventCallBack, &lCallbackUserData);
	// *** prepare IOs ***
	sdi_FdmaIO *lpFdma = (sdi_FdmaIO*)lpGrabber->IoGet(SEQ_OTHRIX_FDMA);   
	// modify DDR frame geometry to fit display output
	SDI_ImageDescriptor lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);//1280*720
	lpFdma->DdrBufferDescSet(0, lFrmDesc);  
	lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);
	lpFdma->DdrBufferDescSet(1, lFrmDesc);  
	lFrmDesc = SDI_ImageDescriptor(WIDTH, HEIGHT, YUV422Stream_UYVY);
	lpFdma->DdrBufferDescSet(2, lFrmDesc);
	lFrmDesc = SDI_ImageDescriptor(1920 ,1080, YUV422Stream_UYVY); // wyf changed 2017.11.29
	lpFdma->DdrBufferDescSet(3, lFrmDesc); 
	//*** allocate DDR buffers ***
	lpFdma->DdrBuffersAlloc(DDR_BUFFER_CNT);
	load_config_file(car_up_left,car_down_right,front_fov_height,back_fov_height,left_fov_width,right_fov_width);	
	Bev_Tab_Init(p_lut_front_test, p_wt_front_test, 0, DST_WIDTH, car_up_left.y , bev_Table);
	Bev_Tab_Init(p_lut_back_test, p_wt_back_test, 1, DST_WIDTH, car_up_left.y , bev_Table);
	Bev_Tab_Init(p_lut_left_test, p_wt_left_test, 2, car_up_left.x , DST_HEIGHT, bev_Table);
	Bev_Tab_Init(p_lut_right_test, p_wt_right_test, 3, car_up_left.x , DST_HEIGHT, bev_Table);  
	// *** prestart grabber ***
	lpGrabber->PreStart();
	// fetched frame buffer storage
	// *** start grabbing ***
	lpGrabber->Start();  
	unsigned long lTimeStart1 = 0, lTimeEnd1 = 0, lTimeDiff1 = 0;
	uint32_t lFrmCnt = 0;
	uint8_t lLoop=0; 
	GETTIME(&lTimeStart1);
  for(;;)
  {   
      // pop all  
	 for (int i = 0;i < 4;i++)   
		{
   			 lFrame[i] = lpGrabber->FramePop(i);			 
   			/*IFrame[0] -->T1 ,IFrame[1] -->T2 ,IFrame[2] -->T3 ,IFrame[3] -->T4 ****wyf added 2017.11.29**/
   		//	 if(lFrame[i].mUMat.empty())
	     	//	 {
	       //		printf("Failed to grab image number %u\n", lFrmCnt);
	        		///break;
	      	///	} // if pop failed
		}  	 
    //****************************wyf added 2017.11.29*******************************
	for (int i = 0;i < 4;i++) {
	frame_map[i] = lFrame[i].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED);	
	}
	//***************************************************************
	if(get_frame_map_flag==0)      //added by che
	{
		for(int i =0;i<3;i++)
		{
			memcpy((uint8_t *)&frame_map_display[i][0]	, (char *)frame_map[i].data, 1280*720*2);	//frame_map_display[3][1280*720*2]; 
		}
		memcpy((uint8_t *)frame_map_displayT4	, (char *)frame_map[3].data, 1920*1080*2);	//frame_map_display[3][1280*720*2]; 
                frame_map_display[0][0] = 0x00;
                frame_map_display[0][1] = 0x00;
                frame_map_display[2][0] = 0xff;
                frame_map_display[2][1] = 0xff;
	}
	
    if (lLoop < 30)
       actualBufferIndex = 0;
    else if(lLoop < 60)
       actualBufferIndex = 1;
    else if(lLoop < 90)
       actualBufferIndex = 2;
    else if(lLoop < 120)
       actualBufferIndex = 3;
    else if(lLoop == 120)
    {
       actualBufferIndex = 3;     
       lLoop = 0;
    }
	lLoop++; 	
	/// console_cmd = 8;///2   //modified by che
	switch(console_cmd)//
	{	
		 case 0 :	//snap of original view
			for(int i =0;i<720;i++)
			memcpy((char*)mem_tmp_T3+i*1280*2,(char *)frame_map[3].data+1920*2*i,1280*2);  // 1920*1080  conver 1280*720
			frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 			
			if(channel_select == 0)
			 	memcpy((char*)SINGLE_BUFFER, (char *) frame_map[0].data, 1280*720*2);//for snap
			else if( channel_select  == 1)
				memcpy((char*)SINGLE_BUFFER, (char *) frame_map[1].data, 1280*720*2);//for snap
			else if(channel_select == 2) 
				memcpy((char*)SINGLE_BUFFER, (char *) frame_map[2].data, 1280*720*2);//for snap
			else if( channel_select  == 3)  
				memcpy((char*)SINGLE_BUFFER, (char *) mem_tmp_T3, 1280*720*2);//for snap
			  DisSnapOriginalVideocap(channel_select);
			break;
		case 1 ://single view resize of video
			for(int i =0;i<720;i++)
			memcpy((char*)mem_tmp_T3+i*1280*2,(char *)frame_map[3].data+1920*2*i,1280*2);  // 1920*1080  conver 1280*720
			frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 			
			///  GETTIME(&lTimeStart2);  
			 DisUndistortionSingleVideo(channel_select,(char *) frame_map_out.data);
			///  GETTIME(&lTimeEnd2);  
  			///  printf("%d\n",lTimeEnd2 - lTimeStart2);
		       break;			
		 case 2://video svm
				memcpy((char*)mem_tmp_T0,(char *)frame_map[0].data,1280*720*2);  // 1920*1080  conver 1280*720
				memcpy((char*)mem_tmp_T1,(char *)frame_map[1].data,1280*720*2);  // 1920*1080  conver 1280*720
				memcpy((char*)mem_tmp_T2,(char *)frame_map[2].data,1280*720*2);  // 1920*1080  conver 1280*720
				for(int i =0;i<720;i++)
					memcpy((char*)mem_tmp_T3+i*1280*2,(char *)frame_map[3].data+1920*2*i,1280*2);  // 1920*1080  conver 1280*720

				frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 
				memset((char *) frame_map_out.data,0,1920*1080*2);	
				bev_process(
				SVM_BUFFER,//result_image_uyvy,
				mem_tmp_T0, //frame_map[0].data,//front_p,//front_image_uyvy,
				mem_tmp_T1, //frame_map[1].data,//back_p,//back_image_uyvy,
				mem_tmp_T2, //frame_map[2].data,//left_p,//left_image_uyvy,
				mem_tmp_T3,//right_p,//right_image_uyvy,
				car_up_left,
				car_down_right,
				front_fov_height,
				right_fov_width,
				DST_WIDTH,
				SIG_WIDTH,//SINGLE_VIEW_WIDTH,
				bev_Table);	

				buf_camfront = (unsigned char *)frame_map[0].data;
				buf_camback  = (unsigned char *)frame_map[1].data;
				buf_camleft  = (unsigned char *)frame_map[2].data;
				buf_camright = mem_tmp_T3;
			#if 0
			      for(i=0;i<1024;i++) //i = (1080-1056)/2;i<12+1056=1068
				{//128 = (1920-DST_HEIGHT)/2
					memcpy((char *) frame_map_out.data+i*1920*2+128*2,(char*)SVM_BUFFER+(i)*DST_WIDTH*2, DST_WIDTH*2);
				}			 
			#else
				rotate270(SVM_BUFFER,SVM_BUFFER_dst,DST_WIDTH,DST_HEIGHT);
			       for(i=0;i<1080;i++) //i = (1080-1056)/2;i<12+1056=1068
				{//128 = (1920-DST_HEIGHT)/2
					memcpy((char *) frame_map_out.data+i*1920*2+256*2,(char*)SVM_BUFFER_dst+(i)*DST_HEIGHT*2, DST_HEIGHT*2);
				}			 
				//for(i=124;i<956;i++) //i =124 = (1080-832)/2;i<124+832=956;
				//{//448 = (1920-1024)/2
				//	memcpy((char *) frame_map_out.data+i*1920*2+448*2,(char*)SVM_BUFFER_dst+(i-124)*1024*2, 1024*2);
				//}
				#endif
			break;
		case 3: //display static picture
			frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 			
			DisOriginalSingleViewPic(channel_select,(char *) frame_map_out.data);
			break;
		case 4://display static picture resize
			frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 			
			DisUndistortionSingleViewPic(channel_select,(char *) frame_map_out.data);
			break;
		case 5://pic svm
			frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 
				

			memset((char *) frame_map_out.data,0,1920*1080*2);	
		      bev_process(
			SVM_BUFFER,//result_image_uyvy,
			front_p,//front_image_uyvy,
			back_p,//back_image_uyvy,
			left_p,//left_image_uyvy,
			right_p,//right_image_uyvy,
			car_up_left,
			car_down_right,
			front_fov_height,
			right_fov_width,
			DST_WIDTH,//Width,
			SIG_WIDTH,//SINGLE_VIEW_WIDTH,
			bev_Table);
			  #if 0

			for(i=0;i<1080;i++) //i = (1080-1056)/2;i<12+1056=1068
			{//128 = (1920-DST_HEIGHT)/2
				memcpy((char *) frame_map_out.data+i*1920*2,(char*)SVM_BUFFER+(i)*DST_WIDTH*2, DST_WIDTH*2);
			}			 
			///  printf("rrrr init done.\n");
			  #else  

			rotate270(SVM_BUFFER,SVM_BUFFER_dst,DST_WIDTH,DST_HEIGHT);
			for(i=0;i<1080;i++) //i = (1080-1056)/2;i<12+1056=1068
			{//128 = (1920-DST_HEIGHT)/2
				memcpy((char *) frame_map_out.data+i*1920*2+256*2,(char*)SVM_BUFFER_dst+(i)*DST_HEIGHT*2, DST_HEIGHT*2);
			}			 

			 //for(i=124;i<956;i++) 
			//{
			//	memcpy((char *) frame_map_out.data+i*1920*2+448*2,(char*)SVM_BUFFER_dst+(i-124)*1024*2, 1024*2);
			//}	
			 #endif
			
			break;
			 case 6:
				for(int i =0;i<720;i++)		
					memcpy((char*)mem_tmp_T3+i*1280*2,(char *)frame_map[3].data+1920*2*i,1280*2);  // 1920*1080  conver 1280*720		
					frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 		
					Show4videoT();		
					actualBufferIndex =3;		
					break;
			case 7:
			frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 
			memset((char *) frame_map_out.data,0,1920*1080*2);	
		case 8:				//added by che
			
				for(int i =0;i<720;i++)
					memcpy((uint8_t *)mem_tmp_T3_tmp+i*1280*2,(uint8_t *)frame_map_displayT4+1920*2*i,1280*2);  // 1920*1080  conver 1280*720

					
				frame_map_out =lFrame[3].mUMat.getMat(vsdk::ACCESS_RW | OAL_USAGE_CACHED); 
				memset((char *) frame_map_out.data,0,1920*1080*2);				

#if 1
			if(get_frame_map_flag==0)//<100)//==0)
			{
			
				for(i=0;i<720;i++)  
					memcpy( frame_map_out.data+i*1920*2, (uint8_t *)&frame_map_display[0][0]+1280*2*i, 1280*2);
				for(i=720;i<1080;i++)  //lines
					memcpy( frame_map_out.data+i*1920*2, (uint8_t *)&frame_map_display[1][0]+1280*2*(i-720), 1280*2);//columns
													
				for(i=0;i<360;i++)  
					memcpy( frame_map_out.data+i*1920*2+1280*2, (uint8_t *)&frame_map_display[1][0]+1280*2*360+i*1280*2, 640*2);
				for(i=360;i<720;i++)  
					memcpy( frame_map_out.data+i*1920*2+1280*2, (uint8_t *)&frame_map_display[1][0]+1280*2*360+640*2+(i-360)*1280*2, 640*2);
				// to be deleted
				//for(i=720;i<1080;i++)  
				//	memcpy( frame_map_out.data+i*1920*2+1280*2, frame_map_dis_play[1].data+1280*2*360+640*2+(i-720)*1280*2, 640*2);
#if 1				
				get_frame_map_flag=1;//++;// = 1 ;
#endif
			}
#if 1			
			else if(get_frame_map_flag==1)//>=100)//==1)
#endif				
#endif				
			{
#if 1				
				for(i=0;i<720;i++)  
					memcpy( frame_map_out.data+i*1920*2,(uint8_t *)&frame_map_display[2][0]+1280*2*i, 1280*2);
				//memcpy( frame_map_out.data+i*1920*2,mem_tmp_T3_tmp+1280*2*i, 1280*2);
				
				for(i=720;i<1080;i++)  //lines
					memcpy( frame_map_out.data+i*1920*2, mem_tmp_T3_tmp+1280*2*(i-720), 1280*2);//columns
													
				for(i=0;i<360;i++)  
					memcpy( frame_map_out.data+i*1920*2+1280*2, mem_tmp_T3_tmp+1280*2*360+i*1280*2, 640*2);
				
				for(i=360;i<720;i++)  
					memcpy( frame_map_out.data+i*1920*2+1280*2,mem_tmp_T3_tmp+1280*2*360+640*2+(i-360)*1280*2, 640*2);
				// to be deleted
				//for(i=720;i<1080;i++)  
					//memcpy( frame_map_out.data+i*1920*2+1280*2, mem_tmp_T3_tmp+1280*2*360+640*2+(i-720)*1280*2, 640*2);
#if 1				
				get_frame_map_flag=0;//++;//=0;
				//if(get_frame_map_flag>=200)
					//get_frame_map_flag=0;
#endif				
#endif
			}
			break;
		case 9:				//added by che

			
			break;
																				
				
	default:break;		   	
      	}	 
//***********DY**********************//	
actualBufferIndex =3;
// push all
    for (int i = 0;i < 4;i++)  //*******;************DY****************
    { 
		if(lpGrabber->FramePush(lFrame[i]) != LIB_SUCCESS)
		{
		 printf("lpGrabber->FramePush(lFrame[i]) %d\n", i);
		  break;
		} // if push failed
    } 
	
    lDcuOutput.PutFrame(lFrame[actualBufferIndex].mUMat); 
    if((lFrmCnt%FRM_TIME_MSR) == 0)
    {
      GETTIME(&lTimeEnd1);
      lTimeDiff1= lTimeEnd1-lTimeStart1;
      lTimeStart1 = lTimeEnd1;      
      printf("%u intesight frames took %lu usec (%5.2ffps)\n",FRM_TIME_MSR,lTimeDiff1,(FRM_TIME_MSR*1000000.0)/((float)lTimeDiff1));
    }// if time should be measured
    
    lFrmCnt++;
	
#ifndef __STANDALONE__  
    if(sStop)
    {   
      break; // break if Ctrl+C pressed
    } // if Ctrl+C
#endif //#ifndef __STANDALONE__  
  } // for ever
  
  if(lpGrabber)
  {
    //*** Stop ISP processing ***
    lpGrabber->Stop();    
    // clean up grabber resources
    lpGrabber->Release();
    
    delete(lpGrabber);
  } // if grabber exists
  
  //
  // Close SDI  

  lRes = sdi::Close(0);
  if(lRes != LIB_SUCCESS)
  {
    lRet = LIB_FAILURE;
  } // if lRes not LIB_SUCCESS
  return  0;// lRet;

} // main()


void *DisplayTaskHD(void  *ptr1)//(void)
{
 
  ///  int i,j;
///	j = 0;
    ptr1 = NULL;
    printf("*************DisplayTask************\n"); 
	sleep(10);
					
}




int32_t DdrBuffersAlloc(void** appFbsVirtual, uint32_t*  apFbsPhysical, size_t aMemSize, uint32_t aBufferCnt)
{
  int32_t lRet    = 0;
  
  // allocate buffers & get physical addresses
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    appFbsVirtual[i] = OAL_MemoryAllocFlag( aMemSize,OAL_MEMORY_FLAG_ALIGN(ALIGN2_CACHELINE)|OAL_MEMORY_FLAG_CONTIGUOUS);    
    if( appFbsVirtual[i] == NULL)
    {
      lRet = -1;
      break;
    }
    apFbsPhysical[i] = (uint32_t)(uintptr_t)OAL_MemoryReturnAddress(appFbsVirtual[i],  ACCESS_PHY); // get physical address
    memset(appFbsVirtual[i], 0x00, aMemSize);
  } // for all framebuffers
  
  if(lRet != 0)
  {
    DdrBuffersFree(appFbsVirtual, apFbsPhysical, aBufferCnt);
  }
  
  return lRet;
} // DdrBuffersAlloc()

//***************************************************************************

void DdrBuffersFree(void** appFbsVirtual,  uint32_t*  apFbsPhysical,  uint32_t aBufferCnt    )
{
  for(uint32_t i = 0; i < aBufferCnt; i++)
  {
    if(appFbsVirtual[i] != NULL)
    {
      OAL_MemoryFree(appFbsVirtual[i]);
    } // if buffer allocated
    
    appFbsVirtual[i]   = NULL;
    apFbsPhysical[i]   = 0;
  } // for all framebuffers
} // DdrBuffersFree()



//***************************************************************************

void SeqEventCallBack(uint32_t aEventType, void* apUserVal)
{
  static uint32_t slFrmCnt = 0;   

  if(aEventType == SEQ_MSG_TYPE_FRAMEDONE)
  {
  ///  printf("Frame done message arrived #%u. User value %d\n", 
  ///         slFrmCnt++,
   ///       *((int32_t*)apUserVal));
  } // if frame done arrived
} // SeqEventCallBack()

//***************************************************************************


#ifndef __STANDALONE__
void SigintHandler(int)
{
  sStop = true;
} // SigintHandler()

//***************************************************************************

int32_t SigintSetup()
{
  int32_t lRet = SEQ_LIB_SUCCESS;
  
  // prepare internal signal handler
  struct sigaction lSa;
  memset(&lSa, 0, sizeof(lSa));
  lSa.sa_handler = SigintHandler;
  
  if( sigaction(SIGINT, &lSa, NULL) != 0)
  {
    VDB_LOG_ERROR("Failed to register signal handler.\n");
    lRet = SEQ_LIB_FAILURE;
  } // if signal not registered
  
  return lRet;
} // SigintSetup()

//***************************************************************************
#endif // #ifndef __STANDALONE__

void DisSnapOriginalVideocap(int num)
{
	int  i = 0;
	unsigned char* p_source = NULL;
	memset((char *) frame_map_out.data,0,1920*1080*2);
     	switch(num)
     	{
            case  0:    
			p_source = 	 frame_map[0].data;
		     break;
            case  1:
			p_source = 	frame_map[1].data;
		     break;
            case  2:
			p_source = 	 frame_map[2].data;
		     break;
            case  3:
			p_source = 	 mem_tmp_T3;
		     break;
			 default:break;
	}
	for(i=0;i<720;i++)  
	memcpy( frame_map_out.data+i*1920*2,p_source+1280*2*i, 1280*2);
		
}


void Show4videoLoop(int num)
{
	int  i = 0;		
	memset((char *) frame_map_out.data,0,1920*1080*2);
     	switch(num)
     	{
            case  0:
			memcpy((char*)mem_ui_t, (char *) frame_map[0].data, 1280*720*2);
			for(i=0;i<720;i++)  
				memcpy((char *) frame_map_out.data+i*1920*2,(char*)mem_ui_t+1280*2*i, 1280*2);			
		     break;
            case  1:
			memcpy((char*)mem_ui_t, (char *) frame_map[1].data, 1280*720*2);
			for(i=0;i<720;i++)  
				memcpy((char *) frame_map_out.data+i*1920*2,(char*)mem_ui_t+1280*2*i, 1280*2);
		     break;
            case  2:
			memcpy((char*)mem_ui_t, (char *) frame_map[2].data, 1280*720*2);
			for(i=0;i<720;i++)  
				memcpy((char *) frame_map_out.data+i*1920*2,(char*)mem_ui_t+1280*2*i, 1280*2);
		     break;
            case  3:
			memcpy((char*)mem_ui_t, (char *) mem_tmp_T3, 1280*720*2);
			for(i=0;i<720;i++)  
				memcpy((char *) frame_map_out.data+i*1920*2,(char*)mem_ui_t+1280*2*i, 1280*2);
		     break;
			 default:break;
	 }
}

void Show4videoT(void)
{    
	int  i = 0;	
	memset((char *) frame_map_out.data,0,4147200);
	for(i=0;i<540;i++) 
		memcpy((char *) frame_map_out.data+i*3840,(char*) frame_map[0].data +2560*(i), 1920);
	for(i=0;i<540;i++)  
	 	memcpy((char *) frame_map_out.data+i*3840+1920,(char*) frame_map[1].data+2560*(i), 1920);
	for(i=0;i<540;i++)  
		memcpy((char *) frame_map_out.data+(i+539)*3840,(char*)frame_map[2].data+1280*2*(i), 960*2);
	for(i=0;i<540;i++)  
		memcpy((char *) frame_map_out.data+(i+539)*3840+1920,(char*)mem_tmp_T3+1280*2*(i), 960*2);
}


void DisUndistortionSingleVideo(int num,char* dis_buf)
{
	int  i = 0;		
	unsigned char* p_source =NULL;
	memset((char *) dis_buf,0,1920*1080*2);
     	switch(num)
     	{
		case  0:   
				p_source  = (unsigned char* )frame_map[0].data;
		break;
		case  1:
				p_source  = (unsigned char* )frame_map[1].data;
		break;
		case  2:
				p_source  = (unsigned char* )frame_map[2].data;
		break;
		case  3:
				p_source  = (unsigned char* )mem_tmp_T3;
		break;
		default:		break;
			 
	}
	UndistortionSVProccess(Lut_Fsv_View, mem_ui_t,p_source,720,1280,720,1280 );
	for(i=0;i<720;i++)  
	memcpy((char *) dis_buf+i*1920*2,(char*)mem_ui_t+1280*2*i, 1280*2);
}



Int32_t Bev_Tab_Init(UInt32_t* p_pos_fusion_lut, UInt32_t* p_wt_lut, Int32_t camid, Int32_t fov_width, Int32_t fov_height, TabBev **bev_Table[4])
{
///	Int32_t wt_upleft, wt_upright, wt_downleft, wt_downright;
	UInt32_t item, wt_item;
       Int32_t i,j ;
	bev_Table[camid] = (TabBev**)malloc(sizeof(TabBev*)* fov_height);
	for ( i = 0; i < fov_height; i++)
	{
		TabBev* p = (TabBev*)malloc(sizeof(TabBev)* fov_width);
		bev_Table[camid][i] = p;
	}
	if (bev_Table == NULL)
	{
		printf("malloc error");
	}
	#if 1
	for ( i = 0; i < fov_height; i++)
	{
		for ( j = 0; j < fov_width; j++)
		{
			item = p_pos_fusion_lut[i * fov_width + j];
			bev_Table[camid][i][j].point_pos.x = (item >> 10) & 0x7FF;
			bev_Table[camid][i][j].point_pos.y = (item >> 21);
			bev_Table[camid][i][j].wt_fusion = item & 0x3FF;

			wt_item = p_wt_lut[2 * i * fov_width + 2 * j];
			bev_Table[camid][i][j].wt_upleft = wt_item >> 16;
			bev_Table[camid][i][j].wt_upright = wt_item & 0xFFFF;
			wt_item = p_wt_lut[2 * i * fov_width + 2 * j + 1];
			bev_Table[camid][i][j].wt_downleft = wt_item >> 16;
			bev_Table[camid][i][j].wt_downright = wt_item & 0xFFFF;
			
		}
	}
	#endif
	return 0;
}
Int32_t load_config_file(CvPoint1& car_up_left,CvPoint1& car_down_right,Int32_t& front_fov_height,\
						   Int32_t& back_fov_height,	Int32_t& left_fov_width,Int32_t& right_fov_width)
{
	Int32_t ret = 0;
	char str[256];
	FILE* p_config_file = fopen("/data/panorama_config.txt", "rt");
	fgets(str, 256, p_config_file);
	fscanf(p_config_file, "%d\n", &car_up_left.y);
	fscanf(p_config_file, "%d\n", &car_up_left.x);
	fscanf(p_config_file, "%d\n", &car_down_right.y);
	fscanf(p_config_file, "%d\n", &car_down_right.x);

	fgets(str, 256, p_config_file);
	fscanf(p_config_file, "%d\n", &front_fov_height);
	fscanf(p_config_file, "%d\n", &back_fov_height);
	fscanf(p_config_file, "%d\n", &left_fov_width);
	fscanf(p_config_file, "%d\n", &right_fov_width);

	fclose(p_config_file);

	printf("car_up_left.x= %d\n", car_up_left.x);
	printf("car_up_left.y= %d\n", car_up_left.y);
	printf("car_down_right.x= %d\n", car_down_right.x);
	printf("car_down_right.y= %d\n", car_down_right.y);
	printf("front_fov_height= %d\n", front_fov_height);
	printf("back_fov_height= %d\n", back_fov_height);
	printf("left_fov_width= %d\n", left_fov_width);
	printf("right_fov_width= %d\n", right_fov_width);

	p_lut_front_test=Lut_Front;
	p_wt_front_test = Wt_Lut_Front;
	p_lut_back_test=Lut_Back;
	p_wt_back_test = (UInt32_t *)Wt_Lut_Back;
	p_lut_left_test=Lut_Left;
	p_wt_left_test = Wt_Lut_Left;
	p_lut_right_test=Lut_Right;
	p_wt_right_test = (UInt32_t *)Wt_Lut_Right;

	/// 	car_up_left.x = 460;//448;//464;//352;
	///	car_up_left.y = 672;//672;//655;//396;
	///	car_down_right.x = 619;//631;// 591;//479;
	///	car_down_right.y = 991;//991;//1008;//627;
	///	front_fov_height = 672;//655;//396;
	///	back_fov_height = 672;//655;//396;
	///	left_fov_width = 460;//448;//464;//352;
	///	right_fov_width =460;//448;//464;//352;		
	return ret;
}
