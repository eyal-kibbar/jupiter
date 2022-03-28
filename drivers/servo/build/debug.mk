

# servo
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := servo-dbg

LOCAL_EXPORT      := $(LOCAL_PATH)/api

LOCAL_DEFINES     := \
	$(COMMON_DEFINES) \
	DEBUG \
	LOG_LEVEL=3  \
	SERVO_CYCLE_FREQ=50


LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	io-dbg \
	logging

LOCAL_CFLAGS := -Og

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

# servo
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := servo-dbg-250

LOCAL_EXPORT      := $(LOCAL_PATH)/api

LOCAL_DEFINES     := \
	$(COMMON_DEFINES) \
	DEBUG \
	LOG_LEVEL=3  \
	SERVO_CYCLE_FREQ=250


LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	io-dbg \
	logging

LOCAL_CFLAGS := -Og

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


# test
# ========================================
include $(CLEAR_VARS)


LOCAL_MODULE := servo-tst

LOCAL_DEFINES     := \
	$(COMMON_DEFINES) \
	STACK_SIZE=170 \
	LOG_LEVEL=3

LOCAL_SRC         := $(LOCAL_PATH)/tst/servo_test.c
LOCAL_STATIC_LIBS := \
	servo-dbg \
	ganymede-dbg \
	io-dbg \
	logging

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# servo test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-servo-tst

LOCAL_STATIC_LIBS := \
	servo-dbg \
	ganymede-dbg \
	io-dbg \
	logging \
	uart-dbg \

LOCAL_TASKS       := servo-tst


LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)
