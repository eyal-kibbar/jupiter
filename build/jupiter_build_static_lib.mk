

include $(CHECK_LOCALS)


_DIR_OUT := $(DIR_OUT)/$(LOCAL_TARGET)/libs

DIR_OBJS := $(_DIR_OUT)/$(LOCAL_MODULE)/objs
$(shell mkdir -p $(DIR_OBJS))

# setting all the object files
OBJS := $(LOCAL_SRC)
OBJS := $(patsubst $(ROOT_PATH)/%.c, %.c, $(OBJS))
OBJS := $(patsubst %.c, $(DIR_OBJS)/%.o, $(OBJS))

# making the required libs dependecies
USED_LIBS := $(patsubst %, $(_DIR_OUT)/lib%.a, $(LOCAL_STATIC_LIBS))

# adding the api folders
$(LOCAL_TARGET)_$(LOCAL_MODULE)_EXPORT := $(LOCAL_EXPORT)
API := $(foreach module, $(LOCAL_STATIC_LIBS), $(value $(LOCAL_TARGET)_$(module)_EXPORT) )

# print information
# ========================================

$(info ========================================)
$(info building static library)
$(info module:  $(LOCAL_MODULE))
$(info objects:)
$(foreach X,$(OBJS), $(info - $(X)))

$(info using libs:)
$(foreach X,$(USED_LIBS), $(info - $(X)))

$(info external apis:)
$(foreach X,$(API), $(info - $(X)))

$(info config:)
$(foreach X,$(LOCAL_DEFINES), $(info - $(X)))

$(info exporting:)
$(foreach X,$(LOCAL_EXPORT), $(info - $(X)))
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

# static library rule
$(_DIR_OUT)/lib$(LOCAL_MODULE).a: LOCAL_TARGET := $(LOCAL_TARGET)
$(_DIR_OUT)/lib$(LOCAL_MODULE).a: $(USED_LIBS)
$(_DIR_OUT)/lib$(LOCAL_MODULE).a: $(OBJS)
	$(AR_$(LOCAL_TARGET)) rcs $@ \
		$^


# object files rules
$(DIR_OBJS)/%.o: LOCAL_CFLAGS       := $(LOCAL_CFLAGS)
$(DIR_OBJS)/%.o: CFLAGS_INCS        := $(CFLAGS_INCS)
$(DIR_OBJS)/%.o: CFLGAS_DEFINES     := $(CFLGAS_DEFINES)
$(DIR_OBJS)/%.o: LOCAL_TARGET       := $(LOCAL_TARGET)
$(DIR_OBJS)/%.o: LOCAL_OBJCOPY_ARGS := $(LOCAL_OBJCOPY_ARGS)
$(DIR_OBJS)/%.o: $(ROOT_PATH)/%.c
	@mkdir -p $(@D)
	$(CC_$(LOCAL_TARGET)) -MMD -MP -MF $@.d -o $@ $(CFLAGS) $(CFLAGS_$(LOCAL_TARGET)) $(LOCAL_CFLAGS) $(CFLGAS_DEFINES) $(CFLAGS_INCS) -c $<
	$(OBJCOPY_$(LOCAL_TARGET)) $(LOCAL_OBJCOPY_ARGS) $@



# adding library to all libs list
$(info adding $(_DIR_OUT)/lib$(LOCAL_MODULE).a)
LIBS += $(_DIR_OUT)/lib$(LOCAL_MODULE).a
