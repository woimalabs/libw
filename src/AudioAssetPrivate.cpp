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

namespace w
{
    AudioAssetPrivate::AudioAssetPrivate(const std::string& filename, bool parallelPlay, bool looping):
        Referenced(),
        resource_(AudioEnginePrivate::get(filename)),
        parallerPlay_(parallelPlay),
        looping_(looping)
    {
    }

    AudioAssetPrivate::~AudioAssetPrivate()
    {
        LOCK

        // Stop listening destroy signals
        std::list<sigc::connection>::iterator i = playingConnections_.begin();
        for(; i != playingConnections_.end(); ++i)
        {
            sigc::connection connection = *i;
            connection.disconnect();
        }
        playingConnections_.empty();

        // Not interested from what we are playing, so empty the playing_ list
        playing_.clear();

        // Unreference related resource
        resource_->decrement();
    }

    bool AudioAssetPrivate::play(float volume)
    {
        // Sanity check for volume value
        if (volume > 1.0f)
            volume = 1.0f;
        else if (volume < 0.0f)
            volume = 0.0f;

        bool r = false;
        TrackerSample* trackerSample = NULL;

        if (playing_.size() == 0)
        {
            trackerSample = new TrackerSample(resource_, volume, looping_);
            trackerSample->increment();
            {
                // Add trackerSample to playing list. NOTE: in case tracker
                // cannot take trackerSample, handleDestroy is OK to proceed
                LOCK
                sigc::connection connection = trackerSample->destroy.connect(sigc::mem_fun(this, &AudioAssetPrivate::handleDestroy));
                playing_.push_back(trackerSample);
                playingConnections_.push_back(connection);

            }
            r = AudioEnginePrivate::play(trackerSample);

            // TrackerSample is destroyed now if it was not added to Tracker
            trackerSample->decrement();
        }

        return r;
    }

    void AudioAssetPrivate::setVolume(float volume)
    {
        LOCK

        // Sanity check for volume value
        if (volume > 1.0f)
            volume = 1.0f;
        else if (volume < 0.0f)
            volume = 0.0f;

        for (std::list<TrackerSample*>::iterator i = playing_.begin(); i != playing_.end(); ++i)
        {
            TrackerSample* tmp = *i;
            tmp->setVolume(volume);
        }
    }

    void AudioAssetPrivate::fadeOut(unsigned int fadeOutTimeMilliseconds)
    {
        LOCK

        for(std::list<TrackerSample*>::iterator i = playing_.begin(); i != playing_.end(); ++i)
        {
            TrackerSample* tmp = *i;
            tmp->fadeOut(fadeOutTimeMilliseconds);
        }
    }

    void AudioAssetPrivate::handleDestroy(unsigned int id)
    {
        LOCK

        // List sizes match
        std::list<TrackerSample*>::iterator i = playing_.begin();
        std::list<sigc::connection>::iterator i2 = playingConnections_.begin();

        for(; i != playing_.end(); ++i, ++i2)
        {

            TrackerSample* tmp = *i;
            if (tmp->id() == id)
            {
                // TrackerSample was consumed, stop listening it
                sigc::connection connection = *i2;
                connection.disconnect();
                playingConnections_.erase(i2);

                // TrackerSample is deleted after this function call
                playing_.erase(i);

                // tmp->id() == id, so this was the one destroyed
                break;
            }
        }
    }
}
