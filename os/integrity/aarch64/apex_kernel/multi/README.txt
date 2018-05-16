Build the INTEGRITY OS
======================

1. Build the OS project
-----------------------
  - Open the s32v234_sdk/os/integrity/kernel/multi/default.gpj project in MULTI
  - Build the project
  
2. Make a bootable image
------------------------
   
   gmemfile baseimage
   mkimage -A arm -O linux -T kernel -C none -a 0x100000 -e 0x100000 -n "INTEGRITY" -d baseimage.bin uImage_integrity
   
3. Set the ATAGS file
---------------------
  - Open the zynq-atags.bin file with the hex editor
  - Change the values appropriately
  - Save the file

4. Force U-boot to use preloaded ATAGs
------------------------------------
  - See s32v234_sdk/os/boot/README.txt (Build U-Boot image)
  
  - The environment (IP address etc in OS), can be hard-code into the monolith kernel project (will be used if no ATAGs found).
    - Use gbin2c -input zynq-atags.bin -o zynq-atags.c 
    - Open zynq-atags.c in text editor and copy the contents of array into the s32v234_sdk/os/integrity/kernel/multi/src/net_server_child_image.c file
      
   
Build the devicetree.dtb
========================

1. Use the zynq-zc702-base-trd.dts file present in this directory
-----------------------------------------------------------------

2. Compile the dts file
-----------------------

  - For now, we are using the dtc compiler present in linux-xlnx tree (please see the linux dir).
    
    linux-xlnx/scripts/dtc/dtc -I dts -O dtb -o /devicetree.dtb mdevicetree.dts
  
    
    
Run the INTEGRITY example
=========================
  - To the SD card, copy following files:
  
    uImage_integrity    // See this file or sdcard_build directory
    devicetree.dtb      // See this file or sdcard_build directory
    boot.bin            // See boot directory or sdcard_build directory
    zynq-atags.bin	// See this file or sdcard_build directory
    
  - When running the board, break the autobuild and set your environment in u-boot (see the boot directory) 
  - Save the environment (saveenv)
  - Reset the board