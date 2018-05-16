##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2014 Freescale Semiconductor;
# All Rights Reserved
#
##############################################################################
#
# THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
# THE POSSIBILITY OF SUCH DAMAGE.
#
##############################################################################

SDK_ROOT := ../../../..

ARM_LIB = libframe_io.a

##############################################################################
# libframe_io.a
##############################################################################
ARM_LIB_SRCS +=                                                              \
    frame_input_memory.cpp                                                   \
    frame_output_memory.cpp                                                  \
    frame_input_patterngen.cpp                                               \
    frame_output_dcu.cpp                                               \
    frame_input_file.cpp                                               \
    frame_input_linuxcam.cpp                                               \
    frame_output_file.cpp                                               \
    frame_output_v234fb.cpp                                               \
    frame_input_isp.cpp                                               \
    frame_output_zynqfb.cpp                                               \

ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/libs/io/dcu/include                                        \
    -I$(SDK_ROOT)/libs/io/frame_io/include                                   \
    -I$(OPENCV_ROOT)/include                                                 \
    -I$(SDK_ROOT)/libs/apex/common/include                                   \
    -I$(SDK_ROOT)/include                                                    \
    -I$(SDK_ROOT)/libs/isp/viu/user/include                                  \
    -I$(SDK_ROOT)/libs/isp/sequencer/kernel/include                          \
    -I$(SDK_ROOT)/libs/io/sdi/include                                        \
    -I$(SDK_ROOT)/libs/io/gdi/include                                        \
    -I$(SDK_ROOT)/isp/inc                                                    \
    -I$(SDK_ROOT)/platform/s32_v234                                          \
    -I$(SDK_ROOT)/libs/utils/common/include                                  
    
ifneq (,$(findstring linux,$(ODIR)))

ARM_LIB_SRCS +=                                                              \
    frame_input_isp.cpp                                                      \
    frame_input_file.cpp                                                     \
    frame_input_linuxcam.cpp                                                 \
    frame_output_file.cpp                                                    \
    frame_output_v234fb.cpp                                                  \
    frame_output_zynqfb.cpp                                                  \

ARM_INCS +=                                                                  \
    -I$(SDK_ROOT)/libs/io/image_camera_io/include                            \
    -I$(SDK_ROOT)/libs/io/framebuffer/user/include/linux                     \
    -I$(OPENCV_ROOT)/include
endif

#ifneq (,$(findstring sa,$(ODIR)))

    ARM_LIB_SRCS +=                                                          \
	frame_input_isp.cpp                                                      \
        frame_output_dcu.cpp                                                 \
        
    ARM_INCS +=                                                              \
        -I$(SDK_ROOT)/libs/isp/fdma/kernel/include                           
#endif 


##############################################################################
