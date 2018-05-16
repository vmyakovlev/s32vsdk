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

SDK_ROOT=../../../../s32v234_sdk
OCV_ROOT=$SDK_ROOT/../opencv_src

DEFAULT_TOOLCHAIN="/build/cmake/toolchains/aarch64-sa-gnueabi-gcc.cmake"
TOOLCHAIN=$SDK_ROOT$DEFAULT_TOOLCHAIN

COMMAND="$(aarch64-elf-gcc -print-file-name=)"
export SCRIPT_LINUX_FS=${COMMAND}

if [ $# -gt 0 ]
  then
  TOOLCHAIN=$1
fi

echo "Toolchain file path to be used:"
echo $TOOLCHAIN 

cmake 												\
  -D CMAKE_INSTALL_PREFIX=$SDK_ROOT/3rdparty/ocv/standalone-arm64				\
  -D CMAKE_TOOLCHAIN_FILE=$TOOLCHAIN								\
  -D WITH_QT=OFF 										\
  -D WITH_FFMPEG=OF 										\
  -D WITH_GSTREAMER=OFF 									\
  -D WITH_GTK=OFF 										\
  -D WITH_1394=OFF 										\
  -D WITH_AVFOUNDATION=OFF 									\
  -D WITH_CUDA=OFF 										\
  -D WITH_VTK=OFF 										\
  -D WITH_EIGEN=OFF 										\
  -D WITH_VFW=OFF 										\
  -D WITH_FFMPEG=OFF 										\
  -D WITH_GSTREAMER=OFF 									\
  -D WITH_GSTREAMER_0_10=OFF 									\
  -D WITH_GTK=OFF 										\
  -D WITH_IMAGEIO=OFF 										\
  -D WITH_IPP=OFF 										\
  -D WITH_JASPER=OFF 										\
  -D WITH_WEBP=OFF 										\
  -D WITH_JPEG=OFF 										\
  -D WITH_OPENEXR=OFF 										\
  -D WITH_OPENGL=OFF 										\
  -D WITH_OPENNI=OFF 										\
  -D WITH_PNG=OFF 										\
  -D WITH_PVAPI=OFF 										\
  -D WITH_GIGEAPI=OFF 										\
  -D WITH_QT=OFF 										\
  -D WITH_WIN32UI=OFF 										\
  -D WITH_QUICKTIME=OFF 									\
  -D WITH_TBB=OFF 										\
  -D WITH_OPENMP=OFF 										\
  -D WITH_CSTRIPES=OFF 										\
  -D WITH_TIFF=OFF 										\
  -D WITH_UNICAP=OFF 										\
  -D WITH_V4L=OFF 										\
  -D WITH_DSHOW=OFF 										\
  -D WITH_MSMF=OFF 										\
  -D WITH_XIMEA=OFF 										\
  -D WITH_XINE=OFF 										\
  -D WITH_OPENCL=OFF 										\
  -D WITH_INTELPERC=OFF 									\
  -D BUILD_SHARED_LIBS=OFF 									\
  -D BUILD_opencv_apps=OFF 									\
  -D BUILD_ANDROID_EXAMPLES=OFF 								\
  -D BUILD_DOCS=OFF 										\
  -D BUILD_EXAMPLES=OFF 									\
  -D BUILD_PERF_TESTS=OFF 									\
  -D BUILD_TESTS=OFF 										\
  -D BUILD_FAT_JAVA_LIB=OFF 									\
  -D BUILD_ANDROID_PACKAGE=OFF 									\
  -D BUILD_ZLIB=ON 										\
  -D BUILD_opencv_core=ON 									\
  -D BUILD_opencv_flann=ON 									\
  -D BUILD_opencv_imgproc=ON 									\
  -D BUILD_opencv_highgui=ON 									\
  -D BUILD_opencv_features2d=ON 								\
  -D BUILD_opencv_calib3d=ON 									\
  -D BUILD_opencv_ml=ON 									\
  -D BUILD_opencv_video=ON 									\
  -D BUILD_opencv_legacy=OFF 									\
  -D BUILD_opencv_objdetect=ON 									\
  -D BUILD_opencv_photo=ON 									\
  -D BUILD_opencv_gpu=OFF 									\
  -D BUILD_opencv_nonfree=ON 									\
  -D BUILD_opencv_contrib=ON 									\
  -D BUILD_opencv_stitching=ON 									\
  -D BUILD_opencv_superres=ON 									\
  -D BUILD_opencv_ts=ON 									\
  -D BUILD_opencv_videostab=OFF 								\
  -D BUILD_PERF_TESTS=OFF 									\
  -D BUILD_TESTS=OFF 										\
  -D BUILD_opencv_apps=OFF 									\
  -D CMAKE_BUILD_TYPE=RELEASE 									\
  $OCV_ROOT

echo "Generating done, building....."

make install

# change library filenames to hardlinks with no version number and remove any symbolic links
#bash ./sym2hard.sh ../linux-arm/lib d
