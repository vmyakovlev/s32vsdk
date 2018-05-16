##############################################################################
#
#  Freescale Semiconductor, Inc.
#  (c) Copyright 2016, Freescale Semiconductor, Inc.
#  ALL RIGHTS RESERVED.
#
##############################################################################
#
#  CONTENT
#    .ini file parser
#    https://github.com/touilleMan/iniparser4
#
#  AUTHOR
#    Sebastien Ashby
#
#  DATE
#    2016-04-20
#
#  LANGUAGE
#    make
#
##############################################################################

SDK_ROOT :=	../../..

ARM_LIB = libiniparser4.a

ARM_LIB_SRCS +=	   \
        dictionary.c \
        iniparser.c  \

 ARM_INCS += \
     -I../src

