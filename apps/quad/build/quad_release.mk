



# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := pid

LOCAL_DEFINES := \
	LOG_LEVEL=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/pid.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)




# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := quad

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=0 \
	RELEASE

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/quad.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	pid \
	mpu \
	uart \
	i2c-mstr \
	logging \
	failsafe \
	servo-250 \
	ganymede \

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================


include $(CLEAR_VARS)

LOCAL_MODULE := quad-receiver

LOCAL_DEFINES := \
	STACK_SIZE=512 \
	LOG_LEVEL=0

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_SRC := \
	$(LOCAL_PATH)/src/common/quad_receiver.c

LOCAL_INC := \
	$(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	spi \
	uart \
	logging \
	failsafe \
	nRF24L01 \
	ganymede \

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := jupiter-quad

LOCAL_DEFINES :=

LOCAL_STATIC_LIBS := \
	pid \
	mpu \
	i2c-mstr \
	spi \
	uart \
	nRF24L01 \
	logging \
	failsafe \
	servo-250 \
	io \
	ganymede \


LOCAL_TASKS := quad failsafe-tsk quad-receiver

LOCAL_CFLAGS := -O3

LOCAL_TARGET := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET := atmega328p
include $(BUILD_EXEC)
