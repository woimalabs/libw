/**
 * libw
 *
 * Copyright (C) 2012-2015 Woima Solutions
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

#include "w/base/ResourceManagerPrivate.hpp"
#include "w/base/Lock.hpp"
#include <w/base/Exception.hpp>
#include <w/base/Log.hpp>
#include <utility> // std::pair
#include <functional>
using std::make_pair;

namespace w
{
    ResourceManagerPrivate* ResourceManagerPrivate::singleton_ = NULL;
    #ifdef ANDROID
        AAssetManager* ResourceManagerPrivate::androidAssetManager_ = NULL;
    #endif

    ResourceManagerPrivate::ResourceManagerPrivate(const std::string& basePath):
        basePath_(basePath)
    {
        if (singleton_ != NULL)
        {
            LOGE("ResourceManagerPrivate (==ResourceManager in public API!) exists already.");
            throw Exception("ResourceManagerPrivate exists already.");
        }
        else
        {
            //LOGI("ResourceManagerPrivate created.");
        }
        singleton_ = this;
    }

    ResourceManagerPrivate::~ResourceManagerPrivate()
    {
        if (singleton_->assetPrivates_.size() == 0)
        {
            LOGI("ResourceManager: all freed.");
        }
        else
        {
            LOGI("ResourceManager: existing references:");
            std::map<std::string, Referenced*>::iterator i;
            for (i = singleton_->assetPrivates_.begin(); i != singleton_->assetPrivates_.end(); i++)
            {
                std::string id = i->first;
                Referenced* referenced = i->second;
                LOGI(" -(id:%s)(refcount:%d)", id.c_str(), referenced->referenceCount());
            }
        }
        singleton_ = NULL;
    }

    Referenced* ResourceManagerPrivate::assetPrivate(const std::string& id)
    {
        if (singleton_ == NULL)
        {
            throw Exception("ResourceManager needed to load resources.");
        }

        Lock lock(singleton_->mutex_);

        Referenced* r = NULL;
        std::string key(id);
        std::map<std::string, Referenced*>::const_iterator i = singleton_->assetPrivates_.find(key);
        if (i != singleton_->assetPrivates_.end())
        {
            r = i->second;
        }
        return r;
    }

    void ResourceManagerPrivate::setAssetPrivate(const std::string& id, Referenced* assetPrivate)
    {
        Lock lock(singleton_->mutex_);

        std::pair<std::string, Referenced*> tmp0 = std::make_pair(id, assetPrivate);
        singleton_->assetPrivates_.insert(tmp0);

        sigc::connection connection = assetPrivate->destroy.connect(
            sigc::mem_fun(singleton_, &ResourceManagerPrivate::handleResourceDestroy));

        std::pair<unsigned int, sigc::connection> tmp1 = std::make_pair(assetPrivate->id(), connection);
        singleton_->assetPrivateConnections_.insert(tmp1);
    }

    void ResourceManagerPrivate::handleResourceDestroy(unsigned int id)
    {
        Lock lock(singleton_->mutex_);

        // Remove Resource from loaded resources list and signal listener list
        std::map<std::string, Referenced*>::iterator i;
        for (i = singleton_->assetPrivates_.begin(); i != singleton_->assetPrivates_.end(); i++)
        {
            Referenced* assetPrivate = i->second;
            if (assetPrivate->id() == id)
            {
                // Remove Resource's destroy signal connection
                std::map<unsigned int, sigc::connection>::iterator i2;
                i2 = singleton_->assetPrivateConnections_.find(id);
                singleton_->assetPrivateConnections_.erase(i2);

                // Resource will delete itself, look Referenced.hpp
                singleton_->assetPrivates_.erase(i);

                break;
            }
        }
    }

    FileHandle* ResourceManagerPrivate::bundledFile(const std::string& filename)
    {
        #ifdef ANDROID
            return new FileHandle(filename, androidAssetManager_);
        #elif __linux__
            return new FileHandle(singleton_->basePath_ + "/" + filename);
        #elif __APPLE__
            NSBundle *b = [NSBundle mainBundle];
            NSString *dir = [b resourcePath];
            char const* tmp = [dir UTF8String];
            return new FileHandle(std::string(tmp) + "/" + filename);
        #endif
    }

    FileHandle* ResourceManagerPrivate::dynamicFile(const std::string& filename, bool onlyRead)
    {
        FileHandle::Type::Enum openType = (onlyRead == true) ?
            FileHandle::Type::ReadOnly_CreateIfNotExisting
            :
            FileHandle::Type::WriteOnly_DestroyOldContent_CreateNewIfNotExisting;
        #ifdef ANDROID
            return new FileHandle(filename, openType, androidAssetManager_);
        #elif __linux__
            return new FileHandle(singleton_->basePath_ + "/" + filename, openType);
        #elif __APPLE__
            // TODO: Check NSArray and NSString-> do they leak here?
            NSArray* appDocumentPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
            NSString* docsDirectory = [appDocumentPaths objectAtIndex: 0];
            const char* tmp = [docsDirectory UTF8String];
            return new FileHandle(std::string(tmp) + "/" + filename, openType);
#endif
    }

    #ifdef ANDROID
        AAssetManager* ResourceManagerPrivate::androidAssetManager()
        {
            if (androidAssetManager_ == NULL)
            {
                throw Exception("androidAssetManager not set.");
            }
            return androidAssetManager_;
        }

        void ResourceManagerPrivate::setAndroidAssetManager(AAssetManager* assetManager)
        {
            androidAssetManager_ = assetManager;
        }
    #endif

    bool ResourceManagerPrivate::exists(const std::string& filename)
    {
        bool r = false;

        #ifdef ANDROID
            AAsset* asset = AAssetManager_open(assetManager_, filename, AASSET_MODE_STREAMING);
            if(asset != NULL)
            {
                r = true;
                AAsset_close(asset);
            }
        #elif __linux__
            // TODO: this is now bit heavy, file opening can be polished away
            std::string fullName;
            fullName += singleton_->basePath_;
            fullName += "/";
            fullName += std::string(filename);
            FILE *file = fopen(fullName.c_str(), "rb");
            if(file)
            {
                r = true;
                fclose(file);
            }
        #elif __APPLE__
            NSBundle *b = [NSBundle mainBundle];
            NSString *dir = [b resourcePath];
            std::string tmp = "/" + filename;
            NSString* fileAndPath = [dir stringByAppendingPathComponent:@(tmp.c_str())];
            if([[NSFileManager defaultManager] fileExistsAtPath: fileAndPath])
            {
                r = true;
            }
        #endif

        return r;
    }
}
