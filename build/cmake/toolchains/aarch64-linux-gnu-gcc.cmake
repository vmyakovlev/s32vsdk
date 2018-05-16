##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2009, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Platform specific build settings for Treerunner Virtual Platform A53
#
#  AUTHOR
#    Stefan Schwarz
#
#  DATE
#    2013-01-23
#
#  LANGUAGE
#    cmake
#
##############################################################################
include(CMakeForceCompiler)

# Tell cmake we're cross compiling
set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_SYSTEM_PROCESSOR arm64)

set(TARGET_OS LINUX)
SET(TARGET_ARCH ARMV8)
SET(TARGET_COMPILER GCC)

#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   aarch64-linux-gnu-gcc)
SET(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# root filesystem containing shared libs
set (LINUX_ROOTFS $ENV{LINUX_S32V234_DIR})

# common compiler flags (TBD)
set (CMAKE_C_FLAGS "-mcpu=generic -march=armv8-a -DPNG_ARM_NEON_OPT=0 -O3" CACHE STRING "C Flags" FORCE )
set (CMAKE_CXX_FLAGS  " ${CMAKE_C_FLAGS}"  CACHE STRING "CXX Flags" FORCE)

# linker flags
set (CMAKE_EXE_LINKER_FLAGS "-Wl,-rpath-link,${LINUX_ROOTFS}/usr/local/tools/opencv/lib:${LINUX_ROOTFS}/usr/lib:${LINUX_ROOTFS}/lib -L${LINUX_ROOTFS}/usr/local/tools/opencv/lib -L${LINUX_ROOTFS}/usr/lib -L${LINUX_ROOTFS}/lib" CACHE STRING "EXE LD Flags" FORCE)
set (CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath-link,${LINUX_ROOTFS}/usr/local/tools/opencv/lib:${LINUX_ROOTFS}/usr/lib:${LINUX_ROOTFS}/lib -L${LINUX_ROOTFS}/usr/local/tools/opencv/lib -L${LINUX_ROOTFS}/usr/lib -L${LINUX_ROOTFS}/lib" CACHE STRING "SHARED LD Flags" FORCE)
set (CMAKE_MODULE_LINKER_FLAGS "-Wl,-rpath-link,${LINUX_ROOTFS}/usr/local/tools/opencv/lib:${LINUX_ROOTFS}/usr/lib:${LINUX_ROOTFS}/lib -L${LINUX_ROOTFS}/usr/local/tools/opencv/lib -L${LINUX_ROOTFS}/usr/lib -L${LINUX_ROOTFS}/lib" CACHE STRING "MODULE LD Flags" FORCE)

# compiler flags for debug target
set (CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS} CACHE STRING "C Debug Flags" FORCE)
set (CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS} CACHE STRING "CXX Debug Flags" FORCE)

# compiler flags for release target
set (CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS} CACHE STRING "C Release Flags" FORCE )
set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS}  CACHE STRING "CXX Release Flags" FORCE)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  $ENV{LINUX_S32V234_DIR})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# OpenCV environment
set (OpenCV_DIR "$ENV{LINUX_S32V234_DIR}/usr/local/tools/opencv/share/OpenCV")

