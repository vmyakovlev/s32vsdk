##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2013, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Generator of OpenCV + OpenCL + Qt makefiles for Integrity RTOS cross compilation
#
#  AUTHOR
#    Rostislav Hulik
#
#  DATE
#    2013-06-12
#
##############################################################################

#!/bin/sh

if [ -z "$INTEGRITY_ROOT_PATH" ]
  then
  echo ""
  echo "  Error: you need to define INTEGRITY_ROOT_PATH environment variable."
  echo ""
  exit;
fi

if [ -z "$MULTI_ROOT_PATH" ]
then
  echo ""
  echo "  Error: you need to define MULTI_ROOT_PATH environment variable."
  echo ""
  exit;
fi

if [ -z "$MULTI_COMPILER_ROOT_PATH" ]
then
  echo ""
  echo "  Error: you need to define MULTI_COMPILER_ROOT_PATH environment variable."
  echo ""
  exit;
fi

SDK_ROOT=../../../../s32v234_sdk

OCV_ROOT=$SDK_ROOT/../opencv_src

cmake \
  -D WITH_CUDA=OFF									\
  -D WITH_QT=OFF									\
  -D BUILD_PERF_TESTS=OFF 								\
  -D BUILD_TESTS=OFF 									\
  -D BUILD_EXAMPLES=OFF 								\
  -D WITH_WEBP=OFF 									\
  -D WITH_OPENCL=OFF 									\
  -D OPENCL_INCLUDE_DIR=$INTEGRITY_ROOT_PATH/INTEGRITY-include/Vivante/sdk/inc 		\
  -D OPENCL_ROOT_DIR=$INTEGRITY_ROOT_PATH/libs/Vivante 					\
  -D CMAKE_TOOLCHAIN_FILE=$SDK_ROOT/build/cmake/toolchains/aarch64-integrity-ghs.cmake	\
  -D BUILD_SHARED_LIBS=OFF 								\
  -D CMAKE_INSTALL_PREFIX=CMAKE_CURRENT_BINARY_DIR/../../integrity-arm64 		\
  -D CMAKE_BUILD_TYPE=Debug 								\
  -D BUILD_opencv_apps=OFF								\
  -D WITH_FFMPEG=ON									\
  -D FFMPEG_LIB_DIR=$(realpath $SDK_ROOT/3rdparty/ffmpeg/integrity-arm64/lib)		\
  -D FFMPEG_INCLUDE_DIR=$(realpath $SDK_ROOT/3rdparty/ffmpeg/integrity-arm64/include)	\
  $OCV_ROOT
  
  
  
