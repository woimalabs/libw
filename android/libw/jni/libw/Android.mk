LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libw

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../../libsigcpp/libsigc++-2.2.10 \
$(LOCAL_PATH)/../../../../include

# Application source files:
SOURCE := ../../../../src

LOCAL_SRC_FILES := \
$(SOURCE)/Log.cpp \
$(SOURCE)/Timer.cpp  \
$(SOURCE)/Mutex.cpp \
$(SOURCE)/Referenced.cpp \
$(SOURCE)/Thread.cpp \
\
$(SOURCE)/StoragePrivate.cpp \
$(SOURCE)/Storage.cpp \
\
$(SOURCE)/AudioResourceManager.cpp \
$(SOURCE)/AudioResource.cpp \
$(SOURCE)/AudioEnginePrivate-Android.cpp \
$(SOURCE)/AudioEngine.cpp \
$(SOURCE)/AudioAssetPrivate.cpp \
$(SOURCE)/AudioAsset.cpp \
$(SOURCE)/TrackerSample.cpp \
$(SOURCE)/TrackerProducerThread.cpp \
$(SOURCE)/Tracker.cpp

# Dependencies
LOCAL_SHARED_LIBRARIES := libsigcpp
LOCAL_STATIC_LIBRARY   := libgnustl
LOCAL_LDLIBS           := -llog -lz

include $(BUILD_SHARED_LIBRARY)
