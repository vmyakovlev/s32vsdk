##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2009, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    Mandatory GDI functionality implementation and demo
#
#  AUTHOR
#    Tomas Babinec
#
#  DATE
#    2013-08-08
#
#  LANGUAGE
#    human readable text
#
##############################################################################

Building gdi library
===========================
1.) Switch to /path/to/gdi/gdi_shr/Debug folder.

2.) Run make.

Building of test_gdi_shr1 demo
==============================
1.) Switch to /path/to/gdi/test_gdi_shr1/Debug folder.

2.) Run make.

Running test_gdi_shr1 demo
==========================

Do the following as root:
1.) LD_LIBRARY_PATH has to be set to gdi/gdi_shr/Debug folder, where the 
libgdi_shr.so binary is located by default.

export LD_LIBRARY_PATH=/path/to/gdi/gdi_shr/Debug:$LD_LIBRARY_PATH

2.) Open one of the text consoles (Ctrl+Alt+F(n)).

3.) Switch to gdi/test_gdi_shr1/Debug folder.

4.) Run ./test_gdi_shr1

Tested on Ubuntu13 in VirtualBox environment.
Console resolution 640x480 pix, 32 bits per pixel.
