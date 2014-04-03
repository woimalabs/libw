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
#include <w/audio/AudioEngine.hpp>
#include <w/base/Log.hpp>
#include <w/base/Exception.hpp>

namespace w
{
    namespace audio
    {
        AudioEnginePrivate* AudioEngine::private_ = NULL;

        AudioEngine::AudioEngine(float volumeAtStart, ResourceManager& resourceManager)
        {
            if (private_ != NULL)
            {
                throw Exception("Only one AudioEngine can exist once.");
            }
            private_ = new AudioEnginePrivate(volumeAtStart, resourceManager);
            LOGI("Created AudioEngine.")
        }

        AudioEngine::~AudioEngine()
        {
            LOGI("Shutting down AudioEngine...")
            if (private_ != NULL)
            {
                delete private_;
                private_ = NULL;
            }
            else
            {
                LOGE("AudioEngine::~AudioEngine(), private == NULL, AudioEngine in corrupted state!")
            }
            LOGI("Shutdown AudioEngine.")
        }

        void AudioEngine::setVolume(float volume)
        {
            if (private_ != NULL)
            {
                private_->setVolume(volume);
            }
            else
            {
                throw Exception("AudioEngine::setVolume(), AudioEngine does not exist.");
            }
        }

        float AudioEngine::volume()
        {
            if (private_ != NULL)
            {
                return private_->volume();
            }
            else
            {
                throw Exception("AudioEngine::volume(), AudioEngine does not exist.");
            }
        }
    }
}
