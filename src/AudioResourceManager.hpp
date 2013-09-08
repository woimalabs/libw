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

#ifndef LIBW_AUDIORESOURCEMANAGER
#define LIBW_AUDIORESOURCEMANAGER

#include "Mutex.hpp"
#include "AudioResource.hpp"
#include <w/Class.hpp>
#include <sigc++/connection.h>
#include <string>
#include <map>

namespace w
{
    class AudioResourceManager
    {
    public:
        UNCOPYABLE(AudioResourceManager)

        AudioResourceManager(const std::string& assetPath);
        ~AudioResourceManager();
        AudioResource* get(const std::string& file);

    private:
        Mutex mutex_;
        std::map<std::string, AudioResource*> resources_;
        std::map<unsigned int, sigc::connection> resourcesConnections_;
        void handleResourceDestroy(unsigned int);
        std::string assetPath_;
    };
}

#endif
