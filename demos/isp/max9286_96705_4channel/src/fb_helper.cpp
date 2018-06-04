/*********************************************************************
* Copyright (C) 2014-2016 by Freescale, Inc.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
 *********************************************************************/

#include "fb_helper.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <iostream>
using namespace std;
using namespace cv;


void readbmp();
void layout_alarm_area(int radar_id,int alarm_level);
void layout_alarm_area1( );

const char *fb_name[FB_NUM_LAYERS] = {
	"/dev/fb0",
	"/dev/fb1",
	"/dev/fb2",
	"/dev/fb3",
	"/dev/fb4",
	"/dev/fb5",
	"/dev/fb6",
	"/dev/fb7"
};

const char *dcu_name[DCU_NUM] =
{
	"/dev/dcu0"
};

/* IOCTL API used by Vivante GPU drivers */
#define MXCFB_SET_PREFETCH	_IOW('F', 0x30, int)
#define MXCFB_GET_PREFETCH	_IOR('F', 0x31, int)

/*  from v4l headers */
#define v4l2_fourcc(a, b, c, d)\
	((__u32)(a) | ((__u32)(b) << 8) | ((__u32)(c) << 16) | ((__u32)(d) << 24))

/* 32bit single buf 4x4 standard */
#define IPU_PIX_FMT_GPU32_ST     v4l2_fourcc('5', 'I', '4', 'S')

char *fb_ptr[FB_NUM_LAYERS];
int fb_fd[FB_NUM_LAYERS];
int dcu_fd;
int fb_screensize[FB_NUM_LAYERS];

char view_mem[1920*1080*4];
char view_mem_temp[1920*1080*4];
char logo_mode_right[240*1080*4];
char logo_mode_temp[240*1080*4];
char logo_mode_left[240*1080*4];

char logo_park_status_temp[240*240*4];
char logo_park_status_scan[240*240*4];
char logo_park_status_ready[240*240*4];
char logo_park_status_run[240*240*4];
char logo_park_status_stop[240*240*4];
char logo_park_status_fail[240*240*4];

char logo_gear_d[240*1080*4];
char logo_gear_r[240*1080*4];
char logo_gear_temp[240*1080*4];

int gui_draw_ok_flag;


////////////////////////////////////////////////////////////
extern int rader1_alarm_level;
extern int rader2_alarm_level;
extern int rader3_alarm_level;
extern int rader4_alarm_level;
extern int rader5_alarm_level;
extern int rader6_alarm_level;
extern int rader7_alarm_level;
extern int rader8_alarm_level;
extern int rader9_alarm_level;
extern int rader10_alarm_level;
extern int rader11_alarm_level;
extern int rader12_alarm_level;

extern int rader1_alarm_level_temp;
extern int rader2_alarm_level_temp;
extern int rader3_alarm_level_temp;
extern int rader4_alarm_level_temp;
extern int rader5_alarm_level_temp;
extern int rader6_alarm_level_temp;
extern int rader7_alarm_level_temp;
extern int rader8_alarm_level_temp;
extern int rader9_alarm_level_temp;
extern int rader10_alarm_level_temp;
extern int rader11_alarm_level_temp;
extern int rader12_alarm_level_temp;

extern short int parking_distance_start_x;
extern short int parking_distance_start_y;
extern short int parking_distance_end_x;
extern short int parking_distance_end_y;
extern short int parking_distance_depth;

extern short int parking_rect_point0_x;
extern short int parking_rect_point0_y;
extern short int parking_rect_point1_x;
extern short int parking_rect_point1_y;
extern short int parking_rect_point2_x;
extern short int parking_rect_point2_y;
extern short int parking_rect_point3_x;
extern short int parking_rect_point3_y;

extern int car_paring_status;//²´³µ×´Ì¬

extern int trail_line1_start_x;
extern int trail_line1_start_y;
extern int trail_line1_end_x;
extern int trail_line1_end_y;
extern int trail_line2_start_x;
extern int trail_line2_start_y;
extern int trail_line2_end_x;
extern int trail_line2_end_y;
extern int trail_arc_start_x;
extern int trail_arc_start_y;
extern int trail_arc_end_x;
extern int trail_arc_end_y;
extern short int trail_arc_radius;
extern short int trail_arc_angle;

extern int direction;  //0x01 --> left   0x02 --> right

extern int parking_mode_select;
extern int parking_mode_ok;
extern int parking_mode_ok_flag;

extern int gear_status_actual;
extern int gear_status_pre;
extern int gear_show_flag;

////////////////////////////////////////////////////////////

/***********************************************************/
void *Gui_meg_thread(void *t) 
{
	int i, j, k;
	int ret;

	int dir;
	int x, y;
	struct IOCTL_LAYER_POS layer_cfg;
	struct IOCTL_LAYER_CHROMA layer_chroma_keys;
	struct IOCTL_LAYER_ALFA_KEY layer_alfa_key;
	struct IOCTL_LAYER_ALFA_VAL layer_alfa_value;
	long int location = 0;

	int fb_resx = 1920;
	int fb_resy = 1080;
	int fb_bpp = 32;

	//int fb_resx = 400;
	//int fb_resy = 400;
	printf("gui_log201805311500\n");
	readbmp();
	/* Perform layer transitions */
	dcu_fd = open("/dev/dcu0", O_RDONLY);
	DIE(dcu_fd < 0, "could not open /dev/dcu0");
	
	/* setup all active FB layers */
	ret = setup_fb_layers(0, fb_resx, fb_resy, fb_bpp, 0);
	DIE(ret != 0, "could not setup fb0 layers");
	//ret = setup_fb_layers(1, fb_resx, fb_resy, fb_bpp, 0);
	//DIE(ret != 0, "could not setup fb1 layers");
	/* Draw in /dev/fb0 and /dev/fb1 */
	//for(x=0; x<360; x++)
		//for(y=0; y<360; y++){
		//	 location = (x + y * fb_resx) * 4;

			//if((y>80) && (y<320) && (x>80) && (x<320))
			//	*(fb_ptr[0] + location)	= 100;
			//else
			//*(fb_ptr[0] + location)	= 255;	/* BLUE */
			//*(fb_ptr[0] + location + 1) = 0;	/* GREEN */
			//*(fb_ptr[0] + location + 2) = 0;	/* RED */
			//*(fb_ptr[0] + location + 3) = 128;	/* ALFA */

			//*(fb_ptr[1] + location)		= 0;
			//*(fb_ptr[1] + location + 1) = 0;
			//*(fb_ptr[1] + location + 2) = 255;
			//*(fb_ptr[1] + location + 3) = 120;
		//}
	memcpy((char*)fb_ptr[0],view_mem,1920*1080*4);

	/* alfa mode set to chroma */
	layer_alfa_key.id = 0;
	layer_alfa_key.key = IOCTL_DCU_ALPHAKEY_WHOLEFRAME;

	if (ioctl(dcu_fd, IOCTL_SET_LAYER_ALPHA_MODE, &layer_alfa_key) == -1)
		printf("DCU0 IOCTL_SET_LAYER_ALPHA_MODE failed: %s\n", strerror(errno));

	/* chroma color key interval */
	//layer_chroma_keys.id = 0;
	//layer_chroma_keys.state = IOCTL_DCU_CHROMA_ON;
	//layer_chroma_keys.min.red = 0;
	//layer_chroma_keys.min.green = 0;
	//layer_chroma_keys.min.blue = 0;
	//layer_chroma_keys.max.red = 255;
	//layer_chroma_keys.max.green = 255;
	//layer_chroma_keys.max.blue = 100;

	//if (ioctl(dcu_fd, IOCTL_SET_LAYER_CHROMA_KEY, &layer_chroma_keys) == -1)
	//	printf("DCU0 IOCTL_SET_LAYER_CHROMA_KEY failed: %s\n", strerror(errno));

	/* set layer transparency */
	//layer_alfa_value.id = 0;
	//layer_alfa_value.val = 128;//128;

	//if (ioctl(dcu_fd, IOCTL_SET_LAYER_ALPHA_VAL, &layer_alfa_value) == -1)
	//	printf("DCU0 IOCTL_SET_LAYER_ALPHA_VAL failed: %s\n", strerror(errno));

	
	while(1)
	{	
		//sleep(2);
		//printf("gui_meg_thread loop\n");
		//dir++;
		//dir = 800;
		//if((dir % 800) == 0)
		//{
			/* alfa mode set to none - chroma on selected pixels */
			layer_alfa_key.id = 0;
			layer_alfa_key.key = IOCTL_DCU_ALPHAKEY_WHOLEFRAME;

			if (ioctl(dcu_fd, IOCTL_SET_LAYER_ALPHA_MODE, &layer_alfa_key) == -1)
				printf("DCU0 IOCTL_SET_LAYER_ALPHA_MODE failed: %s\n", strerror(errno));

			//layer_alfa_key.id = 1;
			//layer_alfa_key.key = IOCTL_DCU_ALPHAKEY_WHOLEFRAME;

			//if (ioctl(dcu_fd, IOCTL_SET_LAYER_ALPHA_MODE, &layer_alfa_key) == -1)
			//	printf("DCU0 IOCTL_SET_LAYER_ALPHA_MODE failed: %s\n", strerror(errno));
		//}
		//else if((dir % 1601) == 0)
		//{
			/* alfa mode set to whole frame - blend all pixels, chroma on selected */
			//layer_alfa_key.id = 0;
			//layer_alfa_key.key = IOCTL_DCU_ALPHAKEY_OFF;

			//if (ioctl(dcu_fd, IOCTL_SET_LAYER_ALPHA_MODE, &layer_alfa_key) == -1)
			//	printf("DCU0 IOCTL_SET_LAYER_ALPHA_MODE failed: %s\n", strerror(errno));
		//}

		/* rotate layer 0 clockwise */
		layer_cfg.id = 0;
		layer_cfg.pos.mX = 0 ;//- ((dir / 50) % 200);
		layer_cfg.pos.mY = 0 ;//- ((2 * dir / 50) % 200);

		if (ioctl(dcu_fd, IOCTL_SET_LAYER_POS, &layer_cfg) == -1)
			printf("DCU0 DCU_IOCTL_SET_LAYER_POS failed: %s\n", strerror(errno));

		//layer_cfg.id = 1;
		//layer_cfg.pos.mX = 100 ;//- ((dir / 50) % 200);
		//layer_cfg.pos.mY = 100 ;//- ((2 * dir / 50) % 200);

		//if (ioctl(dcu_fd, IOCTL_SET_LAYER_POS, &layer_cfg) == -1)
		//	printf("DCU0 DCU_IOCTL_SET_LAYER_POS failed: %s\n", strerror(errno));

		
		if(gui_draw_ok_flag == 1)
		{
			memcpy((char*)fb_ptr[0],view_mem_temp,1920*1080*4);
			gui_draw_ok_flag = 0;
		}
		usleep(300000);

		//sleep(2);
	}
}
void *Gui_draw_thread(void *t) 
{
	Mat temp(1080,1920,CV_8UC4,view_mem_temp);

	Point park_rect_p0,park_rect_p1; //line1
	Point park_rect_p2,park_rect_p3; //line2
	//Point line1_start,line1_end; //line1
	//Point line2_start,line2_end; //line2
	
	Point rect_start,rect_end;   //rect
	
	Point center1,center2;
	int axlen1,axlen2;
	int start_angle1, start_angle2;;
	int end_angle1,end_angle2;

	int i,j;
	//direction = 1;
	//parking_mode_select = 3;
	//car_paring_status = 1;
	int numtt;
	//////////////////////////////////////////////
	/*
	Mat logoImage = imread("/home/wyf/logo1_alpha.bmp",-1);
	//Mat mask = imread("/home/wyf/logo1_alpha.bmp",0);
	if(!logoImage.data)
	{
		printf("read logo1_alpha.bmp error!\n");
	}
	Mat ImageROI = temp(Rect(0,0,400,400));
	*/
	//////////////////////////////////////////////
	while(1)
	{
		numtt++;
		if(numtt>=10)
			numtt = 0;
		memcpy(view_mem_temp,view_mem,1920*1080*4);
		
		//layout_alarm_area1();

		//line1_start.x = 1450;
		//line1_start.y = 500;
		//line1_end.x = 1620;
		//line1_end.y = 750;

		//line2_start.x = 1360;
		//line2_start.y = 760;
		//line2_end.x = 1460;
		//line2_end.y = 910;
		
		//rect_start.x = 1198+abs((parking_distance_start_x/15));
		//rect_start.y = 530+(parking_distance_start_y/15);
		//rect_end.x = 1198+abs((parking_distance_end_x/15));
		//rect_end.y = 530+(parking_distance_end_y/15)+(parking_distance_depth/15);

		park_rect_p0.x = 1198-(parking_rect_point0_x/15);
		park_rect_p0.y = 540+(parking_rect_point0_y/15);
		
		park_rect_p1.x = 1198-(parking_rect_point1_x/15);
		park_rect_p1.y = 540+(parking_rect_point1_y/15);
		
		park_rect_p2.x = 1198-(parking_rect_point2_x/15);
		park_rect_p2.y = 540+(parking_rect_point2_y/15);
		
		park_rect_p3.x = 1198-(parking_rect_point3_x/15);
		park_rect_p3.y = 540+(parking_rect_point3_y/15);

		line(temp,park_rect_p0,park_rect_p1,Scalar(0,0,255,255),12,CV_AA);
		line(temp,park_rect_p0,park_rect_p3,Scalar(0,0,255,255),12,CV_AA);
		line(temp,park_rect_p2,park_rect_p1,Scalar(0,0,255,255),12,CV_AA);
		line(temp,park_rect_p2,park_rect_p3,Scalar(0,0,255,255),12,CV_AA);
		
		layout_alarm_area1();
		
		center1.x = 1198;
		center1.y = 540+trail_arc_radius*10/15-80;
		axlen1= abs(trail_arc_radius*10/15);
		if(trail_arc_radius <0)    //up
		{
			start_angle1 = 90-trail_arc_angle;
			end_angle1 = 90;
		}
		else if(trail_arc_radius >0)  //down
		{
			start_angle1 = 270;
			end_angle1 = 270+trail_arc_angle;
		}
		
		center2.x = 1198;
		center2.y = 540+trail_arc_radius*10/15+80;
		axlen2= abs(trail_arc_radius*10/15);
		if(trail_arc_radius <0)    //up
		{
			start_angle2 = 90-trail_arc_angle;
			end_angle2 = 90;
		}
		else if(trail_arc_radius >0)  //down
		{
			start_angle2 = 270;
			end_angle2 = 270+trail_arc_angle;
		}
			
		//line(temp,line1_start,line1_end,Scalar(0,255,255,255),8,CV_AA);
		//line(temp,line2_start,line2_end,Scalar(0,255,255,255),8,CV_AA);

		//line(temp,park_rect_p0,park_rect_p1,Scalar(0,255,255,255),12,CV_AA);
		//line(temp,park_rect_p0,park_rect_p3,Scalar(0,255,255,255),12,CV_AA);
		//line(temp,park_rect_p2,park_rect_p1,Scalar(0,255,255,255),12,CV_AA);
		//line(temp,park_rect_p2,park_rect_p3,Scalar(0,255,255,255),12,CV_AA);
		
		//rectangle(temp,rect_start,rect_end,Scalar(0,0,255,255),8,CV_AA);
		
		//for(i=rect_end.y;i<rect_start.y;i++)
		//{
		//	for(j=rect_start.x;j<rect_end.x;j=j+4)
		//		*((int*)view_mem_temp+j+(i+rect_end.y)*1920) == 0xffff0000;
		//}
		//for(i=rect_start.y;i<rect_end.y;i++)
		
		
		ellipse(temp,center1,Size(axlen1,axlen1),0,start_angle1,end_angle1,Scalar(0,255,255,255),8,CV_AA);
		ellipse(temp,center2,Size(axlen2,axlen2),0,start_angle2,end_angle2,Scalar(0,255,255,255),8,CV_AA);
		//circle(temp,Point(600,1000),400,Scalar(0,255,0,255),6,CV_AA);
		//ellipse(temp,Point(1000,500),Size(400,200),0,0,360,Scalar(0,200,0,255),8,CV_AA);
		//ellipse(temp,Point(1000,500),Size(400,400),0,200,290,Scalar(0,0,255,255),8,CV_AA);

		//logoImage.copyTo(temp);
		//logoImage.convertTo(temp,temp.type(),1,0);
		//addWeighted(ImageROI,0.5,logoImage,0.3,0.0,temp);
		//temp.convertTo(ImageROI,logoImage.type(),1,0);
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		if(gear_show_flag == 0)
		{
			/*
			if(direction == 1) //left
			{
				memcpy(logo_mode_temp,logo_mode_left,240*1080*sizeof(int));
			}
			else if(direction == 2) //right
			{	
				memcpy(logo_mode_temp,logo_mode_right,240*1080*sizeof(int));
			}
			switch(parking_mode_select)
			{
				case 0:
				case 5:
					for(i=0;i<1080;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							//if( (*((int*)logo_mode_temp+j+i*240) >= 0xff646464)&&(*((int*)logo_mode_temp+j+i*240) <= 0xff64a064) )
							//	*((int*)logo_mode_temp+j+i*240) = 0xff64b464;
							for(j=0;j<240;j++)			
							{				
								if( *((int*)logo_mode_temp+j+i*240) == 0xff646464) //100 100 100
									*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
								else if( *((int*)logo_mode_temp+j+i*240) == 0xff647864) //100 120 100
									*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
								else if( *((int*)logo_mode_temp+j+i*240) == 0xff648c64) //100 140 100
									*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
								else if( *((int*)logo_mode_temp+j+i*240) == 0xff64a064) //100 160 100
									*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							}
						}
					}
					break;
				case 1:
					for(i=0;i<1080;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							if( *((int*)logo_mode_temp+j+i*240) == 0xff646464) //100 100 100
								*((int*)logo_mode_temp+j+i*240) = 0xff00b050;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff647864) //100 120 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff648c64) //100 140 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff64a064) //100 160 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
						}
					}
					break;
				case 2:
					for(i=0;i<1080;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							if( *((int*)logo_mode_temp+j+i*240) == 0xff646464) //100 100 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff647864) //100 120 100
								*((int*)logo_mode_temp+j+i*240) = 0xff00b050;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff648c64) //100 140 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff64a064) //100 160 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
						}
					}
					break;
				case 3:
					for(i=0;i<1080;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							if( *((int*)logo_mode_temp+j+i*240) == 0xff646464) //100 100 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff647864) //100 120 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff648c64) //100 140 100
								*((int*)logo_mode_temp+j+i*240) = 0xff00b050;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff64a064) //100 160 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
						}
					}
					break;
				case 4:
					for(i=0;i<1080;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							if( *((int*)logo_mode_temp+j+i*240) == 0xff646464) //100 100 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff647864) //100 120 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff648c64) //100 140 100
								*((int*)logo_mode_temp+j+i*240) = 0xff646e64;
							else if( *((int*)logo_mode_temp+j+i*240) == 0xff64a064) //100 160 100
								*((int*)logo_mode_temp+j+i*240) = 0xff00b050;
						}
					}
					break;
				default:
					break;
			}
			for(i=0;i<1080;i++)
			{
				memcpy(view_mem_temp+i*1920*4,logo_mode_temp+i*240*4,240*4);
			}
			*/
			switch(parking_mode_select)
			{	
				//printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!******************parking_mode_select = %d\n",parking_mode_select);
				case 1:	
					if(parking_mode_ok_flag==1)
					{
						rectangle(temp,Point(24,40),Point(216,224),Scalar(0,0,255,255),8,CV_AA);
					}
					else 
					{
						if(numtt%2)
						{
							rectangle(temp,Point(24,40),Point(216,224),Scalar(0,0,255,255),8,CV_AA);
						}
					}
					break;				
				case 2:
					if(parking_mode_ok_flag==1)
					{
						rectangle(temp,Point(24,302),Point(216,476),Scalar(0,0,255,255),8,CV_AA);
					}
					else 
					{
						if(numtt%2)
						{
							rectangle(temp,Point(24,302),Point(216,476),Scalar(0,0,255,255),8,CV_AA);
						}
					}
					break;				
				case 3:
					if(parking_mode_ok_flag==1)
					{
						rectangle(temp,Point(20,554),Point(216,736),Scalar(0,0,255,255),8,CV_AA);
					}
					else 
					{
						if(numtt%2)
						{
							rectangle(temp,Point(20,554),Point(216,736),Scalar(0,0,255,255),8,CV_AA);
						}
					}
					break;			
				case 4:
					if(parking_mode_ok_flag==1)
					{
						rectangle(temp,Point(18,818),Point(216,1012),Scalar(0,0,255,255),8,CV_AA);
					}
					else 
					{
						if(numtt%2)
						{
							rectangle(temp,Point(18,818),Point(216,1012),Scalar(0,0,255,255),8,CV_AA);	
						}
					}
					break;				
				default:				
					break;			
			}
			if(parking_mode_ok_flag==1)
				parking_mode_ok = parking_mode_select;
			else
				parking_mode_ok = 5;
		}
		else 
		{	
			if(gear_status_pre != gear_status_actual )
			{
				if(numtt%2)
				{
					if(gear_status_pre ==1)
						memcpy(logo_gear_temp,logo_gear_d,240*1080*sizeof(int));
					else if(gear_status_pre ==2)
						memcpy(logo_gear_temp,logo_gear_r,240*1080*sizeof(int));
					for(i=800;i<1080;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							if( *((int*)logo_gear_temp+j+i*240) == 0xffffff0e) 
								*((int*)logo_gear_temp+j+i*240) = 0xffff0000;
						}
					}
				}
				else 
				{
					if(gear_status_pre ==1)
						memcpy(logo_gear_temp,logo_gear_d,240*1080*sizeof(int));
					else if(gear_status_pre ==2)
						memcpy(logo_gear_temp,logo_gear_r,240*1080*sizeof(int));
				}
			}
			else 
			{
				if(gear_status_pre ==1)
						memcpy(logo_gear_temp,logo_gear_d,240*1080*sizeof(int));
					else if(gear_status_pre ==2)
						memcpy(logo_gear_temp,logo_gear_r,240*1080*sizeof(int));
				for(i=800;i<1080;i++)		
				{			
					for(j=0;j<240;j++)			
					{				
						if( *((int*)logo_gear_temp+j+i*240) == 0xffffff0e) 
							*((int*)logo_gear_temp+j+i*240) = 0xffff0000;
					}
				}
			}
			
			for(i=0;i<1080;i++)
			{
				memcpy(view_mem_temp+i*1920*4,logo_gear_temp+i*240*4,240*4);
			}
		}
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		switch(car_paring_status)
		{
			case 1:           //SCAN
				//for(i=0;i<240;i++)
				//{
				//memcpy(logo_park_status_temp+(i+400)*1920*4+1600*4,logo_park_status_scan+i*240*4,240*4);
				//}
				if(numtt%2)
				{
					memcpy(logo_park_status_temp,logo_park_status_scan,240*240*4);
					for(i=0;i<240;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							if( *((int*)logo_park_status_temp+j+i*240) == 0xffed1c24) 
								*((int*)logo_park_status_temp+j+i*240) = 0xffffff00;
						}
					}
					for(i=0;i<240;i++)
					{
						//memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_temp+i*240*4,240*4);
						memcpy(view_mem_temp+i*1920*4+256*4,logo_park_status_temp+i*240*4,240*4);
					}
				}
				else
				{
					for(i=0;i<240;i++)
					//memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_scan+i*240*4,240*4);
					memcpy(view_mem_temp+i*1920*4+256*4,logo_park_status_scan+i*240*4,240*4);
				}
				
				break;
			case 2:           //READY
				for(i=0;i<240;i++)
				{
					//memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_ready+i*240*4,240*4);
					memcpy(view_mem_temp+(i+0)*1920*4+256*4,logo_park_status_ready+i*240*4,240*4);
				}
				break;
			case 3:			 // RUN
				//for(i=0;i<240;i++)
				//{
				//	memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_run+i*240*4,240*4);
				//}
				if(numtt%2)
				{
					memcpy(logo_park_status_temp,logo_park_status_run,240*240*4);
					for(i=0;i<240;i++)		
					{			
						for(j=0;j<240;j++)			
						{				
							if( *((int*)logo_park_status_temp+j+i*240) == 0xffed1c24) 
								*((int*)logo_park_status_temp+j+i*240) = 0xffffff00;
						}
					}
					for(i=0;i<240;i++)
					{
						//memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_temp+i*240*4,240*4);
						memcpy(view_mem_temp+(i+0)*1920*4+256*4,logo_park_status_temp+i*240*4,240*4);
					}
				}
				else
				{
					for(i=0;i<240;i++)
					//memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_run+i*240*4,240*4);
					memcpy(view_mem_temp+(i+0)*1920*4+256*4,logo_park_status_run+i*240*4,240*4);
				}
				break;
			case 4:			//OK
				for(i=0;i<240;i++)
				{
					//memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_stop+i*240*4,240*4);
					memcpy(view_mem_temp+(i+0)*1920*4+256*4,logo_park_status_stop+i*240*4,240*4);
				}
				break;
			case 5:			//FAIL
				for(i=0;i<240;i++)
				{
					//memcpy(view_mem_temp+(i+400)*1920*4+1600*4,logo_park_status_fail+i*240*4,240*4);
					memcpy(view_mem_temp+(i+0)*1920*4+256*4,logo_park_status_fail+i*240*4,240*4);
				}
				break;
			default:
				break;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		//for(i=0;i<400;i=i+8)
		//{
			//for(j=rect_start.x;j<rect_end.x;j=j+4)
		//	for(j=0;j<400;j=j+16)
				//*((int*)view_mem_temp+j+i*1920) == 0xffffffff;
		//		*((int*)view_mem_temp+j+i*1920) = 0xffff0000;
		//}
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		gui_draw_ok_flag = 1;
		usleep(300000);
	}
}

/* create single buffer per layer and clear all layers */
int setup_fb_layers(int num_fb_active,
		int fb_resx,
		int fb_resy,
		int fb_bpp,
		int fb_tiled
		)
{
	struct IOCTL_LAYER_POS layer_cfg;
	struct IOCTL_LAYER_CHROMA layer_chroma_keys;
	struct IOCTL_LAYER_ALFA_KEY layer_alfa_key;

	int i, j, ret;
	int enabled = 0;
	struct fb_var_screeninfo fb_var_info[FB_NUM_LAYERS];
	struct fb_fix_screeninfo fb_fix_info[FB_NUM_LAYERS];

	//dcu_fd[0] = dcu_fd;////open("/dev/dcu0", O_RDWR);
	//DIE(dcu_fd[0] < 0, "could not open /dev/dcu0");

	i = num_fb_active;
	//for(i=0; i<num_fb_active; i++){

		/*get file descriptor */
		fb_fd[i] = open(fb_name[i], O_RDWR);
		DIE(fb_fd[i] == (-1), "Could not open /dev/fb device");

		/* Get variable screen information */
		ret = ioctl(fb_fd[i], FBIOGET_VSCREENINFO, &fb_var_info[i]);
		DIE(ret == (-1), "Error reading variable information /dev/fb");

		/* set 24 BPP & set value*/
		fb_var_info[i].xres_virtual = fb_resx;
		fb_var_info[i].yres_virtual = fb_resy;
		fb_var_info[i].xres = fb_resx;
		fb_var_info[i].yres = fb_resy;
		fb_var_info[i].bits_per_pixel = fb_bpp;
		fb_var_info[i].nonstd = fb_tiled ? IPU_PIX_FMT_GPU32_ST : 0;
		ret = ioctl(fb_fd[i], FBIOPUT_VSCREENINFO, &fb_var_info[i]);
		DIE(ret == (-1), "Error writing fixed information /dev/fb");

		/* Get fixed screen information */
		ret = ioctl(fb_fd[i], FBIOGET_FSCREENINFO, &fb_fix_info[i]);
		DIE(ret == (-1), "Error reading fixed information /dev/fb");

		ret = ioctl(fb_fd[i], MXCFB_GET_PREFETCH, &enabled);
		if (!ret)
			printf("Current tile status is %d\n", enabled);
		else
			printf("MXCFB_GET_PREFETCH not implemented\n");

		ret = ioctl(fb_fd[i], MXCFB_SET_PREFETCH, &fb_tiled);
		if (ret != 0) {
			printf("Error setting tile mode %s\n", fb_tiled == 0 ? "liniar" : "tiled");
			printf("MXCFB_SET_PREFETCH not implemented\n");
		}

		if((fb_var_info[i].xres != fb_resx) ||
			(fb_var_info[i].yres != fb_resy) ||
			(fb_var_info[i].bits_per_pixel != fb_bpp))
				return 1;

		fb_screensize[i] = fb_var_info[i].xres * fb_var_info[i].yres *
					(fb_var_info[i].bits_per_pixel/8);

		/* map memory */
		fb_ptr[i] = (char *)mmap(0, fb_screensize[i],
			PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd[i], 0);
		DIE(fb_ptr[i] == NULL, "Failed to map /dev/fb to memory");

		/* clear layer contents */
		for(j=0; j<fb_fix_info[i].smem_len; j++)
			*(fb_ptr[i] + j) = 0;

		/* set DCU layer blending, whole frame */
		layer_alfa_key.id = i;
		layer_alfa_key.key = IOCTL_DCU_ALPHAKEY_WHOLEFRAME;
		if (ioctl(dcu_fd, IOCTL_SET_LAYER_ALPHA_MODE, &layer_alfa_key) == -1)
			printf("DCU0 IOCTL_SET_LAYER_ALPHA_MODE failed: %s\n", strerror(errno));

		/* disable chroma */
		layer_chroma_keys.id = i;
		layer_chroma_keys.state = IOCTL_DCU_CHROMA_OFF;
		if (ioctl(dcu_fd, IOCTL_SET_LAYER_CHROMA_KEY, &layer_chroma_keys) == -1)
			printf("DCU0 IOCTL_SET_LAYER_CHROMA_KEY failed: %s\n", strerror(errno));
	//}
	return 0;
}

/* clean */
int clean_fb_layers(int num_fb_active)
{
	int i;

	for(i=0; i<num_fb_active; i++){
		munmap(fb_ptr[i], fb_screensize[i]);
		close(fb_fd[i]);
		close(dcu_fd);
	}
	return 0;
}
void layout_alarm_area1( )
{
	int radar1_clor_value  = 0x00ff0014;
	int radar2_clor_value  = 0x00ff1414;
	int radar3_clor_value  = 0x00ff2814;
	int radar4_clor_value  = 0x00ff3c14;
	int radar5_clor_value  = 0x00ff5014;
	int radar6_clor_value  = 0x00ff6414;
	int radar7_clor_value  = 0x00ff7814;
	int radar8_clor_value  = 0x00ff8c14;
	int radar9_clor_value  = 0x00ffa014;
	int radar10_clor_value = 0x00ffb414;
	int radar11_clor_value = 0x00ffc814;
	int radar12_clor_value = 0x00ffdc14;

	int value_temp[12];
	int i,j,k;
	int cnt[12];
	if(rader1_alarm_level<8)
	{
		cnt[0] = rader1_alarm_level - 1;
		value_temp[0] = radar1_clor_value + cnt[0]*20;
	}
	else
	{
		cnt[0] = 8;
		value_temp[0] = radar1_clor_value;
	}

	if(rader2_alarm_level<8)
	{
		cnt[1] = rader2_alarm_level - 1;
		value_temp[1] = radar2_clor_value + cnt[1]*20;
	}
	else
	{
		cnt[1] = 8;
		value_temp[1] = radar2_clor_value;
	}
	
	if(rader3_alarm_level<8)
	{
		cnt[2] = rader3_alarm_level - 1;
		value_temp[2] = radar3_clor_value + cnt[2]*20;
	}
	else
	{
		cnt[2] = 8;
		value_temp[2] = radar3_clor_value;
	}
	
	if(rader4_alarm_level<8)
	{
		cnt[3] = rader4_alarm_level - 1;
		value_temp[3] = radar4_clor_value + cnt[3]*20;
	}
	else
	{
		cnt[3] = 8;
		value_temp[3] = radar4_clor_value;
	}
	
	if(rader5_alarm_level<8)
	{
		cnt[4] = rader5_alarm_level - 1;
		value_temp[4] = radar5_clor_value + cnt[4]*20;
	}
	else
	{
		cnt[4] = 8;
		value_temp[4] = radar5_clor_value;
	}
	
	if(rader6_alarm_level<8)
	{
		cnt[5] = rader6_alarm_level - 1;
		value_temp[5] = radar6_clor_value + cnt[5]*20;
	}
	else
	{
		cnt[5] = 8;
		value_temp[5] = radar6_clor_value;
	}
	
	if(rader7_alarm_level<8)
	{
		cnt[6] = rader7_alarm_level - 1;
		value_temp[6] = radar7_clor_value + cnt[6]*20;
	}
	else
	{
		cnt[6] = 8;
		value_temp[6] = radar7_clor_value;
	}
	
	if(rader8_alarm_level<8)
	{
		cnt[7] = rader8_alarm_level - 1;
		value_temp[7] = radar8_clor_value + cnt[7]*20;
	}
	else
	{
		cnt[7] = 8;
		value_temp[7] = radar8_clor_value;
	}
	
	if(rader9_alarm_level<8)
	{
		cnt[8] = rader9_alarm_level - 1;
		value_temp[8] = radar9_clor_value + cnt[8]*20;
	}
	else
	{
		cnt[8] = 8;
		value_temp[8] = radar9_clor_value;
	}
	
	if(rader10_alarm_level<8)
	{
		cnt[9] = rader10_alarm_level - 1;
		value_temp[9] = radar10_clor_value + cnt[9]*20;
	}
	else
	{
		cnt[9] = 8;
		value_temp[9] = radar10_clor_value;
	}
	
	if(rader11_alarm_level<8)
	{
		cnt[10] = rader11_alarm_level - 1;
		value_temp[10] = radar11_clor_value + cnt[10]*20;
	}
	else
	{
		cnt[10] = 8;
		value_temp[10] = radar11_clor_value;
	}
	
	if(rader12_alarm_level<8)
	{
		cnt[11] = rader12_alarm_level - 1;
		value_temp[11] = radar12_clor_value + cnt[11]*20;
	}
	else
	{
		cnt[11] = 8;
		value_temp[11] = radar12_clor_value;
	}
	//for(k=0;k<12;k++)
	//	printf("cnt[%d]=%d  value_temp[%d]=0x%x\n",k,cnt[k],k,value_temp[k]);
	for(i=0;i<1080;i++) 	
	{			
		for(j=0;j<1920;j++) 		
		{				
			for(k=0;k<12;k++)
			{
				
				if(cnt[k]<8)
				{
					if( *((int*)view_mem_temp+j+i*1920) == value_temp[k] )
					*((int*)view_mem_temp+j+i*1920) = 0xa0ffff0e;
				}
				else if(cnt[k]== 8)
				{
					if((*((int*)view_mem_temp+j+i*1920) >=value_temp[k])&&(*((int*)view_mem_temp+j+i*1920) <= (value_temp[k]+120)))
					*((int*)view_mem_temp+j+i*1920) = 0xc07c7c7c;
				}
			}
		}	
	}
	
}


void layout_alarm_area(int radar_id,int alarm_level)
{
	int radar1_clor_value  = 0x00ff0014;
	int radar2_clor_value  = 0x00ff1414;
	int radar3_clor_value  = 0x00ff2814;
	int radar4_clor_value  = 0x00ff3c14;
	int radar5_clor_value  = 0x00ff5014;
	int radar6_clor_value  = 0x00ff6414;
	int radar7_clor_value  = 0x00ff7814;
	int radar8_clor_value  = 0x00ff8c14;
	int radar9_clor_value  = 0x00ffa014;
	int radar10_clor_value = 0x00ffb414;
	int radar11_clor_value = 0x00ffc814;
	int radar12_clor_value = 0x00ffdc14;

	int value_temp;
	int i,j;
	int cnt;
	if(alarm_level<8)
	{
		cnt = alarm_level-1;
	}
	else
	{
		cnt = 0;
	}
	
	switch(radar_id)
	{
		case 1:
			value_temp = radar1_clor_value + cnt*20;
			break;
		case 2:
			value_temp = radar2_clor_value + cnt*20;
			break;
		case 3:
			value_temp = radar3_clor_value + cnt*20;
			break;
		case 4:
			value_temp = radar4_clor_value + cnt*20;
			break;
		case 5:
			value_temp = radar5_clor_value + cnt*20;
			break;
		case 6:
			value_temp = radar6_clor_value + cnt*20;
			break;
		case 7:
			value_temp = radar7_clor_value + cnt*20;
			break;
		case 8:
			value_temp = radar8_clor_value + cnt*20;
			break;
		case 9:
			value_temp = radar9_clor_value + cnt*20;
			break;
		case 10:
			value_temp = radar10_clor_value + cnt*20;
			break;
		case 11:
			value_temp = radar11_clor_value + cnt*20;
			break;
		case 12:
			value_temp = radar12_clor_value + cnt*20;
			break;
		default:
			break;
	}
	if(alarm_level>0)
	{
		if(alarm_level<8)
		{
			for(i=0;i<1080;i++)		
			{			
				for(j=0;j<1920;j++)			
				{				
					if(*((int*)view_mem_temp+j+i*1920) == value_temp)
					*((int*)view_mem_temp+j+i*1920) = 0xa0ffff0e;
				}	
			}
		}
		else
		{
			for(i=0;i<1080;i++)		
			{			
				for(j=0;j<1920;j++)			
				{				
					if((*((int*)view_mem_temp+j+i*1920) >=value_temp)&&(*((int*)view_mem_temp+j+i*1920) <= (value_temp+120)))
					*((int*)view_mem_temp+j+i*1920) = 0xc07c7c7c;
				}	
			}
		}
	}
}
void readbmp()
{
	FILE* fp;
	int i,j;
	if((fp = fopen("/home/wyf/radar_alarm_view32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen radar_alarm_view32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)view_mem, sizeof(unsigned char), 1920*1080*4, fp);
		fflush(fp);		
		fclose(fp);		
		fp  = NULL;				
		for(i=0;i<1080;i++)		
		{			
			for(j=0;j<1920;j++)			
			{				
				if(*((int*)view_mem+j+i*1920) == 0xffffffff)
					*((int*)view_mem+j+i*1920) = 0x00ffffff;
				else if(*((int*)view_mem+j+i*1920) == 0xff000000)
					*((int*)view_mem+j+i*1920) = 0xffffffff;
				else if (*((int*)view_mem+j+i*1920) == 0xffff008c) //r1
					*((int*)view_mem+j+i*1920) = 0x00ff008c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff0078)
					*((int*)view_mem+j+i*1920) = 0x00ff0078;
				else if (*((int*)view_mem+j+i*1920) == 0xffff0064)
					*((int*)view_mem+j+i*1920) = 0x00ff0064;
				else if (*((int*)view_mem+j+i*1920) == 0xffff0050)
					*((int*)view_mem+j+i*1920) = 0x00ff0050;
				else if (*((int*)view_mem+j+i*1920) == 0xffff003c)
					*((int*)view_mem+j+i*1920) = 0x00ff003c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff0028)
					*((int*)view_mem+j+i*1920) = 0x00ff0028;
				else if (*((int*)view_mem+j+i*1920) == 0xffff0014)
					*((int*)view_mem+j+i*1920) = 0x00ff0014;

				else if (*((int*)view_mem+j+i*1920) == 0xffff148c) //r2
					*((int*)view_mem+j+i*1920) = 0x00ff148c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff1478)
					*((int*)view_mem+j+i*1920) = 0x00ff1478;
				else if (*((int*)view_mem+j+i*1920) == 0xffff1464)
					*((int*)view_mem+j+i*1920) = 0x00ff1464;
				else if (*((int*)view_mem+j+i*1920) == 0xffff1450)
					*((int*)view_mem+j+i*1920) = 0x00ff1450;
				else if (*((int*)view_mem+j+i*1920) == 0xffff143c)
					*((int*)view_mem+j+i*1920) = 0x00ff143c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff1428)
					*((int*)view_mem+j+i*1920) = 0x00ff1428;
				else if (*((int*)view_mem+j+i*1920) == 0xffff1414)
					*((int*)view_mem+j+i*1920) = 0x00ff1414;

				else if (*((int*)view_mem+j+i*1920) == 0xffff288c) //r3
					*((int*)view_mem+j+i*1920) = 0x00ff288c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff2878)
					*((int*)view_mem+j+i*1920) = 0x00ff2878;
				else if (*((int*)view_mem+j+i*1920) == 0xffff2864)
					*((int*)view_mem+j+i*1920) = 0x00ff2864;
				else if (*((int*)view_mem+j+i*1920) == 0xffff2850)
					*((int*)view_mem+j+i*1920) = 0x00ff2850;
				else if (*((int*)view_mem+j+i*1920) == 0xffff283c)
					*((int*)view_mem+j+i*1920) = 0x00ff283c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff2828)
					*((int*)view_mem+j+i*1920) = 0x00ff2828;
				else if (*((int*)view_mem+j+i*1920) == 0xffff2814)
					*((int*)view_mem+j+i*1920) = 0x00ff2814;

				else if (*((int*)view_mem+j+i*1920) == 0xffff3c8c) //r4
					*((int*)view_mem+j+i*1920) = 0x00ff3c8c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff3c78)
					*((int*)view_mem+j+i*1920) = 0x00ff3c78;
				else if (*((int*)view_mem+j+i*1920) == 0xffff3c64)
					*((int*)view_mem+j+i*1920) = 0x00ff3c64;
				else if (*((int*)view_mem+j+i*1920) == 0xffff3c50)
					*((int*)view_mem+j+i*1920) = 0x00ff3c50;
				else if (*((int*)view_mem+j+i*1920) == 0xffff3c3c)
					*((int*)view_mem+j+i*1920) = 0x00ff3c3c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff3c28)
					*((int*)view_mem+j+i*1920) = 0x00ff3c28;
				else if (*((int*)view_mem+j+i*1920) == 0xffff3c14)
					*((int*)view_mem+j+i*1920) = 0x00ff3c14;

				else if (*((int*)view_mem+j+i*1920) == 0xffff508c) //r5
					*((int*)view_mem+j+i*1920) = 0x00ff508c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff5078)
					*((int*)view_mem+j+i*1920) = 0x00ff5078;
				else if (*((int*)view_mem+j+i*1920) == 0xffff5064)
					*((int*)view_mem+j+i*1920) = 0x00ff5064;
				else if (*((int*)view_mem+j+i*1920) == 0xffff5050)
					*((int*)view_mem+j+i*1920) = 0x00ff5050;
				else if (*((int*)view_mem+j+i*1920) == 0xffff503c)
					*((int*)view_mem+j+i*1920) = 0x00ff503c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff5028)
					*((int*)view_mem+j+i*1920) = 0x00ff5028;
				else if (*((int*)view_mem+j+i*1920) == 0xffff5014)
					*((int*)view_mem+j+i*1920) = 0x00ff5014;
				
				else if (*((int*)view_mem+j+i*1920) == 0xffff648c) //r6
					*((int*)view_mem+j+i*1920) = 0x00ff648c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff6478)
					*((int*)view_mem+j+i*1920) = 0x00ff6478;
				else if (*((int*)view_mem+j+i*1920) == 0xffff6464)
					*((int*)view_mem+j+i*1920) = 0x00ff6464;
				else if (*((int*)view_mem+j+i*1920) == 0xffff6450)
					*((int*)view_mem+j+i*1920) = 0x00ff6450;
				else if (*((int*)view_mem+j+i*1920) == 0xffff643c)
					*((int*)view_mem+j+i*1920) = 0x00ff643c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff6428)
					*((int*)view_mem+j+i*1920) = 0x00ff6428;
				else if (*((int*)view_mem+j+i*1920) == 0xffff6414)
					*((int*)view_mem+j+i*1920) = 0x00ff6414;

				else if (*((int*)view_mem+j+i*1920) == 0xffff788c) //r7
					*((int*)view_mem+j+i*1920) = 0x00ff788c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff7878)
					*((int*)view_mem+j+i*1920) = 0x00ff7878;
				else if (*((int*)view_mem+j+i*1920) == 0xffff7864)
					*((int*)view_mem+j+i*1920) = 0x00ff7864;
				else if (*((int*)view_mem+j+i*1920) == 0xffff7850)
					*((int*)view_mem+j+i*1920) = 0x00ff7850;
				else if (*((int*)view_mem+j+i*1920) == 0xffff783c)
					*((int*)view_mem+j+i*1920) = 0x00ff783c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff7828)
					*((int*)view_mem+j+i*1920) = 0x00ff7828;
				else if (*((int*)view_mem+j+i*1920) == 0xffff7814)
					*((int*)view_mem+j+i*1920) = 0x00ff7814;

				else if (*((int*)view_mem+j+i*1920) == 0xffff8c8c) //r8
					*((int*)view_mem+j+i*1920) = 0x00ff8c8c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff8c78)
					*((int*)view_mem+j+i*1920) = 0x00ff8c78;
				else if (*((int*)view_mem+j+i*1920) == 0xffff8c64)
					*((int*)view_mem+j+i*1920) = 0x00ff8c64;
				else if (*((int*)view_mem+j+i*1920) == 0xffff8c50)
					*((int*)view_mem+j+i*1920) = 0x00ff8c50;
				else if (*((int*)view_mem+j+i*1920) == 0xffff8c3c)
					*((int*)view_mem+j+i*1920) = 0x00ff8c3c;
				else if (*((int*)view_mem+j+i*1920) == 0xffff8c28)
					*((int*)view_mem+j+i*1920) = 0x00ff8c28;
				else if (*((int*)view_mem+j+i*1920) == 0xffff8c14)
					*((int*)view_mem+j+i*1920) = 0x00ff8c14;

				else if (*((int*)view_mem+j+i*1920) == 0xffffa08c) //r9
					*((int*)view_mem+j+i*1920) = 0x00ffa08c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffa078)
					*((int*)view_mem+j+i*1920) = 0x00ffa078;
				else if (*((int*)view_mem+j+i*1920) == 0xffffa064)
					*((int*)view_mem+j+i*1920) = 0x00ffa064;
				else if (*((int*)view_mem+j+i*1920) == 0xffffa050)
					*((int*)view_mem+j+i*1920) = 0x00ffa050;
				else if (*((int*)view_mem+j+i*1920) == 0xffffa03c)
					*((int*)view_mem+j+i*1920) = 0x00ffa03c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffa028)
					*((int*)view_mem+j+i*1920) = 0x00ffa028;
				else if (*((int*)view_mem+j+i*1920) == 0xffffa014)
					*((int*)view_mem+j+i*1920) = 0x00ffa014;

				else if (*((int*)view_mem+j+i*1920) == 0xffffb48c) //r10
					*((int*)view_mem+j+i*1920) = 0x00ffb48c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffb478)
					*((int*)view_mem+j+i*1920) = 0x00ffb478;
				else if (*((int*)view_mem+j+i*1920) == 0xffffb464)
					*((int*)view_mem+j+i*1920) = 0x00ffb464;
				else if (*((int*)view_mem+j+i*1920) == 0xffffb450)
					*((int*)view_mem+j+i*1920) = 0x00ffb450;
				else if (*((int*)view_mem+j+i*1920) == 0xffffb43c)
					*((int*)view_mem+j+i*1920) = 0x00ffb43c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffb428)
					*((int*)view_mem+j+i*1920) = 0x00ffb428;
				else if (*((int*)view_mem+j+i*1920) == 0xffffb414)
					*((int*)view_mem+j+i*1920) = 0x00ffb414;

				else if (*((int*)view_mem+j+i*1920) == 0xffffc88c) //r11
					*((int*)view_mem+j+i*1920) = 0x00ffc88c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffc878)
					*((int*)view_mem+j+i*1920) = 0x00ffc878;
				else if (*((int*)view_mem+j+i*1920) == 0xffffc864)
					*((int*)view_mem+j+i*1920) = 0x00ffc864;
				else if (*((int*)view_mem+j+i*1920) == 0xffffc850)
					*((int*)view_mem+j+i*1920) = 0x00ffc850;
				else if (*((int*)view_mem+j+i*1920) == 0xffffc83c)
					*((int*)view_mem+j+i*1920) = 0x00ffc83c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffc828)
					*((int*)view_mem+j+i*1920) = 0x00ffc828;
				else if (*((int*)view_mem+j+i*1920) == 0xffffc814)
					*((int*)view_mem+j+i*1920) = 0x00ffc814;

				else if (*((int*)view_mem+j+i*1920) == 0xffffdc8c) //r12
					*((int*)view_mem+j+i*1920) = 0x00ffdc8c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffdc78)
					*((int*)view_mem+j+i*1920) = 0x00ffdc78;
				else if (*((int*)view_mem+j+i*1920) == 0xffffdc64)
					*((int*)view_mem+j+i*1920) = 0x00ffdc64;
				else if (*((int*)view_mem+j+i*1920) == 0xffffdc50)
					*((int*)view_mem+j+i*1920) = 0x00ffdc50;
				else if (*((int*)view_mem+j+i*1920) == 0xffffdc3c)
					*((int*)view_mem+j+i*1920) = 0x00ffdc3c;
				else if (*((int*)view_mem+j+i*1920) == 0xffffdc28)
					*((int*)view_mem+j+i*1920) = 0x00ffdc28;
				else if (*((int*)view_mem+j+i*1920) == 0xffffdc14)
					*((int*)view_mem+j+i*1920) = 0x00ffdc14;

			}	
		}	
	}
	/*
	if((fp = fopen("/home/wyf/logo_mode_right32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_mode_right32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_mode_right, sizeof(unsigned char), 240*1080*4, fp);
		fflush(fp);		
		fclose(fp);	
		for(i=0;i<1080;i++)		
		{			
			for(j=0;j<240;j++)			
			{				
				if(*((int*)logo_mode_right+j+i*1920) == 0xffffffff)
					*((int*)logo_mode_right+j+i*1920) = 0x00ffffff;
			}
		}
	}

	if((fp = fopen("/home/wyf/logo_mode_left32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_mode_left32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_mode_left, sizeof(unsigned char), 240*1080*4, fp);
		fflush(fp);		
		fclose(fp);	
		for(i=0;i<1080;i++)		
		{			
			for(j=0;j<240;j++)			
			{				
				if(*((int*)logo_mode_left+j+i*1920) == 0xffffffff)
					*((int*)logo_mode_left+j+i*1920) = 0x00ffffff;
			}
		}
	}
	*/
	if((fp = fopen("/home/wyf/logo_park_status_scan32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_park_status_scan32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_park_status_scan, sizeof(unsigned char), 240*240*sizeof(int), fp);
		fflush(fp);		
		fclose(fp);	
		for(i=0;i<240;i++)		
		{			
			for(j=0;j<240;j++)			
			{				
				if(*((int*)logo_park_status_scan+j+i*240) == 0xffffffff)
					*((int*)logo_park_status_scan+j+i*240) = 0x00ffffff;
				else if(*((int*)logo_park_status_scan+j+i*240) == 0xffc8bec8)
					*((int*)logo_park_status_scan+j+i*240) = 0x80c8bec8;
			}
		}
	}

	if((fp = fopen("/home/wyf/logo_park_status_ready32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_park_status_ready32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_park_status_ready, sizeof(unsigned char), 240*240*sizeof(int), fp);
		fflush(fp);		
		fclose(fp);	
		for(i=0;i<240;i++)		
		{			
			for(j=0;j<240;j++)			
			{				
				if(*((int*)logo_park_status_ready+j+i*240) == 0xffffffff)
					*((int*)logo_park_status_ready+j+i*240) = 0x00ffffff;
				else if(*((int*)logo_park_status_ready+j+i*240) == 0xffc8bec8)
					*((int*)logo_park_status_ready+j+i*240) = 0x80c8bec8;
			}
		}
	}

	if((fp = fopen("/home/wyf/logo_park_status_run32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_park_status_run32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_park_status_run, sizeof(unsigned char), 240*240*sizeof(int), fp);
		fflush(fp);		
		fclose(fp);	
		for(i=0;i<240;i++)		
		{			
			for(j=0;j<240;j++)			
			{				
				if(*((int*)logo_park_status_run+j+i*240) == 0xffffffff)
					*((int*)logo_park_status_run+j+i*240) = 0x00ffffff;
				else if(*((int*)logo_park_status_run+j+i*240) == 0xffc8bec8)
					*((int*)logo_park_status_run+j+i*240) = 0x80c8bec8;
			}
		}
	}

	if((fp = fopen("/home/wyf/logo_park_status_stop32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_park_status_ok32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_park_status_stop, sizeof(unsigned char), 240*240*sizeof(int), fp);
		fflush(fp);		
		fclose(fp);	
		for(i=0;i<240;i++)		
		{			
			for(j=0;j<240;j++)			
			{				
				if(*((int*)logo_park_status_stop+j+i*240) == 0xffffffff)
					*((int*)logo_park_status_stop+j+i*240) = 0x00ffffff;
				else if(*((int*)logo_park_status_stop+j+i*240) == 0xffc8bec8)
					*((int*)logo_park_status_stop+j+i*240) = 0x80c8bec8;
			}
		}
	}

	if((fp = fopen("/home/wyf/logo_park_status_fail32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_park_status_fail32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_park_status_fail, sizeof(unsigned char), 240*240*sizeof(int), fp);
		fflush(fp);		
		fclose(fp);	
		for(i=0;i<240;i++)		
		{			
			for(j=0;j<240;j++)			
			{				
				if(*((int*)logo_park_status_fail+j+i*240) == 0xffffffff)
					*((int*)logo_park_status_fail+j+i*240) = 0x00ffffff;
				else if(*((int*)logo_park_status_fail+j+i*240) == 0xffc8bec8)
					*((int*)logo_park_status_fail+j+i*240) = 0x80c8bec8;
			}
		}
	}

	if((fp = fopen("/home/wyf/logo_gear_d32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_gear_d32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_gear_d, sizeof(unsigned char), 240*1080*sizeof(int), fp);
		fflush(fp);		
		fclose(fp);	
		//for(i=0;i<240;i++)		
		//{			
		//	for(j=0;j<240;j++)			
		//	{				
		//		if(*((int*)logo_park_status_fail+j+i*240) == 0xffffffff)
		//			*((int*)logo_park_status_fail+j+i*240) = 0x00ffffff;
		//	}
		//}
	}

	if((fp = fopen("/home/wyf/logo_gear_r32.bmp", "rb")) == NULL)
	{
		printf("Fail to fopen logo_park_status_fail32.bmp \n");
	}	
	else
	{	
		fseek(fp,54L,SEEK_SET);
		fread((unsigned char*)logo_gear_r, sizeof(unsigned char), 240*1080*sizeof(int), fp);
		fflush(fp);		
		fclose(fp);	
		//for(i=0;i<240;i++)		
		//{			
		//	for(j=0;j<240;j++)			
		//	{				
		//		if(*((int*)logo_park_status_fail+j+i*240) == 0xffffffff)
		//			*((int*)logo_park_status_fail+j+i*240) = 0x00ffffff;
		//	}
		//}
	}
}


