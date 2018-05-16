##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2009, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    CMAKE toolchain file. Original taken from here:
#    http://svn.apache.org/repos/asf/etch/trunk/binding-cpp/runtime/lib/capu/cmake/acme/toolchain/Integrity1002_armv7l_Emerald-P-ES1.toolchain
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
INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME Integrity)

SET(TARGET_OS INTEGRITY)
SET(TARGET_ARCH ARMV8)
SET(TARGET_COMPILER MULTI)
SET(TARGET_BSP devtree-arm64)
SET(MULTI 1)

#set(MAKE_OS "Integrity" CACHE STRING "Defines the target operating system." FORCE)
SET(DEVICE_NAME "S32V234")
SET(DEVICE_TARGET_BUILD "ON")
SET(STUDIO_COMPILER "MULTI")

##### setup for Integrity Multi target compiler
SET(INTEGRITY_INSTALL_PATH $ENV{INTEGRITY_ROOT_PATH} CACHE PATH "Root path to INTEGRITY installation.")
SET(integrity_platform -bsp=${TARGET_BSP} -os_dir=${INTEGRITY_INSTALL_PATH})
SET(integrity_cflags_shared "-bsp=devtree-arm64 -os_dir=${INTEGRITY_INSTALL_PATH} -cpu=armv8 -Ospeed --link_once_templates -L${INTEGRITY_INSTALL_PATH}/libs/arm64 -L${INTEGRITY_INSTALL_PATH}/libs/devtree-arm64 -startfile_dir=${INTEGRITY_INSTALL_PATH}/libs/devtree-arm64 -lposix -livfs -ldl -DCHECKED_BUILD")
SET(integrity_cflag_optimizer "-O2")

SET(CMAKE_C_FLAGS "${integrity_cflags_shared}" CACHE STRING "C Flags" FORCE)
SET(CMAKE_CXX_FLAGS "${integrity_cflags_shared} --exceptions " CACHE STRING "CXX Flags" FORCE)
SET(CMAKE_INTEX_FLAGS ${integrity_platform} -dynamic CACHE STRING "INTEX Flags" FORCE)

SET(CMAKE_C_FLAGS_DEBUG "${integrity_cflags_shared} -G -D_DEBUG" CACHE STRING "C Debug Flags" FORCE)
SET(CMAKE_CXX_FLAGS_DEBUG "${integrity_cflags_shared} -G -D_DEBUG" CACHE STRING "CXX Debug Flags" FORCE)

#### Change Compiler to MULTI
SET(MULTI_TOOLCHAIN_PATH $ENV{MULTI_ROOT_PATH} CACHE PATH "Root path to MULTI tool chain.")
SET(MULTI_COMPILER_PATH $ENV{MULTI_COMPILER_ROOT_PATH} CACHE PATH "Root path to MULTI compiler.")
IF (WIN32)
    SET(CMAKE_C_COMPILER "${MULTI_COMPILER_PATH}/ccintarm64.exe")
	SET(CMAKE_CXX_COMPILER "${MULTI_COMPILER_PATH}/cxintarm64.exe")
ELSE()
    SET(CMAKE_C_COMPILER "${MULTI_COMPILER_PATH}/ccintarm64")
	SET(CMAKE_CXX_COMPILER "${MULTI_COMPILER_PATH}/cxintarm64")
ENDIF()


#### Change other tools in toolchain
SET(CMAKE_AR "${MULTI_COMPILER_PATH}/cxarm64" CACHE FILEPATH "MULTI AR" FORCE)
SET(CMAKE_LINKER "${MULTI_COMPILER_PATH}/elxr" CACHE FILEPATH "MULTI LINKER" FORCE)
SET(CMAKE_NM "${MULTI_COMPILER_PATH}/gnm" CACHE FILEPATH "MULTI NM" FORCE)
SET(CMAKE_STRIP "${MULTI_COMPILER_PATH}/gstrip" CACHE FILEPATH "MULTI STRIP" FORCE)

SET(CMAKE_C_ARCHIVE_CREATE "<CMAKE_AR> -v -c -MD -archive -o <TARGET> <OBJECTS>")
SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> -v -c -MD -archive --link_once_templates -o <TARGET> <OBJECTS>")
#SET(CMAKE_C_ARCHIVE_CREATE "cxarm -v -archive -o <TARGET>  <LINK_FLAGS>  <OBJECTS>")

#### GHS specific tools in toolchain
SET(CMAKE_INTEX "${MULTI_COMPILER_PATH}/intex" CACHE FILEPATH "MULTI INTEX" FORCE)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH ${INTEGRITY_INSTALL_PATH})

# INTEGRITY libs are located here
SET(LINK_DIRECTORIES ${INTEGRITY_INSTALL_PATH}/libs/arm64)
#SET(LINK_DIRECTORIES ${INTEGRITY_INSTALL_PATH}/libs/Vivante)

# search for programs in the build host directories
#SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

ADD_DEFINITIONS("-DOS_INTEGRITY")
ADD_DEFINITIONS("-DARCH_ARMV8")

######## those are needed for openCV ###############
add_definitions(-DPATH_MAX=256)

# this is usually defined in stdio.h
add_definitions(-DP_tmpdir="/tmp")
add_definitions(-D__INTEGRITY__)	# cv core: system.cpp

