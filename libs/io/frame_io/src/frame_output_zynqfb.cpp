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

#include <frame_output_zynqfb.h>


#include <framebuffer.h>
#include <fcntl.h>            
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace io
{  
FrameOutputZynqFB::FrameOutputZynqFB(uint32_t width, uint32_t height, IO_DATA_DEPTH depth, IO_DATA_CHANNELS channels)
{
  m_width = width;
  m_height = height;
  m_depth = depth;
  m_channels = channels;
  
  if (m_depth == IO_DATA_DEPTH_08 && m_channels == IO_DATA_CH3)
  {
    m_cvtcode = -1;
    m_in_frame = cv::Mat(m_height, m_width, CV_8UC3, 0.0);
  }
  else if (m_depth == IO_DATA_DEPTH_08 && m_channels == IO_DATA_CH1)
  {
    m_cvtcode = CV_GRAY2BGR;
    m_in_frame = cv::Mat(m_height, m_width, CV_8UC1, 0.0);
  }
  else
  {
    m_status = IO_ERROR;
    return;
  }
  
  m_fd = open("/dev/fb_axivdma", O_RDWR);
  
  if (m_fd < 1)
    m_status = IO_ERROR;
  else
    m_status = IO_OK;
  
  m_buffers = (uint8_t *)mmap(0, VDMA_BUFFER_SIZE*2, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, m_fd, 0x0);
  
  m_buffer[0] = m_buffers+VDMA_BUFFER_0_OFFSET;
  m_buffer[1] = m_buffers+VDMA_BUFFER_1_OFFSET;
  
  ///////////////////////////////////////////////
  // Paint black on the screen
  
  // Get back buffer index
  ioctl(m_fd, CMD_GET_BACK, &m_back_id);
  
  // Write black to back buffer
  black_buffer_RGB(m_buffer[m_back_id]);
  
  // Switch buffers (back have new back buffer value)
  ioctl(m_fd, CMD_PAINT, &m_back_id);
  
  // Write black to back buffer
  black_buffer_RGB(m_buffer[m_back_id]);
  
  // Switch buffers again
  ioctl(m_fd, CMD_PAINT, &m_back_id);
  
  m_rgb_disp = cv::Mat(VDMA_BUFFER_HEIGHT, VDMA_BUFFER_WIDTH, CV_8UC3, 0.0);
  
}

FrameOutputZynqFB::~FrameOutputZynqFB()
{
  close(m_fd);
}

void FrameOutputZynqFB::PutFrame(void *frame)
{
  if (m_status == IO_OK)
  {
    memcpy(m_in_frame.ptr(), frame, m_width*m_height*m_channels*m_depth);
    if (m_cvtcode != -1)
      cvtColor(m_in_frame, m_rgb_disp, m_cvtcode);
    m_in_frame.copyTo(m_rgb_disp(cv::Rect((VDMA_BUFFER_WIDTH-m_width)/2, (VDMA_BUFFER_HEIGHT-m_height)/2, m_width, m_height)));
    neon_copy_bgr_to_grb(m_rgb_disp.ptr(), m_buffer[m_back_id], VDMA_BUFFER_SIZE/3);
    ioctl(m_fd, CMD_PAINT, &m_back_id);
  }
}

IO_STATUS FrameOutputZynqFB::GetStatus()
{
  return m_status;
}

} // namespace io