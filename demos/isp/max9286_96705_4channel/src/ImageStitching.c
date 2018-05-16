#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include "ImageStitching.h"



#define BASE_ADJUSTMENT (128)
#define CAMERA_NUM (4)
int gains[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
/*******************************************************************************/



Uint32 CAR_LOGO_TOP;
Uint32 CAR_LOGO_HEIGHT_NEW;
Uint32 CAR_LOGO_LEFT;
Uint32 CAR_LOGO_WIDTH_NEW;

unsigned int sum_q1 = 256;
unsigned int sum_q2 = BACK_COEF/*256*/;
unsigned int sum_q3 = 256;
unsigned int sum_q4 = 256;


////int lumaPixelAvg = 0;
int lumaPixelCnt = 0;
int lumaPixelSum = 0;

//unsigned char SVGAMA[256]={2, 3, 5, 6, 8, 9, 11, 12, 14, 15, 16, 18, 19, 21, 22, 24, 25, 27, 28, 29, 31, 32, 33, 35, 36, 38, 39, 40, 42, 43, 44, 46, 47, 48, 50, 51, 52, 53, 55, 56, 57, 58, 60, 61, 62, 63, 64, 66, 67, 68, 69, 70, 71, 73, 74, 75, 76, 77, 78, 79, 80, 81, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 97, 98, 99, 100, 101, 102, 103, 104, 105, 105, 106, 107, 108, 109, 109, 110, 111, 112, 112, 113, 114, 115, 115, 116, 117, 117, 118, 118, 119, 120, 120, 121, 121, 122, 122, 123, 123, 124, 124, 125, 125, 125, 126, 126, 126, 127, 127, 127, 127, 127, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
//unsigned char SVGAMA[256]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255};
unsigned char MYLUT[256]={1, 2, 4, 5, 6, 7, 9, 10, 11, 12, 14, 15, 16, 17, 18, 20, 21, 22, 
						23, 24, 26, 27, 28, 29, 30, 32, 33, 34, 35, 36, 37, 39, 
						40, 41, 42, 43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 56, 
						57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 68, 69, 70, 71, 72, 
						73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 
						88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 100, 
						101, 102, 103, 104, 105, 106, 107, 108, 108, 109, 110, 
						111, 112, 113, 113, 114, 115, 116, 117, 117, 118, 119, 119,
						120, 121, 122, 122, 123, 124, 124, 125, 125, 126, 126, 127, 
						127, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 
						138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 
						150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161,
						162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 
						174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 
						186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 
						198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
						210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 
						224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 
						238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 
						252, 253, 254, 255};



int   size_lut_front = SVM_WIDTH*TOP_LEFT_HEIGHT * 4;
int   size_lut_back = SVM_WIDTH*BACK_RIGHT_HEIGHT * 4;
int   size_lut_left = SVM_HEIGHT*TOP_LEFT_WIDTH * 4;
int   size_lut_right = SVM_HEIGHT*BACK_RIGHT_WIDTH * 4;






void Resize_Interpolation_SingleView(unsigned short *resize_lut,unsigned int **remap_lut, unsigned char * dout,int Nwidth,int Nheight)
{
	unsigned short i_int_1, j_int_1,i_int_2, j_int_2;
	unsigned short a1a2_1,a3a4_1,a1a2_2,a3a4_2;
	unsigned char a1_1, a2_1, a3_1, a4_1,a1_2, a2_2, a3_2, a4_2;

	unsigned char *remap_ptr_y1;
	unsigned char *remap_ptr_uv1;
	unsigned char *remap_ptr_y2;
	unsigned char *remap_ptr_uv2;

	int i,j;
	int currentLine = 1;
	int nextLine = 1;
	int row = 0;
        for (i = 0; i<Nheight; i++)
	{
		for (j = 0; j<Nwidth; j=j+2)
		{
			j_int_1 = *(resize_lut + i*Nwidth*4 + j*4) ;
			i_int_1 = *(resize_lut + i*Nwidth*4 + j*4 + 1) ;
			a1a2_1    = *(resize_lut + i*Nwidth*4 + j*4 + 2) ;
			a3a4_1    = *(resize_lut + i*Nwidth*4 + j*4 + 3) ;

			a1_1 = a1a2_1 >> 8;
			a2_1 = a1a2_1 % 256;
			a3_1 = a3a4_1 >> 8;
			a4_1 = a3a4_1 % 256;


			row = j_int_1;
			if(((i^row)&1) == 1)
			{
				currentLine = 1;
				nextLine = 1;
			}
			else
			{
				currentLine = 0;
				nextLine = 2;
			}		
			remap_ptr_y1=(unsigned char *)remap_lut[row+currentLine]+1;
			remap_ptr_uv1=(unsigned char *)remap_lut[row+currentLine];
			remap_ptr_y2=(unsigned char *)remap_lut[row+nextLine]+1;
			remap_ptr_uv2=(unsigned char *)remap_lut[row+nextLine];
			
			if(i_int_1%2==0)
			{
				i_int_1=i_int_1 & 0xfffe;
				*dout++= (a1_1*(*(remap_ptr_uv1 + i_int_1*2)) + a2_1*(*(remap_ptr_uv1 + (i_int_1)*2)) +
						   a3_1*(*(remap_ptr_uv2 + i_int_1*2)) + a4_1*(*(remap_ptr_uv2 + (i_int_1)*2)))>>8;
			}
			else
			{
				i_int_1=i_int_1 | 0x01;
				*dout++= (a1_1*(*(remap_ptr_uv1 + i_int_1*2+2)) + a2_1*(*(remap_ptr_uv1 + (i_int_1)*2+2)) +
						   a3_1*(*(remap_ptr_uv2 + i_int_1*2+2)) + a4_1*(*(remap_ptr_uv2 + (i_int_1)*2+2)))>>8;
			}

			*dout++= (a1_1*(*(remap_ptr_y1 + i_int_1*2)) + a2_1*(*(remap_ptr_y1 + (i_int_1+1)*2)) +
					  a3_1*(*(remap_ptr_y2 + i_int_1*2)) + a4_1*(*(remap_ptr_y2 + (i_int_1+1)*2)))>>8;




			j_int_2 = *(resize_lut + i*Nwidth*4 + j*4 + 4) ;
			i_int_2 = *(resize_lut + i*Nwidth*4 + j*4 + 4+ 1) ;
			a1a2_2	  = *(resize_lut + i*Nwidth*4 + j*4 + 4 + 2) ;
			a3a4_2	  = *(resize_lut + i*Nwidth*4 + j*4 + 4 + 3) ;
			

			a1_2 = a1a2_2 >> 8;
			a2_2 = a1a2_2 % 256;
			a3_2 = a3a4_2 >> 8;
			a4_2 = a3a4_2 % 256;

			row = j_int_2;
			if(((i^row)&1) == 1)
			{
				currentLine = 1;
				nextLine = 1;
			}
			else
			{
				currentLine = 0;
				nextLine = 2;
			}
			
			remap_ptr_y1=(unsigned char *)remap_lut[row+currentLine]+1;
			remap_ptr_uv1=(unsigned char *)remap_lut[row+currentLine];
			remap_ptr_y2=(unsigned char *)remap_lut[row+nextLine]+1;
			remap_ptr_uv2=(unsigned char *)remap_lut[row+nextLine];
			
			if(i_int_2%2==1)
			{
				i_int_2=i_int_2 | 0x01;
				*dout++= (a1_2*(*(remap_ptr_uv1 + i_int_2*2)) + a2_2*(*(remap_ptr_uv1 + (i_int_2)*2)) +
						   a3_2*(*(remap_ptr_uv2 + i_int_2*2)) + a4_2*(*(remap_ptr_uv2 + (i_int_2)*2)))>>8;
			}
			else
			{
				i_int_2=i_int_2 & 0xfffe;
				*dout++= (a1_2*(*(remap_ptr_uv1 + i_int_2*2+2)) + a2_2*(*(remap_ptr_uv1 + (i_int_2)*2+2)) +
						   a3_2*(*(remap_ptr_uv2 + i_int_2*2+2)) + a4_2*(*(remap_ptr_uv2 + (i_int_2)*2+2)))>>8;
			}
			
			*dout++= (a1_2*(*(remap_ptr_y1 + i_int_2*2)) + a2_2*(*(remap_ptr_y1 + (i_int_2+1)*2)) +
					  a3_2*(*(remap_ptr_y2 + i_int_2*2)) + a4_2*(*(remap_ptr_y2 + (i_int_2+1)*2)))>>8;
			
		}
	}
	
}

#if 0
void stitching_block()
{
	///int i, j;
	///int maxA = 0, secA = 0;
	///int maxIndexA, secIndexA;
	///int avg0, avg1;
	///int sum = 0;
	///int referLumaAvg;
	static int frameCount = 0;
	///static int maxLumaCCD = 0;
	///static int lumaDirection = FORWARD;
	unsigned short n5 = TOP_LEFT_HEIGHT, m5 = TOP_LEFT_WIDTH; // 352,352
	unsigned short m6 = SVM_WIDTH  - BACK_RIGHT_WIDTH;// 832 - 352
	unsigned short n7 = SVM_HEIGHT - BACK_RIGHT_HEIGHT;// 1024 - 352

	///int sumTmpQ1, sumTmpQ2, sumTmpQ3, sumTmpQ4;
	///Uint8 *ptr = SVM_BUFFER;

	///Uint16 TL, TH;
///	static Uint16 hystereticFlag0;
///	static Uint16 hystereticFlag1;
///	static Uint16 hystereticFlag2;
///	static Uint16 hystereticFlag3;
	frameCount++;
	if(frameCount%25 == 0)
	{
		frameCount = 0;
	}
	//\u6574\u4E2A\u4FEF\u89C6\u56FE
	lumaPixelAvg = 0;
	lumaPixelCnt = 0;
	lumaPixelSum = 0;
	//\u5355\u4E2A\u6444\u50CF\u5934\u6574\u4E2A\u5757\u533A\u57DF
	memset(blockLumaAvg, 0, 4*4);
	memset(blockLumaCnt, 0, 4*4);
	memset(blockLumaSum, 0, 4*4);
	//
	//memset(carNearLumaAvg, 0, 4*4);
	//memset(carNearSrcLumaAvg, 0, 4*4);
	//memset(carNearLumaCnt, 0, 4*4);
	//memset(carNearLumaSum, 0, 4*4);
	
	//\u5757\u62FC\u63A5-------------------------------------------------------------------------------------

		stitchPart(CAPTURE_MEM_FRONT_cpy_pic, FRONT_CCD, 	FRONT_ROW_START,    FRONT_ROW_END,  FRONT_COL_START, 	FRONT_COL_END);
		stitchPart(CAPTURE_MEM_BACK_cpy_pic,  BACK_CCD,  	BACK_ROW_START, 	BACK_ROW_END, 	BACK_COL_START, 	BACK_COL_END);
		stitchPart(CAPTURE_MEM_LEFT_cpy_pic,  LEFT_CCD,  	LEFT_ROW_START,   	LEFT_ROW_END, 	LEFT_COL_START, 	LEFT_COL_END);
		stitchPart(CAPTURE_MEM_RIGHT_cpy_pic, RIGHT_CCD, 	RIGHT_ROW_START,   	RIGHT_ROW_END, 	RIGHT_COL_START, 	RIGHT_COL_END);
	//\u878D\u5408\u533A\u57DF\u62FC\u63A5-------------------------------------------------------------------------------------
		stitchFusion(CAPTURE_MEM_FRONT_cpy_pic, 	CAPTURE_MEM_LEFT_cpy_pic, 	FRONT_LEFT, 	0, 	n5, 	0, 	m5);
		stitchFusion(CAPTURE_MEM_FRONT_cpy_pic, 	CAPTURE_MEM_RIGHT_cpy_pic, 	FRONT_RIGHT, 	0, 	n5, 	m6, SVM_WIDTH);
		stitchFusion(CAPTURE_MEM_BACK_cpy_pic, 	CAPTURE_MEM_LEFT_cpy_pic, 	BACK_LEFT, 		n7, SVM_HEIGHT, 0, m5);
		stitchFusion(CAPTURE_MEM_BACK_cpy_pic, 	CAPTURE_MEM_RIGHT_cpy_pic, 	BACK_RIGHT, 	n7, SVM_HEIGHT, m6, SVM_WIDTH);

		///stitchFusion(CAPTURE_MEM_FRONT_cpy, 	CAPTURE_MEM_LEFT_cpy, 	BACK_LEFT, 		n7, SVM_HEIGHT, 0, m5);
		///stitchFusion(CAPTURE_MEM_FRONT_cpy, 	CAPTURE_MEM_RIGHT_cpy, 	BACK_RIGHT, 	n7, SVM_HEIGHT, m6, SVM_WIDTH);
}
#endif
void stitching_block(unsigned int ** c1,unsigned int ** c2,unsigned int ** c3,unsigned int ** c4)
{
////	static int frameCount = 0;
	///static int maxLumaCCD = 0;
	///static int lumaDirection = FORWARD;

		unsigned short n5 = TOP_LEFT_HEIGHT, m5 = TOP_LEFT_WIDTH; // 352,352
		unsigned short m6 = SVM_WIDTH  - BACK_RIGHT_WIDTH;// 832 - 352
		unsigned short n7 = SVM_HEIGHT - BACK_RIGHT_HEIGHT;// 1024 - 352


	///int sumTmpQ1, sumTmpQ2, sumTmpQ3, sumTmpQ4;
	///Uint8 *ptr = SVM_BUFFER;

	///Uint16 TL, TH;
///	static Uint16 hystereticFlag0;
///	static Uint16 hystereticFlag1;
///	static Uint16 hystereticFlag2;
///	static Uint16 hystereticFlag3;
////	frameCount++;
////	if(frameCount%25 == 0)
////	{
////		frameCount = 0;
////	}
	//\u6574\u4E2A\u4FEF\u89C6\u56FE
	////lumaPixelAvg = 0;
	lumaPixelCnt = 0;
	////lumaPixelSum = 0;
	//\u5355\u4E2A\u6444\u50CF\u5934\u6574\u4E2A\u5757\u533A\u57DF
	////memset(blockLumaAvg, 0, 4*4);
	////memset(blockLumaCnt, 0, 4*4);
	////memset(blockLumaSum, 0, 4*4);
	//
	//memset(carNearLumaAvg, 0, 4*4);
	//memset(carNearSrcLumaAvg, 0, 4*4);
	//memset(carNearLumaCnt, 0, 4*4);
	//memset(carNearLumaSum, 0, 4*4);
///	printf("3\n");
	stitchPart(c1,FRONT_CCD,FRONT_ROW_START,FRONT_ROW_END,FRONT_COL_START,FRONT_COL_END);
	stitchPart(c2,BACK_CCD,BACK_ROW_START,BACK_ROW_END,BACK_COL_START,BACK_COL_END);
	stitchPart(c3,LEFT_CCD,LEFT_ROW_START,LEFT_ROW_END,LEFT_COL_START,LEFT_COL_END);
	stitchPart(c4,RIGHT_CCD,RIGHT_ROW_START,RIGHT_ROW_END,RIGHT_COL_START,RIGHT_COL_END);
	stitchFusion(c1, c3, FRONT_LEFT, 0, n5, 	0, m5);
	stitchFusion(c1, c4, FRONT_RIGHT, 0, n5, m6, SVM_WIDTH);
	stitchFusion(c2, c3, BACK_LEFT, n7, SVM_HEIGHT, 0, m5);
	stitchFusion(c2, c4, BACK_RIGHT, n7, SVM_HEIGHT, m6, SVM_WIDTH);
///	printf("5\n");
		///stitchFusion(CAPTURE_MEM_FRONT_cpy, 	CAPTURE_MEM_LEFT_cpy, 	BACK_LEFT, 		n7, SVM_HEIGHT, 0, m5);
		///stitchFusion(CAPTURE_MEM_FRONT_cpy, 	CAPTURE_MEM_RIGHT_cpy, 	BACK_RIGHT, 	n7, SVM_HEIGHT, m6, SVM_WIDTH);
}
void stitchPart(Uint32 **rawDataAddr, Uint16 ccdNum, Uint16 rowStart, Uint16 rowEnd, Uint16 colStart, Uint16 colEnd)
{
	Uint32 i, j;
	Uint32 lutOffset = 0;
	Uint32 currentLine, nextLine;
	Uint8 alpha1, alpha2, alpha3, alpha4;
	Uint16 row, column;
	//Uint32 sumY, resultY;
	int sumY, resultY,resultU,resultV;
	Uint8 *pY1, *pY2, *pUV1, *pUV2;
	Uint16 *lut =NULL;
	Uint32 qCoef=0;
	Uint32 rightOffset=0;
	unsigned char *dstY = SVM_BUFFER+1;
	unsigned char *dstU = SVM_BUFFER;
	unsigned char *dstV = SVM_BUFFER;
	for (i = rowStart; i < rowEnd; i++)
	{
		switch(ccdNum)
		{
		case 0:
			lut = Lut_front;
			lutOffset = i*SVM_WIDTH;
			qCoef = sum_q1/*sum_q1*/;
			rightOffset = 0;
			break;
		case 1:
			lut = Lut_back;
			lutOffset = (i-rowStart)*SVM_WIDTH;
			qCoef = sum_q2/*sum_q2*/;
			rightOffset = 0;
			break;
		case 2:
			lut = Lut_left;
			lutOffset = i*colEnd;
			qCoef = sum_q3/*sum_q3*/;
			rightOffset = 0;
			break;
		case 3:
			lut = Lut_right;
			lutOffset = i*(SVM_WIDTH-colStart);
			qCoef = sum_q4/*sum_q4*/;
			rightOffset = colStart;
			break;
		}

		for (j = colStart; j < colEnd; j++)
		{
			row 	= *(lut + lutOffset*4 + (j-rightOffset)*4); // adjust v to 0~487
			column 	= *(lut + lutOffset*4 + (j-rightOffset)*4 + 1); // adjust u to 0~647
			alpha1 = *(lut + lutOffset*4 + (j-rightOffset)*4 + 2) >> 8;
			alpha2 = *(lut + lutOffset*4 + (j-rightOffset)*4 + 2) % 256;  
			alpha3 = *(lut + lutOffset*4 + (j-rightOffset)*4 + 3) >> 8;
			alpha4 = *(lut + lutOffset*4 + (j-rightOffset)*4 + 3) % 256;
			if(((i^row)&1) == 1)//ji ou field // 0^0 = 0; 1^0  =1  bit operation
			{
				currentLine = 1;
				nextLine = 1;
			}
			else
			{
				currentLine = 0;
				nextLine = 2;
			}
			////uyvy 422
			pUV1 =(unsigned char *)rawDataAddr[row+currentLine];//u
			pY1 =(unsigned char *)rawDataAddr[row+currentLine]+1;//y1
			pUV2=(unsigned char *)rawDataAddr[row+nextLine];//v
			pY2 =(unsigned char *)rawDataAddr[row+nextLine]+1;//y2
			// Y
			sumY =((((*(pY1 + column*2))*alpha1
				   + (*(pY1 + (column+1)*2 ))*alpha2
				   + (*(pY2 + column *2    ))*alpha3
				   + (*(pY2 + (column+1)*2 ))*alpha4) >> 8));//*(Equ_sum[i*SVM_WIDTH+j]))>>8;

			sumY = MYLUT[sumY];		
			resultY = sumY*qCoef/256;
			resultY =  resultY + gains[ccdNum];
			resultY = resultY < 0 ? 0 : (resultY > 255 ? 255 : resultY);
			*(dstY + i*SVM_WIDTH*2 + j*2)=resultY;

			// UV
			if(j%2==0){
				column = column & 0xfffe;///?
				resultU = MYLUT[(( (*(pUV1 + column*2         ))*alpha1
									   + (*(pUV1 + column*2       ))*alpha2
									   + (*(pUV2 + column*2      ))*alpha3
									   + (*(pUV2 + column*2   ))*alpha4) >> 8)];
				resultU = resultU + gains[ccdNum+CAMERA_NUM];
				resultU = resultU < 0 ? 0 : (resultU > 255 ? 255 : resultU);
				*(dstU + i*SVM_WIDTH*2+ j*2) = resultU;/*MYLUT[*(pUV1 + column*2)];*/
			}else{
				column = column | 0x01;///?

				resultV = MYLUT[(( (*(pUV1 + column*2         ))*alpha1
									   + (*(pUV1 + column*2       ))*alpha2
									   + (*(pUV2 + column*2      ))*alpha3
									   + (*(pUV2 + column*2   ))*alpha4) >> 8)];
				resultV = resultV + gains[ccdNum+CAMERA_NUM*2];
				resultV = resultV < 0 ? 0 : (resultV > 255 ? 255 : resultV);				
				*(dstV + i*SVM_WIDTH*2+ j*2) = resultV; /*MYLUT[*(pUV1 + column*2)];*/
			}
			//statistics Y
			lumaPixelCnt++;
			lumaPixelSum += *(pY1 + column*2);	//\u672A\u505A\u63D2\u503C
			//block statistics
			blockLumaCnt[ccdNum]++;
			blockLumaSum[ccdNum] += *(pY1 + column*2);
		}
	}
}
//
//
//
//
void stitchFusion(Uint32 **rawDataAddrFB, Uint32 **rawDataAddrLR, int fusionBlock, Uint16 rowStart, Uint16 rowEnd, Uint16 colStart, Uint16 colEnd)
{
	Uint32 i, j;
	Uint32 currentLine, nextLine;
	Uint8 alpha1, alpha2, alpha3, alpha4;
	Uint16 row, column;
	//Uint32 sumY;
	int sumY, sumU, sumV;
	Uint8 *pY1, *pY2, *pUV1, *pUV2;
	Uint8 yComponent;
	Uint16 *lutFB=NULL, *lutLR = NULL;
	//Uint32 fbY, lrY;
	int fbY, lrY;
	Uint32 fbU, lrU;
	Uint32 fbV, lrV;
	Uint32 fbq= 0, lrq = 0;
	Uint32 fbLutOffset =0, lrLutOffset = 0;
	Uint32 rightOffset = 0;
	unsigned char *dstY = SVM_BUFFER + 1;
	unsigned char *dstU = SVM_BUFFER;
	unsigned char *dstV = SVM_BUFFER;

	///int u;
	///int v;
	int alphaSum;

	for (i = rowStart; i < rowEnd; i++)
	{
		switch (fusionBlock)
		{
		case 0:
			lutFB = Lut_front;
			lutLR = Lut_left;
			fbLutOffset = i*SVM_WIDTH;
			lrLutOffset = i*colEnd;
			fbq = sum_q1;
			lrq = sum_q3;
			rightOffset = 0;
			break;
		case 1:
			lutFB = Lut_front;
			lutLR = Lut_right;
			fbLutOffset = i*SVM_WIDTH;
			lrLutOffset = i*(SVM_WIDTH - colStart);
			fbq = sum_q1;
			lrq = sum_q4;
			rightOffset = colStart;
			break;
		case 2:
			lutFB = Lut_back;
			lutLR = Lut_left;
			fbLutOffset = (i - rowStart)*SVM_WIDTH;
			lrLutOffset = i*colEnd;
			fbq = sum_q2;
			lrq = sum_q3;
			rightOffset = 0;
			break;
		case 3:
			lutFB = Lut_back;
			lutLR = Lut_right;
			fbLutOffset = (i - rowStart)*SVM_WIDTH;
			lrLutOffset = i*(SVM_WIDTH - colStart);
			fbq = sum_q2;
			lrq = sum_q4;
			rightOffset = colStart;
			break;
		}
		if (i == 470)///?
		{
			lutFB = Lut_back;
		}
		for (j = colStart; j < colEnd; j++)
		{
			//FB---------------------------------------------------------
			row 	= *(lutFB + fbLutOffset * 4 + j * 4); // adjust v to 0~487
			column 	= *(lutFB + fbLutOffset * 4 + j * 4 + 1); // adjust u to 0~647

			alpha1 = *(lutFB + fbLutOffset * 4 + j * 4 + 2) >> 8;
			alpha2 = *(lutFB + fbLutOffset * 4 + j * 4 + 2) % 256;
			alpha3 = *(lutFB + fbLutOffset * 4 + j * 4 + 3) >> 8;
			alpha4 = *(lutFB + fbLutOffset * 4 + j * 4 + 3) % 256;


			if (((i ^ row) & 1) == 1)
			{
				currentLine = 1;
				nextLine = 1;
			}
			else
			{
				currentLine = 0;
				nextLine = 2;
			}

			pY1		= (unsigned char *)rawDataAddrFB[row + currentLine] + 1;
			pUV1	= (unsigned char *)rawDataAddrFB[row + currentLine];
			pY2		= (unsigned char *)rawDataAddrFB[row + nextLine] + 1;
			pUV2	= (unsigned char *)rawDataAddrFB[row + nextLine];

			// Y
			/*fbY =    ((MYLUT[*(pY1 + column  *2   )])*alpha1
					+ (MYLUT[*(pY1 + (column+1)*2 )])*alpha2
					+ (MYLUT[*(pY2 + column *2    )])*alpha3
					+ (MYLUT[*(pY2 + (column+1)*2 )])*alpha4);
					*/
			fbY = ((((*(pY1 + column * 2))*alpha1
				   + (*(pY1 + (column + 1) * 2))*alpha2
				   + (*(pY2 + column * 2))*alpha3
				   + (*(pY2 + (column + 1) * 2))*alpha4) >> 8));
			alphaSum = alpha1 + alpha2 + alpha3 + alpha4;
			if( alphaSum != 0 )
				fbY = MYLUT[fbY * 255 / alphaSum];
			fbY = fbY + gains[(fusionBlock >> 1) & 0x1];
			fbY = fbY < 0 ? 0 : (fbY > 255 ? 255 : fbY);
			fbY *= alphaSum / 255.0;
			
			// UV
			if (j % 2 == 0)
			{
				column = column & 0xfffe;
				fbU = ((*(pUV1 + column * 2))*alpha1
					  + (*(pUV1 + column * 2))*alpha2
					  + (*(pUV2 + column * 2))*alpha3
					  + (*(pUV2 + column * 2))*alpha4) >> 8;
				if (alphaSum != 0)
					fbU = MYLUT[fbU * 255 / alphaSum];
				fbU = fbU + gains[((fusionBlock >> 1) & 0x1) + CAMERA_NUM];
				///fbU = fbU < 0 ? 0 : fbU > 255 ? 255 : fbU;
				fbU = ( fbU > 255) ? 255 : fbU;
				fbU *= alphaSum / 255.0;
				
			}
			else
			{
				column = column | 0x01;
				fbV = ((*(pUV1 + column * 2))*alpha1
					  + (*(pUV1 + column * 2))*alpha2
					  + (*(pUV2 + column * 2))*alpha3
					  + (*(pUV2 + column * 2))*alpha4) >> 8;
				if (alphaSum != 0)
					fbV = MYLUT[fbV * 255 / alphaSum];
				fbV = fbV +	gains[((fusionBlock >> 1) & 0x1) + CAMERA_NUM * 2];
				///fbV = fbV < 0 ? 0 : fbV > 255 ? 255 : fbV;
				fbV =(fbV > 255) ? 255 : fbV;
				fbV *= alphaSum / 255.0;
			}
			yComponent = *(pY1 + column * 2);
			//\u7edf\u8ba1\u5355\u4e2a\u6444\u50cf\u5934\u6574\u5757\u4eae\u5ea6\uff0c\u5f53\u524d\u5757\u975e0\u90fd\u6709\u4eae\u5ea6\u503c
			if ((alpha1 + alpha2 + alpha3 + alpha4) != 0)
			{
				//statistics Y
				lumaPixelCnt++;
				lumaPixelSum += yComponent;	//\u672a\u505a\u63d2\u503c

				if ((fusionBlock == 0) || (fusionBlock == 1))	//\u524d\u5757
				{
					blockLumaCnt[0]++;
					blockLumaSum[0] += yComponent;
				}
				if ((fusionBlock == 2) || (fusionBlock == 3))	//\u540e\u5757
				{
					blockLumaCnt[1]++;
					blockLumaSum[1] += yComponent;
				}
			}

			//LR-----------------------------------------------------
			row 	= *(lutLR + lrLutOffset * 4 + (j - rightOffset) * 4); // adjust v to 0~487
			column 	= *(lutLR + lrLutOffset * 4 + (j - rightOffset) * 4 + 1); // adjust u to 0~647

			alpha1 	= *(lutLR + lrLutOffset * 4 + (j - rightOffset) * 4 + 2) >> 8;
			alpha2 	= *(lutLR + lrLutOffset * 4 + (j - rightOffset) * 4 + 2) % 256;
			alpha3 	= *(lutLR + lrLutOffset * 4 + (j - rightOffset) * 4 + 3) >> 8;
			alpha4 	= *(lutLR + lrLutOffset * 4 + (j - rightOffset) * 4 + 3) % 256;


			if (((i ^ row) & 1) == 1)
			{
				currentLine = 1;
				nextLine = 1;
			}
			else
			{
				currentLine = 0;
				nextLine = 2;
			}

			pY1		= (unsigned char *)rawDataAddrLR[row + currentLine] + 1;
			pUV1	= (unsigned char *)rawDataAddrLR[row + currentLine];
			pY2		= (unsigned char *)rawDataAddrLR[row + nextLine] + 1;
			pUV2	= (unsigned char *)rawDataAddrLR[row + nextLine];

			// Y
			/*lrY  =   ((MYLUT[*(pY1 + column   *2   )])*alpha1
					+ (MYLUT[*(pY1 + (column+1)*2  )])*alpha2
					+ (MYLUT[*(pY2 + column *2     )])*alpha3
					+ (MYLUT[*(pY2 + (column+1)*2  )])*alpha4);
			*/
			lrY = ((((*(pY1 + column * 2))*alpha1
				   + (*(pY1 + (column + 1) * 2))*alpha2
				   + (*(pY2 + column * 2))*alpha3
				   + (*(pY2 + (column + 1) * 2))*alpha4) >> 8));			
			alphaSum = alpha1 + alpha2 + alpha3 + alpha4;
			if (alphaSum != 0)
				lrY = MYLUT[lrY * 255 / alphaSum]; 
			lrY = lrY + gains[(fusionBlock & 0x1) + 2];
			lrY = lrY < 0 ? 0 : (lrY > 255 ? 255 : lrY);
			
			lrY *= alphaSum / 255.0;
			
			// UV
			if (j % 2 == 0)
			{
				column = column & 0xfffe;
				lrU   =   ((*(pUV1 + column * 2))*alpha1
					  + (*(pUV1 + column * 2))*alpha2
					  + (*(pUV2 + column * 2))*alpha3
					  + (*(pUV2 + column * 2))*alpha4) >> 8;
				if (alphaSum != 0)
					lrU = MYLUT[lrU * 255 / alphaSum];
				lrU = lrU + gains[(fusionBlock & 0x1) + 2 + CAMERA_NUM];
				///lrU = lrU < 0 ? 0 : lrU > 255 ? 255 : lrU;
				lrU =( lrU > 255 )? 255 : lrU;
				lrU *= alphaSum / 255.0;
				
			}
			else
			{
				column = column | 0x01;
				lrV   =   ((*(pUV1 + column * 2))*alpha1
					  + (*(pUV1 + column * 2))*alpha2
					  + (*(pUV2 + column * 2))*alpha3
					  + (*(pUV2 + column * 2))*alpha4) >> 8;
				if (alphaSum != 0)
					lrV = MYLUT[lrV * 255 / alphaSum];
				lrV = lrV + gains[(fusionBlock & 0x1) + 2 + CAMERA_NUM * 2];
				///lrV = lrV < 0 ? 0 : lrV > 255 ? 255 : lrV;
				lrV = ( lrV > 255) ? 255 : lrV;
				lrV *= alphaSum / 255.0;
				
			}
			yComponent = *(pY1 + column * 2);
			if (alpha1 + alpha2 + alpha3 + alpha4 != 0)
			{
				if ((fusionBlock == 0) || (fusionBlock == 2))	//\u5de6\u5757
				{
					blockLumaCnt[2]++;
					blockLumaSum[2] += yComponent;
				}
				if ((fusionBlock == 1) || (fusionBlock == 3))	//\u53f3\u5757
				{
					blockLumaCnt[3]++;
					blockLumaSum[3] += yComponent;
				}

				//statistics Y
				lumaPixelCnt++;
				lumaPixelSum += yComponent;	//\u672a\u505a\u63d2\u503c
			}

			// pixel blending
			sumY = fbY*fbq / 255 + lrY*lrq / 255;//*(Equ_sum[i*SVM_WIDTH+j]))>>8;
			sumY = sumY < 0 ? 0 : (sumY > 255 ? 255 : sumY);
			// Y
			//sumY= (lumaProbSum[sumY]*(max-min)>>18)+min;
			*(dstY + i*SVM_WIDTH * 2 + j * 2) = sumY;

			// UV
			if (j % 2 == 0)
			{
				sumU = fbU + lrU;
				sumU = sumU < 0 ? 0 : (sumU > 255 ? 255 : sumU);
				*(dstU + i*SVM_WIDTH * 2 + j * 2) = sumU; //u
			}
			else
			{
				sumV = fbV + lrV;
				sumV = sumV < 0 ? 0 : (sumV > 255 ? 255 : sumV);
				*(dstV + i*SVM_WIDTH * 2 + j * 2) = sumV; //v
			}
		}
	}
}

void rotate270(unsigned char *src,unsigned char *dst,int width,int height)
{
	int copyBytes = 4;
	int bytesPerLine = width << 1;
	int step = height << 2;
	int offset = bytesPerLine - copyBytes;
	
	unsigned char *dest = dst;
	unsigned char *source = src;
	unsigned char *psrc = NULL;
	unsigned char *pdst[2] = {NULL,NULL};	
	int i,j,k;
	unsigned char temp;

	for(i=0;i<bytesPerLine;i+=copyBytes)
	{
		pdst[1] = dest;
		pdst[0] = dest+(height<<1);
		psrc = source + offset;

		for(j=0;j<height;++j)
		{
			k = j % 2;
			*((unsigned int *)pdst[k]) = *((unsigned int *)psrc);

			if(1==k)
			{
				temp = *(pdst[1]+1);
				*(pdst[1]+1) = *(pdst[0]-1);
				*(pdst[0]-1) = temp;
			}

			pdst[k] += copyBytes;
			psrc +=bytesPerLine;
		}
		
		dest += step;
		source -= copyBytes;

	
	}
}

void Rotate180(unsigned char *src,int width,int height)
{
	int copyBytes = 4;
	int totalBytes = width*height << 1;
	int end = totalBytes >> 1;
	unsigned char *dst = src + totalBytes - copyBytes;
	unsigned int tmp0;					
	unsigned char tmp1;
	int i;
	
	for(i = 0; i < end; i += copyBytes)
	{
		tmp0 = *((unsigned int *)dst);
		*((unsigned int *)dst) = *((unsigned int *)src);
		*((unsigned int *)src) = tmp0;
	
		//\u4ea4\u6362Y\u5206\u91cf
		tmp1 = dst[1];
		dst[1] = dst[3];
		dst[3] = tmp1;
	
		tmp1 = src[1];
		src[1] = src[3];
		src[3] = tmp1;
	
		dst -= copyBytes;
		src += copyBytes;
	
		//watchdog();
	}
}


void Rotate90(unsigned char *src,unsigned char *dst,int width,int height)
{
	int copyBytes = 4;
	int bytesPerLine = width << 1;
	int step = height << 2;
	int offset = (height-1)*bytesPerLine;	
	unsigned char *dest = dst;
	unsigned char *source = src;
	unsigned char *psrc = NULL;
	unsigned char *pdst[2] = {NULL,NULL};	
	int i,j,k;
	unsigned char temp;
	for(i=0;i<bytesPerLine;i+=copyBytes)
	{
		pdst[0] = dest;
		pdst[1] = dest+(height<<1);
		psrc = source + offset;
		for(j=0;j<height;++j)
		{
			k = j % 2;
			*((unsigned int *)pdst[k]) = *((unsigned int *)psrc);

			if(1==k)
			{
				temp = *(pdst[0]-1);
				*(pdst[0]-1) = *(pdst[1]+1);
				*(pdst[1]+1) = temp;
			}
			pdst[k] += copyBytes;
			psrc -=bytesPerLine;
		}		
		dest += step;
		source += copyBytes;
	}
}


void ReadOriginalImage(void)
{
		FILE* fp_front = NULL;
		FILE* fp_back = NULL;
		FILE* fp_left = NULL;
		FILE* fp_right = NULL;
		printf("read 4 image begin\n");


		if((fp_front = fopen("/home/henry/test/1280720/front.yuv", "rb")) == NULL)
		{
			printf("\nFail to fopen front.yuv\n");
		}
		else
		{		
			fread(front_p, sizeof(unsigned char), PIC_ORISIZE, fp_front);
			fflush(fp_front);
			fclose(fp_front);
		}
	
		if ((fp_back = fopen("/home/henry/test/1280720/back.yuv", "rb")) == NULL)
		{
			printf("Fail to fopen back.yuv\n");
		}
		else
		{		
			fread(back_p, sizeof(unsigned char), PIC_ORISIZE, fp_back);
			fflush(fp_back);
			fclose(fp_back);
		}
			
		if ((fp_left = fopen("/home/henry/test/1280720/left.yuv", "rb")) == NULL)
		{
			printf("Fail to fopen left.yuv\n");
		}
		else
		{		
			fread(left_p, sizeof(unsigned char), PIC_ORISIZE, fp_left);
			fflush(fp_left);
			fclose(fp_left);
		}

			
	if ((fp_right = fopen("/home/henry/test/1280720/right.yuv", "rb")) == NULL)
		{
			printf("Fail to fopen right.yuv\n");
		}
		else
		{		
			fread(right_p, sizeof(unsigned char), PIC_ORISIZE, fp_right);
			fflush(fp_right);
			fclose(fp_right);
		}
	printf("read 4 image end\n");	
#if 0
	for (i = 0; i < 720; i++) //get each row  address of every camera data
	{	
			CAPTURE_MEM_FRONT_cpy_video[i] = (unsigned int*)(&front_p[i * 720 * 2]);
			CAPTURE_MEM_BACK_cpy_video[i] = (unsigned int*)(&back_p[i * 720 * 2]);
			CAPTURE_MEM_LEFT_cpy_video[i] = (unsigned int*)(&left_p[i * 720 * 2]);
			CAPTURE_MEM_RIGHT_cpy_video[i] = (unsigned int*)(&right_p[i * 720 * 2]);
	}	
	printf("get each row  address success\n");	
#endif
}


void ReadLut(void)
{
///	int i;
///	FILE* filelut;
	FILE * fp_fsvlut_file;
	char filename[200]= "";
	char *str1;
	char *filename1;
	char * filename2;
	memset(filename,0,100);
	str1 = "/home/henry/test/wxlut/";
	memcpy(filename,str1,23);
	strcat(filename,"Lut_Front_Single_View.bin");
 ///read single view resize of lut
	if((fp_fsvlut_file= fopen(filename, "rb"))== NULL){
		printf("Lut_Front_Single_View.bin was not opened\n");		
	}

	else	{// front single view lut
		fread(Lut_Fsv_View,sizeof(unsigned int),LUT_FSV_VIEW,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Lut_Front_Single_View.bin success !\n");		
	}

 ///read front svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Lut_Front.bin");
	filename1 = "/home/henry/test/wxlut/Lut_Front.bin";

	if((fp_fsvlut_file= fopen(filename1, "rb"))== NULL){
		printf("Lut_Front.bin was not opened\n");	
		}

	else	{
		fread(Lut_Front,sizeof(unsigned int),LUT_POS_FB,fp_fsvlut_file);    //SINGLEVIEW_SIZES 660608
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Lut_Front.bin  success !\n");		
	}
 ///read back svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Lut_Back.bin");
	if((fp_fsvlut_file= fopen(filename, "rb"))== NULL){
		printf("Lut_Back.bin was not opened\n");
		}

	else	{
		fread(Lut_Back,sizeof(unsigned int),LUT_POS_FB,fp_fsvlut_file);    //SINGLEVIEW_SIZES 658944
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Lut_Back.bin  success !\n");		
	}
 ///read left svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Lut_Left.bin");
	if((fp_fsvlut_file= fopen(filename, "rb"))== NULL){
		printf("Lut_Left.bin was not opened\n");	
		}

	else	{
		fread(Lut_Left,sizeof(unsigned int),LUT_POS_LR,fp_fsvlut_file);    //SINGLEVIEW_SIZES 722944
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Lut_Left.bin success !\n");		
	}
 ///read right svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Lut_Right.bin");
	if((fp_fsvlut_file= fopen(filename, "rb"))== NULL){
		printf("Lut_Right.binwas not opened\n");	
		}

	else	{
		fread(Lut_Right,sizeof(unsigned int),LUT_POS_LR,fp_fsvlut_file);    //SINGLEVIEW_SIZES 720896
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Lut_Right.bin   success !\n");		
	}


	
 ///read wt front svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Wt_Front.bin");
	filename2 = "/home/henry/test/wxlut/Wt_Front.bin";
	if((fp_fsvlut_file= fopen(filename2, "rb"))== NULL){
		printf("Wt_Front.binwas not opened\n");	
		}

	else	{
		fread(Wt_Lut_Front,sizeof(UInt64_t),LUT_WT_FB,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Wt_Front.bin   success !\n");		
	}
 ///read wt back svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Wt_Back.bin");
	if((fp_fsvlut_file= fopen(filename, "rb"))== NULL){
		printf("Wt_Back.binwas not opened\n");	
		}

	else	{
		fread(Wt_Lut_Back,sizeof(UInt64_t),LUT_WT_FB,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Wt_Back.bin   success !\n");		
	}
 ///read wt left svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Wt_Left.bin");
	if((fp_fsvlut_file= fopen(filename, "rb"))== NULL){
		printf("Wt_Left.binwas not opened\n");	
		}

	else	{
		fread(Wt_Lut_Left,sizeof(UInt64_t),LUT_WT_LR,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Wt_Left.bin   success !\n");		
	}
 ///read wt right svm  of lut
	memset(filename,0,100);
	memcpy(filename,str1,strlen(str1));
	strcat(filename,"Wt_Right.bin");
	if((fp_fsvlut_file= fopen(filename, "rb"))== NULL){
		printf("Wt_Right.binwas not opened\n");	
		}

	else	{
		fread(Wt_Lut_Right,sizeof(UInt64_t),LUT_WT_LR,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("Wt_Right.bin   success !\n");		
	}
	
}

#if 0
void ReadLut(void)
{
	///int i;
	FILE * fp_fsvlut_file;		

 ///read single view resize of lut
	if((fp_fsvlut_file= fopen("/home/henry/test/1280720/front_single_view_lut.bin", "rb"))== NULL){
		printf("/home/henry/test/1280720/front_single_view_lut.bin was not opened\n");		
	}

	else	{// front single view lut
		fread(mem_fsv_lut,sizeof(unsigned int),1280*720,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("/home/henry/test/1280720/front_single_view_lut.bin success !\n");		
	}


	

 ///read front svm  of lut
	if((fp_fsvlut_file= fopen("/home/henry/test/1280720/Lut_Front.bin", "rb"))== NULL){
		printf("/home/henry/test/1280720/Lut_Front.bin was not opened\n");	
		}

	else	{
		fread(Lut_Front,sizeof(unsigned int),660608,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("/home/henry/test/1280720/Lut_Front.bin  success !\n");		
	}
 ///read back svm  of lut
	if((fp_fsvlut_file= fopen("/home/henry/test/1280720/Lut_Back.bin", "rb"))== NULL){
		printf("/home/henry/test/1280720/Lut_Back.bin was not opened\n");
		}

	else	{
		fread(Lut_Back,sizeof(unsigned int),658944,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("/home/henry/test/1280720/Lut_Back.bin  success !\n");		
	}
 ///read left svm  of lut
	if((fp_fsvlut_file= fopen("/home/henry/test/1280720/Lut_Left.bin", "rb"))== NULL){
		printf("/home/henry/test/1280720/Lut_Left.bin was not opened\n");	
		}

	else	{
		fread(Lut_Left,sizeof(unsigned int),722944,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("/home/henry/test/1280720/Lut_Left.bin success !\n");		
	}

 ///read right svm  of lut
	if((fp_fsvlut_file= fopen("/home/henry/test/1280720/Lut_Right.bin", "rb"))== NULL){
		printf("/home/henry/test/1280720/Lut_Right.binwas not opened\n");	
		}

	else	{
		fread(Lut_Right,sizeof(unsigned int),720896,fp_fsvlut_file);    //SINGLEVIEW_SIZES 
		fflush(fp_fsvlut_file);
		fclose(fp_fsvlut_file);
		printf("/home/henry/test/1280720/Lut_Right.bin   success !\n");		
	}
}
#endif	
 int saveframe(char *str, void *p, int length, int is_oneframe)
{
	FILE *fd;

	if (is_oneframe) {
		fd = fopen(str, "wb");
	}
	else {
		fd = fopen(str, "a");
	}     

	if (!fd) {
		printf("Open file error\n");
		return -1;
	}
	if (fwrite(p, 1, length, fd)) {
		printf("Write file successfully\n");
		fclose(fd);
		return 0;
	}
	else {
		printf("Write file fail\n");
		fclose(fd);
		return -1;
	}
	
}
/// void Resize_Interpolation_SingleView(unsigned short *resize_lut,unsigned int **remap_lut, unsigned char * dout,int Nwidth,int Nheight)

int UndistortionSVProccess(unsigned int * p_lut,
	unsigned char* result_image,
	unsigned char* source_image,
	int result_height,
	int result_width,
	int source_height,
	int source_width)
{
       int ret = 0;

	///int half_result_height, half_result_width;
	///int half_source_height, half_source_width;
	unsigned int  item;
	int source_pos[2]; 
	int wt_up, wt_left;
	int wt_upleft, wt_upright, wt_downleft, wt_downright;
	int src_widthstep, result_widthstep;// result_nchannels; src_nchannels,
	int yuv[3];
///	unsigned char* p_dst = result_image;
	unsigned char* p_src = source_image;

	///half_source_height = source_height >> 1;
	////half_source_width = source_width >> 1;
	////half_result_height = result_height >> 1;
	////half_result_width = result_width >> 1;

	result_widthstep = result_width << 1;
	src_widthstep = source_width << 1;

	for (int i = 0; i < result_height; i++)
	{
		for (int j = 0; j < result_width; j++)
		{
			item = p_lut[i * result_width + j];

			source_pos[0] = item >> 21;
			source_pos[1] = (item >> 10) & 0x7FF;

			source_pos[0] = source_pos[0] < 0 ? 0 : (source_pos[0] > source_height - 1 ? source_height - 1 : source_pos[0]);
			source_pos[1] = source_pos[1] < 0 ? 0 : (source_pos[1] > source_width - 1 ? source_width - 1 : source_pos[1]);

			wt_up = (item >> 5) & 0x1F;
			wt_left = item & 0x1F;

			wt_upleft = (31 - wt_up) * (31 - wt_left);
			wt_upright = (31 - wt_up) * wt_left;
			wt_downleft = wt_up * (31 - wt_left);
			wt_downright = 31 * 31 - wt_upleft - wt_upright - wt_downleft;

			yuv[0] = (p_src[source_pos[0] * src_widthstep + source_pos[1] * 2 + 1] * wt_upleft
				+ p_src[source_pos[0] * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_upright
				+ p_src[(source_pos[0] + 1) * src_widthstep + source_pos[1] * 2 + 1] * wt_downleft
				+ p_src[(source_pos[0] + 1) * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_downright) >> 10;

			yuv[0] = yuv[0] < 0 ? 0 : (yuv[0] > 255 ? 255 : yuv[0]);

			result_image[i * result_widthstep + j*2+1] = yuv[0];///

			// process u & v channel
			if(((j^source_pos[1])&1)==1)
				
				source_pos[1]+=1;
				
			
			if (j % 2 == 0)
			{
				yuv[1] = p_src[source_pos[0] * src_widthstep + source_pos[1] * 2];
				yuv[1] = yuv[1] < 0 ? 0 : (yuv[1] > 255 ? 255 : yuv[1]);
				result_image[i * result_widthstep + j * 2] = yuv[1];
			}
			else
			{
				yuv[2] = p_src[source_pos[0] * src_widthstep + source_pos[1] * 2];
				yuv[2] = yuv[2] < 0 ? 0 : (yuv[2] > 255 ? 255 : yuv[2]);
				result_image[i * result_widthstep + j * 2] = yuv[2];
			}
		}
	}

	return ret;
}


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
	Int32_t source_width)
{
	Int32_t ret = 0;

	
	uchar* p_src;
	UInt32_t* p_pos_lut;
	UInt64_t* p_wt_lut;
	Int32_t fov_height, fov_width;
	Int32_t dst_height, dst_width;
	Int32_t src_height, src_width;
	Int32_t row_start, col_start;
	uchar* p_dst = result_image;
	memset(p_dst, 0, SVM_WIDTH * SVM_HEIGHT * 2);

	src_height = source_height;
	src_width = source_width;
	dst_height = result_height;
	dst_width = result_width;

	UInt32_t loop_count = 0;
///	Float64_t total_time_1 = 0, total_time_2 = 0;
	while (loop_count < 1)
	{
#ifdef TIME_CONSUME
		time_t  start_time = clock();
#endif
		// process front fov
		p_src = front_image_uyvy;
		p_pos_lut = p_lut_front_test;
		p_wt_lut = p_wt_front_test;
		fov_height = frontback_fov_height;
		fov_width = result_width;
		row_start = 0;
		col_start = 0;
		analysis_single_fov_lut_uyvy(fov_height,
			fov_width,
			p_pos_lut,
			p_wt_lut,
			p_src,
			row_start,
			col_start,
			src_height,
			src_width,
			dst_height,
			dst_width,
			p_dst);
#ifdef TIME_CONSUME		
		time_t  time1_1 = clock();
		double time0_1 = (double)(clock() - start_time);
		//printf("image_result_test cost :%f\n", time0_1);
		total_time_1 += time0_1;
#endif
		loop_count++;
	}

	// process back fov
	p_src = back_image_uyvy;
	p_pos_lut = p_lut_back_test;
	p_wt_lut = p_wt_back_test;
	fov_height = frontback_fov_height;
	fov_width = result_width;
	row_start = car_down_right.y + 1;
	col_start = 0;
	analysis_single_fov_lut_uyvy(fov_height,
		fov_width,
		p_pos_lut,
		p_wt_lut,
		p_src,
		row_start,
		col_start,
		src_height,
		src_width,
		dst_height,
		dst_width,
		p_dst);
#if 1
	// process left fov
	p_src = left_image_uyvy;
	p_pos_lut = p_lut_left_test;
	p_wt_lut = p_wt_left_test;
	fov_height = result_height;
	fov_width = leftright_fov_width;
	row_start = 0;
	col_start = 0;
	analysis_single_fov_lut_uyvy(fov_height,
		fov_width,
		p_pos_lut,
		p_wt_lut,
		p_src,
		row_start,
		col_start,
		src_height,
		src_width,
		dst_height,
		dst_width,
		p_dst);

	// process right fov
	p_src = right_image_uyvy;
	p_pos_lut = p_lut_right_test;
	p_wt_lut = p_wt_right_test;
	fov_height = result_height;
	fov_width = leftright_fov_width;
	row_start = 0;
	col_start = car_down_right.x + 1;

	analysis_single_fov_lut_uyvy(fov_height,
		fov_width,
		p_pos_lut,
		p_wt_lut,
		p_src,
		row_start,
		col_start,
		src_height,
		src_width,
		dst_height,
		dst_width,
		p_dst);
#endif
	return ret;
}






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
	TabBev **bev_Table[4])
{
	Int32_t ret = 0;
	uchar* p_src, *p_src1, *p_src2;
	memset(result_image, 0, 832 * 1024 * 2);
	
	Int32_t yuv[3];
	Int32_t result_widthstep, src_widthstep;
	CvRect1 region_roi;
	Int32_t camid, fusion_pic1, fusion_pic2;

	result_widthstep = result_width << 1;
	src_widthstep = source_width << 1;

	UInt32_t loop_count = 0;
//	Float64_t total_time_1 = 0, total_time_2 = 0;
	while (loop_count < 1)
	{
#ifdef TIME_CONSUME
	time_t  start_time = clock();
#endif
		//front left
		p_src1 = front_image_uyvy;
		p_src2 = left_image_uyvy;
		fusion_pic1 = 0;
		fusion_pic2 = 2;
		region_roi = cvRect1(0, 0, car_up_left.x, car_up_left.y);
		analysis_fusion_region_lut_uyvy(result_image, p_src1, p_src2, fusion_pic1, fusion_pic2, &region_roi, result_widthstep, src_widthstep, bev_Table);

#ifdef TIME_CONSUME		
		time_t  time1_1 = clock();
		double time0_1 = (double)(clock() - start_time);
		//printf("image_result_test cost :%f\n", time0_1);
		total_time_1 += time0_1;
#endif
		loop_count++;
	}
	

	///time_t time1_1 = clock();

	//front
	region_roi = cvRect1(car_up_left.x, 0, car_down_right.x - car_up_left.x + 1, frontback_fov_height);//-10
	camid = 0;
	p_src = front_image_uyvy;
	analysis_single_region_lut_uyvy(result_image, p_src, &region_roi, camid, result_widthstep, src_widthstep, bev_Table);


#ifdef TIME_CONSUME		
	time_t  time1_2 = clock();
	double time0_1 = (double)(clock() - time1_1);
	printf("front cost :%f\n", time0_1);

///	time1_1 = clock();
#endif
	//front right
	p_src1 = front_image_uyvy;
	p_src2 = right_image_uyvy;
	fusion_pic1 = 0;
	fusion_pic2 = 3;
	region_roi = cvRect1(car_down_right.x + 1, 0, car_up_left.x, car_up_left.y);
	analysis_fusion_region_lut_uyvy(result_image, p_src1, p_src2, fusion_pic1, fusion_pic2, &region_roi, result_widthstep, src_widthstep, bev_Table);

#ifdef TIME_CONSUME		
	time1_2 = clock();
	time0_1 = (double)(clock() - time1_1);
	printf("front right cost :%f\n", time0_1);

	///time1_1 = clock();
#endif
	//left
	region_roi = cvRect1(0, car_up_left.y, car_up_left.x, car_down_right.y - car_up_left.y + 1);
	analysis_single_region_lut_uyvy(result_image, left_image_uyvy, &region_roi, 2, result_widthstep, src_widthstep, bev_Table);

#ifdef TIME_CONSUME		
	time1_2 = clock();
	time0_1 = (double)(clock() - time1_1);
	printf("left cost :%f\n", time0_1);

	///time1_1 = clock();
#endif
//right
	region_roi = cvRect1(car_down_right.x + 1, car_up_left.y, result_width - car_down_right.x - 1, car_down_right.y - car_up_left.y + 1);
	analysis_single_region_lut_uyvy(result_image, right_image_uyvy, &region_roi, 3, result_widthstep, src_widthstep, bev_Table);


#ifdef TIME_CONSUME		
	time1_2 = clock();
	time0_1 = (double)(clock() - time1_1);
	printf("right cost :%f\n", time0_1);
	time1_1 = clock();
#endif	//back left
	p_src1 = left_image_uyvy;
	p_src2 = back_image_uyvy;
	fusion_pic1 = 2;
	fusion_pic2 = 1;
	region_roi = cvRect1(0, car_down_right.y + 1, car_up_left.x, frontback_fov_height);

	analysis_fusion_region_lut_uyvy(result_image, p_src1, p_src2, fusion_pic1, fusion_pic2, &region_roi, result_widthstep, src_widthstep, bev_Table);


#ifdef TIME_CONSUME		
	time1_2 = clock();
	time0_1 = (double)(clock() - time1_1);
	printf("back left cost :%f\n", time0_1);
#endif
	//back
	region_roi = cvRect1(car_up_left.x, car_down_right.y + 1, car_down_right.x - car_up_left.x + 1, frontback_fov_height);
	analysis_single_region_lut_uyvy(result_image, back_image_uyvy, &region_roi, 1, result_widthstep, src_widthstep, bev_Table);

///	time1_1 = clock();
	//back right

	p_src1 = right_image_uyvy;
	p_src2 = back_image_uyvy;
	fusion_pic1 = 3;
	fusion_pic2 = 1;
	region_roi = cvRect1(car_down_right.x + 1, car_down_right.y + 1, car_up_left.x, frontback_fov_height);

	analysis_fusion_region_lut_uyvy(result_image, p_src1, p_src2, fusion_pic1, fusion_pic2, &region_roi, result_widthstep, src_widthstep, bev_Table);


#ifdef TIME_CONSUME		
	time1_2 = clock();
	time0_1 = (double)(clock() - time1_1);
	printf("back right cost :%f\n", time0_1);
#endif	

	return ret;
}




void analysis_single_region_lut_uyvy(
	uchar* result_image, uchar* p_src,
	CvRect1* region_roi,
	Int32_t camid,
	Int32_t result_widthstep,
	Int32_t src_widthstep,
	TabBev **bev_Table[4])
{
	Int32_t yuv[2], x, y, row, col,i,j;
	if (camid == 0 || camid == 2) //0:front , 2:left
	{
		for ( i = region_roi->y; i < region_roi->height + region_roi->y; i++)
		{
			for ( j = region_roi->x; j < region_roi->width + region_roi->x; j++)
			{
				x = bev_Table[camid][i][j].point_pos.x;
				y = bev_Table[camid][i][j].point_pos.y;

				yuv[0] = ((p_src[y * src_widthstep + x * 2 + 1] * bev_Table[camid][i][j].wt_upleft
					+ p_src[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[camid][i][j].wt_upright
					+ p_src[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[camid][i][j].wt_downleft
					+ p_src[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[camid][i][j].wt_downright) >> 16)
					;

				yuv[1] = ((p_src[y * src_widthstep + x * 2]))
					;

				result_image[(i)* result_widthstep + (j)* 2 + 1] += yuv[0];
				result_image[(i)* result_widthstep + (j)* 2] += yuv[1];
			}
		}
	}
	else if (camid == 3)  //right
	{
		for ( i = region_roi->y; i < region_roi->height + region_roi->y; i++)
		{
			for ( j = region_roi->x; j < region_roi->width + region_roi->x; j++)
			{
				//row = i - region_roi->y;
				col = j - region_roi->x;
				x = bev_Table[camid][i][col].point_pos.x;
				y = bev_Table[camid][i][col].point_pos.y;

				yuv[0] = ((p_src[y * src_widthstep + x * 2 + 1] * bev_Table[camid][i][col].wt_upleft
					+ p_src[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[camid][i][col].wt_upright
					+ p_src[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[camid][i][col].wt_downleft
					+ p_src[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[camid][i][col].wt_downright) >> 16)
					;

				yuv[1] = ((p_src[y * src_widthstep + x * 2]))
					;

				result_image[(i)* result_widthstep + (j)* 2 + 1] += yuv[0];
				result_image[(i)* result_widthstep + (j)* 2] += yuv[1];
			}
		}
	}
	else    //back
	{
		for ( i = region_roi->y; i < region_roi->height + region_roi->y; i++)
		{
			for ( j = region_roi->x; j < region_roi->width + region_roi->x; j++)
			{
				row = i - region_roi->y;
				//col = j - region_roi->x;
				x = bev_Table[camid][row][j].point_pos.x;
				y = bev_Table[camid][row][j].point_pos.y;

				yuv[0] = ((p_src[y * src_widthstep + x * 2 + 1] * bev_Table[camid][row][j].wt_upleft
					+ p_src[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[camid][row][j].wt_upright
					+ p_src[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[camid][row][j].wt_downleft
					+ p_src[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[camid][row][j].wt_downright) >> 16)
					;

				yuv[1] = ((p_src[y * src_widthstep + x * 2]))
					;

				result_image[(i)* result_widthstep + (j)* 2 + 1] += yuv[0];
				result_image[(i)* result_widthstep + (j)* 2] += yuv[1];
			}
		}
	}
}

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
	uchar* result_image)
{
	Int32_t ret = 0;
	Int32_t i,j, tmp;
	UInt32_t item;
	Int32_t yuv[3];
	Int32_t source_pos[2]; // y, x
	Int32_t wt_fusion;
	Int32_t wt_upleft, wt_upright, wt_downleft, wt_downright;
	Int32_t src_widthstep, src_nchannels, result_widthstep, result_nchannels;
	UInt64_t wt_item;

	result_widthstep = result_width << 1;
	src_widthstep = source_width << 1;	

	for ( i = 0; i < fov_height; i++)
	{
	tmp = i * fov_width ;
		for ( j = 0; j < fov_width; j++)
		{				
			item = p_pos_fusion_lut[tmp + j];
			wt_item = p_wt_lut[tmp+ j];

			source_pos[0] = item >> 21;
			source_pos[1] = (item >> 10) & 0x7FF;
			
			///source_pos[0] = source_pos[0] < 0 ? 0 : (source_pos[0] > source_height - 1 ? source_height - 1 : source_pos[0]);
			///source_pos[1] = source_pos[1] < 0 ? 0 : (source_pos[1] > source_width - 1 ? source_width - 1 : source_pos[1]);

			wt_fusion = item & 0x3FF;
			
			wt_upleft = (wt_item >> 48) ;
			wt_upright = (wt_item >> 32) & 0xFFFF;
			wt_downleft = (wt_item >> 16) & 0xFFFF;
			wt_downright = wt_item & 0xFFFF;

			yuv[0] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2 + 1] * wt_upleft
				+ p_src[source_pos[0] * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_upright
				+ p_src[(source_pos[0] + 1) * src_widthstep + source_pos[1] * 2 + 1] * wt_downleft
				+ p_src[(source_pos[0] + 1) * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_downright) >> 16)
				*wt_fusion >> 10;

		///	yuv[0] = yuv[0] < 0 ? 0 : (yuv[0] > 255 ? 255 : yuv[0]);

			result_image[(row_start + i) * result_widthstep + (col_start + j) * 2 + 1] += yuv[0];			
			// process u & v channel							
			yuv[1] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
				+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
				* wt_fusion >> 10;
		///	yuv[1] = yuv[1] < 0 ? 0 : (yuv[1] > 255 ? 255 : yuv[1]);

			result_image[(row_start + i) * result_widthstep + (col_start + j) * 2] += yuv[1];			
		}
	}
	return 0;
}





void analysis_fusion_region_lut_uyvy(uchar* result_image,
	uchar* p_src1,
	uchar* p_src2,
	Int32_t fusion_pic1,
	Int32_t fusion_pic2,
	CvRect1* region_roi,
	Int32_t result_widthstep,
	Int32_t src_widthstep,
	TabBev **bev_Table[4])
{
	Int32_t yuv[2], x, y, row, col,i,j;
	if (fusion_pic1 == 0 && fusion_pic2 == 2) //front_left
	{
		for ( i = region_roi->y; i < region_roi->height + region_roi->y; i++)
		{
			for ( j = region_roi->x; j < region_roi->width + region_roi->x; j++)
			{
				x = bev_Table[fusion_pic1][i][j].point_pos.x;
				y = bev_Table[fusion_pic1][i][j].point_pos.y;
				col = j - region_roi->x;
				row = i - region_roi->y;

				if (bev_Table[fusion_pic1][i][j].wt_fusion == 1023)
				{
					yuv[0] = ((p_src1[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upleft
						+ p_src1[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upright
						+ p_src1[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downleft
						+ p_src1[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downright) >> 16)
						;

					yuv[1] = ((p_src1[y * src_widthstep + x * 2]))
						;
				}
				else if (bev_Table[fusion_pic2][row][col].wt_fusion == 1023)
				{
					x = bev_Table[fusion_pic2][row][col].point_pos.x;
					y = bev_Table[fusion_pic2][row][col].point_pos.y;
					yuv[0] = ((p_src2[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upleft
						+ p_src2[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upright
						+ p_src2[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downleft
						+ p_src2[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downright) >> 16)
						;

					yuv[1] = ((p_src2[y * src_widthstep + x * 2]))
						;
				}
				else  // fusion region
				{
					yuv[0] = ((p_src1[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upleft
						+ p_src1[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upright
						+ p_src1[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downleft
						+ p_src1[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downright) >> 16)
						* bev_Table[fusion_pic1][i][j].wt_fusion >> 10;

					yuv[1] = ((p_src1[y * src_widthstep + x * 2]))
						* bev_Table[fusion_pic1][i][j].wt_fusion >> 10;


					x = bev_Table[fusion_pic2][row][col].point_pos.x;
					y = bev_Table[fusion_pic2][row][col].point_pos.y;
					yuv[0] += ((p_src2[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upleft
						+ p_src2[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upright
						+ p_src2[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downleft
						+ p_src2[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downright) >> 16)
						* bev_Table[fusion_pic2][row][col].wt_fusion >> 10;
					yuv[1] += ((p_src2[y * src_widthstep + x * 2]))
						* bev_Table[fusion_pic2][row][col].wt_fusion >> 10;
				}

				result_image[(i)* result_widthstep + (j)* 2 + 1] += yuv[0];
				result_image[(i)* result_widthstep + (j)* 2] += yuv[1];
			}
		}
	}
	else if ((fusion_pic1 == 0 && fusion_pic2 == 3) || (fusion_pic1 == 2 && fusion_pic2 == 1)) //front_right  and  left_back
	{
		for ( i = region_roi->y; i < region_roi->height + region_roi->y; i++)
		{
			for ( j = region_roi->x; j < region_roi->width + region_roi->x; j++)
			{
				if (bev_Table[fusion_pic1][i][j].wt_fusion == 1023)
				{
					x = bev_Table[fusion_pic1][i][j].point_pos.x;
					y = bev_Table[fusion_pic1][i][j].point_pos.y;
					//col = j - region_roi->x;
					//row = i - region_roi->y;

					yuv[0] = ((p_src1[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upleft
						+ p_src1[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upright
						+ p_src1[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downleft
						+ p_src1[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downright) >> 16)
						;

					yuv[1] = ((p_src1[y * src_widthstep + x * 2]))
						;
				}
				else if (bev_Table[fusion_pic1][i][j].wt_fusion == 0)
				{
					col = j - region_roi->x;
					row = i - region_roi->y;
					x = bev_Table[fusion_pic2][row][col].point_pos.x;
					y = bev_Table[fusion_pic2][row][col].point_pos.y;

					yuv[0] = ((p_src2[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upleft
						+ p_src2[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upright
						+ p_src2[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downleft
						+ p_src2[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downright) >> 16)
						;

					yuv[1] = ((p_src2[y * src_widthstep + x * 2]))
						;
				}
				else
				{
					x = bev_Table[fusion_pic1][i][j].point_pos.x;
					y = bev_Table[fusion_pic1][i][j].point_pos.y;
					yuv[0] = ((p_src1[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upleft
						+ p_src1[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_upright
						+ p_src1[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downleft
						+ p_src1[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][j].wt_downright) >> 16)
						* bev_Table[fusion_pic1][i][j].wt_fusion >> 10;

					yuv[1] = ((p_src1[y * src_widthstep + x * 2]))
						* bev_Table[fusion_pic1][i][j].wt_fusion >> 10;

					col = j - region_roi->x;
					row = i - region_roi->y;
					x = bev_Table[fusion_pic2][row][col].point_pos.x;
					y = bev_Table[fusion_pic2][row][col].point_pos.y;
					yuv[0] += ((p_src2[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upleft
						+ p_src2[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_upright
						+ p_src2[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downleft
						+ p_src2[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][col].wt_downright) >> 16)
						* bev_Table[fusion_pic2][row][col].wt_fusion >> 10;
					yuv[1] += ((p_src2[y * src_widthstep + x * 2]))
						* bev_Table[fusion_pic2][row][col].wt_fusion >> 10;
				}

				result_image[(i)* result_widthstep + (j)* 2 + 1] += yuv[0];
				result_image[(i)* result_widthstep + (j)* 2] += yuv[1];
			}
		}
	}
	else   //right_back
	{
		for ( i = region_roi->y; i < region_roi->height + region_roi->y; i++)
		{
			for ( j = region_roi->x; j < region_roi->width + region_roi->x; j++)
			{
				if (bev_Table[fusion_pic1][i][col = j - region_roi->x].wt_fusion == 1023)
				{
					col = j - region_roi->x;
					//row = i - region_roi->y;
					x = bev_Table[fusion_pic1][i][col].point_pos.x;
					y = bev_Table[fusion_pic1][i][col].point_pos.y;

					yuv[0] = ((p_src1[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_upleft
						+ p_src1[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_upright
						+ p_src1[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_downleft
						+ p_src1[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_downright) >> 16)
						;

					yuv[1] = ((p_src1[y * src_widthstep + x * 2]))
						;
				}
				else if (bev_Table[fusion_pic1][i][col = j - region_roi->x].wt_fusion == 0)
				{
					//col = j - region_roi->x;
					row = i - region_roi->y;
					x = bev_Table[fusion_pic2][row][j].point_pos.x;
					y = bev_Table[fusion_pic2][row][j].point_pos.y;

					yuv[0] = ((p_src2[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_upleft
						+ p_src2[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_upright
						+ p_src2[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_downleft
						+ p_src2[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_downright) >> 16)
						;

					yuv[1] = ((p_src2[y * src_widthstep + x * 2]))
						;
				}
				else
				{
					col = j - region_roi->x;
					x = bev_Table[fusion_pic1][i][col].point_pos.x;
					y = bev_Table[fusion_pic1][i][col].point_pos.y;
					yuv[0] = ((p_src1[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_upleft
						+ p_src1[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_upright
						+ p_src1[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_downleft
						+ p_src1[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic1][i][col].wt_downright) >> 16)
						* bev_Table[fusion_pic1][i][col].wt_fusion >> 10;

					yuv[1] = ((p_src1[y * src_widthstep + x * 2]))
						* bev_Table[fusion_pic1][i][col].wt_fusion >> 10;

					//col = j - region_roi->x;
					row = i - region_roi->y;
					x = bev_Table[fusion_pic2][row][j].point_pos.x;
					y = bev_Table[fusion_pic2][row][j].point_pos.y;
					yuv[0] += ((p_src2[y * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_upleft
						+ p_src2[y * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_upright
						+ p_src2[(y + 1) * src_widthstep + x * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_downleft
						+ p_src2[(y + 1) * src_widthstep + (x + 1) * 2 + 1] * bev_Table[fusion_pic2][row][j].wt_downright) >> 16)
						* bev_Table[fusion_pic2][row][j].wt_fusion >> 10;
					yuv[1] += ((p_src2[y * src_widthstep + x * 2]))
						* bev_Table[fusion_pic2][row][j].wt_fusion >> 10;
				}

				result_image[(i)* result_widthstep + (j)* 2 + 1] += yuv[0];
				result_image[(i)* result_widthstep + (j)* 2] += yuv[1];
			}
		}
	}
}

CvRect1  cvRect1( int x, int y, int width, int height )
{
	CvRect1 r;

	r.x = x;
	r.y = y;
	r.width = width;
	r.height = height;

	return r;
}

#if 0 //32bit wtlut
Int32_t analysis_panorama_lut_uyvy(UInt32_t* p_lut_front_test,
	UInt32_t* p_lut_back_test,
	UInt32_t* p_lut_left_test,
	UInt32_t* p_lut_right_test,
	UInt32_t* p_wt_front_test,
	UInt32_t* p_wt_back_test,
	UInt32_t* p_wt_left_test,
	UInt32_t* p_wt_right_test,
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
	Int32_t source_width)
{
	Int32_t ret = 0;

	
	uchar* p_src;
	UInt32_t* p_pos_lut;
	UInt32_t* p_wt_lut;
	Int32_t fov_height, fov_width;
	Int32_t dst_height, dst_width;
	Int32_t src_height, src_width;
	Int32_t row_start, col_start;
	uchar* p_dst = result_image;
	memset(p_dst, 0, SVM_WIDTH * SVM_HEIGHT * 2);
	src_height = source_height;
	src_width = source_width;
	dst_height = result_height;
	dst_width = result_width;

	// process front fov
	p_src = front_image_uyvy;
	p_pos_lut = p_lut_front_test;
	p_wt_lut = p_wt_front_test;
	fov_height = frontback_fov_height;
	fov_width = result_width;
	row_start = 0;
	col_start = 0;
	analysis_single_fov_lut_uyvy(fov_height,
		fov_width,
		p_pos_lut,
		p_wt_lut,
		p_src,
		row_start,
		col_start,
		src_height,
		src_width,
		dst_height,
		dst_width,
		p_dst);

	// process back fov
	p_src = back_image_uyvy;
	p_pos_lut = p_lut_back_test;
	p_wt_lut = p_wt_back_test;
	fov_height = frontback_fov_height;
	fov_width = result_width;
	row_start = car_down_right.y + 1;
	col_start = 0;
	analysis_single_fov_lut_uyvy(fov_height,
		fov_width,
		p_pos_lut,
		p_wt_lut,
		p_src,
		row_start,
		col_start,
		src_height,
		src_width,
		dst_height,
		dst_width,
		p_dst);

	// process left fov
	p_src = left_image_uyvy;
	p_pos_lut = p_lut_left_test;
	p_wt_lut = p_wt_left_test;
	fov_height = result_height;
	fov_width = leftright_fov_width;
	row_start = 0;
	col_start = 0;
	analysis_single_fov_lut_uyvy(fov_height,
		fov_width,
		p_pos_lut,
		p_wt_lut,
		p_src,
		row_start,
		col_start,
		src_height,
		src_width,
		dst_height,
		dst_width,
		p_dst);

	// process right fov
	p_src = right_image_uyvy;
	p_pos_lut = p_lut_right_test;
	p_wt_lut = p_wt_right_test;
	fov_height = result_height;
	fov_width = leftright_fov_width;
	row_start = 0;
	col_start = car_down_right.x + 1;

	analysis_single_fov_lut_uyvy(fov_height,
		fov_width,
		p_pos_lut,
		p_wt_lut,
		p_src,
		row_start,
		col_start,
		src_height,
		src_width,
		dst_height,
		dst_width,
		p_dst);


	return ret;
}
Int32_t analysis_single_fov_lut_uyvy(Int32_t fov_height,
	Int32_t fov_width,
	UInt32_t* p_pos_fusion_lut,
	UInt32_t* p_wt_lut,
	uchar* p_src,
	Int32_t row_start,
	Int32_t col_start,
	Int32_t source_height,
	Int32_t source_width,
	Int32_t result_height,
	Int32_t result_width,
	uchar* result_image)
{
	Int32_t  ret = 0;
	Int32_t  i ,j;
	UInt32_t item;
	Int32_t yuv[3];
	Int32_t source_pos[2]; // y, x
	Int32_t wt_fusion;
	Int32_t wt_upleft, wt_upright, wt_downleft, wt_downright;
	Int32_t src_widthstep, src_nchannels, result_widthstep, result_nchannels;
	UInt32_t wt_item;

	result_widthstep = result_width << 1;
	src_widthstep = source_width << 1;
	for ( i = 0; i < fov_height; i++)
	{
	
		for ( j = 0; j < fov_width; j++)
		{				
			item = p_pos_fusion_lut[i * fov_width + j];
			wt_fusion = item & 0x3FF;

			source_pos[0] = item >> 21;
			source_pos[1] = (item >> 10) & 0x7FF;
			
			//source_pos[0] = source_pos[0] < 0 ? 0 : (source_pos[0] > source_height - 1 ? source_height - 1 : source_pos[0]);
			//source_pos[1] = source_pos[1] < 0 ? 0 : (source_pos[1] > source_width - 1 ? source_width - 1 : source_pos[1]);

			
			wt_item = p_wt_lut[2 * i * fov_width + 2 * j];
			wt_upleft = wt_item >> 16;
			wt_upright = wt_item & 0xFFFF;
			wt_item = p_wt_lut[2 * i * fov_width + 2 * j + 1];
			wt_downleft = wt_item >> 16;
			wt_downright = wt_item & 0xFFFF;


			yuv[0] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2 + 1] * wt_upleft
				+ p_src[source_pos[0] * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_upright
				+ p_src[(source_pos[0] + 1) * src_widthstep + source_pos[1] * 2 + 1] * wt_downleft
				+ p_src[(source_pos[0] + 1) * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_downright) >> 16)
				*wt_fusion >> 10;

			yuv[0] = yuv[0] < 0 ? 0 : (yuv[0] > 255 ? 255 : yuv[0]);

			result_image[(row_start + i) * result_widthstep + (col_start + j) * 2 + 1] += yuv[0];

			// process u & v channel
			//\u5f53\u524d\u8fd0\u7b97\u7684\u6e90\u56fe\u548c\u76ee\u6807\u56fe\u4e0d\u540c\u4e3a\u5947/\u5076\u5217\u65f6\uff0c\u53d6\u539f\u56fe\u4e0b\u4e00\u5217\u6c42uv\u503c
			if (((j^source_pos[1]) & 1) == 1)
			{
				source_pos[1] += 1;
			}

		///	if (j % 2 == 0)
			{
				
				yuv[1] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[1] = yuv[1] < 0 ? 0 : (yuv[1] > 255 ? 255 : yuv[1]);
				result_image[(row_start + i) * result_widthstep + (col_start + j) * 2] += yuv[1];
			}
		#if 0
			else
			{
				yuv[2] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[2] = yuv[2] < 0 ? 0 : (yuv[2] > 255 ? 255 : yuv[2]);
				result_image[(row_start + i) * result_widthstep + (col_start + j) * 2] += yuv[2];
			}
			#endif 
		}
	}
	return 0;
}
#endif
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
	int source_width)
{
	int ret = 0;
	int i,j;
	unsigned int  item;
	int source_pos[2]; // y, x
	int wt_up, wt_left, wt_fusion;
	int wt_upleft, wt_upright, wt_downleft, wt_downright;
	int src_widthstep,  result_widthstep;//result_nchannels,src_nchannels;
	int yuv[3];
///	FILE* pf_dst;
	unsigned char* p_src;
	unsigned int* p_lut;

	result_widthstep = result_width << 1;
	src_widthstep = source_width << 1;
///	unsigned char* p_dst = result_image;

	car_up_left.x = 352;
	car_up_left.y = 396;
	car_down_right.x = 479;
	car_down_right.y = 627;

	memset(result_image, 0, SVM_WIDTH * SVM_HEIGHT * 2);
	// process front fov
	p_src = front_image_uyvy;
	p_lut = p_lut_front_test;
	for ( i = 0; i < front_fov_height; i++)
	{
		for ( j = 0; j < result_width; j++)
		{
			item = p_lut[2*i * result_width + 2*j];

			source_pos[0] = item >>  21;
			source_pos[1] = (item >>  10) & 0x7FF;

			source_pos[0] = source_pos[0] < 0 ? 0 : (source_pos[0] > source_height - 1 ? source_height - 1 : source_pos[0]);
			source_pos[1] = source_pos[1] < 0 ? 0 : (source_pos[1] > source_width - 1 ? source_width - 1 : source_pos[1]);

			wt_fusion = (item)  & 0x3FF;

			item = p_lut[2 * i * result_width + 2*j+1];

			wt_up = (item >> 24) & 0xFF;
			wt_left = (item >> 16) & 0xFF;
			wt_upleft = (255 - wt_up) * (255 - wt_left);
			wt_upright = (255 - wt_up) * wt_left;
			wt_downleft = wt_up * (255 - wt_left);
			wt_downright = 65535 - wt_upleft - wt_upright - wt_downleft;

			yuv[0] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2 + 1] * wt_upleft
				+ p_src[source_pos[0] * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_upright
				+ p_src[(source_pos[0] + 1) * src_widthstep + source_pos[1] * 2 + 1] * wt_downleft
				+ p_src[(source_pos[0] + 1) * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_downright) >> 16)
				*wt_fusion >> 10;

			yuv[0] = yuv[0] < 0 ? 0 : (yuv[0] > 255 ? 255 : yuv[0]);

			result_image[i * result_widthstep + j * 2 + 1] += yuv[0];

			// process u & v channel
			if (j % 2 == 0)
			{
				yuv[1] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[1] = yuv[1] < 0 ? 0 : (yuv[1] > 255 ? 255 : yuv[1]);
				result_image[i * result_widthstep + j * 2] += yuv[1];
			}
			else
			{
				yuv[2] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[2] = yuv[2] < 0 ? 0 : (yuv[2] > 255 ? 255 : yuv[2]);
				result_image[i * result_widthstep + j * 2] += yuv[2];
			}
		}
	}

	// process back fov
	p_src = back_image_uyvy;
	p_lut = p_lut_back_test;
	for ( i = 0; i < back_fov_height; i++)
	{
		for ( j = 0; j < result_width; j++)
		{
			item = p_lut[2*i * result_width +2*j];

			source_pos[0] = item >> 21;
			source_pos[1] = (item >> 10) & 0x7FF;

			source_pos[0] = source_pos[0] < 0 ? 0 : (source_pos[0] > source_height - 1 ? source_height - 1 : source_pos[0]);
			source_pos[1] = source_pos[1] < 0 ? 0 : (source_pos[1] > source_width - 1 ? source_width - 1 : source_pos[1]);

			wt_fusion = (item) & 0x3FF;

			item = p_lut[2 * i * result_width + 2*j+1];

			wt_up = (item >> 24) & 0xFF;
			wt_left = (item >> 16) & 0xFF;
			wt_upleft = (255 - wt_up) * (255 - wt_left);
			wt_upright = (255 - wt_up) * wt_left;
			wt_downleft = wt_up * (255 - wt_left);
			wt_downright = 65535 - wt_upleft - wt_upright - wt_downleft;

			yuv[0] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2 + 1] * wt_upleft
				+ p_src[source_pos[0] * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_upright
				+ p_src[(source_pos[0] + 1) * src_widthstep + source_pos[1] * 2 + 1] * wt_downleft
				+ p_src[(source_pos[0] + 1) * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_downright) >> 16)
				*wt_fusion >> 10;

			yuv[0] = yuv[0] < 0 ? 0 : (yuv[0] > 255 ? 255 : yuv[0]);

			result_image[(car_down_right.y + 1 + i) * result_widthstep + j * 2 + 1] += yuv[0];

			// process u & v channel
			if (j % 2 == 0)
			{
				yuv[1] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[1] = yuv[1] < 0 ? 0 : (yuv[1] > 255 ? 255 : yuv[1]);
				result_image[(car_down_right.y + 1 + i) * result_widthstep + j * 2] += yuv[1];
			}
			else
			{
				yuv[2] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[2] = yuv[2] < 0 ? 0 : (yuv[2] > 255 ? 255 : yuv[2]);
				result_image[(car_down_right.y + 1 + i) * result_widthstep + j * 2] += yuv[2];
			}
		}
	}
	// process left fov

	p_src = left_image_uyvy;
	p_lut = p_lut_left_test;
	for ( i = 0; i < result_height; i++)
	{
		for ( j = 0; j < left_fov_width; j++)
		{
			item = p_lut[2*i * left_fov_width + 2*j];

			source_pos[0] = item >>  21;
			source_pos[1] = (item >> 10) & 0x7FF;

			source_pos[0] = source_pos[0] < 0 ? 0 : (source_pos[0] > source_height - 1 ? source_height - 1 : source_pos[0]);
			source_pos[1] = source_pos[1] < 0 ? 0 : (source_pos[1] > source_width - 1 ? source_width - 1 : source_pos[1]);

			wt_fusion = (item ) & 0x3FF;

			item = p_lut[2 * i * left_fov_width + 2*j + 1];

			wt_up = (item >> 24) & 0xFF;
			wt_left = (item >> 16) & 0xFF;
			wt_upleft = (255 - wt_up) * (255 - wt_left);
			wt_upright = (255 - wt_up) * wt_left;
			wt_downleft = wt_up * (255 - wt_left);
			wt_downright = 65535 - wt_upleft - wt_upright - wt_downleft;

			yuv[0] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2 + 1] * wt_upleft
				+ p_src[source_pos[0] * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_upright
				+ p_src[(source_pos[0] + 1) * src_widthstep + source_pos[1] * 2 + 1] * wt_downleft
				+ p_src[(source_pos[0] + 1) * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_downright) >> 16)
				*wt_fusion >> 10;

			yuv[0] = yuv[0] < 0 ? 0 : (yuv[0] > 255 ? 255 : yuv[0]);

			result_image[i * result_widthstep + j * 2 + 1] += yuv[0];

			// process u & v channel
			if (j % 2 == 0)
			{
				yuv[1] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[1] = yuv[1] < 0 ? 0 : (yuv[1] > 255 ? 255 : yuv[1]);
				result_image[i * result_widthstep + j * 2] += yuv[1];
			}
			else
			{
				yuv[2] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[2] = yuv[2] < 0 ? 0 : (yuv[2] > 255 ? 255 : yuv[2]);
				result_image[i * result_widthstep + j * 2] += yuv[2];
			}
		}
	}

	// process right fov
	p_src = right_image_uyvy;
	p_lut = p_lut_right_test;
	for ( i = 0; i < result_height; i++)
	{
		for ( j = 0; j < right_fov_width; j++)
		{
			item = p_lut[2*i * right_fov_width + 2*j];

			source_pos[0] = item >>  21;
			source_pos[1] = (item >>  10) & 0x7FF;

			source_pos[0] = source_pos[0] < 0 ? 0 : (source_pos[0] > source_height - 1 ? source_height - 1 : source_pos[0]);
			source_pos[1] = source_pos[1] < 0 ? 0 : (source_pos[1] > source_width - 1 ? source_width - 1 : source_pos[1]);

			wt_fusion = (item ) & 0x3FF;

			item = p_lut[2 * i * right_fov_width + 2*j + 1];

			wt_up = (item >> 24) & 0xFF;
			wt_left = (item >> 16) & 0xFF;
			wt_upleft = (255 - wt_up) * (255 - wt_left);
			wt_upright = (255 - wt_up) * wt_left;
			wt_downleft = wt_up * (255 - wt_left);
			wt_downright = 65535 - wt_upleft - wt_upright - wt_downleft;

			yuv[0] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2 + 1] * wt_upleft
				+ p_src[source_pos[0] * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_upright
				+ p_src[(source_pos[0] + 1) * src_widthstep + source_pos[1] * 2 + 1] * wt_downleft
				+ p_src[(source_pos[0] + 1) * src_widthstep + (source_pos[1] + 1) * 2 + 1] * wt_downright) >> 16)
				*wt_fusion >> 10;

			yuv[0] = yuv[0] < 0 ? 0 : (yuv[0] > 255 ? 255 : yuv[0]);

			result_image[i * result_widthstep + (j + car_down_right.x + 1) * 2 + 1] += yuv[0];

			// process u & v channel
			if (j % 2 == 0)
			{
				yuv[1] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[1] = yuv[1] < 0 ? 0 : (yuv[1] > 255 ? 255 : yuv[1]);
				result_image[i * result_widthstep + (j + car_down_right.x + 1) * 2] += yuv[1];
			}
			else
			{
				yuv[2] = ((p_src[source_pos[0] * src_widthstep + source_pos[1] * 2] * (wt_upleft + wt_upright)
					+ p_src[(source_pos[0] + 1)* src_widthstep + source_pos[1] * 2] * (wt_downleft + wt_downright)) >> 16)
					* wt_fusion >> 10;
				yuv[2] = yuv[2] < 0 ? 0 : (yuv[2] > 255 ? 255 : yuv[2]);
				result_image[i * result_widthstep + (j + car_down_right.x + 1) * 2] += yuv[2];
			}
		}
	}
	// save result 
////	pf_dst = fopen("E:/Temp/panorama_result.yuv", "wb");
///	fwrite(result_image, sizeof(uchar), Width *  Height * 2, pf_dst);
///	fclose(pf_dst);

	return ret;
}

#endif //xutao


