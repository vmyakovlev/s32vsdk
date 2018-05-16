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
#ifndef CAMERA_H
#define CAMERA_H

#include <string>
#include <camera_v4l.h>

/**
 * Class encapsulates camera operations using v4l
 * (!!! TODO Not possible to use multiple cameras right now, uses static function in back end)
 */
class CameraIO
{  
  public:
    /**
     * Constructor - connects and initializes the camera
     * @param device_name Name of the device file
     * @param width Frame width (must be supported by camera)
     * @param height Frame height (must be supported by camera
     */
    CameraIO(std::string device_name, int width, int height);
    
    /**
     * Destructor - stops capturing and disconnects the camera
     */
    ~CameraIO(void);
  
    /**
     * Gets current frame size
     */
    int GetSize(void);
    
    /**
     * Gets next frame
     */
    void *GetFrame(void);
    
    void *PopFrame(void);
    int PushFrame(void);
};

#endif /* CAMERA_H */
