

# atmega168
TARGET := atmega168
ARCH_$(TARGET) := avr

CFLAGS_$(TARGET)  := $(CFLAGS_$(ARCH_$(TARGET)))
CFLAGS_$(TARGET)  += -DF_CPU=16000000UL -mmcu=$(TARGET)
CC_$(TARGET)      := $(CC_$(ARCH_$(TARGET)))
AR_$(TARGET)      := $(AR_$(ARCH_$(TARGET)))
LDFLAGS_$(TARGET) := -T $(BUILD_DIR)/linker/avr5.ld
OBJCOPY_$(TARGET) := $(OBJCOPY_$(ARCH_$(TARGET)))
AVRDUDE_PORT      ?= /dev/ttyUSB0
AVRDUDE_BAUD      ?= 115200

SUPPORTED_TARGETS += $(TARGET)
