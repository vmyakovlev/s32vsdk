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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <camera_v4l.h>

#define CLEAR(x) memset(&(x), 0, sizeof(x))

struct buffer {
        void   *start;
        size_t  length;
};

static std::string      dev_name;
static int              fd = -1;
struct buffer          *buffers;
static unsigned int     n_buffers;

static void * current_buffer;
static int current_size;

struct v4l2_buffer camera_v4l::mBuf;	//< last poped frame v4l2 metadata
bool camera_v4l::mBufPoped = false;	//< true if a buffer has been poped

////////////////////////////////////////////////////////////////////////////////
// Get current frame size
////////////////////////////////////////////////////////////////////////////////
int camera_v4l::get_current_size()
{
  return current_size;
}

////////////////////////////////////////////////////////////////////////////////
// Set device name to be used
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::set_device_name(std::string name)
{
  dev_name = name;
}

////////////////////////////////////////////////////////////////////////////////
// IOCTL until success
////////////////////////////////////////////////////////////////////////////////
int xioctl(int fh, unsigned int request, void *arg)
{
  int r;

  do 
  {
    r = ioctl(fh, request, arg);
  } while (-1 == r && EINTR == errno);

  return r;
}

////////////////////////////////////////////////////////////////////////////////
// Print error and exit
////////////////////////////////////////////////////////////////////////////////
void errno_exit(const char *s)
{
  fprintf(stderr, "%s error %d, %s\n", s, errno, strerror(errno));
  exit(EXIT_FAILURE);
}

////////////////////////////////////////////////////////////////////////////////
// Read current frame from camera
////////////////////////////////////////////////////////////////////////////////
int camera_v4l::read_frame(void)
{
  struct v4l2_buffer buf;
			
  //CLEAR(buf);

  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;

  if (-1 == xioctl(fd, VIDIOC_DQBUF, &buf)) 
  {
    switch (errno) 
    {
      case EAGAIN:
        return 0;
      case EIO:
      default:
        errno_exit("VIDIOC_DQBUF");
    }
  }

  assert(buf.index < n_buffers);
	
  current_buffer = buffers[buf.index].start;
  current_size = buf.bytesused;

  if (-1 == xioctl(fd, VIDIOC_QBUF, &buf))
    errno_exit("VIDIOC_QBUF");
               
  fflush(stdout);
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
// Pop buffer from camera queue
////////////////////////////////////////////////////////////////////////////////
int camera_v4l::pop_buf(void)
{			
  //CLEAR(mBuf);

  mBuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  mBuf.memory = V4L2_MEMORY_MMAP;
  
  if (-1 == xioctl(fd, VIDIOC_DQBUF, &mBuf)) 
  {
    switch (errno) 
    {
      case EAGAIN:
        return 0;
      case EIO:
      default:
        errno_exit("VIDIOC_DQBUF");
    }
  }

  assert(mBuf.index < n_buffers);
	
  current_buffer = buffers[mBuf.index].start;
  current_size = mBuf.bytesused;

  return 1;
}

////////////////////////////////////////////////////////////////////////////////
// Push buffer back to camera queue
////////////////////////////////////////////////////////////////////////////////
int camera_v4l::push_buf(void)
{			
  if (-1 == xioctl(fd, VIDIOC_QBUF, &mBuf))
    errno_exit("VIDIOC_QBUF");
               
  fflush(stdout);
  
  CLEAR(mBuf);
  return 1;
}

////////////////////////////////////////////////////////////////////////////////
// Get current frame from camera
////////////////////////////////////////////////////////////////////////////////
void *camera_v4l::get_frame(void)
{
  fd_set fds;
  struct timeval tv;
  int r;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  /* Timeout. */
  tv.tv_sec = 5;
  tv.tv_usec = 0;

  r = select(fd + 1, &fds, NULL, NULL, &tv);

  if (-1 == r) 
  {
    if (EINTR == errno)
      return 0;
    errno_exit("select");
  }
  
  if (0 == r) 
  {
    fprintf(stderr, "select timeout\n");
    exit(EXIT_FAILURE);
  }

  if (read_frame())
    return current_buffer;
  else
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Pop current frame from camera
////////////////////////////////////////////////////////////////////////////////
void *camera_v4l::pop_frame(void)
{
  fd_set fds;
  struct timeval tv;
  int r;
  
  if(!mBufPoped)
  {
    FD_ZERO(&fds);
    FD_SET(fd, &fds);

    /* Timeout. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;

    r = select(fd + 1, &fds, NULL, NULL, &tv);

    if (-1 == r) 
    {
      if (EINTR == errno)
	return 0;
      errno_exit("select");
    }
    
    if (0 == r) 
    {
      fprintf(stderr, "select timeout\n");
      exit(EXIT_FAILURE);
    }

    if (pop_buf())
    {
      mBufPoped = true;
      return current_buffer;
    }
    else
      return 0;
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Return current frame back to camera queue
////////////////////////////////////////////////////////////////////////////////
int camera_v4l::push_frame(void)
{
  if(mBufPoped)
  {
    mBufPoped = false;
    return push_buf();
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Stop capturing the images
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::stop_capturing(void)
{
  enum v4l2_buf_type type;

  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(fd, VIDIOC_STREAMOFF, &type))
    errno_exit("VIDIOC_STREAMOFF");
  
}

////////////////////////////////////////////////////////////////////////////////
// Start capturing the images
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::start_capturing(void)
{
  unsigned int i;
  enum v4l2_buf_type type;

  for (i = 0; i < n_buffers; ++i) 
  {
    struct v4l2_buffer buf;

    CLEAR(buf);
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;

    if (-1 == xioctl(fd, VIDIOC_QBUF, &buf)) errno_exit("VIDIOC_QBUF");
  }
  
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(fd, VIDIOC_STREAMON, &type))
    errno_exit("VIDIOC_STREAMON");            
}

////////////////////////////////////////////////////////////////////////////////
// Uninitialize buffers
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::uninit_device(void)
{
  unsigned int i;

  for (i = 0; i < n_buffers; ++i)
    if (-1 == munmap(buffers[i].start, buffers[i].length))
      errno_exit("munmap");
      
   free(buffers);
}

////////////////////////////////////////////////////////////////////////////////
// Initialize buffers
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::init_mmap(void)
{
  struct v4l2_requestbuffers req;

  CLEAR(req);

  req.count = 2;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if (-1 == xioctl(fd, VIDIOC_REQBUFS, &req)) 
  {
    if (EINVAL == errno) 
    {
      fprintf(stderr, "%s does not support memory mapping\n", dev_name.c_str());
      exit(EXIT_FAILURE);
    } 
    else 
    {
      errno_exit("VIDIOC_REQBUFS");
    }
  }

  if (req.count < 2) 
  {
    fprintf(stderr, "Insufficient buffer memory on %s\n", dev_name.c_str());
    exit(EXIT_FAILURE);
  }

  buffers = (buffer *)calloc(req.count, sizeof(*buffers));

  if (!buffers) 
  {
    fprintf(stderr, "Out of memory\n");
    exit(EXIT_FAILURE);
  }

  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) 
  {
    struct v4l2_buffer buf;

    CLEAR(buf);
    buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory      = V4L2_MEMORY_MMAP;
    buf.index       = n_buffers;

    if (-1 == xioctl(fd, VIDIOC_QUERYBUF, &buf))
      errno_exit("VIDIOC_QUERYBUF");

    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start =
    mmap(NULL /* start anywhere */,
         buf.length,
         PROT_READ | PROT_WRITE /* required */,
         MAP_SHARED /* recommended */,
         fd, buf.m.offset);

         if (MAP_FAILED == buffers[n_buffers].start)
           errno_exit("mmap");
  }
  
  struct v4l2_streamparm fps;
  memset(&fps, 0, sizeof(fps));
  fps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(fd, VIDIOC_G_PARM, &fps) < 0) {
    printf("Couldn't query v4l fps!\n");
    errno_exit("mmap");
  }

  fps.parm.capture.timeperframe.numerator = 1;
  fps.parm.capture.timeperframe.denominator = 60;
  if (ioctl(fd, VIDIOC_S_PARM, &fps) < 0) {
    printf("Couldn't set v4l fps!\n");
    errno_exit("mmap");
  }

}

////////////////////////////////////////////////////////////////////////////////
// Function initializes the camera device
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::init_device(int width, int height)
{
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  unsigned int min;

  if (-1 == xioctl(fd, VIDIOC_QUERYCAP, &cap)) 
  {
    if (EINVAL == errno) 
    {
      fprintf(stderr, "%s is no V4L2 device\n", dev_name.c_str());
      exit(EXIT_FAILURE);
    } 
    else 
    {
      errno_exit("VIDIOC_QUERYCAP");
    }
  }

  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) 
  {
    fprintf(stderr, "%s is no video capture device\n", dev_name.c_str());
    exit(EXIT_FAILURE);
  }

  if (!(cap.capabilities & V4L2_CAP_STREAMING)) 
  {
    fprintf(stderr, "%s does not support streaming i/o\n", dev_name.c_str());
    exit(EXIT_FAILURE);
  }
  
  CLEAR(cropcap);

  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (0 == xioctl(fd, VIDIOC_CROPCAP, &cropcap)) 
  {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; // reset to default

    if (-1 == xioctl(fd, VIDIOC_S_CROP, &crop)) 
    {
      switch (errno) 
      {
        case EINVAL:
          /* Cropping not supported. */
          break;
        default:
          /* Errors ignored. */
          break;
      }
    }
  } 
  else 
  {
    /* Errors ignored. */
  }

  CLEAR(fmt);

  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width       = width;
  fmt.fmt.pix.height      = height;
  fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;

  if (-1 == xioctl(fd, VIDIOC_S_FMT, &fmt))
    errno_exit("VIDIOC_S_FMT");

  /* Note VIDIOC_S_FMT may change width and height. */ 
  else 
  {
    /* Preserve original settings as set by v4l2-ctl for example */
    if (-1 == xioctl(fd, VIDIOC_G_FMT, &fmt))
      errno_exit("VIDIOC_G_FMT");
  }

  /* Buggy driver paranoia. */
  min = fmt.fmt.pix.width * 3;
  if (fmt.fmt.pix.bytesperline < min)
    fmt.fmt.pix.bytesperline = min;
  min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
  if (fmt.fmt.pix.sizeimage < min)
    fmt.fmt.pix.sizeimage = min;

  init_mmap();
}

////////////////////////////////////////////////////////////////////////////////
// Function closes the device (file)
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::close_device(void)
{
  if (-1 == close(fd))
    errno_exit("close");

  fd = -1;
}

////////////////////////////////////////////////////////////////////////////////
// Function opens the device for ioctl
////////////////////////////////////////////////////////////////////////////////
void camera_v4l::open_device(void)
{
  struct stat st;

  if (-1 == stat(dev_name.c_str(), &st)) 
  {
    fprintf(stderr, "Cannot identify '%s': %d, %s\n", dev_name.c_str(), errno, strerror(errno));
    exit(EXIT_FAILURE);
  }

  if (!S_ISCHR(st.st_mode)) 
  {
    fprintf(stderr, "%s is no device\n", dev_name.c_str()); exit(EXIT_FAILURE);
  }

  fd = open(dev_name.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);
  
  if (-1 == fd) 
  {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name.c_str(), errno, strerror(errno));
    exit(EXIT_FAILURE);
  }
}