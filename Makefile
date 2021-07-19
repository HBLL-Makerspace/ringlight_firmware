# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE:= attiny1607
CLOCK:= 20000000
CC:= avr-gcc

SRC_DIR:=src
BUILD_DIR:=build
TARGET:=ringlight_firmware_0-0-4
SRCS:=$(shell find $(SRC_DIR) -name *.cpp -or -name *.c -or -name *.s)
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)
SRC_SCTR:=$(shell find $(SRC_DIR) -type d)
DEVICE_PACK:=./include/Atmel.ATtiny_DFP
INCLUDE_DIR:=include
DOC_DIR:=doc

CC:=avr-gcc
CFLAGS:= -Wall -Werror -mmcu=$(DEVICE) -DF_CPU=$(CLOCK) -Os -B $(DEVICE_PACK)/gcc/dev/$(DEVICE)/ -I $(DEVICE_PACK)/include/ -I $(INCLUDE_DIR)

.PHONY: all
all: pre $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	@echo Creating hex file

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

$(TARGET).elf: $(OBJS)
	@echo Linking...



.PHONY: pre
pre:
	@echo $($(BUILD_DIR)/$(SRC_SCTR))
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(SRC_SCTR:%=$(BUILD_DIR)/%)

.PHONY: clean
clean:
	@rm -r -f $(BUILD_DIR)
	@rm -r -f $(DOC_DIR)

.PHONY: doc
doc:
	@mkdir -p $(DOC_DIR)
	doxygen doxygen.conf

# .PHONY install
# install: