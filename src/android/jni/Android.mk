
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := vi3dnative
LOCAL_SRC_FILES := main.cpp
LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv2

LOCAL_C_INCLUDES += $(LOCAL_PATH)/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../


SRC_FILES := $(wildcard $(LOCAL_PATH)/*.cpp)
SRC_FILES += $(wildcard $(LOCAL_PATH)/../../*.cpp)
SRC_FILES += $(wildcard $(LOCAL_PATH)/*.c)
SRC_FILES += $(wildcard $(LOCAL_PATH)/../../*.c)
LOCAL_SRC_FILES := $(SRC_FILES:$(LOCAL_PATH)/%=%)


include $(BUILD_SHARED_LIBRARY)


