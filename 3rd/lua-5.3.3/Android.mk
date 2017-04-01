LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
$(warning "$(LOCAL_PATH)/$(TARGET_ARCH_ABI)/liblua.a")  
LOCAL_MODULE    := lua_static
LOCAL_CFLAGS	:= -std=c99
LOCAL_SRC_FILES := $(LOCAL_PATH)/$(TARGET_ARCH_ABI)/liblua.a
include $(PREBUILT_STATIC_LIBRARY)



