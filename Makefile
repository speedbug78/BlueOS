CROSS_COMPILE=arm-none-eabi-
#CROSS_COMPILE =/opt/OSELAS.Toolchain-2011.11.0/arm-cortexm3-eabi/gcc-4.6.2-newlib-1.19.0-binutils-2.21.1a/bin/arm-cortexm3-eabi-
#CROSS_COMPILE =/opt/arm-2011.03/bin/arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
OC=$(CROSS_COMPILE)objcopy
OD=$(CROSS_COMPILE)objdump
SZ=$(CROSS_COMPILE)size

#SRC := $(wildcard src/*.c src/*.s)
#CFLAGS = -c -fno-common -Os -ffunction-sections -fdata-sections -mcpu=cortex-m3 -Wall -mthumb
CFLAGS= -c -fno-common    \
	-ffunction-sections   \
	-fdata-sections       \
	-Os                   \
	-mcpu=cortex-m3 -Wall \
	-mthumb               \
	-std=c99              \
	-DSTM32F103CB         \
	-DSTM32F10X_MD        \
	-DBLUE_PILL

OUTPUT_DIR = lib
OCFLAGS = -Obinary
ODFLAGS = -S
TARGET  = $(OUTPUT_DIR)/main

INCLUDE =  -I./inc   \
           -I./cmsis \
           -I./sys   \
           -I./drv   \
           -I./drv/MPU9250

SRCS =     ./drv/BlueOS_I2C.c     \
           ./drv/BlueOS_serial.c  \
           ./drv/BlueOS_systick.c \
           ./drv/MPU9250/BlueOS_MPU9255.c

OBJS=$(SRCS:.c=.o)

.PHONY : clean all

all: $(OBJS)

clean:
	-find . -name '*.o'   -exec rm {} \;
	-find . -name '*.elf' -exec rm {} \;
	-find . -name '*.lst' -exec rm {} \;
	-find . -name '*.out' -exec rm {} \;
	-find . -name '*.bin' -exec rm {} \;
	-find . -name '*.map' -exec rm {} \;

%.o: %.c
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $*.o

%.o: %.S
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $*.o
