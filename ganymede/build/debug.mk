

# atmega328p
# ========================================
include $(CLEAR_VARS)

LOCAL_TARGET := atmega328p
LOCAL_MODULE := ganymede-dbg

ARCH := $(ARCH_$(LOCAL_TARGET))

# add common
LOCAL_SRC     := $(COMMON_SRC) $(SRC_$(ARCH))
LOCAL_INC     := $(COMMON_INC) $(INC_$(ARCH))
LOCAL_EXPORT  := $(COMMON_API) $(LOCAL_PATH)/api/arch/$(ARCH)
LOCAL_DEFINES := \
	$(COMMON_DEFINES) \
	DEBUG \
	LOG_LEVEL=1

LOCAL_CFLAGS := -Og

# add uart
LOCAL_SRC += $(UART_SRC) $(UART_SRC_$(ARCH))
LOCAL_INC += $(UART_INC) $(UART_INC_$(ARCH))
LOCAL_DEFINES += $(UART_DEFINES)

# add i2c
LOCAL_SRC     += $(I2C_SRC) $(I2C_SRC_$(ARCH))
LOCAL_INC     += $(I2C_INC) $(I2C_INC_$(ARCH))
LOCAL_DEFINES += $(I2C_DEFINES)


include $(BUILD_STATIC_LIB)

# atmega168
# ========================================
include $(CLEAR_VARS)

LOCAL_TARGET := atmega168
LOCAL_MODULE := ganymede-dbg

ARCH := $(ARCH_$(LOCAL_TARGET))

# add common
LOCAL_SRC     := $(COMMON_SRC) $(SRC_$(ARCH))
LOCAL_INC     := $(COMMON_INC) $(INC_$(ARCH))
LOCAL_EXPORT  := $(COMMON_API) $(LOCAL_PATH)/api/arch/$(ARCH)
LOCAL_DEFINES := \
	$(COMMON_DEFINES) \
	DEBUG \
	LOG_LEVEL=1

LOCAL_CFLAGS := -Og

# add uart
LOCAL_SRC += $(UART_SRC) $(UART_SRC_$(ARCH))
LOCAL_INC += $(UART_INC) $(UART_INC_$(ARCH))
LOCAL_DEFINES += $(UART_DEFINES)

# add i2c
LOCAL_SRC     += $(I2C_SRC) $(I2C_SRC_$(ARCH))
LOCAL_INC     += $(I2C_INC) $(I2C_INC_$(ARCH))
LOCAL_DEFINES += $(I2C_DEFINES)


include $(BUILD_STATIC_LIB)


# test
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := gmd-tst1
LOCAL_DEFINES += STACK_SIZE=256
LOCAL_DEFINES += LOG_LEVEL=3
LOCAL_STATIC_LIBS := \
	ganymede-dbg

LOCAL_SRC += $(LOCAL_PATH)/tst/test_tsk1.c
LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

# test
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := gmd-tst2
LOCAL_DEFINES += STACK_SIZE=256
LOCAL_DEFINES += LOG_LEVEL=3
LOCAL_STATIC_LIBS := \
	ganymede-dbg

LOCAL_SRC += $(LOCAL_PATH)/tst/test_tsk2.c
LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)

