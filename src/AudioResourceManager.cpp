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

#include "AudioResourceManager.hpp"
#include "Lock.hpp"
#include "w/Log.hpp"

namespace w
{
    AudioResourceManager::AudioResourceManager(const std::string& assetPath):
        assetPath_(assetPath)
    {
    }

    AudioResourceManager::~AudioResourceManager()
    {
        LOCK

        if (resources_.size() == 0)
        {
            LOGI("All audio resources freed.");
        }
        else
        {
            LOGE("All audio resources NOT freed: %d still exists.", resources_.size())
        }
    }

    AudioResource* AudioResourceManager::get(const std::string& file)
    {
        LOCK

        AudioResource* r = NULL;
        std::string key = assetPath_ + std::string("/") + file;

        std::map<std::string, AudioResource*>::const_iterator i;
        i = resources_.find(key);
        if (i == resources_.end())
        {
            r = new AudioResource(file);
            sigc::connection connection = r->destroy.connect(sigc::mem_fun(this, &AudioResourceManager::handleResourceDestroy));
            resources_.insert(std::make_pair(key, r));
            resourcesConnections_.insert(std::make_pair(r->id(), connection));
        }
        else
        {
            r = i->second;
        }

        return r;
    }

    void AudioResourceManager::handleResourceDestroy(unsigned int id)
    {
        LOCK

        std::map<std::string, AudioResource*>::iterator i;
        for (i = resources_.begin(); i != resources_.end(); i++)
        {
            AudioResource* tmp = i->second;
            if (tmp->id() == id)
            {
                // Remove AudioResource's destroy signal connection
                std::map<unsigned int, sigc::connection>::iterator i2;
                i2 = resourcesConnections_.find(id);
                resourcesConnections_.erase(i2);

                // AudioResource will delete itself, look Referenced.hpp
                resources_.erase(i);
            }
        }
    }
}
