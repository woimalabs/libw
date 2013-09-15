LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libw

SOURCE := ../../../../src

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../../libsigcpp-android/jni \
$(LOCAL_PATH)/../../../../include

# Application source files:
LOCAL_SRC_FILES := \
\
$(SOURCE)/Condition.cpp \
$(SOURCE)/Log.cpp \
$(SOURCE)/Mutex.cpp \
$(SOURCE)/Timer.cpp \
$(SOURCE)/Referenced.cpp \
$(SOURCE)/Thread.cpp


# Dependencies
LOCAL_SHARED_LIBRARIES := libsigcpp
LOCAL_STATIC_LIBRARY   := libgnustl
LOCAL_LDLIBS           := -llog -lz -landroid

include $(BUILD_SHARED_LIBRARY)

