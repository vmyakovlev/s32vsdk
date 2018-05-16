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
#ifndef CAMERAV4L_H
#define CAMERAV4L_H

#include <string>

/**
 * Class encapsulates actions with v4l camera device
 */
class camera_v4l
{
  static struct v4l2_buffer mBuf;	//< last poped frame v4l2 metadata
  static bool mBufPoped;		//< true if a buffer has been poped
  
  public:
    /**
     * Function return last read buffer
     * @return Last read buffer
     */
    static void * get_current_buffer();
    
    /**
     * Function returns last read buffer size
     * @return Last read buffer size
     */
    static int get_current_size();

    /**
     * Function sets the camera name to be used
     * @param name Name of the device
     */
    static void set_device_name(std::string name);

    /**
     * Reads the frame (sets current buffer variables)
     * @return 1 if ok
     */
    static int read_frame(void);

    /**
     * Get next frame - gets and reads next frame
     * @return Pointer to the last read buffer
     */
    static void *get_frame(void);
    
    /**
     * Pop the buffer (sets current buffer variables)
     * @return 1 if ok
     */
    static int pop_buf(void);
    
    /**
     * Pushes the buffer back 
     * @return 1 if ok
     */
    static int push_buf(void);

    /**
     * Pop next frame - gets and reads next frame
     * @return 1 if ok
     */
    static void *pop_frame(void);
    
    /**
     * Push next frame - return buffer to queue
     * @return 1 if ok
     */
    static int push_frame(void);

    /**
     * Stop capturing signal
     */
    static void stop_capturing(void);

    /**
     * Start capturing signal
     */
    static void start_capturing(void);

    /**
     * Unitinialize device
     */
    static void uninit_device(void);

    /**
     * Initialize mmaped camera frame read
     */
    static void init_mmap(void);

    /**
     * Initialize camera device
     */
    static void init_device(int width, int height);

    /**
     * Close device file
     */
    static void close_device(void);

    /**
     * Open device file
     */
    static void open_device(void);
};

#endif /* CAMERAV4L_H */


