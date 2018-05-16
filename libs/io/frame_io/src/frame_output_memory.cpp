/*****************************************************************************
*
* Freescale Confidential Proprietary
*
* Copyright (c) 2014 Freescale Semiconductor;
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

#include <frame_output_memory.h>
#include <string.h>

namespace io
{

FrameOutputMemory::FrameOutputMemory(void *base_ptr, uint32_t max_size, uint32_t width, uint32_t height, IO_DATA_DEPTH depth, IO_DATA_CHANNELS channels)
{
      m_base_ptr    = base_ptr;
      m_current_ptr = base_ptr;
      m_end_ptr     = (int8_t *)base_ptr + max_size;
      m_width       = width;
      m_height      = height;
      m_frame_size  = width * height * (uint32_t)depth * (uint32_t)channels;
      m_depth       = depth;
      m_channels    = channels;
      
      if ((int8_t *)m_base_ptr + m_frame_size <= m_end_ptr) m_status = IO_OK;
      else m_status = IO_EOF;
}

FrameOutputMemory::~FrameOutputMemory()
{
}

void FrameOutputMemory::PutFrame(void *frame)
{
  if (m_status == IO_OK && (int8_t*)m_current_ptr + m_frame_size <= m_end_ptr) 
  {
    memcpy(m_current_ptr, frame, m_frame_size);
    m_current_ptr = (int8_t *)m_current_ptr + m_frame_size;;
  }
  else
  {
    m_status = IO_EOF;
  }
}

IO_STATUS FrameOutputMemory::GetStatus()
{
  return m_status;
}

} // namespace io