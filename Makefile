#Output file name
OUTFILE  = BlueOS
#Linker script
LDSCRIPT = BlueOS_linker.ld
#Map File
MAPFILE = BlueOS_linker.map
#Directories to find included files
INCLUDE  =  -I./inc -I./config
#Additional link libraries
LDLIBS   =
#-lm
BLDIR = obj
SRCDIR = src
CFGDIR = config
OUTDIR = bin
TARGET = $(OUTDIR)/$(OUTFILE)

#Tool Names
CROSS_COMPILE = arm-none-eabi-

CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
OC=$(CROSS_COMPILE)objcopy
OD=$(CROSS_COMPILE)objdump
SZ=$(CROSS_COMPILE)size
FL=$(shell which st-flash)
SR=$(shell which minicom)

#Compiler Flags
CFLAGS = -c -fno-common   \
	-ffunction-sections   \
	-fdata-sections       \
	-Os                   \
	-mcpu=cortex-m3 -Wall \
	-mthumb               \
	-std=gnu99            \
	-DSTM32F103CB         \
	-DSTM32F10X_MD        \
	-DBLUE_PILL
	#-DMAPLE_MINI

#Linker Flags
LDFLAGS	= -O, --gc-sections \
	-T $(LDSCRIPT)			\
	-Map $(BLDIR)/$(MAPFILE)  \
	-no-startup -nostdlib

#Object Copy Flags
OCFLAGS = -Obinary

#Object Dump Flags
ODFLAGS = -S

#Generate list of source files
SRCS := $(notdir $(wildcard src/*.c src/*.S src/*.s config/*.c))
#Generate list of object files
OBJS = $(addsuffix .o, $(basename $(SRCS)))

.PHONY : clean all

all: $(TARGET).bin $(TARGET).hex $(TARGET).list
	$(SZ) $(TARGET).elf

clean:
	-find . -name '*.o'   -exec rm {} \;
	-find . -name '*.elf' -exec rm {} \;
	-find . -name '*.lst' -exec rm {} \;
	-find . -name '*.out' -exec rm {} \;
	-find . -name '*.bin' -exec rm {} \;
	-find . -name '*.hex' -exec rm {} \;
	-find . -name '*.map' -exec rm {} \;
	-find . -name '$(BLDIR)\*.o'   -exec rm {} \;
	-find . -name '$(BLDIR)\*.elf' -exec rm {} \;
	-find . -name '$(BLDIR)\*.lst' -exec rm {} \;
	-find . -name '$(BLDIR)\*.out' -exec rm {} \;
	-find . -name '$(BLDIR)\*.bin' -exec rm {} \;
	-find . -name '$(BLDIR)\*.hex' -exec rm {} \;
	-find . -name '$(BLDIR)\*.map' -exec rm {} \;
	-find . -name '$(SRCDIR)\*.o'   -exec rm {} \;
	-find . -name '$(SRCDIR)\*.elf' -exec rm {} \;
	-find . -name '$(SRCDIR)\*.lst' -exec rm {} \;
	-find . -name '$(SRCDIR)\*.out' -exec rm {} \;
	-find . -name '$(SRCDIR)\*.bin' -exec rm {} \;
	-find . -name '$(SRCDIR)\*.hex' -exec rm {} \;
	-find . -name '$(SRCDIR)\*.map' -exec rm {} \;
	-find . -name '$(OUTDIR)\*.o'   -exec rm {} \;
	-find . -name '$(OUTDIR)\*.elf' -exec rm {} \;
	-find . -name '$(OUTDIR)\*.lst' -exec rm {} \;
	-find . -name '$(OUTDIR)\*.out' -exec rm {} \;
	-find . -name '$(OUTDIR)\*.bin' -exec rm {} \;
	-find . -name '$(OUTDIR)\*.hex' -exec rm {} \;
	-find . -name '$(OUTDIR)\*.map' -exec rm {} \;

$(TARGET).list: $(TARGET).elf
	@echo "  Dump $(TARGET).elf to $(TARGET).list"
	@$(OD) $(ODFLAGS) $< > $(TARGET).lst

$(TARGET).bin: $(TARGET).elf
	@echo "  Copy $(TARGET).elf to $(TARGET).bin"
	@$(OC) $(OCFLAGS) $(TARGET).elf $(TARGET).bin

$(TARGET).hex: $(TARGET).elf
	@echo "  Copy $(TARGET).elf to $(TARGET).hex"
	@$(OC) $(OCFLAGS) $(TARGET).elf $(TARGET).hex

$(TARGET).elf: $(addprefix $(BLDIR)/, $(OBJS))
	@echo "  LD $<"
	@$(LD) $(LDFLAGS) -o $(TARGET).elf $(addprefix $(BLDIR)/, $(OBJS)) $(LDLIBS)

# Make object files (.o) out of various sources

$(BLDIR)/%.o: $(SRCDIR)/%.c
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@

$(BLDIR)/%.o: $(CFGDIR)/%.c
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@

$(BLDIR)/%.o: $(SRCDIR)/%.S
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@

$(BLDIR)/%.o: $(SRCDIR)/%.s
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@

# $(TARGET).bin
flash: clean all
	@printf "  FLASH  $<\n"
	$(FL) --reset write $(TARGET).bin 0x08000000

serial:
	@printf "  SERIAL $<\n"
	$(SR) -D /dev/ttyUSB0 -b 115200

#Dump some of Flash into file
#st-flash --reset read Flash_Dump.hex 0x08000000 0x08000800

# Dissasembly of .bin file (or Flash dump?)
# arm-none-eabi-objdump -D -bbinary -marm bin/BlueOS.bin -Mforce-thumb > bin/BlueOS.S
