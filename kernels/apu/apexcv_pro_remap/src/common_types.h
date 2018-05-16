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

#ifndef COMMONTYPES_H
#define COMMONTYPES_H

// Includes
#include <stdint.h>

// Definitions
#define CU_COUNT   32
#define CU_INDEXES { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,\
                    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31} 
// Types
// Rect could be used for image blocks: [left, top] for offset, [width, height] for dimension.
struct rect_t { int16u left, top, width, height; };
struct point_t{ int x, y; };
typedef enum { vS8 = 1, vS16 = 2, vU8 = 3, vU16 = 4} type_t;

// Convenience Functions
inline point_t make_point(int x, int y)
{
    point_t pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

inline rect_t make_rect(int left, int top, int width, int height)
{
    rect_t rt;
    rt.top = top;
    rt.left = left;
    rt.width = width;
    rt.height= height;
    return rt;
}

inline void vecsat(vec32s* val, int32_t min, int32_t max)
{
    vif (*val < min)
    {
       *val = min;
    }
    vendif    
    vif (*val > max)
    {
       *val = max;
    }
    vendif
}

#endif /* COMMONTYPES_H */

