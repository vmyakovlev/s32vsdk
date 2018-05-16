##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2014, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Build instructions for FFTW library
#
#  AUTHOR
#    Roman Kubica
#
#  DATE
#    2015-10-08
#
#  LANGUAGE
#    human readable text
#
##############################################################################

==========================================================================
| Please replace the SDK_ROOT text by the path to your s32v234_sdk folder |
==========================================================================

This document explains how to build the FFTW library
       
FFTW library build instructions for Standalone
====================================

  1) Download FFTW library
  ----------------------------------
    - download FFTW 3.3.4 library version from 
        http://www.fftw.org/download.html
    - and put its content into 	
        SDK_ROOT/../fftw_src 
     
  4) Build FFTW library
  -------------------------
    - in SDK_ROOT/fftw/build-v234ce-gnu-sa
    - run:
        ./build.sh              

