LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vi3d_static

LOCAL_SRC_FILES := ../../event.cpp \
				   ../../file.cpp \
				   ../../macro.cpp \
				   ../../object.cpp \
				   ../../system.cpp \
				   ../../vi3d.cpp \
				   ../../window.cpp \
				   window_android.cpp \


LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../ \

LOCAL_EXPORT_LDLIBS := -llog -landroid -lGLESv2 

include $(BUILD_STATIC_LIBRARY)

