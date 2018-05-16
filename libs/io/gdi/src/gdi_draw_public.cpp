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
/**
 * \file 		gdi_draw_public.cpp
 * \brief 		Function definitions for the GDI Draw component
 * \author 	Tomas Babinec
 * \version 	0.1
 * \date 		30-08-2013
 ****************************************************************************/

#include <stdio.h>
#include "gdi_draw_public.hpp"

#include "vdb_log.h"

//
//  GdiDraw_BoxText methods
//

// set default values
GdiDraw_BoxText::GdiDraw_BoxText()
{
  mBoxWidthInPixels = 0;
  mBoxHeightInPixels = 0;
  mOutlineWidthInPixels = 0;
  mOutlineColor = 0;
  mText[0] = '\0';
  mTextColor = 0;
  mTextEdge = LIB_BOX_EDGE_NONE;
  mTextAligment = LIB_TEXT_ALIGN_NONE;
}

// reset to defaults
GdiDraw_BoxText::~GdiDraw_BoxText()
{
  mBoxWidthInPixels = 0;
  mBoxHeightInPixels = 0;
  mOutlineWidthInPixels = 0;
  mOutlineColor = 0;
  mText[0] = '\0';
  mTextColor = 0;
  mTextEdge = LIB_BOX_EDGE_NONE;
  mTextAligment = LIB_TEXT_ALIGN_NONE;
}

LIB_RESULT Draw(uint32_t const cCanvasWidthInPixels,
                uint32_t const cCanvasHeightInPixels,
                uint32_t const cHorizontalOffsetInPixels,
                uint32_t const cVerticalOffsetInPixels,
                GDI_IMAGE_FORMAT const cGdiImageFormat,
                void* const cpBuffer)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
}

//
//  GdiDraw_Icon methods
//

GdiDraw_Icon::GdiDraw_Icon(ICP_Image *iconImg)
{

} // GdiDraw_Icon::GdiDraw_Icon(ICP_Image *iconImg)

GdiDraw_Icon::~GdiDraw_Icon()
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
} // GdiDraw_Icon::~GdiDraw_Icon()

LIB_RESULT Draw(uint32_t const cHorizontalOffsetInPixels,
                uint32_t const cVerticalOffsetInPixels,
                ICP_Image *destImage)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
} // Draw()

//
//  GdiDraw_Line methods
//

GdiDraw_Line::GdiDraw_Line()
{
  mX1 = 0;
  mY1 = 0;
  mX2 = 0;
  mY2 = 0;
  mColor = 0;
  // not implemented completely
  VDB_LOG_FCN_NOT_IMPLEMENTED();
}

GdiDraw_Line::~GdiDraw_Line()
{
  mX1 = 0;
  mY1 = 0;
  mX2 = 0;
  mY2 = 0;
  mColor = 0;
  // not implemented completely
  VDB_LOG_FCN_NOT_IMPLEMENTED();
}

LIB_RESULT Draw(ICP_Image* const cpDestImage)
{
  // not implemented
  VDB_LOG_FCN_NOT_IMPLEMENTED();
  return LIB_FAILURE;
}
