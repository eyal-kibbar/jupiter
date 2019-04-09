
# server
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := srv

LOCAL_DEFINES := \
	$(COMMON_DEFINES) \
	RELEASE

LOCAL_SRC += $(LOCAL_PATH)/src/common/srv.c
LOCAL_INC += $(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_CFLGAS := -O3

LOCAL_STATIC_LIBS := \
	ganymede

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)

