##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2014, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Configures, builds and installs fftw libraries from source files.
#
#  AUTHOR
#    Roman Kubica
#
#  DATE
#    2015-10-07
#
#  LANGUAGE
#    bash script
#
##############################################################################

#!/bin/sh

# remember current path
ORIGIN=$PWD
SDK_ROOT=$PWD/../../..
NJOBS=$(($(nproc)*2))

cp $ORIGIN/patch_files/fftw-wisdom.c $SDK_ROOT/../fftw_src/tools
cp $ORIGIN/patch_files/bench-main.c $SDK_ROOT/../fftw_src/libbench2
cp $ORIGIN/patch_files/timer.c $SDK_ROOT/../fftw_src/libbench2
cp $ORIGIN/patch_files/simd-neon.h $SDK_ROOT/../fftw_src/simd-support
cp $ORIGIN/patch_files/configure $SDK_ROOT/../fftw_src

cd ${SDK_ROOT}/libs/utils/oal/user/build-v234ce-gnu-linux-d
make -j ${NJOBS}


cd $SDK_ROOT/../fftw_src

echo ""
echo "Configure..."
echo ""

ARM_INCS="-I${SDK_ROOT}/include"

ARM_LIBS_PATH="-L${SDK_ROOT}/libs/utils/oal/user/build-v234ce-gnu-linux-d"

LIBS="-loal \
-lc \
-lg \
-lgcc \
-lstdc++"


PREFIX=" --prefix=${SDK_ROOT}/3rdparty/fftw/linux-arm64 --host=aarch64-linux-gnu --enable-single --enable-neon "

PREFIX2="CC=aarch64-linux-gnu-gcc -mcpu=cortex-a53 -nostdlib -fno-builtin -fno-exceptions "

./configure CPPFLAGS="${ARM_INCS}" LDFLAGS="${ARM_LIBS_PATH}" ${PREFIX} "${PREFIX2}" LIBS="${LIBS}"

#################
# Build & Install


echo ""
echo "Building FFTW library"
echo ""

make -j ${NJOBS}

echo ""
echo "Installing FFTW library"
echo ""

make install

cd $ORIGIN

