##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2013, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Build instructions for OpenCV
#
#  AUTHOR
#    Rostislav Hulik
#
#  DATE
#    2013-06-25
#
#  LANGUAGE
#    human readable text
#
##############################################################################

Please note that prior to installation of OpenCV, the FFMPEG library must be built again in order to 
set correct path on the host PC. If the FFMPEG cannot be rebuilt, please set the FFMPEG install path
correctly in s32v234_sdk/3rdparty/ffmpeg/linux-arm64/lib/pkgconfig/*.pc files, the "--prefix" variable.
This --prefix variable points absolutely to the installation directory of FFMPEG.

==========================================================================
| Please replace the SDK_ROOT text by the path to your s32v234_sdk folder |
==========================================================================


OpenCV build instructions for Integrity
=======================================

  1) Extract the OpenCV 3.1.0 sources into SDK_ROOT/../opencv_src directory
  -------------------------------------------------------------------
  
  2) Apply patch
  --------------
    - Switch into SDK_ROOT/../opencv_src directory 
      - Apply available patch by executing 
        "patch -p1 -i SDK_ROOT/3rdparty/ocv/integrity_3.1.0.patch"

  3) Modify v4l2 capture resolution & buffer count (optional)
  -----------------------------------------------------------
  (steps applied for supplied binaries)
    - In files:
	SDK_ROOT/../opencv_src/modules/highgui/src/cap_libv4l.cpp
	SDK_ROOT/../opencv_src/modules/highgui/src/cap_v4l.cpp        
      -	change: #define DEFAULT_V4L_WIDTH & #define DEFAULT_V4L_HEIGHT 
	to resolution supported by your camera HW. Example:
	#define DEFAULT_V4L_WIDTH 1280
	#define DEFAULT_V4L_HEIGHT 720
	Otherwise only the default 640x480 will be available.
      - change: #define MAX_V4L_BUFFERS #define DEFAULT_V4L_BUFFERS 
	to minimize the image dalay. Example:
	#define MAX_V4L_BUFFERS 1
	#define DEFAULT_V4L_BUFFERS 1      
        
  4) Build eventual dependacies (optional)
  ----------------------------------------
    - Build the qt library (see SDK_ROOT/qt/README)
    - Build the ffmpeg library (see SDK_ROOT/ffmpeg/README)
	
  5) Check Environment variables
  ------------------------------
    - Export necessary environment variables
	 
      INTEGRITY_ROOT_PATH="path/to/your/integrity/installation"
      MULTI_ROOT_PATH="path/to/your/multi/installation"
      MULTI_COMPILER_ROOT_PATH="path/to/your/multi/compiler/folder"

  6) Check the settings of the build
  ----------------------------------
    - edit cmake-v234ce-ghs-integrity/build.sh
	
      with_opencl="ON"
      with_qt="OFF" (ON if Qt support needed)
      WITH_FFMPEG="OFF" (ON if ffmpeg support needed)
      cmake_generator=""
        - CMake generator specifies a non default project generator, if not "" 
          (e.g. MinGW).
		
  7) Generate the Makefile and build
  ----------------------------------
    - Switch into SDK_ROOT/3rdparty/ocv/cmake-<platform> directory
    - Execute build.sh script
      - If the script does not set up the make correctly (errors), execute it 
        twice
      - The script uses realpath utility to generate full paths from relative 
        ones - wrong path interpretation can cause configuration or build errors
    - Run "make install"
    - In case of Integrity 12 the build might fail because of multiple 
      declarations of unsigned char type. In such case go to: 
      INTEGRITY_ROOT_PATH/INTEGRITY-include/sys/types.h and temporarily (for the
      OCV build) comment out line 129: "typedef unsigned char	unsigned char;".
    
 OpenCV build instructions for Linux
====================================

  1) Extract the OpenCV 3.1.0 sources into SDK_ROOT/../opencv_src directory
  -------------------------------------------------------------------------
  
  2) Update cmake file for video libraries (OPTIONAL)
  ---------------------------------------------------
  (required for FFMPEG library support)
  - Requires ffmpeg libraries to be built and installed to 
    SDK_ROOT/ffmpeg/linux-arm folder.
  - For information on how to build the ffmpeg libraries please refer to 
    SDK_ROOT/ffmpeg/README.txt.
  
  - To enable use of ffmpeg libraries overwrite file 
    SDK_ROOT/opencv_src/cmake/OpenCVFindLibsVideo.cmake with 
    SDK_ROOT/3rdparty/ocv/OpenCVFindLibsVideo.cmake.
    - this step enables manual setup of ffmpeg libraries install path during OCV 
      configuration (FFMPEG_LIB_DIR & FFMPEG_INCLUDE_DIR cmake variables).
      
  3) Modify v4l2 capture resolution & buffer count (OPTIONAL)
  -----------------------------------------------------------
  (steps applied for supplied binaries)
    - In files:
	SDK_ROOT/../opencv_src/modules/highgui/src/cap_libv4l.cpp
	SDK_ROOT/../opencv_src/modules/highgui/src/cap_v4l.cpp        
      -	change: #define DEFAULT_V4L_WIDTH & #define DEFAULT_V4L_HEIGHT 
	to resolution supported by your camera HW. Example:
	#define DEFAULT_V4L_WIDTH 1280
	#define DEFAULT_V4L_HEIGHT 720
	Otherwise only the default 640x480 will be available.
      - change: #define MAX_V4L_BUFFERS #define DEFAULT_V4L_BUFFERS 
	to minimize the image dalay. Example:
	#define MAX_V4L_BUFFERS 1
	#define DEFAULT_V4L_BUFFERS 1      

  4) Check the settings of the build
  ----------------------------------
    - edit cmake-v234ce-gnu-linux/build.sh
    
  5) Check environment variables
  ----------------------------------------
    - Add necessary entries to PATH variable
    
    export PATH=/path/to/arm/cross/compiler:/$PATH
    
  6) Generate the Makefile and build
  ----------------------------------
    - Switch into SDK_ROOT/3rdparty/ocv/cmake-v234ce-gnu-linux
    - Execute build.sh script
      - A default path 
        (SDK_ROOT/build/cmake/toolchains/aarch64-linux-gnu-gcc.cmake) 
        to tool-chain cmake file will be used. This searches for compiler in 
        /usr/local/tools nfs mounted folder => slow build.
      - Use 1. command line parameter to specify different tool-chain cmake file 
        (enables use of local compiler => faster build). 
        build.sh /path/to/own/toolchain.cmake
      - The script uses realpath utility to generate full paths from relative 
        ones - wrong path interpretation can cause configuration or build errors
        
FFMPEG libraries dependancy
---------------------------
If the ffmpeg support is turned on in the cmake-zn702-gnu-*/build.sh script each 
application using the OCV libraries has to be linked also with the FFMPEG ones. 
Otherwise the build will fail during linking.
        
Version numbering note:
-----------------------
By default after build and installation of the OCV libraries the build.sh script 
automaticatly removes the version number from the library hardlink filenames and 
deletes any symbolic links to it. This way the built libraries can be safely 
transferred to Windows and used at build time in cygwin environment. 

At runtime system looks for the libraries with version number appended to its 
file name. Therefore to execute applications linked to the OCV libraries, 
library files with version strings appended have to be supplied. They can be 
found prepared in SDK_ROOT/os/sdcard_build/lib folder.

To find out dependancies of the elf file, the 
"readelf -d /path/to/the/elf/binary" command can be used.

To allow the libraries to be located when application is executed the 
LD_LIBRARY_PATH has to be set to their location by executing command:
export LD_LIBRARY_PATH=/path/to/runtime/libraries

Running the build in Windows environment
----------------------------------------
When attempting to run the OpenCv library build on Windows in Cygwin usind an
stand-alone compiler like GHS Multi compiler, problems with proper settings of
compiler binary paths in CMAKE toolchain file may arrise. 

In such case it is recomended to:
  - install CMAKE and MAKE utilities directly to Windows,
  - add their executable paths to Windows PATH variable,
  - add path to cygwin/bin folder to Windows PATH variable 
    (to use sh interpreter)
  - run sh build.sh script from s32v234_sdk/3rdparty/ocv directory directly in Windows
    command line.
  
