

# atmega328p
# ========================================
include $(CLEAR_VARS)

LOCAL_TARGET := atmega328p
LOCAL_MODULE := ganymede

ARCH := $(ARCH_$(LOCAL_TARGET))

# add common
LOCAL_SRC     := $(COMMON_SRC) $(SRC_$(ARCH))
LOCAL_INC     := $(COMMON_INC) $(INC_$(ARCH))
LOCAL_EXPORT  := $(COMMON_API) $(LOCAL_PATH)/api/arch/$(ARCH)
LOCAL_DEFINES := \
	$(COMMON_DEFINES) \
	RELEASE \
	LOG_LEVEL=0

LOCAL_CFLAGS := -O3

# add i2c
LOCAL_SRC     += $(I2C_SRC) $(I2C_SRC_$(ARCH))
LOCAL_INC     += $(I2C_INC) $(I2C_INC_$(ARCH))
LOCAL_DEFINES += $(I2C_DEFINES)


include $(BUILD_STATIC_LIB)

# atmega168
# ========================================
include $(CLEAR_VARS)

LOCAL_TARGET := atmega168
LOCAL_MODULE := ganymede

ARCH := $(ARCH_$(LOCAL_TARGET))

# add common
LOCAL_SRC     := $(COMMON_SRC) $(SRC_$(ARCH))
LOCAL_INC     := $(COMMON_INC) $(INC_$(ARCH))
LOCAL_EXPORT  := $(COMMON_API) $(LOCAL_PATH)/api/arch/$(ARCH)
LOCAL_DEFINES := \
	$(COMMON_DEFINES) \
	RELEASE \
	LOG_LEVEL=0

LOCAL_CFLAGS := -O3

# add i2c
LOCAL_SRC     += $(I2C_SRC) $(I2C_SRC_$(ARCH))
LOCAL_INC     += $(I2C_INC) $(I2C_INC_$(ARCH))
LOCAL_DEFINES += $(I2C_DEFINES)


include $(BUILD_STATIC_LIB)
