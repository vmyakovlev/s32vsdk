Building with MULTI cmd line tools
==================================
This directory contains toolchain specific cmake
file to build with MULTI tools. 

You need to set following environment variables:

  1) export INTEGRITY_ROOT_PATH="path/to/the/integrity/install"
  -------------------------------------------------------------
  
  2) export MULTI_ROOT_PATH="path/to/the/multi/tools"
  ---------------------------------------------------
    - Generally it's the folder in ghs named multi_XXX
	
  3) export MULTI_COMPILER_ROOT_PATH="path/to/your/multi/compiler/folder"
  ------------------------------------------------------
	- Generally it's the folder in ghs named comp_XXXXXX

  To avoid recurring warnings of type:

  "System is unknown to cmake, create:
   Platform/Integrity to use this system, please send your config 
   file to cmake@www.cmake.org so it can be added to cmake"

  you may want to copy Integrity.cmake file to 
  /usr/share/cmake-2.8/Modules/Platform/.
  or wherever your cmake installation is located.
  
