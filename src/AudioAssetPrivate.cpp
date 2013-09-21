/**
 * libw
 *
 * Copyright (C) 2013 Woima Solutions
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

#include "AudioAssetPrivate.hpp"
#include "AudioEnginePrivate.hpp"
#include "w/Log.hpp"

namespace w
{
    AudioAssetPrivate::AudioAssetPrivate(const std::string& filename, bool parallelPlay, bool looping):
        Referenced(),
        resource_(AudioEnginePrivate::get(filename)),
        parallerPlay_(parallelPlay),
        looping_(looping)
    {
        // AudioEnginePrivate::get() return always either != NULL or throws Exception
        resource_->increment();
    }

    AudioAssetPrivate::~AudioAssetPrivate()
    {
        LOCK
        LOG
        // Unreference related resource
        resource_->decrement();
        resource_ = NULL;
        LOG
    }

    bool AudioAssetPrivate::play(float volume)
    {
        LOGD("play S");

        // Sanity check for volume value
        if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        else if (volume < 0.0f)
        {
            volume = 0.0f;
        }
        LOGD("play E");

        return AudioEnginePrivate::play(resource_, volume, looping_);
    }

    void AudioAssetPrivate::setVolume(float volume)
    {
        LOCK

        // Sanity check for volume value
        if (volume > 1.0f)
        {
            volume = 1.0f;
        }
        else if (volume < 0.0f)
        {
            volume = 0.0f;
        }

        // TODO: inform audioengine private to tune volume of this assets trackersamples
    }

    void AudioAssetPrivate::fadeOut(unsigned int fadeOutTimeMilliseconds)
    {
        LOCK

        // Sanity check for volume value
        if (fadeOutTimeMilliseconds > 2000)
        {
            fadeOutTimeMilliseconds = 2000;
        }

        // TODO: inform audioengine private to tune volume of this assets trackersamples
    }
}
