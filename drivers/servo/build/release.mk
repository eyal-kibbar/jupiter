# servo
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := servo

LOCAL_EXPORT      := $(LOCAL_PATH)/api

LOCAL_DEFINES     := \
	$(COMMON_DEFINES) \
	RELEASE \
	LOG_LEVEL=0  \


LOCAL_STATIC_LIBS := \
	ganymede \
	logging

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
