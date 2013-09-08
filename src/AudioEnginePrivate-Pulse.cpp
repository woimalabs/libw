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

#include "w/AudioEngine.hpp"
#include "w/Exception.hpp"
#include "AudioEnginePrivate.hpp"
#include "Lock.hpp"

namespace w
{
    AudioEnginePrivate* AudioEnginePrivate::singleton_ = NULL;
    AudioEnginePrivate::State::Enum AudioEnginePrivate::state_ = State::Start;

    AudioEnginePrivate::AudioEnginePrivate(float volumeAtStart, const std::string& assetsPath):
        volumeAtStart_(volumeAtStart),
        assetsPath_(assetsPath)
    {
        if (singleton_ != NULL)
        {
            throw Exception("AudioEngine exists! Create only one AudioEngine per process!");
        }
        singleton_ = this;
    }

    AudioEnginePrivate::~AudioEnginePrivate()
    {
        // TODO
    }

    AudioEnginePrivate::State::Enum AudioEnginePrivate::state()
    {
        return singleton_->state_;
    }

    void AudioEnginePrivate::setVolume(float volume)
    {
        // TODO
    }

    float AudioEnginePrivate::volume()
    {
        return 0.0f; // TODO
    }

    AudioResource* AudioEnginePrivate::get(const std::string& file)
    {
        Lock lock(singleton_->mutex_);

        AudioResource* r = NULL;
        std::string key(file.c_str());
        std::map<std::string, AudioResource*>::const_iterator i;
        i = singleton_->resources_.find(key);

        if (i == singleton_->resources_.end())
        {
            r = new AudioResource(file);
            sigc::connection connection = r->destroy.connect(sigc::mem_fun(singleton_, &AudioEnginePrivate::handleResourceDestroy));
            std::string key(file.c_str());
            singleton_->resources_.insert(std::make_pair(key, r));
            singleton_->resourcesConnections_.insert(std::make_pair(r->id(), connection));
        }
        else
        {
            r = i->second;
        }
        return r;
    }

    void AudioEnginePrivate::handleResourceDestroy(unsigned int id)
    {
        Lock lock(singleton_->mutex_);

        // Remove AudioResource
        std::map<std::string, AudioResource*>::iterator i;
        for (i = singleton_->resources_.begin(); i != singleton_->resources_.end(); i++)
        {

            AudioResource* tmp = i->second;
            if (tmp->id() == id)
            {
                // Remove AudioResource's destroy signal connection
                std::map<unsigned int, sigc::connection>::iterator i2;
                i2 = singleton_->resourcesConnections_.find(id);
                singleton_->resourcesConnections_.erase(i2);

                // AudioResource will delete itself, look Referenced.hpp
                singleton_->resources_.erase(i);
            }
        }
    }
}
