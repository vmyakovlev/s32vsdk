GDI INTEGRITY OS kernel monolith properties
===========================================
  - supports GDI specific requirements (POSIX, shared memory)
  - requires VPD and OAL drivers' MULTI projects to be available in the VSDK
  - included VFS module works with static ISO rootfile system. The iso image has
    to be named static.iso and must be located in the folowing directory: 
    s32v234_sdk/os/integrity/aarch64/gdi_kernel/multi/proj directory.
  - includes Posix system virtual server and shared memory manager.
  
  -requires the following environment variables:
    INTEGRITY_ROOT_PATH="path/to/the/integrity/install"  
    MULTI_ROOT_PATH="path/to/the/multi/tools"
    MULTI_COMPILER_ROOT_PATH="path/to/the/multi/compiler" 

Build the INTEGRITY OS
======================

1. Build the OS project
-----------------------
  - Open the s32v234_sdk/os/integrity/aarch64/gdi_kernel/multi/default.gpj 
    project in MULTI.
  - Build the project.
          
(Optional) Modify the devicetree.dtb
====================================
(e.g. to modify ammount of RAM available to the OS)

1. Modify treerunner.dts
------------------------
  - the treerunner.dts file is located in INTEGRITY_ROOT_PATH/treerunner/ve-fvp

2. Compile the dts file
-----------------------    
  - Use dtc compiler provided with integrity installation located in:
    INTEGRITY_ROOT_PATH/multi/bin/<host os>.
  - run  
    dtc -I dts -O dtb -o treerunner.dtb treerunner.dts
    
3. Replace the treerunner.dtb file with the new one
---------------------------------------------------
  - copy the new treerunner.dtb file to INTEGRITY_ROOT_PATH/treerunner/ve-fvp          
    
(Optional) Create new root file system iso image
================================================

1. Put desired files and directories to a folder (e.g. ./iso_rfs).
------------------------------------------------------------------

2. cd to the ./iso_rfs directory.
---------------------------------

3. Pack the iso image 
---------------------
   run: "mkisofs -J -o static.iso ./*"    
     
    
Run the INTEGRITY OS kernel monolith
====================================

  - please refer to VLAB_README.txt located in s32v234/os directory. 