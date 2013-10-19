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
	audio/TrackerSample.cpp

libw_la_CPPFLAGS = -I$(top_srcdir)/include

library_includedir = $(includedir)/w

library_include_HEADERS = \
	../include/w/ResourceManager.hpp \
	../include/w/Class.hpp \
	../include/w/Exception.hpp \
	../include/w/Log.hpp \
	../include/w/audio/AudioEngine.hpp \
	../include/w/audio/AudioAsset.hpp

libw_la_LDFLAGS = -g -module -avoid-version -shared --disable-static
