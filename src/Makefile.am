lib_LTLIBRARIES = libw.la

libw_la_SOURCES = \
	base/Log.cpp \
	base/Thread.cpp \
	base/Timer.cpp \
	base/ResourceManager.cpp \
	base/ResourceManagerPrivate.cpp \
	base/FileHandle.cpp \
	base/Mutex.cpp \
	base/Referenced.cpp \
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
	graphics/MeshAssetPrivate-GLES2.cpp \
	graphics/Renderer.cpp \
	graphics/RendererPrivate-GLES2.cpp \
	scene/Component.cpp \
	scene/NodePrivate.cpp \
	scene/Node.cpp \
	events/EventBuffer.cpp

libw_la_CPPFLAGS = -I$(top_srcdir)/include

library_base_includedir = $(includedir)/w/base
library_base_include_HEADERS = \
	../include/w/base/Referenced.hpp \
	../include/w/base/ReferencedPointer.hpp \
	../include/w/base/Exception.hpp \
	../include/w/base/Mutex.hpp \
	../include/w/base/ResourceManager.hpp \
	../include/w/base/Class.hpp \
	../include/w/base/Log.hpp \
	../include/w/base/Lock.hpp \
	../include/w/base/Thread.hpp \
	../include/w/base/Sigcpp.hpp

library_audio_includedir = $(includedir)/w/audio
library_audio_include_HEADERS = \
	../include/w/audio/AudioEngine.hpp \
	../include/w/audio/AudioAsset.hpp

library_math_includedir = $(includedir)/w/math
library_math_include_HEADERS = \
	../include/w/math/Ray.hpp \
	../include/w/math/Math.hpp  \
	../include/w/math/Eigen.hpp

library_graphics_includedir = $(includedir)/w/graphics
library_graphics_include_HEADERS = \
	../include/w/graphics/Window.hpp \
	../include/w/graphics/ShaderProgramAsset.hpp \
	../include/w/graphics/TextureAsset.hpp \
	../include/w/graphics/MeshAsset.hpp \
	../include/w/graphics/Renderer.hpp

library_scene_includedir = $(includedir)/w/scene
library_scene_include_HEADERS = \
	../include/w/scene/Component.hpp \
	../include/w/scene/ComponentPrivate.hpp \
	../include/w/scene/Node.hpp \
	../include/w/scene/Visitor.hpp

library_scene_includedir = $(includedir)/w/events
library_scene_include_HEADERS = \
	../include/w/events/EventBuffer.hpp

libw_la_LDFLAGS = -g -module -avoid-version -shared --disable-static
