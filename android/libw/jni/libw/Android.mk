LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libw

SOURCE := ../../../../src

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../../libsigcpp/libsigc++-2.2.10 \
$(LOCAL_PATH)/../../../../include

# Application source files:
LOCAL_SRC_FILES := \
$(SOURCE)/Timer.cpp  \
$(SOURCE)/Mutex.cpp \
$(SOURCE)/Referenced.cpp \
$(SOURCE)/StoragePrivate.cpp \
$(SOURCE)/Storage.cpp \
$(SOURCE)/Log.cpp \
$(SOURCE)/AudioResource.cpp \
$(SOURCE)/Tracker.cpp \
$(SOURCE)/TrackerSample.cpp \
$(SOURCE)/AudioEngine.cpp \
$(SOURCE)/AudioResourceManager.cpp \
$(SOURCE)/AudioAsset.cpp \
$(SOURCE)/AudioAssetPrivate.cpp \
$(SOURCE)/AudioResource.cpp

# Dependencies
LOCAL_SHARED_LIBRARIES := libsigcpp
LOCAL_STATIC_LIBRARY   := libgnustl
#LOCAL_SRC_FILES        := libsigcpp.so
LOCAL_LDLIBS           := -llog -lz -landroid

include $(BUILD_SHARED_LIBRARY)
