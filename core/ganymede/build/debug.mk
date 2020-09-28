

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
	DEBUG \# atmega168
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

include $(BUILD_STATIC_LIB)
	LOG_LEVEL=1

LOCAL_CFLAGS := -Og

include $(BUILD_STATIC_LIB)


# atmega2560
# ========================================
include $(CLEAR_VARS)

LOCAL_TARGET := atmega2560
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

include $(BUILD_STATIC_LIB)



# linux
# ========================================
include $(CLEAR_VARS)

LOCAL_TARGET := linux
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

include $(BUILD_STATIC_LIB)

# linux test
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := gmd-tst

LOCAL_STATIC_LIBS := \
	ganymede-dbg \

LOCAL_TASKS       := \
	gmd-tst \


LOCAL_TARGET      := linux
#LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)



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

LOCAL_TARGET := atmega2560
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

LOCAL_TARGET := atmega2560
include $(BUILD_GMD_TASK)

# test
# ========================================
include $(CLEAR_VARS)

LOCAL_MODULE := gmd-tst
COMMON_LOCAL_DEFINES := LOG_LEVEL=3
LOCAL_STATIC_LIBS := \
	ganymede-dbg

LOCAL_SRC += $(LOCAL_PATH)/tst/test_tsk.c
LOCAL_TARGET := atmega168
LOCAL_DEFINES := \
	$(COMMON_LOCAL_DEFINES) \
	STACK_SIZE=256
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega328p
LOCAL_DEFINES := \
	$(COMMON_LOCAL_DEFINES) \
	 STACK_SIZE=256
include $(BUILD_GMD_TASK)

LOCAL_TARGET := atmega2560
LOCAL_DEFINES := \
	$(COMMON_LOCAL_DEFINES) \
	 STACK_SIZE=256
include $(BUILD_GMD_TASK)

LOCAL_TARGET := linux
LOCAL_DEFINES := \
	$(COMMON_LOCAL_DEFINES) \
	STACK_SIZE=65528
include $(BUILD_GMD_TASK)


# ganymede test
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := gmd-tst

LOCAL_STATIC_LIBS := \
	ganymede-dbg \

LOCAL_TASKS       := \
	gmd-tst1 \
	gmd-tst2 \


LOCAL_TARGET      := atmega168
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega2560
include $(BUILD_EXEC)
