lib_LTLIBRARIES = libw.la

libw_la_SOURCES = \
	AssetManager.cpp \
	AssetManagerPrivate.cpp \
	AudioEngine.cpp \
	AudioEnginePrivate-Pulse.cpp \
	AudioAsset.cpp \
	AudioAssetPrivate.cpp \
	AudioResourceManager.cpp \
	AudioResource.cpp \
	Mutex.cpp \
	Log.cpp \
	Referenced.cpp \
	Tracker.cpp \
	TrackerProducerThread.cpp \
	TrackerSample.cpp \
	Thread.cpp \
	Timer.cpp \
	Storage.cpp \
	StoragePrivate.cpp

libw_la_CPPFLAGS = -I$(top_srcdir)/include

library_includedir = $(includedir)/w

library_include_HEADERS = \
	../include/w/AssetManager.hpp \
	../include/w/AudioEngine.hpp \
	../include/w/AudioAsset.hpp \
	../include/w/Class.hpp \
	../include/w/Exception.hpp \
	../include/w/Log.hpp  \
	../include/w/Storage.hpp

libw_la_LDFLAGS = -module -avoid-version -shared --disable-static
