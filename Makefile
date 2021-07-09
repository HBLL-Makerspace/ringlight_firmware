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
SRCS:=$(shell find $(SRC_DIR) -name *.cpp -or -name *.c)
OBJS:=$(SRCS:%=$(BUILD_DIR)/%.o)

# .PHONY: all
all: $(TARGET).elf

$(TARGET).elf: #$(TARGET).hex
	@echo $(BUILD_DIR)/$(SRCS)
    

# $(TARGET).hex: $(OBJECTS).cpp.o
#         @echo $(SOURCES)


# $(TARGET).hex: $(OBJECTS).c.o
#         @echo $(SOURCES)


# .PHONY install
# install:

# .PHONY doc
# doc:

# .PHONY: clean
# clean:
#         rm -f $(TARGET) $(OBJECTS)