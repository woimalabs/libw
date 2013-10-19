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

#include "w/ResourceManager.hpp"
#include "ResourceManagerPrivate.hpp"
#include <string>

namespace w
{
    ResourceManager::ResourceManager(const std::string& basePath):
        private_(new ResourceManagerPrivate(basePath))
    {
    }

    ResourceManager::ResourceManager(ResourceManager const& r):
        private_(r.private_)
    {
        private_->increment();
    }

    ResourceManager::~ResourceManager()
    {
        private_->decrement();
    }

    ResourceManager& ResourceManager::operator=(ResourceManager const& r)
    {
        if (this != &r)
        {
            private_ = r.private_;
            private_->increment();
        }
        return *this;
    }

    #ifdef ANDROID
        void ResourceManager::setAssetManager(AAssetManager* assetManager)
        {
            ResourceManagerPrivate::setAssetManager(assetManager);
        }
    #endif
}
