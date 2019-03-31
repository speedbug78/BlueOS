#Output file name
OUTFILE  = main
#Linker script
LDSCRIPT = BlueOS_linker.ld
#Directories to find included files
INCLUDE  =  -I./inc
#Additional link libraries
LDLIBS   =
#-lm
BLDIR = obj
SRCDIR = src
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
	-T $(LDSCRIPT)          \
	-no-startup -nostdlib

#Object Copy Flags
OCFLAGS = -Obinary

#Object Dump Flags
ODFLAGS = -S

#Generate list of source files
SRCS := $(notdir $(wildcard src/*.c src/*.S src/*.s))
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

$(BLDIR)/%.o: $(SRCDIR)/%.c
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@

$(BLDIR)/%.o: $(SRCDIR)/%.S
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@

$(BLDIR)/%.o: $(SRCDIR)/%.s
	@echo "  CC $<"
	@$(CC) $(INCLUDE) $(CFLAGS)  $< -o $@
