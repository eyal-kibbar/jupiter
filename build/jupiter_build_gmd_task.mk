

include $(CHECK_LOCALS)

_DIR_OUT := $(DIR_OUT)/$(LOCAL_TARGET)

DIR_OBJS := $(_DIR_OUT)/$(LOCAL_MODULE)/objs
$(shell mkdir -p $(DIR_OBJS))

OBJS := $(LOCAL_SRC)
OBJS := $(patsubst $(ROOT_PATH)/%.c, %.c, $(OBJS))
OBJS := $(patsubst %.c, $(DIR_OBJS)/%.o, $(OBJS))

API := $(foreach module, $(LOCAL_STATIC_LIBS), $(value $(LOCAL_TARGET)_$(module)_EXPORT) )
USED_LIBS := $(patsubst %, $(_DIR_OUT)/lib%.a, $(LOCAL_STATIC_LIBS))

# print information
# ========================================

$(info ========================================)
$(info building task)
$(info module:  $(LOCAL_MODULE))
$(info objects:)
$(foreach X,$(OBJS), $(info - $(X)))

$(info using libs:)
$(foreach X,$(USED_LIBS), $(info - $(X)))

$(info external apis:)
$(foreach X,$(API), $(info - $(X)))

$(info config:)
$(foreach X,$(LOCAL_DEFINES), $(info - $(X)))

$(info )

# compiler flags
# ========================================

# setting define flags
CFLGAS_DEFINES := $(addprefix -D, $(LOCAL_DEFINES))

# setting include flags
CFLAGS_INCS := \
	$(addprefix -I, $(LOCAL_INC))    \
	$(addprefix -I, $(LOCAL_EXPORT)) \
	$(addprefix -I, $(API)) \


# rules
# ========================================

# adding header files dependencies
-include $(addsuffix .d, $(OBJS))

# task rule
UNIQUE_TASK_NAME := $(strip $(subst -,_,$(LOCAL_MODULE)))

$(_DIR_OUT)/tsk$(LOCAL_MODULE).o: LOCAL_TARGET := $(LOCAL_TARGET)
$(_DIR_OUT)/tsk$(LOCAL_MODULE).o: TASK_NAME    := $(UNIQUE_TASK_NAME)
$(_DIR_OUT)/tsk$(LOCAL_MODULE).o: $(OBJS)
	$(CC_$(LOCAL_TARGET)) -r -nostdlib $^ -o $@
	$(OBJCOPY_$(LOCAL_TARGET)) \
		--redefine-sym task=task_$(TASK_NAME) \
		--redefine-sym setup=$(TASK_NAME)_setup \
		--redefine-sym loop=$(TASK_NAME)_loop \
		$@
	

# object files rules
$(DIR_OBJS)/%.o: LOCAL_CFLAGS   := $(LOCAL_CFLAGS)
$(DIR_OBJS)/%.o: CFLAGS_INCS    := $(CFLAGS_INCS)
$(DIR_OBJS)/%.o: CFLGAS_DEFINES := $(CFLGAS_DEFINES)
$(DIR_OBJS)/%.o: LOCAL_TARGET   := $(LOCAL_TARGET)
$(DIR_OBJS)/%.o: $(ROOT_PATH)/%.c
	@mkdir -p $(@D)
	$(CC_$(LOCAL_TARGET)) -MMD -MP -MF $@.d -o $@ $(CFLAGS) $(CFLAGS_$(LOCAL_TARGET)) $(LOCAL_CFLAGS) $(CFLGAS_DEFINES) $(CFLAGS_INCS) -c $<
	

TASKS += $(_DIR_OUT)/tsk$(LOCAL_MODULE).o


