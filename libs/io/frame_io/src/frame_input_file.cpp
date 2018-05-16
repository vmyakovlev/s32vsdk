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

#include <frame_input_file.h>


namespace io
{

FrameInputFile::FrameInputFile(std::string filename) : m_capture(filename)
{
  if (m_capture.isOpened()) m_status   = IO_OK;
  else m_status = IO_ERROR;
  
  m_width    = 0;
  m_height   = 0;
  m_depth    = IO_DATA_DEPTH_NOT_INITIALIZED;
  m_channels = IO_DATA_CH_NOT_INITIALIZED;
}

FrameInputFile::~FrameInputFile()
{
  if (m_status == IO_OK || m_status == IO_EOF) m_capture.release();
}

void * FrameInputFile::GetFrame()
{
  if (m_status != IO_OK) return NULL;
  
  m_capture >> m_frame;
  if (m_frame.empty())
  {
    m_status = IO_EOF;
    return NULL;
  }
  
  m_width = m_frame.cols;
  m_height = m_frame.rows;
  
  switch (m_frame.depth())
  {
    case CV_8U:
    case CV_8S:
      m_depth = IO_DATA_DEPTH_08;
      break;
      
    case CV_16U:
    case CV_16S:
      m_depth = IO_DATA_DEPTH_16;
      break;
      
    case CV_32S:
    case CV_32F:
      m_depth = IO_DATA_DEPTH_32;
      break;
      
    case CV_64F:
      m_depth = IO_DATA_DEPTH_64;
      break;
      
    default:
      m_depth = IO_DATA_DEPTH_NOT_INITIALIZED;
      break;
  }
  
  switch (m_frame.channels())
  {
    case 1:
      m_channels = IO_DATA_CH1;
      break;
      
    case 2:
      m_channels = IO_DATA_CH2;
      break;
      
    case 3:
      m_channels = IO_DATA_CH3;
      break;
      
    case 4:
      m_channels = IO_DATA_CH4;
      break;
      
    default:
      m_channels = IO_DATA_CH_NOT_INITIALIZED;
      break;
  }
  return m_frame.ptr();
}

IO_STATUS FrameInputFile::GetStatus()
{
  return m_status;
}

uint32_t FrameInputFile::GetWidth()
{
  return m_width;
}

uint32_t FrameInputFile::GetHeight()
{
  return m_height;
}

IO_DATA_DEPTH FrameInputFile::GetDepth()
{
  return m_depth;
}

IO_DATA_CHANNELS FrameInputFile::GetChannels()
{
  return m_channels;
}
      
} // namespace io