



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := pid-dbg

LOCAL_DEFINES := \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/pid.c

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

LOCAL_MODULE := quad-dbg

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=3

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
	servo-dbg \
	ganymede-dbg \

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================


include $(CLEAR_VARS)

LOCAL_MODULE := quad-receiver-dbg

LOCAL_DEFINES := \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/quad_receiver.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
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
	mpu-dbg \
	i2c-mstr \
	spi \
	nRF24L01-dbg \
	uart \
	logging \
	failsafe \
	servo \
	io \
	ganymede \


LOCAL_TASKS := quad-dbg failsafe-tsk quad-receiver-dbg

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
