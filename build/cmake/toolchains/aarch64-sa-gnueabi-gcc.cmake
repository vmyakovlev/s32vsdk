##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2009, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Platform specific build settings for S32V234 EVB
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
set(CMAKE_SYSTEM_NAME Standalone)

set(CMAKE_SYSTEM_PROCESSOR arm)

set(TARGET_OS NOOS)
SET(TARGET_ARCH ARMV8L)
SET(TARGET_COMPILER GCC)

#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER   aarch64-elf-gcc)
SET(CMAKE_CXX_COMPILER aarch64-elf-g++)

# root filesystem containing shared libs
set (LINUX_ROOTFS %ENV{SCRIPT_LINUX_FS})

# common compiler flags (TBD)
#set (CMAKE_C_FLAGS "-ggdb3 -mcpu=cortex-a53  -mlittle-endian -fomit-frame-pointer -fno-common -nostdlib -ffunction-sections -fdata-sections -fno-builtin -fno-unwind-tables -static" CACHE STRING "C Flags" FORCE )
#set (CMAKE_CXX_FLAGS  "-ggdb3 -mcpu=cortex-a53 -mlittle-endian -fomit-frame-pointer -fno-common -nostdlib -ffunction-sections -fdata-sections -fno-builtin -static -fno-unwind-tables"  CACHE STRING "CXX Flags" FORCE)
set (CMAKE_C_FLAGS "-mcpu=cortex-a53 -O3 -nostdlib -fno-builtin -fexceptions -ffunction-sections -fdata-sections" CACHE STRING "C Flags" FORCE )
set (CMAKE_CXX_FLAGS  "-mcpu=cortex-a53 -O3 -nostdlib -fno-builtin -fexceptions -ffunction-sections -fdata-sections"  CACHE STRING "CXX Flags" FORCE)

# linker flags
set (CMAKE_EXE_LINKER_FLAGS "" CACHE STRING "EXE LD Flags" FORCE)
set (CMAKE_SHARED_LINKER_FLAGS "" CACHE STRING "SHARED LD Flags" FORCE)
set (CMAKE_MODULE_LINKER_FLAGS "" CACHE STRING "MODULE LD Flags" FORCE)

# compiler flags for debug target
set (CMAKE_C_FLAGS_DEBUG ${CMAKE_C_FLAGS} CACHE STRING "C Debug Flags" FORCE)
set (CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS} CACHE STRING "CXX Debug Flags" FORCE)

# compiler flags for release target
set (CMAKE_C_FLAGS_RELEASE ${CMAKE_C_FLAGS} CACHE STRING "C Release Flags" FORCE )
set (CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS}  CACHE STRING "CXX Release Flags" FORCE)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  ${LINUX_ROOTFS})

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# OpenCV environment
set (OpenCV_DIR "$ENV{LINUX_ROOT_PATH}/usr/local/tools/opencv/share/OpenCV")

