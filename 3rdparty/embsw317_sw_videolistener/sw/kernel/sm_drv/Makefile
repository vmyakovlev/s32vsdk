#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#   File: Makefile
#   Copyright (c) 2016 NXP Semiconductors
#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

obj-m += sm_drv.o
sm_drv-y += main.o

EXTRA_CFLAGS += -I$(PWD)/include

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
