Building of examples for Linux OS
=================================

1) Set following system variables:
----------------------------------
  - export ARCH=arm
     - Target architecture
  - export PATH=/path/to/your/CHESS/compiler/binaries:$PATH
     - Path to the target APEX compiler binaries
  - export PATH=/path/to/your/arm/cross/compiler/binaries:$PATH
     - Path to the compiler binaries
     
2) Build the application
-----------------------
  BUILD WITHOUT APEX COMPILER
  ---------------------------
  - run "make" in corresponding build-v234ce-* directory
  
  REBUILDING THE APEX GRAPHS
  --------------------------
  - cd to corresponding graphs/build-apu-tct-sa-d directory
  - run "make allsub" command
 
