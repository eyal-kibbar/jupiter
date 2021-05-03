LOCAL_PATH := $(call my-dir)


# ========================================

include $(CLEAR_VARS)

LOCAL_MODULE := mpu-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3 \
	MPU_GYRO_FULL_SCALE_RANGE=250 \
	MPU_ACCEL_FULL_SCALE_RANGE=2 \

LOCAL_SRC += $(LOCAL_PATH)/src/common/mpu6050.c
LOCAL_INC += $(LOCAL_PATH)/inc
LOCAL_EXPORT += $(LOCAL_PATH)/api

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

LOCAL_MODULE := mpu

LOCAL_DEFINES := \
	LOG_LEVEL=0 \
	MPU_GYRO_FULL_SCALE_RANGE=2000 \
	MPU_ACCEL_FULL_SCALE_RANGE=2

LOCAL_SRC += $(LOCAL_PATH)/src/common/mpu6050.c
LOCAL_INC += $(LOCAL_PATH)/inc
LOCAL_EXPORT += $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede \
	i2c-mstr \
	logging

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := mpu-tst
LOCAL_DEFINES += STACK_SIZE=512
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
	i2c-mstr-dbg \
	logging \
	uart-dbg

LOCAL_TASKS       := mpu-tst

# for printing floats
LOCAL_LDFLAGS := -Wl,-u,vfprintf -lprintf_flt

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)
