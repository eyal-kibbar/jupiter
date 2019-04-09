
LOCAL_PATH := $(call my-dir)

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
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)



# test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-dbg

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	servo-dbg \
	mpu-dbg \
	srv-dbg \

LOCAL_TASKS       := test

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)




# srv test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-srv-tst

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	srv-dbg \

LOCAL_TASKS       := srv-tst

LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)


# barometer test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-barometer-tst

LOCAL_STATIC_LIBS := \
	ganymede-dbg \
	barometer-dbg \

LOCAL_TASKS       := barometer-tst

LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)


# mpu test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-mpu-tst

LOCAL_STATIC_LIBS := \
	mpu-dbg \
	ganymede-dbg \

LOCAL_TASKS       := mpu-tst


LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)



# servo test debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-servo-tst

LOCAL_STATIC_LIBS := \
	servo-dbg \
	ganymede-dbg \

LOCAL_TASKS       := servo-tst


LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

# hexpod debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-hexpod-dbg

LOCAL_STATIC_LIBS := \
	servo-dbg \
	srv-dbg \
	ganymede-dbg \

LOCAL_TASKS       := \
	hexpod-dbg \
	hexpod-srv-dbg


LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

# hexpod release flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-hexpod

LOCAL_STATIC_LIBS := \
	servo \
	srv \
	ganymede \

LOCAL_TASKS       := \
	hexpod \
	hexpod-srv


LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)


# moisture test flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-moisture-tst

LOCAL_STATIC_LIBS := \
	ganymede-dbg \

LOCAL_TASKS       := \
	moisture-tst \


LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)


# invpend debug flavor
# ========================================
include $(CLEAR_VARS)
LOCAL_MODULE      := jupiter-invpend-dbg

LOCAL_STATIC_LIBS := \
	servo-dbg \
	mpu-dbg \
	ganymede-dbg \

LOCAL_TASKS       := \
	invpend-dbg \


LOCAL_TARGET      := atmega328p
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)

LOCAL_TARGET      := atmega168
LOCAL_LDFLAGS     := -T $(ROOT_PATH)/core/ganymede/$(LOCAL_TARGET).ld
include $(BUILD_EXEC)
