#*-*- Makefile -*-************************************************************
#
#              MOTOROLA Confidential and Proprietary
#
# $Source: /home/cscuscvs/CVS/picasso2/tools/sr2rom/Makefile,v $
#
# content:     Makefile for S-Record to ROM file converter
# language:    Makefile
# author:      Dirk Moeller
# created:     01.12.2000
# version:     $Revision: 1.3 $
# modified:    $Date: 2002/08/20 13:27:40 $
# by:          $Author: dirkm $
#
#*****************************************************************************

CC 	= gcc
CFLAGS 	= -Wall -O -ggdb

OBJECTS = sr2rom.o

sr2rom 	: $(OBJECTS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) 

clean	:
	@/bin/rm -f sr2rom $(OBJECTS)

# end of file
