LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := libw

LOCAL_C_INCLUDES := \
$(LOCAL_PATH)/../../../../../libsigcpp/libsigc++-2.2.10 \
$(LOCAL_PATH)/../../../../../libpng/src \
$(LOCAL_PATH)/../../../../../eigen/eigen-3.2 \
$(LOCAL_PATH)/../../../../include \
$(LOCAL_PATH)/../../../../src

# application source files:
SOURCE := ../../../../src

# files - base
FILES_BASE := $(wildcard $(LOCAL_PATH)/$(SOURCE)/base/*.cpp)

# files - audio
FILES_AUDIO := $(wildcard $(LOCAL_PATH)/$(SOURCE)/audio/*.cpp)
FILES_AUDIO := $(filter-out %/AudioEnginePrivate-iOS.cpp %/AudioEnginePrivate-Pulse.cpp, $(FILES_AUDIO))

# files - graphics
FILES_GRAPHICS := $(wildcard $(LOCAL_PATH)/$(SOURCE)/graphics/*.cpp)
FILES_GRAPHICS := $(filter-out %/WindowPrivate-iOS.cpp %/WindowPrivate-XEGL.cpp, $(FILES_GRAPHICS))

# files - scene
FILES_SCENE := $(wildcard $(LOCAL_PATH)/$(SOURCE)/scene/*.cpp)

# files - events
FILES_EVENTS := $(wildcard $(LOCAL_PATH)/$(SOURCE)/events/*.cpp)

# add files to sources
LOCAL_SRC_FILES := $(FILES_BASE:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(FILES_AUDIO:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(FILES_GRAPHICS:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(FILES_SCENE:$(LOCAL_PATH)/%=%)
LOCAL_SRC_FILES += $(FILES_EVENTS:$(LOCAL_PATH)/%=%)

# dependencies
LOCAL_SHARED_LIBRARIES := libsigcpp libpng
LOCAL_STATIC_LIBRARY   := libgnustl
LOCAL_LDLIBS           := -llog -lz -lOpenSLES -landroid -lGLESv2

include $(BUILD_SHARED_LIBRARY)
