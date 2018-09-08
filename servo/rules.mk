LOCAL_PATH := $(call my-dir)


COMMON_SRC := $(LOCAL_PATH)/src/common/servo.c
COMMON_INC := $(LOCAL_PATH)/inc

# debug flavor
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := servo-dbg

LOCAL_EXPORT      := $(LOCAL_PATH)/api

LOCAL_DEFINES     := \
	SERVO_MAX_PIN=16 \
	LOG_LEVEL=3  \


LOCAL_STATIC_LIBS += ganymede-dbg

LOCAL_CFLAGS = -O3

LOCAL_TARGET := atmega168
ARCH         := $(ARCH_$(LOCAL_TARGET))
LOCAL_INC    := $(COMMON_INC) $(LOCAL_PATH)/inc/arch/$(ARCH)
LOCAL_SRC    := $(COMMON_SRC) $(LOCAL_PATH)/src/arch/$(ARCH)/servo_platform.c
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
ARCH         := $(ARCH_$(LOCAL_TARGET))
LOCAL_INC    := $(COMMON_INC) $(LOCAL_PATH)/inc/arch/$(ARCH)
LOCAL_SRC    := $(COMMON_SRC) $(LOCAL_PATH)/src/arch/$(ARCH)/servo_platform.c
include $(BUILD_STATIC_LIB)


# test task
# ========================================
include $(CLEAR_VARS)


LOCAL_MODULE := servo-tst

LOCAL_DEFINES     += STACK_SIZE=170
LOCAL_DEFINES     += LOG_LEVEL=3

LOCAL_SRC         += $(LOCAL_PATH)/tst/servo_test.c
LOCAL_STATIC_LIBS += servo-dbg ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


