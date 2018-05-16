##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2011, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#
#
#  AUTHOR
#    Stefan Schwarz
#
#  DATE
#    2013-08-05
#
#  LANGUAGE
#    make
#
##############################################################################
OS_TYPE := INTEGRITY

C0_APP := $(ARM_APP)
C0_APP_SRCS := $(ARM_APP_SRCS)
C0_APP_LIBS := $(ARM_APP_LIBS)

C0_LIB := $(ARM_LIB)
C0_LIB_SRCS := $(ARM_LIB_SRCS)
C0_LIB_LIBS := $(ARM_LIB_LIBS)
C0_LIB_ARCS := $(ARM_LIB_ARCS)

C0_DEFS := $(ARM_DEFS)
C0_INCS := $(ARM_INCS)
C0_CCOPTS = $(ARM_CCOPTS)
C0_LDOPTS = -L$(OPENCV_ROOT)/lib/ -L$(OPENCV_ROOT)/share/OpenCV/3rdparty/lib -L$(FFMPEG_ROOT)/lib/ $(ARM_LDOPTS)

MCU = cortex-a53

##############################################################################
PLATFORM = S32V234

OSTYPE := INTEGRITY

CROSS_COMPILE=

C0_CC := $(CROSS_COMPILE)cxintarm64 -c
C0_CX := $(CROSS_COMPILE)cxintarm64 -c
C0_AS := $(CROSS_COMPILE)cxintarm64
C0_LD := $(CROSS_COMPILE)cxintarm64
C0_AR := $(CROSS_COMPILE)cxintarm64 -language=cxx -archive
C0_INT := $(CROSS_COMPILE)intex -bsp devtree-arm64 -os_dir "$(INTEGRITY_ROOT_PATH)"

C0_CCOPTS += -D__S32V234 -bsp devtree-arm64 --prototype_warnings -os_dir "$(INTEGRITY_ROOT_PATH)" -non_shared -I$(INTEGRITY_ROOT_PATH)

C0_AROPTS += -D__S32V234 -D__INTEGRITY__ -bsp devtree-arm64 -w --prototype_warnings -os_dir "$(INTEGRITY_ROOT_PATH)" -non_shared 
C0_CXOPTS += -D__S32V234 -D__INTEGRITY__ -bsp devtree-arm64 -w --prototype_warnings -os_dir "$(INTEGRITY_ROOT_PATH)" -non_shared 
C0_ASOPTS +=
C0_LDOPTS += -D__S32V234 -D__INTEGRITY__ -bsp devtree-arm64 -w --prototype_warnings -os_dir "$(INTEGRITY_ROOT_PATH)" -non_shared 
C0_FCLEAN += \
  ../src/*.d \
  ../src/.*.d \
  ../src/*.dbo \
  ../src/*.ti \
  *.dep \
  *.dla \
  *.dnm \
  *.map \
  *.ael \
  *.ti \
  *.dbo \
  $(C0_APP)_app \
  *_log.txt

C0_INCS += -I.

