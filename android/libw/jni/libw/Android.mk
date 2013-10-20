LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libw

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../../libsigcpp/libsigc++-2.2.10 \
$(LOCAL_PATH)/../../../../include \
$(LOCAL_PATH)/../../../../src

# Application source files:
SOURCE := ../../../../src

LOCAL_SRC_FILES := \
$(SOURCE)/Log.cpp \
$(SOURCE)/Timer.cpp  \
$(SOURCE)/Mutex.cpp \
$(SOURCE)/Referenced.cpp \
$(SOURCE)/Thread.cpp \
$(SOURCE)/FileHandle.cpp \
$(SOURCE)/ResourceManager.cpp \
$(SOURCE)/ResourceManagerPrivate.cpp \
\
$(SOURCE)/audio/AudioResource.cpp \
$(SOURCE)/audio/AudioEnginePrivate-Android.cpp \
$(SOURCE)/audio/AudioEngine.cpp \
$(SOURCE)/audio/AudioAssetPrivate.cpp \
$(SOURCE)/audio/AudioAsset.cpp \
$(SOURCE)/audio/TrackerSample.cpp \
$(SOURCE)/audio/TrackerProducerThread.cpp \
$(SOURCE)/audio/Tracker.cpp \
\
$(SOURCE)/graphics/Window.cpp \
$(SOURCE)/graphics/WindowPrivate-Android.cpp

# Dependencies
LOCAL_SHARED_LIBRARIES := libsigcpp
LOCAL_STATIC_LIBRARY   := libgnustl
LOCAL_LDLIBS           := -llog -lz -lOpenSLES -landroid -lGLESv2

include $(BUILD_SHARED_LIBRARY)
