
# server
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := srv-dbg

LOCAL_DEFINES := \
	$(COMMON_DEFINES) \
	LOG_LEVEL=3 \
	DEBUG

LOCAL_SRC += $(LOCAL_PATH)/src/common/srv.c
LOCAL_INC += $(LOCAL_PATH)/inc

LOCAL_EXPORT := $(LOCAL_PATH)/api

LOCAL_STATIC_LIBS := \
	ganymede-dbg

LOCAL_TARGET := atmega168
include $(BUILD_STATIC_LIB)

LOCAL_TARGET := atmega328p
include $(BUILD_STATIC_LIB)


# test
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := srv-tst

LOCAL_DEFINES := \
	$(COMMON_DEFINES) \
	STACK_SIZE=256 \
	LOG_LEVEL=3

LOCAL_STATIC_LIBS := \
	srv-dbg \
	ganymede-dbg

LOCAL_SRC += $(LOCAL_PATH)/tst/srv_test.c
LOCAL_TARGET := atmega168
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
include $(BUILD_GMD_TASK)


# srv test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-srv-tst

LOCAL_STATIC_LIBS := \
	srv-dbg \
	ganymede-dbg \

LOCAL_TASKS       := srv-tst

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)
