

LOCAL_PATH := $(call my-dir)

SRC_avr := $(LOCAL_PATH)/src/arch/avr/gmd_platform.c
INC_avr := $(LOCAL_PATH)/inc/arch/avr/

SRC_linux := $(LOCAL_PATH)/src/arch/linux/gmd_platform.c
INC_linux := $(LOCAL_PATH)/inc/arch/linux/


COMMON_SRC := \
	$(LOCAL_PATH)/src/common/gmd_kern.c \
	$(LOCAL_PATH)/src/common/gmd_sched.c

COMMON_INC := $(LOCAL_PATH)/inc
COMMON_API := $(LOCAL_PATH)/api

COMMON_DEFINES := GANYMEDE

# uart
# ----------------------------------------
UART_SRC := $(LOCAL_PATH)/src/common/drv/gmd_uart.c
UART_INC := $(LOCAL_PATH)/inc

UART_SRC_avr := $(LOCAL_PATH)/src/arch/avr/drv/gmd_uart_platform.c
UART_INC_avr := $(LOCAL_PATH)/inc/arch/avr/drv

UART_DEFINES := DRV_UART_BAUDRATE=9600


# i2c
# ----------------------------------------
I2C_SRC := $(LOCAL_PATH)/src/common/drv/gmd_i2c.c
I2C_INC := $(LOCAL_PATH)/inc

I2C_SRC_avr := $(LOCAL_PATH)/src/arch/avr/drv/gmd_i2c_platform.c
I2C_INC_avr := $(LOCAL_PATH)/inc/arch/avr/drv

I2C_DEFINES :=


include $(LOCAL_PATH)/build/debug.mk
include $(LOCAL_PATH)/build/release.mk
