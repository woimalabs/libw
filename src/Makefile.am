lib_LTLIBRARIES = libw.la

libw_la_SOURCES = \
	ResourceManager.cpp \
	ResourceManagerPrivate.cpp \
	AudioEngine.cpp \
	AudioEnginePrivate-Pulse.cpp \
	AudioAsset.cpp \
	AudioAssetPrivate.cpp \
	AudioResource.cpp \
	Mutex.cpp \
	Log.cpp \
	Referenced.cpp \
	Tracker.cpp \
	TrackerProducerThread.cpp \
	TrackerSample.cpp \
	Thread.cpp \
	Timer.cpp \
	FileHandle.cpp

libw_la_CPPFLAGS = -I$(top_srcdir)/include

library_includedir = $(includedir)/w

library_include_HEADERS = \
	../include/w/ResourceManager.hpp \
	../include/w/AudioEngine.hpp \
	../include/w/AudioAsset.hpp \
	../include/w/Class.hpp \
	../include/w/Exception.hpp \
	../include/w/Log.hpp

libw_la_LDFLAGS = -g -module -avoid-version -shared --disable-static
