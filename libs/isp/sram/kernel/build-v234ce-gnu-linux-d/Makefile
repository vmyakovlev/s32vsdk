#./Kbuild

check_defined = \
    $(foreach 1,$1,$(__check_defined))
__check_defined = \
    $(if $(value $1),, \
      $(error Please define $1 pointing to kernel directory $(if $(value 2), ($(strip $2) ))))

INCLUDE_DIRS := \
	$(LINUX_S32V234_DIR)/../linux_hdr/                  \
	$(CURDIR)/../include/                                  \
	$(CURDIR)/../../../../../include/                      \
	$(CURDIR)/../../../../../platform/s32_v234/            \

CFLGS =			\
	-g                      \
        -Og                     \
	-nostdinc		\
	-DDEBUG 		\
	-D__KERNEL_MODULE 	\
	-D__ARMV8 		
	
CFLGS += $(foreach includedir, $(INCLUDE_DIRS),-I$(includedir))	
			
# module string prefix
MODULE := sram

# defines driver module object & output file
obj-m := $(MODULE).o 

# what object (source) files is the final output composed of
$(MODULE)-objs := 	\
		../src/mspace_allocator.o	\
		../src/$(MODULE)_core.o 	\
		../src/$(MODULE)_ioctl.o	\
		../src/$(MODULE)_func.o		\
		../../../../utils/log/src/vdb_log.o
		
KERNELDIR ?= $(LINUX_S32V234_DIR)
CROSS_COMPILE ?= aarch64-linux-gnu-

### Targets ###

.PHONY: clean cleansub all allsub

all allsub: module

module: check-env
	$(MAKE) KCPPFLAGS="$(CFLGS)" ARCH=arm64 CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERNELDIR) M=$(CURDIR) modules

check-env:
	$(call check_defined, LINUX_S32V234_DIR)
	
clean cleansub:
	rm -rf *~ core *.o .depend .*.cmd *.ko *.mod.c .tmp_versions *.symvers *.order ../src/.*.cmd $($(MODULE)-objs)
