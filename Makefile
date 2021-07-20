# Name of the executable
TARGET:=ringlight_firmware_0-0-4

# Device name and cpu freqeuncy in Hertz
DEVICE:= attiny1607
CLOCK:= 20000000

# Folder names for the source, build and documentation
SRC_DIR:=src
BUILD_DIR:=build
DOC_DIR:=doc

# Other variables that hold a list of all source files with .o and .s extensions added
# as well as the folder structure of the source folder and include defines
SRCS:=$(shell find $(SRC_DIR) -name *.cpp -or -name *.c -or -name *.s)
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)
ASM:=$(SRCS:%=$(BUILD_DIR)/%.s)
SRC_SCTR:=$(shell find $(SRC_DIR) -type d)
DEVICE_PACK:=./include/Atmel.ATtiny_DFP
INCLUDE_DIR:=include

# AVR-GCC, flags copy and dump stuff
CC:=avr-gcc
CFLAGS:= -Wall -Werror -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -Os -B $(DEVICE_PACK)/gcc/dev/$(DEVICE)/ -I $(DEVICE_PACK)/include/ -I $(INCLUDE_DIR)
OBJCOPY:=avr-objcopy
OBJDUMP:=avr-objdump
SIZE:=avr-size --format=avr --mcu=$(MCU)

# Builds the hex file
.PHONY: all
all: pre $(BUILD_DIR)/$(TARGET).hex

# Creates elf file then hex file
$(BUILD_DIR)/$(TARGET).hex: $(BUILD_DIR)/$(TARGET).elf
	@echo Creating hex file
	$(OBJCOPY) -j .text -j .data -O ihex $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

$(BUILD_DIR)/$(TARGET).elf: $(OBJS)
	@echo Linking...
	$(CC) $(CFLAGS) -o $@ $(OBJS)
	@echo 

###################################################################
###################################################################
#		Builds the object files for each source file			  #
###################################################################
###################################################################

$(BUILD_DIR)/%.s.o: %.s
	@echo Compiling $<
	$(CC) $(CFLAGS) -x assembler-with-cpp -o $@ $<
	@echo 

$(BUILD_DIR)/%.c.o: %.c
	@echo Building c file: $<
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo 

$(BUILD_DIR)/%.cpp.o: %.cpp
	@echo Building cpp file: $<
	$(CC) $(CFLAGS) -c -o $@ $<
	@echo 


###################################################################
###################################################################
#		Creates assembly files instead of objects				  #
###################################################################
###################################################################

$(BUILD_DIR)/%.s.s: %.s
	@echo Compiling $<
	$(CC) $(CFLAGS) -S -x assembler-with-cpp -o $@ $<
	@echo 

$(BUILD_DIR)/%.c.s: %.c
	@echo Building c file: $<
	$(CC) $(CFLAGS) -S -c -o $@ $<
	@echo 

$(BUILD_DIR)/%.cpp.s: %.cpp
	@echo Building cpp file: $<
	$(CC) $(CFLAGS) -S -c -o $@ $<
	@echo 

# Generates assembly files
.PHONY: asm
asm: pre $(ASM)

# Copies the src folder structure in the build directory
.PHONY: pre
pre:
	@echo $($(BUILD_DIR)/$(SRC_SCTR))
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(SRC_SCTR:%=$(BUILD_DIR)/%)

# Removes build and the documentation folder
.PHONY: clean
clean:
	@rm -r -f $(BUILD_DIR)
	@rm -r -f $(DOC_DIR)

# Genereates the documentation
.PHONY: doc
doc:
	@mkdir -p $(DOC_DIR)
	doxygen doxygen.conf

# Flashes the hex file onto the attiny1607
# Uses a custom configuration of avrdude
.PHONY: install
install:
	avrdude -p tiny1607 -c jtag2updi -U flash:w:$(BUILD_DIR)/$(TARGET).hex

# generate disassembly files for debugging
disasm: pre $(BUILD_DIR)/$(TARGET).elf
	$(OBJDUMP) -d $(BUILD_DIR)/$(TARGET).elf > $(BUILD_DIR)/$(TARGET).disasm.s

