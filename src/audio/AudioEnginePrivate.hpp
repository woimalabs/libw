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

#ifndef LIBW_AUDIO_AUDIOENGINEPRIVATE
#define LIBW_AUDIO_AUDIOENGINEPRIVATE

#include "Tracker.hpp"
#include "TrackerSample.hpp"
#include "w/base/FileHandle.hpp"
#include "w/base/Log.hpp"
#include "w/base/Mutex.hpp"
#include "w/base/ResourceManager.hpp"
#include "w/base/ResourceManagerPrivate.hpp"
#include <w/base/Class.hpp>
#include <string>
#ifdef __ANDROID__
    // OpenSL ES is used on Android for audio
    #include <SLES/OpenSLES.h>
    #include <SLES/OpenSLES_Android.h>
#elif __APPLE__
    #import <AudioToolbox/AudioToolbox.h>
    #import <AVFoundation/AVFoundation.h>
#elif __linux__
    // Linux uses pulse audio
    #include <pulse/pulseaudio.h>
#endif

namespace w
{
    namespace audio
    {
        class AudioEnginePrivate
        {
        public:
            UNCOPYABLE(AudioEnginePrivate)

            friend class AudioEngine;

            static float const VolumeOffThreshold = 0.001f;

            struct State
            {
                enum Enum
                {
                    Start,
                    Starting,
                    Run,
                    ShuttingDown,
                    End
                };
            };

            void setMute(bool value)
            {
                tracker_.setMute(value);
            }

            bool mute()
            {
                return tracker_.mute();
            }

            static AudioResource* get(const std::string& filename)
            {
                std::string id = std::string("Audio:") + filename;
                AudioResource* r = dynamic_cast<AudioResource*>(ResourceManagerPrivate::assetPrivate(id));
                if (r == NULL)
                {
                    FileHandle* tmp = ResourceManagerPrivate::getFileHandle(filename);
                    if (tmp != NULL)
                    {
                        r = new AudioResource(tmp);
                        ResourceManagerPrivate::setAssetPrivate(id, r);
                        delete tmp;
                    }
                }

                return r;
            }

            static bool play(AudioResource* resource, bool volume, bool looping)
            {
                TrackerSample* tmp = new TrackerSample(resource, volume, looping);

                tmp->increment();
                bool r = singleton_->tracker_.place(tmp);
                tmp->decrement();

                return r;
            }

    #ifdef __ANDROID__
            void writeCallback();
    #elif __linux__
            void writeCallback(size_t size);
    #elif __APPLE__
            void writeCallback(size_t size, SInt16* targetBuffer);
    #endif

        private:
            AudioEnginePrivate(bool mute, ResourceManager& resourceManager);
            ~AudioEnginePrivate();
            static AudioEnginePrivate* singleton_;
            ResourceManager resourceManager_; // Holding a reference so that AudioEnginePrivate can call ResourceManagerPrivate functions safely
            Tracker tracker_;
            State::Enum state_;
            Mutex mutex_;
            float volumeAtStart_;
            volatile bool mute_;

            #ifdef __ANDROID__
                void createEngine();
                void createBufferQueueAudioPlayer();
                void shutdown();
                void test();

                // Engine interfaces
                SLObjectItf engineObject_;
                SLEngineItf engineEngine_;

                // Output mix interfaces
                SLObjectItf outputMixObject_;

                // Buffer queue player interfaces
                SLObjectItf bqPlayerObject_;
                SLPlayItf bqPlayerPlay_;
                SLAndroidSimpleBufferQueueItf bqPlayerBufferQueue_;
                SLEffectSendItf bqPlayerEffectSend_;
                SLMuteSoloItf bqPlayerMuteSolo_;
                SLVolumeItf bqPlayerVolume_;

                unsigned char buffer_[44100];

            #elif __APPLE__
                void setupAudioUnitSession();
                void setupAudioGraph(UInt32 busCount);
                void audioRouteChangeListenerCallback(void* inUserData,
                    AudioSessionPropertyID inPropertyID, UInt32 inPropertyValueSize,
                    const void* inPropertyValue);
                AudioStreamBasicDescription streamFormat_;
                AudioComponentInstance ioUnit_;

            #elif __linux__
                void configureStream();
                pa_threaded_mainloop* mainloop_;
                pa_context* context_;
                pa_stream* stream_;

            #endif
        };
    }
}

#endif
