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

#include <frame_output_file.h>

namespace io
{  
FrameOutputFile::FrameOutputFile(std::string filename, uint32_t width, uint32_t height, IO_DATA_DEPTH depth, IO_DATA_CHANNELS channels)
{
  m_width = width;
  m_height = height;
  m_depth = depth;
  m_channels = channels;
  
  m_writer.open(filename, CV_FOURCC('F', 'M', 'P', '4'), 30.0, cv::Size(m_width, m_height));
  
  if (m_writer.isOpened()) m_status = IO_OK;
  else m_status = IO_ERROR;
  
  if (m_depth == IO_DATA_DEPTH_08)
    m_format = CV_8U;
  else if (m_depth == IO_DATA_DEPTH_16)
    m_format = CV_16U;
  else if (m_depth == IO_DATA_DEPTH_32)
    m_format = CV_32S;
  else if (m_depth == IO_DATA_DEPTH_64)
    m_format = CV_64F;
  
  m_format = CV_MAKETYPE(m_format, (int)channels);
}

FrameOutputFile::~FrameOutputFile()
{
  //if (m_status == IO_OK) m_writer.release();
}

void FrameOutputFile::PutFrame(void *frame)
{
  cv::Mat image(m_height, m_width, m_format, frame);
  if (m_status == IO_OK && m_writer.isOpened())
    m_writer << image;
  else
    m_status = IO_ERROR;  
}

IO_STATUS FrameOutputFile::GetStatus()
{
  return m_status;
}

} // namespace io