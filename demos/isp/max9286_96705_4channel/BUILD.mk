##############################################################################
#
# Freescale Confidential Proprietary
#
# Copyright (c) 2015 Freescale Semiconductor;
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
#
#  CONTENT
#    Basic DCU output functionality test.
#
#  AUTHOR
#    Armin Strobl
#
#  DATE
#    2015-06-24
#
#  LANGUAGE
#    make
#
##############################################################################

SDK_ROOT := ../../../..

ARM_APP = max9286_96705_4channel
ISP_GRAPH = max9286_96705_4

##############################################################################
# isp_app
##############################################################################

VPATH = ../src/lib

ARM_APP_SRCS += main.cpp \
		mymemcpy.S \
                canrxtask.c\
		cantxtask.c\
		ImageStitching.c\
		KeyTask.c\
		TerminalTask.c\
		demotest.c\
		uart_to_mcu.c\
		fb_helper.cpp\
		network.c\

ARM_INCS +=                                                  \
    -I$(SDK_ROOT)/platform/s32_v234                          \
    -I$(OPENCV_ROOT)/include                                 \
    -I$(SDK_ROOT)/include                                                 \
    -I$(SDK_ROOT)/libs/io/sdi/include                        \
    -I$(SDK_ROOT)/libs/io/gdi/include                        \
    -I$(SDK_ROOT)/libs/utils/common/include                  \
    -I$(SDK_ROOT)/libs/apex/common/include                   \
    -I$(SDK_ROOT)/isp/inc                                    \
    -I$(SDK_ROOT)/libs/io/framebuffer/user/include/linux     \
    -I$(SDK_ROOT)/libs/io/image_camera_io/include            \
    -I$(SDK_ROOT)/libs/io/frame_io/include                   \
    -I$(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/inc                \
    -I$(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/$(ODIR)            \
    -I$(SDK_ROOT)/libs/io/dcu/include                                     \
    -I$(SDK_ROOT)/libs/isp/sequencer/kernel/include                       \
    -I$(SDK_ROOT)/demos/isp/max9286_96705_4channel/include                \

ARM_APP_LIBS +=                                                           \
    $(SDK_ROOT)/libs/io/frame_io/$(ODIR)/libframe_io.a                    \
    $(SDK_ROOT)/libs/io/sdi/$(ODIR)/libsdi.a                              \
    $(SDK_ROOT)/libs/io/gdi/$(ODIR)/libgdi.a                              \
    $(SDK_ROOT)/libs/isp/cam_generic/user/$(ODIR)/libcamdrv.a             \
    $(SDK_ROOT)/libs/isp/csi/user/$(ODIR)/libcsidrv.a                     \
    $(SDK_ROOT)/libs/utils/log/$(ODIR)/liblog.a                           \
    $(SDK_ROOT)/libs/utils/umat/$(ODIR)/libumat.a                            \
    $(SDK_ROOT)/libs/isp/sequencer/user/$(ODIR)/libseqdrv.a               \
    $(SDK_ROOT)/libs/isp/fdma/user/$(ODIR)/libfdmadrv.a                   \
    $(SDK_ROOT)/libs/utils/oal/user/$(ODIR)/liboal.a                      \
    $(SDK_ROOT)/libs/isp/sram/user/$(ODIR)/libsramdrv.a                   \
    $(SDK_ROOT)/isp/firmware/$(ODIR)/sequencer.a                          \
    $(SDK_ROOT)/isp/graphs/$(ISP_GRAPH)/$(ODIR)/$(ISP_GRAPH).a            \
    $(SDK_ROOT)/libs/utils/common/$(ODIR)/libcommon.a                     \
    
ARM_LDOPTS +=                                                                \
    -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs
    #-lopencv_core -lopencv_highgui -lopencv_imgproc  -lopencv_features2d -lopencv_flann -lopencv_imgcodecs -lopencv_ml -lopencv_objdetect -lopencv_photo -lopencv_shape -lopencv_stitching
ifneq (,$(findstring gnu-sa,$(ODIR))) 
  ARM_APP_LIBS +=                                                         \
    $(SDK_ROOT)/libs/io/i2c/$(ODIR)/libi2c.a                              \
    $(SDK_ROOT)/libs/io/dcu/$(ODIR)/libdcu.a                              \

endif

ifneq (,$(findstring gnu-linux,$(ODIR))) 
  ARM_APP_LIBS +=                                                         \
    $(SDK_ROOT)/libs/isp/jpegdec/user/$(ODIR)/libjpegdecdrv.a             \
    $(SDK_ROOT)/libs/isp/h264enc/user/$(ODIR)/libh264encdrv.a             \
    $(SDK_ROOT)/libs/isp/h264dec/user/$(ODIR)/libh264decdrv.a             \
    $(SDK_ROOT)/libs/isp/viu/user/$(ODIR)/libviudrv.a                     \
    
endif

##############################################################################
# STANDALONE SPECIFIC INCLUDES
##############################################################################	
ifneq (,$(findstring -sa,$(ODIR)))

ARM_APP_LIBS +=                                                              \
    $(SDK_ROOT)/libs/startup/v234ce_standalone/$(ODIR)/libv234ce.a           \
    $(SDK_ROOT)/libs/io/i2c/$(ODIR)/libi2c.a                                 \
    $(SDK_ROOT)/libs/io/semihost/$(ODIR)/libSemihost.a                       \
    $(SDK_ROOT)/libs/io/uartlinflex_io/$(ODIR)/liblinflex.a              
    
endif
