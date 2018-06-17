

ifeq ($(LOCAL_MODULE), )
    $(error missing LOCAL_MODULE in $(strip $(LOCAL_PATH)))
endif

