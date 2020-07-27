LOCAL_PATH := $(call my-dir)

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := invpend-dbg

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/invpend.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	mpu-dbg \
	ganymede-dbg \
	i2c-dbg \
	logging

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)




# invpend debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-invpend-dbg

LOCAL_STATIC_LIBS := \
	servo-dbg \
	mpu-dbg \
	ganymede-dbg \
	i2c-dbg \
	logging

LOCAL_TASKS       := \
	invpend-dbg \


LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)
