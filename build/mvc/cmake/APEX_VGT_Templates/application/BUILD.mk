##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  LANGUAGE
#    make
#
##############################################################################

#SDK_ROOT := ../../../../s32v234_sdk
include ../SDK_ROOT.mk
KERNELS  := $(SDK_ROOT)/kernels/apu

##############################################################################
# acf kernels and graphs
##############################################################################

APU_GRAPH_INCS += -I.                                                        \
    -I../graphs                                                              \
	-I../graphs/build-apu-tct-sa-d                                           \
    -I$(SDK_ROOT)/include                                  \
    -I$(SDK_ROOT)/kernels/apu/												 \
	

##############################################################################
# acf_app
##############################################################################
VPATH += ../graphs/
VPATH += ../kernels/apu

include ../Build_AppImport.mk

ARM_APP_SRCS +=                                                               \
				displayData.cpp												  \
			

ARM_INCS +=                                                                  \
    -I./                                                                     \
    -I../src                                                                 \
    -I../graphs                                                              \
	-I../graphs/build-apu-tct-sa-d                                           \
    -I$(SDK_ROOT)/include                              \
    -I$(SDK_ROOT)/kernels/apu/                                               \
	-I$(OPENCV_ROOT)/include												\
    -I$(SDK_ROOT)/libs/apex/icp/include                                  \
    -I$(SDK_ROOT)/libs/apex/acf/include                                  \
    -I$(SDK_ROOT)/libs/apex/drivers/user/include                        \
    -I$(SDK_ROOT)/libs/utils/oal/user/include                           \
	-I$(SDK_ROOT)/libs/utils/communications/include						\
    -I$(SDK_ROOT)/3rdparty/ffmpeg/linux-arm64/include                   \
	-I$(SDK_ROOT)/libs/io/dcu/include									 \
	-I$(SDK_ROOT)/libs/io/frame_io/include                               \


X86_INCS += -I$(SDK_ROOT)/include

ARM_APP_LIBS +=                                                               \
    $(SDK_ROOT)/libs/utils/common/$(ODIR)/libcommon.a                    \
    $(SDK_ROOT)/libs/utils/matrix/$(ODIR)/libmatrix.a                    \
    $(SDK_ROOT)/libs/apex/acf/$(ODIR)/libacf.a                           \
    $(SDK_ROOT)/libs/apex/icp/$(ODIR)/libicp.a                           \
    $(SDK_ROOT)/libs/utils/matrix/$(ODIR)/libmatrix.a                    \
    $(SDK_ROOT)/libs/apex/drivers/user/$(ODIR)/libapexdrv.a                \
    $(SDK_ROOT)/libs/utils/oal/user/$(ODIR)/liboal.a                     \
	$(SDK_ROOT)/libs/io/frame_io/$(ODIR)/libframe_io.a                   \
	$(SDK_ROOT)/libs/utils/communications/$(ODIR)/lib_communications.a   \
    $(SDK_ROOT)/libs/io/image_video_io/$(ODIR)/libimage_video_io.a		 \
 

##############################################################################
# STANDALONE SPECIFIC INCLUDES
##############################################################################	
ifneq (,$(findstring -sa,$(ODIR)))

	ARM_APP_LIBS +=                                                          \
      $(SDK_ROOT)/libs/startup/v234ce_standalone/$(ODIR)/libv234ce.a           \
      $(SDK_ROOT)/libs/io/i2c/$(ODIR)/libi2c.a                                 \
	  $(SDK_ROOT)/libs/io/semihost/$(ODIR)/libSemihost.a                       \
      $(SDK_ROOT)/libs/io/uartlinflex_io/$(ODIR)/liblinflex.a                  \
      $(SDK_ROOT)/libs/io/dcu/$(ODIR)/libdcu.a							 \

    ARM_LDOPTS +=                                                            \
	    -L$(SDK_ROOT)/3rdparty/ocv/standalone-arm64/lib                      \
        -lopencv_video                                                       \
		-lopencv_flann                                                       \
		-lopencv_features2d                                                  \
		-lopencv_imgproc                                                     \
		-lopencv_highgui                                                     \
		-lopencv_imgcodecs                                                       \
	    -lopencv_videoio                                                         \
		-lopencv_core                                                        \
		-lzlib
else
##############################################################################
# INTEGRITY SPECIFIC INCLUDES
##############################################################################

ifneq (,$(findstring -integrity,$(ODIR)))

ARM_LDOPTS +=                                                                \
    -L$(SDK_ROOT)/3rdparty/ocv/integrity-arm/share/OpenCV/3rdparty/lib                \
    -lposix                                                                  \
    -livfs                                                                   \
    -lIlmImf                                                                 \
    -lzlib                                                                   \
    --exceptions                                                             \
    -lopencv_objdetect                                                       \
    -lopencv_core                                                            \
    -lopencv_highgui                                                         \
	-lopencv_imgcodecs                                                       \
	-lopencv_videoio                                                         \
    -lopencv_imgproc                                                         \
    -lopencv_features2d                                                      \
    -lopencv_flann                                                           \
    -llibjasper                                                              \
    -llibjpeg                                                                \
    -llibpng                                                                 \
    -llibtiff

##############################################################################
# LINUX SPECIFIC INCLUDES
##############################################################################			
else
    ARM_LDOPTS +=                                                            \
        -lopencv_core                                                        \
        -lopencv_highgui                                                     \
		-lopencv_imgcodecs                                                   \
		-lopencv_videoio                                                     \
        -lopencv_imgproc                                                     \
        -lopencv_calib3d                                                     \
        -lopencv_ml                                                         \
        -lopencv_video                                                      \
        -lopencv_videostab                                                  \
        -lopencv_flann                                                       \
        -lopencv_features2d                                                  \
        -lopencv_photo                                                      \
        -lopencv_objdetect                                                  \
        -lavcodec                                                            \
        -lavfilter                                                           \
        -lavutil                                                             \
        -lavformat                                                           \
        -lavdevice                                                           \
        -lswresample                                                         \
        -lswscale															\
 
endif
endif


