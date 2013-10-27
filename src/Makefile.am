lib_LTLIBRARIES = libw.la

libw_la_SOURCES = \
	Mutex.cpp \
	Log.cpp \
	Referenced.cpp \
	Thread.cpp \
	Timer.cpp \
	ResourceManager.cpp \
	ResourceManagerPrivate.cpp \
	FileHandle.cpp \
	audio/AudioEngine.cpp \
	audio/AudioEnginePrivate-Pulse.cpp \
	audio/AudioAsset.cpp \
	audio/AudioAssetPrivate.cpp \
	audio/AudioResource.cpp \
	audio/Tracker.cpp \
	audio/TrackerProducerThread.cpp \
	audio/TrackerSample.cpp \
	graphics/Window.cpp \
	graphics/WindowPrivate-XEGL.cpp \
	graphics/ShaderProgramAsset.cpp \
	graphics/ShaderProgramAssetPrivate-GLES2.cpp \
	graphics/TextureAsset.cpp \
	graphics/TextureAssetPrivate-GLES2.cpp \
	graphics/MeshAsset.cpp \
	graphics/MeshAssetPrivate-GLES2.cpp

libw_la_CPPFLAGS = -I$(top_srcdir)/include

library_common_includedir = $(includedir)/w
library_common_include_HEADERS = \
	../include/w/ResourceManager.hpp \
	../include/w/Class.hpp \
	../include/w/Exception.hpp \
	../include/w/Log.hpp

library_audio_includedir = $(includedir)/w/audio
library_audio_include_HEADERS = \
	../include/w/audio/AudioEngine.hpp \
	../include/w/audio/AudioAsset.hpp

library_math_includedir = $(includedir)/w/math
library_math_include_HEADERS = \
	../include/w/math/Vector4.hpp

library_graphics_includedir = $(includedir)/w/graphics
library_graphics_include_HEADERS = \
	../include/w/graphics/Window.hpp \
	../include/w/graphics/ShaderProgramAsset.hpp \
	../include/w/graphics/TextureAsset.hpp \
	../include/w/graphics/MeshAsset.hpp

libw_la_LDFLAGS = -g -module -avoid-version -shared --disable-static
