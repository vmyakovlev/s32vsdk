
#ifdef __cplusplus
extern "C" {
#endif
#ifndef __STANDALONE__
  #include <signal.h>
#endif // #ifdef __STANDALONE__
#include <string.h>
#include <termios.h>
///#include "frame_output_v234fb.h"
///#include "oal.h"
///#include "vdb_log.h"
///#include "sdi.hpp"
///#include "vdb_log.h"
#include <stdio.h> 
#include <stdlib.h>
#include "ImageStitching.h"
#ifdef __cplusplus
}
#endif

#if 1
void  DisOriginalSingleViewPic(char num,char* dis_buf)
{
    	int  i = 0;	
	unsigned char* p_source = NULL ;
	memset((char *)dis_buf,0,1920*1080*2);
     	switch(num)
     	{
		case  0: 
				p_source  =  front_p; 	break;
		case  1:	
				p_source  =  back_p;	break;
		case  2:	
				p_source  =  left_p;	break;
		case  3:	
				p_source  =  right_p;	break;
		default:break;
	}
	for(i=0;i<720;i++)  
		memcpy(dis_buf+i*1920*2,p_source+1280*2*i, 1280*2);			


}


void DisUndistortionSingleViewPic(int num,char* dis_buf)
{
	int  i = 0;		
	unsigned char* p_source =NULL;
	unsigned char  p_result[1280*720*2];
	memset((char *) dis_buf,0,1920*1080*2);
     	switch(num)
     	{
		case  0:   
				p_source  = front_p;		break;
		case  1:
				p_source  = back_p;		break;
		case  2:
				p_source  = left_p;		break;
		case  3:
				p_source  = right_p;		break;
		default:break;
			 
	}
	UndistortionSVProccess(Lut_Fsv_View, p_result,p_source,720,1280,720,1280 );
	for(i=0;i<720;i++)  
	memcpy((char *)dis_buf+i*1920*2,(char*)p_result+1280*2*i, 1280*2);
}

#endif
