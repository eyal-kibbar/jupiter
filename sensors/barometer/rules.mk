LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := barometer-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3 \
	BMP180_OSS=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/hwio.c \
	$(LOCAL_PATH)/src/common/bmp180.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	i2c-mstr-dbg \
	logging

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := barometer-tst

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	barometer-dbg \
	ganymede-dbg \
	i2c-mstr \
	logging

LOCAL_SRC := $(LOCAL_PATH)/tst/barometer_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)



# barometer test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-barometer-tst

LOCAL_STATIC_LIBS := \
	barometer-dbg \
	ganymede \
	i2c-mstr \
	uart \
	logging

LOCAL_TASKS       := barometer-tst

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)
