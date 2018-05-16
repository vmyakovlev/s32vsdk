/*
 * Copyright (c) 2015-2016 Freescale Semiconductor
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
* \file     sony_config_DOL2.h
* \brief    definition of sony camera registers configuration for DOL2
* \author   Tomas Babinec
* \version  0.1
* \date     27.11.2015
* \note
****************************************************************************/

#ifndef SONYCONFIGDOL2_H
#define SONYCONFIGDOL2_H



#define FPS_DOL 30     


#define ROI_1288x728_DOL




#ifdef ROI_1288x808_DOL
#define SONY_CROP_RES_DOL 1288x808
#define SONY_CROPPING_DOL (0x40)
#define SONY_YSIZE_DOL 861
#define WINPVL_DOL ((SONY_YSIZE-809)&0xff)
#define WINPVH_DOL (((SONY_YSIZE-809)>>8)&0xff)
#else

#ifdef ROI_1288x728_DOL
#define SONY_CROP_RES_DOL 1288x728
#define SONY_CROPPING_DOL (0x40)

#define SONY_YSIZE_DOL (729+34+16)
#define SONY_YPOS_DOL ((977-SONY_YSIZE_DOL)>>1)
#define WINPVL_DOL ((SONY_YSIZE_DOL-729)&0xff)
#define WINPVH_DOL (((SONY_YSIZE_DOL-729)>>8)&0xff)

#else 
#define SONY_CROPPING_DOL (0x00)
#define SONY_YSIZE_DOL 1100

#endif  
#endif  
#define VMAXL_DOL (SONY_YSIZE_DOL & 0xff)
#define VMAXH_DOL ((SONY_YSIZE_DOL >>8) & 0xff)
#define HMAXL_DOL (((uint32_t)(((30.0*2250*1100)/SONY_YSIZE_DOL)/FPS_DOL)) & 0xff)
#define HMAXH_DOL ((((uint32_t)(((30.0*2250*1100)/SONY_YSIZE_DOL)/FPS_DOL)) >> 8) & 0xff)

#define CHIP_ID_02_DOL 0x30
#define CHIP_ID_03_DOL 0x31
#define CHIP_ID_04_DOL 0x32
#define CHIP_ID_05_DOL 0x33

static uint8_t Sony_Table_DOL[] = 
{
    CHIP_ID_02_DOL, 0x00, 0x01,  
    CHIP_ID_02_DOL, 0x03, 0x01,  
    CHIP_ID_02_DOL, 0x03, 0x00,  
    CHIP_ID_02_DOL, 0x00, 0x01,  
    CHIP_ID_02_DOL, 0x01, 0x01,  
 
    CHIP_ID_02_DOL, 0x02, 0x00,  
    CHIP_ID_02_DOL, 0x05, 0x01,  
    CHIP_ID_02_DOL, 0x07, 0x43,  
    CHIP_ID_02_DOL, 0x06, 0x00,  
    CHIP_ID_02_DOL, 0x09, 0x11,  
    CHIP_ID_02_DOL, 0x0C, 0x11,  
    CHIP_ID_02_DOL, 0x0F, 0x00,  
    CHIP_ID_02_DOL, 0x12, 0x2C,  
    CHIP_ID_02_DOL, 0x13, 0x01,  
    CHIP_ID_02_DOL, 0x16, 0x09,  
    
    CHIP_ID_02_DOL, 0x14,  0,    
    CHIP_ID_02_DOL, 0x15,  0,    
    
    CHIP_ID_02_DOL, 0x18, 0x0B,  
    CHIP_ID_02_DOL, 0x19, 0x03,  
    
    CHIP_ID_02_DOL, 0x1B, HMAXL_DOL,  
    CHIP_ID_02_DOL, 0x1C, HMAXH_DOL,  
    
    CHIP_ID_02_DOL, 0x1D, 0xC2,  
    
    CHIP_ID_02_DOL, 0x20, 0x04,  
    CHIP_ID_02_DOL, 0x21, 0x00,  
    CHIP_ID_02_DOL, 0x22, 0x00,  
    CHIP_ID_02_DOL, 0x23, 0x95,  
    CHIP_ID_02_DOL, 0x24, 0x00,  
    CHIP_ID_02_DOL, 0x25, 0x00,  
    
    CHIP_ID_02_DOL, 0x2C, 0x31,  
    
        
    CHIP_ID_02_DOL, 0x36, 0x02,   

    CHIP_ID_02_DOL, 0x38, SONY_YPOS_DOL,  
    CHIP_ID_02_DOL, 0x3A, 0xD9,  
    CHIP_ID_02_DOL, 0x3B, 0x02,  
    
    CHIP_ID_02_DOL, 0x3C, 0x0A,  
    CHIP_ID_02_DOL, 0x3E, 0x08,  
    CHIP_ID_02_DOL, 0x3F, 0x05,  
    
    CHIP_ID_02_DOL, 0x49, 0x0A,  
    CHIP_ID_02_DOL, 0x54, 0x66,  
    
    CHIP_ID_02_DOL, 0x5D, 0x00,  
    CHIP_ID_02_DOL, 0x5F, 0x00,  
    
    CHIP_ID_02_DOL, 0x70, 0x02,  
    CHIP_ID_02_DOL, 0x71, 0x01,  
    CHIP_ID_02_DOL, 0x9E, 0x22,  
    CHIP_ID_02_DOL, 0xA5, 0xFB,  
    CHIP_ID_02_DOL, 0xA6, 0x02,  
    CHIP_ID_02_DOL, 0xB3, 0xFF,  
    CHIP_ID_02_DOL, 0xB4, 0x01,  
    CHIP_ID_02_DOL, 0xB5, 0x42,  
    CHIP_ID_02_DOL, 0xB8, 0x10,  
    
    CHIP_ID_02_DOL, 0xC2, 0x01,  
    
    CHIP_ID_03_DOL, 0x08, 0x01,  
    CHIP_ID_03_DOL, 0x09, 0x01,  
    CHIP_ID_03_DOL, 0x0F, 0x0F,  
    CHIP_ID_03_DOL, 0x10, 0x0E,  
    CHIP_ID_03_DOL, 0x11, 0xE7,  
    CHIP_ID_03_DOL, 0x12, 0x9C,  
    CHIP_ID_03_DOL, 0x13, 0x83,  
    CHIP_ID_03_DOL, 0x14, 0x10,  
    CHIP_ID_03_DOL, 0x15, 0x42,  
    CHIP_ID_03_DOL, 0x28, 0x1E,  
    CHIP_ID_03_DOL, 0xED, 0x38,  
    
    CHIP_ID_04_DOL, 0x0c, 0xCF, 
    CHIP_ID_04_DOL, 0x4C, 0x40,  
    CHIP_ID_04_DOL, 0x4D, 0x03,  
    CHIP_ID_04_DOL, 0x61, 0xE0,  
    CHIP_ID_04_DOL, 0x62, 0x02,  
    CHIP_ID_04_DOL, 0x6E, 0x2F,  
    CHIP_ID_04_DOL, 0x6F, 0x30,
    CHIP_ID_04_DOL, 0x70, 0x03,
    CHIP_ID_04_DOL, 0x98, 0x00,
    CHIP_ID_04_DOL, 0x9A, 0x12,
    CHIP_ID_04_DOL, 0x9B, 0xF1,
    CHIP_ID_04_DOL, 0x9C, 0x0C,


    CHIP_ID_05_DOL, 0x44, 0x00,  
    CHIP_ID_05_DOL, 0x46, 0x01,  
    
    CHIP_ID_05_DOL, 0x57, 0x00,   
    CHIP_ID_05_DOL, 0x58, 0x06,   
    
    CHIP_ID_05_DOL, 0x6B, 0x57,  
    CHIP_ID_05_DOL, 0x7F, 0x01,  
    CHIP_ID_05_DOL, 0x81, 0x1B,  
    CHIP_ID_05_DOL, 0x82, 0x6F,  
    CHIP_ID_05_DOL, 0x83, 0x27,  
    CHIP_ID_05_DOL, 0x84, 0x4F,  
    CHIP_ID_05_DOL, 0x85, 0x2F,  
    CHIP_ID_05_DOL, 0x86, 0x2F,  
    CHIP_ID_05_DOL, 0x87, 0x2F,  
    CHIP_ID_05_DOL, 0x88, 0x9F,  
    CHIP_ID_05_DOL, 0x89, 0x37,  
    CHIP_ID_05_DOL, 0x8D, 0x3D,  
    CHIP_ID_05_DOL, 0x8E, 0x01,  
/*
#else
    0x33, 0x44, 0x00,  
    0x33, 0x46, 0x01,  
    0x33, 0x6B, 0x37,  
    0x33, 0x7F, 0x01,  
    0x33, 0x82, 0x5F,  
    0x33, 0x83, 0x17,  
    0x33, 0x84, 0x37,  
    0x33, 0x85, 0x17,  
    0x33, 0x86, 0x17,  
    0x33, 0x87, 0x17,  
    0x33, 0x88, 0x4F,  
    0x33, 0x89, 0x27,  
    0x33, 0x81, 0x1B,  
    0x33, 0x8D, 0x3D,  
    0x33, 0x8E, 0x01,  
    
    CHIP_ID_05_DOL, 0x57, 0x00,   
    CHIP_ID_05_DOL, 0x58, 0x06,   
#endif
*/

    CHIP_ID_02_DOL, 0x01, 0x00,  

};

/* 
static uint8_t Sony_Flip_Table_DOL[] = {

}; 

static uint8_t Sony_ConvH_Table_DOL[] = {
}; 

static uint8_t Sony_ConvL_Table_DOL[] = {
}; 

static uint8_t Sony_6dbAgain_Table_DOL[] = {
  CHIP_ID_02_DOL, 0x15, 0x00,      
  CHIP_ID_02_DOL, 0x14, 0x3c,
  CHIP_ID_02_DOL, 0x21, 0x00,      
  CHIP_ID_02_DOL, 0x20, 0x8b
}; 
#endif 
*/

#endif /* SONYCONFIGDOL2_H */
