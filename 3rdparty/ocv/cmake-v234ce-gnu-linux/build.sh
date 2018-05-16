##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2013, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Generator of OpenCV makefiles for Linux ARM cross-compilation
#
#  AUTHOR
#    Tomas Babinec
#
#  DATE
#    2013-10-16
#
#  LANGUAGE
#    bash script
#
##############################################################################

#!/bin/sh

if [ -z "$LINUX_S32V234_DIR" ];
  then 
    echo "Please define LINUX_S32V234_DIR pointing to kernel directory ";
    exit 0
fi

SDK_ROOT=$PWD/../../../../s32v234_sdk
OCV_ROOT=$SDK_ROOT/../opencv_src

export PKG_CONFIG_PATH=$SDK_ROOT/3rdparty/ffmpeg/linux-arm64/lib/pkgconfig/
DEFAULT_TOOLCHAIN="/build/cmake/toolchains/aarch64-linux-gnu-gcc.cmake"
TOOLCHAIN=$SDK_ROOT$DEFAULT_TOOLCHAIN

echo $SDK_ROOT/3rdparty/ffmpeg/linux-arm64/include

if [ $# -gt 0 ]
  then
  TOOLCHAIN=$1
fi

echo "Toolchain file path to be used:"
echo $TOOLCHAIN 

cmake 												\
  -D CMAKE_INSTALL_PREFIX=$SDK_ROOT/3rdparty/ocv/linux-arm64					\
  -D CMAKE_TOOLCHAIN_FILE=$TOOLCHAIN								\
  -D WITH_QT=OFF 										\
  -D WITH_FFMPEG=ON 										\
  -D FFMPEG_LIB_DIR=$SDK_ROOT/3rdparty/ffmpeg/linux-arm64/lib					\
  -D FFMPEG_INCLUDE_DIR=$SDK_ROOT/3rdparty/ffmpeg/linux-arm64/include				\
  -D WITH_GSTREAMER=OFF 									\
  -D WITH_PYTHON=OFF 										\
  -D WITH_NEON=OFF 										\
  -D ENABLE_NEON=ON \
  -D WITH_GTK=OFF 										\
  -D BUILD_PERF_TESTS=OFF 									\
  -D BUILD_TESTS=OFF 										\
  -D BUILD_opencv_apps=OFF 									\
  -D CMAKE_BUILD_TYPE=RELEASE 									\
  $OCV_ROOT

echo "Generating done, building....."

make install

# change library filenames to hardlinks with no version number and remove any symbolic links
bash ./sym2hard.sh ../linux-arm64/lib d
