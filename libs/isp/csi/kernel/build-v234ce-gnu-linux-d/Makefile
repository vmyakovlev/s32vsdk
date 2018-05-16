#./Kbuild

check_defined = \
    $(foreach 1,$1,$(__check_defined))
__check_defined = \
    $(if $(value $1),, \
      $(error Please define $1 pointing to kernel directory $(if $(value 2), ($(strip $2) ))))

INCLUDE_DIRS := \
	$(LINUX_S32V234_DIR)/../linux_hdr/                          \
	$(CURDIR)/../include/                                          \
	$(CURDIR)/../../../../../include/                              \
	$(CURDIR)/../../../../../platform/s32_v234/                    \
	$(CURDIR)/../../../../../isp/inc/                              \
	$(CURDIR)/../../../../../libs/isp/sequencer/kernel/include     \
	
#	/home/tomas/tools/gcc-linaro-4.9-2014.11-x86_64_aarch64-linux-gnu/libc/usr/include \

CFLGS :=	\
	-DDEBUG           \
	-D__KERNEL_MODULE \
	-D__ARMV8         \
	-nostdinc          \
	-g                 \
	-Og

	
CFLGS += $(foreach includedir, $(INCLUDE_DIRS),-I$(includedir))	

#KXTRA_SYMS := 	/home/tomas/code/vsdk/s32v234_sdk/libs/isp/sram/kernel/build-zc702-gnu-linux-d/Module.symvers

# module string prefix
MODULE := csi

# defines driver module object & output file
obj-m := $(MODULE).o 

# what object (source) files is the final output composed of
$(MODULE)-objs := 	\
		../src/$(MODULE)_core.o         \
		../src/$(MODULE)_ioctl.o        \
		../src/$(MODULE)_func.o         \
		../../../../utils/log/src/vdb_log.o
		
#		../src/sony_func.o              \		
		
KDIR ?= $(LINUX_S32V234_DIR)
CROSS_COMPILE ?= aarch64-linux-gnu-

### Targets ###

.PHONY: clean cleansub all allsub

all allsub: module

module: check-env
	$(MAKE) KCPPFLAGS="$(CFLGS)" ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR)  M=$(CURDIR) modules

check-env:
	$(call check_defined, LINUX_S32V234_DIR)
	
clean cleansub:
	rm -rf *~ core *.o .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order ../src/.*.cmd $($(MODULE)-objs)