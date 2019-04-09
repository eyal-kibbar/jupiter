LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := barometer-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/hwio.c \
	$(LOCAL_PATH)/src/common/bmp180.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede-dbg

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
	ganymede-dbg

LOCAL_SRC := $(LOCAL_PATH)/tst/barometer_test.c

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


