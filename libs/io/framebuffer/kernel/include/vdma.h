/*****************************************************************************
* 
* NXP Confidential Proprietary
*
* Copyright (c) 2014-2016 Freescale Semiconductor
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
#ifndef VDMA_H
#define VDMA_H

/**
 * VDMA Register count
 */
#define VDMA_REG_COUNT 			64

/**
 * VDMA base address
 */
#define VDMA_BASE_ADDRESS 		0x40000000

/**
 * VDMA buffer base address
 */
#define VDMA_BUFFER_BASE_ADDRESS 	0x30000000

/**
 * VDMA buffer size
 */
#define VDMA_BUFFER_SIZE 		0x5EEC00
//#define VDMA_BUFFER_SIZE 		0x2A3000

/**
 * VDMA buffer width
 */
#define VDMA_BUFFER_WIDTH 		1920
//#define VDMA_BUFFER_WIDTH 		1280

/**
 * VDMA buffer width in RGB
 */
#define VDMA_BUFFER_WIDTH3 (VDMA_BUFFER_WIDTH*3)

/**
 * VDMA buffer height
 */
#define VDMA_BUFFER_HEIGHT 		1080
//#define VDMA_BUFFER_HEIGHT 		720

/**
 * VDMA buffer pixel width
 */
#define VDMA_BUFFER_PIXEL_WIDTH 	1

/**
 * VDMA buffer element width
 */
#define VDMA_BUFFER_ELEMENT_WIDTH	3

/**
 * VDMA buffer 0 offset from base
 */
#define VDMA_BUFFER_0_OFFSET 		0

/**
 * VDMA buffer 1 offset from base
 */
#define VDMA_BUFFER_1_OFFSET 		0x5EEC00
//#define VDMA_BUFFER_1_OFFSET 		0x2A3000

/**
 * VDMA initialized flag True
 */
#define VDMA_INITIALIZED 		1

/**
 * VDMA initialized flag False
 */
#define VDMA_UNINITIALIZED 		-1

/******************************************************************************/

/**
 * Initialization structure for VDMA
 */
typedef struct
{
    volatile unsigned int  vdmaPhysBaseAddr; 	// physical base address
    volatile unsigned int* vdmaVirtBaseAddr;	// virtual base address
    int width;				// buffer width
    int height;				// buffer height
    int pixelLength;			// pixel size
    int fbLength;			// buffer length
} framebuffer_axivdma_info;

/******************************************************************************/
/**
 * VDMA Initialization (virtual base allocation...)
 * @param info Pointer to framebuffer init structure
 * @param width Frame buffer width
 * @param height Frame buffer Height
 * @param pixelLength Pixel length
 */
int framebuffer_axivdma_init(framebuffer_axivdma_info *info, 
			     int width, 
			     int height, 
			     int pixelLength);

/**
 * Function clears (zeroes) a buffer specified with i
 * @param i Buffer specified
 */
void framebuffer_axivdma_clear_buffer(int i);

/**
 * True if axivfma is initialized
 */
int framebuffer_axivdma_is_init(void);

/**
 * Free all resources
 * @param info Pointer to framebuffer init structure
 */
void framebuffer_axivdma_uninit(framebuffer_axivdma_info *info);

/**
 * Returns a register value
 * @param info Pointer to framebuffer init structure
 * @param num Register index
 */
unsigned int framebuffer_axivdma_get(framebuffer_axivdma_info *info, int num);

/**
 * Sets a register value
 * @param info Pointer to framebuffer init structure
 * @param num * Register index
 * @param val Value to be set
 */
void framebuffer_axivdma_set(framebuffer_axivdma_info *info, 
			     int num, 
			     unsigned int val);

/**
 * Start and set a vdma mm2s transfer
 * @param info Pointer to framebuffer init structure
 * @param adr1 Frame buffer 0 address
 * @param adr1 Frame buffer 1 address
 * @param circular Is transfer circular?
 */
void framebuffer_axivdma_out_start(framebuffer_axivdma_info *info, 
				   unsigned int adr1, 
				   unsigned int adr2, 
				   int circular);

/**
 * Start and set a vdma s2mm transfer
 * @param info Pointer to framebuffer init structure
 * @param adr1 Frame buffer 0 address
 * @param adr1 Frame buffer 1 address
 */
void framebuffer_axivdma_start(framebuffer_axivdma_info *info, 
			       unsigned int adr1, 
			       unsigned int adr2);

/**
 * Is VDMA running?
 * @param info Pointer to framebuffer init structure
 */
int framebuffer_axivdma_is_running(framebuffer_axivdma_info *info);

/**
 * Is VDMA transfer done?
 * @param info Pointer to framebuffer init structure
 */
int framebuffer_axivdma_is_done(framebuffer_axivdma_info *info);

#endif /* VDMA_H */