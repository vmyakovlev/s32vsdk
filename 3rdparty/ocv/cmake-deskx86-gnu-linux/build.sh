##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2013, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Generator of OpenCV makefiles for Linux desktop compilation
#
#  AUTHOR
#    Tomas Babinec
#
#  DATE
#    2013-10-16
#
##############################################################################

#!/bin/sh

SDK_ROOT=../../../../s32v234_sdk
OCV_ROOT=$SDK_ROOT/../opencv_src

cmake 												\
  -D CMAKE_INSTALL_PREFIX=$SDK_ROOT/3rdparty/ocv/linux-x86					\
  -D WITH_QT=OFF 										\
  -D WITH_FFMPEG=OFF 										\
  -D WITH_GSTREAMER=OFF 									\
  -D WITH_PYTHON=OFF 										\
  -D WITH_GTK=ON 										\
  -D BUILD_PERF_TESTS=OFF 									\
  -D BUILD_TESTS=OFF 										\
  -D BUILD_opencv_apps=OFF 									\
  -D CMAKE_BUILD_TYPE=RELEASE 									\
  $OCV_ROOT

echo "Generating done, building....."

make install
