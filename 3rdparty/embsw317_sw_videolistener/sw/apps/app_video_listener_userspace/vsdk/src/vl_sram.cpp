/*
  Copyright (c) 2016 NXP Semiconductors
  All Rights Reserved.
  
  This file contains sample code only.
  
  THIS SOFTWARE IS PROVIDED BY NXP "AS IS" AND ANY EXPRESSED OR
  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL NXP OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
  IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
  THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "isp_sram.h"
#include "sram_ioctl.h"
#include "vl_sram.h"

//***************************************************************************
//                            PRECOMPILE OPTIONS
//***************************************************************************
#define SRAM_TO_BE_RELEASED_COUNT 2

//***************************************************************************
//                             LOCAL VARIABLES
//***************************************************************************
/* SRAM driver file descriptor */
static int SramFD = -1;
/* Table of allocated SRAM regions. Items with zero are not used. */
static uint64_t au64SramToBeReleased[SRAM_TO_BE_RELEASED_COUNT] = {0U};

//***************************************************************************
//                             GLOBAL FUNCTIONS
//***************************************************************************
int SramInitAllocator(const char *szDrvName) {
    int RetVal = EXIT_SUCCESS;
    uint8_t u8I;

    /*  Open the SRAM allocator driver */
    SramFD = open(szDrvName, 0);
    if (SramFD < 0) {
        printf("[app] Error: Unable to open SRAM device %s: %s\n", szDrvName, strerror(errno));
        RetVal = EXIT_FAILURE;
    }
    else /* Everything OK, now clear the table */
        for(u8I = 0U; u8I < SRAM_TO_BE_RELEASED_COUNT; u8I++)
            au64SramToBeReleased[u8I] = 0U;
    return RetVal;
}

//***************************************************************************
int SramAlloc(unsigned long *pSizeAddr) {
    int ret, RetVal = EXIT_SUCCESS;
    uint32_t u32Idx;
    
    /* Try to find free space in au64SramToBeReleased */
    for(u32Idx = 0; u32Idx < SRAM_TO_BE_RELEASED_COUNT; u32Idx++)
        if(0U == au64SramToBeReleased[u32Idx])
            break;
    if(u32Idx >= SRAM_TO_BE_RELEASED_COUNT) {
        printf("[app] Error: SRAM_TO_BE_RELEASED_COUNT need to be increased\n");
        RetVal = EXIT_FAILURE;
    }
    else {
        ret = ioctl(SramFD, IOCTL_SRAM_S_MALLOC, pSizeAddr);
        if (ret) {
            printf("[app] Error: SRAM allocation failed with code: %d\n", ret);
            RetVal = EXIT_FAILURE;
        }
        else {
            au64SramToBeReleased[u32Idx] = *pSizeAddr;
        }
    }
    return RetVal;
}

//***************************************************************************
int SramRelease(unsigned long Addr) {
    int ret, RetVal = EXIT_SUCCESS;
    uint8_t u8I;

    /* Find the region */
    for(u8I = 0U; u8I < SRAM_TO_BE_RELEASED_COUNT; u8I++)
        if(Addr == au64SramToBeReleased[u8I]) {
            au64SramToBeReleased[u8I] = 0U;
            ret = ioctl(SramFD, IOCTL_SRAM_FREE, &Addr);
            if(ret) {
                printf("[app] Error: Unable to release SRAM memory on address %lu; code %d\n", Addr, ret);
                RetVal = EXIT_FAILURE;
            }
            break;
        }
    if(u8I >= SRAM_TO_BE_RELEASED_COUNT) {
        printf("[app] Error: SramRelease: Cannot find the address in table.\n");
        RetVal = EXIT_FAILURE;
    }
    return RetVal;
}

//***************************************************************************
void SramReleaseAll(void) {
    uint8_t u8I;
    int ret;

    /* Is SRAM device open? */
    if(SramFD >= 0) {
        /* Release SRAM allocated through this module */
        for(u8I = 0U; u8I < SRAM_TO_BE_RELEASED_COUNT; u8I++)
            if(0U != au64SramToBeReleased[u8I]) {
                ret = ioctl(SramFD, IOCTL_SRAM_FREE, &(au64SramToBeReleased[u8I]));
                if(ret)
                    printf("[app] Error: Unable to release SRAM memory on address %lu; code %d\n",
                           au64SramToBeReleased[u8I], ret
                          );
                au64SramToBeReleased[u8I] = 0U;
            }
    }
}

//***************************************************************************
void SramCloseAllocator(void)
{
    SramReleaseAll();
    close(SramFD);
}

//***************************************************************************
