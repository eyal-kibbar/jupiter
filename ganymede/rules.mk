

LOCAL_PATH := $(call my-dir)

SRC_avr := $(LOCAL_PATH)/src/arch/avr/gmd_platform.c
INC_avr := $(LOCAL_PATH)/inc/arch/avr/

COMMON_SRC := \
	$(LOCAL_PATH)/src/common/gmd_kern.c \
	$(LOCAL_PATH)/src/common/gmd_sched.c

COMMON_INC := $(LOCAL_PATH)/inc
COMMON_API := $(LOCAL_PATH)/api

# uart
# ----------------------------------------
UART_SRC := $(LOCAL_PATH)/src/common/drv/gmd_uart.c
UART_INC := $(LOCAL_PATH)/inc

UART_SRC_avr := $(LOCAL_PATH)/src/arch/avr/drv/gmd_uart_platform.c
UART_INC_avr := $(LOCAL_PATH)/inc/arch/avr/drv


# i2c
# ----------------------------------------
I2C_SRC := $(LOCAL_PATH)/src/common/drv/gmd_i2c.c
I2C_INC := $(LOCAL_PATH)/inc

I2C_SRC_avr := $(LOCAL_PATH)/src/arch/avr/drv/gmd_i2c_platform.c
I2C_INC_avr := $(LOCAL_PATH)/inc/arch/avr/drv

# debug flavor
# ========================================
include $(CLEAR_VARS)

LOCAL_TARGET := atmega168
LOCAL_MODULE := ganymede-dbg

ARCH := $(ARCH_$(LOCAL_TARGET))

# add common
LOCAL_SRC += $(COMMON_SRC)
LOCAL_SRC += $(SRC_$(ARCH))

LOCAL_INC += $(COMMON_INC)
LOCAL_INC += $(INC_$(ARCH))

LOCAL_EXPORT := $(COMMON_API)
LOCAL_EXPORT += $(LOCAL_PATH)/api/arch/$(ARCH)
    
LOCAL_DEFINES += GANYMEDE
LOCAL_DEFINES += DEBUG
LOCAL_DEFINES += LOG_LEVEL=1

LOCAL_CFLAGS := -O0


# add uart
LOCAL_SRC += $(UART_SRC)
LOCAL_SRC += $(UART_SRC_$(ARCH))

LOCAL_INC += $(UART_INC)
LOCAL_INC += $(UART_INC_$(ARCH))

LOCAL_DEFINES += DRV_UART_BAUDRATE=9600

# add i2c
LOCAL_SRC += $(I2C_SRC)
LOCAL_SRC += $(I2C_SRC_$(ARCH))

LOCAL_INC += $(I2C_INC)
LOCAL_INC += $(I2C_INC_$(ARCH))

#LOCAL_DEFINES +=


include $(BUILD_STATIC_LIB)






include $(CLEAR_VARS)

LOCAL_TARGET := atmega328p
LOCAL_MODULE := ganymede-dbg

ARCH := $(ARCH_$(LOCAL_TARGET))

# add common
LOCAL_SRC += $(COMMON_SRC)
LOCAL_SRC += $(SRC_$(ARCH))

LOCAL_INC += $(COMMON_INC)
LOCAL_INC += $(INC_$(ARCH))

LOCAL_EXPORT := $(COMMON_API)
LOCAL_EXPORT += $(LOCAL_PATH)/api/arch/$(ARCH)
    
LOCAL_DEFINES += GANYMEDE
LOCAL_DEFINES += DEBUG
LOCAL_DEFINES += LOG_LEVEL=1

LOCAL_CFLAGS := -O0


# add uart
LOCAL_SRC += $(UART_SRC)
LOCAL_SRC += $(UART_SRC_$(ARCH))

LOCAL_INC += $(UART_INC)
LOCAL_INC += $(UART_INC_$(ARCH))

LOCAL_DEFINES += DRV_UART_BAUDRATE=9600

# add i2c
LOCAL_SRC += $(I2C_SRC)
LOCAL_SRC += $(I2C_SRC_$(ARCH))

LOCAL_INC += $(I2C_INC)
LOCAL_INC += $(I2C_INC_$(ARCH))

#LOCAL_DEFINES +=


include $(BUILD_STATIC_LIB)



