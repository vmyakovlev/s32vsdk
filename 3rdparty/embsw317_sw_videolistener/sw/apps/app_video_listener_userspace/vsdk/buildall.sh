#!/bin/bash
#####################################################################
#   File: buildall.sh
#   Copyright (c) 2016 NXP Semiconductors
#####################################################################

#####################################################################
#     THIS IS AN EXAMPLE OF BUILDING OF VIDEO LISTENER BINARIES     #
#####################################################################

# Colorize output of this script
exec > >(sed "-es/ error:/\x1b[1;31m error\x1b[0m:/g" \
         "-es/missing/\x1b[1;31mmissing\x1b[0m/g" \
         "-es/ warning:/\x1b[1;33m warning\x1b[0m:/g" \
         "-es/ info:/\x1b[1;32m info\x1b[0m:/g" \
         "-es/FAILED/\x1b[1;31mFAILED\x1b[0m/g" \
         "-es/SUCCEEDED/\x1b[1;32mSUCCEEDED\x1b[0m/g" \
         "-es/SUCCESS/\x1b[1;32mSUCCESS\x1b[0m/g")
exec 2>&1

# Set variables below (or in your shell)
#export VSDK_PATH=
#export LINUX_PATH=
#export OUT_PATH=
#export LINUX_TOOLCH_DIR=
#export LINUX_TOOLCH_PREF=
#export BARE_TOOLCH_DIR=
#export BARE_TOOLCH_PREF=

# Process command line arguments
if [ "$1" = "-h" ] || [ "$1" = "--help" ]
then
    echo "USAGE: buildall.sh [clean]"
    sleep "0.2"
    exit 0
fi

export PATH=$LINUX_TOOLCH_DIR:$BARE_TOOLCH_DIR:$PATH

# Prepare auxiliary variables
export RUN_STATUS=SUCCEEDED
if [ "$1" == "clean" ]
    then export ACTION=clean
	else export ACTION=allsub
fi
export VIDLISTENER_DIR=$PWD/../../..

# For all VSDK drivers
export LINUX_S32V234_DIR=$LINUX_PATH
export S32V234_SDK_ROOT=$VSDK_PATH

# Check needed variables
ERR=0
echo
echo === Checking variables =========================
if [ "" = "$VSDK_PATH" ]; then echo "ERROR: Please set VSDK_PATH (e.g. ~/s32v234_sdk)"; ERR=1;
else if [ ! -d "$VSDK_PATH" ]; then echo "ERROR: VSDK_PATH is wrong"; ERR=1; 
fi; fi
if [ "" = "$LINUX_PATH" ]; then echo "ERROR: Please set LINUX_PATH (e.g. ~/linux)"; ERR=1;
else if [ ! -d "$LINUX_PATH" ]; then echo "ERROR: LINUX_PATH is wrong"; ERR=1; 
fi; fi
if [ "" = "$OUT_PATH" ]; then echo "ERROR: Please set OUT_PATH (e.g. output)"; ERR=1;
else if [ ! -d "$OUT_PATH" ]; then mkdir -p $OUT_PATH || { echo "ERROR: Failed to create directory OUT_PATH=$OUT_PATH"; ERR=1; }
fi; fi
if [ "" = "$LINUX_TOOLCH_DIR" ]; then echo "ERROR: Please set LINUX_TOOLCH_DIR (e.g. ~/compilers/gcc-linaro-4.8/bin)"; ERR=1;
else if [ ! -d "$LINUX_TOOLCH_DIR" ]; then echo "ERROR: LINUX_TOOLCH_DIR is wrong"; ERR=1; 
fi; fi
if [ "" = "$LINUX_TOOLCH_PREF" ]; then echo "ERROR: Please set LINUX_TOOLCH_PREF (e.g. aarch64-linux-gnu-)"; ERR=1;
fi
if [ "" = "$BARE_TOOLCH_DIR" ]; then echo "ERROR: Please set BARE_TOOLCH_DIR (e.g. ~/compilers/gcc-linaro-4.8/bin)"; ERR=1;
else if [ ! -d "$BARE_TOOLCH_DIR" ]; then echo "ERROR: BARE_TOOLCH_DIR is wrong"; ERR=1; 
fi; fi
if [ "" = "$BARE_TOOLCH_PREF" ]; then echo "ERROR: Please set BARE_TOOLCH_PREF (e.g. aarch64-none-elf-)"; ERR=1;
fi
if [ "$ERR" != "0" ]
then
    echo "NOTE: You can set the variables in your system or in this file $0"
    echo
    exit 1
else
    echo Variables OK
    echo
fi

# Create output folder
mkdir -p $OUT_PATH || { echo FAILED to create output dir \"$OUT_PATH\"; exit 1; }

####################################################################################################
# build SM_DRV
echo === sm_drv.ko ==================================
cd $VIDLISTENER_DIR/kernel/sm_drv
make KERNELDIR=$LINUX_PATH ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- && cp sm_drv.ko $OUT_PATH &&
echo SUCCESS: sm_drv.ko || { echo FAILED: sm_drv.ko; RUN_STATUS=FAILED; }
echo

####################################################################################################
# build applications (userspace app and firmware) from VSDK
echo === applications ==================================
export AVB_TOOLCHAIN_DIR=$BARE_TOOLCH_DIR
cd $S32V234_SDK_ROOT/demos/avb/avb_isp_jpeg_4stream/build-v234ce-gnu-linux-d && make $ACTION && 
cp theA5App.bin vid_list.cfg avb_isp_jpeg_4stream.elf $OUT_PATH &&
echo SUCCESS: applications || { echo FAILED: applications; RUN_STATUS=FAILED; }

####################################################################################################
# build VSDK kernel modules
export ARCH=arm
export CROSS_COMPILE=aarch64-linux-gnu-
echo === oal_cma.ko ==================================
cd $S32V234_SDK_ROOT/libs/utils/oal/kernel/build-v234ce-gnu-linux-d       && make $ACTION && cp oal_cma.ko $OUT_PATH &&
echo SUCCESS: oal_cma.ko || { echo FAILED: oal_cma.ko; RUN_STATUS=FAILED; }
echo === csi.ko ==================================
cd $S32V234_SDK_ROOT/libs/isp/csi/kernel/build-v234ce-gnu-linux-d         && make $ACTION && cp csi.ko     $OUT_PATH &&
echo SUCCESS: csi.ko     || { echo FAILED: csi.ko    ; RUN_STATUS=FAILED; }
#echo === cam.ko ==================================
#cd $S32V234_SDK_ROOT/libs/isp/cam_generic/kernel/build-v234ce-gnu-linux-d && make $ACTION && cp cam.ko     $OUT_PATH &&
#echo SUCCESS: cam.ko     || { echo FAILED: cam.ko    ; RUN_STATUS=FAILED; }
echo === seq.ko ==================================
cd $S32V234_SDK_ROOT/libs/isp/sequencer/kernel/build-v234ce-gnu-linux-d   && make $ACTION && cp seq.ko     $OUT_PATH &&
echo SUCCESS: seq.ko     || { echo FAILED: seq.ko    ; RUN_STATUS=FAILED; }
echo === fdma.ko ==================================
cd $S32V234_SDK_ROOT/libs/isp/fdma/kernel/build-v234ce-gnu-linux-d        && make $ACTION && cp fdma.ko    $OUT_PATH &&
echo SUCCESS: fdma.ko    || { echo FAILED: fdma.ko   ; RUN_STATUS=FAILED; }
#echo === h264enc.ko ==================================
#cd $S32V234_SDK_ROOT/libs/isp/h264enc/kernel/build-v234ce-gnu-linux-d     && make $ACTION && cp h264enc.ko $OUT_PATH &&
#echo SUCCESS: h264enc.ko || { echo FAILED: h264enc.ko; RUN_STATUS=FAILED; }
echo === h264dcd.ko ==================================
cd $S32V234_SDK_ROOT/libs/isp/h264dec/kernel/build-v234ce-gnu-linux-d     && make $ACTION && cp h264dcd.ko $OUT_PATH &&
echo SUCCESS: h264dcd.ko || { echo FAILED: h264dcd.ko; RUN_STATUS=FAILED; }
echo === jpegdcd.ko ==================================
cd $S32V234_SDK_ROOT/libs/isp/jpegdec/kernel/build-v234ce-gnu-linux-d     && make $ACTION && cp jpegdcd.ko $OUT_PATH &&
echo SUCCESS: jpegdcd.ko || { echo FAILED: jpegdcd.ko; RUN_STATUS=FAILED; }
#echo === viulite.ko ==================================
#cd $S32V234_SDK_ROOT/libs/isp/viu/kernel/build-v234ce-gnu-linux-d         && make $ACTION && cp viulite.ko $OUT_PATH &&
#echo SUCCESS: viulite.ko || { echo FAILED: viulite.ko; RUN_STATUS=FAILED; }
echo === sram.ko ==================================
cd $S32V234_SDK_ROOT/libs/isp/sram/kernel/build-v234ce-gnu-linux-d        && make $ACTION && cp sram.ko    $OUT_PATH &&
echo SUCCESS: sram.ko    || { echo FAILED: sram.ko   ; RUN_STATUS=FAILED; }

# Print summary
echo
echo ================================================
echo "$RUN_STATUS; results in $OUT_PATH":
ls $OUT_PATH
echo ================================================
sleep 0.2 # needed due to console output colorization
if [ "$RUN_STATUS" == "SUCCEEDED" ]
  then exit 0
  else exit 1
fi
