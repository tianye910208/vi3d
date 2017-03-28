LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE    := lua_static
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/liblua.a
include $(PREBUILT_STATIC_LIBRARY)




