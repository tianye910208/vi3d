
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vi3dnative
LOCAL_CFLAGS := -std=c99
LOCAL_LDLIBS := -llog -landroid -lm -lEGL -lGLESv2


LOCAL_C_INCLUDES := $(LOCAL_PATH)/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../3rd/lua-5.3.3/src


SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp)
SRC_FILES += $(wildcard $(LOCAL_PATH)/../../*.cpp)
SRC_FILES += $(wildcard $(LOCAL_PATH)/*.c)
SRC_FILES += $(wildcard $(LOCAL_PATH)/../../*.c)

LOCAL_SRC_FILES := $(SRC_FILES:$(LOCAL_PATH)/%=%)

LOCAL_STATIC_LIBRARIES := lua_static

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH)/../../../3rd)
$(call import-module, lua-5.3.3/lib_android)



