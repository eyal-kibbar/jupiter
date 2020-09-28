

# libs:
#   io-dbg
#   logging
#









LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := io-dbg
LOCAL_DEFINES :=
LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/arch/avr/io_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr


LOCAL_STATIC_LIBS :=

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega2560
include $(BUILD_STATIC_LIB)



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := logging
LOCAL_DEFINES :=
LOCAL_EXPORT := \
	$(LOCAL_PATH)/api \
	$(LOCAL_PATH)/api/arch/avr \

LOCAL_SRC := \
	$(LOCAL_PATH)/src/arch/avr/io_logging_platform.c \

LOCAL_INC := \
	$(LOCAL_PATH)/inc \
	$(LOCAL_PATH)/inc/arch/avr


LOCAL_STATIC_LIBS :=

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega2560
include $(BUILD_STATIC_LIB)


include $(LOCAL_PATH)/build/uart.mk
include $(LOCAL_PATH)/build/i2c.mk
include $(LOCAL_PATH)/build/spi.mk
