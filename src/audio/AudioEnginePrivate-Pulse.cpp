/**
 * libw
 *
 * Copyright (C) 2013-2014 Woima Solutions Oy
 *
 * This software is provided 'as-is', without any express or implied warranty. In
 * no event will the authors be held liable for any damages arising from the use
 * of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including
 * commercial applications, and to alter it and redistribute it freely, subject to
 * the following restrictions:
 *
 * 1) The origin of this software must not be misrepresented; you must not claim
 *    that you wrote the original software. If you use this software in a product,
 *    an acknowledgment in the product documentation is appreciated.
 *
 * 2) Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3) This notice may not be removed or altered from any source distribution.
 *
 * @author antti.peuhkurinen@woimasolutions.com
 */

#include "AudioEnginePrivate.hpp"
#include "w/audio/AudioEngine.hpp"
#include "w/base/Exception.hpp"
#include "w/base/Lock.hpp"
#include "w/base/Log.hpp"

static void context_state_callback(pa_context* c, void* ptr)
{
    pa_threaded_mainloop* mainloop = (pa_threaded_mainloop*)ptr;
    switch (pa_context_get_state(c))
    {
        case PA_CONTEXT_READY:
        case PA_CONTEXT_TERMINATED:
        case PA_CONTEXT_FAILED:
            pa_threaded_mainloop_signal(mainloop, 0);
            break;
        default:
            break;
    }
}

static void stream_state_callback(pa_stream *s, void* ptr)
{
    pa_threaded_mainloop* mainloop_ = (pa_threaded_mainloop*)ptr;
    switch (pa_stream_get_state(s))
    {
        case PA_STREAM_READY:
        case PA_STREAM_TERMINATED:
        case PA_STREAM_FAILED:
            pa_threaded_mainloop_signal(mainloop_, 0);
            break;
        default:
            break;
    }
}

static void stream_write_callback(pa_stream* stream, size_t size, void* ptr)
{
    w::audio::AudioEnginePrivate* self = (w::audio::AudioEnginePrivate*)ptr;
    self->writeCallback(size);
}

namespace w
{
    namespace audio
    {
        AudioEnginePrivate* AudioEnginePrivate::singleton_ = NULL;

        AudioEnginePrivate::AudioEnginePrivate(bool mute, ResourceManager& resourceManager):
            resourceManager_(resourceManager),
            tracker_(mute),
            mainloop_(NULL),
            context_(NULL),
            stream_(NULL)
        {
            if(singleton_ != NULL)
            {
                throw Exception("AudioEngine exists! Create only one AudioEngine per process!");
            }

            // Start the mainloop
            mainloop_ = pa_threaded_mainloop_new();
            pa_threaded_mainloop_start(mainloop_);

            pa_threaded_mainloop_lock (mainloop_);
            {
                // Get a context
                context_ = pa_context_new (pa_threaded_mainloop_get_api (mainloop_), "libw");
                if(context_ == NULL)
                {
                    LOGE("pa_context_new failed");
                    throw Exception("pa_context_new failed");
                }
                pa_context_set_state_callback(context_, context_state_callback, mainloop_);

                // Connect
                if(pa_context_get_state(context_) == PA_CONTEXT_UNCONNECTED)
                {
                    if(pa_context_connect(context_, NULL, PA_CONTEXT_NOFLAGS,NULL) < 0)
                    {
                        LOGE("pa_context_connect failed");
                        pa_threaded_mainloop_unlock(mainloop_);
                        throw Exception("pa_context_connect failed");
                    }
                }

                // Wait until connected to pulse daemon (NOTE: polling OK in this one)
                pa_context_state_t state;
                while((state = pa_context_get_state(context_)) != PA_CONTEXT_READY)
                {
                    if(state == PA_CONTEXT_FAILED || state == PA_CONTEXT_TERMINATED)
                    {
                        LOGE("Unable to connect to pulsedaemon");
                        pa_threaded_mainloop_unlock(mainloop_);
                        throw Exception("Unable to connect to pulsedaemon");
                    }
                    pa_threaded_mainloop_wait(mainloop_);
                }
            }
            pa_threaded_mainloop_unlock(mainloop_);

            // Stream
            configureStream();

            // All done
            singleton_ = this;
        }

        void AudioEnginePrivate::configureStream()
        {
            const pa_sample_spec* config = NULL;

            pa_threaded_mainloop_lock(mainloop_);
            {
                if(stream_)
                {
                    pa_stream_disconnect(stream_);
                    pa_stream_unref(stream_);
                    stream_ = NULL;
                }

                // PCM mono 44100Hz 16bit, signed integer, linear, little endian
                pa_sample_spec spec;
                spec.format = PA_SAMPLE_S16LE;
                spec.rate = 44100;
                spec.channels = 1;

                stream_ = pa_stream_new(context_, "libw", &spec, NULL);
                pa_stream_set_state_callback(stream_, stream_state_callback, mainloop_);
                pa_stream_set_write_callback(stream_, stream_write_callback, this);

                if(pa_stream_connect_playback(stream_, NULL, NULL, PA_STREAM_NOFLAGS, NULL, NULL) < 0)
                {
                    goto failed;
                }

                // Wait until stream ready
                pa_stream_state_t state;
                while((state = pa_stream_get_state(stream_)) != PA_STREAM_READY)
                {
                    if(state == PA_STREAM_FAILED || state == PA_STREAM_TERMINATED)
                    {
                        goto failed;
                    }
                    pa_threaded_mainloop_wait(mainloop_);
                }

                // Check Format
                config = pa_stream_get_sample_spec(stream_);
                if(config->format == PA_SAMPLE_S16LE && config->channels == 1 && config->rate == 44100)
                {
                    LOGI("pulse stream configured: PCM mono 44100Hz 16bit, signed integer, linear, little endian");
                    pa_threaded_mainloop_unlock(mainloop_);
                    return;
                }
    failed:
                LOGE("Unsupported pulse configuration");
                throw Exception("Unsupported pulse configuration");
            }
            pa_threaded_mainloop_unlock(mainloop_);
        }

        AudioEnginePrivate::~AudioEnginePrivate()
        {
            tracker_.shutdown();
            while(tracker_.shutdownIsDone() == false)
            {
            }

            if(mainloop_)
            {
                pa_threaded_mainloop_lock(mainloop_);
            }

            if(stream_)
            {
                LOGI("disconnecting pulse stream");
                pa_stream_disconnect(stream_);
                pa_stream_unref(stream_);
                stream_ = NULL;
            }

            if(context_)
            {
                LOGI("disconnecting pulse context");
                pa_context_disconnect(context_);
                pa_context_unref(context_);
                context_ = NULL;
            }

            if(mainloop_)
            {
                LOGI("disconnecting pulse mainloop");
                pa_threaded_mainloop_unlock(mainloop_);
                pa_threaded_mainloop_stop(mainloop_);
                pa_threaded_mainloop_free(mainloop_);
                mainloop_ = NULL;
            }
        }

        void AudioEnginePrivate::writeCallback(size_t size)
        {
            unsigned char* data = new unsigned char[size];
            tracker_.getData(size, data);
            pa_stream_write(stream_, data, size, pa_xfree, 0, PA_SEEK_RELATIVE);
            pa_threaded_mainloop_signal(mainloop_, 0);
        }
    }
}
