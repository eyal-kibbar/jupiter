

# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := quad-dbg

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=3 \
	DEBUG

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/quad.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	pid-dbg \
	mpu-dbg \
	i2c-mstr-dbg \
	uart-dbg \
	logging \
	failsafe \
	servo-dbg-250 \
	ganymede-dbg \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================


include $(CLEAR_VARS)

LOCAL_MODULE := quad-receiver-dbg

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/quad_receiver.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	pid-dbg \
	spi-dbg \
	uart-dbg \
	logging \
	failsafe-dbg \
	nRF24L01-dbg \
	ganymede-dbg \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-quad-dbg

LOCAL_DEFINES :=

LOCAL_STATIC_LIBS := \
	pid-dbg \
	mpu \
	i2c-mstr \
	spi \
	nRF24L01 \
	uart \
	logging \
	failsafe \
	servo-250 \
	io \
	ganymede \

LOCAL_TASKS := quad-dbg quad-receiver-dbg failsafe-tsk

# for printing floats
LOCAL_LDFLAGS := -Wl,-u,vfprintf -lprintf_flt

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
