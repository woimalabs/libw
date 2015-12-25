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

#ifndef LIBW_RESOURCEMANAGER
#define LIBW_RESOURCEMANAGER

#include <w/base/Class.hpp>
#include <w/base/ReferencedPointer.hpp>
#include <w/base/FileHandle.hpp>
#include <string>
#ifdef ANDROID
    #include <jni.h>
    #include <sys/types.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>
#endif

namespace w
{
    class ResourceManager
    {
    public:
        COPYABLE(ResourceManager);

#ifdef ANDROID
        ResourceManager(AAssetManager* androidAssetManager);
#elif __linux__
        ResourceManager(const std::string& basePath);
#elif __APPLE__
        ResourceManager();
#endif
        virtual ~ResourceManager();
        static bool exists(const std::string& filename);

        /* fixed bundled resource files of the app */
        static ReferencedPointer<FileHandle> bundledFile(const std::string& filename);

        /* stored files of the app */
        static ReferencedPointer<FileHandle> dynamicFileForRead(const std::string& filename);
        static ReferencedPointer<FileHandle> dynamicFileForWrite(const std::string& filename);

    private:
        class ResourceManagerPrivate* private_;
    };
}

#endif
