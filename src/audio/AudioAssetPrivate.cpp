/**
 * libw
 *
 * Copyright (C) 2013-2015 Woima Solutions Oy
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
#include "TrackerSample.hpp"
#include "w/base/Log.hpp"
#include "w/base/Helpers.hpp"

namespace w
{
    namespace audio
    {
        AudioAssetPrivate::AudioAssetPrivate(const std::string& filename, bool parallelPlay, bool looping):
            Referenced(),
            resource_(AudioEnginePrivate::get(filename)),
            parallerPlay_(parallelPlay),
            looping_(looping)
        {
            //resource_->increment();
        }

        AudioAssetPrivate::~AudioAssetPrivate()
        {
            // End playing
            fadeOut(0);

            // Unreference related resource (trackersample might still hold a reference to resource until the track ends)
            //resource_->decrement();
            //resource_ = NULL;
        }

        bool AudioAssetPrivate::play(float volume)
        {
            // Sanity check for volume value
            volume = w::clamp(volume, 0.0f, 1.0f);

            LOCK

            ReferencedPointer<TrackerSample> tmp(new TrackerSample(resource_, volume, false));
            tmp.pointer()->audioAssetInterestedFromEnd = tmp.pointer()->ended.connect(sigc::mem_fun(this, &AudioAssetPrivate::handleTrackerSampleEnd));
            bool r = AudioEnginePrivate::play(tmp);
            if(r == true)
            {
                playing_.push_back(tmp);
            }
            return r;
        }

        void AudioAssetPrivate::handleTrackerSampleEnd(unsigned int id)
        {
            LOCK
            for(std::list<ReferencedPointer<TrackerSample> >::iterator i = playing_.begin();
                i != playing_.end();
                i++)
            {
                if((*i).pointer()->id() == id)
                {
                    playing_.erase(i);
                    break;
                }
            }
        }

        void AudioAssetPrivate::setVolume(float volume)
        {
            // Sanity check for the new value
            volume = w::clamp(volume, 0.0f, 1.0f);

            // Tune volume of this asset's trackersamples
            LOCK
            for(std::list<ReferencedPointer<TrackerSample> >::iterator i = playing_.begin();
                i != playing_.end();
                i++)
            {
                (*i).pointer()->setVolume(volume);
            }
        }

        void AudioAssetPrivate::fadeOut(unsigned int fadeOutTimeMilliseconds)
        {
            // Sanity check for the new value
            fadeOutTimeMilliseconds = w::clamp(fadeOutTimeMilliseconds, 0, 1000);

            // Fade out the playing samples. This also disconnects the "ended"
            // signal listening.
            LOCK
            while(playing_.begin() != playing_.end())
            {
                std::list<ReferencedPointer<TrackerSample> >::iterator i = playing_.begin();
                (*i).pointer()->fadeOut(fadeOutTimeMilliseconds);
                playing_.erase(i);
            }
        }
    }
}
