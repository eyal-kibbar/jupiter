LOCAL_PATH := $(call my-dir)


# ========================================

include $(CLEAR_VARS)

LOCAL_MODULE := mpu-dbg

LOCAL_DEFINES += LOG_LEVEL=3

LOCAL_SRC += $(LOCAL_PATH)/src/common/mpu6050.c
LOCAL_INC += $(LOCAL_PATH)/inc
LOCAL_EXPORT += $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	logging

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := mpu-tst
LOCAL_DEFINES += STACK_SIZE=256
LOCAL_DEFINES += LOG_LEVEL=3
LOCAL_STATIC_LIBS := \
	mpu-dbg \
	ganymede-dbg \
	logging

LOCAL_SRC += $(LOCAL_PATH)/tst/mpu_test.c
LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# mpu test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-mpu-tst

LOCAL_STATIC_LIBS := \
	mpu-dbg \
	ganymede-dbg \
	logging

LOCAL_TASKS       := mpu-tst


LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)
