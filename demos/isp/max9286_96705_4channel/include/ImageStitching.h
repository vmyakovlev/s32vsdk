#ifndef _IMAGESTITCHING_H_


#define _IMAGESTITCHING_H_
#define Uint32  unsigned int
#define Uint16  unsigned short
#define Uint8   unsigned char
#define S08 char
#define S32 int
#define U08 unsigned S08
#define U16 unsigned short
#define U32 unsigned int
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef signed short	Int16_t;
typedef unsigned int UInt32_t; 
typedef unsigned long int UInt64_t; 
typedef  int Int32_t; 
typedef  unsigned char uchar;

#define IMG_WIDTH         1280	
#define IMG_HEIGHT        720//1080


typedef struct CvPoint1
{
	int x;
 	int y;
 } 
CvPoint1;

 ///CarPosition car_up_left,car_down_right;
 CvPoint1  car_up_left, car_down_right;

///#define  HEIGHT 1024
///#define WIDTH   832
#define SIG_WIDTH        1280  //960
#define SIG_HEIGHT        720// 800

#define SVM_WIDTH          832		
#define	SVM_HEIGHT         1024	
#define TOP_LEFT_HEIGHT    352
#define TOP_LEFT_WIDTH     352
#define BACK_RIGHT_HEIGHT  352
#define BACK_RIGHT_WIDTH   352

#define FRONT_LEFT				(0u)
#define FRONT_RIGHT			(1u)
#define BACK_LEFT				(2u)
#define BACK_RIGHT				(3u)

#define FRONT_CCD				(0u)
#define BACK_CCD				(1u)
#define LEFT_CCD				(2u)
#define RIGHT_CCD				(3u)

#define FRONT_ROW_START		(0u)
#define FRONT_ROW_END		(TOP_LEFT_HEIGHT)//352
#define FRONT_COL_START		(TOP_LEFT_WIDTH)//352
#define FRONT_COL_END		(SVM_WIDTH-BACK_RIGHT_WIDTH)//832-352

#define BACK_ROW_START		(SVM_HEIGHT-BACK_RIGHT_HEIGHT)//1024-352
#define BACK_ROW_END		(SVM_HEIGHT)//1024
#define BACK_COL_START		(TOP_LEFT_WIDTH)//352
#define BACK_COL_END			(SVM_WIDTH-BACK_RIGHT_WIDTH)//832-352

#define LEFT_ROW_START		(TOP_LEFT_HEIGHT)//3//352
#define LEFT_ROW_END			(SVM_HEIGHT-BACK_RIGHT_HEIGHT)//1024-352
#define LEFT_COL_START		(0u)
#define LEFT_COL_END			(TOP_LEFT_WIDTH)//352

#define RIGHT_ROW_START		(TOP_LEFT_HEIGHT)//352
#define RIGHT_ROW_END		(SVM_HEIGHT-BACK_RIGHT_HEIGHT)//1024-352
#define RIGHT_COL_START		(SVM_WIDTH-BACK_RIGHT_WIDTH)//832-352
#define RIGHT_COL_END		(SVM_WIDTH)//832

int blockLumaAvg[4];
int blockLumaCnt[4];
int blockLumaSum[4];

#define BACK_COEF				(256)
#define FORWARD					(0u)
#define REVERSE					(1u)
#define    SINGLEVIEWSIZE 			(960*800*4)
#define    PIC_ORISIZE  				(1280*720*2) //(1280*1080*2)
#define    CARLOGSIZE 				(128*320*2)
#define    NOTESIZE  				(960*224*2)
#define BIRDVIEW   				(832)
#define SIGLEVIEW 				(960)
///for single view 
#define   SINGLEVIEW_SIZE  				(1280*720) //

//define single view output storage
unsigned char  SINGLE_CAMERA[960*800*2];

//***************************Global variable*****wyf added 2017.11.29****************

	unsigned char  front_p[PIC_ORISIZE];
	unsigned char  back_p[PIC_ORISIZE];
	unsigned char  left_p[PIC_ORISIZE];
	unsigned char  right_p[PIC_ORISIZE]; 
	
///	unsigned int mem_fsv_lut[SINGLEVIEW_SIZE];

	#if 0
	unsigned int Lut_Front[660608*4];
	unsigned int Lut_Back[658944*4];
	unsigned int Lut_Left[722944*4];
	unsigned int Lut_Right[720896*4];
	#endif
	
#define LUT_FSV_VIEW 1280*720
#if 0
#define LUT_WT_FB     (329472*2)
#define LUT_WT_LR     360448*2
#define LUT_POS_FB    329472
#define LUT_POS_LR    360448	
unsigned int Lut_Fsv_View[1280*720];
unsigned int Lut_Front[LUT_POS_FB];
unsigned int Lut_Back[LUT_POS_FB*4];
unsigned int Lut_Left[LUT_POS_LR*4];
unsigned int Lut_Right[LUT_POS_LR*4];
UInt64_t Wt_Lut_Front[LUT_WT_FB];
UInt64_t Wt_Lut_Back[LUT_WT_FB*4];
UInt64_t Wt_Lut_Left[LUT_WT_LR*4];
UInt64_t Wt_Lut_Right[LUT_WT_LR*4];
#endif


#define LUT_WT_FB     (329472)
#define LUT_WT_LR     360448
#define LUT_POS_FB    329472
#define LUT_POS_LR    360448

unsigned int Lut_Fsv_View[1280*720];
unsigned int Lut_Front[LUT_POS_FB];
unsigned int Lut_Back[LUT_POS_FB*4];
unsigned int Lut_Left[LUT_POS_LR*4];
unsigned int Lut_Right[LUT_POS_LR*4];

UInt64_t Wt_Lut_Front[LUT_WT_FB];
UInt64_t Wt_Lut_Back[LUT_WT_FB];
UInt64_t Wt_Lut_Left[LUT_WT_LR];
UInt64_t Wt_Lut_Right[LUT_WT_LR];

unsigned char  mem_carlog[128*320*2];
unsigned char  mem_info[1120*200*2];	


	
	unsigned char  mem_rotat_left[1080*1280*2];
	unsigned char  mem_rotat_right[1080*1280*2];	
  	FILE * fp_front;
	FILE * fp_back;
       FILE * fp_left;		
	FILE * fp_right;	
	FILE * fp_carlog;
	FILE * fp_info;
	FILE * fp_Sviewlut_front;
	FILE * fp_Sviewlut_back;
	FILE * fp_Sviewlut_left;
	FILE * fp_Sviewlut_right;
	FILE * filelut;
	
	#define PIC4            0
	#define CAM4           1
	#define UI_PIC4      1





/************************************************************************/
#if 0
unsigned int *CAPTURE_MEM_FRONT_cpy_pic[IMG_HEIGHT]; 
unsigned int *CAPTURE_MEM_BACK_cpy_pic[IMG_HEIGHT];
unsigned int *CAPTURE_MEM_LEFT_cpy_pic[IMG_HEIGHT];
unsigned int *CAPTURE_MEM_RIGHT_cpy_pic[IMG_HEIGHT];

unsigned int *CAPTURE_MEM_FRONT_cpy_video[IMG_HEIGHT]; 
unsigned int *CAPTURE_MEM_BACK_cpy_video[IMG_HEIGHT];
unsigned int *CAPTURE_MEM_LEFT_cpy_video[IMG_HEIGHT];
unsigned int *CAPTURE_MEM_RIGHT_cpy_video[IMG_HEIGHT];
#endif


unsigned short Lut_front[SVM_WIDTH  * TOP_LEFT_HEIGHT * 4]; 
unsigned short Lut_back[SVM_WIDTH * BACK_RIGHT_HEIGHT * 4];
unsigned short Lut_left[SVM_HEIGHT * TOP_LEFT_WIDTH * 4];
unsigned short Lut_right[SVM_HEIGHT * BACK_RIGHT_WIDTH * 4];

unsigned short Lut_front_u1[SIG_WIDTH  * SIG_HEIGHT * 4]; 
unsigned short Lut_back_u1[SIG_WIDTH  * SIG_HEIGHT * 4];
unsigned short Lut_left_u1[SIG_WIDTH  * SIG_HEIGHT * 4];
unsigned short Lut_right_u1[SIG_WIDTH  * SIG_HEIGHT * 4];

unsigned char  SVM_BUFFER[SVM_WIDTH * SVM_HEIGHT * 2]; 
unsigned char  SVM_BUFFER_dst[SVM_WIDTH * SVM_HEIGHT * 2]; 
unsigned char  SVM_BUFFER_dst0[SVM_WIDTH * SVM_HEIGHT * 2]; 

unsigned char  SINGLE_BUFFER[SIG_WIDTH * SIG_HEIGHT * 2];
unsigned char  canrxbuf[8];
int key,key16;
int channel_select ,cha16;
int  snap_number,console_cmd;
/****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
 int saveframe(char *str, void *p, int length, int is_oneframe);

void stitching_block(unsigned int ** c1,unsigned int ** c2,unsigned int ** c3,unsigned int ** c4);
#if 0
int  analysis_panorama_lut_uyvy(unsigned int* p_lut_front_test,
	unsigned int* p_lut_back_test,
	unsigned int* p_lut_left_test,
	unsigned int* p_lut_right_test,
	unsigned char* result_image,
	unsigned char* front_image_uyvy,
	unsigned char* back_image_uyvy,
	unsigned char* left_image_uyvy,
	unsigned char* right_image_uyvy,
	int front_fov_height,
	int back_fov_height,
	int left_fov_width,
	int right_fov_width,
	int result_height,
	int result_width,
	int source_height,
	int source_width);
#endif
Int32_t analysis_panorama_lut_uyvy(
	UInt32_t* p_lut_front_test,
	UInt32_t* p_lut_back_test,
	UInt32_t* p_lut_left_test,
	UInt32_t* p_lut_right_test,
	UInt64_t* p_wt_front_test,
	UInt64_t* p_wt_back_test,
	UInt64_t* p_wt_left_test,
	UInt64_t* p_wt_right_test,
	uchar* result_image,
	uchar* front_image_uyvy,
	uchar* back_image_uyvy,
	uchar* left_image_uyvy,
	uchar* right_image_uyvy,
	CvPoint1 car_up_left,
	CvPoint1 car_down_right,
	Int32_t frontback_fov_height,
	Int32_t leftright_fov_width,
	Int32_t result_height,
	Int32_t result_width,
	Int32_t source_height,
	Int32_t source_width);


void stitchPart(Uint32 **rawDataAddr, Uint16 ccdNum, Uint16 rowStart, Uint16 rowEnd, Uint16 colStart, Uint16 colEnd);

void stitchFusion(Uint32 **rawDataAddrFB, Uint32 **rawDataAddrLR, int fusionBlock, Uint16 rowStart, Uint16 rowEnd, Uint16 colStart, Uint16 colEnd);

void Resize_Interpolation_SingleView(unsigned short *resize_lut, unsigned int **remap_lut, unsigned char * dout, int Nwidth, int Nheight);

void rotate270(unsigned char *src,unsigned char *dst,int width,int height);
void ReadOriginalImage(void);
void ReadLut(void);

void Show4picT(void);

void  DisOriginalSingleViewPic(char num, char* dis_buf);

void Show4videoT(void);

void Show4videoLoop(int num);

void DisSnapOriginalVideocap(int num);

void DisUndistortionSingleVideo(int num,char* dis_buf);

void DisUndistortionSingleViewPic(int num, char* dis_buf);
int UndistortionSVProccess(unsigned int* p_lut,
	unsigned char* result_image,
	unsigned char* source_image,
	int result_height,
	int result_width,
	int source_height,
	int source_width);

void SVMStitching(unsigned char* sf, unsigned char* sb, unsigned char* sl,unsigned char* sr);
void SVMDis(void);
/*****************************************************************/
typedef struct CvRect1
{
	int x;
	int y;
	int width;
	int height;
///	void cvRect1(int init_x, int init_y, int rect_width, int rect_height)
///	{
///		x = init_x;
///		y = init_y;
///		width = rect_width;
///		height = rect_height;
///	}
}
CvRect1;

CvRect1 cvRect1(int init_x, int init_y, int rect_width, int rect_height);
typedef struct TacPoint_Struct
{
	Int16_t x;
	Int16_t y;
}TacPoint;

typedef struct TabBev_Struct
{
	TacPoint point_pos;
	Int32_t wt_fusion;
	Int32_t wt_upleft;
	Int32_t wt_upright;
	Int32_t wt_downleft;
	Int32_t wt_downright;
}TabBev;

TabBev **bev_Table[4];

Int32_t bev_process(
	uchar* result_image,
	uchar* front_image_uyvy,
	uchar* back_image_uyvy,
	uchar* left_image_uyvy,
	uchar* right_image_uyvy,
	CvPoint1 car_up_left,
	CvPoint1 car_down_right,
	Int32_t frontback_fov_height,
	Int32_t leftright_fov_width,
	Int32_t result_width,
	Int32_t source_width,
	TabBev **bev_Table[4]);
void analysis_fusion_region_lut_uyvy(uchar* result_image,
	uchar* p_src1,
	uchar* p_src2,
	Int32_t fusion_pic1,
	Int32_t fusion_pic2,
	CvRect1* region_roi,
	Int32_t result_widthstep,
	Int32_t src_widthstep,
	TabBev **bev_Table[4]);
void analysis_single_region_lut_uyvy(
	uchar* result_image, uchar* p_src,
	CvRect1* region_roi,
	Int32_t camid,
	Int32_t result_widthstep,
	Int32_t src_widthstep,
	TabBev **bev_Table[4]);


Int32_t analysis_single_fov_lut_uyvy(Int32_t fov_height,
	Int32_t fov_width,
	UInt32_t* p_pos_fusion_lut,
	UInt64_t* p_wt_lut,
	uchar* p_src,
	Int32_t row_start,
	Int32_t col_start,
	Int32_t source_height,
	Int32_t source_width,
	Int32_t result_height,
	Int32_t result_width,
	uchar* result_image);
/**********************************************************************/
void *CANTXTask(void * ptr);

void *CANRXTask(void * ptr);

void *KeyTask(void * ptr);

void *TerminalTask(void *ptr) ;

#ifdef __cplusplus
}
#endif

#endif
