CC=arm-eabi-gcc
AS=arm-eabi-as
LD=arm-eabi-ld
SDK_ROOT=../../../../s32v234_sdk


CFLAGS=                 \
    -Iinclude           \
    -I$(SDK_ROOT)/platform/s32_v234   \
    -DCORE_M4_ENABLE    \
    -DPMC_CHECK_BYPASS  \
    -DSTCU_CHECK_BYPASS \
    -g                  \
    -mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
    -mcpu=cortex-m4     \
    -march=armv7e-m \
    -mthumb             \
    -std=c99   
 
AFLAGS=                \
    -g                 \
    -mcpu=cortex-m4    \
    -march=armv7e-m \
    -mfpu=fpv4-sp-d16 -mfloat-abi=softfp \
    -mthumb
	
LDFLAGS= -T linker/linker.ld -Map=linker/binary.map -L. 



_OBJ =                       \
    startup/crt0_m4_0.o      \
    startup/linflex_serial.o \
    startup/m4_functions.o   \
    startup/pll.o            \
    startup/printf.o  \
    startup/SetupLPDDR2.o \
    startup/qspi_driver.o \
    startup/i2c.o   \
    startup/MIPI.o    \
    startup/HDMI.o      \
    startup/SetupDDR3.o   \
    main.o


OBJ = $(patsubst %,./%,$(_OBJ))
OBJ_28899 = $(addsuffix 28899, $(OBJ))
OBJ_29288 = $(addsuffix 29288, $(OBJ))

all: binary_28899 binary_29288

%.o28899: %.c
	$(CC) -c -o $@ $< $(CFLAGS) -DUSE_EVB28899

%.o28899: %.s
	$(AS) -c -o $@ $< $(AFLAGS) 
	
%.o29288: %.c
	$(CC) -c -o $@ $< $(CFLAGS) -DUSE_EVB29288

%.o29288: %.s
	$(AS) -c -o $@ $< $(AFLAGS) 

binary_28899: $(OBJ_28899)
	$(LD) -o binary28899.elf $^ $(LDFLAGS)

binary_29288: $(OBJ_29288)
	$(LD) -o binary29288.elf $^ $(LDFLAGS)
	



.PHONY: clean

clean:
	rm -f $(ODIR)/*.o28899 $(ODIR)/*.o29288 *~ core $(INCDIR)/*~
	rm -f startup/*.o28899 startup/*.o29288 *~ core $(INCDIR)/*~ 
	rm -f *.o28899 *.o29288
	rm -f linker/binary.map
	rm -f *.elf
