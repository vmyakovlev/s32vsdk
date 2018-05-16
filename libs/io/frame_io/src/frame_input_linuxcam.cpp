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

#include <frame_input_linuxcam.h>

#include <string.h>

static inline void SATURATE(int& value, int min_val, int max_val)
{
  if (value < min_val) value = min_val;
  if (value > max_val) value = max_val;
}

inline void YUYV_to_BGR24(int size_x, int size_y, uint8_t* YUYV_ptr, uint8_t* out)
{
  const int K1 = int(1.402f * (1 << 16));
  const int K2 = int(0.714f * (1 << 16));
  const int K3 = int(0.334f * (1 << 16));
  const int K4 = int(1.772f * (1 << 16));

  uint8_t* out_ptr = out;
  const int pitch = size_x * 2; // 2 bytes per one YU-YV pixel

  for (int y=0; y<size_y; ++y) 
  {
    const uint8_t* src = YUYV_ptr + pitch * y;
    for (int x=0; x<size_x*2; x+=4) 
    { // Y1 U Y2 V
      uint8_t Y1 = src[x + 0];
      uint8_t U  = src[x + 1];
      uint8_t Y2 = src[x + 2];
      uint8_t V  = src[x + 3];

      int uf = (int)U - 128;
      int vf = (int)V - 128;

      int R = Y1 + (K1*vf >> 16);
      int G = Y1 - (K2*vf >> 16) - (K3*uf >> 16);
      int B = Y1 + (K4*uf >> 16);

      SATURATE(R, 0, 255);
      SATURATE(G, 0, 255);
      SATURATE(B, 0, 255);

      *out_ptr++ = uint8_t(B);
      *out_ptr++ = uint8_t(G);
      *out_ptr++ = uint8_t(R);

      R = Y2 + (K1*vf >> 16);
      G = Y2 - (K2*vf >> 16) - (K3*uf >> 16);
      B = Y2 + (K4*uf >> 16);

      SATURATE(R, 0, 255);
      SATURATE(G, 0, 255);
      SATURATE(B, 0, 255);

      *out_ptr++ = uint8_t(B);
      *out_ptr++ = uint8_t(G);
      *out_ptr++ = uint8_t(R);
    }
  }
}

namespace io
{

FrameInputLinuxCam::FrameInputLinuxCam(std::string device_name, int width, int height) : m_camera(device_name, width, height)
{
  m_status   = IO_OK;
    
  m_width    = width;
  m_height   = height;
  m_depth    = IO_DATA_DEPTH_08;
  m_channels = IO_DATA_CH3;
  
  m_bgr_buffer = new uint8_t[width*height*m_depth*m_channels];
  m_yuv_buffer = new uint8_t[width*height*m_depth*IO_DATA_CH2];
}

FrameInputLinuxCam::~FrameInputLinuxCam()
{
  delete m_bgr_buffer;
  delete m_yuv_buffer;
}

void * FrameInputLinuxCam::GetFrame()
{
  if (m_status != IO_OK) return NULL;
  
  void *buf = m_camera.GetFrame();
  memcpy(m_yuv_buffer, buf, m_camera.GetSize());
  YUYV_to_BGR24(m_width, m_height, m_yuv_buffer, m_bgr_buffer);
  
  return m_bgr_buffer;
}

IO_STATUS FrameInputLinuxCam::GetStatus()
{
  return m_status;
}

uint32_t FrameInputLinuxCam::GetWidth()
{
  return m_width;
}

uint32_t FrameInputLinuxCam::GetHeight()
{
  return m_height;
}

IO_DATA_DEPTH FrameInputLinuxCam::GetDepth()
{
  return m_depth;
}

IO_DATA_CHANNELS FrameInputLinuxCam::GetChannels()
{
  return m_channels;
}
      
} // namespace io