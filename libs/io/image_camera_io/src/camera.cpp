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
#include <camera.h>
  
////////////////////////////////////////////////////////////////////////////////
// Get current frame size
////////////////////////////////////////////////////////////////////////////////
int CameraIO::GetSize()
{
  return camera_v4l::get_current_size();
}
  
////////////////////////////////////////////////////////////////////////////////
// Constructor - connects and initializes the camera
////////////////////////////////////////////////////////////////////////////////
CameraIO::CameraIO(std::string device_name, int width, int height)
{
  camera_v4l::set_device_name(device_name);
  camera_v4l::open_device();
  camera_v4l::init_device(width, height);
  camera_v4l::start_capturing();
}
  
////////////////////////////////////////////////////////////////////////////////
// Gets next frame
////////////////////////////////////////////////////////////////////////////////
void * CameraIO::GetFrame()
{
  return camera_v4l::get_frame();
}

////////////////////////////////////////////////////////////////////////////////
// Pops next frame
////////////////////////////////////////////////////////////////////////////////
void * CameraIO::PopFrame()
{
  return camera_v4l::pop_frame();
}

////////////////////////////////////////////////////////////////////////////////
// Pushes back previous frame
////////////////////////////////////////////////////////////////////////////////
int CameraIO::PushFrame()
{
  return camera_v4l::push_frame();
}

////////////////////////////////////////////////////////////////////////////////
// Destructor - stops capturing and disconnects the camera
////////////////////////////////////////////////////////////////////////////////
CameraIO::~CameraIO()
{
  camera_v4l::stop_capturing();
  camera_v4l::uninit_device();
  camera_v4l::close_device();
}